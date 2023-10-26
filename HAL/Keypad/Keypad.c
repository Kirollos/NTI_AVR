/*
 * Keypad.c
 *
 * Created: 21/10/2023 2:26:46 PM
 *  Author: Kirollos
 */ 

#include "../STD_TYPES.h"
#include "Keypad_config.h"
#include "Keypad.h"
#include "../MCAL/PortInterface.h"
#define F_CPU 16000000  // temp
#include <util/delay.h> // temp

#if KEYPAD_ROW_DDR == KEYPAD_OUTPUT
#define first_it row_arr
#define second_it col_arr
#else
#define first_it col_arr
#define second_it row_arr
#endif

static u8 row_arr[][2] = {
	{KEYPAD_ROW1_PORT,KEYPAD_ROW1_PIN},
	{KEYPAD_ROW2_PORT,KEYPAD_ROW2_PIN},
	{KEYPAD_ROW3_PORT,KEYPAD_ROW3_PIN},
	{KEYPAD_ROW4_PORT,KEYPAD_ROW4_PIN}
};
static u8 col_arr[][2] = {
	{KEYPAD_COL1_PORT,KEYPAD_COL1_PIN},
	{KEYPAD_COL2_PORT,KEYPAD_COL2_PIN},
	{KEYPAD_COL3_PORT,KEYPAD_COL3_PIN},
	{KEYPAD_COL4_PORT,KEYPAD_COL4_PIN}
};

static u8 matrix[KEYPAD_ROWS][KEYPAD_COLUMNS] = {
	{KEYPAD_R1C1,KEYPAD_R1C2,KEYPAD_R1C3,KEYPAD_R1C4},
	{KEYPAD_R2C1,KEYPAD_R2C2,KEYPAD_R2C3,KEYPAD_R2C4},
	{KEYPAD_R3C1,KEYPAD_R3C2,KEYPAD_R3C3,KEYPAD_R3C4},
	{KEYPAD_R4C1,KEYPAD_R4C2,KEYPAD_R4C3,KEYPAD_R4C4},
};

static u8 keyPressed = 0;

void Keypad_Init()
{
	// Set DDRs
	DIO_voidSetPinDirection(KEYPAD_ROW1_PORT, KEYPAD_ROW1_PIN, KEYPAD_ROW_DDR);
	DIO_voidSetPinDirection(KEYPAD_ROW2_PORT, KEYPAD_ROW2_PIN, KEYPAD_ROW_DDR);
	DIO_voidSetPinDirection(KEYPAD_ROW3_PORT, KEYPAD_ROW3_PIN, KEYPAD_ROW_DDR);
	DIO_voidSetPinDirection(KEYPAD_ROW4_PORT, KEYPAD_ROW4_PIN, KEYPAD_ROW_DDR);
	
	DIO_voidSetPinDirection(KEYPAD_COL1_PORT, KEYPAD_COL1_PIN, KEYPAD_COL_DDR);
	DIO_voidSetPinDirection(KEYPAD_COL2_PORT, KEYPAD_COL2_PIN, KEYPAD_COL_DDR);
	DIO_voidSetPinDirection(KEYPAD_COL3_PORT, KEYPAD_COL3_PIN, KEYPAD_COL_DDR);
	DIO_voidSetPinDirection(KEYPAD_COL4_PORT, KEYPAD_COL4_PIN, KEYPAD_COL_DDR);
	
	// Set rows initial values
	DIO_voidSetPinValue(KEYPAD_ROW1_PORT, KEYPAD_ROW1_PIN, 1);
	DIO_voidSetPinValue(KEYPAD_ROW2_PORT, KEYPAD_ROW2_PIN, 1);
	DIO_voidSetPinValue(KEYPAD_ROW3_PORT, KEYPAD_ROW3_PIN, 1);
	DIO_voidSetPinValue(KEYPAD_ROW4_PORT, KEYPAD_ROW4_PIN, 1);
	// Set columns initial values
	DIO_voidSetPinValue(KEYPAD_COL1_PORT, KEYPAD_COL1_PIN, 0);
	DIO_voidSetPinValue(KEYPAD_COL2_PORT, KEYPAD_COL2_PIN, 0);
	DIO_voidSetPinValue(KEYPAD_COL3_PORT, KEYPAD_COL3_PIN, 0);
	DIO_voidSetPinValue(KEYPAD_COL4_PORT, KEYPAD_COL4_PIN, 0);
}

u8 Keypad_GetKey()
{
	u8 val = 0;
	for(u8 i = 0; i < 4; i++)
	{
		DIO_voidSetPinValue(first_it[(i)%4][0], first_it[(i)%4][1], 0);
		DIO_voidSetPinValue(first_it[(i+1)%4][0], first_it[(i+1)%4][1], 1);
		DIO_voidSetPinValue(first_it[(i+2)%4][0], first_it[(i+2)%4][1], 1);
		DIO_voidSetPinValue(first_it[(i+3)%4][0], first_it[(i+3)%4][1], 1);
		_delay_ms(20); // wait to scan
		for(u8 j = 0; j < 4; j++)
		{
			u8 pin = DIO_u8GetPinValue(second_it[j][0], second_it[j][1]);
			if(pin == 0)
			{
				if(keyPressed) return 0;
				keyPressed = 1;
				val = matrix[i][j];
				return val;
			}
			_delay_ms(2);
		}
		_delay_ms(2);
	}
	if(keyPressed)
		keyPressed = 0;
	/*DIO_voidSetPinValue(KEYPAD_ROW1_PORT, KEYPAD_ROW1_PIN, 1);
	_delay_ms(50);
	val = DIO_u8GetPinValue(KEYPAD_COL1_PORT, KEYPAD_COL1_PIN) == 1;*/
	return 0;
}