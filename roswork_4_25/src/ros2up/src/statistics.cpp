#include <statistics.hpp>

StatisUp::StatisUp(uWord_16* deviceip):deviceIp_(deviceip){
  lanNum_ = 0;
}

StatisUp::~StatisUp(){


}

void StatisUp::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen > 5){
    statisParam_.statisPeriod.words = buffer_[0];
    statisParam_.trainLong = (uint8_t)buffer_[1];
    statisParam_.busLong = (uint8_t)buffer_[2];
    statisParam_.carLong = (uint8_t)buffer_[3];
    lanNum_ = (uint8_t)buffer_[4];
    int i = 5;
    int index = 0;
    while((i + 9)<datalen){
      int lanNum = (int)buffer_[i];//取车道号码
      int checkP = (int)buffer_[i+1];//取检测断面数 i = 5
      i = i + 2; //i=7
      for(int j = 0;j < checkP;j++){
        StatisData statisdata;
        statisDataList_.push_back(statisdata);
        statisDataList_[index + j].lanNum = (uint8_t)(lanNum * 10 + j);  //检测地点

        statisDataList_[index + j].checkNum = (uint8_t)(int)buffer_[i]; // 断面编号

        statisDataList_[index + j].carCount = (uint8_t)(int)buffer_[i+2]; //[i+1] 8
        statisDataList_[index + j].busCount = (uint8_t)(int)buffer_[i+3]; //[i+2] 9
        statisDataList_[index + j].trainCount = (uint8_t)(int)buffer_[i+4]; //[i+3] 10
        
        
        statisDataList_[index + j].averHeadTime = (uint8_t)(int)(buffer_[i+5] > 0 ? buffer_[i+5] : 0); //[i+4] 11
        statisDataList_[index + j].averOcc = (uint8_t)(int)buffer_[i+6]; //[i+5] 12
        statisDataList_[index + j].averSpeed = (uint8_t)(int)(buffer_[i+7] > 0 ? buffer_[i+7] : 0); //[i+6] 13
        statisDataList_[index + j].averCarLong = (uint8_t)(int)(buffer_[i+8] > 0 ? buffer_[i+8] : 0); //[i+7] 14
        statisDataList_[index + j].averQueueLong = (uint8_t)(int)buffer_[i+9]; //[i+8] 15
        i = i + 10;
      }
      index = index + checkP;
    }
    addStatisdata();//将取出的数据放到buffer中
   }else{

   }
  }
  catch(int e){

  }
}

void StatisUp::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OPER_TYPE_ACTIVE_VEHSDATA);
  addBuffData(OBJ_TAG_DATA_STATISDATA);
}

void StatisUp::addStatisdata(){
  getTimeStamp();//
  uint8_t *out = (uint8_t*)&time_now_;
  for(int i = 0; i < sizeof(uint32_t); i++)//添加时间
    addBuffData(out[i] & 0xFF);

  out = (uint8_t*)&statisParam_;
  for(int i = 0; i < sizeof(StatisParam); i++)//添加配置参数
    addBuffData(out[i] & 0xFF);
  addBuffData((uint8_t)statisDataList_.size());//添加断面数
  //cout<<"duanmian:"<<(int)statisDataList_.size()<<endl;
  for(vec_it_ = statisDataList_.begin();vec_it_ !=statisDataList_.end();vec_it_++){//添加断面统计信息
    out = (uint8_t*)&(vec_it_->lanNum);
    //cout<<"lan id:"<<(int)vec_it_->lanNum<<endl;
    //cout<<"car num:"<<(int)vec_it_->carCount<<endl;
    for(int i = 0; i < sizeof(StatisData); i++)//添加具体断面统计信息
      addBuffData(out[i] & 0xFF);
  }
  statisDataList_.clear();
}
