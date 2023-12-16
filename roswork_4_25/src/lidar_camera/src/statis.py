#!/usr/bin/env python
# -*- coding: utf-8 -*-

import time
import json
import socket
import datetime
import os
import rospy
import math
import threading
from all_msgs_srvs.msg import settings
from all_msgs_srvs.msg import vehicalmsgs
from threading import _Timer
from all_msgs_srvs.msg import systemlog

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

class Statis:
  def __init__(self):
    rospy.init_node('statis_node', anonymous = True)
    self.systemlog_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#发送系统日志
    try:
      self.init_data()
      self.vehs_sub_ = rospy.Subscriber("mmlidar_veh", vehicalmsgs, self.getvehsCb)
      self.statis_pub_ = rospy.Publisher('/statis_info', settings, queue_size = 10)
      self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)
      self.lock_veh=threading.Lock()


    #print(self.StatisPeriod)
      self.mytimer = MyTimer(self.StatisPeriod, self.Timer_pub_statis)
      self.mytimer.start()
      self.Log_pub("Statis timer start!",INFO)
    except Exception as e:
      self.Log_pub("Init is failed:"+str(e),ERROR)
    while not rospy.is_shutdown():
      pass
    self.mytimer.cancel()
      
    

  def __del__(self):
    print("Statis_node end!")
    self.Log_pub("Statis_node end!",ERROR)

  def init_data(self):
    self.logdata = [] #日志数据

    vehlistlong = rospy.get_param("/VehicalLong")
    distanceStopLine = rospy.get_param("/DistanceStopLine")
    statisPeriod = rospy.get_param("/StatisPeriod")
    checkDLong = rospy.get_param("/CheckDLong")
    ncount = rospy.get_param("/nCount")
    deviceid = rospy.get_param("/DeviceID")

    self.frame_id = deviceid
    self.CarLong = vehlistlong[2]
    self.BusLong = vehlistlong[1]
    self.TrainLong = vehlistlong[0]
    self.StopLDistance = distanceStopLine
    #self.SectionList = {'1':[50,90],'2':[50,90],'3':[50,90],'4':[50,90],'5':[50,90],'6':[50,90]} #
    self.SectionList = {}
    self.Queue_list = {}
    self.Lan_stopline = [30,30,30,30,30,30,30,30,30,30,30]
    self.StatisPeriod = statisPeriod #min
   
    #self.lidarHz = 15 #hz of lidar
    self.checkDLong = checkDLong #m
    #self.PeriodNum = 0

    for i in range(int(ncount)):
      lanParamname = "/StatisPosition/" + str(i+1) 
      datalist = rospy.get_param(lanParamname)
      self.SectionList[str(i+1)] = []
      self.Lan_stopline[i+1] = rospy.get_param("/LanParam/" + str(i+1) + "/coordinate")[1]
      for j in range(int(datalist[0])):
        self.SectionList[str(i+1)].append(datalist[j+1][0])
    print self.SectionList
    print self.Lan_stopline
    self.create_queue_by_lancount(int(ncount),self.Lan_stopline)
    self.logdata.append(self.SectionList)
    self.Log_pub("Get lan_section data from rosparams!",INFO)

    #print(self.SectionList)
    #print(deviceid)
    #print(self.StatisPeriod)
    #print(self.TrainLong)
    self.statis_pub_data = settings()

    self.crate_buff_from_info()

  def create_queue_by_lancount(self,lannum,lanYlist):
    if len(lanYlist) >= lannum:
      for i in range(lannum):
        listlan = [False,lanYlist[i+1],[0,0],[0,0]]
        self.Queue_list[str(i+1)] = listlan
    print self.Queue_list

  def crate_buff_from_info(self):
    self.vehStatis_data = {}
    self.vehstream_data = {}
 
    for key,value in self.SectionList.items():
      lanlist = []
      streamlist = []
      for NumCheckindex in range(len(value)):
        checklist = [0,0,0,0,0.0,0.0,0.0] #总车流量,小车,大车,超大车,车头时距(略,=period / 总车流量),流量占有率(时间累计),速度累计,累计车长
        checksslist = [{},{}] # 当前数据,历史数据,检测周期数,周期内几次检测,车辆占有时长总和(单位m/(km/h/3.6) = s)
        lanlist.append(checklist)
        streamlist.append(checksslist)
      self.vehStatis_data[key] = lanlist
      self.vehstream_data[key] = streamlist
      self.Log_pub("Create new buffer is finished!",INFO)

  def getsetParamCb(self,data):
    try:
      #self.lock_veh.acquire()
      if (data.frame_id == self.frame_id) and (data.type == 1) and (data.which_param == 1101):#除周期以外的参数
        print("修改统计参数")
        self.logdata=data.setdatas
        self.Log_pub("Change statis common params!",INFO)
        self.TrainLong = data.setdatas[0]
        self.BusLong = data.setdatas[1]
        self.CarLong = data.setdatas[2]
        index = 4
        lan_sum = int(data.setdatas[3])

        for i in range(lan_sum):
          self.SectionList[str(i+1)] = []
          len_lan = int(data.setdatas[index])
          index = index + 1
          for j in range(len_lan):
            self.SectionList[str(i+1)].append(data.setdatas[index])
            index = index + 1
        print self.SectionList
        self.create_queue_by_lancount(int(lan_sum),self.Lan_stopline)

        #self.logdata.append(self.SectionList)
        self.Log_pub("Set statis common param successful!",INFO)
        self.crate_buff_from_info()
        pass
      elif (data.frame_id == self.frame_id) and (data.type == 1) and (data.which_param == 1102):#周期设置
        self.StatisPeriod = data.setdatas[0]
        #定时器重新生效
        print("统计周期设置")
        self.logdata.append(data.setdatas[0])
        self.Log_pub("Set period params successful!",INFO)
        self.mytimer.cancel()
        self.mytimer = MyTimer(self.StatisPeriod, self.Timer_pub_statis)
        self.mytimer.start()
        self.Log_pub("Timer start again!",INFO)
      elif (data.frame_id == self.frame_id) and (data.type == 3) and (data.which_param == 3001):#ID设置
        self.frame_id = int(data.setdatas[0])
        print("id params ,in statis_node, is changed to ")
        print self.frame_id
        self.logdata.append(data.setdatas[0])
        self.Log_pub("Change deviceid params successful!",INFO)
      elif (data.frame_id == self.frame_id) and (data.type == 0) and (data.which_param == 1001):#道路坐标设置
        index = 1
        mydata = data.setdatas
        for i in range(int(mydata[0])):
          self.Lan_stopline[str(i+1)] = mydata[index + 1]
          index = index + 2
        self.create_queue_by_lancount(int(mydata[0]),self.Lan_stopline)
        self.logdata = mydata
        self.Log_pub("Change laninfo params successful!",INFO)
        
      #self.lock_veh.release()
         
    except Exception as e:
        print(e)
        self.logdata = []
        self.Log_pub("Set param operation Error!",ERROR1)

  def getvehsCb(self,data):
    #print data.frame_id
    if data.frame_id != self.frame_id:
      return
    try:
      #self.lock_veh.acquire()
      #print("000")
      self.getVehs(data.vehicaldatas)
      self.statis()
      #self.lock_veh.release()
    except Exception as e:
      print("Statis Error")
      self.Log_pub("Statis Error!",ERROR)

  def Queue_length(self, sort, lan, spend, y):
    if abs(spend) < 1.5:#
      #print("speed")
      if str(lan) in self.Queue_list.keys():
        #print("lan")
        if self.Queue_list[str(lan)][0]: #已经有人在排队来
          #print("had")
          if y>self.Queue_list[str(lan)][2][1] - 2.5 and y<self.Queue_list[str(lan)][2][1] + 2.5:#近处
            self.Queue_list[str(lan)][2] = [sort,y] 
            #print("low") 
            #print self.Queue_list    
          elif y>self.Queue_list[str(lan)][3][1] - 2.5 and y<self.Queue_list[str(lan)][3][1] + 15:#远处
            self.Queue_list[str(lan)][3] = [sort,y] 
            #print("yuan")
            #print self.Queue_list
        else: #没有人在排队
          #print("no had")
          if y>self.Queue_list[str(lan)][1] - 5 and y<self.Queue_list[str(lan)][1] + 5:#
            #print("add........................................")
            self.Queue_list[str(lan)][0] = True
            self.Queue_list[str(lan)][2] = [sort,y]
            self.Queue_list[str(lan)][3] = [sort,y]
            #print self.Queue_list
    elif abs(spend) > 5:#
      if str(lan) in self.Queue_list.keys():
        if self.Queue_list[str(lan)][0]: #已经有人在排队来
          if y>self.Queue_list[str(lan)][2][1] - 1 and y<self.Queue_list[str(lan)][2][1] + 1:#
            #print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            #print('now:%s' % self.Queue_list)
            self.Queue_list[str(lan)][0] = False
            #print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')
            #print('now1:%s' % self.Queue_list)
            #print('!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!')


  def getVehs(self,data):
    i = 0
    while i < len(data):
      self.Queue_length(int(data[i+1]),int(data[i+2]),data[i+3],data[i+5])
      for key,value in self.SectionList.items(): 
        isFound = False
        if int(data[i+2]) == int(key): #那个车道
          for j in range(len(value)):
            if data[i+5] < value[j] + self.checkDLong / 2.0 and data[i+5] > value[j] - self.checkDLong / 2.0: #那个位置
              #print(str(key) + '  ' + str(int(data[i+1])) + ":+1")
              temp_dir = {}
              temp_dir[str(int(data[i]))] = [data[i+1],data[i+3]]#车型,速度
              self.vehstream_data[key][j][0] = temp_dir
              isFound = True
            if isFound:
              break
        if isFound:
          break
      i = i + 6

  def vehsLong(self,sort):
    if int(sort) == 3: #car 
      return self.CarLong
    elif int(sort) == 4: #bus
      return self.BusLong
    elif int(sort) == 5: #train
      return self.TrainLong
    else:
      return 0

  def statis(self):
    #1/和上个时刻比较,如果重复,移除
    for key,value in self.vehstream_data.items():#取车道,
      i = 0 #取地点编码
      for ssdata in value: #取地点
        for ID,vehdata in ssdata[0].items(): #取当前id 与 过去id做比较
          if ssdata[1].keys().count(ID) == 0: #表示是新的数据,添加
            self.vehStatis_data[key][i][0] = self.vehStatis_data[key][i][0] + 1 #总流量
            #print(str(key) + "车道,断面" + str(i) + "+1 = " + str(self.vehStatis_data[key][i][0]))
            self.vehStatis_data[key][i][int(vehdata[0])-2] = self.vehStatis_data[key][i][int(vehdata[0]) - 2] + 1 #单一车辆流量
            self.vehStatis_data[key][i][4] = self.vehStatis_data[key][i][4] + self.vehsLong(vehdata[0]) / (float(math.fabs(vehdata[1]))/3.6) # 时间占有的累加,单位s
            self.vehStatis_data[key][i][5] = self.vehStatis_data[key][i][5] + abs(vehdata[1]) # 速度累计            
            #print(str(key) + "车道,断面" + str(i) + "+all_spend = " + str(self.vehStatis_data[key][i][5]) + ", arvspeed=" +  str(self.vehStatis_data[key][i][5]/self.vehStatis_data[key][i][0]))    
            self.vehStatis_data[key][i][6] = self.vehStatis_data[key][i][6] + self.vehsLong(vehdata[0]) # 车长累计
          #不管是新旧,源数据删除取代
        self.vehstream_data[key][i][1] = ssdata[0]
        self.vehstream_data[key][i][0] = {}
        i = i + 1

  #取排队长度
  def get_queue_length(self,lan):
    ret = 0.0
    lan = int(lan)
    #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!! this lan is:%s" % str(lan))
    #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!! this Queue key is:%s" % self.Queue_list.keys())
    #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!! this Queue list is:%s" % self.Queue_list)
    if str(lan) in self.Queue_list.keys():
      #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!! this lan1 is:%s" % self.Queue_list)
      if self.Queue_list[str(lan)][0]: #已经有人在排队来
        #print("!!!!!!!!!!!!!!!!!!!!!!!!!!!! this lan2 is:%s" % self.Queue_list[str(lan)][0])
        ret = self.Queue_list[str(lan)][3][1] - self.Queue_list[str(lan)][2][1] + self.vehsLong(self.Queue_list[str(lan)][3][0])
        #print("Queue length is: %s" % ret)
    return ret

  def Timer_pub_statis(self):
    t = time.time()
    t_s = int(t)
    t_m = int((t - t_s) * 1000)
    self.statis_pub_data.stamp.secs = t_s
    self.statis_pub_data.stamp.nsecs = t_m
    self.statis_pub_data.frame_id = self.frame_id
    self.statis_pub_data.type = 1
    self.statis_pub_data.which_param = 11

    #self.lock_veh.acquire()

    self.statis_pub_data.setdatas.append(float(self.StatisPeriod))#统计周期
    self.statis_pub_data.setdatas.append(float(self.TrainLong))#A车长
    self.statis_pub_data.setdatas.append(float(self.BusLong))#B车长
    self.statis_pub_data.setdatas.append(float(self.CarLong))#C车长
    self.statis_pub_data.setdatas.append(float(len(self.SectionList)))#车道数
    for key,value in self.vehStatis_data.items():
      self.statis_pub_data.setdatas.append(float(key))#车道号码
      self.statis_pub_data.setdatas.append(float(len(value)))#断面数
      for i in range(len(value)):
        self.statis_pub_data.setdatas.append(float(value[i][0]))#总流量
        all_ss = value[i][0]
        if all_ss == 0:
          all_ss = -1
        self.statis_pub_data.setdatas.append(float(value[i][1]))#1车流量
        self.statis_pub_data.setdatas.append(float(value[i][2]))#2车流量
        self.statis_pub_data.setdatas.append(float(value[i][3]))#3车流量
        self.statis_pub_data.setdatas.append(float(self.StatisPeriod * 60.0 / all_ss))#车头时距
        self.statis_pub_data.setdatas.append(float(value[i][4] / (self.StatisPeriod * 60.0)))#车辆占有率
        self.statis_pub_data.setdatas.append(float(value[i][5] / all_ss))#平均车速
        self.statis_pub_data.setdatas.append(float(value[i][6] / all_ss))#平均车长
        self.statis_pub_data.setdatas.append(self.get_queue_length(float(key)))#排队长度
        #self.statis_pub_data.setdatas.append(-99.9)#排队长度
        #数据清零
        value[i][0] = 0
        value[i][1] = 0
        value[i][2] = 0
        value[i][3] = 0
        value[i][4] = 0.0
        value[i][5] = 0.0
        value[i][6] = 0.0

    #self.lock_veh.release()

    self.statis_pub_.publish(self.statis_pub_data)
    self.statis_pub_data.setdatas = []
    #print("statis data have published!")
    
  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.frame_id
    systemlog_.node_id = "statis_node"
    systemlog_.type = int(logtype)
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.systemlog_pub_.publish(systemlog_)
    self.logdata = []
    
    

if __name__ == '__main__' :
  try:
    statis = Statis()
  except Exception as e:
    print e

























