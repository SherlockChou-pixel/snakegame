#include "Room.h"
#include "Snake.h" // 为了创建 Snake
#include <nlohmann/json.hpp>
#include "../protocol/Protocol.h" 
#include <nlohmann/json.hpp>
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
bool Room::add_player(Player&& player){
    if(players.size()>=max_size) return false;
    players.emplace_back(std::move(player));
    return true;
}
/* 初始化地图，蛇，食物*/
std::vector<std::pair<int,std::string>> Room::startGame()
{
    initialGameWorld();
    setRunning(true);
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
std::string Room::updateGameState()
{
    for(auto &player:players)
    {
        if(player.snake!=nullptr )
            player.snake->move();
    } 

    nlohmann::json gameStateJson;
    gameStateJson["type"] = "game_state_update";
    gameStateJson["room_id"] = id;

    nlohmann::json playersArray = nlohmann::json::array();
    for (const Player& p : players) {
        nlohmann::json playerState;
        playerState["id"] = p.id;
        playerState["score"] = p.score;
        if (p.snake != nullptr) {
            playerState["snake_body"] = p.snake->getBody();
        }
        playersArray.push_back(playerState);
    }
    gameStateJson["players"] = playersArray;

    // 返回这个消息字符串
    return gameStateJson.dump();
}
void Room::handlePlayerInput(int player_id, Direction new_dir){
    for (auto& player : players) {
        if (player.id == player_id) {
            if (player.snake != nullptr) {
                player.snake->setDirection(new_dir);
            }
            return; // 找到并处理后返回
        }
    }

}

void Room::removePlayer(int playerId) {
    players.erase(
        std::remove_if(players.begin(), players.end(),
            [playerId](const Player& p) { return p.id == playerId; }),
        players.end()
    );
    // 可选：打印日志
    std::cout << "Player " << playerId << " removed from room " << id << "." << std::endl;
}
Room::~Room() {
    // 析构函数
    std::cout<<"房间炸了"<<std::endl;
}