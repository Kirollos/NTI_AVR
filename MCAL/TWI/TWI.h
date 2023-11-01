/*
 * TWI.h
 *
 * Created: 31/10/2023 11:42:28 AM
 *  Author: Kirollos
 */ 


#ifndef TWI_H_
#define TWI_H_

#include "../../STD_TYPES.h"

#define TWI_RD			1
#define TWI_WR			0

enum TWI_PS
{
	TWI_PS_1,
	TWI_PS_4,
	TWI_PS_16,
	TWI_PS_64
};

enum TWI_SPEED_MODE
{
	TWI_SPEED_MODE_100K,
	TWI_SPEED_MODE_400K
};

typedef enum
{
	MT_STATUS_CODE_START = 0x08,
	MT_STATUS_CODE_REP_START = 0x10,
	MT_STATUS_CODE_SLAW_TX_ACK = 0x18,
	MT_STATUS_CODE_SLAW_TX_NACK = 0x20,
	MT_STATUS_CODE_DATA_TX_ACK = 0x28,
	MT_STATUS_CODE_DATA_TX_NACK = 0x30,
	MT_STATUS_CODE_ARB_LOST = 0x38
} TWI_MT_STATUS_CODE;

typedef enum
{
	MR_STATUS_CODE_START = 0x08,
	MR_STATUS_CODE_REP_START = 0x10,
	MR_STATUS_CODE_SLAR_TX_ACK = 0x40,
	MR_STATUS_CODE_SLAR_TX_NACK = 0x48,
	MR_STATUS_CODE_DATA_RX_ACK = 0x50,
	MR_STATUS_CODE_DATA_RX_NACK = 0x58
} TWI_MR_STATUS_CODE;

void TWI_init(enum TWI_PS Prescaler, enum TWI_SPEED_MODE SpeedMode);
void TWI_sendByte(u8 copy_u8Data);
void TWI_sendStart(void);
void TWI_sendStop(void);
void TWI_receiveByte_ACK(u8* pData);
void TWI_receiveByte_NoACK(u8* pData);
void TWI_getMTStatus(TWI_MT_STATUS_CODE* pStatus);
void TWI_getMRStatus(TWI_MR_STATUS_CODE* pStatus);


#endif /* TWI_H_ */