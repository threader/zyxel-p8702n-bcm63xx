/***********************************************************************
 *
 *  Copyright (c) 2006-2007  Broadcom Corporation
 *  All Rights Reserved
 *
# 
# 
# This program may be linked with other software licensed under the GPL. 
# When this happens, this program may be reproduced and distributed under 
# the terms of the GPL. 
# 
# 
# 1. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
#    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
#    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH 
#    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND 
#    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, 
#    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR 
#    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE 
#    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR 
#    PERFORMANCE OF THE SOFTWARE. 
# 
# 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR 
#    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, 
#    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY 
#    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN 
#    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; 
#    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE 
#    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS 
#    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY 
#    LIMITED REMEDY. 
#
 *
 ************************************************************************/

#ifndef __VoIP_log_INCLUDE__
#define __VoIP_log_INCLUDE__

#define LOG_CALL_STATISTICS			 (1<<4)
#define LOG_SIP_CALL_SIGNALING		 (1<<3)
#define LOG_SIP_REGISTRATION		 (1<<2)
#define LOG_PHONE_EVENT				 (1<<1)
#define LOG_OTHERS					 (1<<0)
#define SYSLOG_FMT_SIZE			   220
#define TEXT_VOIP_SIZE			   170				
#define CATEGORY_MASK			  0x31

#define CALL_STATISTICS			"Call Statistics"
#define SIP_CALL_SIGNALING		"SIP call signaling"
#define SIP_REGISTRATION		"SIP registration"
#define PHONE_EVENT				"Phone event"
#define OTHERS					"Others"
#define MAX_LEN_VOIP 170
#define SERIAL_NUMBER_MAX_LEN		64
#define MODEL_NAME_MAX_LEN			64
#define MAC_ADDRESS_MAX_LEN			64
#define PHONE_NUMBER_MAX_LEN		64

#if 0
typedef enum
{
   LOG_LEVEL_ERR    = 3, /**< Message at error level. */
   LOG_LEVEL_NOTICE = 5, /**< Message at notice level. */
   LOG_LEVEL_INFO = 6    /**< Message at debug level. */
} VoIPLogLevel;
#endif
/*____________________________________________________________________________*/
/*	functions                                                                                                                     */
/*____________________________________________________________________________*/

//int voipSyslog(char *category, VoIPLogLevel level, char *Phone_Num, int Phone_line_Indicator, char *Content);

int voipSyslog(
	const int category, 			
	const int level, 				
	const char *Phone_Num, 			
	const int  Phone_line_Indicator, 	/*FXS1 or FXS2*/
	char *Content				
);


#endif /*__VoIP_log_INCLUDE__*/

