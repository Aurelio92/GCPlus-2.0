#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "si.h"

static uint8_t SIInputMessage[0x24];
static uint8_t SIBitCounter = 0;
static uint8_t SIByteCounter = 0;
static uint8_t SICMDReceived = 0;
static uint8_t SITempByte = 0x00;

void __interrupt(base(0x200C)) hi_int(void) //High priority interrupt
{
    if (SMT1PWAIE && SMT1PWAIF)
    {
        SMT1PWAIF = 0;

        SITempByte <<= 1;
        SITempByte |= (SMT1CPWL < 128) ? 1U : 0U;

        //Clear flags to allow reception data with bit 7 set
        SICMDReceived = 0;
        T6TMR = 0;
        //PIR3bits.TMR0IF = 0;

        ++SIBitCounter;
        if (SIBitCounter == 8) {
            SIInputMessage[SIByteCounter] = SITempByte;
            SITempByte = 0x00;
            SIBitCounter = 0;
            ++SIByteCounter;
        }/* else if (SIBitCounter == 1) { //Check for stop bit
            if (SITempByte == 1 && SIByteCounter > 0) {
                SICMDReceived = 1;
            }
        }*/
    }

    if (PIR9bits.TMR6IF) {
        if (SIByteCounter) {
            SICMDReceived = 1;
        } else {
            SIClear();
        }
        PIR9bits.TMR6IF = 0;
    }
}

void SIInit(void) {
    //Set Timer6 as Falling Edge Reset. Free running with period = 5 us
    T6CON = 0x00;
    T6CLK = 0x01; //FOSC/4 (16MHz)
    T6RST = 0x00; //T6INPPS as reset source
    T6TMR = 0x00;
    T6PR = 0x50; //5us
    T6HLT = 0x05; //Falling edge reset. Free running
    PIR9bits.TMR6IF = 0;
    PIE9bits.TMR6IE = 1;

    //Use timer 2 to wait 1ms with no toggling on the data line
    //This will ensure that the bus is idle when we start the program
    T2CON = 0x00;
    T2CLK = 0x01; //FOSC/4 (16MHz)
    T2RST = 0x00;
    T2TMR = 0x00;
    T2PR = 125;
    PIR4bits.TMR2IF = 0;
    T2CON = 0xF0; //T2ON = 0. Prescaler = 1:128. Postscaler = 1:1
    while(!PIR4bits.TMR2IF) {
        if (PORTBbits.RB2 == 0) {
            T2TMR = 0x00;
        }
    }

    CCPTMRS1 = 0x55; //PWM5-8 source set to TMR2

    SIConfigureCLC();

    T6CON = 0x80; //ON. 1:1 prescaler. 1:1 postscaler

    //Setup SMT for input decoding
    //HIGH AND LOW MEASURE MODE REPEAT ACQUISITION
    //Use SMT1SIG as input
    //Interrupt on SMT1PWAIF
    SMT1PRL = 0xFF;
    SMT1PRH = 0xFF;
    SMT1PRU = 0xFF;
    SMT1TMRL = 0x00;
    SMT1TMRH = 0x00;
    SMT1TMRU = 0x00;
    SMT1CON0 = 0x88; //SMT1SIG active low
    SMT1CON1 = 0x43; //High and low time measurement
    SMT1CLK = 0x01; //FOSC
    SMT1WIN = 0x00;
    SMT1SIG = 0x00; //PPS //0x18; //CLC3OUT

    PIR1bits.SMT1PWAIF = 0;
    IPR1bits.SMT1PWAIP = 1; //High priority
    PIE1bits.SMT1PWAIE = 1;
    SMT1CON1bits.GO = 1;
}

