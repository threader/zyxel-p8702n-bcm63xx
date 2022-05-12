/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: endpoint.c
*    Creation Date: 8 June 2000 (v0.00)
*    VSS Info:
*        $Revision: 28 $
*        $Date: 7/24/01 6:25p $
*
****************************************************************************
*    Description:
*
*      Endpoint Abstraction Layer
*
****************************************************************************/
#include <stdio.h>
#include <log.h>
#include <errno.h>
#include <fcntl.h>      /* open */
#include <unistd.h>     /* exit */
#include <sys/ioctl.h>  /* ioctl */
#include <endpointdrv.h>
//#include <endptarchive.h>              /* Endpoint archive type */
//#include <bcmos.h>
//#include <rtp.h>


static int testfile = -1;
void endpointKernelTest( void )
{
   ENDPOINTDRV_TESTPARM testParm = {0, 0, 0};

   LOG(("endpointKernelTest"));

   if((testfile = open("/dev/bcmendpoint0", O_RDWR)) == -1)
   {
      LOG(( "Endpoint: open error %d", errno ));
   }
   else
   {
      LOG(("user:Invoking endpoint kernel test"));
      ioctl( testfile, ENDPOINTIOCTL_TEST, &testParm );
   }
}

#if 0

extern UINT32 GetLocalIp( void );


/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/

#define RTP_PORT1                3456        /* First RTP port */

/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/

/* Mapping of endpt & cnxid */
typedef struct GWZCNXMAP
{
   int endpt;
   UINT16 rtpPort;                     /* rtp port number */
   int rtpHandle;                      /* rtp socket handle */
} GWZCNXMAP;

static GWZCNXMAP gwCnxMap[MAX_CNX];


static int testfile = -1;

static void eventCallback(int lineId, int cnxId, EPEVT event);

#ifdef ENDPOINT_NULL_SHIM
static void packetCallback( int cnxId, HSZNETPACKET *netPacket, int length );
#else
static void packetCallback( int cnxId, RTPPACKET *netPacket, int length );
#endif

static void gwInitData( void );
static void packetCallback_rtptest( int cnxId, RTPPACKET *netPacket, int length );
static void gwPacketReceive( int cxid, RTPPACKET *packet, int length );

void endpointUserTest( void )
{
   EPSTATUS result;
   UINT i;
   UINT numEndPoints;
   CODECLIST  codecListLocal;
   CODECLIST  codecListRemote;
   EPZCAP epCap;
   EPZCNXPARAM epCnxParms;

   LOG(("endpointUserTest"));

    codecListLocal.numCodecs = MAX_CODECS;
    codecListLocal.codecs[0].type = CODEC_PCMU;
    codecListLocal.codecs[0].rtpPayloadType = RTP_PAYLOAD_PCMU;
    codecListLocal.codecs[1].type = CODEC_PCMA;
    codecListLocal.codecs[1].rtpPayloadType = RTP_PAYLOAD_PCMA;
    codecListLocal.codecs[2].type = CODEC_G726_32;
    codecListLocal.codecs[2].rtpPayloadType = RTP_PAYLOAD_G726_32;

    codecListRemote.numCodecs = MAX_CODECS;
    codecListRemote.codecs[0].type = CODEC_PCMU;
    codecListRemote.codecs[0].rtpPayloadType = RTP_PAYLOAD_PCMU;
    codecListRemote.codecs[1].type = CODEC_PCMA;
    codecListRemote.codecs[1].rtpPayloadType = RTP_PAYLOAD_PCMA;
    codecListRemote.codecs[2].type = CODEC_G726_32;
    codecListRemote.codecs[2].rtpPayloadType = RTP_PAYLOAD_G726_32;

#if (MAX_CODECS<3)
#error Codec list initialization: wrong array size.
#endif

   /* Initialize Endpoint API */

   result = endptStartup( EPCOUNTRY_NORTH_AMERICA, eventCallback, packetCallback );
   if(result != EPSTATUS_SUCCESS)
   {
      LOG(("endptStartup failed (%d)", result));
   }
   else
   {
       LOG(("endptStartup succeeded"));

      numEndPoints = endptGetNumEndPoints( EPTYPE_VOCM );
      numEndPoints = 2;

      /* initialize endpoints */
      for ( i = 0; i < numEndPoints; i++)
      {
         result = endptInit(EPTYPE_VOCM, (short)i, i+1);
         if( result != EPSTATUS_SUCCESS )
         {
            LOG(("endptInit failed for endpoint %d", i+1));
			   continue;
         }

         /* get endpoint capabilities */
         result = endptCapabilities( i+1, &epCap );
         if( result != EPSTATUS_SUCCESS )
         {
   		   LOG(("endptInit failed for endpoint %d\n", i+1));
			   continue;
         }

         /* Set up local connection parameters. Eventually these parameters
         must come from external source (eg MIB or Config Database) */
         epCnxParms.mode  =   EPCNXMODE_SNDRX;
         epCnxParms.codecListLoc = codecListLocal;
         epCnxParms.codecListRem = codecListRemote;
         epCnxParms.period = 5;
         epCnxParms.echocancel = 1;
         epCnxParms.silence = 0;
         epCnxParms.pktsize = CODEC_G711_PAYLOAD_BYTE;   /* Not used ??? */

         result = endptCreateConnection( i+1, i+1, &epCnxParms );

         if ( result != EPSTATUS_SUCCESS )
         {
            LOG(("endptCreateConnection failed for endpoint %d", i+1));
         }
      }
      LOG(("Test initialization completed"));
   }
}



