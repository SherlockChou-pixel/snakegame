#ifndef _SNAKE_H
#define _SNAKE_H
#include <utility>
#include <vector>
#include <iostream>
#include <unordered_set>
enum class Direction{
    up=0,
    down=1,
    left=2,
    right=3
};

class Snake{
private:
    int id; // 每条蛇的id号，对应玩家
    int len; // 蛇的长度
    int boardW_{0}; // 0 表示不使用边界约束
    int boardH_{0};

    // 蛇身坐标：第一个元素是蛇头，后续是身体（pair<x, y>）
    std::vector<std::pair<int, int>> body_;
    
    // 当前移动方向
    Direction dir_;
public:
    // 构造：指定初始头部位置、长度、方向
    // boardW/boardH 用于边界环绕（若为 0 则不环绕）
    Snake(int _id, int startX, int startY, int _len, Direction _dir, int boardW = 0, int boardH = 0);
    
    void setDirection(Direction dir);
    void setBoardSize(int w, int h);
    void grow();
    // 移动
    void move();
    //获取头部位置
    std::pair<int,int> getHeadPos() const;
    //获取整个身子
    const std::vector<std::pair<int,int>>& getBody() const;
    ~Snake();
};





#endif