#include <iostream>
#include <memory>
#include <atomic>
#include <thread>
#include <ctime>
#include <cstdlib>

// 引入你的核心组件
#include "server/EpollServer.h"      // 网络层
#include "server/GameHandler.h"      // 业务处理层 (包含协议解析)
#include "game/Map.h"          // 地图定义 (刚才新建的)

int main() {
    // 初始化随机数种子 (虽然静态地图暂时不用，但留着好习惯)
    srand(static_cast<unsigned int>(time(NULL)));

    const int PORT = 8888;
    const int MAP_WIDTH = 25;
    const int MAP_HEIGHT = 25;

    std::cout << "=== 🐍 贪吃蛇服务器启动 ===" << std::endl;
    std::cout << "监听端口: " << PORT << std::endl;
    std::cout << "地图尺寸: " << MAP_WIDTH << " x " << MAP_HEIGHT << std::endl;
    std::cout << "按 'q' + 回车 退出服务器\n" << std::endl;

    try {
        // 1. 创建地图对象 (独立于 Room)
        // 使用 unique_ptr 管理生命周期
        auto game_map = std::make_unique<GameMap>(MAP_WIDTH, MAP_HEIGHT);
        
        // 验证一下地图生成是否成功 (可选调试)
        std::cout << "[DEBUG] 地图已初始化，网格大小: " 
                  << game_map->getWidth() << "x" << game_map->getHeight() << std::endl;

        // 2. 创建服务器实例
        auto server = std::make_unique<EpollServer>(PORT);
        server->set_listen(5); // 设置监听队列长度

        // 3. 创建业务处理器 (GameHandler)
        // 将 server 和 game_map 的原始指针传进去 (注意：确保 game_map 生命周期长于 handler)
        auto handler = std::make_unique<GameHandler>(server.get(), game_map.get());

        // 4. 绑定处理器到服务器
        server->setMessageHandler(std::move(handler));

        // 5. 启动键盘监听线程 (用于优雅退出)
        std::atomic<bool> quit_flag(false);
        std::thread input_thread([&quit_flag, &server]() {
            char c;
            while (!quit_flag.load()) {
                if (std::cin.get(c)) {
                    if (c == 'q' || c == 'Q') {
                        std::cout << "\n[系统] 收到退出指令，正在关闭服务器..." << std::endl;
                        server->stop(); // 触发 Epoll 停止
                        quit_flag.store(true);
                        break;
                    }
                } else {
                    // cin 出错或 EOF
                    break;
                }
            }
        });

        // 6. 运行服务器主循环 (阻塞直到 stop() 被调用)
        server->run();

        // 7. 清理工作
        quit_flag.store(true);
        if (input_thread.joinable()) {
            input_thread.join();
        }
        
        std::cout << "=== 服务器已安全关闭 ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "❌ 发生严重错误: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}