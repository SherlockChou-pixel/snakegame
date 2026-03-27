#ifndef _SNAKE_H
#define _SNAKE_H
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include "Player.h"

class Snake{
private:
    int len; // 蛇的长度
    const int init_len=3;
    // 蛇身坐标：第一个元素是蛇头，后续是身体（pair<x, y>）
    std::vector<std::pair<int, int>> body_;
    // 当前移动方向
    Direction dir_;
    Direction pending_dir_;
    std::pair<int,int> board_size;

public:
    // 构造：指定初始头部位置、长度、方向
    Snake(int startX, int startY, int _len, Direction _dir,std::pair<int,int> _board_size);
    void setDirection(Direction dir);
    //生长
    void grow();
    // 移动
    void move();
    //获取头部位置
    std::pair<int,int> getHeadPos() const;
    //获取整个身子
    const std::vector<std::pair<int,int>>& getBody() const;
    //获取方向
    Direction getdir() const;
    //碰撞检测
    bool checkSelfCollision() const;
    //发送身体
    nlohmann::json toJson() const;
    //重置身体
    void resetToInitialPos(int startX,int startY,Direction dir);
    ~Snake();
};





#endif