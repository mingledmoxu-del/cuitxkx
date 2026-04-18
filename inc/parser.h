#ifndef PARSER_H
#define PARSER_H

#include <util.h>

char *parser_input(char *buf, int size);
int   parser_split(char *line, char **argv, int max_args);

#endif