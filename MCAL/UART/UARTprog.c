/*
 * UARTprog.c
 *
 * Created: 29/10/2023 9:47:58 PM
 *  Author: Kirollos
 */ 

#include "UART_config.h"
#include "UART_private.h"
#include "UART.h"

static void (*RXC_cb)(u8);

void UART_Init(u16 copyu16_baudRate)
{
	u8 b = 0, c = 0;
	UART_UBRRH = (UART_BAUD_VALUE(copyu16_baudRate)&0xFF00) >> 8;
	UART_UBRRL = (UART_BAUD_VALUE(copyu16_baudRate)&0x00FF);
	#if UART_DATA_SIZE == 7
	b |= (1 << UART_UCSRB_UCSZ2);
	c |= (3 << UART_UCSRC_UCSZ0);
	#else
	c |= (UART_DATA_SIZE << UART_UCSRC_UCSZ0);
	#endif
	c |= (UART_STOP_BITS << UART_UCSRC_USBS) | (UART_PARITY_MODE << UART_UCSRC_UPM0);
	c |= (UART_MODE << UART_UCSRC_UMSEL);
	c |= (1 << UART_UCSRC_URSEL);
	
	UART_UCSRA = 0;
	UART_UCSRB = b;
	UART_UCSRC = c;
}

void UART_TXEnable()
{
	UART_UCSRB |= (1 << UART_UCSRB_TXEN);
}

void UART_TXDisable()
{
	UART_UCSRB &= ~(1 << UART_UCSRB_TXEN);
}

void UART_RXEnable()
{
	UART_UCSRB |= (1 << UART_UCSRB_RXEN);
}

void UART_RXDisable()
{
	UART_UCSRB &= ~(1 << UART_UCSRB_RXEN);
}

void UART_SendByteSync(u8 copyu8_Data)
{
	if(! (UART_UCSRB & (1 << UART_UCSRB_TXEN))) return;
	while( !(UART_UCSRA & (1 << UART_UCSRA_UDRE)) );
	#if UART_DATA_SIZE == 7
	UART_UCSRB &= (1 << UART_UCSRB_TXB8);
	if(copyu8_Data & 0x0100)
		UART_UCSRB |= (1 << UART_UCSRB_TXB8);
	#endif
	UART_UDR = copyu8_Data;
}

void UART_SendStringSync(const u8* data)
{
	for(u8 i = 0; data[i] != '\0'; i++)
	{
		UART_SendByteSync(data[i]);
	}
}

u8 UART_ReceiveByteSync()
{
	if(! (UART_UCSRB & (1 << UART_UCSRB_RXEN))) return -1;
	while( !(UART_UCSRA & (1 << UART_UCSRA_RXC)) );
	#if UART_DATA_SIZE == 7
	u8 a = UART_UCSRA, b = UART_UCSRB, c = UART_UCSRC;
	u16 data = ((b & (1 << UART_UCSRB_RXB8) >> UART_UCSRB_RXB8 ) << 8) | UART_UDR;
	return data;
	#else
	return UART_UDR;
	#endif
}

void UART_ReceiveStringSync(u8* str, u8 copyu8_length)
{
	u8 counter = 0;
	while(counter < (copyu8_length-1))
	{
		str[counter] = UART_ReceiveByteSync();
		counter ++;
	}
	str[counter] = 0;
}

void UART_ReceiveBufferSync(u8* buff, u8 copyu8_length)
{
	u8 counter = 0;
	while(counter < copyu8_length)
	{
		buff[counter] = UART_ReceiveByteSync();
		counter ++;
	}
}


//void UART_StartReceiveAsync(u8* buff, u8 copyu8_length)
void UART_RXSetCallbackFunc(void (*cb)(u8))
{
	RXC_cb = cb;
}

void UART_TXIntEnable()
{
	
}

void UART_RXIntEnable()
{
	UART_UCSRB |= (1 << UART_UCSRB_RXCIE);
}

// RXC vect
void __vector_13 (void) __attribute__((signal));
void __vector_13 (void)
{
	#if UART_DATA_SIZE == 7
	u16 data = ((UART_UCSRB & (1 << UART_UCSRB_RXB8) >> UART_UCSRB_RXB8 ) << 8) | UART_UDR;
	#else
	u8 data = UART_UDR;
	#endif
	(*(volatile u8*)0x33) = data;
	if(RXC_cb != 0)
		(*RXC_cb)(data);
}

// UDRE vect
void __vector_14 (void) __attribute__((signal));
void __vector_14 (void)
{
	
}

// TXC vect
void __vector_15 (void) __attribute__((signal));
void __vector_15 (void)
{
	
}