/*
 * _7SEG.c
 *
 * Created: 21/10/2023 11:29:09 AM
 *  Author: Kirollos
 */ 

#include "STD_TYPES.h"
#include "7SEG_CONFIG.h"
#include "7SEG.h"
#include "PortInterface.h"


void SevSeg_Init()
{
	DIO_voidSetPinDirection(SEVSEG_BCD_PORT, SEVSEG_BCD_A, 1);
	DIO_voidSetPinDirection(SEVSEG_BCD_PORT, SEVSEG_BCD_B, 1);
	DIO_voidSetPinDirection(SEVSEG_BCD_PORT, SEVSEG_BCD_C, 1);
	DIO_voidSetPinDirection(SEVSEG_BCD_PORT, SEVSEG_BCD_D, 1);
	DIO_voidSetPinDirection(SEVSEG_COM1_PORT, SEVSEG_COM1_PIN, 1);
	DIO_voidSetPinDirection(SEVSEG_COM2_PORT, SEVSEG_COM2_PIN, 1);
	DIO_voidSetPinDirection(SEVSEG_COM3_PORT, SEVSEG_COM3_PIN, 1);
	DIO_voidSetPinDirection(SEVSEG_COM4_PORT, SEVSEG_COM4_PIN, 1);
	DIO_voidSetPinValue(SEVSEG_COM1_PORT, SEVSEG_COM1_PIN, 1);
	DIO_voidSetPinValue(SEVSEG_COM2_PORT, SEVSEG_COM2_PIN, 1);
	DIO_voidSetPinValue(SEVSEG_COM3_PORT, SEVSEG_COM3_PIN, 1);
	DIO_voidSetPinValue(SEVSEG_COM4_PORT, SEVSEG_COM4_PIN, 1);
}

void SevSeg_Display(u8 number)
{
	// digit
	//u8 digit = number % 10;
	//u8 tens = (number/10) % 10;
	//u8 hundreds = (number/100) % 10;
	//u8 thousands = (number/1000);
	DIO_voidSetPinValue(SEVSEG_BCD_PORT, SEVSEG_BCD_A, (number & 1));
	DIO_voidSetPinValue(SEVSEG_BCD_PORT, SEVSEG_BCD_B, (number & 2) >> 1);
	DIO_voidSetPinValue(SEVSEG_BCD_PORT, SEVSEG_BCD_C, (number & 4) >> 2);
	DIO_voidSetPinValue(SEVSEG_BCD_PORT, SEVSEG_BCD_D, (number & 8) >> 3);
	DIO_voidSetPinValue(SEVSEG_COM1_PORT, SEVSEG_COM1_PIN, 0);
}