#include "Snake.h"
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <ncurses.h>

static void printBoard(const Snake& snake, int width, int height)
{
    clear();

    std::vector<std::string> board(height, std::string(width, '.'));
    for (auto [x, y] : snake.getBody()) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            board[y][x] = 'o';
        }
    }
    auto [hx, hy] = snake.getHeadPos();
    if (hx >= 0 && hx < width && hy >= 0 && hy < height) {
        board[hy][hx] = 'H';
    }

    for (int y = 0; y < height; ++y) {
        mvprintw(y, 0, "%s", board[y].c_str());
    }
    refresh();
}

int main(int argc, char* argv[])
{
    constexpr int kWidth = 10;
    constexpr int kHeight = 10;

    initscr();            // 初始化屏幕
    cbreak();             // 禁用行缓冲
    noecho();             // 不回显输入
    keypad(stdscr, TRUE); // 启用特殊键（如箭头）
    nodelay(stdscr, TRUE); // 非阻塞输入
    curs_set(0);          // 隐藏光标

    // 在 10x10 地盘上创建一条长度为 3 的蛇，初始朝右移动
    Snake snake(1, 2, 2, 3, Direction::right, kWidth, kHeight);

    while (true)
    {
        int ch = getch(); // 非阻塞获取字符
        if (ch != ERR) {  // ERR 表示无输入
            if (ch == 'q' || ch == 'Q') {
                break;
            }
            switch (ch) {
                case 'w': case KEY_UP:    snake.setDirection(Direction::up); break;
                case 's': case KEY_DOWN:  snake.setDirection(Direction::down); break;
                case 'a': case KEY_LEFT:  snake.setDirection(Direction::left); break;
                case 'd': case KEY_RIGHT: snake.setDirection(Direction::right); break;
                default: break;
            }
        }

        snake.move();
        printBoard(snake, kWidth, kHeight);
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }

    endwin(); // 结束 ncurses
    return 0;
}
