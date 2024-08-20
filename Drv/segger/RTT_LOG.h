#ifndef __RTT_LOG_H_
#define __RTT_LOG_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "SEGGER_RTT.h"
#include "RTT_LOG_Conf.h"

#if RTT_LOG_ENABLE
#if LIBC_PRINTF
#define RTT_LOG_PRINT(format, ...) rtt_printf(RTT_LOG_TERMINAL, format, ##__VA_ARGS__)
#else
#define RTT_LOG_PRINT(format, ...) SEGGER_RTT_printf(RTT_LOG_TERMINAL, format, ##__VA_ARGS__)
#endif
#else
#define RTT_LOG_PRINT(format, ...)
#endif

typedef enum
{
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
} LOG_LEVEL;

// 去掉__FILE__中的路径
#define GET_FILE_NAME(path) strrchr(path, PATH_SEPARATOR) ? strrchr(path, PATH_SEPARATOR) + 1 : path

// 调试log输出 青色
#define RTT_LOG_DEBUG(format, ...)                                                                                                                         \
    if (RTT_LOG_LEVEL <= LOG_LEVEL_DEBUG)                                                                                                                  \
    {                                                                                                                                                      \
        RTT_LOG_PRINT(RTT_LOG_COLOR_DEBUG "[DEBUG] %s:%d: " RTT_CTRL_RESET format RTT_LOG_NEWLINE_SIGN, GET_FILE_NAME(__FILE__), __LINE__, ##__VA_ARGS__); \
    }

// 信息log输出 绿色
#define RTT_LOG_INFO(format, ...)                                                                                                                        \
    if (RTT_LOG_LEVEL <= LOG_LEVEL_INFO)                                                                                                                 \
    {                                                                                                                                                    \
        RTT_LOG_PRINT(RTT_LOG_COLOR_INFO "[INFO] %s:%d: " RTT_CTRL_RESET format RTT_LOG_NEWLINE_SIGN, GET_FILE_NAME(__FILE__), __LINE__, ##__VA_ARGS__); \
    }

// 警告log输出 黄色
#define RTT_LOG_WARN(format, ...)                                                                                                                        \
    if (RTT_LOG_LEVEL <= LOG_LEVEL_WARN)                                                                                                                 \
    {                                                                                                                                                    \
        RTT_LOG_PRINT(RTT_LOG_COLOR_WARN "[WARN] %s:%d: " RTT_CTRL_RESET format RTT_LOG_NEWLINE_SIGN, GET_FILE_NAME(__FILE__), __LINE__, ##__VA_ARGS__); \
    }

// 错误log输出 玫红色
#define RTT_LOG_ERROR(format, ...)                                                                                                                         \
    if (RTT_LOG_LEVEL <= LOG_LEVEL_ERROR)                                                                                                                  \
    {                                                                                                                                                      \
        RTT_LOG_PRINT(RTT_LOG_COLOR_ERROR "[ERROR] %s:%d: " RTT_CTRL_RESET format RTT_LOG_NEWLINE_SIGN, GET_FILE_NAME(__FILE__), __LINE__, ##__VA_ARGS__); \
    }

void rtt_init(void);
void rtt_printf(uint8_t BufferIndex, const char *fmt, ...);

#endif