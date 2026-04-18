#include "parser.h"

typedef struct cmd_entry_e {
    const char     *cmd_name;
    command_handler cmd_func;
    const char     *cmd_desc;
    const char     *cmd_help;
} cmd_entry_t;

static cmd_entry_t cmd_table[] = {
    {"quit", cmd_quit, "退出游戏", "输入quit直接退出"},
    {"help", cmd_help, "帮助列表", "输入help [cmd]获取帮助"},
    {"go", cmd_go, "前往地点", "输入go [direction]前往某处"},
    {"look", cmd_look, "查看地点", "输入look看向某处"},
    {"talk", cmd_talk, "和某人对话", "输入talk [sb] [topic]和某人聊某事"},
    {"see", cmd_see, "查看某人", "输入see [sb] 检视某人"},
    {NULL, NULL, NULL},
};

int cmd_quit(int argc, char **argv) {
    printf("我们江湖有缘，再见！\n");
    return 1;
}

int cmd_help(int argc, char **argv) {

    // 只输入help
    if (argc == 1) {
        printf("指令大全\n");
        for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
            printf("%s \t %s \n", cmd_table[i].cmd_name, cmd_table[i].cmd_desc);
        }

        printf("使用help [cmd] 来获取使用教程\n");
    } else {
        int found = 0;
        for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
            if (strcmp(argv[1], cmd_table[i].cmd_name) == 0) {
                printf("%s\n", cmd_table[i].cmd_help);
                found = 1;
                break;
            }
        }

        if (!found)
            printf("未找到名为 %s 的命令\n", argv[1]);
    }
    return 0;
}

int cmd_go(int argc, char **argv) {
    if (argc < 2) {
        printf("你要去哪里？\n");
        printf("用法提示：输入 'go [方向]' (例如: go north)\n");
        return 0;
    }

    room_loc_t *next_loc = NULL;

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

    if (next_loc == NULL) {
        printf("前面没有路了，你看看其他地方。\n");
        return 0;
    }
    // 3. 真正执行移动
    if (strcmp(argv[1], "up") == 0)
        printf("你顺着粗糙的树干小心攀爬，借着枝节一点点向上移动。\n");
    else if (strcmp(argv[1], "down") == 0)
        printf("你弯下身子，顺着狭窄的洞口缓缓向下钻入，周围的光线一点点被黑暗吞没。\n");
    else
        printf("你向 %s 走去，脚下的路显得格外漫长...\n", argv[1]);
    sleep(1);

    current_loc = next_loc; // 更新玩家当前所在的指针地址！
    // 4. 到达新地点后自动“看一看”
    cmd_look(1, NULL);
    return 0;
}

int cmd_look(int argc, char **argv) {
    printf(CLR_GREEN CLR_BOLD "【%s】" CLR_RESET "\n", current_loc->room_name);
    printf("%s\n", current_loc->room_desc);

    // 2. 打印出口信息
    printf(CLR_CYAN "\n这里明显的出口有：" CLR_RESET);
    printf(CLR_BOLD CLR_YELLOW);
    int count = 0;
    if (current_loc->north) {
        printf("北 ");
        count++;
    }
    if (current_loc->south) {
        printf("南 ");
        count++;
    }
    if (current_loc->east) {
        printf("东 ");
        count++;
    }
    if (current_loc->west) {
        printf("西 ");
        count++;
    }
    if (current_loc->up) {
        printf("上 ");
        count++;
    }
    if (current_loc->down) {
        printf("下 ");
        count++;
    }
    if (count == 0) {
        printf("无");
    }
    printf("\n");
    printf(CLR_RESET);

    if (current_loc->room_npc[0] != NULL) {
        printf(CLR_CYAN "这里有：" CLR_RESET);
        printf(CLR_YELLOW CLR_BOLD);
        for (int i = 0; i < 3 && current_loc->room_npc[i] != NULL; i++) {
            printf("%s(%s)", current_loc->room_npc[i]->room_npc_name, current_loc->room_npc[i]->room_npc_id);
        }
        printf(CLR_RESET);
        printf("\n");
    }

    return 0;
}

int cmd_talk(int argc, char **argv) {
    if (argc == 1) {
        printf("和谁聊天？\n");
        return 0;
    }

    if (argc == 2) {
        int found = 0;
        for (int i = 0; i < 3 && current_loc->room_npc[i] != NULL; i++) {
            if (strcmp(argv[1], current_loc->room_npc[i]->room_npc_id) == 0) {
                printf(CLR_YELLOW CLR_BOLD "%s" CLR_RESET "对你说：\n", current_loc->room_npc[i]->room_npc_name);
                printf("%s\n", current_loc->room_npc[i]->room_npc_dialog);
                found = 1;
                break;
            }
        }
        if (!found) {

            printf("这里没有" CLR_YELLOW CLR_BOLD " %s " CLR_RESET "这个人\n", argv[1]);
            return 0;
        }
    }

    return 0;
}

int cmd_see(int argc, char **argv) {
    if (argc == 1) {
        printf("你要查看谁？\n");
        return 0;
    }

    if (argc == 2) {
        int found = 0;
        for (int i = 0; i < 3 && current_loc->room_npc[i]->room_npc_id != NULL; i++) {
            if (strcmp(argv[1], current_loc->room_npc[i]->room_npc_id) == 0) {
                printf("%s\n", current_loc->room_npc[i]->room_npc_desc);
                found = 1;
                break;
            }
        }
        if (!found) {
            printf("这里没有" CLR_YELLOW CLR_BOLD " %s " CLR_RESET "这个人\n", argv[1]);
            return 0;
        }
    }
    return 0;
}

char *parser_input(char *buf, int size) {
    printf(CLR_GREEN CLR_BOLD "%s" CLR_RESET " > ", current_loc->room_name);

    // 读取标准输入
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = 0;
        return buf;
    }

    return NULL;
}

int parser_split(char *line, char **argv, int max_args) {
    int count = 0;

    // 1. 分割第一个单词
    char *token = strtok(line, " ");

    // 2. 循环找出后续的
    while (token != NULL && count < max_args) {
        argv[count] = token;
        count++;

        token = strtok(NULL, " ");
    }
    return count;
}

int parser_execute(int argc, char **argv) {

    if (argc == 0)
        return 0;
    for (int i = 0; cmd_table[i].cmd_name != NULL; i++) {
        if (strcmp(argv[0], cmd_table[i].cmd_name) == 0) {
            return cmd_table[i].cmd_func(argc, argv);
        }
    }
    printf("你自言自语中，不知道在说什么。\n");
    return 0;
}
