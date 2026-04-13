#ifndef _MESSAGEQUEUE_H
#define _MESSAGEQUEUE_H
#include<queue>
#include<mutex>
#include<string>
enum class MsgType {
    DATA,        // 普通数据
    CONNECT,     // 连接
    DISCONNECT   // 断开
};
struct NetMessage{
    int client_fd;
    std::string data;
    MsgType type;
};

class MessageQueue {
public:
    static MessageQueue& instance() {
        static MessageQueue inst;
        return inst;
    }

    // ====================== 方法重载 START ======================

    // 1. 推送【普通数据消息】
    void push(int client_fd, const std::string& data) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push({ client_fd, data, MsgType::DATA });
    }

    // 2. 推送【连接事件】（重载）
    void push(int client_fd, MsgType type) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push({ client_fd, "", type });
    }

    // ====================== 方法重载 END ======================

    bool pop(NetMessage& out_msg) {
        std::lock_guard<std::mutex> lock(m_mutex);
        if (m_queue.empty()) return false;

        out_msg = m_queue.front();
        m_queue.pop();
        return true;
    }

private:
    MessageQueue() = default;
    std::queue<NetMessage> m_queue;
    std::mutex m_mutex;
};

#endif
