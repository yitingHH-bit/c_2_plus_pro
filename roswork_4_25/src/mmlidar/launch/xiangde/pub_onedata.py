#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import json
import socket
import time,datetime
import os
import threading
import random
from mmlidar.msg import vehicalmsgs

from evdev import InputDevice
from select import select

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
        self.pub_veh = vehicalmsgs()
        self.rate = rospy.Rate(1)
        while not rospy.is_shutdown():
          self.pub_data()



    def __del__(self):
        print('析构函数')
   

    def pub_data(self):
        self.pub_veh.stamp.secs = 100000
        self.pub_veh.frame_id = '192.168.2.101'
        self.pub_veh.vehicaldatas = [100,0,1,-30,-5,50]
        self.pub.publish(self.pub_veh)
        self.rate.sleep()
        

if __name__ == '__main__':
    try:
        tcp_link = Tcp_link()
    except Exception as e:
        print(e)




























