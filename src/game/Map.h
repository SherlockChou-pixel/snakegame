// map.h (可选，也可以直接在 GameHandler 里定义)
#ifndef _MAP_H
#define _MAP_H

#include <vector>
#include <nlohmann/json.hpp>

// 定义地图格子类型
enum CellType {
    EMPTY = 0,   // 空地
    WALL = 1,    // 墙壁/边界
    SNAKE_HEAD = 2,
    SNAKE_BODY = 3,
    FOOD = 4
};

// 地图类 (为了演示，我们只做静态地图)
class GameMap {
private:
    int width;
    int height;

public:
    GameMap(int w, int h) : width(w), height(h) {}
    // 获取宽度
    int getWidth() const {
        return width;
    }

    // 获取高度
    int getHeight() const {
        return height;
    }

    // 将地图转换为 nlohmann::json 对象
    nlohmann::json toJson() const {
        nlohmann::json j;
        j["width"] = width;
        j["height"] = height;
        return j;
    }
};

#endif