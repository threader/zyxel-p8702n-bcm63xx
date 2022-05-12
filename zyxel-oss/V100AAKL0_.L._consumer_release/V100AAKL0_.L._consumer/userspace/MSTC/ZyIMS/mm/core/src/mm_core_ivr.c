/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, dIsclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>


#include "global.h"
#include "mm_glb.h"
#include "mm_core_fsm_types.h"
#include "mm_core_ivr.h"
#include "icf_types.h"
#include "dsp_driver_itf.h"
#include "tr104.h"

uint32 ivrsys_startaddr = NULL;
uint32 ivrsys_length = NULL;
uint32 ivrusr_startaddr = NULL;
uint32 ivrusr_length = NULL;


/**********************global variables***************************************/
pthread_mutex_t IVRNowProcessMutex = PTHREAD_MUTEX_INITIALIZER;;
VoiceElement_t* 	NowProcessElement=NULL; 
uint8* ivrSysCfgBuf=NULL;
uint8* ivrUsrCfgBuf=NULL;

/***********************function declarations**********************************/
VoiceElement_t* ivrCreateVoiceElement(uint32 size);
int ivrRecordStart(uint16 ivrIndex);
int ivrPlayStart(
	dspChan_t *dspChan_p,
	uint16 ivrIndex, /*user input ivr index , 50-70 means sysivr*/
	uint8  repeat,
	uint32 timeInterval
);
int 
ivrReplay(
	tm_id_t timer_id, 
	void*arg
);

extern int
RtpSendToDsp(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type,
	icf_uint32_t null_pload_type, 
	icf_uint8_t *InMessage, 
	icf_uint16_t MessageLen, 
	icf_uint32_t TimeStamp, 
	void* handle
);
int ivrSysLoadFromFile(char *path, int itemIndex, int codec);

