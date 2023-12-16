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

#include <statis2tra.h>
#include <ros/ros.h>
#include <iostream>
#include <time.h>

#define PERVEHDATALEN 6

TraBridge::TraBridge(ros::NodeHandle *nh):nh_(*nh)
{
  UpperPcIp_ = "192.168.8.148";//这个本机作为tcp_server暂时用不上
  LongPort_ = 4040;//雷达实时数据，长链接的端口号码
  ShortPort_ = 5050;//配置短链接的端口号码
  DeviceIp.uwords = 1;//设备链路地址
  
  conn_ = false;//server是否创建成功的标志
  sure_pulse_ = false;//是否发送车辆脉冲信息的标志

  recvbuffer_ = new RecvBuffer(&recv_buf_[0],9600);//创建一个接受消息的buff实例
  //unsigned short ip = 0; 
  magback_ = new MsgBack(&DeviceIp);//无数据回应
  errback_ = new ErrBack(&DeviceIp);//错误消息应答
  timeback_ = new TimeBack(&DeviceIp);//时间回应
  paramback_ = new ParamBack(&DeviceIp);//参数回应
  statisup_ = new StatisInfoUp(&DeviceIp);//统计数据主动上传
  historyback_ = new HistoryDataBack(&DeviceIp);//历史数据回应
  pulesdateup_ = new PulesDataUp(&DeviceIp);//脉冲数据主动上传

  memset(recv_buf_,0,sizeof(recv_buf_));//初始化接收的buffer
  publog_ = nh_.advertise<all_msgs_srvs::systemlog>("/systemLog_info",10);//Log日志发布
  getParams();//获取ros参数
  cout<<"try to create TCP link!"<<endl;
  Log_pub("try to create TCP link!",LOGTYPE::WARRING);
  createTcpLink();//创建tcp_server
  if(!conn_){
    exit(0);
  }

  subveh_ =  nh_.subscribe<all_msgs_srvs::vehicalmsgs>("/pules_veh", 1, &TraBridge::getVehsCb,this);//接受车辆脉冲实时信息
  substatis_ =  nh_.subscribe<all_msgs_srvs::settings>("/statis_info", 1, &TraBridge::getStatisCb,this);//接受系统统计消息

  subset_ = nh_.subscribe<all_msgs_srvs::settings>("/ros_param_setting", 1, &TraBridge::getSetsCb,this);//接受系统配置信息的热参数修改
  suberr_ = nh_.subscribe<all_msgs_srvs::errorMsgs>("/systemError_info", 1,&TraBridge::getSysMsgsCb,this);//接收系统错误和消息,随时上传
  pubset_ = nh_.advertise<all_msgs_srvs::settings>("/tra_settings",10);//接受信号机配置信息，然后发送给ros网络
  serClient_ = nh_.serviceClient<all_msgs_srvs::settingcallback>("/get_param_from_file_tra");//向trasetting节点请求设置消息的ros服务机制的客户端

  Log_pub("ros2up_node init finished!",LOGTYPE::INFO);
}

//创建tcp_server
void TraBridge::createTcpLink()
{
    cout<<"my serverIP:"<<UpperPcIp_.c_str()<<","<<LongPort_<<endl;
    ostringstream ss1;
    ss1<<"my serverIP:"<<UpperPcIp_.c_str()<<",LongPort"<<LongPort_;
    Log_pub(ss1.str(),LOGTYPE::INFO);

    TcpInterface *tcpInterface = new Server(LongPort_,ShortPort_,boost::bind(&TraBridge::tcpdataProc, this, _1, _2),UpperPcIp_.c_str());
    if(tcpInterface != NULL)
    {
      delete tcpInterface_;
      tcpInterface_ = tcpInterface;
      conn_ = true;//创建成功标志
      cout<<"server created successful!"<<conn_<<endl;
      string str_log = tcpInterface_->get_log();
      str_log += "\nserver created successful!\n";
      Log_pub(str_log,LOGTYPE::INFO);
    }
}
TraBridge::~TraBridge(){
  delete tcpInterface_;
  delete magback_;
  delete errback_;
  delete timeback_;
  delete paramback_;
  delete statisup_;
  delete historyback_;
  delete pulesdateup_;

  cout<<"ros2up_node end"<<endl;
  pthread_mutex_destroy(&mutex_); //注
}

