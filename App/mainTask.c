#include "mainTask.h"

VOID mainTask(ULONG id)
{
    (void)id;

    while (1)
    {
        tx_thread_sleep(1000);
    }
}
