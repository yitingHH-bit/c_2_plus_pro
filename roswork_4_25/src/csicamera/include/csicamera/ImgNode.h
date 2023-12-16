
#ifndef _IMGNODE_H_
#define _IMGNODE_H_


#include "ros/ros.h"
#include <all_msgs_srvs/systemlog.h>

#include <iostream>
#include <time.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

#include <thread>
// #include <pthread.h>
#include <mutex>
#include <atomic>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/mat.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
// #include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <yaml-cpp/yaml.h>
#include <queue>

#include "capture.h"
#include "stream.h"
#include "tcpcontrol.h"

#include <all_msgs_srvs/systemlog.h>
#include <all_msgs_srvs/vehicalmsgs.h>
#include <all_msgs_srvs/settings.h>


using namespace std;
using namespace cv;//opencv库
using namespace Eigen;//Eigen库

// #define STREAM_WIDTH  1920
// #define STREAM_HEIGHT 1080

// static std::mutex mutex;

class ImgNode{

public:
	ImgNode(ros::NodeHandle *nh);
	~ImgNode();

private:
    ros::NodeHandle nh_; 
    ros::Subscriber subveh_;        //accept car real time info topic
    ros::Subscriber subsetting_;    //accept setting topic
    ros::Publisher publog_;         //publisher log


    cv::Mat frame;
    std::mutex main_pMutex; //add thread lock
    cv::Mat pushFrame;
    Capture *cap;
    Stream *strStream;
    std::atomic_bool isPushStream;

    // queue<Mat> Q_Frames_;//图片队列
    // int Queue_len;//读取的队列长度
    
    bool Is_save_img_and_lidardate;
    bool Is_save_video;
    bool isopen_;//相机是否打开的标志

    bool SureSendImg_;//标志是否传送照片数据

    //图像显示参数
    int Width_display_;//图像宽
    int Height_display_;//图像高
    bool SureDisplay_;//显示的标志
    //图像获取参数
    int Width_get_;//获取图像的宽
    int Height_get_;//获取图像的高
    int Fps_get_;//获取图像的频率
    int Method_;//获取图像的方位，默认为0 ，如果需要旋转90度，则为1,依次类推
    bool SureRect_ = true;//是否需要在图片上标框

    //网络参数
    string UpperHost_;//如果本机是客户端，将会用到，如果本机是服务端，将不会用到
    int LongPort_;//图像传输的长连接的端口号
    int Id_;//设备id的过度变量，包含来设备方位和编号
    uWord_16 DeviceId_;//设备id ,包含了设备的方位和编号


    //保存参数
    bool SureSave_;//是否保存的标志
    string SavePath_;//保存的路径
    bool TimeName_;//是否按照时间来命名
    int nameIndex_;//序号命名的名称

    string CliPath_;//计算用原始矩阵路径


    vector<float> camera_k_;//相机内参数据列表，接受ros数据用

    vector<float> install_;//安装参数列表

    vector<vector<float> > installparam_;//标校参数列表

    bool isNewHei_;//高度是否为新的标志，如果是新的，需要从配置文件中重新载入变换矩阵
    float Lidar_height_;//雷达架高
    float dltaY_install;//与标准位置(测初始参数位置)雷达前后距离上的差别
    float dltaX_install;//与标准位置(测初始参数位置)雷达在左右距离上的差别
    float Yaw_install_;//雷达安装偏航角
    float Pitch_install_;//俯仰角度
    float Roll_install_;//滚转角度    

    float cli_offset_;//标校带的宽度
    int tcount_;//标校带的分道数，总的标校区域宽度最好不要超过32米

    int PERVEHDATALEN_;//车辆实时信息数据列表中，分给每一个车的数据长度，默认为6

    float offset_v_[3] = {1.0,2.0,2.5};//像素计算前，需要对车辆的高度进行一个相对于地面的提升补偿

    map<int,vector<float> > vehmap_;//雷达检测车辆的信息
    map<int, vector<float> >::iterator mapit_;//迭代器

        //数据过滤的区域，实际的范围单位m
    float SCALE_MIN_X_;
    float SCALE_MAX_X_;
    float SCALE_MIN_Y_;
    float SCALE_MAX_Y_;
    float scale_rect_;//图像标框长度系数，默认为1,如果整体放大设置>1


    Eigen::Matrix<double,3,3> K_33_;//相机内参模型，从相机坐标转换成像素坐标
    vector<Eigen::Matrix<double,3,4> > T_34_;//相机位置的变换矩阵，从雷达三维坐标到相机坐标的转化
    vector<vector<float> > Error_;//计算出的像素坐标，通过平均误差来进行补偿，得到最后的像素坐标
    vector<Eigen::Matrix<double,4,4> > E_44_;//修正参数矩阵，通过标校参数，得出修改矩阵
    vector<Eigen::Matrix<double,3,4> > C_34_;//计算投影矩阵，用于最后的计算，等于K_33_ × T_34_ × E_44_



    pthread_mutex_t mutex_;  //线程锁
    pthread_mutexattr_t mutexattr_;//init

    vector<float> null_vector_;

    //sub Background 

    Ptr<BackgroundSubtractor> fgbg;
    int numSubBackground;
    bool IfSubBackground;
    cv::Mat fgMask;

    void filter_mask(cv::Mat &mask,cv::Mat &outputMask);
    void detect_vehicles(cv::Mat &fg_mask,cv::Rect &finaBox,const cv::Rect carRol);
    float IOU(const cv::Rect rect,const cv::Rect rect1);


    void init_data();//初始化参数
    void param_get();//开机获取ROS参数


    //main thread 
    void loop_while_playVideo();
    void loop_while_pushVideo();

    void loop_while();

    //video process
    void init_camera(Mat &frame, mutex &main_pMutex,const int VideoWidth,const int VideoHeight);
    void CaptureVideoThread(const int VideoWidth,const int VideoHeight);

    void drawBox(Mat &frame);

    void VideoPush();

    void play_video();


    void Log_pub(string logstr,uint32_t type);

    void getCarXYZ(cv::Mat &drawFrame,ofstream &imgmisspointtxtfile);

    void rect_image_wh(Mat &drawFrame,double px1,double py1,double width,double height);//长宽标框
    void putText_image(Mat &drawFrame,string str, Point point);//文字显示
    // void rect_image(double px1,double py1,double px2,double py2);//两点标框


    /////////////////////////////////////////////////////////////////////////////////////////
    ///////getVehsCb////////////////getVehsCb/////////////getVehsCb//////////////getVehsCb///
    void getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist);//实施车辆接受函数
    void get_vehmap(vector<float> vehs);//在线车辆ros消息解析
    bool rect_filt(float sort, float state, float vy, float x, float y);//标框原始数据过滤器
    ///////getVehsCb////////////////getVehsCb/////////////getVehsCb//////////////getVehsCb///
    /////////////////////////////////////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////////////////////////////////////
    ///////getSettingCb////////////////getSettingCb/////////////getSettingCb//////////////getSettingCb///
    void getSettingCb(const all_msgs_srvs::settingsConstPtr &sets);//热参数设置函数
    void read_yaml(float lidar_height);//加载yaml文件，读取参数，并计算 计算矩阵
    string getYamlname(float lidar_height);//获取坐标映射的配置参数文件的名称
    void cal_E_44_(Eigen::Matrix<double,4,4> &E_44,vector<float> &params);//将数列变换成eigen矩阵
    ///////getSettingCb////////////////getSettingCb/////////////getSettingCb//////////////getSettingCb///
    /////////////////////////////////////////////////////////////////////////////////////////

    std::time_t getTimeStamp();

};

#endif
