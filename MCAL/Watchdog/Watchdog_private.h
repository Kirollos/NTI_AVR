/*
 * Watchdog_private.h
 *
 * Created: 05/11/2023 10:05:37 AM
 *  Author: Kirollos
 */ 


#ifndef WATCHDOG_PRIVATE_H_
#define WATCHDOG_PRIVATE_H_


#define WDT_WDTCR		*((volatile u8*) 0x41)

#define WDT_PRESCALE_MASK	0x07

#define WDT_WDTCR_WDTOE		4
#define WDT_WDTCR_WDE		3



#endif /* WATCHDOG_PRIVATE_H_ */