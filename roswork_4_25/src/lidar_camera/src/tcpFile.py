#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
过程：
1,接受客户端链接，接受名称
2,回复0x01
3,接受大小
4,回复0x02
5,按大小接受文件，每次接受2048字节
6,修改可执行权限
'''


import os
import rospy
import struct
import tqdm
import fcntl
import socket
import time
from all_msgs_srvs.msg import systemlog
from all_msgs_srvs.msg import settings


ERROR = 0
WARRING = 1
SYSINFO = 2

HOST = ''
PORT = 4567 #udp端口号码
BUFSIZ = 1024 #接受一次信息的数据量
ADDR = (HOST, PORT) #本机服务器的地址和端口


class TcpFile:
  def __init__(self):
    self.init_data() #数据初始化
    self.systemlog_pub_ = rospy.Publisher("/systemLog_info", systemlog, queue_size = 10) #日志发布
    self.setting_sub_ = rospy.Subscriber('/ros_param_setting', settings, self.getsetParamCb)#id接受
    self.recvFile()
    #thread.start_new_thread(self.sendinfo,())#开线程监听udp消息，并回复
  
  def init_data(self):
    self.deviceID = rospy.get_param("/DeviceID")#从ros中获取id
    self.SerSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)#创建tcp
    self.SerSock.bind(ADDR)#bing
    self.logdata = []

  def recvFile(self):
    while not rospy.is_shutdown():
      #print('waiting for file message...')
      try:
        self.SerSock.listen(1)
        conn, addr = self.SerSock.accept()
        file_name = conn.recv(1024)#接受文件名称
        print file_name
        conn.send(b'0x1')#回应信号
        self.download(file_name,conn)
        conn.close()
      except Exception as e:
        self.Log_pub(str(e),SYSINFO)

  def download(self,file_name,file_socket):
    #print(file_socket.recv(1024))
    file_size = struct.unpack('q',file_socket.recv(1024))[0]#接受大小
    if file_size < 1024:
       print('文件大小：%s B' % file_size)
    elif file_size < 1024*1024 and file_size >= 1024:
        print('文件大小：%s KB' % (file_size/1024))
    else:
       print('文件大小：%s MB' % (file_size/(1024*1024)))
    f = open(self.openfile(file_name),'wb')
    print('开始传输...')
    download_size = 2048
    file_socket.send(b'0x12')
    print "00"
    start = time.time()
    for i in tqdm.tqdm(range(int(file_size/download_size) + 1)):
        data = file_socket.recv(download_size)#接受正文
        f.write(data)
    end = time.time()
    f.close()
    print('传输完成！')
    print('大约耗时' + str(end-start)+ '秒')
    os.system('chmod +x ' + self.openfile(file_name))

  def openfile(self,filename):
    if(filename.find("pubimage")!=-1):#pubimage
      return "/home/hsax/gui_ws/devel/lib/csicamera/" + filename
    elif(filename.find("statis_node")!=-1):#statis_node
      return "/home/hsax/gui_ws/devel/lib/lidar_camera/" + filename
    elif(filename.find("ros2up")!=-1):#ros2up
      return "/home/hsax/gui_ws/devel/lib/ros2up/" + filename
    elif(filename.find("Time.py")!=-1):#Time
      return "/home/hsax/gui_ws/src/lidar_camera/bdinfo/script/" + filename
    elif(filename.find("cslidar_pub.py")!=-1):#cslidar_pub
      return "/home/hsax/gui_ws/src/lidar_camera/mmlidar/launch/chuansu/" + filename
    else: 
      return "/home/hsax/gui_ws/src/lidar_camera/lidar_camera/src/" + filename

  #获取ip
  def get_ip_address(ifname):
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    return socket.inet_ntoa(fcntl.ioctl(
        s.fileno(),
        0x8915,  # SIOCGIFADDR
        struct.pack('256s', ifname[:15])
    )[20:24])

  #发送log
  def Log_pub(self, str_describe,logtype):
    systemlog_ = systemlog()
    systemlog_.device_id = self.deviceID
    systemlog_.node_id = "tcp_file"
    systemlog_.type = logtype;
    systemlog_.describe = str_describe
    systemlog_.data = self.logdata
    self.systemlog_pub_.publish(systemlog_)
    self.logdata = []
  
  #修改id
  def getsetParamCb(self,data):
    if (data.frame_id == self.deviceID) and (data.type == 3) and (data.which_param == 3001):#雷达ID
      self.deviceID = int(data.setdatas[0])

  def __del__(self):
    self.SerSock.close()#释放
    print("Log_node end!")

if __name__ == '__main__':
  try:
    rospy.init_node('recv_file', anonymous = True)
    recvfile = TcpFile()
    rospy.spin()
    print("end")
  except Exception as e:
    print e
 

























