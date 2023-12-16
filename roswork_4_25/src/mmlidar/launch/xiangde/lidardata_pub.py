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
SAVEDATA=False



def get_time():
    now = time.time()
    local_time = time.localtime(now)
    return time.strftime('%Y-%m-%d %H:%M:%S',local_time)
    #return time.strftime('%Y-%m-%d',local_time)

class Tcp_link:
    def __init__(self):
        rospy.init_node('mmlidar_pub', anonymous = True)
        self.pub = rospy.Publisher('mmlidar_veh', vehicalmsgs, queue_size = 10)
        self.index = 0
        self.seq = 0
        self.buf=''
        self.rate = rospy.Rate(10)
       
        self.pub_veh = vehicalmsgs()

        HOST= rospy.get_param("~host")
        PORT=rospy.get_param("~port")
        BUFFER=rospy.get_param("~baud")
        SAVEDATA=rospy.get_param("~saveSure")
        print("get socket param successful! host:%s , port:%d, baud:%d" % (HOST,PORT,BUFFER))
        self.host = HOST
        self.port = PORT
        self.baud = BUFFER
        self.datasave = SAVEDATA
        print(self.datasave)
        if self.datasave:
            self.file_init()

        while not self.link_tcp() and not rospy.is_shutdown():
            print("waiting server!")
            time.sleep(2)
            #self.__del__()
        self.receive_data()

    def file_init(self):
        now = time.time()
        local_time = time.localtime(now)
        #date_filetime =  time.strftime('%Y-%m-%d %H:%M:%S',local_time)
        date_doctime =  time.strftime('%Y-%m-%d',local_time)
        docname= u'/home/prototype/gui_ws/src/mmlidar/launch/datasave/' + date_doctime
        org_txtname = u'/org_data.txt'

        if not os.path.exists(docname):
          os.mkdir(docname)
        self.org_file = open(docname+org_txtname,"a+")
        print("info: org_file has been opened!")

    def __del__(self):
        self.soc.close()
        self.org_file.close()
        print('析构')


    def link_tcp(self):
        try:
            #print(0)
            self.soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.soc.settimeout(0.1)
            self.soc.connect((self.host,self.port))
            return True
            print("connect successful!")
        except:
            print("connect failure!")
            return False  

    def saveData(self):
        self.org_file.write("TimeNow:\t"+str((time.time()* 1000 - 1589000000000) // 10  ) +'\n')
        self.org_file.write("data:\n"+self.buf+'\n\n')
        self.org_file.flush()

    def receive_data(self):
        try:
            #print(1)
            while not rospy.is_shutdown(): 
                try:
    	            self.buf=self.soc.recv(self.baud)
                    if self.datasave:
                      self.saveData()
                except Exception as e:
                    print(e)
                    if str(e) == 'timed out':
                        self.buf = '~~{"deviceIP":"192.168.203.12","vehicles":[{"carType":"0","traceId":"0","trackStatus":"1","vY":"0","x":"-1000","y":"-1000"}]}~~'
                self.deal_data()
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
    	    self.data = json.loads(self.buf[2:-2])
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
            while not self.link_tcp() and not rospy.is_shutdown():
                print("waiting server!")
                time.sleep(2)
            #return 

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
        print(0)
        tcp_link = Tcp_link()
    except Exception as e:
        print(e)




























