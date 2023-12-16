#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
程序简介（非常重要）：
1,本节点作用主要有（都是回调机制）
 1)提供服务给ros2tra获取参数配置
 2)接受ros2tra参数配置，并发送配置到statis_node
 3)接受ros2tra脉冲配置，并发送配置到statis_node
 3)接受ros2tra对时间信息，并发送配置到bd_info 

'''

import time
import struct
import time,datetime
import os
import yaml
import rospy
from all_msgs_srvs.srv import *
from all_msgs_srvs.msg import settings
from all_msgs_srvs.msg import systemlog
from all_msgs_srvs.msg import bdinfomsg

CONFIGPATH="/home/hsax/roswork/src/lidar_camera/config/"#配置文件的路径
STATISCONFIG = "StatisInfo.yaml"#统计断面配置文件名称
#LANINFOCONFIG = "LanInfo.yaml"#道路信息配置文件名称
#INSTALLCONFIG = "InstallInfo.yaml"#安装和标校文件配置的名称
#NETWORKCONFIG = "NetworkInfo.yaml"#网络参数配置的文件名称

#log中对于信息的等级定义
ERROR = 10
ERROR1 = 11#严重错误
WARRING = 1
INFO = 2
DEBUG = 3

class TraSettingNode:
  def __init__(self):
    self.init_data()#初始化参数
    self.callBack_serv_ = rospy.Service('/get_param_from_file_tra', settingcallback, self.recv_setting_callback)#上位机参数查询操作,ros服务机制
    self.settingVal_pub_ = rospy.Publisher('/ros_param_setting', settings, queue_size = 10)#给统计节点设置热参数，用这个话题，将解析好的，需要的参数发送给其他节点
    self.time_pub_ = rospy.Publisher('/time_now', bdinfomsg, queue_size = 10)
    self.systemlog_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#发送对时消息
    self.setting_sub_ = rospy.Subscriber("/tra_settings", settings, self.settingSysParams);#上位机配置信息话题监听

  def __del__(self):
    print("setting.py is end!")
    self.Log_pub("setting.py is end!",INFO)

  def init_data(self):
    deviceid = rospy.get_param("/DeviceID")#只获取了一个设备id编号
    CONFIGPATH = rospy.get_param("/CONFIG_PATH")#读取配置文件路径
    self.deviceIp = deviceid
    self.logdata = []
    self.manufactname = rospy.get_param("/MANUFACTURER_NAME")#获取制造商的名称
    self.equipmentname = rospy.get_param("/EQUIPMENT_MODEL")#获取产品的名称
    self.test_items = rospy.get_param("/TEST_ITEMS")#获取检测项目
    self.test_mode = rospy.get_param("/DETECTION_MODE")#获取检测模式
    self.delay_time = rospy.get_param("/SIGNAL_DELAY_TIME")#获取检测输出延时
    self.test_max_channel = rospy.get_param("/TEST_MAX_CHANNEL")#获取最大检测通道
    pass

  #配置信息查询的服务机制，返回已经配置的信息
  def recv_setting_callback(self,req):
    try:
      ret = []
      if req.fram_id != self.deviceIp:#检查id编号
         print "failed require"
         self.Log_pub("Require failed in result of different deviceid",WARRING)
         ret =  [-0.101,-0.101,-0.101]#如果不是本设备id则返回-0.101
         return settingcallbackResponse(ret)
      obj_tag = req.type #取服务调取的类型
      if obj_tag == 7:#信号机配置参数查询
        ret =  self.return_device_param()#读取信号机参数信息
      if obj_tag == 8:#信号机历史数据查询
        ret =  self.return_device_param()#读取历史统计数据
      self.logdata = ret
      self.Log_pub("Required data",INFO)
      return settingcallbackResponse(ret)
    except Exception as e:
      print e
      self.Log_pub(str(e),ERROR1)  

#返回设备参数
  def return_device_param(self):
    print("read tra require device param data")
    self.Log_pub("read tra require device param data",INFO)
    ret = []
    ret.append(float(len(self.manufactname)))#厂商名字长度
    for s in self.manufactname:
      ret.append(float(s))#厂商的名字
    ret.append(float(len(self.equipmentname)))#型号名字长度
    for s in self.equipmentname:
      ret.append(float(s))#型号的名字
    ret.append(float(self.test_max_channel))#最大检测通道数
    ret.append(float(self.test_items))#检测项目
    ret.append(float(self.test_mode))#检测手段
    ret.append(float(self.delay_time))#检测输出延时
    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      ret.append(float(direc['StatisPeriod']))#车道总数   
      ret.append(float(direc['VehicalLong'][0])) #A
      ret.append(float(direc['VehicalLong'][1]))#B
      ret.append(float(direc['VehicalLong'][2])) #C
    return ret
   
#统计参数设置
#协议

  def statis_param_setting(self,data):
    print("tra set statis param in trasetting_node")
    self.Log_pub("tra set statis param in trasetting_node",INFO)
    setting = settings()
    setting.frame_id = self.deviceIp
    setting.type = 7 #信号机配置参数查询
    setting.which_param = 7000 #信号机配置参数热参数设置

    param_info_list = b''
    for val in data:
      param_info_list += struct.pack('<B',val)#将float类型转换成byte
    lan_info_turple = struct.unpack('<HBBB',param_info_list[0:5])#将4byte转换成int  
    direc = {}
    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      direc['VehicalLong'] = [lan_info_turple[1],lan_info_turple[2],lan_info_turple[3]] #取车长配置
      direc['StatisPeriod'] = lan_info_turple[0] #取周期
    with open(CONFIGPATH + STATISCONFIG, 'w') as file_to_read:#写文件
      yaml.dump(direc,file_to_read)
      self.logdata = data
      self.Log_pub("Write tra param info to savefile",INFO)
    
    #添加数据,发送数据到ros,具体协议不明白见statis.cpp解析
    setting.setdatas.append(direc['StatisPeriod'])
    setting.setdatas.append(direc['VehicalLong'][0])
    setting.setdatas.append(direc['VehicalLong'][1])
    setting.setdatas.append(direc['VehicalLong'][2])
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish tra param info to ros",INFO)

#脉冲模式的设置
  def statis_pulse_setting(self,data):
    print("tra set pulse mode in trasetting_node")
    self.Log_pub("tra set pulse mode in trasetting_node",INFO)
    setting = settings()
    setting.frame_id = self.deviceIp
    setting.type = 7 #信号机脉冲参数
    setting.which_param = 7001 #信号机脉冲设置

    pulse_info_list = []
    for val in data:
      pulse_info_list.append(struct.pack('<B',val))#将float类型转换成byte
    direc = {}
    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      direc['PulseMode'] = pulse_info_list
    with open(CONFIGPATH + STATISCONFIG, 'w') as file_to_read:#写文件
      yaml.dump(direc,file_to_read)
      self.logdata = pulse_info_list
      self.Log_pub("Write tra pulse info to savefile",INFO)
    
    #添加数据,发送数据到ros,具体协议不明白见statis.cpp解析
    setting.setdatas = pulse_info_list
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish tra pulse info to ros",INFO)
           
#对时操作设置
  def statis_period_setting(self,data):
    print("Tra set time info in setting_node")
    self.Log_pub("Tra set time info to savefile",INFO)
    time_now = bdinfomsg()
    time_now.device_id = self.deviceIp#这一个需要使用之前的id

    time_info_list = b''
    for val in data:
      time_info_list += struct.pack('<B',val)#将float类型转换成byte
    time_info_turple = struct.unpack('<I',time_info_list)#将4byte转换成int
    time_list = time.localtime(time_info_turple[0])#将秒数变成日期

    time_now.timenow = str(time_list[2])+'/'+str(time_list[1])+'/'+str(time_list[0])+'-'+str(time_list[3])+':'+str(time_list[4])+':'+str(time_list[5])#写成timenow需要的str格式
    #热参数设置
    time_now.isNew = 1
    time_now.node_name = "tra_setting_node"
    self.time_pub_.publish(time_now)
    self.logdata = list(time_list)
    self.Log_pub("Publish tra time info to ros",INFO)
       

#上位机设置的参数分流解析
#'''
#	1,解析数据头,把原始的设置信息，分流到各个对应的设置函数中进行通讯协议的解析,
#	2,ros_setting中的消息是float格式的，但是对应的是通讯协议中的byte，例如,0x64 -> 100 ->100.0
#       3,ros_setting中的type和which_param是协议信息进行区分的标志，详情见ros2up中tcpcontrol.h里的SettingType，和ROSParamType
#'''
  def deal_unpack_setting(self,Type,sort,data):
    try:
      print("enter setting")
      self.Log_pub("Enter setting",INFO)
      if Type == 7:#信号机配置参数
        if sort == 700:#信号机配置一般参数
          self.statis_param_setting(data)
        if sort == 701:#信号机配置脉冲参数
          self.statis_pulse_setting(data)
        if sort == 702:#信号机对时操作
          self.statis_period_setting(data)
    except Exception as e:
      print e
      self.Log_pub(str(e),ERROR)  

  #上位机设置信息的回调函数
  def settingSysParams(self,msg):
    if msg.frame_id != self.deviceIp:
       return
    try:
      self.deal_unpack_setting(msg.type,msg.which_param,msg.setdatas)
    except Exception as e:
      self.Log_pub(str(e),ERROR1)      

  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceIp
    systemlog_.node_id = "tra_setting_node"
    systemlog_.type = logtype;
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.systemlog_pub_.publish(systemlog_)
    self.logdata = []
    
if __name__ == '__main__':
  try:
    rospy.init_node('tra_setting_node', anonymous = True)
    traSettingNode = TraSettingNode()
    rospy.spin()
    print("end")
  except Exception as e:
    print e

























