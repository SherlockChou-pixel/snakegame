#include "RoomManager.h"

RoomManager::RoomManager(INetworkSender& sender) : networkSenderRef(sender) {
    
}
RoomManager::~RoomManager(){}