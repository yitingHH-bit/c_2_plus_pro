#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
程序简介（非常重要）：
1,本节点作用主要有（都是回调机制）
  （1）解析上位机发送的配置信息，这个配置信息是由ros2up节点直接将数据部分通过ros话题的形式转送过来的
  （2）读写yaml配置文件，将解析好的上位机配置信息写入yaml保存，如果需要，将yaml配置好的信息读出来反馈给上位机
  （3）让上位机配置的信息生效，例如设置本机ip
2,本节点接受由ros2up节点传送过来的配置信息，话题名称为/ros_settings
3,接受配置信息按照tcp协议格式进行解析，不同的是每个字节现在都成了float类型数据
4,将解析完成的数据，写到对应的yaml文件中，如果需要对系统设置生效的，当场设置生效
5,将需要给其他节点动态热配置的参数（不用重启便能生效的参数），通过/ros_param_setting话题传送给其他需要的节点
6,接受读取参数文件的服务，上位机需要获取已经配置的参数，由ros2up节点的ros客户端来请求本节点的server服务，将读到的相关信息传送给ros2up，此服务名称为/get_param_from_file

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

from udpSendInfo import UDPSendInfo
from systemLog import WriteLog
from systemMonitor import SystemMon

CONFIGPATH="/home/hsax/roswork/src/lidar_camera/config/"#配置文件的路径
STATISCONFIG = "StatisInfo.yaml"#统计断面配置文件名称
LANINFOCONFIG = "LanInfo.yaml"#道路信息配置文件名称
INSTALLCONFIG = "InstallInfo.yaml"#安装和标校文件配置的名称
NETWORKCONFIG = "NetworkInfo.yaml"#网络参数配置的文件名称

#log中对于信息的等级定义
ERROR = 10
ERROR1 = 11#严重错误
WARRING = 1
INFO = 2
DEBUG = 3

class SettingNode:
  def __init__(self):
    self.init_data()#初始化参数
    self.callBack_serv_ = rospy.Service('/get_param_from_file', settingcallback, self.recv_setting_callback)#上位机参数查询操作,ros服务机制
    self.settingVal_pub_ = rospy.Publisher('/ros_param_setting', settings, queue_size = 10)#给每个节点设置热参数，用这个话题，将解析好的，需要的参数发送给其他节点
    self.systemlog_pub_ = rospy.Publisher('/systemLog_info', systemlog, queue_size = 10)#发送系统日志
    
    self.setting_sub_ = rospy.Subscriber("/ros_settings", settings, self.settingSysParams);#上位机配置信息话题监听

  def __del__(self):
    print("setting.py is end!")
    self.Log_pub("setting.py is end!",INFO)

  def init_data(self):
    deviceid = rospy.get_param("/DeviceID")#只获取了一个设备id编号
    self.deviceId = deviceid
    self.logdata = []
    pass

  #配置信息查询的服务机制，返回已经配置的信息
  def recv_setting_callback(self,req):
    try:

      ret = []
      if req.fram_id != self.deviceId:#检查id编号
         print "failed require"
         self.Log_pub("Require failed in result of different deviceid",WARRING)
         ret =  [-0.101,-0.101,-0.101]#如果不是本设备id则返回-0.101
         return settingcallbackResponse(ret)
      obj_tag = req.type #取服务调取的类型

      print "****************************************************"
      print "obj_tag: ",obj_tag
      print "****************************************************"

      if obj_tag == 0:#道路信息查询的
        ret =  self.return_lan_info()#读取道路信息配置文件
      elif obj_tag == 1:#统计参数查询
        ret =  self.return_statis_setinfo()#读取统计配置信息配置文件
      elif obj_tag == 2:#转换参数查询(目前还没有使用)
        ret =  self.return_transform_info()
      elif obj_tag == 3:#网络参数查询
        ret =  self.return_network_info()#读取网络信息配置文件
      elif obj_tag == 4:#标校参数查询
        ret =  self.return_cli_info()#读取安装和标校信息配置文件
      elif obj_tag == 5:#统计周期参数查询
        ret =  self.return_period_info()#读取统计周期信息配置文件
      elif obj_tag == 6:#外部设备ip参数查询
        ret =  self.return_out_device_info()#读取外部设备信息配置文件
      self.logdata = ret
      self.Log_pub("Required data",INFO)
      return settingcallbackResponse(ret)
    except Exception as e:
      print e
      self.Log_pub(str(e),ERROR1)  
 
