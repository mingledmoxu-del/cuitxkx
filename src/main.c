#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <unistd.h>

#include "item.h"
#include "parser.h"
#include "player.h"
#include "skill.h"

/* --- 全局解析器变量定义 --- */
int   parser_argc = 0; /* 指令参数个数 */
char *parser_argv[5];  /* 参数字符串指针数组（最多支持 5 个参数） */
char  parser_buf[64];  /* 用户输入原始文本缓冲区 */

// /* 玩家当前的地理位置，初始设定为「player_spawn」（在 room.c 中定义） */
// room_loc_t *current_loc = &player_spawn;

int is_input_pending(void) {
    struct timeval timeval = {0, 0};

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);

    return select(1, &fds, NULL, NULL, &timeval) > 0;
}

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
    int need_prompt = 1;
    /* 物品与角色属性初始化 */
    player_add_item(&omo, &con_wound_medic);
    player_add_item(&omo, &con_foucus_medic);
    player_add_skill(&omo, &skill_zazen);
    /* 初始进入游戏时自动展示周遭环境 */
    cmd_look(1, NULL);
    /* src/main.c */
    static int was_fighting = 0; // 记录上一轮是否在战斗中
    while (1) {
        int is_fighting = (omo.fight_target != NULL);
        // [探测点]：如果刚才在打，现在不打了，说明战斗刚结束
        if (was_fighting && !is_fighting) {
            need_prompt = 1;
        }
        was_fighting = is_fighting;
        // [显示点]：只有在需要提示符，且当前没在打架时才显示
        if (need_prompt && !is_fighting) {
            printf("\n" CLR_GREEN CLR_BOLD "%s" CLR_RESET " > ", omo.player_cur_loc->room_name);
            fflush(stdout);
            need_prompt = 0;
        }
        if (is_input_pending()) {
            if (parser_input(parser_buf, sizeof(parser_buf)) != NULL) {
                parser_argc = parser_split(parser_buf, parser_argv, 5);

                // 1. 如果有词，执行逻辑
                if (parser_argc > 0) {
                    parser_execute(parser_argc, parser_argv);
                }
                // 2. 无论有没有词（哪怕是空回车），
                // 只要当前没在战斗，就允许主循环在下一圈刷出提示符
                if (omo.fight_target == NULL) {
                    need_prompt = 1;
                }
            }
        }
        combat_tick(&omo);
        usleep(10000);
    }
    /* 游戏程序结束 */
    system("clear");
    return 0;
}