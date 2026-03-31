#include "Room.h"
#include "Snake.h" // 为了创建 Snake
#include <nlohmann/json.hpp>
#include "../protocol/Protocol.h" 

Room::Room(const std::string& id): id(id){
    food=std::make_unique<Food>(width,height);
    
}
void Room:: initialGameWorld()
{
    width=25;
    height=25;
    food->generateRandom(width,height,players);

}
void Room::setMap(GameMap*map){
    map_=map;
    width=map_->getWidth();
    height=map_->getHeight();
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
    nlohmann::json j,foodjson;
    j["width"]=width;
    j["height"]=height;
    auto [x,y]=food->getPosition();
    foodjson["x"]=x;
    foodjson["y"]=y;
    j["food"]=foodjson;
    for(auto &play:players)
    {
        j["player"]=playerToJson(play);
        std::string str=Protocol::build_response(2,j);
        messagesToSend.push_back({play.id,str});
    }
    return messagesToSend;
        

}
std::vector<nlohmann::json> Room::getPlayerInfo() 
{
    std::vector<nlohmann::json> playerInfo;
    for(auto &player:players)
    {
        playerInfo.push_back(playerToJson(player));
    }
    return playerInfo;
}
std::string Room::updateGameState()
{
    for(auto &player:players)
    {

        if (player.state == PlayerState::dead) {
            // 更新重生计时器
            if (player.respawnTimer > 0) {
                player.respawnTimer--;
            } else {
                Snake* snake = player.snake; 
                std::pair<int, int> safePosition = findSafeSpawnPosition();
                
                    snake->resetToInitialPos(safePosition.first,safePosition.second,Direction::right); 
                    player.state = PlayerState::alive;
                    std::cout << "Player " << player.id << " has respawned at position (" 
                              << safePosition.first << ", " << safePosition.second << ")." << std::endl;
                
            }
        }
        else if(player.snake!=nullptr )
        {
            player.snake->move();
            auto head_pos=player.snake->getHeadPos();
            if(head_pos==food->getPosition())
            {
                player.snake->grow();
                food->generateRandom(width,height,players);
                player.score++;
            }
            if(player.snake->checkSelfCollision())
            {
                // setRunning(false);
                // break;
            }
        }

        for (auto& player : players) {
            if (player.state == PlayerState::alive) { // 只检查存活的玩家
                Snake* snake = player.snake;
                if (snake->checkSelfCollision()) {
                    handlePlayerDeath(player);
                }
        }
    }
            
    } 

    nlohmann::json gameStateJson;
    gameStateJson["type"] = "game_state_update";
    gameStateJson["room_id"] = id;

    nlohmann::json playersArray = nlohmann::json::array();
    for (const Player& p : players) {
        nlohmann::json playerState;
        playerState["id"] = p.id;
        playerState["score"] = p.score;
        playerState["player_state"]=p.state;
        if (p.snake != nullptr) {
            playerState["snake_body"] = p.snake->getBody();
        }
        playersArray.push_back(playerState);
    }
    gameStateJson["players"] = playersArray;


    nlohmann::json foodjson;
    auto [x,y]=food->getPosition();
    foodjson["x"]=x;
    foodjson["y"]=y;
    gameStateJson["food"]=foodjson;
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

void Room:: handlePlayerDeath(Player& player)
{
    if(player.state==PlayerState::alive)
    {
        player.state=PlayerState::dead;
        player.respawnTimer=5;
        player.score/=2;
    }

}

std::pair<int,int> Room::findSafeSpawnPosition(){
    return std::make_pair(width/2,height/2);
}

nlohmann::json Room::playerToJson(Player& player){
    nlohmann::json j;
    j["id"]=player.id;
    j["player_state"]=player.state;
    j["score"]=player.score;

    // 如果蛇不为空，则序列化其身体
    if (player.snake != nullptr) {
        j["snake"] = player.snake->getBody(); 
    } else {
        j["snake"] = nlohmann::json::array(); // 如果蛇为空，可以发送一个空数组或其他标识
    }
    return j;
}
Room::~Room() {

    for(auto &player:players)
    {
        //释放蛇内存
        if(player.snake!=nullptr)
        {
            delete player.snake;
            player.snake=nullptr;
        }
    }
    // 析构函数
    std::cout<<"房间炸了"<<std::endl;
}