/*______________________________________________________________________________
**	ivrRecordAcquireMutex
**	descriptions:  protect IVR Record data: NowProcessElement , acquire semaphore: IVRNowProcessMutex
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ivrRecordAcquireMutex(void){
	pthread_mutex_lock(&IVRNowProcessMutex);
}

/*______________________________________________________________________________
**	ivrRecordReleaseMutex
**	descriptions:  protect IVR Record data: NowProcessElement , release semaphore: IVRNowProcessMutex
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ivrRecordReleaseMutex(void){
	pthread_mutex_unlock(&IVRNowProcessMutex);
}


/*______________________________________________________________________________
**	ivrCheckCRC
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: TRUE: CRC check success/ FALSE: CRC check failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrCheckCRC(uint32 startaddr, uint32 len)
{
	uint32 crc=0, i=0;
	
	for( i=0; i<(len-4) ;i++) {
		crc += *(uint8*)(startaddr+4+i);
		if ( crc > 0xffff)
			crc -= 0xffff;
	}
	
	if (((VoiceHeader_t*)startaddr)->crc == crc){
		VDSP_PRINTF("ivrCheckCRC success crc = %x\n", crc);
		return TRUE;
	}
	else {
		VDSP_PRINTF("ivrCheckCRC failed!!!!\n");
		VDSP_PRINTF("ivrCheckCRC, len = %d\n", len);
		VDSP_PRINTF("ivrCheckCRC, crc = %x\n", crc);
		VDSP_PRINTF("ivrCheckCRC, ((VoiceHeader_t*)startaddr)->crc = %x\n", ((VoiceHeader_t*)startaddr)->crc);
		return FALSE;

	}

}

/*______________________________________________________________________________
**	ivrCountCRC
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: CRC value
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

uint32 ivrCountCRC(uint32 startaddr, uint32 len)
{
	uint32 crc=0, i=0;
	VDSP_PRINTF("ivrCountCRC len = %d\n", len);
	for( i=0; i<(len-4) ;i++) {
		crc += *(uint8*)(startaddr+4+i);
		if ( crc > 0xffff)
			crc -= 0xffff;
	}

	return crc;
}
/*______________________________________________________________________________
**	ivrDeleteVoiceEement
**
**	descriptions:
**	parameters: index: the physical number.
**	local:
**	global:
**	return: TRUE, FALSE
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrDeleteVoiceEement(uint16 index, uint16 type) {

	uint32 	Del_Element_Start_Addr=0, Del_Element_Len=0;
	uint32	Total_Len=0, Del_Offset_To_Element=0;
	uint16	i=0;
	uint8*  	CfgBuf = NULL;	

	VDSP_PRINTF("ivrDeleteVoiceEement index = %d, type= %d\n", index, type);
	if (type==TYPE_IVR_SYS_DEFAULT) {
		if (ivrSysCfgBuf == NULL){
			VDSP_PRINTF("ivrDeleteVoiceEement ERROR sysCfgBuf NULL\n");
			return FALSE;
		}
		else{
			CfgBuf=ivrSysCfgBuf;
		}
	} 
	else {
		if (ivrUsrCfgBuf == NULL){
			VDSP_PRINTF("ivrDeleteVoiceEement ERROR usrCfgBuf NULL\n");
			return FALSE;
		}
		else{
			CfgBuf=ivrUsrCfgBuf;
		}
	}
	VDSP_PRINTF("ivrSysCfgBuf = %x\n", ivrSysCfgBuf);
	VDSP_PRINTF("ivrUsrCfgBuf = %x\n", ivrUsrCfgBuf);
	VDSP_PRINTF("CfgBuf = ", CfgBuf);
	
	VDSP_PRINTF("ivrDeleteVoiceEement...\n");
	if (((VoiceHeader_t*)CfgBuf)->label[index].used == 0){		//not used
		VDSP_PRINTF("ivrDeleteVoiceEement item not used\n");
		return TRUE;
	}	

	Del_Offset_To_Element =  ((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element;
	VDSP_PRINTF("Del_Offset_To_Element %x\n", Del_Offset_To_Element);
	Del_Element_Start_Addr = (uint32)CfgBuf+Del_Offset_To_Element;
	VDSP_PRINTF("Del_Element_Start_Addr %x\n",Del_Element_Start_Addr);
	Del_Element_Len = ((VoiceElement_t*)Del_Element_Start_Addr)->element_length;
	VDSP_PRINTF("Del_Element_Len %d\n", Del_Element_Len);
	Total_Len = LEN_VOICE_HEADER+((VoiceHeader_t*)CfgBuf)->total_elements_length;	
	VDSP_PRINTF("Total_Len %d\n", Total_Len);
	VDSP_PRINTF("zyMemcpy len %d\n", Total_Len-(((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element+Del_Element_Len));
	zyMemcpy((uint8*)Del_Element_Start_Addr, (uint8*)(Del_Element_Start_Addr+Del_Element_Len),
		Total_Len-(((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element+Del_Element_Len));

	for( i = 0; i<Max_Voice_Elements; i++) {
		if ( (((VoiceHeader_t*)CfgBuf)->label[i].used!=0) && 
				(((VoiceHeader_t*)CfgBuf)->label[i].offset_to_element>Del_Offset_To_Element)) {
				((VoiceHeader_t*)CfgBuf)->label[i].offset_to_element-=Del_Element_Len;
		}	
	}

	((VoiceHeader_t*)CfgBuf)->total_elements_length -=  Del_Element_Len;
	((VoiceHeader_t*)CfgBuf)->label[index].used = 0;
	((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element=0;

	return TRUE;

}


/*______________________________________________________________________________
**	ivrInsertVoiceEement
**
**	descriptions: Insert voice element into list, permission check should prior at call this function.
**	parameters: index:the physical number of index
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrInsertVoiceEement(VoiceElement_t* element, uint16 index, uint16 type) 
{
	int CfgBugLen = 0;
	uint8*  CfgBuf = NULL;
	
	VDSP_PRINTF("ivrInsertVoiceEement type = %d\n", type);
	VDSP_PRINTF("ivrInsertVoiceEement index = %d\n", index);
	
	if (type == TYPE_IVR_SYS_DEFAULT) {
		if (ivrSysCfgBuf==NULL)
			return FALSE;
		else {
			CfgBuf=ivrSysCfgBuf;
			CfgBugLen=MAX_IVR_SYS_BUF_SIZE;
		}
	} 
	else {
		if (ivrUsrCfgBuf==NULL)
			return FALSE;
		else {
			CfgBuf=ivrUsrCfgBuf;
			CfgBugLen=MAX_IVR_USR_BUF_SIZE;
		}
	}
	VDSP_PRINTF("ivrSysCfgBuf = %x\n", ivrSysCfgBuf);
	VDSP_PRINTF("ivrUsrCfgBuf = %x\n", ivrUsrCfgBuf);
	VDSP_PRINTF("CfgBuf = %x\n", CfgBuf);
	/*Firstly find out if the original element exist*/
	if (((VoiceHeader_t*)CfgBuf)->label[index].used != 0) {
		
		if (ivrDeleteVoiceEement(index, type) == FALSE) {
			VDSP_PRINTF("Delete voice element failed.\n");
			return FALSE;
		}	
	}


	if (LEN_VOICE_HEADER+ ((VoiceHeader_t*)CfgBuf)->total_elements_length+ element->element_length > CfgBugLen) {
		VDSP_PRINTF("Buffer in flash is overflow\n");
		return FALSE;
	}



	zyMemcpy( CfgBuf+LEN_VOICE_HEADER+((VoiceHeader_t*)CfgBuf)->total_elements_length,
		element, element->element_length);
	

	((VoiceHeader_t*)CfgBuf)->label[index].used = 1;

	
	//put32((uint8*)&(((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element), 
	//	LEN_VOICE_HEADER+((VoiceHeader_t*)CfgBuf)->total_elements_length);
	((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element=LEN_VOICE_HEADER+((VoiceHeader_t*)CfgBuf)->total_elements_length;
	VDSP_PRINTF("ivrInsertVoiceEement offset_to_element: %x\n",((VoiceHeader_t*)CfgBuf)->label[index].offset_to_element);
	//put32((uint8*)&(((VoiceHeader_t*)CfgBuf)->total_elements_length),
	//	(((VoiceHeader_t*)CfgBuf)->total_elements_length)+element->element_length);
	((VoiceHeader_t*)CfgBuf)->total_elements_length=(((VoiceHeader_t*)CfgBuf)->total_elements_length)+element->element_length;	
	VDSP_PRINTF("ivrInsertVoiceEement totals_element_length: %x\n",((VoiceHeader_t*)CfgBuf)->total_elements_length);

	return TRUE;
}



/*______________________________________________________________________________
**	ivrInit
**	descriptions: Call by VoIP initialization process
**	parameters:
**	local:
**	global:
**	return: 0
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrInit(
	void
){
	int ret = 0;
	VDSP_PRINTF("ZYXEL_IVR_MAX_LEN = %d\n", ZYXEL_IVR_MAX_LEN);
	VDSP_PRINTF("Max_Single_Element_Size = %d\n", Max_Single_Element_Size);
	VDSP_PRINTF("Max_Voice_Elements = %d \n", Max_Voice_Elements);
	VDSP_PRINTF("LEN_VOICE_HEADER = %d\n", LEN_VOICE_HEADER);
	VDSP_PRINTF("MAX_IVR_SYS_BUF_SIZE = %d\n", MAX_IVR_SYS_BUF_SIZE);
	VDSP_PRINTF("MAX_IVR_USR_BUF_SIZE = %d\n", MAX_IVR_USR_BUF_SIZE);
	VDSP_PRINTF("IVR_SYS_FLASH_OFFSET = %d\n", IVR_SYS_FLASH_OFFSET);
	


	ivrsys_startaddr=NULL;
	ivrsys_length=LEN_VOICE_HEADER;
	if(ivrsys_startaddr==NULL) {
		ivrsys_startaddr=zyMalloc(MAX_IVR_SYS_BUF_SIZE);
	}
	ivrSysCfgBuf = NULL;

	ivrusr_startaddr=NULL;	
	ivrusr_length=LEN_VOICE_HEADER;	
	if(ivrusr_startaddr==NULL) {
		ivrusr_startaddr=zyMalloc(MAX_IVR_USR_BUF_SIZE);
	}
	ivrUsrCfgBuf = NULL;
	
	if(ivrusr_startaddr==NULL || ivrsys_startaddr==NULL){
		VDSP_PRINTF("ivrInit ivrusr_startaddr or ivrsys_startaddr malloc failed\n");
		return 0;
	}
	
	/**************init for usr ivr******************************/
	zyMemset((void *)ivrusr_startaddr, 0, MAX_IVR_USR_BUF_SIZE);
	ret = flashIvrRead(ivrusr_startaddr, MAX_IVR_USR_BUF_SIZE, 0);
	VDSP_PRINTF("flashIvrRead ivrusr_startaddr ret = %d\n", ret);	
	ivrusr_length = ((VoiceHeader_t*)ivrusr_startaddr)->total_elements_length+LEN_VOICE_HEADER;
	VDSP_PRINTF("flashIvrRead ivrusr_length = %d\n", ivrusr_length);	
	if(ivrusr_length > MAX_IVR_USR_BUF_SIZE){
		ivrusr_length = MAX_IVR_USR_BUF_SIZE;
	}

	if (ivrCheckCRC(ivrusr_startaddr, ivrusr_length)!=TRUE) {
		VDSP_PRINTF("ivrCheckCRC ivr usr failed, clear to default\n");
		ivrUsrClearToDefault();
	}

	
	/***************init for sys ivr ******************************/
	zyMemset((void *)ivrsys_startaddr,0,MAX_IVR_SYS_BUF_SIZE);
	ret = flashIvrRead(ivrsys_startaddr, MAX_IVR_SYS_BUF_SIZE, IVR_SYS_FLASH_OFFSET); /*sys ivr resides after usr ivr*/
	VDSP_PRINTF("flashIvrRead ivrsys_startaddr ret = %d\n", ret);
	ivrsys_length = ((VoiceHeader_t*)ivrsys_startaddr)->total_elements_length+LEN_VOICE_HEADER;
	VDSP_PRINTF("flashIvrRead ivrsys_length = %d\n", ivrsys_length);	
	if(ivrsys_length > MAX_IVR_SYS_BUF_SIZE){
		ivrsys_length = MAX_IVR_SYS_BUF_SIZE;
	}


	VDSP_PRINTF("ivrInit ivrsys_startaddr:%x, ivrsys_length:%d\n", ivrsys_startaddr, ivrsys_length);
	VDSP_PRINTF("ivrInit ivrusr_startaddr:%x, ivrusr_length:%d\n", ivrusr_startaddr, ivrusr_length);

	
	if(ivrsys_length == LEN_VOICE_HEADER){		
		ret = ivrSysLoadDefaultFromFile("/bin/ivrsys.bin");	
		if (ret == TRUE) {
			ret = ivrSysLoadFromFile("/bin/howler_ulaw.bin", IVRPRON_HOWLER_TONE, ZYNOS_CODEC_PCMU);
			VDSP_PRINTF("ivrSysLoadFromFile ret = %d",ret);
		}
		
	}

	if (ivrCheckCRC(ivrsys_startaddr, ivrsys_length)!=TRUE) {
		VDSP_PRINTF("ivrCheckCRC ivr sys failed, clear to default\n");
		ivrSysLoadDefaultFromFile("/bin/ivrsys.bin");		
	}

	

	ivrDump();
	#if 0
	rtpHeader = zyMalloc(RTP_FIXED_HEADER_LEN);
	/* Check IVR SYS Codec Compatiability */
	for(i = 0; i< NUM_IVRSYS; i++){
		if (((VoiceHeader_t*)ivrsys_startaddr)->label[i].used == 0)
			continue;
		zyMemset(rtpHeader, 0, RTP_FIXED_HEADER_LEN);
		StartAddrInFlash = (uint32)ivrsys_startaddr+ ((VoiceHeader_t*)ivrsys_startaddr)->label[i].offset_to_element;
		zyMemcpy(rtpHeader,  (uint8*)(StartAddrInFlash+4), RTP_FIXED_HEADER_LEN);
		if(DSPCheckCodecAvail(rtpHeader->pt) == FALSE){
			ZyPrintf("\r\n IVR SYS is NOT Usable\r\n");
		}
	}
	/* Check IVR User Codec Compatiability */
	for(i = 0; i< NUM_IVRUSR; i++){
		if (((VoiceHeader_t*)ivrusr_startaddr)->label[i].used == 0)
			continue;
		zyMemset(rtpHeader, 0, RTP_FIXED_HEADER_LEN);
		StartAddrInFlash = (uint32)ivrusr_startaddr+ ((VoiceHeader_t*)ivrusr_startaddr)->label[i].offset_to_element;
		zyMemcpy(rtpHeader,  (uint8*)(StartAddrInFlash+4), RTP_FIXED_HEADER_LEN);
		if(DSPCheckCodecAvail(rtpHeader->pt) == FALSE){		
			ivrUsrClearToDefault();
		}
	}

	zyFree(rtpHeader);
	#endif
	
	return 0;
}



