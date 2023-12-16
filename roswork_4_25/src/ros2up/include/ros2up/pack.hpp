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
    Pack(){ 
       crc_ = 0;
    };
    uint8_t* packData(vector<float> buffer){
      clear();
      buffer_ = buffer;
      //cout<<"bufflen:"<<buffer_.size()<<endl;
      retbuffer_[bufferLen_++] = TCPCOL_HEAD::UP_HEAD;//开头添加0x02
      addHead();
      //cout<<"bufflen1 to:"<<bufferLen_<<endl;
      thispack();
      //cout<<"bufflen2 to:"<<bufferLen_<<endl;
      retbuffer_[bufferLen_++] = crc_;//末尾加上rcc
      retbuffer_[bufferLen_++] = TCPCOL_HEAD::UP_HEAD;//末尾加上0x02
      //cout<<"pack send data len:"<<bufferLen_<<endl;
      return &retbuffer_[0];
    };
    unsigned int getBufferLen(){
      return bufferLen_;
    };
    void clear(){
      bufferLen_ = 0; //
      crc_ = 0;
      //cout<<"buffer clear!"<<endl;
      memset(retbuffer_,0,sizeof(retbuffer_));
    };
    ~Pack(){};

  protected:
    vector<float> buffer_;
    unsigned int bufferLen_;
    uint8_t retbuffer_[9600];
    uint8_t crc_; //校验字节
    uInt_32 time_now_;//当前时间

    virtual void thispack() = 0;
    virtual void addHead() = 0;//0x7E之外的头信息
    void addBuffData(uint8_t data){
       crc_ ^= data;//变化前进行crc校验
       //cout<<"crc:"<<(int)crc_<<",data:"<<(int)data<<endl;
       retbuffer_[bufferLen_++]=(uint8_t)(data & 0xFF);
    };
    void getTimeStamp()
    {
        chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
        time_t timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
        time_now_.uInt32 = (uint32_t)(timestamp/1000);//精度在s
    };
};

#endif

