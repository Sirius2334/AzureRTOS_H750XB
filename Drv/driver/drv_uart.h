#ifndef __DRV_UART_H_
#define __DRV_UART_H_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include "usart.h"
#include "lwrb.h"

#define RX_RINGBUF_SIZE 128
#define TX_RINGBUF_SIZE 128

extern lwrb_t rx_ringbuf;
extern lwrb_t tx_ringbuf;

void uart_init(UART_HandleTypeDef *huart);
uint32_t uart_send(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t len);
uint32_t uart_send_dma(UART_HandleTypeDef *huart, const uint8_t *pData, uint16_t len);
uint32_t uart_printf(UART_HandleTypeDef *huart, const char *fmt, ...);
void uart_dma_transfer(UART_HandleTypeDef *huart);
void HAL_UARTEx_IdleCallback(UART_HandleTypeDef *huart);

#endif
