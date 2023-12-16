
#include "ImgNode.h"


#define CAM_ID 1


using namespace std;

ImgNode::ImgNode(ros::NodeHandle *nh):nh_(*nh){
  publog_ = nh_.advertise<all_msgs_srvs::systemlog>("/systemLog_info",10);
  subveh_ = nh_.subscribe<all_msgs_srvs::vehicalmsgs>("/mmlidar_veh", 1, &ImgNode::getVehsCb,this);
  subsetting_ =  nh_.subscribe<all_msgs_srvs::settings>("/ros_param_setting", 1, &ImgNode::getSettingCb,this);



  init_data();
  param_get();

  read_yaml(Lidar_height_);

  //thread for capture cam
  CaptureVideoThread(Width_get_,Height_get_);
  
  std::thread push_video_thread(&ImgNode::VideoPush, this);
  push_video_thread.detach();

  //std::thread play_video_thread(&ImgNode::play_video, this);
  //play_video_thread.detach();

  loop_while();


}

ImgNode::~ImgNode(){
  delete cap;
  cv::destroyWindow("Movie");
}

void ImgNode::init_data(){

  Is_save_img_and_lidardate = false;
  //Is_save_video = true;

  isopen_ = false;//cam is open

  pthread_mutex_init(&mutex_,&mutexattr_);//初始化线程锁

  PERVEHDATALEN_ =6;//ros消息中每个车辆信息长度，用于分割消息
  tcount_ = 2;//整个图片的标校调整区域划分，2代表可以分别调整两个区域，分别为来向和去向
  cli_offset_ = 16; //标校区域，每个宽16m，因此这个车道总宽tcount_ × cli_offset_
  Lidar_height_ = -1;//初始化默认高度为-1m，表示还没有加载
  isNewHei_ = false;//是否程序启动后，用户动态调整的高度行为标志
  
  dltaX_install = 0;//安装设备的位置距离安装远点的x偏移
  dltaY_install = 0;//安装设备的位置距离安装远点的y偏移

  Width_get_ = 640;
  Height_get_ = 480;
  Fps_get_ = 30;

  numSubBackground = 1;
  IfSubBackground = true;
}

//从ROS中获取参数并解析到类成员中
void ImgNode::param_get(){
    
    ros::param::get("img_param/width",Width_get_);//获取照片源的宽
    ros::param::get("img_param/height",Height_get_);//获取照片源的高
    ros::param::get("img_param/fps",Fps_get_);//获取照片源的zhen率
    ros::param::get("img_param/method",Method_);//获取照片的方线，0,1,2，3对应不旋转，旋转90，旋转180,旋转270
    // ros::param::get("img_param/bufflen",Queue_len);//获取照片储存队列的长度，结合Fps_get_,来确定和雷达数据的延时对齐
    ostringstream ss;
    ss<<"img_param:"<<Width_get_<<","<<Height_get_<<","<<Fps_get_<<","<<Method_;
    Log_pub(ss.str(),LOGTYPE::INFO);//发送log信息
    // cout << "-------------------------------------------------------------" << endl;
    // cout<<"img_param:"<<Width_get_<<","<<Height_get_<<","<<Fps_get_<<","<<Method_<<endl;
    // cout << "-------------------------------------------------------------" << endl;

    ros::param::get("display_param/width",Width_display_);//获取显示的宽度
    ros::param::get("display_param/height",Height_display_);//获取显示的高
    ros::param::get("display_param/isure",SureDisplay_);//获取确定是否展示的标志

    ros::param::get("save_param/isure",SureSave_);//获取确定是否储存的标志
    ros::param::get("save_param/iTimeName",TimeName_);//获取是否以时间来命令，如果不以时间命名，就用序号
    ros::param::get("save_param/path",SavePath_);//获取保存的路径
    ros::param::get("CLI_PATH",CliPath_);//计算用原始矩阵的文件路径

    ros::param::get("rect_param/isure",SureRect_);//获取是否要对照片进行标框
    ros::param::get("rect_param/scale_x_min",SCALE_MIN_X_);//过滤参数，显示的车辆x范围的最小值
    ros::param::get("rect_param/scale_x_max",SCALE_MAX_X_);//过滤参数，显示的车辆x范围的最大值
    ros::param::get("rect_param/scale_y_min",SCALE_MIN_Y_);//过滤参数，显示的车辆y范围的最小值
    ros::param::get("rect_param/scale_y_max",SCALE_MAX_Y_);//过滤参数，显示的车辆y范围的最大值
    ros::param::get("rect_param/rect_scale",scale_rect_);//获取，对框框的大小调整，比例系数，默认为1,如果需要整体变大，则可以设置大于1的系数
   
    ros::param::get("offset_h/car",offset_v_[0]);//补偿高度，例如小车空间坐标高度需要比地面高度高1米，可设置此参数调整
    ros::param::get("offset_h/bus",offset_v_[1]);//补偿高度，例如小车空间坐标高度需要比地面高度高2米，可设置此参数调整
    ros::param::get("offset_h/trian",offset_v_[2]);//补偿高度，例如小车空间坐标高度需要比地面高度高2.5米，可设置此参数调整


    //添加安装参数
    vector<float> install;//安装参数向量
    int auxcount;//标校参数的个数
    ros::param::get("/AuxiliaryParamCount",auxcount);
    ros::param::get("/MainInstallParam",install);
    install_ = install;//安装参数单独储存
    //installparam_.push_back(install_);

    for(int i=0;i<auxcount;i++){
      ostringstream ss;
      ss<<i+1;//标校序号从1开始
      string paramneame = "/AuxiliaryInstallParam/" + ss.str();
      ros::param::get(paramneame,install);//分别获取
      installparam_.push_back(install);//依次添加到标校列表
    }

    ros::param::get("/DeviceID",Id_);//获取设备id号码
    DeviceId_.uwords = (uint16_t)Id_;//赋值给类成员
    if(abs(Lidar_height_ - install_.at(2)) > 0.0001){//如果安装高度不等于获取的安装高度参数
      Lidar_height_ = install_.at(2);//雷达高度将赋值新的高度，即获取的安装高度参数
      isNewHei_ = true;//此时标志位为true，根据这个标志为，后面决定是否来加载新的变换矩阵
    }

    //安装参数各自储存到类成员
    Yaw_install_ = install_.at(3);//偏航角度yaw
    Pitch_install_ = install_.at(4);//俯仰角度pitch
    Roll_install_ = install_.at(5);//滚转角roll
    dltaY_install = install_.at(1);//Y 
    dltaX_install = install_.at(0);//X
    ss.str("");//清空
    ss<<"global install height:"<<Lidar_height_<<",yaw:"<<Yaw_install_<<",id:"<<Id_;
    Log_pub(ss.str(),LOGTYPE::INFO);   
    cout<<"global install height:"<<Lidar_height_<<",yaw:"<<Yaw_install_<<",id:"<<Id_<<endl;

    //int nLancount = 0;
    //ros::param::get("/AuxiliaryParamCount",nLancount);
    
    ros::param::get("/MyselfNetIp",UpperHost_);//从参数获取自己的ip，作为服务器用不上
    ros::param::get("/UpperCamNetPort",LongPort_);//获取视频传输的长连接的端口号码
    ros::param::get("tcp_param/isure",SureSendImg_);//获取确定发送图片的标志

    ros::param::get("camera_matrix_k/data",camera_k_);//获取相机内参数k

    K_33_ <<camera_k_[0],camera_k_[1],camera_k_[2],\
           camera_k_[3],camera_k_[4],camera_k_[5],\
           camera_k_[6],camera_k_[7],camera_k_[8];//赋值

    //调试，log
    ss.str("");
    ss<<"camera_matrix_k:data[3]:"<<camera_k_[3];
    Log_pub(ss.str(),LOGTYPE::INFO);
    cout<<"camera_matrix_k:data[3]:"<<camera_k_[3]<<endl;
    cout<<"param get over!"<<endl;
    Log_pub("param get over!",LOGTYPE::INFO);
}

