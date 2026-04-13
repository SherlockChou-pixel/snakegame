#ifndef _GAME_HANDLER_H
#define _GAME_HANDLER_H

#include "IMessageHandler.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class RoomManager;

class GameHandler : public IMessageHandler {
private:
    RoomManager* roomManagerRef;

public:
    explicit GameHandler(RoomManager& rm);

    void onConnect(int client_fd) override;
    void onMessage(int client_fd, const char* buffer, size_t length) override;
    void onDisconnect(int client_fd) override;
};

#endif