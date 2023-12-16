#ifndef __MSG_HEAD__
#define __MSG_HEAD__
#pragma pack(1)


#include "stdint.h"
//#include <pthread.h>
#include <iostream>
#include <cmath>
#include "tcpcontrol.h"
#include <vector>
#include <pack.hpp>



//配置参数
struct Setting{
 uWord_16 peroid;//统计周期
 uint8_t train_long;//A类车长
 uint8_t bus_long;//B类车长
 uint8_t car_long;//C类车长
 uInt_32 other;//保留 
};
//检测项目
struct DetePro{
 int16_t bit0:2;//详细提供区分类型，0:不区分车型，1:分A,c两种车型，2:分A,B,C三种车型，3:不提供车流量
 int16_t bit1:1;//是否提供车辆占有率
 int16_t bit2:1;//是否提供车辆平均行使速度
 int16_t bit3:1;//是否提供平均车长
 int16_t bit4:1;//是否提供平均车头时距
 int16_t bit5:1;//是否提供排队长度
 int16_t bit6:9;//保留位 
};
//信号机参数查询
struct ParamBack_s{
 uint8_t name_len;//名字长度
 vector<char> name;//公司名称
 uint8_t devicename_len;//设备名称长度
 vector<char> devicename;//设备名称
 uint8_t dete_num;//最大检测通道数
 DetePro detepro;//检测项目
 uint8_t deteway;//检测方法，1:线圈，2:视频，3:雷达，4:其他
 uint8_t signal_delay;//信号输出延时，单位0.01s
 Setting setting;//配置
};

struct SingleSData{//单通道统计数据
    uint8_t lanNum;//检测通道编号
    uint8_t trainCount;//A车流量
    uint8_t busCount;//B车流浪
    uint8_t carCount;//C车流量
    uint8_t averOcc;//平均占有率
    uint8_t averSpeed;//平均车速
    uint8_t averCarLong;//平均车长
    uint8_t averHeadTime;//平均车头时距
    uint8_t averQueueLong;//平均排队长度
    uInt_32 others;//保留其他
};

struct StatisData{
 uInt_32 time_now;//时间
 Setting setdata;//配置参数
 uint8_t deteCount;//统计数据对应的检测通道数
 vector<SingleSData> singledatas;//单路检测统计数据表
};

//历史数据
struct HistoryData{
 uint8_t hisnum;//历史流水号
 StatisData hisdata;//历史数据
};

//脉冲数据
struct PulseData{
 uint8_t dete_num;//检测通道
 uint8_t vehs_dir;//车辆方向，0表示离开检测区，1,表示车辆进入检测区
};

 
using namespace std;
//******************脉冲数据上传***********************//
class PulesDataUp: public Pack
{
  public:
    PulesDataUp(uWord_16* deviceip);
    ~PulesDataUp();
    void thispack();
};
//******************历史统计数据回复***********************//
class HistoryDataBack: public Pack
{
  public:
    HistoryDataBack(uWord_16* deviceip);
    ~HistoryDataBack();
    void thispack();
};

//******************统计参数查询回复***********************//
class StatisInfoUp: public Pack
{
  public:
    StatisInfoUp(uWord_16* deviceip);
    ~StatisInfoUp();
    void thispack();
};

//******************配置参数查询回复***********************//
class ParamBack: public Pack
{
  public:
    ParamBack(uWord_16* deviceip);
    ~ParamBack();
    void thispack();
  private:
    ParamBack_s settinginfo_;
};

//******************时间查询回复***********************//
class TimeBack: public Pack
{
  public:
    TimeBack(uWord_16* deviceip);
    ~TimeBack();
    void thispack();
};
//******************出错应答***********************//
class ErrBack: public Pack
{
  public:
    ErrBack(uWord_16* deviceip);
    ~ErrBack();
    void thispack();
};

//******************无内容消息回复***********************//
class MsgBack: public Pack
{
  public:
    MsgBack(uWord_16* deviceip);
    ~MsgBack();
    void setHead(uint8_t operaType,uint8_t dataType);
    void thispack();
};


#endif