/********************************************get_lidar_car_info**********************************************************/
void ImgNode::getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist){

    pthread_mutex_lock(&mutex_);
    vehmap_.erase(vehmap_.begin(),vehmap_.end());//接受消息前,先清空
    get_vehmap(vehicalist->vehicaldatas);//将数据写到map中，id为key
    pthread_mutex_unlock(&mutex_);

}
void ImgNode::get_vehmap(vector<float> vehs){
    int size = vehs.size();
    if(size < PERVEHDATALEN_)//数据小于一个车的信息长度，返回
      cout<<"veh_data failure!"<<endl;
    else{   

      // cout<<"veh_data size: "<< size <<endl;

        
      for(int i = 0; i < size; i = i + PERVEHDATALEN_){//对于信息，每PERVEHDATALEN_个数据取一辆车，循环


          // cout<<"veh_data size------------!"<< size <<endl;

          if(rect_filt(vehs[i+1],vehs[i+2],vehs[i+3],vehs[i+4],vehs[i+5])){//设置条件添加,过滤器
          // cout<<"veh_data size------------++++++++++++++++++!"<< size <<endl;
          // vector<float> listdata(5,-1);
          vector<float> listdata;
          listdata.insert(listdata.begin(),vehs[i+1]);//sort，类型s
          listdata.insert(listdata.begin()+1,vehs[i+2]);//lan，车道号
          listdata.insert(listdata.begin()+2,vehs[i+3]);//vy，速度
          listdata.insert(listdata.begin()+3,vehs[i+4]);//x，x值
          listdata.insert(listdata.begin()+4,vehs[i+5]);//y，y值
          vehmap_.insert(make_pair(vehs[i],listdata));//ID为key
          // cout<<"veh_data size------listdata.size()"<< listdata.size() <<endl;
          // cout << "vehs[i]:"<<vehs[i]<<endl;
          // cout<<"sort: "<<vehs[i+1]<<" lan: "<< vehs[i+2] << " vy: " << vehs[i+3] << " x:" <<vehs[i+4] << " y: "<<vehs[i+5]<<endl;
        }
      } 

      // cout << "vehmap_: " << vehmap_.size() <<endl;

    }
 }
