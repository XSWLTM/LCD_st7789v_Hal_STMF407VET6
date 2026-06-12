/**
 * blockwrite_default.h - Set LCD write window
 *
 * Sets the column/row address window using CASET (0x2A) and RASET (0x2B)
 * commands, then starts RAM write with RAMWR (0x2C).
 */
#ifndef __BLOCKWRITE_DEFAULT_H
#define __BLOCKWRITE_DEFAULT_H

#include "interface.h"

static void BlockWrite(unsigned int Xstart, unsigned int Xend,
                       unsigned int Ystart, unsigned int Yend)
{
    /* CASET: Column Address Set (0x2A)
     * Xstart[15:8], Xstart[7:0], Xend[15:8], Xend[7:0] */
    WriteComm(0x2A);
    WriteData((Xstart) >> 8);
    WriteData((Xstart));
    WriteData((Xend) >> 8);
    WriteData(Xend);

    /* RASET: Row Address Set (0x2B)
     * Ystart[15:8], Ystart[7:0], Yend[15:8], Yend[7:0] */
    WriteComm(0x2B);
    WriteData((Ystart) >> 8);
    WriteData((Ystart));
    WriteData((Yend) >> 8);
    WriteData(Yend);

    /* RAMWR: Memory Write (0x2C) - subsequent writes go to GRAM */
    WriteComm(0x2C);
}

#endif /* __BLOCKWRITE_DEFAULT_H */
