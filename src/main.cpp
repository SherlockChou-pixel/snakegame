#include "Snake.h"
#include "EpollServer.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>

int main(int argc, char* argv[])
{
    int port = 8888;
    EpollServer server(port);
    server.set_listen(5);
    std::cout << "=====================================" << std::endl;
    std::cout << "服务端已启动，持续监听端口 " << port << std::endl;
    std::cout << "=====================================" << std::endl;

    server.run();

    return 0;
}
