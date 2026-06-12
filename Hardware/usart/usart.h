/**
 * usart.h - USART1 driver interface
 */
#ifndef __USART_H
#define __USART_H

#include "stm32f4xx_hal.h"
#include <stdio.h>

extern UART_HandleTypeDef huart1;
extern uint8_t USART_RX_BUF[64];
extern uint8_t USART_RX_STA;
extern uint8_t Res;

void uart_init(uint32_t bound);
void USART1_IRQHandler(void);

#endif
