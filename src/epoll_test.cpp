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
using namespace std;

#define MAX_EVENTS 1024
#define PORT 8888
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>   // 包含错误码定义
#include <stdio.h>   // 包含perror函数

/**
 * @brief 设置文件描述符为非阻塞模式（带错误处理）
 * 
 * @param fd 要操作的文件描述符
 * @return int 成功返回0，失败返回-1并设置errno
 */
int set_nonbl(int fd) {
    // 获取当前文件状态标志，失败时返回-1并打印错误信息
    int flag = fcntl(fd, F_GETFL, 0);
    if (flag == -1) {
        perror("fcntl F_GETFL failed");  // 打印具体错误原因
        return -1;
    }

    // 开启非阻塞模式，失败时返回-1并打印错误信息
    if (fcntl(fd, F_SETFL, flag | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL failed");
        return -1;
    }

    return 0;
}
int main(int argc, char* argv[]){

    int server_fd,client_fd;
    struct sockaddr_in server_addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        cerr << "创建套接字失败：" << strerror(errno) <<endl;
        return -1;
    }


    cout << "套接字创建成功（fd：" << server_fd << "）" << endl;

    // 2. 设置端口复用（避免重启服务端时端口占用）
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }


    memset(&server_addr,0,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    

    // 4. 绑定端口
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cerr << "绑定端口 " << PORT << " 失败：" << strerror(errno) << endl;
        close(server_fd);
        return -1;
    }
    cout << "成功绑定端口 " << PORT << endl;

    // 5. 开始监听
    if (listen(server_fd, 5) == -1) {
        cerr << "监听失败：" << strerror(errno) << endl;
        close(server_fd);
        return -1;
    }
    //设置非阻塞
    set_nonbl(server_fd);
    cout << "=====================================" << endl;
    cout << "服务端已启动，持续监听端口 " << PORT << endl;
    cout << "=====================================" << endl;

    //创建epoll实例
    int epoll_fd=epoll_create1(EPOLL_CLOEXEC);
    if(epoll_fd<0)
    {
        perror("epoll_createl failed");
        close(server_fd);
        return -1;
    }
    //将监听socket添加到epoll中
    struct epoll_event event,events[MAX_EVENTS];;
    event.events=EPOLLIN; //监听可读事件
    event.data.fd=server_fd;
    if(epoll_ctl(epoll_fd,EPOLL_CTL_ADD,server_fd,&event)<0)
    {
        perror("epoll_cntl (ADD server_fd) failed");
        close(epoll_fd);
        close(server_fd);
        return -1;
    }
    while (1) {
        // 等待事件
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;

            // 新连接
            if (fd == server_fd) {
                struct sockaddr_in caddr;
                socklen_t len = sizeof(caddr);
                int cfd = accept(server_fd, (struct sockaddr*)&caddr, &len);
                set_nonbl(cfd);

                // 添加到 epoll
                event.events = EPOLLIN;
                event.data.fd = cfd;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cfd, &event);
                printf("new client: %d\n", cfd);
            } 
            // 客户端数据
            else if (events[i].events & EPOLLIN) {
                char buf[1024];
                int len = read(fd, buf, sizeof(buf)-1);

                if (len <= 0) {
                    close(fd);
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                    printf("client close: %d\n", fd);
                } else {
                    buf[len] = 0;
                    printf("recv %d: %s\n", fd, buf);
                    write(fd, buf, len);
                }
            }
        }
    }
    close(server_fd);
    return 0;
}