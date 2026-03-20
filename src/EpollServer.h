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

class EpollServer {
private:
    int port;
    int server_fd;
    int epoll_fd;
    int opt = 1;
    struct sockaddr_in server_addr;
    static constexpr int MAX_EVENTS = 1024;
    bool running;

    int set_nonbl(int fd);
    void handle_client_event(int client_fd);
    void close_client(int client_fd);

public:
    explicit EpollServer(int _port);
    void set_listen(int backlog = 5);
    void run();
    void stop();
    ~EpollServer();
};

#endif