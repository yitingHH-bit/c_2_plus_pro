#!/usr/bin/env python
#coding=utf-8

#代码描述（重要）
#时间源有两个，优先时间源为外部时钟（程序中的DNS），ros其他节点可以通过/time_now话题，随时发送bdinfomsg消息，一旦本节点收到话题消息，时间立即更新。
#另一个时钟源为北斗模块，定义一个定时器，默认时间为6小时（INTERVAL = 6），每6小时取一次北斗消息（一次消息为10组数据，每组数据携带不同的消息，综合能得到时间和地点信息，详情见北斗说明）。
#定时器时间到，首先接受时间和位置消息，查看上次更新的时间距离现在有没有超过5.98小时，如果超过，判断在上一个定时周期里，没有外部时钟源对系统进行时间更新，此时便使用北斗时间对系统进行校时一次。如果没有超过，判断在上一个周期内，已经由外部时钟源对系统进行来更新，此时便不做时间处理。
#每一个定时周期完成后，都会向ros网络发送一个位置信息话题bdinfo_pos。

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
ERROR = 10 #错误
ERROR1 = 11 #错误
WARRING = 1 #警告
INFO = 2 #普通消息
DEBUG = 3 #调试信息

#定时器类，时间精度为s
class MyTimer(_Timer):
  def run(self):
    while not self.finished.is_set():
      self.function(*self.args, **self.kwargs)
      self.finished.wait(self.interval) 

