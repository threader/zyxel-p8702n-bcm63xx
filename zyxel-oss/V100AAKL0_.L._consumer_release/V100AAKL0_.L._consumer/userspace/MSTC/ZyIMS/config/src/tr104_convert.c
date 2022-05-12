/*
   $Id: tr104_convert.c 0.00.0.1 2008/11/12 14:51 ctpan Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 ZyXEL Communications, Corp.
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
 * $Log: tr104_convert.c $
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "icf_defs.h"

#include "global.h"
#include "tr104_global.h"
#include "tr104.h"
#include "tr104_object.h"
#include "tr104_convert.h"
#include "endpt.h"


/***************************************************************************
** Function Name: tr104_Convert_Enable
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/


int 
tr104ConvertEnable(char * str)
{
   int ret = 0;	
 
   /* Copy Line enabled value */
   if( str != NULL )
   {
		TR104_DBGS1( "enable status = ", str);
	
      /* convert line status string to boolean value */
      if( zyStrcmp("Enabled" , str ) == 0 )
      {
		ret = TR104_TRUE;
      }
      else if( zyStrcmp("Disabled" , str) == 0 )
      {
		ret = TR104_FALSE;
      }
      else
      {
         /* Brcm doesn't support quenscient state */
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
      }	  
   }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    }	
   return ret;
}


