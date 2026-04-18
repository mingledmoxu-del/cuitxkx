#include <stdio.h>
#include <string.h>

#include "parser.h"

int   parser_argc = 0;
char *parser_argv[5];
char  parser_buf[64];

int main() {

    printf("欢迎来到cuitxkx，输入 'quit' 退出\n");

    while (1) {
        if (parser_input(parser_buf, sizeof(parser_buf)) == NULL)
            break;
        parser_argc = parser_split(parser_buf, parser_argv, 5);
        if (parser_execute(parser_argc, parser_argv))
            break;
    }

    return 0;
}