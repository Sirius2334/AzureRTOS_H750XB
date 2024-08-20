#include "initTask.h"
#include "RTT_LOG.h"
#include "drv_sdram.h"
#include "drv_uart.h"

void initTask(void)
{
    sdram_init();
    rtt_init();
    uart_init(&huart1);
}
