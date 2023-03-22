#include "main.h"

static uint8_t braking;

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

    LATB4 = 0;

    braking = 0;
}

void rumbleSpin(uint8_t speed) {
    braking = 0;
    CCPR1H = speed;
    LATB4 = 1; //Enable DRV2603
}

void rumbleBrake(void) {
    CCPR1H = 0x00;
    if (!braking)
        LATB4 = 1; //Enable DRV2603
    else
        LATB4 = 0; //Disable DRV2603

    braking = 1;
}

void rumbleStop(void) {
    braking = 0;
    CCPR1H = 0x00;
    LATB4 = 0; //Disable DRV2603
}
