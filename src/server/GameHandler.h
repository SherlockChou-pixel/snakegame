#ifndef _GAME_HANDLER_H
#define _GAME_HANDLER_H

#include "IMessageHandler.h"
#include "EpollServer.h"
#include "../game/Map.h"      // 修复地图头文件路径
#include "../protocol/Protocol.h"  // 修复协议头文件路径
#include "../game/Snake.h"
#include "RoomManager.h"
#include <nlohmann/json.hpp>
#include <atomic>
#include <iostream>

using json = nlohmann::json;

class GameHandler : public IMessageHandler {
private:

    RoomManager* roomManagerRef;
    std::atomic<int> msg_count{0};

public:
    // 构造函数参数变更
    explicit GameHandler(RoomManager& rm) : roomManagerRef(&rm) {}

    void onConnect(int client_fd) override {
        std::cout << "[GAME] 新玩家连接 fd=" << client_fd << std::endl;
    }
    //处理收到的消息
    void onMessage(int client_fd, const char* buffer, size_t length) override {
        
        std::cout << "[GAME] 收到消息 fd=" << buffer << std::endl;
        ProtocolData data=Protocol::parse(buffer);
        int cmd_id=data.cmd_id;
        std::cout << "[GAME] 命令ID=" << cmd_id << std::endl;
        switch(cmd_id)
        {
            case 1:roomManagerRef->creatRoom(client_fd);
            break;
            case 2:roomManagerRef->startGame(data.data.value("room_id", ""));break;
            default:break;
        }
    }

    void onDisconnect(int client_fd) override {
        std::cout << "[GAME] 玩家断开 fd=" << client_fd << std::endl;
    }
    ~GameHandler(){
        std::cout<<"以gamehandler释放资源"<<std::endl;
    }
private:
    
};

#endif