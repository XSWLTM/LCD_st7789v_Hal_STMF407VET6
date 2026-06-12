/**
 * interface.c - LCD interface protocol implementations
 *
 * All register-level GPIO access (BSRR/BRR) replaced with
 * HAL_GPIO_WritePin() calls. The SPI bit-bang protocol is
 * preserved for 3-line/4-line modes.
 *
 * Supported modes (compile-time selectable via main.h):
 *   - LCD_SPI3Line:    3-wire SPI (D/C encoded in first SDA bit)
 *   - LCD_SPI3Line2:   3-wire SPI dual SDA
 *   - LCD_SPI4Line:    4-wire SPI (separate RS pin)
 *   - LCD_MCU8:        8-bit parallel 8080
 *   - LCD_MCUH8:       8-bit parallel high byte
 *   - LCD_MCU16:       16-bit parallel 8080
 */
#include "interface.h"
#include "lcd.h"

extern LCD_CONFIG lcdconfig;

/* ==================================================================
 * CS pin handler - supports both PC3 (default) and PB8 (SPI4Line_CS_D8)
 * ================================================================== */
void CS_SET(void)
{
    if (lcdconfig.lcd_interface == SPI4Line_CS_D8)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_SET);
}

void CS_CLR(void)
{
    if (lcdconfig.lcd_interface == SPI4Line_CS_D8)
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LCD_CS_PORT, LCD_CS_PIN, GPIO_PIN_RESET);
}

/* ==================================================================
 * 3-Line SPI (active mode)
 * D/C is encoded as: SDA=0 before first SCK → Command
 *                    SDA=1 before first SCK → Data
 * ================================================================== */
#ifdef LCD_SPI3Line

void SendDataSPI(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (dat & 0x80)
            LCD_SPI_SDA_SET();
        else
            LCD_SPI_SDA_CLR();
        dat <<= 1;
        LCD_SPI_SCL_CLR();
        LCD_SPI_SCL_SET();
    }
}

void WriteComm(uint16_t data)
{
    LCD_CS_CLR();

    LCD_SPI_SDA_CLR();      /* D/C = 0: command */
    LCD_SPI_SCL_CLR();
    LCD_SPI_SCL_SET();

    SendDataSPI((uint8_t)data);

    LCD_CS_SET();
}

void WriteData(uint16_t data)
{
    LCD_CS_CLR();

    LCD_SPI_SDA_SET();      /* D/C = 1: data */
    LCD_SPI_SCL_CLR();
    LCD_SPI_SCL_SET();

    SendDataSPI((uint8_t)data);

    LCD_CS_SET();
}

void SendData(unsigned int color)
{
    WriteData(color >> 8);
    WriteData(color);
}

#endif /* LCD_SPI3Line */

/* ==================================================================
 * 4-Line SPI (separate RS pin for D/C)
 * ================================================================== */
#ifdef LCD_SPI4Line

void SendDataSPI(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        LCD_SPI_SCL_CLR();      /* SCL low first */
        if (dat & 0x80)
            LCD_SPI_SDA_SET();
        else
            LCD_SPI_SDA_CLR();
        LCD_SPI_SCL_SET();      /* SCL high (rising edge latches data) */
        dat <<= 1;
    }
}

void WriteComm(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RS_CLR();           /* RS = 0: command */

#ifdef commdata16
    SendDataSPI(data >> 8);
#endif
    SendDataSPI((uint8_t)data);

    LCD_CS_SET();
}

void WriteData(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RS_SET();           /* RS = 1: data */

#ifdef commdata16
    SendDataSPI(data >> 8);
#endif
    SendDataSPI((uint8_t)data);

    LCD_CS_SET();
}

void SendData(unsigned int color)
{
    LCD_CS_CLR();
    LCD_RS_SET();           /* RS = 1: data */
    SendDataSPI(color >> 8);
    SendDataSPI((uint8_t)color);
    LCD_CS_SET();
}

#endif /* LCD_SPI4Line */

#ifdef LCD_SPI4Line_CS_D8
/* Identical to LCD_SPI4Line but CS is on PB8 instead of PC3.
 * The CS_SET/CS_CLR functions above handle the pin selection
 * based on lcdconfig.lcd_interface. The WriteComm/WriteData/SendData
 * functions are identical to LCD_SPI4Line, so we reuse that block. */
#endif

/* ==================================================================
 * 3-Line SPI Dual SDA mode
 * ================================================================== */
#ifdef LCD_SPI3Line2

