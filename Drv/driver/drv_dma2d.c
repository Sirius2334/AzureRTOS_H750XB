#include "drv_dma2d.h"

static inline void DMA2D_Fill(void *pDst, uint32_t width, uint32_t height, uint32_t lineOff, uint32_t pixelFormat, uint32_t color)
{
    /* DMA2D配置 */
    DMA2D->CR = 0x00030000UL;                                // 配置为寄存器到储存器模式
    DMA2D->OCOLR = color;                                    // 设置填充使用的颜色，格式应该与设置的颜色格式相同
    DMA2D->OMAR = (uint32_t)pDst;                            // 填充区域的起始内存地址
    DMA2D->OOR = lineOff;                                    // 行偏移，即跳过的像素，注意是以像素为单位
    DMA2D->OPFCCR = pixelFormat;                             // 设置颜色格式
    DMA2D->NLR = (uint32_t)(width << 16) | (uint16_t)height; // 设置填充区域的宽和高，单位是像素

    /* 启动传输 */
    DMA2D->CR |= DMA2D_CR_START;

    /* 等待DMA2D传输完成 */
    while (DMA2D->CR & DMA2D_CR_START)
        ;
}

void DMA2D_MemCopy(void *pSrc, void *pDst, int width, int height, int OffLineSrc, int OffLineDst, uint32_t pixelFormat)
{
    /* DMA2D配置 */
    DMA2D->CR = 0x00000000UL;
    DMA2D->FGMAR = (uint32_t)pSrc;
    DMA2D->OMAR = (uint32_t)pDst;
    DMA2D->FGOR = OffLineSrc;
    DMA2D->OOR = OffLineDst;
    DMA2D->FGPFCCR = pixelFormat;
    DMA2D->NLR = (uint32_t)(width << 16) | (uint16_t)height;
    // DMA2D->OPFCCR |= DMA2D_OPFCCR_RBS;

    /* 启动传输 */
    DMA2D->CR |= DMA2D_CR_START;

    /* 等待DMA2D传输完成 */
    while (DMA2D->CR & DMA2D_CR_START)
        ;
}

void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color)
{
    void *pDist = ((uint32_t *)0xC0000000 + y * 800 + x);
    DMA2D_Fill(pDist, w, h, 800 - w, LTDC_PIXEL_FORMAT_ARGB8888, color);
}

void DMA2D_Copy_YCbCr_To_RGB(uint32_t *pSrc,
                             uint32_t *pDst,
                             uint16_t x,
                             uint16_t y,
                             uint16_t xsize,
                             uint16_t ysize,
                             uint32_t PixelFormat,
                             uint32_t ChromaSampling)
{
    uint32_t cssMode = DMA2D_CSS_420;
    uint32_t inputLineOffset = 0;
    uint32_t destination = 0;

    /* 处理输入行偏移 */
    if (ChromaSampling == JPEG_420_SUBSAMPLING)
    {
        cssMode = DMA2D_CSS_420;

        inputLineOffset = xsize % 16;
        if (inputLineOffset != 0)
        {
            inputLineOffset = 16 - inputLineOffset;
        }
    }
    else if (ChromaSampling == JPEG_444_SUBSAMPLING)
    {
        cssMode = DMA2D_NO_CSS;

        inputLineOffset = xsize % 8;
        if (inputLineOffset != 0)
        {
            inputLineOffset = 8 - inputLineOffset;
        }
    }
    else if (ChromaSampling == JPEG_422_SUBSAMPLING)
    {
        cssMode = DMA2D_CSS_422;

        inputLineOffset = xsize % 16;
        if (inputLineOffset != 0)
        {
            inputLineOffset = 16 - inputLineOffset;
        }
    }

    /* 输出地址，特别注意末尾乘以2对应RGB565，如果输出格式是ARGB8888，需要乘以4 */
    destination = (uint32_t)pDst + ((y * 800) + x) * 4;

    /* DMA2D采用存储器到存储器模式，并且执行FPC颜色格式转换, 这种模式是前景层作为DMA2D输入 */
    DMA2D->CR = 0x00010000UL | (1 << 9);
    DMA2D->OOR = 800 - xsize;

    /* 输出格式 */
    DMA2D->OPFCCR = PixelFormat | (DMA2D_REGULAR_ALPHA << 20) | (DMA2D_RB_REGULAR << 21);

    /* 前景层输入格式 */
    DMA2D->FGPFCCR = DMA2D_INPUT_YCBCR | (DMA2D_REPLACE_ALPHA << 16) | (DMA2D_REGULAR_ALPHA << 20) | (DMA2D_RB_REGULAR << 21) | (0xFFU << 24) | (cssMode << 18);

    DMA2D->FGOR = inputLineOffset;
    DMA2D->NLR = (uint32_t)(xsize << 16) | (uint16_t)ysize;
    DMA2D->OMAR = (uint32_t)destination;
    DMA2D->FGMAR = (uint32_t)pSrc;

    /* 启动传输 */
    DMA2D->CR |= DMA2D_CR_START;

    /* 等待DMA2D传输完成 */
    while (DMA2D->CR & DMA2D_CR_START)
        ;
}
