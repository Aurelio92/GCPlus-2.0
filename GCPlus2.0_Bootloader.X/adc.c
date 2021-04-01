/*
    File:           adc.c
    Description:    This source implements the needed routines to gather data from the analog inputs.
                    Every transfer is handled by DMA modules to reduce the load on the CPU.
                    This system automatically scans all the channels specified in ADCChannels.
*/

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"

uint8_t ADCChannels[ADC_NCHANNELS] = {
    //SX, SY,   CX,   CY,   R,   L
    0x02, 0x03, 0x00, 0x01, 0x13, 0x14
};

uint8_t ADCValues[ADC_NCHANNELS] = {
    0x80, 0x80, 0x80, 0x80, 0x00, 0x00
};

void ADCInit(uint8_t sxCh, uint8_t syCh, uint8_t cxCh, uint8_t cyCh) {
    uint8_t i;

    //Setup ADC. On each interrupt the averaged 8bit result will be in ADFLTRH
    ADCON0 = 0x80;  //ADC enabled. Continuous operation.
                    //Left justified. Will be retriggered by DMA1.
    ADCON1 = 0x00;
    ADCON2 = 0x03;  //Burst Average mode
    ADCON2bits.CRS = 6; //ADFLTR = ACC >> CRS
    ADCON3 = 0x07; //Interrupt regardless of threshold test results
    ADCLK = 0x3F;
    ADREF = 0x00;
    ADACQL = 0xFF;
    //ADPREL = 0xFF;
    ADRPT = 4;     //Average on 4 samples
    ADACT = 0x1D;   //Trigger on each write to ADPCH

    //Enable charge pump
    ADCPbits.CPON = 1;
    while (!ADCPbits.CPRDY);

    ADCChannels[0] = sxCh;
    ADCChannels[1] = syCh;
    ADCChannels[2] = cxCh;
    ADCChannels[3] = cyCh;

    //Read each channel once
    ADTIF = 0;
    for (i = 0; i < ADC_NCHANNELS; i++) {
        ADPCH = ADCChannels[i];
        while(!ADTIF);
        ADTIF = 0;
        ADCValues[i] = ADFLTRL;
    }

    //Unlock
    asm ("banksel PRLOCK");
    asm ("movlw 0x55");
    asm ("movwf PRLOCK");
    asm ("movlw 0xAA");
    asm ("movwf PRLOCK");
    asm ("bsf PRLOCK, 0");

    //Setup DMA1 to copy from ADCChannels to ADPCH on DMA2SCNTIF
    DMA1SSAU = ((uint32_t)ADCChannels >> 16) & 0xFF;
    DMA1SSAH = ((uint32_t)ADCChannels >> 8) & 0xFF;
    DMA1SSAL = ((uint32_t)ADCChannels) & 0xFF;
    DMA1DSAH = ((uint32_t)&ADPCH >> 8) & 0xFF;
    DMA1DSAL = ((uint32_t)&ADPCH) & 0xFF;
    DMA1SSZH = 0x00;
    DMA1SSZL = ADC_NCHANNELS;
    DMA1DSZH = 0x00;
    DMA1DSZL = 0x01;
    DMA1SIRQ = 42;  //DMA2SCNT
    DMA1CON1bits.DMODE = 0b00; //DMA1DPTR remains unchanged after each transfer completion
    DMA1CON1bits.DSTP = 0;      //SIRQEN bit is not cleared when Destination Counter reloads
    DMA1CON1bits.SMR = 0b00;    //DMA1SSA points to SFR/GPR Data Space
    DMA1CON1bits.SMODE = 0b01;  //DMA1SPTR is incremented after each transfer completion
    DMA1CON1bits.SSTP = 0;      //SIRQEN bit is not cleared when Source Counter reloads
    DMA1CON0 = 0xC0; //Enable DMA1 module. SIRQEN = 1

    //Setup DMA2 to copy from ADFLTRL to ADCValues on ADTIF
    DMA2SSAU = 0;
    DMA2SSAH = ((uint32_t)&ADFLTRL >> 8) & 0xFF;
    DMA2SSAL = ((uint32_t)&ADFLTRL) & 0xFF;
    DMA2DSAH = ((uint32_t)ADCValues >> 8) & 0xFF;
    DMA2DSAL = ((uint32_t)ADCValues) & 0xFF;
    DMA2SSZH = 0x00;
    DMA2SSZL = 0x01;
    DMA2DSZH = 0x00;
    DMA2DSZL = ADC_NCHANNELS;
    DMA2SIRQ = 11;  //ADTIF
    DMA2CON1bits.DMODE = 0b01; //DMA2DPTR is incremented after each transfer completion
    DMA2CON1bits.DSTP = 0;      //SIRQEN bit is not cleared when Destination Counter reloads
    DMA2CON1bits.SMR = 0b00;    //DMA2SSA points to SFR/GPR Data Space
    DMA2CON1bits.SMODE = 0b00;  //DMA2SPTR remains unchanged after each transfer completion
    DMA2CON1bits.SSTP = 0;      //SIRQEN bit is not cleared when Source Counter reloads
    DMA2CON0 = 0xC0; //Enable DMA2 module. SIRQEN = 1

    DMA1CON0bits.DGO = 1; //Start ADC transfers
}
