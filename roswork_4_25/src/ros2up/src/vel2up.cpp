/*
程序简介（非常重要）：
1,本节点的主要是完成与上位机的通讯
2,收到上位机的消息后，只负责初步分类，将接受到的消息下发到ros中，由setting_node负责进一步的解析配置
3,本节点接受ros中要上传给上位机的消息，并将其打包成为tcp传输格式，这个工作分别由各打包类函数来完成
4,给上位机发送消息，分为长链接，和短链接，长链接发送的消息是设备主动上传的消息，例如：车辆实时信息，周期统计信息，系统状态等，短链接主要完成配置信息和反馈应答信息
5,接受ros中主动上传的信息，主要是通过接受固定ros话题来完成
6,给setting_node发送尚未解析的数据，也是通过固定ros话题来完成
7,获取系统原有配置信息，然后回传给上位机，其中获取配置文件的工作，主要由setting_node获取，通过服务调用的方法传给本节点，再发送给上位机

*/

#include <vel2up.h>
#include <ros/ros.h>
#include <iostream>
#include <time.h>


namespace ros2up{

#define PERVEHDATALEN 6

ArgBridge::ArgBridge(ros::NodeHandle *nh):nh_(*nh)
{
  UpperPcIp_ = "192.168.203.55";//这个本机作为tcp_server暂时用不上
  LongPort_ = 4040;//雷达实时数据，长链接的端口号码
  ShortPort_ = 5050;//配置短链接的端口号码
  DeviceIp.word_s.byte0 = ProDirID::NORTH;//设备id的方位
  DeviceIp.word_s.byte1 = DeviceID::DEVICE_00;//设备id的编号
  
  conn_ = false;//server是否创建成功的标志
  sure_sendVehs_ = true;//是否发送车辆信息的标志

  recvbuffer_ = new RecvBuffer(&recv_buf_[0],9600);//创建一个接受消息的buff实例
  //unsigned short ip = 0; 
  vehicalUp_ = new VehicalUp(&DeviceIp);//车辆协议打包实例
  statisUp_ = new StatisUp(&DeviceIp);//统计数据协议打包实例
  laninfoback_ = new LanInfoBack(&DeviceIp);//车道信息回复协议打包实例
  statisinfoback_ = new StatisInfoBack(&DeviceIp);//统计设置回复协议打包实例
  periodback_ = new PeriodBack(&DeviceIp);//周期回复协议打包实例
  installinfoback_ = new InstallInfoBack(&DeviceIp);//安装标校参数回复协议打包实例
  settingback_ = new SettingBack(&DeviceIp); //设置回复协议打包实例
  deviceipback_ = new Deviceipback(&DeviceIp);//设备ip回复协议打包实例
  devStateUp_ = new DevStateUp(&DeviceIp);//设备状态回复协议打包实例
  outdeviceback_ = new Outdeviceback(&DeviceIp);//平台或者信号机等外部设备ip回复协议打包实例
  onlineQback_ = new OnLineQBack(&DeviceIp);//心跳机制，在线查询反馈接口
  memset(recv_buf_,0,sizeof(recv_buf_));//初始化接收的buffer
  publog_ = nh_.advertise<all_msgs_srvs::systemlog>("/systemLog_info",10);//Log日志发布
  getParams();//获取ros参数
  do
  {
     cout<<"try to create Server!"<<endl;
     Log_pub("try to create Server!",LOGTYPE::WARRING);
     createTcpServer();//创建tcp_server
  }while(!conn_ && nh_.ok() && sure_sendVehs_);//直到创建成功

  subveh_ =  nh_.subscribe<all_msgs_srvs::vehicalmsgs>("/mmlidar_veh", 1, &ArgBridge::getVehsCb,this);//接受车辆实时信息
  substatis_ =  nh_.subscribe<all_msgs_srvs::settings>("/statis_info", 1, &ArgBridge::getStatisCb,this);//接受系统统计消息
  substate_ = nh_.subscribe<all_msgs_srvs::systemState>("/systemState_info", 1, &ArgBridge::getStateCb,this);//接受系统状态
  
  subset_ = nh_.subscribe<all_msgs_srvs::settings>("/ros_param_setting", 1, &ArgBridge::getSetsCb,this);//接受系统配置信息的热参数修改
  suberr_ = nh_.subscribe<all_msgs_srvs::errorMsgs>("/systemError_info", 1,&ArgBridge::getSysMsgsCb,this);//接收系统错误和消息,随时上传
  
  pubset_ = nh_.advertise<all_msgs_srvs::settings>("/ros_settings",10);//接受上位机配置信息，然后发送给ros网络
  
  serClient_ = nh_.serviceClient<all_msgs_srvs::settingcallback>("/get_param_from_file");//向setting节点请求设置消息的ros服务机制的客户端

  Log_pub("ros2up_node init finished!",LOGTYPE::INFO);
}

//创建tcp_server
void ArgBridge::createTcpServer()
{
    cout<<"my serverIP:"<<UpperPcIp_.c_str()<<","<<LongPort_<<endl;
    ostringstream ss1;
    ss1<<"my serverIP:"<<UpperPcIp_.c_str()<<",LongPort"<<LongPort_;
    Log_pub(ss1.str(),LOGTYPE::INFO);
    if (server_.createServer(LongPort_,ShortPort_,
     boost::bind(&ArgBridge::tcpdataProc, this, _1, _2),UpperPcIp_.c_str()) == true)//connecte successfully
    {
      conn_ = true;//创建成功标志
      //cout<<"server created successful!"<<conn_<<endl;
      string str_log = server_.get_log();
      str_log += "\nserver created successful!\n";
      Log_pub(str_log,LOGTYPE::INFO);
    }
}
ArgBridge::~ArgBridge(){
  delete recvbuffer_;
  delete vehicalUp_;
  delete statisUp_;
  delete laninfoback_;
  delete statisinfoback_;
  delete periodback_;
  delete installinfoback_;
  delete settingback_;
  delete deviceipback_;
  delete devStateUp_;
  delete outdeviceback_;
  delete onlineQback_;
  server_.close();
  cout<<"ros2up_node end"<<endl;
  pthread_mutex_destroy(&mutex_); //注
}

static int c = 0;
//发送车辆信息
void ArgBridge::getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist){
    if(server_.get_clientnum() <= 0){//检测，如果没有客户端连接，则不发送
      return ;
    }
    if(server_.long_send(vehicalUp_->packData(vehicalist->vehicaldatas),vehicalUp_->getBufferLen()) == -1){//发送车辆实时信息，如果失败，打印失败log，如果成功，则不打印
      string str_log = server_.get_log();
      str_log += "\nvehs sended failed!\n";
      Log_pub(str_log,LOGTYPE::WARRING);
      cout<<"send failed"<<endl;
    }
    else{
       
    }
}

