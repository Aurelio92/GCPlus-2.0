#include "main.h"

config_t config;

void configInit(void) {
    uint8_t* data = (uint8_t*)&config;
    uint8_t i;
    for (i = 0; i < sizeof(config_t); i++) {
        asm("clrwdt");
        data[i] = EEPROMReadByte(i);

        //Check magic code
        if (i == 3U && config.magicCode != 0x322B4347UL) {
            //Magic code wrong. Set default configuration
            configSetDefault();
            configFlashAll();
        }
    }
}

void configSetDefault(void) {
    //Clear all parameters
    memset((void*)&config, 0, sizeof(config_t));

    //Header
    config.magicCode = 0x322B4347UL;
    config.version = GCP2_VERSION;

    //Sticks configuration
    config.SXMin = 0x00;
    config.SXMax = 0xFF;
    config.SYMin = 0x00;
    config.SYMax = 0xFF;
    config.CXMin = 0x00;
    config.CXMax = 0xFF;
    config.CYMin = 0x00;
    config.CYMax = 0xFF;
    config.SXChan = 0x02;
    config.SYChan = 0x03;
    config.CXChan = 0x00;
    config.CYChan = 0x01;
    config.axInvert = 0x00;
    config.SDeadzone = 0x18;
    config.CDeadzone = 0x18;
    config.deadzoneMode = DZ_MODE_SCALEDRADIAL;

    //Rumble
    config.rumbleIntensity = 0xFF;

    //Triggers mode
    config.triggersMode = TRIG_MODE_DIGITAL;
}

void configFlashAll(void) {
    uint8_t i;
    uint8_t* data = (uint8_t*)&config;
    for (i = 0; i < sizeof(config_t); i++) {
        EEPROMWriteByte(i, data[i]);
    }
}
