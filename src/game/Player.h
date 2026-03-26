#ifndef _PLAYER_H
#define _PLAYER_H
#include <memory>
class Snake;
struct Player{
    int id;  //玩家的id号，用于识别
    std::unique_ptr<Snake> snake;//玩家的蛇
    int score;//玩家的分数
};


enum class Direction{
    up=0,
    down=1,
    left=2,
    right=3
};
#endif