void SIConfigureCLC(void) {
    T2CON = 0x00;
    T2CLKCON = 0x01; //FOSC/4 (16MHz)
    T2HLT = 0x04; //Resets at rising TMR2_ers
    T2RST = 0x12; //CLC2_out reset
    T2PR = 0x1F; //2us
    T2TMR = 0x00;
    T2CON = 0x80;

    //50%
    PWM5DCH = 0x0F;
    PWM5DCL = 0x00;
    PWM5CON = 0x80; //Enable PWM

    CLC1POL = 0x00;
    CLC1SEL0 = 0x18; //PWM5
    CLC1SEL1 = 0x05; //HFINTOSC
    CLC1SEL2 = 0x00; //Unused
    CLC1SEL3 = 0x00; //Unused
    CLC1GLS0 = 0x08; //HFINTOSC
    CLC1GLS1 = 0x02; //PWM5
    CLC1GLS2 = 0x00; //'0'
    CLC1GLS3 = 0x00; //'0'
    CLC1CON = 0x84; //D-FLIP-FLOP

    CLC2POL = 0x00;
    CLC2SEL0 = 0x2C; //SCK
    CLC2SEL1 = 0x05; //HFINTOSC
    CLC2SEL2 = 0x00; //Unused
    CLC2SEL3 = 0x00; //Unused
    CLC2GLS0 = 0x08; //HFINTOSC
    CLC2GLS1 = 0x01; //!SCK
    CLC2GLS2 = 0x00; //'0'
    CLC2GLS3 = 0x00; //'0'
    CLC2CON = 0x84; //D-FLIP-FLOP

    CLC3POL = 0x00;
    CLC3SEL0 = 0x24; //CLC1OUT
    CLC3SEL1 = 0x18; //PWM5
    CLC3SEL2 = 0x2B; //SDO
    CLC3SEL3 = 0x2C; //SCK
    CLC3GLS0 = 0x02; //CLC1OUT
    CLC3GLS1 = 0x04; //!PWM5
    CLC3GLS2 = 0x90; //(!SDO + SCK)
    CLC3GLS3 = 0x60; //(SDO + !SCK)
    CLC3CON = 0x82; //4-AND

    CLC4POL = 0x04;
    CLC4SEL0 = 0x26; //CLC3OUT
    CLC4SEL1 = 0x2C; //SCK
    CLC4SEL2 = 0x25; //CLC2OUT
    CLC4SEL3 = 0x00; //Unused
    CLC4GLS0 = 0x02; //CLC3OUT
    CLC4GLS1 = 0x00; //'0'
    CLC4GLS2 = 0x14; //!(!SCK+!CLC2OUT)
    CLC4GLS3 = 0x00; //'0'
    CLC4CON = 0x83; //SR

    SPI1CLK = 0x05; //TMR2
    SPI1BAUD = 0x00; //250kHz
    SPI1CON1 = 0x00;
    SPI1CON2 = 0x02; //Transmit only
    SPI1CON0 = 0x82; //Master mode
}

void SISendMessage(uint8_t* msg, uint8_t len) {
    int8_t leftToSend;
    uint8_t idx;

    //PIE9bits.TMR6IE = 0;
    //PIR9bits.TMR6IF = 0;
    //T6CON = 0x00;

    //SMT disabled while sending data
    SMT1CON1bits.GO = 0;

    while (!SPI1STATUSbits.TXBE);

    //Let the PIC drive the GCC data line
    TRISBbits.TRISB2 = 0;

    //Reset incoming data
    SIBitCounter = 0;
    SIByteCounter = 0;
    SICMDReceived = 0;
    SITempByte = 0x00;

    T2TMR = 0x01; //Will be reset by CLC

    SPI1TWIDTH = 0x01; //Stop bit at the end of the message
    SPI1INTFbits.SRMTIF = 0;
    SPI1TCNTL = len; //Message length in byte

    if (len == 1) {
        SPI1TXB = msg[0];
        SPI1TXB = 0x80; //Stop bit
    } else {
        SPI1TXB = msg[0];
        SPI1TXB = msg[1];
        leftToSend = len - 2;
        idx = 2;

        while(leftToSend >= 0) {
            if (SPI1TCNTL < (leftToSend + 2)) { //Check if we have to fill the FIFO
                if (leftToSend == 0) { //We are done sending the message. Send stop bit
                    SPI1TXB = 0x80; //Stop bit
                } else {
                    SPI1TXB = msg[idx++];
                }
                leftToSend--;
            }
        }
    }

    while (SPI1INTFbits.SRMTIF == 0);
    while (!SPI1STATUSbits.TXBE);
    SPI1INTFbits.SRMTIF = 0;

    //Release the GCC data line
    SMT1CON1bits.GO = 1;
    TRISBbits.TRISB2 = 1;

    //T6TMR = 0x00;
    //T6CON = 0x80;
    //PIR9bits.TMR6IF = 0;
    //PIE9bits.TMR6IE = 1;
}

uint8_t SIGetCommand(uint8_t* msg) {
    uint8_t i;
    if (!SICMDReceived) {
        return 0;
    }
    for (i = 0; i < SIByteCounter; i++) {
        msg[i] = SIInputMessage[i];
    }

    return SIByteCounter;
}

uint8_t SIGetCommandCode(uint8_t* msg) {
    uint8_t i;
    if (SIByteCounter) {
        return 0;
    }
    *msg = SIInputMessage[0];

    return 1;
}

uint8_t SICommadReceived(void) {
    return SICMDReceived;
}

void SIClear(void) {
    //Reset incoming data
    SIBitCounter = 0;
    SIByteCounter = 0;
    SICMDReceived = 0;
    SITempByte = 0x00;
}