bool ImgNode::rect_filt(float sort, float state, float vy, float x, float y){
      bool ret = true;
      //车型过滤 + 状态过滤 + 方向过滤 + 位置过滤
      // if (x <= SCALE_MIN_X_ || x >= SCALE_MAX_X_ || y <= SCALE_MIN_Y_ || y >= SCALE_MAX_Y_){
      //     //cout<<"rect"<<endl;
      //     ret = false;
      // }
      return ret;
}
/********************************************get_Upper_machine_Setting***************************************************/
//系统参数热修改的回调函数
void ImgNode::getSettingCb(const all_msgs_srvs::settingsConstPtr &sets){

  // std::cout << "##############ImgNode::getSettingCb####################ImgNode::getSettingCb##############################" << std::endl;
  if(sets->frame_id != DeviceId_.uwords){
    return;
  }

  if((int)sets->type == SettingType::INSTALL_INFO &&(int)sets->which_param == ROSParamType::INSTALL_SET_INFO_EVERY){//设置安装参数
    Log_pub("Setting install param in img_node",LOGTYPE::INFO);
    cout<<"Setting install param in img_node"<<endl;

    pthread_mutex_lock(&mutex_);//对矩阵上锁，防止修改错误
    //首先安装高度检查，如果高度发生变化，则需要重新取新的变换矩阵，如果不变，整体计算中，
    if(abs(sets->setdatas[3] - Lidar_height_) > 0.001){//
      isNewHei_ = true;
      install_.at(2) = Lidar_height_ = sets->setdatas[3];
    }
    //将新的安装参数加入到类成员中
    install_.at(3) = Yaw_install_ = sets->setdatas[4];
    install_.at(0) = dltaX_install = sets->setdatas[1];
    install_.at(1) = dltaY_install = sets->setdatas[2];
    install_.at(4) = Pitch_install_ = sets->setdatas[5];//pitch
    install_.at(5) = Roll_install_ = sets->setdatas[6];//roll

    //重新计算计算矩阵，根据高度是否变化来决定是否重新从文件中获取新的高度下的变换矩阵
    read_yaml(Lidar_height_);//整体计算

    pthread_mutex_unlock(&mutex_);//解锁

    //调试打印，log
    ostringstream ss;
    ss<<"install change H:"<<Lidar_height_<<",Yaw_install_:"<<Yaw_install_;
    cout<<"install change H:"<<Lidar_height_<<",Yaw_install_:"<<Yaw_install_<<endl;
    Log_pub(ss.str(),LOGTYPE::INFO);
  }
  else if((int)sets->type == SettingType::INSTALL_INFO &&(int)sets->which_param == ROSParamType::INSTALL_SET_CLI_EVERY){//设置标校参数
    cout<<"cli param setting cb! in img_node"<<endl;
    Log_pub("Setting Cli param in img_node",LOGTYPE::INFO);
    int lanum = (int)sets->setdatas[0]-1;//获取设置的是哪一个区域的标校参数

    pthread_mutex_lock(&mutex_);//上锁

    installparam_.at(lanum).clear();//将这个标校区域里的标校参数清理
    ostringstream ss;
    for(int i=1;i<sets->setdatas.size();i++){
      installparam_.at(lanum).push_back(sets->setdatas[i]);//将新设置的标校参数设置到相应的区域的类成员列表中
      ss<<"cli param["<<lanum<<"] ["<<i<<"] change:"<<installparam_.at(lanum).at(i-1)<<endl;
      cout<<"cli param["<<lanum<<"] ["<<i<<"] change:"<<installparam_.at(lanum).at(i-1)<<endl;
    }
    Log_pub(ss.str(),LOGTYPE::INFO);//log

    cal_E_44_(E_44_.at(lanum),installparam_.at(lanum));//将列表转换成矩阵
    C_34_.at(lanum) = K_33_ * T_34_.at(lanum) * E_44_.at(lanum);//重新计算本区域的计算矩阵

    pthread_mutex_unlock(&mutex_);//解锁
  }
  else if((int)sets->type == SettingType::NETWORK_INFO &&(int)sets->which_param == ROSParamType::NETWORK_ID_INFO_EVERY){//设置ID
    cout<<"enter set param in img_node,now this id is "<< DeviceId_.uwords<<endl;//调试
    Log_pub("Setting DeviceID param in img_node",LOGTYPE::INFO);//log

    DeviceId_.uwords = (uint16_t)sets->setdatas[0];//赋值

    //cout<<"enter set param in img_node,now set this id is "<< (uint16_t)sets->setdatas[0]<<","<<DeviceId_.uwords<<endl;//调试

    ostringstream ss;
    ss<<(int)(sets->setdatas[1])<<"."<<(int)(sets->setdatas[2])<<"."<<(int)(sets->setdatas[3])<<"."<<(int)(sets->setdatas[4])<<endl;
     UpperHost_ = ss.str();//赋值

    ss.str("");
    cout<<"network change ID:"<<DeviceId_.uwords<<",IP:"<<UpperHost_<<endl;//调试
    ss<<"network change ID:"<<DeviceId_.uwords<<",IP:"<<UpperHost_;
    Log_pub(ss.str(),LOGTYPE::INFO); //log   
  }
}
//将一维向量转换成矩阵
void assignment(Eigen::Matrix<double,3,4> &eig,vector<float> &tf_list){
  if(tf_list.size()==16){
     eig << tf_list[0],tf_list[1],tf_list[2],tf_list[3],\
           tf_list[4],tf_list[5],tf_list[6],tf_list[7],\
           tf_list[8],tf_list[9],tf_list[10],tf_list[11];//赋值
  }
}
//根据高度，动态加载参数文件中的参数，标志每个高度下都有一个比较好用的参数
void ImgNode::read_yaml(float lidar_height){
  if(isNewHei_){//如果是新设置的高度，便可以读取参数文件，刚启动和热参数设置安装参数的时候，改变来安装高度，便需要重新获取
    ostringstream ss;
    ss<<"修改高度:"<<lidar_height;
    Log_pub(ss.str(),LOGTYPE::INFO);   
    cout<<"修改高度:"<<lidar_height<<endl;

    ifstream fin(getYamlname(lidar_height));//根据文件名称，打开配置文件
    YAML::Node condig = YAML::Load(fin);//获取文件参数
    cout<<"取参数成功:"<<getYamlname(lidar_height)<<endl;
    cli_offset_ = condig["offset"].as<float>();//设置每个调整区域的宽度
    cout<<"转换成功"<<endl;
    tcount_ = condig["tcount"].as<int>();//设置调整区域的数量，2表示来去方向
    ss.str("");
    ss<<"cli_offset:"<<cli_offset_;
    Log_pub(ss.str(),LOGTYPE::INFO);      
    cout<<"cli_offset:"<<cli_offset_<<endl;
    vector<float> tf_list;

    //清理变换矩阵，马上要从参数文件中获取
    T_34_.clear();
    
    //清理误差补偿列表，马上要从参数文件中获取
    Error_.clear();
   //根据区域数量，获取参数
    for(int i=0; i<tcount_;i++)
    {
      ostringstream ss;
      ss<<i;
      tf_list = condig["data"][ss.str()].as<vector<float>>();//获取该区域的变换矩阵
      Eigen::Matrix<double,3,4> eig;
      assignment(eig,tf_list);//一维变二维
      cout<<"tf:["<<i<<"]:"<<eig<<endl;
      T_34_.push_back(eig);//放入类成员，变换矩阵

      tf_list = condig["error"][ss.str()].as<vector<float>>();
      Error_.push_back(tf_list); //该矩阵下计算的平均误差，用于最后计算补偿使用
    }
    isNewHei_ = false;//更新完成之后，就不是新高度来
  }
  //清理计算矩阵，计算矩阵 = 相机内参矩阵×变换矩阵×标校矩阵
  C_34_.clear();
  //清理标校矩阵
  E_44_.clear();

  //各计算区域内最终用于计算的计算矩阵，像素坐标 = 计算矩阵 × 车辆实际空间坐标
  for(int i=0;i<tcount_;i++){
    //转换标校参数列表，成为标校矩阵
    Eigen::Matrix<double,4,4> init_M44;
    E_44_.push_back(init_M44);
    cal_E_44_(E_44_.at(i),installparam_.at(i));

    //通过标校矩阵，内参矩阵和变换矩阵，得出用于计算像素坐标的计算矩阵
    cout<<"lan:"<<i<<endl<<"E_44"<<E_44_.at(i)<<endl;
    C_34_.push_back(K_33_ * T_34_.at(i) * E_44_.at(i));
    cout<<"T_34_["<<i<<"]:"<<T_34_.at(i)<<endl;
  }
  cout<<"yaml get over!"<<endl;
  Log_pub("yaml get over!",LOGTYPE::INFO);  
}
//返回雷达对应位置
string ImgNode::getYamlname(float lidar_height){
  int heiname = (int)(lidar_height * 1000);
  //if(heiname >=3750 && heiname < 4250)
      //heiname = 4000;
  if(heiname >=0) //目前测试，只加载4米高度的参数，如果有更多高度参数的补充，可以修改此处，适应更多的高度参数
      heiname = 4000;
  else if(heiname >=4250 && heiname < 4750)
      heiname = 4500;
  else if(heiname >=4750 && heiname < 5250)
      heiname = 5000;
  else if(heiname >=5250 && heiname < 5750)
      heiname = 5500;
  else if(heiname >=5750 && heiname < 6250)
      heiname = 6000;
  else if(heiname >=6250 && heiname < 6750)
      heiname = 6500;
  else if(heiname >=6750 && heiname < 7250)
      heiname = 7000;
  else if(heiname >=7250 && heiname < 7750)
      heiname = 7500;
  else if(heiname >=7750 && heiname < 8250)
      heiname = 8000;
  else if(heiname >=8250 && heiname < 8750)
      heiname = 8500;
  else if(heiname >=8750 && heiname < 9250)
      heiname = 9000;
  else if(heiname >=9250 && heiname < 9750)
      heiname = 9500;
  else if(heiname >=9750 && heiname < 10250)
      heiname = 10000;
  else if(heiname >=10250 && heiname < 10750)
      heiname = 10500;
  else if(heiname >=10750 && heiname < 11250)
      heiname = 11000;
  else if(heiname >=11250 && heiname < 11750)
      heiname = 11500;
  else if(heiname >=11750 && heiname < 12250)
      heiname = 12000;
  else
      heiname = 6000;
  ostringstream ss;
  ss<<heiname;
  cout<<"hei:"<<heiname<<endl;
  return CliPath_ + ss.str() + ".yaml";//配置文件的目录地点
}
//通过校准参数来计算标校准矩阵，E_44为校准矩阵
void ImgNode::cal_E_44_(Eigen::Matrix<double,4,4> &E_44,vector<float> &params){
  if(params.size() < 6)
    return;
  E_44 << 1,0,0,0,\
          0,1,0,0,\
          0,0,1,0,\
          0,0,0,1;//初始化
  float pi = 3.141592654;

  //yaw角度的变换
  float term = params[3] * pi / 180.0;
  Eigen::Matrix<double,4,4> R_z ;
  R_z << cos(term),-sin(term),0,0,\
        sin(term),cos(term),0,0,\
        0,0,1,0,\
        0,0,0,1;

  //roll滚转角的变换
  term = params[5] * pi / 180.0;
  Eigen::Matrix<double,4,4> R_x ;
  R_x<< 1,0,0,0,\
        0,cos(term),-sin(term),0,\
        0,sin(term),cos(term),0,\
        0,0,0,1;

  //pitch俯仰角的变换
  term = params[4] * pi / 180.0;
  Eigen::Matrix<double,4,4> R_y ;
  R_y<< cos(term),0,sin(term),0,\
        0,1,0,0,\
        -sin(term),0,cos(term),0,\
        0,0,0,1;

  //三轴位移的变换
  Eigen::Matrix<double,4,4> T_xyz ;
  T_xyz<< 1,0,0,params[0],\
          0,1,0,params[1],\
          0,0,1,params[2],\
          0,0,0,1;
  //分别左乘合并成一个矩阵
  E_44 = T_xyz * E_44;
  E_44 = R_y * E_44;
  E_44 = R_x * E_44;
  E_44 = R_z * E_44;
}

