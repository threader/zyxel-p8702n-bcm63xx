/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
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
*    Filename: endpoint_user.h
*
****************************************************************************
*    Description:
*
*      Endpoint Abstraction Layer for the Linux User.
*      The actual endpoint functions are implemented in the kernel. 
*
****************************************************************************/

#include <vrgEndpt.h>
#include <endpt.h>

/*****************************************************************************
*  FUNCTION:   vrgEndptCapabilities
*
*  PURPOSE:    Retrieve the capabilities for an endpoint
*
*  PARAMETERS: endptState   - state of the endpt object
*              capabilities - a pointer to the capabilities struct to populate
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptCapabilities( ENDPT_STATE *endptState, EPZCAP* capabilities );


/*
*****************************************************************************
** FUNCTION:   vrgEndptSignal
**
** PURPOSE:    Generate a signal on the endpoint (or connection)
**
** PARAMETERS: endptState  - endpt object.
**             cnxId       - connection identifier (-1 if not applicable)
**             signal      - signal type code (see EPSIG)
**             value       - signal value
**                            BR signal types - 1
**                            OO signal types - 0 == off, 1 == on
**                            TO signal types - 0 = stop/off, 1= start/on
**                            String types - (char *) cast to NULL-term string value
**              duration   - pulse duration (-1 if not used)
**              period     - pulse period (-1 if not used)
**              repetition - depending on the pulsed signal, this parameter
**                           has different meaning:
**                             - 'em': defines the report pulse count (rpc)
**                             - other pulsed signals: defines the repetition (rep)
**                           (-1 if not used)
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptSignal( ENDPT_STATE *endptState, int cnxId, EPSIG signal, unsigned int value, int duration, int period, int repetition );


/*****************************************************************************
*  FUNCTION:   vrgEndptCreateConnection
*
*  PURPOSE:    Function to create a connection
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier
*              cnxParam    - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptCreateConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );


/*****************************************************************************
*  FUNCTION:   vrgEndptModifyConnection
*
*  PURPOSE:    Function to modify a connection
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier
*              cnxParam    - ptr to connection parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptModifyConnection( ENDPT_STATE *endptState, int cnxId, EPZCNXPARAM *cnxParam );


/*****************************************************************************
*  FUNCTION:   vrgEndptDeleteConnection
*
*  PURPOSE:    Function to delete a connection
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId );

/*
*****************************************************************************
** FUNCTION:   vrgEndptMuteConnection
**
** PURPOSE:    Toggle the mute status of a connection on the endpoint.
**
** PARAMETERS: endptState  - endpt object.
**             cnxId        - connection identifier
**             mute        - mute status for the connection
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptMuteConnection ( ENDPT_STATE *endptState, int cnxId, VRG_BOOL mute);

/*
*****************************************************************************
** FUNCTION:   vrgEndptPacket
**
** PURPOSE:    Transmit a endpt packet to the endpoint. This function is asynchronous,
**             it does not wait for the data to be transmitted  before returning.
**             In addition, the buffer release mechanism is asynchronous.
**             The endpoint owns the packet passed to this function until a callback
**             is invoked releasing the packet back to the caller (endptPacketReleaseCallback).
**             The value of 'bufDesc' will be passed to the callback function so that
**             the caller can identify which packet is being released by the endpoint.
**
** PARAMETERS: endptState - endpt object.
**             epPacket   - pointer to the EPPACKET packet.
**             length     - the length of the endpt packet including header
**             bufDesc     - buffer descriptor value that uniquely identifies
**                          the packet. This value is returned in the
**                           callback invoked once the buffer has been completely
**                           transmitted. This value is only meaningful to the
**                           caller and is opaque to the endpoint.
**
** RETURNS:    EPSTATUS
**
** NOTE:       If this function does not return EPSTATUS_SUCCESS, it is the caller's
**             responsibility to free the RTP packet.
**
*****************************************************************************
*/
EPSTATUS vrgEndptPacket( ENDPT_STATE *endptState, int cnxId, EPPACKET* epPacket, int length, unsigned int bufDesc);


