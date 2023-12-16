#include <statis.hpp>
#define PERVEHDATALEN_ 6

#define SCALE_MAX_X_  20
#define SCALE_MIN_X_  -20
#define SCALE_MAX_Y_  200
#define SCALE_MIN_Y_  0


Statis_node::Statis_node(ros::NodeHandle *nh):nh_(*nh)
{
    subveh_ = nh_.subscribe<all_msgs_srvs::vehicalmsgs>("/mmlidar_veh", 1, &Statis_node::getVehsCb,this);//接受ros实时车辆信息
    subset_ = nh_.subscribe<all_msgs_srvs::settings>("/ros_param_setting", 1, &Statis_node::getSetCb,this);//接受ros系统配置热参数信息
    publog_ = nh_.advertise<all_msgs_srvs::systemlog>("/systemLog_info",10);//发送log消息
    //pubstation_ = nh_.advertise<lidar_camera::systemState>("/systemState_info",10);//发送系统状态
    pubsta_ = nh_.advertise<all_msgs_srvs::settings>("/statis_info",10);//发送统计信息
    cout<<" Statis_node Statis_node Statis_node Statis_node Statis_node"<<endl;
    cout<<" ++++++++++++++++++++++=========================+++++++++++++++++++++=============="<<endl;
    init_data();//初始化参数
    cout<<" ++++++++++++++++++++++=========================+++++++++++++++++++++=============="<<endl;
    get_rosparams();//获取ros参数
    cout<<" ++++++++++++++++++++++=========================+++++++++++++++++++++=============="<<endl;
    init_data1();//获得ros部分参数后，转化为类成员参数
    cout<<" ++++++++++++++++++++++=========================+++++++++++++++++++++=============="<<endl;
    
    ros::spin();//阻塞，不让程序结束
}

Statis_node::~Statis_node()
{
  //pubTimer->DeleteThread();
  delete pubTimer;
}

void Statis_node::init_data()
{
    frame_id = 0;//设备编号
    carLong = 5;//默认小车长度
    busLong = 10;//大车长度
    trainLong = 15;//超大车长度
    lancount = 0;//检测车道的个数

    statisPeriod = 60;//统计计算的周期，s

    lidar_fps = 15;//雷达zhen率

    //关于排队统计的相关参数，具体含义见头文件
    flush_head = 2.5;//m
    flush_tail = 15.0;//m
    sure_queue_dis = 5;//m
    sure_queue_km = 1.5;//km/h
    cancel_queue_km = 10.0;//km/h
    cancel_queue_dis = 1.0;//m

    pubTimer = new CTimer("Timer_pub");//实例化一个定时器变量
    pthread_mutex_init(&mutex_,&mutexattr_);//初始化一个线程

}

void Statis_node::get_rosparams()
{
    vector<float> Vehslong_list;//车长度数组
    ros::param::get("/VehicalLong",Vehslong_list);//获取数据
    carLong = Vehslong_list[2];//分配车长度
    busLong = Vehslong_list[1];
    trainLong = Vehslong_list[0];
    ros::param::get("/StatisPeriod",statisPeriod);//获取统计周期
    ros::param::get("/CheckDLong",checkbarLong);//获取检测带的长度
    ros::param::get("/nCount",lancount);//获取检测车道总数
    ros::param::get("/Countlist",Countlist);//获取检测车道list
    ros::param::get("/DeviceID",frame_id);//获取设备编号

    ros::param::get("/Queue/flush_head",flush_head);//关于统计的相关设置，具体含义见头文件
    ros::param::get("/Queue/flush_tail",flush_tail);
    ros::param::get("/Queue/sure_queue_km",sure_queue_km);
    ros::param::get("/Queue/sure_queue_dis",sure_queue_dis);
    ros::param::get("/Queue/cancel_queue_km",cancel_queue_km);
    ros::param::get("/Queue/cancel_queue_dis",cancel_queue_dis);

    ros::param::get("LAN_INFO_PATH",LanInfoPath);

}

