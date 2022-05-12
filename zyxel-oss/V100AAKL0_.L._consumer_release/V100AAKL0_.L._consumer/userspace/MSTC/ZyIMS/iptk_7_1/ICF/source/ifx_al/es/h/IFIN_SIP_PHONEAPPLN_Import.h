/**************************************************************************
 **   FILE NAME			:
 **   PROJECT			  :
 **   MODULES	      :
 **   SRC VERSION		: V1.0
 **
 **   DATE				  :
 **   AUTHOR			  :
 **   DESCRIPTION		:	
 **   FUNCTIONS			:	
 **   COMPILER			:
 **   REFERENCE			:
 **							
 **   COPYRIGHT			: Infineon Technologies AG 2003

 **  Version Control Section  **
 **   $Author: achhibber $
 **   $Date: 2006/04/17 04:02:28 $
 **   $Revisions$
 **   $Log: IFIN_SIP_PHONEAPPLN_Import.h,v $
 **   Revision 1.3  2006/04/17 04:02:28  achhibber
 **   no message
 **       Revision history
***********************************************************************/

#ifndef __IFIN_SIP_PHONEAPPLN_CFG_H__
#define __IFIN_SIP_PHONEAPPLN_CFG_H__

#include "IFIN_SIP_AddrIf.h"

/* Permission to be used while opening the FIFO's */
#define	IFIN_SIP_PA_PERMS			0666

/* Some macro's to define the size */
#define IFIN_SIP_MAX_DISPLAY_INFO   80


/* Signals from the Phone Application to the SIP Module */
typedef enum
{
	IFX_PA_OFF_HOOK=1,
	IFX_PA_ON_HOOK,
	IFX_PA_ACCEPT,
	IFX_PA_REJECT,
	IFX_PA_AWNSER,
	/* Added for Ver2.0 Supplementary service */
	IFX_PA_HOLD_EVENT,
	IFX_PA_BLIND_TX_EVENT,
	IFX_PA_ATTENDED_EVENT,
	IFX_PA_CONFERENCE_EVENT,
	IFX_PA_RESUME_EVENT,
	IFX_PA_TOGGLE_EVENT,
	IFX_PA_CALL_FWD_INFO,
	IFX_PA_LOGIN,
	IFX_PA_LOGOUT,
	IFX_PA_TX_INPROG_SUCC, /*Added After discussion*/
#ifdef  FAX_INTEG
   IFX_PA_FAX_CNG,
	IFX_PA_FAX_CED,
   IFX_PA_FAX_DIS
#endif
}e_PA2SIP_Signal;


/* Signals given by the SIP Module to the Phone application */
typedef enum
{
	IFX_INCOMINGCALL_SIGNAL =1,
	IFX_CALLPROG_SIGNAL,
	IFX_ALERT_SIGNAL,
	IFX_RELEASE_SIGNAL,/*BUSY*/
	IFX_CALLWAITING_SIGNAL,
	IFX_CONNECTED_SIGNAL,
	IFX_STOPTONE_SIGNAL,
  	IFX_SIP_INIT_SUCCESS,
  	IFX_SIP_INIT_FAIL,
	IFX_HOLD_CALL_SUCCESS,
	IFX_HOLD_CALL_FAIL ,
	IFX_BLIND_TX_SUCCESS,
	IFX_BLIND_TX_FAIL,
	IFX_ATTENDED_TX_SUCCESS,
	IFX_ATTENDED_TX_FAIL,
	IFX_TOGGLE_SUCCESS,
	IFX_TOGGLE_FAIL,
	IFX_CONFERENCE_SUCCESS,
	IFX_CONFERENCE_FAIL,
	IFX_TX_INPROGRESS,/*Added After discussion*/
	IFX_INFO_MESSAGE,
#ifdef ATA
        IFX_INT_INCOMING_SIGNAL=50,
#endif
	IFX_MAX_SIGNAL
} e_SIP2PA_Signal;

/*Added for supliment service*/ 
/*Call Forward Enum*/
typedef enum
{
  IFX_FWD_BUSY=1,
  IFX_FWD_NO_ANS,
  IFX_FWD_ALWAYS,
  IFX_FWD_DISABLE
}e_PAToSIP_CallFwd;

/* Message format from Phone Application to SIP */
typedef struct
{
	uchar8	ucChannelId;/* Channel Id */
	uint8 ucConnId1;/*Connection ID For First Call*/
	uint8 ucConnId2;/*Connection ID For Second Call*/
	e_PA2SIP_Signal	eEventId;/* Signal */
	e_PAToSIP_CallFwd eCallFwdType;/*Call Forward on type*/
	x_IFIN_SIP_CalledAddr xCalledAddr;/* Address */

}x_IFIN_SIP_PAMsg;
/* Message format from SIP to Phone Application */
typedef struct
{
	uchar8	ucChannelId;/* Channel Id */
	uint8 ucConnId1;/*Connection ID For First Call*/
	uint8 ucConnId2;/*Connection ID For Second Call*/
	e_SIP2PA_Signal	eEventId;/* Signal */
  union
  {
	  uchar8 acDisplay[IFIN_SIP_MAX_DISPLAY_INFO];/* Display Info Error Msg From SIP*/
	  x_IFIN_SIP_CalledAddr xCalledAddr;/* Address for incoming call signal*/
  }ux_SIPInfo;
}x_IFIN_PA_SIPMsg;



#endif /* __IFIN_SIP_PHONEAPPLN_CFG_H__ */