/*______________________________________________________________________________
**	ivrSysLoadAll
**	descriptions: Load ivrsys data from flash into ram ivrCfgBuf
**	parameters:
**	local:
**	global:
**	return: TRUE: success		FALSE: failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int ivrSysLoadAll(void)
{	
	int ret = FALSE;
	VDSP_PRINTF("ivrSysLoadAll\n");
	if( NULL == ivrSysCfgBuf ){
		ivrSysCfgBuf = zyMalloc(MAX_IVR_SYS_BUF_SIZE);
	}
	
	if( NULL == ivrSysCfgBuf ) {
		VDSP_PRINTF("(ivrSysLoadAll) Can't allocate ivrSysCfgBuf !!\n");
		return FALSE;
	}

	zyMemset(ivrsys_startaddr, 0, MAX_IVR_SYS_BUF_SIZE);
	ret = flashIvrRead(ivrsys_startaddr, MAX_IVR_SYS_BUF_SIZE, IVR_SYS_FLASH_OFFSET);
	VDSP_PRINTF("ivrSysLoadAll flashIvrRead ret =%d\n", ret);
	if(ret != TRUE){
		return FALSE;
	}
		
	ivrsys_length=((VoiceHeader_t*)ivrsys_startaddr)->total_elements_length+LEN_VOICE_HEADER;
	if(ivrsys_length > MAX_IVR_SYS_BUF_SIZE){
		ivrsys_length = MAX_IVR_SYS_BUF_SIZE;
	}
	VDSP_PRINTF("ivrSysLoadAll ivrsys_length =%d\n", ivrsys_length);
	
	if(ivrCheckCRC(ivrsys_startaddr, ivrsys_length)!= TRUE){
		return FALSE;
	}
	
	zyMemset(ivrSysCfgBuf, 0, MAX_IVR_SYS_BUF_SIZE);
	zyMemcpy(ivrSysCfgBuf, (uint8*)ivrsys_startaddr, ivrsys_length);

	return TRUE;
}

/*______________________________________________________________________________
**	ivrSysSaveAll
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: TRUE: success		FALSE: failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int ivrSysSaveAll(void)
{
	int wcount = 0;
	uint32	crc=0;
	int ret = FALSE;

	if (ivrSysCfgBuf==NULL)
		return FALSE;

	wcount=((VoiceHeader_t*)ivrSysCfgBuf)->total_elements_length+LEN_VOICE_HEADER;
	
	if(wcount>MAX_IVR_SYS_BUF_SIZE){
		wcount=MAX_IVR_SYS_BUF_SIZE;
	}
	VDSP_PRINTF("ivrSysSaveAll wcount:%d\n",wcount);
	
	/*count crc*/
	crc = ivrCountCRC( (uint32)(ivrSysCfgBuf),  wcount);
	((VoiceHeader_t*)ivrSysCfgBuf)->crc = crc;
	VDSP_PRINTF("ivrSysSaveAll, crc = %x\n",crc);
	
	
	zyMemset(ivrsys_startaddr, 0, MAX_IVR_SYS_BUF_SIZE);	
	zyMemcpy((uint8 *)ivrsys_startaddr, (uint8*)ivrSysCfgBuf, wcount);
	

	ret = flashIvrWrite(ivrsys_startaddr, wcount, IVR_SYS_FLASH_OFFSET);
	VDSP_PRINTF("ivrSysSaveAll flashIvrWrite ret = %d\n", ret);
	ivrsys_length = wcount;

	
	zyFree(ivrSysCfgBuf);
	ivrSysCfgBuf=NULL;
	
	return TRUE;

}


