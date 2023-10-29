/*
 * Timer1_private.h
 *
 * Created: 24/10/2023 3:16:11 PM
 *  Author: Kirollos
 */ 


#ifndef TIMER1_H_
#define TIMER1_H_

#include "../../STD_TYPES.h"

enum TIMER1_CLK_SEL
{
	TIMER1_NO_CLK_SRC,
	TIMER1_CLK_DIV_1,
	TIMER1_CLK_DIV_8,
	TIMER1_CLK_DIV_64,
	TIMER1_CLK_DIV_256,
	TIMER1_CLK_DIV_1024,
	TIMER1_CLK_EXT_FALLING,
	TIMER1_CLK_EXT_RISING
};

enum TIMER1_WGM
{
	TIMER1_WGM_NORMAL,
	TIMER1_WGM_PWM_PHASE_CORRECT_8BIT,
	TIMER1_WGM_PWM_PHASE_CORRECT_9BIT,
	TIMER1_WGM_PWM_PHASE_CORRECT_10BIT,
	TIMER1_WGM_CTC_OCR1A,
	TIMER1_WGM_FAST_PWM_8BIT,
	TIMER1_WGM_FAST_PWM_9BIT,
	TIMER1_WGM_FAST_PWM_10BIT,
	TIMER1_WGM_PWM_PHASE_FREQ_CORRECT_ICR1,
	TIMER1_WGM_PWM_PHASE_FREQ_CORRECT_OCR1A,
	TIMER1_WGM_PWM_PHASE_CORRECT_ICR1,
	TIMER1_WGM_PWM_PHASE_CORRECT_OCR1A,
	TIMER1_WGM_CTC_ICR1,
	TIMER1_WGM_FAST_PWM_ICR1 = 14,
	TIMER1_WGM_FAST_PWM_OCR1A
};

enum TIMER1_CMP_TYPE
{
	TIMER1_CMP_A,
	TIMER1_CMP_B	
};

enum TIMER1_CMPM
{
	TIMER1_CMPM_NORMAL,
	TIMER1_CMPM_TOGGLE,
	TIMER1_CMPM_CLEAR,
	TIMER1_CMPM_SET	
};

enum TIMER1_ICNC
{
	TIMER1_ICNC_OFF,
	TIMER1_ICNC_ON
};

enum TIMER1_ICES
{
	TIMER1_ICES_FALLING_EDGE,
	TIMER1_ICES_RISING_EDGE
};

enum TIMER1_INT_TYPE
{
	TIMER1_INT_OVF = 2,
	TIMER1_INT_CMPB,
	TIMER1_INT_CMPA,
	TIMER1_INT_IC
};

typedef struct 
{
	enum TIMER1_WGM		wave_gen_mode;
	enum TIMER1_CMPM	cmpA_match_mode;
	enum TIMER1_CMPM	cmpB_match_mode;
	enum TIMER1_ICNC	ic_noise_canceller;
	enum TIMER1_ICES	ic_edge_select;
} Timer1_Parameters;


void Timer1_Init(Timer1_Parameters* pTimer1_params);
void Timer1_start(enum TIMER1_CLK_SEL clksel);
void Timer1_stop();
u32 Timer1_GetCounts();
void Timer1_setDelayTimeMilliSec(u32 copy_u32TimeMS);
void Timer1_EnableInt(enum TIMER1_INT_TYPE copyinttype);
void Timer1_DisableInt(enum TIMER1_INT_TYPE copyinttype);
void Timer1_setCallBack(void (*T1cbfn)());
void Timer1_setFastPWM(enum TIMER1_CMP_TYPE comparetype, u8 copyu8_frequency, double copydouble_duty);
void Timer1_setphaseCorrectPWM(enum TIMER1_CMP_TYPE comparetype, u8 copyu8_frequency, double copydouble_duty);

void Timer1_ICU_setCallBack(void (*T1cbfn)());
void Timer1_ICU_setTrigger(enum TIMER1_ICES trigg);
u16 Timer1_ICU_takeReading();

void Timer1_MeasureFrequency(u16* pFreq);
void Timer1_MeasureDuty(double* pDuty);



#endif /* TIMER1_H_ */