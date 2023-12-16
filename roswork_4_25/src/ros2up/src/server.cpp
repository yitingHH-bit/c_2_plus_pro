#include "server.h"
#include <errno.h>
#include <pthread.h>
#include <sstream>
using namespace std;


#define DBG_LOG_TRACE(format, ...) \
    printf(format, ##__VA_ARGS__); \
    printf("\n")
#define DBG_LOG_DEBUG(format, ...) \
    printf(format, ##__VA_ARGS__); \
    printf("\n")
#define DBG_LOG_ERROR(format, ...) \
    printf(format, ##__VA_ARGS__); \
    printf("\n")
#define DBG_LOG_WARNING(format, ...) \
    printf(format, ##__VA_ARGS__);   \
    printf("\n")

using namespace std;
void *Server::listen_long(void *p)
{
  Server *pThis = (Server *)p;
  int connfd;
  cout<<"pThis->listen_running_:"<<pThis->listen_running_<<endl;
  while(pThis->listen_running_){
    cout<<"long listen start"<<endl;
    if( (connfd = accept(pThis->long_listen_fd_, (struct sockaddr*)NULL, NULL)) == -1){
      printf("accept long socket error: %s(errno: %d)",strerror(errno),errno);
      ostringstream ss;
      ss<<"accept long socket error:"<<strerror(errno)<<"(errno:"<<errno<<")";
      pThis->Add_log(ss.str());
      continue;
    }
    else
    {
      pthread_mutex_lock(&pThis->mutex_);  //
      cout<<"One log client add successful"<<endl;
      pThis->Add_log("One log client add successful");
      pThis->clientcnn_.push_back(connfd);//添加套字节
      pthread_mutex_unlock(&pThis->mutex_);  //
    }
  }
}
void* Server::tcp_recv(void *p)
{
    uint8_t recvbuff[9600];
    int recvlen = 0;
    Server *pThis = (Server *)p;
    cout<<"short client listen create start!:"<<pThis->short_running_<<endl;
    pThis->Add_log("Short client listen create start!");
    while (pThis->short_running_)
    {
        cout<<"short client listen start!"<<endl;
        pThis->Add_log("Short client listen start!");
        if( (pThis->short_client_fd_ = accept(pThis->short_listen_fd_, (struct sockaddr*)NULL, NULL)) == -1){
          printf("accept short socket error: %s(errno: %d)",strerror(errno),errno);
          ostringstream ss;
          ss<<"accept short socket error:"<<strerror(errno)<<"(errno:"<<errno<<")";
          pThis->Add_log(ss.str());
          continue;
        }
        else{
           cout<<"one short client conn!"<<endl;
           pThis->Add_log("One short client connecte successful!");
        }
        memset(recvbuff, 0, sizeof(recvbuff));
        if ((recvlen = read(pThis->short_client_fd_, recvbuff, sizeof(recvbuff))) == -1)
        {
            DBG_LOG_ERROR("tcp_recv error:%d", errno);
            ostringstream ss;
            ss<<"Short connection recv error:"<<strerror(errno)<<"(errno:"<<errno<<")";
            pThis->Add_log(ss.str());
            continue;
        }
        else{
            cout<<"recv one short data!"<<endl;
            pThis->Add_log("Recv one short data in TCP");
        }
        pThis->recv_cb_(recvbuff, recvlen);
    }
    return NULL;
}

void* Server::listen_long_state(void *p){//定时监听长链接状态
  Server *pThis = (Server *)p;
  int connfd;
  int times = 0;
  while(pThis->long_netcheck_){
    sleep(5);//5秒
    //cout<<"now have :"<<pThis->clientcnn_.size()<<"long client connection!"<<endl;
    if(pThis->clientcnn_.size() > 0){//定期去检查
      //cout<<"client net check one!"<<endl;
      //pThis->Add_log("Check long client net state in TCP");
      vector<int> indexlist;
      int index = -1;
      for(pThis->clientcnn_it_ = pThis->clientcnn_.begin();pThis->clientcnn_it_!=pThis->clientcnn_.end();pThis->clientcnn_it_++)
      {
         index++;
         uint8_t buff = 0;
         int n = write(*pThis->clientcnn_it_,&buff, 1);
         //cout<<"long net check send:"<<n<<"byte data to client!"<<endl;
         ostringstream ss;
         ss<<"long net check send:"<<n<<"byte data to client!";
         //pThis->Add_log(ss.str());    
         if(n <= 0){
             //cout<<"one long client net break!"<<endl;
             pThis->Add_log("One long client net break! in TCP");
             indexlist.push_back(index);
         }
      }
      for(int i=indexlist.size();i>0;i--){
        pthread_mutex_lock(&pThis->mutex_);//
        ::close(*(pThis->clientcnn_.begin() + indexlist.at(i-1)));//
        //cout<<"close long client net!"<<endl;
        pThis->Add_log("Close long client net! in TCP");
        pThis->clientcnn_.erase(pThis->clientcnn_.begin() + indexlist.at(i-1));
        pthread_mutex_unlock(&pThis->mutex_);//
      }
    }
  }
}

Server::~Server(){
    close();
    pthread_mutex_destroy(&mutex_); //注释3
}

bool Server::createServer( int longport, int shortport, tcp_recv_cb cb, const char* serverip){
    pthread_mutex_init(&mutex_, NULL);  //init
    int res;
    pthread_attr_t attr;
    //recv_thread_ = 0;
    //DBG_LOG_TRACE("connect server %s start\n", serverip);
    //句柄检查
    if (NULL == cb)//NULL == serverip
    {
        DBG_LOG_ERROR("NULL == port || NULL == recv_callback\n");
        Add_log("Short recv_callback = NULL! in TCP");
        return false;
    }
    //cout<<"socket enter0!"<<endl;
    long_listen_fd_ = socket(AF_INET, SOCK_STREAM,0);
    struct timeval timeout={3,0};//3s
    int ret=setsockopt(long_listen_fd_,SOL_SOCKET,SO_SNDTIMEO,(const char*)&timeout,sizeof(timeout));
    if(ret == -1){
      cout<<"set long_socket send timeout is failed!"<<endl;
      Add_log("Set long_socket sending timeout is failed! in TCP");
    }
    //int ret=setsockopt(long_listen_fd_,SOL_SOCKET,SO_RCVTIMEO,(const char*)&timeout,sizeof(timeout));
    //if(ret == -1){
      //cout<<"set long_socket recv timeout is failed!"<<endl;
    //}
    if(long_listen_fd_ == -1)
    {
        DBG_LOG_DEBUG("socket 创建失败");
        Add_log("Long socket 创建失败! in TCP");
        exit(-1);
    }

    recv_cb_ = cb;
    //cout<<"socket enter1!"<<endl;
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(longport);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);//serverip);
    //cout<<"socket enter2!"<<endl;
    int bindret = bind(long_listen_fd_, (struct sockaddr*)&addr, sizeof(addr));
    if( bindret == -1){
        printf("bind long socket error: %s(errno: %d)\n",strerror(errno),errno);
        ostringstream ss;
        ss<<"Bind long socket error:"<<strerror(errno)<<"(errno:"<<errno<<")";
        Add_log(ss.str());       
        return false;
    }
    //cout<<"socket enter3!"<<endl;
    if( listen(long_listen_fd_, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        ostringstream ss;
        ss<<"Listen long socket error:"<<strerror(errno)<<"(errno:"<<errno<<")";
        Add_log(ss.str());
        return false;
    }
    //cout<<"socket enter!"<<endl;
    //创建线程，监听长链接
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //分离属性
    cout<<"thread0:"<<recv_thread_[0]<<endl;
    res = pthread_create(&recv_thread_[0], &attr, listen_long, this);
    cout<<"thread0:"<<recv_thread_[0]<<endl;
    if(res == -1)
    {
        DBG_LOG_DEBUG("pthread long listen 创建失败：");
        Add_log("Pthread long listen 创建失败! in TCP");
        goto error;
    }
    DBG_LOG_DEBUG("pthread long listen 创建成功：");
    Add_log("Pthread long listen 创建成功! in TCP");
    listen_running_ = 1;//长链接监听正在进行
    cout<<"listen_running_:"<<listen_running_<<endl;

    //

    short_listen_fd_ = socket(AF_INET, SOCK_STREAM,0);
    //起短链接服务
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(shortport);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(short_listen_fd_, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        printf("bind socket error: %s(errno: %d)\n",strerror(errno),errno);
        ostringstream ss;
        ss<<"Bing short socket error:"<<strerror(errno)<<"(errno:"<<errno<<")";
        Add_log(ss.str());
        return false;
    }

    if( listen(short_listen_fd_, 10) == -1){
        printf("listen socket error: %s(errno: %d)\n",strerror(errno),errno);
        ostringstream ss;
        ss<<"Listen socket error:"<<strerror(errno)<<"(errno:"<<errno<<")";
        Add_log(ss.str());
        return false;
    }
    //创建线程，接受短链接
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //分离属性
    cout<<"thread1:"<<recv_thread_[1]<<endl;
    res = pthread_create(&recv_thread_[1], &attr, tcp_recv, this);
    cout<<"thread1:"<<recv_thread_[1]<<endl;
    if(res == -1)
    {
        DBG_LOG_DEBUG("pthread short server 创建失败：");
        Add_log("Pthread short server 创建失败! in TCP");
        goto error;
    }
    DBG_LOG_DEBUG("pthread short serve 创建成功：");
    Add_log("Pthread short serve 创建成功! in TCP");
    short_running_ = 1;//等待客户端

   // pthread_attr_init(&attr);
   // pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //分离属性
   // res = pthread_create(&recv_thread_[2], &attr, listen_long_state, this);
    //if(res == -1)
   // {
     //   DBG_LOG_DEBUG("pthread long net check 创建失败：");
    //    Add_log("Pthread long net check 创建失败! in TCP");
      //  goto error;
   // }
   // DBG_LOG_DEBUG("pthread long net check  创建成功：");
  //  Add_log("Pthread long net check  创建成功! in TCP");
   // long_netcheck_ = 1;
    return true;

error:
    cout<<"go to error"<<endl;
    listen_running_ =  0;
    short_running_ = 0;
    if (0 != recv_thread_[0])
    {
        pthread_join(recv_thread_[0], NULL);
    }    
    if (0 != recv_thread_[1])
    {
        pthread_join(recv_thread_[1], NULL);
    }
    return false;
}
int Server::short_response(uint8_t *data, int len){
    int ret = 0;
    int sended_len = 0;

    if (short_client_fd_ <= 0)
    {
        return -1;
    }
    //反复发送，直到全部发完
    int times = 0;
    while (sended_len != len)
    {
        times++;
        int retlen = 0;
        retlen = write(short_client_fd_, data + sended_len, len - sended_len);
        if (retlen < 0 || times > 2)
        {
            DBG_LOG_ERROR("client send failed! ret=%d\n", ret);
            Add_log("Short client send failed! in TCP");
            ret = -1;
            break;
        }
        sended_len += retlen;
    }
    cout<<"server short sended len:"<<sended_len<<endl;
    ostringstream ss;
    ss<<"server short sended len:"<<sended_len;
    Add_log(ss.str());
    ::close(short_client_fd_);//断开链接
    cout<<"短链接 服务器断开!"<<endl;
    Add_log("短链接 服务器断开!");
    return ret;
}
int Server::long_send(uint8_t *data, int len){
    int ret = -1;
    pthread_mutex_lock(&mutex_);
    if (clientcnn_.size() <= 0)
    {
        //cout<<"long send:2"<<endl;
        pthread_mutex_unlock(&mutex_);  //
        return -1;
    }
    vector<int> indexlist;
    int index = -1;
    //cout<<"long client num in vehs:"<<clientcnn_.size()<<endl;
    for(clientcnn_it_ = clientcnn_.begin();clientcnn_it_<clientcnn_.end();clientcnn_it_++)
    {
      ret++;
      //cout<<"now send client num in vehs is :"<<clientcnn_.size()<<endl;
      index++;
      int sended_len = 0;
      //反复发送，直到全部发完
      int times = 0;
      //int startime,endtime;
      //startime = clock();
      while (sended_len != len)
      {
        times++;
        int retlen = 0;
        //DBG_LOG_ERROR("long send start! times=%d\n", times);
        retlen = write(*clientcnn_it_, data + sended_len, len - sended_len);
        if (retlen < 0 && times > 2)
        {
            DBG_LOG_ERROR("long send failed! in vehs times=%d\t", times);
            ostringstream ss;
            ss<<"Long send failed! in vehs times="<<times;
            Add_log(ss.str());
            indexlist.push_back(index);
            ret--;
            break;
        }
        //cout<<"vehs sended len:"<<retlen<<endl;
        sended_len += retlen;
        //endtime = clock();
        //cout<<"server send time e2:"<<(endtime - startime)/1000<<endl;
        //if(sended_len == len){
          //cout<<"vehs send successful:"<<sended_len<<endl;
        //}
      }

    }
    for(int i=indexlist.size();i>0;i--){
      ::close(*(clientcnn_.begin() + indexlist.at(i-1)));
      clientcnn_.erase(clientcnn_.begin() + indexlist.at(i-1));
      cout<<"close one long client in vehserver"<<endl;
      Add_log("Close one long client");
    }
    pthread_mutex_unlock(&mutex_);  //
    return ret;
}

int Server::get_clientnum(){
  return clientcnn_.size();
}

string Server::get_log(){
  string log = log_str;
  log_str = "";
  return log;
}
void Server::Add_log(string logadd){//添加日志信息
  log_str+=logadd + "\n\t\t\t";
}

void Server::close(void){
    listen_running_ = 0;
    short_running_ = 0;
    if(clientcnn_.size() > 0){
      for(clientcnn_it_ = clientcnn_.begin();clientcnn_it_!=clientcnn_.end();clientcnn_it_++)
      {
         ::close(*clientcnn_it_);
         cout<<"close long clietn socket"<<endl;
         Add_log("Close long clietn socket");
      }
    }
    if (long_listen_fd_ > 0)
    {
        ::close(long_listen_fd_);
        //closesocket(long_listen_fd_);
        //long_listen_fd_ = NULL;
        cout<<"close long server socket"<<endl;
        Add_log("Close long server socket");
    }
    for(int i=0; i<3;i++){
      if (0 != recv_thread_[i])
      {
        pthread_join(recv_thread_[i], NULL);
      }
    }
}