//从ros中获取统计信息，消息类型为settings,所以会解析
void ArgBridge::getStatisCb(const all_msgs_srvs::settingsConstPtr &sets){
    if(sets->frame_id != DeviceIp.uwords && (sets->type !=SettingType::STATIS_INFO || sets->which_param !=ROSParamType::STATIS_UP)){//再次检验，其实也可以不用检验，话题名称唯一
      return;
    }
    if(server_.get_clientnum() <= 0){//如果没有客户端连接上，不发送
      return ;
    }
    if(server_.long_send(statisUp_->packData(sets->setdatas),statisUp_->getBufferLen()) == -1){//tcp发送统计信息
      string str_log = server_.get_log();
      str_log += "\n\t\t\tsend statis data failed!\n";
      Log_pub(str_log,LOGTYPE::WARRING);
    }
    else{
      cout<<"send statis data successful"<<endl;
    }
}

//从ros中获取系统状态信息的消息回调函数
void ArgBridge::getStateCb(const all_msgs_srvs::systemStateConstPtr &data){
  if(data->device_id != DeviceIp.uwords){
    return;
  }
  //将话题数据放入vector中
  vector<float> vec_data;
  vec_data.push_back(data->occ_CPU);
  vec_data.push_back(data->occ_GPU);
  vec_data.push_back(data->temperature);
  vec_data.push_back(data->occ_RAM);
  vec_data.push_back(data->occ_memory_home);
  vec_data.push_back(data->occ_memory_sda1);
  vec_data.push_back(data->precess_state);
  vec_data.push_back(data->camera_state);
  vec_data.push_back(data->mmlidar_state);
  if(server_.long_send(devStateUp_->packData(vec_data),devStateUp_->getBufferLen()) == -1){//tcp发送系统状态信息
      //cout<<"send device state data failed"<<endl;
      string str_log = server_.get_log();
      str_log += "\n\t\t\tsend device state data failed!\n";
      Log_pub(str_log,LOGTYPE::WARRING);
  }
  else{
    //cout<<"send statis data successful"<<endl;
  }

}

