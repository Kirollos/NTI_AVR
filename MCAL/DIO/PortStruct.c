/*
 * PortInterface.c
 *
 * Created: 21/10/2023 8:56:29 AM
 *  Author: Kirollos
 */ 

#include "STD_TYPES.h"
#include "PortStruct.h"

_GPIO* GPIO_PortA = (_GPIO*) 0x39;
_GPIO* GPIO_PortB = (_GPIO*) 0x36;
_GPIO* GPIO_PortC = (_GPIO*) 0x33;
_GPIO* GPIO_PortD = (_GPIO*) 0x30;


void __DIO_voidSetPinDirection(_GPIO* port, u8 Copy_u8PinId, u8 Copy_u8Direction)
{
	/*
	// Method 1
	if(!Copy_u8Direction)
		port->DDR.Byte &= ~(1 << Copy_u8PinId);
	else
		port->DDR.Byte |= (Copy_u8Direction << Copy_u8PinId);
	*/
	// Method 2
	switch(Copy_u8PinId)
	{
		case 0:
		port->DDR.Bit.B0 = Copy_u8Direction;
		break;
		case 1:
		port->DDR.Bit.B1 = Copy_u8Direction;
		break;
		case 2:
		port->DDR.Bit.B2 = Copy_u8Direction;
		break;
		case 3:
		port->DDR.Bit.B3 = Copy_u8Direction;
		break;
		case 4:
		port->DDR.Bit.B4 = Copy_u8Direction;
		break;
		case 5:
		port->DDR.Bit.B5 = Copy_u8Direction;
		break;
		case 6:
		port->DDR.Bit.B6 = Copy_u8Direction;
		break;
		case 7:
		port->DDR.Bit.B7 = Copy_u8Direction;
		break;
	}
}

void __DIO_voidSetPinValue(_GPIO* port, u8 Copy_u8PinId, u8 Copy_u8Value)
{
	/*
	// Method 1
	if(!Copy_u8Value)
		port->PORT.Byte &= ~(1 << Copy_u8PinId);
	else
		port->PORT.Byte |= (Copy_u8Value << Copy_u8PinId);
	*/
	// Method 2
	switch(Copy_u8PinId)
	{
		case 0:
		port->PORT.Bit.B0 = Copy_u8Value;
		break;
		case 1:
		port->PORT.Bit.B1 = Copy_u8Value;
		break;
		case 2:
		port->PORT.Bit.B2 = Copy_u8Value;
		break;
		case 3:
		port->PORT.Bit.B3 = Copy_u8Value;
		break;
		case 4:
		port->PORT.Bit.B4 = Copy_u8Value;
		break;
		case 5:
		port->PORT.Bit.B5 = Copy_u8Value;
		break;
		case 6:
		port->PORT.Bit.B6 = Copy_u8Value;
		break;
		case 7:
		port->PORT.Bit.B7 = Copy_u8Value;
		break;
	}
}

u8 __DIO_u8GetPinValue(_GPIO* port, u8 Copy_u8PinId)
{
	//return port->PIN.Byte & (1 << Copy_u8PinId);
	u8 ret = 0;
	switch(Copy_u8PinId)
	{
		case 0:
		ret = port->PIN.Bit.B0;
		break;
		case 1:
		ret = port->PIN.Bit.B1;
		break;
		case 2:
		ret = port->PIN.Bit.B2;
		break;
		case 3:
		ret = port->PIN.Bit.B3;
		break;
		case 4:
		ret = port->PIN.Bit.B4;
		break;
		case 5:
		ret = port->PIN.Bit.B5;
		break;
		case 6:
		ret = port->PIN.Bit.B6;
		break;
		case 7:
		ret = port->PIN.Bit.B7;
		break;
	}
	return ret;
}

void __DIO_voidSetPortDirection(_GPIO* port, u8 Copy_u8Direction)
{
	port->DDR.Byte = Copy_u8Direction;
}

void __DIO_voidSetPortValue(_GPIO* port, u8 Copy_u8Value)
{
	port->PORT.Byte = Copy_u8Value;
}

u8 __DIO_u8GetPortValue(_GPIO* port)
{
	return port->PIN.Byte;
}