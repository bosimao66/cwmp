



//平台依赖linux，接口调用


#if 0
以下是一个简单的 TCP 服务器的示例代码，使用 C 语言和 Linux 套接字编程实现：

c
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
  
#define PORT 8080  
#define BUF_SIZE 1024  
  
int main() {  
    int server_fd, client_fd;  
    struct sockaddr_in server_addr, client_addr;  
    socklen_t client_len = sizeof(client_addr);  
    char buffer[BUF_SIZE];  
  
    // 创建套接字  
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
        perror("socket");  
        exit(1);  
    }  
  
    // 设置服务器地址信息  
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = INADDR_ANY;  
    server_addr.sin_port = htons(PORT);  
  
    // 绑定套接字和地址  
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {  
        perror("bind");  
        exit(1);  
    }  
  
    // 监听连接请求  
    if (listen(server_fd, 5) == -1) {  
        perror("listen");  
        exit(1);  
    }  
  
    printf("Server is listening on port %d...\n", PORT);  
  
    // 接受客户端连接请求  
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len)) == -1) {  
        perror("accept");  
        exit(1);  
    }  
  
    printf("Client connected from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));  
  
    // 读取客户端发送的数据并回复  
    while (1) {  
        memset(buffer, 0, BUF_SIZE);  
        int n = read(client_fd, buffer, BUF_SIZE);  
        if (n <= 0) {  
            break;  
        }  
        printf("Received: %s\n", buffer);  
        write(client_fd, buffer, n);  
    }  
  
    // 关闭套接字和连接  
    close(client_fd);  
    close(server_fd);  
  
    return 0;  
}

这个示例代码创建了一个 TCP 服务器，监听本地的 8080 
端口，等待客户端的连接请求。当客户端连接成功后，服务器会读取客户端发送的数据，�
�回复相同的数据。当客户端断开连接时，服务器也会关闭连接并退出程序。

你可以使用 gcc 编译器编译这个代码，例如：

bash
gcc server.c -o server

然后在终端中运行这个服务器程序：

bash
./server

现在，你可以使用 telnet 或其他 TCP 客户端工具连接到这个服务器，测试它的功能。
#endif

#include "linux_itf.h"

#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  

#include <unistd.h>  
#include <arpa/inet.h>  


#include "log.h"

/*=============================================
listen封装，简化为只输入 ipv4 以及端口

#include <sys/types.h>
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *my_addr, socklen_t addrlen);
返回：成功返回fd，失败-1
===============================================*/
int LINUX_itf_bind(char *ipv4, int port, int *out)
{
    int server_fd;  
    struct sockaddr_in server_addr;  
    
    // 创建套接字  
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {  
        perror("socket");  
        return -1;  
    }  
  
    // 设置服务器地址信息  
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;  
    server_addr.sin_addr.s_addr = inet_addr(ipv4);  
    server_addr.sin_port = htons(port);  

    *out = server_fd;
    return bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

}


/*=============================================
listen封装

#include <sys/socket.h>

int listen(int s, int backlog);
返回：成功0，失败-1
===============================================*/
int LINUX_itf_listen(int fd)
{
    return listen(fd, 5);
}

/*=============================================
accept封装

#include <sys/types.h>
#include <sys/socket.h>

int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
返回：成功fd，失败-1
===============================================*/
int LINUX_itf_accept(int fd, char *ipv4, int size, int *port)
{
    
    struct sockaddr_in addr;  
    socklen_t len = sizeof(addr);  
    //struct sockaddr addr;

    int ret;

    //LOG_SHOW("accept is comming ……\n");
    ret = accept(fd, (struct sockaddr *)&addr, &len); 
    if(ret != -1)
    {
        unsigned char *ip = (unsigned char *)(&(addr.sin_addr.s_addr));
        snprintf(ipv4, size, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);

        //LOG_INFO("--------------------->%s 连接\n", ipv4);
        *port = ntohs(addr.sin_port);  
    }
    
    return ret;
}

/*=============================================
connect封装

#include <sys/types.h>          //See NOTES 
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);

返回：成功0，失败-1
===============================================*/
int LINUX_itf_connect(int fd, char *ipv4, int port)
{
    //int ret;
    struct sockaddr_in addr;  
  
    // 设置服务器地址信息  
    memset(&addr, 0, sizeof(addr));  
    addr.sin_family = AF_INET;  
    addr.sin_addr.s_addr = inet_addr(ipv4);  
    addr.sin_port = htons(port);  
    
    return connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
}





