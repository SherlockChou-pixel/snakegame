#include "Protocol.h"
#include <iostream>

Protocol::Protocol() {
    head = 0;
    len = 0;
}

Protocol::~Protocol() {
}

// 解析协议数据
ProtocolData Protocol::parse(const std::string& json_str) {
    ProtocolData result;
    try {
        std::cout << "json_str: " << json_str << std::endl;
        nlohmann::json j = nlohmann::json::parse(json_str);
        // 先检测指令类型
        if (j.contains("cmd")) {
            result.cmd_id = j["cmd"].get<int>();
            std::cout << "cmd_id: " << 666 << std::endl;
            // 再获取对应数据
            if (j.contains("data")) {
                result.data = j["data"];
            } else {
                result.data = nlohmann::json::object();
            }
            result.valid = true;
        } else {
            result.error_msg = "Missing 'cmd' field in request";
        }
    } catch (const std::exception& e) {
        result.error_msg = std::string("Parse error: ") + e.what();
        // 添加日志输出，便于调试
        std::cout << "Parse error: " << e.what() << std::endl;
    }
    
    if (!result.valid && result.error_msg.empty()) {
        result.error_msg = "Invalid protocol format";
    }
    return result;
}

// 构建响应
std::string Protocol::build_response(int cmd_id, const nlohmann::json& data) {
    nlohmann::json response;
    response["cmd"] = cmd_id;
    response["status"] = "success";
    response["data"] = data;
    
    return response.dump();
}

// 构建错误响应
std::string Protocol::build_error(int cmd_id, const std::string& error_msg) {
    nlohmann::json response;
    response["cmd"] = cmd_id;
    response["status"] = "error";
    response["msg"] = error_msg;
    
    return response.dump();
}