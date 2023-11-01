/*
 * TWIprog.c
 *
 * Created: 31/10/2023 11:41:43 AM
 *  Author: Kirollos
 */ 

#include "TWI_config.h"
#include "TWI_private.h"
#include "TWI.h"

void TWI_init(enum TWI_PS Prescaler, enum TWI_SPEED_MODE SpeedMode)
{
	TWI_TWBR = (u8) (SpeedMode==TWI_SPEED_MODE_100K ? TWI_TWBR_100K : TWI_TWBR_400K);
	TWI_TWSR = (u8) Prescaler;
};

void TWI_sendByte(u8 copy_u8Data)
{
	TWI_TWDR = copy_u8Data;
	TWI_TWCR = (1 << TWI_TWCR_TWINT) | (1 << TWI_TWCR_TWEN);
	while(!(TWI_TWCR & (1 << TWI_TWCR_TWINT)));
}
void TWI_sendStart(void)
{
	TWI_TWCR = (1 << TWI_TWCR_TWINT) | (1 << TWI_TWCR_TWSTA) | (1 << TWI_TWCR_TWEN);
	while(!(TWI_TWCR & (1 << TWI_TWCR_TWINT)));
}
void TWI_sendStop(void)
{
	TWI_TWCR = (1 << TWI_TWCR_TWINT) | (1 << TWI_TWCR_TWEN) | (1 << TWI_TWCR_TWSTO);
}
void TWI_receiveByte_ACK(u8* pData)
{
	TWI_TWCR = (1 << TWI_TWCR_TWINT) | (1 << TWI_TWCR_TWEN) | (1 << TWI_TWCR_TWEA);
	while(!(TWI_TWCR & (1 << TWI_TWCR_TWINT)));
	*pData = TWI_TWDR;
}
void TWI_receiveByte_NoACK(u8* pData)
{
	TWI_TWCR = (1 << TWI_TWCR_TWINT) | (1 << TWI_TWCR_TWEN) | (0 << TWI_TWCR_TWEA);
	while(!(TWI_TWCR & (1 << TWI_TWCR_TWINT)));
	*pData = TWI_TWDR;
}
void TWI_getMTStatus(TWI_MT_STATUS_CODE* pStatus)
{
	*pStatus = TWI_TWSR & TWI_TWSR_TWSTS_MASK;
}
void TWI_getMRStatus(TWI_MR_STATUS_CODE* pStatus)
{
	*pStatus = TWI_TWSR & TWI_TWSR_TWSTS_MASK;
}