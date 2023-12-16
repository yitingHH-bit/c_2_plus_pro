#include <device_state.hpp>

DevStateUp::DevStateUp(uWord_16* deviceip):deviceIp_(deviceip){

}

DevStateUp::~DevStateUp(){


}

void DevStateUp::thispack(){
  int datalen = buffer_.size();
  try{
   if (datalen == 9){
    stateData_.occ_cpu.Float32 =  buffer_[0];
    stateData_.occ_gpu.Float32 =  buffer_[1];
    stateData_.core_tem.Float32 =  buffer_[2];
    stateData_.occ_memory.Float32 =  buffer_[3];
    stateData_.space_home.Float32 =  buffer_[4];
    stateData_.space_sda1.Float32 =  buffer_[5];
    stateData_.pre_sta = (uint8_t)buffer_[6];
    stateData_.cam_sta = (uint8_t)buffer_[7];
    stateData_.lid_sta = (uint8_t)buffer_[8];
    addStatisdata();//将取出的数据放到buffer中
   }else{

   }
  }
  catch(int e){

  }
}

void DevStateUp::addHead(){
  addBuffData(deviceIp_->word_s.byte0);
  addBuffData(deviceIp_->word_s.byte1);
  addBuffData(OperType::OPER_TYPE_ACTIVE_VEHSDATA);
  addBuffData(ObjTag::OBJ_TAG_DATA_DEVICE_STATION);
}

void DevStateUp::addStatisdata(){
  uint8_t *out = (uint8_t*)&(stateData_.occ_cpu);
  for(int i = 0; i < sizeof(StateData); i++){//添加
      addBuffData(out[i] & 0xFF);
  }
}
