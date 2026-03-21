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

// 【核心修改】使用 ncurses 渲染
void Room::displayMap() const {
    // 清屏 (清除上一帧的内容)
    clear(); 

    // 遍历逻辑地图板，将其绘制到 ncurses 窗口
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char ch = board[y][x];
            
            // 可选：给不同元素添加颜色 (需要先初始化颜色对)
            // if (ch == 'H') attron(COLOR_PAIR(1));
            // if (ch == 'F') attron(COLOR_PAIR(2));
            
            // mvaddch(y, x, ch) 将字符 ch 移动到坐标 (y, x) 并绘制
            mvaddch(y, x, ch);
            
            // 如果用了颜色，记得恢复
            // attroff(COLOR_PAIR(1)); 
        }
    }

    // 绘制边框 (可选，让游戏区域更明显)
    // box(stdscr, 0, 0); 

    // 【关键】刷新屏幕，将缓冲区内容显示到终端
    refresh();
}

Room::~Room() {
    // 析构函数
}