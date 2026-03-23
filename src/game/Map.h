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
    std::vector<std::vector<int>> grid; // 用 int 存储 CellType，方便 JSON 序列化
    int width;
    int height;

public:
    GameMap(int w, int h) : width(w), height(h) {
        // 初始化为全空
        grid.resize(height, std::vector<int>(width, EMPTY));
        
        // 绘制边界墙壁
        for (int i = 0; i < width; i++) {
            grid[0][i] = WALL;           // 上边
            grid[height-1][i] = WALL;    // 下边
        }
        for (int i = 0; i < height; i++) {
            grid[i][0] = WALL;           // 左边
            grid[i][width-1] = WALL;     // 右边
        }
        
        // 在中间放一个食物 (示例)
        grid[height/2][width/2] = FOOD;
    }

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
        j["grid"] = grid; // nlohmann/json 能自动处理 vector<vector<int>>
        return j;
    }
};

#endif