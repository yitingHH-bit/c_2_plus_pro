#ifndef __TCPCOL_HEAD__
#define __TCPCOL_HEAD__
#pragma pack(1)


#include <vector>
#include <bitset>

enum TCPCOL_HEAD{
  UP_HEAD = 0x7E,
};


enum LOGTYPE{
  WARRING = 1, //警告信息
  INFO = 2, //普通输出
  DEBUG = 3,//调试
  ERROR = 10, //致命错误信息
  ERROR1 = 11 //错误信息
};

enum DeviceID{//链路地址
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
  OPER_TYPE_TRA_INFO_REQUEST = 0x80,//信号机查询消息
  OPER_TYPE_TRA_SETTING_INFO = 0x81,//信号机发送设置消息
  OPER_TYPE_DET_ACTIVE_VEHSDATA = 0x82,//检测器主动上传信息
  OPER_TYPE_DET_RESPOND_INFO = 0x83,//检测器查询应答
  OPER_TYPE_DET_RESPOND_SETTING = 0x84,//检测器对设置请求的应答
  OPER_TYPE_TRA_RESPOND_ACTIVE = 0x85,//信号机对主动上传应答
  OPER_TYPE_RESPOND_ERROR = 0x86,//数据包错误应答
};

enum ObjTag{
  OBJ_TAG_SET_ONLINE = 0x01,//连机
  OBJ_TAG_SET_TIMING = 0x02,//对时
  OBJ_TAG_PAR_BAUD = 0x03,//通讯波特率
  OBJ_TAG_SET_SETPARAM = 0x04,//检测器的配置参数
  OBJ_TAG_DATA_STATISDATA = 0x05,//车辆统计数据
  OBJ_TAG_DATA_HISTORYDATA = 0x06,//检测器中保留的历史统计数据
  OBJ_TAG_SET_PULSEUPLOADMODE = 0x07,//实时脉冲数据上传使能设置
  OBJ_TAG_DATA_PULSEDATA = 0x08,//实时脉冲数据
  OBJ_TAG_DATA_ERRORINFO = 0x09,//检测器故障
};

enum ERRORTYPE {
  CRC_ERR = 0x01,//校验码错误
  VISION_ERR = 0x02,//协议版本不兼容
  DATATYPE_ERR = 0x03,//消息类型错误
  DATAINFO_ERR = 0x04,//消息类型中数据无效
  UNKNOWN_ERR = 0x05,//未知消息
};

enum VehicalType {
  VEH_TYPE_UNKNOWN = 0x00,
  VEH_TYPE_PEOPLE,
  VEH_TYPE_NONVEH,
  VEH_TYPE_CAR,//
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
  REQUEST_DEV_NET,//6,外部设备的通讯,信号机和平台
//-------------上方是上位机查询命令，下面是信号机查询命令-------------------//
  SETTING_PARAM_INFO,//7,信号机配置参数
  REQUEST_HISTORY_DATA,//8,信号机请求历史数据
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
  TRA_PARAM_INFO_SETTING = 700,//信号机配置一般参数
  TRA_PULSE_INFO_SETTING = 701,//信号机配置脉冲参数
  TRA_TIME_INFO_SETTING = 702,//信号机对时操作

  LAN_SET_COORD_EVERY = 1001,//道路数据热设置,坐标设置
  STATIS_SET_OTHER_EVERY = 1101,//统计数据热设置,除周期外
  STATIS_SET_PERIOD_EVERY = 1102,//统计数据热设置,周期
  NETWORK_ID_INFO_EVERY = 3001,//设备编号ID热设置
  OUTDEV_INFO_INFO_EVERY = 3002,//外部设备ip和网络的状态的修改
  INSTALL_SET_INFO_EVERY = 5000,//安装参数热修改,安装参数
  INSTALL_SET_CLI_EVERY = 5001,//校准参数热修改,校准
  TRA_PARAM_INFO_EVERY = 7000,//信号机配置参数热修改
  TRA_PULSE_INFO_EVERY = 7001,//信号机配置脉冲参数热修改



};

struct Word_s{
  uint8_t byte0;
  uint8_t byte1;
};

typedef union word{
    int16_t words;
    uint16_t uwords;
    Word_s  word_s;
    //World_bit word_bit;
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


