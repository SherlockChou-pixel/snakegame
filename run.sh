#!/bin/bash

# 切换到工程目录
cd /home/chou/snakegame

# 创建构建目录（如果不存在）
mkdir -p build

# 进入构建目录
cd build

# 运行 cmake
cmake ..

# 运行 make
make

# 运行生成的可执行文件
../bin/ServerTest