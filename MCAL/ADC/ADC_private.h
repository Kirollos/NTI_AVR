/*
 * ADC_private.h
 *
 * Created: 24/10/2023 9:17:41 AM
 *  Author: Kirollos
 */ 


#ifndef ADC_PRIVATE_H_
#define ADC_PRIVATE_H_


#define ADC_ADMUX		*((volatile u8*) 0x27)
#define ADC_ADCSRA		*((volatile u8*) 0x26)
#define ADC_ADCH		*((volatile u8*) 0x25)
#define ADC_ADCL		*((volatile u8*) 0x24)
#define ADC_SFIOR		*((volatile u8*) 0x50)


#define ADC_REGBIT_ADMUX_REF	6
#define ADC_REGBIT_ADMUX_ADLAR	5
#define ADC_REGBIT_ADMUX_MUXX	0

#define ADC_REGBIT_ADCSRA_PS	0
#define ADC_REGBIT_ADCSRA_IE	3
#define ADC_REGBIT_ADCSRA_IF	4
#define ADC_REGBIT_ADCSRA_ATE	5
#define ADC_REGBIT_ADCSRA_ADSC	6
#define ADC_REGBIT_ADCSRA_EN	7

#define ADC_REGBIT_SFIOR_TS		5


#endif /* ADC_PRIVATE_H_ */