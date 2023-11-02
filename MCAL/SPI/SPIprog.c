/*
 * SPIprog.c
 *
 * Created: 30/10/2023 12:09:58 PM
 *  Author: Kirollos
 */ 

#include "SPI_config.h"
#include "SPI_private.h"
#include "SPI.h"

static void (*SPI_cb)(u8);

void SPI_init(SPI_parameters* parameters)
{
	u8 c=0, s=0;
	c |= ((parameters->clk_rate & 0b11) << SPI_SPCR_SPR0);
	c |= (parameters->clk_phase << SPI_SPCR_CPHA);
	c |= (parameters->clk_pol << SPI_SPCR_CPOL);
	c |= (parameters->mode << SPI_SPCR_MSTR);
	c |= (parameters->dataorder << SPI_SPCR_DORD);
	
	s |= (((parameters->clk_rate >> 2) & 1) << SPI_SPSR_SPI2X);
	
	c |= (parameters->enable << SPI_SPCR_SPE);
	c |= (parameters->int_enable << SPI_SPCR_SPIE);
	
	SPI_SPCR = c;
	SPI_SPSR = s;
}

void SPI_transfer(u8 copy_u8ch, u8* pch)
{
	SPI_SPDR = copy_u8ch;
	while(!((SPI_SPSR >> SPI_SPSR_SPIF) & 1));
	*pch = SPI_SPDR;
}

void SPI_AsyncSend(u8 copy_u8ch)
{
	SPI_SPDR = copy_u8ch;
}

void SPI_AsyncSetCallback(void (*spicb)(u8))
{
	SPI_cb = spicb;
}

// SPI complete
void __vector_12 (void) __attribute__((signal));
void __vector_12 (void)
{
	if(SPI_cb != 0) {
		u8 val = SPI_SPDR;
		(*SPI_cb)(val);
	}
}