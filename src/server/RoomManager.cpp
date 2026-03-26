#include "RoomManager.h"
#include "../game/Room.h"      // 在 .cpp 中包含，以获得 Room 的完整定义
#include "../game/Snake.h"     // 在 .cpp 中包含，以获得 Snake 的完整定义
#include <nlohmann/json.hpp>
RoomManager::RoomManager(INetworkSender& sender) : networkSenderRef(sender) {
    
}
void RoomManager::creatRoom(int client_fd)
{
    std::string newRoomid="Room"+std::to_string(roomid++);
    Player player;
    player.id=client_fd;
    player.snake = std::make_unique<Snake>(0,0,3,Direction::left,std::make_pair(25, 25)); 
    player.score=0;
    auto room=std::make_unique<Room>(newRoomid);
    room->add_player(std::move(player));

    activeRooms[newRoomid]=std::move(room);
    std::cout<<"已成功创建房间"<<std::endl;

    nlohmann::json j;
    j["room_id"]=newRoomid;
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
RoomManager::~RoomManager(){}