/*______________________________________________________________________________
**	ivrSysClearToDefault
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrSysClearToDefault(void)
{	
	int i = 0;
	
	if( NULL == ivrSysCfgBuf ){
		ivrSysCfgBuf = zyMalloc(MAX_IVR_SYS_BUF_SIZE);
	}
	
	if( NULL == ivrSysCfgBuf ) {
		VDSP_PRINTF("(ivrSysClearToDefault) Can't allocate ivrSysCfgBuf !!\n");
		return FALSE;
	}
	
	zyMemset(ivrSysCfgBuf, 0, MAX_IVR_SYS_BUF_SIZE);

	for( i = 0; i<NUM_IVRSYS; i++) {
		((VoiceHeader_t*)ivrSysCfgBuf)->label[i].type=TYPE_IVR_SYS_DEFAULT;
		((VoiceHeader_t*)ivrSysCfgBuf)->label[i].used=0;
		((VoiceHeader_t*)ivrSysCfgBuf)->label[i].offset_to_element=0;
	}
	((VoiceHeader_t*)ivrSysCfgBuf)->total_elements_length=0;

	ivrSysSaveAll();
	
	return TRUE;
}

/*______________________________________________________________________________
**	ivr_sys_load_default_file
**	descriptions: Load ivrusr data from flash into ram ivrCfgBuf
**	parameters:
**	local:
**	global:
**	return: TRUE: success		FALSE: failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrSysLoadDefaultFromFile(char *path)
{	
	int rcount = 0, ret = 0, crc = 0;
	FILE * fp = NULL;	
	
	VDSP_PRINTF("ivrSysLoadDefaultFromFile\n");
	fp = fopen(path, "r");
	if(fp == NULL){
		VDSP_PRINTF("ivrSysLoadDefaultFromFile fail, file cannot open\n");
		return FALSE;
	}	
		
	zyMemset(ivrsys_startaddr, 0, MAX_IVR_SYS_BUF_SIZE);
	rcount = fread(ivrsys_startaddr, 1, MAX_IVR_SYS_BUF_SIZE, fp);			
	fclose(fp);
	ivrsys_length = rcount;
	VDSP_PRINTF("ivrSysLoadDefaultFromFile, read size = %d\n", rcount);
	
	if (ivrCheckCRC(ivrsys_startaddr, ivrsys_length)!=TRUE) {
		VDSP_PRINTF("ivrCheckCRC ivr sys failed, clear to default\n");
		ivrSysClearToDefault();
		return FALSE;
	}
	((VoiceHeader_t *)ivrsys_startaddr)->total_elements_length = ivrsys_length -LEN_VOICE_HEADER;
	crc = ivrCountCRC(ivrsys_startaddr, ivrsys_length);
	((VoiceHeader_t *)ivrsys_startaddr)->crc = crc;
	VDSP_PRINTF("ivrSysLoadDefaultFromFile crc = %x\n", crc);
	ret = flashIvrWrite(ivrsys_startaddr, ivrsys_length, IVR_SYS_FLASH_OFFSET);
	VDSP_PRINTF("ivrUsrSaveAll flashIvrWrite ret = %d, ivrsys_length = %d\n", ret, ivrsys_length);

	if(ret == 0){
		return FALSE;
	}
	
	return TRUE;
}


/*______________________________________________________________________________
**	ivrSysLoadFromFile
**	descriptions: Load ivrsys data from file
**	parameters:
**	local:
**	global:
**	return: TRUE: success		FALSE: failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
#define DEFAULT_BLOCK_SIZE 512

int ivrSysLoadFromFile(char *path, int itemIndex, int codec)
{
	int rcount = 0;
	FILE * fp = NULL;
	uint8 buff[DEFAULT_BLOCK_SIZE];
	VoiceElement_t * element = ivrCreateVoiceElement(Max_Single_Element_Size);
	
	if (ivrSysLoadAll()!=TRUE){
		return FALSE;
	}

	
	/* Check IVR SYS Codec Compatiability */
	
	fp = fopen(path, "r");
	if(fp == NULL){
		VDSP_PRINTF("ivrSysLoadFromFile fail, file cannot open\n");
		return FALSE;
	}
	//fseek(fp, 58, SEEK_SET); /*Wave file format, header 58 bytes, RIFF, PCMU*/
	
	while(!feof(fp)){	
		
		rcount = fread(buff, 1, DEFAULT_BLOCK_SIZE, fp);
		
		if(rcount>0){
			zyMemcpy((uint8*)element+element->element_length, buff, rcount);
			element->element_length += rcount;				
		}
		else{
			break;
		}
		
		
	}			

	VDSP_PRINTF("ivrSysLoadFromFile, element load complete, element length = %d\n", element->element_length);
	
	((rtp_header_t)(element->rtp_header)).version = 2;	
	((rtp_header_t)(element->rtp_header)).pt = codec;
	((rtp_header_t)(element->rtp_header)).ts = rand();
	((rtp_header_t)(element->rtp_header)).ssrc = rand();
	((rtp_header_t)(element->rtp_header)).seq = 0;
	ivrInsertVoiceEement(element, itemIndex, TYPE_IVR_SYS_DEFAULT);

	if (ivrSysSaveAll()!=TRUE){
		return FALSE;
	}
	
	return TRUE;
}


/*______________________________________________________________________________
**	ivrUsrLoadAll
**	descriptions: Load ivrusr data from flash into ram ivrCfgBuf
**	parameters:
**	local:
**	global:
**	return: TRUE: success		FALSE: failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/



int ivrUsrLoadAll(void)
{
	int ret = FALSE;
	VDSP_PRINTF("ivrUsrLoadAll\n");
	
	if(ivrUsrCfgBuf==NULL)
		ivrUsrCfgBuf = zyMalloc(MAX_IVR_USR_BUF_SIZE);
	if (ivrUsrCfgBuf==NULL) {
		VDSP_PRINTF("(ivrUsrLoadAll) Can't allocate ivrUsrCfgBuf !!\n");
		return FALSE;
	}
	
	
	zyMemset(ivrusr_startaddr, 0, MAX_IVR_USR_BUF_SIZE);
	ret = flashIvrRead(ivrusr_startaddr, MAX_IVR_USR_BUF_SIZE, 0);
	ivrusr_length=((VoiceHeader_t*)ivrusr_startaddr)->total_elements_length+LEN_VOICE_HEADER;
	if(ivrusr_length > MAX_IVR_USR_BUF_SIZE){
		ivrusr_length = MAX_IVR_USR_BUF_SIZE;
	}
	
	VDSP_PRINTF("ivrUsrLoadAll flashIvrRead ret =%d\n", ret);

	if(ivrCheckCRC(ivrusr_startaddr, ivrusr_length)!= TRUE){
		return FALSE;
	}
	
	zyMemset(ivrUsrCfgBuf, 0, MAX_IVR_USR_BUF_SIZE);
	zyMemcpy(ivrUsrCfgBuf, (uint8*)ivrusr_startaddr, ivrusr_length);
	
	

	return TRUE;
}


/*______________________________________________________________________________
**	ivrUsrSaveAll
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: TRUE: success		FALSE: failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrUsrSaveAll(void)
{	
	int wcount=0;	
	uint32	crc=0;
	int ret = FALSE;
	
	VDSP_PRINTF("ivrUsrSaveAll\n");	
	if (ivrUsrCfgBuf==NULL)
		return FALSE;

	/*count crc*/
	
	
	wcount=((VoiceHeader_t*)ivrUsrCfgBuf)->total_elements_length+LEN_VOICE_HEADER;
	
	if(wcount > MAX_IVR_USR_BUF_SIZE){
		wcount = MAX_IVR_USR_BUF_SIZE;
	}

	VDSP_PRINTF("ivrUsrSaveAll wcount:%d\n", wcount);
	
	crc = ivrCountCRC( (uint32)(ivrUsrCfgBuf),  wcount);
	((VoiceHeader_t*)ivrUsrCfgBuf)->crc = crc;
	VDSP_PRINTF("ivrUsrSaveAll, crc = %x\n",crc);
	
	
	zyMemset(ivrusr_startaddr, 0, MAX_IVR_USR_BUF_SIZE);
	zyMemcpy((uint8 *)ivrusr_startaddr, (uint8*)ivrUsrCfgBuf, wcount);
	ivrusr_length = wcount;	

	
	ret = flashIvrWrite(ivrusr_startaddr, wcount, 0);
	VDSP_PRINTF("ivrUsrSaveAll flashIvrWrite ret = %d\n", ret);

	zyFree(ivrUsrCfgBuf);
	ivrUsrCfgBuf=NULL;

	return TRUE;

}

