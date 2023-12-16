/*
 * Show Line of CARLA 0.9.1 Town1
 */

#include <MapVeh.h>

namespace veh
{
  MapVel::MapVel(ros::NodeHandle nh):
    nh_(nh)
 {
     ros::Time::init();
     veh_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("Map_vehicals", 10);
     txt_pub_ = nh_.advertise<visualization_msgs::MarkerArray>("Map_txts", 10);
     time_pub_ = nh_.advertise<visualization_msgs::Marker>("Map_txtTime", 10);
    //接收车辆信息
    get_veh_sub_ = nh_.subscribe<all_msgs_srvs::vehicalmsgs>("/mmlidar_veh", 1, &MapVel::vehicalCb, this);
    data_init();

 }
 MapVel::~MapVel()
 {
      
 }

   void MapVel::data_init()
  {
    bugindex_ = 1;
    shape_ = visualization_msgs::Marker::CUBE;
    //y_ = 0;

    LIDAR2GROUDZ = 5.7;
    PERVEHDATALEN=6;
    SCALE_MIN_X = -50;
    SCALE_MAX_X = 50;
    SCALE_MIN_Y = 0;
    SCALE_MAX_Y = 500;



    nh_.getParam("scale_min_x",SCALE_MIN_X);
    nh_.getParam("scale_max_x",SCALE_MAX_X);
    nh_.getParam("scale_min_x",SCALE_MIN_Y);
    nh_.getParam("scale_max_y",SCALE_MAX_Y);

  }
///周期更新隐身的车辆
  void MapVel::loop_pub()
  {
    //显示处理，包括预测和透明更新
    ROS_INFO("0");
    display_map();
    ROS_INFO("1");
    if(bugindex_++%50 == 0) 
    {
        unMap();
        bugindex_ = 1;
    }
    ROS_INFO("2");
    veh_pub_.publish(vehs_);
    vehs_.markers.clear();
    ROS_INFO("3");
    txt_pub_.publish(txts_);
    txts_.markers.clear();
    ROS_INFO("4");
  }


///检查更新在线车辆列表，返回是否存在
  bool MapVel::isExist(float id ,float type ,float state ,float vy ,float x ,float y)
  {
    bool ret = false;
    for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_)
    {
       //cout<<"itera:"<<id<<endl;
       if((int)(mapit_->first) == (int)id)
       {
         cout<<"update:"<<id<<endl;
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
         //if(state == 0 || abs(vy - 0)< 0.00001)//已经添加的，静止的车辆移除
            // mapit_->second[5] = 0;
         break;
       }
    }
    return ret;
  }

///解析在线车辆信息list
  void MapVel::get_vehs_data(vector<float> vehs, int time)
  {
    int size = vehs.size();
    //cout<<size<<endl;
    if(size < PERVEHDATALEN)
      cout<<"veh_data failure!"<<endl;
    else{     
      for(int i = 0; i < size; i = i + PERVEHDATALEN){
        if(!isExist(vehs[i],vehs[i+1],vehs[i+2],vehs[i+3],vehs[i+4],vehs[i+5])){
          //cout<<i<<endl;
          if(vehs[i+5] <31){//静止单位车辆不添加
             //cout<<"remove!"<<endl;
             continue;
          }
          vector<float> listdata(9,-1);
          listdata.insert(listdata.begin(),vehs[i+1]);//sort
          listdata.insert(listdata.begin()+1,vehs[i+2]);//lan
          listdata.insert(listdata.begin()+2,vehs[i+3]);//vy
          listdata.insert(listdata.begin()+3,vehs[i+4]);//x
          listdata.insert(listdata.begin()+4,vehs[i+5]);//y
          listdata.insert(listdata.begin()+5,1.0);//是否在线
          listdata.insert(listdata.begin()+6,-1.0);//上个位置的信息
          listdata.insert(listdata.begin()+7,-1.0);
          listdata.insert(listdata.begin()+8,time);
          //vehmap_.insert(make_pair<int,vector<float> >(vehs[i],listdata));//ID为key
          vehmap_.insert(make_pair(vehs[i],listdata));//ID为key
          ROS_INFO("Insert data");
        }
      }
    }
  }
//丢弃
  void MapVel::unMap()
  {
    for(mapdelit_ = vehdelmap_.begin();mapdelit_ != vehdelmap_.end();++mapdelit_)
    {
      make_vel(mapdelit_->second[0],mapdelit_->second[3],mapdelit_->second[4],mapdelit_->second[6],mapdelit_->second[7],"my_frame",mapdelit_->first,mapdelit_->second[5],false);
    }
    vehdelmap_.clear();
  }
