/*
 * Show Line of CARLA 0.9.1 Town1
 */

#include <UnpackVeh.h>

namespace veh
{
  UnpackVeh::UnpackVeh(){


  }
  UnpackVeh::UnpackVeh(map<int,vector<float> > &vehmap):
  vehmap_(vehmap)
 {
   UnpackVeh::PERVEHDATALEN = 6;
 }
 UnpackVeh::~UnpackVeh()
 {
      
 }
 map<int,vector<float> >UnpackVeh::get_vehmap(){
   return vehmap_;
 }


///检查更新在线车辆列表，返回是否存在
  bool UnpackVeh::isExist(float id ,float type ,float state ,float vy ,float x ,float y)
  {
    bool ret = false;
    for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_)
    {
       //cout<<"itera:"<<id<<endl;
       if((int)(mapit_->first) == (int)id)
       {
         //cout<<"update:"<<id<<endl;
         ret = true;
         //更新数据
         mapit_->second[6] = mapit_->second[3];//更新上一时刻的位置信息
         mapit_->second[7] = mapit_->second[4];
         mapit_->second[0] = type;
         mapit_->second[1] = state;
         mapit_->second[2] = vy;
         mapit_->second[3] = x;
         mapit_->second[4] = y;
         mapit_->second[5] = 1.0;
         break;
       }
    }
    return ret;
  }

///解析在线车辆信息list
  void UnpackVeh::get_vehmap(vector<float> vehs)
  {
    int size = vehs.size();
    //cout<<size<<endl;
    if(size < PERVEHDATALEN)
      cout<<"veh_data failure!"<<endl;
    else{     
      for(int i = 0; i < size; i = i + PERVEHDATALEN){
        if(!isExist(vehs[i],vehs[i+1],vehs[i+2],vehs[i+3],vehs[i+4],vehs[i+5])){
          //cout<<i<<endl;
          vector<float> listdata(8,-1);
          listdata.insert(listdata.begin(),vehs[i+1]);//sort
          listdata.insert(listdata.begin()+1,vehs[i+2]);//state
          listdata.insert(listdata.begin()+2,vehs[i+3]);//vy
          listdata.insert(listdata.begin()+3,vehs[i+4]);//x
          listdata.insert(listdata.begin()+4,vehs[i+5]);//y
          listdata.insert(listdata.begin()+5,1.0);//是否在线
          listdata.insert(listdata.begin()+6,-1.0);//上个位置的信息
          listdata.insert(listdata.begin()+7,-1.0);
          vehmap_.insert(make_pair(vehs[i],listdata));//ID为key
          //ROS_INFO("Insert data");
        }
      }
    }
 }

}




