/*******************************************getCarXYZ********************************************************/
///给图片标框，根据宽高，矩形中点+矩形长宽
void ImgNode::rect_image_wh(Mat &drawFrame,double px1,double py1,double width,double height)
{
    if(!SureRect_){//是否要标框，如果配置不标框，则返回
      return;
    }
    if(width>100) width = height= 100;//框最大不超过100pix，这个需要调整
    cv::rectangle(drawFrame, Point(px1 - width/2,py1 - height/2),Point(px1 + width/2,py1 + height/2),Scalar(0,0,255),2,1,0);
    cv::circle(drawFrame, cv::Point(px1, py1), 5, cv::Scalar(255, 255, 0), -1);
    return;
}


//给图片写文字
void ImgNode::putText_image(Mat &drawFrame,string str, Point point)
{ 
  putText(drawFrame,str,point,FONT_HERSHEY_SIMPLEX,1,Scalar(255,23,0),1,1);//在图片上写文字
}

void ImgNode::filter_mask(cv::Mat &mask,cv::Mat &outputMask){

  cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(2,2));
  cv::Mat closing;
  cv::morphologyEx(mask,closing,cv::MORPH_CLOSE,element);
  cv::Mat opening;
  cv::morphologyEx(closing,opening,cv::MORPH_OPEN,element);
  cv::dilate(opening,outputMask,element,cv::Point(-1,-1),2);

}

