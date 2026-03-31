cmake_minimum_required(VERSION 3.10)
project(SnakeServer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# 添加可执行文件
# 假设你的主函数在 src/main.cpp 中，如果不在，请修改下面的文件名
add_executable(ServerTest 
    src/game/Snake.cpp
    src/game/Room.cpp
    src/game/Food.cpp
    src/main.cpp
    src/server/EpollServer.cpp
    src/protocol/Protocol.cpp
    src/server/NetworkSenderAdapter.cpp
    src/server/RoomManager.cpp
    src/utils/ThreadPool.cpp
    # 如果有其他 cpp 文件，继续加在这里
)

# 链接 pthread 库 (epoll 在 libc 中，无需单独链接库)
target_link_libraries(ServerTest pthread ncurses)