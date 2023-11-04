/*
 * Fingerprint.h
 *
 * Created: 03/11/2023 10:13:28 PM
 *  Author: Kirollos
 */ 


#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_

#include "../../STD_TYPES.h"

void H_FingerPS_init();
u8 H_FingerPS_handShake();
u8 H_FingerPS_AuraControl();
u8 H_FingerPS_genImg();
u8 H_FingerPS_convertImg2CharFile(u8 bufferID);
u8 H_FingerPS_genTemplate();
u8 H_FingerPS_strTemplate();
u8 H_FingerPS_searchFinger();
u8 H_FingerPS_emptyLibrary();
u8 H_FingerPS_deleteFinger();
u8 FingerPS_match();



#endif /* FINGERPRINT_H_ */