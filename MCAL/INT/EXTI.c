/*
 * EXTI.c
 *
 * Created: 22/10/2023 2:13:29 PM
 *  Author: Kirollos
 */ 
#include "../../STD_TYPES.h"
#include "EXTI.h"
#include <avr/interrupt.h>

#define I_FLAG		7

#define EXTI_GICR_REG	*((volatile u8*) 0x5B)
#define EXTI_MCUCR_REG	*((volatile u8*) 0x55)
#define EXTI_MCUCSR_REG	*((volatile u8*) 0x54)

/*
0 - INT 0
1 - INT 1
2 - INT 2
*/
#define EXTI_CB_INT0	0
#define EXTI_CB_INT1	1
#define EXTI_CB_INT2	2
static void (*EXTI_cbfn[3])(void) = {0, 0, 0};

void EXTI_voidEnable(u8 copy_u8ID, u8 copy_u8TriggerID)
{
	EXTI_GICR_REG |= (1 << copy_u8ID);
	if(copy_u8ID == EXTI_INT0)
	{
		EXTI_MCUCR_REG &= ~(3);
		EXTI_MCUCR_REG |= copy_u8TriggerID;
	}
	else
	if(copy_u8ID == EXTI_INT1)
	{
		EXTI_MCUCR_REG &= ~(3 << 2);
		EXTI_MCUCR_REG |= (copy_u8TriggerID << 2);
	}
	else
	if(copy_u8ID == EXTI_INT2)
	{
		EXTI_MCUCSR_REG &= ~(1 << 6);
		EXTI_MCUCSR_REG |= (copy_u8TriggerID==EXTI_MODE_RISE_EDGE << 6);
	}
}

void EXTI_voidDisable(u8 copy_u8ID)
{
	EXTI_GICR_REG &= ~(1 << copy_u8ID);
}

void EXTI_voidSetCallback(u8 copy_u8ID, void (*cbfn)(void))
{
	switch(copy_u8ID)
	{
		case EXTI_INT0:
		EXTI_cbfn[EXTI_CB_INT0] = cbfn;
		break;
		case EXTI_INT1:
		EXTI_cbfn[EXTI_CB_INT1] = cbfn;
		break;
		case EXTI_INT2:
		EXTI_cbfn[EXTI_CB_INT2] = cbfn;
		break;
	}
}

void __vector_1 (void) __attribute__((signal));
void __vector_1 (void)
{
	if(EXTI_cbfn[EXTI_CB_INT0] != 0)
	{
		(*EXTI_cbfn[EXTI_CB_INT0])();
	}
}

void __vector_2 (void) __attribute__((signal));
void __vector_2 (void)
{
	if(EXTI_cbfn[EXTI_CB_INT1] != 0)
	{
		(*EXTI_cbfn[EXTI_CB_INT1])();
	}
}

void __vector_3 (void) __attribute__((signal));
void __vector_3 (void)
{
	if(EXTI_cbfn[EXTI_CB_INT2] != 0)
	{
		(*EXTI_cbfn[EXTI_CB_INT2])();
	}
}