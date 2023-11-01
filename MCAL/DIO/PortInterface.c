/*
 * PortInterface.c
 *
 * Created: 21/10/2023 8:56:29 AM
 *  Author: Kirollos
 */ 

#include "../../STD_TYPES.h"
#include "PortStruct.h"
#include "PortInterface.h"

/*
extern _GPIO* GPIO_PortA;
extern _GPIO* GPIO_PortB;
extern _GPIO* GPIO_PortC;
extern _GPIO* GPIO_PortD;
*/

void DIO_voidSetPinDirection(PortType port, u8 Copy_u8PinId, u8 Copy_u8Direction)
{
	_GPIO* p;
	switch(port)
	{
		case PortA:
		p = GPIO_PortA;
		break;
		case PortB:
		p = GPIO_PortB;
		break;
		case PortC:
		p = GPIO_PortC;
		break;
		case PortD:
		p = GPIO_PortD;
		break;
		default:
		return;
	}
	__DIO_voidSetPinDirection(p, Copy_u8PinId, Copy_u8Direction);
}

void DIO_voidSetPinValue(PortType port, u8 Copy_u8PinId, u8 Copy_u8Value)
{
	_GPIO* p;
	switch(port)
	{
		case PortA:
		p = GPIO_PortA;
		break;
		case PortB:
		p = GPIO_PortB;
		break;
		case PortC:
		p = GPIO_PortC;
		break;
		case PortD:
		p = GPIO_PortD;
		break;
		default:
		return;
	}
	__DIO_voidSetPinValue(p, Copy_u8PinId, Copy_u8Value);
}

u8 DIO_u8GetPinValue(PortType port, u8 Copy_u8PinId)
{
	_GPIO* p;
	switch(port)
	{
		case PortA:
		p = GPIO_PortA;
		break;
		case PortB:
		p = GPIO_PortB;
		break;
		case PortC:
		p = GPIO_PortC;
		break;
		case PortD:
		p = GPIO_PortD;
		break;
		default:
		return 0;
	}
	return __DIO_u8GetPinValue(p, Copy_u8PinId);
}

void DIO_voidSetPortDirection(PortType port, u8 Copy_u8Direction)
{
	_GPIO* p;
	switch(port)
	{
		case PortA:
		p = GPIO_PortA;
		break;
		case PortB:
		p = GPIO_PortB;
		break;
		case PortC:
		p = GPIO_PortC;
		break;
		case PortD:
		p = GPIO_PortD;
		break;
		default:
		return;
	}
	__DIO_voidSetPortDirection(p, Copy_u8Direction);
}

void DIO_voidSetPortValue(PortType port, u8 Copy_u8Value)
{
	_GPIO* p;
	switch(port)
	{
		case PortA:
		p = GPIO_PortA;
		break;
		case PortB:
		p = GPIO_PortB;
		break;
		case PortC:
		p = GPIO_PortC;
		break;
		case PortD:
		p = GPIO_PortD;
		break;
		default:
		return;
	}
	__DIO_voidSetPortValue(p, Copy_u8Value);
}

u8 DIO_u8GetPortValue(PortType port)
{
	_GPIO* p;
	switch(port)
	{
		case PortA:
		p = GPIO_PortA;
		break;
		case PortB:
		p = GPIO_PortB;
		break;
		case PortC:
		p = GPIO_PortC;
		break;
		case PortD:
		p = GPIO_PortD;
		break;
		default:
		return 0;
	}
	return __DIO_u8GetPortValue(p);
}