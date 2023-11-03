/*
 * RTC.c
 *
 * Created: 03/11/2023 11:13:02 AM
 *  Author: Kirollos
 */ 

#include "RTC_config.h"
#include "RTC_private.h"
#include "RTC.h"
#include "../../MCAL/TWI/TWI.h"

static void IntToBCD(u8* pint, u8* pbcd)
{
	u8 units = (*pint) % 10;
	u8 tens = (*pint) / 10;
	*pbcd = ((tens&0xF) << 4) | (units&0xF);
}

static void BCDToInt(u8* pbcd, u8 *pint)
{
	u8 units = ((*pbcd) & 0xF);
	u8 tens = (((*pbcd) & 0xF0) >> 4);
	*pint = tens*10 + units;
}

void H_RTC_init()
{
	TWI_init(TWI_PS_1, TWI_SPEED_MODE_100K);
}

void H_RTC_setTime(RTC_Time copyRTC_time)
{
	TWI_MT_STATUS_CODE status;
	TWI_sendStart();
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_START)
	{
		// Error
		return;
	}
	TWI_sendByte(RTC_SLA | TWI_WR);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_SLAW_TX_ACK)
	{
		// FAIL
		return;
	}
	TWI_sendByte(RTC_REG_SEC);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	// Start sending data
	u8 bcd;
	// Seconds
	IntToBCD(&copyRTC_time.second, &bcd);
	// CH << 7 | SECS-10 << 4 | SECS-1 << 0
	TWI_sendByte((0<<7) | (bcd));
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	
	// Minutes
	IntToBCD(&copyRTC_time.minute, &bcd);
	// MINS-10 << 4 | MINS-1 << 0
	TWI_sendByte(bcd);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	
	// Hours
	if(copyRTC_time.hour_12h >= 1 && copyRTC_time.hour_12h <= 12)
	{
		IntToBCD(&copyRTC_time.hour_12h, &bcd);
		// 12H bit << 6 | AM/PM << 5 | HRS-10 << 4 | HRS-1 << 0
		TWI_sendByte((RTC_12H_ACTIVE<<6) | (copyRTC_time.hour_AMPM << 5) | (bcd&0x1F));
	}
	else if(copyRTC_time.hour >= 0 && copyRTC_time.hour <= 24)
	{
		IntToBCD(&copyRTC_time.hour, &bcd);
		// 24H bit << 6 | HRS-10 << 4 | HRS-1 << 0
		TWI_sendByte((RTC_24H_ACTIVE<<6) | (bcd&0x3F));
	}
	else
	{
		// FAIL
		return;
	}
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	// ----
	TWI_sendStop();
}

u8 H_RTC_getTime(RTC_Time* pRTC_time)
{
	u8 ret_bytes[3];
	TWI_MT_STATUS_CODE status_MT;
	TWI_MR_STATUS_CODE status_MR;
	TWI_sendStart();
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_START)
	{
		// Error
		return 1;
	}
	TWI_sendByte(RTC_SLA | TWI_WR);
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_SLAW_TX_ACK)
	{
		// FAIL
		return 2;
	}
	TWI_sendByte(RTC_REG_SEC);
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return 3;
	}
	// Repeat start & switch to read mode:
	TWI_sendStart();
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_REP_START)
	{
		// Error
		return 4 ;
	}
	TWI_sendByte(RTC_SLA | TWI_RD);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_SLAR_TX_ACK)
	{
		// FAIL
		return 5;
	}
	TWI_receiveByte_ACK(ret_bytes);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_ACK)
	{
		return 6;
	}
	TWI_receiveByte_ACK(ret_bytes+1);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_ACK)
	{
		return 7;
	}
	TWI_receiveByte_NoACK(ret_bytes+2);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_NACK)
	{
		return 8;
	}
	TWI_sendStop();
	
	// Seconds byte:
	ret_bytes[0] &= 0x7F;
	BCDToInt(&(ret_bytes[0]), &(pRTC_time->second));
	// Minutes byte:
	ret_bytes[1] &= 0x7F;
	BCDToInt(&(ret_bytes[1]), &(pRTC_time->minute));
	// Hour byte:
	u8 hour_mode = (ret_bytes[2] >> 6) & 0x01;
	if(hour_mode == RTC_12H_ACTIVE) // 12 hr
	{
		u8 hr = ret_bytes[2] & 0x1F;
		BCDToInt(&hr, &(pRTC_time->hour_12h));
		pRTC_time->hour_AMPM = (ret_bytes[2] >> 5) & 0x01;
	}
	else // 24 hr
	{
		u8 hr = ret_bytes[2] &= 0x3F;
		BCDToInt(&hr, &pRTC_time->hour);
	}
	return 0;
}

