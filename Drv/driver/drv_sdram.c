#include "drv_sdram.h"
#include "RTT_LOG.h"

FMC_SDRAM_CommandTypeDef command; // 控制指令

void sdram_init(void)
{
    SDRAM_Initialization_Sequence(&hsdram1, &command); // 配置SDRAM
}

/**
 * @brief  配置SDRAM相关时序和控制方式
 *
 * @param  hsdram pointer to a SDRAM_HandleTypeDef structure that contains
 *                the configuration information for SDRAM module.
 * @param  Command SDRAM command structure
 *
 * @retval None
 */
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
    __IO uint32_t tmpmrd = 0;

    /* Configure a clock configuration enable command */
    Command->CommandMode = FMC_SDRAM_CMD_CLK_ENABLE;  // 开启SDRAM时钟
    Command->CommandTarget = FMC_COMMAND_TARGET_BANK; // 选择要控制的区域
    Command->AutoRefreshNumber = 1;
    Command->ModeRegisterDefinition = 0;

    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT); // 发送控制指令
    for (uint32_t i = 0; i < 240000; i++)                  // 延时等待
        __NOP();

    /* Configure a PALL (precharge all) command */
    Command->CommandMode = FMC_SDRAM_CMD_PALL;        // 预充电命令
    Command->CommandTarget = FMC_COMMAND_TARGET_BANK; // 选择要控制的区域
    Command->AutoRefreshNumber = 1;
    Command->ModeRegisterDefinition = 0;

    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT); // 发送控制指令

    /* Configure a Auto-Refresh command */
    Command->CommandMode = FMC_SDRAM_CMD_AUTOREFRESH_MODE; // 使用自动刷新
    Command->CommandTarget = FMC_COMMAND_TARGET_BANK;      // 选择要控制的区域
    Command->AutoRefreshNumber = 8;                        // 自动刷新次数
    Command->ModeRegisterDefinition = 0;

    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT); // 发送控制指令

    /* Program the external memory mode register */
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2 |
             SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
             SDRAM_MODEREG_CAS_LATENCY_3 |
             SDRAM_MODEREG_OPERATING_MODE_STANDARD |
             SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command->CommandMode = FMC_SDRAM_CMD_LOAD_MODE;   // 加载模式寄存器命令
    Command->CommandTarget = FMC_COMMAND_TARGET_BANK; // 选择要控制的区域
    Command->AutoRefreshNumber = 1;
    Command->ModeRegisterDefinition = tmpmrd;

    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT); // 发送控制指令

    HAL_SDRAM_ProgramRefreshRate(hsdram, 918); // 配置刷新率
}

uint8_t SDRAM_Test(void)
{
    uint32_t i = 0;        // 计数变量
    uint16_t ReadData = 0; // 读取到的数据
    uint8_t ReadData_8b;

    uint32_t ExecutionTime_Begin; // 开始时间
    uint32_t ExecutionTime_End;   // 结束时间
    uint32_t ExecutionTime;       // 执行时间
    float ExecutionSpeed;         // 执行速度

#if (SDRAM_Bits >= 32)

#endif

    RTT_LOG_INFO("start sdram test!!!");

    /* 16 位数据写测试 */
    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

    for (i = 0; i < SDRAM_Size / 2; i++)
    {
        *(__IO uint16_t *)(SDRAM_BANK_ADDR + 2 * i) = (uint16_t)i; // 写入数据
    }
    ExecutionTime_End = HAL_GetTick();                                       // 获取 systick 当前时间，单位ms
    ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;                 // 计算擦除时间，单位ms
    ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime * 1000; // 计算速度，单位 MB/S

    RTT_LOG_INFO("Write as 16 bits, size of data: %d MB, time used: %d ms, writting speed: %.2f MB/s", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);

    /* 16 位数据读测试 */
    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms

    for (i = 0; i < SDRAM_Size / 2; i++)
    {
        ReadData = *(__IO uint16_t *)(SDRAM_BANK_ADDR + 2 * i); // 从SDRAM读出数据
    }
    ExecutionTime_End = HAL_GetTick();                                       // 获取 systick 当前时间，单位ms
    ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;                 // 计算擦除时间，单位ms
    ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime * 1000; // 计算速度，单位 MB/S

    RTT_LOG_INFO("Read data finished: %d MB, time used: %d ms, reading speed: %.2f MB/s", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);

    /* 数据校验 */
    RTT_LOG_INFO("data verifying...");

    for (i = 0; i < SDRAM_Size / 2; i++)
    {
        ReadData = *(__IO uint16_t *)(SDRAM_BANK_ADDR + 2 * i); // 从SDRAM读出数据
        if (ReadData != (uint16_t)i)                            // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            RTT_LOG_INFO("SDRAM test failed!!!");
            return ERROR; // 返回失败标志
        }
    }
    RTT_LOG_INFO("16 bits data writting and reading passed");

    /* 进行8位数据的读写测试 */
    RTT_LOG_INFO("gonna to write data as 8 bits");

    /* 8 位数据写测试 */
    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms
    for (i = 0; i < SDRAM_Size; i++)
    {
        *(__IO uint8_t *)(SDRAM_BANK_ADDR + i) = (uint8_t)i;
    }
    ExecutionTime_End = HAL_GetTick();                                       // 获取 systick 当前时间，单位ms
    ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;                 // 计算擦除时间，单位ms
    ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime * 1000; // 计算速度，单位 MB/S
    RTT_LOG_INFO("Write as 8 bits, size of data: %d MB, time used: %d ms, writting speed: %.2f MB/s", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);

    RTT_LOG_INFO("writting finished, read data and compare...");

    /* 8 位数据读测试 */
    ExecutionTime_Begin = HAL_GetTick(); // 获取 systick 当前时间，单位ms
    for (i = 0; i < SDRAM_Size; i++)
    {
        ReadData_8b = *(__IO uint8_t *)(SDRAM_BANK_ADDR + i);
    }
    ExecutionTime_End = HAL_GetTick();                                       // 获取 systick 当前时间，单位ms
    ExecutionTime = ExecutionTime_End - ExecutionTime_Begin;                 // 计算擦除时间，单位ms
    ExecutionSpeed = (float)SDRAM_Size / 1024 / 1024 / ExecutionTime * 1000; // 计算速度，单位 MB/S
    RTT_LOG_INFO("Read data finished: %d MB, time used: %d ms, reading speed: %.2f MB/s", SDRAM_Size / 1024 / 1024, ExecutionTime, ExecutionSpeed);

    /* 数据校验 */
    RTT_LOG_INFO("data verifying...");

    for (i = 0; i < SDRAM_Size; i++)
    {
        ReadData_8b = *(__IO uint8_t *)(SDRAM_BANK_ADDR + i); // 从SDRAM读出数据
        if (ReadData_8b != (uint8_t)i)                        // 检测数据，若不相等，跳出函数,返回检测失败结果。
        {
            RTT_LOG_INFO("8 bits writting and reading test failed!!!");
            RTT_LOG_INFO("please check the connection of NBL0 and NBL1");
            return ERROR; // 返回失败标志
        }
    }
    RTT_LOG_INFO("8 bits data writting and reading passed");

    /* 测试结束 */
    RTT_LOG_INFO("SDRAM writting and reading passed, congratulations!!!");
    return SUCCESS; // 返回成功标志
}
