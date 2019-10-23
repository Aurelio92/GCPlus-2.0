#include <xc.h>
#include <stdint.h>
#include "i2cslv.h"

#define SLV_ADDR 0x40

#define DAC_SX DAC1CON1
#define DAC_SY DAC3CON1
#define DAC_CX DAC2CON1
#define DAC_CY DAC4CON1

void i2cSlvInit(void) {
    //Configure i2c as slave. 7-bit address SLV_ADDR
    //Clock stretch enabled
    SSPSTAT = 0x80;
    SSPCON2 = 0x01;
    SSPCON3 = 0x00;
    SSPMSK = 0xFF;
    SSPADD = SLV_ADDR;
    SSPCON1 = 0x36;

    //Clear SSP interrupt flag and enable interrupt
    PIR1bits.SSP1IF = 0;
    PIE1bits.SSP1IE = 1;
}

void i2cSlvISR(void) {
    uint8_t i2cData;
    static uint8_t regAddress = 0;
    static uint8_t nextWrite = 0;
    static uint8_t gpioMemory[8] = {0xFF, 0xFF, 0xFF, 0x80, 0x80, 0x80, 0x80, 0x00};

    i2cData = SSPBUF; //Clear BF

    if (SSPSTATbits.R_nW == 1) {
        if((SSPSTATbits.D_nA == 1) && (SSPCON2bits.ACKSTAT == 1)) {
            //Read completed
        } else {
            if (regAddress < 8) {
                SSPBUF = gpioMemory[regAddress++];
            } else {
                SSPBUF = 0x00;
            }
        }

    } else if (SSPSTATbits.D_nA == 0) {
        nextWrite = 0;
    } else {
        if (nextWrite == 0) {
            regAddress = i2cData;
            nextWrite = 1;
        } else {
            if (regAddress < 8) {
                gpioMemory[regAddress++] = i2cData;
                LATA = gpioMemory[0];
                LATB = gpioMemory[1];
                LATC = gpioMemory[2];
                DAC_SX = gpioMemory[3];
                DAC_SY = gpioMemory[4] >> 3;
                DAC_CX = gpioMemory[5] >> 3;
                DAC_CY = gpioMemory[6] >> 3;
            }
        }
    }

    SSPCON2bits.ACKDT = 0;
    PIR1bits.SSP1IF = 0;
    SSPCON1bits.CKP = 1; //Release SCL
}