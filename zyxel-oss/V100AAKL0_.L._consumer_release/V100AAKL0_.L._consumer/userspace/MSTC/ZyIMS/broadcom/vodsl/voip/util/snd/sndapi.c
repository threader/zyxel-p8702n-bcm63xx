/****************************************************************************
*
*  Copyright (c) 2003-2008 Broadcom Corporation
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
*    Description:
*
*      Sound playing/recording task.   
*    
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bcmos.h>
#include <bosTimer.h>
#include <bosSem.h>
#include <bosEvent.h>
#include <bosTask.h>
#include <xchgAssert.h>
#include <endpoint_user.h>
#include <codec.h>
#include <vodslCfg.h>

#include <vrgCmgrEnv.h>
#include <vrgCmgrLog.h>
#include <vrgCmgrApi.h>
#include <snd.h>
#include <sys/time.h>


/* ---- Private Constants and Types -------------------------- */
#define SOUND_TASK_NAME          "SNDT"

#define SOUND_TASK_STACKSIZE     ( 4 * 1024 )

#define SOUND_TASK_PRIORITY MTA_TASK_DATA

#define SOUND_TASK_BASERATE_MSEC 10

#define MAX_BUFFER_LENGTH 640

typedef struct
{
   UINT8 bytesPerFrame;       /* Size of a base frame */
   UINT8 msecPerFrame;        /* Rate of a base frame */
   UINT8 samplesPerMsec;      /* 8 for narrow band codec, and 16 for wide band codec */
} SOUNDFORMAT;

typedef struct
{
   BOOL outputEnabled;
   BOOL inputEnabled;
   BOOL deleteConnection;
   UINT16 seq;
   UINT32 ts;
   UINT32 ssrc;
   UINT8  pt;
   UINT32 tsInterval;       /* Samples of a super packet, i.e, increment of time stamp between 2 RTP packets */
   UINT32 pktSize;          /* Super packet size in bytes, i.e, payload size of each RTP packet */
   UINT8 pktPeriod;         /* Super packet size in ms */
   const char* buffer;
   UINT32 bufferLen;
   UINT32 currentPos;
   UINT8  numBaseMsElapsed;
   ENDPT_STATE* endptObjStatep;
   SNDCB sndCBp;
} SNDPROP;

typedef struct
{
  RTPPACKET RTPheader;
  char RTPdata[MAX_BUFFER_LENGTH];
} RTPDATA;

typedef struct
{
   SOUNDFORMAT soundFormat;
   UINT8 rtpPayloadType;
   CODEC_TYPE eptCodec;
} CODEC_ATTRIBUTE;

/* ---- Private Variables ------------------------------------ */

/* This table is used to define format attributes of a supported codec */
static const CODEC_ATTRIBUTE codecAttributeTable[] =
{
   { {20, 30, 8}, 4,  CODEC_G7231_53 },
   { {24, 30, 8}, 4,  CODEC_G7231_63 },
   { {10, 5,  8}, 97, CODEC_G726_16  },
   { {15, 5,  8}, 98, CODEC_G726_24  },
   { {20, 5,  8}, 2,  CODEC_G726_32  },
   { {25, 5,  8}, 99, CODEC_G726_40  },
   { {10, 10, 8}, 18, CODEC_G729     },
   { {40, 5,  8}, 0,  CODEC_PCMU     },
   { {40, 5,  8}, 8,  CODEC_PCMA     },
   { {0,  0,  0}, 0,  CODEC_NULL     }    /* This line is required to indicate the end of the list */
};
#if 0
static BOS_SEM     gSndEventSem;
#endif 
static BOS_TASK_ID gSndTaskId;
static BOS_EVENT_SET gEventSet;
static BOS_EVENT  gProcessEvent;
static BOS_TIMER gTickTimer;
static BOS_EVENT *gTickTimerEvent;
static EPZCNXPARAM   cnxParam;

static SNDPROP sndProp[VODSL_MAX_CNX];

