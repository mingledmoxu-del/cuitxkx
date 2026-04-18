cmake_minimum_required(VERSION 3.10)
project(MUD_Demo C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# 添加可执行文件
add_executable(mud ./demo/demo.c)