#读取外部设备信息配置文件 
  def return_out_device_info(self):
    print("read out device network info in setting_node")
    self.Log_pub("Read out device network info from savefile",INFO)
    ret = []
    with open(CONFIGPATH + NETWORKCONFIG, 'r') as file_to_read:#打开文件
      direc = yaml.load(file_to_read)#读取文件
      traiplist = direc['TranfficIp'].split(".")#信号灯的ip
      platiplist = direc['PlatIp'].split(".")#平台的ip
      ret.append(float(direc['TranfficState']))#信号灯的链接状态，是否要链接
      ret.append(float(direc['TranfficType']))    #信号灯的类型（Server or client）
      for val in traiplist:
        ret.append(float(val))
      ret.append(float(direc['TranfficPort']))   #信号灯的端口号码  
      ret.append(float(direc['PlatState']))#平台的链接状态，是否要链接
      ret.append(float(direc['PlatType']))    #平台的类型
      for val in platiplist:
        ret.append(float(val))
      ret.append(float(direc['PlatPort']))    #平台的端口号码
    return ret

#读取统计点的配置信息
  def return_statis_setinfo(self):
    print("read statis info in setting_node")
    self.Log_pub("Read statis info from savefile",INFO)
    ret = []
    # print "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++=========="
    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      ret.append(float(direc['VehicalLong'][0])) #A
      ret.append(direc['VehicalLong'][1]) #B
      ret.append(direc['VehicalLong'][2]) #C
      ret.append(direc['nCount'])#车道总数
      checkDlong = direc['CheckDLong'] #检测带宽度

      for lan in direc['Countlist']:
        lan_list = direc['StatisPosition'][str(lan)]

        ret.append(lan) #添加车道号   
        ret.append(lan_list[0]) #添加车道断面数
        all_num = lan_list[0]


        if all_num >= 1:
          for i in range(all_num):
            SP_index = i*3

            # leeleeleeleeeleelee
            # ret.append(i+1)#断面号
            # ret.append(checkDlong)#宽度
            # ret.append(lan_list[i+1])#距离 
            ret.append(lan_list[SP_index+1])#断面号
            ret.append(lan_list[SP_index+2]) #宽度          
            ret.append(lan_list[SP_index+3])#距离     
            # leeleeleeleeeleelee
    # print "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++=========="

    return ret

