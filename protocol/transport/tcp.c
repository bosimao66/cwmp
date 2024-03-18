 


//设想：本地管理（静态结构体变量），然后通过某个接口，汇总起来，
//把管理权限给到一个“超级管理员”…………

#include <string.h>
#include <stdlib.h>
#include <stdio.h>


#include "tcp.h"
#include "linux.h"
#include "log.h"
#include "pool2.h"
#include "linux_itf.h"





//===========================
//tcp对象
//==========================
#define TCP_IPV4_NAME_MAX_LEN 64
#define TCP_USER_NUM 32

typedef struct __tcpUser_obj{       //用户
    char ipv4[TCP_IPV4_NAME_MAX_LEN];
    int port;    
    int fd;
    char used;
}tcpUser_obj_t;


typedef struct __tcp_server{
    char ipv4[TCP_IPV4_NAME_MAX_LEN];
    int port;
    int fd;
    tcpUser_obj_t user[TCP_USER_NUM];
    //int userCnt;
    pthread_t accept;
}tcp_server_t;


//客户端
typedef struct __tcp_clinet{
    char ipv4[TCP_IPV4_NAME_MAX_LEN];
    int port;
    int fd;
}tcp_client_t;






//使用内存池
int pool_id_g = -1;
char pool_name_g[] = "tcp";
#define POOL_MALLOC(SIZE) pool_user_malloc(pool_id_g, SIZE)
#define POOL_FREE(HANDLE) pool_user_free(HANDLE)



//用户列表管理-占用
int tcpUser_get_id(tcp_server_t *tcp)
{
    int i;
    for(i = 0; i < TCP_USER_NUM; i++)
    {
        if(tcp->user[i].used == 0)
        {
            tcp->user[i].used = 1;
            return i;
        }
    }
    return -1;  //调用者，可能会导致数组溢出
}

//用户列表管理-释放
void tcpUser_release(tcp_server_t *tcp, int id)
{
    if(id >= 0 && id < TCP_USER_NUM)
    {
        tcp->user[id].used = 0;
    }
}

//用户列表管理-获取用户数量
int tcpUser_cnt_get(tcp_server_t *tcp)
{
    int i, ret;
    ret = 0;
    for(i = 0; i < TCP_USER_NUM; i++)
    {
        if(tcp->user[i].used == 1)
        {
            ret++;
        }
    }
    return ret;
}

//用户列表管理-匹配
int tcpUser_match_fd(tcp_server_t *tcp, int fd)
{
    int i;
    for(i = 0; i < TCP_USER_NUM; i++)
    {
        if(tcp->user[i].fd == fd)
        {
            return i;
        }
    }
    return -1;
}



//tcp服务器对象创建
tcp_server_t *tcp_server_create(char *ipv4, int port)
{
    int i;
    
    tcp_server_t *tcp = (tcp_server_t *)POOL_MALLOC(sizeof(tcp_server_t));
    if(ipv4 != NULL)strncpy(tcp->ipv4, ipv4, TCP_IPV4_NAME_MAX_LEN);
    tcp->port = port;
    

    for(i = 0; i < TCP_USER_NUM; i++)
    {
        tcp->user[i].used = 0;
        tcp->user[i].fd = -1;
    }
    //tcp->userCnt = 0;
    return tcp;
}

//tcp服务器对象摧毁
void tcp_server_destory(tcp_server_t *tcp)
{
    if(tcp != NULL)POOL_FREE(tcp);
}


//服务器客户端创建
tcp_client_t *tcp_client_create(char *ipv4, int port)
{
    //int i;
    
    tcp_client_t *client = (tcp_client_t *)POOL_MALLOC(sizeof(tcp_client_t));
    if(ipv4 != NULL)strncpy(client->ipv4, ipv4, TCP_IPV4_NAME_MAX_LEN);
    client->port = port;

    return client;
}

//服务器客户端摧毁
void tcp_client_destory(tcp_client_t * client)
{
    POOL_FREE(client);
}


//初始化
void tcp_init()
{
    pool_id_g = pool_appy_user_id(pool_name_g);
}






//接受线程
void *thread_tcp_accept(void *in)
{
    if(in == NULL){LOG_ALARM("thread accept");return NULL;}
    tcp_server_t *tcp = (tcp_server_t *)in;

    int userId, fd, port;
    char ipv4[TCP_IPV4_NAME_MAX_LEN] = {0};
    
    //tcp->user[userId].used = 
    while(1)
    {
        fd = LINUX_itf_accept(tcp->fd, (char *)ipv4, TCP_IPV4_NAME_MAX_LEN, &port);
        if(fd != -1)//代表成功
        {
            while(1)    //申请用户列表资源
            {
                userId = tcpUser_get_id(tcp);
                if(userId >= 0)
                {
                    strncpy(tcp->user[userId].ipv4, ipv4, TCP_IPV4_NAME_MAX_LEN);
                    tcp->user[userId].port = port;
                    tcp->user[userId].used = 1;
                    tcp->user[userId].fd = fd;
                    break;
                }
             
                LOG_ALARM("userId");
                system("sleep 3");    
            }


            
             
        }
        else
        {
            LOG_ALARM("accept");
            system("sleep 3");
        }
    }
    
    return NULL;                           
}



