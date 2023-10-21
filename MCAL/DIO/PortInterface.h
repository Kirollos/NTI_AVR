/*
 * PortInterface.c
 *
 * Created: 21/10/2023 8:56:29 AM
 *  Author: Kirollos
 */ 


#ifndef PORTINTERFACE_H_
#define PORTINTERFACE_H_

typedef unsigned char u8;

typedef enum
{
	PortA,
	PortB,
	PortC,
	PortD	
} PortType;

void DIO_voidSetPinDirection(PortType port, u8 Copy_u8PinId, u8 Copy_u8Direction);
void DIO_voidSetPinValue(PortType port, u8 Copy_u8PinId, u8 Copy_u8Value);
u8 DIO_u8GetPinValue(PortType port, u8 Copy_u8PinId);
void DIO_voidSetPortDirection(PortType port, u8 Copy_u8Direction);
void DIO_voidSetPortValue(PortType port, u8 Copy_u8Value);
u8 DIO_u8GetPortValue(PortType port);


#endif /* PORTINTERFACE_H_ */