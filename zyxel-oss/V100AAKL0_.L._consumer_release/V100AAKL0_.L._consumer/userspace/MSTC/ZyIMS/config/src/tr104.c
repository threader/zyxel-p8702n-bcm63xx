/*
   $Id: TR104.c 0.00.0.1 2008/10/21 11:21 ctpan Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2003-2005 ZyXEL Communications, Corp.
 *	All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 *
 *************************************************************************/

/*
 * $Log: TR104.c $
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cms.h>
#include <cms_util.h>
#include <cms_core.h>
#include <cms_obj.h>

#include "global.h"
#include "tr104_global.h"
#include "tr104_object.h"
#include "tr104.h"
#include "tr104_init.h"
#include "icf_defs.h"

#include "switch.h"

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%		global Variable
%%----------------------------------------------------------------------------*/

/*Lock */
uint8  tr104LineStatsLockDone = 0;	// 1 : Locked , 0 : Release Lock 

/*Debug Switch*/
uint8  Tr104Debug = 0;

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%		data declaration
%%----------------------------------------------------------------------------*/
//Ssu-Ying, support Multiple Profile
VoiceServiceObj_t	*tr104VoiceService;	/*TR104 Profile*/
extern 	ZyXEL_VoiceSipAccount_t *ZyXEL_VoiceSipAccount;	/*TR104 Profile*/
extern	ZyXEL_VoicePSTN_t	*VoicePSTN;			/* VOIP PSTN */
extern	ZyXEL_VoicePhoneBook_t	*VoicePhoneBook;	/* VOIP PhoneBook */

/*______________________________________________________________________________
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%		function declaration
%%----------------------------------------------------------------------------*/

#define TR104_VPROFILE_PTR(idx)		((VoiceProfObj_t*) (tr104VoiceService->voiceProfObjArray)+idx)
//Ssu-Ying, support Multiple Profile
#define TR104_VLINE_PTR(VpItr , idx) 				((VoiceProfLineObj_t*) (tr104VoiceService->voiceProfObjArray[VpItr].lineProfPtrLst)+idx)
#define TR104_VPHYINTF_PTR(idx)					((VoicePhyIntfObj_t*) (tr104VoiceService->phyInterface) +idx)
#define TR104_VSIP_ACCOUNT_PTR(idx)			((ZyXEL_VoiceSipAccount_t*) ZyXEL_VoiceSipAccount +idx)
#define TR104_VSIP_ACCOUNT_VPROFILE_PTR(idx)	((ZyXEL_VoiceSipAccount_t*) (ZyXEL_VoiceSipAccount->ptrProf)+idx)
#define TR104_VSIP_ACCOUNT_VLINE_PTR(idx)		((ZyXEL_VoiceSipAccount_t*) (ZyXEL_VoiceSipAccount->ptrLine) +idx)
#define TR104_VFXO_PTR(idx)						((ZyXEL_VoiceFXO_t*) (tr104VoiceService->ZyXEL_VoiceFXO) +idx)
#define TR104_VPSTN_PTR(idx)					((ZyXEL_VoicePSTN_t*) tr104VoiceService->ZyXEL_VoicePSTN+idx)
#define TR104_VPHONE_BOOK_PTR(idx)				((ZyXEL_VoicePhoneBook_t *) tr104VoiceService->ZyXEL_VoicePhoneBook+idx)	

extern uint32 sipAccountNumber;

//Ssu-Ying, support Multiple Profile
/*______________________________________________________________________________
**  name: tr104ProfileIndexCheck
**  descriptions: check the voice profile Index before geting the profile Object .
**  parameters: VpItr
**  return: TR104_SUCCESS : check ok , TR104_ERROR : check error
**  called by:
**  call:
**____________________________________________________________________________*/
int 
tr104ProfileIndexCheck(int VpItr)
{
	int ret = TR104_ERROR ;
	int minValue = 0 , maxValue = 0 ;
	if(tr104VoiceService != NULL){
		if(tr104VoiceService->voiceCapabilities != NULL){
			minValue = 0;
			maxValue = tr104VoiceService->voiceProfileNumberOfEntries;
			if( ( VpItr >= minValue ) && ( VpItr < maxValue)) {	
				ret = TR104_SUCCESS ; 
			}
		}
	}
	return ret; 
}
/*______________________________________________________________________________
**  name: tr104LineIndexCheck
**  descriptions: check the Line Index before geting the Line Object .
**  parameters: LineItr
**  return: TR104_SUCCESS : check ok , TR104_ERROR : check error
**  called by:
**  call:
**____________________________________________________________________________*/
int 
tr104LineIndexCheck(int VpItr, int LineItr)
{
	int ret = TR104_ERROR ;
	int ProfminValue = 0 , ProfmaxValue = 0 , LinefminValue = 0 , LinefmaxValue = 0 ;	
	if(tr104VoiceService != NULL){
		ProfminValue = 0;
		ProfmaxValue = tr104VoiceService->voiceProfileNumberOfEntries;
		if((VpItr >= ProfminValue) && (VpItr < ProfmaxValue)) {
			LinefminValue = 0;
			LinefmaxValue = tr104GetVPNumOfLine(VpItr);
			if( ( LineItr >= LinefminValue ) && ( LineItr < LinefmaxValue)) {	
				ret = TR104_SUCCESS ; 
			}
		}
	}
	return ret; 
}

/*______________________________________________________________________________
**  name: tr104AcquireStatsLock
**  descriptions: Must set lock before using the Statistic object .
**  parameters: void
**  return: 
**  called by:
**  call:
**____________________________________________________________________________*/
int 
tr104AcquireStatsLock(void)
{
	return dirps();
}

/*______________________________________________________________________________
**  name: tr104ReleaseStatsLock
**  descriptions: Must release lock after using the Statistic object .
**  parameters: void
**  return: 
**  called by:
**  call:
**____________________________________________________________________________*/
void 
tr104ReleaseStatsLock(int x)
{
	restore();
}

/*______________________________________________________________________________
**  name: tr104LstElmNew
**  descriptions: new an init element
**  parameters: void
**  return: a tr104LstElm_t
**  called by:
**  call:
**____________________________________________________________________________*/
tr104LstElm_t*	
tr104LstElmNew(void)
{
	tr104LstElm_t* e = (tr104LstElm_t*) zyMalloc(sizeof(tr104LstElm_t));

	e->value_ = NULL;
	e->next_ = NULL;
	e->pre_ = NULL;

	return e;
}
/*______________________________________________________________________________
**  name: tr104LstElmFree
**  descriptions: free th appointed elem
**  parameters: (IN) tr104LstElm_t
**  return: void
**  called by:
**  call:
**____________________________________________________________________________*/
void
tr104LstElmFree(tr104LstElm_t* e)
{
	zyFree(e);
}

/*______________________________________________________________________________
**  name: tr104ObjLstNew
**  descriptions: new an init List
**  parameters: void
**  return: tr104ObjLst_t
**  called by:
**  call:
**____________________________________________________________________________*/
tr104ObjLst_t* 
tr104ObjLstNew(void)
{
	tr104ObjLst_t* _this = (tr104ObjLst_t*)zyMalloc(sizeof(tr104ObjLst_t));

	if(!_this)
		return NULL;
	_this->head_ = NULL;
	_this->tail_ = NULL;
	_this->size_ = 0;

	return _this;
}
/*______________________________________________________________________________
**  name: tr104LstAllElmClear
**  descriptions: free all the elems of this object list
**  parameters: (IN) tr104ObjLst_t
**  return: void
**  called by:
**  call:
**____________________________________________________________________________*/
void 
tr104LstAllElmClear(tr104ObjLst_t* _this )
{
	
	tr104LstElm_t* Iterator = ((_this)?_this->head_:NULL);
	tr104LstElm_t* temp = NULL;
	
	if ( !_this ){
		TR104_DBG("<tr104ObjLstClear>: List is NULL.");
		return ;
	}
	while ( Iterator ) {
		temp = Iterator;
		Iterator = Iterator->next_;
		tr104LstElmFree(temp);
	}
	/* list will be reused , so we need to set the list to init status.*/
	_this->head_ = NULL;
	_this->tail_=NULL;
	_this->size_=0;
	
}
/*______________________________________________________________________________
**  name: tr104LstFree
**  descriptions: free th appointed list
**  parameters: (IN) tr104ObjLst_t
**  return: void
**  called by:
**  call:
**____________________________________________________________________________*/
void    
tr104LstFree(tr104ObjLst_t* _this)
{
	if( !_this ){
		return;
	}
	tr104LstAllElmClear(_this);	
	if(_this){
		zyFree(_this);
	}
}	

/*______________________________________________________________________________
**  name: tr104LstAddElmTail   
**  descriptions: put element at tail of list
**  parameters: (IN) tr104ObjLst_t , (IN) value
**  return: TR104_SUCCESS , TR104_ERROR
**  called by:
**  call:
**____________________________________________________________________________*/
int 
tr104LstAddElmTail(tr104ObjLst_t* _this , void* value) 
{
	tr104LstElm_t* e = NULL;
	
	if ( (e=tr104LstElmNew()) == NULL ) {
		TR104_DBG("<tr104ObjLstAddElmTail>: lstElmNew error.");
		return TR104_ERROR;
	}
	e->value_ = value;

	if ( _this->size_==0 ) {
		_this->head_ = e;
		_this->tail_ = e;
		_this->size_++;
	}else{
		e->pre_ = _this->tail_;
		_this->tail_->next_ = e;
		_this->tail_ = e;
		_this->size_++;
	}
	return TR104_SUCCESS;
}



/*______________________________________________________________________________
**  name: tr104LstAddElmHead   
**  descriptions: put element at head of list
**  parameters: (IN) tr104ObjLst_t , (IN) value
**  return: TR104_SUCCESS , TR104_ERROR
**  called by:
**  call:
**____________________________________________________________________________*/
int 
tr104LstAddElmHead(tr104ObjLst_t* _this , void* value)
{
	tr104LstElm_t* e = NULL;
	
	if ( (e=tr104LstElmNew()) == NULL ) {
		TR104_DBG("<tr104ObjLstAddElmTail>: lstElmNew error.");
		return TR104_ERROR;
	}

	e->value_ = value;

	if ( _this->size_==0 ) {
		_this->head_ = e;
		_this->tail_ = e;
		_this->size_++;
	}
	else {
		e->next_ = _this->head_;
		_this->head_->pre_ = e;
		_this->head_ = e;
		_this->size_++;
	}
	return TR104_SUCCESS;
}

/*______________________________________________________________________________
**  name: tr104LstInsert   
**  descriptions: Insert element at the appointed position of the list
**  parameters: (IN) tr104ObjLst_t , (IN) value , (IN) pos
**  return: 	TR104_SUCCESS , 
**			r_pos :the real position that the elem insert to .
**  called by:
**  call:
**____________________________________________________________________________*/

int   
tr104LstInsert(tr104ObjLst_t* _this, void* value, int pos) /*reserve*/
{
	tr104LstElm_t* e = NULL;
	tr104LstElm_t* iter = NULL;
	int    r_pos;

	if( !_this || pos<0 ) {
		return TR104_ERROR;
	}
	if( (e=tr104LstElmNew()) == NULL ) {
		TR104_DBG("<tr104LstInsert>: lstElmNew error.");
		return TR104_ERROR;
	}
	e->value_=value;
	if ( _this->size_==0 ) {
		_this->head_ = e;
		_this->tail_ = e;
		_this->size_++;
		r_pos = 0;
	} else if( pos>=_this->size_ ) { /* put element at tail of list*/
		e->pre_ = _this->tail_;
		_this->tail_->next_ = e;
		_this->tail_ = e;
		_this->size_++;
		r_pos = _this->size_-1;
	} else if( pos<=0 ) { /* put element at head of list*/
		e->next_ = _this->head_;
		_this->head_->pre_ = e;
		_this->head_ = e;
		_this->size_++;

		r_pos = 0;
	} else { /* insert element at $pos of list*/
		for(r_pos=0,iter=_this->head_ ; (r_pos<pos)&&(iter!=NULL) ; r_pos++,iter=iter->next_) {
			iter->pre_->next_ = e;
			e->pre_ = iter->pre_;
			e->next_ = iter;
			iter->pre_ = e;

			_this->size_++;
		}
	}

	return r_pos;
}
/*______________________________________________________________________________
**  name: tr104LstPeek   
**  descriptions: peek the value of the element in the index pos of this list
**  parameters: (IN) tr104ObjLst_t , (IN) pos
**  return: 	element
**  called by:
**  call:
**____________________________________________________________________________*/
/*get element by position*/
void*   
tr104LstPeek(tr104ObjLst_t* _this , int pos)
{
	tr104LstElm_t* Iter = ((_this)?_this->head_:NULL);
	int i = 0;

	if ( _this==NULL || pos>=_this->size_ || pos<0 )
		return NULL;

	while (i<pos)
	{
		Iter = Iter->next_;
		i++;
	}
	return Iter->value_;
}

