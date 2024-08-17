#include "RTT_LOG.h"

void rtt_init(void)
{
    SEGGER_RTT_Init();
}

void rtt_printf(uint8_t BufferIndex, const char *fmt, ...)
{
    char buf[128];
    uint32_t len;

    va_list arg;

    va_start(arg, fmt);
    len = vsnprintf(buf, sizeof(buf), fmt, arg);
    va_end(arg);

    SEGGER_RTT_Write(BufferIndex, (uint8_t *)buf, len);
}