//获取完ros参数后，需要对类成员进行再一次的初始化，相当于将信息转化
void Statis_node::init_data1()
{
  // for(int i=0;i<lancount;i++)

  for(int i=0; i<Countlist.size();i++)
  {//根据车道总数，来获取统计配置参数

    LanInfo laninfo;//车道信息
    // laninfo.lannum = i + 1;//车道号码
    laninfo.lannum = (int)Countlist[i];

    // string str_lan = to_string(i+1);
    string str_lan = to_string(laninfo.lannum);
    string param_name = "/LanParam/" + str_lan + "/type";
    int lantype;
    ros::param::get(param_name.c_str(),lantype);//获取车道类型的参数
    laninfo.lantype = lantype;//车道类型
    cout<<"laninfo.lannum: "<<param_name<<endl;
    
    param_name = "/LanParam/" + str_lan + "/width";
    float lanwidth;
    ros::param::get(param_name.c_str(),lanwidth);//获取车道宽度参数
    laninfo.lanwidth = lanwidth;//车道宽度
    cout<<"laninfo.lannum: "<<param_name<<endl;

    vector<float> coord;
    param_name = "/LanParam/" + str_lan + "/coordinate";
    ros::param::get(param_name,coord);//获取车道坐标参数
    laninfo.postion.x = coord[0];
    laninfo.postion.y = coord[1];//车道位置，停止线位置
    laninfo.queue.isQue = false;//是否有排队
    cout<<"laninfo.lannum: "<<param_name<<endl;

    //float checkDlong = 2;
    //ros::param::get("/CheckDLong",checkbarLong);
 
    //int checkcount=0;

    //获取该车道检测点的统计设置
    vector<float>  list_data;
    param_name = "/StatisPosition/" + str_lan;
    ros::param::get(param_name.c_str(),list_data);

    cout<<"param_name: "<<param_name <<endl;

    for(int j=0;j<list_data[0];j++){
      StatisInfo stat;
      // stat.checknum = j;//检测带的编号
      // stat.checkwidth = checkbarLong;//检测带的宽度
      // stat.distance = list_data[j+1];
      int jTemp = j * 3;

      stat.checknum = list_data[jTemp+1];//检测带的编号
      stat.checkwidth = list_data[jTemp+2];//检测带的宽度
      stat.distance = list_data[jTemp+3];

      stat.isNewcar = true;
      laninfo.statis.push_back(stat);
    }//统计消息
    laninfo_all.push_back(laninfo);//每个道路的信息加入到类成员列表中
  }
  statis_clear();//统计数据首次清零
  pubTimer->Start(statisPeriod * 1000,boost::bind(&Statis_node::pub_statisdata, this), true);	//异步循环执行，间隔时间毫秒
  
}

void Statis_node::getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist)
{
    //cout<<"get vehs"<<endl;
    pthread_mutex_lock(&mutex_);
    //cout<<"get vehs1"<<endl;
    vehmap_.erase(vehmap_.begin(),vehmap_.end());//接受消息前,先清空
    //cout<<"get vehs2"<<endl;
    get_vehmap(vehicalist->vehicaldatas);//解析数据
    //cout<<"get vehs3"<<endl;
    deal_vehs();//统计数据
    //cout<<"get vehs4"<<endl;
    pthread_mutex_unlock(&mutex_);

}
void Statis_node::deal_vehs()
{
  for(mapit_ = vehmap_.begin();mapit_ != vehmap_.end();++mapit_)
  {
     //取数据
     float veh_x,veh_y;
     veh_x = mapit_->second[3];//x
     veh_y = mapit_->second[4];//y
     int lannum = (int)mapit_->second[1];//lannum
     int sort = (int)mapit_->second[0];//sort
     float speed = mapit_->second[2];//v
     int id = (int)mapit_->first;//id
     //cout<<"lan_num"<<lannum<<",x:"<<veh_x<<endl;
     for(laninfo_allit_ = laninfo_all.begin();laninfo_allit_ != laninfo_all.end();laninfo_allit_++)//遍历每个路来选数据对应的车道
     {
       if(lannum == laninfo_allit_->lannum)//找出属于该车道的车道信息
       {
         statis_single(id,sort,lannum,veh_x,veh_y,speed,*laninfo_allit_);//对该车辆进行统计车流量
         statis_queue(id,sort,lannum,veh_x,veh_y,speed,*laninfo_allit_);//对该车道进行统计排队长度
       }
     }
  }
}

