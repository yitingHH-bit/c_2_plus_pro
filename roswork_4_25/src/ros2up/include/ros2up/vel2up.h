#ifndef VEL2UP__HEAD__
#define VEL2UP__HEAD__

#include <ros/ros.h>

#include <tcplink.h>
#include <server.h>
#include <vector>
#include <map>
#include <iostream>
#include "tcpcontrol.h"
#include <vehicalup.hpp>
#include <statistics.hpp>
#include <device_state.hpp>
#include <inquireBack.hpp>
#include <recvbuffer.hpp>

#include <all_msgs_srvs/vehicalmsgs.h>
#include <all_msgs_srvs/settings.h>
#include <all_msgs_srvs/systemlog.h>
#include <all_msgs_srvs/errorMsgs.h>
#include <all_msgs_srvs/systemState.h>
#include <all_msgs_srvs/settingcallback.h>


namespace ros2up{

using namespace std;


class ArgBridge{
  public:
    ArgBridge(ros::NodeHandle *nh);
    ~ArgBridge();

  private:
    ros::NodeHandle nh_;//ros句柄
    ros::Subscriber subveh_;//接受车辆实时信息
    ros::Publisher pubset_;//发布设置信息，上位机配置的信息，本节点只负责解析头，分类，将具体的解析任务分配下去给setting_node
    ros::Publisher publog_;//发布log信息
    ros::Subscriber subset_;//接受setting_node发布的热参数配置信息
    ros::Subscriber suberr_;//接受错误信息，本节点没有使用
    ros::Subscriber substatis_;//接受statis_node发送过来的统计信息
    ros::Subscriber substate_;//接受系统监控节点发送过来的系统状态信息
    ros::ServiceClient serClient_;//上位机请求配置的信息，本节点发送ros的请求消息，向setting_node的server请求获取
    //Tcplink tcplink_;//当本机在tcp通讯中作为client通讯端时的通讯变量，此处用不上
    Server server_;//本机在tcp通讯中作为server端通讯的所有接口类变量，负责通讯

    uint8_t recv_buf_[9600];//接受上位机数据的缓冲buff

    string UpperPcIp_;//作为tcp_client时，server的ip，此处用不上
    int LongPort_; //tcp数据传输，长链接的通讯端口
    int ShortPort_; //tcp数据传输，短链接的通讯端口
    uWord_16 DeviceIp;//本机的设备编码，由编号和方位组成

    bool conn_;//tcp_server创建成功标志
    bool sure_sendVehs_;//是否发送车辆实时信息的标志
    pthread_mutex_t mutex_;  //线程锁

    //int get_vehs_data_times_ = 0;
    vector<float> null_vector_;//log消息的数组
   //上传协议层接口
    RecvBuffer *recvbuffer_;//接受tcp消息的缓冲buff类
    VehicalUp  *vehicalUp_;//车辆实时信息上传打包类
    StatisUp  *statisUp_;//统计信息上传打包类
    LanInfoBack *laninfoback_;//车道信息查询反馈信息打包类
    StatisInfoBack *statisinfoback_;//统计断面设置查询反馈信息打包类
    PeriodBack *periodback_;//周期信息查询反馈信息打包类
    InstallInfoBack *installinfoback_;//安装和标校查询反馈信息打包类
    Deviceipback *deviceipback_;//设备ip信息查询反馈打包类
    SettingBack *settingback_;//设置应答打包
    DevStateUp *devStateUp_;//设备状态信息上传信息打包类
    Outdeviceback *outdeviceback_;//外部设备网络信息查询反馈信息打包类
    OnLineQBack *onlineQback_;//心跳机制，在线查询接口

    void getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist);//接受ros车辆实时信息回调函数
    void getSetsCb(const all_msgs_srvs::settingsConstPtr &sets);//接受setting_node热参数配置的回调函数
    void getStatisCb(const all_msgs_srvs::settingsConstPtr &sets);//接受ros统计信息回调函数
    void getStateCb(const all_msgs_srvs::systemStateConstPtr &data);//接受ros系统转台的回调函数
    void getSysMsgsCb(const all_msgs_srvs::errorMsgsConstPtr &errmsgs);//接受ros系统错误消息的回调函数，目前还没有用上
    
    void tcpdataProc(uint8_t *data, unsigned int data_len);//tcp消息，短链接消息接受函数
    void createTcpServer();//创建tcp_server
    void getParams();//从ros中获取参数配置
    void dataUnpack(uint8_t *data, unsigned int data_len);//上位机传过来的消息，进行初步解包分类
    void request_back(uint8_t oper_type, uint8_t obj_tag, vector<float> &data);//上传应答数据
    void Log_pub(string logstr,uint32_t type);//发布log消息
};


}

#endif


