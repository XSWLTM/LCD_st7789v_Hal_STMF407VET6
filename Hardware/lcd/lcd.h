/**
 * lcd.h - LCD driver for ST7789V on STM32F407VET6 HAL
 *
 * Provides display API: init, color fills, text rendering, drawing.
 * Pin definitions are in sys.h.
 */
#ifndef __LCD_H
#define __LCD_H

#include "sys.h"

/* ==================================================================
 * LCD Interface Enumeration
 * ================================================================== */
enum LCD_INTERFACE {
    MCU16 = 0,
    SPI4Line,
    MCU8,
    MCUH8,
    SPI3Line,
    SPI3Line2,
    SPI4Line_CS_D8
};

/* ==================================================================
 * LCD Configuration Structure
 * ================================================================== */
typedef struct {
    int col;                    /* Display width */
    int row;                    /* Display height */
    int col_pre;                /* Column offset */
    int row_pre;                /* Row offset */
    int delay_time;             /* Default delay time */
    uint8_t is_commdata16;      /* 16-bit command mode flag */
    enum LCD_INTERFACE lcd_interface;
    uint32_t pic_bytes;         /* Bytes per picture in flash */
} LCD_CONFIG;

/* ==================================================================
 * Extern declarations
 * ================================================================== */
extern LCD_CONFIG lcdconfig;
extern uint16_t POINT_COLOR;
extern uint16_t BACK_COLOR;

/* ==================================================================
 * LCD API Functions
 * ================================================================== */
void LCD_GPIOInit(void);
void LCD_CONFIG_INIT(void);
void LCD_RESET(void);
void LCD_Init(void);

/* Color / display functions */
void DispColor(unsigned int color);
void DispBand(void);
void DispFrame(void);
void DispGrayHor16(void);
void DispGrayHor32(void);
void DispQiPan(void);
void DispBlock(void);

/* Pixel-level */
void WriteOneDot(unsigned int color);
void WriteDispData(unsigned int color1, unsigned int color2);
void LCD_Fast_DrawPoint(uint16_t x, uint16_t y, uint16_t color);
void LCD_DrawPoint(uint16_t x, uint16_t y);

/* Cursor */
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);

/* Text */
void LCD_ShowChar(uint16_t x, uint16_t y, uint8_t num, uint8_t size, uint8_t mode);
void LCD_ShowString(uint16_t x, uint16_t y, uint16_t width, uint16_t height,
                    uint8_t size, uint8_t *p);
void Show_Str(uint16_t x, uint16_t y, uint16_t fc, uint16_t bc,
              uint8_t *str, uint8_t size, uint8_t mode);

/* Drawing */
void PutPixel(unsigned int x, unsigned int y, unsigned int color);
void DrawLine(unsigned int Xstart, unsigned int Xend,
              unsigned int Ystart, unsigned int Yend, unsigned int color);
void DrawGird(unsigned int color);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/* 9-bit color mode (H9Bit) */
void DispColor_9bit_window(uint8_t r, uint8_t g, uint8_t b);
void DispBand_9b(void);
void DispFrame_9b(void);
void DispGrayHor16_9b(void);
void DispChess(uint8_t div);
void TEST_STAND_9b(void);

#endif /* __LCD_H */
