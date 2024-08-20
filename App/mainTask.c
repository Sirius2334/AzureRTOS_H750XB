#include "mainTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

VOID mainTask(ULONG id)
{
    (void)id;

    RTT_LOG_INFO("hello world, pi = %f", 3.1415926);

    uart_printf(&huart1, "hello world, pi = %f", 3.1415926);

    while (1)
    {
        tx_thread_sleep(1000);
    }
}
