/*
 * ADCprog.c
 *
 * Created: 24/10/2023 9:17:22 AM
 *  Author: Kirollos
 */ 

#include "ADC_config.h"
#include "ADC_private.h"
#include "ADC.h"

#define F_CPU 16000000  // temp
#include <util/delay.h> // temp

static volatile void (*ADC_cbfn[29])(u16);
static ADC_Channels Active_channel = SE_ADC0;

void ADC_init(ADC_Parameters* pParameters)
{
	ADC_ADMUX = 0;
	ADC_ADMUX |= (pParameters->Ref << ADC_REGBIT_ADMUX_REF) | (pParameters->Adj << ADC_REGBIT_ADMUX_ADLAR);
	ADC_ADCSRA = 0;
	ADC_ADCSRA |= (pParameters->Enable << ADC_REGBIT_ADCSRA_EN) | (pParameters->Prescaler << ADC_REGBIT_ADCSRA_PS) | (pParameters->AutoTrigger << ADC_REGBIT_ADCSRA_ATE);
	ADC_SFIOR &= ~(7 << ADC_REGBIT_SFIOR_TS);
	ADC_SFIOR |= (pParameters->TriggerSource << ADC_REGBIT_SFIOR_TS);
}

u16 ADC_getDigitalValueSyncNonBlocking(ADC_Channels ADC_channel)
{
	ADC_ADMUX &= ~(15 << ADC_REGBIT_ADMUX_MUXX);
	ADC_ADMUX |= (ADC_channel << ADC_REGBIT_ADMUX_MUXX);
	
	ADC_ADCSRA |= (1 << ADC_REGBIT_ADCSRA_ADSC); // Start conversion
	u16 counter = 0x1FF;
	while(--counter && (ADC_ADCSRA & (1<<ADC_REGBIT_ADCSRA_ADSC)));
	if(!counter)
		return 0;
	u16 value = ADC_ADCL;
	value |= (ADC_ADCH << 8);
	return value;
}

void ADC_defineDigitalValueAsynchCallBack(ADC_Channels ADC_channel, void (*ADCcbfn)(u16))
{
	//ADC_ADMUX &= ~(15 << ADC_REGBIT_ADMUX_MUXX);
	//ADC_ADMUX |= (ADC_channel << ADC_REGBIT_ADMUX_MUXX);
	//ADC_ADCSRA |= (1 << ADC_REGBIT_ADCSRA_IE);
	
	ADC_cbfn[ADC_channel] = ADCcbfn;
	
	//ADC_ADCSRA |= (1 << ADC_REGBIT_ADCSRA_ADSC); // Start conversion
}

void ADC_getDigitalValueAsynchCallBack(ADC_Channels ADC_channel)
{
	ADC_ADMUX &= ~(15 << ADC_REGBIT_ADMUX_MUXX);
	ADC_ADMUX |= (ADC_channel << ADC_REGBIT_ADMUX_MUXX);
	ADC_ADCSRA |= (1 << ADC_REGBIT_ADCSRA_IE);
	Active_channel = ADC_channel;
	ADC_ADCSRA |= (1 << ADC_REGBIT_ADCSRA_ADSC); // Start conversion
}

void __vector_16 (void) __attribute__((signal));
void __vector_16 (void)
{
	u16 value = ADC_ADCL;
	value |= (ADC_ADCH << 8);
	if(ADC_cbfn[Active_channel] != 0)
	{
		(*ADC_cbfn[Active_channel])(value);
	}
}