/*
 * Timerprog0.c
 *
 * Created: 24/10/2023 3:15:59 PM
 *  Author: Kirollos
 */ 

#include "Timer0_config.h"
#include "Timer0_private.h"
#include "Timer0.h"

u32 timer0_num_ovf = 0;
u32 timer0_rem_counts = 0;

static volatile void (*Timer0_cbfn)();
static u32 timeMS = 0;

void Timer0_Init(Timer0_Parameters* pTimer0_params)
{
	TIMER0_TCCR0 = 0; // Reset
	TIMER0_TCCR0 |= (pTimer0_params->cmp_match_mode << TCCR0_CMP_MATCH_OFFSET); // Compare match
	TIMER0_TCCR0 |= ((pTimer0_params->wave_gen_mode & 1) << TCCR0_WGM00) | (((pTimer0_params->wave_gen_mode & 2) >> 1) << TCCR0_WGM01); // Wave generation mode
	
}

void Timer0_start(enum TIMER0_CLK_SEL clksel)
{
	TIMER0_TCCR0 &= ~TIMER0_TCCR0_PRESCALER_MASK; // clear any previous clk sel
	Timer0_setDelayTimeMilliSec(timeMS);
	TIMER0_TCCR0 |= clksel;
};

void Timer0_stop()
{
	TIMER0_TCCR0 &= ~TIMER0_TCCR0_PRESCALER_MASK; // clear any previous clk sel
}

u32 Timer0_GetCounts()
{
	
}

void Timer0_setDelayTimeMilliSec(u32 copy_u32TimeMS)
{
	if(MACRO_MODE == WGM_NORMAL) {
		// tick time (us) = prescaler/FCPU
		// tick time (ms) = prescaler/FCPU   / 1000
		// num of counts = delaytime (ms) / tick time (ms)
		// no of overflows = num of counts / 256
		// rem counts = num of counts % 256
		timeMS = copy_u32TimeMS;
		u32 c = copy_u32TimeMS* 1000UL / (Prescaler_Map[MACRO_PRESCALER] / F_CPU_MHZ);
		timer0_num_ovf = c / 256;
		timer0_rem_counts = c % 256;
	}
	else if(MACRO_MODE == WGM_CTC)
	{
		
	}
}

void Timer0_EnableInt(enum TIMER0_INT_TYPE copyinttype)
{
	TIMER0_TIMSK |= 1 << copyinttype;
};

void Timer0_DisableInt(enum TIMER0_INT_TYPE copyinttype)
{
	TIMER0_TIMSK &= ~(1 << copyinttype);
};

void Timer0_setCallBack(void (*T0cbfn)())
{
	Timer0_cbfn = T0cbfn;
}

void Timer0_setFastPWM(u8 copyu8_frequency, double copydouble_duty)
{
	// duty = ton / tperiod
	// ton = duty * tperiod
	// ton = duty / freq
	double f = (copydouble_duty) * 255;
	if(((TIMER0_TCCR0 & 0b110000)>> TCCR0_CMP_MATCH_OFFSET ) == CMPM_CLEAR)
	{
		TIMER0_OCR0 = f;
	}
	else
	{
		TIMER0_OCR0 = 255 - f;
	}
}

void Timer0_setphaseCorrectPWM(u8 copyu8_frequency, double copydouble_duty)
{
	// duty = ton / tperiod
	// ton = duty * tperiod
	// ton = duty / freq
	double f = (copydouble_duty) * 255;
	if(((TIMER0_TCCR0 & 0b110000)>> TCCR0_CMP_MATCH_OFFSET ) == CMPM_CLEAR)
	{
		TIMER0_OCR0 = f;
	}
	else
	{
		TIMER0_OCR0 = 511 - f;	
	}
}

// ISR CMP
void __vector_10 (void) __attribute__((signal));
void __vector_10 (void)
{
	if(Timer0_cbfn != 0)
		(*Timer0_cbfn)();
}

// ISR OVF
void __vector_11 (void) __attribute__((signal));
void __vector_11 (void)
{
	static u32 ovf_counter = 0;
	
	if(++ovf_counter == timer0_num_ovf)
	{
		TIMER0_TCNT0 = 256 - timer0_rem_counts;
		ovf_counter = 0;
		// call callback here
		(*Timer0_cbfn)();
	}
}