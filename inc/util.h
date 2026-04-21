#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

/**
 * @brief ANSI 控制台颜色宏定义
 * 用于在支持 ANSI 序列的终端中输出彩色文字。
 * 用法示例: printf(CLR_RED "这是红色的字" CLR_RESET "\n");
 */

/* --- 基础前景颜色 (文字颜色) --- */
#define CLR_RESET   "\033[0m"  /* 重置所有样式 */
#define CLR_BLACK   "\033[30m" /* 黑色 */
#define CLR_RED     "\033[31m" /* 红色 */
#define CLR_GREEN   "\033[32m" /* 绿色 */
#define CLR_YELLOW  "\033[33m" /* 黄色 */
#define CLR_BLUE    "\033[34m" /* 蓝色 */
#define CLR_MAGENTA "\033[35m" /* 洋红色/紫色 */
#define CLR_CYAN    "\033[36m" /* 青色/天蓝色 */
#define CLR_WHITE   "\033[37m" /* 白色 */

/* --- 文本格式属性 --- */
#define CLR_BOLD      "\033[1m" /* 加粗或高亮显示 */
#define CLR_DIM       "\033[2m" /* 昏暗显色（部分终端支持） */
#define CLR_UNDERLINE "\033[4m" /* 下划线 */
#define CLR_BLINK     "\033[5m" /* 慢速闪烁 */
#define CLR_REVERSE   "\033[7m" /* 颜色反转（背景与前景对换） */
#define CLR_HIDDEN    "\033[8m" /* 消隐 */

#endif