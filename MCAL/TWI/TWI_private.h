/*
 * TWI_private.h
 *
 * Created: 31/10/2023 11:42:16 AM
 *  Author: Kirollos
 */ 


#ifndef TWI_PRIVATE_H_
#define TWI_PRIVATE_H_

#define TWI_TWBR	*((volatile u8*) 0x20)
#define TWI_TWCR	*((volatile u8*) 0x56)
#define TWI_TWSR	*((volatile u8*) 0x21)
#define TWI_TWDR	*((volatile u8*) 0x23)
#define TWI_TWAR	*((volatile u8*) 0x22)


#define TWI_TWBR_VAL(SCL_freq) ((( F_CPU/(SCL_freq) ) - 16) / 2)

#define TWI_TWBR_100K TWI_TWBR_VAL(100000UL)
#define TWI_TWBR_400K TWI_TWBR_VAL(400000UL)


#define TWI_TWCR_TWIE		0
#define TWI_TWCR_TWEN		2
#define TWI_TWCR_TWWC		3
#define TWI_TWCR_TWSTO		4
#define TWI_TWCR_TWSTA		5
#define TWI_TWCR_TWEA		6
#define TWI_TWCR_TWINT		7

#define TWI_TWSR_TWPS0		0
#define TWI_TWSR_TWPS1		1
#define TWI_TWSR_TWSTS_MASK	(0xF8)


#endif /* TWI_PRIVATE_H_ */