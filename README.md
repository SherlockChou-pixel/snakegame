# SnakeServer - 贪吃蛇多人游戏服务器

## 项目描述

这是一个基于C++实现的贪吃蛇多人在线游戏服务器项目。服务器使用Linux epoll机制实现高并发网络处理，支持多个玩家同时在线游戏。项目采用模块化设计，将网络通信、游戏逻辑和数据管理分离，便于维护和扩展。

## 特性

- **高并发网络服务器**: 使用epoll实现高效的I/O多路复用，支持大量并发连接
- **线程池处理**: 利用线程池进行任务调度，提高服务器响应性能
- **房间管理**: 支持多个游戏房间，玩家可以创建或加入房间进行游戏
- **游戏逻辑**: 完整的贪吃蛇游戏逻辑，包括蛇的移动、食物生成、碰撞检测等
- **协议解析**: 自定义网络协议，支持客户端与服务器的通信
- **跨平台兼容**: 基于Linux系统开发，充分利用epoll的优势

## 架构设计

项目采用分层架构设计，将不同功能模块分离：

### 网络层 (Network Layer)
- `EpollServer`: 负责网络连接的监听、接受和数据传输
- `NetworkSenderAdapter`: 网络发送适配器，统一管理数据发送接口

### 逻辑层 (Logic Layer)
- `GameHandler`: 游戏业务处理器，负责解析客户端消息和处理游戏逻辑
- `Protocol`: 网络协议解析器，定义客户端与服务器的通信协议

### 数据层 (Data Layer)
- `RoomManager`: 房间管理器，管理游戏房间的创建、加入和销毁
- `Snake`: 蛇对象，管理蛇的状态和行为
- `Food`: 食物对象，管理食物生成和位置
- `Room`: 游戏房间，管理房间内的玩家和游戏状态

### 工具层 (Utils Layer)
- `ThreadPool`: 线程池实现，提供任务并发处理能力

## 构建要求

- **操作系统**: Linux (支持epoll)
- **编译器**: 支持C++17标准的编译器 (如g++)
- **构建工具**: CMake 3.10 或更高版本
- **依赖库**:
  - pthread (POSIX线程库)
  - ncurses (终端界面库)

## 构建和运行

### 自动构建脚本 (推荐)

```bash
# 给脚本执行权限
chmod +x run.sh

# 运行构建和启动脚本
./run.sh
```

### 手动构建

```bash
# 创建构建目录
mkdir -p build
cd build

# 生成Makefile
cmake ..

# 编译项目
make

# 运行服务器
../bin/ServerTest
```

服务器启动后将监听8888端口，输出服务器状态信息。按 'q' + 回车可以优雅退出服务器。

## 项目结构

```
snakegame/
├── CMakeLists.txt          # CMake构建配置
├── run.sh                  # 自动构建和运行脚本
├── note.txt                # 项目笔记和技术说明
├── bin/                    # 可执行文件输出目录
│   └── ServerTest          # 服务器可执行文件
├── build/                  # 构建目录 (CMake生成)
└── src/                    # 源代码目录
    ├── main.cpp            # 程序入口
    ├── game/               # 游戏逻辑模块
    │   ├── Snake.cpp/h     # 蛇类实现
    │   ├── Food.cpp/h      # 食物类实现
    │   ├── Room.cpp/h      # 房间类实现
    │   ├── Map.h           # 地图定义
    │   └── Player.h        # 玩家定义
    ├── server/             # 服务器模块
    │   ├── EpollServer.cpp/h      # epoll服务器实现
    │   ├── RoomManager.cpp/h      # 房间管理器
    │   ├── NetworkSenderAdapter.cpp/h  # 网络发送适配器
    │   ├── GameHandler.h           # 游戏处理器接口
    │   ├── IMessageHandler.h       # 消息处理器接口
    │   └── INetworkSender.h        # 网络发送接口
    ├── protocol/           # 协议模块
    │   ├── Protocol.cpp/h  # 协议解析实现
    └── utils/              # 工具模块
        └── ThreadPool.cpp/h # 线程池实现
```

## 使用方法

1. **启动服务器**: 运行上述构建命令启动服务器
2. **客户端连接**: 客户端需要通过TCP连接到服务器的8888端口
3. **游戏流程**:
   - 玩家连接后可以创建或加入游戏房间
   - 每个房间支持多个玩家同时游戏
   - 服务器处理游戏逻辑并广播游戏状态

## 开发和调试

- 项目使用C++17标准，确保编译器支持
- 调试时可以通过服务器输出的日志信息了解运行状态
- 网络协议定义在`Protocol.h`中，可根据需要扩展

## 贡献指南

欢迎提交Issue和Pull Request来改进项目！

1. Fork本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 创建Pull Request

## 许可证

本项目采用MIT许可证 - 查看 [LICENSE](LICENSE) 文件了解详情。

## 致谢

- 感谢Linux epoll机制提供的高性能网络支持
- 感谢C++标准库和CMake构建系统的强大功能</content>
<parameter name="filePath">/home/chou/snakegame/README.md