//从ros中获取设置信息热参数的回调函数
void ArgBridge::getSetsCb(const all_msgs_srvs::settingsConstPtr &sets){
    ostringstream ss;
    ss<<"param setting enter ros2up ,my id:"<<DeviceIp.uwords<<",setdata id:"<<sets->frame_id;
    null_vector_ = sets->setdatas;
    Log_pub(ss.str(),LOGTYPE::INFO);//发布log信息

    if(!sure_sendVehs_ && sets->frame_id != DeviceIp.uwords){//核对设备id
      Log_pub("id not match ,setting failed!",LOGTYPE::INFO);
      return;
    }
  //对网络参数和本机链路地址进行在线修改，实际没有修改执行，这个地方有需要的时候可以
    if(sets->type ==SettingType::NETWORK_INFO && sets->which_param ==ROSParamType::NETWORK_ID_INFO_EVERY)
    {
      DeviceIp.uwords = (uint16_t)sets->setdatas[0];
      ostringstream ss;
      ss<<(int)(sets->setdatas[1])<<"."<<(int)(sets->setdatas[2])<<"."<<(int)(sets->setdatas[3])<<"."<<(int)(sets->setdatas[4])<<endl;
      UpperPcIp_ = ss.str();

      ostringstream ss1;
      ss1<<"set device id:"<<DeviceIp.uwords<<",ip:"<<UpperPcIp_<<",successful!"<<endl;
      Log_pub(ss1.str(),LOGTYPE::WARRING);
    }
}

//获取系统消息反馈，错误消息，这里暂时没有执行，跟状态消息一起发送了
void ArgBridge::getSysMsgsCb(const all_msgs_srvs::errorMsgsConstPtr &errmsgs){
    if(server_.get_clientnum() <= 0){
      return ;
    }
}

//从ros中获取配置参数
void ArgBridge::getParams()
{
    ros::param::get("/UpperNetIp",UpperPcIp_);
    ros::param::get("/UpperLongNetPort",LongPort_);
    ros::param::get("/UpperShortNetPort",ShortPort_);
    ros::param::get("~sure_sendVehs",sure_sendVehs_);
    int Id = 0;
    ros::param::get("/DeviceID",Id);
    DeviceIp.uwords = (uint16_t)Id;
    cout<<"get params successful,ID:"<<DeviceIp.uwords<<endl;
    cout<<"ServerIP:"<<UpperPcIp_<<","<<"Port:"<<LongPort_<<endl;
    
    ostringstream ss;
    ss<<"get params successful,ID:"<<DeviceIp.uwords<<",ServerIP:"<<UpperPcIp_<<",LongPort:"<<LongPort_<<",ShortPort:"<<ShortPort_<<",successful!\n";
    Log_pub(ss.str(),LOGTYPE::INFO);
}