/*______________________________________________________________________________
**	ivrUsrClearToDefault
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrUsrClearToDefault(void)
{
	int i=0;
	
	VDSP_PRINTF("ivrUsrClearToDefault\n");
	
	if(ivrUsrCfgBuf==NULL)
		ivrUsrCfgBuf = zyMalloc(MAX_IVR_USR_BUF_SIZE);
	if (ivrUsrCfgBuf==NULL) {
		VDSP_PRINTF("(ivrUsrClearToDefault) Can't allocate ivrUsrCfgBuf !!\n");
		return FALSE;
	}

	zyMemset(ivrUsrCfgBuf, 0, MAX_IVR_USR_BUF_SIZE);

	for( i = 0; i<NUM_IVRUSR; i++) {
		((VoiceHeader_t*)ivrUsrCfgBuf)->label[i].type=TYPE_IVR_USR_DEFINED;
		((VoiceHeader_t*)ivrUsrCfgBuf)->label[i].used=0;
		((VoiceHeader_t*)ivrUsrCfgBuf)->label[i].offset_to_element=0;
	}
	
	((VoiceHeader_t*)ivrUsrCfgBuf)->total_elements_length=0;

	ivrUsrSaveAll();

	return TRUE;
}


/*______________________________________________________________________________
**	ivrDump
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrDump(void)
{
	int i=0;	

	if (ivrusr_startaddr == NULL){
		VDSP_PRINTF("ivrDump ivrusr_startaddr NULL return\n");
		return FALSE;
	}
	
	VDSP_PRINTF("	IVR USR DUMP\n");

	for( i = 0; i<NUM_IVRUSR; i++) {		
		VDSP_PRINTF("	label = %d\n",i);
		VDSP_PRINTF("	type = %d\n",((VoiceHeader_t*)ivrusr_startaddr)->label[i].type);
		VDSP_PRINTF("	used = %d\n",((VoiceHeader_t*)ivrusr_startaddr)->label[i].used);
		VDSP_PRINTF("	offset_to_element = %d\n",((VoiceHeader_t*)ivrusr_startaddr)->label[i].offset_to_element);
	}	
	
	VDSP_PRINTF("	IVR USR DUMP END\n");

	if (ivrsys_startaddr == NULL){
		VDSP_PRINTF("ivrDump ivrsys_startaddr NULL return\n");
		return FALSE;
	}

	VDSP_PRINTF("	IVR SYS DUMP\n");

	for( i = 0; i<NUM_IVRSYS; i++) {		
		VDSP_PRINTF("	label = %d\n",i);
		VDSP_PRINTF("	type = %d\n",((VoiceHeader_t*)ivrsys_startaddr)->label[i].type);
		VDSP_PRINTF("	used = %d\n",((VoiceHeader_t*)ivrsys_startaddr)->label[i].used);
		VDSP_PRINTF("	offset_to_element = %d\n",((VoiceHeader_t*)ivrsys_startaddr)->label[i].offset_to_element);
	}	
	VDSP_PRINTF("	IVR SYS DUMP END\n");

	return TRUE;
}


/*______________________________________________________________________________
**	ivrVoiceElementRemoveTail
**
**	descriptions: remove voice element tail to remove # tone when recording over
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ivrVoiceElementRemoveTail(
	VoiceElement_t*  element,
	int msec
) {	
	rtp_header_t * rtpHeader = NULL;
	int packettime = 0, packetsize = 0;
	rtpHeader = &(element->rtp_header);

	if(rtpHeader == NULL){
		return FALSE;
	}
	
	switch(rtpHeader->pt){
		case ZYNOS_CODEC_PCMU:
		case ZYNOS_CODEC_PCMA:
			packetsize = IVR_G711_SIZE;				/*Fix rtp packetsize = 20ms*/
			packettime = IVR_G711_TIME;				/*Fix rtp packetsize = 20ms*/
			break;
		case ZYNOS_CODEC_G723:
			packetsize = IVR_G723_SIZE;
			packettime = IVR_G723_TIME;				/*Fix rtp packetsize = 30ms*/
			break;
		case ZYNOS_CODEC_G726_16:
		case ZYNOS_CODEC_G726_24:
		case ZYNOS_CODEC_G726_32:
		case ZYNOS_CODEC_G726_40:	
			packetsize = IVR_G726_SIZE;				/*Fix rtp packetsize = 30ms*/
			packettime = IVR_G726_TIME;				/*Fix rtp packetsize = 20ms*/
			break;
		case ZYNOS_CODEC_G729:		
			packetsize = IVR_G729_SIZE;				/*Fix rtp packetsize = 20ms*/
			packettime = IVR_G729_TIME;				/*Fix rtp packetsize = 20ms*/
			break;
		default:
			VDSP_PRINTF("payload type not supported = %d\n", rtpHeader->pt);			
			return FALSE;
			break;
	}

	VDSP_PRINTF("original voice element length = %d\n", element->element_length);
	element->element_length -= msec*packetsize/packettime;
	VDSP_PRINTF("after tail remove, voice element length = %d\n", element->element_length);

	return TRUE;
	
}


/*______________________________________________________________________________
**	ivrCreateVoiceElement
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
VoiceElement_t* 
ivrCreateVoiceElement(
	uint32 size
) {
	VoiceElement_t*	ivr_element;
	VDSP_PRINTF("ivrCreateVoiceElement\n");
	ivr_element = (VoiceElement_t*)zyMalloc(size);		//default maxinum size of one voice entry is 20k (record 20sec)
	if (ivr_element==NULL) {
		VDSP_PRINTF("Alloc ivr_element failed in ivrCreateVoiceElement\n");
		return NULL;
	}
	zyMemset(ivr_element, 0, size);

	ivr_element->element_length = LEN_VOICE_ELEMENT_PRE;
	return (ivr_element);
}

/*______________________________________________________________________________
**	ivrReleaseVoiceElement
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

void
ivrReleaseVoiceElement(
	VoiceElement_t**  element
){
	if ( *element != NULL ) {
		zyFree(*element);
		*element = NULL;
	}

	return;
}


/*______________________________________________________________________________
**	ivrRecordFunctions
**	descriptions: Prepare to start record voice. allocate one space for voice element
**	parameters:
**	local:
**	global:
**	return: TRUE: success. FALSE: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ivrRecordFunctions(
	uint16 ivrIndex, /*user input ivr index , 50-70 means sysivr*/
	int action
){
	int ret = TRUE;
	switch(action){
		case IVR_FUNC_ACTION_START:
			ret = ivrRecordStart(ivrIndex);
			break;
		case IVR_FUNC_ACTION_STOP_SAVE:
			ret = ivrRecordStop(ivrIndex);
			break;
		case IVR_FUNC_ACTION_STOP:
			ivrRecordHalt(ivrIndex);
			break;
		default:
			break;
	}

	return ret;
}

/*______________________________________________________________________________
**	ivrRecordStart
**	descriptions: Prepare to start record voice. allocate one space for voice element
**	parameters:
**	local:
**	global:
**	return: TRUE: success. FALSE: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrRecordStart(
	uint16 ivrIndex
){
	VDSP_PRINTF("ivrRecordStart\n");
	if (ivrIndex >= IVRSYS_OFFSET) {
		ivrIndex -= IVRSYS_OFFSET;
		if((ivrIndex >= NUM_IVRSYS) || (ivrIndex < 0)){
			VDSP_PRINTF("sys ivr index out of range, index = %d\n", ivrIndex);
			return FALSE;
		}
		VDSP_PRINTF("type SYS_DEFAULT\n");
	} 
	else{
		if((ivrIndex >= NUM_IVRUSR) || (ivrIndex < 0)){
			VDSP_PRINTF("usr ivr index out of range, index = %d\n", ivrIndex);
			return FALSE;
		}
		VDSP_PRINTF("type USR_DEFINED\n");
	}
	VDSP_PRINTF("index = %d\n", ivrIndex);

	

	
	ivrRecordAcquireMutex();
	NowProcessElement = ivrCreateVoiceElement(Max_Single_Element_Size);	
	ivrRecordReleaseMutex();
	
	if (NowProcessElement==NULL)
		return FALSE;

	return TRUE;
} /*ivrRecordStart*/

