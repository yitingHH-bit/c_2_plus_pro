#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''程序概要：（非常重要）
#1,两个线程工作，一个是正常的系统硬件检测线程（主线程），一个是检测信息上报线程（self.pub_data）
#2,主线程是每一秒读取系统资源占用，上报线程是每一秒检查是否有异常，如果有异常及时上报，如果没有异常，60秒后上报
#3,主线程的报警值由ros配置文件设置，有一个指标超标，异常标志位为true，上报线程检测到异常标志为true时，上报
#4,程序同时接受log的error信息，如果是进程错误，相机或者雷达进程错误，异常标志位也为true，同样上报
#
####注意：扩展内存的目录名称'''

import rospy
import os
import sys
import psutil
import time
import threading
import thread
from all_msgs_srvs.msg import systemState
from all_msgs_srvs.msg import systemlog
from all_msgs_srvs.msg import settings

ERROR = 10 #进程错误，上报
ERROR1 = 11#一般log错误
WARRING = 1#警告
INFO = 2#普通信息
DEBUG = 3#调试log

INTERVAL = 30 #系统正常上报的时间间隔
GPULOADFILE = "/sys/devices/gpu.0/load"#GPU值的硬件文件
HOME_FILE = "/home/hsax"#本机的储存地址
SDA1_FILE = "/media/hsax/D8BF-4F7F"#扩展储存的地址

class SystemMon:
  def __init__(self):
    #rospy.init_node('system_monitor_node', anonymous = True)
    self.log_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#定义一个系统log发送端口
    try:
      self.init_data()#初始化数据
      self.error_sub_ = rospy.Subscriber("/systemLog_info", systemlog, self.getLogCb)#接收系统log消息，主要是监控整个软件进程的状态，如果出现致命错误报警
      self.state_pub_ = rospy.Publisher('/systemState_info', systemState, queue_size = 10)#发送系统状态信息
      self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)#接受系统热参数修改
      thread.start_new_thread(self.pub_data,())#启动新线程，主要完成状态信息发布
      self.Log_pub("Init finished!",INFO)#发布log
    except Exception as e:
      self.Log_pub("Init Error!",ERROR)#发布log
    while not rospy.is_shutdown():#主循环
      #time.sleep(1)
      self.rate_.sleep()
      self.lock.acquire()#上锁
      try:
        #内核温度
        self.sysState_.temperature = psutil.sensors_temperatures()['thermal-fan-est'][0].current
        #内存占用
        self.sysState_.occ_RAM = psutil.virtual_memory().percent
        #CPU占有率
        self.sysState_.occ_CPU = psutil.cpu_percent(0)
        #GPU占用率
        with open(GPULOADFILE, 'r') as gpuFile:
          data_str = gpuFile.read()
          self.sysState_.occ_GPU = int(data_str[:-1])

        #硬盘储存占用,单位GB
        #st_sda1 = os.statvfs(SDA1_FILE)
        #st_home = os.statvfs(HOME_FILE)
        #self.sysState_.occ_memory_sda1 = st_sda1.f_bavail * st_sda1.f_frsize/1024/1024/1024
        #self.sysState_.occ_memory_home = st_home.f_bavail * st_home.f_frsize/1024/1024/1024

        self.sysState_.occ_memory_sda1 = 100
        self.sysState_.occ_memory_home = 5

        if((self.sysState_.temperature > self.alarm_temperature) or (self.sysState_.occ_RAM > self.alarm_occ_RAM) or (self.sysState_.occ_CPU > self.alarm_occ_CPU) or (self.sysState_.occ_GPU > self.alarm_occ_GPU) or (self.sysState_.occ_memory_sda1 < self.alarm_occ_memory_sda1) or (self.sysState_.occ_memory_home < self.alarm_occ_memory_home)):#任何一个超过报警值，便发送error_log
          self.logdata = [self.sysState_.temperature,self.sysState_.occ_RAM,self.sysState_.occ_CPU,self.sysState_.occ_GPU,self.sysState_.occ_memory_sda1,self.sysState_.occ_memory_home,self.sysState_.precess_state,self.sysState_.camera_state,self.sysState_.mmlidar_state]
