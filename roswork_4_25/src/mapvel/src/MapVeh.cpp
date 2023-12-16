/*
 * Show Line of CARLA 0.9.1 Town1
 */

#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <all_msgs_srvs/vehicalmsgs.h>
#include <visualization_msgs/MarkerArray.h>
#include <sstream>
#include <vector>
#include <map>
#include <typeinfo>
#include <cmath>
#include <MapVeh.h>

using namespace veh;

using namespace std;

int main( int argc, char** argv )
{
   ros::init(argc, argv, "Map_vehs");
   ros::NodeHandle n;
   veh::MapVel mapvel(n);
   ros::spin();
   return 0;
 }



























