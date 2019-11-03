#include "main.h"

#define DEBOUNCE 10 //ms

//Handy macros
#define CHECK_TIMER(b) if (toggledButtons.##b) buttonsTimers[BUTTON_##b##_ID] = 0;

/*#define CHECK_DEBOUNCE(b, t) if (buttonsTimers[t] >= DEBOUNCE) { \
        buttonsTimers[t] = DEBOUNCE; \
        outButtons.##b = prevButtons.##b ^ 1; \
    }*/

#define CHECK_DEBOUNCE_BYTE0(b) if (buttonsTimers[BUTTON_##b##_ID] >= DEBOUNCE) { \
        buttonsTimers[BUTTON_##b##_ID] = DEBOUNCE; \
        outButtons.byte0 |= buttonsMapByte0[BUTTON_##b##_ID]; \
    }

#define CHECK_DEBOUNCE_BYTE1(b) if (buttonsTimers[BUTTON_##b##_ID] >= DEBOUNCE) { \
        buttonsTimers[BUTTON_##b##_ID] = DEBOUNCE; \
        outButtons.byte1 |= buttonsMapByte1[BUTTON_##b##_ID]; \
    }

#define CHECK_DEBOUNCE(b) CHECK_DEBOUNCE_BYTE0(b) \
    CHECK_DEBOUNCE_BYTE1(b)

inButtons_t prevButtons;
outButtons_t outButtons;

uint8_t buttonsTimers[N_BUTTONS];
uint8_t buttonsMessage[8];
uint8_t buttonsMapByte0[N_BUTTONS];
uint8_t buttonsMapByte1[N_BUTTONS];

void buttonsInit(void) {
    //Use timer 0 as a 1 ms timer for debouncing
    T0CON0 = 0x00;
    T0CON1 = 0x47; //FOSC/4 (16MHz). Prescaler = 1:128
    TMR0H = 124; //Rollover at 124 ticks
    TMR0IF = 0;
    T0CON0 = 0x80; //T0ON = 1. 8bit. 1:1 postscaler

    //Default mapping
    memset(buttonsMapByte0, 0, N_BUTTONS);
    memset(buttonsMapByte1, 0, N_BUTTONS);
    //Byte 0
    buttonsMapByte0[BUTTON_A_ID] = 0x01;
    buttonsMapByte0[BUTTON_B_ID] = 0x02;
    buttonsMapByte0[BUTTON_X_ID] = 0x04;
    buttonsMapByte0[BUTTON_Y_ID] = 0x08;
    buttonsMapByte0[BUTTON_ST_ID] = 0x10;
    //Byte 1
    buttonsMapByte1[BUTTON_DL_ID] = 0x01;
    buttonsMapByte1[BUTTON_DR_ID] = 0x02;
    buttonsMapByte1[BUTTON_DD_ID] = 0x04;
    buttonsMapByte1[BUTTON_DU_ID] = 0x08;
    buttonsMapByte1[BUTTON_Z_ID] = 0x10;
    buttonsMapByte1[BUTTON_RD_ID] = 0x20;
    buttonsMapByte1[BUTTON_LD_ID] = 0x40;
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
    CHECK_TIMER(A)
    CHECK_TIMER(B)
    CHECK_TIMER(X)
    CHECK_TIMER(Y)
    CHECK_TIMER(ST)
    CHECK_TIMER(DL)
    CHECK_TIMER(DR)
    CHECK_TIMER(DD)
    CHECK_TIMER(DU)
    CHECK_TIMER(Z)
    CHECK_TIMER(RD)
    CHECK_TIMER(LD)
    CHECK_TIMER(LA)
    CHECK_TIMER(RA)
    CHECK_TIMER(Z2)

    if (!TMR0IF) return;
    TMR0IF = 0; //Reset the timer interrupt flag

    //Increment the timers
    for (i = 0; i < N_BUTTONS; i++) {
        buttonsTimers[i]++;
    }

    outButtons.byte0 = 0x00;
    outButtons.byte1 = 0x80; //Make sure er is 1
    //Update valid buttons
    CHECK_DEBOUNCE(A)
    CHECK_DEBOUNCE(B)
    CHECK_DEBOUNCE(X)
    CHECK_DEBOUNCE(Y)
    CHECK_DEBOUNCE(ST)
    CHECK_DEBOUNCE(DL)
    CHECK_DEBOUNCE(DR)
    CHECK_DEBOUNCE(DD)
    CHECK_DEBOUNCE(DU)
    CHECK_DEBOUNCE(Z)
    CHECK_DEBOUNCE(RD)
    CHECK_DEBOUNCE(LD)
    CHECK_DEBOUNCE(Z2)
    //Slightly different check for the analog triggers
    if (buttonsTimers[BUTTON_LA_ID] >= DEBOUNCE) {
        buttonsTimers[BUTTON_LA_ID] = DEBOUNCE;
        if (prevButtons.LA) {
            outButtons.LA = 0;
        } else {
            outButtons.LA = 0xFF;
        }
    }
    if (buttonsTimers[BUTTON_RA_ID] >= DEBOUNCE) {
        buttonsTimers[BUTTON_RA_ID] = DEBOUNCE;
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
    if (triggersMode == TRIG_MODE_DIGITAL) {
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

void buttonsSetMapByte0(uint8_t* map) {
    uint8_t i;

    for (i = 0; i < N_BUTTONS; i++) {
        buttonsMapByte0[i] = map[i];
    }
}

void buttonsSetMapByte1(uint8_t* map) {
    uint8_t i;

    for (i = 0; i < N_BUTTONS; i++) {
        buttonsMapByte1[i] = map[i];
    }
}