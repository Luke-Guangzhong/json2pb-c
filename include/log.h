#ifndef __LOG_H__
#define __LOG_H__

#include <libgen.h>
#include <stdio.h>
#include <string.h>

#define LOG_CLRSTR_NONE      "\033[0m"       // 默认显示
#define LOG_CLRSTR_RED       "\033[0;91m"    // 亮红色
#define LOG_CLRSTR_GREEN     "\033[0;32m"    // 绿色
#define LOG_CLRSTR_BLUE      "\033[0;32;34m" // 蓝色
#define LOG_CLRSTR_DARK_GRAY "\033[1;30m"    // 灰色
#define LOG_CLRSTR_CYAN      "\033[0;36m"    // 青色
#define LOG_CLRSTR_PURPLE    "\033[0;35m"    // 紫色
#define LOG_CLRSTR_BROWN     "\033[0;33m"    // 棕色
#define LOG_CLRSTR_YELLOW    "\033[5;42;33m" // 绿黄色
#define LOG_CLRSTR_WHITE     "\033[1;37m"    // 白色

#define INFO(format, args...)        \
    fprintf(stdout, format, ##args); \
    fflush(stdout)

#define ERROR(format, args...)                                                    \
    fprintf(stderr,                                                               \
            LOG_CLRSTR_NONE LOG_CLRSTR_RED "[ERROR]"                              \
                                           "[%s][%s][%d]" LOG_CLRSTR_NONE format, \
            basename(__FILE__), __FUNCTION__, __LINE__, ##args);                  \
    fflush(stderr)

#ifdef DEBUG_ENABLE
#define DEBUG(format, args...)                                             \
    fprintf(stdout,                                                        \
            LOG_CLRSTR_NONE LOG_CLRSTR_BLUE "[DEBUG]"                      \
                                            "[%s]" LOG_CLRSTR_NONE format, \
            __FUNCTION__, ##args);                                         \
    fflush(stdout)
#else
#define DEBUG(format, args...)
#endif

#define FUNCTION_ENTER() DEBUG("enter!\n")
#define FUNCTION_EXIT()  DEBUG("exit!\n")

#endif