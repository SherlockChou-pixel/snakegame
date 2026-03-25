#include "Room.h"
#include "Snake.h"
#include <nlohmann/json.hpp>
#include "../protocol/Protocol.h" 

Room::Room(const std::string& id): id(id),food_(nullptr){

}
void Room:: initialGameWorld()
{
    width=25;
    height=25;

}
void Room::setMap(GameMap*map){
    map_=map;
    width=map_->getWidth();
    height=map_->getHeight();
}
void Room::setFood(Food* food) {
    food_ = food;
}
bool Room::add_player(Player player){
    if(players.size()>=max_size) return false;
    players.emplace_back(player);
    return true;
}
/* 初始化地图，蛇，食物*/
std::vector<std::pair<int,std::string>> Room::startGame()
{
    initialGameWorld();
    std::vector<std::pair<int,std::string>> messagesToSend;
    nlohmann::json j;
    j["width"]=width;
    j["height"]=height;
    for(const auto &play:players)
    {
        std::string str=Protocol::build_response(2,j);
        messagesToSend.push_back({play.id,str});
    }
    return messagesToSend;
        

}
Room::~Room() {
    // 析构函数
    std::cout<<"房间炸了"<<std::endl;
}