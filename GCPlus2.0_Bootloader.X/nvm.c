#include "main.h"

void NVMUnlock(void) {
    INTCON0bits.GIE = 0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    INTCON0bits.GIE = 1;
}

void PGMEraseRow(uint16_t addr) {
    if (addr & 63)
        return;
    if (addr < PAYLOAD_ADDR || (addr + 64) > 0x8000)
        return;

    NVMCON1 = 0;
    TBLPTRU = 0;
    TBLPTRH = (addr >> 8) & 0xFF;
    TBLPTRL = addr & 0xFF;
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;
    NVMCON1bits.FREE = 1;
    NVMCON1bits.WREN = 1;
    NVMUnlock();
    NVMCON1bits.WREN = 0;
}

void PGMWriteBlock(uint16_t addr, uint8_t* data) {
    if (addr & 63)
        return;
    if (addr < PAYLOAD_ADDR || (addr + 64) > 0x8000)
        return;

    uint16_t i;

    /*NVMCON1 = 0;

    //Load address
    TBLPTRU = 0;
    TBLPTRH = (addr >> 8) & 0xFF;
    TBLPTRL = addr & 0xFF;*/

    PGMEraseRow(addr);

    //Copy block to the latches
    asm("tblrd*-");
    for (i = 0; i < 64; i++) {
        TABLAT = data[i];
        asm("tblwt+*");
    }
    //Write block
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;
    NVMCON1bits.WREN = 1;
    NVMCON1bits.FREE = 0;

    NVMUnlock();

    NVMCON1bits.WREN = 0;
}

void PGMReadBlock(uint16_t addr, uint8_t* data) {
    if (addr & 63)
        return;
    if (addr < PAYLOAD_ADDR || (addr + 64) > 0x8000)
        return;

    uint16_t i;

    //Load address
    TBLPTRU = 0;
    TBLPTRH = (addr >> 8) & 0xFF;
    TBLPTRL = addr & 0xFF;

    //Select PGM
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;

    //Read block from the latches
    for (i = 0; i < 64; i++) {
        asm("tblrd*+");
        data[i] = TABLAT;
    }
}

uint8_t EEPROMReadByte(uint8_t address) {
    NVMCON1 = 0;
    NVMADRL = address;
    NVMCON1bits.RD = 1;
    return NVMDAT;
}

void EEPROMWriteByte(uint8_t address, uint8_t data) {
    NVMCON1 = 0;
    NVMADRL = address;
    NVMDAT = data;
    NVMCON1bits.WREN = 1;
    NVMUnlock();
    while (NVMCON1bits.WR);
    NVMCON1bits.WREN = 0;
}
