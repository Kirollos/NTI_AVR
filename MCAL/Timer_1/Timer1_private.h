/*
 * Timer1_private.h
 *
 * Created: 24/10/2023 3:16:11 PM
 *  Author: Kirollos
 */ 


#ifndef TIMER1_PRIVATE_H_
#define TIMER1_PRIVATE_H_

#include "../../STD_TYPES.h"

#define TIMER1_TCCR1A		*((volatile u8*)  0x4F)
#define TIMER1_TCCR1B		*((volatile u8*)  0x4E)
#define TIMER1_TCNT1		*((volatile u16*) 0x4C)
#define TIMER1_OCR1A		*((volatile u16*) 0x4A)
#define TIMER1_OCR1B		*((volatile u16*) 0x48)
#define TIMER1_ICR1			*((volatile u16*) 0x46)
#define TIMER1_TIMSK		*((volatile u8*)  0x59)
#define TIMER1_TIFR			*((volatile u8*)  0x58)

#define TIMER1_TCCR1B_PRESCALER_MASK				0x07

#define TCCR1A_CMPB_MATCH_OFFSET				4
#define TCCR1A_CMPA_MATCH_OFFSET				6
#define TCCR1B_ICES1_OFFSET						6
#define TCCR1B_ICNC1_OFFSET						7
#define TCCR1A_WGM10								0
#define TCCR1A_WGM11								1
#define TCCR1B_WGM12								3
#define TCCR1B_WGM13								4

#define TIFR_ICF1								5

#define MACRO_PRESCALER		(TIMER1_TCCR1B & TIMER1_TCCR1B_PRESCALER_MASK)
#define MACRO_MODE			(((TIMER1_TCCR1B & (1 << TCCR1B_WGM13)) >> TCCR1B_WGM13 << 3) | ((TIMER1_TCCR1B & (1 << TCCR1B_WGM12)) >> TCCR1B_WGM12 << 2) | ((TIMER1_TCCR1A & (1 << TCCR1A_WGM11)) >> TCCR1A_WGM11 << 1) | ((TIMER1_TCCR1A & (1 << TCCR1A_WGM10)) >> TCCR1A_WGM10))

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

#endif /* TIMER1_PRIVATE_H_ */