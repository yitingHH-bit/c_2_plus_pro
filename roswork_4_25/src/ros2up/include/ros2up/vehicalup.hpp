#ifndef __VEHSUP1_HEAD__
#define __VEHSUP1_HEAD__
#pragma pack(1)


#include "stdint.h"
//#include <pthread.h>
#include <iostream>
#include <cmath>
#include "tcpcontrol.h"
#include <vector>
#include <pack.hpp>

struct coordinate{
  uInt_32 x;
  uInt_32 y;
};

struct VehInfo{
  uWord_16 id;
  uint8_t  lanNum;
  uint8_t  lanType;
  uint8_t  vehType;
  coordinate coord;
  uInt_32 speedY;
};

typedef union vehs{
  VehInfo vehdata;
  uint8_t out[17];
}VehsDatas;
 
using namespace std;
class VehicalUp: public Pack
{
  public:
    VehicalUp(uWord_16* deviceip);
    ~VehicalUp();
    void thispack();
    void addHead();
  private:
    //vector<VehsDatas> vehsDataList_;
    //vector<VehsDatas>::iterator vec_it_;//迭代器
    vector<VehInfo> vehsDataList_;
    vector<VehInfo>::iterator vec_it_;//迭代器
    uWord_16* deviceIp_;//设备编号
    void addVehsdata();//添加协议具体的数据
    uint8_t getLanType(float yspeed);//获取道路信息,感觉多余
};

#endif

