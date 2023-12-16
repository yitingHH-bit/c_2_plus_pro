#include <ros/ros.h>
#include <ImgNode.h>

using namespace std;

int main(int argc, char** argv){
    ros::init(argc, argv, "pubimage");
    ros::NodeHandle nh;
    ImgNode imgNode(&nh);
}




    // VideoCapture capture("/home/lee/video.mp4");

    // // capture.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M','J','P','G'));

    // if(!capture.isOpened())
    // {
    //     cout<<"Movie open Error"<<endl;
    //     return -1;
    // }
    // Mat frame;
    // namedWindow("Movie Player");
    // while(1)
    // {
    //     if(!capture.read(frame))
    //         break;

    //     imshow("Movie Player",frame);
    //     char c=waitKey(33);
    //     if(c==27)
    //         break;
    // }
    // capture.release();
    // destroyWindow("Movie Player");


    // return 0;