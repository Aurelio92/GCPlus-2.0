#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "si.h"
#include "adc.h"
#include "buttons.h"

#define ABS(n) ((n >= 0) ? (n) : (-n))

void clockInit(void);
void portsInit(void);
void updateMode(void);
uint8_t EEPROMRead(uint8_t address);
void EEPROMWrite(uint8_t address, uint8_t data);
void LUTBuild(uint8_t* LUT, uint8_t minVal, uint8_t maxVal, uint8_t origin, uint8_t dz, uint8_t invert);

extern uint8_t ADCValues[ADC_NCHANNELS];
extern outButtons_t outButtons;

uint8_t bootloaderMode;

__EEPROM_DATA(0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00, 0xFF, 0x00);
__EEPROM_DATA(0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);

void main(void) {
    uint8_t i;
    char cmd[16];
    uint8_t cmdLen = 0;
    char msg0x00[] = {0x09, 0x00, 0x03};
    char msgTemp[] = {0x00, 0xFF, 0x03};
    char msg0x40[] = {0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00};
    char msg0x41[] = {0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00};
    char msgGen[10];
    uint32_t magic = 0;
    inButtons_t tempButtons;

    uint8_t LUT_SX[256];
    uint8_t LUT_SY[256];
    uint8_t LUT_CX[256];
    uint8_t LUT_CY[256];

    uint8_t invert;
    uint8_t deadzone;
    uint8_t rumble;

    clockInit();
    portsInit();

    //Reset ranges
    if (!tempButtons.X && !tempButtons.Y && !tempButtons.A) {
        EEPROMWrite(0x00, 0xFF);
        EEPROMWrite(0x01, 0x00);
        EEPROMWrite(0x02, 0xFF);
        EEPROMWrite(0x03, 0x00);
        EEPROMWrite(0x04, 0xFF);
        EEPROMWrite(0x05, 0x00);
        EEPROMWrite(0x06, 0xFF);
        EEPROMWrite(0x07, 0x00);
        EEPROMWrite(0x09, 0x18);
    }

    SIInit();
    ADCInit();

    //Wait 5ms to make sure the ADC has valid data
    T4CLK = 0x01; //FOSC/4 (16MHz)
    T4RST = 0x00;
    T4TMR = 0x00;
    T4PR = 125;
    T4CON = 0xF5; //T4ON = 0. Prescaler = 1:128. Postscaler = 1:5
    PIR7bits.TMR4IF = 0;
    while(!PIR7bits.TMR4IF);
    T4CON = 0;

    buttonsInit();
    //INTCON0 = 0xE0; //Low/High interrupts enabled
    INTCON0 = 0x80; //Interrupts enabled with no priority

/*
    //Check for magic string at 0x2000
    NVMCON1 = 0;
    NVMCON1bits.REG0 = 0;
    NVMCON1bits.REG1 = 1;
    //Load address
    TBLPTRU = 0;
    TBLPTRH = 0x20;
    TBLPTRL = 0x00;
    magic = 0;
    asm("tblrd*+");
    magic |= TABLAT;
    magic <<= 8;
    asm("tblrd*+");
    magic |= TABLAT;
    magic <<= 8;
    asm("tblrd*+");
    magic |= TABLAT;

    tempButtons.PORTA = PORTA;
    tempButtons.PORTB = PORTB;
    tempButtons.PORTC = PORTC;

    bootloaderMode = 0;
    if (magic == 0x47432B) {
        if (!tempButtons.X && !tempButtons.Y && !tempButtons.Z) { //Safe/bootloader mode
            bootloaderMode = 1;
        }
    } else {
        bootloaderMode = 1;
    }

    if (!bootloaderMode) {
        asm("goto 0x2040");
    }*/

    invert = EEPROMRead(0x08);
    deadzone = EEPROMRead(0x09);
    rumble = EEPROMRead(0x0A);

    if (deadzone > 0x30) {
        deadzone = 0x18;
        EEPROMWrite(0x09, 0x18);
    }

    //Build LUTs
    LUTBuild(LUT_SX, EEPROMRead(0x01), EEPROMRead(0x00), ADCValues[0], deadzone, invert & 0x01);
    LUTBuild(LUT_SY, EEPROMRead(0x03), EEPROMRead(0x02), ADCValues[1], deadzone, invert & 0x02);
    LUTBuild(LUT_CX, EEPROMRead(0x05), EEPROMRead(0x04), ADCValues[2], deadzone, invert & 0x04);
    LUTBuild(LUT_CY, EEPROMRead(0x07), EEPROMRead(0x06), ADCValues[3], deadzone, invert & 0x08);

    /*while(1){
        SISendMessage(msgTemp, 3);
        //Wait 80us to make sure the ADC has valid data
        T4CLK = 0x01; //FOSC/4 (16MHz)
        T4RST = 0x00;
        T4TMR = 0x00;
        T4PR = 20;
        T4CON = 0xF5; //T4ON = 1. Prescaler = 1:128. Postscaler = 1:5
        PIR7bits.TMR4IF = 0;
        while(!PIR7bits.TMR4IF);
        T4CON = 0;
    }*/

    while (1) {
        buttonsUpdate();
        cmdLen = SIGetCommand(cmd);
        if (cmdLen > 0) {
            switch(cmd[0]) {
                case 0x40:
                    msg0x40[0] = outButtons.byte0;
                    msg0x40[1] = outButtons.byte1;
                    msg0x40[2] = LUT_SX[ADCValues[0]];
                    msg0x40[3] = LUT_SY[ADCValues[1]];
                    msg0x40[4] = LUT_CX[ADCValues[2]];
                    msg0x40[5] = LUT_CY[ADCValues[3]];
                    msg0x40[6] = outButtons.byte6;
                    msg0x40[7] = outButtons.byte7;
                    SISendMessage(msg0x40, 8);
                break;

                case 0x00:
                case 0xFF:
                    SISendMessage(msg0x00, 3);
                break;

                case 0x41:
                case 0x42:
                    SISendMessage(msg0x41, 10);
                break;

                case 0x1F: //Read ranges+invert
                    for (i = 0; i < 9; i++) {
                        msgGen[i] = EEPROMRead(i);
                    }
                    SISendMessage(msgGen, 9);
                break;

                case 0x20: //Write ranges+invert
                    for (i = 0; i < 9; i++) {
                        EEPROMWrite(i, cmd[i + 1]);
                    }
                    asm("reset");
                break;

                case 0x21: //Write DZ
                    deadzone = cmd[1];
                    EEPROMWrite(0x09, deadzone);
                    asm("reset");
                break;

                case 0x22: //Read DZ
                    msgGen[0] = deadzone;
                    SISendMessage(msgGen, 1);
                break;

                case 0x23: //Write rumble
                    rumble = cmd[1];
                    EEPROMWrite(0x0A, rumble);
                break;

                case 0x24: //Read rumble
                    msgGen[0] = rumble;
                    SISendMessage(msgGen, 1);
                break;

                case 0x25: //Update mode
                    updateMode();
                break;

                case 0x30: //Read firmware version
                    msgGen[0] = 2;
                    msgGen[1] = 0;
                    SISendMessage(msgGen, 2);
                break;

                case 0x31: //Read board version
                    msgGen[0] = 2;
                    msgGen[1] = 0;
                    SISendMessage(msgGen, 2);
                break;

                default:
                    SIClear();
                break;
            }
        }
    }
}