//从上位机获取信息，解析，并将具体协议内容下发给setting_node节点具体解析，此处只是将消息分类
void ArgBridge::dataUnpack(uint8_t *data, unsigned int data_len)
{
  
  uint32_t len_ = 0;
  uint8_t crc = 0;
  for(int i = 1;i < data_len - 2; i++){//除了0 和倒数2位，倒数第一位是结尾，倒数第二位是crc校验位
      crc ^=data[i];//crc，校验
      recvbuffer_->add_data(data[i]);//将数据放入vector_buff中
      null_vector_.push_back(data[i]);//日志记录data
      cout << "----" <<endl;
      cout << "i:"<< i << "->"<< (float)data[i] <<endl;
  }
  //crc校验,和最后一位的查看
  if(crc != data[data_len - 2] && data[data_len - 1] != TCPCOL_HEAD::UP_HEAD){
    cout<<"crc or tail is error"<<endl;
    Log_pub("crc or tail is error",LOGTYPE::ERROR);
    recvbuffer_->empty_data();//清空内存
    return;
  }
  cout<<"cross through crc!"<<endl;
  Log_pub("cross through crc!",LOGTYPE::INFO);
  int index = 0;
  uWord_16 lianluID;//链路地址
  uint8_t agr_v,oper_type,obj_tag;
  lianluID.word_s.byte0 = recvbuffer_->get_data(index++) & 0xFF;//取链路地址，就是设备编号
  lianluID.word_s.byte1 = recvbuffer_->get_data(index++) & 0xFF;
  oper_type = recvbuffer_->get_data(index++) & 0xFF;//取操作类型
  obj_tag = recvbuffer_->get_data(index++) & 0xFF;//取对象类型

  cout << "lianluID.word_s.byte0: "<< (int)lianluID.word_s.byte0 << endl;
  cout << "lianluID.word_s.byte1: "<< (int)lianluID.word_s.byte1 << endl;

  cout << "----------------------_________________------------------____________" <<endl;
  cout << "oper_type: " <<  (int)oper_type <<endl;
  cout << "obj_tag: " <<  (int)obj_tag <<endl;

  cout << "OperType::OPER_TYPE_SETTING_INFO: " <<  OperType::OPER_TYPE_SETTING_INFO <<endl;
  cout << "OperType::OPER_TYPE_INFO_REQUEST: " <<  OperType::OPER_TYPE_INFO_REQUEST <<endl;
  cout << "OperType::OPER_TYPE_ONLINE_REQUEST: " <<  OperType::OPER_TYPE_ONLINE_REQUEST <<endl;

  cout << "----------------------_________________------------------____________" <<endl;

//1,上位机下发配置信息
  if(oper_type == OperType::OPER_TYPE_SETTING_INFO){//配置信息，判断操作类型为信息配置
    cout<<"this is a setting message!"<<endl;
    Log_pub("This is a setting message!",LOGTYPE::INFO);//调试和log

    all_msgs_srvs::settings setting_ros;//定义ros内部消息格式
    setting_ros.frame_id = lianluID.uwords;//填写设备链路地址，即设备编号和与编号
    for(int i = 4;i<recvbuffer_->get_data_len();i++)//循环添加数据到ros消息中
      setting_ros.setdatas.push_back((float)recvbuffer_->get_data(i));

    //根据具体的配置信息，给ros的setting消息来定义不同的type,和which_param
    if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_LAN)//配置车道信息
    {
      cout<<"this is lan setting msgs!"<<endl;
      Log_pub("This is lan setting msgs!",LOGTYPE::INFO);

      setting_ros.type = SettingType::LAN_INFO;//车道信息
      setting_ros.which_param = ROSParamType::LAN_INFO_SETTING;//配置车道信息
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_STATIS)//配置断面统计信息
    {
      cout<<"this is statis setting msgs!"<<endl;
      Log_pub("This is statis setting msgs!",LOGTYPE::INFO);

      setting_ros.type = SettingType::STATIS_INFO;//统计信息
      setting_ros.which_param = ROSParamType::STATIS_LAN_SETTING;//配置车道统计断面数据
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_STATIS_PEROID)//配置断面统计周期
    {
      cout<<"this is period setting msgs!"<<endl;
      Log_pub("This is period setting msgs!",LOGTYPE::INFO);

      setting_ros.type = SettingType::STATIS_INFO;//统计信息
      setting_ros.which_param = ROSParamType::STATIS_PERIOD_SETTING;//配置统计周期
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_INSTALL)//安装校准信息配置
    {
      cout<<"this is install setting msgs!"<<endl;
      Log_pub("This is install setting msgs!",LOGTYPE::INFO);

      setting_ros.type = SettingType::INSTALL_INFO;//安装信息
      setting_ros.which_param = ROSParamType::INSTALL_CLI_SETTING;//配置标校信息
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_DEVICE_IP)//设备IP设置
    {
      cout<<"this is network setting msgs!"<<endl;
      Log_pub("This is network setting msgs!",LOGTYPE::INFO);

      setting_ros.type = SettingType::NETWORK_INFO;//网络信息
      setting_ros.which_param = ROSParamType::DEVICE_IP_SETTING;//配置网络ip
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_OUT_DEVICE_IP)//外部设备,信号机IP设置
    {
      cout<<"this is tra_dev network setting msgs!"<<endl;
      Log_pub("This is tra_dev network setting msgs!",LOGTYPE::INFO);

      setting_ros.type = SettingType::NETWORK_INFO;//网络信息
      setting_ros.which_param = ROSParamType::OUT_DEVICE_IP_SETTING;//配置外部平台的ip
    }
    pubset_.publish(setting_ros);//发送配置信息到节点,进行解析
    Log_pub("Send setting msgs to setting_node",LOGTYPE::INFO);

    vector<float> data;
    request_back(oper_type,obj_tag,data);//给上位机应答，数据长度为0
    Log_pub("responed successfully",LOGTYPE::INFO);
  }
