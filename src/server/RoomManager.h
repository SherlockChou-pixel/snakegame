#ifndef _ROOMMANAGER_H_
#define _ROOMMANAGER_H_ 
#include <string>
#include <queue>
#include "../protocol/Protocol.h"
#include <unordered_map>
#include "NetworkSenderAdapter.h"
#include <thread>
#include <atomic>
#include "../game/Player.h"
#include <nlohmann/json.hpp>
class Room; // 前向声明
class Snake; // 前向声明

class RoomManager{
private:
    int roomid=0;
    // std::queue<Player> wait_queue;//玩家等待队列
    INetworkSender& networkSenderRef;                 // 依赖注入的引用
    std::unordered_map<std::string , std::unique_ptr<Room>> activeRooms; 
    //游戏循环
    std::thread gameLoopTread;
    std::atomic<bool> gameLoopRunning;
public:

    void joinRoom(int client_fd);
    void creatRoom(int client_fd);
    void startGame(const std::string&);
    void runGameLoop();
    void startGameLoop();
    void updateAllRooms();
    explicit RoomManager(INetworkSender& sender);

    void changePlayerDirection(const std::string& room_id, int player_id, Direction dir);
    
    /*玩家离开*/
    void handlePlayerDisconnect(int client_fd);
    ~RoomManager();
};
#endif