void Statis_node::getSetCb(const all_msgs_srvs::settingsConstPtr &sets)
{
  cout<<"get param"<<endl;//debug
  Log_pub("get param",LOGTYPE::INFO);//log
  pthread_mutex_lock(&mutex_);//上锁
  try{
    if(sets->frame_id != frame_id)//不是该设备的
      {return;}
    
    if(sets->type == SettingType::STATIS_INFO && sets->which_param == ROSParamType::STATIS_SET_OTHER_EVERY)
    {  
        cout<<"statis param"<<endl;
        Log_pub("statis param",LOGTYPE::INFO);//log
        carLong = sets->setdatas[2];//小车长度
        busLong = sets->setdatas[1];//中车长度
        trainLong = sets->setdatas[0];//大车长度
        lancount = (int)sets->setdatas[3];//车道数n
        int index = 4;
        // int lee_count = 0; queue

        if(lancount>0){

          sleep(5);
          laninfo_all.clear();

          cout<<"MMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMMM"<<endl;

          ifstream fin(LanInfoPath);
          YAML::Node LanConfig = YAML::Load(fin);

          cout << "lancount = (int)sets->setdatas[3]:  "<<lancount <<endl;

          for(int i=0; i<lancount;i++){
              
              LanInfo laninfo;

              int lannum_data = (int)sets->setdatas[index]; //取车道编号
              int checkpoint_count = (int)sets->setdatas[index+1];//取检测点数目
              
              cout<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"<<endl;

              laninfo.lannum = lannum_data;
              ostringstream str_lan;
              str_lan<<to_string(lannum_data);
              laninfo.lantype = LanConfig["LanParam"][str_lan.str()]["type"].as<int>();
              laninfo.lanwidth = LanConfig["LanParam"][str_lan.str()]["width"].as<float>();
              vector<float> coord;
              coord = LanConfig["LanParam"][str_lan.str()]["coordinate"].as<vector<float>>();
              laninfo.postion.x = coord[0];
              laninfo.postion.y = coord[1];
              laninfo.queue.isQue = false;

              cout<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"<<endl;

              index = index + 2;
              for(int j=0;j<checkpoint_count;j++){
                  StatisInfo statis_data;
                  statis_data.checknum = sets->setdatas[index];//检测带编号
                  cout << "statis_data.checknum: "<<sets->setdatas[index] << endl;
                  statis_data.checkwidth = sets->setdatas[index+1];//检测带的宽度
                  cout << "statis_data.checkwidth: "<<sets->setdatas[index+1] << endl;
                  statis_data.distance = sets->setdatas[index+2];//取出检测点的距离
                  cout << "statis_data.distance: "<<sets->setdatas[index+2] << endl;
                  index = index + 3;
                  statis_data.isNewcar = true;
                  laninfo.statis.push_back(statis_data);
                  }
            cout<<"SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS"<<endl;
            laninfo_all.push_back(laninfo);
            }
          statis_clear();
          }
      }

    else if(sets->type == SettingType::STATIS_INFO && sets->which_param == ROSParamType::STATIS_SET_PERIOD_EVERY)
    {
      cout<<"period param"<<endl;
      Log_pub("period param",LOGTYPE::INFO);//log
      statisPeriod = sets->setdatas[0];
      if(pubTimer!=NULL){
        delete pubTimer;//删除定时器
      }
      pubTimer = new CTimer("Timer_pub"); //创建一个新的定时器
      pubTimer->Start(statisPeriod * 1000,boost::bind(&Statis_node::pub_statisdata, this), true); //定时器里是以毫秒为单位，而周期是以秒为单位，所以×1000    

    }
    else if(sets->type == SettingType::NETWORK_INFO && sets->which_param == ROSParamType::NETWORK_ID_INFO_EVERY)
    {
      cout<<"id param"<<endl;
      Log_pub("id param",LOGTYPE::INFO);//log
      frame_id = (int)sets->setdatas[0];
    }
    else if(sets->type == SettingType::LAN_INFO && sets->which_param == ROSParamType::LAN_SET_COORD_EVERY)
    {
      cout<<"lan param"<<endl;
      Log_pub("lan param",LOGTYPE::INFO);//log
      lancount = (int)sets->setdatas[0];//车道数
      int index = 0;
      for(int i=1;i<=lancount;i++){
        if(i<=laninfo_all.size()){
          for(laninfo_allit_=laninfo_all.begin();laninfo_allit_!=laninfo_all.end();laninfo_allit_++){
            if(laninfo_allit_->lannum == i){
              laninfo_allit_->lantype = int(sets->setdatas[++index]);
              laninfo_allit_->lanwidth = sets->setdatas[++index];
              laninfo_allit_->postion.x = sets->setdatas[++index];
              laninfo_allit_->postion.y = sets->setdatas[++index];
              break;//找到就跳出
            }
          }
        }
        else if(i>laninfo_all.size()){//如果有增加的车道，表示要添加
          LanInfo laninfo;//车道信息
          laninfo.lannum = i;
          //其他类型，热参数中没有添加，需要在setting节点的道路信息热参数配置中添加
          laninfo.lantype = int(sets->setdatas[++index]);
          laninfo.lanwidth = sets->setdatas[++index];
          laninfo.postion.x = sets->setdatas[++index];
          laninfo.postion.y = sets->setdatas[++index];  
          QueueInfo queuei; 
          queuei.isQue = false;
          laninfo.queue = queuei;
          laninfo_all.push_back(laninfo);  
        }      
      }
      for(int i=lancount,j=0;i<laninfo_all.size(); j++){//遍历车道结构体集合
        if(j>24)//一个车道的检测点不超过24个
          break;
        for(laninfo_allit_=laninfo_all.begin();laninfo_allit_!=laninfo_all.end();laninfo_allit_++){
          if(laninfo_allit_->lannum > i){  
            laninfo_all.erase(laninfo_allit_);//如果改变小了，就删除多余的车道信息
            break;
          }
        }       
      }
      for(laninfo_allit_=laninfo_all.begin();laninfo_allit_!=laninfo_all.end();laninfo_allit_++){
          cout<<"lan:"<<laninfo_allit_->lannum<<",x:"<<laninfo_allit_->postion.x<<",y"<<laninfo_allit_->postion.y<<",duan:"<<laninfo_allit_->statis.size()<<endl;
      }       
    }
  }
  catch(int e){
    cout<<"error"<<e<<endl;
    Log_pub("params setting error",LOGTYPE::ERROR);//log
  }
  pthread_mutex_unlock(&mutex_);
}

