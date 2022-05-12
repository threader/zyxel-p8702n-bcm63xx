/****************************************************************************
*
*  Copyright (c) 2004-2008 Broadcom Corporation
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
*    point for all call manager configuration data whether it comes from 
*    NVRAM, remote provisioning, or elsewhere. The source of each piece 
*    may be different depending on the availability of different provisioning
*    data sources.
*
****************************************************************************/
#ifndef _PROVISDATA_H
#define _PROVISDATA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vrgCmgrApi.h>

/* ---- Constants and Types ---------------------------------------------- */


typedef enum PROV_DATA_TYPE
{
   PROV_DATA_INT,
   PROV_DATA_BOOL,
   PROV_DATA_UINT8,
   PROV_DATA_UINT16,
   PROV_DATA_UINT32,
   PROV_DATA_STRING,
   PROV_DATA_MAX
} PROV_DATA_TYPE;

#define PARM_UNUSED        (-1)
#define PARM_INIT          (-2)

/* ---- Function Prototypes ---------------------------------------------- */

/*****************************************************************************
* FUNCTION:   provisInit
*
* PURPOSE:    Main entry point for provisioning.
*             This should be the first function called by the client
*             in order to activate provisioning.
*
* PARAMETERS: optional callback function to be informed of updates to 
*             provisioning data
*
* RETURNS:    0 on success, non-zero otherwise
*
*****************************************************************************/
int provisInit( void );

void provisDefault( void );

/*****************************************************************************
* FUNCTION:   getProvisData
*
* PURPOSE:    Retrieve the provisioned value from either NVRAM or remote
*
* PARAMETERS: dataID - identify which data value is to be retrieved
*
* RETURNS:    void * - non NULL if retrieval was successful
*
* NOTE:       A macro is used to allow provisioning error messages to print
*             the data ID as a string, even for custom provisioning IDs.
*****************************************************************************/
void *getProvisData( PROVIS_DATA_ID dataId, int parm, int parm2 );

/* Private function, always use the macro above instead. */
void *setProvisDataImpl( PROVIS_DATA_ID dataId, int parm, int parm2, void* data2, char *idString );

/* For setting parameters which are common across endpoints */
#define setProvisData1(ID, DATA)                   setProvisDataImpl(ID, PARM_UNUSED, PARM_UNUSED, DATA, #ID)
/* For setting parameters which are configurable per endpoint */
#define setProvisData2(ID, PARAM1, DATA)           setProvisDataImpl(ID, PARAM1, PARM_UNUSED, DATA, #ID)
/* For setting parameter arrays which are configurable per endpoint */
#define setProvisData3(ID, PARAM1, PARAM2, DATA)   setProvisDataImpl(ID, PARAM1, PARAM2, DATA, #ID)

/*****************************************************************************
* FUNCTION:   setProvisData
*
* PURPOSE:    Set the provisioned value
*
* PARAMETERS: dataID - identify which data value is to be retrieved
*
* RETURNS:    void * - non NULL if retrieval was successful
*
*****************************************************************************/
void *setProvisData( PROVIS_DATA_ID provItemId, int endpt, int index, void *pItemValue, int length );

/****************************************************************************
* FUNCTION:   enableProvisInit
*             disableProvisInit
*
* PURPOSE:    Set the provisioned initial flag TRUE or FALSE
*
* PARAMETERS: none
*
* RETURNS:    void
*
*****************************************************************************/
void enableProvisInit( void );
void disableProvisInit( void );


/*****************************************************************************
* FUNCTION:   showProvisSettings
*
* PURPOSE:    Debugging aid
*
* PARAMETERS: NONE
*
* RETURNS:    NONE
*
* NOTE:
*****************************************************************************/
void showProvisSettings( void );

/*****************************************************************************
* FUNCTION:   provisSetVodslLogLevel
*
* PURPOSE:    set log level for userspace app;Map mdm log levels to vodsl loglevels
*
* PARAMETERS: NONE
*
* RETURNS:    NONE
*
* NOTE:
*****************************************************************************/
int provisSetVodslLogLevel( void );

/* 
** Custom provisioning function interface definitions. If custom
** provisioning is enabled, these functions must be provided 
** elsewhere. They will be called internally by the provisioning
** functions provisInit() and getProvisData().
*/
#if PROV_CFG_CUSTOM_SUPPORT
int  provisCustomInit( void );
void *getProvisCustom( PROVIS_DATA_ID dataId, int parm );
#endif

#ifdef __cplusplus
}
#endif

#endif