/*______________________________________________________________________________
**	ivrRecordStop
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return: TRUE:success ; FALSE :failed.
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int 
ivrRecordStop(
	int ivrIndex
){
	int type=0, status=0, ret=FALSE;
	VDSP_PRINTF("ivrRecordStop\n");
	/* Load Image from flash into ram according to type*/

	if (ivrIndex >= IVRSYS_OFFSET) {
		if (ivrSysLoadAll()!=TRUE){
			goto exit;
		}

		type = TYPE_IVR_SYS_DEFAULT;
		ivrIndex -= IVRSYS_OFFSET;
		if((ivrIndex >= NUM_IVRSYS) || (ivrIndex < 0)){
			VDSP_PRINTF("sys ivr index out of range, index = %d\n", ivrIndex);
			goto exit;
		}
	} 
	else{
		if (ivrUsrLoadAll()!=TRUE){
			goto exit;
		}

		type = TYPE_IVR_USR_DEFINED;
		if((ivrIndex >= NUM_IVRUSR) || (ivrIndex < 0)){
			VDSP_PRINTF("usr ivr index out of range, index = %d\n", ivrIndex);
			goto exit;
		}
	}
	VDSP_PRINTF("type = %d index = %d\n", type, ivrIndex);

	ivrRecordAcquireMutex();
	ivrVoiceElementRemoveTail(NowProcessElement, 500); /*remove # tone*/
	ivrInsertVoiceEement(NowProcessElement, ivrIndex, type);
	ivrRecordReleaseMutex();

	

	/* Store Image from ram to flash */
	if ( type==TYPE_IVR_SYS_DEFAULT ) {
		if (ivrSysSaveAll()!=TRUE)
			goto exit;
	} 
	else {
		if (ivrUsrSaveAll()!=TRUE)
			goto exit;
	}

	ret = ivrDump();
exit:
	ivrRecordAcquireMutex();
	ivrReleaseVoiceElement(&NowProcessElement);
	ivrRecordReleaseMutex();
	
	return ret;
}

/*______________________________________________________________________________
**	ivrRecordHalt
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void 
ivrRecordHalt(
	int ivrIndex
){
	if( NowProcessElement != NULL){
		ivrRecordAcquireMutex();
		ivrReleaseVoiceElement(&NowProcessElement);
		ivrRecordReleaseMutex();
	}
	return;
}


/*______________________________________________________________________________
**	rtpRecord
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return: 0: Success
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int	rtpRecord(
	icf_call_id_t call_id, 
	icf_stream_id_t stream_id, 
	icf_media_type_t stream_type, 
	uint16 logicalId, 
	icf_uint32_t pload_type, /*FAX/RTP/RTCP*/
	icf_uint8_t *packet, 
	icf_uint16_t len, 
	icf_uint32_t timestamp, 
	void* handle
)
{


	ivrRecordAcquireMutex();	

	if ( NULL == NowProcessElement )
		goto exit;


	if( ZYNOS_CODEC_RFC2833 == ((rtp_header_t *)packet)->pt ){
		VDSP_PRINTF("rtpRecord ptype RFC2833 exiting...\n");
		goto exit;
	}
	
	if ( LEN_VOICE_ELEMENT_PRE == NowProcessElement->element_length ) {		// the first rtp packet, record the header.
		zyMemcpy( (void*)&(NowProcessElement->rtp_header), packet, RTP_FIXED_HEADER_LEN);	
		VDSP_PRINTF("rtpRecord first rtp packet payload = %d\n", NowProcessElement->rtp_header.pt);
	}

	if ( (((rtp_header_t*)packet)->ssrc==NowProcessElement->rtp_header.ssrc) &&
		(((rtp_header_t*)packet)->pt==NowProcessElement->rtp_header.pt) &&
		(NowProcessElement->element_length + (len-RTP_FIXED_HEADER_LEN) < Max_Single_Element_Size)) {

		zyMemcpy(  (uint8*)NowProcessElement+NowProcessElement->element_length, packet+RTP_FIXED_HEADER_LEN,  len-RTP_FIXED_HEADER_LEN);
		NowProcessElement->element_length+= (len-RTP_FIXED_HEADER_LEN);		
	}
			
exit:
	ivrRecordReleaseMutex();

	return 0;
}


/*______________________________________________________________________________
**	ivrPlayFunctions
**	descriptions: Prepare to start record voice. allocate one space for voice element
**	parameters:
**	local:
**	global:
**	return: TRUE: success. FALSE: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ivrPlayFunctions(dspChan_t *dspChan_p, uint16 ivrIndex, int action){
	int ret = TRUE;
	switch(action){
		case IVR_FUNC_ACTION_START:			
			ret = ivrPlayStart(dspChan_p, ivrIndex, 0, 0);
			break;
		case IVR_FUNC_ACTION_STOP:
			/*stop periodic timer*/
			ret = ivrPlayStop(dspChan_p);
			break;		
		default:
			break;
	}

	return ret;
}
/*______________________________________________________________________________
**	ivrDeleteItem
**	descriptions: Prepare to start record voice. allocate one space for voice element
**	parameters:
**	local:
**	global:
**	return: TRUE: success. FALSE: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
ivrDeleteItem(uint16 ivrIndex){
	int ivrType = 0;
	int ret = FALSE;
	if (ivrIndex >= IVRSYS_OFFSET) {
			ivrType = TYPE_IVR_SYS_DEFAULT;
			ivrIndex -= IVRSYS_OFFSET;

			if(ivrSysLoadAll()!=TRUE){
				return FALSE;
			}
					
	}else{
			ivrType = TYPE_IVR_USR_DEFINED;
			if(ivrUsrLoadAll()!=TRUE){
				return FALSE;
			}
	}

	if(TRUE == ivrDeleteVoiceEement(ivrIndex, ivrType)){
		if(ivrType == TYPE_IVR_SYS_DEFAULT){
			ret = ivrSysSaveAll();
		}
		else if(ivrType == TYPE_IVR_USR_DEFINED){
			ret = ivrUsrSaveAll();
		}
	}

	return ret;
}



/*______________________________________________________________________________
**	ivrPlay
**	descriptions:
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrPlay(
	tm_id_t timer_id, 
	void*arg
){	
	uint32 timestamp = 0;
	uint16 logicalId = (uint16)arg;	
	dspChan_t* dspChan_p = NULL;
	
	
	dspChan_p = dspPoolFindDspChanByLogicalId(logicalId);
	
	if(dspChan_p == NULL){	
		VDSP_PRINTF("ivrPlay() dsp chan NULL, exiting \n");
		goto exit;
	}

	
	

	if (  dspChan_p->per.numOfPackets == 0 ) {		/*avoid remaining timer event affect state machine run when timer already stopped.*/	
		VDSP_PRINTF("ivrPlay() numOfPackets 0, exiting \n");
		goto exit;
	}

	if ( dspChan_p->per.seqOfPacket == dspChan_p->per.numOfPackets ) {	
		if (dspChan_p->per.Repeat==0) {			/* one time play */						
			timerPeriodicStop(dspChan_p->per.periodTimerId);
			VDSP_PRINTF("ivrPlay, the end of one time play. stop periodic timer id = %d \n", dspChan_p->per.periodTimerId);
			dspChan_p->per.periodTimerId = 0;
			ivrPlayStart(dspChan_p, IVRSYS_OFFSET+IVRPRON_MAIN_PROMPT, 0, 0);
			goto exit;
		} else {										/* multiple times play */
			/* overflow of voice element, repeat from the head*/
			VDSP_PRINTF("ivrPlay, the end of one multiple time play. chanId=%d \n", dspChan_p->phLogicalId);

			if (dspChan_p->per.periodTimerId != 0) {							
				timerPeriodicStop(dspChan_p->per.periodTimerId);
				VDSP_PRINTF("stop periodic timer id = %d\n",dspChan_p->per.periodTimerId);
				dspChan_p->per.periodTimerId = 0;
			}
		
			dspChan_p->per.ReplayTimerId = timerStartS(dspChan_p->per.Interval, ivrReplay, dspChan_p->phLogicalId);
		
			VDSP_PRINTF("reenable timerPeriodicStartS in multiple times play. timerid=%d \n", dspChan_p->per.periodTimerId);
			goto exit;
		}
	} else if ( dspChan_p->per.seqOfPacket > dspChan_p->per.numOfPackets ) {
		VDSP_PRINTF("ivrPlay() seqOfPacket > numOfPackets, exiting \n");
		goto exit;
	}

	zyMemcpy(dspChan_p->per.packet+RTP_FIXED_HEADER_LEN, 
			 (uint8*)dspChan_p->per.elementStartAddr+ LEN_VOICE_ELEMENT_PRE+(dspChan_p->per.seqOfPacket * dspChan_p->per.packetsize),
			dspChan_p->per.packetsize);
		
	((rtp_header_t*)dspChan_p->per.packet)->seq = htons( dspChan_p->per.seqOfPacket );
	if ( dspChan_p->per.voipIvrCallBack != NULL ) {
		
		dspChan_p->per.voipIvrCallBack(dspChan_p->call_id, dspChan_p->stream_id, dspChan_p->stream_type, 
			0, dspChan_p->per.packet, RTP_FIXED_HEADER_LEN+dspChan_p->per.packetsize, 0, NULL);			
	}

	timestamp=((rtp_header_t*)dspChan_p->per.packet)->ts;

	((rtp_header_t*)dspChan_p->per.packet)->ts = htonl(timestamp+dspChan_p->per.packettime*8);

	((rtp_header_t*)dspChan_p->per.packet)->m = 0; 
	dspChan_p->per.seqOfPacket++;
