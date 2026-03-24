#include "Room.h"
#include "Snake.h"



Room::Room(int size):size(size), food_(nullptr){

}
void Room::setMap(GameMap*map){
    map_=map;
    width=map_->getWidth();
    height=map_->getHeight();
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
void Room::updateMap(const Snake& snake) {

}


Room::~Room() {
    // 析构函数
    std::cout<<"房间炸了"<<std::endl;
}