void H_RTC_setDate(RTC_Date copyRTC_date)
{
	TWI_MT_STATUS_CODE status;
	TWI_sendStart();
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_START)
	{
		// Error
		return;
	}
	TWI_sendByte(RTC_SLA | TWI_WR);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_SLAW_TX_ACK)
	{
		// FAIL
		return;
	}
	TWI_sendByte(RTC_REG_DAY);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	// Start sending data
	u8 bcd;
	// Day
	IntToBCD(&(copyRTC_date.day), &bcd);
	// Day
	TWI_sendByte((bcd & 0x07));
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	
	// Date
	IntToBCD(&copyRTC_date.date, &bcd);
	// DATE-10 << 4 | DATE-1 << 0
	TWI_sendByte(bcd);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	
	// Month
	IntToBCD(&copyRTC_date.month, &bcd);
	// MON-10 << 4 | MON-1 << 0
	TWI_sendByte(bcd);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	
	// Year
	IntToBCD(&copyRTC_date.year, &bcd);
	// YEAR-10 << 4 | YEAR-1 << 0
	TWI_sendByte(bcd);
	TWI_getMTStatus(&status);
	if(status != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return;
	}
	// ----
	TWI_sendStop();
}

u8 H_RTC_getDate(RTC_Date* pRTC_date)
{
	u8 ret_bytes[4];
	TWI_MT_STATUS_CODE status_MT;
	TWI_MR_STATUS_CODE status_MR;
	TWI_sendStart();
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_START)
	{
		// Error
		return 1;
	}
	TWI_sendByte(RTC_SLA | TWI_WR);
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_SLAW_TX_ACK)
	{
		// FAIL
		return 2;
	}
	TWI_sendByte(RTC_REG_DAY);
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_DATA_TX_ACK)
	{
		// FAIL
		return 3;
	}
	// Repeat start & switch to read mode:
	TWI_sendStart();
	TWI_getMTStatus(&status_MT);
	if(status_MT != MT_STATUS_CODE_REP_START)
	{
		// Error
		return 4 ;
	}
	TWI_sendByte(RTC_SLA | TWI_RD);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_SLAR_TX_ACK)
	{
		// FAIL
		return 5;
	}
	TWI_receiveByte_ACK(ret_bytes);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_ACK)
	{
		return 6;
	}
	TWI_receiveByte_ACK(ret_bytes+1);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_ACK)
	{
		return 7;
	}
	TWI_receiveByte_ACK(ret_bytes+2);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_ACK)
	{
		return 8;
	}
	TWI_receiveByte_NoACK(ret_bytes+3);
	TWI_getMRStatus(&status_MR);
	if(status_MR != MR_STATUS_CODE_DATA_RX_NACK)
	{
		return 9;
	}
	TWI_sendStop();
	
	// Day byte:
	ret_bytes[0] &= 0x07;
	BCDToInt(&(ret_bytes[0]), &(pRTC_date->day));
	// Date byte:
	ret_bytes[1] &= 0x3F;
	BCDToInt(&(ret_bytes[1]), &(pRTC_date->date));
	// Month byte:
	ret_bytes[2] &= 0x1F;
	BCDToInt(&(ret_bytes[2]), &(pRTC_date->month));
	// Year byte:
	BCDToInt(&(ret_bytes[3]), &(pRTC_date->year));
	return 0;
}
