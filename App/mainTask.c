#include "mainTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

#include "tim.h"

VOID mainTask(ULONG id)
{
    (void)id;

    RTT_LOG_INFO("hello world, pi = %f", 3.1415926);

    uart_printf(&huart1, "hello world, pi = %f", 3.1415926);

    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
    TIM12->CCR1 = 100;

    while (1)
    {
        tx_thread_sleep(1000);
    }
}
