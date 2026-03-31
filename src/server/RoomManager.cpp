#include "RoomManager.h"
#include "../game/Room.h"      // 在 .cpp 中包含，以获得 Room 的完整定义
#include "../game/Snake.h"     // 在 .cpp 中包含，以获得 Snake 的完整定义

RoomManager::RoomManager(INetworkSender& sender) : networkSenderRef(sender) {

}
void RoomManager:: joinRoom(int client_fd)
{
    for(auto& [roomid,room]:activeRooms)
    {
        if(!room->isRoomFull())
        {
            Player player;
            player.id=client_fd;
            player.score=0;
            nlohmann::json j=room->playerToJson(player);
            j["room_id"]=roomid;
            room->add_player(std::move(player));
            std::cout<<"新玩家已加入房间"<<std::endl;
            std::string successMsg = Protocol::build_response(1,j);
            std::cout<<"发送消息"<<successMsg<<std::endl;
            networkSenderRef.sendToClient(client_fd, successMsg);

            auto all_players=room->getPlayerInfo();
            nlohmann::json msg_for_existing_players;
            msg_for_existing_players["players"]=all_players;
            for(auto& player:room->getPlayers())
            {
                networkSenderRef.sendToClient(player.id, Protocol::build_response(6, msg_for_existing_players));
            }
            return;
        }
    }
    std::cout<<"没有可用房间，创建新房间"<<std::endl;
    creatRoom(client_fd);
}
void RoomManager::creatRoom(int client_fd)
{
    std::string newRoomid="Room"+std::to_string(roomid++);
    Player player;
    player.id=client_fd;
    player.score=0;
    auto room=std::make_unique<Room>(newRoomid);

    nlohmann::json j=room->playerToJson(player);
    j["room_id"]=newRoomid;
    
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
        auto& players=activeRooms[roomid]->getPlayers();
        int startX=0,startY=0;
        for(auto& player:players)
        {
           if(player.snake==nullptr)
           {
                player.snake = new Snake(startX, startY, 3, Direction::left, std::make_pair(25, 25));
                player.state = PlayerState::alive;
           }
           startX+=4;
           startY+=4;
        }
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

void RoomManager::startGameLoop()
{
    if(!gameLoopTread.joinable())
    {
        gameLoopRunning.store(true);
        gameLoopTread = std::thread(&RoomManager::runGameLoop, this);
    }
}
/*更新所有房间游戏状态*/
void RoomManager::updateAllRooms(){
    // 使用迭代器遍历，这样可以在遍历时安全地删除元素
    for(auto it = activeRooms.begin(); it != activeRooms.end(); ) {
        auto& [roomid, room] = *it; // 解引用迭代器得到 pair
        Room* roomPtr = room.get();
        
        if(roomPtr->isRunning()) {
            std::string msg = roomPtr->updateGameState();
            nlohmann::json res = nlohmann::json::parse(msg);
            const std::vector<Player>& roomPlayers = room->getPlayers();
            
            for (const Player& player : roomPlayers) {
                int clientId = player.id;
                networkSenderRef.sendToClient(clientId, Protocol::build_response(4, res));
            }
        }

        // 检查房间是否为空
        if (room->isRoomEmpty()) {
            std::cout << "Cleaning up empty room: " << roomid << std::endl;
            // erase 返回下一个有效的迭代器，直接赋值给 it
            it = activeRooms.erase(it);
        } else {
            // 房间不为空，移动到下一个
            ++it;
        }
    }
}
//方向改变
void RoomManager::changePlayerDirection(const std::string& room_id, int player_id, Direction dir)
{
    auto it = activeRooms.find(room_id);
    if (it != activeRooms.end()) {
        // 找到房间，直接调用其处理函数
        it->second->handlePlayerInput(player_id, dir);
    } else {
        std::cerr << "Room with ID " << room_id << " not found." << std::endl;
    }
}
void RoomManager::handlePlayerDisconnect(int client_fd){
    std::cout<<"开始清理"<<std::endl;
    for(auto& [room_id,room]:activeRooms)
    {
        
        room->removePlayer(client_fd);
        auto all_players=room->getPlayerInfo();
        nlohmann::json msg_for_existing_players;
        msg_for_existing_players["players"]=all_players;
        for(auto& player:room->getPlayers())
        {
            networkSenderRef.sendToClient(player.id, Protocol::build_response(6, msg_for_existing_players));
        }
        break;
    }

}

RoomManager::~RoomManager(){

    gameLoopRunning.store(false);
    if(gameLoopTread.joinable())
        gameLoopTread.join();
}