static int c = 0;
//发送车辆信息
void TraBridge::getVehsCb(const all_msgs_srvs::vehicalmsgsConstPtr &vehicalist){

}

//从ros中获取统计信息，消息类型为settings,所以会解析
void TraBridge::getStatisCb(const all_msgs_srvs::settingsConstPtr &sets){

}

//从ros中获取设置信息热参数的回调函数
void TraBridge::getSetsCb(const all_msgs_srvs::settingsConstPtr &sets){
   if(!sure_pulse_ && sets->frame_id != DeviceIp.uwords){//核对设备id
     Log_pub("id not match ,setting failed!",LOGTYPE::INFO);
     return;
   }
  //对网络参数和本机链路地址进行在线修改，实际没有修改执行，这个地方有需要的时候可以
   if(sets->type ==SettingType::NETWORK_INFO && sets->which_param ==ROSParamType::NETWORK_ID_INFO_EVERY)
    {
      DeviceIp.uwords = (uint16_t)sets->setdatas[0];
      ostringstream ss;

    }
}

//获取系统消息反馈，错误消息，这里暂时没有执行，跟状态消息一起发送了
void TraBridge::getSysMsgsCb(const all_msgs_srvs::errorMsgsConstPtr &errmsgs){

}

//从ros中获取配置参数
void TraBridge::getParams()
{
    ros::param::get("/TranfficIp",UpperPcIp_);
    ros::param::get("/TranfficPort",LongPort_);
    ShortPort_ = LongPort_;
    ros::param::get("/TranfficMode",sure_pulse_);
    int Id = 0;
    ros::param::get("/DeviceID",Id);
    DeviceIp.uwords = (uint16_t)Id;
    cout<<"get params successful,ID:"<<DeviceIp.uwords<<endl;
    cout<<"TraIP:"<<UpperPcIp_<<","<<"Port:"<<LongPort_<<endl;
    
    ostringstream ss;
    ss<<"get params successful,ID:"<<DeviceIp.uwords<<",ServerIP:"<<UpperPcIp_<<",LongPort:"<<LongPort_<<",ShortPort:"<<ShortPort_<<",successful!\n";
    std::cout<<"ss";
    Log_pub(ss.str(),LOGTYPE::INFO);
}

