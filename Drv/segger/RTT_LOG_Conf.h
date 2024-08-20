#ifndef __RTT_LOG_CONF_H_
#define __RTT_LOG_CONF_H_

/**
 * @brief segger rtt buffer size
 * @note UP是上行, 即输出; DOWN是下行, 即输入
 *
 * @note SEGGER_RTT_Printf的缓存大小
 *
 * @note 上行通道数量, 下行通道数量
 */
#define BUFFER_SIZE_UP (1024)
#define BUFFER_SIZE_DOWN (16)
#define SEGGER_RTT_PRINTF_BUFFER_SIZE (64)
#define SEGGER_RTT_MAX_NUM_UP_BUFFERS (3)
#define SEGGER_RTT_MAX_NUM_DOWN_BUFFERS (3)

/**
 * @brief Use micro printf lib
 *
 * @note 如果其余打印使用了这个打印库, 使能该选项会减少约2KB Flash,
 *       如果仅使用RTT打印浮点数, 关掉此选项可以减少约10KB Flash
 */
#define LIBC_PRINTF 1

/**
 * @brief Enable segger rtt log
 */
#define RTT_LOG_ENABLE 1

/**
 * @brief RTT LOG 使用的通道
 */
#define RTT_LOG_TERMINAL (0)

/**
 * @brief 选择LOG等级 低于此等级的LOG不会打印
 */
#define RTT_LOG_LEVEL LOG_LEVEL_DEBUG

/**
 * @brief RTT LOG使用的换行符
 */
#define RTT_LOG_NEWLINE_SIGN "\r\n"

/**
 * @brief RTT LOG不同等级的颜色
 */
#define RTT_LOG_COLOR_DEBUG RTT_CTRL_TEXT_CYAN
#define RTT_LOG_COLOR_INFO RTT_CTRL_TEXT_GREEN
#define RTT_LOG_COLOR_WARN RTT_CTRL_TEXT_YELLOW
#define RTT_LOG_COLOR_ERROR RTT_CTRL_TEXT_RED

/**
 * @brief 地址分隔符  Linux中是正斜杠'/'  Windows中是反斜杠'\'
 * @note 输出__FILE__文件名时可能带有很长的路径 只需要文件名就行了
 */
#define PATH_SEPARATOR '/'

#endif