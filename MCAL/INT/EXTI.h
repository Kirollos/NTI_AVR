/*
 * EXTI.h
 *
 * Created: 22/10/2023 2:13:19 PM
 *  Author: Kirollos
 */ 


#ifndef EXTI_H_
#define EXTI_H_

#define EXTI_INT0	6
#define EXTI_INT1	7
#define EXTI_INT2	5

#define EXTI_MODE_LOW_CHANGE	0
#define EXTI_MODE_ANY_CHANGE	1
#define EXTI_MODE_FALL_EDGE		2
#define EXTI_MODE_RISE_EDGE		3

void EXTI_voidEnable(u8 copy_u8ID, u8 copy_u8TriggerID);
void EXTI_voidDisable(u8 copy_u8ID);
void EXTI_voidSetCallback(u8 copy_u8ID, void (*cbfn)(void));


#endif /* EXTI_H_ */