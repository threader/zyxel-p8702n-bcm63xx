/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
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
*    Filename: endpoint_user.c
*
****************************************************************************
*    Description:
*
*      Endpoint Abstraction Layer for the Linux User.
*      The endpoint functions are implemented in the kernel. This file
*      contains only wrapper functions, which call the appropriate
*      endpoint functions in the endpoint driver.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit, getpid */
#include <sys/types.h>  /* getpid */
#include <sys/ioctl.h>  /* ioctl */
#include <bcmtypes.h>

#include <vodslLog.h>
#include <endpointdrv.h>
#include <vodslCfg.h>
#include <bosSleep.h>
#include <bosTask.h>
#include <bosError.h>
#include <endpoint_user.h>

#include <linux/serial.h>
#include <termios.h>
#include <unistd.h>

#ifdef NTR_SUPPORT
#include <ntrSync.h>
#endif

#ifdef MSTC_VOICE_KSOCKET_RTP
#define KSOCKET_RTP_SUPPORT 1
#else
#define KSOCKET_RTP_SUPPORT 0
#endif

#ifdef MSTC_VOICE_GR909 /*Klose, support GR909*/
#define FILE_LTEST_RESULT "/var/ltest_result"
#endif
/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

/* since there is no "Linux" specific error codes defined in endpoint API,
   we can use one of those defined... */
#define EPSTATUS_DRIVER_ERROR       EPSTATUS_UNKNOWN_TYPE

#define NOT_INITIALIZED             (-1)

typedef void (*rtpDropPacketResetCallback)(void);

typedef struct
{
   endptEventCallback         pEventCallBack;
   endptPacketCallback        pPacketCallBack;
   rtpDropPacketResetCallback pRtpDropPacketResetCallBack;
   int                        fileHandle;
   int                        logFileHandle;

} ENDPTUSER_CTRLBLOCK;

/* ---- Private Variables ------------------------------------------------ */

static ENDPTUSER_CTRLBLOCK endptUserCtrlBlock = {NULL, NULL, NULL, NOT_INITIALIZED, NOT_INITIALIZED};
static BOOL bGlobalTaskExit = FALSE;
static volatile int endptDeinitialized;

/* ---- Private Function Prototypes -------------------------------------- */

