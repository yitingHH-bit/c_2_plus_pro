文档说明:
1, lidardata_pub.py:雷达接受数据和打包数据,并发送到ROS网络,调节启动参数也能保存历史数据
2, lidardata_pub_txt.py:会放历史数据,打包发布到ROS,但是必须在root用户权限下执行,具有空格键暂停的功能
   如何进入root用户权限:
      $ sudo su
      $ source .bashrc

3, lidardata_pub_txt1.py:会放历史数据,打包发布到ROS ,不需要在root用户权限下执行,但是没有空格键暂停功能
