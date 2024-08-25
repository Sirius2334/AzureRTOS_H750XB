#ifndef __DRV_DMA2D_H_
#define __DRV_DMA2D_H_

#include "main.h"
#include "dma2d.h"

void FillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t color);
void DMA2D_MemCopy(void *pSrc, void *pDst, int width, int height, int OffLineSrc, int OffLineDst, uint32_t pixelFormat);
void LCD_CopyBuffer(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t *color);

#endif
