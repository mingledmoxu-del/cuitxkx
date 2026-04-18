#include "parser.h"

/**
 * @brief 指令条目结构体定义
 */
typedef struct cmd_entry_e {
    const char     *cmd_name; /* 指令触发名称（用户输入） */
    command_handler cmd_func; /* 对应的处理函数指针 */
    const char     *cmd_desc; /* 简短的功能描述 */
    const char     *cmd_help; /* 详细的使用帮助文本 */
} cmd_entry_t;

/**
 * @brief 指令映射表
 * 集中管理游戏中所有可用的指令。新增指令需在此添加条目。
 */
static cmd_entry_t cmd_table[] = {
    {"quit", cmd_quit, "退出游戏", "用法：直接输入 quit 离开江湖"},
    {"help", cmd_help, "帮助列表", "用法：help [指令名] 获取具体教程"},
    {"go",   cmd_go,   "前往地点", "用法：go [方向] (由 north/south/east/west/up/down 组成)"},
    {"look", cmd_look, "观察环境", "用法：直接输入 look 查看当前房间与出口"},
    {"talk", cmd_talk, "与人对话", "用法：talk [人物ID] 与房间内的 NPC 攀谈"},
    {"see",  cmd_see,  "仔细检视", "用法：see [人物ID] 观察对方的容貌或状态"},
    {NULL,   NULL,     NULL,       NULL},
};

/**
 * @brief 处理「quit」指令
 */
int cmd_quit(int argc, char **argv) {
    printf("青山不改，绿水长流，我们江湖有缘再见！\n");
    return 1; /* 返回 1 会导致主循环 break */
}

/**
 * @brief 处理「help」指令
 * 如果不带参数，列出所有指令；如果带参数，显示该指令的详细用法。
 */
int cmd_help(int argc, char **argv) {

    /* 情况 A：用户只输入了 help */
    if (argc == 1) {
        printf("=== 已学会的江湖指令 ===\n");
        for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
            printf("%s \t %s \n", cmd_table[i].cmd_name, cmd_table[i].cmd_desc);
        }

        printf("\n提示：使用 'help [指令名]' 来获取该指令的秘籍（用法）。\n");
    } 
    /* 情况 B：用户输入了 help [某指令] */
    else {
        int found = 0;
        for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
            if (strcmp(argv[1], cmd_table[i].cmd_name) == 0) {
                printf("【%s 指令秘籍】\n%s\n", cmd_table[i].cmd_name, cmd_table[i].cmd_help);
                found = 1;
                break;
            }
        }

        if (!found)
            printf("抱歉，你尚未领悟名为「%s」的招式。\n", argv[1]);
    }
    return 0;
}

/**
 * @brief 处理「go」指令
 * 实现玩家在不同房间指针间的跳转。
 */
int cmd_go(int argc, char **argv) {
    /* 1. 参数完整性检查 */
    if (argc < 2) {
        printf("你想往哪儿走？\n");
        printf("用法提示：输入 'go [方向]' (例如: go east)\n");
        return 0;
    }

    room_loc_t *next_loc = NULL;

    /* 2. 匹配移动方向，查找当前房间对应的出口指针 */
    if (strcmp(argv[1], "down") == 0) {
        next_loc = current_loc->down;
    } else if (strcmp(argv[1], "up") == 0) {
        next_loc = current_loc->up;
    } else if (strcmp(argv[1], "east") == 0) {
        next_loc = current_loc->east;
    } else if (strcmp(argv[1], "west") == 0) {
        next_loc = current_loc->west;
    } else if (strcmp(argv[1], "north") == 0) {
        next_loc = current_loc->north;
    } else if (strcmp(argv[1], "south") == 0) {
        next_loc = current_loc->south;
    }

    /* 3. 边界检查：如果目标方向没有路（指针为 NULL） */
    if (next_loc == NULL) {
        printf("前面已经没有路了，你张望了一阵，决定留在原地。\n");
        return 0;
    }

    /* 4. 执行移动过程中的氛围描写 */
    if (strcmp(argv[1], "up") == 0)
        printf("你顺着粗糙的树干小心攀爬，借着枝节一点点向上移动。\n");
    else if (strcmp(argv[1], "down") == 0)
        printf("你弯下身子，顺着狭窄的洞口缓缓向下钻入，周围的光线一点点被黑暗吞没。\n");
    else
        printf("你向着 %s 方向出发，脚下的路在迷雾中延伸...\n", argv[1]);
    
    /* 模拟行路时间 */
    sleep(1);

    /* 核心逻辑：更新玩家当前所在的房间指针 */
    current_loc = next_loc; 

    /* 5. 到达新地点后自动执行「look」指令，展示新环境 */
    cmd_look(1, NULL);
    return 0;
}

/**
 * @brief 处理「look」指令
 * 展示当前房间的名称、描述、可见出口以及存在的 NPC。
 */
