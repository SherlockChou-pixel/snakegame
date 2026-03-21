#ifndef _ROOM_H
#define _ROOM_H
#include <vector>
#include <iostream>
#include <ncurses.h>//非阻塞输出
#include "Snake.h"
#include "Food.h"
class Room
{
private:
    
    int size;//房间容纳人数
    int width;//地图宽度
    int height;//地图长度
    /*初始化一个地图*/
    std::vector<std::string> board;
    Food* food_;  // 指向食物的指针
public:
    Room(int size,int width, int height);
    /*更新地图位置*/
    void updateMap(const Snake& snake);
    /*添加食物*/
    void setFood(Food* food);
    void displayMap() const;
    ~Room();
};


#endif