#时间管理类
class TimeManager:
  def __init__(self):
    self.node_name = 'timemanager_node'
    rospy.init_node(self.node_name, anonymous = True)#初始化节点
    self.systemlog_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#发送系统日志
    self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)#接受动态配置信息
    self.time_sub_ = rospy.Subscriber('/time_now', bdinfomsg, self.getTimeCb)#接受授时信息
    self.position_pub_ = rospy.Publisher('bdinfo_pos', bdinfomsg, queue_size = 10)#发布北斗位置
    self.init_data()#信息初始化

    self.mytimer = MyTimer(self.Peroid_interval * 3600, self.timer_fun) #初始化定时器 单位是s，所以设定数据×3600为小时
    self.mytimer.start()#开始定时器
    rospy.spin()#ros中阻塞机制，不让节点结束

  #数据初始化
  def init_data(self):
    self.conn = False #默认串口没有打开
    self.logdata = [] #日志数据列表
    self.deviceID = rospy.get_param("/DeviceID")#从ros中获取设备id
    self.Peroid_interval = rospy.get_param("/TIME_INTERVAL")#从ros中获取更新时间，单位小时
    self.buand = 9600 #一次接受数据长度
    self.msg_time = '00:00' #初始化时间变量
    self.msg_lon = 0 #初始化经度
    self.msg_alt = 0 #初始化纬度
    self.datetime = '00/00/2000' #初始化日期
    self.datetime_DNS = '00/00/2000' #初始化系统传过来的日期
    self.msg_time_DNS = '00:00'#初始化系统传过来的时间
    self.history_flash_time = time.time()#获取当前时间点，作为更新的计算的初始时间点
    self.Log_pub("init finished!",INFO) #发送log数据

  #设置时间
  #输入：1,日期，2,时间
  def set_time(self,date,time):
    try:
      os.system('echo nvidia | sudo -S date -s '+ date)#更新系统日期
      os.system('echo nvidia | sudo -S date -s ' + time)#更新系统时间
      self.history_flash_time = time.time() #记录本次更新时间，方便定时器做比较
      self.logdata.append("now time is " + date + "-" + time)#给log写数据
      self.Log_pub("Timer flashing finished!",INFO)#发送log数据
    except Exception as e:
      self.logdata.append(e)
      self.Log_pub("Timer flashing Failed!",WARRING)

  #从配置文件中获取北斗通讯接口
  def get_port(self):
    try:
      return rospy.get_param("/BDPORT") #返回设置的接口名称
    except:
      self.Log_pub("Config has no '/BDPORT' param",WARRING)#发送log警告信息
      return "/dev/ttyUSB0"#如果没有返回默认

  #运行参数热修改
  def getsetParamCb(self,data):
    if (data.frame_id == self.deviceID) and (data.type == 3) and (data.which_param == 3001):#换ID
      self.deviceID = int(data.setdatas[0])
      #self.Log_pub("device id is change into " + str(self.deviceID), 2)
  
  #连接北斗模块
  def connect_bd(self):   
    self.port_dev = self.get_port()#获取接口名称
    if os.path.exists(self.port_dev): 
      self.port=serial.Serial(self.port_dev,self.buand)#如果存在，实例化一个接口对象
      if not self.port.isOpen(): #如果还没有打开
        self.port.open() #打开端口
        self.conn = True #串口标示已经打开了
        #print(self.port_dev + " have opened!")
        self.Log_pub("Connecte with BDTimer successful!",INFO)#发送log
    else:
      self.Log_pub("Connecte with BDTimer failed!",INFO)#如果设置的这个端口名称不存在，发送log信息

  #外部受时信息，回调函数
  def getTimeCb(self,time_now):#接受时间 
      if (time_now.device_id == self.deviceID):#更新的信息戳与本设备id号码一致
        timeget = time_now.timenow #取时间
        self.msg_time_DNS = timeget.split("-")[1]#取时间
        self.datetime_DNS = timeget.split("-")[0]#取日期
        self.set_time(self.datetime_DNS,self.msg_time_DNS)#更新时间
        
  
  #定时器执行函数
  def timer_fun(self):
     times = 0 #取北斗信息的次数，默认每次到时间，取10次，能取到完整的一秒所有的信息
     if self.conn: #版别是否链接成功
       while times<10:#取10次，做10次拆包解析
         times = tiems + 1
         data=self.port.readline()
         self.unpackmsg(data) #解析串口数据
       time_now = time.time()#记录现在时间
       if( (time_now - self.history_flash_time) > Peroid_interval * 3590):#如果上次更新的时间距离现在超过来设置的更新时间，才更新
         self.set_time(self.datetime,self.msg_time) 
       self.pose_pub()#发布时间和位置    
     else:#设备没有链接成功
       self.connect_bd()

  def unpackmsg(self,msg_pack):
        if msg_pack.find("GNRMC") != -1: #能取时间，日期，和经纬度
            arr=msg_pack.split(',')
            if arr[1] != '':
                self.msg_time = str(int(arr[1][0:2])+8) + ":" + arr[1][2:4] + ":" + arr[1][4:6]
            if arr[5] != '':
                self.msg_lon = float(arr[5])
                self.msg_alt = float(arr[3])
            if arr[9] != '':
                self.datetime =  arr[9][2:4] +"/"+ arr[9][0:2] + "/20" + arr[9][4:6]
            return
        elif msg_pack.find("GNGGA") != -1: #能取时间和经纬度
            arr=msg_pack.split(',')
            if arr[1] != '':
                self.msg_time = str(int(arr[1][0:2])+8) + ":" + arr[1][2:4] + ":" + arr[1][4:6]
            if arr[4] != '':
                self.msg_lon = float(arr[4])
                self.msg_alt = float(arr[2])
            return
        elif msg_pack.find("GNGLL") != -1:#能取时间和经纬度
            arr=msg_pack.split(',')
            if arr[5] != '':
                self.msg_time = str(int(arr[5][0:2])+8) + ":" + arr[5][2:4] + ":" + arr[5][4:6]
            if arr[3] != '':
                self.msg_lon = float(arr[3])
                self.msg_alt = float(arr[1])
            return

  #发送ros话题数据
  def pose_pub(self):
    poseinfo = bdinfomsg()#初始话话题消息，bdinfo功能包msg下自定义消息
    poseinfo.node_name = self.node_name #节点名称
    poseinfo.device_id = self.deviceID #设备id号码
    poseinfo.timenow = self.datetime + '-' + self.msg_time #时间拼接
    poseinfo.lon = self.msg_lon #经度
    poseinfo.alt = self.msg_alt #纬度
    self.position_pub_.publish(poseinfo)#发送ros话题数据

  #发送log消息话题数据，接口
  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceID
    systemlog_.node_id = self.node_name
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



      

