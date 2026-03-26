#include "RoomManager.h"
#include "../game/Room.h"      // 在 .cpp 中包含，以获得 Room 的完整定义
#include "../game/Snake.h"     // 在 .cpp 中包含，以获得 Snake 的完整定义
#include <nlohmann/json.hpp>
RoomManager::RoomManager(INetworkSender& sender) : networkSenderRef(sender) {
    gameLoopRunning.store(true);
    gameLoopTread = std::thread(&RoomManager::runGameLoop, this);
}
void RoomManager::creatRoom(int client_fd)
{
    std::string newRoomid="Room"+std::to_string(roomid++);
    Player player;
    player.id=client_fd;
    player.snake = std::make_unique<Snake>(0,0,3,Direction::left,std::make_pair(25, 25)); 
    player.score=0;
    nlohmann::json j;
    j["room_id"]=newRoomid;
    j["id"]=player.id;
    if (player.snake != nullptr)
        j["snake"]=player.snake->getBody();
    j["score"]=player.score;
    auto room=std::make_unique<Room>(newRoomid);
    room->add_player(std::move(player));

    activeRooms[newRoomid]=std::move(room);
    std::cout<<"已成功创建房间"<<std::endl;


    std::string successMsg = Protocol::build_response(1,j);
    std::cout<<"发送消息"<<successMsg<<std::endl;
    networkSenderRef.sendToClient(client_fd, successMsg);
}
void RoomManager::startGame(const std::string& roomid)
{
    if(activeRooms.find(roomid)!=activeRooms.end())
    {
        std::vector<std::pair<int,std::string>>msg=activeRooms[roomid]->startGame();
        for(const auto&[fd,s]:msg)
        {
            networkSenderRef.sendToClient(fd,s);
        }
        
    }
}

void RoomManager::runGameLoop(){
    const int TICK_INTERVAL_MS = 200; // 定义游戏循环的频率，100毫秒一次，即10帧/秒
    while(gameLoopRunning.load())
    {
        auto frameStartTime = std::chrono::steady_clock::now();
        updateAllRooms();
        auto frameEndTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEndTime - frameStartTime);
    
        if(elapsedTime.count() < TICK_INTERVAL_MS)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(TICK_INTERVAL_MS - elapsedTime.count()));
        }
    }

    std::cout<<"游戏结束"<<std::endl;
}
/*更新所有房间游戏状态*/
void RoomManager::updateAllRooms(){
    for(auto&[roomid,room]:activeRooms)
    {
        Room* roomPtr=room.get();
        if(roomPtr->isRunning())
        {
            std::string msg=roomPtr->updateGameState();
            const std::vector<Player>& roomPlayers = room->getPlayers();
            // std::cout<<msg<<std::endl;
            // 遍历玩家列表，将消息发送给每一个玩家
            for (const Player& player : roomPlayers) {
                int clientId = player.id; // Player 结构体里的 id
                networkSenderRef.sendToClient(clientId, msg);
            }
        }
    }
}
RoomManager::~RoomManager(){

    gameLoopRunning.store(false);
    if(gameLoopTread.joinable())
        gameLoopTread.join();
}