#include "mainTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

#include "drv_dma2d.h"
#include "tim.h"

#include "lvgl.h"
#include "porting/lv_port_disp.h"
#include "demos/stress/lv_demo_stress.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "demos/benchmark/lv_demo_benchmark.h"

VOID mainTask(ULONG id)
{
    (void)id;

    RTT_LOG_INFO("hello world, pi = %f", 3.1415926);

    uart_printf(&huart1, "hello world, pi = %f", 3.1415926);

    FillRect(0, 0, 800, 480, 0); // 清屏

    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
    TIM12->CCR1 = 30;

    FillRect(100, 100, 600, 280, 0xFFFFFFFF);

    lv_init();
    lv_port_disp_init();

    lv_demo_stress();
    // lv_demo_widgets();
    // lv_demo_benchmark();

    while (1)
    {
        lv_tick_inc(5);
        lv_task_handler();
        tx_thread_sleep(5);
    }
}
