#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''程序解析，非常重要
1,获取ros参数，和初始化udp网络设置后，开启一个线程监听udp广播
2,接受udp广播内容解析，是否为获取信息协议的内容
3,如果满足协议，返回ip,mac,id等信息
4,打包发送

'''

import json
import os
import rospy
import thread
import uuid
import fcntl
import struct
import socket
import netifaces
from all_msgs_srvs.msg import systemlog
from all_msgs_srvs.msg import settings


ERROR = 0
WARRING = 1
SYSINFO = 2

ETHNETNAME = 'eth0'

UDPHEAD = b'\x02'  #协议头
UDPOPER = b'\x83'  #协议操作类型
UDPTAG =  b'\x64'  #操作表示,请求
UDPTAG_R =  b'\x65'  #操作表示,回复

DARALEN = 23  #2+4+4+4+6

HOST = ''
PORT = 8888 #udp端口号码
BUFSIZ = 1024 #接受一次信息的数据量
ADDR = (HOST, PORT) #本机服务器的地址和端口




class UDPSendInfo:
  def __init__(self):
    self.init_data() #数据初始化
    self.systemlog_pub_ = rospy.Publisher("/systemLog_info", systemlog, queue_size = 10) #日志发布
    self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)#id接受
    thread.start_new_thread(self.sendinfo,())#开线程监听udp消息，并回复
  
  def init_data(self):
    self.deviceID = rospy.get_param("/DeviceID")#从ros中获取id
    self.udpSerSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)#创建udp
    self.udpSerSock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
    self.udpSerSock.bind(ADDR)#bing
    self.logdata = []
    # self.video_stream_password = [12,34,56]
    self.video_stream_password = rospy.get_param("/video_stream_password")

    # rospy.set_param("/video_stream_password",[12,34,89])


  def sendinfo(self):
    while not rospy.is_shutdown():
      print('waiting for message...')
      try:
        data, addr = self.udpSerSock.recvfrom(BUFSIZ)#等待接收udp数据
        #print data
        if(self.bool_unpack_required(data)):#如果收到的是b'0x01',发送ip ,mac, addr
          content = self.pack_info()
          #json_string = json.dumps(content)#转化为json
          self.udpSerSock.sendto(content, addr)#发送
      except Exception as e:
        print e
        self.Log_pub(str(e),SYSINFO)

  #获取发送内容
  def pack_info(self):
    list_data0 = b''
    list_data0 +=UDPHEAD
    list_data0 +=UDPOPER
    list_data0 +=UDPTAG_R

    list_data0 +=struct.pack("<B",DARALEN)
    # print('("<B",DARALEN):',DARALEN)
    print('list_data0',list_data0)

    list_data0 +=struct.pack("<B",DARALEN>>8)
    # print('("<B",DARALEN>>8):',DARALEN>>8)
    print('list_data0',list_data0)

    list_data0 +=struct.pack("<B",DARALEN>>16)
    # print('("<B",DARALEN>>16):',DARALEN>>16)
    print('list_data0',list_data0)
    
    list_data0 +=struct.pack("<B",DARALEN>>24)
    # print('("<B",DARALEN>>24):',DARALEN>>24)
    print('list_data0',list_data0)

    try:
      list_data0 += self.get_deviceid()#id
      print('self.get_deviceid()',self.get_deviceid())
      print('list_data0',list_data0)
    except Exception as e:
      print e
      list_data0 += b'00'

    try:
      list_data0 += self.get_ip_address(ETHNETNAME)#ip
      print('self.get_ip_address(ETHNETNAME)',self.get_ip_address(ETHNETNAME))
      print('list_data0',list_data0)
    except Exception as e:
      print e
      list_data0 += b'0000'

    try:
      list_data0 += self.get_gateways_address() #gateway
      print('self.get_gateways_address()',self.get_gateways_address())
      print('list_data0',list_data0)
    except Exception as e:
      print e
      list_data0 += b'0000'

    try:
      list_data0 += self.get_netmask_address(ETHNETNAME) #netmask
      print('self.get_netmask_address(ETHNETNAME)',self.get_netmask_address(ETHNETNAME))
      print('list_data0',list_data0)
    except Exception as e:
      print e
      list_data0 += b'0000'

    try:
      list_data0 += self.get_mac_address(ETHNETNAME) #mac
      print('self.get_mac_address(ETHNETNAME)',self.get_mac_address(ETHNETNAME))
      print('list_data0',list_data0)
    except Exception as e:
      print e
      list_data0 += b'000000'

    # user-password
    try:
      list_data0 += self.get_user_password()
      print('self.get_user_password()',self.get_user_password())
      print('list_data0',list_data0)
    except Exception as e:
      print e
      print 'get_user_password error'
      list_data0 += b'000'

    # # user-password

    list_data0 +=UDPHEAD
    print len(list_data0)
    return list_data0
    

  #获取设备id
  def get_deviceid(self):
    id_ = self.deviceID
    return struct.pack("<B",int(id_%256)) + struct.pack("<B",int(id_/256))

  #get password
  def get_user_password(self):
    ret = ''
    
    for val in self.video_stream_password:
      ret += chr(int(val))
    return ret 


  #获取mac
  def get_mac_address(self,ifname): 
    ret = ''
    list_mac = netifaces.ifaddresses(ifname)[netifaces.AF_LINK][0]['addr'].split(":")
    for val in list_mac:
      ret += val
    ret = bytearray.fromhex(ret)
    return ret

  #解析广播内容
  def bool_unpack_required(self,data):
    ret = False
    data_list = list(struct.unpack('<BBBB',data))
    print data_list
    if(data_list[0] == ord(UDPHEAD) and data_list[1] == ord(UDPOPER) and data_list[2] == ord(UDPTAG) and data_list[3] == ord(UDPHEAD)):#判断标准
      ret = True
    #print ret
    return ret

  #获取默认网关
  def get_gateways_address(self):
    ret = b''
    list_gatways = netifaces.gateways()['default'][netifaces.AF_INET][0].split(".")
    for val in list_gatways:
      ret +=chr(int(val))
    return ret

  #获取ip
  def get_ip_address(self,ifname):
    ret = b''
    list_ip = netifaces.ifaddresses(ifname)[netifaces.AF_INET][0]['addr'].split(".")
    print("_________________________list_ip_________________",list_ip)
    for val in list_ip:
      ret += chr(int(val))
    # print("ret:",ret)
    return ret

  #获取netmask
  def get_netmask_address(self,ifname):
    ret = b''
    list_netmask = netifaces.ifaddresses(ifname)[netifaces.AF_INET][0]['netmask'].split(".")
    for val in list_netmask:
      ret += chr(int(val))
    return ret

  #发送log
  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceID
    systemlog_.node_id = "udp_node"
    systemlog_.type = logtype;
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.systemlog_pub_.publish(systemlog_)
    self.logdata = []
  
  #接受系统热参数修改，修改id
  def getsetParamCb(self,data):
    if (data.frame_id == self.deviceID) and (data.type == 3) and (data.which_param == 3001):#雷视ID
      self.deviceID = int(data.setdatas[0])

  def __del__(self):
    print("Log_node end!")

if __name__ == '__main__':
  try:
    rospy.init_node('udp', anonymous = True)
    udp = UDPSendInfo()
    rospy.spin()
    print("end")
  except Exception as e:
    print e
 

























