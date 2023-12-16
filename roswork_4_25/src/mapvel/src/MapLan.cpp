/*
 * Show Line of CARLA 0.9.1 Town1
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <visualization_msgs/MarkerArray.h>

#include <cmath>
#include <dynamic_reconfigure/server.h>
#include <mapvel/dynamicLoadConfig.h>

using namespace std;

#define Datalong 10
float *dynamicdata;

void callback(mapvel::dynamicLoadConfig &config, uint32_t level){
   //ROS_INFO("the angle:%f,interval:%f",config.theta_deg,config.interval);
   dynamicdata[0] = config.pathnum;
   dynamicdata[1] = config.sureStopLine;
   dynamicdata[2] = config.lan_width;
   dynamicdata[3] = config.lan_long;
   dynamicdata[4] = config.lan_halflong;
   dynamicdata[5] = config.theta_deg;
   dynamicdata[6] = config.interval;
   dynamicdata[7] = config.org_x;
   dynamicdata[8] = config.org_y;
   ROS_INFO("the angle:%f,interval:%f",dynamicdata[5],dynamicdata[6]);
   dynamicdata[9] = 1;//表示更新
}

class LanLine
{
public:

  LanLine(ros::NodeHandle nh, float dynamic[]):
  nh_(nh),
  pathdata_(dynamic)
  {
     ros::Time::init();
     lan_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("MarkerArray", 10);
     f_ = boost::bind(&callback,_1,_2);
     server_.setCallback(f_);
     
     cout<<"data:"<<pathdata_[0]<<","<<pathdata_[1]<<","<<pathdata_[2]<<","<<pathdata_[3]<<","<<pathdata_[4]<<","<<pathdata_[5]<<","<<pathdata_[6]<<","<<pathdata_[7]<<","<<pathdata_[8]<<endl;
     //init_data();
     idex_ = 0;
     init_info(false);
     draw_lan();
     pub_info();
  }
  ~LanLine(){

  }

  void pub_info(){
    ros::Rate r(1);
    while (ros::ok())
    {
        if(abs(pathdata_[9] - 1.0) < 0.0001){
            init_info(true);
            draw_lan();
            lan_pub_.publish(lines_);
            init_info(false);
            draw_lan();
            pathdata_[9] = 0;
        }
        lan_pub_.publish(lines_);
        r.sleep();
        ros::spinOnce();
    }
  }
private:
  float lanwidth_;
  float lanlong_;
  int idex_;
  ros::NodeHandle nh_;
  visualization_msgs::MarkerArray lines_;
  visualization_msgs::Marker line_strip_;
  ros::Publisher lan_pub_;
  float *pathdata_;

  dynamic_reconfigure::Server<mapvel::dynamicLoadConfig> server_;
  dynamic_reconfigure::Server<mapvel::dynamicLoadConfig>::CallbackType f_;


  void draw_lan(){
    line_strip_.color.r = 255;
    line_strip_.color.g = 255;
    line_strip_.color.b = 240;

    if(abs(pathdata_[1] - 1.0) < -0.0001){
      line_strip_.scale.x = 0.5;
      ROS_INFO("draw stopline!");   
      draw_line(-pathdata_[0] * pathdata_[2] + pathdata_[7], 0 , 0, pathdata_[7], 0 , 0);//停止线段
    }
    for(int i = 0 ; i <= pathdata_[0]; i++)
    {
      line_strip_.scale.x = 0.5;
      if(abs(i-pathdata_[0]) < 0.0001 && pathdata_[4] != pathdata_[3]){
          draw_line(-i * pathdata_[2] - pathdata_[7], 0 - pathdata_[8] , 0,-i * pathdata_[2] - pathdata_[7] + pathdata_[4] * tan(pathdata_[5] / 180.0 * 3.1415926), pathdata_[4] - pathdata_[8] , 0);
          draw_line(-i * pathdata_[2] - pathdata_[7] + pathdata_[4] * tan(pathdata_[5] / 180.0 * 3.1415926), pathdata_[4] - pathdata_[8] , 0,-(i-1) * pathdata_[2] - pathdata_[7] + (pathdata_[4] + pathdata_[2]) * tan(pathdata_[5] / 180.0 * 3.1415926), pathdata_[4] + pathdata_[2] - pathdata_[8] , 0);
          break;
      }
      draw_line(-i * pathdata_[2] - pathdata_[7], 0 - pathdata_[8] , 0,-i * pathdata_[2] - pathdata_[7] + pathdata_[3] * tan(pathdata_[5] / 180.0 * 3.1415926), pathdata_[3] - pathdata_[8] , 0);
    }

    line_strip_.color.r = 0;
    line_strip_.color.g = 255;
    line_strip_.color.b = 0;
  
    for(int i=0;i<= pathdata_[3]/pathdata_[6];i++){
        float len = 10;
        draw_line(-pathdata_[0] * pathdata_[2] - pathdata_[7] -len, i * pathdata_[6]  - pathdata_[8] ,0 , pathdata_[0] * pathdata_[2] - pathdata_[7] + len, i * pathdata_[6] - pathdata_[8], 0);
    }
  }
  void draw_line(double x1,double y1, double z1,double x2,double y2, double z2){
    geometry_msgs::Point p,q;
    p.x = x1;
    p.y = y1;
    p.z = z1;
    q.x = x2;
    q.y = y2;
    q.z = z2;
    line_strip_.id = idex_++;
    line_strip_.points.push_back(p);
    line_strip_.points.push_back(q);   
    lines_.markers.push_back(line_strip_);
    line_strip_.points.clear();
  }

  void init_info(bool isdelete){
    line_strip_.header.frame_id = "/my_frame";
    line_strip_.header.stamp= ros::Time::now();
    line_strip_.ns = "points_and_lines";
    line_strip_.action = visualization_msgs::Marker::ADD;
    if(isdelete)
         line_strip_.action = visualization_msgs::Marker::DELETEALL;
    line_strip_.pose.orientation.w = 1.0;

    line_strip_.type = visualization_msgs::Marker::LINE_STRIP;
    line_strip_.scale.x = 0.1;
    line_strip_.color.r = 255;
    line_strip_.color.g = 251;
    line_strip_.color.b = 240;
    line_strip_.color.a = 1.0;
  }
};

int main(int argc, char** argv)
{
  float dynamic[Datalong] = {4.0,0,3.15,400,200,0,25.0,0};//分别表示车道数,有无停车线,车道宽,车道长,半车道长,安装偏角, 等距线,是否更新
  ros::param::get("~pathnum",dynamic[0]);
  ros::param::get("~sureStopLine",dynamic[1]);
  ros::param::get("~lan_width",dynamic[2]);
  ros::param::get("~lan_long",dynamic[3]);
  ros::param::get("~lan_halflong",dynamic[4]);
  ros::param::get("~theta",dynamic[5]);
  ros::param::get("~interval",dynamic[6]);
  ros::param::get("~org_x",dynamic[7]);
  ros::param::get("~org_y",dynamic[8]);
  ostringstream ss;
  ss<<"lan_width:"<<dynamic[2]<<",lan_long:"<<dynamic[3];
  cout<<ss.str()<<endl;
  
  dynamicdata = dynamic;
  ros::init(argc, argv, "draw_lanline");
  ros::NodeHandle nh;
  LanLine lanline(nh,dynamicdata);

//等待接受后就可以退出了，以免占带宽
  ros::spin();
  return 0;
}



























