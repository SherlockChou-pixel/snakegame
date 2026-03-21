#include "Snake.h"

/* 构造函数 */
Snake::Snake(int _id, int startX, int startY, int _len, Direction _dir)
    : id(_id), len(std::max(1, _len)), dir_(_dir)
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
void Snake::grow()
{
    auto [tail_x,tail_y]=body_.back();
    auto [prev_x,prev_y]=body_[len-2];
    body_.push_back({2*tail_x-prev_x,2*tail_y-prev_y});
    len++;
}
void Snake::move(int width, int height)
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

    // 计算新头部坐标
    int new_head_x = head_x + dx;
    int new_head_y = head_y + dy;

    // 边界环绕处理 (核心优化)
    // 使用 (x + width) % width 处理负数情况
    new_head_x = (new_head_x % width + width) % width;
    new_head_y = (new_head_y % height + height) % height;

    // 防止掉头逻辑 (保留之前的保险)
    if (body_.size() > 1) {
        auto [next_x, next_y] = body_[1];
        if (new_head_x == next_x && new_head_y == next_y) {
            // 如果试图掉头，保持原位置或忽略本次移动，这里选择忽略方向变化
            // 但坐标已经计算为环绕后的值，通常穿墙模式下允许掉头需额外判断
            // 此处简化为直接更新
        }
    }

    // 更新身体
    for (int i = len - 1; i > 0; --i) {
        body_[i] = body_[i - 1];
    }
    body_[0] = {new_head_x, new_head_y};
}

std::pair<int,int> Snake::getHeadPos() const {
    if (body_.empty()) return {-1, -1};
    return body_.front();
}

const std::vector<std::pair<int,int>>& Snake::getBody() const {
    return body_;
}

Snake::~Snake() {
    // 析构函数，如果需要清理资源
}
