/*
 * ADC.h
 *
 * Created: 24/10/2023 9:17:30 AM
 *  Author: Kirollos
 */ 


#ifndef ADC_H_
#define ADC_H_

#include "../../STD_TYPES.h"

enum ADC_ENABLE
{
	ADC_DISABLE,
	ADC_ENABLE
};
typedef enum
{
	// Single ended input
	SE_ADC0,
	SE_ADC1,
	SE_ADC2,
	SE_ADC3,
	SE_ADC4,
	SE_ADC5,
	SE_ADC6,
	SE_ADC7,
	// Differential input
	DIFF_ADC00_10X,
	DIFF_ADC10_10X,
	DIFF_ADC00_200X,
	DIFF_ADC10_200X,
	DIFF_ADC22_10X,
	DIFF_ADC32_10X,
	DIFF_ADC22_200X,
	DIFF_ADC32_200X,
	DIFF_ADC01_1X,
	DIFF_ADC11_1X,
	DIFF_ADC21_1X,
	DIFF_ADC31_1X,
	DIFF_ADC41_1X,
	DIFF_ADC51_1X,
	DIFF_ADC61_1X,
	DIFF_ADC71_1X,
	DIFF_ADC02_1X,
	DIFF_ADC12_1X,
	DIFF_ADC22_1X,
	DIFF_ADC32_1X,
	DIFF_ADC42_1X
} ADC_Channels;

enum ADC_REF
{
	AREF,
	AVCC,
	INTERNAL_REF = 3
};
enum ADC_LAR
{
	ADJUST_RIGHT,
	ADJUST_LEFT
};
enum ADC_PRESCALER
{
	DIV_2,
	DIV_4=2,
	DIV_8,
	DIV_16,
	DIV_32,
	DIV_64,
	DIV_128
};
enum ADC_AUTOTRIG
{
	ATE_DISABLE,
	ATE_ENABLE
};
enum ADC_TRIGSRC
{
	FreeRunning,
	AnalogComparator,
	ExtIntReq0,
	Timer0CmpMatch,
	Timer0Overflow,
	Timer1CmpMatchB,
	Timer1Overflow,
	Timer1CaptEvent
};

typedef struct
{	
	enum ADC_REF Ref;
	enum ADC_LAR Adj;
	enum ADC_PRESCALER Prescaler;
	enum ADC_ENABLE	   Enable;
	enum ADC_AUTOTRIG  AutoTrigger;
	enum ADC_TRIGSRC   TriggerSource;
} ADC_Parameters;

void ADC_init(ADC_Parameters* pParameters);
u16 ADC_getDigitalValueSyncNonBlocking(ADC_Channels ADC_channel);
void ADC_defineDigitalValueAsynchCallBack(ADC_Channels ADC_channel, void (*ADCcbfn)(u16));
void ADC_getDigitalValueAsynchCallBack(ADC_Channels ADC_channel);


#endif /* ADC_H_ */