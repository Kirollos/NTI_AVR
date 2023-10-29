/*
 * UART_private.h
 *
 * Created: 29/10/2023 9:48:34 PM
 *  Author: Kirollos
 */ 


#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_


#define UART_UCSRA		*((volatile u8*) 0x2b)
#define UART_UCSRB		*((volatile u8*) 0x2a)
#define UART_UCSRC		*((volatile u8*) 0x40)
#define UART_UDR		*((volatile u8*) 0x2c)
#define UART_UBRRL		*((volatile u8*) 0x29)
#define UART_UBRRH			UART_UCSRC

#define UART_UCSRC_UCSZ0		1
#define UART_UCSRC_UCSZ1		2
#define UART_UCSRC_USBS			3
#define UART_UCSRC_UPM0			4
#define UART_UCSRC_UPM1			5
#define UART_UCSRC_UMSEL		6
#define UART_UCSRC_URSEL		7
#define UART_UCSRB_UCSZ2		2
#define UART_UCSRB_TXB8			0
#define UART_UCSRB_RXB8			1
#define UART_UCSRB_TXEN			3
#define UART_UCSRB_RXEN			4
#define UART_UCSRB_UDRIE		5
#define UART_UCSRB_TXCIE		6
#define UART_UCSRB_RXCIE		7
#define UART_UCSRA_TXC			6
#define UART_UCSRA_RXC			7
#define UART_UCSRA_UDRE			5

#define UART_BAUD_VALUE		(((F_CPU)/(UART_BAUD_RATE*16UL))-1)


#define UART_SETREG(reg, value) do {\
u8 tmp_reg = value;\
reg = tmp_reg;\
} while(0)

#define UART_SET_USCRC(value) do {\
u8 tmp_reg = value;\
tmp_reg |= (1<<UART_UCSRC_URSEL);\
UART_UCSRC = tmp_reg;\
} while(0)


#endif /* UART_PRIVATE_H_ */