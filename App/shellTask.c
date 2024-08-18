#include "shellTask.h"

char *getlwrbEvent(lwrb_evt_type_t evt)
{
    switch (evt)
    {
    case LWRB_EVT_READ:
        return "LWRB_EVT_READ";
        break;
    case LWRB_EVT_WRITE:
        return "LWRB_EVT_WRITE";
        break;
    case LWRB_EVT_RESET:
        return "LWRB_EVT_RESET";
        break;
    default:
        break;
    }
    return NULL;
}

void lwrb_recv_cb(struct lwrb *buff, lwrb_evt_type_t evt, lwrb_sz_t bp)
{
    (void)buff;

    if (evt == LWRB_EVT_WRITE)
        tx_queue_send(&shellQueueHandle, (void *)bp, TX_NO_WAIT);
}

VOID shellTask(ULONG id)
{
    (void)id;
    char c;
    uint32_t string_len;

    lwrb_set_evt_fn(&rx_ringbuf, lwrb_recv_cb);
    userShellInit(&shell);

    while (1)
    {
        tx_queue_receive(&shellQueueHandle, &string_len, TX_WAIT_FOREVER);

        for (uint16_t i = 0; i < string_len; i++)
        {
            if (lwrb_read(&rx_ringbuf, &c, 1))
                // if (shell.read && shell.read(&c, 1) == 1)
                shellHandler(&shell, c);

            // if (SEGGER_RTT_Read(0, &c, 1))
            //     shellHandler(&shell, c);
            // tx_thread_sleep(10);
        }
    }
}

int shellFunc(int argc, char *argv[])
{
    uart_printf(&huart1, "%d parameter(s)\r\n", argc);
    // shellPrint(&shell, "%dparameter(s)\r\n", argc);
    for (unsigned char i = 0; i < argc; i++)
    {
        uart_printf(&huart1, "%s\r\n", (char *)argv[i]);
        // shellPrint(&shell, "%s\r\n", (char *)argv[i]);
    }

    return 0;
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0) | SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN),
                 func,
                 shellFunc,
                 test);