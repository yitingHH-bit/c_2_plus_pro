#!/usr/bin/env python
#coding=utf-8

import rospy
import serial
from all_msgs_srvs.msg import bdinfomsg
from all_msgs_srvs.msg import settings
import time
import thread
import os


class MSerialPort:
    message=''
    def __init__(self,port,buand):
        self.port=serial.Serial(port,buand)
        self.msg_time = '-1:00'
        self.msg_lon = 0
        self.msg_alt = 0
        self.datetime = '00/00/2000'
        self.unpack = bdinfomsg('bdinfo_pub_node',0,1,self.datetime + "-" + self.msg_time,self.msg_lon,self.msg_alt)
        self.param_sub_ = rospy.Subscriber("/ros_param_setting", settings, self.paramset);
        if not self.port.isOpen():
            self.port.open()
    def port_open(self):
        if not self.port.isOpen():
            self.port.open()
            self.port.timeout = 0.5
    def port_close(self):
        self.port.close()
    def send_data(self,data):
        number=self.port.write(data)
        return number
    def read_data(self):
        data = ''
        while True:
            data=self.port.readline()
            self.unpackmsg(data)
    def paramset(self, msg):
      pass

    def unpackmsg(self,msg_pack):
        if msg_pack.find("GNRMC") != -1:
            arr=msg_pack.split(',')
            if arr[1] != '':
                self.msg_time = str(int(arr[1][0:2])+8) + ":" + arr[1][2:4] + ":" + arr[1][4:6]
            if arr[5] != '':
                self.msg_lon = float(arr[5])
                self.msg_alt = float(arr[3])
            if arr[9] != '':
                self.datetime =  arr[9][2:4] +"/"+ arr[9][0:2] + "/20" + arr[9][4:6]
                os.system('echo nvidia | sudo -S date -s '+ self.datetime)
                os.system('echo nvidia | sudo -S date -s ' + self.msg_time)
            self.unpack = bdinfomsg('bdinfo_pub_node',0,1,self.datetime + "-" + self.msg_time,self.msg_lon,self.msg_alt)
        elif msg_pack.find("GNGGA") != -1:
            arr=msg_pack.split(',')
            if arr[1] != '':
                self.msg_time = str(int(arr[1][0:2])+8) + ":" + arr[1][2:4] + ":" + arr[1][4:6]
            if arr[4] != '':
                self.msg_lon = float(arr[4])
                self.msg_alt = float(arr[2])
            self.unpack = bdinfomsg('bdinfo_pub_node',0,1,self.datetime + "-" + self.msg_time,self.msg_lon,self.msg_alt)
        elif msg_pack.find("GNGLL") != -1:
            arr=msg_pack.split(',')
            if arr[5] != '':
                self.msg_time = str(int(arr[5][0:2])+8) + ":" + arr[5][2:4] + ":" + arr[5][4:6]
            if arr[3] != '':
                self.msg_lon = float(arr[3])
                self.msg_alt = float(arr[1])
            self.unpack = bdinfomsg('bdinfo_pub_node',0,1,self.datetime + "-" + self.msg_time,self.msg_lon,self.msg_alt)


class Sendbdinfo:
    def __init__(self):
        rospy.init_node('bdinfo_publisher', anonymous = True)
        self.pub = rospy.Publisher('bdinfo', bdinfomsg, queue_size = 10)
        self.rate = rospy.Rate(10)
        self.deviceID = rospy.get_param("/DeviceID")

    def bdinfo_Publisher(self,msg_pack):
        msg_pack.device_id = self.deviceID
        self.pub.publish(msg_pack)
        self.rate.sleep()

if __name__ == '__main__':
    try:
        if os.path.exists('/dev/ttyUSB0') == False:
            print("/dev/ttyUSB0 not found!")
        else:
            mSerial=MSerialPort('/dev/ttyUSB0',9600)
            thread.start_new_thread(mSerial.read_data,())

            rospy.init_node('bdinfo_pub_node', anonymous = True)
            sendbdinfo=Sendbdinfo()

            while not rospy.is_shutdown():
                time.sleep(1)
                try:
                    sendbdinfo.bdinfo_Publisher(mSerial.unpack)
                    pass
                except Exception as e:
                    print('timeout')
                    print(e)
    except rospy.ROSInterruptException:
        pass


