#ifndef __MAIN_H
#define __MAIN_H
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>

/* ==================================================================
 * LCD Resolution
 * ================================================================== */
#define COL         240
#define ROW         320
#define COL_Pre     0
#define ROW_Pre     0
#define Delay_Time  500

/* ==================================================================
 * LCD Interface Select (compile-time)
 * Only one should be enabled at a time
 * ================================================================== */
//#define LCD_SPI3Line
#define LCD_SPI4Line
//#define LCD_SPI4Line_CS_D8
//#define LCD_MCU8
//#define LCD_MCUH8
//#define LCD_MCU16

//#define commdata16    /* 16-bit command mode */

/* ==================================================================
 * Test Modes
 * ================================================================== */
#define TEST_Stand
//#define TEST_STAND_H9B
#define String_Enable

/* ==================================================================
 * Type aliases (replacing original u8/u16/u32 from sys.h)
 * ================================================================== */
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;

#endif
