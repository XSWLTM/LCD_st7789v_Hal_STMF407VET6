/**
 * sys.h - System-level definitions for STM32F407VET6 HAL
 *
 * Replaces original sys.h (bit-band macros) with HAL-compatible GPIO pin
 * definitions. All direct register access has been replaced with HAL
 * GPIO functions.
 */
#ifndef __SYS_H
#define __SYS_H

#include "stm32f4xx_hal.h"
#include "main.h"

/* ==================================================================
 * LCD Control Pins (3-line SPI mode)
 *
 * Pin mapping for STM32F407VET6 (LQFP-100):
 *   LCD_CS  -> PB12  (Chip Select)
 *   LCD_SCL -> PB13  (SPI Clock / WR strobe)
 *   LCD_SDA -> PC6  (SPI Data / RD strobe)
 *   LCD_RS  -> PC4  (Data/Command select)
 *   LCD_RST -> PC7  (Hardware Reset, active low)
 *   LCD_PD  -> PD2  (Power Down / Backlight)
 * ================================================================== */

/* LCD CS pin */
#define LCD_CS_PORT GPIOB
#define LCD_CS_PIN GPIO_PIN_12

/* LCD SCL / WR pin */
#define LCD_SPI_SCL_PORT GPIOB
#define LCD_SPI_SCL_PIN GPIO_PIN_13

/* LCD SDA / RD pin */
#define LCD_SPI_SDA_PORT GPIOB
#define LCD_SPI_SDA_PIN GPIO_PIN_15

/* LCD SDA2 (for dual-SPI mode) */
#define LCD_SPI_SDA2_PORT GPIOB
#define LCD_SPI_SDA2_PIN GPIO_PIN_0 // 未知

/* LCD RS (Data/Command) pin */
#define LCD_RS_PORT GPIOC
#define LCD_RS_PIN GPIO_PIN_5

/* LCD RST (Reset) pin */
#define LCD_RST_PORT GPIOB
#define LCD_RST_PIN GPIO_PIN_14

/* LCD PD (Power Down / Backlight) pin */
#define LCD_PD_PORT GPIOB
#define LCD_PD_PIN GPIO_PIN_1

/* LCD data bus port (for parallel modes) */
#define LCD_DATA_PORT GPIOB // 未知,用不到

/* LCD SPI SDO (MISO read-back, optional) -- not used in SPI 3-line mode */
#define LCD_SPI_SDO_PORT GPIOC
#define LCD_SPI_SDO_PIN GPIO_PIN_9


/* ==================================================================
 * Button Pins
 * ================================================================== */
#define KEY_ENTER_PORT GPIOC
#define KEY_ENTER_PIN GPIO_PIN_2
#define KEY_STEP_PORT GPIOC
#define KEY_STEP_PIN GPIO_PIN_1
#define KEY_UP_PORT GPIOC
#define KEY_UP_PIN GPIO_PIN_0

/* ==================================================================
 * SPI Flash Pins
 * ================================================================== */
#define FLASH_CS_PORT GPIOA
#define FLASH_CS_PIN GPIO_PIN_4
#define FLASH_SCK_PORT GPIOA
#define FLASH_SCK_PIN GPIO_PIN_5
#define FLASH_MISO_PORT GPIOA
#define FLASH_MISO_PIN GPIO_PIN_6
#define FLASH_MOSI_PORT GPIOA
#define FLASH_MOSI_PIN GPIO_PIN_7

/* ==================================================================
 * USART1 Pins
 * ================================================================== */
#define USART1_TX_PORT GPIOA
#define USART1_TX_PIN GPIO_PIN_9
#define USART1_RX_PORT GPIOA
#define USART1_RX_PIN GPIO_PIN_10

/* ==================================================================
 * GPIO Set/Clear helper macros (using HAL for thread-safety)
 * These replace the original BSRR/BRR register macros.
 * ================================================================== */
#define LCD_CS_SET() HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET)
#define LCD_CS_CLR() HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET)

#define LCD_RS_SET() HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_SET)
#define LCD_RS_CLR() HAL_GPIO_WritePin(LCD_RS_PORT, LCD_RS_PIN, GPIO_PIN_RESET)

#define LCD_RST_SET() HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_SET)
#define LCD_RST_CLR() HAL_GPIO_WritePin(LCD_RST_PORT, LCD_RST_PIN, GPIO_PIN_RESET)

#define LCD_WR_SET() HAL_GPIO_WritePin(LCD_SPI_SCL_PORT, LCD_SPI_SCL_PIN, GPIO_PIN_SET)
#define LCD_WR_CLR() HAL_GPIO_WritePin(LCD_SPI_SCL_PORT, LCD_SPI_SCL_PIN, GPIO_PIN_RESET)

#define LCD_RD_SET() HAL_GPIO_WritePin(LCD_SPI_SDA_PORT, LCD_SPI_SDA_PIN, GPIO_PIN_SET)
#define LCD_RD_CLR() HAL_GPIO_WritePin(LCD_SPI_SDA_PORT, LCD_SPI_SDA_PIN, GPIO_PIN_RESET)

