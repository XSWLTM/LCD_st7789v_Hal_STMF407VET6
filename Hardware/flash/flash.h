/**
 * flash.h - W25Q16 SPI Flash driver (HAL version)
 *
 * Provides read/write/erase operations and picture display functions.
 * Flash CS is on PA4, controlled via software GPIO.
 */
#ifndef __FLASH_H
#define __FLASH_H

#include "sys.h"

/* W25Q16 Manufacturer/Device ID */
#define W25Q16          0xEF14
#define SPI_FLASH_TYPE  W25Q16
#define FLASH_ID        0xEF14

/* W25X series command set */
#define W25X_WriteEnable        0x06
#define W25X_WriteDisable       0x04
#define W25X_ReadStatusReg      0x05
#define W25X_WriteStatusReg     0x01
#define W25X_ReadData           0x03
#define W25X_FastReadData       0x0B
#define W25X_FastReadDual       0x3B
#define W25X_PageProgram        0x02
#define W25X_BlockErase         0xD8
#define W25X_SectorErase        0x20
#define W25X_ChipErase          0xC7
#define W25X_PowerDown          0xB9
#define W25X_ReleasePowerDown   0xAB
#define W25X_DeviceID           0xAB
#define W25X_ManufactDeviceID   0x90
#define W25X_JedecDeviceID      0x9F

/* Flash CS control - defined in sys.h, reusing here */
/* (No local definition needed - sys.h provides SPI_FLASH_CS_SET/CLR) */

/* Picture info structure */
typedef struct {
    uint8_t  pic_format;
    uint8_t  pic_num;
    uint16_t pic_width;
    uint16_t pic_height;
    uint32_t pic_bytes;
} PIC_INFO;

extern PIC_INFO pic_info;
extern uint32_t pic_start_addr;
extern uint8_t start_x;
extern uint8_t is_Fread;

/* Flash operations */
void SPI_Flash_Init(void);
uint16_t SPI_Flash_ReadID(void);
uint8_t  SPI_Flash_ReadSR(void);
void     SPI_FLASH_Write_SR(uint8_t sr);
void     SPI_FLASH_Write_Enable(void);
void     SPI_FLASH_Write_Disable(void);
void     SPI_Flash_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void     SPI_Flash_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void     SPI_Flash_Write_NoCheck(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void     SPI_Flash_Erase_Chip_ALL(void);
void     SPI_Flash_Erase_Sector(uint32_t Dst_Addr);
void     SPI_Flash_Wait_Busy(void);
void     SPI_Flash_PowerDown(void);
void     SPI_Flash_WAKEUP(void);

/* Picture display from Flash */
void Read_FlashPic(uint32_t ReadAddr, uint32_t NumByteToRead);
void Disp_Flash_Pic_Num(uint8_t num);
void Read_FlashPic_H(uint32_t ReadAddr, uint32_t NumByteToRead);
void Read_FlashPic_9b(uint32_t ReadAddr, uint32_t NumByteToRead);
void Disp_Flash_Pic_9b_Num(uint8_t num);
void DisPic_Windows_From_FLASH(uint32_t ReadAddr, uint32_t NumByteToRead);

#endif
