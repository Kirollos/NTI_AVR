/*
 * RTC_private.h
 *
 * Created: 03/11/2023 11:13:48 AM
 *  Author: Kirollos
 */ 


#ifndef RTC_PRIVATE_H_
#define RTC_PRIVATE_H_

#define RTC_SLA		RTC_BASE_ADDR

#define RTC_REG_SEC		0x00
#define RTC_REG_MIN		0x01
#define RTC_REG_HRS		0x02
#define RTC_REG_DAY		0x03
#define RTC_REG_DATE	0x04
#define RTC_REG_MONTH	0x05
#define RTC_REG_YEAR	0x06
#define RTC_REG_CONTROL	0x07
#define RTC_RAM_START	0x08
#define RTC_RAM_END		0x3F

#define RTC_CH_ACTIVE	0
#define RTC_CH_INACTIVE	1
#define RTC_12H_ACTIVE	1
#define RTC_24H_ACTIVE	0


#endif /* RTC_PRIVATE_H_ */