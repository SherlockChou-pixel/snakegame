#ifndef _ROOM_H
#define _ROOM_H

#include <vector>
#include <iostream>
#include <ncurses.h>

// 修改处：使用前向声明，移除 #include "Snake.h"
class Snake; 
#include "Food.h" // 如果 Food 类没有循环依赖，可以保留，否则也建议前向声明
#include "../game/Map.h" 
/*用于管理玩家，蛇和食物的交互等逻辑*/
class Room
{
private:
    int size;//人数大小
    GameMap* map_;//地图大小
    Food* food_;//食物位置
    //存放玩家
    std::vector<int> players;
    int width;
    int height;
public:
    Room(int size);
    void setMap(GameMap*map);
    void player_join();
    void updateMap(const Snake& snake); 
    void setFood(Food* food);
    void eatFood(Snake& snake, Food* food);
    ~Room();
};

#endif