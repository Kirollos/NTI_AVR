/*
 * EEPROMprog.c
 *
 * Created: 31/10/2023 3:05:43 PM
 *  Author: Kirollos
 */ 

#include "EEPROM_config.h"
#include "EEPROM_private.h"
#include "EEPROM.h"
#include "../../MCAL/TWI/TWI.h"

void EEPROM_init()
{
	TWI_init(TWI_PS_1, TWI_SPEED_MODE_100K);
}

u8 EEPROM_writeByte(u16 copy_u16addr, u8 copy_u8data)
{
	TWI_MT_STATUS_CODE status;
	TWI_sendStart();
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_START)
	{
		// FAIL
		return 1;
	}
	TWI_sendByte(SLA | A2 | (((copy_u16addr >> 8) & 0x03) << 1) | TWI_WR);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_SLAW_TX_ACK)
	{
		// FAIL
		return 2;
	}
	TWI_sendByte(copy_u16addr & 0xFF);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return 3;
	}
	TWI_sendByte(copy_u8data);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		return 4;
	}
	TWI_sendStop();
	return 0;
}

u8 EEPROM_readByte(u16 copy_u16addr, u8* ptrdata)
{
	TWI_MT_STATUS_CODE status_MT;
	TWI_MR_STATUS_CODE status_MR;
	TWI_sendStart();
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_START)
	{
		// FAIL
		return 1;
	}
	TWI_sendByte(SLA | A2 | (((copy_u16addr >> 8) & 0x03) << 1) | TWI_WR);
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_SLAW_TX_ACK)
	{
		// FAIL
		return 2;
	}
	TWI_sendByte(copy_u16addr & 0xFF);
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return 3;
	}
	TWI_sendStart();
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_REP_START)
	{
		// FAIL
		return 4;
	}
	TWI_sendByte(SLA | A2 | (((copy_u16addr >> 8) & 0x03) << 1) | TWI_RD);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_SLAR_TX_ACK)
	{
		// FAIL
		return 5;
	}
	TWI_receiveByte_NoACK(ptrdata);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_NACK)
	{
		return 4;
	}
	TWI_sendStop();
	return 0;
}