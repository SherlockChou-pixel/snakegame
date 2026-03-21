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
    Snake snake(1, 5, 5, 3, Direction::right);
    
    // 创建食物并随机生成位置（避免与蛇重叠）
    Food food;
    food.generateRandom(width, height, snake.getBody());
    
    // 设置食物到房间
    room->setFood(&food);
    // 这里需要扩展Room来显示食物
    food.setPosition(6,5);
    // 更新地图
    room->updateMap(snake);

    // 显示地图
    room->displayMap();

    
    // 测试吃食逻辑：模拟移动和吃食
    std::cout << "初始蛇长度: " << snake.getBody().size() << std::endl;
    
    for (int step = 0; step < 5; ++step) {
        std::cout << "\n--- 步骤 " << step + 1 << " ---" << std::endl;
        
        // 设置方向（例如向右）
        snake.setDirection(Direction::right);
        
        // 先移动蛇
        snake.move(width,height);
        
        // 然后检查吃食
        room->eatFood(snake, &food);
        
        // 更新地图
        room->updateMap(snake);
        
        // 显示地图
        room->displayMap();
        
        // 显示蛇长度
        std::cout << "蛇长度: " << snake.getBody().size() << std::endl;
    }
    
    return 0;
}