/* ---- Private Function Prototypes -------------------------- */
static void sndTimerTaskInitCB( BOS_TASK_ARG taskArg );
static void sndTimerTaskMain( BOS_TASK_ARG taskArg );
static void sndTimerTaskDeinitCB( BOS_TASK_ARG taskArg );

/* ---- Public Variables ------------------------------------ */

/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   sndInit
*
*  PURPOSE:    Initialize the SND module
*
*  PARAMETERS:
*    None
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndInit( )
{
   BOS_STATUS status;
   int streamId;

   VRGCMGR_LOG_INFO(( NULL, "sndInit()\n" ));

   for( streamId=0; streamId < VODSL_MAX_CNX; streamId++ )
   {
      memset( &sndProp[streamId], 0, sizeof(SNDPROP) );
   }

   gSndTaskId = 0;
   status = bosTaskCreateEx( SOUND_TASK_NAME,
                             SOUND_TASK_STACKSIZE,
                             SOUND_TASK_PRIORITY,
                             sndTimerTaskInitCB,
                             sndTimerTaskMain,
                             sndTimerTaskDeinitCB,
                             0,
                             &gSndTaskId );

   if ( status != BOS_STATUS_OK )
   {
      VRGCMGR_LOG_CRIT(( NULL, "sndInit: Error creating task\n" ));
      return SND_STATUS_FAIL;
   }

   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutPlay
*
*  PURPOSE:    Plays the given buffer on the named endpoint using the given
*              streamId as the cnxId. The endpoint connection is created and
*              the RTP stream is started with the given parameters.
*
*  PARAMETERS:
*     endptState   - [IN] pointer to the endpoint state structure
*     streamId     - [IN] stream identifier (also used as cnxId during
*                         endpoint connection creation)
*     buffer       - [IN] pointer to play data buffer
*     length       - [IN] length of the play data buffer
*     sndOpenDescp - [IN] pointer to open description structure
*     eptCodec     - [IN] play data encoding type codec type
*     pktPeriod    - [IN] packetization period to use for stream
*     rtpCode      - [IN] RTP payload type to use for stream
*     sndCallBack  - [IN] callback function for SND events
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutPlay( ENDPT_STATE *endptState,
                      int streamId,
                      const char  *buffer,
                      unsigned int length,
                      CODEC_TYPE   eptCodec,
                      UINT8        pktPeriod,
                      UINT8        rtpCode,
                      SNDCB        sndCallBack )
{
   SNDOPENDESC   sndOpen;
   EPSTATUS      epStatus;

   /* Sanity check. */
   XCHG_ASSERT(endptState != NULL);
   XCHG_ASSERT(streamId < VODSL_MAX_CNX);
   XCHG_ASSERT(streamId >= 0);
   XCHG_ASSERT(buffer != NULL);
   XCHG_ASSERT(length > 0);
   XCHG_ASSERT(eptCodec != CODEC_NULL);
	
   /* Create endpoint connection. */
   memset( &cnxParam, 0, sizeof(cnxParam) );
   cnxParam.mode = EPCNXMODE_RCVO;
   cnxParam.cnxParmList.send.numCodecs = 1;
   cnxParam.cnxParmList.send.numPeriods = 1;
   cnxParam.cnxParmList.send.codecs[0].type = eptCodec;
   cnxParam.cnxParmList.send.codecs[0].rtpPayloadType = rtpCode;
   cnxParam.cnxParmList.send.period[0] = pktPeriod;
   cnxParam.cnxParmList.recv.numCodecs = 1;
   cnxParam.cnxParmList.recv.numPeriods = 1;
   cnxParam.cnxParmList.recv.codecs[0].type = eptCodec;
   cnxParam.cnxParmList.recv.codecs[0].rtpPayloadType = rtpCode;
   cnxParam.cnxParmList.recv.period[0] = pktPeriod;
   cnxParam.namedPhoneEvts = 0;
   cnxParam.echocancel = TRUE;
   cnxParam.silence = FALSE;
   cnxParam.dataMode = 0;

   //epStatus = endptCreateConnection( endptState, streamId, &cnxParam );
   epStatus = vrgEndptModifyConnection( endptState, streamId, &cnxParam );   
   if ( epStatus != EPSTATUS_SUCCESS )
   {
      VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to create connection %d, err %d", streamId,
            epStatus ));
      return SND_STATUS_FAIL;
   }

   /* Configure SND properties and setup the RTP stream. */
   sndOpen.eptCodec = eptCodec;
   sndOpen.pktPeriod = pktPeriod;
   sndOpen.rtpCode = rtpCode;
   sndOpen.endptObjStatep = endptState;
   sndOpen.sndCallBack = sndCallBack;
	
   if ( sndOutOpen( streamId, &sndOpen ) != SND_STATUS_SUCCESS )
   {
      VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to initialize SND properties %d", streamId ));
	  //endptDeleteConnection( endptState, streamId );
	  //vrgEndptDeleteConnection( endptState, streamId );
      return SND_STATUS_FAIL;
   }
   sndProp[streamId].deleteConnection = TRUE;  

   /* Start playing the stream. */
   if ( sndOutWrite( streamId, buffer, length ) != SND_STATUS_SUCCESS )
   {
      VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to play annoucement"));
      //endptDeleteConnection( endptState, streamId );
      //vrgEndptDeleteConnection( endptState, streamId );
      return SND_STATUS_FAIL;
   }
   
   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutStop
