#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <random>
#include "Player.h" 
#include"Snake.h"
Food::Food(int x, int y) : x_(x), y_(y) {}

void Food::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

std::pair<int, int> Food::getPosition() const {
    return {x_, y_};
}

void Food::generateRandom(int width, int height, std::vector<Player>& players) {
    // 静态引擎，避免每次调用都重新种子化
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    // 使用哈希表存储占用位置
    std::unordered_set<int> occupiedSet;
    for (const auto& player : players) {
        
        for(const auto& pos : player.snake->getBody())
        {
            int key = pos.second * width + pos.first;
            occupiedSet.insert(key);
        }
    }
    
    int totalCells = width * height;
    if (occupiedSet.size() >= totalCells) {
        x_ = -1;
        y_ = -1;
        return;
    }
    
    // 定义分布对象
    std::uniform_int_distribution<> distX(0, width - 1);
    std::uniform_int_distribution<> distY(0, height - 1);

    while (true) {
        // 正确调用方式：分布对象(引擎)
        x_ = distX(gen);
        y_ = distY(gen);
        int key = y_ * width + x_;
        if (occupiedSet.find(key) == occupiedSet.end()) {
            break;
        }
    }
}