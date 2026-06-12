/**
 * delay.h - Delay functions for STM32F4 HAL
 *
 * Replaces original delay.h which used SysTick register manipulation.
 * Uses HAL_Delay (SysTick-based) for milliseconds and DWT cycle counter
 * for microsecond delays.
 */
#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx_hal.h"

void delay_init(void);
void delay_ms(uint16_t nms);
void delay_us(uint32_t nus);
void Delay(uint16_t nms);
void WaitKey(void);
void StopDelay(unsigned int sdly);

#endif
