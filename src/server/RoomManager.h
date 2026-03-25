#ifndef _ROOMMANAGER_H_
#define _ROOMMANAGER_H_ 
#include <queue>
#include "../game/Snake.h"
#include "../game/Room.h"
#include "NetworkSenderAdapter.h"
struct Player{
    int id;  //玩家的id号，用于识别
    Snake* snake; //玩家的蛇
    int score;//玩家的分数
};

class RoomManager{
private:
    std::queue<Player> wait_queue;//玩家等待队列
    INetworkSender& networkSenderRef;                 // 依赖注入的引用

public:
    void join_room();
    explicit RoomManager(INetworkSender& sender);
    ~RoomManager();
};
#endif