//从上位机获取信息，解析，并将具体协议内容下发给setting_node节点具体解析，此处只是将消息分类
void TraBridge::dataUnpack(uint8_t *data, unsigned int data_len)
{
  uint32_t len_ = 0;
  uint8_t crc = 0;
  for(int i = 1;i < data_len - 2; i++){//除了0 和倒数2位，倒数第一位是结尾，倒数第二位是crc校验位
      crc ^=data[i];//crc，校验
      recvbuffer_->add_data(data[i]);//将数据放入vector_buff中
      null_vector_.push_back(data[i]);//日志记录data
  }
  //crc校验
  if(crc != data[data_len - 2]){
    cout<<"crc or tail is error"<<endl;
    Log_pub("crc or tail is error",LOGTYPE::ERROR);
    recvbuffer_->empty_data();//清空内存

    vector<float> data = {(float)ERRORTYPE::CRC_ERR};//校验码错误
    if(tcpInterface_->long_send(errback_->packData(data,OperType::OPER_TYPE_RESPOND_ERROR,ObjTag::OBJ_TAG_DATA_ERRORINFO),errback_->getBufferLen()) == -1)
    {
       string str_log = tcpInterface_->get_log();
       cout<<"crc_err data responed failed!"<<endl;
       Log_pub(str_log+"crc_err data responed failed!",LOGTYPE::WARRING); 
    }
    else{
       string str_log = tcpInterface_->get_log();
       Log_pub(str_log+"crc_err data responed successful!",LOGTYPE::INFO); 
    }
    recvbuffer_->empty_data();//清空内存
    return;
  }
  cout<<"cross through crc!"<<endl;
  Log_pub("cross through crc!",LOGTYPE::INFO);
  int index = 0;
  uWord_16 lianluID;//取链路地址
  uint8_t agr_v,oper_type,obj_tag;
  if(recvbuffer_->get_data(index) & 0x01){//末尾是1，表示链路为单字节
    lianluID.uwords = (uint16_t)(recvbuffer_->get_data(index++)>>2 & 0xFF); 
  }else{
    lianluID.word_s.byte0 = (uint8_t)(recvbuffer_->get_data(index++)>>2 & 0xFF);
    lianluID.word_s.byte1 = (uint8_t)(recvbuffer_->get_data(index++)>>2 & 0xFF);
  }

  agr_v = recvbuffer_->get_data(index++) & 0xFF;//取协议版本
  if(!Judge_compatibility(agr_v)){//版本不兼容
    cout<<"protocol_vision is not compatibility"<<endl;
    Log_pub("protocol_vision is not compatibility",LOGTYPE::ERROR);
    recvbuffer_->empty_data();//清空内存

    vector<float> data = {(float)ERRORTYPE::VISION_ERR};//协议不兼容
    if(tcpInterface_->long_send(errback_->packData(data,OperType::OPER_TYPE_RESPOND_ERROR,ObjTag::OBJ_TAG_DATA_ERRORINFO),errback_->getBufferLen()) == -1)
    {//发送错误回应
       string str_log = tcpInterface_->get_log();
       cout<<"protocol_vision responed failed!"<<endl;
       Log_pub(str_log+"protocol_vision responed failed!",LOGTYPE::WARRING); 
    }
    else{
       string str_log = tcpInterface_->get_log();
       Log_pub(str_log+"protocol_vision responed successful!",LOGTYPE::INFO); 
    }
    recvbuffer_->empty_data();//清空内存
  }
  oper_type = recvbuffer_->get_data(index++) & 0xFF;//取操作类型
  obj_tag = recvbuffer_->get_data(index++) & 0xFF;//取对象类型
//1,0x81上位机下发设置信息
  if(oper_type == OperType::OPER_TYPE_TRA_SETTING_INFO){//配置信息，判断操作类型为信号机发送设置消息
    cout<<"this is a setting message!"<<endl;
    Log_pub("This is a setting message!",LOGTYPE::INFO);//调试和log

    all_msgs_srvs::settings setting_ros;//定义ros内部消息格式
    setting_ros.frame_id = lianluID.uwords;//填写设备链路地址，即设备编号和与编号
    for(int i = index;i<recvbuffer_->get_data_len();i++)//循环添加数据到ros消息中
      setting_ros.setdatas.push_back((float)recvbuffer_->get_data(i));

    //根据具体的配置信息，给ros的setting消息来定义不同的type,和which_param
    if(obj_tag == ObjTag::OBJ_TAG_SET_ONLINE)//连机请求
    {
      deal_online_setting();
    }
    else if(obj_tag == ObjTag::OBJ_TAG_SET_TIMING)//对时设置
    {
      deal_time_setting(setting_ros);
    }
    else if(obj_tag == ObjTag::OBJ_TAG_SET_SETPARAM)//参数设置
    {
      deal_param_setting(setting_ros);
    }
    else if(obj_tag == ObjTag::OBJ_TAG_SET_PULSEUPLOADMODE)//脉冲模式设置
    {
      deal_pulsemode_setting(setting_ros);
    }
  }
//2,0x80信号机查询消息
  else if(oper_type == OperType::OPER_TYPE_TRA_INFO_REQUEST){//信号机查询消息
    cout<<"this a required msg!"<<endl;
    Log_pub("This a required msg!",LOGTYPE::INFO);

    all_msgs_srvs::settingcallback srvdata;//这里使用了ros通讯机制中的server/client的机制，即client带着请求数据，请求服务，server收到请求后，处理有一个反馈结果返回给client;
    //注意：server定义在setting_node中
    srvdata.request.fram_id = lianluID.uwords;
    if(obj_tag == ObjTag::OBJ_TAG_SET_ONLINE){//联机查询，信号机10s发送一次
      deal_online_inquire();
    }
    else if(obj_tag == ObjTag::OBJ_TAG_SET_TIMING){//时间查询
      deal_time_inquire();
    }
    else if(obj_tag == ObjTag::OBJ_TAG_SET_SETPARAM){//参数查询
      deal_setparam_inquire(srvdata);
    }    
    else if(obj_tag == ObjTag::OBJ_TAG_DATA_HISTORYDATA){//历史数据查询
       //****还要添加时间段****//
      deal_history_inquire(srvdata);
    } 
  }
  recvbuffer_->empty_data();//清空内存
}


