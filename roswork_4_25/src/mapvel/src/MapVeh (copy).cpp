/*
 * Show Line of CARLA 0.9.1 Town1
 */

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <mmlidar/vehicalmsgs.h>
#include <visualization_msgs/MarkerArray.h>
#include <sstream>
#include <vector>
#include <map>
#include <typeinfo>
#include <cmath>

namespace veh
{

using namespace std;

class MapVel
{
public:
  MapVel(ros::NodeHandle nh):
    nh_(nh)
 {
     ros::Time::init();
     veh_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("Map_vehicals", 10);
     txt_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("Map_txts", 10);
    //接收车辆信息
    get_veh_sub_ = nh_.subscribe<mmlidar::vehicalmsgs>("/mmlidar_veh", 1, &MapVel::vehicalCb, this);
    data_init();

 }
 ~MapVel()
 {
      
 }


private:
  ros::NodeHandle       nh_;
  ros::Subscriber       get_veh_sub_;
  ros::Publisher        veh_pub_;
  ros::Publisher        txt_pub_;
  visualization_msgs::MarkerArray vehs_;
  visualization_msgs::MarkerArray txts_;
  visualization_msgs::Marker veh_;
  visualization_msgs::Marker txtid_;

  map<int,vector<float> > vehmap_;
  map<int, vector<float> >::iterator mapit_;
  float LIDAR2ORGX;//雷达相对原点的x
  float LIDAR2ORGY;//雷达相对原点的y
  float PERVEHDATALEN;//ros车辆消息数组长度
  //车辆显示范围
  float SCALE_MIN_X;
  float SCALE_MAX_X;
  float SCALE_MIN_Y;
  float SCALE_MAX_Y;

  //显示频率，计算时间
  int FPS_DISPLAY;
   
  //int y_;
  uint32_t shape_;

  void data_init()
  {
    shape_ = visualization_msgs::Marker::CUBE;
    //y_ = 0;
    LIDAR2ORGX=0.0;
    LIDAR2ORGY=30.0;
    PERVEHDATALEN=6;

    SCALE_MIN_X = -1000.0;
    SCALE_MAX_X = 1000.0;
    SCALE_MIN_Y = -3000.0;
    SCALE_MAX_Y = 3000.0;
    FPS_DISPLAY = 10;
    
  }
///周期更新隐身的车辆
  void loop_pub()
  {
    //显示处理，包括预测和透明更新
    display_map();
    veh_pub_.publish(vehs_);
    vehs_.markers.clear();
    txt_pub_.publish(txts_);
    txts_.markers.clear();
  }


///检查更新在线车辆列表，返回是否存在
  bool isExist(float id ,float type ,float state ,float vy ,float x ,float y)
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
  void get_vehs_data(vector<float> vehs)
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
          vehmap_.insert(make_pair<int,vector<float> >(vehs[i],listdata));//ID为key
          ROS_INFO("Insert data");
        }
      }
    }
  }
 

