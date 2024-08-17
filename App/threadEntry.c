#include "threadEntry.h"

TX_THREAD mainTaskHandle;

UINT thread_init(VOID *memory_ptr)
{
    UINT ret = TX_SUCCESS;
    TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL *)memory_ptr;

    /* Allocate the stack for main Task  */
    CHAR *TaskStack;
    if (tx_byte_allocate(byte_pool,
                         (VOID **)&TaskStack,
                         mainTask_STACK_SIZE,
                         TX_NO_WAIT) != TX_SUCCESS)
    {
        return TX_POOL_ERROR;
    }

    /* Create main Task.  */
    if (tx_thread_create(&mainTaskHandle,
                         "mainTask",
                         mainTask,
                         0,
                         TaskStack,
                         mainTask_STACK_SIZE,
                         20,
                         20,
                         TX_NO_TIME_SLICE,
                         TX_AUTO_START) != TX_SUCCESS)
    {
        return TX_THREAD_ERROR;
    }

    return ret;
}
