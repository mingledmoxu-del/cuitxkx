#ifndef PARSER_H
#define PARSER_H

#include <util.h>

// 命令处理统一接口
typedef int (*command_handler)(int argc, char **argv);

// 命令结构体
typedef struct cmd_entry_e cmd_entry_t;

int cmd_quit(int argc, char **argv);
int cmd_help(int argc, char **argv);
int cmd_go(int argc, char **argv);

char *parser_input(char *buf, int size);
int   parser_split(char *line, char **argv, int max_args);
int   parser_execute(int argc, char **argv);

#endif