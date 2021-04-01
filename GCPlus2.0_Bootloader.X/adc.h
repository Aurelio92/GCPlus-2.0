#ifndef _ADC_H_
#define _ADC_H_

#define ADC_NCHANNELS 6

extern uint8_t ADCValues[ADC_NCHANNELS];
#define ADC_SX  ADCValues[0]
#define ADC_SY  ADCValues[1]
#define ADC_CX  ADCValues[2]
#define ADC_CY  ADCValues[3]
#define ADC_R   ADCValues[4]
#define ADC_L   ADCValues[5]

void ADCInit(uint8_t sxCh, uint8_t syCh, uint8_t cxCh, uint8_t cyCh);

#endif