//开启tcp服务器，8080端口
int tcp_server_start(tcp_server_t *tcp)
{
    int ret;
    //绑定
    ret = LINUX_itf_bind(tcp->ipv4, tcp->port, &(tcp->fd));
    if(ret < 0){LOG_ALARM("bind"); return ret;};
    
    //监听
    ret = LINUX_itf_listen(tcp->fd);
    if(ret < 0){LOG_ALARM("listen"); return ret;};
    
    //等待连接（开启线程）
    /*int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);*/
    ret = pthread_create(&(tcp->accept), NULL, thread_tcp_accept, (void *)tcp);
    if(ret != 0){LOG_ALARM("pthread"); return ret;};

    return 0;
}



//开启tcp客户端，8081端口
int tcp_client_start(tcp_client_t *client, char *ipv4, int port)
{
    int ret;
    if(client == NULL){LOG_ERROR("clinet NULL"); return -1;};
    if(ipv4 == NULL){LOG_ALARM("clinet ipv4"); return -1;};
    
    //绑定
    ret = LINUX_itf_bind(client->ipv4, client->port, &(client->fd));
    if(ret < 0){LOG_ALARM("clinet bind"); return ret;};
    
    //连接
    ret = LINUX_itf_connect(client->fd, ipv4, port);
    if(ret < 0){LOG_ALARM("clinet connect"); return ret;};

    LOG_INFO("connect to [%s:%d] OK", ipv4, port);
    return 0;
}

//读写数据
/*============================================================
#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count);

ssize_t read(int fd, void *buf, size_t count)


返回值
成功：读取/写入的字节数
失败：-1
==============================================================*/
int tcp_server_write(tcp_server_t *tcp, int userId, char *msg, int size)
{
    if(userId < 0 || userId > TCP_USER_NUM)
    {
        LOG_ALARM("userId");
        return -1;
    }
    return write(tcp->user[userId].fd, msg, size);
}

int tcp_server_read(tcp_server_t *tcp, int userId, char *content, int size)
{
    if(userId < 0 || userId > TCP_USER_NUM)
    {
        LOG_ALARM("userId");
        return -1;
    }
    return read(tcp->user[userId].fd, content, size);
}

int tcp_client_send(tcp_client_t *cl, char *msg, int size)
{
    return write(cl->fd, (const void *)msg, size);
}

//接收数据
int tcp_client_read(tcp_client_t *cl, char *content, int size)
{
    return read(cl->fd, (void *)content, size);
}



//测试
/*
#include <poll.h>
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
*/
void tcp_test(char *ipv4, int port)
{
    int i, ret;
    //int userId;
    char buf512[512] = {0};
    int len;
    //int firstMark;

    tcp_init();

    tcp_server_t *tcp = tcp_server_create(ipv4, port);

    tcp_server_start(tcp);
    
    struct pollfd fds[TCP_USER_NUM] = {0};
    
    pool_show();

    

    while (1) 
    {
        for(i = 0; i < TCP_USER_NUM; i++)
        {
            if(tcp->user[i].used == 1)
            {
                fds[i].fd = tcp->user[i].fd;
                fds[i].events = POLLIN;
            }
            else
            {
                memset(&(fds[i]), 0, sizeof(struct pollfd));
            }
        }
        
        ret = poll(fds, TCP_USER_NUM, 2000); // 阻塞直到有事件发生或超时
        if (ret == -1) 
        {
            LOG_ALARM("poll");
            system("sleep 3");
            continue;
        }
        
        for(i = 0; i < TCP_USER_NUM; i++)
        {
            if ((tcp->user[i].used == 1) && (fds[i].revents & POLLIN))  //有数据进入
            {
                len = read(fds[i].fd, buf512, 24);
                if(len == 0)  //读取的长度len大于0，说明有数据可读；等于0，代表断开连接，小于0，没有数据
                {
                    LOG_INFO("i=%d [%s-%d] 断开连接\n", i, tcp->user[i].ipv4, tcp->user[i].port);
                    tcpUser_release(tcp, i);
                }
                else
                {
                     LOG_SHOW("-----------------------i=%d [%s-%d]msg %dbyte--------------------\n%s\n",
                                        i, tcp->user[i].ipv4, tcp->user[i].port, len, buf512);
                }
            }
        }
        
    }

}



//测试
void tcp_client_test(char *ipv4, int port, char *targetIpv4, int targetPort)
{
    char buf521[512] = {0};

    tcp_init();

    tcp_client_t *client = tcp_client_create(ipv4, port);

    tcp_client_start(client, targetIpv4, targetPort);  
    
    snprintf(buf521, 512, "this msg form is client [%s:%d]!", ipv4, port);
    tcp_client_send(client, buf521, strlen(buf521));

    

    while(1)
    {
        system("sleep 3");

        snprintf(buf521, 512, "this is second msg……!");
        tcp_client_send(client, buf521, strlen(buf521));

    }

}



