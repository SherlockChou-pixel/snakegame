// NetworkSenderAdapter.h
#ifndef _NETWORK_SENDER_ADAPTER_H
#define _NETWORK_SENDER_ADAPTER_H

#include "INetworkSender.h" // 确保包含接口头文件
#include "EpollServer.h"
#include <vector>
#include <string>

class NetworkSenderAdapter : public INetworkSender { 
private:
    EpollServer* server;

public:
    explicit NetworkSenderAdapter(EpollServer* s) : server(s) {}

    void sendToClient(int client_fd, const std::string& msg) override; // override 关键字会检查签名是否匹配
    void broadcastToClients(const std::vector<int>& client_fds, const std::string& msg) override;
};

#endif // _NETWORK_SENDER_ADAPTER_H