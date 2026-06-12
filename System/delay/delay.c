/**
 * delay.c - Delay functions for STM32F407VET6 HAL
 *
 * Uses HAL_GetTick() for ms delay and DWT cycle counter for us delay.
 * The DWT is a Cortex-M4 debug unit that provides a free-running cycle
 * counter, giving sub-microsecond resolution at 168 MHz.
 */
#include "delay.h"
#include "sys.h"

static uint32_t cpu_freq_mhz = 168; /* System clock in MHz */
static uint8_t  dwt_enabled = 0;

/**
 * @brief  Initialize delay subsystem
 *         Enables DWT cycle counter for microsecond delays.
 */
void delay_init(void)
{
    /* Enable DWT access */
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }

    /* Reset and enable cycle counter */
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    /* Get actual system clock */
    cpu_freq_mhz = HAL_RCC_GetSysClockFreq() / 1000000;
    dwt_enabled = 1;
}

/**
 * @brief  Microsecond delay using DWT cycle counter
 * @param  nus: microseconds to delay
 */
void delay_us(uint32_t nus)
{
    if (!dwt_enabled) return;

    uint32_t start = DWT->CYCCNT;
    uint32_t ticks = nus * cpu_freq_mhz;

    while ((DWT->CYCCNT - start) < ticks) {
        /* spin */
    }
}

/**
 * @brief  Millisecond delay using HAL tick
 * @param  nms: milliseconds to delay
 */
void delay_ms(uint16_t nms)
{
    HAL_Delay(nms);
}

/**
 * @brief  Alias for delay_ms (compatibility with original code)
 */
void Delay(uint16_t nms)
{
    delay_ms(nms);
}

/**
 * @brief  Wait for key press with timeout
 *         Blocks until KEY_STEP is pressed or timeout expires.
 */
void WaitKey(void)
{
    uint16_t i, j;
    for (j = 0; j < 1000; j++) {
        for (i = 0; i < 200; i++) {
            if (KEY_STEP) {
                delay_ms(10);
            } else {
                break;
            }
        }
    }
}

/**
 * @brief  Conditional delay with ENTER key override
 *         If ENTER key is held, performs regular delay.
 *         Otherwise, waits for key press.
 * @param  sdly: delay duration in "units"
 */
void StopDelay(unsigned int sdly)
{
    unsigned int i, j, k;
    for (k = 0; k < 50; k++) {
        if (ENTER) {
            for (i = 0; i < sdly; i++)
                for (j = 0; j < 255; j++);
        } else {
            WaitKey();
            delay_ms(20);
        }
    }
}
