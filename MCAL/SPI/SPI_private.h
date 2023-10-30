/*
 * SPI_private.h
 *
 * Created: 30/10/2023 12:10:21 PM
 *  Author: Kirollos
 */ 


#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

#define SPI_SPCR		*((volatile u8*) 0x2d)
#define SPI_SPSR		*((volatile u8*) 0x2e)
#define SPI_SPDR		*((volatile u8*) 0x2f)

#define SPI_SPCR_SPR0		0
#define SPI_SPCR_SPR1		1
#define SPI_SPCR_CPHA		2
#define SPI_SPCR_CPOL		3
#define SPI_SPCR_MSTR		4
#define SPI_SPCR_DORD		5
#define SPI_SPCR_SPE		6
#define SPI_SPCR_SPIE		7

#define SPI_SPSR_SPI2X		0
#define SPI_SPSR_WCOL		6
#define SPI_SPSR_SPIF		7



#endif /* SPI_PRIVATE_H_ */