float ImgNode::IOU(const cv::Rect rect,const cv::Rect rect1){

  float iou;
  cv::Rect rect2 = rect | rect1;
  cv::Rect rect3 = rect & rect1;
  iou = rect3.area()*1.0 / rect2.area();
  return iou;
}

void ImgNode::detect_vehicles(cv::Mat &fg_mask,cv::Rect &finaBox,const cv::Rect carRol){

  int offsetX = carRol.x;
  int offsetY = carRol.y;

  int min_contour_width = 17;
  int min_contour_height = 17;

  //gui contrast box
  int basisBoxcx = (int)fg_mask.size().width/2;
  int basisBoxcy = (int)fg_mask.size().height/2;
  int basisSquare = (int)fg_mask.size().width/1.5;
  cv::Rect basisbox(basisBoxcx-basisSquare,basisBoxcy-basisSquare,basisSquare,basisSquare);

  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;
  findContours( fg_mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_L1 );

  vector<vector<Point> > contours_poly( contours.size() );
  vector<cv::Rect> boundRect(contours.size());

  for( size_t i = 0; i< contours.size(); i++ )
  {
    cv::approxPolyDP( contours[i], contours_poly[i], 3, true );
    boundRect[i] = boundingRect( contours_poly[i] );

    if(boundRect[i].width>=min_contour_width && boundRect[i].height>=min_contour_height){
      boundRect.erase (boundRect.begin()+i);
      }
  }

  /////find the most suitable box
  //find many box in croped fg_mask
  if(boundRect.size() > 1){
    vector<float> IOUlist(boundRect.size());
    for(size_t i =0; i<boundRect.size();i++){
      cv::Rect box_ = boundRect[i];
      float iou = IOU(basisbox,box_);
      IOUlist.at(i)=iou;
    }
    int maxPosition = max_element(IOUlist.begin(),IOUlist.end()) - IOUlist.begin();
    // cout << "boundRect.size: "<<boundRect.size()<<endl;
    // cout << "IOUlist.size: "<<IOUlist.size()<<endl;
    // cout << "maxPosition: "<<maxPosition<<endl;
    //find biggest iou with basisbox
    if(IOUlist.at(maxPosition)!=0){
      finaBox.x = boundRect.at(maxPosition).x+offsetX;
      finaBox.y = boundRect.at(maxPosition).y+offsetY;
      finaBox.width = boundRect.at(maxPosition).width;
      finaBox.height = boundRect.at(maxPosition).height;

    }
    //when all box have no iou with basisbox,and give basisbox to finaBox
    else{
      finaBox.x = basisbox.x+offsetX;
      finaBox.y = basisbox.y+offsetY;
      finaBox.width = basisbox.width;
      finaBox.height = basisbox.height;
    }
  }
  else{
    //boundRect only one box
    if(boundRect.size() == 1){
      finaBox.x = boundRect.at(0).x+offsetX;
      finaBox.y = boundRect.at(0).y+offsetY;
      finaBox.width = boundRect.at(0).width;
      finaBox.height = boundRect.at(0).height;
    }
    //boundRect no box 0
    else{
      finaBox.x = basisbox.x+offsetX;
      finaBox.y = basisbox.y+offsetY;
      finaBox.width = basisbox.width;
      finaBox.height = basisbox.height;
    }

  }
}

