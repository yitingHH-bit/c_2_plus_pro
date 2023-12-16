#ifndef __STATUP_HEAD__
#define __STATUP_HEAD__
#pragma pack(1)

#include "stdint.h"
#include <iostream>
#include <cmath>
#include "tcpcontrol.h"
#include <vector>
#include <pack.hpp>

struct StatisParam{
    uWord_16 statisPeriod; //统计周期
    uint8_t  trainLong;//A车长度
    uint8_t  busLong;//B车长度
    uint8_t  carLong;//C车长度
    uInt_32  others;//保留其他
};

struct StatisData{
    uint8_t lanNum;//车道编号
    uint8_t trainCount;//A车流量
    uint8_t busCount;//B车流浪
    uint8_t carCount;//C车流量
    uint8_t averOcc;//平均占有率
    uint8_t averSpeed;//平均车速
    uint8_t averCarLong;//平均车长
    uint8_t averHeadTime;//平均车头时距
    uint8_t averQueueLong;//平均排队长度

    uint8_t checkNum;

    uInt_32 others;//保留其他
};
using namespace std;
class StatisUp: public Pack
{
  public:
    StatisUp(uWord_16* deviceip);
    ~StatisUp();
    void thispack();
    void addHead();
  private:
    StatisParam statisParam_;
    vector<StatisData> statisDataList_;
    vector<StatisData>::iterator vec_it_;//迭代器
    uint8_t lanNum_;
    uWord_16* deviceIp_;//设备编号
    void addStatisdata();//添加协议具体的数据
};
#endif

