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
