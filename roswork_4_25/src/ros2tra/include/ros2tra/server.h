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


class Server
{
  public:
    bool createServer(int longport, int shortport,  tcp_recv_cb cb,const char* serverip = NULL);
    int long_send(uint8_t *data, int len);//给长链接客户端发送信息
    int short_response(uint8_t *data, int len);//给短链接客户端发送信息
    int get_clientnum();//获取客户端个数
    string get_log();//获取日志信息
    void Add_log(string logadd);//添加日志信息
    void close(void);
    ~Server();
  private:
    string log_str;//通讯日志内容
    pthread_mutex_t mutex_;  //线程锁
    int listen_running_ = 0;
    int short_running_ = 0;
    int long_netcheck_ = 0;
    pthread_t recv_thread_[3] = {0,1,2};
    static void *tcp_recv(void *p);
    static void *listen_long(void *p);//监听长链接的客户
    static void *listen_long_state(void *p);//定时监听长链接状态
    tcp_recv_cb recv_cb_;
    int long_listen_fd_;
    int short_listen_fd_;
    int short_client_fd_;
    vector<int> clientcnn_;//请求连接上的客户端
    vector<int>::iterator clientcnn_it_;//
};

#endif

