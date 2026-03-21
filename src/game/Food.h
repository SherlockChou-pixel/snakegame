#ifndef _FOOD_H
#define _FOOD_H

#include <utility>
#include <vector>

class Food {
private:
    int x_;
    int y_;
public:
    Food(int x = 0, int y = 0);
    void setPosition(int x, int y);
    std::pair<int, int> getPosition() const;
    // 随机生成位置（传入地图大小和占用位置）
    void generateRandom(int width, int height, const std::vector<std::pair<int, int>>& occupied);
};

#endif