#ifndef EPOLL_SERVER_H
#define EPOLL_SERVER_H

#include <iostream>
#include <vector>
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
#include <memory>
#include <unordered_map>  // 添加map容器支持
#include <string>         // 添加string支持

class IMessageHandler;

// 定义客户端上下文结构
struct ClientContext {
    int fd;
    std::string buffer;  // 用于处理粘包
};

/*网络层 */
class EpollServer {
private:
    int port;
    int server_fd;
    int epoll_fd;
    int opt = 1;
    struct sockaddr_in server_addr;
    static constexpr int MAX_EVENTS = 1024;
    bool running;
    std::unique_ptr<IMessageHandler> messageHandler; 
    int set_nonbl(int fd);
    void handle_client_event(int client_fd);
    void close_client(int client_fd);
    
    // 添加客户端映射
    std::unordered_map<int, std::shared_ptr<ClientContext>> clients;

public:
    explicit EpollServer(int _port);
    void set_listen(int backlog = 5);
    void run();
    void stop();
    //对外暴露接口
    void setMessageHandler(std::unique_ptr<IMessageHandler> handler);
    void sendToClient(int client_fd, const std::string& msg);  
    
    ~EpollServer();
};

#endif