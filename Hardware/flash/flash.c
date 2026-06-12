/**
 * flash.c - W25Q16 SPI Flash driver (HAL version)
 *
 * All register-level SPI access replaced with HAL_SPI calls.
 * Flash CS is on PA4, controlled as a GPIO output pin.
 *
 * Key adaptation: The original flash.c used PAout(4) for CS control,
 * and called SPIx_ReadWriteByte() which now uses HAL_SPI_TransmitReceive().
 */
#include "flash.h"
#include "spi.h"
#include "delay.h"
#include "lcd.h"
#include "interface.h"
#include "blockwrite/blockwrite_default.h"

uint8_t is_Fread = 0;
uint32_t pic_start_addr = 4096 << 2;  /* Start address for pictures (16 KB offset) */
PIC_INFO pic_info;
uint8_t start_x = 2;

/* ==================================================================
 * Flash GPIO and SPI initialization
 * PA4 = CS (GPIO output, manual control)
 * ================================================================== */
void SPI_Flash_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* Enable GPIOA clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* PA4 as output push-pull for CS */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* CS high (inactive) */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    /* Initialize SPI1 */
    SPIx_Init();
}

/* ==================================================================
 * Read Status Register
 * BIT7: SPR, BIT6: RV, BIT5: TB, BIT4-2: BP, BIT1: WEL, BIT0: BUSY
 * ================================================================== */
uint8_t SPI_Flash_ReadSR(void)
{
    uint8_t byte = 0;

    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_ReadStatusReg);
    byte = SPIx_ReadWriteByte(0xFF);
    SPI_FLASH_CS_SET();

    return byte;
}

/* Write Status Register (SPR, TB, BP bits only) */
void SPI_FLASH_Write_SR(uint8_t sr)
{
    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_WriteStatusReg);
    SPIx_ReadWriteByte(sr);
    SPI_FLASH_CS_SET();
}

/* Write Enable (set WEL bit) */
void SPI_FLASH_Write_Enable(void)
{
    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_WriteEnable);
    SPI_FLASH_CS_SET();
}

/* Write Disable (clear WEL bit) */
void SPI_FLASH_Write_Disable(void)
{
    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_WriteDisable);
    SPI_FLASH_CS_SET();
}

/* Read Manufacturer/Device ID (W25Q16 = 0xEF14) */
uint16_t SPI_Flash_ReadID(void)
{
    uint16_t Temp = 0;

    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(0x90);
    SPIx_ReadWriteByte(0x00);
    SPIx_ReadWriteByte(0x00);
    SPIx_ReadWriteByte(0x00);
    Temp |= SPIx_ReadWriteByte(0xFF) << 8;
    Temp |= SPIx_ReadWriteByte(0xFF);
    SPI_FLASH_CS_SET();

    return Temp;
}

/* ==================================================================
 * Read data from Flash
 * ReadAddr: 24-bit address
 * NumByteToRead: number of bytes to read
 * ================================================================== */
void SPI_Flash_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint16_t i;

    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_ReadData);
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPIx_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < NumByteToRead; i++) {
        pBuffer[i] = SPIx_ReadWriteByte(0xFF);
    }

    SPI_FLASH_CS_SET();
}

/* ==================================================================
 * Page Program (up to 256 bytes per page)
 * Assumes the page has been erased (all 0xFF) beforehand.
 * ================================================================== */
void SPI_Flash_Write_Page(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t i;

    SPI_FLASH_Write_Enable();
    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_PageProgram);
    SPIx_ReadWriteByte((uint8_t)((WriteAddr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((WriteAddr) >> 8));
    SPIx_ReadWriteByte((uint8_t)WriteAddr);

    for (i = 0; i < NumByteToWrite; i++) {
        SPIx_ReadWriteByte(pBuffer[i]);
    }

    SPI_FLASH_CS_SET();
    SPI_Flash_Wait_Busy();
}

/* Write without checking (assumes area is already erased) */
void SPI_Flash_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t pageremain;

    pageremain = 256 - WriteAddr % 256;
    if (NumByteToWrite <= pageremain) pageremain = NumByteToWrite;

    while (1) {
        SPI_Flash_Write_Page(pBuffer, WriteAddr, pageremain);
        if (NumByteToWrite == pageremain) break;
        else {
            pBuffer += pageremain;
            WriteAddr += pageremain;
            NumByteToWrite -= pageremain;
            if (NumByteToWrite > 256) pageremain = 256;
            else pageremain = NumByteToWrite;
        }
    }
}

/* Write with automatic erase (sector-level, 4KB) */
uint8_t SPI_FLASH_BUF[4096];

