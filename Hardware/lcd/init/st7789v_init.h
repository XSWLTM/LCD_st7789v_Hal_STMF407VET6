/**
 * st7789v_init.h - ST7789V LCD controller initialization sequence
 *
 * Exact copy from vendor reference: 002_003_ST7789V_2.8TN.h
 */
#ifndef __ST7789V_INIT_H
#define __ST7789V_INIT_H

#include "interface.h"
#include "delay.h"

static void LCD_IC_Init(void)
{
    WriteComm(0x01);
    Delay(5);

    WriteComm(0x11);
    Delay(120);

    WriteComm(0x36);
    WriteData(0x00);

    WriteComm(0x3a);
    WriteData(0x55);

    WriteComm(0xb0);
    WriteData(0x00);
    WriteData(0xe0);

    WriteComm(0xb2);
    WriteData(0x0c);
    WriteData(0x0c);
    WriteData(0x00);
    WriteData(0x33);
    WriteData(0x33);

    WriteComm(0xb7);
    WriteData(0x75);

    WriteComm(0xbb);
    WriteData(0x13);

    WriteComm(0xc0);
    WriteData(0x2c);

    WriteComm(0xc2);
    WriteData(0x01);

    WriteComm(0xc3);
    WriteData(0x11);

    WriteComm(0xc5);
    WriteData(0x2c);

    WriteComm(0xc4);
    WriteData(0x20);

    WriteComm(0xc6);
    WriteData(0x0f);

    WriteComm(0xd0);
    WriteData(0xa4);
    WriteData(0xa1);

    WriteComm(0xE0);
    WriteData(0xD0);
    WriteData(0x1A);
    WriteData(0x1E);
    WriteData(0x0A);
    WriteData(0x0A);
    WriteData(0x27);
    WriteData(0x3B);
    WriteData(0x44);
    WriteData(0x4A);
    WriteData(0x2B);
    WriteData(0x16);
    WriteData(0x15);
    WriteData(0x1A);
    WriteData(0x1E);

    WriteComm(0xE1);
    WriteData(0xD0);
    WriteData(0x1A);
    WriteData(0x1E);
    WriteData(0x0A);
    WriteData(0x0A);
    WriteData(0x27);
    WriteData(0x3A);
    WriteData(0x43);
    WriteData(0x49);
    WriteData(0x2B);
    WriteData(0x16);
    WriteData(0x15);
    WriteData(0x1A);
    WriteData(0x1D);

    WriteComm(0x21);   // 开启反相
    WriteComm(0x29);
}

#endif /* __ST7789V_INIT_H */
