#include "mainTask.h"
#include "RTT_LOG.h"

VOID mainTask(ULONG id)
{
    (void)id;

    RTT_LOG_INFO("hello world, pi = %f", 3.1415926);

    while (1)
    {
        tx_thread_sleep(1000);
    }
}
