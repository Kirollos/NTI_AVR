/*
 * PortInterface.c
 *
 * Created: 21/10/2023 8:56:29 AM
 *  Author: Kirollos
 */ 

#ifndef __PORTSTRUCT_H__
#define __PORTSTRUCT_H__

typedef unsigned char u8;

typedef struct
{
	union __GPIOREG
	{
		u8 Byte;
		struct{
			u8 B0:1;
			u8 B1:1;
			u8 B2:1;
			u8 B3:1;
			u8 B4:1;
			u8 B5:1;
			u8 B6:1;
			u8 B7:1;
		} Bit;
	};

	union __GPIOREG PIN;
	union __GPIOREG DDR;
	union __GPIOREG PORT;
}  My_GPIO;

typedef volatile My_GPIO _GPIO;

void __DIO_voidSetPinDirection(_GPIO* port, u8 Copy_u8PinId, u8 Copy_u8Direction);
void __DIO_voidSetPinValue(_GPIO* port, u8 Copy_u8PinId, u8 Copy_u8Value);
u8 __DIO_u8GetPinValue(_GPIO* port, u8 Copy_u8PinId);
void __DIO_voidSetPortDirection(_GPIO* port, u8 Copy_u8Direction);
void __DIO_voidSetPortValue(_GPIO* port, u8 Copy_u8Value);
u8 __DIO_u8GetPortValue(_GPIO* port);

#endif // __PORTSTRUCT_H__
