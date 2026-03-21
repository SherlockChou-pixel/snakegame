#include "game/Snake.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>
#include "game/Room.h"
#include "game/Food.h"
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[])
{
    srand(time(NULL));  // 初始化随机种子
    
    int width = 10;
    int height = 10;
    auto room = std::make_unique<Room>(1, width, height);
    
    // 创建蛇
    Snake snake(1, 5, 5, 3, Direction::right, width, height);
    
    // 创建食物并随机生成位置（避免与蛇重叠）
    Food food;
    food.generateRandom(width, height, snake.getBody());
    
    // 设置食物到房间
    room->setFood(&food);
    
    // 更新地图
    room->updateMap(snake);
    // 这里需要扩展Room来显示食物
    
    // 显示地图
    room->displayMap();
    
    return 0;
}
