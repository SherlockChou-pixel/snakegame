#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <cerrno>
#include <csignal>  // 处理信号（如Ctrl+C）

using namespace std;

// 全局变量：服务端套接字fd，用于信号处理函数中关闭
int server_fd = -1;

// 信号处理函数：捕获Ctrl+C，优雅关闭服务端
void signal_handler(int sig) {
    if (sig == SIGINT) {
        cout << "\n收到终止信号，正在关闭服务端..." << endl;
        if (server_fd != -1) {
            close(server_fd);  // 关闭服务端套接字
            cout << "服务端套接字已关闭" << endl;
        }
        exit(0);  // 正常退出
    }
}

// 常量定义
const int PORT = 8888;
const int BUFFER_SIZE = 1024;
const int BACKLOG = 5;

int main() {
    // 注册信号处理函数（捕获Ctrl+C）
    signal(SIGINT, signal_handler);

    // 1. 创建TCP套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        cerr << "创建套接字失败：" << strerror(errno) << endl;
        return -1;
    }
    cout << "套接字创建成功（fd：" << server_fd << "）" << endl;

    // 2. 设置端口复用（避免重启服务端时端口占用）
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        cerr << "设置套接字选项失败：" << strerror(errno) << endl;
        close(server_fd);
        return -1;
    }

    // 3. 配置服务端地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
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
    if (listen(server_fd, BACKLOG) == -1) {
        cerr << "监听失败：" << strerror(errno) << endl;
        close(server_fd);
        return -1;
    }
    cout << "=====================================" << endl;
    cout << "服务端已启动，持续监听端口 " << PORT << endl;
    cout << "按 Ctrl+C 可终止服务端" << endl;
    cout << "=====================================" << endl;

    // 6. 无限循环：持续接收客户端连接
    while (true) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        
        // 阻塞等待客户端连接（核心：每次循环接收一个新客户端）
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if (client_fd == -1) {
            cerr << "接收客户端连接失败：" << strerror(errno) << endl;
            continue;  // 跳过本次错误，继续等待下一个连接
        }

        // 打印客户端信息
        cout << "\n【新客户端连接】" << endl;
        cout << "客户端IP：" << inet_ntoa(client_addr.sin_addr) << endl;
        cout << "客户端端口：" << ntohs(client_addr.sin_port) << endl;

        // 7. 接收客户端数据
        char buffer[BUFFER_SIZE] = {0};
        ssize_t recv_len = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
        if (recv_len == -1) {
            cerr << "接收数据失败：" << strerror(errno) << endl;
            close(client_fd);  // 关闭该客户端连接
            continue;
        } else if (recv_len == 0) {
            cout << "客户端主动关闭连接（IP：" << inet_ntoa(client_addr.sin_addr) << "）" << endl;
            close(client_fd);
            continue;
        }

        // 处理接收的数据（确保字符串以\0结尾）
        buffer[recv_len] = '\0';
        cout << "收到数据：" << buffer << endl;

        // 8. 发送响应给客户端
        string response = "[Snake服务端响应] 已收到你的消息：" + string(buffer);
        ssize_t send_len = send(client_fd, response.c_str(), response.length(), 0);
        if (send_len == -1) {
            cerr << "发送响应失败：" << strerror(errno) << endl;
        } else {
            cout << "已发送响应：" << response << endl;
        }

        // 9. 关闭当前客户端连接（保持服务端继续运行）
        close(client_fd);
        cout << "【客户端连接关闭】等待下一个客户端..." << endl;
    }

    // 理论上不会执行到这里（循环是无限的），仅做兜底
    close(server_fd);
    return 0;
}