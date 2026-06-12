/**
 * usart.c - USART1 driver for STM32F407VET6 (HAL version)
 *
 * Provides printf() redirect via fputc, and interrupt-based echo.
 * USART1: TX=PA9, RX=PA10, Baud=256000 (default)
 *
 * Original used USART_Init (StdPeriph). Now uses HAL_UART_Init.
 */
#include "usart.h"
#include "sys.h"
#include "delay.h"

UART_HandleTypeDef huart1;

/* Receive buffer */
uint8_t USART_RX_BUF[64];
uint8_t USART_RX_STA = 0;
uint8_t Res = 0;

/**
 * @brief  Initialize USART1
 * @param  bound: Baud rate (e.g., 256000)
 *
 * Pin mapping for STM32F407VET6:
 *   TX = PA9  (AF7)
 *   RX = PA10 (AF7)
 */
void uart_init(uint32_t bound)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_USART1_CLK_ENABLE();

    /* PA9(TX) - Alternate function push-pull */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA10(RX) - Alternate function, no pull (or pull-up for idle high) */
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 configuration */
    huart1.Instance = USART1;
    huart1.Init.BaudRate = bound;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart1);

    /* Enable RX interrupt */
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

/**
 * @brief  printf() redirect - sends character via USART1
 *         Called by the C standard library when using printf.
 */
int fputc(int ch, FILE *f)
{
    /* Wait for TX data register empty */
    while (!(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE)));
    huart1.Instance->DR = (uint8_t)ch;
    return ch;
}
