#ifndef __RECV_HEAD__
#define __RECV_HEAD__


#include "stdint.h"
#include <pthread.h>
#include "boost/bind.hpp"
#include "boost/function.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <cmath>

using namespace std;


//typedef boost::function<void(uint8_t *data, unsigned int data_len)> tcp_recv_cb;


class RecvBuffer
{
  public:
    RecvBuffer(uint8_t *data, unsigned int data_len);
    uint8_t get_data(int index);
    int add_data(uint8_t *data ,unsigned int data_len);
    int add_data(uint8_t data);
    int remove_data(unsigned int data_len);
    int empty_data();
    int get_data_len();
    ~RecvBuffer();
  private:
    uint8_t *buffer_;
    unsigned int buffer_len_;
    unsigned int data_len_;
    unsigned int data_index_;
    bool isCover;
    
    


};

#endif

