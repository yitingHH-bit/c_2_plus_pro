#ifndef __TCPCOL_HEAD__
#define __TCPCOL_HEAD__
#pragma pack(1)


#include <vector>
#include <bitset>

enum TCPCOL_HEAD{
  UP_HEAD = 0x02,
  SINGLE_HEAD = 0x7E
};
enum UpSortData{
  VEHS_DATAS = 0x00 //车辆信息上传
};

/**
enum CarType{
  CAR = 3,//小车
  BUS = 4,//中车
  TRA = 5//大车
};
**/

enum CarType{
  CAR = 0,//小车
  BUS = 1,//中车
  TRA = 2//大车
};

enum LOGTYPE{
  WARRING = 1, //警告信息
  INFO = 2, //普通输出
  DEBUG = 3,//调试
  ERROR = 10, //致命错误信息
  ERROR1 = 11 //错误信息
};

enum DeviceID{
  DEVICE_00 = 0x00,
  DEVICE_01 = 0x01,
  DEVICE_20 = 0x20,
  DEVICE_21 = 0x21,
  DEVICE_30 = 0x30,
  DEVICE_31 = 0x31,
  DEVICE_40 = 0x40,
  DEVICE_41 = 0x41
};


enum AgrVersion{
  V_10 = 0x10
};

enum ProDirID{
  NORTH = 0x01,
  SOUTH = 0x03,
  EAST = 0x02,
  WEST = 0x04,
  EAST_NORTH = 0x12,
  EAST_SOUTH = 0x23,
  WEST_NORTH = 0x14,
  WEST_SOUTH = 0x34
};

enum OperType{
  OPER_TYPE_REQUEST_INQUIRY = 0x00,
  OPER_TYPE_REQUEST_SET,
  OPER_TYPE_ACTIVE_UPLOAD,
  OPER_TYPE_RESPOND_INQUIRY,
  OPER_TYPE_RESPOND_SET,
  OPER_TYPE_ACTIVE_UPLOADRESPOND,
  OPER_TYPE_RESPOND_ERROR,
  OPER_TYPE_INFO_REQUEST = 0x80,//请求参数配置
  OPER_TYPE_SETTING_INFO = 0x81,//下发配置信息
  OPER_TYPE_ACTIVE_VEHSDATA = 0x82,//主动上传车辆信息
  OPER_TYPE_RESPOND_INFO = 0x83,//信息查询应答
  OPER_TYPE_RESPOND_SETTING = 0x84,//信息查询应答
  OPER_TYPE_ONLINE_REQUEST = 0x85,//在线查询，和应答（心跳）
};

enum ObjTag{
  OBJ_TAG_SET_ONLINE = 0x01, //1
  OBJ_TAG_SET_TIMING ,
  OBJ_TAG_PAR_BAUD ,
  OBJ_TAG_SET_SETPARAM ,
  OBJ_TAG_DATA_STATISDATA ,
  OBJ_TAG_DATA_HISTORYDATA ,
  OBJ_TAG_SET_PULSEUPLOADMODE ,
  OBJ_TAG_DATA_PULSEDATA ,
  OBJ_TAG_DATA_ERRORINFO ,
  OBJ_TAG_DATA_NOWVEHSDATA = 0x50, //80
  OBJ_TAG_DATA_IMG = 0x55,//图像  85
  OBJ_TAG_DATA_SETTING_LAN = 0x56,//车道配置  86
  OBJ_TAG_DATA_SETTING_STATIS = 0x57,//统计   87
  OBJ_TAG_DATA_SETTING_STATIS_PEROID = 0x58,//统计周期 88
  OBJ_TAG_DATA_SETTING_INSTALL = 0x59, //安装标校准参数 89
  OBJ_TAG_DATA_SETTING_DEVICE_IP = 0x60, //设备ip,id 96
  OBJ_TAG_DATA_SETTING_OUT_DEVICE_IP = 0x61 ,//外部设备(信号机及平台)网络信息 97
  OBJ_TAG_DATA_DEVICE_STATION = 0x62 ,//设备自身状态信息  98
  OBJ_TAG_DATA_INFO_UDP = 0x65 ,//udp上传的标志 101
  OBJ_TAG_DATA_ONLINE = 0x87 ,//心跳在线查询标志  135
  OBJ_TAG_DATA_ONLINE_RES = 0x88 ,//心跳在线查询回复 136
};

enum LanType {
  LAN_TYPE_CHANNEL = 0x00,
  LAN_TYPE_COME,
  LAN_TYPE_OUT,
  LAN_TYPE_GO
};

enum VehicalType {
  VEH_TYPE_UNKNOWN = 0x00,
  VEH_TYPE_PEOPLE,
  VEH_TYPE_NONVEH,
  VEH_TYPE_CAR,
  VEH_TYPE_BUS,
  VEH_TYPE_TRAIN
};
enum SettingType{
  LAN_INFO = 0x00000000,//道路信息
  STATIS_INFO,//1统计信息
  TRANSFORM_INFO,//2 
  NETWORK_INFO,//3网络信息
  INSTALL_INFO,//4安装信息
  REQUEST_PERIOD,//5统计数据上报周期
  REQUEST_DEV_NET//6,外部设备的通讯,信号机和平台
};
enum ROSParamType{
  LAN_INFO_UP = 10,//道路配置信息(up)
  STATIS_UP = 11,//统计数据上传(up)
  LAN_INFO_SETTING = 100,//道路配置信息
  STATIS_LAN_SETTING = 111,//配置统计车道信息
  STATIS_PERIOD_SETTING = 112,//配置统计周期
  DEVICE_IP_SETTING = 300,//配置设备ip
  OUT_DEVICE_IP_SETTING = 301,//配置外部设备ip
  INSTALL_CLI_SETTING = 500,//配置统计周期

  LAN_SET_COORD_EVERY = 1001,//道路数据热设置,坐标设置
  STATIS_SET_OTHER_EVERY = 1101,//统计数据热设置,除周期外
  STATIS_SET_PERIOD_EVERY = 1102,//统计数据热设置,周期
  NETWORK_ID_INFO_EVERY = 3001,//设备编号ID热设置
  OUTDEV_INFO_INFO_EVERY = 3002,//外部设备ip和网络的状态的修改
  INSTALL_SET_INFO_EVERY = 5000,//安装参数热修改,安装参数
  INSTALL_SET_CLI_EVERY = 5001//校准参数热修改,校准

};

struct Word_s{
  uint8_t byte0;
  uint8_t byte1;
};

typedef union word{
    int16_t words;
    uint16_t uwords;
    Word_s  word_s;
}uWord_16;

struct uInt_s{
  uWord_16 word0;
  uWord_16 word1;
};

typedef union Int{
    uint32_t uInt32;
    int32_t  Int32;
    float    Float32;
    uInt_s    union_32;
}uInt_32;

struct Double_s{
  uInt_32 Int0;
  uInt_32 Int1;
};

typedef union Double{
    uint64_t uInt64;
    double  Double64;
    Double_s union_64;
}uDouble_64;

#pragma pack()

#endif