void SPI_Flash_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint32_t secpos;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i;

    secpos = WriteAddr / 4096;
    secoff = WriteAddr % 4096;
    secremain = 4096 - secoff;

    if (NumByteToWrite <= secremain) secremain = NumByteToWrite;

    while (1) {
        SPI_Flash_Read(SPI_FLASH_BUF, secpos * 4096, 4096);
        for (i = 0; i < secremain; i++) {
            if (SPI_FLASH_BUF[secoff + i] != 0xFF) break;
        }
        if (i < secremain) {
            /* Sector needs erasing */
            SPI_Flash_Erase_Sector(secpos);
            for (i = 0; i < secremain; i++) {
                SPI_FLASH_BUF[i + secoff] = pBuffer[i];
            }
            SPI_Flash_Write_NoCheck(SPI_FLASH_BUF, secpos * 4096, 4096);
        } else {
            /* Already erased, write directly */
            SPI_Flash_Write_NoCheck(pBuffer, WriteAddr, secremain);
        }

        if (NumByteToWrite == secremain) break;
        else {
            secpos++;
            secoff = 0;
            pBuffer += secremain;
            WriteAddr += secremain;
            NumByteToWrite -= secremain;
            if (NumByteToWrite > 4096) secremain = 4096;
            else secremain = NumByteToWrite;
        }
    }
}

/* ==================================================================
 * Erase operations
 * Chip Erase: ~25 seconds for W25Q16
 * Sector Erase (4KB): ~150ms
 * ================================================================== */
void SPI_Flash_Erase_Chip_ALL(void)
{
    SPI_Flash_Wait_Busy();
    SPI_FLASH_Write_Enable();
    SPI_Flash_Wait_Busy();

    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_ChipErase);
    SPI_FLASH_CS_SET();

    SPI_Flash_Wait_Busy();
}

void SPI_Flash_Erase_Sector(uint32_t Dst_Addr)
{
    Dst_Addr = Dst_Addr << 12;  /* Convert sector number to byte address */

    SPI_FLASH_Write_Enable();
    SPI_Flash_Wait_Busy();

    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_SectorErase);
    SPIx_ReadWriteByte((uint8_t)((Dst_Addr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((Dst_Addr) >> 8));
    SPIx_ReadWriteByte((uint8_t)Dst_Addr);
    SPI_FLASH_CS_SET();

    SPI_Flash_Wait_Busy();
}

/* ==================================================================
 * Wait for busy flag to clear
 * ================================================================== */
void SPI_Flash_Wait_Busy(void)
{
    while ((SPI_Flash_ReadSR() & 0x01) == 0x01);
}

/* Power down / wakeup */
void SPI_Flash_PowerDown(void)
{
    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_PowerDown);
    SPI_FLASH_CS_SET();
    delay_us(3);
}

void SPI_Flash_WAKEUP(void)
{
    SPI_FLASH_CS_CLR();
    SPIx_ReadWriteByte(W25X_ReleasePowerDown);
    SPI_FLASH_CS_SET();
    delay_us(3);
}

/* ==================================================================
 * Picture Display Functions
 *
 * These stream pixel data from the SPI Flash directly to the LCD.
 * ================================================================== */

/**
 * Stream image from Flash to LCD, pixel by pixel
 * Used for parallel LCD interface modes.
 */
void DisPic_Windows_From_FLASH(uint32_t ReadAddr, uint32_t NumByteToRead)
{
    uint32_t j;
    unsigned char pic_data[2];

    LCD_CS_CLR();
    LCD_RS_SET();
    LCD_RD_SET();

    for (j = 0; j < NumByteToRead; j++) {
        SPI_FLASH_CS_CLR();
        SPIx_ReadWriteByte(W25X_ReadData);
        SPIx_ReadWriteByte((uint8_t)((ReadAddr + j) >> 16));
        SPIx_ReadWriteByte((uint8_t)((ReadAddr + j) >> 8));
        SPIx_ReadWriteByte((uint8_t)(ReadAddr + j));

        pic_data[0] = SPIx_ReadWriteByte(0xFF);
        pic_data[1] = SPIx_ReadWriteByte(0xFF);

        DATAOUT((pic_data[0]));
        LCD_WR_CLR();
        LCD_WR_SET();

        DATAOUT((pic_data[1]));
        LCD_WR_CLR();
        LCD_WR_SET();
    }

    LCD_CS_SET();
    SPI_FLASH_CS_SET();
}

/**
 * Display picture by number from Flash (16-bit color mode)
 * Streams data using the SendData() interface function.
 */
