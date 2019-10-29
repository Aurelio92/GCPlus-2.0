#ifndef NVM_H
#define NVM_H

void NVMUnlock(void);
void PGMEraseRow(uint16_t addr);
void PGMWriteBlock(uint16_t addr, uint8_t* data);
void PGMReadBlock(uint16_t addr, uint8_t* data);
uint8_t EEPROMReadByte(uint8_t address);
void EEPROMWriteByte(uint8_t address, uint8_t data);

#endif