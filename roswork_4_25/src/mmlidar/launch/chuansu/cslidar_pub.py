#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import json
import socket
import time,datetime
import os
import threading
import random
from mmlidar.msg import vehicalmsgs
from ros2up.msg import settings
from lidar_camera.msg import systemlog
import struct
import math

HOST= "192.168.61.21"
PORT=5000
BUFFER=9600
SAVEDATA=True

ERROR = 10
ERROR1 = 11
WARRING = 1
INFO = 2
DEBUG = 3

SAVE_PATH = u'/home/prototype/gui_ws/src/lidar_camera/mmlidar/launch/datasave/'

def get_time():
    now = time.time()
    local_time = time.localtime(now)
    return time.strftime('%Y-%m-%d %H:%M:%S',local_time)
    #return time.strftime('%Y-%m-%d',local_time)

class Tcp_link:
    def __init__(self):
        rospy.init_node('mmlidar_pub', anonymous = True)
        self.pub = rospy.Publisher('mmlidar_veh', vehicalmsgs, queue_size = 10)
        self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)
        self.systemlog_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#发送系统日志
        self.lock = threading.Lock()
        self.send_count_ = 0;

        self.logdata = [] #log data

        self.soc = None
        self.rate = rospy.Rate(10)
        self.vehdata = []
        self.pub_veh = vehicalmsgs()

        try:
          HOST= rospy.get_param("/LidarNetIp")
          PORT=rospy.get_param("/LidarNetPort")
          BUFFER=rospy.get_param("/LidarNetBuad")
          SAVEDATA=rospy.get_param("/LidarDataSave")
          InstallParamlist = rospy.get_param("/MainInstallParam")
          LanCount = rospy.get_param("/LanCount")
          deviceid = rospy.get_param("/DeviceID")
        except:
          self.Log_pub("Get ROS params failed!",ERROR)
          


        print("get lidar param successful! host:%s , port:%d, baud:%d" % (HOST,PORT,BUFFER))
        self.host = HOST
        self.port = PORT
        self.baud = BUFFER
        self.datasave = SAVEDATA
        self.device_id = deviceid
        self.Log_pub("Get lidar params,host:" + str(HOST) + ",port:" + str(PORT),INFO)
    
        
        self.theta_ = InstallParamlist[3]
        self.height = InstallParamlist[2]
        self.deltax = InstallParamlist[0]
        self.lanInfo_ = {}
        for i in range(LanCount):
          paramname_str = "/LanParam/" + str(i+1) + "/coordinate"
          paramname_width_str = "/LanParam/" + str(i+1) + "/width"
          paramdir= rospy.get_param(paramname_str)
          #print(str(i+1))
          self.lanInfo_[str(i+1)] = [paramdir[0],paramdir[0] + rospy.get_param(paramname_width_str)]
          #print(self.lanInfo_)
        print("lan info:")
        print(self.lanInfo_)
        if self.datasave:
            self.file_init()
        link_time = 0
        while not self.link_tcp() and not rospy.is_shutdown():
            link_time = link_time + 1
            if link_time > 12:
              link_time = 0
              self.Log_pub("Cannot connecte our lidar!",ERROR)
            print("waiting connection with lidar,wait 10s again!")
            self.Log_pub("Waiting connection with lidar,wait 10s again!",WARRING)
            time.sleep(10)
        self.receive_data()

    def file_init(self):
        now = time.time()
        local_time = time.localtime(now)
        #date_filetime =  time.strftime('%Y-%m-%d %H:%M:%S',local_time)
        date_doctime =  time.strftime('%Y-%m-%d',local_time)
        docname= SAVE_PATH + date_doctime
        org_txtname = u'/org_data.txt'

        if not os.path.exists(docname):
          os.mkdir(docname)
        self.org_file = open(docname+org_txtname,"a+")
        print("info: org_file has been opened!")
        #self.Log_pub("Lidar data save file --org_file has been opened!",INFO)

    def __del__(self):
        self.soc.close()
        self.org_file.close()
        print('析构')


    def link_tcp(self):
        try:
            self.soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.soc.settimeout(10)
            self.soc.connect((self.host,self.port))
            #print("connect successful!")
            self.Log_pub("Lidar connect successful!",INFO)
            return True
        except:
            #print("connect failure!")
            self.Log_pub("Lidar connect failure!",ERROR)
            return False  

    def saveData(self,s):
        self.org_file.write("TimeNow:\t"+str((time.time()* 1000 - 1589000000000) // 10  ) +'\n')
        self.org_file.write("data:\n"+s+'\n\n')
        self.org_file.flush()

    def receive_data(self):
        try:
            self.sendcmd('INIT')
            buf=self.soc.recv(BUFFER)
            self.deal_data(buf)

            self.sendParams(0,int(self.height*100 + 2048))
            buf=self.soc.recv(BUFFER)
            self.deal_data(buf)

            self.sendParams(4,int(self.theta_*100 + 2048))
            buf=self.soc.recv(BUFFER)
            self.deal_data(buf)

            self.sendParams(8,int(self.deltax*100 + 2048))
            buf=self.soc.recv(BUFFER)
            self.deal_data(buf)

            self.sendParams(200,1)
            buf=self.soc.recv(BUFFER)
            self.deal_data(buf)

            while not rospy.is_shutdown(): 
                try:
                    self.lock.acquire()
    	            buf=self.soc.recv(BUFFER)
                    self.deal_data(buf)
                    self.lock.release()
                except Exception as e:
                    print(e)
                    if str(e) == 'timed out':
                        self.buf = None
                    self.Log_pub("Recv data failure from lidar!",ERROR1)
                #self.deal_data()
        except Exception as e:
            print("recv data failure!")
            self.Log_pub("Recv data failure from lidar!",ERROR)
            #print(e)
    def getsetParamCb(self,data):
      print("enter lidar_driv ,%s" % self.device_id)
      if (data.frame_id == self.device_id) and (data.type == 0) and (data.which_param == 1001):#道路坐标设置
        index = 0
        mydata = data.setdatas
        self.lanInfo_ = {}
        for i in range(int(mydata[0])):
          self.lanInfo_[str(i+1)] = [mydata[index + 1],mydata[index + 1] + 3.65]
          index = index + 2
      elif (data.frame_id == self.device_id) and (data.type == 4) and (data.which_param == 5000):#雷达安装参数热设置,偏航角度的设置
        self.theta_ = data.setdatas[4]
        self.height = data.setdatas[3]
        self.deltax = data.setdatas[1]
        self.lock.acquire()
        #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        #print(self.theta_)
        self.sendParams(200,3)
        buf=self.soc.recv(BUFFER)
        self.deal_data(buf)
        #print("0!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

        self.sendParams(0,int(self.height * 100 + 2048))
        buf=self.soc.recv(BUFFER)
        self.deal_data(buf)
        #print("1!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

        self.sendParams(4,int(self.theta_ * 100 + 2048))
        buf=self.soc.recv(BUFFER)
        self.deal_data(buf)
        #print("2!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

        self.sendParams(8,int(self.deltax*100 + 2048))
        buf=self.soc.recv(BUFFER)
        self.deal_data(buf)
        #print("3!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")

        self.sendParams(200,1)
        buf=self.soc.recv(BUFFER)
        self.deal_data(buf)
        #print("4!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!")
        self.lock.release()

        #print("偏航角修改后的值:")
        #print(self.theta_)
        self.logdata = [self.theta_]
        self.Log_pub("Change the yaw!",INFO)
      elif (data.frame_id == self.device_id) and (data.type == 3) and (data.which_param == 3001):#雷达ID
        #print "ID号码修改后的值:"
        self.device_id = int(data.setdatas[0])
        self.logdata = [self.device_id]
        self.Log_pub("Change the deviceId!",INFO)


    def sendSetting(self,cmd):
      try:
        stri = ''
        for str1 in cmd:
          stri += str1.encode()
        stri += b'\x00\x00\x00\x04'
        stri += b'\x00\xc8'
        stri += b'\x00\x01'
        self.soc.send(stri)
        print("send mode cmd!")
        self.Log_pub("Send mode_cmd to lidar!",INFO)
      except Exception as e:
        print e
        self.Log_pub(str(e),ERROR1)

    def sendParams(self,postion,data):
      try:
        stri = ''
        for str1 in 'CAPM':
          stri += str1.encode()
        stri += b'\x00\x00\x00\x04'
        stri += struct.pack('>H',postion)
        stri += struct.pack('>H',data)
        self.soc.send(stri)
        log_str = ''
        if postion==200:
          log_str += "mode set"
        elif postion==0:
          log_str += "height param set"
        elif postion==4:
          log_str += "yaw param set"
        elif postion==8:
          log_str += "X param set"
        else:
          log_str += "unknow set"
        print("send " + log_str + " cmd!")
        self.logdata.append(data)
        self.Log_pub("Send " + log_str + " cmd! to lidar!",INFO)
      except Exception as e:
        print e
        self.Log_pub(str(e),ERROR1)

    def sendcmd(self, cmd):
      try:
        stri = ''
        for str1 in cmd:
          stri += str1.encode()
        self.soc.send(stri)
        print("send conn cmd!")
        self.Log_pub("Send CONN_cmd to lidar!",INFO)
      except Exception as e:
        print e
        self.Log_pub(str(e),ERROR1)

    def receive_data_by_len(self,count):
      data=''
      try:
        data = self.soc.recv(count)
        return data
      except Exception as e:
        print e
        return None
    def getLanNum(self,x,y):
      ret = 0
      #theta_rad = self.theta_ * math.pi / 180.0
      #actual_x = x * math.cos(theta_rad) - y * math.sin(theta_rad)
      #actual_y = x * math.sin(theta_rad) + y * math.cos(theta_rad)
      actual_x = x 
      actual_y = y
      for key, value in self.lanInfo_.items():
        if actual_x >= value[0] and actual_x < value[1]:
          ret = float(key)
          break
      return ret,actual_x,actual_y

    def deal_data(self,buf):
      #print("enter deal_data,buff_len:%s" % len(buf))
      head_ascii = ''
      try:
        head_ascii = buf[0:4].encode() #有可能不是字母
      except:
        return
      #print("frame head :%s" % head_ascii)
      if head_ascii == 'DONE':
        print("receive DONE!")
        self.Log_pub("Recv DONE",INFO)
      elif head_ascii == 'TRAJ':
        buff_list = struct.unpack('>i',buf[4:8])
        #print("buf_len:%s,%s" % (len(buf),buff_list[0]))
        if (len(buf) - 8 - buff_list[0])<0:
          rec_slen = buff_list[0] + 8 - len(buf)
          rec_alen = 0
          #print("begin:%s,%s" % (rec_slen,rec_alen))
          while rec_alen != rec_slen:
            buf_add=self.soc.recv(rec_slen - rec_alen)
            rec_alen = rec_alen + len(buf_add)
            buf = buf + buf_add
           # print("start:%s,%s" % (rec_slen,rec_alen))
        carcount = (buff_list[0] - 74) / 80.0
        
        #if carcount > 0:
           #print("target num: %s" % carcount)
        carcount = int(carcount)
        if carcount == 0 or carcount > 255:
           return
        bufcrc16 = buf[8:8 + 72 + 80 * carcount]
        crc16get = buf[8 + 72 + 80 * carcount:]
        #print("crc len:%s" % len(crc16get))
        bufinfo = buf[8:8+72]
        self.car_id = struct.unpack(">H",bufinfo[14:16])[0]
        buf = buf[8+72:]
        index = 0
        while index < carcount:
          carstruct = buf[index * 80: (index+1) * 80 ]
          tuplelist = list(struct.unpack('>BffffffBBBdfffHBdddIB',carstruct))
          self.vehdata.append(tuplelist[0])#ID
          self.vehdata.append(self.kindofTar(tuplelist[8]))#type
          lanNum,x,y = self.getLanNum(tuplelist[1],tuplelist[2])
          self.vehdata.append(lanNum)#state,one byte / lanNum
          self.vehdata.append(tuplelist[4])#v
          self.vehdata.append(x)#x
          self.vehdata.append(y)#y
          index = index + 1
          #print(self.vehdata)
        if self.crc16(bufcrc16, False) != crc16get:
          #self.vehdata = []
          #print("crc checkout failed!")
          self.pub_data()
          return 
        else:
          self.pub_data() 
        #print("get car data end!")
      elif head_ascii == '#KKA':
        print(head_ascii)
        self.Log_pub("Recv the KKA from lidar!",INFO)
        #self.sendcmd("INIT")
      else:
        print("other data!")
        self.Log_pub("Recv unkown data from lidar!",INFO)

     
    def kindofTar(self, cartype):
      if cartype == 1: #car
        return 3
      elif cartype == 2:#bus
        return 4
      elif cartype == 3:#train
        return 5
      else:
        return 0

    def crc16(self, x, invert):
      a = 0xFFFF
      b = 0xA001
      for byte in x:
        a ^= ord(byte)
        for i in range(8):
          last = a % 2
          a >>= 1
          if last == 1:
              a ^= b
      s = hex(a).upper()
      return s[4:6]+s[2:4] if invert == True else s[2:4]+s[4:6]
            
    def pub_data(self):
      #self.pub_veh.stamp= time.time()
      self.send_count_
      #self.pub_veh.frame_id = self.device_id
      self.pub_veh.frame_id = self.device_id
      i = 0
      for val in self.vehdata:
        self.pub_veh.vehicaldatas.insert(i,val)
        i = i + 1
      self.pub.publish(self.pub_veh)
      self.send_count_ = self.send_count_ + 1;
      #print("mmlidata send times:%s" % self.send_count_)

      if self.datasave:
        self.saveData(str(self.vehdata))
      self.vehdata=[]
      self.pub_veh.vehicaldatas=[]

    def Log_pub(self, str_describe,logtype):
      systemlog_ = systemlog()
      systemlog_.device_id = self.device_id
      systemlog_.node_id = "lidar_node"
      systemlog_.type = logtype;
      systemlog_.describe = str_describe
      systemlog_.data = self.logdata
      self.systemlog_pub_.publish(systemlog_)
      self.logdata = []

if __name__ == '__main__':
    try:
        tcp_link = Tcp_link()
    except Exception as e:
        print(e)
        




























