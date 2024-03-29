/*
 * Fingerprint_private.h
 *
 * Created: 03/11/2023 10:13:36 PM
 *  Author: Kirollos
 */ 


#ifndef FINGERPRINT_PRIVATE_H_
#define FINGERPRINT_PRIVATE_H_

#include "../../STD_TYPES.h"

#define FP_BAUDRATE		57600
//#define FP_BAUDRATE		9600

// Instruction codes
#define FP_CMD_GenImg			0x01
#define FP_CMD_Img2Tz			0x02
#define FP_CMD_Match			0x03
#define FP_CMD_Search			0x04
#define FP_CMD_RegModel			0x05
#define FP_CMD_Store			0x06
#define FP_CMD_LoadChar			0x07
#define FP_CMD_UpChar			0x08
#define FP_CMD_DownChr			0x09
#define FP_CMD_UpImage			0x0A
#define FP_CMD_DownImage		0x0B
#define FP_CMD_DeleteChar		0x0C
#define FP_CMD_Empty			0x0D
#define FP_CMD_SetSysPara		0x0E
#define FP_CMD_ReadSysPara		0x0F
#define FP_CMD_SetPwd			0x12
#define FP_CMD_VfyPwd			0x13
#define FP_CMD_GetRandomCode	0x14
#define FP_CMD_SetAddr			0x15
#define FP_CMD_ReadInfPage		0x16
#define FP_CMD_Control			0x17
#define FP_CMD_WriteNotepad		0x18
#define FP_CMD_ReadNotepad		0x19
#define FP_CMD_TemplateNum		0x1D
#define FP_CMD_ReadIndexTable	0x1F
#define FP_CMD_GetImageEx		0x28
#define FP_CMD_Cancel			0x30
#define FP_CMD_AuraControl		0x35
#define FP_CMD_CheckSensor		0x36
#define FP_CMD_GetAlgVer		0x39
#define FP_CMD_GetFwVer			0x3A
#define FP_CMD_ReadProdInfo		0x3C
#define FP_CMD_SoftRst			0x3D
#define FP_CMD_HandShake		0x40


enum PacketID
{
	PacketID_CMD = 0x01,
	PacketID_DATA = 0x02,
	PacketID_ACK = 0x07,
	PacketID_END = 0x08
};

#define FDATA_MAXLEN	30

typedef struct
{
	u16 u16_Header;
	u32 u32_Address;
	enum PacketID u8_PID;
	u16 u16_dataLen;
	u8 pu8_data[FDATA_MAXLEN];
	u16 u16_checksum;
} FP_Frame;

static const u16 Frame_Header = 0xEF01;

#define MAX_BUFF_SIZE	32


#endif /* FINGERPRINT_PRIVATE_H_ */