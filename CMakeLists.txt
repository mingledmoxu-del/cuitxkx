cmake_minimum_required(VERSION 3.10)
project(MUD_Demo C)

# 设置 C 标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include_directories(
    inc
    text
)

# 添加可执行文件
add_executable(mud demo/demo.c)
add_executable(game src/main.c 
src/parser.c
src/room.c    # 【新增】
src/item.c
src/player.c
src/skill.c
)

# 配置头文件包含目录
target_include_directories(mud PUBLIC inc)
target_include_directories(game PUBLIC inc)