void ImgNode::getCarXYZ(cv::Mat &drawFrame,ofstream &imgmisspointtxtfile){
  if(!SureRect_){//配置，确定是否打框操作
    return;}

  // scale_rect_ = 6;

  double vehs_x1 ,vehs_y1,vehs_z1;//车辆的三轴坐标
  bool docrop = false;
  for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_)//迭代取出已经解析好的车辆信息
  {
      float offset = 0;
      vehs_x1 = mapit_->second[3];//车辆的x值
      vehs_y1 = mapit_->second[4];//车辆的y值

      float x = vehs_y1;

      int lannum = (vehs_x1 + cli_offset_ * tcount_/2.0)/cli_offset_; //通过x值来计算，该车属于哪一个标校区域，每一个标校区域内的计算矩阵是不一样的。计算所在标校区域代号

      if(lannum<0)//如果计算小于零，就按照最小序号0来计算
        lannum = 0;
      if(lannum>=tcount_)//如果计算大于最大区域号码，则按最大的来，标校区域从0开始算，所以-1
        lannum = tcount_-1;
      int pixoffset[2] = {0};//框框像素补偿值，初始化为零,分别标示x,y的补偿值
      float square = 0; //边框长度初始化为零

      if((int)mapit_->second[0] == CarType::CAR){//小车

          vehs_z1 = -install_.at(2) + offset_v_[0];//小车中心距离地面上调offset_v_[0]，默认1米，可以设置  
          // cout << "+================================++=++==================================" << endl;
          // cout << "vehs_z1: "<<vehs_z1<<endl;
          //给小车配，当前区域的像素补偿值
          pixoffset[0] = Error_.at(lannum).at(0);//x的补偿值
          pixoffset[1] = Error_.at(lannum).at(1);//y的补偿值
          if(x>300)//当距离太远，大于135m,我们直接赋值最小的20个像素的框
            {square = 20 ;docrop = false;}
          else  
            {
            // square = (vehs_y1 * vehs_y1 * 0.0041 - 1.122 * vehs_y1 + 95.85) * scale_rect_;
            square = 1.811e-07 *x*x*x*x - 0.0001549 * x*x*x + 0.04777 * x*x - 6.407 * x + 354.7;
            docrop = true;
            }


      }else{//除了小车已外的其他车型，中车和大车
          vehs_z1 = -install_.at(2) + offset_v_[1];//大车中心距离地面上调offset_v_[1]，默认2米，可以设置
          //大车的像素补偿值
          pixoffset[0] = Error_.at(lannum).at(2);//x的补偿值
          pixoffset[1] = Error_.at(lannum).at(3);//y的补偿值

          if(x > 300)//如果距离大于120米，我们给赋最小值
            {square = 25 ;docrop = false;}
          else
            {
            // square = (vehs_y1 * vehs_y1 * 0.017 - 4.09 * vehs_y1 + 284.2) * scale_rect_;//框大小的计算，同样的道理，我们是利用一元二次方程来近似计算的    
            square = 1.811e-07 *x*x*x*x - 0.0001549 * x*x*x + 0.04777 * x*x - 6.407 * x + 354.7;
            docrop = true;
            }
      }

      Vector3d v_3f1(vehs_x1,vehs_y1,vehs_z1);//将三个值，变成eigen向量类型
      Eigen::Matrix<double,4,1> v41(v_3f1(0),v_3f1(1),v_3f1(2),1);//将坐标变成4×1的齐次类型
      v_3f1 = C_34_.at(lannum) * v41; //三轴坐标通过齐次类型 × 计算矩阵，得到像素坐标（x,y,z）
      v_3f1 = v_3f1 / v_3f1(2);//同除以z，进行归一化，得到（x,y,1） 


      int pixel_X = v_3f1(0) - pixoffset[0];
      int pixel_Y =  v_3f1(1) - pixoffset[1];
 
      int pixel_W = square;
      int pixel_H = square;

      // ofstream imgmisspointtxtfile; 
      // imgmisspointtxtfile.open("/home/hsax/camera_lidar_clibration/img_miss_mmlidar_point.txt");
      
      if(Is_save_img_and_lidardate){
      	if(pixel_X < 0 || pixel_Y < 0 || pixel_X>1920 || pixel_Y>1080){
            string strname;
            ostringstream timeStamp;
            timeStamp << getTimeStamp();
            strname = timeStamp.str(); 
            imgmisspointtxtfile << "------------------------------------------" << "\n";
            imgmisspointtxtfile << strname +".jpg"<< "\n";   
            imgmisspointtxtfile <<to_string(vehs_x1)<<";"<<to_string(vehs_y1)<<";"<<to_string(pixel_X)<<";"<<to_string(pixel_Y)<<"\n";
          } 
        }


      if(IfSubBackground && docrop && !fgMask.empty()){ 
        // crop fgMask and detect car box
        int cropx = abs((int)pixel_X/2);
        int cropy = abs((int)pixel_Y/2);
        int cropw = abs((int)square/2);
        int croph = abs((int)square/2);

        // cout << "cropx: " << cropx << " cropy: "<< cropy << " croph_w: " << cropw << endl;
        // cout << " fgMask.width: " << fgMask.size().width << endl; 
        // cout << " fgMask.height: " << fgMask.size().height << endl; 
        if(cropx >= (int)fgMask.size().width || cropy >= (int)fgMask.size().height){
          cout << "+===================================================+" << endl;
          cout << "radar_vehs_x1: " << vehs_x1 << " ,radar_vehs_y1: " << vehs_y1 << " ,radar_vehs_z1: " << vehs_z1 << endl;
          cout << "out piex cropx: " << cropx << " ,out piex cropy: "<< cropy << " ,croph_w: " << cropw << endl;
          continue;
        }
        if((cropx+cropw) > (int)Width_get_/2){
          cropw = fgMask.size().width - cropx;
        }
        if((cropy+croph) > (int)Height_get_/2){
          croph = fgMask.size().height - cropy;
        }

        cv::Rect carRol(cropx,cropy,cropw,croph);
        cv::Mat cropedfgMask = fgMask(carRol);
        cv::Mat filtedMask;
        filter_mask(cropedfgMask,filtedMask);
        cv::Rect subBackgroundBox;
        detect_vehicles(filtedMask,subBackgroundBox,carRol);

        // cout << "subBackgroundBox_x: " << subBackgroundBox.x <<endl;        
        // cout << "subBackgroundBox_y: " << subBackgroundBox.y <<endl;
        // cout << "subBackgroundBox.width: " << subBackgroundBox.width <<endl;
        // cout << "subBackgroundBox.height: " << subBackgroundBox.height <<endl;
        // // <<subBackgroundBox.width,subBackgroundBox.height<<endl;
        pixel_X = subBackgroundBox.x * 2;
        pixel_Y = subBackgroundBox.y * 2;
        pixel_W = subBackgroundBox.width * 2;
        pixel_H = subBackgroundBox.height * 2;
        // imshow("cropedfgMask",cropedfgMask);
        // imshow("filtedMask",filtedMask);
        // std::cout <<  "do crop" << std::endl;
      } 
      // cout << "v_3f1" << v_3f1.size() <<endl;
      //给图片打匡
      rect_image_wh(drawFrame,pixel_X,pixel_Y,pixel_W,pixel_H);

      ostringstream ss;
      ss<<(int)(abs(mapit_->second[2]));//将速度vy也写在图片上
      putText_image(drawFrame,ss.str(), Point(pixel_X - pixel_W/2.0,pixel_Y - 5));//写文字
  }
}
/*******************************************main_loop_while********************************************************/
void ImgNode::loop_while(){


  while(nh_.ok()){

  }
}

