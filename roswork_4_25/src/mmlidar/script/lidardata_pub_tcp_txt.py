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

#DATA_PATH = '/home/hsax/roswork/src/mmlidar/datahistory/2021-04-01/org_data_.txt'

DATA_PATH = '/home/hsax/camera_lidar_clibration/4/2021-04-07/org_data_.txt'

class Read_mmlidar_txt:
    def __init__(self):
        rospy.init_node('mmlidar_pub', anonymous = True)
        self.pub = rospy.Publisher('mmlidar_veh', vehicalmsgs, queue_size = 1)
        self.index = 0
        self.seq = 0

        self.rate = rospy.Rate(15)
        
        self.pub_veh = vehicalmsgs()

        # self.lines 
        self.read_txt()

        self.receive_data_from_txt()

    def read_txt(self):
        with open(DATA_PATH, 'r') as file_to_read:
            self.lines = file_to_read.readlines()

    def receive_data_from_txt(self):
        gap = 0
        j = 2
        num = 1

        while not rospy.is_shutdown(): 
            self.rate.sleep()
            self.dict_data = {}

            time = self.lines[j+gap*num - 2]
            raw_data = self.lines[j+gap*num]

            if j+gap*num >= (len(self.lines)-2):
                print("------------------------------------------------------------------------------")
                print("------------------------------------------------------------------------------")
                print("------------------------------------------------------------------------------")
                print("------------------------------------------------------------------------------")
                num = 1
            gap = 4
            num += 1 

            dict_str_data_split = raw_data.strip().split('~~')

            dic_index = 0
            if dict_str_data_split[dic_index] == '':
                dic_index += 1
                dict_str_data = dict_str_data_split[dic_index]
                
            # dict_str_data = raw_data.replace("~~",'')
            # print(j+gap*num)
            # print('--====',dict_str_data)
            self.dict_data  = eval(dict_str_data)

            self.deal_data()

    def vehicle_deal(self, value):
        #print("#####################################################")
        #print("len(value)",len(value))
        #print(value)
        for dic in value:

            vehlist = [float(dic['traceId']),float(dic['carType']),float(dic['trackStatus']),float(dic['vY']),float(dic['x']),float(dic['y'])]
            # print('---------------------------------------------')
            # print(vehlist)

            self.add_pubdata(vehlist)

        # print("veh_data insert successful!")
        self.pub_data()

    def lans_deal(self, value):
        print('lansdata !do nothing')        
    def flow_deal(self,value):
        print('flowdata !do nothing')

    def deal_data(self):
        try:
    	    # self.data = json.loads(self.buf[2:-2])
         #    # print('-----',self.data)
            for key,value in self.dict_data.items():       
                if key == u'deviceIP':
                    # self.pub_veh.frame_id = str(value)
                    self.pub_veh.frame_id = 1

                    # print(key,':',value)
                    pass
                else:
                    if key == u'vehicles':
                        #print('==================-------------------------')
                        #print(key,'--:--',value)
                        self.vehicle_deal(value)
                   # if key == u'laneRealTimeData':
                        # print(key,':',value)
                       # self.lans_deal(value)
                    #if key == u'vehiclesFlow':
                    #    # print(key,':',value)
                        #self.flow_deal(value)
        except Exception as e:
            print("deal data failure")
            print(e)
            #return 

    def add_pubdata(self, lists):

        #self.pub_veh.vehicaldatas = []
        #self.index = 0
        # pri
        if isinstance(lists,list):
            index = self.index
            for value in lists:
                self.pub_veh.vehicaldatas.insert(index,value)
                index = index + 1
            self.index = self.index + len(lists)

            #print('self.index:',self.index)

    def pub_data(self):
        #self.pub_veh.stamp= time.time()
        # print('----')
        # print('vehicaldatas:',len(self.pub_veh.vehicaldatas))
        self.pub.publish(self.pub_veh)
        self.pub_veh.vehicaldatas = []
        self.index = 0
        #self.rate.sleep()
        # print("veh_data pub successful!")

if __name__ == '__main__':

    read_mmlidar = Read_mmlidar_txt()




























