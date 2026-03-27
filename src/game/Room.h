#ifndef _ROOM_H
#define _ROOM_H

#include <vector>
#include <iostream>
#include "../server/RoomManager.h"
#include "Player.h" 
#include <memory>
class Snake; 
#include "Food.h" // 如果 Food 类没有循环依赖，可以保留，否则也建议前向声明
#include "../game/Map.h" 
/*用于管理玩家，蛇和食物的交互等逻辑*/
class Room
{
private:
    std::string id;//房间号
    GameMap* map_;//地图大小
    std::unique_ptr<Food> food;//食物位置
    //存放玩家
    std::vector<Player> players;
    int width;
    int height;
    const int max_size=4;//最大人数
    bool isRunningGame=false;

    const int RESPAWN_TIME_SEC = 3; // 重生时间为3秒
    const int FRAMES_PER_SECOND = 60; // 假设你的游戏循环频率是10FPS
public:
    Room(const std::string& id);
    void setMap(GameMap*map);
    bool add_player(Player&& player);
    std::vector<std::pair<int,std::string>> startGame();
    void initialGameWorld();
    bool isRunning() const{ return isRunningGame;}
    void setRunning(bool status){isRunningGame=status;}
    std::string updateGameState();

    const std::vector<Player>& getPlayers() const { return players; }
    /*处理玩家输入*/
    void handlePlayerInput(int player_id, Direction new_dir);
    /*处理玩家死亡*/
    void handlePlayerDeath(Player& player);
    std::pair<int,int> findSafeSpawnPosition();
    /*判断房间有没有空*/
    bool isRoomEmpty() const{return players.empty();}
    void removePlayer(int playerId);
    ~Room();
};

#endif