void endpointUserRtpTest( void )
{
   LOG(("endpointUserRtpTest"));

   EPZCNXPARAM param;
   UINT32      numLocalEndPoints;
   int         i;

   UINT32 myaddr = GetLocalIp();

   numLocalEndPoints = 4;

   gwInitData();

   endptStartup( EPCOUNTRY_NORTH_AMERICA, eventCallback, packetCallback_rtptest );

   for ( i = 0; i < numLocalEndPoints; i++ )
   {
      endptInit( EPTYPE_VOCM, i, i+1 );
   }

   rtpInit();

   for( i=0; i<MAX_CNX; i++ )
   {
      RTPSTATUS r;
      r = rtpOpen( &gwCnxMap[i+1].rtpHandle, myaddr, gwCnxMap[i+1].rtpPort, gwPacketReceive );
      LOG(("rtpOpen: Return code: %d, rsHandle=%d\n", r, gwCnxMap[i+1].rtpHandle));
   }


   param.mode = EPCNXMODE_SNDRX;
   param.codecListLoc.numCodecs = 1;
   param.codecListLoc.codecs[0].type = CODEC_PCMU;
   param.codecListLoc.codecs[0].rtpPayloadType = RTP_PAYLOAD_PCMU;
   param.codecListRem.numCodecs = 1;
   param.codecListRem.codecs[0].type = CODEC_PCMU;
   param.codecListRem.codecs[0].rtpPayloadType = RTP_PAYLOAD_PCMU;
   param.namedPhoneEvts = 0;
   param.period = 5;
   param.echocancel = 1;
   param.silence = 0;

#if 0
   for ( i = 0; i < MAX_CNX; i++ )
   {
      rtpSetRTPRemote( gwCnxMap[i+1].rtpHandle, myaddr, 3456 + i*2, RTP_TTL );
      endptCreateConnection( i+1, i+1, &param );
   }
#else
   for ( i = 0; i < MAX_CNX; i++ )
   {
      endptCreateConnection( i+1, i+1, &param );
   }
   rtpSetRTPRemote( gwCnxMap[1].rtpHandle, myaddr, 3458, RTP_TTL );  /* line 1 sends to port 3458 (line 2) */
   rtpSetRTPRemote( gwCnxMap[2].rtpHandle, myaddr, 3456, RTP_TTL );  /* line 2 sends to port 3456 (line 1) */
   rtpSetRTPRemote( gwCnxMap[3].rtpHandle, myaddr, 3462, RTP_TTL );  /* line 3 sends to port 3462 (line 4) */
   rtpSetRTPRemote( gwCnxMap[4].rtpHandle, myaddr, 3460, RTP_TTL );  /* line 4 sends to port 3460 (line 3) */
#endif
   //endptCreateConnection( 1, 1, &param );

   /* The same endpoint parameters are used for the second line
   ** except for the endpoint connection mode (set to receive-only). */
   //param.mode = EPCNXMODE_RCVO;
   //endptCreateConnection( 2, 2, &param );
}


static int cnxTable[4] = {2, 1, 4, 3};
#if 0
static int hookStatus [4] = {0, 0, 0, 0};
static int isOffHook(int lineId)
{
   if( lineId >0 && lineId <= 4)
      return hookStatus[lineId-1];
   else
      return 0;
}
static void hookState(int lineId, int state)
{
   if( lineId >0 && lineId <= 4)
      hookStatus[lineId-1] = state;
}
#endif

static int findPair(int lineId)
{
   if( lineId >0 && lineId <= 4)
      return cnxTable[lineId-1];
   else
      return 0;
}

static void eventCallback(int lineId, int cnxId, EPEVT event)
{
   if(event == EPEVT_OFFHOOK)
   {
#if 0
      hookState(lineId, 1);

      endptSignal( lineId, -1, EPSIG_RINGING,  0 );
      if(!isOffHook(findPair(lineId)))
      {
         endptSignal( findPair(lineId), -1, EPSIG_RINGING,  1 );
      }
#endif
   }
   else if(event == EPEVT_ONHOOK)
   {
#if 0
      hookState(lineId, 0);

      endptSignal( lineId, -1, EPSIG_RINGING,  0 );
      endptSignal( findPair(lineId), -1, EPSIG_RINGING,  0 );
#endif
   }
   else
   {
      LOG(("Endpoint event lineId %d, cnxId %d, event %d", lineId, cnxId, event ));
   }
}

#ifdef ENDPOINT_NULL_SHIM
static void packetCallback( int cnxId, HSZNETPACKET *netPacket, int length )
#else
static void packetCallback( int cnxId, RTPPACKET *rtpPacket, int length )
#endif
{
   int lineOut;
   lineOut = findPair(cnxId);
   if(lineOut)
   {
#ifdef ENDPOINT_NULL_SHIM
      endptNetPacket( lineOut, netPacket, length );
#else
      endptRtpPacket( lineOut, rtpPacket, length );
#endif
    }
}

static void packetCallback_rtptest( int cnxId, RTPPACKET* rtpPacket, int length )
{
   rtpSend( gwCnxMap[cnxId].rtpHandle, (void*)rtpPacket, length, 0 );
}

static void gwPacketReceive( int cnxId, RTPPACKET *rtpPacket, int length )
{
   endptRtpPacket( cnxId+1, (RTPPACKET*)rtpPacket, length );
}

static void gwInitData( void )
{
   int   cnx;

   /* Initialize Connection MAP */
   for (cnx = 0; cnx < MAX_CNX; cnx++)
   {
      gwCnxMap[cnx+1].endpt = -1;
      gwCnxMap[cnx+1].rtpPort = (unsigned short) (RTP_PORT1 + 2 * cnx);
      gwCnxMap[cnx+1].rtpHandle = -1;
   }
}


#endif
