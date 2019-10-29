#include "main.h"

#define DEBOUNCE 10 //ms

//Handy macros
#define CHECK_TIMER(b, t) if (toggledButtons.##b) buttonsTimers[t] = 0;

#define CHECK_DEBOUNCE(b, t) if (buttonsTimers[t] >= DEBOUNCE) { \
        buttonsTimers[t] = DEBOUNCE; \
        outButtons.##b = prevButtons.##b ^ 1; \
    }

inButtons_t prevButtons;
outButtons_t outButtons;

uint8_t buttonsTimers[14];
uint8_t buttonsMessage[8];

void buttonsInit(void) {
    //This bit must always be 1 for proper working
    outButtons.er = 1;

    //Use timer 4 as a 1 ms timer for debouncing
    T4CLK = 0x01; //FOSC/4 (16MHz)
    T4RST = 0x00;
    T4TMR = 0x00;
    T4PR = 125;
    T4CON = 0xF0; //T2ON = 0. Prescaler = 1:128. Postscaler = 1:1
    PIR7bits.TMR4IF = 0;
}

void buttonsUpdate(void) {
    uint8_t i;
    uint8_t PA, PB, PC;
    inButtons_t toggledButtons;

    //Check which pins were toggled
    PA = PORTA;
    PB = PORTB;
    PC = PORTC;
    toggledButtons.PORTA = PA ^ prevButtons.PORTA;
    toggledButtons.PORTB = PB ^ prevButtons.PORTB;
    toggledButtons.PORTC = PC ^ prevButtons.PORTC;
    //Store values for next frame
    prevButtons.PORTA = PA;
    prevButtons.PORTB = PB;
    prevButtons.PORTC = PC;

    //Reset the timers if the pins were toggled
    CHECK_TIMER(A, 0)
    CHECK_TIMER(B, 1)
    CHECK_TIMER(X, 2)
    CHECK_TIMER(Y, 3)
    CHECK_TIMER(ST, 4)
    CHECK_TIMER(DL, 5)
    CHECK_TIMER(DR, 6)
    CHECK_TIMER(DD, 7)
    CHECK_TIMER(DU, 8)
    CHECK_TIMER(Z, 9)
    CHECK_TIMER(RD, 10)
    CHECK_TIMER(LD, 11)
    CHECK_TIMER(LA, 12)
    CHECK_TIMER(RA, 13)

    if (!PIR7bits.TMR4IF) return;
    PIR7bits.TMR4IF = 0; //Reset the timer interrupt flag

    //Increment the timers
    for (i = 0; i < 14; i++) {
        buttonsTimers[i]++;
    }

    //Update valid buttons
    CHECK_DEBOUNCE(A, 0)
    CHECK_DEBOUNCE(B, 1)
    CHECK_DEBOUNCE(X, 2)
    CHECK_DEBOUNCE(Y, 3)
    CHECK_DEBOUNCE(ST, 4)
    CHECK_DEBOUNCE(DL, 5)
    CHECK_DEBOUNCE(DR, 6)
    CHECK_DEBOUNCE(DD, 7)
    CHECK_DEBOUNCE(DU, 8)
    CHECK_DEBOUNCE(Z, 9)
    CHECK_DEBOUNCE(RD, 10)
    CHECK_DEBOUNCE(LD, 11)
    //Slightly different check for the analog triggers
    if (buttonsTimers[12] >= DEBOUNCE) {
        buttonsTimers[12] = DEBOUNCE;
        if (prevButtons.LA) {
            outButtons.LA = 0;
        } else {
            outButtons.LA = 0xFF;
        }
    }
    if (buttonsTimers[13] >= DEBOUNCE) {
        buttonsTimers[13] = DEBOUNCE;
        if (prevButtons.RA) {
            outButtons.RA = 0;
        } else {
            outButtons.RA = 0xFF;
        }
    }
}

uint8_t* buttonsGetMessage(uint8_t analogMode, uint8_t triggersMode) {
    buttonsMessage[0] = outButtons.byte0;
    buttonsMessage[1] = outButtons.byte1;
    buttonsMessage[2] = ADC_SX;
    buttonsMessage[3] = ADC_SY;

    uint8_t ra, la;
    if (!triggersMode) {
        ra = outButtons.RA;
        la = outButtons.LA;
    } else {
        ra = ADC_R;
        la = ADC_L;
    }

    switch (analogMode) {
        case 0:
        case 5:
        case 6:
        case 7:
            buttonsMessage[4] = ADC_CX;
            buttonsMessage[5] = ADC_CY;
            buttonsMessage[6] = (la & 0xF0) | (ra >> 4);
            buttonsMessage[7] = 0x00; //Analog A/B
        break;

        case 1:
            buttonsMessage[4] = (ADC_CX & 0xF0) | (ADC_CY >> 4);
            buttonsMessage[5] = la;
            buttonsMessage[6] = ra;
            buttonsMessage[7] = 0x00; //Analog A/B
        break;

        case 2:
            buttonsMessage[4] = (ADC_CX & 0xF0) | (ADC_CY >> 4);
            buttonsMessage[5] = (la & 0xF0) | (ra >> 4);
            buttonsMessage[6] = 0x00; //Analog A
            buttonsMessage[7] = 0x00; //Analog B
        break;

        case 3:
            buttonsMessage[4] = ADC_CX;
            buttonsMessage[5] = ADC_CY;
            buttonsMessage[6] = ADC_L;
            buttonsMessage[7] = ADC_R;
        break;

        case 4:
            buttonsMessage[4] = ADC_CX;
            buttonsMessage[5] = ADC_CY;
            buttonsMessage[6] = 0x00; //Analog A
            buttonsMessage[7] = 0x00; //Analog B
        break;
    }

    return buttonsMessage;
}