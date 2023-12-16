#include <recvbuffer.hpp>

RecvBuffer::RecvBuffer(uint8_t *data, unsigned int data_len){
  buffer_ = data;
  buffer_len_ = data_len;
  data_len_ = 0;
  data_index_ = 0;
  isCover = false;
}

uint8_t RecvBuffer::get_data(int index = 0){
  if(index < 0)
    index = 0;
  return buffer_[(data_index_ + index)%buffer_len_];
}

int RecvBuffer::add_data(uint8_t *data ,unsigned int data_len){
  if (data_len==1){
    buffer_[(data_index_ + data_len_)%buffer_len_] = data[0];
    data_len_++;
  }
  for(int i=0;i<data_len;i++){
    add_data(&data[i],1);
  }
}
int RecvBuffer::add_data(uint8_t data){
    buffer_[(data_index_ + data_len_)%buffer_len_] = data;
    data_len_++;
}

int RecvBuffer::get_data_len(){
  return (int)data_len_;
}

int RecvBuffer::remove_data(unsigned int data_len){
  if(data_len > data_len_)
    data_len = data_len_;
  data_len_ = data_len_ - data_len;
  data_index_ = (data_index_ + data_len)%buffer_len_;
  
}

int RecvBuffer::empty_data(){
  data_len_ = 0;
  data_index_ = 0;
  return data_len_;
}

RecvBuffer::~RecvBuffer(){


}
