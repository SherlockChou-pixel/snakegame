#ifndef _ROOMMANAGER_H_
#define _ROOMMANAGER_H_ 
#include <string>
#include <queue>
#include "../protocol/Protocol.h"
#include <unordered_map>
#include "NetworkSenderAdapter.h"
class Room; // 前向声明
class Snake; // 前向声明

class RoomManager{
private:
    int roomid=0;
    // std::queue<Player> wait_queue;//玩家等待队列
    INetworkSender& networkSenderRef;                 // 依赖注入的引用
    std::unordered_map<std::string , std::unique_ptr<Room>> activeRooms; 

public:
    void creatRoom(int client_fd);
    void startGame(const std::string&);
    explicit RoomManager(INetworkSender& sender);
    ~RoomManager();
};
#endif