#include "main.h"

void rumbleInit(void) {
    T4CON = 0x00;
    T4CLKCON = 0x01; //FOSC/4 (16MHz)
    T4HLT = 0x00; //Software gate control
    T4PR = 0xFF; //62.5 kHz, 8bit resolution
    T4TMR = 0x00;
    T4CON = 0x80;

    CCPTMRS0bits.C1TSEL = 2; //Timer4 to CCP1
    CCPR1L = 0x00;
    CCPR1H = 0x80; //50% => 0V
    CCP1CON = 0x9C; //Enabled. Left-aligned PWM

    //Enable DRV2603
    LATB4 = 1;
}

void rumbleSpin(uint8_t speed) {
    CCPR1H = speed;
}

void rumbleBrake(void) {
    CCPR1H = 0x60;
}

void rumbleStop(void) {
    CCPR1H = 0x80;
}
