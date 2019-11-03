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
}

void rumbleSpin(uint8_t speed) {
    LATB4 = 1; //Enable DRV2603
    CCPR1H = speed;
}

void rumbleBrake(void) {
    LATB4 = 1; //Enable DRV2603
    CCPR1H = 0x60;
}

void rumbleStop(void) {
    LATB4 = 0; //Disable DRV2603
    CCPR1H = 0x80;
}