//2,上位机请求设备已有的配置信息
  else if(oper_type == OperType::OPER_TYPE_INFO_REQUEST){//上位机请求参数配置信息
    cout<<"this a required msg!"<<endl;
    Log_pub("This a required msg!",LOGTYPE::INFO);

    all_msgs_srvs::settingcallback srvdata;//这里使用了ros通讯机制中的server/client的机制，即client带着请求数据，请求服务，server收到请求后，处理有一个反馈结果返回给client;
    //注意：server定义在setting_node中
    srvdata.request.fram_id = lianluID.uwords;
    if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_LAN){//请求车道信息
      cout<<"this require lan msg!"<<endl;
      Log_pub("This require lan msg!",LOGTYPE::INFO);
      srvdata.request.type = SettingType::LAN_INFO;
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_INSTALL){//请求标校参数  
      cout<<"this require install msg!"<<endl;
      Log_pub("This require install msg!",LOGTYPE::INFO);
      srvdata.request.type = SettingType::INSTALL_INFO;
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_STATIS){//请求统计设置信息
      cout<<"this require statis msg!"<<endl;
      Log_pub("This require statis msg!",LOGTYPE::INFO);
      srvdata.request.type = SettingType::STATIS_INFO;
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_STATIS_PEROID){//请求统计周期
      cout<<"this require period msg!"<<endl;
      srvdata.request.type = SettingType::REQUEST_PERIOD;
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_DEVICE_IP){//请求设备id和ip网络信息
      cout<<"this require network msg!"<<endl;
      Log_pub("This require network msg!",LOGTYPE::INFO);
      srvdata.request.type = SettingType::NETWORK_INFO;
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_OUT_DEVICE_IP){//请求外部设备网络状态
      cout<<"this require out network msg!"<<endl;
      Log_pub("This require out network msg!",LOGTYPE::INFO);
      srvdata.request.type = SettingType::REQUEST_DEV_NET;
    }
    if (serClient_.call(srvdata))//ros的client向server请求服务
    {
      cout<<"call srv successful in ros2up_node"<<endl;
      Log_pub("Call srv successful in ros2up_node",LOGTYPE::INFO);
      request_back(oper_type,obj_tag,srvdata.response.setdatas);//srvdata.response.setdatas为服务返回的数据，把这个数据返回给上位机
    }
  }
