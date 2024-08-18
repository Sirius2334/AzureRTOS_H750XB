#include "threadEntry.h"

#define SHELL_SINGLE_MSG_SIZE 1
#define SHELL_SINGLE_MSG_MAX_NUMBER 4

TX_THREAD mainTaskHandle;

TX_THREAD shellTaskHandle;
TX_QUEUE shellQueueHandle;
TX_MUTEX shellMutex;

UINT thread_init(VOID *memory_ptr)
{
    UINT ret = TX_SUCCESS;
    TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL *)memory_ptr;

    /* Initialize hardware driver */
    initTask();

    /* Allocate the stack for main Task  */
    CHAR *mainTaskStack;
    if (tx_byte_allocate(byte_pool,
                         (VOID **)&mainTaskStack,
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
                         mainTaskStack,
                         mainTask_STACK_SIZE,
                         9,
                         9,
                         TX_NO_TIME_SLICE,
                         TX_AUTO_START) != TX_SUCCESS)
    {
        return TX_THREAD_ERROR;
    }

    /* Allocate the stack for main Task  */
    CHAR *shellTaskStack;
    if (tx_byte_allocate(byte_pool,
                         (VOID **)&shellTaskStack,
                         shellTask_STACK_SIZE,
                         TX_NO_WAIT) != TX_SUCCESS)
    {
        return TX_POOL_ERROR;
    }

    /* Create main Task.  */
    if (tx_thread_create(&shellTaskHandle,
                         "shellTask",
                         shellTask,
                         0,
                         shellTaskStack,
                         shellTask_STACK_SIZE,
                         28,
                         28,
                         TX_NO_TIME_SLICE,
                         TX_AUTO_START) != TX_SUCCESS)
    {
        return TX_THREAD_ERROR;
    }

    /* Allocate the stack for shell queue.  */
    CHAR *shellMessageStack;
    if (tx_byte_allocate(byte_pool, (VOID **)&shellMessageStack,
                         SHELL_SINGLE_MSG_MAX_NUMBER * SHELL_SINGLE_MSG_SIZE * sizeof(ULONG), TX_NO_WAIT) != TX_SUCCESS)
    {
        return TX_POOL_ERROR;
    }
    /* Create shell queue.  */
    if (tx_queue_create(&shellQueueHandle, "shellMessage", SHELL_SINGLE_MSG_SIZE,
                        shellMessageStack, SHELL_SINGLE_MSG_MAX_NUMBER * SHELL_SINGLE_MSG_SIZE * sizeof(ULONG)) != TX_SUCCESS)
    {
        return TX_QUEUE_ERROR;
    }

    /* Create shell mutex.  */
    if (tx_mutex_create(&shellMutex, "shellMutex", TX_NO_INHERIT) != TX_SUCCESS)
    {
        return TX_MUTEX_ERROR;
    }

    return ret;
}
