/**
 * spi.h - SPI1 driver for W25Q16 Flash (HAL version)
 *
 * All register-level SPI access replaced with HAL_SPI functions.
 * For STM32F407VET6, SPI1 is on APB2 (max 84 MHz).
 */
#ifndef __SPI_H
#define __SPI_H

#include "sys.h"

/* SPI speed prescaler constants (for HAL_SPI) */
#define SPI_SPEED_2     SPI_BAUDRATEPRESCALER_2      /* 42 MHz (APB2/2) */
#define SPI_SPEED_4     SPI_BAUDRATEPRESCALER_4      /* 21 MHz */
#define SPI_SPEED_8     SPI_BAUDRATEPRESCALER_8      /* 10.5 MHz */
#define SPI_SPEED_16    SPI_BAUDRATEPRESCALER_16     /* 5.25 MHz */
#define SPI_SPEED_32    SPI_BAUDRATEPRESCALER_32     /* 2.625 MHz */
#define SPI_SPEED_64    SPI_BAUDRATEPRESCALER_64     /* 1.3125 MHz */
#define SPI_SPEED_128   SPI_BAUDRATEPRESCALER_128    /* 656.25 kHz */
#define SPI_SPEED_256   SPI_BAUDRATEPRESCALER_256    /* 328.125 kHz */

extern SPI_HandleTypeDef hspi1;

void SPIx_Init(void);
void SPIx_SetSpeed(uint8_t SpeedSet);
uint8_t SPIx_ReadWriteByte(uint8_t TxData);

#endif
