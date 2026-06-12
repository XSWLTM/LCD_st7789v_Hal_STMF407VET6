/**
 * system_stm32f4xx.c - System initialization for STM32F407VET6
 *
 * Provides SystemInit() called from startup before main().
 * Configures FPU, vector table offset, and initial system clock.
 */
#include "stm32f4xx_hal.h"

/* External: defined in main.c */
extern void Error_Handler(void);

/* SystemCoreClock variable - used by CMSIS and HAL */
uint32_t SystemCoreClock = 16000000U;

/**
 * @brief  AHB and APB prescaler tables
 *         Required by HAL_RCC_ClockConfig() / HAL_RCC_GetPCLKxFreq()
 */
const uint8_t AHBPrescTable[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};

/* Vector table base offset */
#define VECT_TAB_OFFSET  0x00

/**
 * @brief  System Clock Configuration
 *         Called from main() to set up the full clock tree.
 *
 *         HSE (8 MHz) -> PLL (x336) -> SYSCLK = 168 MHz
 *         AHB  = 168 MHz
 *         APB1 = 42 MHz  (max for APB1 peripherals)
 *         APB2 = 84 MHz  (max for APB2 peripherals)
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /* Enable Power Control clock */
    __HAL_RCC_PWR_CLK_ENABLE();

    /* Voltage regulator scale 1 for 168 MHz operation */
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /* HSE Oscillator: 8 MHz */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;              /* HSE / 8 = 1 MHz */
    RCC_OscInitStruct.PLL.PLLN = 336;            /* 1 MHz * 336 = 336 MHz (VCO) */
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;  /* VCO / 2 = 168 MHz (SYSCLK) */
    RCC_OscInitStruct.PLL.PLLQ = 7;              /* VCO / 7 = 48 MHz */
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    /* Clock tree: SYSCLK = 168 MHz */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* HCLK = 168 MHz */
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;      /* APB1 = 42 MHz */
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;      /* APB2 = 84 MHz */
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

    /* SystemCoreClock is updated internally by HAL_RCC_ClockConfig */

    /* Enable instruction/data caches and prefetch buffer */
    __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
    __HAL_FLASH_DATA_CACHE_ENABLE();
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
}

/**
 * @brief  System initialization called from startup before main()
 *         Configures FPU and vector table.
 */
void SystemInit(void)
{
    /* FPU settings for Cortex-M4 */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2) | (3UL << 11*2));  /* Enable CP10, CP11 full access */
#endif

    /* Configure vector table offset */
#ifdef VECT_TAB_SRAM
    SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET;
#else
    SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET;
#endif
}
