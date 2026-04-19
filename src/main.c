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
    system("clear");
    /* 启动界面仪式感：打印华丽的诗词与框体 */
    printf(CLR_YELLOW CLR_BOLD);
    printf("========================================\n");
    printf("        《青溪渡 · 江湖行》\n");
    printf("========================================\n\n");
    printf(CLR_RESET CLR_CYAN);
    printf("江湖未远风犹在，青山入梦水东流，\n");
    printf("一剑未出心已动，半生浮沉此间游。\n\n");
    printf("今朝踏入尘缘路，且试锋芒问去留。\n\n");
    printf(CLR_RESET CLR_DIM);
    printf("----------------------------------------\n");
    printf("按 Enter 键开始游戏...\n");
    printf(CLR_RESET);

    /* 等待用户交互：阻塞直至按下回车键 */
    getchar();
    printf(">正在载入江湖世界...\n");
    usleep(300000);
    printf(">正在初始化地图...\n");
    usleep(300000);
    printf(">正在初始化人物...\n");
    system("clear");
    /* 物品与角色属性初始化 */
    player_add_item(&omo, &con_wound_medic);
    player_add_item(&omo, &con_foucus_medic);

    /* 初始进入游戏时自动展示周遭环境 */
    cmd_look(1, NULL);
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
    system("clear");
    return 0;
}