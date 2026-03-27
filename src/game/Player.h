#ifndef _PLAYER_H
#define _PLAYER_H
#include <memory>
class Snake;

enum class Direction{
    up=0,
    down=1,
    left=2,
    right=3
};

enum class PlayerState{
    alive=0,
    dead=1
};
struct Player{
    int id;  //玩家的id号，用于识别
    std::unique_ptr<Snake> snake;//玩家的蛇
    int score;//玩家的分数
    PlayerState state;
    int respawnTimer=0;
};



#endif