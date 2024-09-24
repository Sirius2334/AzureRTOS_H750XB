#include "mainTask.h"
#include "RTT_LOG.h"
#include "drv_uart.h"

#include "drv_dma2d.h"
#include "tim.h"

#include "sdmmc.h"
#include "jpeg.h"
#include "decode_dma.h"

#include "lvgl.h"
#include "porting/lv_port_disp.h"
#include "demos/stress/lv_demo_stress.h"
#include "demos/widgets/lv_demo_widgets.h"
#include "demos/benchmark/lv_demo_benchmark.h"

#include "arm_math.h"

#include "ux_device_cdc_acm.h"

#define JPEG_OUTPUT_BUFFER_ADDR (((uint32_t *)0xC0000000) + 800 * 480 * 4)
#define CHUNK_SIZE_IN ((uint32_t)(64 * 1024))  // 单次解码输入数据最大长度
#define CHUNK_SIZE_OUT ((uint32_t)(64 * 1024)) // 单次解码输出数据最大长度

void bsp_InitDWT(void);
static void DSP_Sine(void);

__ASM(".global jpeg_start, jpeg_end;"
      // "jpeg_start: .incbin \"../ltdc.jpg\";"
      "jpeg_start: "
      // ".incbin \"../ltdc.bmp\";"
      ".incbin \"../ltdc.jpg\";"
      "jpeg_end: ");

extern unsigned int jpeg_start, jpeg_end;
extern __IO uint32_t Jpeg_HWDecodingEnd;

UX_SLAVE_CLASS_CDC_ACM *cdc_acm;
uint8_t UserRxBuffer[16];
const uint8_t UserTxMessage[] = "MY CDC CLASS IS RUNNING!\r\n";

typedef struct __attribute__((__packed__)) BITMAPFILEHEADER /* size: 40 */
{
    uint16_t bfType;      // 文件的类型，该值必需是0x4D42，也就是字符'BM'。
    uint32_t bfSize;      // 位图文件的大小，用字节为单位
    uint16_t bfReserved1; // 保留，必须设置为0
    uint16_t bfReserved2; // 保留，必须设置为0
    uint32_t bfOffBits;   // 位图数据距离文件开头偏移量，用字节为单位
} BITMAP_FILE_HEADER;

void jpeg_test(void)
{
    uint8_t *jpeg_start_addr = (uint8_t *)&jpeg_start;
    uint8_t *jpeg_end_addr = (uint8_t *)&jpeg_end;
    uint32_t jpeg_length = jpeg_end_addr - jpeg_start_addr;

    uint32_t xPos = 0, yPos = 0;

    RTT_LOG_INFO("jpeg_start_addr = 0x%08x, jpeg_end_addr = 0x%08x, length = %d", jpeg_start_addr, jpeg_end_addr, jpeg_length);

    /* JPEG decoding with DMA (Not Blocking ) Method */
    JPEG_Decode_DMA(&hjpeg, (uint32_t)jpeg_start_addr, jpeg_length, (uint32_t)0xC0200000);
    // JPEG_Decode_DMA(&hjpeg, (uint32_t)image_320_240_jpg, IMAGE_320_240_JPG_SIZE, (uint32_t)0xC0200000);

    /* Wait till end of JPEG decoding */
    while (Jpeg_HWDecodingEnd == 0)
        ;

    /* Copy RGB decoded Data to the display FrameBuffer */
    xPos = (800 - hjpeg.Conf.ImageWidth) / 2;
    yPos = (480 - hjpeg.Conf.ImageHeight) / 2;
    RTT_LOG_INFO("ColorSpace = %d, ChromaSubsampling = %d, ImageWidth = %d, ImageHeight = %d, ImageQuality = %d",
                 hjpeg.Conf.ColorSpace,
                 hjpeg.Conf.ChromaSubsampling,
                 hjpeg.Conf.ImageWidth,
                 hjpeg.Conf.ImageHeight,
                 hjpeg.Conf.ImageQuality);

    DMA2D_Copy_YCbCr_To_RGB((uint32_t *)0xC0200000, (uint32_t *)0xC0000000, xPos, yPos, hjpeg.Conf.ImageWidth, hjpeg.Conf.ImageHeight, LTDC_PIXEL_FORMAT_ARGB8888, hjpeg.Conf.ChromaSubsampling);
}

BITMAP_FILE_HEADER *pbitmap_header = (BITMAP_FILE_HEADER *)&jpeg_start;

