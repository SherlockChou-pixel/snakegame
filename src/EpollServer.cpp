#include "EpollServer.h"

#include <unistd.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include "IMessageHandler.h"
EpollServer::EpollServer(int _port): port(_port), server_fd(-1), epoll_fd(-1), running(false) 
,messageHandler(nullptr){
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "创建套接字失败：" << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "套接字创建成功（fd：" << server_fd << "）" << std::endl;

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "绑定端口 " << port << " 失败：" << strerror(errno) << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "成功绑定端口 " << port << std::endl;
}

int EpollServer::set_nonbl(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) {
        perror("fcntl F_GETFL failed");
        return -1;
    }

    if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL failed");
        return -1;
    }

    return 0;
}

void EpollServer::set_listen(int backlog) {
    if (listen(server_fd, backlog) == -1) {
        std::cerr << "监听失败：" << strerror(errno) << std::endl;
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    std::cout << "监听成功，backlog=" << backlog << std::endl;
}

void EpollServer::handle_client_event(int client_fd) {
    char buffer[1024];
    while (true) {
        ssize_t count = recv(client_fd, buffer, sizeof(buffer), 0);
        if (count == -1) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                break;
            }
            perror("recv failed");
            close_client(client_fd);
            break;
        } else if (count == 0) {
            close_client(client_fd);
            break;
        } else {
            if (messageHandler != nullptr) {
                messageHandler->onMessage(client_fd, buffer, count);
            }
            ssize_t sent = 0;
            while (sent < count) {
                std::string reply_msg = "射出来了:" + std::string(buffer, count) + "\r\n";
                ssize_t n =send(client_fd, reply_msg.c_str(), reply_msg.size(), 0);

                if (n == -1) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    }
                    perror("send failed");
                    close_client(client_fd);
                    return;
                }
                sent += n;
            }
        }
    }
}
void EpollServer::setMessageHandler(std::unique_ptr<IMessageHandler> handler) {
    messageHandler = std::move(handler); // 👈 使用 std::move 转移所有权
}
void EpollServer::close_client(int client_fd) {
    std::cout << "客户端断开 fd=" << client_fd << std::endl;
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client_fd, nullptr);
    close(client_fd);
}

void EpollServer::run() {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1 failed");
        exit(EXIT_FAILURE);
    }

    if (set_nonbl(server_fd) == -1) {
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    struct epoll_event event; 
    event.events = EPOLLIN;
    event.data.fd = server_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event) == -1) {
        perror("epoll_ctl add server_fd failed");
        close(server_fd);
        close(epoll_fd);
        exit(EXIT_FAILURE);
    }

    std::vector<epoll_event> events(MAX_EVENTS);
    running = true;
    std::cout << "开始 epoll 循环" << std::endl;

    while (running) {
        int n = epoll_wait(epoll_fd, events.data(), MAX_EVENTS, -1);
        if (n == -1) {
            if (errno == EINTR) {
                continue;
            }
            perror("epoll_wait failed");
            break;
        }

        for (int i = 0; i < n; ++i) {
            if (events[i].data.fd == server_fd) {
                while (true) {
                    struct sockaddr_in client_addr;
                    socklen_t client_len = sizeof(client_addr);
                    int client = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
                    if (client == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                            break;
                        }
                        perror("accept failed");
                        break;
                    }

                    if (set_nonbl(client) == -1) {
                        close(client);
                        continue;
                    }

                    struct epoll_event client_event;
                    client_event.events = EPOLLIN | EPOLLET;
                    client_event.data.fd = client;
                    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client, &client_event) == -1) {
                        perror("epoll_ctl add client failed");
                        close(client);
                        continue;
                    }

                    std::cout << "新客户端连接 fd=" << client << std::endl;
                }
            } else {
                handle_client_event(events[i].data.fd);
            }
        }
    }
}

void EpollServer::stop() {
    if (running) {
        running = false;
    }
}

EpollServer::~EpollServer() {
    std::cout << "EpollServer 退出，关闭资源" << std::endl;
    if (epoll_fd != -1) {
        close(epoll_fd);
    }
    if (server_fd != -1) {
        close(server_fd);
    }
}
