/*
 * LCD_config.h
 *
 * Created: 23/10/2023 10:59:45 AM
 *  Author: Kirollos
 */ 


#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#define LCD_RS_PORT		PortA
#define LCD_RS_PIN		3

#define LCD_EN_PORT		PortA
#define LCD_EN_PIN		2

#define LCD_DATA_PORT	PortB
#define LCD_DATA_D0		0
#define LCD_DATA_D1		0
#define LCD_DATA_D2		0
#define LCD_DATA_D3		0

#define LCD_DATA_D4		0
#define LCD_DATA_D5		1
#define LCD_DATA_D6		2
#define LCD_DATA_D7		4

#define LCD_MODE		__4_BIT_MODE
#define LCD_LINES		__2_LINES
#define LCD_FONT		__FONT_5x10
#define LCD_DISPLAY		1
#define LCD_CURSOR		1
#define LCD_CURS_BLINK	0
#define LCD_ENTRY		__MODE_INC
#define LCD_DISP_SHIFT	__DISP_SHIFT_OFF



#endif /* LCD_CONFIG_H_ */