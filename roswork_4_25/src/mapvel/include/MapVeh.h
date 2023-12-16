/*
 * Show Line of CARLA 0.9.1 Town1
 */
#ifndef LIBMAPVEH_H
#define LIBMAPVEH_H

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>
#include <sstream>
#include <vector>
#include <map>
#include <typeinfo>
#include <cmath>
#include <dynamic_reconfigure/server.h>
#include <mapvel/dynamicLoadConfig.h>

#include <all_msgs_srvs/vehicalmsgs.h>

namespace veh
{

using namespace std;

class MapVel
{
  public:
  MapVel(ros::NodeHandle nh);
 ~MapVel();


private:
  ros::NodeHandle       nh_;
  ros::Subscriber       get_veh_sub_;
  ros::Publisher        veh_pub_;
  ros::Publisher        txt_pub_;
  ros::Publisher        time_pub_;
  visualization_msgs::MarkerArray vehs_;
  visualization_msgs::MarkerArray txts_;
  visualization_msgs::Marker veh_;
  visualization_msgs::Marker txtid_;
  visualization_msgs::Marker txtTime_;



  map<int,vector<float> > vehmap_;
  map<int,vector<float> > vehdelmap_;
  map<int, vector<float> >::iterator mapit_;
  map<int, vector<float> >::iterator mapdelit_;

  //veh::UnpackVeh getvehs_;
  float LIDAR2ORGX;//雷达相对原点的x
  float LIDAR2ORGY;//雷达相对原点的y
  float LIDAR2GROUDZ;//雷达的高度
  float PERVEHDATALEN;//ros车辆消息数组长度
  int bugindex_;
  //车辆显示范围
  float SCALE_MIN_X;
  float SCALE_MAX_X;
  float SCALE_MIN_Y;
  float SCALE_MAX_Y;
  float Thangle;

  //显示频率，计算时间
  int FPS_DISPLAY;
   
  //int y_;
  uint32_t shape_;

  void data_init();
///周期更新隐身的车辆
  void loop_pub();

///检查更新在线车辆列表，返回是否存在
  bool isExist(float id ,float type ,float state ,float vy ,float x ,float y);

///解析在线车辆信息list
  void get_vehs_data(vector<float> vehs, int time);

//更新显示
  void display_map();
  void unMap();

  void vehicalCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist);

  void make_txt(float x , float y, float z, string frameID, int carID, bool isadd, int time = -1);

  void make_vel(float sort, float x0, float y0, float x1 ,float y1 ,string frameID ,int carID, bool Invisible,bool isadd, int time = -1);
};

}

#endif


























