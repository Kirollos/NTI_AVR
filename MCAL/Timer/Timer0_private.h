/*
 * Timer0_private.h
 *
 * Created: 24/10/2023 3:16:11 PM
 *  Author: Kirollos
 */ 


#ifndef TIMER0_PRIVATE_H_
#define TIMER0_PRIVATE_H_

#include "../../STD_TYPES.h"

#define TIMER0_TCCR0		*((volatile u8*) 0x53)
#define TIMER0_TCNT0		*((volatile u8*) 0x52)
#define TIMER0_OCR0			*((volatile u8*) 0x5c)
#define TIMER0_TIMSK		*((volatile u8*) 0x59)
#define TIMER0_TIFR			*((volatile u8*) 0x58)

#define TIMER0_TCCR0_PRESCALER_MASK				0x07

#define TCCR0_CMP_MATCH_OFFSET					4
#define TCCR0_WGM00								6
#define TCCR0_WGM01								3

#define MACRO_PRESCALER		(TIMER0_TCCR0 & TIMER0_TCCR0_PRESCALER_MASK)
#define MACRO_MODE			(((TIMER0_TCCR0 & (1 << TCCR0_WGM01)) >> TCCR0_WGM01 << 1) | ((TIMER0_TCCR0 & (1 << TCCR0_WGM00)) >> TCCR0_WGM00))

static u16 Prescaler_Map[] =
{
	0,    //NO_CLK_SRC,
	1,    //CLK_DIV_1,
	8,    //CLK_DIV_8,
	64,   //CLK_DIV_64,
	256,  //CLK_DIV_256,
	1024, //CLK_DIV_1024,
	0,    //CLK_EXT_FALLING,
	0,    //CLK_EXT_RISING
};

#endif /* TIMER0_PRIVATE_H_ */