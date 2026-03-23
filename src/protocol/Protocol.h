#ifndef _PROTOCOL_H
#define _PROTOCOL_H

#include <string>
#include <nlohmann/json.hpp>

// 添加命令ID定义
enum {
    CMD_START = 1,    // 开始命令
    CMD_MAP = 2,      // 地图数据命令
    CMD_MOVE = 3,     // 移动命令
    CMD_UNKNOWN = 99  // 未知命令
};

// 协议数据结构
struct ProtocolData {
    bool valid = false;      // 是否有效
    int cmd_id = 0;          // 命令ID
    nlohmann::json data;     // 数据载荷
    std::string error_msg;   // 错误信息
};

class Protocol{
    int head;//包头
    int len;//长度
public:
    Protocol();
    ~Protocol();
    
    // 解析协议数据
    static ProtocolData parse(const std::string& json_str);
    
    // 构建响应
    static std::string build_response(int cmd_id, const nlohmann::json& data);
    
    // 构建错误响应
    static std::string build_error(int cmd_id, const std::string& error_msg);
};

#endif