#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H
#include <iostream>
#include <stdio.h>      // printf, perror
#include <stdlib.h>     // exit, EXIT_FAILURE
#include <string.h>     // memset, strlen
#include <unistd.h>     // close
#include <arpa/inet.h>  // inet_pton, htons
#include <sys/socket.h> // socket, bind, listen, accept
#include <sys/epoll.h>  // epoll_create1, epoll_ctl, epoll_wait
#include <fcntl.h>      // fcntl, O_NONBLOCK
#include <errno.h>      // errno, EAGAIN
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>   // 包含错误码定义
#include <stdio.h>   // 包含perror函数
class EpollServer{
private:
    int port;
    int server_fd;
    int client_fd;
    int opt=1;
    struct sockaddr_in server_addr;
public:
    EpollServer(int _port);
    ~EpollServer();
};


#endif