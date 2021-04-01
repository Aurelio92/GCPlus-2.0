#ifndef _NVM_H_
#define _NVM_H_

void NVMUnlock(void);
uint8_t EEPROMReadByte(uint8_t address);
void EEPROMWriteByte(uint8_t address, uint8_t data);

#endif