int  
tr104ObjLstGetSize(tr104ObjLst_t* _this)
{
	return (_this)?_this->size_:TR104_ERROR;
}


/********************************************************************
**  
**
**  APIs class: the Object Ptr Get
**  descriptions:  root Object
**
*********************************************************************/

/***************************************************************************
** Function Name: tr104VoiceServiceObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceServiceObj_t* 
tr104VoiceServiceObjPtrGet(void)
{
	return tr104VoiceService;
}
//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104VoicePSTNPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
ZyXEL_VoicePSTN_t* 
tr104VoicePSTNPtrGet(int PSTNIdx) 
{
	/* "TR104_VPHONE_BOOK_CNT" is local setting , need modify when useing the PSTN object*/
	if( ( PSTNIdx < 0 ) || ( PSTNIdx >= TR104_VPSTN_CNT)) {
		ZyPrintf("please check maxValue : TR104_VPSTN_CNT \n\r");
		ZyPrintf("tr104VoicePSTNPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
	return  TR104_VPSTN_PTR(PSTNIdx);
}

/***************************************************************************
** Function Name: tr104VoicePhoneBookPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
ZyXEL_VoicePhoneBook_t* 
tr104VoicePhoneBookPtrGet(int PhoneBookIdx)
{	
	/* "TR104_VPHONE_BOOK_CNT" is local setting , need modify when useing the PhoneBook object*/
	if ( ( PhoneBookIdx < 0 ) || ( PhoneBookIdx >= TR104_VPHONE_BOOK_CNT)) {
		ZyPrintf("please check maxValue : TR104_VPHONE_BOOK_CNT \n\r");
		ZyPrintf("tr104VoicePhoneBookPtrGet : error : can't get the object pointer \r\n");		
		return NULL;
	}
	return TR104_VPHONE_BOOK_PTR(PhoneBookIdx);
}
/***************************************************************************
** Function Name: tr104VoiceCommonObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/

ZyXEL_VoiceCommon_t* 
tr104VoiceCommonObjPtrGet(void) 
{
	return (tr104VoiceService->ZyXEL_voiceCommon);
}

/*______________________________________________________________________________
**  
**
**  APIs class: the Object Ptr Get
**  descriptions:   tree : VoiceCap
**
**______________________________________________________________________________*/
/***************************************************************************
** Function Name: tr104VoiceCapObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceCapObj_t* 
tr104VoiceCapObjPtrGet(void) 
{
	return (tr104VoiceService->voiceCapabilities);
}
/***************************************************************************
** Function Name: tr104VoiceCapSipObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceCapSipObj_t* 
tr104VoiceCapSipObjPtrGet(void) 
{
	return (tr104VoiceService->voiceCapabilities->CapOfSIP);
}
/*______________________________________________________________________________
**  tr104VoiceCapCodecsTabPtrGet
**  Full Path : tr104VoiceService->voiceProfObjArray.{i}.VoiceCapCodec
**  descriptions:
**  parameters: void
**  return: CapCodecsTable
**  called by:
**  call:
**____________________________________________________________________________*/

tr104ObjLst_t* 
tr104VoiceCapCodecsTabPtrGet(void) 
{
	return (tr104VoiceService->voiceCapabilities->CapCodecsTable);
}
/***************************************************************************
** Function Name: tr104GetCapCodecByEntryID
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceCapCodecObj_t* 
tr104GetCapCodecByEntryID(uint32 entryId) 
{
	int i=0;
	tr104ObjLst_t* capCodecsTab = tr104VoiceCapCodecsTabPtrGet() ;
	tr104LstElm_t* iter=(capCodecsTab)?capCodecsTab->head_:NULL;

	VoiceCapCodecObj_t* codecObj=NULL;
	if(capCodecsTab==NULL){
		ZyPrintf(" Error : List is NULL\n ");
		return NULL;
	}
	
	for(i=0;i<capCodecsTab->size_;i++){
		codecObj = iter->value_;
		if(codecObj->entryID == entryId){
			TR104_DBG("find match codec");
			return codecObj;
		}
		iter=iter->next_;
	}
	ZyPrintf("No Match Codec : ID= %d \n",entryId);
	return NULL;	
}


/*______________________________________________________________________________
**  
**
**  APIs class: the Object Ptr Get
**  descriptions:   tree : VoiceProf
**
**______________________________________________________________________________*/
//Ssu-Ying, support Multiple Profile
/***************************************************************************
** Function Name: tr104VoiceProfObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      : 
****************************************************************************/
VoiceProfObj_t* 
tr104VoiceProfObjPtrGet(int VpItr) 
{
	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		return TR104_VPROFILE_PTR(VpItr);
	}else{
		ZyPrintf("tr104VoiceProfObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
	
}
/***************************************************************************
** Function Name: tr104VoiceProfFaxT38ObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfFaxT38Obj_t* 
tr104VoiceProfFaxT38ObjPtrGet( int VpItr ) 
{
	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		return (tr104VoiceService->voiceProfObjArray[VpItr].faxT38Obj);
	}else{
		ZyPrintf("tr104VoiceProfFaxT38ObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceProfSipObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfSipObj_t* 
tr104VoiceProfSipObjPtrGet( int VpItr ) 
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		return (tr104VoiceService->voiceProfObjArray[VpItr].SIPObj);
	}else{
		ZyPrintf("tr104VoiceProfSipObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceProfSipEventSubTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceProfSipEventSubTabPtrGet( int VpItr ) 
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfSipObj_t* parent = tr104VoiceProfSipObjPtrGet(VpItr);
		return (parent->eventsubScribeTab);
	}else{
		ZyPrintf("tr104VoiceProfSipEventSubTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
	
}
/***************************************************************************
** Function Name: tr104GetProfSipEventSubObjByEventName
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfSipEventSubObj_t* 
tr104GetProfSipEventSubObjByEventName(int VpItr , char* eventName) 
{
	int i=0;
	tr104ObjLst_t* sipEventSubTab = tr104VoiceProfSipEventSubTabPtrGet(VpItr) ;
	tr104LstElm_t* iter=(sipEventSubTab)?sipEventSubTab->head_:NULL;

	VoiceProfSipEventSubObj_t* eventSubObj=NULL;
	if(sipEventSubTab==NULL){
		ZyPrintf(" Error : List is NULL\n ");
		return NULL;
	}
	
	for(i=0;i<sipEventSubTab->size_;i++){
		eventSubObj = iter->value_;
		if(zyStrcmp(eventSubObj->event,eventName)==0){
			TR104_DBG("find match event");
			return eventSubObj;
		}
		iter=iter->next_;
	}
	ZyPrintf("No Match event : name = %s \n",eventName);
	return NULL;	
}
/***************************************************************************
** Function Name: tr104VoiceProfSipRespMapTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceProfSipRespMapTabPtrGet( int VpItr )
{
	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfSipObj_t* parent = tr104VoiceProfSipObjPtrGet(VpItr);
		return (parent->responseMapTab);
	}else{
		ZyPrintf("tr104VoiceProfSipRespMapTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104GetProfSipRespMapObjByRespNum
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfSipRespMapObj_t* 
tr104GetProfSipRespMapObjByRespNum(int VpItr ,  uint32 respNum)
{
	int i=0;
	tr104ObjLst_t* sipRespMapTab = tr104VoiceProfSipRespMapTabPtrGet(VpItr) ;
	tr104LstElm_t* iter=(sipRespMapTab)?sipRespMapTab->head_:NULL;

	VoiceProfSipRespMapObj_t* respMapObj=NULL;
	if(sipRespMapTab==NULL){
		ZyPrintf(" Error : List is NULL\n ");
		return NULL;
	}
	
	for(i=0;i<sipRespMapTab->size_;i++){
		respMapObj = iter->value_;
		if(respMapObj->SIPResponseNumber==respNum){
			TR104_DBG("find match Resp Num");
			return respMapObj;
		}
		iter=iter->next_;
	}
	ZyPrintf("No Match Resp Num : respNum= %d \n",respNum);
	return NULL;	
}
/***************************************************************************
** Function Name: tr104VoiceProfToneObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfToneObj_t* 
tr104VoiceProfToneObjPtrGet( int VpItr )
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		return (tr104VoiceService->voiceProfObjArray[VpItr].toneObj);
	}else{
		ZyPrintf("tr104VoiceProfToneObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceServiceObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceProfToneEventTabPtrGet( int VpItr ) 
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfToneObj_t* parent = tr104VoiceProfToneObjPtrGet(VpItr);
		return (parent->toneEventTab);
	}else{
		ZyPrintf("tr104VoiceProfToneEventTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104GetProfToneEventObjByFuncName
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfToneEventObj_t* 
tr104GetProfToneEventObjByFuncName( int VpItr , uint8 FuncName) 
{
	int i=0;
	tr104ObjLst_t* toneEventTab = tr104VoiceProfToneEventTabPtrGet(VpItr) ;
	tr104LstElm_t* iter=(toneEventTab)?toneEventTab->head_:NULL;

	VoiceProfToneEventObj_t* eventObj=NULL;
	if(toneEventTab==NULL){
		ZyPrintf(" Error : List is NULL\n ");
		return NULL;
	}
	
	for(i=0;i<toneEventTab->size_;i++){
		eventObj = iter->value_;
		if(eventObj->function == FuncName){
			TR104_DBG("find match FuncName");
			return eventObj;
		}
		iter=iter->next_;
	}
	ZyPrintf("No Match Resp Num : FuncName= %d \n",FuncName);
	return NULL;	
}

/***************************************************************************
** Function Name: tr104VoiceProfToneDescriptionTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceProfToneDescriptionTabPtrGet(int VpItr) 
{
	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfToneObj_t* parent = tr104VoiceProfToneObjPtrGet(VpItr);
		return (parent->toneDescriptionTab);
	}else{
		ZyPrintf("tr104VoiceProfToneDescriptionTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}

/***************************************************************************
** Function Name: tr104VoiceProfTonePatternTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceProfTonePatternTabPtrGet(int VpItr) 
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfToneObj_t* parent = tr104VoiceProfToneObjPtrGet(VpItr);
		return (parent->tonePatternTab);
	}else{
		ZyPrintf("tr104VoiceProfTonePatternTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}

}

/***************************************************************************
** Function Name: tr104VoiceProfNumPlanObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfNumPlanObj_t* 
tr104VoiceProfNumPlanObjPtrGet(int VpItr)
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		return (tr104VoiceService->voiceProfObjArray[VpItr].numberingPlanObj);
	}else{
		ZyPrintf("tr104VoiceProfNumPlanObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceProfNumPlanPrefixInfoTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceProfNumPlanPrefixInfoTabPtrGet(int VpItr) 
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfNumPlanObj_t* parent = tr104VoiceProfNumPlanObjPtrGet(VpItr);
		return (parent->PrefixInfoTab);
	}else{
		ZyPrintf("tr104VoiceProfNumPlanPrefixInfoTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}

/*______________________________________________________________________________
**  
**
**  APIs class: the Object Ptr Get
**  descriptions:   tree : VoiceProfRTP
**
**____________________________________________________________________________*/
/***************************************************************************
** Function Name: tr104VoiceProfRtpObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfRtpObj_t* 
tr104VoiceProfRtpObjPtrGet(int VpItr)
{
	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		return (tr104VoiceService->voiceProfObjArray[VpItr].RTP_Profile);
	}else{
		ZyPrintf("tr104VoiceProfRtpObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceProfRtpRtcpObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfRtpRtcpObj_t* 
tr104VoiceProfRtpRtcpObjPtrGet(int VpItr) 
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfRtpObj_t* parent =  (tr104VoiceService->voiceProfObjArray[VpItr].RTP_Profile);
		return parent->RTCP_Profile;
	}else{
		ZyPrintf("tr104VoiceProfRtpRtcpObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceProfRtpSrtpObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfRtpSrtpObj_t* 
tr104VoiceProfRtpSrtpObjPtrGet(int VpItr)
{
	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfRtpObj_t* parent =  (tr104VoiceService->voiceProfObjArray[VpItr].RTP_Profile);
		return parent->SRTP_Profile;
	}else{
		ZyPrintf("tr104VoiceProfRtpSrtpObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceProfRtpRedundancyObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfRtpRedundancyObj_t* 
tr104VoiceProfRtpRedundancyObjPtrGet(int VpItr)
{

	if(TR104_SUCCESS == tr104ProfileIndexCheck(VpItr)){
		VoiceProfRtpObj_t* parent =  (tr104VoiceService->voiceProfObjArray[VpItr].RTP_Profile);
		return parent->RTPRedundancy;
	}else{
		ZyPrintf("tr104VoiceProfRtpRedundancyObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}



/*______________________________________________________________________________
**  
**
**  APIs class: the Object Ptr Get
**  descriptions:   tree : VoiceLine
**
**______________________________________________________________________________*/

/***************************************************************************
** Function Name: tr104VoiceLineObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfLineObj_t* 
tr104VoiceLineObjPtrGet(int VpItr, int LineItr)
{
	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//return TR104_VLINE_PTR(VpItr , LineItr);
		VoiceProfObj_t* pProf = tr104VoiceProfObjPtrGet(VpItr);
		return (pProf->lineProfPtrLst) + LineItr;	 
	}else{
		TR104_DBG("tr104VoiceLineObjPtrGet : error : can't get the object pointer\r\n");
		return NULL;
	}
	
}

/***************************************************************************
** Function Name: tr104VoiceLineSipObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfLineSipObj_t* 
tr104VoiceLineSipObjPtrGet(int VpItr, int LineItr) 
{
	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//VoiceProfLineObj_t* parent = TR104_VLINE_PTR(VpItr , LineItr);
		//return parent->line_sip;
		VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		return pLine->line_sip;		
	}else{
		ZyPrintf("tr104VoiceLineSipObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}

/***************************************************************************
** Function Name: tr104VoiceLineSipEventSubTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceLineSipEventSubTabPtrGet(int VpItr, int LineItr)
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		VoiceProfLineSipObj_t* parent = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);
		return parent->sipEventSubTab;
	}else{
		ZyPrintf("tr104VoiceLineSipEventSubTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
	
}

/***************************************************************************
** Function Name: tr104VoiceLineProcessingObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfLineVoiceProcessingObj_t* 
tr104VoiceLineProcessingObjPtrGet(int VpItr, int LineItr)
{
	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//VoiceProfLineObj_t* parent = TR104_VLINE_PTR(VpItr , LineItr);
		//return parent->voiceProcessing;
		VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		return pLine->voiceProcessing;
	}else{
		ZyPrintf("tr104VoiceLineProcessingObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}

/***************************************************************************
** Function Name: tr104VoiceLineStatsObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfLineStatsObj_t* 
tr104VoiceLineStatsObjPtrGet(int VpItr, int LineItr)
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//VoiceProfLineObj_t* parent = TR104_VLINE_PTR(VpItr , LineItr);
		//return parent->lineStatistics;
		VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		return pLine->lineStatistics;
	}else{
		ZyPrintf("tr104VoiceLineStatsObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}

}

/***************************************************************************
** Function Name: tr104VoiceLineCallingFeatureObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfLineCallingFeaturesObj_t* 
tr104VoiceLineCallingFeatureObjPtrGet(int VpItr, int LineItr) 
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//VoiceProfLineObj_t* parent = TR104_VLINE_PTR(VpItr , LineItr);
		//return parent->callingFeatures;
		VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		return pLine->callingFeatures;
	}else{
		ZyPrintf("tr104VoiceLineCallingFeatureObjPtrGet : error : can't get the object pointer for VpItr , LineItr %d , %d\r\n", VpItr , LineItr);
		return NULL;
	}
}

/***************************************************************************
** Function Name: tr104VoiceLineCallingFeatureObjPtrSet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
int 
tr104VoiceLineCallingFeatureObjPtrSet(int VpItr, int LineItr ,	VoiceProfLineCallingFeaturesObj_t *lineCallingFeaturesObj ) 
{
	int ret=0;	
	/* save the VoiceProfLineCallingFeaturesObj_t to FLASH */

	ret = tr104StoreMdmVoiceLineCallingFeaturesCfg(VpItr+1, LineItr+1, lineCallingFeaturesObj);
	return ret;
}

