/*
 * Timer0_private.h
 *
 * Created: 24/10/2023 3:16:11 PM
 *  Author: Kirollos
 */ 


#ifndef TIMER0_H_
#define TIMER0_H_

#include "../../STD_TYPES.h"

enum TIMER0_CLK_SEL
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

enum TIMER0_WGM
{
	WGM_NORMAL,
	WGM_PWM_PHASE_CORRECT,
	WGM_CTC,
	WGM_FAST_PWM
};

enum TIMER0_CMPM
{
	CMPM_NORMAL,
	CMPM_TOGGLE,
	CMPM_CLEAR,
	CMPM_SET	
};

enum TIMER0_INT_TYPE
{
	TIMER0_INT_OVF,
	TIMER0_INT_CMP
};

typedef struct 
{
	enum TIMER0_WGM		wave_gen_mode;
	enum TIMER0_CMPM	cmp_match_mode;
} Timer0_Parameters;


void Timer0_Init(Timer0_Parameters* pTimer0_params);
void Timer0_start(enum TIMER0_CLK_SEL clksel);
void Timer0_stop();
u32 Timer0_GetCounts();
void Timer0_setDelayTimeMilliSec(u32 copy_u32TimeMS);
void Timer0_EnableInt(enum TIMER0_INT_TYPE copyinttype);
void Timer0_DisableInt(enum TIMER0_INT_TYPE copyinttype);
void Timer0_setCallBack(void (*T0cbfn)());
void Timer0_setFastPWM(u8 copyu8_frequency, double copydouble_duty);
void Timer0_setphaseCorrectPWM(u8 copyu8_frequency, double copydouble_duty);



#endif /* TIMER0_H_ */