/*
 * UART.h
 *
 * Created: 29/10/2023 9:48:16 PM
 *  Author: Kirollos
 */ 


#ifndef UART_H_
#define UART_H_

#include "../../STD_TYPES.h"

void UART_Init();
void UART_TXEnable();
void UART_TXDisable();
void UART_RXEnable();
void UART_RXDisable();
void UART_TXIntEnable();
void UART_RXIntEnable();
void UART_SendByteSync(u8 copyu8_Data);
void UART_SendStringSync(const u8* data);
u8 UART_ReceiveByteSync();
void UART_ReceiveStringSync(u8* buff, u8 copyu8_length);

//void UART_StartReceiveAsync(u8* buff, u8 copyu8_length);
void UART_RXSetCallbackFunc(void (*cb)(u8));



#endif /* UART_H_ */