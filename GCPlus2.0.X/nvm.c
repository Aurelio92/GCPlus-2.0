#include "main.h"

void NVMUnlock(void) {
    INTCON0bits.GIE = 0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    INTCON0bits.GIE = 1;
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