exit:		
	
	return 0;
}




/*______________________________________________________________________________
**	ivrReplay
**	descriptions:
**	parameters:
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int 
ivrReplay(
	tm_id_t timer_id, 
	void*arg
){
	uint16 logicalId = (uint16) arg;
	dspChan_t* dspChan_p = NULL;

	VDSP_PRINTF("ivrReplay() dsp logicalId = %d\n", logicalId);
	
			
	dspChan_p = dspPoolFindDspChanByLogicalId(logicalId);

	if(dspChan_p == NULL){
		return 0;
	}
	
	
	
	if(dspChan_p->per.ReplayTimerId != 0){
		timerStop(dspChan_p->per.ReplayTimerId);
		dspChan_p->per.ReplayTimerId = 0;
	}

	/* reset packet content for repeat play */
	dspChan_p->per.seqOfPacket = 0;

	((rtp_header_t*)dspChan_p->per.packet)->ts=htonl(240);

	((rtp_header_t*)dspChan_p->per.packet)->m = 1;

	if(dspChan_p->per.periodTimerId != 0){
		timerPeriodicStop(dspChan_p->per.periodTimerId);
		dspChan_p->per.periodTimerId = 0;
	}
	dspChan_p->per.periodTimerId= timerPeriodicStartS( dspChan_p->per.packettime, ivrPlay, dspChan_p->phLogicalId);
	
	return 0;
}


/*______________________________________________________________________________
**	ivrPlayStart
**	descriptions:
**	parameters: 	repeat=0 means one time play,  
**				repeat=1 means periodically play.
**	local:
**	global:
**	return: 
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int 
ivrPlayStart(
	dspChan_t *dspChan_p,
	uint16 ivrIndex, /*user input ivr index , 50-70 means sysivr*/
	uint8  repeat,
	uint32 timeInterval
){
	uint32 StartAddrInFlash = 0;
	rtp_header_t* rtpHeader = NULL;
	uint16 ivrType = 0;
	

	if ( dspChan_p == NULL ) {
		VDSP_PRINTF("ivrPlayStart dspChan_p NULL return...\n");
		return FALSE;
	}
	
	dspChan_p->per.voipIvrCallBack = RtpSendToDsp;

	if (ivrIndex >= IVRSYS_OFFSET) {
		ivrType = TYPE_IVR_SYS_DEFAULT;
		ivrIndex -= IVRSYS_OFFSET;
				
	}else{
		ivrType = TYPE_IVR_USR_DEFINED;
	}

	VDSP_PRINTF("Enter ivrPlayStart, ivrType: %d, index: %d\n", ivrType, ivrIndex);	

	if (ivrType == TYPE_IVR_SYS_DEFAULT) {
		if ( ( ivrIndex >= NUM_IVRSYS ) || ( ivrIndex < 0 ) ) {
			VDSP_PRINTF("sys ivr index out of range, index = %d\n", ivrIndex);
			return FALSE;
		}
		
		if(ivrIndex == IVRPRON_MAIN_PROMPT){
			repeat = 1; 
		}
		
		StartAddrInFlash = ivrsys_startaddr;		 
		if (ivrCheckCRC(StartAddrInFlash, ivrsys_length)!=TRUE) {
			VDSP_PRINTF("ivrCheckCRC ivr sys failed\n");
			return FALSE;
		}
	} 
	else if (ivrType == TYPE_IVR_USR_DEFINED) {
		if ( ( ivrIndex>= NUM_IVRUSR ) || ( ivrIndex < 0 ) ) {
			VDSP_PRINTF("usr ivr index out of range, index = %d\n", ivrIndex);
			return FALSE;
		}
		StartAddrInFlash = ivrusr_startaddr;
		if (ivrCheckCRC(StartAddrInFlash, ivrusr_length)!=TRUE) {
			VDSP_PRINTF("ivrCheckCRC ivr usr failed\n");
			return FALSE;
		}
	}
	else 
	{
		return FALSE;
	}
	

	if (((VoiceHeader_t*)StartAddrInFlash)->label[ivrIndex].used == 0) {
		VDSP_PRINTF("ivrPlayPeriodicallyStart: element is empty. physical index = %d\n", ivrIndex);
		// TODO: beep? notice customer this element is empty.
		return FALSE;
	}
	
	dspChan_p->per.elementStartAddr = (uint32)StartAddrInFlash+ ((VoiceHeader_t*)StartAddrInFlash)->label[ivrIndex].offset_to_element;

	rtpHeader = malloc(RTP_FIXED_HEADER_LEN);
	zyMemset(rtpHeader, 0, RTP_FIXED_HEADER_LEN);
	zyMemcpy(rtpHeader,  (uint8*)(dspChan_p->per.elementStartAddr+4), RTP_FIXED_HEADER_LEN);

	VDSP_PRINTF("ivrPlayStart codec = %d\n", rtpHeader->pt);
	switch(rtpHeader->pt){
		case ZYNOS_CODEC_PCMU:
		case ZYNOS_CODEC_PCMA:
			dspChan_p->per.packetsize = IVR_G711_SIZE;				/*Fix rtp packetsize = 20ms*/
			dspChan_p->per.packettime = IVR_G711_TIME;				/*Fix rtp packetsize = 20ms*/
			break;
		case ZYNOS_CODEC_G723:
			dspChan_p->per.packetsize = IVR_G723_SIZE;
			dspChan_p->per.packettime = IVR_G723_TIME;				/*Fix rtp packetsize = 30ms*/
			break;
		case ZYNOS_CODEC_G726_16:
		case ZYNOS_CODEC_G726_24:
		case ZYNOS_CODEC_G726_32:
		case ZYNOS_CODEC_G726_40:	
			dspChan_p->per.packetsize = IVR_G726_SIZE;				/*Fix rtp packetsize = 30ms*/
			dspChan_p->per.packettime = IVR_G726_TIME;				/*Fix rtp packetsize = 20ms*/
			break;
		case ZYNOS_CODEC_G729:		
			dspChan_p->per.packetsize = IVR_G729_SIZE;				/*Fix rtp packetsize = 20ms*/
			dspChan_p->per.packettime = IVR_G729_TIME;				/*Fix rtp packetsize = 20ms*/
			break;
		default:
			VDSP_PRINTF("payload type not supported = %d\n", rtpHeader->pt);
			dspChan_p->per.elementStartAddr = NULL;			
			zyFree(rtpHeader);
			rtpHeader = NULL;
			return FALSE;
			break;
	}

	if(MOH_PHY_ID != dspChan_p->phPhysicalId){ /*skip check for MOH dspChan codec*/
		if(MM_SUCCESS == dspPoolModifyDspCodecRm(dspChan_p->dspId, dspChan_p->codec_num, rtpHeader->pt)){
			dspChan_p->codec_num = rtpHeader->pt;
			dspChan_p->silenceSupp = 0;
			dspChan_p->echoCancel = 1;	
			dspPoolAcquireMutex();
			dspDriverChanModify(dspChan_p, SENDRECV);
			dspPoolReleaseMutex();
		}
		else{
			dspChan_p->per.elementStartAddr = NULL;
			zyFree(rtpHeader);
			rtpHeader = NULL;
			VDSP_PRINTF("dsp resource not available for IVR codec = %d\n", rtpHeader->pt);
			return FALSE;
		}
	}
	
	VDSP_PRINTF("ivrPlayStart packetsize = %d, packettime= %d\n", dspChan_p->per.packetsize, dspChan_p->per.packettime);
	
	dspChan_p->per.packet = malloc(RTP_FIXED_HEADER_LEN + dspChan_p->per.packetsize);
	zyMemset(dspChan_p->per.packet, 0, RTP_FIXED_HEADER_LEN + dspChan_p->per.packetsize);
	zyMemcpy(dspChan_p->per.packet, rtpHeader, RTP_FIXED_HEADER_LEN);
	dspChan_p->per.numOfPackets= (((VoiceElement_t*)dspChan_p->per.elementStartAddr)->element_length - LEN_VOICE_ELEMENT_PRE)/dspChan_p->per.packetsize;

	dspChan_p->per.seqOfPacket = 0;		// from 0
	dspChan_p->per.Repeat = repeat;
	dspChan_p->per.Interval = timeInterval;
	dspChan_p->per.periodTimerId= timerPeriodicStartS( dspChan_p->per.packettime, ivrPlay,  dspChan_p->phLogicalId);
	

	VDSP_PRINTF("Register ivrPlay in ivrPlayStart, timerid=%d\n", dspChan_p->per.periodTimerId);
	VDSP_PRINTF("numOfPacket:%d\n", dspChan_p->per.numOfPackets);

	if (rtpHeader != NULL){
		zyFree(rtpHeader);
		rtpHeader = NULL;
        }
	return TRUE;
}

