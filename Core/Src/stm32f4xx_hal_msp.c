/**
 * stm32f4xx_hal_msp.c - HAL MSP initialization
 *
 * Provides HAL_MspInit() which was previously using the weak default.
 * Critical: enables SYSCFG clock which is required for DBGMCU debug access.
 */
#include "stm32f4xx_hal.h"

void HAL_MspInit(void)
{
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}
