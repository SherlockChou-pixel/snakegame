#include "Snake.h"
#include "EpollServer.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "IMessageHandler.h"
#include <string>

class TestHandler : public IMessageHandler {
public:
    // 处理收到的消息
    void onMessage(int client_fd, const char* buffer, size_t length) override {
        std::cout << "[TestHandler] 收到来自 FD=" << client_fd 
                  << " 的消息，长度=" << length << std::endl;
        
        // ⚠️ 注意：buffer 可能包含非文本字符，直接 cout 可能会乱码或截断
        // 这里我们假设输入的是纯文本，为了安全打印，我们可以逐字节打印十六进制，或者转为 string
        std::string msg(buffer, length);
        std::cout << "[TestHandler] 内容预览: " << msg << std::endl;
    }

    // 处理断开连接
    void onDisconnect(int client_fd) override {
        std::cout << "[TestHandler] 客户端 FD=" << client_fd << " 已断开连接" << std::endl;
    }
};
int main(int argc, char* argv[])
{
    int port = 8888;
    EpollServer server(port);
    server.set_listen(5);
    server.setMessageHandler(std::make_unique<TestHandler>());
    std::cout << "=====================================" << std::endl;
    std::cout << "服务端已启动，持续监听端口 " << port << std::endl;
    std::cout << "=====================================" << std::endl;

    server.run();

    return 0;
}
