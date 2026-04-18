#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 定义颜色宏
#define CLR_RESET   "\033[0m"
#define CLR_BLACK   "\033[30m"
#define CLR_RED     "\033[31m"
#define CLR_GREEN   "\033[32m"
#define CLR_YELLOW  "\033[33m"
#define CLR_BLUE    "\033[34m"
#define CLR_MAGENTA "\033[35m"
#define CLR_CYAN    "\033[36m"
#define CLR_WHITE   "\033[37m"

#define CLR_BOLD      "\033[1m"
#define CLR_DIM       "\033[2m"
#define CLR_UNDERLINE "\033[4m"
#define CLR_BLINK     "\033[5m"
#define CLR_REVERSE   "\033[7m"
#define CLR_HIDDEN    "\033[8m"

#endif