void ImgNode::loop_while_playVideo(){
  cv::Mat showFrame;
  int rows = showFrame.rows;
  int cols = showFrame.cols;
  while(nh_.ok()){
    ros::spinOnce(); //call subscribe callback funcation 
    //img display show
    main_pMutex.lock();
    frame.copyTo(showFrame);
    main_pMutex.unlock();

    // getCarXYZ();

    cv::Size s = showFrame.size();
    rows = s.height;
    cols = s.width;
    // std::cout << "height：" <<  rows << " width" << cols << std::endl;  
    if(rows!=0 && cols!=0){
      imshow("Movie",frame);
      if(cv::waitKey(33)==27){
        cap->ifShutdownCam = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;}}
  }
}

/******************************************camera_start************************************************************/
void ImgNode::init_camera(cv::Mat &frame, std::mutex &main_pMutex,const int VideoWidth,const int VideoHeight){
  
  std::cout << "VideoWidth:" <<  VideoWidth << "VideoHeight" << VideoHeight << std::endl;
  fgbg = cv::createBackgroundSubtractorMOG2();
  cap = new Capture(CAM_ID,VideoWidth,VideoHeight,frame,main_pMutex,fgbg,numSubBackground,fgMask);
  isopen_  = cap->m_IsOpen;

  if(isopen_){
    if(IfSubBackground){cap->TrainbgSub();cap->read_cam_subBack();}
    else{cap->read_cam();}
  }
  else{
    std::cout << "Failed to open camera" << std::endl;
  }
}
void ImgNode::CaptureVideoThread(const int VideoWidth,const int VideoHeight){
 //thread for capture cam
  frame = cv::Mat(cv::Size(VideoWidth,VideoWidth), CV_8UC3);
  std::thread init_thread_cam(&ImgNode::init_camera, this,std::ref(frame),std::ref(main_pMutex),VideoWidth,VideoHeight);
  init_thread_cam.detach();
  std::this_thread::sleep_for(std::chrono::seconds(1));
}


void ImgNode::Log_pub(string logstr,uint32_t type){
  all_msgs_srvs::systemlog systemlog_;
  systemlog_.device_id = DeviceId_.uwords;
  systemlog_.node_id = "imgup_node";
  systemlog_.type = type;
  systemlog_.describe = logstr;
  for(int i=0;i<null_vector_.size();i++){
    systemlog_.data.push_back(null_vector_.at(i));
  }
  vector<float> null_v;
  null_vector_= null_v;
  publog_.publish(systemlog_);
}

