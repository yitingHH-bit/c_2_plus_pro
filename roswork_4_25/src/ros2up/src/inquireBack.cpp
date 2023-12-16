#include <inquireBack.hpp>

//------------------------------车道信息查询应答----------------------------------

LanInfoBack::LanInfoBack(uWord_16* deviceip):deviceIp_(deviceip){

}

LanInfoBack::~LanInfoBack(){


}

void LanInfoBack::thispack(){
  //cout<<"veh buffer len:"<<
  int datalen = buffer_.size();
  cout<<"lan buffer_len:"<<datalen<<endl; 
  try{
   if (datalen > 8){
    lanallnum_ = (uint8_t)buffer_[0];//总车道数
    int i = 1;
    laninfoList_.clear();
    while((i+7)<datalen){
      LanBack landata;
      landata.lanid = (uint8_t)buffer_[i];//id
      landata.lanwidth.Float32 = buffer_[i+1];//width
      landata.lanlong.Float32 =  buffer_[i+2];//道路长度
      landata.lanattr = (uint8_t)buffer_[i+3];//道路属性
      landata.lantype = (uint8_t)buffer_[i+4];//道路类型
      landata.coordx.Float32 = buffer_[i+5];//左下角坐标x
      landata.coordy.Float32 = buffer_[i+6];//左下角坐标y
      landata.maxspeed.Float32 = buffer_[i+7];//最大速度
      laninfoList_.push_back(landata);
      i = i + 8;
    }
    addLanInfo();//将取出的数据放到buffer中
   }else{

   }
  }
  catch(int e){

  }
}
void LanInfoBack::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_RESPOND_INFO);
  addBuffData(ObjTag::OBJ_TAG_DATA_SETTING_LAN);
}
void LanInfoBack::addLanInfo(){
  addBuffData(lanallnum_);//添加总车道数
  uint8_t *out = NULL;
  for(vec_it_ = laninfoList_.begin();vec_it_ !=laninfoList_.end();vec_it_++){//添加车道信息
    out = (uint8_t*)&(vec_it_->lanid);
    for(int i = 0; i < sizeof(LanBack); i++){//添加具体车道信息
      addBuffData(out[i] & 0xFF);
    }
  }

}

//--------------------------------统计设置信息查询应答------------------------------------------
StatisInfoBack::StatisInfoBack(uWord_16* deviceip):deviceIp_(deviceip){

}

StatisInfoBack::~StatisInfoBack(){


}

void StatisInfoBack::thispack(){
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

void StatisInfoBack::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_RESPOND_INFO);
  addBuffData(ObjTag::OBJ_TAG_DATA_SETTING_STATIS);
}

//--------------------------------统计周期查询应答------------------------------------------
PeriodBack::PeriodBack(uWord_16* deviceip):deviceIp_(deviceip){

}

PeriodBack::~PeriodBack(){


}

void PeriodBack::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen > 0){
     uInt_32 period;
     period.uInt32 = (uint32_t)buffer_[0];
     uint8_t* out = (uint8_t*)&(period.union_32.word0);
     for(int i = 0; i < sizeof(uInt_32); i++){//
       addBuffData(out[i] & 0xFF);
     }
   }else{
     addBuffData(0x00);
   }
  }
  catch(int e){

  }
}

void PeriodBack::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_RESPOND_INFO);
  addBuffData(ObjTag::OBJ_TAG_DATA_SETTING_STATIS_PEROID);
}


//--------------------------------安装和标校设置查询应答------------------------------------------
InstallInfoBack::InstallInfoBack(uWord_16* deviceip):deviceIp_(deviceip){

}

InstallInfoBack::~InstallInfoBack(){


}

void InstallInfoBack::thispack(){
  //cout<<"veh buffer len:"<<
  int datalen = buffer_.size();
  cout<<"data_len:"<<datalen<<endl;
  try{
   if (datalen > 7){
    lanallnum_ = (uint8_t)buffer_[0];//总车道数
    int i = 1;
    cliinfoList_.clear();
    while((i+6)<datalen){
      InstallBack instdata;
      instdata.num = (uint8_t)buffer_[i];//num
      //cout<<"install num:"<<(uint8_t)buffer_[i]<<endl;
      instdata.x.Float32 = buffer_[i+1];//x
      instdata.y.Float32 = buffer_[i+2];//y
      instdata.z.Float32 =  buffer_[i+3];//z
      instdata.roll.Float32 = buffer_[i+4];//roll
      instdata.pitch.Float32 = buffer_[i+5];//pitch
      instdata.yaw.Float32 = buffer_[i+6];//yaw
      cliinfoList_.push_back(instdata);
      i = i + 7;
    }
    addCliInfo();//将取出的数据放到buffer中
   }else{

   }
  }
  catch(int e){

  }
}

void InstallInfoBack::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_RESPOND_INFO);
  addBuffData(ObjTag::OBJ_TAG_DATA_SETTING_INSTALL);
}

