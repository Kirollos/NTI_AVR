/*
 * Fingerprintprog.c
 *
 * Created: 03/11/2023 10:13:21 PM
 *  Author: Kirollos
 */ 

#include "Fingerprint_config.h"
#include "Fingerprint_private.h"
#include "Fingerprint.h"
#include "../../MCAL/UART/UART.h"

#define F_CPU		16000000
#include <util/delay.h>

enum PacketID
{
	PacketID_CMD = 0x01,
	PacketID_DATA = 0x02,
	PacketID_ACK = 0x07,
	PacketID_END = 0x08
};

typedef struct  
{
	u16 u16_Header;
	u32 u32_Address;
	enum PacketID u8_PID;
	u16 u16_dataLen;
	u8* pu8_data;
	u16 u16_checksum;
} FP_Frame;

static const u16 Frame_Header = 0xEF01;

#define MAX_BUFF_SIZE	64

volatile u8 buff[MAX_BUFF_SIZE];
volatile u8 buff_idx = 0;
u8 buff_ptr = 0;
volatile u8 buff_len = 0;

//static void sendFrame(u32 copyu32_addr, enum PacketID copyu8_pid, u16 copyu16_datalen, u8* pdata)
static void sendFrame(FP_Frame* outgoing_frame)
{
	u16 chksum = outgoing_frame->u8_PID + outgoing_frame->u16_dataLen;
	// Frame header
	UART_SendByteSync((Frame_Header & 0xFF00) >> 8);	// Higher byte
	UART_SendByteSync((Frame_Header & 0x00FF));			// Lower byte
	// Address bytes
	UART_SendByteSync((outgoing_frame->u32_Address & 0xFF000000) >> 24);	// Most significant byte
	UART_SendByteSync((outgoing_frame->u32_Address & 0x00FF0000) >> 16);
	UART_SendByteSync((outgoing_frame->u32_Address & 0x0000FF00) >> 8);
	UART_SendByteSync((outgoing_frame->u32_Address & 0x000000FF));			// Least significant byte
	// Packet ID
	UART_SendByteSync(outgoing_frame->u8_PID);						// 1 Byte
	// Packet Length
	UART_SendByteSync((outgoing_frame->u16_dataLen & 0xFF00) >> 8);	// Higher byte
	UART_SendByteSync((outgoing_frame->u16_dataLen & 0x00FF));		// Lower byte
	// Packet data
	for(u16 i = 0; i < outgoing_frame->u16_dataLen-2; i++)
	{
		UART_SendByteSync(outgoing_frame->pu8_data[i]);
		chksum += outgoing_frame->pu8_data[i];
	}
	outgoing_frame->u16_checksum = chksum;
	// Checksum
	UART_SendByteSync((chksum & 0xFF00) >> 8);			// Higher byte
	UART_SendByteSync((chksum & 0x00FF));				// Lower byte
}

static u8 receiveFrame(u8* buff, FP_Frame* pframe) // 1 - checksum success
{
	while(buff_len < 9);
	pframe->u16_Header = (buff[0] << 8) | buff[1];
	pframe->u32_Address = (buff[2] << 24) | (buff[3] << 16) | (buff[4] << 8) | buff[5];
	pframe->u8_PID = buff[6];
	pframe->u16_dataLen = (buff[7] << 8) | buff[8];
	u16 chksum = pframe->u8_PID + pframe->u16_dataLen;
	u8 i;
	while(buff_len < 9+pframe->u16_dataLen);
	for(i = 0; i < pframe->u16_dataLen-2; i++)
	{
		pframe->pu8_data[i] = buff[9+i];
		chksum += buff[9+i];
	}
	pframe->u16_checksum = buff[9+(i++)];
	pframe->u16_checksum <<= 8;
	pframe->u16_checksum |= buff[9+i];
	buff_len = 0;
	return chksum == pframe->u16_checksum;
}

u8* getnextByte(void)
{
	u8* pch = buff+buff_ptr;
	buff_ptr = (buff_ptr+1)%MAX_BUFF_SIZE;
	return pch;
}

u8 scanFrameHeader(void)
{
	for(u8 i = 0; i != MAX_BUFF_SIZE; i++)
	{
		if(buff[i] == (Frame_Header>>8) && buff[(i+1)%MAX_BUFF_SIZE])
		{
			buff_ptr = i;
			return 1;
		}
	}
	return 0;
}

void onReceiveByte(u8 ch)
{
//	if((buff_idx < buff_len)) {
		buff[buff_idx] = ch;
		buff_idx = (buff_idx+1)%MAX_BUFF_SIZE;
		buff_len ++;
//	}
}

void H_FingerPS_init()
{
	_delay_ms(200);
	UART_Init(FP_BAUDRATE);
	UART_TXEnable();
	UART_RXEnable();
	UART_RXSetCallbackFunc(onReceiveByte);
	UART_RXIntEnable();
}

u8 H_FingerPS_handShake()
{
	u8 data[] = {0x40};
	FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame ggframe;
	sendFrame(&hsframe);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &ggframe);
	if(!checksum)
	{
		// FAIL
		return 1;
	}
	return ggframe.pu8_data[0];
}

u8 H_FingerPS_AuraControl()
{
	u8 data[] = {0x35, 0x02, 0x03, 0x01, 0x10};
	FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0007, data};
	FP_Frame ggframe;
	sendFrame(&hsframe);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &ggframe);
	if(!checksum)
	{
		// FAIL
		return 1;
	}
	return ggframe.pu8_data[0];
}

u8 H_FingerPS_genImg()
{
	u8 data[] = {0x01};
	FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame ggframe;
	sendFrame(&hsframe);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &ggframe);
	if(!checksum)
	{
		// FAIL
		return 1;
	}
	return ggframe.pu8_data[0];
}

u8 H_FingerPS_convertImg2CharFile(u8 bufferID)
{
	u8 data[] = {0x02, bufferID};
	FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0004, data};
	FP_Frame ggframe;
	sendFrame(&hsframe);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &ggframe);
	if(!checksum)
	{
		// FAIL
		return 1;
	}
	return ggframe.pu8_data[0];
}

u8 H_FingerPS_genTemplate()
{
	u8 data[] = {0x05};
	FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame ggframe;
	sendFrame(&hsframe);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &ggframe);
	if(!checksum)
	{
		// FAIL
		return 1;
	}
	return ggframe.pu8_data[0];
}

u8 H_FingerPS_strTemplate()
{
	u8 data[] = {0x06};
	FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame ggframe;
	sendFrame(&hsframe);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &ggframe);
	if(!checksum)
	{
		// FAIL
		return 69;
	}
	return ggframe.pu8_data[0];
}

u8 H_FingerPS_searchFinger()
{
	
}

u8 H_FingerPS_emptyLibrary()
{
	
}

u8 H_FingerPS_deleteFinger()
{
	
}

u8 FingerPS_match()
{
	
}