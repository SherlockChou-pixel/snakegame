#include "EpollServer.h"

EpollServer::EpollServer(int _port):port(_port){
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        std::cerr << "创建套接字失败：" << strerror(errno) <<std::endl;
    }
    std::cout << "套接字创建成功（fd：" << server_fd << "）" << std::endl;
}

EpollServer::~EpollServer(){

}