/*
 * Watchdog.h
 *
 * Created: 05/11/2023 10:05:23 AM
 *  Author: Kirollos
 */ 


#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include "../../STD_TYPES.h"

enum WDT_PRESCALE
{
	WDT_PRESCALE_16K,
	WDT_PRESCALE_32K,
	WDT_PRESCALE_64K,
	WDT_PRESCALE_128K,
	WDT_PRESCALE_256K,
	WDT_PRESCALE_512K,
	WDT_PRESCALE_1024K,
	WDT_PRESCALE_2048K
};

void WDT_Enable(enum WDT_PRESCALE timeout);
void WDT_Disable(void);
void WDT_sleep(enum WDT_PRESCALE timeout);
void WDT_refresh(void);


#endif /* WATCHDOG_H_ */