void clockInit(void) {
    // NOSC HFINTOSC; NDIV 1;
    OSCCON1 = 0x60;
    // CSWHOLD may proceed; SOSCPWR Low power;
    OSCCON3 = 0x00;
    // MFOEN disabled; LFOEN disabled; ADOEN disabled; SOSCEN disabled; EXTOEN disabled; HFOEN disabled;
    OSCEN = 0x00;
    // HFFRQ 64_MHz;
    OSCFRQ = 0x08;
    // TUN 0;
    OSCTUNE = 0x00;
}

void portsInit(void) {
    bool state = (unsigned char)GIE;
    GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x00; // unlock PPS

    //Data line wired to RB2
    //Reception
    T6INPPS = 0x0A; //RB2 to T6INPPS. Used as reset trigger
    /*//SPI1SDIPPS = 0x0F; //RB7 to SDI (fucking Microchip)
    SPI1SDIPPS = 0x08; //RB0 to SDI (fucking Microchip)
    //RB7PPS = 0x09; //CCP1 to RB7
    RB0PPS = 0x09; //CCP1 to RB0
    SPI1SCKPPS = 0x0A; //RB2 to SCK*/
    SMT1SIGPPS = 0x0A; //RB2

    //Transmission
    RB2PPS = 0x04; //CLC4 to RB2

    //RA6PPS = 0x02; //CLC2 to RA0

    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0x01; // lock PPS

    GIE = state;
    //TRISA = 0xFF;
    //TRISA = 0x3F; //RA6 and RA7 as output
    //TRISB = 0x7F; //RB7 as output
    //TRISB = 0xFC; //RB0 and RB1 as output
    TRISA = 0xFF;
    TRISB = 0xFF;
    TRISC = 0xFF;
    ANSELA = 0x0F;
    ANSELB = 0x00;
    ANSELC = 0x00;
    WPUA = 0xF0;
    WPUB = 0x23; //0x27;
    WPUC = 0xFF;
    ODCONA = 0x00;
    ODCONB = 0x04;//4;
    ODCONC = 0x00;
}

