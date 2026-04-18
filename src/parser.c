#include "parser.h"

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
