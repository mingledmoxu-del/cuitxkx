#ifndef PARSER_H
#define PARSER_H

#include "player.h"
#include "room.h"

#include <util.h>

extern player_def_t omo;

/**
 * @brief 指令处理函数统一接口
 * @param argc 参数个数
 * @param argv 参数字符串数组
 * @return int 执行状态码
 */
typedef int (*command_handler)(int argc, char **argv);

/* 指令条目结构体前置声明 */
typedef struct cmd_entry_e cmd_entry_t;

/* --- 指令处理函数声明 --- */
int cmd_quit(int argc, char **argv);  /* 退出游戏 */
int cmd_help(int argc, char **argv);  /* 显示帮助功能 */
int cmd_go(int argc, char **argv);    /* 处理玩家移动逻辑 */
int cmd_look(int argc, char **argv);  /* 观察当前房间环境 */
int cmd_talk(int argc, char **argv);  /* 与指定 NPC 进行对话 */
int cmd_see(int argc, char **argv);   /* 检视指定物体或人物的详细描述 */
int cmd_get(int argc, char **argv);   /* 检视指定物体或人物的详细描述 */
int cmd_wear(int argc, char **argv);  /* 检视指定物体或人物的详细描述 */
int cmd_check(int argc, char **argv); /* 检视指定物体或人物的详细描述 */

/* --- 核心解析器函数声明 --- */

/**
 * @brief 读取用户从终端输入的指令
 * @param buf 存放输入的缓冲区
 * @param size 缓冲区最大长度
 * @return char* 如果成功则返回缓冲区指针，失败返回 NULL
 */
char *parser_input(char *buf, int size);

/**
 * @brief 将输入的一行文本字符串按空格拆分为参数数组
 * @param line 原始输入文本
 * @param argv 解析后的动态参数数组容器
 * @param max_args 最大允许解析出的参数数量
 * @return int 返回实际解析出的参数总数
 */
int parser_split(char *line, char **argv, int max_args);

/**
 * @brief 按照解析出的指令名称匹配并调用相应的处理函数
 * @param argc 参数个数
 * @param argv 参数数组指针
 * @return int 返回处理函数的执行结果（通常 0 为成功）
 */
int parser_execute(int argc, char **argv);

#endif