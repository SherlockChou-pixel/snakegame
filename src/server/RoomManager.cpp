#include "RoomManager.h"
#include "../game/Room.h"
#include <nlohmann/json.hpp>
RoomManager::RoomManager(INetworkSender& sender) : networkSenderRef(sender) {
    
}
void RoomManager::creatRoom(int client_fd)
{
    std::string newRoomid="Room"+std::to_string(roomid++);
    int queue_size=wait_queue.size();
    Player player;
    player.id=client_fd;
    // player.snake=std::make_unique<Snake*>();
    player.score=0;
    auto room=std::make_unique<Room>(newRoomid);
    room->add_player(player);

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