#ifndef __SHELL_TASK_H_
#define __SHELL_TASK_H_

#include "main.h"
#include "tx_api.h"
#include "drv_uart.h"

#include "shell_port.h"

extern TX_THREAD shellTaskHandle;
extern TX_QUEUE shellQueueHandle;

VOID shellTask(ULONG id);

#endif