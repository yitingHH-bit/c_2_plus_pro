#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import json
import socket
import time,datetime
import os
import thread
import random
from all_msgs_srvs.msg import vehicalmsgs

HOST= "127.0.1.1"
PORT=8898

BUFFER=9600
SAVEDATA=True

# HOST= "192.168.8.108"
# PORT=8898
# BUFFER=9600
# SAVEDATA=False

# HOST1= "192.168.2.101"
# PORT1=8080
# BUFFER1=9600



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

        # HOST= rospy.get_param("~host")
        # PORT=rospy.get_param("~port")
        # BUFFER=rospy.get_param("~baud")
        # SAVEDATA=rospy.get_param("~saveSure")
        print("get socket param successful! host:%s , port:%d, baud:%d" % (HOST,PORT,BUFFER))
        self.host = HOST
        self.port = PORT
        self.baud = BUFFER
        self.datasave = SAVEDATA
        print(self.datasave)

        if self.datasave:
            self.file_init()

        if not self.link_tcp():
            self.__del__()
        self.receive_data()



    def file_init(self):
        now = time.time()
        local_time = time.localtime(now)
        #date_filetime =  time.strftime('%Y-%m-%d %H:%M:%S',local_time)
        date_doctime =  time.strftime('%Y-%m-%d',local_time)
        docname= u'/home/hsax/camera_lidar_clibration/' + date_doctime
        org_txtname = u'/org_data_.txt'

        if not os.path.exists(docname):
          os.mkdir(docname)
        self.org_file = open(docname+org_txtname,"a+")
        print("info: org_file has been opened!")

    def __del__(self):
        self.soc.close()
        # self.soc2up.close()

        #self.org_file.close()
        print('析构函数')


    def link_tcp(self):
        try:
            # print("connecting0")
            self.soc=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            self.soc.settimeout(10)
            self.soc.connect((self.host,self.port))
            # self.soc2up=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
            # self.soc2up.settimeout(0.1)
            # self.soc2up.connect(("192.168.3.34",6000))
            return True
            print("mmlidar connect successful!")
        except:
            print("mmlidar connect failure!")
            return False  

    def saveData(self):
        self.org_file.write("TimeNow:\t"+str((time.time()* 1000 - 1589000000000) // 10  ) +'\n')
        #print("TimeNow:\t"+str(time.time()) +'\n')
        self.org_file.write("data:\n"+self.buf+'\n\n')
        #print("=======++++++++")
        self.org_file.flush()

    def receive_data(self):
        try:
            #print(1)
            while not rospy.is_shutdown(): 
                try:
    	            self.buf=self.soc.recv(self.baud)
                    #print("self.buf:===",self.buf)
                    if self.datasave:
                      #print("=====+++++++=++++++++++")
                      self.saveData()
                except Exception as e:
                    # print(e)
                    if str(e) == 'timed out':
                        self.buf = '~~{"deviceIP":"192.168.203.12","vehicles":[{"carType":"0","traceId":"0","trackStatus":"1","vY":"0","x":"-1000","y":"-1000"}]}~~'
                # self.soc2up.send(self.buf) 
                self.deal_data()
        except Exception as e:
            print("recv mmlidar data failure!")
            print(e)

    def vehicle_deal(self, value):
        #check_vehlist= []
        for dic in value:
            vehlist = []
            for key, it in dic.items():
                if key == 'carType':
                    vehlist.insert(1,float(it))
	            #print('1,carType:',float(it))

                elif key == 'traceId':
                    vehlist.insert(0,float(it))
                    #print('0,traceId:',float(it))

                elif key == 'trackStatus':
                    vehlist.insert(2,float(it))
                    #print('2,trackStatus:',float(it))

                elif key == 'vY':
                    vehlist.insert(3,float(it))
                    #print('3,vY:',float(it))

                elif key == 'x':
                    vehlist.insert(4,float(it)+20)
                    #print('4,x:',float(it))

                elif key == 'y':
                    vehlist.insert(5,float(it))
                    #print('5,y:',float(it))

		#check_vehlist.append(vehlist)
            self.add_pubdata(vehlist)
            #print("-+++++++++======================++++++++++++++++======")
            #print("check_vehlist:",check_vehlist)

        #print("veh_data insert successful!")
        self.pub_data()

    def lans_deal(self, value):
        print('mmlidar lansdata !do nothing')        
    def flow_deal(self,value):
        print('mmlidar flowdata !do nothing')

    def deal_data(self):
        try:
    	    self.data = json.loads(self.buf[2:-2])
            #print('-----',self.data)
            for key,value in self.data.items():       
                if key == u'deviceIP':
                    # self.pub_veh.frame_id = str(value)
                    self.pub_veh.frame_id = 1

                    #print(key,':',value)
                    pass
                else:
                    if key == u'vehicles':
                        # print('==================-------------------------')
                        # print(key,'--:--',value)
                        self.vehicle_deal(value)
                        
                        #print('self.pub_veh.stamp:',self.pub_veh.stamp)
                        #print('self.pub_veh.frame_id:',self.pub_veh.frame_id)
	 	 	#print('self.pub_veh.vehicaldatas:',self.pub_veh.vehicaldatas)
			
                    #if key == u'laneRealTimeData':
                        # print(key,':',value)
                        #self.lans_deal(value)
                    #if key == u'vehiclesFlow':
                        # print(key,':',value)
                        #self.flow_deal(value)
        except Exception as e:
            print("deal mmlidar data failure")
            print(e)
            #return 

    def add_pubdata(self, lists):

        if isinstance(lists,list):
            #print('---------------------------------------------++++++')
            #print("lists:",lists)
            index = self.index
            for value in lists:
                self.pub_veh.vehicaldatas.insert(index,value)
                index = index + 1
            self.index = self.index + len(lists)
	    #print('====================================================')
            #print('self.pub_veh.vehicaldatas',self.pub_veh.vehicaldatas)

    def pub_data(self):
        #self.pub_veh.stamp= time.time()
        #print('====================================================-------')
        #print('---lidardata_pub_tcp.py--vehicaldatas:',self.pub_veh.vehicaldatas)
        self.pub.publish(self.pub_veh)
        #print("veh_data pub successful!: ",len(self.pub_veh.vehicaldatas))
        self.pub_veh.vehicaldatas = []
        self.index = 0
        #self.rate.sleep()
        

if __name__ == '__main__':
    try:
        print(0)
        tcp_link = Tcp_link()
    except Exception as e:
        print(e)




























