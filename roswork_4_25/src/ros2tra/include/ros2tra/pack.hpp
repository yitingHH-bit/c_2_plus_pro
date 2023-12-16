#ifndef __PACK_HEAD__
#define __PACK_HEAD__


#include "stdint.h"
#include <string.h>
//#include <pthread.h>
#include <iostream>
#include <cmath>
#include "tcpcontrol.h"
#include <vector>
#include <time.h>
#include <chrono>


using namespace std;
class Pack
{
  public:
    Pack(uWord_16* deviceIp):deviceIp_(deviceIp){ 
       crc_ = 0;
    };
    uint8_t* packData(vector<float> buffer,uint8_t operaType,uint8_t objType){
      clear();//清空数据
      buffer_ = buffer;//数据转移
      //cout<<"bufflen:"<<buffer_.size()<<endl;
      retbuffer_[bufferLen_++] = TCPCOL_HEAD::UP_HEAD;//开头添加0x7E
      addHead(operaType,objType);
      //cout<<"bufflen1 to:"<<bufferLen_<<endl;
      thispack();
      //cout<<"bufflen2 to:"<<bufferLen_<<endl;
      retbuffer_[bufferLen_++] = crc_;//末尾加上rcc
      retbuffer_[bufferLen_++] = TCPCOL_HEAD::UP_HEAD;//末尾加上0x7E
      //cout<<"pack send data len:"<<bufferLen_<<endl;
      return &retbuffer_[0];
    };
    unsigned int getBufferLen(){
      return bufferLen_;
    };
    void clear(){
      bufferLen_ = 0; //
      crc_ = 0;
      memset(retbuffer_,0,sizeof(retbuffer_));
    };
    ~Pack(){};

  protected:
    vector<float> buffer_;//外部数据
    unsigned int bufferLen_;//发送的数据长度
    uint8_t retbuffer_[9600];//要发送的数据
    uint8_t crc_; //校验字节
    uInt_32 time_now_;//当前时间
    uint8_t operaType_;//操作类型
    uint8_t objType_;//对象标识
    uWord_16* deviceIp_;//设备的链路地址

    virtual void thispack() = 0;
    virtual void addHead(uint8_t operaType,uint8_t objType){
      addLoadNum();
      addBuffData(AgrVersion::V_10);
      addBuffData(operaType);
      addBuffData(objType);      
    };//0x7E之外的头信息
    void addBuffData(uint8_t data){
       if(data & 0xFF == 0x7E){
         crc_ ^= 0x7D;//
         crc_ ^= 0x5E;//
         retbuffer_[bufferLen_++]=(uint8_t)(0x7D);
         retbuffer_[bufferLen_++]=(uint8_t)(0x5E);
       }else if(data & 0xFF == 0x7D){
         crc_ ^= 0x7D;//
         crc_ ^= 0x5D;//
         retbuffer_[bufferLen_++]=(uint8_t)(0x7D);
         retbuffer_[bufferLen_++]=(uint8_t)(0x5D);
       }else{
         crc_ ^= data;//
         retbuffer_[bufferLen_++]=(uint8_t)(data & 0xFF);//
       }
    };
    void addLoadNum(){
      if(deviceIp_->uwords < 64){
        addBuffData(((deviceIp_->word_s.byte0) << 2) + 0x01);
      }else{
        addBuffData(((deviceIp_->word_s.byte0) << 2));
        addBuffData(((deviceIp_->word_s.byte1) << 2) + 0x01);
      }
    };
    void getTimeStamp()
    {
        chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
        time_t timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
        time_now_.uInt32 = (uint32_t)(timestamp/1000);//精度在s
    };
};

#endif