void TraBridge::Log_pub(string logstr,uint32_t type)
{
  all_msgs_srvs::systemlog systemlog_;
  systemlog_.device_id = DeviceIp.uwords;
  systemlog_.node_id = "ros2tra_node";
  systemlog_.type = type;
  systemlog_.describe = logstr;
  for(int i=0;i<null_vector_.size();i++){
    systemlog_.data.push_back(null_vector_.at(i));
  }
  vector<float> null_v;
  null_vector_= null_v;
  publog_.publish(systemlog_);
}
bool TraBridge::Judge_compatibility(uint8_t arg_v){//版本兼容性判断
  return true;
}
unsigned int TraBridge::preUnpack(uint8_t *data,unsigned int data_len){//预处理，将数据中的复用字节还原
  vector<uint8_t> data_cp;
  data_cp.push_back(data[0] & 0xFF);
  for(int i=1;i<data_len-1;i++){
    if(data[i] & 0xFF == 0x7D){
      if(data[i+1] & 0xFF == 0x5D){
        data_cp.push_back(0x7D);
      }else{
        data_cp.push_back(0x7E);        
      }
      i++;
    }
  }
  data_cp.push_back(data[data_len-1] & 0xFF);
  data_len = data_cp.size();//置换后的列表
  for(int i=0;i<data_len;i++){
    data[i] = data_cp[i];
  }
  return data_len;
}
//----------------------设置处理--------------------//
//处理联机设置
void TraBridge::deal_online_setting(){
  cout<<"this is online setting msgs!"<<endl;
  Log_pub("This is online setting msgs!",LOGTYPE::INFO);

  if(tcpInterface_->long_send(magback_->packData(null_ptr,OperType::OPER_TYPE_DET_RESPOND_SETTING,ObjTag::OBJ_TAG_SET_ONLINE),magback_->getBufferLen()) == -1)
  {
     string str_log = tcpInterface_->get_log();
     cout<<"online responed failed!"<<endl;
     Log_pub(str_log+"online responed failed!",LOGTYPE::WARRING);       
  }
}
//处理对时设置
void TraBridge::deal_time_setting(all_msgs_srvs::settings &ros_set){
  cout<<"this is time setting msgs!"<<endl;
  Log_pub("This is time setting msgs!",LOGTYPE::INFO);

  ros_set.type = SettingType::SETTING_PARAM_INFO;//信号机参数配置
  ros_set.which_param = ROSParamType::TRA_TIME_INFO_SETTING;//信号机对时操作

  pubset_.publish(ros_set);//发送配置信息到节点,进行解析
  Log_pub("Send time setting msgs to setting_node",LOGTYPE::INFO);

  if(tcpInterface_->long_send(timeback_->packData(null_ptr,OperType::OPER_TYPE_DET_RESPOND_SETTING,ObjTag::OBJ_TAG_SET_TIMING),timeback_->getBufferLen()) == -1)
  {
     string str_log = tcpInterface_->get_log();
     cout<<"time setting responed failed!"<<endl;
     Log_pub(str_log+"time setting responed failed!",LOGTYPE::WARRING);       
  }
}
//参数设置
void TraBridge::deal_param_setting(all_msgs_srvs::settings &ros_set){
  cout<<"this is param setting msgs!"<<endl;
  Log_pub("This is param setting msgs!",LOGTYPE::INFO);

  ros_set.type = SettingType::SETTING_PARAM_INFO;//信号机参数配置
  ros_set.which_param = ROSParamType::TRA_PARAM_INFO_SETTING;//配置一般参数

  pubset_.publish(ros_set);//发送配置信息到节点,进行解析
  Log_pub("Send param setting msgs to setting_node",LOGTYPE::INFO);

  if(tcpInterface_->long_send(magback_->packData(null_ptr,OperType::OPER_TYPE_DET_RESPOND_SETTING,ObjTag::OBJ_TAG_SET_SETPARAM),magback_->getBufferLen()) == -1)
  {
     string str_log = tcpInterface_->get_log();
     cout<<"param setting responed failed!"<<endl;
     Log_pub(str_log+"param setting responed failed!",LOGTYPE::WARRING);       
  }
}
//脉冲模式设置
void TraBridge::deal_pulsemode_setting(all_msgs_srvs::settings &ros_set){
  cout<<"this is pulsemode setting msgs!"<<endl;
  Log_pub("This is pulsemode setting msgs!",LOGTYPE::INFO);

  ros_set.type = SettingType::SETTING_PARAM_INFO;//信号机参数配置
  ros_set.which_param = ROSParamType::TRA_PULSE_INFO_SETTING;//信号机配置脉冲参数

  pubset_.publish(ros_set);//发送配置信息到节点,进行解析
  Log_pub("Send pulsemode setting msgs to setting_node",LOGTYPE::INFO);

  if(tcpInterface_->long_send(magback_->packData(null_ptr,OperType::OPER_TYPE_DET_RESPOND_SETTING,ObjTag::OBJ_TAG_SET_PULSEUPLOADMODE),magback_->getBufferLen()) == -1)
  {
     string str_log = tcpInterface_->get_log();
     cout<<"pulsemode setting responed failed!"<<endl;
     Log_pub(str_log+"pulsemode setting responed failed!",LOGTYPE::WARRING);       
  }

}
//---------------------------查询分处理-----------------------------//
//联机查询
void TraBridge::deal_online_inquire(){//联机查询
   cout<<"this is online request msgs!"<<endl;
   Log_pub("This is online request msgs!",LOGTYPE::INFO);

   if(tcpInterface_->long_send(magback_->packData(null_ptr,OperType::OPER_TYPE_DET_RESPOND_INFO,ObjTag::OBJ_TAG_SET_ONLINE),magback_->getBufferLen()) == -1)
   {
      string str_log = tcpInterface_->get_log();
      cout<<"online request responed failed!"<<endl;
      Log_pub(str_log+"online request responed failed!",LOGTYPE::WARRING);       
   }
}
//时间查询
void TraBridge::deal_time_inquire(){
   cout<<"this is time inquiret msgs!"<<endl;
   Log_pub("This is time inquire msgs!",LOGTYPE::INFO);

   if(tcpInterface_->long_send(timeback_->packData(null_ptr,OperType::OPER_TYPE_DET_RESPOND_INFO,ObjTag::OBJ_TAG_SET_TIMING),timeback_->getBufferLen()) == -1)
   {
      string str_log = tcpInterface_->get_log();
      cout<<"time inquire responed failed!"<<endl;
      Log_pub(str_log+"time inquire responed failed!",LOGTYPE::WARRING);       
   }  
}
//配置参数请求
void TraBridge::deal_setparam_inquire(all_msgs_srvs::settingcallback &srvdata){
  cout<<"this require param msg!"<<endl;
  Log_pub("This require param msg!",LOGTYPE::INFO);
  srvdata.request.type = SettingType::SETTING_PARAM_INFO;
  if (serClient_.call(srvdata))//ros的client向server请求服务
  {
    cout<<"call srv for param successful in ros2up_node"<<endl;
    Log_pub("Call srv for param successful in ros2up_node",LOGTYPE::INFO);
    if(tcpInterface_->long_send(paramback_->packData(srvdata.response.setdatas,OperType::OPER_TYPE_DET_RESPOND_INFO,ObjTag::OBJ_TAG_SET_SETPARAM),paramback_->getBufferLen()) == -1)
    {
       string str_log = tcpInterface_->get_log();
       cout<<"param inquire responed failed!"<<endl;
       Log_pub(str_log+"param inquire responed failed!",LOGTYPE::WARRING);       
    }  
  }
}
//历史数据请求
void TraBridge::deal_history_inquire(all_msgs_srvs::settingcallback &srvdata){
  cout<<"this require history data msg!"<<endl;
  Log_pub("This require history data msg!",LOGTYPE::INFO);
  srvdata.request.type = SettingType::REQUEST_HISTORY_DATA;

  if (serClient_.call(srvdata))//ros的client向server请求服务
  {
    cout<<"call srv for history data successful in ros2up_node"<<endl;
    Log_pub("Call srv for history data successful in ros2up_node",LOGTYPE::INFO);
    if(tcpInterface_->long_send(historyback_->packData(srvdata.response.setdatas,OperType::OPER_TYPE_DET_RESPOND_INFO,ObjTag::OBJ_TAG_DATA_HISTORYDATA),historyback_->getBufferLen()) == -1)
    {
       string str_log = tcpInterface_->get_log();
       cout<<"history data inquire responed failed!"<<endl;
       Log_pub(str_log+"history data inquire responed failed!",LOGTYPE::WARRING);       
    }  
  }
}