/***************************************************************************
** Function Name: tr104ConvertTransport
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertTransport(char * str)
{
   int ret = 0;	
   
   /* Copy Line enabled value */
   if( str != NULL ){

	 TR104_DBGS1( "Transport status = ", str);
      /* convert line status string to boolean value */
      if( zyStrcmp("UDP" , str ) == 0 ){
		ret = TR104I_TRANSPORT_UDP;
      }
      else if( zyStrcmp("TCP" , str) == 0 ){
		ret = TR104I_TRANSPORT_TCP;
      }
	else if( zyStrcmp("TLS" , str) == 0 ){
		ret = TR104I_TRANSPORT_TLS;
      }
	else if( zyStrcmp("SCTP" , str) == 0 ){
		ret = TR104I_TRANSPORT_SCTP;
      }
      else
      {
		 ret = ICF_TRANSPORT_TYPE_INVALID;
      }	  
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = ICF_TRANSPORT_TYPE_INVALID;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertPassthrough
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertPassthrough(char * str)
{
   int ret = 0;	
   
   /* Copy Line enabled value */
   if( str != NULL )
   {
	 TR104_DBGS1( "Passthrough status = ", str);
	
      /* convert line status string to boolean value */
      if( zyStrcmp("Disable" , str ) == 0 ){
		ret = TR104I_PASSTHROUGH_DISABLE;
      }else if( zyStrcmp("Force" , str) == 0 ){
		ret = TR104I_PASSTHROUGH_FORCE;
      }else if( zyStrcmp("Auto" , str) == 0 ){	/*Reserve*/
		/*ret = TR104I_PASSTHROUGH_AUTO;*/
		ret = TR104I_PASSTHROUGH_FORCE;
      }
      else
      {
       	 ZyPrintf( "INVALID parameter value :  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
      }	  
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}
/***************************************************************************
** Function Name: tr104ConvertDTMFMethod
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertDTMFMethod(char * str)
{
	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){
	 TR104_DBGS1( "DTMFMethod status = ", str);	
      /* convert line status string to boolean value */
      if( zyStrcmp("InBand" , str ) == 0 ){/*MDMVS_INBAND*/
		ret = TR104I_DTMF_INBAND;
      }
      else if( zyStrcmp("RFC2833" , str) == 0 ){/*MDMVS_RFC2833*/
		ret = TR104I_DTMF_RFC2833;
      }
	else if( zyStrcmp("SIPInfo" , str) == 0 ){/*MDMVS_SIPINFO*/
		ret = TR104I_DTMF_SIPINFO;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    }	
   }else{	/*see TR104 , empty is a posiblle case*/
		 ret = TR104I_DTMF_NULL;
   }
   return ret;

}

/***************************************************************************
** Function Name: tr104ConvertInBoundAuth
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/int 
tr104ConvertInBoundAuth(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){

	 TR104_DBGS1( "InBoundAuth status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("None" , str ) == 0 ){
		ret = TR104I_INBOUNDAUTH_NONE;
      }
	else if( zyStrcmp("Digest" , str) == 0 ){
		ret = TR104I_INBOUNDAUTH_DIGEST;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}

/***************************************************************************
** Function Name: tr104ConvertTCFMethod
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/int
tr104ConvertTCFMethod(char* str)
{
	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){
	 TR104_DBGS1( "passthrough status = ", str);	
      /* convert line status string to boolean value */
      if( zyStrcmp("Local" , str ) == 0 ){
		ret = TR104I_TCFMETHOD_LOCAL;
      }
      else if( zyStrcmp("Network" , str) == 0 ){
		ret = TR104I_TCFMETHOD_NETWORK;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertLineStatus
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int
tr104ConvertLineStatus(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){
	 TR104_DBGS1( "Status status = ", str);	
      /* convert line status string to boolean value */
      if( zyStrcmp("Up" , str ) == 0 ){
		ret = TR104I_STATUS_UP;
      }else if( zyStrcmp("Initializing" , str) == 0 ){
		ret = TR104I_STATUS_INITIALIZING;
      }else if( zyStrcmp("Registering" , str) == 0 ){
		ret = TR104I_STATUS_REGISTERING;
      }else if( zyStrcmp("Unregistering" , str) == 0 ){
		ret = TR104I_STATUS_UNREGISTERING;
      }else if( zyStrcmp("Error" , str) == 0 ){
		ret = TR104I_STATUS_ERROR;
      }else if( zyStrcmp("Testing" , str) == 0 ){
		ret = TR104I_STATUS_TESTING;
      }else if( zyStrcmp("Quiescent" , str) == 0 ){
		ret = TR104I_STATUS_QUIECCENT;
      }else if( zyStrcmp("Disabled" , str) == 0 ){
		ret = TR104I_STATUS_DISABLED;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertCallState
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int
tr104ConvertCallState(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){
	 TR104_DBGS1( "CallState status = ", str);
	
      /* convert line status string to boolean value */
      if( zyStrcmp("Idle" , str ) == 0 ){
		ret = TR104I_STATE_IDLE;
      }else if( zyStrcmp("Calling" , str) == 0 ){
		ret = TR104I_STATE_CALLING;
      }else if( zyStrcmp("Ringing" , str) == 0 ){
		ret = TR104I_STATE_RINGING;
      }else if( zyStrcmp("Connecting" , str) == 0 ){
		ret = TR104I_STATE_CONNECTING;
      }else if( zyStrcmp("InCall" , str) == 0 ){
		ret = TR104I_STATE_INCALL;
      }else if( zyStrcmp("Hold" , str) == 0 ){
		ret = TR104I_STATE_HOLD;
      }else if( zyStrcmp("Disconnecting" , str) == 0 ){
		ret = TR104I_STATE_DISCONNECTING;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}
/***************************************************************************
** Function Name: tr104ConvertRegType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int 
tr104ConvertRegType(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){

	 TR104_DBGS1( "RegType status = ", str);	
	
      /* convert line status string to boolean value */
      if( zyStrcmp("Reg_On" , str ) == 0 ){
		ret = TR104I_REGTYPE_ON;
      }else if( zyStrcmp("Reg_Off" , str) == 0 ){
		ret = TR104I_REGTYPE_OFF;
      	}else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}
/***************************************************************************
** Function Name: tr104ConvertURLType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int
tr104ConvertURLType(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){

	TR104_DBGS1( "URLType status = ", str);	
      /* convert line status string to boolean value */
      if( zyStrcmp("URL_Tel" , str ) == 0 ){
		ret = ICF_ADDRESS_PLAN_TEL_URI;
      }else if( zyStrcmp("URL_Sip" , str) == 0 ){
		ret = ICF_ADDRESS_PLAN_SIP_URI;
      	}
	#if 1 /*Jason,SIP over TLS*/
	else if( zyStrcmp("URL_Sips", str)==0 ){
		ret = ICF_ADDRESS_PLAN_SIPS_URI;
	} 
	#endif
	  else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertMixMode
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertMixMode(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){
	TR104_DBGS1( "MixerMode status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("RemoteMix" , str ) == 0 ){
		ret = TR104I_MIXMODE_REMOTE;
      }else if( zyStrcmp("LocalMix" , str) == 0 ){
		ret = TR104I_MIXMODE_LOCAL;
      	}else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertRemoteRingBack
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertRemoteRingback(char* str)
{

	int ret = 0;	
	   /* Copy Line enabled value */
   if( str != NULL ){
	TR104_DBGS1( "remoteRingback status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("Inactive" , str ) == 0 ){
		ret = TR104I_REMOTE_RINGBACK_INACTIVE;
      }else if( zyStrcmp("Active" , str) == 0 ){
		ret = TR104I_REMOTE_RINGBACK_ACTIVE;
      	}else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertDialMethod
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertDialMethod(char* str)
{

	int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "DialMethod status = ", str);			
      /* convert line status string to boolean value */
      if( zyStrcmp("DialMethod_EUROP" , str ) == 0 ){
		ret = CALL_FEATURE_DIAL_METHOD_EUROP;
      }else if( zyStrcmp("DialMethod_USA" , str) == 0 ){
		ret = CALL_FEATURE_DIAL_METHOD_USA;
      	}else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}

/***************************************************************************
** Function Name: tr104ConvertCWStatus
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int 
tr104ConvertCWStatus(char* str)
{

	int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "CWStatus status = ", str);	
      /* convert line status string to boolean value */
      if( zyStrcmp("Disabled", str ) == 0 ){
		ret = TR104I_CWSTATUS_DISABLED;
      }else if( zyStrcmp("Idle" , str) == 0 ){
		ret = TR104I_CWSTATUS_IDLE;
      }else if( zyStrcmp("SecondaryRinging" , str) == 0 ){
		ret = TR104I_CWSTATUS_SECONDARYRINGING;
      }else if( zyStrcmp("SecondaryConnecting" , str) == 0 ){
		ret = TR104I_CWSTATUS_SECONDARYCONNECTING;
      }else if( zyStrcmp("SecondaryConnected" , str) == 0 ){
		ret = TR104I_CWSTATUS_SECONDARYCONNECTED;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}


/***************************************************************************
** Function Name: tr104ConvertConfCallStatus
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int 
tr104ConvertConfCallStatus(char* str)
{
	int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "ConfCallStatus status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("Disabled" , str ) == 0 ){
		ret = TR104I_CWSTATUS_DISABLED;
      }else if( zyStrcmp("Idle" , str) == 0 ){
		ret = TR104I_CWSTATUS_IDLE;
      }else if( zyStrcmp("SecondaryRinging" , str) == 0 ){
		ret = TR104I_CWSTATUS_SECONDARYRINGING;
      }else if( zyStrcmp("SecondaryConnecting" , str) == 0 ){
		ret = TR104I_CWSTATUS_SECONDARYCONNECTING;
      }else if( zyStrcmp("SecondaryConnected" , str) == 0 ){
		ret = TR104I_CWSTATUS_SECONDARYCONNECTED;
      }else if( zyStrcmp("InConferenceCall" , str) == 0 ){
		ret = TR104I_CCSTATUS_INCONFERENCECALL;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}

//Ssu-Ying, support Multiple Profile
#if 0
/***************************************************************************
** Function Name: tr104ConvertPhyIntfType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int 
tr104ConvertPhyIntfType(char* str)
{

	int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "PhyIntfType status = ", str);			
      /* convert line status string to boolean value */
      if( zyStrcmp("PhyIntf_FXS" , str ) == 0 ){
		ret = TR104I_PHYINTF_FXS;
      }else if( zyStrcmp("PhyIntf_FXO" , str) == 0 ){
		ret = TR104I_PHYINTF_FXO;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}
#endif

/***************************************************************************
** Function Name: tr104ConvertPortsType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int 
tr104ConvertPortsType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "PortsType status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("LoopStart" , str ) == 0 ){
		ret = LOOP_START;
      }else if( zyStrcmp("GroundStart" , str) == 0 ){
		ret = GROUND_START;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}

/***************************************************************************
** Function Name: tr104ConvertDialingType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int
tr104ConvertDialingType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "DialingType status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("DialingTypeAuto" , str ) == 0 ){
		ret = AUTOMATIC;
      }else if( zyStrcmp("DialingTypeTone" , str) == 0 ){
		ret = TONE;
      }else if( zyStrcmp("DialingTypePulse" , str) == 0 ){
		ret = PULSE;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}


/***************************************************************************
** Function Name: tr104ConvertDialToneType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/

int 
tr104ConvertDialToneType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "DialToneType status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("NTTDialTone" , str ) == 0 ){
		ret = NTT_DIAL_TONE;
      }else if( zyStrcmp("PrivateDialTone" , str) == 0 ){
		ret = PRIVATE_DIAL_TONE;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertReceiveToneType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertReceiveToneType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "ReceiveToneType status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("RecvToneIR" , str ) == 0 ){
		ret = IR;
      }else if( zyStrcmp("RecvToneVOIPSIR" , str) == 0 ){
		ret = VOIP_SIR;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}


/***************************************************************************
** Function Name: tr104ConvertCallerIDType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertCallerIDType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "CallIDType status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("DuringRing" , str ) == 0 ){
		ret = DURING_RING;
      }else if( zyStrcmp("PriorRing" , str) == 0 ){
		ret = PRIOR_RING;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}


/***************************************************************************
** Function Name: tr104ConvertCallerIDPayloadType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int
tr104ConvertCallerIDPayloadType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "CallerIDPayloadType status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("MDMFCID" , str ) == 0 ){
		ret = MDMF_CID;
      }else if( zyStrcmp("SDMFCID" , str) == 0 ){
		ret = SDMF_CID;
      }else if( zyStrcmp("ETSIMDMFCID" , str) == 0 ){
		ret = ETSI_MDMF_CID;
	}else if( zyStrcmp("DTMFCID" , str) == 0 ){
		ret = DTMF_CID;
      }else if( zyStrcmp("UKCID" , str) == 0 ){
		ret = UK_CID;
	}else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}


/***************************************************************************
** Function Name: tr104ConvertTASType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertTASType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "TASType status = ", str);			
      /* convert line status string to boolean value */
      if( zyStrcmp("LINEREVE" , str ) == 0 ){
		ret = LINE_REVE;
      }else if( zyStrcmp("RPAS" , str) == 0 ){
		ret = RP_AS;
      }else if( zyStrcmp("DTAS" , str) == 0 ){
		ret = DT_AS;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
}

/***************************************************************************
** Function Name: tr104ConvertPhoneBookFlag
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertPhoneBookFlag(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "PhoneBookFlag status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("SDConfigure" , str ) == 0 ){
		ret = SPEED_DIAL_CONFIGURE;
      }else if( zyStrcmp("SDActive" , str) == 0 ){
		ret = SPEED_DIAL_ACTIVE;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;


}

/***************************************************************************
** Function Name: tr104ConvertPhoneBookType
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertPhoneBookType(char* str)
{

   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "PhoneBookType status = ", str);			
      /* convert line status string to boolean value */
      if( zyStrcmp("SDSipProxy" , str ) == 0 ){
		ret = SPEED_DIAL_SIP_PROXY;
      }else if( zyStrcmp("SDSipNonProxy" , str) == 0 ){
		ret = SPEED_DIAL_SIP_NONPROXY;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}
/***************************************************************************
** Function Name: tr104ConvertRingerEventFunction
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertRingerEventFunction(char* str)
{
	
   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "RingerEventFunction status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("Default" , str ) == 0 ){
		ret = RINGER_EVENT_DEFAULT;
      }else if( zyStrcmp("RingSplash" , str) == 0 ){
		ret = RINGER_EVENT_RINGSPLASH;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}
/***************************************************************************
** Function Name: tr104ConvertToneEventFunction
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int
tr104ConvertToneEventFunction(char* str)
{
	
   int ret = 0;	
   if( str != NULL ){
	TR104_DBGS1( "ToneEventFunction status = ", str);		
      /* convert line status string to boolean value */
      if( zyStrcmp("Busy" , str ) == 0 ){
		ret = TONE_EVENT_BUSY;
      }else if( zyStrcmp("Confirmation" , str) == 0 ){
		ret = TONE_EVENT_CONFIRMATION;
      }else if( zyStrcmp("Dial" , str) == 0 ){
		ret = TONE_EVENT_DIAL;
      }else if( zyStrcmp("MessageWaiting" , str) == 0 ){
		ret = TONE_EVENT_MESSAGEWAITING;
      }else if( zyStrcmp("OffHookWarning" , str) == 0 ){
		ret = TONE_EVENT_OFFHOOKWARNING;
      }else if( zyStrcmp("RingBack" , str) == 0 ){
		ret = TONE_EVENT_RINGBACK;
      }else if( zyStrcmp("ReOrder" , str) == 0 ){
		ret =TONE_EVENT_REORDER ;
      }else if( zyStrcmp("Stutterdial" , str) == 0 ){
		ret = TONE_EVENT_STUTTERDIAL;
      }else if( zyStrcmp("CallWaiting1" , str) == 0 ){
		ret =TONE_EVENT_CW1 ;
      }else if( zyStrcmp("CallWaiting2" , str) == 0 ){
		ret = TONE_EVENT_CW2;
      }else if( zyStrcmp("CallWaiting3" , str) == 0 ){
		ret =TONE_EVENT_CW3 ;
      }else if( zyStrcmp("CallWaiting4" , str) == 0 ){
		ret = TONE_EVENT_CW4;
      }else if( zyStrcmp("AlertingSignal" , str) == 0 ){
		ret = TONE_EVENT_ALERTINGSIGNAL;
      }else if( zyStrcmp("SpecialDial" , str) == 0 ){
		ret = TONE_EVENT_SPECIALDIAL;
      }else if( zyStrcmp("SpecialInfo" , str) == 0 ){
		ret = TONE_EVENT_SPECIALINFO;
      }else if( zyStrcmp("Release" , str) == 0 ){
		ret = TONE_EVENT_RELEASE;
      }else if( zyStrcmp("Congestion" , str) == 0 ){
		ret = TONE_EVENT_CONGESTION;
      }else if( zyStrcmp("UserDefined1" , str) == 0 ){
		ret = TONE_EVENT_USERDEF1;
      }else if( zyStrcmp("UserDefined2" , str) == 0 ){
		ret =TONE_EVENT_USERDEF2 ;
      }else if( zyStrcmp("UserDefined3" , str) == 0 ){
		ret = TONE_EVENT_USERDEF3;
      }else if( zyStrcmp("UserDefined4" , str) == 0 ){
		ret = TONE_EVENT_USERDEF4;
      }else{
       	 ZyPrintf( "INVALID parameter value \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
    	}	
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;

}

/***************************************************************************
** Function Name: tr104ConvertLoggingLevel
**
** Description  : Convert the string to int type
**                           
** Returns      : CMSRET_SUCCESS when successful.
****************************************************************************/
int 
tr104ConvertLoggingLevel(char* str)
{

   int ret = 0;	
   if( str != NULL ){
		TR104_DBGS1( "LoggingLevel status = ", str);		
	      /* convert line status string to boolean value */
	      if( zyStrcmp(str , "Debug") == 0 )
		{
			ret = ZyXEL_LOG_DEBUG;
	      }
		else if( zyStrcmp(str , "Notice") == 0 )
		{
			ret = ZyXEL_LOG_NOTICE;
	      }
		else
		{
			 ret = ZyXEL_LOG_ERROR;
	   	}
   }else{
		ZyPrintf( "Pointer is NULL  \n" );
		 ret = TR104RET_INVALID_PARAM_VALUE;
   }
   return ret;
   
}

/*______________________________________________________________________________
**  tr104ConvertCodecEntryID
**  descriptions: convert the Codec EntryId (defined in CMS) to the Codec Number that we need. 
**  parameters: entryId (defined by CMS)
**  return:  the Mapping Codec Number on ZyNOS
**  called by: 
**		tr104LoadMdmVoiceLineCodecListCfg -- InternetGatewayDevice.Services.VoiceService.{i}.VoiceProfile.{i}.Line.{i}.Codec.List.{i}.
**		tr104LoadMdmVoiceCapCodecsCfg --InternetGatewayDevice.Services.VoiceService.{i}.Capabilities.Codecs.{i}.
**  call:
**____________________________________________________________________________*/
int
tr104ConvertCodecEntryID(int entryId )
{
	int ret=0;

	if(entryId==CODEC_PCMU){
		ret=ZYNOS_CODEC_PCMU;
	}else if(entryId==CODEC_PCMA){
		ret=ZYNOS_CODEC_PCMA;
	}else if(entryId==CODEC_G729){
		ret=ZYNOS_CODEC_G729;
	}else if(entryId==CODEC_G7231_63){
		ret=ZYNOS_CODEC_G723;
	}else if(entryId==CODEC_G726_24){
		ret=ZYNOS_CODEC_G726_24;
	}else if(entryId== CODEC_G726_32){
		ret=ZYNOS_CODEC_G726_32;
	}else if(entryId==CODEC_G728){
		ret=ZYNOS_CODEC_G728;
	}else if(entryId==CODEC_G722_MODE_1){
		ret=ZYNOS_CODEC_G722MODE1;
	}else if(entryId==CODEC_T38){
		ret=ZYNOS_CODEC_T38;
	}else if(entryId==CODEC_NTE ){
		ret=ZYNOS_CODEC_RFC2833;
	}else{
		TR104_DBG(" Unknown Codec Type ");
		ret= ZYNOS_CODEC_UNKNOWN;
	}
 	return ret;
}

/*______________________________________________________________________________
**  tr104ConvertDialPlan
**  descriptions: for XML syntax , we will parse the "&lt;" to '<' and the "&gt;" to '>' . 
**  parameters: original :MDM , result : TR104
**  return:  TR104_SUCCESS , TR104_ERROR , result 
**  called by: 
**  call:
**____________________________________________________________________________*/
int  
tr104ConvertDialPlan(char* result  , char* original)
{
	char *head, *tail, *index ;
	int  ret=TR104_SUCCESS;
	if(original == NULL){
		zyStrcpy(result,"");
		TR104_DBG(" DialPlan Original is Null Pointer ");
		ret = TR104_ERROR;
	}else{
		zyStrcpy(result,original);

		TR104_DBGS1("before convert : result = ",result);
		index=result;
		while( head = strchr(index, '&') ) {/* there is gateway syntax in this rule */
			index = head + 1;
			if( (tail = strchr(index, ';')) == NULL ) {
				ZyPrintf("There is invalid syntax in this rule. Bad Dial Plan!\n\r");
		            ret=TR104_ERROR;
					break;
		        } else {
		            *tail = '\0';
		            if(zyStrcmp(index, "lt") == 0){/*DSP_FXS_PORT*/
		            		TR104_DBG("match lt");
		                *head ='<';
		            }else if(zyStrcmp(index, "gt") == 0){/*DSP_FXO_PORT*/
		            		TR104_DBG("match gt");
		                *head ='>';
		            }else{
				  ZyPrintf("There is invalid syntax in this rule. Bad Dial Plan!\n\r");
				ret=TR104_ERROR;
		               break;
		            }	
				head=head+1;
				zyStrcpy(head, (char *)(tail + 1));/* cover '=gwx=' with following digits */
				index=head;
				TR104_DBGS1("change : ret = ",result);
		        }	
		 }
		if(ret==TR104_ERROR){
			zyStrcpy(result,"");
		}
		TR104_DBGS1(" after convert : result= ",result);
	}
	
	return ret;
}

