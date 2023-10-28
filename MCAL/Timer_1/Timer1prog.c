/*
 * Timerprog1.c
 *
 * Created: 24/10/2023 3:15:59 PM
 *  Author: Kirollos
 */ 

#include "Timer1_config.h"
#include "Timer1_private.h"
#include "Timer1.h"

u32 timer1_num_ovf = 0;
u32 timer1_rem_counts = 0;

static volatile void (*Timer1_cbfn)();
static u32 timeMS = 0;

void Timer1_Init(Timer1_Parameters* pTimer1_params)
{
	TIMER1_TCCR1A = TIMER1_TCCR1B = 0; // Reset
	TIMER1_TCCR1A |= (pTimer1_params->cmpB_match_mode << TCCR1A_CMPB_MATCH_OFFSET | pTimer1_params->cmpA_match_mode << TCCR1A_CMPA_MATCH_OFFSET); // Compare match
	TIMER1_TCCR1A |= ((pTimer1_params->wave_gen_mode & 1) << TCCR1A_WGM10) | (((pTimer1_params->wave_gen_mode & 2) >> 1) << TCCR1A_WGM11); // Wave generation mode
	TIMER1_TCCR1B |= (((pTimer1_params->wave_gen_mode & 4) >> 2) << TCCR1B_WGM12) | (((pTimer1_params->wave_gen_mode & 8) >> 3) << TCCR1B_WGM13); // Wave generation mode
	TIMER1_TCCR1B |= (pTimer1_params->ic_edge_select << TCCR1B_ICES1_OFFSET) | (pTimer1_params->ic_noise_canceller << TCCR1B_ICNC1_OFFSET);
	
}

void Timer1_start(enum TIMER1_CLK_SEL clksel)
{
	TIMER1_TCCR1B &= ~TIMER1_TCCR1B_PRESCALER_MASK; // clear any previous clk sel
	Timer1_setDelayTimeMilliSec(timeMS);
	TIMER1_TCCR1B |= clksel;
};

void Timer1_stop()
{
	TIMER1_TCCR1B &= ~TIMER1_TCCR1B_PRESCALER_MASK; // clear any previous clk sel
}

u32 Timer1_GetCounts()
{
	
}

void Timer1_setDelayTimeMilliSec(u32 copy_u32TimeMS)
{
	if(MACRO_MODE == WGM_NORMAL) {
		// tick time (us) = prescaler/FCPU
		// tick time (ms) = prescaler/FCPU   / 1000
		// num of counts = delaytime (ms) / tick time (ms)
		// no of overflows = num of counts / 256
		// rem counts = num of counts % 256
		timeMS = copy_u32TimeMS;
		u32 c = copy_u32TimeMS* 1000UL / (Prescaler_Map[MACRO_PRESCALER] / F_CPU_MHZ);
		timer1_num_ovf = c / 0xFFFF;
		timer1_rem_counts = c % 0xFFFF;
	}
	else if(MACRO_MODE == WGM_CTC)
	{
		
	}
}

void Timer1_EnableInt(enum TIMER1_INT_TYPE copyinttype)
{
	TIMER1_TIMSK |= 1 << copyinttype;
};

void Timer1_DisableInt(enum TIMER1_INT_TYPE copyinttype)
{
	TIMER1_TIMSK &= ~(1 << copyinttype);
};

void Timer1_setCallBack(void (*T1cbfn)())
{
	Timer1_cbfn = T1cbfn;
}

void Timer1_setFastPWM(u8 copyu8_frequency, double copydouble_duty)
{
	// duty = ton / tperiod
	// ton = duty * tperiod
	// ton = duty / freq
	u16 topper = 0x00ff;
	switch(MACRO_MODE)
	{
		case WGM_PWM_PHASE_CORRECT_8BIT:
		topper = 0x0ff;
		break;
		case WGM_PWM_PHASE_CORRECT_9BIT:
		topper = 0x1ff;
		break;
		case WGM_PWM_PHASE_CORRECT_10BIT:
		topper = 0x3ff;
		break;
		default:
		topper = 0xffff;
		break;
	}
	double f = (copydouble_duty) * topper;
	if(((TIMER1_TCCR1A & 0b110000)>> TCCR1A_CMPA_MATCH_OFFSET ) == CMPM_CLEAR)
	{
		TIMER1_OCR1A = f;
	}
	else
	{
		TIMER1_OCR1A = topper - f;
	}
	
	if(((TIMER1_TCCR1A & 0b001100)>> TCCR1A_CMPB_MATCH_OFFSET ) == CMPM_CLEAR)
	{
		TIMER1_OCR1B = f;
	}
	else
	{
		TIMER1_OCR1B = topper - f;
	}
}

void Timer1_setphaseCorrectPWM(u8 copyu8_frequency, double copydouble_duty)
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