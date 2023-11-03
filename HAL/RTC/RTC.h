/*
 * RTC.h
 *
 * Created: 03/11/2023 11:13:59 AM
 *  Author: Kirollos
 */ 


#ifndef RTC_H_
#define RTC_H_

#include "../../STD_TYPES.h"

enum RTC_Time_AMPM
{
	HOUR_AM,
	HOUR_PM
};

enum RTC_Day
{
	RTC_Day_Sunday = 1,
	RTC_Day_Monday,
	RTC_Day_Tuesday,
	RTC_Day_Wednesday,
	RTC_Day_Thursday,
	RTC_Day_Friday,
	RTC_Day_Saturday,
};

typedef struct  
{
	u8 hour_12h;	/* Use this to set RTC to 12 hours mode */
	enum RTC_Time_AMPM hour_AMPM;
					/* AM = 0		,		PM = 1			*/
	u8 hour;		/* Use this to set RTC to 24 hours mode */
	u8 minute;
	u8 second;
} RTC_Time;

typedef struct
{
	enum RTC_Day day;
	u8 date;
	u8 month;
	u8 year;
} RTC_Date;


void H_RTC_init();
void H_RTC_setTime(RTC_Time copyRTC_time);
u8 H_RTC_getTime(RTC_Time* pRTC_time);
void H_RTC_setDate(RTC_Date copyRTC_date);
u8 H_RTC_getDate(RTC_Date* pRTC_date);


#endif /* RTC_H_ */