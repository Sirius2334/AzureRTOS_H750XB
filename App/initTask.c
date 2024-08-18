#include "initTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

void initTask(void)
{
    rtt_init();
    uart_init(&huart1);
}
