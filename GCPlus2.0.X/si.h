#ifndef _SI_H_
#define _SI_H_

void SIInit(void);
void SIConfigureCLC(void);
void SISendMessage(uint8_t* msg, uint8_t len);
uint8_t SIGetCommand(uint8_t* msg);
uint8_t SIGetCommandCode(uint8_t* msg);
uint8_t SICommadReceived(void);
void SIClear(void);

#endif