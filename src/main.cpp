#include "Snake.h"
#include "EpollServer.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "IMessageHandler.h"
#include <string>
#include <csignal> // 必须包含
#include <atomic>

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
    
    auto server = std::make_unique<EpollServer>(port);
    server->set_listen(5);
    server->setMessageHandler(std::make_unique<TestHandler>());
    std::cout << "=====================================" << std::endl;
    std::cout << "服务端已启动，持续监听端口 " << port << std::endl;
    std::cout << "(输入 q 后回车可退出)" << std::endl;
    std::cout << "=====================================" << std::endl;

    std::atomic<bool> quit_keyboard(false);
    std::thread keyboard_thread([&]() {
        while (!quit_keyboard.load()) {
            char c;
            if (!(std::cin.get(c))) {
                break;
            }
            if (c == 'q' || c == 'Q') {
                std::cout << "检测到退出命令，正在关闭服务端..." << std::endl;
                server->stop();
                quit_keyboard.store(true);
                break;
            }
        }
    });

    server->run();

    quit_keyboard.store(true);
    if (keyboard_thread.joinable()) {
        keyboard_thread.join();
    }

    return 0;
}
