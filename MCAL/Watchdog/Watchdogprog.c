/*
 * Watchdogprog.c
 *
 * Created: 05/11/2023 10:05:13 AM
 *  Author: Kirollos
 */ 

#include "Watchdog_private.h"
#include "Watchdog.h"

void WDT_Enable(enum WDT_PRESCALE timeout)
{
	WDT_WDTCR |= (1 << WDT_WDTCR_WDE);
	WDT_sleep(timeout);
}

void WDT_Disable(void)
{
	WDT_WDTCR |= (1<<WDT_WDTCR_WDTOE) | (1<<WDT_WDTCR_WDE);
	WDT_WDTCR = 0x00;
}

void WDT_sleep(enum WDT_PRESCALE timeout)
{
	WDT_WDTCR &= ~(WDT_PRESCALE_MASK);
	WDT_WDTCR |= (timeout&WDT_PRESCALE_MASK);
}

void WDT_refresh(void)
{
	asm("WDR");
}