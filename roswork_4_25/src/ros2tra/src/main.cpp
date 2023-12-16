#include <ros/ros.h>
#include <statis2tra.h>

using namespace std;

int main(int argc, char** argv){
    ros::init(argc, argv, "ros2tra_node");
    ros::NodeHandle nh;
    TraBridge traBridge(&nh);
    //ros::Rate loop_rate(15);
    ros::spin();
}
