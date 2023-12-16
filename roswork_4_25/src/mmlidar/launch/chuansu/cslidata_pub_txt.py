#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import json
import socket
import time,datetime
import os
import threading
import random
from all_msgs_srvs.msg import vehicalmsgs
from all_msgs_srvs.msg import settings
import math
import struct

from evdev import InputDevice
from select import select

HOST= "192.168.203.99"
PORT=8898
BUFFER=9600

DATA_PATH = '../../datahistory/org_data.txt'

def get_time():
    now = time.time()
    local_time = time.localtime(now)
    return time.strftime('%Y-%m-%d %H:%M:%S',local_time)
    #return time.strftime('%Y-%m-%d',local_time)

class Tcp_link:
    def __init__(self):
        rospy.init_node('mmlidar_pub', anonymous = True)
        self.pub = rospy.Publisher('mmlidar_veh', vehicalmsgs, queue_size = 1)
        self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)
        self.pause = -1
        self.rate = rospy.Rate(15)#rate
        self.my_thread = threading.Thread(target = self.klistener) #add a thread
        self.my_thread.start()
       
        self.pub_veh = vehicalmsgs()#init send data

        HOST= rospy.get_param("/LidarNetIp")
        PORT=rospy.get_param("/LidarNetPort")
        BUFFER=rospy.get_param("/LidarNetBuad")
        SAVEDATA=rospy.get_param("/LidarDataSave")
        InstallParamlist = rospy.get_param("/MainInstallParam")
        LanCount = rospy.get_param("/LanCount")
        deviceid = rospy.get_param("/DeviceID")


        print("get socket param successful! host:%s , port:%d, baud:%d" % (HOST,PORT,BUFFER))
        self.host = HOST
        self.port = PORT
        self.baud = BUFFER
        self.datasave = SAVEDATA
        self.device_id = deviceid
    
        
        self.theta_ = InstallParamlist[3]
        self.lanInfo_ = {}
        print("车道总数:")
        print(LanCount)
        for i in range(LanCount):
          paramname_str = "/LanParam/" + str(i+1) + "/coordinate"
          paramname_width_str = "/LanParam/" + str(i+1) + "/width"
          paramdir= rospy.get_param(paramname_str)
          print("正在加载的车道号码:")  
          print(str(i+1))
          self.lanInfo_[str(i+1)] = [paramdir[0],paramdir[0] + rospy.get_param(paramname_width_str)]
        self.filePath =DATA_PATH
        print("加载的各个车道的情况:")
        print(self.lanInfo_)
        self.receive_data()

    def getsetParamCb(self,data):
      if (data.frame_id == self.device_id) and (data.type == 0) and (data.which_param == 1001):#道路坐标设置
        index = 0
        mydata = data.setdatas
        self.lanInfo_ = {}
        for i in range(int(mydata[0])):
          self.lanInfo_[str(i+1)] = [mydata[index + 3],mydata[index + 3] + mydata[index + 2]]
          index = index + 4
        print("道路修改后的情况:")
        print self.lanInfo_
      elif (data.frame_id == self.device_id) and (data.type == 4) and (data.which_param == 5000):#雷达安装参数热设置,偏航角度的设置
        self.theta_ = data.setdatas[4]
        print("偏航角修改后的值:")
        print(self.theta_)
      elif (data.frame_id == self.device_id) and (data.type == 3) and (data.which_param == 3001):#雷达ID
        self.device_id = int(data.setdatas[0])
        print "ID号码修改后的值:"
        print self.device_id



    def __del__(self):
        #self.soc.close()
        print('析构函数')


    def link_tcp(self):#don't be needed in this program
        try:
            self.soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.soc.connect((self.host,self.port))
            return True
            print("connect successful!")
        except:
            print("connect failure!")
            return False  

    def klistener(self):
        while  not rospy.is_shutdown():
            dev = InputDevice('/dev/input/event1')
            select([dev], [], [])                                          
            for event in dev.read():
                if event.value == 1 and event.code == 57:
                    self.pause = -self.pause
                    print(self.pause)
                    print self.pub_veh.stamp.secs
                if event.code == 109:
                    print("listener is end") 
                    threading.Thread._Thread__stop(self.my_thread)
        threading.Thread._Thread__stop(self.my_thread)
   

    def receive_data(self):
        try:
          while not rospy.is_shutdown(): #loop open the txt file when reading end
            i=0   
            with open(self.filePath, 'r') as file_to_read:
              while not rospy.is_shutdown(): 
                if self.pause == 1:#when pause
                    #print "jixu"
                    continue
                lines = file_to_read.readline() # 整行读取数据
                if not lines:# read none line
                    #print "break"
                    break
                    pass
                if lines.find("[") != -1:
                   # print "normal"
                    i = lines.find("]")
                    self.buf = lines[1:i]#find the ']' index as end
                    self.rate.sleep()
                    self.deal_data()
                elif lines.find("TimeNow") != -1:
                    self.pub_veh.stamp.secs = int(lines[-13:-3])

        except Exception as e:
            print("recv data failure!")
            print(e)
    def deal_data(self):
        try:
    	    self.data = self.buf.split(",")#split line data by ','
            self.pub_data()

        except Exception as e:
            print("deal data failure")
            print(e)
            return 

    def getLanNum(self,x,y):
      ret = 0
      #print(self.theta_)
      #theta_rad = self.theta_ * math.pi / 180.0
      #actual_x = x * math.cos(theta_rad) - y * math.sin(theta_rad)
      #actual_y = x * math.sin(theta_rad) + y * math.cos(theta_rad)
      actual_x = x
      actual_y = y
      for key, value in self.lanInfo_.items():
        if actual_x >= value[0] and actual_x < value[1]:
          ret = float(key)
          break
      #print self.lanInfo_
      #print("X:%s,lan:%s" % (actual_x,ret))
      return ret,actual_x,actual_y

    def pub_data(self):
        #for val in self.data:
        self.pub_veh.frame_id = self.device_id
        i = 0
        data_len = len(self.data)
        if data_len > 1530:
          return
        while i < data_len:
          self.pub_veh.vehicaldatas.append(float(self.data[i]))#append a data into pub_data
          self.pub_veh.vehicaldatas.append(float(self.data[i+1]))#append a data into pub_data
          lanNum,x,y = self.getLanNum(float(self.data[i+4]),float(self.data[i+5]))
          #print("x0:%s,y0:%s" % (self.data[i+4],self.data[i+5]))
          #print("x:  %s,y:  %s,lan:%s" % (x,y,lanNum))
          self.pub_veh.vehicaldatas.append(lanNum)#append a data into pub_data
          self.pub_veh.vehicaldatas.append(float(self.data[i+3]))#append a data into pub_data
          self.pub_veh.vehicaldatas.append(x)#append a data into pub_data
          self.pub_veh.vehicaldatas.append(y)#append a data into pub_data
          i = i + 6
        self.pub.publish(self.pub_veh)
        self.pub_veh.vehicaldatas = [] #empty the pub_data
        self.data = [] #empty the line_data
        

if __name__ == '__main__':
    try:
        tcp_link = Tcp_link()
    except Exception as e:
        print(e)




