//更新显示
  void MapVel::display_map()
  {
     //显示
    for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_)
    {
      if((int)mapit_->second[5] == 0)
      {
        vehdelmap_.insert(make_pair(mapit_->first,mapit_->second));
        vehmap_.erase(mapit_);//直接删除
        continue;
        if(mapit_->second[3] >= SCALE_MIN_X && mapit_->second[3] <= SCALE_MAX_X && mapit_->second[4] >= SCALE_MIN_Y && mapit_->second[4] <= SCALE_MAX_Y || abs(mapit_->second[2] - 0) < 0.0001 || mapit_->second[1] == 0)
        {
           mapit_->second[6] = mapit_->second[3];//更新上一时刻的位置信息
           mapit_->second[7] = mapit_->second[4];
           mapit_->second[4] = mapit_->second[4] + mapit_->second[2] * (1.0 / 1);
        }
        else
        {
           vehdelmap_.insert(make_pair(mapit_->first,mapit_->second));
           vehmap_.erase(mapit_);
        }
      }
      //过滤250米以外的目标和+20m以外的目标
      //if(mapit_->second[3] > 10 || mapit_->second[4] > 150)
          //continue;
      make_vel(mapit_->second[0],mapit_->second[3],mapit_->second[4],mapit_->second[6],mapit_->second[7],"my_frame",mapit_->first,mapit_->second[5],true, mapit_->second[8]);
      mapit_->second[5] = 0;
    }
  }


  void MapVel::vehicalCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist)
  {
    ROS_INFO("get_veldata");
    //检查更新数据
    get_vehs_data(vehicalist->vehicaldatas, (int)vehicalist->stamp.sec);    
    loop_pub();
  }

  void MapVel::make_txt(float x , float y, float z, string frameID, int carID, bool isadd ,int time)
  {
     txtid_.header.frame_id = frameID;
     txtid_.header.stamp = ros::Time::now();
     txtid_.ns = "basic_shapes";
     txtid_.id = carID;
 
     txtid_.type = visualization_msgs::Marker::TEXT_VIEW_FACING;
     if(isadd)
       txtid_.action = visualization_msgs::Marker::ADD;
     else
       txtid_.action = visualization_msgs::Marker::DELETE;
 
     txtid_.pose.position.x = x;
     txtid_.pose.position.y = y + 5;
     txtid_.pose.position.z = z;
     txtid_.pose.orientation.x = 0.0;
     txtid_.pose.orientation.y = 0.0;
     txtid_.pose.orientation.z = 0.0;
     txtid_.pose.orientation.w = 1.0;

     //txtid_.scale.x = shape_wid;
     //txtid_.scale.y = 0.1;
     txtid_.scale.z = 6;
 
     txtid_.color.r = 255;
     txtid_.color.g = 23;
     txtid_.color.b = 17;
     txtid_.color.a = 1.0;

     float vehs_x = x;
     float vehs_y = y;
     ///float vehs_y = -LIDAR2GROUDZ;
     //double temp = y*y - x*x-LIDAR2GROUDZ * LIDAR2GROUDZ;
     //if(temp < 0)
         //return;
     float vehs_z = LIDAR2GROUDZ;

     ostringstream ss ;
     //ss<<carID<<"("<<x<<","<<y<<")";
     //ss<<carID<<"("<<vehs_x<<","<<vehs_y<<","<<vehs_z<<")";
     ss<<"("<<vehs_x<<","<<vehs_y<<","<<vehs_z<<")";

 
     txtid_.text = ss.str();
 
     txtid_.lifetime = ros::Duration();

     txts_.markers.push_back(txtid_);

  }
  void MapVel::make_vel(float sort, float x0, float y0, float x1 ,float y1 ,string frameID ,int carID, bool Invisible, bool isadd, int time)
  {
     //float pos_x = x0 - LIDAR2ORGX;
     //float pos_y = y0 - LIDAR2ORGY;
     float pos_x = x0;
     float pos_y = y0;
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

     if(false){ //大车,浅蓝色
       shape_lon = 8;
       shape_wid = 2.5;
       shape_hei = 2.5;
       color_r = 108;
       color_g = 36;
       color_b = 45;
     }
     else if((int)sort == 4 || (int)sort == 5){ //超大车，蓝色
       shape_lon = 10;
       shape_wid = 2.5;
       shape_hei = 2.5;
       color_r = 0;
       color_g = 0;
       color_b = 255;
     }
     else if(false){ //骑行车，黄色
       shape_lon = 1.5;
       shape_wid = 1.0;
       shape_hei = 1.3;
       color_r = 255;
       color_g = 255;
       color_b = 0;
     }
     

     //make_txt(pos_x,pos_y+shape_lon/2 + 1,shape_hei + 0.5,frameID,carID,isadd);
     make_txt(pos_x,pos_y,shape_hei + 2,frameID,carID,isadd,time);

     veh_.header.frame_id = frameID;
     veh_.header.stamp = ros::Time::now();
     veh_.ns = "basic_shapes";
     veh_.id = carID;
     veh_.text = "";
 
     veh_.type = shape_;
     if(isadd)
       veh_.action = visualization_msgs::Marker::ADD;
     else
       veh_.action = visualization_msgs::Marker::DELETE;
 
     veh_.pose.position.x = pos_x;
     veh_.pose.position.y = pos_y;
     veh_.pose.position.z = shape_hei / 2.0;

     //求角度
     double thta = 0;
     //为了稳定小距离的角度
     //if(abs(x0-x1) > 0.3 && abs(atan2(y0-y1,x0-x1))> (3.14/6))
     //{
        //thta = atan2(y0-y1,x0-x1);
     //}
     veh_.pose.orientation.x = 0.0;
     veh_.pose.orientation.y = 0.0;
     //veh_.pose.orientation.z = -sin(thta/2);
     //veh_.pose.orientation.w = cos(thta/2);
     veh_.pose.orientation.z = 0;
     veh_.pose.orientation.w = 1;

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
}




























