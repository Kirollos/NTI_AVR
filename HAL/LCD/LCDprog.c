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

static volatile u8 r=0, c=0;

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
	_delay_ms(5);
	// lower 4 bits
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D7, GET_BIT(copy_u8Byte, 3));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D6, GET_BIT(copy_u8Byte, 2));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D5, GET_BIT(copy_u8Byte, 1));
	DIO_voidSetPinValue(LCD_DATA_PORT, LCD_DATA_D4, GET_BIT(copy_u8Byte, 0));
	// enable pulse
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 1);
	_delay_ms(1);
	DIO_voidSetPinValue(LCD_EN_PORT, LCD_EN_PIN, 0);
	_delay_ms(5);
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
	_delay_ms(1);
	H_LCD_void_sendCommand(LCD_FUNC_SET);
	_delay_ms(1);
	H_LCD_void_sendCommand(LCD_FUNC_SET | (LCD_MODE << 4) | (LCD_LINES << 3) | (LCD_FONT << 2));
	
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
	c++;
	if(c%20 == 0)
	{
		c = 0;
		r = (r+1) % 4;
		H_LCD_void_gotoXY(r, 0);
	}
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
	//strcpy(szNumber, itoa(copy_s32Num, 10));
	H_LCD_void_sendString(szNumber);
}

void H_LCD_void_sendDouble(double copy_doubleNum)
{
	u8 szNumber[20];
	sprintf(szNumber, "%.2f", copy_doubleNum);
	//strcpy(szNumber, ftoa(copy_doubleNum, 2));
	H_LCD_void_sendString(szNumber);
}

void H_LCD_void_gotoXY(u8 copy_u8Row,u8 copy_u8Col)
{
	// x ranges from 0 to LCD_COLUMNS
	// y ranges from 0 to LCD_ROWS
	r = copy_u8Row%4;
	c = copy_u8Col%20;
	#if LCD_LINES == __2_LINES
	// Set DDRAM
	switch(copy_u8Row)
	{
		case 0:
		H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | (0x00 +copy_u8Col));
		break;
		case 1:
		H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | (0x40 +copy_u8Col));
		break;
		case 2:
		H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | (0x14 +copy_u8Col));
		break;
		case 3:
		H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | (0x54 +copy_u8Col));
		break;
	}
	#else
	H_LCD_void_sendCommand(LCD_SET_DDRAM_ADDR | (20*copy_u8Row + copy_u8Col));
	#endif
}

void H_LCD_void_getXY(u8* pRow, u8* pCol)
{
	*pRow = r;
	*pCol = c;
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
	r = c = 0;
}