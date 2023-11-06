/*
 * Fingerprint.h
 *
 * Created: 03/11/2023 10:13:28 PM
 *  Author: Kirollos
 */ 


#ifndef FINGERPRINT_H_
#define FINGERPRINT_H_

#include "../../STD_TYPES.h"

typedef enum
{
	FP_STATUS_OK = 0x00,
	FP_STATUS_ERR_RECV,
	FP_STATUS_ERR_NO_FINGER,
	FP_STATUS_ERR_FAIL_ENROLL,
	FP_STATUS_ERR_FAIL_GEN_CHAR_DISORDER = 0x06,
	FP_STATUS_ERR_FAIL_GEN_CHAR_LACKNESS,
	FP_STATUS_ERR_FAIL_FINGER_MISMATCH,
	FP_STATUS_ERR_FAIL_FIND_MATCH_FINGER,
	FP_STATUS_ERR_FAIL_COMBINE_CHAR_FILES,
	FP_STATUS_ERR_PAGEID_OUT_OF_RANGE,
	FP_STATUS_ERR_TEMPLATE_READ,
	FP_STATUS_ERR_TEMPLATE_UPLOAD,
	FP_STATUS_ERR_MODULE_NORECV,
	FP_STATUS_ERR_FAIL_UPLOAD_IMG,
	FP_STATUS_ERR_FAIL_DELETE_TEMPLATE,
	FP_STATUS_ERR_FAIL_CLEAR_FINGER_LIB,
	FP_STATUS_ERR_WRONG_PASSWORD = 0x13,
	FP_STATUS_ERR_FAIL_GENERATE_IMAGE_LACKNESS = 0x15,
	FP_STATUS_ERR_WRITING_FLASH = 0x18,
	FP_STATUS_ERR_DEFINITION,
	FP_STATUS_ERR_INVALID_REG_NUM,
	FP_STATUS_ERR_INCORRECT_CONFIG_REG,
	FP_STATUS_ERR_WRONG_NOTEPAD_PAGE_NO,
	FP_STATUS_ERR_FAIL_COMPORT_OP,
	FP_STATUS_CHECKSUM_FAIL // Custom  0x1E
} FP_STATUS;

enum FP_Aura_Control
{
	FP_Aura_Control_Breathing = 0x01,
	FP_Aura_Control_FlashingLight,
	FP_Aura_Control_LightAlwaysON,
	FP_Aura_Control_LightAlwaysOFF,
	FP_Aura_Control_LightGraduallyON,
	FP_Aura_Control_LightGraduallyOFF
};

enum FP_Aura_Color
{
	FP_Aura_Color_Red = 0x01,
	FP_Aura_Color_Blue,
	FP_Aura_Color_Purple
};

void H_FingerPS_init();
FP_STATUS H_FingerPS_handShake();
FP_STATUS H_FingerPS_AuraControl(enum FP_Aura_Control copyu8_CtrlCode, u8 copyu8_Speed, enum FP_Aura_Color copyu8_ColorIndex, u8 copyu8_Timing);
FP_STATUS H_FingerPS_genImg();
FP_STATUS H_FingerPS_convertImg2CharFile(u8 copyu8_bufferID);
FP_STATUS H_FingerPS_genTemplate();
FP_STATUS H_FingerPS_strTemplate(u8 copyu8_bufferID, u16 copyu16_pageID);
FP_STATUS H_FingerPS_searchFinger(u8 copyu8_bufferID, u16 copyu16_startPage, u16 copyu16_pageNum, u16* pPageID, u16* pMatchScore);
FP_STATUS H_FingerPS_emptyLibrary();
FP_STATUS H_FingerPS_deleteFinger(u16 copyu16_pageID, u16 copyu16_nTemplates);
FP_STATUS H_FingerPS_LoadCharFile(u8 copyu8_bufferID, u16 copyu16_pageID);
FP_STATUS H_FingerPS_match(u16* pMatchScore);
FP_STATUS H_FingerPs_getNTemplates(u16* pNTemplates);



#endif /* FINGERPRINT_H_ */