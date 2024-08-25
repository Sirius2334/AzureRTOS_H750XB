#include "mainTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

#include "drv_dma2d.h"
#include "tim.h"

#include "sdmmc.h"

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

    // lv_init();
    // lv_port_disp_init();

    // lv_demo_stress();
    // lv_demo_widgets();
    // lv_demo_benchmark();

    HAL_SD_CardInfoTypeDef SDCardInfo; // SD卡信息结构体
    // 得到卡的信息并输出到串口1
    HAL_SD_GetCardInfo(&hsd1, &SDCardInfo);                                                    // HAL_SD_CardInfoTypeDef *pCardInfo);//获取SD卡信息
    uart_printf(&huart1, "Card Type:%d\r\n", SDCardInfo.CardType);                             // 卡类型
    uart_printf(&huart1, "Card CardVersion:%d\r\n", SDCardInfo.CardVersion);                   // CardVersion
    uart_printf(&huart1, "Card RCA:%d\r\n", SDCardInfo.RelCardAdd);                            // 卡相对地址
    uart_printf(&huart1, "Card Capacity:%d MB\r\n", (uint32_t)(SDCardInfo.LogBlockNbr) >> 11); // 显示容量
    uart_printf(&huart1, "Card BlockSize:%d\r\n\r\n", SDCardInfo.LogBlockSize);                // 显示块大小

    uint16_t sd_size;
    uint8_t buf[512], rd_buf[512];

    /* sd card write test */
    for (sd_size = 0; sd_size < 512; sd_size++)
        buf[sd_size] = sd_size; // 给扇区6写数据

    HAL_SD_WriteBlocks(&hsd1, buf, 512 * 6, 1, 0xff);
    uart_printf(&huart1, "\r\nWrite DATA ENDED\r\n");

    // 读取扇区6的内容
    /* sd card read test */
    if (HAL_SD_ReadBlocks(&hsd1, rd_buf, 512 * 6, 1, 0xff) == HAL_OK)
    {
        uart_printf(&huart1, "SECTOR 6  DATA:\r\n");
        for (sd_size = 0; sd_size < 512; sd_size++)
        {
            uart_printf(&huart1, "0x%02x ", rd_buf[sd_size]); // 打印扇区数据
            tx_thread_sleep(1);
        }
        uart_printf(&huart1, "\r\nRead DATA ENDED\r\n");
    }

    /* sd card erase test */
    HAL_SD_Erase(&hsd1, 6, 6);
    uart_printf(&huart1, "\r\nErase DATA ENDED\r\n");
    tx_thread_sleep(1000);

    memset(rd_buf, 0, 512);
    /* sd card read test */
    if (HAL_SD_ReadBlocks(&hsd1, rd_buf, 512 * 6, 1, 0xff) == HAL_OK)
    {
        uart_printf(&huart1, "SECTOR 6  DATA:\r\n");
        for (sd_size = 0; sd_size < 512; sd_size++)
        {
            uart_printf(&huart1, "0x%02x ", rd_buf[sd_size]); // 打印扇区数据
            tx_thread_sleep(1);
        }
        uart_printf(&huart1, "\r\nRead DATA ENDED\r\n");
    }
    else
        uart_printf(&huart1, "SECTOR 6 read failed.\r\n");

    while (1)
    {
        // lv_tick_inc(5);
        // lv_task_handler();
        tx_thread_sleep(5);
    }
}
