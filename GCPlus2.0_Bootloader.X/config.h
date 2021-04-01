#ifndef _CONFIG_H_
#define _CONFIG_H_

#define DZ_MODE_RADIAL          0
#define DZ_MODE_SCALEDRADIAL    1

#define TRIG_MODE_DIGITAL   0
#define TRIG_MODE_ANALOG    1

typedef struct {
    //Header
    uint32_t magicCode; //0x322B4347 = 'GC+2'
    uint16_t version;

    //OFFSET: 0x06
    //Sticks configuration
    uint8_t SXMin;
    uint8_t SXMax;
    uint8_t SYMin;
    uint8_t SYMax;
    uint8_t CXMin;
    uint8_t CXMax;
    uint8_t CYMin;
    uint8_t CYMax;
    uint8_t SXChan;
    uint8_t SYChan;
    uint8_t CXChan;
    uint8_t CYChan;

    //OFFSET: 0x12
    union {
        struct {
            uint8_t SXInvert : 1;
            uint8_t SYInvert : 1;
            uint8_t CXInvert : 1;
            uint8_t CYInvert : 1;
            uint8_t : 4; //padding
        };
        uint8_t axInvert;
    };
    //OFFSET: 0x13
    uint8_t SDeadzone;
    uint8_t CDeadzone;

    //0: radial. 1: scaled radial
    uint8_t deadzoneMode;

    //Rumble
    uint8_t rumbleIntensity;

    //Triggers mode: 0: digital, referred to GND. 1: analog
    uint8_t triggersMode;
} config_t;

extern config_t config;

void configInit(void);
void configSetDefault(void);
void configFlashAll(void);

#endif  /* CONFIG_H */
