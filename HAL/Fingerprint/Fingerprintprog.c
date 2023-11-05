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

#include <string.h>

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

#define MAX_BUFF_SIZE	1024

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
#include "../LCD/LCD.h"

static u8 receiveFrame(u8* pbuff, FP_Frame* pframe) // 1 - checksum success
{
	while(buff_len < 9);
	pframe->u16_Header = (pbuff[0] << 8) | pbuff[1];
	pframe->u32_Address = (pbuff[2] << 24) | (pbuff[3] << 16) | (pbuff[4] << 8) | pbuff[5];
	pframe->u8_PID = pbuff[6];
	pframe->u16_dataLen = (pbuff[7] << 8) | pbuff[8];
	u16 chksum = pframe->u8_PID + pframe->u16_dataLen;
	u8 i;
	while(buff_len < 9+pframe->u16_dataLen);
	for(i = 0; i < pframe->u16_dataLen-2; i++)
	{
		pframe->pu8_data[i] = pbuff[9+i];
		chksum += pbuff[9+i];
	}
	pframe->u16_checksum = pbuff[9+(i++)];
	pframe->u16_checksum <<= 8;
	pframe->u16_checksum |= pbuff[9+i];
	// Clean the buffer
	buff_len = 0;
	//for(u8 x = 0; x <= 9+i; x++)
	for(u8 x = 0; x < 2; x++)
	{
		pbuff[x] = 0;
	}
	buff_ptr=buff_idx=0;
	return chksum == pframe->u16_checksum;
}

static u8* getnextByte(void)
{
	u8* pch = buff+buff_ptr;
	//buff_ptr = (buff_ptr+1)%MAX_BUFF_SIZE;
	return pch;
}

static u8 scanFrameHeader(void)
{
	for(u8 i = 0; i != MAX_BUFF_SIZE; i++)
	{
		if(buff[i] == (Frame_Header>>8) && buff[(i+1)%MAX_BUFF_SIZE])
		{
			//buff_ptr = i;
			return 1;
		}
	}
	return 0;
}

static void constructFrame(enum PacketID pid, const u8* data, const u8 data_len, const FP_Frame* frame)
{
	FP_Frame tmpf = {Frame_Header, 0xFFFFFFFF, pid, data_len + 2, (u8*) data};
	memcpy((u8*) frame, &tmpf, sizeof(FP_Frame));
}

static void onReceiveByte(u8 ch)
{
	buff[buff_idx] = ch;
	buff_idx = (buff_idx+1)%MAX_BUFF_SIZE;
	buff_len ++;
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

FP_STATUS H_FingerPS_handShake()
{
	u8 data[] = {FP_CMD_HandShake};
	//fp_frame hsframe = {frame_header, 0xffffffff, packetid_cmd, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_AuraControl(enum FP_Aura_Control copyu8_CtrlCode, u8 copyu8_Speed, enum FP_Aura_Color copyu8_ColorIndex, u8 copyu8_Timing)
{
	u8 data[] = {FP_CMD_AuraControl, copyu8_CtrlCode, copyu8_Speed, copyu8_ColorIndex, copyu8_Timing};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0007, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}
//GenImg
FP_STATUS H_FingerPS_genImg()
{
	u8 data[] = {FP_CMD_GenImg};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}
//Img2Tz
FP_STATUS H_FingerPS_convertImg2CharFile(u8 copyu8_bufferID)
{
	u8 data[] = {FP_CMD_Img2Tz, copyu8_bufferID};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0004, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_genTemplate()
{
	u8 data[] = {FP_CMD_RegModel};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_strTemplate(u8 copyu8_bufferID, u16 copyu16_pageID)
{
	u8 data[] = {FP_CMD_Store, copyu8_bufferID, (copyu16_pageID>>8), (copyu16_pageID&0xFF)};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	H_LCD_void_sendString("a");
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	H_LCD_void_sendString("b");
	u8* p = getnextByte();
	H_LCD_void_sendString("c");
	checksum = receiveFrame(p, &rx_frame);
	H_LCD_void_sendString("d");
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_searchFinger(u8 copyu8_bufferID, u16 copyu16_startPage, u16 copyu16_pageNum, u16* pPageID, u16* pMatchScore)
{
	u8 data[] = {FP_CMD_Search, copyu8_bufferID, (copyu16_startPage>>8), (copyu16_startPage&0xFF), (copyu16_pageNum>>8), (copyu16_pageNum&0xFF)};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	if(rx_frame.pu8_data[0] == FP_STATUS_OK)
	{
		*pPageID = (rx_frame.pu8_data[1] << 8) | (rx_frame.pu8_data[2]);
		*pMatchScore = (rx_frame.pu8_data[3] << 8) | (rx_frame.pu8_data[4]);
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_emptyLibrary()
{
	u8 data[] = {FP_CMD_Empty};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_deleteFinger(u16 copyu16_pageID, u16 copyu16_nTemplates)
{
	u8 data[] = {FP_CMD_DeleteChar, (copyu16_pageID>>8), (copyu16_pageID&0xFF), (copyu16_nTemplates>>8), (copyu16_nTemplates&0xFF)};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS H_FingerPS_LoadCharFile(u8 copyu8_bufferID, u16 copyu16_pageID)
{
	u8 data[] = {FP_CMD_LoadChar, copyu8_bufferID, (copyu16_pageID>>8), (copyu16_pageID&0xFF)};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	return rx_frame.pu8_data[0];
}

FP_STATUS FingerPS_match(u16* pMatchScore)
{
	u8 data[] = {FP_CMD_Match};
	//FP_Frame hsframe = {Frame_Header, 0xFFFFFFFF, PacketID_CMD, 0x0003, data};
	FP_Frame tx_frame, rx_frame;
	constructFrame(PacketID_CMD, data, sizeof(data), &tx_frame);
	sendFrame(&tx_frame);
	_delay_ms(100); // Wait for response to build up
	u8 checksum = -1;
	while(scanFrameHeader() == 0);
	u8* p = getnextByte();
	checksum = receiveFrame(p, &rx_frame);
	if(!checksum)
	{
		// FAIL
		return FP_STATUS_CHECKSUM_FAIL;
	}
	if(rx_frame.pu8_data[0] == FP_STATUS_OK)
	{
		*pMatchScore = (rx_frame.pu8_data[1] << 8) | (rx_frame.pu8_data[2]);
	}
	return rx_frame.pu8_data[0];
}