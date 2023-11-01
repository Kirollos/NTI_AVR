/*
 * EEPROM.h
 *
 * Created: 31/10/2023 3:05:11 PM
 *  Author: Kirollos
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "../../STD_TYPES.h"

void EEPROM_init();
u8 EEPROM_writeByte(u16 copy_u16addr, u8 copy_u8data);
u8 EEPROM_readByte(u16 copy_u16addr, u8* ptrdata);

#endif /* EEPROM_H_ */