void SendDataSPI(uint8_t dat)
{
    uint8_t i;
    for (i = 0; i < 8; i++) {
        if (dat & 0x80)
            LCD_SPI_SDA_SET();
        else
            LCD_SPI_SDA_CLR();
        dat <<= 1;
        LCD_SPI_SCL_CLR();
        LCD_SPI_SCL_SET();
    }
}

void WriteComm(uint16_t data)
{
    LCD_CS_CLR();
    LCD_SPI_SDA_CLR();
    LCD_SPI_SCL_CLR();
    LCD_SPI_SCL_SET();
    SendDataSPI((uint8_t)data);
    LCD_CS_SET();
}

void WriteData(uint16_t data)
{
    LCD_CS_CLR();
    LCD_SPI_SDA_SET();
    LCD_SPI_SCL_CLR();
    LCD_SPI_SCL_SET();
    SendDataSPI((uint8_t)data);
    LCD_CS_SET();
}

void SendData(unsigned int color)
{
    uint8_t i;
    uint8_t dat1 = color >> 8;
    uint8_t dat2 = (uint8_t)color;

    LCD_SPI_SDA_SET();
    LCD_SPI_SDA2_SET();
    LCD_SPI_SCL_CLR();
    LCD_SPI_SCL_SET();

    for (i = 0; i < 8; i++) {
        if (dat1 & 0x80)
            LCD_SPI_SDA_SET();
        else
            LCD_SPI_SDA_CLR();
        if (dat2 & 0x80)
            LCD_SPI_SDA2_SET();
        else
            LCD_SPI_SDA2_CLR();
        dat1 <<= 1;
        dat2 <<= 1;
        LCD_SPI_SCL_CLR();
        LCD_SPI_SCL_SET();
    }
}

#endif /* LCD_SPI3Line2 */

/* ==================================================================
 * 8-bit Parallel 8080 (MCU8)
 * ================================================================== */
#ifdef LCD_MCU8

void WriteComm(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RD_SET();
    LCD_RS_CLR();

#ifdef commdata16
    DATAOUT(data >> 8);
    LCD_WR_CLR();
    LCD_WR_SET();
#endif

    DATAOUT((uint8_t)data);
    LCD_WR_CLR();
    LCD_WR_SET();

    LCD_CS_SET();
}

void WriteData(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();

#ifdef commdata16
    DATAOUT(data >> 8);
    LCD_WR_CLR();
    LCD_WR_SET();
#endif

    DATAOUT((uint8_t)data);
    LCD_WR_CLR();
    LCD_WR_SET();

    LCD_CS_SET();
}

void SendData(unsigned int color)
{
    DATAOUT(color >> 8);
    LCD_WR_CLR();
    LCD_WR_SET();

    DATAOUT((uint8_t)color);
    LCD_WR_CLR();
    LCD_WR_SET();
}

#endif /* LCD_MCU8 */

/* ==================================================================
 * 8-bit Parallel High Byte (MCUH8)
 * ================================================================== */
#ifdef LCD_MCUH8

void WriteComm(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RD_SET();
    LCD_RS_CLR();
    DATAOUT(data << 8);
    LCD_WR_CLR();
    LCD_WR_SET();
    LCD_CS_SET();
}

void WriteData(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();
    DATAOUT(data << 8);
    LCD_WR_CLR();
    LCD_WR_SET();
    LCD_CS_SET();
}

void SendData(unsigned int color)
{
    DATAOUT(color);
    LCD_WR_CLR();
    LCD_WR_SET();

    DATAOUT(color << 8);
    LCD_WR_CLR();
    LCD_WR_SET();
}

#endif /* LCD_MCUH8 */

/* ==================================================================
 * 16-bit Parallel 8080 (MCU16)
 * ================================================================== */
#ifdef LCD_MCU16

void WriteComm(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RD_SET();
    LCD_RS_CLR();
    DATAOUT(data);
    LCD_WR_CLR();
    LCD_WR_SET();
    LCD_CS_SET();
}

void WriteData(uint16_t data)
{
    LCD_CS_CLR();
    LCD_RD_SET();
    LCD_RS_SET();
    DATAOUT(data);
    LCD_WR_CLR();
    LCD_WR_SET();
    LCD_CS_SET();
}

void SendData(unsigned int color)
{
    DATAOUT(color);
    LCD_WR_CLR();
    LCD_WR_SET();
}

#endif /* LCD_MCU16 */

/* ==================================================================
 * Convenience: Write command + data in one call
 * ================================================================== */
void WriteCommData(uint16_t com, uint16_t data)
{
    WriteComm(com);
    WriteData(data);
}