void InstallInfoBack::addCliInfo(){
  addBuffData(lanallnum_);//添加区域总数
  uint8_t *out = NULL;
  for(vec_it_ = cliinfoList_.begin();vec_it_ !=cliinfoList_.end();vec_it_++){//添加标校数据
    out = (uint8_t*)&(vec_it_->num);
    for(int i = 0; i < sizeof(InstallBack); i++){//添加具体区域标校信息
      addBuffData(out[i] & 0xFF);
      //cout<<"install data :"<<(uint8_t)out[i]<<endl;
    }
  }
}

//--------------------------------设备ip查询应答------------------------------------------
Deviceipback::Deviceipback(uWord_16* deviceip):deviceIp_(deviceip){
  cout<<"deviceback new!"<<endl;
}

Deviceipback::~Deviceipback(){


}

void Deviceipback::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen >= 9){
    int index = 0;
    uWord_16 ID ;
    ID.uwords= (uint16_t)buffer_[index++];
    devicebackinfo_.id.word_s.byte0= (uint8_t)ID.word_s.byte0;//ID0
    devicebackinfo_.id.word_s.byte1= (uint8_t)ID.word_s.byte1;//ID1
    devicebackinfo_.ip1 = (uint8_t)buffer_[index++];//
    devicebackinfo_.ip2 = (uint8_t)buffer_[index++];//
    devicebackinfo_.ip3 = (uint8_t)buffer_[index++];//
    devicebackinfo_.ip4 = (uint8_t)buffer_[index++];//
    devicebackinfo_.goal1 = (uint8_t)buffer_[index++];//
    devicebackinfo_.goal2 = (uint8_t)buffer_[index++];//
    devicebackinfo_.goal3 = (uint8_t)buffer_[index++];//
    devicebackinfo_.goal4 = (uint8_t)buffer_[index++];//
   }else{

   }
   uint8_t *out = &(devicebackinfo_.id.word_s.byte0);
   for(int i = 0; i < sizeof(DeviBack); i++){//添加具体区域标校信息
      addBuffData(out[i] & 0xFF);
   }
  }
  catch(int e){

  }
}

void Deviceipback::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_RESPOND_INFO);
  addBuffData(ObjTag::OBJ_TAG_DATA_SETTING_DEVICE_IP);
}

//--------------------------------外部设备网络查询应答------------------------------------------
Outdeviceback::Outdeviceback(uWord_16* deviceip):deviceIp_(deviceip){
  cout<<"deviceback new!"<<endl;
}

Outdeviceback::~Outdeviceback(){


}

void Outdeviceback::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen == 14){
    outdeviBack_.tra_sta= (uint8_t)buffer_[0];//信号机状态
    outdeviBack_.tra_comm_type= (uint8_t)buffer_[1];//通讯方式
    outdeviBack_.tra_ip1= (uint8_t)buffer_[2];//ip
    outdeviBack_.tra_ip2= (uint8_t)buffer_[3];//
    outdeviBack_.tra_ip3= (uint8_t)buffer_[4];//
    outdeviBack_.tra_ip4= (uint8_t)buffer_[5];//
    outdeviBack_.tra_port.uInt32= (uint32_t)buffer_[6];//端口
    outdeviBack_.plat_sta= (uint8_t)buffer_[7];//平台状态
    outdeviBack_.plat_comm_type= (uint8_t)buffer_[8];//通讯方式
    outdeviBack_.plat_ip1= (uint8_t)buffer_[9];//ip
    outdeviBack_.plat_ip2= (uint8_t)buffer_[10];//
    outdeviBack_.plat_ip3= (uint8_t)buffer_[11];//
    outdeviBack_.plat_ip4= (uint8_t)buffer_[12];//
    outdeviBack_.plat_port.uInt32= (uint32_t)buffer_[13];//端口
   }else{

   }
   uint8_t *out = &(outdeviBack_.tra_sta);
   for(int i = 0; i < sizeof(outdeviBack_); i++){//添加具体区域标校信息
      addBuffData(out[i] & 0xFF);
   }
  }
  catch(int e){

  }
}

void Outdeviceback::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_RESPOND_INFO);
  addBuffData(ObjTag::OBJ_TAG_DATA_SETTING_OUT_DEVICE_IP);
}


//--------------------------------设置应答------------------------------------------
SettingBack::SettingBack(uWord_16* deviceip):deviceIp_(deviceip){
  operaType_ = 0;
  dataType_ = 0;
}

SettingBack::~SettingBack(){

}

void SettingBack::thispack(){
  return;
}

void SettingBack::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(operaType_);
  addBuffData(dataType_);
}
void SettingBack::setHead(uint8_t operaType,uint8_t dataType){
  operaType_ = operaType;
  dataType_ = dataType;
}

//--------------------------------在线查询（心跳）应答------------------------------------------
OnLineQBack::OnLineQBack(uWord_16* deviceip):deviceIp_(deviceip){
  operaType_ = 0;
  dataType_ = 0;
}

OnLineQBack::~OnLineQBack(){

}

void OnLineQBack::thispack(){
  return;
}

void OnLineQBack::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(operaType_);
  addBuffData(dataType_);
}
void OnLineQBack::setHead(uint8_t operaType,uint8_t dataType){
  operaType_ = operaType;
  dataType_ = dataType;
}