//更新显示
  void display_map()
  {
     //显示
    for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_)
    {
      if((int)mapit_->second[5] == 0)
      {
        if(mapit_->second[3] >= SCALE_MIN_X && mapit_->second[3] <= SCALE_MAX_X && mapit_->second[4] >= SCALE_MIN_Y && mapit_->second[4] <= SCALE_MAX_Y)
        {
           mapit_->second[6] = mapit_->second[3];//更新上一时刻的位置信息
           mapit_->second[7] = mapit_->second[4];
           mapit_->second[4] = mapit_->second[4] + mapit_->second[2] * (1.0 / FPS_DISPLAY);
        }
        else
        {
           vehmap_.erase(mapit_);
        }
      }
      make_vel(mapit_->second[0],mapit_->second[3],mapit_->second[4],mapit_->second[6],mapit_->second[7],"my_frame",mapit_->first,mapit_->second[5]);
      mapit_->second[5] = 0;
    }
  }


  void vehicalCb(const mmlidar::vehicalmsgsConstPtr &vehicalist)
  {
    ROS_INFO("get_veldata");
    //检查更新数据
    get_vehs_data(vehicalist->vehicaldatas);    
    loop_pub();
  }

  void make_txt(float x , float y, float z, string frameID, int carID)
  {
     txtid_.header.frame_id = frameID;
     txtid_.header.stamp = ros::Time::now();
     txtid_.ns = "basic_shapes";
     txtid_.id = carID;
 
     txtid_.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
     txtid_.action = visualization_msgs::Marker::ADD;
 
     txtid_.pose.position.x = x;
     txtid_.pose.position.y = y;
     txtid_.pose.position.z = z;
     txtid_.pose.orientation.x = 0.0;
     txtid_.pose.orientation.y = 0.0;
     txtid_.pose.orientation.z = 0.0;
     txtid_.pose.orientation.w = 1.0;

     //txtid_.scale.x = shape_wid;
     //txtid_.scale.y = 0.1;
     txtid_.scale.z = 2;
 
     txtid_.color.r = 255;
     txtid_.color.g = 23;
     txtid_.color.b = 17;
     txtid_.color.a = 1.0;

     ostringstream ss ;
     ss<<carID;

 
     txtid_.text = ss.str();
 
     txtid_.lifetime = ros::Duration();

     txts_.markers.push_back(txtid_);
  }
  void make_vel(float sort, float x0, float y0, float x1 ,float y1 ,string frameID ,int carID, bool Invisible)
  {
     float pos_x = x0 - LIDAR2ORGX;
     float pos_y = y0 - LIDAR2ORGY;
     float pos_z = 0;
     float appear = 1.0;
     float shape_lon = 5;
     float shape_wid = 2.2;
     float shape_hei = 1.5;
     float color_r = 0;
     float color_g = 128;
     float color_b = 0;
     
     if(!Invisible)
     {
       appear = 0.3;
     }

     if((int)sort == 1){ //大车,浅蓝色
       shape_lon = 8;
       shape_wid = 2.5;
       shape_hei = 2.5;
       color_r = 108;
       color_g = 36;
       color_b = 45;
     }
     else if((int)sort == 2){ //超大车，蓝色
       shape_lon = 10;
       shape_wid = 2.5;
       shape_hei = 2.5;
       color_r = 0;
       color_g = 0;
       color_b = 255;
     }
     else if((int)sort == 3){ //骑行车，黄色
       shape_lon = 1.5;
       shape_wid = 1.0;
       shape_hei = 1.3;
       color_r = 255;
       color_g = 255;
       color_b = 0;
     }
     

     make_txt(pos_x,pos_y+shape_lon/2 + 1,shape_hei + 0.5,frameID,carID);

     veh_.header.frame_id = frameID;
     veh_.header.stamp = ros::Time::now();
     veh_.ns = "basic_shapes";
     veh_.id = carID;
     veh_.text = "";
 
     veh_.type = shape_;
     veh_.action = visualization_msgs::Marker::ADD;
 
     veh_.pose.position.x = pos_x;
     veh_.pose.position.y = pos_y;
     veh_.pose.position.z = shape_hei / 2.0;

     //求角度
     double thta = 0;
     //为了稳定小距离的角度
     if(abs(x0-x1) > 0.3 && abs(atan2(y0-y1,x0-x1))> (3.14/6))
     {
        thta = atan2(y0-y1,x0-x1);
     }
     veh_.pose.orientation.x = 0.0;
     veh_.pose.orientation.y = 0.0;
     veh_.pose.orientation.z = -sin(thta/2);
     veh_.pose.orientation.w = cos(thta/2);

     veh_.scale.x = shape_wid;
     veh_.scale.y = shape_lon;
     veh_.scale.z = shape_hei;
 
     veh_.color.r = color_r;
     veh_.color.g = color_g;
     veh_.color.b = color_b;
     veh_.color.a = appear;
 
     veh_.lifetime = ros::Duration();

     vehs_.markers.push_back(veh_);
     //veh_.clear();
  }
};
}

int main( int argc, char** argv )
{
   ros::init(argc, argv, "Map_vehs");
   ros::NodeHandle n;
   veh::MapVel mapvel(n);
   ros::spin();
   return 0;
 }



























