/**
 * spi.c - SPI1 driver for W25Q16 Flash (HAL version)
 *
 * Original code used direct register writes to configure SPI1.
 * Now uses HAL_SPI_Init() and HAL_SPI_TransmitReceive().
 *
 * SPI1 for STM32F407VET6:
 *   SCK  - PA5  (AF5)
 *   MISO - PA6  (AF5)
 *   MOSI - PA7  (AF5)
 *   CS   - PA4  (GPIO output, manual control)
 */
#include "spi.h"

SPI_HandleTypeDef hspi1;

/**
 * @brief  Initialize SPI1 for Flash communication
 *         Mode 3: CPOL=1, CPHA=1, MSB first, 8-bit data
 *         Initial speed: fPCLK/256
 */
void SPIx_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable clocks */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_SPI1_CLK_ENABLE();

    /* PA5(SCK), PA7(MOSI) - Alternate function push-pull */
    GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA6(MISO) - Alternate function, no pull */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* SPI1 configuration */
    hspi1.Instance = SPI1;
    hspi1.Init.Mode = SPI_MODE_MASTER;
    hspi1.Init.Direction = SPI_DIRECTION_2LINES;
    hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
    hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;    /* CPOL = 1 */
    hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;          /* CPHA = 1 */
    hspi1.Init.NSS = SPI_NSS_SOFT;                  /* Software CS control */
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
    hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
    hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
    hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    hspi1.Init.CRCPolynomial = 7;

    HAL_SPI_Init(&hspi1);

    /* Enable SPI1 */
    __HAL_SPI_ENABLE(&hspi1);

    /* Dummy read to clear any pending data */
    SPIx_ReadWriteByte(0xFF);
}

/**
 * @brief  Change SPI baud rate prescaler
 * @param  SpeedSet: SPI_BAUDRATEPRESCALER_xxx value
 */
void SPIx_SetSpeed(uint8_t SpeedSet)
{
    /* Disable SPI before changing config */
    __HAL_SPI_DISABLE(&hspi1);

    hspi1.Init.BaudRatePrescaler = SpeedSet;
    HAL_SPI_Init(&hspi1);

    __HAL_SPI_ENABLE(&hspi1);
}

/**
 * @brief  Transmit and receive one byte via SPI1
 * @param  TxData: Byte to send
 * @return Byte received
 */
uint8_t SPIx_ReadWriteByte(uint8_t TxData)
{
    uint8_t RxData = 0xFF;

    HAL_SPI_TransmitReceive(&hspi1, &TxData, &RxData, 1, 100);

    return RxData;
}
