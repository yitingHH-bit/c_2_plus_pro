#ifndef __DEVSTATUP_HEAD__
#define __DEVSTATUP_HEAD__
#pragma pack(1)

#include "stdint.h"
#include <iostream>
#include <cmath>
#include "tcpcontrol.h"
#include <vector>
#include <pack.hpp>

struct StateData{
    uInt_32 occ_cpu;//CPU
    uInt_32 occ_gpu;//GPU
    uInt_32 core_tem;//温度
    uInt_32 occ_memory;//内存
    uInt_32 space_home;//机身内存
    uInt_32 space_sda1;//外挂内存
    uint8_t pre_sta;//程序状态
    uint8_t cam_sta;//相机状态
    uint8_t lid_sta;//雷达状态

};
using namespace std;
class DevStateUp: public Pack
{
  public:
    DevStateUp(uWord_16* deviceip);
    ~DevStateUp();
    void thispack();
    void addHead();
  private:
    StateData stateData_;
    //vector<StatisData> statisDataList_;
    //vector<StatisData>::iterator vec_it_;//迭代器
    //uint8_t lanNum_;
    uWord_16* deviceIp_;//设备编号
    void addStatisdata();//添加协议具体的数据
};
#endif

