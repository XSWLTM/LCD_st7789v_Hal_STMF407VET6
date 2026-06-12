/**
 * stm32f4xx_it.c - Interrupt Service Routines
 *
 * Handles system exceptions and USART1 interrupt.
 * SysTick_Handler calls HAL_IncTick() for HAL_Delay support.
 */
#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "usart.h"

extern UART_HandleTypeDef huart1;
extern uint8_t Res;

/* ==================================================================
 * Cortex-M4 Processor Exception Handlers
 * ================================================================== */

void NMI_Handler(void)
{
    while (1);
}

void HardFault_Handler(void)
{
    while (1);
}

void MemManage_Handler(void)
{
    while (1);
}

void BusFault_Handler(void)
{
    while (1);
}

void UsageFault_Handler(void)
{
    while (1);
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

/**
 * @brief  SysTick Handler
 *         Increments HAL tick counter for HAL_Delay().
 */
void SysTick_Handler(void)
{
    HAL_IncTick();
}

/* ==================================================================
 * Peripheral Interrupt Handlers
 * ================================================================== */

/**
 * @brief  USART1 Interrupt Handler
 *         Delegates to the USART driver's handler.
 */
void USART1_IRQHandler(void)
{
    uint8_t rx_byte;

    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE)) {
        rx_byte = (uint8_t)(huart1.Instance->DR & 0xFF);

        /* Echo */
        while (!(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)));
        huart1.Instance->DR = rx_byte;

        /* Store globally for application use */
        Res = rx_byte;
    }

    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_ORE)) {
        __HAL_UART_CLEAR_OREFLAG(&huart1);
    }
}