//3,在线查询，心跳机制
  else if(oper_type == OperType::OPER_TYPE_ONLINE_REQUEST){//心跳机制
    if(obj_tag == ObjTag::OBJ_TAG_DATA_ONLINE){//心跳在线查询
      vector<float> data;
      request_back(oper_type,obj_tag,data);//心跳应答，数据长度为0
    }
  }
  recvbuffer_->empty_data();//清空内存
}

//上传应答数据，参数有：
//1,操作类型；
//2,对象类型；
//3,数据列表
void ArgBridge::request_back(uint8_t oper_type, uint8_t obj_tag, vector<float> &data){
  //cout<<"return inquired info"<<endl;
  null_vector_ = data;//记录log_data
  if(oper_type == OperType::OPER_TYPE_SETTING_INFO){//配置信息应答
     Log_pub("Enter setting responed!",LOGTYPE::INFO);
     settingback_->setHead(OperType::OPER_TYPE_RESPOND_SETTING,obj_tag);//添加协议头
     if(server_.short_response(settingback_->packData(data),settingback_->getBufferLen()) == -1)//短链接发送
     {
        string str_log = server_.get_log();
        cout<<"setting data responed failed!"<<endl;
        Log_pub(str_log + "Setting responed failed!",LOGTYPE::WARRING);
     }
     else
     {
        string str_log = server_.get_log();
        cout<<"setting message return successful in ros2up_node"<<endl;
        Log_pub(str_log +"Setting responed successful !",LOGTYPE::INFO);
     }
  }
  else if(oper_type == OperType::OPER_TYPE_ONLINE_REQUEST){//心跳设备在线应答
     Log_pub("Enter online Q responed!",LOGTYPE::INFO);
     onlineQback_->setHead(oper_type,ObjTag::OBJ_TAG_DATA_ONLINE_RES);//添加协议头
     if(server_.short_response(onlineQback_->packData(data),onlineQback_->getBufferLen()) == -1)//短链接发送
     {
        string str_log = server_.get_log();
        cout<<"online responed failed!"<<endl;
        Log_pub(str_log + "online responed failed!",LOGTYPE::WARRING);
     }
     else
     {
        string str_log = server_.get_log();
        cout<<"online responed message return successful in ros2up_node"<<endl;
        Log_pub(str_log +"online responed successful !",LOGTYPE::INFO);
     }
  }
  else if(oper_type == OperType::OPER_TYPE_INFO_REQUEST){//请求参数配置应答
    Log_pub("Enter info inquired!",LOGTYPE::INFO);
    //判断setting_node中server给返回的数据是否是想要的，其中如果返回值无效，返回的数据长度为3,则都为0.101,表示返回无效值，主要是
    if(data.size() == 3 && abs(data[0] - (-0.101))<0.0001 && abs(data[1] - (-0.101))<0.0001 && abs(data[2] - (-0.101))<0.0001){
      cout<<"this device require failed!"<<endl;
      Log_pub("This device require failed!",LOGTYPE::INFO);
      return;
    }
    if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_LAN){//请求车道信息应答
      cout<<"enter inquired lan info"<<endl;
      if(server_.short_response(laninfoback_->packData(data),laninfoback_->getBufferLen()) == -1){
        string str_log = server_.get_log();
        cout<<"lan info responed failed!"<<endl;
        Log_pub(str_log + "Lan info responed failed!",LOGTYPE::WARRING);
      }else{
        string str_log = server_.get_log();
        cout<<"lan info responed successful!"<<endl;
        Log_pub(str_log + "Lan info responed successful!",LOGTYPE::INFO);
      }
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_INSTALL){//请求标校参数应答
      if(server_.short_response(installinfoback_->packData(data),installinfoback_->getBufferLen()) == -1){
        string str_log = server_.get_log();
        cout<<"install info responed failed!"<<endl;
        Log_pub(str_log + "Install info responed failed!",LOGTYPE::WARRING);
      }else{
        string str_log = server_.get_log();
        cout<<"install info responed successful!"<<endl;
        Log_pub(str_log + "Install info responed successful!",LOGTYPE::INFO);
      }
      cout<<"ros2up send len:"<<statisinfoback_->getBufferLen()<<endl;
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_STATIS){//请求统计设置信息应答
      if(server_.short_response(statisinfoback_->packData(data),statisinfoback_->getBufferLen()) == -1){
        string str_log = server_.get_log();
        cout<<"statis setting responed failed!"<<endl;
        Log_pub(str_log + "Statis info responed failed!",LOGTYPE::WARRING);
      }else{
        string str_log = server_.get_log();
        cout<<"statis info responed successful!"<<endl;
        Log_pub(str_log + "Statis info responed successful!",LOGTYPE::INFO);
      }
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_STATIS_PEROID){//请求统计周期应答
      if(server_.short_response(periodback_->packData(data),periodback_->getBufferLen()) == -1){
        string str_log = server_.get_log();
        cout<<"statis period responed failed!"<<endl;
        Log_pub(str_log + "Statis info responed failed!",LOGTYPE::WARRING);
      }else{
        string str_log = server_.get_log();
        cout<<"period info responed successful!"<<endl;
        Log_pub(str_log + "Period info responed successful!",LOGTYPE::INFO);
      }
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_DEVICE_IP){//请求IP应答
      if(server_.short_response(deviceipback_->packData(data),deviceipback_->getBufferLen()) == -1){
        string str_log = server_.get_log();
        cout<<"network responed failed!"<<endl;
        Log_pub(str_log + "Network info responed failed!",LOGTYPE::WARRING);
      }else{
        string str_log = server_.get_log();
        cout<<"network info responed successful!"<<endl;
        Log_pub(str_log + "Network info responed successful!",LOGTYPE::INFO);
      }
    }
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_SETTING_OUT_DEVICE_IP){//请求外部设备网络状态应答
      if(server_.short_response(outdeviceback_->packData(data),outdeviceback_->getBufferLen()) == -1){
        string str_log = server_.get_log();
        cout<<"network responed failed!"<<endl;
        Log_pub(str_log + "Network info responed failed!",LOGTYPE::WARRING);
      }else{
        string str_log = server_.get_log();
        cout<<"network info responed successful!"<<endl;
        Log_pub(str_log + "Network info responed successful!",LOGTYPE::INFO);
      }
    }
  }
}
void ArgBridge::Log_pub(string logstr,uint32_t type)
{
  all_msgs_srvs::systemlog systemlog_;
  systemlog_.device_id = DeviceIp.uwords;
  systemlog_.node_id = "ros2up_node";
  systemlog_.type = type;
  systemlog_.describe = logstr;
  for(int i=0;i<null_vector_.size();i++){
    systemlog_.data.push_back(null_vector_.at(i));
  }
  vector<float> null_v;
  null_vector_= null_v;
  publog_.publish(systemlog_);
}

void ArgBridge::tcpdataProc(uint8_t *data, unsigned int data_len)
{
   //cout<<"Recieved tcp server data!"<<endl;
   Log_pub("Network info responed successful!",LOGTYPE::INFO);
   if((data[0] & 0xFF) == TCPCOL_HEAD::UP_HEAD)//head
   {
     cout<<"a message from short net in ros2up_node!"<<endl;
     Log_pub("Get a message with 0x02 head from short net!",LOGTYPE::INFO);
     dataUnpack(data,data_len);
   }
   else{
     cout<<"get unkown data from upper!"<<endl;  
     Log_pub("Get a unkown message without 0x02 head from short net!",LOGTYPE::INFO); 
     settingback_->setHead(0x00,0x00);
     vector<float> data;
     if(server_.short_response(settingback_->packData(data),settingback_->getBufferLen()) == -1)
     {
        string str_log = server_.get_log();
        cout<<"unkown data responed failed!"<<endl;
        Log_pub(str_log+"Unkown data responed failed!",LOGTYPE::WARRING); 
     }
     else{
        string str_log = server_.get_log();
        Log_pub(str_log+"Unkown data responed successful!",LOGTYPE::INFO); 
     }
   }
}
}



