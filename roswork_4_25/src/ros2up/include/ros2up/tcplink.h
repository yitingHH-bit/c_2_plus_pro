#ifndef __TCPLINK_HEAD__
#define __TCPLINK_HEAD__


#include "stdint.h"
#include <pthread.h>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

using namespace boost;


typedef boost::function<void(uint8_t *data, unsigned int data_len)> tcp_recv_cb;


class Tcplink
{
  public:
    bool tcpconnect(const char* serverip, int port, tcp_recv_cb cb);
    int send(uint8_t *data, int len);
    void close(void);
    ~Tcplink();
  private:
    int running_;
    pthread_t recv_thread_;
    //static void *tcp_recv(void *p);
    static void *tcp_recv(void *p);
    tcp_recv_cb recv_cb_;
    int socket_fd_;


};

#endif

