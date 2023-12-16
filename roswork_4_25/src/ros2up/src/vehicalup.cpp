#include <vehicalup.hpp>

VehicalUp::VehicalUp(uWord_16* deviceip):deviceIp_(deviceip){
  //deviceIp_->uwords = *deviceip;
}

VehicalUp::~VehicalUp(){


}

void VehicalUp::thispack(){
  //cout<<"veh buffer len:"<<
  int datalen = buffer_.size();
  try{
   if (datalen > 5){
    int i = 0;
    vehsDataList_.clear();
    while((i+5)<datalen){
      VehInfo vehdata;
      vehdata.id.uwords = (uint16_t)buffer_[i];//id
      // cout<<"vehdata.id.uwords:"<<vehdata.id.uwords<<endl;

      vehdata.lanNum = (uint8_t)buffer_[i+2];//lan
      // cout<<"vehdata.lanNum:"<<vehdata.lanNum<<endl;

      vehdata.lanType = (uint8_t)getLanType(buffer_[i+3]);//lan sort
      // cout<<"vehdata.lanType:"<<vehdata.lanType<<endl;

      vehdata.vehType = (uint8_t)buffer_[i+1];
      // cout<<"vehdata.vehType:"<<vehdata.vehType<<endl;

      vehdata.coord.x.Float32 = buffer_[i+4];
      //cout<<"vehdata.coord.x:"<<vehdata.coord.x<<endl;

      vehdata.coord.y.Float32 = buffer_[i+5];
      //cout<<"vehdata.coord.y:"<<vehdata.coord.y<<endl;

      vehdata.speedY.Float32 = buffer_[i+3];
      //cout<<"vehdata.speedY:"<<vehdata.speedY<<endl;
      
      vehsDataList_.push_back(vehdata);
      i = i + 6;
    }
    addVehsdata();//将取出的数据放到buffer中
   }else{

   }
  }
  catch(int e){

  }
}

uint8_t VehicalUp::getLanType(float yspeed){
  uint8_t ret = LAN_TYPE_COME;
  if(yspeed > 0.0)
    ret = LAN_TYPE_GO;
  return ret;
}

void VehicalUp::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OPER_TYPE_ACTIVE_VEHSDATA);
  addBuffData(OBJ_TAG_DATA_NOWVEHSDATA);
}

void VehicalUp::addVehsdata(){
  getTimeStamp();//
  uint8_t *out = (uint8_t*)&time_now_;
  for(int i = 0; i < sizeof(uint32_t); i++){//添加时间
    //cout<<"Time:"<<(int)out[i]<<endl;
    addBuffData(out[i] & 0xFF);
  }

  addBuffData((uint8_t)vehsDataList_.size());//添加目标数
  for(vec_it_ = vehsDataList_.begin();vec_it_ !=vehsDataList_.end();vec_it_++){//添加车辆信息
    out = (uint8_t*)&(vec_it_->id);
    for(int i = 0; i < sizeof(VehInfo); i++){//添加具体车辆信息
      addBuffData(out[i] & 0xFF);
    }
  }
}