/*****************************************************************************************************************/
// test for img streamer
void ImgNode::drawBox(cv::Mat &frame){
  // while(1){
  cv::circle(frame, cv::Point(200, 300), 100, cv::Scalar(223, 12, 115), 7, 8);
  // }
}
void ImgNode::VideoPush(){

  cv::Mat pushFrame;
  
  std::string user = "user";
  std::string passwd ="123456";

  // Width_get_
  // Height_get_

  strStream = new Stream(user,passwd,Width_get_,Height_get_);
  strStream->addEndpoint("/test", pushFrame); 
  std::thread stream_thread(&Stream::thread, strStream);

  int count = 0;

  string SavePath_ = "/home/hsax/camera_lidar_clibration/save_img";

  ofstream lidartxtfile; 
  lidartxtfile.open("/home/hsax/camera_lidar_clibration/2021-4-6_mmlidar_xd.txt") ;
  
  VideoWriter writer;
  int codec = cv::VideoWriter::fourcc('D','I','V','X');
  writer.open("/home/hsax/camera_lidar_clibration/save_video.avi",codec,30.0,Size(1920,1080),true);

  ofstream imgmisspointtxtfile; 
  imgmisspointtxtfile.open("/home/hsax/camera_lidar_clibration/img_miss_mmlidar_point.txt");
 
  double traceID_ , sort_, lan_ ,vy_ , x_ ,y_;
 
  time_t tt;

  while(nh_.ok()){

    ros::spinOnce();

    count = count + 1;

    main_pMutex.lock();
    frame.copyTo(pushFrame);
    
    //timestamp on push frame
    time( &tt );
    tt = tt + 8*3600;
    tm* t= gmtime( &tt );
    ostringstream time_str;
    time_str<<t->tm_mon + 1<<"month "<<t->tm_mday<<"day "<<t->tm_hour<<"hour "<<t->tm_min<<"min "<<t->tm_sec<<"sec ";
    putText_image(pushFrame,time_str.str(), Point(20,100));//写文字


    if(Is_save_img_and_lidardate){
    //save pushFrame img
      string strname;
      ostringstream imgss;
      imgss << getTimeStamp() << "--"<<to_string(count);
      strname = imgss.str();      
	
      lidartxtfile << "------------------------------------------" << "\n";
      lidartxtfile << strname +".jpg"<< "\n";   

      for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_){
   
        traceID_ = mapit_->first;
        sort_ = mapit_->second[0];
      	lan_ = mapit_->second[1]; 
      	vy_ = mapit_->second[2];
      	x_ = mapit_->second[3];//车辆的x值
      	y_ = mapit_->second[4];//车辆的y值

      	lidartxtfile <<to_string(traceID_)<<";"<<to_string(sort_)<< ";"<<to_string(lan_)<<";"<<to_string(vy_)<<";"<<to_string(x_)<<";"<<to_string(y_)<<"\n";
      	}
      cv::imwrite(SavePath_ + "/"+strname + ".jpg",frame);
      //writer << pushFrame;
    }

    getCarXYZ(pushFrame,imgmisspointtxtfile);
    cv::circle(pushFrame, cv::Point(200, 300), 100, cv::Scalar(223, 12, 115), 7, 8);
    main_pMutex.unlock();
    
   
    std::this_thread::sleep_for(std::chrono::milliseconds(80)); 
    }
 }

//test  
void ImgNode::play_video(){

  std::this_thread::sleep_for(std::chrono::seconds(1));

  cv::Mat showFrame;
  cv::Mat subbackFrame;

  ofstream imgmisspointtxtfile; 
  imgmisspointtxtfile.open("/home/hsax/camera_lidar_clibration/play_.txt");
 

  time_t tt;
  int rows = showFrame.rows;
  int cols = showFrame.cols;
  while (1){
    main_pMutex.lock();
    frame.copyTo(showFrame);
    main_pMutex.unlock();

    // if(showFrame.empty()){break;}
    cv::Size s = showFrame.size();
    rows = s.height;
    cols = s.width;

    time( &tt );
    tt = tt + 8*3600;
    tm* t= gmtime( &tt );
    ostringstream time_str;
    time_str<<t->tm_mon + 1<<"month "<<t->tm_mday<<"day "<<t->tm_hour<<"hour "<<t->tm_min<<"min "<<t->tm_sec<<"sec ";
    putText_image(showFrame,time_str.str(), Point(20,100));//写文字

    ros::spinOnce();
    getCarXYZ(showFrame,imgmisspointtxtfile);

    cv::circle(showFrame, cv::Point(200, 300), 100, cv::Scalar(223, 12, 115), 7, 8);

    if(rows!=0 && cols!=0){

      cv::Mat result_show;
      cv::resize(showFrame,result_show,cv::Size(960,540));

      imshow("Movie",result_show);

      // if(IfSubBackground && !FinishSubBackTrain){imshow("fgMask",fgMask);}
      if(IfSubBackground && !fgMask.empty()){imshow("fgMask",fgMask);}
      

      if(cv::waitKey(33)==27){
        cap->ifShutdownCam = true;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        break;
      }
    }
    
    // std::cout << "in ImgNode" << std::endl;

  }
}
/************************************************end**************************************************************/

///获取绝对时间戳
std::time_t ImgNode::getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
    std::time_t timestamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
    return (timestamp-1589000000000)/10;//这里是取后面9位数，并省略一个精度
    //return timestamp;//这里是取后面9位数，并省略一个精度
}
