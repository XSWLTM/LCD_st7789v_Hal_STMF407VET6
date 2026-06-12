/**
 * main.c - Application entry point for ST7789V LCD on STM32F407VET6 HAL
 *
 * Ported from original S300 reference program.
 * Cycle through test patterns: frame, grayscale, flash image,
 * color bands, solid colors, and "Hello!" text.
 */
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "flash.h"
#include "interface.h"
#include "lcd.h"

/* ==================================================================
 * Declarations
 * ================================================================== */
extern uint16_t POINT_COLOR;

#ifdef TEST_Stand
/**
 * @brief  Standard test sequence
 *         Displays frame, grayscale, picture, color bands, and solid colors.
 */
void TEST_STAND(void)
{
    /* Frame border test */
    DispFrame();
    StopDelay(Delay_Time);

    /* 16-level grayscale pattern */
    DispGrayHor16();
    StopDelay(Delay_Time);

    /* Display picture from SPI Flash (picture #0) */
    Disp_Flash_Pic_Num(0);
    StopDelay(Delay_Time);

    /* Color bands */
    DispBand();
    StopDelay(Delay_Time);

    /* Solid color tests */
    DispColor(RED);
    StopDelay(Delay_Time);

    DispColor(GREEN);
    StopDelay(Delay_Time);

    DispColor(BLUE);
    StopDelay(Delay_Time);

    DispColor(WHITE);
    StopDelay(Delay_Time);

    DispColor(BLACK);
    StopDelay(Delay_Time);
}
#endif

#ifdef TEST_STAND_H9B
void TEST_STAND_9b(void)
{
    DispFrame_9b();
    StopDelay(Delay_Time);

    Disp_Flash_Pic_9b_Num(0);
    StopDelay(Delay_Time);

    DispBand_9b();
    StopDelay(Delay_Time);

    DispColor(0xFC00);  /* Red (9-bit) */
    StopDelay(Delay_Time);

    DispColor(GREEN);
    StopDelay(Delay_Time);

    DispColor(0x003F);  /* Blue (9-bit) */
    StopDelay(Delay_Time);

    DispColor(WHITE);
    StopDelay(Delay_Time);

    DispColor(BLACK);
    StopDelay(Delay_Time);
}
#endif

void SystemClock_Config(void);

/**
 * @brief  Error handler - called on unrecoverable HAL errors
 */
void Error_Handler(void)
{
    __disable_irq();
    while (1) { }
}

/* ==================================================================
 * Main Entry Point
 * ================================================================== */
int main(void)
{
    /* HAL initialization */
    HAL_Init();

    /* Keep debugger active in sleep/stop/standby — prevents chip lock */
    DBGMCU->CR |= DBGMCU_CR_DBG_SLEEP | DBGMCU_CR_DBG_STOP | DBGMCU_CR_DBG_STANDBY;

    /* Brief startup delay: gives debugger a window to connect */
    for (volatile uint32_t i = 0; i < 2000000; i++) { __NOP(); }

    /* System clock: HSE 8 MHz -> PLL -> 168 MHz */
    SystemClock_Config();

    /* Initialize delay subsystem (DWT cycle counter) */
    delay_init();

    /* Initialize USART1 at 256000 baud */
    uart_init(256000);

    /* Initialize LCD (GPIO, config, reset, ST7789V init sequence) */
    LCD_Init();

    /* Initialize SPI Flash (W25Q16) */
    SPI_Flash_Init();

    /* Main loop: cycle through test patterns indefinitely */
    while (1) {
#ifdef TEST_Stand
        TEST_STAND();

        POINT_COLOR = RED;
        LCD_ShowString(60, 60, 100, 16, 16, (uint8_t *)"Hello!");
        StopDelay(Delay_Time);
        StopDelay(Delay_Time);
#endif

#ifdef TEST_STAND_H9B
        TEST_STAND_9b();
#endif
    }
}