int cmd_look(int argc, char **argv) {
    /* 1. 打印房间名与详细描述（带颜色高亮） */
    printf(CLR_GREEN CLR_BOLD "【%s】" CLR_RESET "\n", current_loc->room_name);
    printf("%s\n", current_loc->room_desc);

    /* 2. 扫描并打印有效的出口信息 */
    printf(CLR_CYAN "\n这里明显的出口有：" CLR_RESET);
    printf(CLR_BOLD CLR_YELLOW);
    int count = 0;
    if (current_loc->north) { printf("北 "); count++; }
    if (current_loc->south) { printf("南 "); count++; }
    if (current_loc->east)  { printf("东 "); count++; }
    if (current_loc->west)  { printf("西 "); count++; }
    if (current_loc->up)    { printf("上 "); count++; }
    if (current_loc->down)  { printf("下 "); count++; }

    if (count == 0) printf("无");
    printf(CLR_RESET "\n");

    /* 3. 遍历 NPC 列表并显示 */
    if (current_loc->room_npc[0] != NULL) {
        printf(CLR_CYAN "你注意到了这里还有：" CLR_RESET);
        printf(CLR_YELLOW CLR_BOLD);
        for (int i = 0; i < 3 && current_loc->room_npc[i] != NULL; i++) {
            printf("%s(%s) ", current_loc->room_npc[i]->room_npc_name, current_loc->room_npc[i]->room_npc_id);
        }
        printf(CLR_RESET "\n");
    }

    return 0;
}

/**
 * @brief 处理「talk」指令
 * 与当前房间内的 NPC 进行基本对话。
 */
int cmd_talk(int argc, char **argv) {
    if (argc == 1) {
        printf("你想和谁聊天？需要指定对方的身分 ID。\n");
        return 0;
    }

    /* 搜索房间内的 NPC 列表 */
    if (argc == 2) {
        int found = 0;
        for (int i = 0; i < 3 && current_loc->room_npc[i] != NULL; i++) {
            /* 匹配 NPC 的英文 ID */
            if (strcmp(argv[1], current_loc->room_npc[i]->room_npc_id) == 0) {
                printf(CLR_YELLOW CLR_BOLD "%s" CLR_RESET "对你说：\n", current_loc->room_npc[i]->room_npc_name);
                printf("%s\n", current_loc->room_npc[i]->room_npc_dialog);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("这里并没有叫「" CLR_YELLOW CLR_BOLD "%s" CLR_RESET "」的人，莫非你产生了幻听？\n", argv[1]);
        }
    }

    return 0;
}

/**
 * @brief 处理「see」指令
 * 详细观察（检视）房间内的 NPC。
 */
int cmd_see(int argc, char **argv) {
    if (argc == 1) {
        printf("你要仔细查看谁？\n");
        return 0;
    }

    if (argc == 2) {
        int found = 0;
        for (int i = 0; i < 3 && current_loc->room_npc[i] != NULL; i++) {
            if (strcmp(argv[1], current_loc->room_npc[i]->room_npc_id) == 0) {
                /* 打印 NPC 的背景或外貌描述 */
                printf("%s\n", current_loc->room_npc[i]->room_npc_desc);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("此处四下空旷，并没有所谓「" CLR_YELLOW CLR_BOLD "%s" CLR_RESET "」的踪影。\n", argv[1]);
        }
    }
    return 0;
}

/**
 * @brief 实现自定义的提示符输入
 */
char *parser_input(char *buf, int size) {
    /* 显示带有当前房间名称的交互提示符 */
    printf(CLR_GREEN CLR_BOLD "%s" CLR_RESET " > ", current_loc->room_name);

    if (fgets(buf, size, stdin)) {
        /* 去除输入行尾的换行符 \n */
        buf[strcspn(buf, "\n")] = 0;
        return buf;
    }

    return NULL;
}

/**
 * @brief 基于空格的词法拆分
 */
int parser_split(char *line, char **argv, int max_args) {
    int count = 0;

    /* 使用 strtok 进行切词 */
    char *token = strtok(line, " ");

    while (token != NULL && count < max_args) {
        argv[count] = token;
        count++;
        token = strtok(NULL, " ");
    }
    return count;
}

/**
 * @brief 核心分发逻辑
 * 遍历指令表，匹配用户输入的第一个单词并执行对应的函数。
 */
int parser_execute(int argc, char **argv) {
    if (argc == 0)
        return 0;

    for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
        if (strcmp(argv[0], cmd_table[i].cmd_name) == 0) {
            /* 找到匹配指令，调用对应的函数接口 */
            return cmd_table[i].cmd_func(argc, argv);
        }
    }
    
    /* 未匹配到任何指令时的默认反馈（增加仪式感） */
    printf("你对着空气自言自语，却始终没有人回应，周围只有山风在轻轻回响。\n");
    return 0;
}
