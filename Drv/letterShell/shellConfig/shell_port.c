#include "shell.h"
#include "shell_port.h"

#define SHELL_BUFF_SIZE 512

Shell shell;
static char shellBuffer[SHELL_BUFF_SIZE];

extern TX_MUTEX shellMutex;

/**
 * @brief 用户shell写
 *
 * @param data 数据
 * @param len 数据长度
 *
 * @return short 实际写入的数据长度
 */
short userShellWrite(char *data, unsigned short len)
{
    uint32_t sendLen = 0;
    do
    {
        sendLen += uart_send_dma(&huart1, (uint8_t *)(data + sendLen), len - sendLen);
    } while (sendLen < len);
    // sendLen = SEGGER_RTT_Write(0, data, len);
    return sendLen;
}

/**
 * @brief 用户shell读
 *
 * @param data 数据
 * @param len 数据长度
 *
 * @return short 实际读取到
 */
short userShellRead(char *data, unsigned short len)
{
    (void)data;
    (void)len;
    // if (SEGGER_RTT_Read(0, data, 1))
    //     return 1;
    return 0;
}

/**
 * @brief 用户shell上锁
 *
 * @param shell shell
 *
 * @return int 0
 */
int userShellLock(Shell *shell)
{
    (void)shell;
    tx_mutex_get(&shellMutex, TX_WAIT_FOREVER);
    return 0;
}

/**
 * @brief 用户shell解锁
 *
 * @param shell shell
 *
 * @return int 0
 */
int userShellUnlock(Shell *shell)
{
    (void)shell;
    tx_mutex_put(&shellMutex);
    return 0;
}

/**
 * @brief 用户shell初始化
 *
 */
void userShellInit(Shell *shell)
{
    shell->write = userShellWrite;
    shell->read = userShellRead;
    // shell->lock = userShellLock;
    // shell->unlock = userShellUnlock;

    shellInit(shell, shellBuffer, SHELL_BUFF_SIZE);
}
