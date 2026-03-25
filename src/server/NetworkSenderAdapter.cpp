// NetworkSenderAdapter.cpp
#include "NetworkSenderAdapter.h"

void NetworkSenderAdapter::sendToClient(int client_fd, const std::string& msg) {
    server->sendToClient(client_fd, msg);
}

void NetworkSenderAdapter::broadcastToClients(const std::vector<int>& client_fds, const std::string& msg) {
    for (int fd : client_fds) {
        server->sendToClient(fd, msg);
    }
}