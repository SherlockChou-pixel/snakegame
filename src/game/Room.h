#ifndef _ROOM_H
#define _ROOM_H

#include <vector>
#include <iostream>
#include <ncurses.h>

// 修改处：使用前向声明，移除 #include "Snake.h"
class Snake; 
#include "Food.h" // 如果 Food 类没有循环依赖，可以保留，否则也建议前向声明
/*用于管理玩家，蛇和食物的交互等逻辑*/
class Room
{
private:
    int size;
    int width;
    int height;
    std::vector<std::string> board;
    Food* food_;
    //存放玩家
    std::vector<int> players;
public:
    Room(int size, int width, int height);
    std::pair<int,int> getRoomSize() const;
    // 现在 Snake 已声明，可以正常使用引用
    void updateMap(const Snake& snake); 
    void setFood(Food* food);
    void eatFood(Snake& snake, Food* food);
    void displayMap() const;
    ~Room();
};

#endif