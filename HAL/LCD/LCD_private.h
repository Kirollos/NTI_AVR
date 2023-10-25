/*
 * LCD_private.h
 *
 * Created: 23/10/2023 10:59:53 AM
 *  Author: Kirollos
 */ 


#ifndef LCD_PRIVATE_H_
#define LCD_PRIVATE_H_

// bit math
//#define GET_BIT(x,bth)	((x & (1 << bth))==1)
#define GET_BIT(x,bth)	((x>>bth) & 1)

// Commands
#define	LCD_CLEAR		0x01
#define LCD_RET_HOME	0x02
#define LCD_ENT_MODE	0x04
#define LCD_DISP_ONOFF	0x08
#define LCD_CURS_SHIFT	0x10
#define	LCD_FUNC_SET	0x20
#define LCD_SET_CGRAM_ADDR	0x40
#define LCD_SET_DDRAM_ADDR	0x80

#if LCD_LINES == __2_LINES
#define LCD_ROWS		04
#else
#define LCD_ROWS		01
#endif
#define LCD_COLUMNS		20

#define __4_BIT_MODE	0
#define __8_BIT_MODE	1
#define __1_LINES		0
#define __2_LINES		1
#define __FONT_5x7		0
#define __FONT_5x10		1
#define __MODE_INC		1
#define __MODE_DEC		0
#define __DISP_SHIFT_OFF 0
#define __DISP_SHIFT_ON  1


#endif /* LCD_PRIVATE_H_ */