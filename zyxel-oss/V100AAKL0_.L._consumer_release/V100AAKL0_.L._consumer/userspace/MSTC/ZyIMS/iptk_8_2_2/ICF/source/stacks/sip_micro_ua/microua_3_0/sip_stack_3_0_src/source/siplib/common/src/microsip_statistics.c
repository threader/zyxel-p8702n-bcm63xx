/***********************************************************************
 ** FUNCTION:
 **		Statistics APIs for stack

 *********************************************************************
 **
 ** FILENAME:
 ** microsip_statistics.c
 **
 ** DESCRIPTION:
 ** This file handles the statistics module
 **
 ** DATE	  NAME		REFERENCE		REASON
 ** ----	  ----		---------	       --------
 **23-03-2002 mahesh 	Core Stack
 **
 **	COPYRIGHT , Aricent, 2006
 *********************************************************************/

#include "microsip_statistics.h"

SIP_U32bit glbSipParserMemErr;
SIP_U32bit glbSipParserNetErr;
SIP_U32bit glbSipParserProtoErr;
SIP_U32bit glbSipParserApiCount;
SIP_U32bit glbSipParserReqParsed;
SIP_U32bit glbSipParserRespParsed;
SIP_U32bit glbSipParserReqSent;
SIP_U32bit glbSipParserRespSent;

#ifdef SIP_THREAD_SAFE
synch_id_t glbLockStatisticsMutex;
#endif


/*******************************************************************************
** FUNCTION: sip_initStatistics
** DESCRIPTION: inits Statistics
** 
** PARAMETERS:
**      	      dType(IN)			:The error Level
**				 scope(IN)			:The scope from  which statistics has 
**										to be collected
**				  err(IN)			:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/
SipBool sip_initStatistics
#ifdef ANSI_PROTO
 (SIP_U8bit dType,SIP_U8bit scope,SipError *err)
#else
 (dType, scope, err)
  SIP_U8bit dType;
  SIP_U8bit scope;
  SipError *err;
#endif
{
#ifndef SIP_STATISTICS
	*err = E_STATS_DISABLED;
	dType=dType;   /* just to remove warning */
	scope=scope;
	return SipFail;
#else
	switch(dType)
	{
		case SIP_STAT_TYPE_API	:
			switch(scope)
			{
				case SIP_STAT_API_ALL :
					glbSipParserApiCount=0;
					glbSipParserReqParsed=0;
					glbSipParserRespParsed=0;
					glbSipParserReqSent=0;
					break;
				case SIP_STAT_API_COUNT :
					glbSipParserApiCount=0;
					break;
				case SIP_STAT_API_REQ_PARSED :
					glbSipParserReqParsed=0;
					break;
				case SIP_STAT_API_RESP_PARSED :
					glbSipParserRespParsed=0;
					break;
				case SIP_STAT_API_REQ_SENT :
					glbSipParserReqSent=0;
					break;
				case SIP_STAT_API_RESP_SENT :
					glbSipParserRespSent=0;
					break;
				default:
					*err = E_INV_PARAM;
					return SipFail;
			}
			break;
		case SIP_STAT_TYPE_ERROR:
			switch(scope)
			{
				case SIP_STAT_ERROR_ALL :
					glbSipParserMemErr=0;
					glbSipParserNetErr=0;
					glbSipParserProtoErr=0;
					break;
				case SIP_STAT_ERROR_MEM :
					glbSipParserMemErr=0;
					break;
				case SIP_STAT_ERROR_NETWORK :
					glbSipParserNetErr=0;
					break;
				case SIP_STAT_ERROR_PROTOCOL :
					glbSipParserProtoErr=0;
					break;
				default:
					*err = E_INV_PARAM;
					return SipFail;
			}
			break;
		case SIP_STAT_TYPE_INTERNAL:
			break;
		default:
			*err = E_INV_STATSTYPE;
			return SipFail;
	}
	*err = E_NO_ERROR;
	return SipSuccess;
#endif

}
/*******************************************************************************
** FUNCTION: sip_getStatistics
** DESCRIPTION: Retrieves particular statistics
** 
** PARAMETERS:
**      	      	dType(IN)			:The error Level
**				 	scope(IN)			:The scope from  which statistics has 
**					reset(IN)			:Whether to reset the statistics.
**				  	err(IN)				:The error Value
** Return Value:
**				  SipBool	
*******************************************************************************/

SipBool sip_getStatistics
#ifdef ANSI_PROTO
 ( SIP_U8bit dType,SIP_U8bit scope,SIP_U8bit reset,SIP_Pvoid stats,SipError *err)
#else
 (dType, scope, reset, stats, err)
 SIP_U8bit dType;
 SIP_U8bit scope;
 SIP_U8bit reset;
 SIP_Pvoid stats;
 SipError *err;
#endif
{
#ifndef SIP_STATISTICS
	*err =	E_STATS_DISABLED;
	dType=dType; /* just to remove warning */
	scope=scope;
	reset=reset;
	stats=stats;
	return SipFail;
#else
	switch(dType)
	{
		case SIP_STAT_TYPE_API	:
			switch(scope)
			{
				case SIP_STAT_API_COUNT :
					*((SIP_U32bit *)stats)=glbSipParserApiCount;
					if(reset == SIP_STAT_RESET)
						glbSipParserApiCount = 0;
					break;
				case SIP_STAT_API_REQ_PARSED :
					*((SIP_U32bit *)stats)=glbSipParserReqParsed;
					if(reset == SIP_STAT_RESET)
						glbSipParserReqParsed = 0;
					break;
				case SIP_STAT_API_RESP_PARSED :
					*((SIP_U32bit *)stats)=glbSipParserRespParsed;
					if(reset == SIP_STAT_RESET)
						glbSipParserRespParsed = 0;
					break;
				case SIP_STAT_API_REQ_SENT :		
					*((SIP_U32bit *)stats)=glbSipParserReqSent;
					if(reset == SIP_STAT_RESET)
						glbSipParserReqSent = 0;
					break;
				case SIP_STAT_API_RESP_SENT :		
					*((SIP_U32bit *)stats)=glbSipParserRespSent;
					if(reset == SIP_STAT_RESET)
						glbSipParserRespSent = 0;
					break;
				default:
					*err = E_INV_PARAM;
					return SipFail;
			}
			break;
		case SIP_STAT_TYPE_ERROR:
			switch(scope)
			{
				case SIP_STAT_ERROR_MEM :			
					*((SIP_U32bit *)stats)=glbSipParserMemErr;
					if(reset == SIP_STAT_RESET)
						glbSipParserMemErr = 0;
					break;
				case SIP_STAT_ERROR_NETWORK :		
					*((SIP_U32bit *)stats)=glbSipParserNetErr;
					if(reset == SIP_STAT_RESET)
						glbSipParserNetErr = 0;
					break;
				case SIP_STAT_ERROR_PROTOCOL :		
					*((SIP_U32bit *)stats)=glbSipParserProtoErr;
					if(reset == SIP_STAT_RESET)
						glbSipParserProtoErr = 0;
					break;
				default:
					*err = E_INV_PARAM;
					return SipFail;
			}
			break;
		case SIP_STAT_TYPE_INTERNAL:
		default:
			*err = E_INV_PARAM;
			return SipFail;
	}
	*err = E_NO_ERROR;
	return SipSuccess;
#endif
}

