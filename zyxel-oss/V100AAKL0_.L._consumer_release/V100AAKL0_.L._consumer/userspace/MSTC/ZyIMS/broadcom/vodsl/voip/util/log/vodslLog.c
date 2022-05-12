/****************************************************************************
*
*  Copyright (c) 2006-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*  vodslLog.c
*
*  PURPOSE:
*
*     Logging implementation for tasks in vodsl process.
*
*  NOTES:
*
****************************************************************************/

#include <stdio.h>
#include <time.h>
#include "vodslLog.h"

VODSL_LOGLEVEL logLevel[VODSL_MODULE_MAX] = { VODSL_LOG_NOTICE, VODSL_LOG_NOTICE };

/* Highest Log level */
void vodslLogInfo( const char *format,...)                
{                          
   va_list varArgList;
   
   if ( VODSL_LOG_DEBUG <= logLevel[VODSL_MODULE_GENERAL] )
   {
      va_start (varArgList, format);
      vodslPrint (format, varArgList);
      va_end (varArgList);
   }
}

/* Medium Log level */
void vodslLog( const char *format,...)                
{                          
   va_list varArgList;
   if ( VODSL_LOG_NOTICE <= logLevel[VODSL_MODULE_GENERAL] )
   {
      va_start (varArgList, format);
      vodslPrint (format, varArgList);
      va_end (varArgList);
   }
}

/* Lowest Log level */
void vodslLogErr( const char *format,...)                
{                          
   va_list varArgList;
   
   if ( VODSL_LOG_ERROR <= logLevel[VODSL_MODULE_GENERAL] )
   {
      va_start (varArgList, format);
      vodslPrint (format, varArgList);
      va_end (varArgList);
   }
}


void vodslPrint( const char *format, va_list va)
{
   struct tm *tm_ptr;
   time_t curtime;

   time( &curtime );    
   tm_ptr = gmtime( &curtime );
   printf("%02d:%02d:%02d ",
            tm_ptr->tm_hour,
            tm_ptr->tm_min,
            tm_ptr->tm_sec);

   vprintf (format, va);

   printf("\n");  
}

VODSL_LOGLEVEL vodslGetLogLevel(VODSL_MODULE module)
{
   if (module < VODSL_MODULE_MAX)
   {
      return logLevel[module];
   } 
   else 
   {
      return VODSL_LOG_OFF;
   }
}

void vodslSetLogLevel(VODSL_LOGLEVEL level)
{  
   int i;
   for (i=0; i< VODSL_MODULE_MAX; i++)
   {
      logLevel[i] = level;
   }
}

