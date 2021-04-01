#ifndef _SI_H_
#define _SI_H_

//GCC standard commands
#define SI_CMD_ID       0x00
#define SI_CMD_POLL     0x40
#define SI_CMD_ORIGINS  0x41
#define SI_CMD_CALIB    0x42
#define SI_CMD_RESET    0xFF

//GC+2.0 custom commands
#define GCP_CMD_LOCKUNLOCK      0x10
#define GCP_CMD_GETVER          0x11
#define GCP_CMD_WRITEEEPROM     0x12
#define GCP_CMD_READEEPROM      0x13
#define GCP_CMD_RESET           0x14
#define GCP_CMD_BOOTBL          0x15
#define GCP_CMD_SETMAPBYTE0     0x16
#define GCP_CMD_SETMAPBYTE1     0x17
#define GCP_CMD_GETMAPBYTE0     0x18
#define GCP_CMD_GETMAPBYTE1     0x19
#define GCP_CMD_REBUILDLUT      0x20

//GC+2.0 bootloader specific commands
#define GCP_CMD_RESETIDX        0x70
#define GCP_CMD_FILLBUFFER      0x71
#define GCP_CMD_READBUFFER      0x72
#define GCP_CMD_WRITEFLASH      0x73
#define GCP_CMD_READFLASH       0x74
#define GCP_CMD_BOOTPAYLOAD     0x75

#define GCP_ERR_NONE        0x00
#define GCP_ERR_LOCKED      0xFF
#define GCP_ERR_WRONGARG    0xFE
#define GCP_ERR_WRONGMODE   0xFD

//Rumble
#define PAD_MOTOR_STOP          0
#define PAD_MOTOR_RUMBLE        1
#define PAD_MOTOR_STOP_HARD     2


void SIInit(void);
void SIConfigureCLC(void);
void SISendMessage(uint8_t* msg, uint8_t len);
uint8_t SIGetCommand(uint8_t* msg);
uint8_t SIGetCommandCode(uint8_t* msg);
uint8_t SICommadReceived(void);
void SIClear(void);

#endif
