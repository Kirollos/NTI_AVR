/*
 * LCDprog.c
 *
 * Created: 23/10/2023 10:59:29 AM
 *  Author: Kirollos
 */ 

#include <stdio.h>
#include "../../MCAL/PortInterface.h" // DIO
#include "LCD_private.h"
#include "LCD_config.h"
#include "LCD.h"

#define F_CPU 16000000  // temp
#include <util/delay.h> // temp

static void H_LCD_void_latchByte(u8 copy_u8Byte)
{
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 0);
	_delay_ms(1);
	#if LCD_MODE == __4_BIT_MODE
	// higher 4 bits
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D7, GET_BIT(copy_u8Byte, 7));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D6, GET_BIT(copy_u8Byte, 6));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D5, GET_BIT(copy_u8Byte, 5));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D4, GET_BIT(copy_u8Byte, 4));
	// enable pulse
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 1);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 0);
	_delay_ms(30);
	// lower 4 bits
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D7, GET_BIT(copy_u8Byte, 3));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D6, GET_BIT(copy_u8Byte, 2));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D5, GET_BIT(copy_u8Byte, 1));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D4, GET_BIT(copy_u8Byte, 0));
	// enable pulse
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 1);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 0);
	_delay_ms(30);
	#elif LCD_MODE == __8_BIT_MODE
	DIO_voidSetPortValue(LCD_DATA_PORT, copy_u8Byte);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 0);
	_delay_ms(2);
	#endif
}

void H_LCD_void_Init(void)
{
	_delay_ms(50);
	DIO_voidSetPinDirection(LCD_RS_PORT, LCD_RS_PIN, 1);
	DIO_voidSetPinDirection(LCD_EN_PORT, LCD_EN_PIN, 1);
	
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 0);
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, 0);
	
	#if LCD_MODE == __4_BIT_MODE
	
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D7, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D6, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D5, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D4, 1);
	
	// Function set
	H_LCD_void_sendCommand(LCD_FUNC_SET);
	_delay_ms(5);
	H_LCD_void_sendCommand(LCD_FUNC_SET);
	_delay_ms(5);
	H_LCD_void_sendCommand((LCD_LINES << 7) | (LCD_FONT << 6));
	
	#elif LCD_MODE == __8_BIT_MODE

	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D0, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D1, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D2, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D3, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D4, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D5, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D6, 1);
	DIO_voidSetPinDirection(LCD_DATA_PORT, LCD_DATA_D7, 1);
	
	// Function Set
	H_LCD_void_sendCommand(LCD_FUNC_SET);
	H_LCD_void_sendCommand(LCD_FUNC_SET | (LCD_MODE << 4) | (LCD_LINES << 7) | (LCD_FONT << 6));
	#else
	#error ("Invalid LCD mode.")
	#endif
	
	// Display on-off
	H_LCD_void_sendCommand(LCD_DISP_ONOFF | (LCD_DISPLAY << 2) | (LCD_CURSOR << 1) | (LCD_CURS_BLINK << 0));
	// Display clear
	H_LCD_void_sendCommand(LCD_CLEAR);
	// Entry mode
	H_LCD_void_sendCommand(LCD_ENT_MODE | (LCD_ENTRY << 1) | (LCD_DISP_SHIFT << 0));
	// End.
	
	H_LCD_void_clearScreen();
}

void H_LCD_void_sendData(u8 copy_u8data)
{
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, 1);
	_delay_ms(1);
	H_LCD_void_latchByte(copy_u8data);
}

void H_LCD_void_sendCommand(u8 copy_u8command)
{
	DIO_voidSetPinValue(LCD_RS_PORT, LCD_RS_PIN, 0);
	_delay_ms(1);
	H_LCD_void_latchByte(copy_u8command);
}

void H_LCD_void_sendString(const s8 * pstr)
{
	u8 i;
	for(i = 0; pstr[i] != '\0'; i++)
	{
		H_LCD_void_sendData(pstr[i]);
	}
}

void H_LCD_void_sendIntNum(s32 copy_s32Num)
{
	u8 szNumber[10];
	sprintf(szNumber, "%d", copy_s32Num);
	H_LCD_void_sendString(szNumber);
}

void H_LCD_void_gotoXY(u8 copy_u8Row,u8 copy_u8Col)
{
	// x ranges from 0 to LCD_COLUMNS
	// y ranges from 0 to LCD_ROWS
	#if LCD_LINES == 2
	static u8 addresses[LCD_ROWS][LCD_COLUMNS] =
	{
		{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13},
		{0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53},
		{0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27},
		{0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67}
	};
	// Set DDRAM
	H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | addresses[copy_u8Row][copy_u8Col]);
	#else
	H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | (20*copy_u8Row + copy_u8Col));
	#endif
}

void H_LCD_void_creatCustomChar(const u8 * ArrPattern,u8 copy_u8charCode)
{
	copy_u8charCode &= 0x7; // & 00000111
	
	H_LCD_void_sendCommand(LCD_SET_CGRAM_ADDR + (8*copy_u8charCode));
	_delay_ms(30);
	for(u8 i = 0; i < 8; i++)
	{
		H_LCD_void_sendData(ArrPattern[i]);
		_delay_ms(40);
	}
}

void H_LCD_void_displayCustomChar(u8 copy_u8charCode)
{
	H_LCD_void_sendData(copy_u8charCode);
}

void H_LCD_void_clearScreen(void)
{
	H_LCD_void_sendCommand(LCD_CLEAR);
	_delay_ms(2);
	H_LCD_void_sendCommand(0x80);
}