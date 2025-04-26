#ifndef _ADC_H_
#define _ADC_H_

#define ADC_NCHANNELS 7

extern uint8_t ADCValues[ADC_NCHANNELS];
// ADCValues[0] is ignored to resolve a bug
#define ADC_SY  ADCValues[1]
#define ADC_SX  ADCValues[2]
#define ADC_CX  ADCValues[3]
#define ADC_CY  ADCValues[4]
#define ADC_R   ADCValues[5]
#define ADC_L   ADCValues[6]

void ADCInit(uint8_t sxCh, uint8_t syCh, uint8_t cxCh, uint8_t cyCh);

#endif
