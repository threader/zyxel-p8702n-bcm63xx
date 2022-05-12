/***************************************************************************
*
*    Copyright 2003  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*
****************************************************************************
*
*    Description:
*
****************************************************************************/

#ifndef SND_H
#define SND_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Include Files ---------------------------------------- */
#include <endpt.h>
#include <annce.h>
#include <cxcEnv.h>

/* ---- Constants and Types ---------------------------------- */

/* Snd Status */
typedef enum
{
   SND_STATUS_SUCCESS = 0,
   SND_STATUS_FAIL,
   SND_STATUS_UNSUPPORTED_FORMAT,
   SND_STATUS_INPROGRESS,
} SNDSTATUS;

/* Wav Evt */
typedef enum
{
   SNDEVT_OUT_WRITEDONE = 0,
   SNDEVT_OUT_CLOSE
} SNDEVT;

typedef void ( *SNDCB )( int streamId, SNDEVT sndEvt, const char *buffer, int length );

typedef struct
{
   SNDCB sndCallBack;
   CODEC_TYPE eptCodec;
   UINT8 pktPeriod;
   UINT8 rtpCode;
   ENDPT_STATE* endptObjStatep;
} SNDOPENDESC;

/* ---- Variable Externs ------------------------------------- */

/* ---- Function Prototypes ---------------------------------- */

SNDSTATUS sndInit( );
SNDSTATUS sndOutPlay( ENDPT_STATE *endptState,
                      int streamId,
                      const char  *buffer,
                      unsigned int length,
                      CODEC_TYPE   eptCodec,
                      UINT8        pktPeriod,
                      UINT8        rtpCode,
                      SNDCB        sndCallBack );
SNDSTATUS sndOutStop( int streamId );
SNDSTATUS sndOutOpen( int streamId, SNDOPENDESC *sndOpenDescp );
SNDSTATUS sndOutWrite( int streamId, const char* buffer, unsigned int length );
SNDSTATUS sndOutPause( int streamId );
SNDSTATUS sndOutRestart( int streamId );
SNDSTATUS sndOutReset( int streamId );
SNDSTATUS sndOutClose( int streamId );
void sndDeinit( void );

void sndPlayAnnouncement( ANNCECMD annceCmd, ENDPT_STATE *endptState, int streamId, ANNCERSN reason );
#ifdef __cplusplus
}
#endif

#endif /* SND_H */

