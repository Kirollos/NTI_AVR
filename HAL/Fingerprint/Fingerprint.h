/*
 * Fingerprint.h
 *
 * Created: 03/11/2023 10:13:28 PM
 *  Author: Kirollos
 */ 


#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_

#include "../../STD_TYPES.h"

void H_FingerPS_handShake();
void H_FingerPS_genImg();
void H_FingerPS_convertImg2CharFile();
void H_FingerPS_genTemplate();
void H_FingerPS_strTemplate();
void H_FingerPS_searchFinger();
void H_FingerPS_emptyLibrary();
void H_FingerPS_deleteFinger();




#endif /* FINGERPRINT_H_ */