#include "game/Snake.h"
#include "game/Room.h"
#include "game/Food.h"
#include <ncurses.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <memory>

// 定义游戏速度 (毫秒)
const int GAME_SPEED_MS = 150; 

int main(int argc, char* argv[])
{
    // 1. 初始化随机种子
    srand(time(NULL));

    // 2. 初始化 ncurses
    initscr();              // 启动 ncurses 模式
    // start_color();
    // init_pair(1, COLOR_GREEN, COLOR_BLACK); // 蛇的颜色
    // init_pair(2, COLOR_RED, COLOR_BLACK);   // 食物的颜色
    cbreak();               // 禁用行缓冲，立即接收字符
    noecho();               // 不显示输入的字符
    keypad(stdscr, TRUE);   // 启用特殊键（如方向键）
    nodelay(stdscr, TRUE);  // 非阻塞模式：getch() 不会等待用户输入
    curs_set(0);            // 隐藏光标

    int width = 20;         // 稍微调大一点地图以便游玩
    int height = 10;
    
    // 创建房间
    auto room = std::make_unique<Room>(1, width, height);
    
    // 创建蛇 (初始位置在中间)
    int start_x = width / 2;
    int start_y = height / 2;
    auto snake=std::make_unique<Snake>(1, start_x, start_y, 3, Direction::right, std::make_pair(width, height));
    
    // 创建食物
    Food food;
    food.generateRandom(width, height, snake->getBody());
    room->setFood(&food);

    // 初始渲染
    room->updateMap(*snake);
    room->displayMap();
    
    // 在屏幕某处显示提示信息
    // 显示初始状态
    mvprintw(height + 1, 0, "%-50s", " ");
    mvprintw(height + 1, 0, "Keys: Arrows | 'q': Quit | Len: %lu", snake->getBody().size());
    refresh();
    bool gameRunning = true;
    Direction currentDir = Direction::right;
    bool gameOverBySelf = false; // 标记是否是撞自己死的
    // 3. 游戏主循环
    while (gameRunning) {
        // --- 处理输入 ---
        int ch = getch(); // 获取按键
        
        if (ch != ERR) { // 如果有按键输入
            switch (ch) {
                case KEY_UP:
                    if (currentDir != Direction::down) currentDir = Direction::up;
                    break;
                case KEY_DOWN:
                    if (currentDir != Direction::up) currentDir = Direction::down;
                    break;
                case KEY_LEFT:
                    if (currentDir != Direction::right) currentDir = Direction::left;
                    break;
                case KEY_RIGHT:
                    if (currentDir != Direction::left) currentDir = Direction::right;
                    break;
                case 'q':
                case 'Q':
                    gameRunning = false;
                    break;
                default:
                    break;
            }
        }

        if (!gameRunning) break;

        // --- 更新逻辑 ---
        snake->setDirection(currentDir);
        snake->move();

        if (snake->checkSelfCollision()) {
            gameRunning = false;
            gameOverBySelf = true; // 标记原因
            break;
        }

        // 吃食物逻辑
        // 注意：eatFood 可能会修改 snake 的长度和 food 的位置
        room->eatFood(*snake, &food);
        

        // --- 渲染 ---
        room->updateMap(*snake);
        room->displayMap();
        
        // 更新状态栏
        mvprintw(height + 1, 0, "%-50s", " ");
        mvprintw(height + 1, 0, "Keys: Arrows | 'q': Quit | Len: %lu", snake->getBody().size());
        refresh();

        // --- 控制帧率 ---
        std::this_thread::sleep_for(std::chrono::milliseconds(GAME_SPEED_MS));
    }

    // 4. 清理 ncurses
    endwin(); 
    
    std::cout << "\n=== GAME OVER ===\n";
    if (gameOverBySelf) {
        std::cout << "原因：你撞到了自己的身体！\n";
    } else {
        std::cout << "原因：你撞到了墙壁！\n";
    }
    std::cout << "最终长度：" << snake->getBody().size() << std::endl;
    return 0;
}