void Statis_node::statis_queue(int carid, int type,int lannum,float x,float y,float vy,LanInfo &laninfo)//统计排队长度
{
  if(vy > 0)//去向就不算排队
    return;
  if(vy > -sure_queue_km){//判断是否停车，认为小于1.5就是停车
    if(laninfo.queue.isQue){//已经有人在排队了
      if(y > (laninfo.queue.jdist - flush_head) &&  y < (laninfo.queue.jdist + flush_head)){//更新车头
        //cout<<"lan:"<<lannum<<",车头更新,距离："<<y<<endl;
        laninfo.queue.jsort = type;
        laninfo.queue.jdist = y;
      }else if(y > (laninfo.queue.ydist - flush_head) &&  y < (laninfo.queue.ydist + flush_tail)){//更新车队末尾
        //cout<<"lan:"<<lannum<<",车尾更新,距离："<<y<<endl;
        laninfo.queue.ysort = type;
        laninfo.queue.ydist = y;
      }
    }else{//没有人在排队
      if(y > (laninfo.postion.y - sure_queue_dis) &&  y < (laninfo.postion.y + sure_queue_dis)){//是否满足排队的位置条件，在停车线附近
       // cout<<"lan:"<<lannum<<",开始排队,距离："<<y<<endl;
        laninfo.queue.isQue = true;//排队置位
        laninfo.queue.jsort = type;
        laninfo.queue.jdist = y;
        laninfo.queue.ysort = type;
        laninfo.queue.ydist = y;
      }
    }
  }
  else if(vy < -cancel_queue_km){//车流速度大于10km/h就认为没有在排队了
    if(laninfo.queue.isQue && y > (laninfo.queue.jdist - cancel_queue_dis) &&  y < (laninfo.queue.jdist + cancel_queue_dis)){//有排队且位置更新在排队头部的一定范围内
      //cout<<"lan:"<<lannum<<",取消排队,距离："<<y<<",速度："<<vy<<endl;
      laninfo.queue.isQue = false;//排队结束
    }
  }
}

