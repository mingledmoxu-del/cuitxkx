#include <stdio.h>
#include <string.h>

#include "item.h"
#include "parser.h"
#include "player.h"

/* --- 全局解析器变量定义 --- */
int   parser_argc = 0; /* 指令参数个数 */
char *parser_argv[5];  /* 参数字符串指针数组（最多支持 5 个参数） */
char  parser_buf[64];  /* 用户输入原始文本缓冲区 */

// /* 玩家当前的地理位置，初始设定为「player_spawn」（在 room.c 中定义） */
// room_loc_t *current_loc = &player_spawn;

/**
 * @brief 游戏程序唯一入口
 * 实现主要的游戏循环：输入 -> 解析 -> 执行
 */
int main() {

    /* 启动界面欢迎词 */
    printf("欢迎来到 cuitxkx，输入 'quit' 退出游戏\n");

    /* 初始进入游戏时自动执行「look」指令，展示周遭环境 */
    cmd_look(1, NULL);
    player_add_item(&omo, &con_wound_medic);
    player_add_item(&omo, &con_grand_medic);
    /* 核心游戏引擎循环 */
    while (1) {
        /* 第一步：获取玩家输入。如果检测到 EOF 或输入错误，则退出循环 */
        if (parser_input(parser_buf, sizeof(parser_buf)) == NULL)
            break;

        /* 第二步：将输入的原始文本拆分为独立的参数词条 */
        parser_argc = parser_split(parser_buf, parser_argv, 5);

        /* 第三步：执行指令。如果 parser_execute 返回非 0 值（如 quit 指令），则退出 */
        if (parser_execute(parser_argc, parser_argv))
            break;
    }

    /* 游戏程序结束 */
    return 0;
}