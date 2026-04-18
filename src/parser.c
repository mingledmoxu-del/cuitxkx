#include "parser.h"

typedef struct cmd_entry_e {
    const char     *name;
    command_handler func;
    const char     *desc;
    const char     *help;
} cmd_entry_t;

static cmd_entry_t cmd_table[] = {
    {"quit", cmd_quit, "退出游戏", "输入quit直接退出"},
    {"help", cmd_help, "帮助列表", "输入help [cmd]获取帮助"},
    {"go", cmd_go, "前往地点", "输入go [direction]前往某处"},
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
        for (int i = 0; cmd_table[i].name != NULL; i++) {
            printf("%s \t %s \n", cmd_table[i].name, cmd_table[i].desc);
        }

        printf("使用help [cmd] 来获取使用教程\n");
    } else {
        int found = 0;
        for (int i = 0; cmd_table[i].name != NULL; i++) {
            if (strcmp(argv[1], cmd_table[i].name) == 0) {
                printf("%s\n", cmd_table[i].help);
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
    } else {
        printf("你向 %s 走去，脚下的路显得格外漫长...\n", argv[1]);
        sleep(1);
        printf("你来到了 xxx \n");
    }
    return 0;
}

char *parser_input(char *buf, int size) {
    printf("> ");

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
    for (int i = 0; cmd_table[i].name != NULL; i++) {
        if (strcmp(argv[0], cmd_table[i].name) == 0) {
            return cmd_table[i].func(argc, argv);
        }
    }
    printf("你自言自语中，不知道在说什么。\n");
    return 0;
}
