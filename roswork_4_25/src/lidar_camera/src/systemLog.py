#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''程序介绍：（非常重要）
1,本程序主要是写log文件，采用回调机制工作，当ros中有/systemLog_info话题时，写log文件
2,log分为错误，警告，调试，一般信息
3,文件保存更具月份来区分目录，根据日期来新建文件,同时删除上上个月的数据

'''

import time
import datetime
import os
import rospy
import thread
import psutil
from all_msgs_srvs.msg import systemlog
from all_msgs_srvs.msg import errorMsgs
from all_msgs_srvs.msg import settings

LOGPATH = "../Log/"#默认log保存的路径


class WriteLog:
  def __init__(self):
    self.init_data()#初始化数据p
    self.log_sub_ = rospy.Subscriber("/systemLog_info", systemlog, self.getLogCb)#日志消息接受
    self.error_pub_ = rospy.Publisher('/systemError_info', errorMsgs, queue_size = 10)#系统错误发布
    self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)#参数配置
  
  def init_data(self):
    self.deviceID = rospy.get_param("/DeviceID")#获取id
    self.log_path = LOGPATH
    self.log_path = rospy.get_param("/LOG_PATH")#获取log储存路径
    self.oldMon_ = self.getNowMon()
    self.delMon_ = '2020-12' #上上个月初始值
    #初次创建
    self.getNowTime()#获取当前时间
    folderName = self.log_path + self.getNowMon()#创建目录名称
    self.createLogFileFolder(folderName)#创建目录
    fileName = folderName + "/" + self.getNowDate()#创建文件名称
    self.createLogFile(fileName,True)#创建文件

  def getsetParamCb(self,data):
    if (data.frame_id == self.deviceID) and (data.type == 3) and (data.which_param == 3001):#雷达ID
      self.deviceID = int(data.setdatas[0])
      #自己给自己加上log
      nowtime_str = self.getNowTime()
      logString = ''
      logString += "[" + nowtime_str + "]  "
      logString += "[sysLog_node][INFO   ]: Change the deviceID!" 
      logString += "\n\t\t\tdata:" + str(int(data.setdatas[0]))
      #文件操作
      self.fileDeal()
      self.writeLog(logString)

  #Log消息监听
  def getLogCb(self,data):
    if data.device_id != self.deviceID:
      return 
    nowtime_str = self.getNowTime()#获取当前时间
    #组织文字格式
    logString = ''
    logString += "[" + nowtime_str + "]  "
    if data.type >= 10:#错误信息Error
      logString += "[" + data.node_id + "][ERROR  ]:" + data.describe + "!!!!!!!!!!!!!!!!!!!!"
    elif data.type == 1:#警告信息Warring
      logString += "[" + data.node_id + "][WARRING]:"+ data.describe
    elif data.type == 2:#正常输出打印info
      logString += "[" + data.node_id + "][INFO   ]:" + data.describe
    else:
      logString += "[" + data.node_id + "][DEBUG  ]:" + data.describe
    if len(data.data)>0:
      logString += "\n\t\t\tdata:" + str(data.data)
    #文件操作
    self.fileDeal()
    self.writeLog(logString)#写文件

  #Log消息写文件
  def writeLog(self,logMsgs):
    print logMsgs
    self.log_file.write(logMsgs +'\n')
    self.log_file.flush()

  def pub_error(self,typenum):
    err_info = errorMsgs()
    err_info.device_id = self.deviceID
    err_info.type = typenum
    error_pub_.publish(err_info)
    

  #判断是否应该换文件了
  def fileDeal(self):
    folderName = self.log_path + self.getNowMon()
    if self.oldMon_ != self.getNowMon():
      self.createLogFileFolder(folderName)
      fileName = folderName + "/" + self.getNowDate()
      self.createLogFile(fileName)
    elif self.oldDate_ != self.getNowDate():
      fileName = folderName + "/" + self.getNowDate()
      self.createLogFile(fileName)

  #创建文件夹,用月分命名,同时删除上上个月
  def createLogFileFolder(self,folderName):
    if not os.path.exists(folderName):
       os.mkdir(folderName)
       self.deleteLogFileFolder(self.log_path + self.delMon_)
       self.delMon_ = self.oldMon_
       self.oldMon_ = self.getNowMon()
    else:
       self.oldMon_ = self.getNowMon()
    pass

  #删除文件目录
  def deleteLogFileFolder(self,folderName):
    if os.path.exists(folderName): #判断mydata文件夹是否存在
      for root, dirs, files in os.walk(folderName, topdown=False):
        for name in files:
            os.remove(os.path.join(root, name)) #删除文件
        for name in dirs:
            os.rmdir(os.path.join(root, name)) #删除文件夹
      os.rmdir(folderName) #删除folderName文件夹

  #创建文件,用日期命名
  def createLogFile(self,fileName,isNew = False):
    fileName = fileName + ".log"
    if not isNew:
      self.log_file.close()
    self.log_file = open(fileName,"a+")
    self.oldDate_ = self.getNowDate()
    pass

  #获取日期
  def getNowDate(self):
    return time.strftime('%Y-%m-%d',time.localtime(time.time()))

  #获取月份
  def getNowMon(self):
    return time.strftime('%Y-%m',time.localtime(time.time()))

  #获取时间
  def getNowTime(self):
    self.now_ = time.time()
    self.local_time_ = time.localtime(self.now_)
    self.nowMon_ = time.strftime('%Y-%m',self.local_time_)
    self.nowDate_ = time.strftime('%Y-%m-%d',self.local_time_)
    return time.strftime('%H:%M:%S',self.local_time_)

  def sysMonitor(self):
    pass

  def __del__(self):
    print("Log_node end!")


























