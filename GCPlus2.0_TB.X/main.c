/*
 * File:   main.c
 * Author: aurelio
 *
 * Created on April 8, 2019, 9:30 PM
 */

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config VCAPEN = OFF     // Voltage Regulator Capacitor Enable bit (Vcap functionality is disabled on RA6.)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low Power Brown-Out Reset Enable Bit (Low power brown-out is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

/*PINOUT

    RA0 -> NC
    RA1 -> Z2
    RA2 -> SX       DAC1OUT1
    RA3 -> A
    RA4 -> CY       DAC4OUT1
    RA5 -> CX       DAC2OUT1
    RA6 -> X
    RA7 -> B

    RB0 -> LT
    RB1 -> DU
    RB2 -> SY       DAC3OUT1
    RB3 -> DD
    RB4 -> DR
    RB5 -> DL

    RC0 -> Y
    RC1 -> Z
    RC2 -> Start
    RC3 -> SCL      I2C
    RC4 -> SDA      I2C
    RC5 -> RT
    RC6 -> R
    RC7 -> L
*/

#include <xc.h>
#include "i2cslv.h"

void interrupt ISR(void) {
    if (PIR1bits.SSP1IF) {
        i2cSlvISR();
    }
}

void main(void) {
    //Initialize clock to 4 MHz
    //Start from HFINTOSC at 16 Mhz. We need 1:4 postscaler
    OSCCON = 0x68;
    asm("clrwdt");
    while (!(OSCSTAT & 0x10));

    //Set WDT to 8 seconds
    WDTCON = 0x1A;

    //GPIO initialization
    //All outputs high (released)
    LATA = 0xFF;
    LATB = 0xFF;
    LATC = 0x3F;

    //Digital outputs configured as open drain
    ODCONA = 0xCA;
    ODCONB = 0x3B;
    ODCONC = 0xE7;

    //All outputs but i2c and not used
    TRISA = 0x00;
    TRISB = 0xC0;
    TRISC = 0x18;

    //DAC initialization
    DAC1CON1 = 0x80;
    DAC1CON0 = 0xA0;
    DAC2CON1 = 0x10;
    DAC2CON0 = 0xA0;
    DAC3CON1 = 0x10;
    DAC3CON0 = 0xA0;
    DAC4CON1 = 0x10;
    DAC4CON0 = 0xA0;

    //i2c initialization
    i2cSlvInit();

    //Enable interrupts
    INTCON = 0xC0;

    while (1) {
        asm("clrwdt");
    }
}