#读取车道信息
  def return_lan_info(self):
    ret = []
    print("read lan info in setting_node")
    self.Log_pub("Read lan info from savefile",INFO)
    with open(CONFIGPATH + LANINFOCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      ret.append(float(direc['LanCount'])) #车道总数
      for i in range(int(direc['LanCount'])):
        ret.append(float(i+1)) #编号
        ret.append(float(direc['LanParam'][str(i+1)]['width'])) #width
        ret.append(float(direc['LanParam'][str(i+1)]['long'])) #long
        ret.append(float(direc['LanParam'][str(i+1)]['attribute'])) #attribute
        ret.append(float(direc['LanParam'][str(i+1)]['type'])) #type
        ret.append(float(direc['LanParam'][str(i+1)]['coordinate'][0])) #x
        ret.append(float(direc['LanParam'][str(i+1)]['coordinate'][1])) #y
        ret.append(float(direc['LanParam'][str(i+1)]['maxspeed'])) #maxspeed
    return ret
  def return_transform_info(self):
    return []

#设备网络ip查询
  def return_network_info(self):
    print("read network info in setting_node")
    self.Log_pub("Read network info from savefile",INFO)
    ret = []
    with open(CONFIGPATH + NETWORKCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      netiplist = direc['MyselfNetIp'].split(".")#netip
      netgoallist = direc['MyselfNetGoal'].split(".")#netgoalways
      deviceID = direc['DeviceID']#id

      print "***********************8888888888888******************88888"
      print "deviceID: ",float(deviceID)
      print "***********************8888888888888******************88888"


      ret.append(float(deviceID))    
      for val in netiplist:
        ret.append(float(val))
      for val in netgoallist:
        ret.append(float(val))
    return ret

#校准参数查询
  def return_cli_info(self):
    print("read install info in setting_node")
    self.Log_pub("Read install info from savefile",INFO)
    ret = []
    with open(CONFIGPATH + INSTALLCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      ret.append(9)
      for i in range(9):
        if i == 0:
          lannamelist = direc['MainInstallParam'] 
        else:
          lannamelist = direc['AuxiliaryInstallParam'][str(i)]
        ret.extend([float(i),lannamelist[0],lannamelist[1],lannamelist[2],lannamelist[5],lannamelist[4],lannamelist[3]])#x,y,z,roll,pitch,yaw
    return ret

  def return_period_info(self):
    print("read period info in setting_node")
    self.Log_pub("Read period info from savefile",INFO)
    ret = []
    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      ret.append(float(direc['StatisPeriod']))
    return ret

#统计车道数据设置
#协议
#	{大车长度，中车长度，小车长度，
#	 车道总数n，
#   		车道编号1，断面数量m,
#				断面编号1,断面宽度w1,断面距离d1
#                                ...
#				断面编号m,断面宽度wm,断面距离dm
 #  		车道编号2，断面数量m,
#				断面编号1,断面宽度w1,断面距离d1
 #                               ...
#				断面编号m,断面宽度wm,断面距离dm
#		...
 #  		车道编号n，断面数量m,
#				断面编号1,断面宽度w1,断面距离d1
 #                               ...
#				断面编号m,断面宽度wm,断面距离dm}
#
#
  def statis_lan_setting(self,data):
    print("set statis info in setting_node")
    self.Log_pub("Set statis info to savefile",INFO)
    setting = settings()
    setting.frame_id = self.deviceId
    setting.type = 1
    setting.which_param = 1101
    direc = {}

    Countlist = []

    print "statis_lan_setting data: ",data

    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      direc['VehicalLong'] = [data[0],data[1],data[2]] #取车长配置
      direc['nCount'] = data[3] #取车道总数

      i =j = 0
      index = 4
      #注意：文件中的车道配置数组要大于实际的总数，这样才不至于出错，当配置数有多余的时候，可以不用，但是不能少，如果道路突然添加很多，此处设置并不会再添加，导致出错
      while i < data[3]:
        dirname = int(data[index])#取车道编码
        Countlist.append(dirname)
        j = data[index + 1]#取车道检测点数
        lan_list = []
        lan_list.append(int(j))#先添加检测点数
        index = index + 2
        for vali in range(int(j)):
          # leeleeleeleeeleelee
          # lan_list.append(data[index + 2])#再依次添加检测点距离
          lan_list.append(data[index])
          lan_list.append(data[index+1])
          lan_list.append(data[index+2])
          # leeleeleeleeeleelee
          index = index + 3
        direc['StatisPosition'][str(dirname)] = lan_list#保存语句
        i = i + 1

      direc['Countlist'] = Countlist

    with open(CONFIGPATH + STATISCONFIG, 'w') as file_to_read:#写文件
      yaml.dump(direc,file_to_read)
      self.logdata = data
      self.Log_pub("Write statis info to savefile",INFO)
    
    #添加数据,发送数据到ros,具体协议不明白见statis.cpp解析
    setting.setdatas.append(direc['VehicalLong'][0])
    setting.setdatas.append(direc['VehicalLong'][1])
    setting.setdatas.append(direc['VehicalLong'][2])
    setting.setdatas.append(direc['nCount'])#添加总车道数字
    for i in range(int(direc['nCount'])):
      setting.setdatas.append(i+1)
      setting.setdatas.append(direc['StatisPosition'][str(i+1)][0])#添加车道的检测点数
      for j in range(int(direc['StatisPosition'][str(i+1)][0])):
        SP_index = j*3
        setting.setdatas.append(direc['StatisPosition'][str(i+1)][SP_index+1])
        setting.setdatas.append(direc['StatisPosition'][str(i+1)][SP_index+2])
        setting.setdatas.append(direc['StatisPosition'][str(i+1)][SP_index+3])#依次添加检测点距离

    print("setting.setdatas: ",setting.setdatas)
    self.settingVal_pub_.publish(setting)

    self.logdata = setting.setdatas
    self.Log_pub("Publish statis info to ros",INFO)
           
#统计周期数据
  def statis_period_setting(self,data):
    print("set period info in setting_node")
    self.Log_pub("Set period info to savefile",INFO)
    setting = settings()
    setting.frame_id = self.deviceId#这一个需要使用之前的id
    setting.type = 1
    setting.which_param = 1102

    direc = {}
    with open(CONFIGPATH + STATISCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      lan_info_list0 = b''
      for val in data:
        lan_info_list0 += struct.pack('<B',val)#将float类型转换成byte
      lan_info_turple = struct.unpack('<I',lan_info_list0)#将4byte转换成int
      if lan_info_turple[0]<1: #防止错输
        lan_info_turple[0] = 1
      direc['StatisPeriod'] = lan_info_turple[0]#写id到具体变量中
    with open(CONFIGPATH + STATISCONFIG, 'w') as file_to_read:#将修改后的文件，写文件
      yaml.dump(direc,file_to_read)
      self.logdata = lan_info_turple
      self.Log_pub("Write period info to savefile",INFO)

    #热参数设置
    setting.setdatas.append(lan_info_turple[0])
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish period info to ros",INFO)


#道路信息设置
#  '''协议
#	{车道总数n，配置车道数(==车道总数，所以用不上)，
#		车道编号1(有解析，没有使用)，车道宽度，车道长度，车道属性，车道类型，车道坐标x,车道坐标y，车道最大速度，（1+4+4+1+1+4+4+4 = 23）
#		车道编号2，车道宽度，车道长度，车道属性，车道类型，车道坐标x,车道坐标y，车道最大速度，
#		车道编号3，车道宽度，车道长度，车道属性，车道类型，车道坐标x,车道坐标y，车道最大速度，
#		...
#		车道编号n，车道宽度，车道长度，车道属性，车道类型，车道坐标x,车道坐标y，车道最大速度}
# '''
  def lan_info_setting(self,data):
    print("set lan info in setting_node")
    self.Log_pub("Set lan info to savefile",INFO)
    setting = settings()
    setting.frame_id = self.deviceId
    setting.type = 0
    setting.which_param = 1001
    direc = {}
    path = CONFIGPATH + LANINFOCONFIG
    with open(path, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      direc['LanCount'] = int(data[0])#取总车道数
      i = 0
      index = 2#没有用到index=1的数据，因为配置车道数和车道总数要相等，每一次配置为全部配置
#同样道理车道数，初始化，配置文件中应该有一个很多的配置路数，道路少了，可以取少序号的，道路多了，不会动态增多，必然出错
      while i < int(data[1]):
        lan_info_list0 = b''
        for val in data[index:index+23]:#每23个byte为一个道路的完整信息
          lan_info_list0 += struct.pack('<B',val)#将float类型的数据转换成byte类型
        lan_info_turple = struct.unpack('<BffBBfff',lan_info_list0)#index = 0 的车道编号没有使用到
        j = 0
        direc['LanParam'][str(lan_info_turple[0])]['width'] = lan_info_turple[1]#车道宽
        direc['LanParam'][str(lan_info_turple[0])]['long'] = lan_info_turple[2]#车道长
        direc['LanParam'][str(lan_info_turple[0])]['attribute'] = lan_info_turple[3]#车道属性
        direc['LanParam'][str(lan_info_turple[0])]['type'] = lan_info_turple[4]#车道类型
        direc['LanParam'][str(lan_info_turple[0])]['coordinate'][0] = lan_info_turple[5]#车道x
        direc['LanParam'][str(lan_info_turple[0])]['coordinate'][1] = lan_info_turple[6]#车道y
        direc['LanParam'][str(lan_info_turple[0])]['maxspeed'] = lan_info_turple[7]#车道最大速度
        i = i + 1
        index = index + 23
    with open(CONFIGPATH + LANINFOCONFIG, 'w') as file_to_read:#写文件
      yaml.dump(direc,file_to_read)   
      self.logdata = lan_info_turple
      self.Log_pub("Write lan info to savefile",INFO)

    #热参数设置 {车道数目n，type1,width1,x1,y1,...,typen,widthn,xn,yn}
    setting.setdatas.append(direc['LanCount'])#车道数
    for i in range(direc['LanCount']):
      setting.setdatas.append(direc['LanParam'][str(i+1)]['type'])#type
      setting.setdatas.append(direc['LanParam'][str(i+1)]['width'])#width
      setting.setdatas.append(direc['LanParam'][str(i+1)]['coordinate'][0])#x0
      setting.setdatas.append(direc['LanParam'][str(i+1)]['coordinate'][1])#y
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish lan info to ros",INFO)

#安装标校准参数设定
#'''协议
#	{车道编号，   （注意：0表示安装参数，1,2表示标校带的编号）
#         x,y,z,roll,pitch,yaw}  (注意：文件中储存的顺序有所不同为：x,y,z,yaw,pitch,roll)
#
#'''
  def install_cli_setting(self,data):
    print("set install info in setting_node")
    self.Log_pub("Set install info to savefile",INFO)
    setting = settings()
    setting.frame_id = self.deviceId
    setting.type = 4

    direc = {}
    with open(CONFIGPATH + INSTALLCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      lan_info_list0 = b''
      for val in data[1:]:
        lan_info_list0 += struct.pack('<B',val)
      lan_info_turple = struct.unpack('<ffffff',lan_info_list0)
      if int(data[0]) == 0:#首位表示标识，0为安装参数，其他为对应的标校带号码
        setting.which_param = 5000#给imgpub_node设置热参数的时候，安装和标校标识是不一样的
        print "setting install 0 in setting_node"
        direc['MainInstallParam'] = [lan_info_turple[0],lan_info_turple[1],lan_info_turple[2],lan_info_turple[5],lan_info_turple[4],lan_info_turple[3]]
      else:
        print("setting install " + str(int(data[0])) + " in setting_node")
        setting.which_param = 5001
        direc['AuxiliaryInstallParam'][str(int(data[0]))] = [lan_info_turple[0],lan_info_turple[1],lan_info_turple[2],lan_info_turple[5],lan_info_turple[4],lan_info_turple[3]]
      setting.setdatas = [data[0],lan_info_turple[0],lan_info_turple[1],lan_info_turple[2],lan_info_turple[5],lan_info_turple[4],lan_info_turple[3]]#{标识，x,y,z,yaw,pitch,yaw}
    with open(CONFIGPATH + INSTALLCONFIG, 'w') as file_to_read:
      yaml.dump(direc,file_to_read)       
      self.logdata = lan_info_turple
      self.Log_pub("Write install info to savefile",INFO)

    #发布安装标校参数
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish install info to ros",INFO)

  #网络参数设置
#'''协议
#	{雷达预编号，雷达编号，
#	雷达ip1,雷达ip2,雷达ip3,雷达ip4,
#	网关号1,网关号2,网关号3,网关号4}
#'''
  def network_setting(self,data):
    print("set network info in setting_node")
    self.Log_pub("Set network info to savefile",INFO)
    setting = settings()
    setting.frame_id = self.deviceId
    setting.type = 3
    setting.which_param = 3001

    print "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"

    print "data: ",data

    #解析IP
    lan_info_list0 = b''
    for val in data:
      lan_info_list0 += struct.pack('<B',val)#float转换为byte

    lan_info_turple = struct.unpack('<HBBBBBBBB',lan_info_list0)#将byte[]转换成各种数据
    iplist = []
    for val in lan_info_turple[1:5]:#取ip
      iplist.append(int(val))
    ip = str(iplist[0]) + "." + str(iplist[1]) + "." + str(iplist[2]) + "." + str(iplist[3])
    netgoal = []
    for val in lan_info_turple[5:]:#取网关
      netgoal.append(int(val))
    goal = str(netgoal[0]) + "." + str(netgoal[1]) + "." + str(netgoal[2]) + "." + str(netgoal[3])

    #解析ID
    deviceId = lan_info_turple[0]

    #设置IP，让网络生效
    strfinal = ''
    f = open(CONFIGPATH +"interfaces", "w+")       
    strfinal += "auto eth0\n"
    strfinal += "iface eth0 inet static\n"
    strfinal += "address " + ip + "\n"
    strfinal += "netmask 255.255.255.0\n"
    strfinal += "gateway " + goal + "\n\n"
    f.write(strfinal + "\n")                  
    f.close()
    os.system('echo hsax123 | sudo -S cp -rf ' + CONFIGPATH + '/interfaces /etc/network/interfaces') #写文件
    os.system('echo hsax123 | sudo -S ip addr flush eth0')#更新网络
    os.system('echo hsax123 | sudo -S systemctl restart networking.service')#重启网络
   
    self.deviceId = deviceId#将id就地修改
   
    #写IP到文件，保存设置
    direc = {}
    with open(CONFIGPATH + NETWORKCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      direc['MyselfNetIp'] = ip #保存ip
      direc['MyselfNetGoal'] = goal #保存netgoalways
      direc['DeviceID'] = deviceId #保存id

      print "WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW"
      print "***********************8888888888888******************88888======================"
      print "direc['DeviceID']: ",deviceId
      print "***********************8888888888888******************88888======================="


    with open(CONFIGPATH + NETWORKCONFIG, 'w') as file_to_read:#写文件
      yaml.dump(direc,file_to_read)
      self.logdata = lan_info_turple
      self.Log_pub("Write network info to savefile",INFO)

    #热参数设置{id, ip1,ip2,ip3,ip4}，作为服务器，好像没有作用
    setting.setdatas = [deviceId,iplist[0],iplist[1],iplist[2],iplist[3]]
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish network info to ros",INFO)

#外部设备网络设置
#'''协议
#	{信号机链接状态，信号机通讯方式，信号机ip1,信号机ip2,信号机ip3,信号机ip4,信号机port，
#	 平台链接状态，平台通讯方式，平台ip1,平台ip2,平台ip3,平台ip4,平台port}
#'''
  def out_device_setting(self,data):
    print("set device_out network info in setting_node")
    self.Log_pub("Set device_out network info to savefile",INFO)
    setting = settings()
    setting.frame_id = self.deviceId
    setting.type = 3
    setting.which_param = 3002

    #解析IP
    lan_info_list0 = b''
    for val in data:
      lan_info_list0 += struct.pack('<B',val)#将float转化成byte
    lan_info_turple = struct.unpack('<BBBBBBIBBBBBBI',lan_info_list0) #遵循与上位机传输协议格式
    #写IP到文件，保存
    direc = {}
    with open(CONFIGPATH + NETWORKCONFIG, 'r') as file_to_read:
      direc = yaml.load(file_to_read)
      direc['TranfficState'] = lan_info_turple[0]#保存设置
      if lan_info_turple[0] == 1:#如果设置为有链接，或理解成需要链接，便进行设置，如果表示不需要链接，则没有必要设置保存
        direc['TranfficIp'] = str(lan_info_turple[2])+"."+str(lan_info_turple[3])+"."+str(lan_info_turple[4])+"."+str(lan_info_turple[5])#保存信号灯ip
        direc['TranfficPort'] = lan_info_turple[6]#保存信号灯端口
        direc['TranfficType'] = lan_info_turple[1]#保存信号灯的链接方式，分为udp ,tcp_server,tcp_client
      direc['PlatState'] = lan_info_turple[7]

      if lan_info_turple[7] == 1:#如果设置为有链接，或理解成需要链接，便进行设置，如果表示不需要链接，则没有必要设置保存
        direc['PlatIp'] = str(lan_info_turple[9])+"."+str(lan_info_turple[10])+"."+str(lan_info_turple[11])+"."+str(lan_info_turple[12])#平台的ip
        direc['PlatPort'] = lan_info_turple[13]#平台的端口
        direc['PlatType'] = lan_info_turple[8]#平台的链接方式,分为udp ,tcp_server,tcp_client
    with open(CONFIGPATH + NETWORKCONFIG, 'w') as file_to_read:#写文件
      yaml.dump(direc,file_to_read)
      self.logdata = lan_info_turple
      self.Log_pub("Write out device network info to savefile",INFO)

    #热参数设置
    setting.setdatas = list(lan_info_turple)
    self.settingVal_pub_.publish(setting)
    self.logdata = setting.setdatas
    self.Log_pub("Publish network info to ros",INFO)
       

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
      if Type == 0:#道路信息
        if sort == 100:#道路信息设置
          self.lan_info_setting(data)
      elif Type == 1: #统计信息
        if sort == 111: #统计信息设置
          self.statis_lan_setting(data)
        elif sort == 112: #统计周期心思设置
          self.statis_period_setting(data)
      elif Type == 2:#
        pass
      elif Type == 3:#网络信息
        if sort == 300: #自身网络信息设置，ip netgoal,id
          self.network_setting(data)
        elif sort == 301: #与外部网络通讯设置
          self.out_device_setting(data)
      elif Type == 4:#安装标校信息
        if sort == 500:#安装标校信息设置
          self.install_cli_setting(data)
        pass
      elif Type == 5:
        pass
      elif Type == 6:
        pass
      print("topic")
      pass
    except Exception as e:
      print e
      self.Log_pub(str(e),ERROR)  

  #上位机设置信息的回调函数
  def settingSysParams(self,msg):

    print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"
    print "msg.framed_id: ",msg.frame_id
    print "self.deviceId: ",self.deviceId
    print "msg.type: ",msg.type
    print "msg.which_param: ",msg.which_param
    print "msg.setdatas: ",msg.setdatas
    print "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"

    if msg.frame_id != self.deviceId and msg.which_param != 300:

      print "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
      print "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
      print "msg.frame_id: ",msg.frame_id
      print "self.deviceId: ",self.deviceId
      print "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"
      print "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^"

      return
    try:
      self.deal_unpack_setting(msg.type,msg.which_param,msg.setdatas)
    except Exception as e:
      self.Log_pub(str(e),ERROR1)      

  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceId
    systemlog_.node_id = "setting_node"
    systemlog_.type = logtype;
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.systemlog_pub_.publish(systemlog_)
    self.logdata = []
    
if __name__ == '__main__':
  try:
    rospy.init_node('setting_node', anonymous = True)
    settingNode = SettingNode()
    writeLog = WriteLog()
    udpserver = UDPSendInfo()
    systemMon = SystemMon()
    rospy.spin()
    print("end")
  except Exception as e:
    print e

