VOID mainTask(ULONG id)
{
    (void)id;

    RTT_LOG_INFO("hello world, pi = %f", 3.1415926);

    uart_printf(&huart1, "hello world, pi = %f", 3.1415926);

    FillRect(0, 0, 800, 480, 0); // 清屏

    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_1);
    TIM12->CCR1 = 30;

    FillRect(0, 0, 100, 480, 0xFFFF0000);
    FillRect(100, 0, 100, 480, 0xFFFFFF00);
    FillRect(200, 0, 100, 480, 0xFF00FF00);
    FillRect(300, 0, 100, 480, 0xFF00FFFF);
    FillRect(400, 0, 100, 480, 0xFF0000FF);
    FillRect(500, 0, 100, 480, 0xFFFF00FF);
    FillRect(600, 0, 100, 480, 0xFFFFFFFF);
    FillRect(700, 0, 100, 480, 0xFF000000);

    jpeg_test();

    RTT_LOG_INFO("type = 0x%04X, size = %d Bytes, offset = %d.",
                 (uint16_t)pbitmap_header->bfType,
                 (uint32_t)pbitmap_header->bfSize,
                 (uint32_t)pbitmap_header->bfOffBits);

    uint8_t *jpeg_start_addr = (uint8_t *)&jpeg_start;
    uint8_t *jpeg_end_addr = (uint8_t *)&jpeg_end;
    uint32_t jpeg_length = jpeg_end_addr - jpeg_start_addr;
    RTT_LOG_INFO("jpeg_start_addr = 0x%08x, jpeg_end_addr = 0x%08x, length = %d", jpeg_start_addr, jpeg_end_addr, jpeg_length);

    // DMA2D_MemCopy((uint8_t *)&jpeg_start + 52, (uint32_t *)0xC0000000, 480, 480, 0, 800 - 480, LTDC_PIXEL_FORMAT_ARGB8888);

    // lv_init();
    // lv_port_disp_init();

    // lv_demo_stress();
    // lv_demo_widgets();
    // lv_demo_benchmark();

    // HAL_SD_CardInfoTypeDef SDCardInfo; // SD卡信息结构体
    // // 得到卡的信息并输出到串口1
    // HAL_SD_GetCardInfo(&hsd1, &SDCardInfo);                                                    // HAL_SD_CardInfoTypeDef *pCardInfo);//获取SD卡信息
    // uart_printf(&huart1, "Card Type:%d\r\n", SDCardInfo.CardType);                             // 卡类型
    // uart_printf(&huart1, "Card CardVersion:%d\r\n", SDCardInfo.CardVersion);                   // CardVersion
    // uart_printf(&huart1, "Card RCA:%d\r\n", SDCardInfo.RelCardAdd);                            // 卡相对地址
    // uart_printf(&huart1, "Card Capacity:%d MB\r\n", (uint32_t)(SDCardInfo.LogBlockNbr) >> 11); // 显示容量
    // uart_printf(&huart1, "Card BlockSize:%d\r\n\r\n", SDCardInfo.LogBlockSize);                // 显示块大小

    // uint16_t sd_size;
    // uint8_t buf[512], rd_buf[512];

    // /* sd card write test */
    // for (sd_size = 0; sd_size < 512; sd_size++)
    //     buf[sd_size] = sd_size; // 给扇区6写数据

    // HAL_SD_WriteBlocks(&hsd1, buf, 512 * 6, 1, 0xff);
    // uart_printf(&huart1, "\r\nWrite DATA ENDED\r\n");

    // // 读取扇区6的内容
    // /* sd card read test */
    // if (HAL_SD_ReadBlocks(&hsd1, rd_buf, 512 * 6, 1, 0xff) == HAL_OK)
    // {
    //     uart_printf(&huart1, "SECTOR 6  DATA:\r\n");
    //     for (sd_size = 0; sd_size < 512; sd_size++)
    //     {
    //         uart_printf(&huart1, "0x%02x ", rd_buf[sd_size]); // 打印扇区数据
    //         tx_thread_sleep(1);
    //     }
    //     uart_printf(&huart1, "\r\nRead DATA ENDED\r\n");
    // }

    // /* sd card erase test */
    // HAL_SD_Erase(&hsd1, 6, 6);
    // uart_printf(&huart1, "\r\nErase DATA ENDED\r\n");
    // tx_thread_sleep(1000);

    // memset(rd_buf, 0, 512);
    // /* sd card read test */
    // if (HAL_SD_ReadBlocks(&hsd1, rd_buf, 512 * 6, 1, 0xff) == HAL_OK)
    // {
    //     uart_printf(&huart1, "SECTOR 6  DATA:\r\n");
    //     for (sd_size = 0; sd_size < 512; sd_size++)
    //     {
    //         uart_printf(&huart1, "0x%02x ", rd_buf[sd_size]); // 打印扇区数据
    //         tx_thread_sleep(1);
    //     }
    //     uart_printf(&huart1, "\r\nRead DATA ENDED\r\n");
    // }
    // else
    //     uart_printf(&huart1, "SECTOR 6 read failed.\r\n");

    /* Local Variables */
    ULONG actual_length;
    UX_SLAVE_DEVICE *device;
    device = &_ux_system_slave->ux_system_slave_device;

    bsp_InitDWT();

    while (1)
    {
        /* Check if device is configured */
        if ((device->ux_slave_device_state == UX_DEVICE_CONFIGURED) && (cdc_acm != UX_NULL))
        {
            ux_device_class_cdc_acm_write(cdc_acm, (UCHAR *)UserTxMessage, sizeof(UserTxMessage) - 1, &actual_length);
        }

        // lv_tick_inc(5);
        // lv_task_handler();
        tx_thread_sleep(1000);
        // DSP_Sine();
    }
}