/***************************************************************************
** Function Name: tr104VoicePhyIntfObjPtrSet
**
** Description  : the Object Ptr Set
**                           
** Returns      :  
****************************************************************************/
int 
tr104VoicePhyIntfObjPtrSet(int interfaceId ,	VoicePhyIntfObj_t* phyIntfObj ) 
{
	int ret=0;	
	/* save the VoiceProfLineCallingFeaturesObj_t to FLASH */

	ret = tr104StoreMdmVoicePhyIntfObj(interfaceId+1, phyIntfObj);
	return ret;
}

/*______________________________________________________________________________
**  name: tr104VoiceLineCodecObjPtrGet   
**  descriptions: Get the object ptr which  indicates the state of the transmit and
**  				receive codec for this voice line instance
**  parameters: (IN) LineItr 
**  return: VoiceProfLineCodecObj_t*
**  called by: tr104VoiceLineCodecTabPtrGet (internal use)
**  call:
**____________________________________________________________________________*/

VoiceProfLineCodecObj_t* 
tr104VoiceLineCodecObjPtrGet(int VpItr, int LineItr) 
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//VoiceProfLineObj_t* parent = TR104_VLINE_PTR(VpItr , LineItr);
		//return parent->lineCodec;
		VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		return pLine->lineCodec;
	}else{
		ZyPrintf("tr104VoiceLineCodecObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/*______________________________________________________________________________
**  name: tr104VoiceLineCodecTabPtrGet   
**  descriptions: Get the codec List of the line(LineItr) 
**  parameters: (IN) LineItr 
**  return: the Line Codec Table Ptr (Codec Table ptr)
**  called by: tr104GetLineCodecsByEntryID (internal use)
**  call:
**____________________________________________________________________________*/
tr104ObjLst_t* 
tr104VoiceLineCodecTabPtrGet(int VpItr, int LineItr) 
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		VoiceProfLineCodecObj_t* parent=tr104VoiceLineCodecObjPtrGet(VpItr, LineItr);
		return parent->codecList;
	}else{
		ZyPrintf("tr104VoiceLineCodecTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/*______________________________________________________________________________
**  name: tr104GetLineCodecByEntryID   
**  descriptions: Get the codec by EntryID from the line(LineItr) Codec list.
**  parameters: (IN) LineItr , (IN) entryId 
**  return: the Codec ptr of the line
**  called by:
**  call:
**____________________________________________________________________________*/

VoiceProfLineCodecListObj_t* 
tr104GetLineCodecByEntryID(int VpItr, int LineItr , uint32 entryId) 
{
	int i=0;
	tr104ObjLst_t* lineCodecsTab = tr104VoiceLineCodecTabPtrGet(VpItr, LineItr) ;
	tr104LstElm_t* iter=(lineCodecsTab)?lineCodecsTab->head_:NULL;

	VoiceProfLineCodecListObj_t* codecObj=NULL;
	if(lineCodecsTab==NULL){
		ZyPrintf(" Error : List is NULL\n ");
		return NULL;
	}
	
	for(i=0;i<lineCodecsTab->size_;i++){
		codecObj = iter->value_;
		if(codecObj->entryID == entryId){
			TR104_DBGS1("find match codec , CODEC: ",codecObj->codec);
			return codecObj;
		}
		iter=iter->next_;
	}
	ZyPrintf("No Match Codec : ID= %d \n",entryId);
	return NULL;	
}

/*______________________________________________________________________________
**  name: tr104GetLineCodecByCodecName   
**  descriptions: Get the codec by codec from the line(LineItr) Codec list.
**  parameters: (IN) LineItr , (IN) entryId 
**  return: the Codec ptr of the line
**  called by:
**  call:
**____________________________________________________________________________*/

VoiceProfLineCodecListObj_t* 
tr104GetLineCodecByCodecName(int VpItr, int LineItr , uint8 *codec) 
{
	int i=0;
	tr104ObjLst_t* lineCodecsTab = tr104VoiceLineCodecTabPtrGet(VpItr, LineItr) ;
	tr104LstElm_t* iter=(lineCodecsTab)?lineCodecsTab->head_:NULL;

	VoiceProfLineCodecListObj_t* codecObj=NULL;
	if(lineCodecsTab==NULL){
		ZyPrintf(" Error : List is NULL\n ");
		return NULL;
	}
	
	for(i=0;i<lineCodecsTab->size_;i++){
		codecObj = iter->value_;
		if( !zyStrcmp(codecObj->codec, codec)){
			TR104_DBGS1("find match codec , CODEC: ",codecObj->codec);
			return codecObj;
		}
		iter=iter->next_;
	}

	return NULL;	
}

/*______________________________________________________________________________
**  name: tr104GetLineCodecSilenceSuppression   
**  descriptions: Get the first codec silence suppression from the line(LineItr) Codec list.
**  parameters: (IN) LineItr
**  return: the Silence Suppression of the line (first codec), 0 off, 1 on
**  called by:
**  call:
**____________________________________________________________________________*/

uint8
tr104GetLineCodecSilenceSuppression(int VpItr, int LineItr) 
{
	int i=0;
	tr104ObjLst_t* lineCodecsTab = tr104VoiceLineCodecTabPtrGet(VpItr, LineItr) ;
	VoiceProfLineCodecListObj_t* codecObj=NULL;
	
	if(lineCodecsTab != NULL){
		if(NULL != lineCodecsTab->head_){
			codecObj = lineCodecsTab->head_->value_;
			return codecObj->silenceSuppression;
		}else{
			ZyPrintf(" tr104GetLineCodecSilenceSuppression : Error : head is NULL\n ");
			return TR104_ERROR ;
		}
	}else{
		ZyPrintf("tr104GetLineCodecSilenceSuppression : Error : List is NULL\n ");
		return TR104_ERROR ;
	}	

}

/***************************************************************************
** Function Name: tr104VoiceLineRingerObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
VoiceProfLineRingerObj_t* 
tr104VoiceLineRingerObjPtrGet(int VpItr, int LineItr) 
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		//VoiceProfLineObj_t* parent = TR104_VLINE_PTR(VpItr , LineItr);
		//return parent->ringer;
		VoiceProfLineObj_t* pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		return pLine->ringer;
	}else{
		ZyPrintf("tr104VoiceLineRingerObjPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceLineRingerEventTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceLineRingerEventTabPtrGet(int VpItr, int LineItr) 
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		VoiceProfLineRingerObj_t* parent = tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);
		return parent->ringerEventTab;
	}else{
		ZyPrintf("tr104VoiceLineRingerEventTabPtrGet : error : can't get the object pointer \r\n");
		return NULL;
	}
}
/***************************************************************************
** Function Name: tr104VoiceLineRingerDescriptionTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      :  
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceLineRingerDescriptionTabPtrGet(int VpItr, int LineItr)
{
	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		VoiceProfLineRingerObj_t* parent = tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);
		return parent->ringerDescriptionTab;
	}else{
		ZyPrintf("tr104VoiceLineRingerDescriptionTabPtrGet : error : can't get the object pointer \r\n");
		return NULL ;
	}
}
/***************************************************************************
** Function Name: tr104VoiceLineRingerPatternTabPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      : 
****************************************************************************/
tr104ObjLst_t* 
tr104VoiceLineRingerPatternTabPtrGet(int VpItr, int LineItr) 
{

	if(TR104_SUCCESS == tr104LineIndexCheck(VpItr, LineItr)){
		VoiceProfLineRingerObj_t* parent = tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);
		return parent->ringerPatternTab;
	}else{
		ZyPrintf("tr104VoiceLineRingerPatternTabPtrGet : error : can't get the object pointer \r\n");
		return NULL ;
	}
}
/*______________________________________________________________________________
**  
**
**  APIs class: the Object Ptr Get
**  descriptions:   tree : VoicePhyIntfMgn
**
**______________________________________________________________________________*/

/***************************************************************************
** Function Name: tr104VoicePhyIntfObjPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      : 
****************************************************************************/
VoicePhyIntfObj_t* 
tr104VoicePhyIntfObjPtrGet(int phyIntfIdx) 
{
	int ret = TR104_ERROR ;
	int minValue = 0 , maxValue = 0;

	TR104_DBG(" tr104VoicePhyIntfObjPtrGet > ENTER");
	
	if(tr104VoiceService != NULL){
		if(tr104VoiceService->voiceCapabilities != NULL){
			minValue = 0;		
			maxValue = NUM_FXS_CHANNELS;
			if( ( phyIntfIdx >= minValue ) && ( phyIntfIdx < maxValue)) {
				TR104_DBG("phyIntfIdx < maxValue && phyIntfIdx >=minValue");
				ret = TR104_SUCCESS ; 
			}
		}
	}
	if(ret == TR104_SUCCESS){
		return TR104_VPHYINTF_PTR(phyIntfIdx);
	}else{
		ZyPrintf("tr104VoicePhyIntfObjPtrGet : error : can't get the object pointer \r\n");
		return NULL ;
	}

}

/***************************************************************************
** Function Name: tr104VoiceFXOPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      : 
****************************************************************************/
ZyXEL_VoiceFXO_t*
tr104VoiceFXOPtrGet(int fxoIndex )/*index of FXO*/
{
	int ret = TR104_ERROR ;
	int minValue = 0 , maxValue = 0; 
	if(tr104VoiceService != NULL){
		if(tr104VoiceService->ZyXEL_VoiceFXO!= NULL){
			minValue = 0;
			maxValue = tr104VoiceService->ZyXEL_VoiceFXO->numberOfPort;
			if( ( fxoIndex >= minValue ) && ( fxoIndex < maxValue)) {	
				ret = TR104_SUCCESS ; 
			}
		}
	}
	if(ret == TR104_SUCCESS){
		return TR104_VFXO_PTR(fxoIndex);
	}else{
		ZyPrintf("tr104VoiceFXOPtrGet : error : can't get the object pointer \r\n");
		return NULL ;
	}
}
/***************************************************************************
** Function Name: tr104VoiceFXOPortPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      : 
****************************************************************************/
ZyXEL_VoiceFXOPort_t* 
tr104VoiceFXOPortPtrGet(int fxoIndex )/*index of FXO*/
{
	int ret = TR104_ERROR ;
	int minValue = 0 , maxValue = 0; 
	if(tr104VoiceService != NULL){
		if(tr104VoiceService->ZyXEL_VoiceFXO!= NULL){
			minValue = 0;
			maxValue = tr104VoiceService->ZyXEL_VoiceFXO->numberOfPort;
			if( ( fxoIndex >= minValue ) && ( fxoIndex < maxValue)) {	
				ret = TR104_SUCCESS ; 
			}
		}
	}
	if(ret == TR104_SUCCESS){
		return TR104_VFXO_PTR(fxoIndex);
	}else{
		ZyPrintf("tr104VoiceFXOPortPtrGet : error : can't get the object pointer \r\n");
		return NULL ;
	}
}
/***************************************************************************
** Function Name: tr104VoiceSipAccountPtrGet
**
** Description  : the Object Ptr Get
**                           
** Returns      : 
****************************************************************************/
ZyXEL_VoiceSipAccount_t*
tr104VoiceSipAccountPtrGet(int SipAccountId)
{
	int ret = TR104_ERROR ;
	int minValue = 0 , maxValue = 0; 
	if(ZyXEL_VoiceSipAccount != NULL){
		minValue = 0;
		maxValue = sipAccountNumber;
		if( ( SipAccountId >= minValue ) && ( SipAccountId < maxValue)) {	
			ret = TR104_SUCCESS ; 
		}
	}
	if(ret == TR104_SUCCESS){
		return TR104_VSIP_ACCOUNT_PTR(SipAccountId);
	}else{
		TR104_DBG("tr104VoiceSipAccountPtrGet : error : can't get the object pointer");
		return NULL ;
	}
}



/********************************************************************
**  
**
**  APIs class: Parameter Getting APIs 
**  descriptions: the APIs to get the Parameter
**
**
*********************************************************************/

/***************************************************************************
** Function Name: tr104GetNumProfile
**
** Description  : get the Parameter
**                           
** Returns      : 
****************************************************************************/
uint32
tr104GetNumProfile(void)
{	
	return tr104VoiceService->voiceProfileNumberOfEntries;
}

/***************************************************************************
** Function Name: tr104GetMaxProfile
**
** Description  : get the Parameter
**                           
** Returns      : 
****************************************************************************/
uint32 
tr104GetMaxProfile(void)
{	
	#if 1 /*Jason , temporary use */
	return 1;
	#else
	VoiceCapObj_t* voiceCap = tr104VoiceService->voiceCapabilities;
	return voiceCap->maxProfileCount;
	#endif
}
/***************************************************************************
** Function Name: tr104GetVPNumOfLine
**
** Description  : get the Parameter
**                           
** Returns      : 
****************************************************************************/
uint32
tr104GetVPNumOfLine(int VpItr)
{	
	VoiceProfObj_t* obj =tr104VoiceProfObjPtrGet(VpItr);
	return obj->numberOfLines;
}
/***************************************************************************
** Function Name: tr104GetMaxLine
**
** Description  : get the total SipAccount number
**                           
** Returns      : 
****************************************************************************/
uint32
tr104GetMaxLine(void)
{	
	return sipAccountNumber;
}

/***************************************************************************
** Function Name: tr104GetMaxSessionsPerLine
**
** Description  : get the Parameter
**                           
** Returns      : 
****************************************************************************/
uint32
tr104GetMaxSessionsPerLine(void)
{	
	VoiceCapObj_t* voiceCap = tr104VoiceService->voiceCapabilities;
	return voiceCap->maxSessionsPerLine;
}

/***************************************************************************
** Function Name: tr104GetNumOfFXO
**
** Description  : get the Parameter
**                           
** Returns      : 
****************************************************************************/
uint8
tr104GetNumOfFXO(void)
{	
	TR104_DBG(" tr104GetNumOfFXO > ENTER");
	return NUM_FXO_CHANNELS;
}

/***************************************************************************
** Function Name: tr104GetNumOfPhyIntf
**
** Description  : get the Parameter
**                           
** Returns      : 
****************************************************************************/
uint32 
tr104GetNumOfPhyIntf(void)
{	
	return NUM_FXS_CHANNELS;
}

/********************************************************************
**  
**
**  API Class : set APIs 
**  descriptions:Area of the Setting APIs of TR104 Module
**
**
*********************************************************************/

int 
tr104LineSetProfSelect(int VpItr,int LineItr)
{
	VoiceProfObj_t* profObj=NULL;
	VoiceProfObj_t* originalProfPtr=NULL;
	VoiceProfLineObj_t * lineObj=NULL;

	if ( TR104_ERROR == tr104ProfileIndexCheck(VpItr)) {
		return TR104RET_INVALID_PARAM_VALUE;
	}
	if ( TR104_ERROR == tr104LineIndexCheck(VpItr , LineItr)) {
		return TR104RET_INVALID_PARAM_VALUE;
	}

	lineObj=tr104VoiceLineObjPtrGet(VpItr, LineItr);

	/* clear the line pointer record of the original Profile.*/
	if((lineObj->VoiceProfObjSelect)!=NULL){
		originalProfPtr = lineObj->VoiceProfObjSelect;
		originalProfPtr->lineProfPtrLst=NULL;
	}

	/* Get the wanted Profile object. */
	profObj=tr104VoiceProfObjPtrGet(VpItr);

	/* For line , set the wanted profile pointer to Voice ProfileObjSelect.*/
	/* For Profile , set the line pointer to lineProfPtrLst;*/
	profObj->lineProfPtrLst=lineObj;
	lineObj->VoiceProfObjSelect = profObj;

	return TR104_SUCCESS;
}


/********************************************************************
**  
**
**  API Class : Dump APIs 
**  descriptions:Area of the Dump APIs of TR104 Module
**
**
*********************************************************************/

/***************************************************************************
** Function Name: tr104DumpVoiceServiceObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/

void 
tr104DumpVoiceServiceObj(void)
{
	ZyPrintf("\n  *********Dump tr104VoiceServiceObj************ \n ");
	ZyPrintf("\n  voiceProfileNumberOfEntries = %d \n ",tr104VoiceService->voiceProfileNumberOfEntries);

}

/***************************************************************************
** Function Name: tr104DumpVoiceCommonObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceCommonObj(void)
{
	ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();
	
	ZyPrintf("\n  *********Dump tr104ZyXEL_voiceCommonObj************ \n ");
	ZyPrintf("\n  softwareVersion = %s \n ", pCommon->softwareVersion);
	ZyPrintf("\n  ifName = %s \n ", pCommon->ifName );
	ZyPrintf("\n  loggingLevel = %d \n ", pCommon->loggingLevel);
	ZyPrintf("\n  voipPort = %d \n ", pCommon->voipPort );
	
	ZyPrintf("\n  ivrsyspermit = %d \n ", pCommon->ivrsyspermit );
	ZyPrintf("\n  ivrLanguage = %d \n ", pCommon->ivrLanguage );
	ZyPrintf("\n  ivrcodec = %d \n ", pCommon->ivrcodec );
	ZyPrintf("\n  specialFlag = %d \n ", pCommon->specialFlag );

	ZyPrintf("\n  CallFallBack = %d \n ", pCommon->callFallBack );	
	ZyPrintf("\n  dialMethod = %d :CALL_FEATURE_DIAL_METHOD_EUROP \n ", pCommon->dialMethod );
	ZyPrintf("\n  ZyXEL_pound = %d \n ", pCommon->ZyXEL_pound);
	ZyPrintf("\n  voipIOPFlags = %d \n ", pCommon->voipIOPFlags);
	
}

/***************************************************************************
** Function Name: tr104DumpVoiceCapObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceCapObj(void)
{
	VoiceCapObj_t* pCapObj = tr104VoiceCapObjPtrGet();
	
	ZyPrintf("\n  *********Dump VoiceCapObj************ \n ");
	ZyPrintf("\n  maxProfileCount = %d \n ", pCapObj->maxProfileCount );
	ZyPrintf("\n  maxLineCount = %d \n ", pCapObj->maxLineCount );
	ZyPrintf("\n  maxSessionsPerLine = %d \n ", pCapObj->maxSessionsPerLine );
	ZyPrintf("\n  maxSessionCount = %d \n ", pCapObj->maxSessionCount );
	ZyPrintf("\n  signalingProtocols = %s \n ", pCapObj->signalingProtocols );
	ZyPrintf("\n  regions = %s \n ", pCapObj->regions );
	ZyPrintf("\n  RTCP = %d \n ", pCapObj->RTCP );
	ZyPrintf("\n  SRTP = %d \n ", pCapObj->SRTP );
	ZyPrintf("\n  RTPRedundancy = %d \n ", pCapObj->RTPRedundancy );
	ZyPrintf("\n  DSCPCoupled = %d \n ", pCapObj->DSCPCoupled );
	ZyPrintf("\n  ethernetTaggingCoupled = %d \n ", pCapObj->ethernetTaggingCoupled );
	ZyPrintf("\n  PSTNSoftSwitchOver = %d \n ", pCapObj->PSTNSoftSwitchOver );
	ZyPrintf("\n  faxT38 = %d \n ", pCapObj->faxT38 );
	ZyPrintf("\n  faxPassThrough = %d \n ", pCapObj->faxPassThrough );
	ZyPrintf("\n  modemPassThrough = %d \n ", pCapObj->modemPassThrough );
	ZyPrintf("\n  toneGeneration = %d \n ", pCapObj->toneGeneration );
	ZyPrintf("\n  ringGeneration = %d \n ", pCapObj->ringGeneration );
	ZyPrintf("\n  digitMap = %d \n ", pCapObj->digitMap );
	ZyPrintf("\n  numberingPlan = %d \n ", pCapObj->numberingPlan );
	ZyPrintf("\n  buttonMap = %d \n ", pCapObj->buttonMap );
	ZyPrintf("\n  voicePortTests = %d \n ", pCapObj->voicePortTests );
	ZyPrintf("\n  ZyXEL_pound = %d \n ", pCapObj->ZyXEL_pound);
	
}
/***************************************************************************
** Function Name: tr104GetNumOfPhyIntf
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceCapSipObj(void)
{
	VoiceCapSipObj_t* pCapSipObj = tr104VoiceCapSipObjPtrGet();
	
	ZyPrintf("\n  *********Dump tr104VoiceCapSipObj************ \n ");
	ZyPrintf("\n  role = %s \n ", pCapSipObj->role );
	ZyPrintf("\n  extensions = %s \n ", pCapSipObj->extensions );
	ZyPrintf("\n  transports = %s \n ", pCapSipObj->transports );
	ZyPrintf("\n  URISchemes = %s \n ", pCapSipObj->URISchemes );
	ZyPrintf("\n  eventSubscription = %d \n ", pCapSipObj->eventSubscription );
	ZyPrintf("\n  responseMap = %d \n ", pCapSipObj->responseMap );
}
/***************************************************************************
** Function Name: tr104DumpVoiceCapCodecTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceCapCodecTab(void)
{
	int i=0;
	tr104ObjLst_t*	CodecTab = tr104VoiceCapCodecsTabPtrGet();
	tr104LstElm_t*	iter = (CodecTab)?CodecTab->head_:NULL;
	VoiceCapCodecObj_t* pCapCodecsObj = NULL ;

	if(CodecTab == NULL){
		ZyPrintf("Table is NULL\n");
		return;
	}
	ZyPrintf("\n  *********Dump tr104VoiceCapCodecTab************ \n ");

	for(i=0;i<CodecTab->size_;i++){
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return;
		}
		pCapCodecsObj=(VoiceCapCodecObj_t*) iter->value_;
		
		ZyPrintf("\n  entryID = %d \n ", pCapCodecsObj->entryID );
		ZyPrintf("\n  codecName = %s \n ", pCapCodecsObj->codecName );
		ZyPrintf("\n  bitRate = %d \n ", pCapCodecsObj->bitRate );
		ZyPrintf("\n  packetizationPeriod = %s \n ", pCapCodecsObj->packetizationPeriod );
		ZyPrintf("\n  silenceSuppression = %d \n ", pCapCodecsObj->silenceSuppression );
		ZyPrintf("**********");
		iter = iter->next_;
	}
	ZyPrintf("\n  *********Dump tr104VoiceCapCodecTab ok************ \n ");
}
/***************************************************************************
** Function Name: tr104DumpVoiceProfileObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfileObj(int VpItr)
{
	VoiceProfObj_t* pProfObj=tr104VoiceProfObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump tr104VoiceProfileObj************ \n ");
	ZyPrintf("\n  ZyXEL_SPNum = %d \n ", pProfObj->ZyXEL_SPNum);	
	ZyPrintf("\n  enable = %d \n ", pProfObj->enable );
	ZyPrintf("\n  reset = %d \n ", pProfObj->reset );
	ZyPrintf("\n  numberOfLines = %d \n ", pProfObj->numberOfLines );
	ZyPrintf("\n  name = %s \n ", pProfObj->name );
	ZyPrintf("\n  signalingProtocol = %s \n ", pProfObj->signalingProtocol );
	ZyPrintf("\n  maxSessions = %d \n ", pProfObj->maxSessions );
	ZyPrintf("\n  DTMFMethod = %d \n ", pProfObj->DTMFMethod );
	ZyPrintf("\n  DTMFMethodG711 = %d \n ", pProfObj->DTMFMethodG711 );	
	ZyPrintf("\n  region = %s \n ", pProfObj->region );
	ZyPrintf("\n  ZyXEL_countrycode = %d \n ", pProfObj->ZyXEL_countrycode );
	ZyPrintf("\n  faxPassThrough = %d \n ", pProfObj->faxPassThrough);
	ZyPrintf("\n  ZyXEL_faxPassThroughPCMA = %d \n ", pProfObj->ZyXEL_faxPassThroughPCMA );
	ZyPrintf("\n  modemPassThrough = %d \n ", pProfObj->modemPassThrough);
	ZyPrintf("\n  digitMap = %s \n ", pProfObj->digitMap );
	ZyPrintf("\n  digitMapEnable = %d \n ", pProfObj->digitMapEnable );
	ZyPrintf("\n  STUNEnable = %d \n ", pProfObj->STUNEnable );
	ZyPrintf("\n  STUNServer = %s \n ", pProfObj->STUNServer );
	ZyPrintf("\n  ZyXEL_stunServPort = %d \n ", pProfObj->ZyXEL_stunServPort );
	ZyPrintf("\n  nonVoiceBandwidthReservedUpstream = %d \n ", pProfObj->nonVoiceBandwidthReservedUpstream );
	ZyPrintf("\n  nonVoiceBandwidthReservedDownstream = %d \n ", pProfObj->nonVoiceBandwidthReservedDownstream );
	ZyPrintf("\n  PSTNFailOver = %d \n ", pProfObj->PSTNFailOver );	
	ZyPrintf("\n  ZyXEL_ActiveBoundIfName = %s \n ", pProfObj->ZyXEL_ActiveBoundIfName);
	ZyPrintf("\n  ZyXEL_BoundIfList = %s \n ", pProfObj->ZyXEL_BoundIfList);
	ZyPrintf("\n  ZyXEL_BoundIfName = %s \n ", pProfObj->ZyXEL_BoundIfName);
	ZyPrintf("\n  ZyXEL_BoundIpAddr = %s \n ", pProfObj->ZyXEL_BoundIpAddr);
#ifdef BUILD_IGNORE_DIRECT_IP
	ZyPrintf("\n  ZyXEL_ignoreDirectIP = %s \n ", pProfObj->ZyXEL_ignoreDirectIP);
#endif	
}
/***************************************************************************
** Function Name: tr104DumpVoiceProfSipObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceProfSipObj(int VpItr)
{
	VoiceProfSipObj_t* ProfSipObj=tr104VoiceProfSipObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump tr104VoiceProfSipObj************ \n ");
	ZyPrintf("\n  proxyServer = %s \n ", ProfSipObj->proxyServer );
	ZyPrintf("\n  proxyServerPort = %d \n ", ProfSipObj->proxyServerPort );
	ZyPrintf("\n  proxyServerTransport = %d \n ", ProfSipObj->proxyServerTransport );

	ZyPrintf("\n  registrarServer = %s \n ", ProfSipObj->registrarServer );
	ZyPrintf("\n  registrarServerPort = %d \n ", ProfSipObj->registrarServerPort );
	ZyPrintf("\n  registrarServerTransport = %d \n ", ProfSipObj->registrarServerTransport );
	
	ZyPrintf("\n  registerExpires = %d \n ", ProfSipObj->registerExpires );
	ZyPrintf("\n  registersMinExpires = %d \n ", ProfSipObj->registersMinExpires);
	ZyPrintf("\n  registerRetryInterval = %d \n ", ProfSipObj->registerRetryInterval );
	ZyPrintf("\n  registrationPeriod = %d \n ", ProfSipObj->registrationPeriod);	
	
	ZyPrintf("\n  userAgentDomain = %s \n ", ProfSipObj->userAgentDomain );
	ZyPrintf("\n  userAgentPort = %d \n ", ProfSipObj->userAgentPort );
	ZyPrintf("\n  userAgentTransport = %d \n ", ProfSipObj->userAgentTransport );
	
	ZyPrintf("\n  outboundProxy = %s \n ", ProfSipObj->outboundProxy );
	ZyPrintf("\n  outboundProxyPort = %d \n ", ProfSipObj->outboundProxyPort );
	
	ZyPrintf("\n  ZyXEL_outboundKAIntvl = %d \n ", ProfSipObj->ZyXEL_outboundKAIntvl );
	ZyPrintf("\n  ZyXEL_KAServer = %d \n ", ProfSipObj->ZyXEL_KAServer );
	

	ZyPrintf("\n  ZyXEL_minSE = %d \n ", ProfSipObj->ZyXEL_minSE );
	ZyPrintf("\n  ZyXEL_mwiExpireTime = %d \n ", ProfSipObj->ZyXEL_mwiExpireTime );
	ZyPrintf("\n  inviteExpires = %d \n ", ProfSipObj->inviteExpires );	

	ZyPrintf("\n  DSCPMark = %d \n ", ProfSipObj->DSCPMark );
	ZyPrintf("\n  ZyXEL_sigVLanTagTPID = %d \n ", ProfSipObj->ZyXEL_sigVLanTagTPID );
	ZyPrintf("\n  ZyXEL_sigVLanTagTCI = %d \n ", ProfSipObj->ZyXEL_sigVLanTagTCI );

	ZyPrintf("\n  ZyXEL_rfc3325support = %d \n ", ProfSipObj->ZyXEL_rfc3325support );
	ZyPrintf("\n  ZyXEL_rfc3262support = %d \n ", ProfSipObj->ZyXEL_rfc3262support );
	ZyPrintf("\n  ZyXEL_fakeSipIP = %s \n ", ProfSipObj->ZyXEL_fakeSipIP );
	ZyPrintf("\n  ZyXEL_fakeSipPort = %d \n ", ProfSipObj->ZyXEL_fakeSipPort );
	
	ZyPrintf("\n  organization = %s \n ", ProfSipObj->organization );	
	ZyPrintf("\n  TimerT1 = %d \n ", ProfSipObj->TimerT1 );
	ZyPrintf("\n  TimerT2 = %d \n ", ProfSipObj->TimerT2 );
	ZyPrintf("\n  TimerT4 = %d \n ", ProfSipObj->TimerT4 );
	ZyPrintf("\n  TimerA = %d \n ", ProfSipObj->TimerA );
	ZyPrintf("\n  TimerB = %d \n ", ProfSipObj->TimerB );
	ZyPrintf("\n  TimerC = %d \n ", ProfSipObj->TimerC );
	ZyPrintf("\n  TimerD = %d \n ", ProfSipObj->TimerD );
	ZyPrintf("\n  TimerE = %d \n ", ProfSipObj->TimerE );
	ZyPrintf("\n  TimerF = %d \n ", ProfSipObj->TimerF );
	ZyPrintf("\n  TimerG = %d \n ", ProfSipObj->TimerG );
	ZyPrintf("\n  TimerH = %d \n ", ProfSipObj->TimerH );
	ZyPrintf("\n  TimerI = %d \n ", ProfSipObj->TimerI );
	ZyPrintf("\n  TimerJ = %d \n ", ProfSipObj->TimerJ );
	ZyPrintf("\n  TimerK = %d \n ", ProfSipObj->TimerK);

	ZyPrintf("\n  inboundAuth = %d \n ", ProfSipObj->inboundAuth );
	ZyPrintf("\n  inboundAuthUsername = %s \n ", ProfSipObj->inboundAuthUsername );
	ZyPrintf("\n  inboundAuthPassword = %s \n ", ProfSipObj->inboundAuthPassword );
	ZyPrintf("\n  useCodecPriorityInSDPResponse = %d \n ", ProfSipObj->useCodecPriorityInSDPResponse);
	ZyPrintf("\n  VLANIDMark = %d \n ", ProfSipObj->VLANIDMark );
	ZyPrintf("\n  ethernetPriorityMark = %d \n ", ProfSipObj->ethernetPriorityMark);
	ZyPrintf("\n  SIPEventSubscribeNumberOfElements = %d \n ", ProfSipObj->SIPEventSubscribeNumberOfElements );
	ZyPrintf("\n  SIPResponseMapNumberOfElements = %d \n ", ProfSipObj->SIPResponseMapNumberOfElements);

#if HEARTBEAT_MECHANISM_ENABLE 
	ZyPrintf("\n  ZyXEL_HeartbeatEnable = %d \n ", ProfSipObj->ZyXEL_HeartbeatEnable );
	ZyPrintf("\n  ZyXEL_RateOfHeartbeat = %d \n ", ProfSipObj->ZyXEL_RateOfHeartbeat );
#endif
	ZyPrintf("\n ZyXEL_rfc3263support = %d \n",ProfSipObj->ZyXEL_rfc3263support);

}

/***************************************************************************
** Function Name: tr104DumpVoiceProfSipEventSubTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceProfSipEventSubTab(int VpItr)
{
	int i=0;
	tr104ObjLst_t* sipEventSubTab = tr104VoiceProfSipEventSubTabPtrGet(VpItr);
	tr104LstElm_t* iter=(sipEventSubTab)?sipEventSubTab->head_:NULL;
	VoiceProfSipEventSubObj_t* eventSubObj = NULL;
	
	ZyPrintf("\n  *********Dump VoiceProfSipEventSubTab************ \n ");
	
	if(sipEventSubTab == NULL){
		ZyPrintf("Table is NULL\n");
		return;
	}
	for(i=0;i<sipEventSubTab->size_;i++){
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return;
		}
		eventSubObj=(VoiceProfSipEventSubObj_t*) iter->value_;
		ZyPrintf("event =%s \n ",eventSubObj->event);
		ZyPrintf("notifier =%s \n ",eventSubObj->notifier);
		ZyPrintf("notifierTransport = %d \n",eventSubObj->notifierTransport);
		ZyPrintf("expireTime = %d \n",eventSubObj->expireTime);
		
	}
}

/***************************************************************************
** Function Name: tr104DumpVoiceProfSipRespMapTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfSipRespMapTab(int VpItr)
{
	int i=0;
	tr104ObjLst_t* sipRespMapTab = tr104VoiceProfSipRespMapTabPtrGet(VpItr);
	tr104LstElm_t* iter=(sipRespMapTab)?sipRespMapTab->head_:NULL;
	VoiceProfSipRespMapObj_t* eventSubObj = NULL;
	ZyPrintf("\n  *********Dump VoiceProfSipRespMapTab************ \n ");
	
	if(sipRespMapTab == NULL){
		ZyPrintf("Table is NULL\n");
		return;
	}
	for(i=0;i<sipRespMapTab->size_;i++){
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return;
		}
		eventSubObj=(VoiceProfSipRespMapObj_t*) iter->value_;
		ZyPrintf("SIPResponseNumber =%d \n ",eventSubObj->SIPResponseNumber);
		ZyPrintf("textMessage =%s \n ",eventSubObj->textMessage);
		ZyPrintf("tone = %d \n",eventSubObj->tone);
	}
}


/***************************************************************************
** Function Name: tr104DumpVoiceProfNumPlanObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfNumPlanObj(int VpItr)
{
	VoiceProfNumPlanObj_t* numPlanObj = tr104VoiceProfNumPlanObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump VoiceProfNumPlanObj************ \n ");
	ZyPrintf("\n  minimumNumberOfDigits = %d \n ", numPlanObj->minimumNumberOfDigits);
	ZyPrintf("\n  maximumNumberOfDigits = %d \n ", numPlanObj->maximumNumberOfDigits);

	ZyPrintf("\n  interDigitTimerStd = %d \n ", numPlanObj->interDigitTimerStd);
	ZyPrintf("\n  interDigitTimerOpen = %d \n ", numPlanObj->interDigitTimerOpen);	
	ZyPrintf("\n  ZyXEL_dialingLongInterval = %d \n ", numPlanObj->ZyXEL_dialingLongInterval);
	
	ZyPrintf("\n  invalidNumberTone = %d \n ", numPlanObj->invalidNumberTone);
	ZyPrintf("\n  prefixInfoMaxEntries = %d \n ", numPlanObj->prefixInfoMaxEntries);
	ZyPrintf("\n  prefixInfoNumberOfEntries = %d \n ", numPlanObj->prefixInfoNumberOfEntries);
	
}
/***************************************************************************
** Function Name: tr104DumpVoiceProfNumPlanPrefixinfoTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfNumPlanPrefixinfoTab(int VpItr)
{
	int i=0;
	tr104ObjLst_t* prefixInfoTab= tr104VoiceProfNumPlanPrefixInfoTabPtrGet(VpItr);
	tr104LstElm_t* iter=(prefixInfoTab)?prefixInfoTab->head_:NULL;
	VoiceProfNumPlanPrefixInfoObj_t* prefixInfoObj=NULL;
	if(prefixInfoTab==NULL){
		ZyPrintf("Table is NULL\n");
		return;
	}
	ZyPrintf("\n  *********Dump VoiceProfNumPlanPrefixinfoTab************ \n ");
	for(i=0;i<prefixInfoTab->size_;i++){	
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return;
		}
		prefixInfoObj=iter->value_;		
		ZyPrintf("\n  prefixRange = %s \n ", prefixInfoObj->prefixRange);
		ZyPrintf("\n  prefixMinNumberOfDigits = %d \n ", prefixInfoObj->prefixMinNumberOfDigits);
		ZyPrintf("\n  prefixMaxNumberOfDigits = %d \n ", prefixInfoObj->prefixMaxNumberOfDigits);		
		ZyPrintf("\n  numberOfDigitsToRemove = %d \n ", prefixInfoObj->numberOfDigitsToRemove);
		ZyPrintf("\n  posOfDigitsToRemove = %d \n ", prefixInfoObj->posOfDigitsToRemove);	
		ZyPrintf("\n  dialTone = %d \n ", prefixInfoObj->dialTone);
		ZyPrintf("\n  facilityAction = %s \n ", prefixInfoObj->facilityAction);
		ZyPrintf("\n  facilityActionArgument = %s \n ", prefixInfoObj->facilityActionArgument);

		iter = iter->next_;
	}
}
/***************************************************************************
** Function Name: tr104DumpVoiceProfRtpObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfRtpObj(int VpItr)
{
	VoiceProfRtpObj_t* ProfRtpObj = tr104VoiceProfRtpObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump TR104VoiceProfRtpObj************ \n ");
	ZyPrintf("\n  localPortMin = %d \n ", ProfRtpObj->localPortMin );
	ZyPrintf("\n  localPortMax = %d \n ", ProfRtpObj->localPortMax );
	ZyPrintf("\n  DSCPMark = %d \n ", ProfRtpObj->DSCPMark );
	ZyPrintf("\n  VLANIDMark = %d \n ", ProfRtpObj->VLANIDMark );
	ZyPrintf("\n  ethernetPriorityMark = %d \n ", ProfRtpObj->ethernetPriorityMark );
	ZyPrintf("\n  telephoneEventPayloadType = %d \n ", ProfRtpObj->telephoneEventPayloadType );

}
/***************************************************************************
** Function Name: tr104DumpVoiceProfRtpRtcpObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfRtpRtcpObj(int VpItr)
{
	VoiceProfRtpRtcpObj_t* ProfRtpRtcpObj = tr104VoiceProfRtpRtcpObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump VoiceProfRtpRtcpObj_t************ \n ");
	ZyPrintf("\n  enable = %d \n ", ProfRtpRtcpObj->enable);
	ZyPrintf("\n  txRepeatInterval = %d \n ",ProfRtpRtcpObj->txRepeatInterval );
	ZyPrintf("\n  localCName = %s \n ", ProfRtpRtcpObj->localCName );
}
/***************************************************************************
** Function Name: tr104DumpVoiceProfRtpSrtpObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfRtpSrtpObj(int VpItr)
{
	VoiceProfRtpSrtpObj_t* srtpObj = tr104VoiceProfRtpSrtpObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump VoiceProfRtpSrtpObj************ \n ");
	ZyPrintf("\n  enable = %d \n ", srtpObj->enable);
	ZyPrintf("\n  encryptionKeySizes = %s \n ", srtpObj->encryptionKeySizes);
	ZyPrintf("\n  ZyXELCryptoSuiteName = %s \n ", srtpObj->ZyXEL_CryptoSuiteName);
}
/***************************************************************************
** Function Name: tr104DumpVoiceProfRtpRedundancyObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceProfRtpRedundancyObj(int VpItr)
{
	VoiceProfRtpRedundancyObj_t* pRedundancyObj = tr104VoiceProfRtpRedundancyObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump VoiceProfRtpRedundancyObj************ \n ");
	ZyPrintf("\n  enable = %d \n ", pRedundancyObj->enable);

}
/***************************************************************************
** Function Name: tr104DumpVoiceProfFaxT38Obj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceProfFaxT38Obj(int VpItr)
{
	VoiceProfFaxT38Obj_t* ProfFaxT38Obj = tr104VoiceProfFaxT38ObjPtrGet(VpItr);
	ZyPrintf("\n  *********Dump TR104VoiceProfFaxT38Obj************ \n ");

	ZyPrintf("\n  enable = %d \n ", ProfFaxT38Obj->enable );
	ZyPrintf("\n  bitRate = %d \n ", ProfFaxT38Obj->bitRate );
	ZyPrintf("\n  highSpeedPacketRate = %d \n ", ProfFaxT38Obj->highSpeedPacketRate );
	ZyPrintf("\n  highSpeedRedundancy = %d \n ", ProfFaxT38Obj->highSpeedRedundancy );
	ZyPrintf("\n  lowSpeedRedundancy = %d \n ", ProfFaxT38Obj->lowSpeedRedundancy );	
	ZyPrintf("\n  TCFMethod = %d \n ", ProfFaxT38Obj->TCFMethod );
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineObj(int VpItr, int LineItr)
{
	VoiceProfLineObj_t* ProfLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	ZyPrintf("Address : tr104DumpVoiceLineObj = %x ,  LineItr =%d \n" , ProfLine , LineItr);
	ZyPrintf("\n  *********Dump TR104VoiceProfLineObj************ \n ");
	ZyPrintf("\n  ZyXEL_CMAcntNum = %d \n ", ProfLine->ZyXEL_CMAcntNum);		
	ZyPrintf("\n  enable = %d \n ", ProfLine->enable );
	ZyPrintf("\n  directoryNumber = %s \n ", ProfLine->directoryNumber );
	ZyPrintf("\n  status = %d \n ", ProfLine->status );
	ZyPrintf("\n  callState = %d \n ", ProfLine->callState );

	ZyPrintf("\n  phyReferenceList = %s \n ", ProfLine->phyReferenceList );
	
	ZyPrintf("\n  ZyXEL_phyReferenceFXSList = %d \n ", ProfLine->ZyXEL_phyReferenceFXSList);
	
	ZyPrintf("\n  ZyXEL_AutoDialEnable = %d \n ", ProfLine->ZyXEL_AutoDialEnable );
	ZyPrintf("\n  ZyXEL_AutoDialNumber = %s \n ", ProfLine->ZyXEL_AutoDialNumber );

	ZyPrintf("\n  ringMuteStatus = %d \n ", ProfLine->ringMuteStatus );
	ZyPrintf("\n  ringVolumeStatus = %d \n ", ProfLine->ringVolumeStatus );

}

/***************************************************************************
** Function Name: tr104DumpVoiceLineSipObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineSipObj(int VpItr, int LineItr)
{
	VoiceProfLineSipObj_t* LineSip = tr104VoiceLineSipObjPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump TR104VoiceProfLineSipObj************ \n ");
	ZyPrintf("\n  authUserName = %s \n ", LineSip->authUserName);
	ZyPrintf("\n  authPassword = %s \n ", LineSip->authPassword);

	ZyPrintf("\n  URI = %s \n ",LineSip->URI);
	
	ZyPrintf("\n  ZyXEL_registrarType = %d \n ", LineSip->ZyXEL_registrarType);	
	ZyPrintf("\n  ZyXEL_url_type = %d \n ", LineSip->ZyXEL_url_type);
	ZyPrintf("\n  ZyXEL_callFwdTableIndex = %d \n ", LineSip->ZyXEL_callFwdTableIndex );
	ZyPrintf("\n  ZyXEL_mixerMode = %d \n ", LineSip->ZyXEL_mixerMode );
	ZyPrintf("\n  SIPEventSubscribeNumberOfElements = %d \n ", LineSip->SIPEventSubscribeNumberOfElements );
	
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineSipEventSubTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineSipEventSubTab(int VpItr, int LineItr)
{	
	int i=0;
	tr104ObjLst_t* sipEventSubTab = tr104VoiceLineSipEventSubTabPtrGet(VpItr, LineItr);
	tr104LstElm_t* iter=(sipEventSubTab)?sipEventSubTab->head_:NULL;
	VoiceProfLineSipEventSubObj_t* eventSubObj = NULL;
	if(sipEventSubTab==NULL){
		ZyPrintf("Table is NULL\n");
		return;
	}

	ZyPrintf("\n  *********Dump VoiceProfLineSipEventSubTab************ \n ");

	for(i=0;i<sipEventSubTab->size_;i++){	
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return;
		}
		eventSubObj=iter->value_;
		ZyPrintf("\n event = %d \n",eventSubObj->event);
		ZyPrintf("\n authUserName = %d \n",eventSubObj->authUserName);
		ZyPrintf("\n authPassword = %d \n",eventSubObj->authPassword);
		iter = iter->next_;
	}
	
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineRingerObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineRingerObj(int VpItr, int LineItr)
{
	VoiceProfLineRingerObj_t* ringerObj = tr104VoiceLineRingerObjPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump VoiceLineRingerObj************ \n ");
	
	ZyPrintf("\n  eventNumberOfEntries = %d \n ", ringerObj->eventNumberOfEntries);
	ZyPrintf("\n  descriptionNumberOfEntries = %d \n ", ringerObj->descriptionNumberOfEntries);
	ZyPrintf("\n  patternNumberOfEntries = %d \n ", ringerObj->patternNumberOfEntries);

}
/***************************************************************************
** Function Name: tr104DumpVoiceLineRingerEventTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineRingerEventTab(int VpItr, int LineItr)
{
	tr104ObjLst_t* pRingerEventTab = tr104VoiceLineRingerEventTabPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump VoiceLineRingerEventTab************ \n ");
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineRingerDesriptionTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineRingerDesriptionTab(int VpItr, int LineItr)
{
	tr104ObjLst_t* pDescriptionTab = tr104VoiceLineRingerDescriptionTabPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump VoiceLineRingerDesriptionTab************ \n ");
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineRingerPatternTab
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineRingerPatternTab(int VpItr, int LineItr)
{
	tr104ObjLst_t* pPatternTab = tr104VoiceLineRingerPatternTabPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump VoiceLineRingerPatternTab************ \n ");
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineCallingFeaturesObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineCallingFeaturesObj(int VpItr, int LineItr)
{
	VoiceProfLineCallingFeaturesObj_t* CallingFeatures = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump TR104VoiceLineCallingFeaturesObj************ \n ");
	ZyPrintf("\n  callerIDEnable = %d \n ", CallingFeatures->callerIDEnable );
	ZyPrintf("\n  repeatDialEnable = %d \n ", CallingFeatures->repeatDialEnable );	
	ZyPrintf("\n  callerIDNameEnable = %d \n ", CallingFeatures->callerIDNameEnable );	
	
	ZyPrintf("\n  callerIDName = %s \n ", CallingFeatures->callerIDName );
	
	ZyPrintf("\n  callWaitingEnable = %d \n ", CallingFeatures->callWaitingEnable );
	ZyPrintf("\n  callWaitingStatus = %d \n ", CallingFeatures->callWaitingStatus );
	ZyPrintf("\n  maxSessions = %d \n ", CallingFeatures->maxSessions );

	ZyPrintf("\n  conferenceCallingStatus = %d \n ", CallingFeatures->conferenceCallingStatus);

	ZyPrintf("\n  conferenceCallingSessionCount = %d \n ", CallingFeatures->conferenceCallingSessionCount );
	
	ZyPrintf("\n  callForwardUnconditionalEnable = %d \n ", CallingFeatures->callForwardUnconditionalEnable );
	ZyPrintf("\n  callForwardOnBusyEnable = %d \n ", CallingFeatures->callForwardOnBusyEnable );
	ZyPrintf("\n  callForwardOnNoAnswerEnable = %d \n ", CallingFeatures->callForwardOnNoAnswerEnable );
	ZyPrintf("\n  callForwardOnNoAnswerRingCount = %d \n ", CallingFeatures->callForwardOnNoAnswerRingCount );

	ZyPrintf("\n  callForwardUnconditionalNumber = %s \n ", CallingFeatures->callForwardUnconditionalNumber );
	ZyPrintf("\n  callForwardOnBusyNumber = %s \n ", CallingFeatures->callForwardOnBusyNumber );
	ZyPrintf("\n  callForwardOnNoAnswerNumber = %s \n ", CallingFeatures->callForwardOnNoAnswerNumber );
		
	ZyPrintf("\n  callTransferEnable = %d \n ", CallingFeatures->callTransferEnable );
	ZyPrintf("\n  ZyXEL_callTransferAfterConference = %d \n ", CallingFeatures->ZyXEL_callTransferAfterConference);
	ZyPrintf("\n  ZyXEL_warmLineEnable = %d \n ", CallingFeatures->ZyXEL_warmLineEnable);
	ZyPrintf("\n  ZyXEL_warmLineNum = %s \n ", CallingFeatures->ZyXEL_warmLineNum);
	ZyPrintf("\n  ZyXEL_timerFirstDigitWarmLine = %d \n ", CallingFeatures->ZyXEL_timerFirstDigitWarmLine);
	ZyPrintf("\n  ZyXEL_hotLineEnable = %d \n ", CallingFeatures->ZyXEL_hotLineEnable);
	ZyPrintf("\n  ZyXEL_hotLineNum = %s \n ", CallingFeatures->ZyXEL_hotLineNum);

#if 1	/* aircheng add */
	ZyPrintf("\n  ZyXEL_remoteRingbackTone = %d \n ", CallingFeatures->ZyXEL_remoteRingbackTone );
	ZyPrintf("\n  ZyXEL_remoteRingbackToneIndex = %d \n ", CallingFeatures->ZyXEL_remoteRingbackToneIndex);	
	ZyPrintf("\n  ZyXEL_musicOnHoldTone = %d \n ", CallingFeatures->ZyXEL_musicOnHoldTone );
	ZyPrintf("\n  ZyXEL_musicOnHoldToneIndex = %d \n ", CallingFeatures->ZyXEL_musicOnHoldToneIndex);
#endif

	ZyPrintf("\n  ZyXEL_missedCallEmailEnable = %d \n ", CallingFeatures->ZyXEL_missedCallEmailEnable);
	ZyPrintf("\n  ZyXEL_missedCallEmailServerAddr = %d \n ", CallingFeatures->ZyXEL_missedCallEmailServerAddr);
	ZyPrintf("\n  ZyXEL_missedCallEmailAddr = %s \n ", CallingFeatures->ZyXEL_missedCallEmailAddr);
	ZyPrintf("\n  ZyXEL_missedCallEmailTitle = %s \n ", CallingFeatures->ZyXEL_missedCallEmailTitle);
	ZyPrintf("\n  MWIEnable = %d \n ", CallingFeatures->MWIEnable );
	ZyPrintf("\n  ZyXEL_mwiExpireTime = %d \n ", CallingFeatures->ZyXEL_mwiExpireTime);
	ZyPrintf("\n  messageWaiting = %d \n ", CallingFeatures->messageWaiting );
	
	ZyPrintf("\n  anonymousCallBlockEnable = %d \n ", CallingFeatures->anonymousCallBlockEnable );
	ZyPrintf("\n  anonymousCallEnable = %d \n ", CallingFeatures->anonymousCallEnable );
	ZyPrintf("\n  doNotDisturbEnable = %d \n ", CallingFeatures->doNotDisturbEnable );
	ZyPrintf("\n  callReturnEnable = %d \n ", CallingFeatures->callReturnEnable );
	ZyPrintf("\n  repeatDialEnable = %d \n ", CallingFeatures->repeatDialEnable );	
	
	ZyPrintf("\n  ZyXEL_Call_Waiting_Reject_Time = %d \n ", CallingFeatures->ZyXEL_Call_Waiting_Reject_Time );
	ZyPrintf("\n  ZyXEL_conferenceURI = %s \n ", CallingFeatures->ZyXEL_conferenceURI );	

}
/***************************************************************************
** Function Name: tr104DumpVoiceLineCodecObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineCodecObj(int VpItr, int LineItr)
{
	VoiceProfLineCodecObj_t* lineCodecObj = tr104VoiceLineCodecObjPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump VoiceLineCodecObj************ \n ");
	
	ZyPrintf("\n  transmitCodec = %s \n ", lineCodecObj->transmitCodec);
	ZyPrintf("\n  receiveCodec = %s \n ", lineCodecObj->receiveCodec);	
	ZyPrintf("\n  transmitBitRate = %d \n ", lineCodecObj->transmitBitRate);	
	ZyPrintf("\n  receiveBitRate = %d \n ", lineCodecObj->receiveBitRate);	
	ZyPrintf("\n  transmitSilenceSuppression = %d \n ", lineCodecObj->transmitSilenceSuppression);	
	ZyPrintf("\n  receiveSilenceSuppression = %d \n ", lineCodecObj->receiveSilenceSuppression);	
	ZyPrintf("\n  transmitPacketizationPeriod = %d \n ", lineCodecObj->transmitPacketizationPeriod);	
		
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineCodecList
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceLineCodecList(int VpItr, int LineItr)
{
	int i=0;
	tr104ObjLst_t*	CodecLst = tr104VoiceLineCodecTabPtrGet(VpItr, LineItr);
	tr104LstElm_t*	iter = (CodecLst)?CodecLst->head_:NULL;
	VoiceProfLineCodecListObj_t* pLineCodecsObj = NULL ;

	if(CodecLst == NULL){
		ZyPrintf("Table is NULL\n");
		return;
	}
	ZyPrintf("\n  *********Dump tr104DumpVoiceLineCodecList************ \n ");

	for(i=0;i<CodecLst->size_;i++){
		if(iter->value_==NULL){
			ZyPrintf("value_ is NULL\n");
			return;
		}
		pLineCodecsObj=(VoiceCapCodecObj_t*) iter->value_;
		
		ZyPrintf("\n  entryID = %d \n ", pLineCodecsObj->entryID );
		ZyPrintf("\n  codec = %s \n ", pLineCodecsObj->codec );
		ZyPrintf("\n  bitRate = %d \n ", pLineCodecsObj->bitRate );
		ZyPrintf("\n  packetizationPeriod = %s \n ", pLineCodecsObj->packetizationPeriod );
		ZyPrintf("\n  silenceSuppression = %d \n ", pLineCodecsObj->silenceSuppression );
		ZyPrintf("\n  enable = %d \n ", pLineCodecsObj->enable);
		ZyPrintf("\n  priority = %d \n ", pLineCodecsObj->priority);
		
		ZyPrintf("**********");
		iter = iter->next_;
	}
	ZyPrintf("\n  *********Dump tr104DumpVoiceLineCodecList ok************ \n ");
}
/***************************************************************************
** Function Name: tr104DumpVoiceLineProcessingObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceLineProcessingObj(int VpItr, int LineItr)
{
	VoiceProfLineVoiceProcessingObj_t* voiceProcessing = tr104VoiceLineProcessingObjPtrGet(VpItr, LineItr);
	ZyPrintf("\n  *********Dump tr104DumpVoiceLineProcessingObj************ \n ");
	ZyPrintf("\n  transmitGain = %d \n ", voiceProcessing->transmitGain );
	ZyPrintf("\n  receiveGain = %d \n ", voiceProcessing->receiveGain );	
	ZyPrintf("\n  echoCancellationEnable = %d \n ", voiceProcessing->echoCancellationEnable );
	ZyPrintf("\n  echoCancellationInUse = %d \n ", voiceProcessing->echoCancellationInUse );	
	ZyPrintf("\n  echoCancellationTail = %d \n ", voiceProcessing->echoCancellationTail );			
}

/***************************************************************************
** Function Name: tr104DumpVoicePhyIntfObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoicePhyIntfObj(int phyIntfIdx)
{
	VoicePhyIntfObj_t* PhyIntfobj = tr104VoicePhyIntfObjPtrGet(phyIntfIdx);
	ZyPrintf("\n  *********Dump tr104VoicePhyIntfObj************ \n ");
	ZyPrintf("\n  phyPort = %s \n ", PhyIntfobj->phyPort);
	ZyPrintf("\n  interfaceID = %d \n ", PhyIntfobj->interfaceID);
	ZyPrintf("\n  description = %s \n ", PhyIntfobj->description );

	ZyPrintf("\n  potsType = %d \n ", PhyIntfobj->ZyXEL_PotsType);
	ZyPrintf("\n  dialingType = %d \n ", PhyIntfobj->ZyXEL_DialingType);

	ZyPrintf("\n  ZyXEL_DialToneType = %d \n ", PhyIntfobj->ZyXEL_DialToneType);
	ZyPrintf("\n  ZyXEL_ReceiveToneType = %d \n ", PhyIntfobj->ZyXEL_ReceiveToneType);
	ZyPrintf("\n  ZyXEL_FlashMaxInterval = %d \n ", PhyIntfobj->ZyXEL_FlashMaxInterval);
	ZyPrintf("\n  ZyXEL_FlashMinInterval = %d \n ", PhyIntfobj->ZyXEL_FlashMinInterval);
	ZyPrintf("\n  ZyXEL_JitterBufSize = %d \n ", PhyIntfobj->ZyXEL_JitterBufSize);
	
	ZyPrintf("\n  ZyXEL_CallerIDType = %d \n ", PhyIntfobj->ZyXEL_CallerIDType);
	ZyPrintf("\n  ZyXEL_LineSelect = %d \n ", PhyIntfobj->ZyXEL_LineSelect);

	ZyPrintf("\n  ZyXEL_TasTimeoutInterval = %d \n ", PhyIntfobj->ZyXEL_TasTimeoutInterval);
	ZyPrintf("\n  ZyXEL_TasTimeoutInterval_2 = %d \n ", PhyIntfobj->ZyXEL_TasTimeoutInterval_2);
	ZyPrintf("\n  ZyXEL_CallerIDRingTimeout = %d \n ", PhyIntfobj->ZyXEL_CallerIDRingTimeout);
	ZyPrintf("\n  ZyXEL_FirstRingTimoutInterval = %d \n ", PhyIntfobj->ZyXEL_FirstRingTimoutInterval);
	ZyPrintf("\n  ZyXEL_FirstTASType = %d \n ", PhyIntfobj->ZyXEL_FirstTASType);
	ZyPrintf("\n  ZyXEL_SecondTASType = %d \n ", PhyIntfobj->ZyXEL_SecondTASType);
	ZyPrintf("\n  ZyXEL_CallerID_DTAS_ACK_Timeout = %d \n ", PhyIntfobj->ZyXEL_CallerID_DTAS_ACK_Timeout);

	ZyPrintf("\n  ZyXEL_MsnIncomingSelect = %d \n ", PhyIntfobj->ZyXEL_MsnIncomingSelect);
	ZyPrintf("\n  ZyXEL_MsnOutgoingSelect = %d \n ", PhyIntfobj->ZyXEL_MsnOutgoingSelect);
	ZyPrintf("\n  ZyXEL_HowlerToneTimeout = %d \n ", PhyIntfobj->ZyXEL_HowlerToneTimeout);
	
}

/***************************************************************************
** Function Name: tr104DumpVoiceFxoObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceFxoObj(int fxoIdx)
{
	ZyXEL_VoiceFXO_t* fxoObj = tr104VoiceFXOPtrGet(fxoIdx);
	ZyPrintf("\n  *********Dump tr104DumpVoiceFxoObj************ \n ");
	ZyPrintf("\n  numberOfPort = %d \n ", fxoObj->numberOfPort);
}

/***************************************************************************
** Function Name: tr104DumpVoiceFxoPortObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void 
tr104DumpVoiceFxoPortObj(int fxoIdx)
{
	ZyXEL_VoiceFXOPort_t* fxoPort = tr104VoiceFXOPortPtrGet(fxoIdx);
	ZyPrintf("\n  *********Dump tr104DumpVoiceFxoPortObj************ \n ");
	ZyPrintf("\n  fxoDialLongInterval = %d \n ", fxoPort->fxoDialLongInterval);
	ZyPrintf("\n  fxoPhoneSelect = %d \n ", fxoPort->fxoPhoneSelect);
	ZyPrintf("\n  fxoDTMFPauseDuration = %d \n ", fxoPort->fxoDTMFPauseDuration);
	ZyPrintf("\n  fxoFlashMinInterval = %d \n ", fxoPort->fxoFlashMinInterval);
	ZyPrintf("\n  fxoFlashMaxInterval = %d \n ", fxoPort->fxoFlashMaxInterval);
	ZyPrintf("\n  fxoLifeLineStableTime = %d \n ", fxoPort->fxoLifeLineStableTime);
	ZyPrintf("\n  fxoDTMFDigitDuration = %d \n ", fxoPort->fxoDTMFDigitDuration);
	ZyPrintf("\n  pollingPollarityTime = %d \n ", fxoPort->pollingPollarityTime);
	ZyPrintf("\n  detectCIDInterval = %d \n ", fxoPort->detectCIDInterval);

}


/***************************************************************************
** Function Name: tr104DumpVoiceSipAccountObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/
void
tr104DumpVoiceSipAccountObj(int SipAccountId)
{
	ZyXEL_VoiceSipAccount_t* VoiceSipAccount = tr104VoiceSipAccountPtrGet(SipAccountId);
	ZyPrintf("\n  *********Dump tr104DumpVoiceSipAccountObj************ \n ");
	ZyPrintf("\n  sipAccountId = %d \n ", VoiceSipAccount->sipAccountId);
	ZyPrintf("\n  enable = %d \n ", VoiceSipAccount->enable);
	ZyPrintf("\n  SPId = %d \n ", VoiceSipAccount->SPId);
	ZyPrintf("\n  LineItr = %d \n ", VoiceSipAccount->LineItr);
	ZyPrintf("\n  VpItr = %d \n ", VoiceSipAccount->VpItr);
	ZyPrintf("\n  flag = %d \n ", VoiceSipAccount->flag);

}

/***************************************************************************
** Function Name: tr104DumpVoicePSTNObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/

void tr104DumpVoicePSTNObj(int PSTNIdx){
	ZyXEL_VoicePSTN_t* VoicePSTN_p = NULL;
	VoicePSTN_p = tr104VoicePSTNPtrGet(PSTNIdx);
	ZyPrintf("\n  *********Dump VoicePSTN %d************ \n ", PSTNIdx);
	if(NULL != VoicePSTN_p){	
		ZyPrintf("\n  forcePSTNTab = %s \n ", VoicePSTN_p->forcePSTNTab);
		ZyPrintf("\n  flags = %d \n ", VoicePSTN_p->flags);
		ZyPrintf("\n  reversed = %s \n ", VoicePSTN_p->reversed);
	}
}

/***************************************************************************
** Function Name: tr104DumpVoicePhoneBookObj
**
** Description  : Dump parameters
**                           
** Returns      : 
****************************************************************************/

void tr104DumpVoicePhoneBookObj(int PhoneBookIdx){
	ZyXEL_VoicePhoneBook_t* voicePhBook_p = NULL;
	voicePhBook_p = tr104VoicePhoneBookPtrGet(PhoneBookIdx);
	ZyPrintf("\n  *********Dump VoicePhoneBook %d************ \n ", PhoneBookIdx);
	if(NULL != voicePhBook_p){	
		ZyPrintf("\n  origNumber = %s \n ", voicePhBook_p->origNumber);
		ZyPrintf("\n  forceSipURI = %s \n ", voicePhBook_p->forceSipURI);
		ZyPrintf("\n  speedNumber = %s \n ", voicePhBook_p->speedNumber);
		ZyPrintf("\n  name = %s \n ", voicePhBook_p->name);
		ZyPrintf("\n  flags = %d \n ", voicePhBook_p->flags);
		ZyPrintf("\n  type = %d \n ", voicePhBook_p->type);
	}
}

int tr104FindLineIdFromRegisterUrl(char *regUrl, int typeUrl){
	char directoryNum[32+1] = {0};
	VoiceProfLineObj_t *lineObj = NULL;


	int LineItr = 0, VpItr=0 , SipAccountId = 0;
	uint32 NumOfProfile = 0;
	uint32 NumOfLine = 0;


	if(((sscanf(regUrl, "%[^@]@", directoryNum)) && typeUrl == ICF_ADDRESS_PLAN_SIP_URI) || ((sscanf(regUrl, "+%[^@]@", directoryNum)) && typeUrl == ICF_ADDRESS_PLAN_TEL_URI)){
		ZyPrintf("directoryNum %s\n", directoryNum);

		for (SipAccountId = 0; SipAccountId < sipAccountNumber; SipAccountId++)
		{
			VpItr = mapSipAccountIdToVpItr(SipAccountId);
			LineItr = mapSipAccountIdToLineItr(SipAccountId);
			lineObj=tr104VoiceLineObjPtrGet(VpItr,LineItr);
			if(NULL != lineObj){
				if(!strcmp(lineObj->directoryNumber, directoryNum)){
					return SipAccountId;
				}
			}		
		}
	
	}

	return -1;
}

//Ssu-Ying, support Multiple Profile
/*****************************************************************************
**  FUNCTION:       mapSipAccountIdToVpItr
**
**  PUROPOSE:       Maps the SipAccountId to the VpItr
**
**  INPUT PARMS:    SipAccountId
**
**  RETURNS:        VpItr value
**
*****************************************************************************/
int mapSipAccountIdToVpItr(int SipAccountId)
{
	ZyXEL_VoiceSipAccount_t * VoiceSipAccount = NULL;
	VoiceSipAccount = tr104VoiceSipAccountPtrGet(SipAccountId);
	if(NULL == VoiceSipAccount){
		return -1;
	}
	else{
		return VoiceSipAccount->VpItr;
	}
}

/*****************************************************************************
**  FUNCTION:       mapSipAccountIdToLineItr
**
**  PUROPOSE:       Maps the SipAccountId to the LineItr
**
**  INPUT PARMS:    SipAccountId
**
**  RETURNS:        VpItr value
**
*****************************************************************************/
int mapSipAccountIdToLineItr(int SipAccountId)
{
	ZyXEL_VoiceSipAccount_t * VoiceSipAccount = NULL;	
	VoiceSipAccount = tr104VoiceSipAccountPtrGet(SipAccountId);
	if( NULL == VoiceSipAccount){
		return -1;
	}
	else{
		return VoiceSipAccount->LineItr;	
	}
}

/*****************************************************************************
**  FUNCTION:       mapVpItrLineItrToSipAccountId
**
**  PUROPOSE:       Maps the LineItr and the VpItr to the SipAccountId
**
**  INPUT PARMS:    VpItr value , LineItr value
**
**  RETURNS:        SipAccountId
**
*****************************************************************************/
int mapVpItrLineItrToSipAccountId(int VpItr, int LineItr)
{
	int SipAccountId = 0, VpIdx = 0, LineIdx = 0;
	uint32 NumOfProfile = 0;
	uint32 NumOfLine = 0;
	
	NumOfProfile = tr104GetNumProfile();
	for(VpIdx=0;VpIdx<NumOfProfile;VpIdx++) {
		NumOfLine = tr104GetVPNumOfLine(VpIdx);
		for ( LineIdx=0; LineIdx<NumOfLine; LineIdx++ ) {
			if ((VpItr == VpIdx) && (LineItr == LineIdx))
			{
				return SipAccountId;
			}else {
				SipAccountId++;
			}
		}
	}
	return SipAccountId;
}

