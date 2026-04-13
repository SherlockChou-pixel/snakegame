#include "GameHandler.h"
#include "RoomManager.h"   // 这里才能包含
#include "../protocol/Protocol.h"
#include <iostream>

using json = nlohmann::json;

GameHandler::GameHandler(RoomManager& rm) 
    : roomManagerRef(&rm) 
{
}

void GameHandler::onConnect(int client_fd) {
    std::cout << "[GAME] 新玩家连接 fd=" << client_fd << std::endl;
}

void GameHandler::onMessage(int client_fd, const char* buffer, size_t length) {
    std::cout << "[GAME] 收到消息：" << buffer << std::endl;
    ProtocolData data = Protocol::parse(buffer);
    int cmd_id = data.cmd_id;

    switch (cmd_id) {
        case 1: roomManagerRef->joinRoom(client_fd); break;
        case 2: roomManagerRef->startGame(data.data.value("room_id", "")); break;
        case 3: roomManagerRef->changePlayerDirection(
            data.data.value("room_id", ""),
            data.data.value("player_id", 0),
            static_cast<Direction>(data.data.value("direction", 0))
        ); break;
        default: break;
    }
}

void GameHandler::onDisconnect(int client_fd) {
    std::cout << "[GAME] 玩家断开 fd=" << client_fd << std::endl;
    roomManagerRef->handlePlayerDisconnect(client_fd);
}