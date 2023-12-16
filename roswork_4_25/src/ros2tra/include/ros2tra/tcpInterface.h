#ifndef __SERVER_HEAD__
#define __SERVER_HEAD__


#include "stdint.h"
#include <pthread.h>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>

//using namespace boost;
using namespace std;


typedef boost::function<void(uint8_t *data, unsigned int data_len)> tcp_recv_cb;


class TcpInterface
{
  public:
    //bool createServer(int longport, int shortport,  tcp_recv_cb cb,const char* serverip = NULL);
    TcpInterface(){};
    ~TcpInterface(){};
    virtual int long_send(uint8_t *data, int len){};//给长链接客户端发送信息
    virtual int short_response(uint8_t *data, int len){};//给短链接客户端发送信息
    string get_log(){
      string log = log_str;
      log_str = "";
      return log;
    };//获取日志信息
    void Add_log(string logadd){
      log_str+=logadd + "\n\t\t\t";
    };//添加日志信息
    virtual void close(void){};
  protected:
    string log_str;//通讯日志内容
    tcp_recv_cb recv_cb_;//接受函数回调用
};

class Server : public TcpInterface
{
  public:
    Server(int longport, int shortport,  tcp_recv_cb cb,const char* serverip = NULL);
    int long_send(uint8_t *data, int len);//给长链接客户端发送信息
    int short_response(uint8_t *data, int len);//给短链接客户端发送信息
    int get_clientnum();//获取客户端个数
    void close(void);
    ~Server();
  private:

    pthread_mutex_t mutex_;  //线程锁
    int listen_running_ = 0;
    int short_running_ = 0;
    int long_netcheck_ = 0;
    pthread_t recv_thread_[3] = {0,1,2};
    static void *tcp_recv(void *p);
    static void *listen_long(void *p);//监听长链接的客户
    static void *listen_long_state(void *p);//定时监听长链接状态

    int long_listen_fd_;
    int short_listen_fd_;
    int short_client_fd_;
    vector<int> clientcnn_;//请求连接上的客户端
    vector<int>::iterator clientcnn_it_;//
};

class Tcplink : public TcpInterface
{
  public:
    Tcplink(const char* serverip, int port, tcp_recv_cb cb);
    //int send(uint8_t *data, int len);
    int long_send(uint8_t *data, int len);//给长链接客户端发送信息
    int short_response(uint8_t *data, int len){};//给短链接客户端发送信息
    void close(void);
    bool connectServer(const char* serverip, int port, tcp_recv_cb cb);//链接服务器
    ~Tcplink();
  private:
    int running_;
    pthread_t recv_thread_;
    static void *tcp_recv(void *p);
    int socket_fd_;

};

#endif

