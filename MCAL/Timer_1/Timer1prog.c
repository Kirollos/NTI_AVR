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
static volatile void (*Timer1_capt_cbfn)();
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
	if(MACRO_MODE == TIMER1_WGM_NORMAL) {
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
	else if(MACRO_MODE == TIMER1_WGM_CTC_OCR1A)
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

void Timer1_setFastPWM(enum TIMER1_CMP_TYPE comparetype, u8 copyu8_frequency, double copydouble_duty)
{
	// duty = ton / tperiod
	// ton = duty * tperiod
	// ton = duty / freq
	u16 topper = 0x00ff;
	switch(MACRO_MODE)
	{
		case TIMER1_WGM_PWM_PHASE_CORRECT_8BIT:
		topper = 0x0ff;
		break;
		case TIMER1_WGM_PWM_PHASE_CORRECT_9BIT:
		topper = 0x1ff;
		break;
		case TIMER1_WGM_PWM_PHASE_CORRECT_10BIT:
		topper = 0x3ff;
		break;
		default:
		topper = 0xffff;
		break;
	}
	double f = (copydouble_duty) * topper;
	if(comparetype == TIMER1_CMP_A) {
		if(((TIMER1_TCCR1A & 0b11000000)>> TCCR1A_CMPA_MATCH_OFFSET ) == TIMER1_CMPM_CLEAR)
		{
			TIMER1_OCR1A = f;
		}
		else
		{
			TIMER1_OCR1A = topper - f;
		}
	}
	else if(comparetype == TIMER1_CMP_B) {
		if(((TIMER1_TCCR1A & 0b00110000)>> TCCR1A_CMPB_MATCH_OFFSET ) == TIMER1_CMPM_CLEAR)
		{
			TIMER1_OCR1B = f;
		}
		else
		{
			TIMER1_OCR1B = topper - f;
		}
	}
}

void Timer1_setphaseCorrectPWM(enum TIMER1_CMP_TYPE comparetype, u8 copyu8_frequency, double copydouble_duty)
{
	// duty = ton / tperiod
	// ton = duty * tperiod
	// ton = duty / freq
	
	u16 topper = 0x00ff;
	switch(MACRO_MODE)
	{
		case TIMER1_WGM_PWM_PHASE_CORRECT_8BIT:
		topper = 0x0ff;
		break;
		case TIMER1_WGM_PWM_PHASE_CORRECT_9BIT:
		topper = 0x1ff;
		break;
		case TIMER1_WGM_PWM_PHASE_CORRECT_10BIT:
		topper = 0x3ff;
		break;
		default:
		topper = 0xffff;
		break;
	}
	double f = (copydouble_duty) * topper;
	if(((TIMER1_TCCR1A & 0b11000000)>> TCCR1A_CMPA_MATCH_OFFSET ) == TIMER1_CMPM_CLEAR)
	{
		TIMER1_OCR1A = f;
	}
	else
	{
		TIMER1_OCR1A = (topper+1)*2 - 1 - f;
	}
	
	if(((TIMER1_TCCR1A & 0b00110000)>> TCCR1A_CMPB_MATCH_OFFSET ) == TIMER1_CMPM_CLEAR)
	{
		TIMER1_OCR1B = f;
	}
	else
	{
		TIMER1_OCR1A = (topper+1)*2 - 1 - f;
	}
}

void Timer1_ICU_setCallBack(void (*T1cbfn)())
{
	Timer1_capt_cbfn = T1cbfn;
}

void Timer1_ICU_setTrigger(enum TIMER1_ICES trigg)
{
	TIMER1_TCCR1B &= (1 << TCCR1B_ICES1_OFFSET);
	TIMER1_TCCR1B |= (trigg << TCCR1B_ICES1_OFFSET);
}

u16 Timer1_ICU_takeReading()
{
	return TIMER1_ICR1;
}

void Timer1_MeasureFrequency(u16* pFreq)
{
	TIMER1_TIFR |= (1 << TIFR_ICF1);
	u16 buff1 = TIMER1_ICR1;
	while(TIMER1_TIFR & (1 << TIFR_ICF1));
	u16 buff2 = TIMER1_ICR1;
	u16 period = buff2 - buff1;
	u16 freq = 1/period;
	*pFreq = freq;
}

void Timer1_MeasureDuty(double* pDuty)
{
	TIMER1_TCCR1B |= (1 << TCCR1B_ICES1_OFFSET);
	TIMER1_TIFR |= (1 << TIFR_ICF1);
	while(TIMER1_TIFR & (1 << TIFR_ICF1) == 0);
	u16 buff1 = TIMER1_ICR1;
	TIMER1_TCCR1B &= ~(1 << TCCR1B_ICES1_OFFSET);
	TIMER1_TIFR |= (1 << TIFR_ICF1);
	while(TIMER1_TIFR & (1 << TIFR_ICF1) == 0);
	u16 buff2 = TIMER1_ICR1;
	TIMER1_TCCR1B |= (1 << TCCR1B_ICES1_OFFSET);
	TIMER1_TIFR |= (1 << TIFR_ICF1);
	while(TIMER1_TIFR & (1 << TIFR_ICF1) == 0);
	u16 buff3 = TIMER1_ICR1;
	u16 onperiod = buff2 - buff1;
	u16 period = buff3 - buff1;
	double duty = onperiod;
	*pDuty = duty;
}

// ISR CMPA
void __vector_7 (void) __attribute__((signal));
void __vector_7 (void)
{
	if(Timer1_cbfn != 0)
		(*Timer1_cbfn)();
}

// ISR CMPB
void __vector_8 (void) __attribute__((signal));
void __vector_8 (void)
{
	if(Timer1_cbfn != 0)
	(*Timer1_cbfn)();
}

// ISR OVF
void __vector_9 (void) __attribute__((signal));
void __vector_9 (void)
{
	if(Timer1_cbfn != 0)
	(*Timer1_cbfn)();
	/*static u32 ovf_counter = 0;
	
	if(++ovf_counter == timer1_num_ovf)
	{
		TIMER1_TCNT1 = 256 - timer1_rem_counts;
		ovf_counter = 0;
		// call callback here
		//(*Timer1_cbfn)();
	}*/
}

// ISR CAPT
void __vector_6 (void) __attribute__((signal));
void __vector_6 (void)
{
	if(Timer1_capt_cbfn != 0)
	(*Timer1_capt_cbfn)();
}
