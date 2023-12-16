#!/usr/bin/env python
#coding=utf-8

import rospy
import serial
from all_msgs_srvs.msg import bdinfomsg
from all_msgs_srvs.msg import settings
from all_msgs_srvs.msg import systemlog
import time
import thread
import os
from threading import _Timer

INTERVAL = 6 #半天更新一次时间
ERROR = 10
ERROR1 = 11
WARRING = 1
INFO = 2
DEBUG = 3

class MyTimer(_Timer):
  def run(self):
    while not self.finished.is_set():
      self.function(*self.args, **self.kwargs)
      self.finished.wait(self.interval)

class TimeManager:
  def __init__(self):
    self.node_name = 'timemanager_node'
    rospy.init_node(self.node_name, anonymous = True)
    self.systemlog_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#发送系统日志
    self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)
    self.time_sub_ = rospy.Subscriber('/time_now', bdinfomsg, self.getTimeCb)
    self.position_pub_ = rospy.Publisher('bdinfo_pos', bdinfomsg, queue_size = 10)
    self.init_data()
    thread.start_new_thread(self.set_time,())

    self.mytimer = MyTimer(INTERVAL * 10, self.Timer_fun)
    self.mytimer.start()
  
    trytime = 0
    while not rospy.is_shutdown():
       trytime = trytime + 1
       self.port_dev = self.get_port()
       if self.isExists: 
         self.port=serial.Serial(self.port_dev,self.buand)
         if not self.port.isOpen():
            self.port.open()
         print(self.port_dev + " have opened!")
         self.Log_pub("Connecte with BDTimer successful!",INFO)
         break
       else:
         if trytime % 12 == 0:
           self.Log_pub("try to connecte with BDTimer",WARRING)
         time.sleep(10)
    self.Timer_fun() #开机更新一次
    while not rospy.is_shutdown():
      try:
        #self.unpackmsg(self.port.readline())
        pass
      except Exception as e:
        self.Log_pub("Recv data from BDTimer failed!",WARRING)
    
    self.mytimer.cancel()
    
    
  def init_data(self):
    #self.port = self.get_port()
    self.logdata = []
    self.time_now = False  #收到新时间,立即更新
    self.time_in = False  #6小时如果没有收到时间消息,更新
    self.deviceID = rospy.get_param("/DeviceID")
    self.buand = 9600
    self.msg_time = '00:00'
    self.msg_lon = 0
    self.msg_alt = 0
    self.datetime = '00/00/2000'
    self.datetime_out = '00/00/2000'
    self.msg_time_out = '00:00'
    self.Log_pub("init finished!",INFO)

  def get_port(self):
    if os.path.exists('/dev/ttyUSB0') == True:
      self.isExists = True
      return '/dev/ttyUSB0'
    elif os.path.exists('/dev/ttyUSB1') == True: 
      self.isExists = True
      return '/dev/ttyUSB1'
    elif os.path.exists('/dev/ttyUSB2') == True: 
      self.isExists = True
      return '/dev/ttyUSB2'
    elif os.path.exists('/dev/ttyUSB3') == True: 
      self.isExists = True
      return '/dev/ttyUSB3'
    elif os.path.exists('/dev/ttyUSB4') == True: 
      self.isExists = True
      return '/dev/ttyUSB4'
    else:
      self.isExists = False
      return ''

  #运行参数热修改
  def getsetParamCb(self,data):
    if (data.frame_id == self.deviceID) and (data.type == 3) and (data.which_param == 3001):#换ID
      self.deviceID = int(data.setdatas[0])
      #self.Log_pub("device id is change into " + str(self.deviceID), 2)

  #时间回调函数
  def getTimeCb(self,time_now):#接受时间
      if (time_now.device_id == self.deviceID):#
        timeget = time_now.timenow
        self.msg_time_out = timeget.split("-")[1]
        self.datetime_out = timeget.split("-")[0]
        self.time_now = True

  #线程设置时间
  def set_time(self):
    history_time = '00:00'
    self.Log_pub("Enter set_time thread",INFO)
    print("进入设置时间线程")
    while not rospy.is_shutdown():
      try:
        if self.time_in: #定时更新,采用  
          print("msg_time:%s" % self.msg_time)
          self.time_in = False
          if abs(int(history_time.split(":")[0]) - int(self.msg_time.split(":")[0])) > (INTERVAL-1):
            os.system('echo nvidia | sudo -S date -s '+ self.datetime)
            os.system('echo nvidia | sudo -S date -s ' + self.msg_time)
            history_time = self.msg_time
            self.pose_pub()
            print("从北斗获取更新时间成功")
            self.Log_pub("Set time from BDTIMER:" + self.datetime + '-' +self.datetime,INFO)
          else:
            print("从北斗获取更新时间失败")
            self.Log_pub("Set time from BDTIMER failed!",WARRING)
        elif self.time_now:
          os.system('echo nvidia | sudo -S date -s '+ self.datetime_out)
          os.system('echo nvidia | sudo -S date -s ' + self.msg_time_out)
          history_time = self.msg_time
          self.time_now = False
          self.mytimer.cancel()
          self.mytimer = MyTimer(INTERVAL, self.Timer_fun)
          self.mytimer.start()#定时器重新启动
          print("从外界获取更新时间")
          self.Log_pub("Set time from out device:" + self.datetime_out + '-' +self.msg_time_out,INFO)
      except Exception as e:
        self.Log_pub(str(e) + " - Set system time failed!" ,ERROR)
        
  def Timer_fun(self):
    try:
      self.unpackmsg(self.port.readline())
    except Exception as e:
      self.Log_pub(str(e) + "Recv data from BDTIME failed!", ERROR)
    self.time_in = True
    print("6小时更新时间到!")
    self.Log_pub("6小时更新时间到!", INFO)

  def unpackmsg(self,msg_pack):
        if msg_pack.find("GNRMC") != -1:
            arr=msg_pack.split(',')
            if arr[1] != '':
                self.msg_time = str(int(arr[1][0:2])+8) + ":" + arr[1][2:4] + ":" + arr[1][4:6]
            if arr[5] != '':
                self.msg_lon = float(arr[5])
                self.msg_alt = float(arr[3])
            if arr[9] != '':
                self.datetime =  arr[9][2:4] +"/"+ arr[9][0:2] + "/20" + arr[9][4:6]
            return
        elif msg_pack.find("GNGGA") != -1:
            arr=msg_pack.split(',')
            if arr[1] != '':
                self.msg_time = str(int(arr[1][0:2])+8) + ":" + arr[1][2:4] + ":" + arr[1][4:6]
            if arr[4] != '':
                self.msg_lon = float(arr[4])
                self.msg_alt = float(arr[2])
            return
        elif msg_pack.find("GNGLL") != -1:
            arr=msg_pack.split(',')
            if arr[5] != '':
                self.msg_time = str(int(arr[5][0:2])+8) + ":" + arr[5][2:4] + ":" + arr[5][4:6]
            if arr[3] != '':
                self.msg_lon = float(arr[3])
                self.msg_alt = float(arr[1])
            return
  def pose_pub(self):
    poseinfo = bdinfomsg()
    poseinfo.node_name = self.node_name
    poseinfo.device_id = self.deviceID
    poseinfo.isNew = 0
    poseinfo.timenow = self.datetime + '-' + self.msg_time
    poseinfo.lon = self.msg_lon
    poseinfo.alt = self.msg_alt
    self.position_pub_.publish(poseinfo)

  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceID
    systemlog_.node_id = "Timer_node"
    systemlog_.type = logtype;
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.systemlog_pub_.publish(systemlog_)
    self.logdata = []


if __name__ == '__main__':
    try:
      timeManager = TimeManager()
    except rospy.ROSInterruptException:
        pass


