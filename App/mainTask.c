#include "mainTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

#include "drv_dma2d.h"
#include "tim.h"

VOID mainTask(ULONG id)
{
    (void)id;

    RTT_LOG_INFO("hello world, pi = %f", 3.1415926);

    uart_printf(&huart1, "hello world, pi = %f", 3.1415926);

    FillRect(0, 0, 800, 480, 0); // 清屏

    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
    TIM12->CCR1 = 100;

    FillRect(100, 100, 600, 280, 0xFFFFFFFF);

    while (1)
    {
        tx_thread_sleep(1000);
    }
}
