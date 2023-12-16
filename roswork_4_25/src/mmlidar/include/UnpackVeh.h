#ifndef GETVEHS_HEAD__
#define GETVEHS_HEAD__


#include <ros/ros.h>
#include <visualization_msgs/Marker.h>

#include <all_msgs_srvs/vehicalmsgs.h>

#include <visualization_msgs/MarkerArray.h>
#include <sstream>
#include <vector>
#include <map>
#include <typeinfo>
#include <cmath>
#include <eigen3/Eigen/Dense>

namespace veh{

using namespace std;
class UnpackVeh{
  public:
    UnpackVeh();
    UnpackVeh(map<int,vector<float> > &vehmap);
    ~UnpackVeh();
    void get_vehmap(vector<float> vehs);
    map<int,vector<float> > get_vehmap();
    int PERVEHDATALEN;
  private:
    map<int,vector<float> > vehmap_;
    map<int, vector<float> >::iterator mapit_;
    bool isExist(float id ,float type ,float state ,float vy ,float x ,float y);
};
}


#endif






















