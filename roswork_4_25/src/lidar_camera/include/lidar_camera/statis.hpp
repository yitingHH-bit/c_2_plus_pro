#ifndef __STATISNODE_HEAD__
#define __STATISNODE_HEAD__
#include <iostream>
//#include <fstream>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <ros/ros.h>

#include <yaml-cpp/yaml.h>
#include <fstream>

//#include <tf/transform_broadcaster.h>
#include <all_msgs_srvs/vehicalmsgs.h>
#include <all_msgs_srvs/systemlog.h>
#include <all_msgs_srvs/settings.h>
#include <all_msgs_srvs/errorMsgs.h>
#include <all_msgs_srvs/systemState.h>

#include "tcpcontrol.h"
#include <queue>
#include <CTimer.hpp>
#include <string>


/*程序解析（非常重要）
1,获取ros参数信息，解析出已经设置的（或者默认的）道路信息
2,将获取到的道路参数信息，转化成为程序中的车道信息相关结构体，具体结构体见下定义
3,ros设定的车辆信息话题回调函数，将一zhen车辆的所有信息，通过简单位置过滤后，接受到map中
4,对map中的没一辆信息进行处理，处理规程如下：
  4.1 针对车辆信息中自带的车道信息--车道号码，从上面生成的道路结构体中选出对应的车道号的车道
  4.2 车流量统计：（1）针对该车道中每一个检测带，判断车辆是否在检测带内
		（2）针对该检测带，判断该检测带中上一zhen是否有车，如果有车，判断是否是同一个id，如果不是同一个id，表示流量+1;如果判断上一zhen没有车，则表示这是新进入的车辆，直接流量+1
		 (3) 如果此次车辆速度大，预测（v * t > 检测带宽）到下一zhen车辆出界，我们就指示下一zhen车辆是新到来的，就不用判断id号了，直接流量+1，雷达zhen数默认为15zhen
  4.3 排队长度计算：（1）去向不计算排队长度
		  （2）大于规定的低速sure_queue_km，不计算排队
		  （3）如果已经在排队，此车在排队头范围内，更新排队头部距离信息，如果在排队尾部一定范围，更新排队尾部，并记录车型
		  （4）如果没有人在排队，车辆在停止线一定范围内，认定为开始排队
		  （5）如果在排队过程中，在排队头部附近，车辆速度大于指定速度（默认为10km/h），就默认为非排队状况，此时意味着道路畅通
		  （6）计算排队长度为 排队尾部 - 排队头部 + 尾部车长
5,定时器周期时间到，对每个车道信息，进行一次汇总，通过ros话题发布，到ros2up_node 中进行协议打包上传到上位机
6,当周期发生修改后，对定时器进行一次重新计时，上报后，对统计数据清零，但是对排队长度不清零

*/

using namespace std; 

struct Coord{
  float x;
  float y;
};//道路的坐标x,y表示车道左近角坐标

struct QueueInfo{
  bool isQue;//是否有排队
  uint8_t jsort;//近车类型
  float jdist;//近车距离
  uint8_t ysort;//远处类型
  float ydist;//远处车距离
  float queue_len;//排队长度 
};//排队信息

struct StatisInfo{
  uint8_t checknum;//检测带的编号，从零开始
  float distance;//检测带的位置
  float checkwidth;//检测带的宽度
  bool isNewcar;//上一次是否有车在,没车为True,有车为False
  uint8_t old_carid;//车的id号码
  float arvSpeed;//平均车速
  float arvCarLong;//平均车长
  int carCount_sum;//总流量
  int carCount_car;//小车流量
  int carCount_bus;//中车流量
  int carCount_tra;//大车流量
  float arvTime;//车头时距
  float arvOcc;//车道占有率
};


struct LanInfo{
  uint32_t lannum;//车道号
  uint32_t lantype;//车道类型
  float lanwidth;//车道宽
  Coord postion;//车道位置
  QueueInfo queue;//排队信息
  vector<StatisInfo> statis;//统计信息
};

class Statis_node
{
  public:
    Statis_node(ros::NodeHandle *nh);
    ~Statis_node();
  private:
    ros::NodeHandle nh_;
    ros::Subscriber subveh_; //接受车的消息
    ros::Subscriber subset_;//接受设置消息
    ros::Publisher publog_;//发布日志
    ros::Publisher pubsta_;//发布统计信息
    ros::Publisher pubstation_;//发布系统状态信息

    void getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist);//收到车辆实时消息的回调
    void getSetCb(const all_msgs_srvs::settingsConstPtr &sets);//收到热参数修改消息的回调
    
    void init_data();//初始化成员变量
    void get_rosparams();//从ros中获取参数
    void init_data1();//进一步初始化

    void get_vehmap(vector<float> vehs);//在线车辆解析
    bool rect_filt(float sort, float state, float vy, float x, float y);//过滤车辆
    void deal_vehs();//计算
    void statis_single(int carid, int type,int lannum,float x,float y,float vy,LanInfo &laninfo);//车道统计
    void statis_queue(int carid, int type,int lannum,float x,float y,float vy,LanInfo &laninfo);//排队长度统计

    void pub_statisdata();//发送统计信息
    void getTimeStamp();//获取时间戳的
    void statis_compute(LanInfo &laninfo);//统计计算
    void statis_clear();//统计清零isNewcar
    void Log_pub(string logstr,uint32_t type);//发布log消息

    string LanInfoPath;

    int frame_id;//设备代号
    float carLong;//小车长度
    float busLong;//大车长度
    float trainLong;//超大车长度

    int lancount;//检测车道的个数
    vector<int> Countlist;

    float statisPeriod;//统计计算的周期,分钟
    float checkbarLong;//检测带的长度
    CTimer *pubTimer=NULL;//定时器
    int lidar_fps;//雷达zhen率

    //统计排队长度需要调整的参数
    float flush_head;//更新排队长度排头的范围，默认+-2.5m
    float flush_tail;//更新排队长度排尾的范围，默认-2.5,+15.0m
    float sure_queue_km;//在停车线附近低速范围停车算排队，默认1.5km/h
    float sure_queue_dis;//在停车线附近范围停车算排队，默认+-5m
    float cancel_queue_km;//取消排队状态的速度,默认10km/h
    float cancel_queue_dis;//取消排队状态的排头范围,默认+-1.0m

    //发布的消息
    all_msgs_srvs::settings statis_data;//发布的统计信息
    time_t data_stamp;//时间变量

    pthread_mutex_t mutex_;  //线程锁
    pthread_mutexattr_t mutexattr_;//init

    vector<float> log_vector_;//日志数据

    vector<LanInfo> laninfo_all;//车道的信息
    vector<LanInfo>::iterator laninfo_allit_;

    map<int,vector<float> > vehmap_;//雷达检测车辆的信息
    map<int, vector<float> >::iterator mapit_;//迭代器

};

#endif



