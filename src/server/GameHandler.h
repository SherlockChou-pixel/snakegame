#ifndef _GAME_HANDLER_H
#define _GAME_HANDLER_H

#include "IMessageHandler.h"
#include "EpollServer.h"
#include "../game/Map.h"      // 修复地图头文件路径
#include "../protocol/Protocol.h"  // 修复协议头文件路径
#include <nlohmann/json.hpp>
#include <atomic>
#include <iostream>

using json = nlohmann::json;

class GameHandler : public IMessageHandler {
private:
    EpollServer* server;
    GameMap* map;           // 👈 改为持有 GameMap 指针
    std::atomic<int> msg_count{0};

public:
    // 构造函数参数变更
    GameHandler(EpollServer* srv, GameMap* m) : server(srv), map(m) {}

    void onConnect(int client_fd) override {
        std::cout << "[GAME] 新玩家连接 fd=" << client_fd << std::endl;
        
        // 1. 发送欢迎语
        json welcome;
        welcome["cmd"] = CMD_START;
        welcome["status"] = "success";
        welcome["msg"] = "Connected! Receiving map...";
        server->sendToClient(client_fd, welcome.dump() + "\n");

        // 2. 发送地图
        // sendMap(client_fd);
    }
    //处理收到的消息
    void onMessage(int client_fd, const char* buffer, size_t length) override {  
        msg_count++;
        
        std::string json_line(buffer, length);  // 转换为string
        
        // 解析协议
        ProtocolData pd = Protocol::parse(json_line);
        if (!pd.valid) {
            std::string err = Protocol::build_error(CMD_UNKNOWN, pd.error_msg) + "\n";
            server->sendToClient(client_fd, err);
            return;
        }

        std::cout << "[LOG] fd=" << client_fd << " Cmd=" << pd.cmd_id << std::endl;

        json response_data;

        switch (pd.cmd_id) {
            case CMD_MAP:
                // 客户端主动请求刷新地图
                sendMap(client_fd);
                return; // 已经在 sendMap 里发了，直接返回

            case CMD_MOVE: {
                std::string dir = pd.data.value("dir", "UP");
                std::cout << "  -> 玩家尝试移动方向: " << dir << std::endl;
                
                // TODO: 这里以后会调用 map->moveSnake(...)
                response_data["result"] = "received";
                response_data["dir"] = dir;
                response_data["note"] = "Logic not implemented yet";
                break;
            }

            default:
                response_data["note"] = "Command received but not handled";
        }

        // 发送响应
        std::string reply = Protocol::build_response(pd.cmd_id, response_data) + "\n";
        server->sendToClient(client_fd, reply);
    }

    void onDisconnect(int client_fd) override {
        std::cout << "[GAME] 玩家断开 fd=" << client_fd << std::endl;
    }

private:
    // 封装发送地图的逻辑
    void sendMap(int client_fd) {
        if (!map) return;
        
        // 获取地图的 JSON 表示
        json map_json = map->toJson();
        
        // 打包并发送
        std::string packet = Protocol::build_response(CMD_MAP, map_json) + "\n";
        server->sendToClient(client_fd, packet);
        std::cout << "  -> 已发送地图数据给 fd=" << client_fd << std::endl;
    }
};

#endif