*
*  PURPOSE:    Stops the currently playing announcement on the given endpoint
*              and with given streamId.
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier (also used as cnxId during
*                         endpoint connection deletion)
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutStop( int streamId )
{
   SNDSTATUS sndStatus = SND_STATUS_SUCCESS;

   VRGCMGR_LOG_INFO(( NULL, "sndOutStop()\n" ));

   /* Sanity check. */
   XCHG_ASSERT( (streamId < VODSL_MAX_CNX) &&
                (streamId >= 0));

   /* Stop playback. */
   if( sndOutReset(streamId) != SND_STATUS_SUCCESS )
   {
      VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to stop stream %d", streamId ));
      sndStatus = SND_STATUS_FAIL;
   }

   /* Delete the connection. */
   #if 0
   if( endptDeleteConnection(sndProp[streamId].endptObjStatep, streamId)
         != EPSTATUS_SUCCESS )
   
   if( vrgEndptDeleteConnection(sndProp[streamId].endptObjStatep, streamId)
         != EPSTATUS_SUCCESS )
   {
      VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to delete stream %d", streamId ));
      sndStatus = SND_STATUS_FAIL;
   }
   #endif
   /* Close stream. */
   if( sndOutClose(streamId) != SND_STATUS_SUCCESS )
   {
      VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to close stream %d", streamId ));
      sndStatus = SND_STATUS_FAIL;
   }

   return sndStatus;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutOpen
*
*  PURPOSE:    Opens a stream for playback
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier
*     sndOpenDescp - [IN] pointer to open description structure
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutOpen( int streamId, SNDOPENDESC *sndOpenDescp )
{
   int i = 0;
   BOOL formatMatch = FALSE;
   CODEC_TYPE eptCodec = CODEC_NULL;
   UINT8 rtpCode;
   
   VRGCMGR_LOG_INFO(( NULL, "sndOutOpen()\n" ));

   XCHG_ASSERT(streamId < VODSL_MAX_CNX && streamId >= 0);

   if ( (sndProp[streamId].outputEnabled || sndProp[streamId].inputEnabled) || (sndProp[streamId].buffer != NULL) )
   {
      VRGCMGR_LOG_CRIT(( NULL, "sndOutOpen: Playing/recording in progress for stream %d", streamId ));
      VRGCMGR_LOG_CRIT(( NULL, "outputEnabled = %d, inputEndabled= %d, buff =%p\n",  \
                          sndProp[streamId].outputEnabled, \
			  sndProp[streamId].inputEnabled, \
		          sndProp[streamId].buffer ));
      return SND_STATUS_INPROGRESS;
   }

   eptCodec = sndOpenDescp->eptCodec;
   rtpCode = sndOpenDescp->rtpCode;

   if ( eptCodec == CODEC_NULL )
   {
      VRGCMGR_LOG_CRIT(( NULL, "sndOutOpen: NULL codec" ));
      return SND_STATUS_UNSUPPORTED_FORMAT;
   }

   /* Search the codecFormatTable to find eptCodec and get its attributes */
   while ( codecAttributeTable[i].eptCodec != CODEC_NULL )
   {
      if ( codecAttributeTable[i].eptCodec == eptCodec && codecAttributeTable[i].rtpPayloadType == rtpCode )
      {
         sndProp[streamId].pt = codecAttributeTable[i].rtpPayloadType;
         /* We assume the pktPeriod from the caller must be x times of 10 msec */
         UINT16 n = sndOpenDescp->pktPeriod / codecAttributeTable[i].soundFormat.msecPerFrame;
         n = (n == 0) ? 1 : n;
         sndProp[streamId].pktPeriod = n * codecAttributeTable[i].soundFormat.msecPerFrame; 
         sndProp[streamId].pktSize = n * codecAttributeTable[i].soundFormat.bytesPerFrame;
         sndProp[streamId].tsInterval = sndProp[streamId].pktPeriod * codecAttributeTable[i].soundFormat.samplesPerMsec;
         formatMatch = TRUE;
         break;
      }
      i++;
   }
   
   if ( !formatMatch )
   {   
      VRGCMGR_LOG_CRIT(( NULL, "sndOutOpen: Codec %d not supported", eptCodec ));
      return SND_STATUS_UNSUPPORTED_FORMAT;
   }
               
   sndProp[streamId].sndCBp = sndOpenDescp->sndCallBack;
   sndProp[streamId].endptObjStatep = sndOpenDescp->endptObjStatep;
   sndProp[streamId].seq = 0;
   sndProp[streamId].ts = 0;
   sndProp[streamId].ssrc = rand();
   sndProp[streamId].deleteConnection = FALSE;

   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutWrite
*
*  PURPOSE:    Sends a data block to the specified stream for playback
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier
*     buffer       - [IN] pointer to play data buffer
*     length       - [IN] length of the play data buffer
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutWrite( int streamId, const char* buffer, unsigned int length )
{
   VRGCMGR_LOG_INFO(( NULL, "SND: sndOutWrite\n"));

   XCHG_ASSERT(streamId < VODSL_MAX_CNX && streamId >= 0);

   if ( sndProp[streamId].buffer )
   {
      /* Todo: The sound module can potentialy store many buffers
       * However the current implementation prevents multiple buffer
       * To prevent memory leaks don't overwrite the previous pointer and
       * simply return FAIL */
      VRGCMGR_LOG_CRIT(( NULL, "sndOutWrite: Already contains buffer %x",(UINT32)sndProp[streamId].buffer ));
      return SND_STATUS_FAIL;
   }
   sndProp[streamId].outputEnabled = TRUE;
   sndProp[streamId].buffer = buffer;
   sndProp[streamId].bufferLen = length;
   sndProp[streamId].currentPos = 0;
   sndProp[streamId].numBaseMsElapsed = 0;

   /* Cause the SND task to wake up for processing if it is asleep. */
   VRGCMGR_LOG_INFO(( NULL, "sndOutWrite: bosEventSened() called\n" ));
   XCHG_ASSERT( bosEventSend( &gProcessEvent ) == BOS_STATUS_OK);

   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutPause
*
*  PURPOSE:   Pause playback of the output
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutPause( int streamId )
{
   VRGCMGR_LOG_INFO(( NULL, "SND: sndOutPause\n"));

   XCHG_ASSERT(streamId < VODSL_MAX_CNX && streamId >= 0);

   sndProp[streamId].outputEnabled = FALSE;
   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutRestart
*
*  PURPOSE:    Resumes playback of a stream that had been paused
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutRestart( int streamId )
{
   VRGCMGR_LOG_INFO(( NULL, "SND: sndOutRestart\n"));

   XCHG_ASSERT(streamId < VODSL_MAX_CNX && streamId >= 0);

   sndProp[streamId].outputEnabled = TRUE;
   sndProp[streamId].numBaseMsElapsed = 0;
   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutReset
*
*  PURPOSE:    Stops playback and instructs the driver to return all data buffers
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutReset( int streamId )
{
   VRGCMGR_LOG_INFO(( NULL, "SND: sndOutReset\n"));

   XCHG_ASSERT(streamId < VODSL_MAX_CNX && streamId >= 0);

   sndProp[streamId].outputEnabled = FALSE;
   if ( sndProp[streamId].sndCBp )
   {
      /* Return any buffer if there are any */
      (sndProp[streamId].sndCBp)( streamId, SNDEVT_OUT_WRITEDONE, sndProp[streamId].buffer, sndProp[streamId].currentPos );
   }

   sndProp[streamId].buffer = NULL;

   return SND_STATUS_SUCCESS;
}

/*****************************************************************************
*
*  FUNCTION:   sndOutClose
*
*  PURPOSE:    Closes a stream for playback
*
*  PARAMETERS:
*     streamId     - [IN] stream identifier
*
*  RETURNS:    SND_STATUS_SUCCESS or error code
*
*  NOTES:
*
*****************************************************************************/
SNDSTATUS sndOutClose( int streamId )
{
   VRGCMGR_LOG_INFO(( NULL, "SND: sndOutClose\n"));

   XCHG_ASSERT(streamId < VODSL_MAX_CNX && streamId >= 0);

   if ( sndProp[streamId].buffer )
   {
      return SND_STATUS_INPROGRESS;
   }
   else
   {
      sndProp[streamId].outputEnabled = FALSE;
      if ( sndProp[streamId].sndCBp )
      {
         (sndProp[streamId].sndCBp)( streamId, SNDEVT_OUT_CLOSE, NULL, 0 );
      }
      return SND_STATUS_SUCCESS;
   }
}

/*****************************************************************************
*
*  FUNCTION:   sndTimerTaskInitCB
*
*  PURPOSE:    SND task initialization function. This function will be
*              automatically called by the OS prior to invoking the
*              task's main function. Task OS resource allocation and
*              initialization may be performed here.
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     None
*
*  NOTES:
*
*****************************************************************************/
static void sndTimerTaskInitCB( BOS_TASK_ARG taskArg )
{
   VRGCMGR_LOG_INFO(( NULL, "SND: sndTimerTaskInitCB\n" ));
   /* Create events and timers used to drive SND task. */
   XCHG_ASSERT( bosEventSetCreate( &gSndTaskId, &gEventSet )
         == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventCreate( &gSndTaskId, &gProcessEvent ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosTimerCreate( &gTickTimer ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosTimerGetEvent( &gTickTimer, &gTickTimerEvent )
         == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventSetAddEvent( &gEventSet, &gProcessEvent )
         == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventSetAddEvent( &gEventSet, gTickTimerEvent )
         == BOS_STATUS_OK);
   
}

/*****************************************************************************
*
*  FUNCTION:   sndTimerTaskMain
*
*  PURPOSE:    The 10ms timer task that generates the RTP packets
*              and sends them to the endpoint module
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     None
*
*  NOTES:
*
*****************************************************************************/
static void sndTimerTaskMain( BOS_TASK_ARG taskArg )
{
	struct timeval timv;
   VRGCMGR_LOG_INFO(( NULL, "SND: sndTimerTaskMain\n" ));
   while( 1 )
   {
      BOS_STATUS status;
	 

      status = bosEventSetReceive( &gEventSet, BOS_EVENT_SET_WAIT_MODE_ANY);
	  //gettimeofday(&timv,NULL);
	  //printf("usec: %ld  ", timv.tv_usec);

      if ( status == BOS_STATUS_RESET )
      {
         VRGCMGR_LOG_INFO(( NULL, "SND: reset\n" ));
         /* This task has been notified that a reset is pending.
          * Acknowledge the notification and then block until the
          * task is resumed. */
         bosTaskResetAck();

         /* The task should stop running by exiting from this main function */
         return;
      }
      else if (status == BOS_STATUS_OK )
      {
         /* Find out why we were woken up. */
         if( bosEventSetIsEventSet( &gEventSet, gTickTimerEvent ) )
         {
            /* Woken up to send more packets. */
            BOOL stillProcessing = FALSE;
            int streamId;

            for ( streamId = 0; streamId < VODSL_MAX_CNX; streamId++ )
            {
               if ( sndProp[streamId].outputEnabled &&
                    sndProp[streamId].currentPos < sndProp[streamId].bufferLen &&
                    sndProp[streamId].buffer )
               {       
                  unsigned int len;

                  /* We're still processing this stream. */
                  stillProcessing = TRUE;

                  /* We assume pktPeriod from the caller must be x times of 10ms */
                  if ( ((sndProp[streamId].numBaseMsElapsed * SOUND_TASK_BASERATE_MSEC) %
                           sndProp[streamId].pktPeriod) != 0 )
                  {     
                  
                     sndProp[streamId].numBaseMsElapsed++;
					 //printf("CONTINUE\n");
                     /* Need to sleep more */
                     continue;
                  }
                 
                  sndProp[streamId].numBaseMsElapsed =
                     (sndProp[streamId].numBaseMsElapsed == 0) ?
                     1 : ((sndProp[streamId].numBaseMsElapsed * SOUND_TASK_BASERATE_MSEC) /
                           sndProp[streamId].pktPeriod);

                  /* The stream is enabled and there is still some data available
                   * in the buffer. Get number of bytes of a RTP packet */
                  if ( (sndProp[streamId].bufferLen - sndProp[streamId].currentPos)
                        < sndProp[streamId].pktSize )
                  {
                     len = sndProp[streamId].bufferLen - sndProp[streamId].currentPos;
                  }
                  else
                  {
                     len = sndProp[streamId].pktSize;
                  }

                  if ( len > 0 )
                  {
                     /* There is still data available for a RTP packet */
                     RTPDATA rtpPacket;
                     EPPACKET epPacket;

                     /* Form the RTP packet and send to the endpoint module */
                     rtpPacket.RTPheader.version = 2;
                     rtpPacket.RTPheader.p = 0;
                     rtpPacket.RTPheader.x = 0;
                     rtpPacket.RTPheader.cc = 0;
                     rtpPacket.RTPheader.m = 0;
                     rtpPacket.RTPheader.pt = sndProp[streamId].pt;
                     
                     RTPPACKET_SET_SEQNUM( &(rtpPacket.RTPheader), sndProp[streamId].seq );
                     RTPPACKET_SET_TIMESTAMP( &(rtpPacket.RTPheader), sndProp[streamId].ts );
                     RTPPACKET_SET_SSRC( &(rtpPacket.RTPheader), sndProp[streamId].ssrc );

                     /* Increment ts by the number of samples per sndProp[streamId].pktPeriod ms */
                     sndProp[streamId].ts += sndProp[streamId].tsInterval;
                     /* Increment seq */
                     sndProp[streamId].seq++;

                     memcpy( rtpPacket.RTPdata, &sndProp[streamId].buffer[sndProp[streamId].currentPos], len );

                     epPacket.mediaType = EPMEDIATYPE_RTP;
                     epPacket.packetp = &rtpPacket;
                	
					 //printf("WRITE\n");
                     //endptPacket( sndProp[streamId].endptObjStatep, streamId, (EPPACKET *)&epPacket, len + sizeof(RTPPACKET), 0 );                    
                     vrgEndptPacket( sndProp[streamId].endptObjStatep, streamId, (EPPACKET *)&epPacket, len + sizeof(RTPPACKET), 0 );

                     /* Increment the current buffer read position */
                     sndProp[streamId].currentPos += len;
                  }
                  else
                  {
                     /* There is no data to be sent  */
                     sndProp[streamId].currentPos = sndProp[streamId].bufferLen;
                  }
               
                  /* If the current read position meets or exeeds the buffer then
                   * issue a callback and cleanup. */
                  if ( sndProp[streamId].currentPos >= sndProp[streamId].bufferLen )
                  {
                     if ( sndProp[streamId].sndCBp )
                     {
                        (sndProp[streamId].sndCBp)( streamId,
                                                    SNDEVT_OUT_WRITEDONE,
                                                    sndProp[streamId].buffer,
                                                    sndProp[streamId].bufferLen );
                     }
                     sndProp[streamId].buffer = NULL;

                     if( sndProp[streamId].deleteConnection )
                     {                     	
                     	#if 0
                        if( endptDeleteConnection(sndProp[streamId].endptObjStatep, streamId) != EPSTATUS_SUCCESS )
			
						if( vrgEndptDeleteConnection(sndProp[streamId].endptObjStatep, streamId) != EPSTATUS_SUCCESS )	
                        {
                           VRGCMGR_LOG_CRIT(( NULL, "SND: Failed to delete stream %d", streamId ));
                        }
                        else
						#endif
						
                        {
                           /* Closed connection. Invoke the callback. */
                           if ( sndProp[streamId].sndCBp )
                           {
                              (sndProp[streamId].sndCBp)( streamId, SNDEVT_OUT_CLOSE, NULL, 0 );
                           }
                        }
                        sndProp[streamId].outputEnabled = FALSE;
                     }
                  }
               }
            }   /* for */

            if( !stillProcessing )
            {
               /* No streams to process. Stop timer. */
               XCHG_ASSERT( bosTimerStop( &gTickTimer ) == BOS_STATUS_OK);
            }
         }
         else if( bosEventSetIsEventSet( &gEventSet, &gProcessEvent ) )
         {
            /* A stream was started. Start timer for processing. */
            XCHG_ASSERT( bosTimerStart( &gTickTimer,
                     SOUND_TASK_BASERATE_MSEC,
                     BOS_TIMER_MODE_REPEAT ) == BOS_STATUS_OK);
         }
      } /* else if*/
      else if ( status = BOS_STATUS_ERR )
      {
         VRGCMGR_LOG_CRIT(( NULL, "SND: bos status error\n" ));
      }
   }  /* while (1) */
}

/*****************************************************************************
*
*  FUNCTION:   sndTimerTaskDeinitCB
*
*  PURPOSE:    SND task deinitialization function. This function will be
*              automatically called by the OS after the task's main
*              function has exited. Any task OS resources allocated by the
*              task initialization function (sndTimerTaskInitCB) should be
*              de-allocated here.
*
*  PARAMETERS:
*     None
*
*  RETURNS:
*     None
*
*  NOTES:
*
*****************************************************************************/
static void sndTimerTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Cleanup */
   XCHG_ASSERT( bosEventSetRemoveAllEvents( &gEventSet ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosTimerStop( &gTickTimer ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosTimerDestroy( &gTickTimer ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventDestroy( &gProcessEvent ) == BOS_STATUS_OK);
   XCHG_ASSERT( bosEventSetDestroy( &gEventSet ) == BOS_STATUS_OK);
}

/*
*****************************************************************************
** FUNCTION:   sndDeinit
**
** PURPOSE:    SND module deinit - call once during system shutdown.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:       It is assumed that this function is only called after the
**             task has been notified of a pending application reset, and it
**             has acknowledged the notification. This implies that the
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
void sndDeinit( void )
{
   VRGCMGR_LOG_INFO(( NULL, "sndDeinit()\n" ));
   /* Shutdown task */
   if ( gSndTaskId != 0 )
   {
      /* Currently, in the Linux User bos implementation, the bosTaskProceedShutdown is not defined
       * it relied on linux system to kill the parent process and its threads, and properly clean the 
       * resource
       * */
      /* the task should be blocked in the reset-acknowledgement state.
       * Advance the task to the reset-active state. This will cause the task to
       * resume operation. In this state, the task should perform any operations
       * required for proper shutdown before exiting from its main task function. */
      /* bosTaskProceedToShutdown( &gSndTaskId ); */

      /* Wait for the task to complete de-initialization */
      /* bosTaskWaitForDone( &gSndTaskId ); */
   }
}
