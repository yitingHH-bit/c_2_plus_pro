#ifndef __INQUIRE_HEAD__
#define __INQUIRE_HEAD__
#pragma pack(1)


#include "stdint.h"
//#include <pthread.h>
#include <iostream>
#include <cmath>
#include "tcpcontrol.h"
#include <vector>
#include <pack.hpp>

struct LanBack{//车道查询应答
  uint8_t  lanid;
  uInt_32  lanwidth;
  uInt_32  lanlong;
  uint8_t  lanattr;
  uint8_t  lantype;
  uInt_32  coordx;
  uInt_32  coordy;
  uInt_32  maxspeed;
};

struct Duanmian{//断面信息
  uint8_t duanum;//断面编号
  uint8_t width;//宽度
  uint8_t distance;//距离
};

struct Staits_lan{//统计车道信息
  uint8_t lan_num;//车道号
  uint8_t count_duanmian;//断面数
  vector<Duanmian> info_duanmian;//断面信息
};

struct StatBack{//统计信息查询应答
  uint8_t  trainlong;
  uint8_t  buslong;
  uint8_t  carlong;
  uint8_t  lan_count;
  vector<Staits_lan> info_lan;//道路信息 
};


struct InstallBack{//安装和标校参数查询应答
  uint8_t  num;
  uInt_32  x;
  uInt_32  y;
  uInt_32  z;
  uInt_32  roll;
  uInt_32  pitch;
  uInt_32  yaw;
};
struct DeviBack{//设备IP查询应答
  uWord_16  id;
  uint8_t  ip1;
  uint8_t  ip2;
  uint8_t  ip3;
  uint8_t  ip4;
  uint8_t  goal1;
  uint8_t  goal2;
  uint8_t  goal3;
  uint8_t  goal4;
};
struct OutdeviBack{//外部设备查询应答
  uint8_t  tra_sta;
  uint8_t  tra_comm_type;
  uint8_t  tra_ip1;
  uint8_t  tra_ip2;
  uint8_t  tra_ip3;
  uint8_t  tra_ip4;
  uInt_32   tra_port;
  uint8_t  plat_sta;
  uint8_t  plat_comm_type;
  uint8_t  plat_ip1;
  uint8_t  plat_ip2;
  uint8_t  plat_ip3;
  uint8_t  plat_ip4;
  uInt_32   plat_port;
};

 
using namespace std;
//******************道路信息查询回复***********************//
class LanInfoBack: public Pack
{
  public:
    LanInfoBack(uWord_16* deviceip);
    ~LanInfoBack();
    void thispack();
    void addHead();
  private:
    vector<LanBack> laninfoList_;
    vector<LanBack>::iterator vec_it_;//迭代器
    uint8_t lanallnum_;//车道数
    uWord_16* deviceIp_;//设备编号
    void addLanInfo();//添加车道信息

};

//******************统计参数查询回复***********************//
class StatisInfoBack: public Pack
{
  public:
    StatisInfoBack(uWord_16* deviceip);
    ~StatisInfoBack();
    void thispack();
    void addHead();
  private:
    uWord_16* deviceIp_;//设备编号

};

//******************周期参数查询回复***********************//
class PeriodBack: public Pack
{
  public:
    PeriodBack(uWord_16* deviceip);
    ~PeriodBack();
    void thispack();
    void addHead();
  private:
    uWord_16* deviceIp_;//设备编号

};

//******************安装参数查询回复***********************//
class InstallInfoBack: public Pack
{
  public:
    InstallInfoBack(uWord_16* deviceip);
    ~InstallInfoBack();
    void thispack();
    void addHead();
  private:
    vector<InstallBack> cliinfoList_;
    vector<InstallBack>::iterator vec_it_;//迭代器
    uint8_t lanallnum_;//区域数
    uWord_16* deviceIp_;//设备编号
    void addCliInfo();//添加车道信息

};

//******************本机ip参数查询回复***********************//
class Deviceipback: public Pack
{
  public:
    Deviceipback(uWord_16* deviceip);
    ~Deviceipback();
    void thispack();
    void addHead();
  private:
    DeviBack devicebackinfo_;
    uWord_16* deviceIp_;//设备编号
};

//******************外部设备状态参数查询回复***********************//
class Outdeviceback: public Pack
{
  public:
    Outdeviceback(uWord_16* deviceip);
    ~Outdeviceback();
    void thispack();
    void addHead();
  private:
    OutdeviBack outdeviBack_;
    uWord_16* deviceIp_;//设备编号
};

//******************设置回复***********************//
class SettingBack: public Pack
{
  public:
    SettingBack(uWord_16* deviceip);
    ~SettingBack();
    void setHead(uint8_t operaType,uint8_t dataType);
    void thispack();
    void addHead();
  private:
    uint8_t operaType_;
    uint8_t dataType_;
    uWord_16* deviceIp_;//设备编号
};

//******************在线查询（心跳）回复***********************//
class OnLineQBack: public Pack
{
  public:
    OnLineQBack(uWord_16* deviceip);
    ~OnLineQBack();
    void setHead(uint8_t operaType,uint8_t dataType);
    void thispack();
    void addHead();
  private:
    uint8_t operaType_;
    uint8_t dataType_;
    uWord_16* deviceIp_;//设备编号
};


#endif

