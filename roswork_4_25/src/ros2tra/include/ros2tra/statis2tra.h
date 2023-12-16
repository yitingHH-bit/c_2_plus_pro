#ifndef __STATIS2TRA__HEAD__
#define __STATIS2TRA__HEAD__

#include <ros/ros.h>

#include "stddef.h"
#include <tcpInterface.h>
#include <vector>
#include <map>
#include <iostream>
#include "tcpcontrol.h"
#include "stdint.h"

#include <MsgBack.hpp>
#include <recvbuffer.hpp>

#include <all_msgs_srvs/vehicalmsgs.h>
#include <all_msgs_srvs/settings.h>
#include <all_msgs_srvs/systemlog.h>
#include <all_msgs_srvs/errorMsgs.h>
#include <all_msgs_srvs/systemState.h>
#include <all_msgs_srvs/settingcallback.h>


using namespace std;


class TraBridge{
  public:
    TraBridge(ros::NodeHandle *nh);
    ~TraBridge();

  private:
    ros::NodeHandle nh_;//ros句柄
    ros::Subscriber subveh_;//接受车辆实时信息,脉冲传输需要
    ros::Publisher pubset_;//发布设置信息，信号机配置的信息，本节点只负责解析头，分类，将具体的解析任务分配下去给setting_node
    ros::Publisher publog_;//发布log信息

    ros::Subscriber suberr_;//接受错误信息，并上传给信号机
    ros::Subscriber subset_;//接受setting_node发布的热参数配置信息
    ros::Subscriber substatis_;//接受statis_node发送过来的统计信息

    ros::ServiceClient serClient_;//信号机请求配置的信息，本节点发送ros的请求消息，向setting_node的server请求获取

    TcpInterface *tcpInterface_ = NULL;//本机在tcp通讯中接口，负责通讯

    uint8_t recv_buf_[9600];//接受信号机数据的缓冲buff
    RecvBuffer *recvbuffer_;//recv_buf_操作接口
    vector<float> null_ptr;//空指针

    string UpperPcIp_;//作为tcp_client时，server的ip
    int LongPort_; //tcp数据传输，长链接的通讯端口
    int ShortPort_; //tcp数据传输，短链接的通讯端口
    uWord_16 DeviceIp;//本机的设备编码，由编号和方位组成

    bool conn_;//tcp_server创建成功标志
    bool sure_pulse_;//发送统计数据，还是发送脉冲数据
    pthread_mutex_t mutex_;  //线程锁

    vector<float> null_vector_;//log消息的数组
   //上传协议层接口
    MsgBack *magback_;//无数据回应
    ErrBack *errback_;//错误应答
    TimeBack *timeback_;//时间回应
    ParamBack *paramback_;//参数回应
    StatisInfoUp *statisup_;//统计数据主动上传
    HistoryDataBack *historyback_;//历史数据回应
    PulesDataUp *pulesdateup_;//脉冲数据主动上传

    void getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist);//接受车辆脉冲回调函数
    void getSetsCb(const all_msgs_srvs::settingsConstPtr &sets);//接受setting_node热参数配置的回调函数
    void getStatisCb(const all_msgs_srvs::settingsConstPtr &sets);//接受ros统计信息回调函数
    void getErrCb(const all_msgs_srvs::systemStateConstPtr &data);//接受ros系统转台的回调函数
    void getSysMsgsCb(const all_msgs_srvs::errorMsgsConstPtr &errmsgs);//接受ros系统错误消息的回调函数，目前还没有用上
    
    void tcpdataProc(uint8_t *data, unsigned int data_len);//tcp消息，短链接消息接受函数
    void createTcpLink();//创建tcp_server
    void getParams();//从ros中获取参数配置
    void dataUnpack(uint8_t *data, unsigned int data_len);//上位机传过来的消息，进行初步解包分类
    void Log_pub(string logstr,uint32_t type);//发布log消息
    bool Judge_compatibility(uint8_t arg_v);//版本兼容性判断
    unsigned int preUnpack(uint8_t *data,unsigned int data_len);//预处理，将数据中的复用字节还原

    //设置处理
    void deal_online_setting();//处理联机请求
    void deal_time_setting(all_msgs_srvs::settings &ros_set);//对时请求
    void deal_param_setting(all_msgs_srvs::settings &ros_set);//参数设置
    void deal_pulsemode_setting(all_msgs_srvs::settings &ros_set);//脉冲模式设置

    //查询处理
    void deal_online_inquire();//联机查询
    void deal_time_inquire();//时间查询
    void deal_setparam_inquire(all_msgs_srvs::settingcallback &srvdata);//配置参数请求
    void deal_history_inquire(all_msgs_srvs::settingcallback &srvdata);//历史数据请求
};



#endif