//-----------------------------------接受数据回调函数--------------------//
void TraBridge::tcpdataProc(uint8_t *data, unsigned int data_len)//接受数据回调函数
{
   cout<<"Recieved tcp server data!"<<endl;
   Log_pub("Network info responed successful!",LOGTYPE::INFO);
   if((data[0] & 0xFF) == TCPCOL_HEAD::UP_HEAD && (data[data_len -1] & 0xFF) == TCPCOL_HEAD::UP_HEAD)//head
   {
     cout<<"a message from short net in ros2up_node!"<<endl;
     Log_pub("Get a message with 0x7E head from short net!",LOGTYPE::INFO);
     data_len = preUnpack(data,data_len);//预处理，将数据中的复用字节还原
     dataUnpack(data,data_len);
   }
   else{
     cout<<"get unkown data from upper!"<<endl;  
     Log_pub("Get a unkown message without 0x7E head from short net!",LOGTYPE::INFO); 
     vector<float> data = {(float)ERRORTYPE::UNKNOWN_ERR};//添加未知消息错误内容
     if(tcpInterface_->long_send(errback_->packData(data,OperType::OPER_TYPE_RESPOND_ERROR,ObjTag::OBJ_TAG_DATA_ERRORINFO),errback_->getBufferLen()) == -1)
     {
        string str_log = tcpInterface_->get_log();
        cout<<"unkown data responed failed!"<<endl;
        Log_pub(str_log+"Unkown data responed failed!",LOGTYPE::WARRING); 
     }
     else{
        string str_log = tcpInterface_->get_log();
        Log_pub(str_log+"Unkown data responed successful!",LOGTYPE::INFO); 
     }
   }
}



