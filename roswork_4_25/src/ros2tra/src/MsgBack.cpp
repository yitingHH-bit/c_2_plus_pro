#include <MsgBack.hpp>
//--------------------------------脉冲数据上传------------------------------------------
PulesDataUp::PulesDataUp(uWord_16* deviceip):Pack(deviceip){
  operaType_ = OperType::OPER_TYPE_DET_RESPOND_INFO;
  objType_ = ObjTag::OBJ_TAG_SET_SETPARAM;
}

PulesDataUp::~PulesDataUp(){


}

void PulesDataUp::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen > 2){
     for(int i=0;i<datalen;i++)
       addBuffData((uint8_t)buffer_[i]);
   }else{
     addBuffData(0x00);
   }
  }
  catch(int e){

  }
}
//--------------------------------历史数据应答------------------------------------------
HistoryDataBack::HistoryDataBack(uWord_16* deviceip):Pack(deviceip){
  operaType_ = OperType::OPER_TYPE_DET_RESPOND_INFO;
  objType_ = ObjTag::OBJ_TAG_SET_SETPARAM;
}

HistoryDataBack::~HistoryDataBack(){


}

void HistoryDataBack::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen > 2){
     for(int i=0;i<datalen;i++)
       addBuffData((uint8_t)buffer_[i]);
   }else{
     addBuffData(0x00);
   }
  }
  catch(int e){

  }
}


//--------------------------------统计数据上传------------------------------------------
StatisInfoUp::StatisInfoUp(uWord_16* deviceip):Pack(deviceip){
  operaType_ = OperType::OPER_TYPE_DET_RESPOND_INFO;
  objType_ = ObjTag::OBJ_TAG_SET_SETPARAM;
}

StatisInfoUp::~StatisInfoUp(){


}

void StatisInfoUp::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen > 2){
     for(int i=0;i<datalen;i++)
       addBuffData((uint8_t)buffer_[i]);
   }else{
     addBuffData(0x00);
   }
  }
  catch(int e){

  }
}

//--------------------------------配置参数查询应答------------------------------------------
ParamBack::ParamBack(uWord_16* deviceip):Pack(deviceip){
  operaType_ = OperType::OPER_TYPE_DET_RESPOND_INFO;
  objType_ = ObjTag::OBJ_TAG_SET_SETPARAM;
}

ParamBack::~ParamBack(){


}

void ParamBack::thispack(){
  int datalen = buffer_.size();
  try{
   getTimeStamp();
   uint8_t *out = (uint8_t*)&(time_now_);
   for(int i = 0; i < sizeof(time_now_); i++){//添加具体时间
      addBuffData(out[i] & 0xFF);
   }
  }
  catch(int e){

  }
}

//--------------------------------时间查询应答------------------------------------------
TimeBack::TimeBack(uWord_16* deviceip):Pack(deviceip){
  operaType_ = OperType::OPER_TYPE_DET_RESPOND_INFO;
  objType_ = ObjTag::OBJ_TAG_SET_TIMING;
}

TimeBack::~TimeBack(){


}

void TimeBack::thispack(){
  int datalen = buffer_.size();
  try{
   getTimeStamp();
   uint8_t *out = (uint8_t*)&(time_now_);
   for(int i = 0; i < sizeof(time_now_); i++){//添加具体时间
      addBuffData(out[i] & 0xFF);
   }
  }
  catch(int e){

  }
}
//-------------------------------出错应答------------------------------------------
ErrBack::ErrBack(uWord_16* deviceip):Pack(deviceip){
  operaType_ = OperType::OPER_TYPE_RESPOND_ERROR;
  objType_ = ObjTag::OBJ_TAG_DATA_ERRORINFO;
}

ErrBack::~ErrBack(){


}

void ErrBack::thispack(){
  int datalen = buffer_.size();
  try{
   if(datalen == 1){
      addBuffData((uint8_t)buffer_[0]);
   }
  }
  catch(int e){

  }
}

//--------------------------------无消息内容应答------------------------------------------
MsgBack::MsgBack(uWord_16* deviceip):Pack(deviceip){
  operaType_ = 0;
  objType_ = 0;
}

MsgBack::~MsgBack(){

}

void MsgBack::thispack(){
  return;
}



