/*
 * Keypad_config.h
 *
 * Created: 21/10/2023 2:27:19 PM
 *  Author: Kirollos
 */ 

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

#define KEYPAD_INPUT			0
#define KEYPAD_OUTPUT			1

#define KEYPAD_ROW_DDR			KEYPAD_OUTPUT
#define KEYPAD_COL_DDR			KEYPAD_INPUT

#define KEYPAD_ROW1_PORT		PortC
#define KEYPAD_ROW1_PIN			5

#define KEYPAD_ROW2_PORT		PortC
#define KEYPAD_ROW2_PIN			4

#define KEYPAD_ROW3_PORT		PortC
#define KEYPAD_ROW3_PIN			3

#define KEYPAD_ROW4_PORT		PortC
#define KEYPAD_ROW4_PIN			2

#define KEYPAD_COL1_PORT		PortD
#define KEYPAD_COL1_PIN			7

#define KEYPAD_COL2_PORT		PortD
#define KEYPAD_COL2_PIN			6

#define KEYPAD_COL3_PORT		PortD
#define KEYPAD_COL3_PIN			5

#define KEYPAD_COL4_PORT		PortD
#define KEYPAD_COL4_PIN			3

#define KEYPAD_ROWS				4
#define KEYPAD_COLUMNS			4//3

// Key definition
#define KEYPAD_R1C1				'1'
#define KEYPAD_R1C2				'2'
#define KEYPAD_R1C3				'3'
#define KEYPAD_R1C4				//'4'

#define KEYPAD_R2C1				'4'//'5'
#define KEYPAD_R2C2				'5'//'6'
#define KEYPAD_R2C3				'6'//'7'
#define KEYPAD_R2C4				//'8'

#define KEYPAD_R3C1				'7'//'9'
#define KEYPAD_R3C2				'8'//'A'
#define KEYPAD_R3C3				'9'//'B'
#define KEYPAD_R3C4				//'C'

#define KEYPAD_R4C1				'*'//'D'
#define KEYPAD_R4C2				'0'//'E'
#define KEYPAD_R4C3				'#'//'F'
#define KEYPAD_R4C4				//'0'


#endif /* KEYPAD_CONFIG_H_ */