/*
*****************************************************************************
** FUNCTION:   vrgEndptConsoleCmd
**
** PURPOSE:    Display endpoint info to console
**
** PARAMETERS: endptState  - state of the endpt object
**             cnxId       - the connection index
**             cmd         - endpoint console command type
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptConsoleCmd( ENDPT_STATE *endptState, EPCONSOLECMD cmd, EPCMD_PARMS *consoleCmdParams );

#ifndef PSTN_LIFE_LINE_SUPPORT
#define PSTN_LIFE_LINE_SUPPORT 0
#endif

#if PSTN_LIFE_LINE_SUPPORT
/* pstn lifeline support
*****************************************************************************
** FUNCTION:   vrgEndptGetHookStatus
**
** PURPOSE:    Get current Hook status of FXO line
**
** PARAMETERS: endptState  - state of the endpt object
**             cnxId       - the connection index
**             cmd         - endpoint console command type
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptGetHookStatus( EPCASSTATE *stat );

/* pstn lifeline support
*****************************************************************************
** FUNCTION:   vrgEndptActiveRelay
**
** PURPOSE:    Set voice channel relays as on/off
**
** PARAMETERS: cmd         - relay command, TRUE as relay is actived, FALSE as 
**                           relay is disabled
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptActiveRelay( VRG_BOOL cmd );
#endif

/*
*****************************************************************************
** FUNCTION:   vrgEndptProvSet
**
** PURPOSE:    Set a value to the endpoint provisioning database
**             The application would use this API to store a value 
**             in the endpoint provisioning database so that the
**             parameter's value is directly available to the endpoint.
**
** PARAMETERS: line        -  [IN]  Line id
**             itemId      -  [IN]  Provisioning item id
**             itemValue   -  [IN]  Pointer to the variable whose value needs to be
**                                  stored in the endpoint provisioning database
**             itemLength  -  [IN]  Length/Size of the variable whose value needs to be
**                                  stored in the endpoint provisioning database.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptProvSet( int line, EPPROV itemId, void* itemValue, int itemLength );


/*
*****************************************************************************
** FUNCTION:   vrgEndptProvGet
**
** PURPOSE:    Get a value from the endpoint provisioning database
**             The application would use this API to get a value 
**             that is currently stored in the endpoint provisioning database.
**
** PARAMETERS: line        -  [IN]  Line id
**             itemId      -  [IN]  Provisioning item id
**             itemValue   -  [OUT] Pointer to the variable that will be 
**                                  filled with the current value in the 
**                                  endpoint provisioning database
**             itemLength  -  [IN]  Length/Size of the variable whose value needs to be
**                                  stored in the endpoint provisioning database.
**
** RETURNS:    EPSTATUS
**
** NOTE: The caller of this function should allocate memory for itemValue
**
*****************************************************************************
*/
EPSTATUS vrgEndptProvGet( int line, EPPROV itemId, void* itemValue, int itemLength );

/* In order to provide Linux user endpoint APIs, we need to redefine 
** the macros in endpt.h in order to use the Linux user versions 
** of the vrgEndptxxx functions (which are just wrappers for ioctl calls
** to the kernel), not the "real" vrgEndptxxx functions defined in vrgEndpt.c */

#ifdef endptCapabilities
#  undef endptCapabilities
#  define endptCapabilities vrgEndptCapabilities
#endif 

#ifdef endptSignal
#  undef endptSignal
#  define endptSignal vrgEndptSignal
#endif 

#ifdef endptCreateConnection
#  undef endptCreateConnection
#  define endptCreateConnection vrgEndptCreateConnection
#endif 

#ifdef endptModifyConnection
#  undef endptModifyConnection
#  define endptModifyConnection vrgEndptModifyConnection
#endif 

#ifdef endptDeleteConnection
#  undef endptDeleteConnection
#  define endptDeleteConnection vrgEndptDeleteConnection
#endif 

#ifdef endptPacket
#  undef endptPacket
#  define endptPacket vrgEndptPacket
#endif 

#ifdef endptConsoleCmd
#  undef endptConsoleCmd
#  define endptConsoleCmd vrgEndptConsoleCmd
#endif 

#ifdef endptMuteConnection
#  undef endptMuteConnection
#  define endptMuteConnection vrgEndptMuteConnection
#endif 

#if PSTN_LIFE_LINE_SUPPORT
#ifdef endptGetHookStatus
#  undef endptGetHookStatus
#  define endptGetHookStatus vrgEndptGetHookStatus
#endif 

#ifdef endptActiveRelay
#  undef endptActiveRelay
#  define endptActiveRelay vrgEndptActiveRelay
#endif
#endif  /* PSTN_LIFE_LINE_SUPPORT */

#ifdef endptProvGet
#  undef endptProvGet
#  define endptProvGet vrgEndptProvGet
#endif

#ifdef endptProvSet
#  undef endptProvSet
#  define endptProvSet vrgEndptProvSet
#endif

int isEndptInitialized( void );

