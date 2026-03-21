#include "Room.h"
#include "Snake.h"



Room::Room(int size,int width, int height):size(size),width(width),height(height), food_(nullptr){

    board=std::vector<std::string>(height, std::string(width, '.'));
}

void Room::setFood(Food* food) {
    food_ = food;
}
void Room::eatFood(Snake& snake,Food*food)
{
    if(food->getPosition()==snake.getHeadPos())
    {
        snake.grow();
        food->generateRandom(width, height, snake.getBody());
    
        // 设置食物到房间
        setFood(food);
    
        // 更新地图
        updateMap(snake);
    }
}
//获取尺寸
std::pair<int,int> Room::getRoomSize() const {
    return std::make_pair(width, height);
}
void Room::updateMap(const Snake& snake) {
    // 重置地图
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), '.');
    }
    // 放置蛇的身体
    for (const auto& pos : snake.getBody()) {
        int x = pos.first;
        int y = pos.second;
        if (x >= 0 && x < width && y >= 0 && y < height) {
            board[y][x] = '#';  // 假设 y 是行，x 是列
        }
    }
    // 头部用 'H' 表示
    auto head = snake.getHeadPos();
    if (head.first >= 0 && head.first < width && head.second >= 0 && head.second < height) {
        board[head.second][head.first] = 'H';
    }
    // 放置食物
    if (food_) {
        auto foodPos = food_->getPosition();
        int fx = foodPos.first;
        int fy = foodPos.second;
        if (fx >= 0 && fx < width && fy >= 0 && fy < height) {
            board[fy][fx] = 'F';
        }
    }
}

void Room::displayMap() const {
    for (const auto& row : board) {
        std::cout << row << std::endl;
    }
    std::cout << std::endl;
}

Room::~Room() {
    // 析构函数
}