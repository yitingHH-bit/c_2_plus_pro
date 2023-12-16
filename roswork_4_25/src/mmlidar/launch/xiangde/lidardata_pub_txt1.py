#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import json
import socket
import time,datetime
import os
import thread
import random
from mmlidar.msg import vehicalmsgs

HOST= "192.168.203.99"
PORT=8898
BUFFER=9600



def get_time():
    now = time.time()
    local_time = time.localtime(now)
    return time.strftime('%Y-%m-%d %H:%M:%S',local_time)
    #return time.strftime('%Y-%m-%d',local_time)

class Tcp_link:
    def __init__(self):
        rospy.init_node('mmlidar_pub', anonymous = True)
        self.pub = rospy.Publisher('mmlidar_veh', vehicalmsgs, queue_size = 1)
        self.index = 0
        self.seq = 0
        self.rate = rospy.Rate(10)
       
        self.pub_veh = vehicalmsgs()

        #HOST= rospy.get_param("~host")
        #PORT=rospy.get_param("~port")
        #BUFFER=rospy.get_param("~baud")
        print("get socket param successful! host:%s , port:%d, baud:%d" % (HOST,PORT,BUFFER))
        self.host = HOST
        self.port = PORT
        self.host1 = "192.168.2.200"
        self.port1 = 8100
        self.baud = BUFFER

        #if not self.link_tcp():
            #self.__del__()
        self.receive_data()



    def __del__(self):
        #self.soc.close()
        print('析构函数')


    def link_tcp(self):
        try:
            self.soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.soc.connect((self.host,self.port))
            self.soc2up=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.soc2up.settimeout(0.1)
            self.soc2up.connect((self.host1,port1))
            return True
            print("connect successful!")
        except:
            print("connect failure!")
            return False  

    def receive_data(self):
        try:
            i=0
            with open('/home/prototype/org_data.txt', 'r') as file_to_read:
              while not rospy.is_shutdown(): 
                #time.sleep(0.01)
    	        #self.buf=self.soc.recv(self.baud)
                lines = file_to_read.readline() # 整行读取数据
                #print(lines)
                if not lines:
                    break
                    pass
                if lines.find("~~") != -1:
                    i = i+1
                    self.buf = lines
                    #print(self.buf[2:-3])
                    self.rate.sleep()
                    self.deal_data()
                elif lines.find(".") != -1:
                    self.pub_veh.stamp = lines[lines.find(".")-9:lines.find(".")]
              print("file end!")
              print(i)
        except Exception as e:
            print("recv data failure!")
            print(e)

    def vehicle_deal(self, value):
        for dic in value:
            vehlist = []
            for key, it in dic.items():
                if key == 'carType':
                    vehlist.insert(1,float(it))
                elif key == 'traceId':
                    vehlist.insert(0,float(it))
                elif key == 'trackStatus':
                    vehlist.insert(2,float(it))
                elif key == 'vY':
                    vehlist.insert(3,float(it))
                elif key == 'x':
                    vehlist.insert(4,float(it))
                elif key == 'y':
                    vehlist.insert(5,float(it))
            self.add_pubdata(vehlist)
        print("veh_data insert successful!")
        self.pub_data()

    def lans_deal(self, value):
        print('lansdata !do nothing')        
    def flow_deal(self,value):
        print('flowdata !do nothing')

    def deal_data(self):
        try:
    	    self.data = json.loads(self.buf[2:-3])
            #print()
            for key,value in self.data.items():       
                if key == u'deviceIP':
                    self.pub_veh.frame_id = str(value)
                    pass
                else:
                    if key == u'vehicles':
                        self.vehicle_deal(value)
                    if key == u'laneRealTimeData':
                        self.lans_deal(value)
                    if key == u'vehiclesFlow':
                        self.flow_deal(value)
        except Exception as e:
            print("deal data failure")
            print(e)
            return 

    def add_pubdata(self, lists):
        if isinstance(lists,list):
            index = self.index
            for value in lists:
                self.pub_veh.vehicaldatas.insert(index,value)
                index = index + 1
            self.index = self.index + len(lists)

    def pub_data(self):
        #self.pub_veh.stamp= time.time()
        self.pub.publish(self.pub_veh)
        self.pub_veh.vehicaldatas = []
        self.index = 0
        #self.rate.sleep()
        print("veh_data pub successful!")
        

if __name__ == '__main__':
    try:
        tcp_link = Tcp_link()
    except Exception as e:
        print(e)




























