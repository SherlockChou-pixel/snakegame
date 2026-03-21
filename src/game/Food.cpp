#include "Food.h"
#include <cstdlib>
#include <ctime>
#include <unordered_set>

Food::Food(int x, int y) : x_(x), y_(y) {}

void Food::setPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

std::pair<int, int> Food::getPosition() const {
    return {x_, y_};
}

void Food::generateRandom(int width, int height, const std::vector<std::pair<int, int>>& occupied) {
    // 使用哈希表存储占用位置（key = y * width + x）
    std::unordered_set<int> occupiedSet;
    for (const auto& pos : occupied) {
        int key = pos.second * width + pos.first;
        occupiedSet.insert(key);
    }
    
    // 随机生成直到不冲突
    int totalCells = width * height;
    if (occupiedSet.size() >= totalCells) {
        // 地图全满，无法生成（可选处理）
        x_ = -1;
        y_ = -1;
        return;
    }
    
    while (true) {
        x_ = rand() % width;
        y_ = rand() % height;
        int key = y_ * width + x_;
        if (occupiedSet.find(key) == occupiedSet.end()) {
            break;
        }
    }
}