void LUTBuild(uint8_t* LUT, uint8_t minVal, uint8_t maxVal, uint8_t origin, uint8_t dz, uint8_t invert) {
    int16_t i;
    int16_t range = ((int16_t)maxVal - (int16_t)minVal) / 2;

    for (i = 0; i < 256; i++) {
        int16_t radius = i - origin;
        if (invert) radius = -radius;
        if (ABS(radius) < (int16_t)dz) {
            LUT[i] = 0x80;
        } else {
            int16_t tempVal = radius * 128 / range;
            tempVal += 128;
            if (tempVal < 0) tempVal = 0;
            if (tempVal > 0xFF) tempVal = 0xFF;
            LUT[i] = (uint8_t)tempVal & 0xFF;
        }
    }
}

uint8_t EEPROMRead(uint8_t address) {
    NVMCON1 = 0;
    NVMADRL = address;
    NVMCON1bits.RD = 1;
    return NVMDAT;
}

void EEPROMWrite(uint8_t address, uint8_t data) {
    NVMCON1 = 0;
    NVMADRL = address;
    NVMDAT = data;
    NVMCON1bits.WREN = 1;
    INTCON0bits.GIE = 0;
    NVMCON2 = 0x55;
    NVMCON2 = 0xAA;
    NVMCON1bits.WR = 1;
    INTCON0bits.GIE = 1;
    while (NVMCON1bits.WR);
    NVMCON1bits.WREN = 0;
}

void updateMode(void) {
    uint8_t cmd[16];
    uint8_t cmdLen = 0;
    uint8_t msg[4];
    uint8_t buffer[64];
    uint8_t bufferIdx = 0;
    int16_t checksum = 0;
    uint8_t i;

    PR0 = 160; //10us

    WDTCON0bits.SEN = 0;

    while (1) {
        cmdLen = SIGetCommand(cmd);
        if (cmdLen > 0) {
            switch(cmd[0]) {
                case 0x10: //Reset write index
                    bufferIdx = 0;
                    msg[0] = 0x00;
                    SISendMessage(msg, 1);
                break;

                case 0x11: //Write 4 bytes to buffer
                    if (bufferIdx >= 64) {
                        msg[0] = 0x01;
                        SISendMessage(msg, 3);
                    } else {
                        checksum = 0;
                        for (i = 0; i < 8; i++) {
                            buffer[bufferIdx + i] = cmd[1 + i];
                            checksum += cmd[1 + i];
                        }
                        checksum = ~checksum;
                        checksum++;

                        //Compare checksum
                        if ((checksum & 0xFF) == cmd[9]) { //Checksum is valid
                            bufferIdx += 8;
                            msg[0] = 0x00; //OK
                            SISendMessage(msg, 1);
                        } else {
                            msg[0] = 0x01; //Error wrong checksum
                            SISendMessage(msg, 1);
                        }
                    }
                break;

                case 0x12:
                    if (cmd[1] < 0x20 || cmd[1] >= 0x60) { //Out of range address
                        msg[0] = 0x02; //Error
                        SISendMessage(msg, 1);
                    } else {
                        NVMCON1 = 0;

                        //Load address
                        TBLPTRU = 0;
                        TBLPTRH = cmd[1];
                        TBLPTRL = cmd[2];

                        //Erase block
                        NVMCON1bits.REG0 = 0;
                        NVMCON1bits.REG1 = 1;
                        NVMCON1bits.WREN = 1;
                        NVMCON1bits.FREE = 1;

                        INTCON0bits.GIE = 0;
                        NVMCON2 = 0x55;
                        NVMCON2 = 0xAA;
                        NVMCON1bits.WR = 1;
                        INTCON0bits.GIE = 1;

                        //Copy block to the latches
                        asm("tblrd*-");
                        for (i = 0; i < 64; i++) {
                            TABLAT = buffer[i];
                            asm("tblwt+*");
                        }

                        //Write block
                        NVMCON1bits.REG0 = 0;
                        NVMCON1bits.REG1 = 1;
                        NVMCON1bits.WREN = 1;
                        NVMCON1bits.FREE = 0;

                        INTCON0bits.GIE = 0;
                        NVMCON2 = 0x55;
                        NVMCON2 = 0xAA;
                        NVMCON1bits.WR = 1;
                        INTCON0bits.GIE = 1;

                        NVMCON1bits.WREN = 0;

                        msg[0] = 0x00; //OK
                        SISendMessage(msg, 1);
                    }
                break;

                case 0x13:
                    asm("reset");
                break;
            }
        }
    }
}
