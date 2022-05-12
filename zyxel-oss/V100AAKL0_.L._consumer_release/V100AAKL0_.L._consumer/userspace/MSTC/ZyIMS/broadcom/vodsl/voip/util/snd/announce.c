


#include <vrgCmgrEnv.h>
#include <vrgCmgrApi.h>
#include <vrgCmgrLog.h>
#include <env.h>
#include <snd.h>
#include <endpt.h>
#include <vrgEndpt.h>
/* 
#include <vrgCmgrApi.h>
#include <vrgCmgrStats.h>
*/
#include "vodsl_annce.h"

#define VODSL_ANNCE_MAX    1

ANNCEINFO gAnnceInfo[VODSL_ANNCE_MAX] = 
{
   {
      CODEC_PCMU,
      20,
      RTP_PAYLOAD_PCMU,
      sizeof(vodsl_annce_array),
      vodsl_annce_array
   }
};

static ANNCEINFO * sndGetAnnouncement(ANNCERSN reason,  ANNCEINFO *annceInfo);
static void sndEventCB( int streamId, SNDEVT sndEvt, const char *buffer, int length );

/*
*****************************************************************************
** FUNCTION:   sndPlayAnnouncement
**
** PURPOSE:    Play an announcement for the specified "reason". A downloaded
**             announcement will be played if it exists or a signal will be
**             generated on the endpoint.
**
** PARAMETERS: endptState - pointer to the endpoint state structure
**             reason     - The reason for requested announcement.
**
** RETURNS:    Nothing
*****************************************************************************
*/
void sndPlayAnnouncement( ANNCECMD annceCmd, ENDPT_STATE *endptState, int streamId, ANNCERSN reason )
{
   ANNCEINFO *announcement;

   LOG(("sndPlayAnnoucement\n"));  

   switch(annceCmd)
   {
      case ANNCECMD_PLAY:
      {	  	
         announcement = sndGetAnnouncement(reason, gAnnceInfo);

         /* Check whether an announcement has been downloaded for this reason. */
         if( announcement != NULL )
         {
            if( announcement->length > 0 )
            {
                  /* First try to stop the reorder tone so we're not trying to play
                   * an announcement over a signal. This is a weird case where an
                   * announcement gets downloaded after the reorder tone was
                   * requested and nothing stopped the reorder tone. */
                  /* gwSignal( endptState->lineId, -1, MGES_REORDER, 0, -1, -1, -1 ); */				 

                sndOutPlay( endptState,
                            streamId,
                            announcement->bufp,
                            announcement->length,
                            announcement->eptCodec,
                            announcement->pktPeriod,
                            announcement->rtpCode,
                            sndEventCB );

            }
            else
            {
                /* Downloaded file invalid. Print an error message and fall through
                * to the default. */
                LOG(("ERROR: announcement not valid. Using default tone.\n"));
            }
         }
         else
         {
          /* No announcement was downloaded. Play reorder tone by default. */
	   /* 
             if( gwSignal( endptState->lineId, -1, MGES_REORDER, 1, -1, -1, -1 ) != MGERSP_SUCCESS )
             {
                 LOG(("ERROR: Could not generate default tone in place of announcement.\n"));
             } */
         }
      }
      break;
      case ANNCECMD_STOP:
      default:
      {
         sndOutStop(streamId);
      }
   }
}

static ANNCEINFO * sndGetAnnouncement(ANNCERSN reason,  ANNCEINFO *annceInfo)
{

  /* current only support one announcement */
  return ( &(annceInfo[0]) );
}

void sndEventCB( int streamId, SNDEVT sndEvt, const char *buffer, int length )
{
  VRGCMGR_STATUS  rc;
   /* empty call back function 
   ** this function can be used as clean up resouce after
   ** finished announcement playout 
   */
  LOG(("sndEventCB\n"));
   printf("sendEventCB\n");
   #if 0
#ifdef IP_SIP  
  rc = vrgCmgrSignal( 0 , VRGCMGR_CMD_RELEASE_ANNCESTREAM, &streamId, NULL );
#elif IP_MGCP
  gwSndEventCB( streamId, sndEvt, buffer, length );
#endif
	#endif

}
