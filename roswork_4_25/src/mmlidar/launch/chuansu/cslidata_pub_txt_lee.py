#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from all_msgs_srvs.msg import vehicalmsgs

import os
import numpy as np 
import numpy.ma as ma
import json

import time,datetime


def read_radar_txt_data(org_data_txt):

  save_json = False

  doc_data = {'TimeNow':[],
        'data':[]}

  file = open(org_data_txt)
  raw_data = file.read().splitlines()
  for i in range(len(raw_data)):
    # print(raw_data[i].split(':'))
    if raw_data[i].split(':')[0] == 'TimeNow':
      doc_data['TimeNow'].append(raw_data[i].split(':')[1])

    elif raw_data[i].split(':')[0] == 'data':
      i += 1
      doc_data['data'].append(raw_data[i])

  if save_json:
    json_file = open('radar_raw_data.json','w')
    json.dump(doc_data,json_file,indent=1)
    json_file.close()

  return doc_data


def txt2list(data_):
  datalist = data_[1:-1].split(',')

  data = []
  for i in datalist:
    data.append(float(i))

  return data


if __name__ == '__main__':

  DATA_PATH = '../../datahistory/org_data.txt'
  doc_data = read_radar_txt_data(DATA_PATH)
  radar_list = []


  rospy.init_node("mmlidar_pub_lee")

  pub = rospy.Publisher('mmlidar_veh', vehicalmsgs, queue_size = 1)

  rate = rospy.Rate(15)

  vehicalmsgs = vehicalmsgs()

  while not rospy.is_shutdown():

    for i in range(len(doc_data['TimeNow'])):

      time_ = float(doc_data['TimeNow'][i])
      data_ = doc_data['data'][i]

      radar_data_list = txt2list(data_)


      print("time_:",int(time_))
      print('radar_data_list:',len(radar_data_list),'\n')
      vehicalmsgs.stamp.secs = int(time_)
      vehicalmsgs.frame_id = 1
      vehicalmsgs.vehicaldatas = radar_data_list

      pub.publish(vehicalmsgs)

      rate.sleep()

      # break
    break


















