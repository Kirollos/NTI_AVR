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
	NO_CLK_SRC,
	CLK_DIV_1,
	CLK_DIV_8,
	CLK_DIV_64,
	CLK_DIV_256,
	CLK_DIV_1024,
	CLK_EXT_FALLING,
	CLK_EXT_RISING
};

enum TIMER1_WGM
{
	WGM_NORMAL,
	WGM_PWM_PHASE_CORRECT_8BIT,
	WGM_PWM_PHASE_CORRECT_9BIT,
	WGM_PWM_PHASE_CORRECT_10BIT,
	WGM_CTC_OCR1A,
	WGM_FAST_PWM_8BIT,
	WGM_FAST_PWM_9BIT,
	WGM_FAST_PWM_10BIT,
	WGM_PWM_PHASE_FREQ_CORRECT_ICR1,
	WGM_PWM_PHASE_FREQ_CORRECT_OCR1A,
	WGM_PWM_PHASE_CORRECT_ICR1,
	WGM_PWM_PHASE_CORRECT_OCR1A,
	WGM_CTC_ICR1,
	WGM_FAST_PWM_ICR1 = 14,
	WGM_FAST_PWM_OCR1A
};

enum TIMER1_CMP_TYPE
{
	CMP_A,
	CMP_B	
};

enum TIMER1_CMPM
{
	CMPM_NORMAL,
	CMPM_TOGGLE,
	CMPM_CLEAR,
	CMPM_SET	
};

enum TIMER1_ICNC
{
	ICNC_OFF,
	ICNC_ON
};

enum TIMER1_ICES
{
	ICES_FALLING_EDGE,
	ICES_RISING_EDGE
};

enum TIMER1_INT_TYPE
{
	TIMER1_INT_OVF = 2,
	TIMER1_INT_CMPB,
	TIMER1_INT_CMPA,
	TIMER0_INT_IC
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



#endif /* TIMER1_H_ */