#define LCD_PD_SET() HAL_GPIO_WritePin(LCD_PD_PORT, LCD_PD_PIN, GPIO_PIN_SET)
#define LCD_PD_CLR() HAL_GPIO_WritePin(LCD_PD_PORT, LCD_PD_PIN, GPIO_PIN_RESET)

/* SPI bit-bang (3-line SPI) */
#define LCD_SPI_SCL_SET() HAL_GPIO_WritePin(LCD_SPI_SCL_PORT, LCD_SPI_SCL_PIN, GPIO_PIN_SET)
#define LCD_SPI_SCL_CLR() HAL_GPIO_WritePin(LCD_SPI_SCL_PORT, LCD_SPI_SCL_PIN, GPIO_PIN_RESET)
#define LCD_SPI_SDA_SET() HAL_GPIO_WritePin(LCD_SPI_SDA_PORT, LCD_SPI_SDA_PIN, GPIO_PIN_SET)
#define LCD_SPI_SDA_CLR() HAL_GPIO_WritePin(LCD_SPI_SDA_PORT, LCD_SPI_SDA_PIN, GPIO_PIN_RESET)
#define LCD_SPI_SDA2_SET() HAL_GPIO_WritePin(LCD_SPI_SDA2_PORT, LCD_SPI_SDA2_PIN, GPIO_PIN_SET)
#define LCD_SPI_SDA2_CLR() HAL_GPIO_WritePin(LCD_SPI_SDA2_PORT, LCD_SPI_SDA2_PIN, GPIO_PIN_RESET)

/* Touch panel */
#define TP_CS_SET() HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_SET)
#define TP_CS_CLR() HAL_GPIO_WritePin(TP_CS_PORT, TP_CS_PIN, GPIO_PIN_RESET)
#define TP_DCLK_SET() HAL_GPIO_WritePin(TP_DCLK_PORT, TP_DCLK_PIN, GPIO_PIN_SET)
#define TP_DCLK_CLR() HAL_GPIO_WritePin(TP_DCLK_PORT, TP_DCLK_PIN, GPIO_PIN_RESET)
#define TP_DIN_SET() HAL_GPIO_WritePin(TP_DIN_PORT, TP_DIN_PIN, GPIO_PIN_SET)
#define TP_DIN_CLR() HAL_GPIO_WritePin(TP_DIN_PORT, TP_DIN_PIN, GPIO_PIN_RESET)
#define TP_DOUT_SET() HAL_GPIO_WritePin(TP_DOUT_PORT, TP_DOUT_PIN, GPIO_PIN_SET)
#define TP_DOUT_CLR() HAL_GPIO_WritePin(TP_DOUT_PORT, TP_DOUT_PIN, GPIO_PIN_RESET)

/* SPI Flash CS */
#define SPI_FLASH_CS_SET() HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_SET)
#define SPI_FLASH_CS_CLR() HAL_GPIO_WritePin(FLASH_CS_PORT, FLASH_CS_PIN, GPIO_PIN_RESET)

/* Button reads */
#define ENTER (HAL_GPIO_ReadPin(KEY_ENTER_PORT, KEY_ENTER_PIN) == GPIO_PIN_SET)
#define KEY_STEP (HAL_GPIO_ReadPin(KEY_STEP_PORT, KEY_STEP_PIN) == GPIO_PIN_SET)
#define KEY_UP (HAL_GPIO_ReadPin(KEY_UP_PORT, KEY_UP_PIN) == GPIO_PIN_SET)

/* Touch panel reads */
#define PEN_IRQ HAL_GPIO_ReadPin(TP_PENIRQ_PORT, TP_PENIRQ_PIN)
#define DOUT HAL_GPIO_ReadPin(TP_DOUT_PORT, TP_DOUT_PIN)
#define LCD_SPI_SDO HAL_GPIO_ReadPin(LCD_SPI_SDO_PORT, LCD_SPI_SDO_PIN)

/* Data bus access (parallel modes) */
#define DATAOUT(x) (LCD_DATA_PORT->ODR = (x))
#define DATAIN (LCD_DATA_PORT->IDR)

/* ==================================================================
 * Color Constants (RGB565)
 * ================================================================== */
#define WHITE 0xFFFF
#define BLACK 0x0000
#define BLUE 0x001F
#define BRED 0xF81F
#define GRED 0xFFE0
#define GBLUE 0x07FF
#define RED 0xF800
#define MAGENTA 0xF81F
#define GREEN 0x07E0
#define CYAN 0x7FFF
#define YELLOW 0xFFE0
#define BROWN 0xBC40
#define BRRED 0xFC07
#define GRAY 0x8430
#define GRAY25 0xADB5

#define DARKBLUE 0x01CF
#define LIGHTBLUE 0x7D7C
#define GRAYBLUE 0x5458
#define LIGHTGREEN 0x841F
#define LGRAY 0xC618
#define LGRAYBLUE 0xA651
#define LBBLUE 0x2B12

/* Touch panel calibration range */
#define X_min 0x0043
#define X_max 0x07AE
#define Y_min 0x00A1
#define Y_max 0x0759

#endif /* __SYS_H */
