#include "Snake.h"

/* 构造函数 */
Snake::Snake(int _id, int startX, int startY, int _len, Direction _dir, int boardW, int boardH)
    : id(_id), len(std::max(1, _len)), boardW_(boardW), boardH_(boardH), dir_(_dir)
{
    // 初始化蛇身：预留空间防止后续扩容而造成额外开销
    body_.reserve(100);

    // 方向数组，用于初始化身体和移动
    static const std::vector<std::pair<int, int>> direction = {
        {0, -1},  // UP：y减1（假设屏幕y轴向下）
        {0, 1},   // DOWN：y加1
        {-1, 0},  // LEFT：x减1
        {1, 0}    // RIGHT：x加1
    };

    auto [dx, dy] = direction[static_cast<int>(dir_)];

    // 头部位置
    body_.push_back({startX, startY});
    // 蛇身其余部分从头往反方向延伸
    for (int i = 1; i < len; ++i) {
        body_.push_back( {startX - dx * i, startY - dy * i});
    }

    std::cout << _id << " 蛇已就绪" << std::endl;
}

static bool isOpposite(Direction a, Direction b)
{
    return (a == Direction::up && b == Direction::down) ||
           (a == Direction::down && b == Direction::up) ||
           (a == Direction::left && b == Direction::right) ||
           (a == Direction::right && b == Direction::left);
}

void Snake::setDirection(Direction dir)
{
    if (!isOpposite(dir_, dir)) {
        dir_ = dir;
    }
    // 直接掉头时忽略输入，保持原方向
}

void Snake::setBoardSize(int w, int h)
{
    boardW_ = w;
    boardH_ = h;
}

void Snake::move()
{
    // 方向数组
    static const std::vector<std::pair<int, int>> direction = {
        {0, -1},  // UP：y减1（假设屏幕y轴向下）
        {0, 1},   // DOWN：y加1
        {-1, 0},  // LEFT：x减1
        {1, 0}    // RIGHT：x加1
    };

    if (body_.empty())
        return;
    // 头部坐标
    auto [head_x, head_y] = body_.front();
    auto [dx, dy] = direction[static_cast<int>(dir_)];

    // 防止蛇直接掉头（理论上 setDirection 已阻断，作保险）
    if (body_.size() > 1) {
        auto [next_x, next_y] = body_[1];
        if (head_x + dx == next_x && head_y + dy == next_y) {
            // 直接掉头时仍按原方向前进（不改变 head_x/head_y）
        } else {
            head_x += dx;
            head_y += dy;
        }
    } else {
        head_x += dx;
        head_y += dy;
    }

    // 边界环绕
    if (boardW_ > 0) {
        head_x %= boardW_;
        if (head_x < 0)
            head_x += boardW_;
    }
    if (boardH_ > 0) {
        head_y %= boardH_;
        if (head_y < 0)
            head_y += boardH_;
    }

    // 更新身体：尾部依次跟随前一节
    for (int i = len - 1; i > 0; --i) {
        body_[i] = body_[i - 1];
    }
    body_[0] = {head_x, head_y};
}

std::pair<int, int> Snake::getHeadPos() const
{
    return body_.front();
}

const std::vector<std::pair<int, int>>& Snake::getBody() const
{
    return body_;
}

Snake::~Snake()
{
    std::cout << id << " 蛇完了" << std::endl;
}
