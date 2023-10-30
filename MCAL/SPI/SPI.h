/*
 * SPI.h
 *
 * Created: 30/10/2023 12:10:06 PM
 *  Author: Kirollos
 */ 


#ifndef SPI_H_
#define SPI_H_

#include "../../STD_TYPES.h"

enum SPI_CLOCK_RATE
{
	SPI_DIV_1X_4,
	SPI_DIV_1X_16,
	SPI_DIV_1X_64,
	SPI_DIV_1X_128,
	SPI_DIV_2X_2,
	SPI_DIV_2X_8,
	SPI_DIV_2X_32,
	SPI_DIV_2X_64,
};

enum SPI_CLOCK_PHASE
{
	SPI_LSAMPLE_TSETUP, // Lead Sample, Trail Setup
	SPI_LSETUP_TSAMPLE  // Lead Setup,  Trail Sample
};

enum SPI_CLOCK_POL
{
	SPI_LRISING_TFALLING,
	SPI_LFALLING_TRISING
};

enum SPI_MODE
{
	SPI_MODE_SLAVE,
	SPI_MODE_MASTER
};

enum SPI_DATA_ORDER
{
	SPI_MSB,
	SPI_LSB
};

enum SPI_EN
{
	SPI_DISABLE,
	SPI_ENABLE
};

enum SPI_INT_EN
{
	SPI_INT_DISABLE,
	SPI_INT_ENABLE
	};

typedef struct
{
	enum SPI_CLOCK_RATE clk_rate;
	enum SPI_CLOCK_PHASE clk_phase;
	enum SPI_CLOCK_POL clk_pol;
	enum SPI_MODE mode;
	enum SPI_DATA_ORDER dataorder;
	enum SPI_EN enable;
	enum SPI_INT_EN int_enable;
} SPI_parameters;


void SPI_init(SPI_parameters* parameters);
void SPI_transfer(u8 copy_u8ch, u8* pch);
void SPI_send(u8 copy_u8ch);
void SPI_AsyncSetCallback(void (*spicb)(u8));


#endif /* SPI_H_ */