/* 开关全局中断的宏 */
#define ENABLE_INT() __set_PRIMASK(0)  /* 使能全局中断 */
#define DISABLE_INT() __set_PRIMASK(1) /* 禁止全局中断 */

/* 宏定义 */
#define DWT_CYCCNT *(volatile unsigned int *)0xE0001004
#define DWT_CR *(volatile unsigned int *)0xE0001000
#define DEM_CR *(volatile unsigned int *)0xE000EDFC
#define DBGMCU_CR *(volatile unsigned int *)0xE0042004

#define DEM_CR_TRCENA (1 << 24)
#define DWT_CR_CYCCNTENA (1 << 0)

void bsp_InitDWT(void)
{
    DEM_CR |= (unsigned int)DEM_CR_TRCENA;
    DWT_CYCCNT = (unsigned int)0u;
    DWT_CR |= (unsigned int)DWT_CR_CYCCNTENA;
}

float32_t src[10] = {3.141593f,
                     1.570796f,
                     1.047198f,
                     0.785398f,
                     0.628319f,
                     0.523599f,
                     0.448799f,
                     0.392699f,
                     0.349066f,
                     0.314159f};
float32_t res[10] = {0.0f};

static void DSP_Sine(void)
{
    uint16_t i;
    uint32_t start, end, cnt;

    DISABLE_INT();
    start = DWT_CYCCNT;
    res[0] = arm_sin_f32(src[0]);
    res[1] = arm_sin_f32(src[1]);
    res[2] = arm_sin_f32(src[2]);
    res[3] = arm_sin_f32(src[3]);
    res[4] = arm_sin_f32(src[4]);
    res[5] = arm_sin_f32(src[5]);
    res[6] = arm_sin_f32(src[6]);
    res[7] = arm_sin_f32(src[7]);
    res[8] = arm_sin_f32(src[8]);
    res[9] = arm_sin_f32(src[9]);
    end = DWT_CYCCNT;
    cnt = end - start;
    ENABLE_INT();

    for (i = 0; i < 10; i++)
    {
        RTT_LOG_INFO("%f", res[i]);
    }

    RTT_LOG_INFO("----arm_sin_f32 = %d\r\n", cnt / 10);

    DISABLE_INT();
    start = DWT_CYCCNT;
    res[0] = sinf(src[0]);
    res[1] = sinf(src[1]);
    res[2] = sinf(src[2]);
    res[3] = sinf(src[3]);
    res[4] = sinf(src[4]);
    res[5] = sinf(src[5]);
    res[6] = sinf(src[6]);
    res[7] = sinf(src[7]);
    res[8] = sinf(src[8]);
    res[9] = sinf(src[9]);
    end = DWT_CYCCNT;
    cnt = end - start;
    ENABLE_INT();

    for (i = 0; i < 10; i++)
    {
        RTT_LOG_INFO("%f", res[i]);
    }

    RTT_LOG_INFO("----CMake SIN = %d\r\n", cnt / 10);
}
