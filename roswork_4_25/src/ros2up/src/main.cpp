#include <ros/ros.h>
#include <vel2up.h>

using namespace std;
using namespace ros2up;

int main(int argc, char** argv){
    ros::init(argc, argv, "ros2up_node");
    ros::NodeHandle nh;
    ArgBridge argBridge(&nh);
    ros::Rate loop_rate(15);
    //while (ros::ok())
    //{
        //ros::spinOnce();
        //loop_rate.sleep();
    //}
    ros::spin();
}
