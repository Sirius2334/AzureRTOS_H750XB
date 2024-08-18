#ifndef __SHELL_PORT_H_
#define __SHELL_PORT_H_

#include "shell.h"
#include "tx_api.h"
#include "drv_uart.h"
#include "RTT_LOG.h"

extern Shell shell;

void userShellInit(Shell *shell);

#endif
