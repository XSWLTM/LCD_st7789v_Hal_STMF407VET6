/**
 * interface.h - LCD interface protocol layer
 *
 * Provides WriteComm(), WriteData(), SendData() for multiple
 * interface modes (SPI3Line, SPI4Line, MCU8, MCU16, etc.)
 * Selected at compile-time via defines in main.h
 */
#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "sys.h"

void CS_SET(void);
void CS_CLR(void);
void WriteComm(uint16_t data);      /* Write command byte */
void WriteData(uint16_t data);      /* Write data byte */
void SendData(unsigned int color);  /* Send 16-bit pixel data */
void SendDataSPI(uint8_t dat);      /* Send 8-bit SPI byte */
void WriteCommData(uint16_t com, uint16_t data);

#endif