#数组顺序：温度/内存/CPU/GPU/扩展储存/机身储存/软件系统状态/相机状态/雷达状态
          self.Log_pub("Monitor Error",ERROR1)#发布错误log
          self.error = True#系统监控异常标志
      except Exception as e:
        self.Log_pub("Monitor Error!",ERROR1)
      self.lock.release()#解锁
  
  def init_data(self):
    self.deviceID = rospy.get_param("/DeviceID")#获取设备id
    self.alarm_temperature = rospy.get_param("/Alarm_value/temperature")#获取检测的温度报警值
    self.alarm_occ_RAM = rospy.get_param("/Alarm_value/occ_RAM")#获取运行内存报警值
    self.alarm_occ_CPU = rospy.get_param("/Alarm_value/occ_CPU")#获取CPU使用情况报警值
    self.alarm_occ_GPU = rospy.get_param("/Alarm_value/occ_GPU")#获取GPU使用情况报警值
    self.alarm_occ_memory_sda1 = rospy.get_param("/Alarm_value/occ_memory_sda1")#获取扩展储存报警值
    self.alarm_occ_memory_home = rospy.get_param("/Alarm_value/occ_memory_home")#获取机身储存报警值
    self.logdata = []#log数据初始化
    self.error = False#报警标志初始化
    self.lock = threading.RLock()#线程锁
    self.sysState_ = systemState()#系统状态消息初始化
    self.sysState_.device_id = self.deviceID#填写设备id
    self.sysState_.occ_CPU = 0#init
    self.sysState_.occ_GPU = 0#init
    self.sysState_.temperature =0 #init
    self.sysState_.occ_RAM = 0#init
    self.sysState_.occ_memory_sda1 = 0#init
    self.sysState_.occ_memory_home = 0#init
    self.init_monitor()#初始化系统软件状态
    self.rate_ = rospy.Rate(1)#rospy的延时操作，1Hz

  def init_monitor(self):
    self.sysState_.precess_state = 0#整个软件进程状态
    self.sysState_.camera_state = 0#相机软件进程状态
    self.sysState_.mmlidar_state = 0#毫米波雷达进程状态


  def getLogCb(self,errdata):
    if errdata.device_id != self.deviceID:#本机错误
      return 
    if errdata.type == ERROR:#系统软件进程错误
      self.error = True #错误标志
      self.sysState_.precess_state = 1 #进程错误标志
      if errdata.node_id == "imgup_node":
        self.sysState_.camera_state = 1 #相机进程错误标志
      elif errdata.node_id == "lidar_node": 
        self.sysState_.mmlidar_state = 1 #雷达进程错误标志
   
  def getsetParamCb(self,data):#系统参数热修改
    if (data.frame_id == self.deviceID) and (data.type == 3) and (data.which_param == 3001):#设备id修改
      try:
        self.deviceID = int(data.setdatas[0])
        self.Log_pub("device id is change into " + str(self.deviceID), 2)
      except Exception as e:
        self.Log_pub("Device id set failed!",ERROR1)

  #发送系统状态数据，发布条件为每分钟一次，每分钟检查60次，如果有错误标志随时发送
  def pub_data(self):
    numbcount = 0
    while True:
      numbcount = numbcount + 1
      if (numbcount % INTERVAL == 0) or self.error:
        numbcount = 1
        try:
          self.lock.acquire()#线程锁
          self.state_pub_.publish(self.sysState_)#发布系统信息
          self.error = False#错误标志复位
          self.init_monitor()#恢复状态
          self.lock.release()#解锁
        except Exception as e:
          self.Log_pub("Send state Error!",ERROR1)
      #time.sleep(1)
      self.rate_.sleep()
    pass

  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceID
    systemlog_.node_id = "systemState_node"
    systemlog_.type = logtype;
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.log_pub_.publish(systemlog_)

    self.logdata = []

  def __del__(self):
    print("Monitor_node end!")
    self.Log_pub("Monitor_node end!",ERROR)
 

#if __name__ == '__main__' :
  #try:
    #systemMon = SystemMon()
  #except Exception as e:
    #print e




