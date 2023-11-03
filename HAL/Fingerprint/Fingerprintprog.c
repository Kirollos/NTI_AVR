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

void H_FingerPS_handShake()
{
	_delay_ms(200);
	UART_Init(FP_BAUDRATE);
}

void H_FingerPS_genImg()
{
	
}

void H_FingerPS_convertImg2CharFile()
{
	
}

void H_FingerPS_genTemplate()
{
	
}

void H_FingerPS_strTemplate()
{
	
}

void H_FingerPS_searchFinger()
{
	
}

void H_FingerPS_emptyLibrary()
{
	
}

void H_FingerPS_deleteFinger()
{
	
}