void Disp_Flash_Pic_Num(uint8_t num)
{
    uint32_t i;
    uint8_t data_h, data_l;
    uint32_t ReadAddr;
    uint32_t NumByteToRead;

    ReadAddr = lcdconfig.pic_bytes * num + pic_start_addr;
    NumByteToRead = lcdconfig.pic_bytes;

    BlockWrite(lcdconfig.col_pre, lcdconfig.col + lcdconfig.col_pre - 1,
               lcdconfig.row_pre, lcdconfig.row + lcdconfig.row_pre - 1);

    SPI_FLASH_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();
    LCD_CS_CLR();
    is_Fread = 1;

    SPIx_ReadWriteByte(W25X_ReadData);
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPIx_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < (NumByteToRead >> 1); i++) {
        data_h = SPIx_ReadWriteByte(0xFF);
        data_l = SPIx_ReadWriteByte(0xFF);
        SendData(((uint16_t)(data_h) << 8) | data_l);
    }

    SPI_FLASH_CS_SET();
    LCD_CS_SET();
    is_Fread = 0;
}

/**
 * Read and display picture data from Flash (generic version)
 */
void Read_FlashPic(uint32_t ReadAddr, uint32_t NumByteToRead)
{
    uint32_t i;
    uint8_t data_h, data_l;

    BlockWrite(lcdconfig.col_pre, lcdconfig.col + lcdconfig.col_pre - 1,
               lcdconfig.row_pre, lcdconfig.row + lcdconfig.row_pre - 1);

    SPI_FLASH_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();
    LCD_CS_CLR();
    is_Fread = 1;

    SPIx_ReadWriteByte(W25X_ReadData);
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPIx_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < (NumByteToRead >> 1); i++) {
        data_h = SPIx_ReadWriteByte(0xFF);
        data_l = SPIx_ReadWriteByte(0xFF);
        SendData(((uint16_t)(data_h) << 8) | data_l);
    }

    SPI_FLASH_CS_SET();
    LCD_CS_SET();
    is_Fread = 0;
}

/**
 * Read picture from Flash for H9Bit interface (9-bit per channel color)
 */
void Read_FlashPic_9b(uint32_t ReadAddr, uint32_t NumByteToRead)
{
    uint32_t i;
    int data_r, data_g, data_b;
    int data_first_9bit, data_second_9bit;

    BlockWrite(lcdconfig.col_pre, lcdconfig.col + lcdconfig.col_pre - 1,
               lcdconfig.row_pre, lcdconfig.row + lcdconfig.row_pre - 1);

    SPI_FLASH_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();
    LCD_CS_CLR();

    SPIx_ReadWriteByte(W25X_ReadData);
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPIx_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < NumByteToRead / 3; i++) {
        data_r = SPIx_ReadWriteByte(0xFF);
        data_g = SPIx_ReadWriteByte(0xFF);
        data_b = SPIx_ReadWriteByte(0xFF);

        data_first_9bit = ((data_r & 0xFC) << 8) | (((data_g & 0xE0) << 2));
        data_second_9bit = ((data_g & 0x1C) << 11) | ((data_b & 0xFC) << 5);

        DATAOUT(data_first_9bit);
        LCD_WR_CLR();
        LCD_WR_SET();

        DATAOUT(data_second_9bit);
        LCD_WR_CLR();
        LCD_WR_SET();
    }

    SPI_FLASH_CS_SET();
    LCD_CS_SET();
}

void Disp_Flash_Pic_9b_Num(uint8_t num)
{
    Read_FlashPic_9b(lcdconfig.pic_bytes * num + pic_start_addr, lcdconfig.pic_bytes);
}

/**
 * Read picture from Flash, high byte first
 */
void Read_FlashPic_H(uint32_t ReadAddr, uint32_t NumByteToRead)
{
    uint32_t i;
    uint8_t data_h, data_l;

    LCD_GPIOInit();
    delay_ms(10);

    BlockWrite(lcdconfig.col_pre, lcdconfig.col + lcdconfig.col_pre - 1,
               lcdconfig.row_pre, lcdconfig.row + lcdconfig.row_pre - 1);

    SPI_FLASH_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();
    LCD_CS_CLR();

    SPIx_ReadWriteByte(W25X_ReadData);
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 16));
    SPIx_ReadWriteByte((uint8_t)((ReadAddr) >> 8));
    SPIx_ReadWriteByte((uint8_t)ReadAddr);

    for (i = 0; i < NumByteToRead / 2; i++) {
        data_l = SPIx_ReadWriteByte(0xFF);
        data_h = SPIx_ReadWriteByte(0xFF);
        DATAOUT(((uint16_t)(data_h) << 8) | data_l);
        LCD_WR_CLR();
        LCD_WR_SET();
    }

    SPI_FLASH_CS_SET();
    LCD_CS_SET();
}