EPSTATUS vrgEndptInitPacket
(   
   endptEventCallback         packetCallback
);
EPSTATUS vrgEndptInitEvent
(   
   endptEventCallback         notifyCallback
);
/* ---- Function implementations ----------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   vrgEndptDriverOpen
**
** PURPOSE:    Opens the Linux kernel endpoint driver.
**             This function should be the very first call used by the
**             application before isssuing any other endpoint APIs because
**             the ioctls for the endpoint APIs won't reach the kernel
**             if the driver is not successfully opened.
**
** PARAMETERS:
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptDriverOpen(void)
{
   /* Open and initialize Endpoint driver */
   if( ( endptUserCtrlBlock.fileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
   {
      printf( "%s: open error %d\n", __FUNCTION__, errno );
      return ( EPSTATUS_DRIVER_ERROR );
   }
   else
   {
      printf( "%s: Endpoint driver open success\n", __FUNCTION__ );
   }

   return ( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptDriverClose
**
** PURPOSE:    Close endpoint driver
**
** PARAMETERS: None
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptDriverClose()
{
   if ( close( endptUserCtrlBlock.fileHandle ) == -1 )
   {
      printf("%s: close error %d", __FUNCTION__, errno);
      return ( EPSTATUS_DRIVER_ERROR );
   }

   endptUserCtrlBlock.fileHandle = NOT_INITIALIZED;

   return( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   EndpointEventTask
**
** PURPOSE:    Read events from the Endpoint driver. This task is suspended
**             in the Endpoint driver in the kernel if no events are available
**             from the endpoint. The task is waked up again if new events
**             (hook events, dtmf digits) are reported by the endpoint.
**
** PARAMETERS: arg - not used
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void EndpointEventTask( void *arg )
{
   ENDPOINTDRV_EVENT_PARM tEventParm;
   ENDPT_STATE endptState;
   int rc = IOCTL_STATUS_FAILURE;
   #if 1/*MTS_VOICE, rt_priority_Eason*/
   nice(-10);
   printf("%s, getpid = %d\n", __FUNCTION__, getpid());
   #endif
   #if 1 /*MTS_VOICE*/
   /*Eason, register application signal handler*/
	AppRegisterSignal();
   #endif
   tEventParm.size = sizeof(ENDPOINTDRV_EVENT_PARM);

   LOG(( "Endpoint Event task started with pid %u...\n", getpid() ));

   while( !bGlobalTaskExit )
   {
      tEventParm.length = 0;

      /* Check if kernel driver is opened */
      BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

      /* Get the event from the endpoint driver. */
      rc = ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_GET_EVENT, &tEventParm);
      if( rc == IOCTL_STATUS_SUCCESS )
      {
         endptState.lineId = tEventParm.lineId;

         if ( (tEventParm.event == EPEVT_FSK_CLIDRX) ||
              (tEventParm.event == EPEVT_DTMF_CLIDRX) )
         {
            VRG_CLIDRX_DATA* vrgClidRxData = (VRG_CLIDRX_DATA*)tEventParm.eventData;
            LOG(("USER DEBUG CLIDRX date = %s \n", vrgClidRxData->date));
            LOG(("USER DEBUG CLIDRX number = %s \n", vrgClidRxData->number));
            LOG(("USER DEBUG CLIDRX name = %s \n", vrgClidRxData->name));
            LOG(("USER DEBUG CLIDRX reasonNoname = %c \n", vrgClidRxData->reasonNoName));
            LOG(("USER DEBUG CLIDRX reasonNonumber = %c \n", vrgClidRxData->reasonNoNumber));
         }

         if ( endptUserCtrlBlock.pEventCallBack )
         {
            (endptUserCtrlBlock.pEventCallBack)(&endptState, tEventParm.cnxId, tEventParm.event, tEventParm.eventData, tEventParm.length, tEventParm.intData );
         }
      }
      else if ( rc == IOCTL_STATUS_SHUTDOWN )
      {
         break;
         /* protection from deadlock */
         bosSleep(1);
      }
      else if ( rc == IOCTL_STATUS_DEBUG )
      {
         /* 
         ** Task was interrupted by a debug signal. Reset the RTP dropped packets counter to it's
         ** initial value. The egress RTP threads should start dropping packets to prevent the DSP's
         ** egress path from being choked by a big burst of RTP packets. 
         */
         if ( endptUserCtrlBlock.pRtpDropPacketResetCallBack )
         {
            (endptUserCtrlBlock.pRtpDropPacketResetCallBack)();
         }
      }
   }

   printf("Exiting endpoint event task (pid = %u)\n", getpid() );
}


/*
*****************************************************************************
** FUNCTION:   EndpointPacketTask
**
** PURPOSE:    Read packets from the Endpoint driver. This task is suspended
**             in the Endpoint driver in the kernel if no packets are available
**             from the Endpoint. The task is waked up again if new packets
**             arrive from the Endpoint.
**
** PARAMETERS: arg - not used
**
** RETURNS:    nothing
**
*****************************************************************************
*/
static void EndpointPacketTask( void *arg )
{
   ENDPOINTDRV_PACKET_PARM tPacketParm;
   UINT8 data[1024];
   int rc = IOCTL_STATUS_SUCCESS;
   EPPACKET epPacket;

   tPacketParm.size = sizeof(ENDPOINTDRV_PACKET_PARM);

   LOG(( "Endpoint Packet task started with pid %u ...\n", getpid() ));

   while( !bGlobalTaskExit )
   {
      epPacket.mediaType   = 0;
      epPacket.packetp     = &data[0];
      tPacketParm.epPacket = &epPacket;
      tPacketParm.cnxId = 0;
      tPacketParm.length = 0;

      /* Check if kernel driver is opened */
      BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

      /* Get the packet from the endpoint driver. */
      rc = ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_GET_PACKET, &tPacketParm);
      if( rc == IOCTL_STATUS_SUCCESS )
      {
         if( endptUserCtrlBlock.pPacketCallBack )
         {
            (endptUserCtrlBlock.pPacketCallBack)(  NULL,
                                                   tPacketParm.cnxId,
                                                   tPacketParm.epPacket,
                                                   tPacketParm.length );
         }
      }
      else if ( rc == IOCTL_STATUS_SHUTDOWN )
      {
         break;
         /* protection from deadlock */
         bosSleep(1);
      }
      else if ( rc == IOCTL_STATUS_DEBUG )
      {
         /* 
         ** Task was interrupted by a debug signal. Reset the RTP dropped packets counter to it's
         ** initial value. The egress RTP threads should start dropping packets to prevent the DSP's
         ** egress path from being choked by a big burst of RTP packets. 
         */
         if ( endptUserCtrlBlock.pRtpDropPacketResetCallBack )
         {
            (endptUserCtrlBlock.pRtpDropPacketResetCallBack)();
         }
      }
   }

   LOG(("Exiting endpoint packet task (pid = %u)\n", getpid() ));
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptInit
**
** PURPOSE:    Module initialization for the VRG endpoint. The endpoint
**             module is responsible for controlling a set of endpoints.
**             Individual endpoints are initialized using the vrgEndptInit() API.
**
** PARAMETERS: country           - Country type
**             notifyCallback    - Callback to use for event notification
**             packetCallback           - Callback to use for endpt packets
**             getProvisionCallback     - Callback to get provisioned values.
**                                        May be set to NULL.
**             setProvisionCallback     - Callback to get provisioned values.
**                                        May be set to NULL.
**             packetReleaseCallback    - Callback to release ownership of
**                                        endpt packet back to caller
**             taskShutdownCallback     - Callback invoked to indicate endpt
**                                        task shutdown
**
** RETURNS:    EPSTATUS
**
** NOTE:       getProvisionCallback, setProvisionCallback,
**             packetReleaseCallback, and taskShutdownCallback are currently not used within
**             the DSL framework and should be set to NULL when
**             invoking this function.
**
*****************************************************************************
*/
EPSTATUS vrgEndptInit
(
   VRG_ENDPT_INIT_CFG        *endptInitCfg,
   endptEventCallback         notifyCallback,
   endptPacketCallback        packetCallback,
   endptGetProvCallback       getProvisionCallback,
   endptSetProvCallback       setProvisionCallback,
   endptPacketReleaseCallback packetReleaseCallback,
   endptTaskShutdownCallback  taskShutdownCallback
)
{
   ENDPOINTDRV_INIT_PARAM tStartupParam;
   BOS_TASK_ID eventTaskId    = 0;
   BOS_TASK_ID packetTaskId   = 0;

   tStartupParam.endptInitCfg = endptInitCfg;
   tStartupParam.epStatus     = EPSTATUS_DRIVER_ERROR;
   tStartupParam.size         = sizeof(ENDPOINTDRV_INIT_PARAM);
#if 0 /*MTS_VOICE*/
   endptUserCtrlBlock.pEventCallBack   = notifyCallback;
   endptUserCtrlBlock.pPacketCallBack  = packetCallback;
#endif
   bGlobalTaskExit = FALSE;
#if 1 /*MTS_VOICE*/
	  /* Open and initialize Endpoint driver */
	  if( ( endptUserCtrlBlock.fileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
	  {
		 LOG(( "Endpoint: open error %d", errno ));
	  }
#endif

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );
fprintf(stderr, "%s, ENDPOINTIOCTL_ENDPT_INIT\n", __FUNCTION__);
   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_INIT, &tStartupParam ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "vrgEndptInit: error during ioctl" ));
      return ( tStartupParam.epStatus );
   }
   else
   {
      /* Endpoint initialization was OK. Clear the deinit flag */
      endptDeinitialized = 0;
   }
fprintf(stderr, "%s, ENDPOINTIOCTL_ENDPT_INIT end\n", __FUNCTION__);


#if 0 /*MTS_VOICE*/
   /* Start the task for reading events from the endpoint driver */
   bosTaskCreate( "eptEvent",
                  BOS_CFG_TASK_LINUX_DEFAULT_STACK_SIZE,
                  BOS_CFG_TASK_HIGH_VAL,
                  &EndpointEventTask,
                  0,
                  &eventTaskId );

   /* Start the task for reading packets from the endpoint driver */
   bosTaskCreate( "eptPacket",
                  BOS_CFG_TASK_LINUX_DEFAULT_STACK_SIZE,
                  BOS_CFG_TASK_HIGH_VAL,
                  &EndpointPacketTask,
                  0,
                  &packetTaskId );
#endif 
	  vrgEndptInitPacket(packetCallback);
#if 0 /*MTS_VOICE*/

   if( (eventTaskId == 0) || (packetTaskId==0) )
   {
      LOG(( "Couldn't initialize event and/or packet threads !!!" ));
      return ( EPSTATUS_DRIVER_ERROR );
   }
#endif
   return ( tStartupParam.epStatus );
}
#if 1 /*MTS_VOICE*/
void vrgEndptOpenDriver(void)
{
	if( ( endptUserCtrlBlock.fileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
	{
		LOG(( "Endpoint: open error %d", errno ));
	}

}


EPSTATUS vrgEndptInitEvent
(   
   endptEventCallback         notifyCallback
)
{
   ENDPOINTDRV_INIT_PARAM tStartupParam;
   BOS_TASK_ID eventTaskId    = 0;
   BOS_TASK_ID packetTaskId   = 0;

   endptUserCtrlBlock.pEventCallBack   = notifyCallback;
	#if 1
   endptUserCtrlBlock.fileHandle       = -1;

   /* Open and initialize Endpoint driver */
   if( ( endptUserCtrlBlock.fileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
   {
      LOG(( "Endpoint: open error %d", errno ));
   }
   else
   {
	#endif
      bGlobalTaskExit = FALSE;	
      /* Start the task for reading events from the endpoint driver */
      bosTaskCreate( "eptEvent",
                     BOS_CFG_TASK_LINUX_DEFAULT_STACK_SIZE,
                     BOS_CFG_TASK_HIGH_VAL,
                     &EndpointEventTask,
                     0,
                     &eventTaskId );      
	#if 1
   }
	#endif

   return 0;
}

EPSTATUS vrgEndptInitPacket
(   
   endptEventCallback         packetCallback
)
{
   ENDPOINTDRV_INIT_PARAM tStartupParam;
   BOS_TASK_ID eventTaskId    = 0;
   BOS_TASK_ID packetTaskId   = 0;


   endptUserCtrlBlock.pPacketCallBack= packetCallback;
	#if 1
   endptUserCtrlBlock.fileHandle       = -1;

   /* Open and initialize Endpoint driver */
   if( ( endptUserCtrlBlock.fileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
   {
      LOG(( "Endpoint: open error %d", errno ));
   }
   else
   {
	#endif
      bGlobalTaskExit = FALSE;	
      /* Start the task for reading events from the endpoint driver */
      bosTaskCreate( "eptPacket",
                     BOS_CFG_TASK_LINUX_DEFAULT_STACK_SIZE,
                     BOS_CFG_TASK_HIGH_VAL,
                     &EndpointPacketTask,
                     0,
                     &packetTaskId );

	#if 1
   }
	#endif
   return 0;
}
#endif
/*
*****************************************************************************
** FUNCTION:   vrgEndptDeinit
**
** PURPOSE:    VRG endpoint module shutdown - call once during system shutdown.
**             This will shutdown all endpoints and free all resources used by
**             the VRG endpt manager. (i.e. this function should free all resources
**             allocated in vrgEndptInit() and vrgEndptCreate()).
**
** PARAMETERS: none
**
** RETURNS:    EPSTATUS
**             This function should only return an error under catastrophic
**             circumstances. i.e. Something that cannot be fixed by re-invoking
**             the module initialization function.
**
** NOTE:       It is assumed that this function is only called after all endpoint
**             tasks have been notified of a pending application reset, and each
**             one has acknowledged the notification. This implies that each endpoint
**             task is currently blocked, waiting to be resumed so that they may
**             complete the shutdown procedure.
**
**             It is also assumed that no task is currently blocked on any OS
**             resource that was created in the module initialization functions.
**
*****************************************************************************
*/
EPSTATUS vrgEndptDeinit( void )
{
   int filehandle = open("/dev/bcmendpoint0", O_RDWR);
   if ( filehandle == -1 )
   {
      LOG(("Endpoint driver open ERROR \n"));
      return( EPSTATUS_DRIVER_ERROR );
   }

   if ( ioctl( filehandle, ENDPOINTIOCTL_ENDPT_DEINIT, NULL ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   bGlobalTaskExit = TRUE;

   close( filehandle );

   endptDeinitialized = 1;

   return( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumEndpoints
**
** PURPOSE:    Retrieve the number of endpoints
**
** PARAMETERS: None
**
** RETURNS:    Number of endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumEndpoints( void )
{
   ENDPOINTDRV_ENDPOINTCOUNT_PARM endpointCount;
   int retVal = 0;
   
   int filehandle = open("/dev/bcmendpoint0", O_RDWR);

   endpointCount.size = sizeof(ENDPOINTDRV_ENDPOINTCOUNT_PARM);

   if ( ioctl( filehandle, ENDPOINTIOCTL_ENDPOINTCOUNT, &endpointCount ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }
   else
   {
      retVal = endpointCount.endpointNum;
   }

   close(filehandle);

   return( retVal );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumFxoEndpoints
**
** PURPOSE:    Retrieve the number of FXO endpoints
**
** PARAMETERS: None
**
** RETURNS:    Number of FXO endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumFxoEndpoints( void )
{
   ENDPOINTDRV_ENDPOINTCOUNT_PARM endpointCount;
   int retVal = 0;
   
   int filehandle = open("/dev/bcmendpoint0", O_RDWR);

   endpointCount.size = sizeof(ENDPOINTDRV_ENDPOINTCOUNT_PARM);

   if ( ioctl( filehandle, ENDPOINTIOCTL_FXOENDPOINTCOUNT, &endpointCount ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }
   else
   {
      retVal = endpointCount.endpointNum;
   }

   close(filehandle);

   return( retVal );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptGetNumDectEndpoints
**
** PURPOSE:    Retrieve the number of DECT endpoints
**
** PARAMETERS: None
**
** RETURNS:    Number of DECT endpoints
**
*****************************************************************************
*/
int vrgEndptGetNumDectEndpoints( void )
{
   ENDPOINTDRV_ENDPOINTCOUNT_PARM endpointCount;
   int retVal = 0;
   
   int filehandle = open("/dev/bcmendpoint0", O_RDWR);

   endpointCount.size = sizeof(ENDPOINTDRV_ENDPOINTCOUNT_PARM);

   if ( ioctl( filehandle, ENDPOINTIOCTL_DECTENDPOINTCOUNT, &endpointCount ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }
   else
   {
      retVal = endpointCount.endpointNum;
   }

   close(filehandle);

   return( retVal );
}

#if PSTN_LIFE_LINE_SUPPORT
/*
*****************************************************************************
** FUNCTION:   vrgEndptIsPstnLifeLineSupported
**
** PURPOSE:    Determines whether pstn life line is supported for this platform.
**
** PARAMETERS: None
**
** RETURNS:    true if supported; false otherwise
**
*****************************************************************************
*/
VRG_BOOL vrgEndptIsPstnLifeLineSupported( void )
{
   ENDPOINTDRV_ISSUPPORTED_PARM isSupported;

   isSupported.size       = sizeof(ENDPOINTDRV_ISSUPPORTED_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ISPSTNLIFELINESUPPORTED, &isSupported ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( isSupported.isSupported );
}
#endif

/*
*****************************************************************************
** FUNCTION:   vrgEndptCreate
**
** PURPOSE:    This function is used to create an VRG endpoint object.
**
** PARAMETERS: physId      (in)  Physical interface.
**             lineId      (in)  Endpoint line identifier.
**             endptState  (out) Created endpt object.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptCreate( int physId, int lineId, VRG_ENDPT_STATE *endptState )
{
   ENDPOINTDRV_CREATE_PARM tInitParm;

   tInitParm.physId     = physId;
   tInitParm.lineId     = lineId;
   tInitParm.endptState = endptState;
   tInitParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tInitParm.size       = sizeof(ENDPOINTDRV_CREATE_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_CREATE, &tInitParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   return( tInitParm.epStatus );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptDestroy
**
** PURPOSE:    This function is used to destroy VRG endpoint object
**             (previously created with vrgEndptCreate)
**
** PARAMETERS: endptState (in) Endpt object to be destroyed.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptDestroy( VRG_ENDPT_STATE *endptState )
{
   ENDPOINTDRV_DESTROY_PARM tInitParm;

   tInitParm.endptState = endptState;
   tInitParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tInitParm.size       = sizeof(ENDPOINTDRV_DESTROY_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_DESTROY, &tInitParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   return( tInitParm.epStatus );
}


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
EPSTATUS vrgEndptCapabilities( ENDPT_STATE* endptState, EPZCAP* capabilities )
{
   ENDPOINTDRV_CAP_PARM tCapParm;

   tCapParm.capabilities= capabilities;
   tCapParm.state       = endptState;
   tCapParm.epStatus    = EPSTATUS_DRIVER_ERROR;
   tCapParm.size        = sizeof(ENDPOINTDRV_CAP_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_CAPABILITIES, &tCapParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tCapParm.epStatus );
}

/*****************************************************************************
*  FUNCTION:   vrgEndptSetLineState
*
*  PURPOSE:    Set CAS line state for a given endpoint
*
*  PARAMETERS: endptState   - state of the endpt object
*              casState     - desired CAS state for the endpoint
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptSendCasEvtToEndpt( ENDPT_STATE *endptState, CAS_CTL_EVENT_TYPE eventType, CAS_CTL_EVENT event )
{
   ENDPOINTDRV_SENDCASEVT_CMD_PARM tCasCtlEvtParm;
   int fileHandle;

   tCasCtlEvtParm.epStatus      = EPSTATUS_DRIVER_ERROR;
   tCasCtlEvtParm.casCtlEvtType = eventType;
   tCasCtlEvtParm.casCtlEvt     = event;
   tCasCtlEvtParm.lineId        = endptState->lineId;
   tCasCtlEvtParm.size          = sizeof(ENDPOINTDRV_SENDCASEVT_CMD_PARM);
/*
   tHookstatParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tHookstatParm.hookstat   = casState;
   tHookstatParm.lineId     = endptState->lineId;
   tHookstatParm.size       = sizeof(ENDPOINTDRV_HOOKSTAT_CMD_PARM);
*/
//printf("tHookstatParm.size =%d, endpointdrv_hookstat_cmd_parm=%d\n", tHookstatParm.size, sizeof(ENDPOINTDRV_HOOKSTAT_CMD_PARM));

   fileHandle = open("/dev/bcmendpoint0", O_RDWR);
						 
   if ( ioctl( fileHandle, ENDPOINTIOCTL_SEND_CAS_EVT, &tCasCtlEvtParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   close(fileHandle);
   return( tCasCtlEvtParm.epStatus );
}

/*****************************************************************************
*  FUNCTION:   vrgEndptGetRtpStats
*
*  PURPOSE:    get RTP statistics
*
*  PARAMETERS: endptState  - endpt object
*              cnxId       - connection identifier
*              epRtpStats  - RTP stats
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptGetRtpStats( ENDPT_STATE *endptState, int cnxId, EPZCNXSTATS* epRtpStats )
{
   ENDPOINTDRV_RTP_STATS_PARM tStatsParm;

   tStatsParm.state = endptState;
   tStatsParm.cnxId = cnxId;
   tStatsParm.stats = epRtpStats;
   tStatsParm.epStatus = EPSTATUS_DRIVER_ERROR;
   tStatsParm.size = sizeof(tStatsParm);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_GET_RTP_STATS, &tStatsParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tStatsParm.epStatus );
}


/*****************************************************************************
*  FUNCTION:   vrgEndptSignal
*
*  PURPOSE:    Generate a signal on the endpoint (or connection)
*
*  PARAMETERS: endptState  - state of the endpt object
*              cnxId       - connection identifier (-1 if not applicable)
*              signal      - signal type code (see EPSIG)
*              value       - signal value
*                          BR signal types - 1
*                          OO signal types - 0 == off, 1 == on
*                          TO signal types - 0 = stop/off, 1= start/on
*                          String types - (char *) cast to NULL-term string value
*
*  RETURNS:    EPSTATUS
*
*****************************************************************************/
EPSTATUS vrgEndptSignal
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPSIG          signal,
   unsigned int   value,
   int            duration,
   int            period,
   int            repetition
)
{
   ENDPOINTDRV_SIGNAL_PARM tSignalParm;

   tSignalParm.cnxId    = cnxId;
   tSignalParm.state    = endptState;
   tSignalParm.signal   = signal;
   tSignalParm.value    = value;
   tSignalParm.epStatus = EPSTATUS_DRIVER_ERROR;
   tSignalParm.duration = duration;
   tSignalParm.period   = period;
   tSignalParm.repetition = repetition;
   tSignalParm.size     = sizeof(ENDPOINTDRV_SIGNAL_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_SIGNAL, &tSignalParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tSignalParm.epStatus );
}

#ifdef NTR_SUPPORT
/*****************************************************************************
*  FUNCTION:   vrgEndptNtrCmd
*
*  PURPOSE:    Function to update PCM feedback divider control registers for NTR
*
*  PARAMETERS: endptState  - state of the endpt object
*              EPNTR_PARMS - ptr to NTR parameter block
*
*  RETURNS:    EPSTATUS
*
*  NOTE:       This function is overloaded to either (i) set the PCM feedback registers 
*              or to (ii) get the register values for the local register as well as the 
*              NTR register.  This is decided by the value of 'epNtrParm->ntrAction'.
*              In both cases, 'tNtrParm.pcmStepsAdjust' and 'tNtrParm.bNtrEnbl'
*              are copied to kernel space, and upon return from the ioctl, will then
*              copy the values of 'epNtrParm->localCount' and 'epNtrParm->ntrCount'
*              to userspace.  It is up to the calling function / programmer to realize
*              that you cannot perform a get+set in just one call, and thus to use
*              the variables properly ( e.g. do not expect correct values for
*              'epNtrParm->localCount' and 'epNtrParm->ntrCount' when calling this 
*              function with 'epNtrParm->ntrAction' set to SET_FEEDBACK )
*****************************************************************************/
EPSTATUS vrgEndptNtrCmd
( 
   EPNTR_PARMS             *epNtrParm
)
{
   ENDPOINTDRV_NTR_PARM tNtrParm;
   int fileHandle;

   /* Allocate memory here that is accessed by kernel space when writing to the feedback registers
    * This same allocated memory will be filled in upon return from kernel space with the values 
    * of the NTR counter and local oscillator counter (if doing a 'set'), else the */
   tNtrParm.pcmStepsAdjust    = epNtrParm->pcmStepsAdjust;
   tNtrParm.bNtrEnbl          = (VRG_UINT8) epNtrParm->isEnbl;
   tNtrParm.epStatus          = EPSTATUS_NOTREADY;
   
   if ( epNtrParm->ntrAction == SET_FEEDBACK ) /* SET */
   {
      tNtrParm.ntrAction = 0;  /* SET */
   }
   else if ( epNtrParm->ntrAction == GET_COUNTS )
   {
      tNtrParm.ntrAction = 1;  /* GET */
   }
   else if ( epNtrParm->ntrAction == GET_NDIV )
   {
      tNtrParm.ntrAction = 2;  /* GET NDIV */
   }   
   else if ( epNtrParm->ntrAction == PRINT_REGS )
   {
      tNtrParm.ntrAction = 3;  /* DEBUG: print registers */
   } 
   else
   {
      VODSL_LOGERROR(("%s: Unexpected action was issued.", __FUNCTION__ ));
      return EPSTATUS_BAD_MODE;
   }

#if ( NTR_DEBUG_LOG )
   VODSL_LOGERROR(("%s: tNtrParm.pcmStepsAdjust=%i", __FUNCTION__, tNtrParm.pcmStepsAdjust));
   VODSL_LOGERROR(("%s: tNtrParm.bNtrEnbl=%u", __FUNCTION__, tNtrParm.bNtrEnbl));
   VODSL_LOGERROR(("%s: tNtrParm.epStatus=%u", __FUNCTION__, tNtrParm.epStatus));
   VODSL_LOGERROR(("%s: tNtrParm.ntrAction=%u", __FUNCTION__, tNtrParm.ntrAction));
#endif /* NTR_DEBUG_LOG */

   tNtrParm.size              = sizeof(ENDPOINTDRV_NTR_PARM);

   fileHandle = open("/dev/bcmendpoint0", O_RDWR);
   
   VRG_UINT8 isInitCounter = 0;

   /* Wait for endpoint to be ready to accept ioctl */
   while ( !isEndptInitialized() )
   {
      bosSleep( NTR_WAIT_TIME_FOR_ENDPT_INIT_MS );  /* sleep before checking again */
      isInitCounter ++;

      if ( NTR_WAIT_TIME_FOR_ENDPT_INIT_COUNT <= isInitCounter )
      {
         VODSL_LOGERROR(( "%s: timeout on waiting for endpoint to be ready", __FUNCTION__ ));
         
         return ( EPSTATUS_NOTREADY );
      }
   }
   /* Copy 'tNtrParm' to kernel space, which includes a command */
   if ( ioctl( fileHandle, ENDPTIO_NTR_CMD, &tNtrParm ) != IOCTL_STATUS_SUCCESS )
   {
      VODSL_LOGERROR(("%s: error during ioctl", __FUNCTION__ ));
   }
   /* command is complete.  Now duplicate parameters to calling parent function */

   epNtrParm->localCount = tNtrParm.localCount;
   epNtrParm->ntrCount = tNtrParm.ntrCount;
   epNtrParm->ndivInt = tNtrParm.ndivInt;
   epNtrParm->ndivFrac = tNtrParm.ndivFrac;

#if ( NTR_DEBUG_LOG )
   VODSL_LOGERROR(("%s: tNtrParm.epStatus=[%u]", __FUNCTION__, (unsigned int) tNtrParm.epStatus));
   VODSL_LOGERROR(("%s: tNtrParm.localCount=0x[%08lx]", __FUNCTION__, tNtrParm.localCount));
   VODSL_LOGERROR(("%s: tNtrParm.ntrCount=0x[%08lx]", __FUNCTION__, tNtrParm.ntrCount));
   VODSL_LOGERROR(("%s: tNtrParm.ndivInt=0x[%08lx]", __FUNCTION__, tNtrParm.ndivInt));
   VODSL_LOGERROR(("%s: tNtrParm.ndivFrac=0x[%08lx]", __FUNCTION__, tNtrParm.ndivFrac));

   if (tNtrParm.epStatus != EPSTATUS_SUCCESS)
   {
      VODSL_LOGERROR(("%s: *** tNtrParm != EPSTATUS_SUCCESS!", __FUNCTION__ ));
   } else {
      VODSL_LOGERROR(("tNtrParm == EPSTATUS_SUCCESS"));
   }
#endif /* NTR_DEBUG_LOG */

   close(fileHandle);
   
   return( tNtrParm.epStatus );
}
#endif /* NTR_SUPPORT */

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
EPSTATUS vrgEndptCreateConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
)
{
   ENDPOINTDRV_CONNECTION_PARM tConnectionParm;

   tConnectionParm.cnxId      = cnxId;
   tConnectionParm.cnxParam   = cnxParam;
   tConnectionParm.state      = endptState;
   tConnectionParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tConnectionParm.size       = sizeof(ENDPOINTDRV_CONNECTION_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_CREATE_CONNECTION, &tConnectionParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__))
   }

   return( tConnectionParm.epStatus );
}

#if KSOCKET_RTP_SUPPORT /*MTS_VOICE*/
/*****************************************************************************
*  FUNCTION:   vrgEndptMakeSession
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
EPSTATUS vrgEndptMakeSession
( 
   ENDPT_STATE   *endptState, 
   int            cnxId, 
   int            callId, 
   int			  stream_id,
   uint32         saddr,
   uint16         sport,
   uint32         daddr,
   uint16         dport,
   uint8		  mediaMode,
   uint8          qosMark,
   char *ifName
)
{
   ENDPOINTDRV_MAKESESSION_PARM tConnectionParm;
   tConnectionParm.endptIndex = endptState->lineId;
   tConnectionParm.channel = cnxId;
   tConnectionParm.callId = callId;
   tConnectionParm.stream_id = stream_id;
   tConnectionParm.saddr = saddr;
   tConnectionParm.sport = sport;
   tConnectionParm.daddr = daddr;
   tConnectionParm.dport = dport;
   tConnectionParm.mediaMode = mediaMode;   
   tConnectionParm.qosMark = qosMark;
   if(ifName==NULL || ifName[0]=='\0'){
   	memset(tConnectionParm.ifName, 0, 32);
   }
   else{
   	snprintf(tConnectionParm.ifName, 32-1, "%s", ifName);
   }

   //printf("vrgEndptMakeSession endptIndex =>%d, channel =>%d<=\n", endptState->lineId, cnxId);

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_MAKE_SESSION, &tConnectionParm ) != 0 )
   {
      printf("vrgEndptMakeSession: error during ioctl\n");
   }

   return EPSTATUS_SUCCESS;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptDeleteSession
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
EPSTATUS vrgEndptDeleteSession
( 
   ENDPT_STATE   *endptState, 
   int            cnxId,
   int            callId,
   int			  stream_id,
   int * tx_rtp_pkt_cnt, 
   int * tx_rtp_octet_cnt, 
   int * rx_rtp_pkt_cnt, 
   int * rx_rtp_octet_cnt, 
   int * lost
)
{
	ENDPOINTDRV_DELETESESSION_PARM tConnectionParm;
	tConnectionParm.endptIndex = endptState->lineId;
	tConnectionParm.channel = cnxId;  
	tConnectionParm.callId = callId;  
	tConnectionParm.stream_id = stream_id; 


	//printf("vrgEndptDeleteSession endptIndex =>%d, channel =>%d<=\n", endptState->lineId, cnxId);

	if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_DELETE_SESSION, &tConnectionParm ) != 0 )
	{
		printf("vrgEndptDeleteSession: error during ioctl\n");
	}
	else{		
		#if 0
		printf("vrgEndptDeleteSession success\n");
		printf("rtp statistics\n");
		printf("tx_rtp_pkt_cnt = %d\n", tConnectionParm.tx_rtp_pkt_cnt);
		printf("tx_rtp_octet_cnt = %d\n", tConnectionParm.tx_rtp_octet_cnt);
		printf("rx_rtp_pkt_cnt = %d\n", tConnectionParm.rx_rtp_pkt_cnt);
		printf("rx_rtp_octet_cnt = %d\n", tConnectionParm.rx_rtp_octet_cnt);
		printf("lost = %d\n", tConnectionParm.lost);
		#endif
		*tx_rtp_pkt_cnt = tConnectionParm.tx_rtp_pkt_cnt;
		*tx_rtp_octet_cnt = tConnectionParm.tx_rtp_octet_cnt;
		*rx_rtp_pkt_cnt = tConnectionParm.rx_rtp_pkt_cnt;
		*rx_rtp_octet_cnt = tConnectionParm.rx_rtp_octet_cnt;
		*lost = tConnectionParm.lost;
	}

	return EPSTATUS_SUCCESS;
}

/*****************************************************************************
*  FUNCTION:   vrgEndptShowSession
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
EPSTATUS vrgEndptShowSession
(   void    
)
{
	ENDPOINTDRV_SHOWSESSION_PARM tConnectionParm;

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_SHOW_SESSION, &tConnectionParm ) != 0 )
   {
      printf("vrgEndptShowSession: error during ioctl\n");
   }

   printf("vrgEndptShowSession success\n");

   return EPSTATUS_SUCCESS;
}

#endif
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
EPSTATUS vrgEndptModifyConnection
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPZCNXPARAM   *cnxParam
)
{
   ENDPOINTDRV_CONNECTION_PARM tConnectionParm;

   tConnectionParm.cnxId      = cnxId;
   tConnectionParm.cnxParam   = cnxParam;
   tConnectionParm.state      = endptState;
   tConnectionParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tConnectionParm.size       = sizeof(ENDPOINTDRV_CONNECTION_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_MODIFY_CONNECTION, &tConnectionParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tConnectionParm.epStatus );
}


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
EPSTATUS vrgEndptDeleteConnection( ENDPT_STATE *endptState, int cnxId )
{
   ENDPOINTDRV_DELCONNECTION_PARM tDelConnectionParm;

   tDelConnectionParm.cnxId      = cnxId;
   tDelConnectionParm.state      = endptState;
   tDelConnectionParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tDelConnectionParm.size       = sizeof(ENDPOINTDRV_DELCONNECTION_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_DELETE_CONNECTION, &tDelConnectionParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tDelConnectionParm.epStatus );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptMuteConnection
**
** PURPOSE:    Toggle the mute status of a connection on the endpoint.
**
** PARAMETERS: endptState  - endpt object.
**             cxid        - connection identifier
**             mute        - mute status for the connection
**
** RETURNS:    EPSTATUS
**
*****************************************************************************
*/
EPSTATUS vrgEndptMuteConnection ( ENDPT_STATE *endptState, int cnxId, VRG_BOOL mute)
{
   ENDPOINTDRV_MUTECONNECTION_PARM tMuteConnectionParm;

   tMuteConnectionParm.state      = endptState;
   tMuteConnectionParm.cnxId      = cnxId;
   tMuteConnectionParm.mute       = mute;
   tMuteConnectionParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tMuteConnectionParm.size       = sizeof(ENDPOINTDRV_MUTECONNECTION_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_MUTE_CONNECTION, &tMuteConnectionParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tMuteConnectionParm.epStatus );

}

/*****************************************************************************
*  FUNCTION:   vrgEndptLinkInternal
*
*  PURPOSE:    Function to internally link or unlink two endpoints
*
*  PARAMETERS: endptState       - state of the endpt object
*              otherEndptState  - state of the other endpoint object
*              link             - VRG_TRUE to link, VRG_FALSE to unlink
*
*  RETURNS:    EPSTATUS
*
*  NOTE:
*****************************************************************************/
EPSTATUS vrgEndptLinkInternal( ENDPT_STATE *endptState, ENDPT_STATE *otherEndptState, VRG_BOOL link )
{
   ENDPOINTDRV_LINKINTERNAL_PARM tInternalLinkParm;

   tInternalLinkParm.state1   = endptState;
   tInternalLinkParm.state2   = otherEndptState;
   tInternalLinkParm.link     = link;
   tInternalLinkParm.epStatus = EPSTATUS_DRIVER_ERROR;
   tInternalLinkParm.size     = sizeof( ENDPOINTDRV_LINKINTERNAL_PARM );

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_LINK_INTERNAL, &tInternalLinkParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tInternalLinkParm.epStatus );
}

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
EPSTATUS vrgEndptPacket
(
   ENDPT_STATE   *endptState,
   int            cnxId,
   EPPACKET      *epPacket,
   int            length,
   unsigned int   bufDesc
)
{
   ENDPOINTDRV_PACKET_PARM tPacketParm;

   tPacketParm.cnxId       = cnxId;
   tPacketParm.state       = endptState;
   tPacketParm.length      = length;
   tPacketParm.bufDesc     = bufDesc;
   tPacketParm.epPacket    = epPacket;
   tPacketParm.epStatus    = EPSTATUS_DRIVER_ERROR;
   tPacketParm.size        = sizeof(ENDPOINTDRV_PACKET_PARM);

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ENDPT_PACKET, &tPacketParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   return( tPacketParm.epStatus );
}


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
EPSTATUS vrgEndptConsoleCmd( ENDPT_STATE *endptState, EPCONSOLECMD cmd, EPCMD_PARMS *consoleCmdParams )
{
   ENDPOINTDRV_CONSOLE_CMD_PARM tConsoleParm;
   int fileHandle;

   fileHandle = open("/dev/bcmendpoint0", O_RDWR);

   tConsoleParm.state      = endptState;
   tConsoleParm.cmd        = cmd;
   tConsoleParm.lineId     = endptState->lineId;
   tConsoleParm.consoleCmdParams = consoleCmdParams;
   tConsoleParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tConsoleParm.size       = sizeof(ENDPOINTDRV_CONSOLE_CMD_PARM);

   if ( ioctl( fileHandle, ENDPOINTIOCTL_ENDPT_CONSOLE_CMD, &tConsoleParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   close(fileHandle);

   return( tConsoleParm.epStatus );
}

int isEndptInitialized( void )
{
   ENDPOINTDRV_ISINITIALIZED_PARM isInitParm;
   int retVal = 0;
   
   isInitParm.size = sizeof(ENDPOINTDRV_ISINITIALIZED_PARM);
   #if 1 /*MTS_VOICE*/
		 /* Open and initialize Endpoint driver */
		 if( ( endptUserCtrlBlock.fileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
		 {
			LOG(( "Endpoint: open error %d", errno ));
		 }
#endif

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_ISINITIALIZED, &isInitParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }
   else
   {
      retVal = isInitParm.isInitialized;
   }

   return( retVal );
}

#if 1 /* MTS_VOICE */
/* 
*****************************************************************************
** FUNCTION:   vrgEndptGetFxsHookStatus
**
** PURPOSE:    Get current Hook status of FXS line
**
** PARAMETERS: endptState  - state of the endpt object
**             stat       - hook status
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptGetFxsHookStatus( ENDPT_STATE   *endptState,EPCASSTATE *stat)
{
	ENDPOINTDRV_HOOKSTAT_CMD_PARM tHookstatParm;
	int fileHandle;

	tHookstatParm.epStatus   = EPSTATUS_DRIVER_ERROR;
	tHookstatParm.hookstat   = EPCAS_ONHOOK;
	tHookstatParm.lineId     = endptState->lineId;
	tHookstatParm.size       = sizeof(ENDPOINTDRV_HOOKSTAT_CMD_PARM);

	fileHandle = open("/dev/bcmendpoint0", O_RDWR);
	if(fileHandle == -1){
		return EPSTATUS_ENDPT_UNKNOWN;
	}

	if ( ioctl( fileHandle, ENDPOINTIOCTL_TEST, &tHookstatParm ) != 0 ) {
		LOG(("endptGetHookStatus: error during ioctl"));
	}
	*stat = tHookstatParm.hookstat;
	close(fileHandle);
	return( tHookstatParm.epStatus );
}
#endif
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
EPSTATUS vrgEndptGetHookStatus( EPCASSTATE *stat )
{
   ENDPOINTDRV_HOOKSTAT_CMD_PARM tHookstatParm;
   int fileHandle;

   tHookstatParm.epStatus   = EPSTATUS_DRIVER_ERROR;
   tHookstatParm.hookstat   = EPCAS_ONHOOK;
   tHookstatParm.lineId     = 0;
   tHookstatParm.size       = sizeof(ENDPOINTDRV_HOOKSTAT_CMD_PARM);

//printf("tHookstatParm.size =%d, endpointdrv_hookstat_cmd_parm=%d\n", tHookstatParm.size, sizeof(ENDPOINTDRV_HOOKSTAT_CMD_PARM));

   fileHandle = open("/dev/bcmendpoint0", O_RDWR);

   if ( ioctl( fileHandle, ENDPOINTIOCTL_HOOKSTAT, &tHookstatParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   *stat = tHookstatParm.hookstat;

   close(fileHandle);
   return( tHookstatParm.epStatus );
}

/* pstn lifeline support
*****************************************************************************
** FUNCTION:   vrgEndptSetRelayStat
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
EPSTATUS vrgEndptActiveRelay( VRG_BOOL cmd )
{
   ENDPOINTDRV_RELAY_CMD_PARM tRelayParm;
   int fileHandle;

   tRelayParm.cmd        = cmd;
   tRelayParm.lineId     = 0;
   tRelayParm.size       = sizeof(ENDPOINTDRV_RELAY_CMD_PARM);

//printf("tRelayParm.size = %d\n", tRelayParm.size);

   fileHandle = open("/dev/bcmendpoint0", O_RDWR);

   if ( ioctl( fileHandle, ENDPOINTIOCTL_SET_RELAY, &tRelayParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(("%s: error during ioctl", __FUNCTION__));
   }

   close(fileHandle);
   return( tRelayParm.epStatus );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptLogOpen
**
** PURPOSE:    Open the endpoint driver for user-space logging
**
** PARAMETERS: None
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptLogOpen()
{
   if( ( endptUserCtrlBlock.logFileHandle = open("/dev/bcmendpoint0", O_RDWR) ) == -1 )
   {
      printf("Endpoint: open error %d", errno);
   }

   return( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptLogClose
**
** PURPOSE:    Close endpoint driver after all user-space logs are printed
**
** PARAMETERS: None
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptLogClose()
{
   if ( close( endptUserCtrlBlock.logFileHandle ) == -1 )
   {
      printf("Endpoint: close error %d", errno);
   }

   return( EPSTATUS_SUCCESS );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptPrintf
**
** PURPOSE:    Use ioctl to printf user-space message
**
** PARAMETERS: fmt         - Unformatted string to be printed
**             ...         - String paramters to be formatted
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptPrintf( char *fmt, va_list vList)
{
   ENDPOINTDRV_PRINTF_PARM printfParm;
   char str[MAX_PRINTF_SIZE];
   struct tm *tm_ptr;
   time_t curtime;

   time( &curtime );
   tm_ptr = gmtime( &curtime );

   vsnprintf(str, MAX_PRINTF_SIZE-1, fmt, vList);
   str[MAX_PRINTF_SIZE-1] = '\0';
   
   printfParm.size = MAX_PRINTF_SIZE;
   snprintf(printfParm.str, MAX_PRINTF_SIZE-1, "%02d:%02d:%02d %s\n",
           tm_ptr->tm_hour,
           tm_ptr->tm_min,
           tm_ptr->tm_sec,
           str);

   printfParm.str[MAX_PRINTF_SIZE - 1] = '\0';
   if ( ioctl( endptUserCtrlBlock.logFileHandle, ENDPOINTIOCTL_PRINTF, &printfParm ) != IOCTL_STATUS_SUCCESS )
   {
      printf(("%s: error during ioctl\n", __FUNCTION__));
   }

   return( EPSTATUS_SUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptProvSet
**
** PURPOSE:    Set a value to the endpoint provisioning database
**             The application would use this API to store a value
**             in the endpoint provisioning database so that the
**             parameter's value is directly available to the endpoint.
**
** PARAMETERS: line           -  [IN]  Line id
**             provItemId     -  [IN]  Provisioning item id
**             provItemValue  -  [IN]  Pointer to the variable whose value needs to be
**                                     stored in the endpoint provisioning database
**             provItemLength -  [IN]  Length/Size of the variable whose value needs to be
**                                     stored in the endpoint provisioning database.
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptProvSet( int line, EPPROV provItemId, void* provItemValue, int provItemLength )
{
   ENDPOINTDRV_PROV_PARM provParm;

   provParm.size           = sizeof(ENDPOINTDRV_PROV_PARM);
   provParm.provItemId     = provItemId;
   provParm.provItemValue  = provItemValue;
   provParm.provItemLength = provItemLength;
   provParm.line           = line;
   provParm.epStatus       = EPSTATUS_DRIVER_ERROR;

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_PROV_SET, &provParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   return( provParm.epStatus );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptProvGet
**
** PURPOSE:    Get a value from the endpoint provisioning database
**             The application would use this API to get a value
**             that is currently stored in the endpoint provisioning database.
**
** PARAMETERS: line           -  [IN]  Line id
**             provItemId     -  [IN]  Provisioning item id
**             provItemValue  -  [OUT] Pointer to the variable that will be
**                                     filled with the current value in the
**                                     endpoint provisioning database
**             provItemLength -  [IN]  Length/Size of the variable whose value needs to be
**                                     stored in the endpoint provisioning database.
**
** RETURNS:    EPSTATUS
**
** NOTE: The caller of this function should allocate memory for provItemValue
**
*****************************************************************************
*/
EPSTATUS vrgEndptProvGet( int line, EPPROV provItemId, void* provItemValue, int provItemLength )
{
   ENDPOINTDRV_PROV_PARM provParm;

   provParm.size           = sizeof(ENDPOINTDRV_PROV_PARM);
   provParm.provItemId     = provItemId;
   provParm.provItemValue  = provItemValue;
   provParm.provItemLength = provItemLength;
   provParm.line           = line;
   provParm.epStatus       = EPSTATUS_DRIVER_ERROR;

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_PROV_GET, &provParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   return( provParm.epStatus );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptProbeSet
**
** PURPOSE:    Debug SLAC registers. 
**
** PARAMETERS: 
**
** RETURNS:    EPSTATUS
**
** NOTE:
*****************************************************************************
*/
EPSTATUS vrgEndptProbeSet( int deviceId, int line, int reg, int regSize, void* probeValue, int probeValueSize, int indirectValue )
{
   ENDPOINTDRV_PROBE_PARM probeParm;

   probeParm.size              = sizeof(ENDPOINTDRV_PROBE_PARM);
   probeParm.deviceId          = deviceId;
   probeParm.chan              = line;
   probeParm.reg               = reg;
   probeParm.regSize           = regSize;
   probeParm.probeValue        = probeValue;
   probeParm.probeValueLength  = probeValueSize;
   probeParm.indirectness      = indirectValue;
   probeParm.epStatus          = EPSTATUS_DRIVER_ERROR;

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_PROBE_SET, &probeParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   return( probeParm.epStatus );
}


/*
*****************************************************************************
** FUNCTION:   vrgEndptProbeGet
**
** PURPOSE:    Debug SLAC registers. 
**
** PARAMETERS: 
**
** RETURNS:    EPSTATUS
**
** NOTE: The caller of this function should allocate memory for provItemValue
**
*****************************************************************************
*/
EPSTATUS vrgEndptProbeGet( int deviceId, int line, int reg, int regSize, void* probeValue, int probeValueSize, int indirectValue )
{
   ENDPOINTDRV_PROBE_PARM probeParm;

   probeParm.size              = sizeof(ENDPOINTDRV_PROBE_PARM);
   probeParm.deviceId          = deviceId;
   probeParm.chan              = line;
   probeParm.reg               = reg;
   probeParm.regSize           = regSize;
   probeParm.probeValue        = probeValue;
   probeParm.probeValueLength  = probeValueSize;
   probeParm.indirectness      = indirectValue;
   probeParm.epStatus          = EPSTATUS_DRIVER_ERROR;

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );

   if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPOINTIOCTL_PROBE_GET, &probeParm ) != IOCTL_STATUS_SUCCESS )
   {
      LOG(( "%s: error during ioctl", __FUNCTION__ ));
   }

   return( probeParm.epStatus );
}

/*
*****************************************************************************
** FUNCTION:   vrgEndptSetRtpDroppedPacketResetCallback
**
** PURPOSE:    Register a callback function responsible for resetting the
**             the dropped RTP packets counter back to it's default value.
**
** PARAMETERS: callback - [IN] The callback function to register.
**
** RETURNS:    Nothing
**
** NOTE: This function is only used in debug mode.
**
*****************************************************************************
*/
void vrgEndptSetRtpDroppedPacketResetCallback(void *callback)
{
   endptUserCtrlBlock.pRtpDropPacketResetCallBack = (rtpDropPacketResetCallback)callback;
}

#ifdef MSTC_VOICE_GR909 /* Support GR909 function,__MSTC_VOICE__,Klose,20121004*/
EPSTATUS vrgEndptVoiceLineTest(ENDPOINTDRV_LT_STATISTICS_PARM *lineTestParm)
{
   FILE *fp=NULL;	

   /* Check if kernel driver is opened */
   BOS_ASSERT( endptUserCtrlBlock.fileHandle != NOT_INITIALIZED );	

	if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPTIO_GET_LT_STATISTICS, lineTestParm ) != IOCTL_STATUS_SUCCESS )
	{
		printf("vrgEndptVoiceLineTest: error during ioctl\n");
	}
	else{
      if(lineTestParm->mode == 1){
         if ((fp = fopen(FILE_LTEST_RESULT,"w")) == NULL){
            printf( "****************can't open file:%s***********************\r\n", FILE_LTEST_RESULT);
            fprintf(stderr, "can't open file:%s\r\n", FILE_LTEST_RESULT);
         }else{
            printf("****************Open file successful:%s***********************\r\n",FILE_LTEST_RESULT);
            if(lineTestParm->fltMask_1){
               fprintf(fp, "  FAIL \n");
            }else{
               fprintf(fp, "  PASS \n");
            }

            fclose(fp);

            printf("LtLineVMask = 0x%04x\n", lineTestParm->fltMask_1);
            printf("vAcTip = %li mV\n", lineTestParm->vAcTip);
            printf("vAcRing = %li mV\n", lineTestParm->vAcRing);
            printf("vAcDiff = %li mV\n", lineTestParm->vAcDiff);
            printf("vDcTip = %li mV\n", lineTestParm->vDcTip);
            printf("vDcRing = %li mV\n", lineTestParm->vDcRing);
            printf("vDcDiff = %li mV\n", lineTestParm->vDcDiff);
            printf("measStatus = %hu\n", lineTestParm->measStatus_1);
         }
      }else if(lineTestParm->mode == 2){
         if ((fp = fopen(FILE_LTEST_RESULT,"w")) == NULL){
            printf( "****************can't open file:%s***********************\r\n", FILE_LTEST_RESULT);
            fprintf(stderr, "can't open file:%s\r\n", FILE_LTEST_RESULT);
         }else{
            printf("****************Open file successful:%s***********************\r\n",FILE_LTEST_RESULT);
            if(lineTestParm->fltMask_2){
               fprintf(fp, "  OFF HOOK \n");
            }else{
               fprintf(fp, "  ON HOOK \n");
            }
            fclose(fp);

      		printf("%s, fltMask = 0x%04x\n",__FUNCTION__, lineTestParm->fltMask_2);
      		printf("%s, rLoop1 = %li 1/10 Ohm \n",__FUNCTION__, lineTestParm->rLoop1);
      		printf("%s, rLoop2 = %li 1/10 Ohm \n",__FUNCTION__, lineTestParm->rLoop2);
      		printf("%s, measStatus = %hu\n",__FUNCTION__, lineTestParm->measStatus_2);
         }
      }else if(lineTestParm->mode == 3){
         if ((fp = fopen(FILE_LTEST_RESULT,"w")) == NULL){
            printf( "****************can't open file:%s***********************\r\n", FILE_LTEST_RESULT);
            fprintf(stderr, "can't open file:%s\r\n", FILE_LTEST_RESULT);
         }else{
            printf("****************Open file successful:%s***********************\r\n",FILE_LTEST_RESULT);

            int iren = (int)(lineTestParm->ren/1000)+1;/* ren number */
            if(lineTestParm->ren > 20){
               fprintf(fp, "  %d Phone \n\n", iren);
               fprintf(fp, "(Depend on different phone, it shows not the same result.)\n");
               fprintf(fp, "(Note : Please check the phone is on-hook.)\n");
            }else{
               fprintf(fp, "  0 Phone \n");
            }
            fclose(fp);
            
            printf("LtRingersMask = 0x%04x\n", lineTestParm->fltMask_3);
            printf("ren = %li milli REN\n", lineTestParm->ren);
            printf("rentg = %li milli REN\n", lineTestParm->rentg);
            printf("renrg = %li milli REN\n", lineTestParm->renrg);
            printf("measStatus = %hu\n", lineTestParm->measStatus_3);
         }
      }else if(lineTestParm->mode == 4){
         if ((fp = fopen(FILE_LTEST_RESULT,"w")) == NULL){
            printf( "****************can't open file:%s***********************\r\n", FILE_LTEST_RESULT);
            fprintf(stderr, "can't open file:%s\r\n", FILE_LTEST_RESULT);
         }else{
            printf("****************Open file successful:%s***********************\r\n",FILE_LTEST_RESULT);
            if(lineTestParm->fltMask_4){
               fprintf(fp, "  FAIL \n");
            }else{
               fprintf(fp, "  PASS \n");
            }

            fclose(fp);

            printf("LtResFltMask = 0x%04x\n", lineTestParm->fltMask_4);
            printf("rtg = %li 1/10 Ohm \n", lineTestParm->rtg);
            printf("rrg = %li 1/10 Ohm \n", lineTestParm->rrg);
            printf("rtr = %li 1/10 Ohm \n", lineTestParm->rtr);
            printf("rGnd = %li 1/10 Ohm \n", lineTestParm->rGnd);
            printf("measStatus = %hu\n", lineTestParm->measStatus_4);            
         }
      }
	}

	return EPSTATUS_SUCCESS;
	
}
#endif

EPSTATUS vrgEndptGetRtpStatistics
( 
   int            cnxId,
   int            phyId,
   int * tx_rtp_pkt_cnt, 
   int * tx_rtp_octet_cnt, 
   int * rx_rtp_pkt_cnt, 
   int * rx_rtp_octet_cnt, 
   int * lost,
   int * jitter
)
{
	ENDPOINTDRV_GET_RTP_STATISTICS_PARM tConnectionParm;
	tConnectionParm.channel = cnxId;  
	tConnectionParm.endptIndex = phyId;  

	//printf("vrgEndptGetRtpStatistics endptIndex =>%d, channel =>%d<=\n", phyId, cnxId);

	if ( ioctl( endptUserCtrlBlock.fileHandle, ENDPTIO_GET_RTP_STATISTICS, &tConnectionParm ) != 0 )
	{
		printf("vrgEndptGetRtpStatistics: error during ioctl\n");
	}
	else{		
		#if 0
		printf("vrgEndptGetRtpStatistics success\n");
		printf("rtp statistics\n");
		printf("tx_rtp_pkt_cnt = %d\n", tConnectionParm.tx_rtp_pkt_cnt);
		printf("tx_rtp_octet_cnt = %d\n", tConnectionParm.tx_rtp_octet_cnt);
		printf("rx_rtp_pkt_cnt = %d\n", tConnectionParm.rx_rtp_pkt_cnt);
		printf("rx_rtp_octet_cnt = %d\n", tConnectionParm.rx_rtp_octet_cnt);
		printf("lost = %d\n", tConnectionParm.lost);
		printf("jitter = %d\n", tConnectionParm.jitter);
		#endif
		*tx_rtp_pkt_cnt = tConnectionParm.tx_rtp_pkt_cnt;
		*tx_rtp_octet_cnt = tConnectionParm.tx_rtp_octet_cnt;
		*rx_rtp_pkt_cnt = tConnectionParm.rx_rtp_pkt_cnt;
		*rx_rtp_octet_cnt = tConnectionParm.rx_rtp_octet_cnt;
		*lost = tConnectionParm.lost;
		*jitter = tConnectionParm.jitter;
	}

	return EPSTATUS_SUCCESS;
}


