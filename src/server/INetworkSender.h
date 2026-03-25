// INetworkSender.h
#ifndef _I_NETWORK_SENDER_H
#define _I_NETWORK_SENDER_H

#include <string>
#include <vector>

class INetworkSender {
public:
    virtual ~INetworkSender() = default;

    virtual void sendToClient(int client_fd, const std::string& message) = 0;
    virtual void broadcastToClients(const std::vector<int>& client_fds, const std::string& message) = 0;
};

#endif // _I_NETWORK_SENDER_H