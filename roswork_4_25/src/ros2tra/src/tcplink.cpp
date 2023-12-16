#include "tcpInterface.h"
#include <errno.h>
#include <pthread.h>


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

void* Tcplink::tcp_recv(void *p)
{
    uint8_t recvbuff[9600];
    int recvlen = 0;
    Tcplink *pThis = (Tcplink *)p;
    while (pThis->running_)
    {
        memset(recvbuff, 0, sizeof(recvbuff));
        if ((recvlen = read(pThis->socket_fd_, recvbuff, sizeof(recvbuff))) == -1)
        {
            DBG_LOG_ERROR("tcp_recv error:%d", errno);
            continue;
        }
        pThis->recv_cb_(recvbuff, recvlen);
    }
    return NULL;
}

Tcplink::~Tcplink(){
    close();
}

Tcplink::Tcplink(const char* serverip, int port, tcp_recv_cb cb){
 if(!connectServer(serverip,port,cb)){
   return ;
 }
}
bool Tcplink::connectServer(const char* serverip, int port, tcp_recv_cb cb){//链接服务器
    int res;
    pthread_attr_t attr;
    recv_thread_ = 0;
    DBG_LOG_TRACE("connect server %s start\n", serverip);
    //句柄检查
    if (NULL == serverip || NULL == cb)
    {
        DBG_LOG_ERROR("NULL == device || NULL == recv_callback\n");
        return false;
    }

    socket_fd_ = socket(AF_INET, SOCK_STREAM,0);
    if(socket_fd_ == -1)
    {
        DBG_LOG_DEBUG("socket 创建失败：");
        return false;
    }

    recv_cb_ = cb;

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(serverip);

    res = connect(socket_fd_,(struct sockaddr*)&addr,sizeof(addr));
    if(res == -1)
    {
        DBG_LOG_DEBUG("bing 创建失败：");
        goto error;
    }
    DBG_LOG_DEBUG("client 链接成功：");

    running_ = 1;

    //创建线程，接收数据
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); //分离属性
    res = pthread_create(&recv_thread_, &attr, tcp_recv, this);
    if (0 != res)
    {
        DBG_LOG_ERROR("Client recv thread create failed!\n");
        goto error;
    }

    DBG_LOG_TRACE("open Client %s end\n", serverip);
    return true;

error:
    running_ = 0;
    if (socket_fd_ > 0)
    {
        ::close(socket_fd_);
    }
    if (0 != recv_thread_)
    {
        pthread_join(recv_thread_, NULL);
    }
    return false;
}
int Tcplink::long_send(uint8_t *data, int len){
    int ret = 0;
    int sended_len = 0;

    if (socket_fd_ <= 0)
    {
        return -1;
    }
    //反复发送，直到全部发完
    while (sended_len != len)
    {
        int retlen = 0;
        //cout<<"wait send len:"<<len<<",sended_len:"<<sended_len<<endl;
        retlen = write(socket_fd_, data + sended_len, len - sended_len);
        //retlen = write(socket_fd_, data, len);
        if (retlen < 0)
        {
            DBG_LOG_ERROR("client send failed! ret=%d\n", ret);
            return -1;
        }
        //cout<<"sended len:"<<retlen<<endl;
        sended_len += retlen;
    }
    
    return ret;


}
void Tcplink::close(void){
    running_ = 0;
    if (socket_fd_ > 0)
    {
        ::close(socket_fd_);
    }
    if (0 != recv_thread_)
    {
        pthread_join(recv_thread_, NULL);
    }


}