void Statis_node::statis_single(int carid, int type,int lannum,float x,float y,float vy,LanInfo &laninfo)//统计车流量
{
  vector<StatisInfo>::iterator statis_it;
  for(statis_it = laninfo.statis.begin();statis_it!=laninfo.statis.end();statis_it++){
    if(y < (statis_it->distance + statis_it->checkwidth/2.0) && y > (statis_it->distance - statis_it->checkwidth/2.0)){//是否在检测带中
      //cout<<"info_num:"<<laninfo.lannum<<endl;
      if(statis_it->isNewcar){//如果新来的
        //cout<<"new lan:"<<lannum<<",id:"<<carid<<"，总数目："<<statis_it->carCount_sum<<"+1"<<endl;
        statis_it->carCount_sum++;//总流量+1
        float carlen = carLong;
        if(type == CarType::CAR){ //分类+1
          statis_it->carCount_car++;
          carlen = carLong;
        }
        else if(type == CarType::BUS){
          statis_it->carCount_bus++;
          carlen = busLong;
        }
        else if(type == CarType::TRA){
          statis_it->carCount_tra++;
          carlen = trainLong;
        }
        statis_it->arvSpeed += abs(vy);//车速累加
        statis_it->arvOcc += carlen/(abs(vy)/3.6);//时间占有的累加,单位s
        
        statis_it->old_carid = (uint8_t)carid;//储备新id
        statis_it->isNewcar = false;//下一个已经不是新来的
       // cout<<"new lan:"<<lannum<<",isNewcar:"<<statis_it->isNewcar<<",old id:"<<(int)statis_it->old_carid<<endl;
      }else{//如果里面还有车
        if(carid != statis_it->old_carid){//不是之前出现的
           statis_it->carCount_sum++;//总流量+1
           //cout<<"nex lan:"<<lannum<<",id:"<<carid<<",总数目："<<statis_it->carCount_sum<<"+1"<<endl;
           float carlen = carLong;
           if(type == CarType::CAR){ //分类+1
             statis_it->carCount_car++;
             carlen = carLong;
           }
           else if(type == CarType::BUS){
             statis_it->carCount_bus++;
             carlen = busLong;
           }
           else if(type == CarType::TRA){
             statis_it->carCount_tra++;
             carlen = trainLong;
           }
           statis_it->arvSpeed += abs(vy);//车速累加
           statis_it->arvOcc += carlen/(abs(vy)/3.6);//时间占有的累加,单位s
        
           statis_it->old_carid = (uint8_t)carid;//储备新id
        //cout<<"nex lan:"<<lannum<<",isNewcar:"<<statis_it->isNewcar<<",old id:"<<(int)statis_it->old_carid<<endl;
        }

        //预测刷新,如果出界，就判定下次进来的一定是新来的。速度慢了有风险
        if(vy < 0){//来向
          if(y + vy/(3.6 * lidar_fps) < (statis_it->distance - statis_it->checkwidth/2.0)){
            statis_it->isNewcar = 1;
            //cout<<"lan:"<<lannum<<",y:"<<y<<"vy:"<<vy<<",即将要出界:"<<(int)statis_it->isNewcar<<endl;
          }
        }
        else if(vy > 0){//去向
          if(y + vy/(3.6 * lidar_fps) > (statis_it->distance + statis_it->checkwidth/2.0)){
            statis_it->isNewcar = 1;   
            //cout<<"lan:"<<lannum<<",y:"<<y<<"vy:"<<vy<<",即将要出界:"<<(int)statis_it->isNewcar<<endl;
          } 
        }
      }
      break;
    }
  }
}

