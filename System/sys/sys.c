/**
 * sys.c - System-level helper functions for STM32F407VET6 HAL
 *
 * Note: SystemClock_Config() is in Core/Src/system_stm32f4xx.c
 */
#include "sys.h"

/**
 * @brief  Configure JTAG/SWD mode
 * @param  mode: 0 = JTAG+SWD enabled (default)
 *               1 = SWD only (JTAG disabled, frees PA15/PB3/PB4)
 *               2 = Both disabled (not recommended for debugging)
 */
void JTAG_Set(uint8_t mode)
{
    switch (mode) {
        case 1: /* SWD only - disable JTAG, frees PA15, PB3, PB4 */
            __HAL_RCC_GPIOA_CLK_ENABLE();
            __HAL_RCC_GPIOB_CLK_ENABLE();
            HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
            HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3 | GPIO_PIN_4);
            break;
        case 2: /* Full disable - dangerous for debugging */
            break;
        default:
            break;
    }
}

/**
 * @brief  Software system reset
 */
void Sys_Soft_Reset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief  Get system clock frequency in MHz
 */
uint32_t SystemCoreClock_Get(void)
{
    return HAL_RCC_GetSysClockFreq() / 1000000;
}