/*______________________________________________________________________________
**	ivrPlayStart
**	descriptions: stop play ivr to dsp channel
**	parameters:
**	local:
**	global:
**	return: TRUE/ FALSE
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

int ivrPlayStop(dspChan_t *dspChan_p){
	
	if(dspChan_p == NULL){
		return FALSE;
	}
	VDSP_PRINTF("Enter ivrPlayStop phone logicalId = %d\n", dspChan_p->phLogicalId);
	
	/* Stop the ivrPlay timer */
	if ( dspChan_p->per.periodTimerId != 0 ) {
		timerPeriodicStop(dspChan_p->per.periodTimerId);
		dspChan_p->per.periodTimerId = 0;
	}

	if ( dspChan_p->per.ReplayTimerId != 0 ) {
		timerPeriodicStop(dspChan_p->per.ReplayTimerId);
		dspChan_p->per.periodTimerId = 0;
	}

	dspChan_p->per.voipIvrCallBack = NULL;

	return TRUE;
}

/*______________________________________________________________________________
**	ivrListRecord
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void ivrListAllRecord(void) 
{
	int i;
	uint32	NumofElements;
	uint32	ElementPosition;
	rtp_header_t* rtpHeader = NULL;
	uint16 Packetsize;
	
	ZyPrintf("============================================================== \n");
	ZyPrintf("			 IVR SYS List						     \n");
	ZyPrintf("============================================================== \n");
	ZyPrintf("IVR SYS checksum = %x\n",((VoiceHeader_t*)ivrsys_startaddr)->crc);
	ZyPrintf("ElementID  Status Codec NumofRtpPackets  Size(Bytes) PositionOfElement\n");

	rtpHeader = malloc(RTP_FIXED_HEADER_LEN);
	for (i=0; i<NUM_IVRSYS; i++) {
		if (  ((VoiceHeader_t*)ivrsys_startaddr)->label[i].used ==0) {
			ZyPrintf("     %2d     empty   \n",i);
		} else {
			ElementPosition = ivrsys_startaddr+((VoiceHeader_t*)ivrsys_startaddr)->label[i].offset_to_element;
			memset(rtpHeader, 0, RTP_FIXED_HEADER_LEN);
			memcpy(rtpHeader,  (uint8*)(ElementPosition+4), RTP_FIXED_HEADER_LEN);
			switch(rtpHeader->pt){
				case ZYNOS_CODEC_PCMU:
				case ZYNOS_CODEC_PCMA:
					Packetsize = IVR_G711_SIZE;
					break;
				case ZYNOS_CODEC_G723:
					Packetsize = IVR_G723_SIZE;
					break;
				case ZYNOS_CODEC_G726_16:
				case ZYNOS_CODEC_G726_24:
				case ZYNOS_CODEC_G726_32:
				case ZYNOS_CODEC_G726_40:
					Packetsize = IVR_G726_SIZE;
					break;
				case ZYNOS_CODEC_G729:
					Packetsize = IVR_G729_SIZE;
					break;
			}
			NumofElements = (((VoiceElement_t*)ElementPosition)->element_length -LEN_VOICE_ELEMENT_PRE)/Packetsize;
			ZyPrintf("     %2d      used    %2d	%4d    	  %5d       %x	    \n",i, rtpHeader->pt, NumofElements, NumofElements*Packetsize+LEN_VOICE_ELEMENT_PRE,ElementPosition);
		}
	}
#ifndef N0_IVRUSR_PATH
	ZyPrintf("\n");
	ZyPrintf("============================================================== \n");
	ZyPrintf("			  IVR USR List						     \n");
	ZyPrintf("============================================================== \n");
	ZyPrintf("IVR USR checksum = %x\n",((VoiceHeader_t*)ivrusr_startaddr)->crc);
	ZyPrintf("ElementID  Status Codec NumofRtpPackets  Size(Bytes) PositionOfElement\n");

	for (i=0; i<NUM_IVRUSR; i++) {
		if (  ((VoiceHeader_t*)ivrusr_startaddr)->label[i].used ==0) {
			ZyPrintf("     %2d     empty   \n",i);
		} else {
			ElementPosition = ivrusr_startaddr+((VoiceHeader_t*)ivrusr_startaddr)->label[i].offset_to_element;
			memset(rtpHeader, 0, RTP_FIXED_HEADER_LEN);
			memcpy(rtpHeader,  (uint8*)(ElementPosition+4), RTP_FIXED_HEADER_LEN);
			switch(rtpHeader->pt){
				case ZYNOS_CODEC_PCMU:
				case ZYNOS_CODEC_PCMA:
					Packetsize = IVR_G711_SIZE;
					break;
				case ZYNOS_CODEC_G723:
					Packetsize = IVR_G723_SIZE;
					break;
				case ZYNOS_CODEC_G726_16:
				case ZYNOS_CODEC_G726_24:
				case ZYNOS_CODEC_G726_32:
				case ZYNOS_CODEC_G726_40:
					Packetsize = IVR_G726_SIZE;
					break;
				case ZYNOS_CODEC_G729:
					Packetsize = IVR_G729_SIZE;
					break;
			}
			NumofElements = (((VoiceElement_t*)ElementPosition)->element_length -LEN_VOICE_ELEMENT_PRE)/Packetsize;
			ZyPrintf("     %2d      used    %2d	%4d    	  %5d       %x	    \n",i, rtpHeader->pt, NumofElements, NumofElements*Packetsize+LEN_VOICE_ELEMENT_PRE,ElementPosition);
		}
	}
#endif
	free(rtpHeader);
}