bool Statis_node::rect_filt(float sort, float state, float vy, float x, float y){
  bool ret = true;
      //车型过滤 + 状态过滤 + 方向过滤 + 位置过滤
  if (x <= SCALE_MIN_X_ || x >= SCALE_MAX_X_ || y <= SCALE_MIN_Y_ || y >= SCALE_MAX_Y_){

      ret = false;
  }
  return ret;
}
void Statis_node::get_vehmap(vector<float> vehs)
{
    int size = vehs.size();
    if(size < PERVEHDATALEN_)
      cout<<"veh_data failure!"<<endl;
    else{     
      for(int i = 0; i < size; i = i + PERVEHDATALEN_){
          if(rect_filt(vehs[i+1],vehs[i+2],vehs[i+3],vehs[i+4],vehs[i+5])){//设置条件添加,过滤器
          vector<float> listdata(5,-1);
          listdata.insert(listdata.begin(),vehs[i+1]);//sort
          listdata.insert(listdata.begin()+1,vehs[i+2]);//lan
          listdata.insert(listdata.begin()+2,vehs[i+3]);//vy
          listdata.insert(listdata.begin()+3,vehs[i+4]);//x
          listdata.insert(listdata.begin()+4,vehs[i+5]);//y
          vehmap_.insert(make_pair(vehs[i],listdata));//ID为key
        }
      }
      //cout<<"veh_size_get:"<<vehmap_.size()<<endl;
    }
}
void Statis_node::pub_statisdata()
{//发送统计信息
  statis_data.stamp = ros::Time::now();//时间戳
  statis_data.frame_id = frame_id;//雷达设备的id
  statis_data.type = SettingType::STATIS_INFO;//大类
  statis_data.which_param = ROSParamType::STATIS_UP;//小类

  statis_data.setdatas.push_back(float(statisPeriod));//统计周期
  statis_data.setdatas.push_back(float(carLong));//小车长
  statis_data.setdatas.push_back(float(busLong));//中车长
  statis_data.setdatas.push_back(float(trainLong));//大车长
  statis_data.setdatas.push_back(float(laninfo_all.size()));//车道数
  for(laninfo_allit_ = laninfo_all.begin();laninfo_allit_!=laninfo_all.end();laninfo_allit_++){//车道
    statis_compute(*laninfo_allit_);//先计算下各数据
    statis_data.setdatas.push_back(float(laninfo_allit_->lannum));//车道号码
    statis_data.setdatas.push_back(float(laninfo_allit_->statis.size()));//断面数目
    vector<StatisInfo>::iterator statisit;
    for(statisit=laninfo_allit_->statis.begin();statisit!=laninfo_allit_->statis.end();statisit++){//断面
      statis_data.setdatas.push_back(float(statisit->checknum));//断面编号 leeleelee
      statis_data.setdatas.push_back(float(statisit->carCount_sum));//总流量
      statis_data.setdatas.push_back(float(statisit->carCount_car));//小车流量
      statis_data.setdatas.push_back(float(statisit->carCount_bus));//中车流量
      statis_data.setdatas.push_back(float(statisit->carCount_tra));//大车流量
      statis_data.setdatas.push_back(float(statisit->arvTime));//车头时距
      statis_data.setdatas.push_back(float(statisit->arvOcc));//车辆占有率
      //cout<<"lan:"<<(int)laninfo_allit_->lannum<<",occ:"<<float(statisit->arvOcc)<<endl;
      statis_data.setdatas.push_back(float(statisit->arvSpeed));//平均车速
      statis_data.setdatas.push_back(float(statisit->arvCarLong));//平均车长
      if(laninfo_allit_->queue.isQue)
        statis_data.setdatas.push_back(float(laninfo_allit_->queue.queue_len));//排队长度
      else
        statis_data.setdatas.push_back(0.0);//排队长度
    }
  }

  pubsta_.publish(statis_data);  //发布统计数据
  statis_clear();  //统计数据清零工作
  statis_data.setdatas.clear();
  cout<<"pub 清零"<<endl;
}
void Statis_node::statis_compute(LanInfo &laninfo){//统计计算
  float carlen = carLong;
  if(laninfo.queue.ysort == CarType::CAR)
    carlen = carLong;
  else if(laninfo.queue.ysort == CarType::CAR)
    carlen = busLong;
  else if(laninfo.queue.ysort == CarType::CAR)
    carlen = trainLong;
  laninfo.queue.queue_len = laninfo.queue.ydist+carlen-laninfo.queue.jdist;//计算排队长度

  //计算统计指标
  vector<StatisInfo>::iterator statisit;
  for(statisit = laninfo_allit_->statis.begin();statisit!=laninfo_allit_->statis.end();statisit++){
    if(statisit->carCount_sum>0){//有车流通过,求平均值
      statisit->arvSpeed = statisit->arvSpeed / statisit->carCount_sum;//平均车速
      statisit->arvCarLong = (carLong*statisit->carCount_car + busLong*statisit->carCount_bus + trainLong*statisit->carCount_tra ) / statisit->carCount_sum;//平均车长
      statisit->arvTime = (statisPeriod) / statisit->carCount_sum;//车头时距
    }else{
      statisit->arvSpeed = 0;
      statisit->arvCarLong = 0;
      statisit->arvTime = 0;
    }
    if(statisPeriod>0)
      statisit->arvOcc = statisit->arvOcc / (statisPeriod);//车辆占有率
    else
      statisit->arvOcc = 1;
  }
}
void Statis_node::statis_clear(){//统计清零
  for(laninfo_allit_ = laninfo_all.begin();laninfo_allit_!=laninfo_all.end();laninfo_allit_++){
    vector<StatisInfo>::iterator statisit;
    for(statisit = laninfo_allit_->statis.begin();statisit!=laninfo_allit_->statis.end();statisit++){
      statisit->isNewcar = true;
      statisit->arvSpeed = 0;
      statisit->arvCarLong = 0;
      statisit->carCount_sum = 0;
      statisit->carCount_car = 0;
      statisit->carCount_bus = 0;
      statisit->carCount_tra = 0;
      statisit->arvTime = 0;
      statisit->arvOcc = 0;
    }
  }
}

void Statis_node::getTimeStamp()
{
    chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());//获取当前时间点
    data_stamp =  tp.time_since_epoch().count(); //计算距离1970-1-1,00:00的时间长度
    //time_now_.uInt32 = (uint32_t)(timestamp/1000);//精度在s
}

void Statis_node::Log_pub(string logstr,uint32_t type)
{
  all_msgs_srvs::systemlog systemlog_;
  systemlog_.device_id = frame_id;
  systemlog_.node_id = "statis_node";
  systemlog_.type = type;
  systemlog_.describe = logstr;
  for(int i=0;i<log_vector_.size();i++){
    systemlog_.data.push_back(log_vector_.at(i));
  }
  vector<float> null_v;
  log_vector_= null_v;
  publog_.publish(systemlog_);
}

int main(int argc ,char **argv)
{
    ros::init(argc, argv, "statis_node");
    ros::NodeHandle nh;
    Statis_node Statis_node(&nh);

}
