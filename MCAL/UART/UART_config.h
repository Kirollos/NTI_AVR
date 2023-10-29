/*
 * UART_config.h
 *
 * Created: 29/10/2023 9:48:26 PM
 *  Author: Kirollos
 */ 


#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

/* Data Size:
0 - 5bits
1 - 6bits
2 - 7bits
3 - 8bits
7 - 9bits
*/
#define UART_DATA_SIZE		3

#define F_CPU				16000000
#define UART_BAUD_RATE		9600

/* Mode:
0 - Asynchronous
1 - Synchronous
*/
#define UART_MODE			0

/* Parity:
0 - Disabled
2 - Enabled - Even parity
3 - Enabled - Odd parity
*/
#define UART_PARITY_MODE	0

/* Stop bits:
0 - 1 bit
1 - 2 bits
*/
#define UART_STOP_BITS		0



#endif /* UART_CONFIG_H_ */