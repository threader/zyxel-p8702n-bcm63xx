/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/

/***************************************************************************
 * Include Files area
 ***************************************************************************/

#include <sys/times.h>
#include "mm_core_fsm_types.h"
#include "mm_macros.h"
#include "mm_port_wrapper.h"
#include "mm_core_rtp.h"
#include "icf_types.h"
#include "mm_glb.h"
#include "switch.h"

#ifdef DUMP_RTCP_PKT
/*
*****************************************************************************
** FUNCTION:   dispRTCPPacket
**
** PURPOSE:    Displays packet type information when RTCP packet recieved
**
** PARAMETERS: packetBuffer - pointer to RTCPPACKET packet
**             bufferLength - length of RTCPPACKET packet
**
** RETURNS:    Nothing
**
** NOTE:
*****************************************************************************
*/
static void dispRTCPPacket(
	unsigned char* packetBuffer, 
	int bufferLength)
{
   int             i, k;
   unsigned char  *ptr        = packetBuffer;
   unsigned int    length     = 0;
   unsigned int    byteOffset = 0;
   unsigned int    lengthSum  = 0;
   unsigned int    headerType = 0;

   RTCP_PRINTF( "\n\nRTCP Packet\n" );
   RTCP_PRINTF( "Header types recieved: " );

   /* While there are still more RTCP packets in the compound RTCP packet,
      print out the packet types found */
   while(!( lengthSum == bufferLength ))
   {
      headerType = *(ptr + 1);
      length = *(ptr + 3);
      byteOffset = (length+1)*4;
      lengthSum = lengthSum + byteOffset;

      /* Print current packet type found in header */
      switch(headerType)
      {
         case 200:
         {
            RTCP_PRINTF( "Sender Report" );
         }
         break;

         case 201:
         {
            RTCP_PRINTF( "Receiver Report" );
         }
         break;

         case 202:
         {
            RTCP_PRINTF( "Source Description" );
         }
         break;

         case 203:
         {
            RTCP_PRINTF( "Goodbye" );
         }
         break;

         case 204:
         {
            RTCP_PRINTF( "Application-Defined" );
         }
         break;

         default:
         break;
      }

      if(!( lengthSum == bufferLength ))
      {
         /* Another RTCP packet in compounded packet */
         RTCP_PRINTF( ", " );
      }
      else
      {
         /* End of compound packet */
         RTCP_PRINTF( "\n\n" );
         break;
      }

      /* Move pointer to beginning of next packet */
      ptr = ptr + byteOffset;
   }
   
   /* Output the raw packet in hex */
   for (i=0,k=1; i<bufferLength; packetBuffer++, i++, k++)
   {
      RTCP_PRINTF("0x%02x ", *packetBuffer);
      if (k == 16)
      {
         k=0;
         RTCP_PRINTF("\n");
      }
      fflush(stdout);
   }

   RTCP_PRINTF("\n\n");
}
/*______________________________________________________________________________
**	rtcp_debug_display
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void
rtcp_debug_display(
	icf_uint16_t begin,
	icf_uint16_t end,
	icf_uint8_t *bp
) {
	int i;
	icf_uint8_t  *data;

	data = bp;

	for( i = begin ; i < end ; i++){
		if( ((i-begin)!= 0) && ((i-begin)%10 == 0))
			RTCP_PRINTF("\n");
		RTCP_PRINTF("%02x ", *(data+i));
	}
	return;
}
#endif


/*______________________________________________________________________________
**	rtcpParseRR
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
#if 1 /*Jason , rtp statistic*/
void rtcpParseRR(
	rtpTable_t *table,
	rtcp_rr_t *p_rr,
	struct timeval recv_time_tv
){
	icf_uint32_t recv_time_sec;	   
	icf_uint32_t recv_time_frac;
	icf_uint32_t recv_time;

	p_rr->ssrc = ntohl(p_rr->ssrc);

	if ( p_rr->ssrc == table->ssrc )
	{
		table->rtcp_stats.rtp_rr.lsr = ntohl(p_rr->lsr);
		table->rtcp_stats.rtp_rr.dlsr= ntohl(p_rr->dlsr);

 		recv_time_sec = recv_time_tv.tv_sec + SECOND_FROM_1900;

		recv_time_frac = (icf_uint32_t) ((double)recv_time_tv.tv_usec*(double)(1LL<<32)*1.0e-6);
		recv_time = MM_RTP_MAKE_64BTO32B(recv_time_sec, recv_time_frac);
		if (1 == table->rtcp_stats.tx_rtcp_sr_sent)
		{
			/* Round Trip Delay*/ 
			if (p_rr->lsr != 0)
			{
				table->rtcp_stats.rtp_rr.roundtrip_delay = 
					recv_time - table->rtcp_stats.rtp_rr.lsr - table->rtcp_stats.rtp_rr.dlsr;	  
				table->rtcp_stats.rtp_rr.totalrtt += table->rtcp_stats.rtp_rr.roundtrip_delay;
				//ZyPrintf("\n RTT = %d \n",table->rtcp_stats.rtp_rr.roundtrip_delay);
			}
		
		}
		table->rtcp_stats.remoteLossRate = p_rr->fraction;
		table->rtcp_stats.remoteJitter = ntohl(p_rr->jitter);
		table->rtcp_stats.totalRemoteJitter += table->rtcp_stats.remoteJitter;
		++table->rtcp_stats.totalRemoteCnt;
	}	
	else
	{
		RTCP_PRINTF("\r\n Received rtcp report is not from us");
		return;
	}
}
#else
void rtcpParseRR(
	rtpTable_t *table,
	rtcp_rr_t *p_rr,
	struct timeval recv_time_tv
){
	icf_uint32_t recv_time_sec;	   
	icf_uint32_t recv_time_frac;
	icf_uint32_t recv_time;

	p_rr->ssrc = ntohl(p_rr->ssrc);

	if ( p_rr->ssrc == table->ssrc )
	{
		table->rtcp_stats.rtp_rr.lsr = ntohl(p_rr->lsr);
		table->rtcp_stats.rtp_rr.dlsr= ntohl(p_rr->dlsr);

 		recv_time_sec = recv_time_tv.tv_sec;
		recv_time_frac = (icf_uint32_t) ((double)recv_time_tv.tv_usec*(double)(1LL<<32)*1.0e-6);
		recv_time = MM_RTP_MAKE_64BTO32B(recv_time_sec, recv_time_frac);
		if (1 == table->rtcp_stats.tx_rtcp_sr_sent)
		{
			if (1 == table->rtcp_stats.rx_rtcp_rr_recvd)
			{
				table->rtcp_stats.rtp_rr.roundtrip_delay += 
					recv_time - table->rtcp_stats.rtp_rr.lsr - table->rtcp_stats.rtp_rr.dlsr;

				table->rtcp_stats.rtp_rr.roundtrip_delay >>= 1;

				/*RTCP_PRINTF("\r\n RTT = %d \n",table->rtcp_stats.rtp_rr.roundtrip_delay);*/
			}
			else
			{
				/* Round Trip Delay*/ 
				if (p_rr->lsr != 0)
				{
					table->rtcp_stats.rtp_rr.roundtrip_delay = 
						recv_time - table->rtcp_stats.rtp_rr.lsr - table->rtcp_stats.rtp_rr.dlsr;	  

					/*RTCP_PRINTF("\r\n RTT = %d \n",table->rtcp_stats.rtp_rr.roundtrip_delay);*/
				}

				table->rtcp_stats.rx_rtcp_rr_recvd = 1;
			}		
		}
	}	
	else
	{
		RTCP_PRINTF("\r\n Received rtcp report is not from us");
		return;
	}
}
#endif
/*______________________________________________________________________________
**	rtcpRxProc
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_int32_t rtcpRxProc( 
       rtpTable_t *table,
	void *packet, 
	icf_int32_t length 
){
#if !defined(RTP_BCM_RTCP_DRIVER_TASK)
	rtcp_t *p_rtcp = MM_NULL;
	rtcp_rr_t *p_rr = MM_NULL;
	icf_int32_t msg_len = 0;
	icf_int32_t rtcp_pk_size = 0;
	struct timeval recv_time;
	icf_uint32_t count = 0;

	gettimeofday(&recv_time, MM_NULL);

	p_rtcp = (rtcp_t *)packet;
	msg_len = length;
	while (msg_len >= RTCP_COMMON_HEADER_BYTE_LEN)
	{
		if (RTP_VERSION != p_rtcp->common.version)
		{
			RTCP_PRINTF("\r\n Unknown RTP version");
			return 0;
		}
	
		p_rtcp->common.length = ntohs(p_rtcp->common.length);
		rtcp_pk_size = (p_rtcp->common.length + 1) * 4;
		if (rtcp_pk_size > msg_len)
		{
			RTCP_PRINTF("\r\n Received rtcp_pk_size less than msg_len!!");
			return 0;
		}

		switch (p_rtcp->common.pt)  
		{
			case RTCP_SR:
				/*RTCP_PRINTF("\r\n Received RTCP SR");*/

				if ( ntohl(p_rtcp->r.sr.ssrc) != table->remote_ssrc )
				{
					RTCP_PRINTF("\r\n Received rtcp sr packet from unknown ssrc");		
					return 0;
				}
				p_rtcp->r.sr.ntp_sec = ntohl(p_rtcp->r.sr.ntp_sec);
				p_rtcp->r.sr.ntp_frac = ntohl(p_rtcp->r.sr.ntp_frac);

				table->rtcp_stats.rtp_rr.last_recv_lsr = MM_RTP_MAKE_64BTO32B(p_rtcp->r.sr.ntp_sec, p_rtcp->r.sr.ntp_frac);
				table->rtcp_stats.rtp_rr.last_recv_sr_time.tv_usec = recv_time.tv_usec;
				table->rtcp_stats.rtp_rr.last_recv_sr_time.tv_sec = recv_time.tv_sec;	    	    

				for (count = 0; count < p_rtcp->common.count; count++)
				{ 
					p_rr = &(p_rtcp->r.sr.rr[count]);
					rtcpParseRR(table, p_rr, recv_time);
				}
				table->rtcp_stats.rx_rtcp_sr_recvd = 1;
				break;

			case RTCP_RR:
				/*RTCP_PRINTF("\r\n Received RTCP RR");*/

				if (0 == table->remote_ssrc)
				{
					/* rcv.ssrc is not set, so we adopt the incoming one */
					table->remote_ssrc = ntohl(p_rtcp->r.rr.ssrc);
				}
				else if ( ntohl(p_rtcp->r.rr.ssrc) != table->remote_ssrc )
				{
					RTCP_PRINTF("\r\n Receiving rtcp rr packet from unknown ssrc");
					return 0;
				}
    	  
				for (count = 0; count< p_rtcp->common.count; count++)
				{ 
					p_rr = &(p_rtcp->r.rr.rr[count]);
					rtcpParseRR(table, p_rr, recv_time);	
				}
				table->rtcp_stats.rx_rtcp_rr_recvd = 1;

				break;

			case RTCP_SDES: 
				/*RTCP_PRINTF("\r\n Received RTCP SDES");*/

				break;

			case RTCP_BYE:
				RTCP_PRINTF("\r\n Received RTCP BYE");
				for (count = 0; count < p_rtcp->common.count; count++) 
				{
					if (ntohl(p_rtcp->r.bye.src[count]) == table->ssrc) {
						table->rtcp_stats.rx_rtcp_bye_recvd = 1;
						break;
					}
				}
				break;
			case RTCP_APP:
				RTCP_PRINTF("\r\n Received RTCP APP");
				break;
				
			case RTCP_XR:
				RTCP_PRINTF("\r\n Received RTCP XR");
				break;

			default:
				RTCP_PRINTF("\r\n Received unknown rtcp packet type");
				return 0;

		}

		msg_len -= rtcp_pk_size;
		p_rtcp = (rtcp_t *) (rtcp_pk_size + (char *) p_rtcp);
	}

	table->rtcp_stats.rx_rtcp_pkt_cnt++;


#else

	/* Send the packet to Boardcom DSP driver task*/
	if (table->voipRtcpCallBack != MM_NULL)
	{
		table->voipRtcpCallBack(table->call_id, table->stream_id, table->stream_type, 0, packet, length, 0, NULL);
	}

#endif

	return 0;
}

/*______________________________________________________________________________
**	rtcpSdesItem
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpSdesItem(
	icf_uint8_t type, 
	char *data, 
	char *string)
{
	rtcp_sdes_item_t *item = (rtcp_sdes_item_t *)data;

	item->type = type;
	item->length = zyStrlen(string);
	zyStrcpy(item->data, string);
	return item->length + 2;
} 
/*______________________________________________________________________________
**	rtcpModifySdes
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpModifySdes(
	rtpTable_t *table,
	icf_uint8_t	*p_msg
){
	rtcp_t *p_rtcp = (rtcp_t *)p_msg;
	icf_uint8_t *end = p_rtcp + (p_rtcp->common.length + 1) * 4;
	icf_uint8_t *sdes = MM_NULL;
	icf_uint32_t ssrc=0;
	int nchunk=0;
	icf_uint8_t chunk_start = 1;
	char	name[64];

	sdes = &(p_rtcp->r.sdes);
	while(sdes < end)
	{
		if (chunk_start)
		{
			if (sdes + 4 <= end)
			{
				ssrc = ntohl(p_rtcp->r.sdes.src);
				sdes += 4;
			}
			else
			{
				RTCP_PRINTF("incorrect chunk start in RTCP SDES");
				break;
			}
			chunk_start = 0;
		}
		else
		{
			if (sdes + 2 <= end)
			{
				uint8 itemtype = sdes[0];
				uint8 itemlen = sdes[1];

				if (RTCP_SDES_END == itemtype)
				{
					/* pad to next 32bit boundary*/
					sdes = (uint8*)(((unsigned long)sdes + 4) & ~0x3);
					nchunk++;
					if (nchunk < p_rtcp->common.count)
					{
						chunk_start = 0;
						continue;
					}
					else
					{
						break;
					}
				}

				if ((sdes+2) + itemlen <= end)
				{
					if(RTCP_SDES_CNAME == itemtype)
					{
						zyMemset(sdes, 0, itemlen);
						zyMemset(name, 0, sizeof(name));
						zyStrncpy(name, table->rtcp_stats.cname, itemlen);	/* follow oringinal string length to modify  */
						rtcpSdesItem(RTCP_SDES_CNAME, sdes, name);
					}
					sdes += 2;
					sdes += itemlen;
				}
				else
				{
					RTCP_PRINTF("bad item length in RTCP SDES");
					break;
				}
			}
			else
			{
				break;	/*end of packet */
			}
		}
	}

}

/*______________________________________________________________________________
**	rtcpSend
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpPacketParse(
	rtpTable_t *table,
	icf_uint8_t *packet, 
	icf_uint16_t len
){
	rtcp_t *p_rtcp = MM_NULL;
	icf_int32_t msg_len = 0;
	icf_int32_t rtcp_pk_size = 0;


	p_rtcp = (rtcp_t *)packet;
	msg_len = len;
	while (msg_len >= RTCP_COMMON_HEADER_BYTE_LEN)
	{
	
		p_rtcp->common.length = ntohs(p_rtcp->common.length);
		rtcp_pk_size = (p_rtcp->common.length + 1) * 4;

		switch (p_rtcp->common.pt)  
		{
			case RTCP_SR:
				/*RTCP_PRINTF("\r\n Send RTCP SR");*/

				table->rtcp_stats.tx_rtp_pkt_cnt = ntohl(p_rtcp->r.sr.psent);
				table->rtcp_stats.tx_rtp_octet_cnt = ntohl(p_rtcp->r.sr.osent);

				table->rtcp_stats.rtp_rr.max_seq = ntohl(p_rtcp->r.sr.rr[0].last_seq);
				table->rtcp_stats.rtp_rr.jitter = ntohl(p_rtcp->r.sr.rr[0].jitter);
				table->rtcp_stats.rtp_rr.lsr = ntohl(p_rtcp->r.sr.rr[0].lsr);
				table->rtcp_stats.rtp_rr.dlsr = ntohl(p_rtcp->r.sr.rr[0].dlsr);
				
				table->rtcp_stats.tx_rtcp_sr_sent = 1;
				break;

			case RTCP_RR:
				/*RTCP_PRINTF("\r\n Send RTCP RR");*/

				table->rtcp_stats.rtp_rr.max_seq = ntohl(p_rtcp->r.rr.rr[0].last_seq);
				table->rtcp_stats.rtp_rr.jitter = ntohl(p_rtcp->r.rr.rr[0].jitter);
				table->rtcp_stats.rtp_rr.lsr = ntohl(p_rtcp->r.rr.rr[0].lsr);
				table->rtcp_stats.rtp_rr.dlsr = ntohl(p_rtcp->r.rr.rr[0].dlsr);

				table->rtcp_stats.tx_rtcp_rr_sent = 1;
				break;

			case RTCP_SDES: 
				/*RTCP_PRINTF("\r\n Send RTCP SDES");*/
				rtcpModifySdes(table, (icf_uint8_t*)p_rtcp);
				break;

			case RTCP_BYE:
				RTCP_PRINTF("\r\n Send RTCP BYE");

				break;
			case RTCP_APP:
				RTCP_PRINTF("\r\n Send RTCP APP");
				break;
				
			case RTCP_XR:
				RTCP_PRINTF("\r\n Send RTCP XR");
				break;
		}

		msg_len -= rtcp_pk_size;
		p_rtcp = (rtcp_t *) (rtcp_pk_size + (char *) p_rtcp);
	}

	return;
}

/*______________________________________________________________________________
**	rtcpSend
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

void rtcpSendPacket(
	icf_call_id_t call_id, 
	icf_stream_id_t	stream_id, 
	icf_media_type_t	stream_type,
	uint16 logicalId,
	icf_uint32_t pload_type, 
	icf_uint8_t *packet, 
	icf_uint16_t len, 
	icf_uint32_t timestamp, 
	void* handle
){
	rtpTable_t *table = MM_NULL;


#ifdef DUMP_RTCP_PKT 
	dispRTCPPacket((unsigned char *)packet, len);
#endif 	

	if( (table = rtpTableGetById(call_id, stream_id, stream_type)) == MM_NULL ) {
		return;
	}

	if((0 != table->rtcp_socket.remote_addr) && (1 == table->rtcp_enabled))
	{
		rtcpSend(table, 0, packet, len);
	}

	return;
}


/*______________________________________________________________________________
**	rtcpSend
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

void rtcpSend(
	rtpTable_t *table,
	icf_uint8_t event,
	icf_uint8_t *packet, 
	icf_uint16_t len
){
	#if SRTP_SUPPORT
	int err = 0;
	#endif
	if(ICF_CODEC_TYPE_IMAGE == table->stream_type) /* stop RTCP during T38 FAX */
	{
		RTCP_PRINTF("\r\n Stop RTCP Sending");
		return;
	}

#ifdef RTP_BCM_RTCP_DRIVER_TASK

	/* Boardcom privated RTCP */
	rtcpPacketParse(table, packet, len);
	#if SRTP_SUPPORT
	if(NULL != table->srtpSendSession){
		int length = len;		
		err = srtp_protect_rtcp(table->srtpSendSession, packet, &length);
		if(err != 0){
			ZyPrintf("srtp_protect ret = %d\n", err);
		}
		len = length;		
	}
	#endif
	udpSend(table->rtcp_socket.s, (char *)packet, len, table->rtcp_socket.remote_port, table->rtcp_socket.remote_addr);

#else
	icf_uint8_t	msg_buf[512];
	icf_uint8_t	*tmp_buf;
	icf_uint16_t	msg_len = 0;
	icf_int32_t nsent = 0;

	tmp_buf = msg_buf;
	if ( table->rtcp_stats.tx_last_rtp_pktcnt < table->rtcp_stats.tx_rtp_pkt_cnt)
	{
		/*RTCP_PRINTF("\r\n Send RTCP SR");*/
		msg_len += rtcpWriteSR(table, (icf_uint8_t*)tmp_buf);
		tmp_buf = msg_buf + msg_len;
		msg_len += rtcpWriteSdes(table, (icf_uint8_t*)tmp_buf);
	}
	else
	{
		/*RTCP_PRINTF("\r\n Send RTCP RR");*/
		msg_len = rtcpWriteRR(table, (icf_uint8_t*)tmp_buf);
		tmp_buf = msg_buf + msg_len;
		msg_len += rtcpWriteSdes(table, (icf_uint8_t*)tmp_buf);
	}

	if((RTCP_BYE == event) && (table->rtcp_stats.rx_rtcp_pkt_cnt > 0))
	{
		RTCP_PRINTF("\r\n Send RTCP BYE");
		tmp_buf = msg_buf + msg_len;
		msg_len += rtcpWriteBye(table, (icf_uint8_t*)tmp_buf);
		table->rtcp_stats.tx_rtcp_bye_sent = 1;
	}	

	nsent = udpSend(table->rtcp_socket.s, 
					msg_buf, 
					msg_len, 
					table->rtcp_socket.remote_port, 
					table->rtcp_socket.remote_addr);

	if((nsent < 0) || (nsent != len))	
	{
		++table->rtcp_stats.tx_rtcp_failcnt;
	}
	else	
	{
		++table->rtcp_stats.tx_rtcp_cnt;
	}

#endif

	return;
}

/*______________________________________________________________________________
**	rtcpJitter
**
**	descriptions:	Determining Number of Packets Expected and Lost
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
#if 1 /*Jason , rtp statistic*/
void rtcpJitter(
	rtcpStats_t *rtcp_stats, 
	icf_uint32_t timestamp
){
//	icf_uint32_t    arrival, transit;
	int       d;

	struct timeval now;
	struct timeval last;
	int diff_s, diff_us;
	int diff_t, diff_ts;
	
	gettimeofday(&now, NULL);
	//arrival = now.tv_sec + SECOND_FROM_1900;
	//transit = arrival - timestamp;
	last = rtcp_stats->rtp_rr.last_arrival_time;
	diff_s = now.tv_sec - last.tv_sec;
	if (now.tv_usec >= last.tv_usec) {
		diff_us = now.tv_usec - last.tv_usec;		
	}
	else {
		diff_us = 1000000 + now.tv_usec - last.tv_usec;
		--diff_s;
	}
	diff_t = (diff_s * 8000) + (int)(diff_us / 125);
	diff_ts = timestamp - rtcp_stats->rtp_rr.last_ts;
	if (rtcp_stats->rx_rtp_pkt_cnt > 0)
	{
//		d = transit - rtcp_stats->rtp_rr.transit;
		d = diff_t - diff_ts;

		if (d < 0) d = -d;
		rtcp_stats->rtp_rr.jitter += d - ((rtcp_stats->rtp_rr.jitter + 8) >> 4);
	}

	rtcp_stats->rtp_rr.last_arrival_time = now;
	rtcp_stats->rtp_rr.last_ts = timestamp;
	rtcp_stats->rtp_rr.totalJitter += rtcp_stats->rtp_rr.jitter;
//	rtcp_stats->rtp_rr.transit = transit;
}
#else
void rtcpJitter(
	rtcpStats_t *rtcp_stats, 
	icf_uint32_t timestamp
){
	icf_uint32_t    arrival, transit;
	int       d;

	struct timeval now;
	
	gettimeofday(&now, NULL);
	arrival = now.tv_sec + SECOND_FROM_1900;

	transit = arrival - timestamp;
	if (rtcp_stats->rx_rtp_pkt_cnt > 0)
	{
		d = transit - rtcp_stats->rtp_rr.transit;

		if (d < 0) d = -d;
		rtcp_stats->rtp_rr.jitter += d - ((rtcp_stats->rtp_rr.jitter + 8) >> 4);
	}

	rtcp_stats->rtp_rr.transit = transit;
}
#endif
/*______________________________________________________________________________
**	rtcpGetEnable
**
**	descriptions:	
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpGetEnable(
	rtpTable_t *table,
	uint16 line_id
){
	table->rtcp_enabled = gMmCfgDb_t.port_range[line_id].rtcp_enable;
}

/*______________________________________________________________________________
**	rtcpGetTxInterval
**
**	descriptions:	
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpGetTxInterval(
	rtpTable_t *table,
	uint16 line_id
){
	table->rtcp_tx_timer.interval = gMmCfgDb_t.port_range[line_id].rtcp_tx_interval;
}

/*______________________________________________________________________________
**	rtcpGetLocalCname
**
**	descriptions:	
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpGetLocalCname(
	rtcpStats_t	*rtcp_stats,
	uint16 line_id
){
#ifdef RTP_BCM_RTCP_DRIVER_TASK
	zyStrncpy(rtcp_stats->cname, gMmCfgDb_t.port_range[line_id].local_cname, 9);	/* boarddriver randomly generate cname len=9*/
	rtcp_stats->cname[9+1] = '\0';
	rtcp_stats->cname_len = zyStrlen(rtcp_stats->cname);
#else
	zyStrcpy(rtcp_stats->cname, gMmCfgDb_t.port_range[line_id].local_cname);
	rtcp_stats->cname_len = zyStrlen(rtcp_stats->cname);
#endif
}

/*______________________________________________________________________________
**	rtcpStoreRtpStats
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpStoreRtpStats(
	rtpTable_t *table, 
	icf_uint8_t *p_rtp_packet, 
	icf_uint16_t length
){
	rtp_header_t *p_rtp_header = MM_NULL;
	rtp_header_ext_t *p_rtp_header_exten = MM_NULL;
	icf_uint16_t exten_len = 0;

	p_rtp_header= (rtp_header_t *)p_rtp_packet;

	/* padding */
	if (0 != p_rtp_header->p)
	{
		table->rtcp_stats.rxPeddingOctetCnt = p_rtp_packet[length-1];
	}
	else
	{
		table->rtcp_stats.rxPeddingOctetCnt = 0;
	}

	/* CSRC */
	if (p_rtp_header->cc != 0)
	{
		table->rtcp_stats.rxCSRCOctetCnt = (icf_uint16_t)(p_rtp_header->cc * 4);
	}
	else
	{
		table->rtcp_stats.rxCSRCOctetCnt = 0;
	}

	/* header extension */
	if (p_rtp_header->x != 0)
	{
		p_rtp_header_exten = p_rtp_packet + sizeof(rtp_header_t) + table->rtcp_stats.rxCSRCOctetCnt;
		exten_len = p_rtp_header_exten->len;

		/*section 5.3.1 RFC1889 */
		table->rtcp_stats.rxExtenOctetCnt = (ntohs(exten_len) + 1) << 1;
	}
	else
	{
		table->rtcp_stats.rxExtenOctetCnt = 0;
	}

	table->rtcp_stats.rx_rtp_octet_cnt += (icf_uint32_t)(length 
										-RTP_FIXED_HEADER_BYTE_LEN 
										-table->rtcp_stats.rxPeddingOctetCnt
										-table->rtcp_stats.rxCSRCOctetCnt 
										-table->rtcp_stats.rxExtenOctetCnt);
	table->rtcp_stats.rx_rtp_pkt_cnt++;
}
/*______________________________________________________________________________
**	rtcpStoreRtpInfo
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpStoreRtpInfo(
	rtpTable_t *table,
	void *packet, 
	icf_int32_t length 
){
	rtp_header_t*	rtp_packet = (rtp_header_t*)packet;
	icf_uint32_t  ssrc = 0;
	icf_uint16_t  seq = 0;

	ssrc = ntohl(rtp_packet->ssrc);
	seq = ntohs(rtp_packet->seq);

	if(table->rtcp_stats.rx_rtp_pkt_cnt > 0) 
	{
		if(ssrc == table->remote_ssrc)
		{
			rtcpUpdateSeq(&table->rtcp_stats.rtp_rr, seq);
			rtcpStoreRtpStats(table, (icf_uint8_t*)rtp_packet, length);
			rtcpJitter(&table->rtcp_stats, ntohl(rtp_packet->ts));
		}
		else
		{
			RTCP_PRINTF("\r\n Invalid ssrc, remote_ssrc=%d, incoming ssrc=%d\n", table->remote_ssrc, ssrc);
		}
	}
	else
	{
		rtcpInitSeq(&table->rtcp_stats.rtp_rr, seq);
#ifdef VOIP_SYSLOG /*Jason , syslog*/
		++(table->rtcp_stats.rtp_rr.received);
#endif
		rtcpStoreRtpStats(table, rtp_packet, length); 
		table->remote_ssrc = ssrc;
	}

}



/*______________________________________________________________________________
**	rtcpInitSeq
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpInitSeq(
	rtp_rr_t *src,
	icf_uint16_t seq
) {
	src->base_seq = seq;
#ifdef VOIP_SYSLOG /*Jason , syslog*/	
	src->max_seq = seq;
#else
	src->max_seq = seq - 1;
#endif
	src->bad_seq = RTP_SEQ_MOD + 1;	/* so seq == bad_seq is false */
	src->cycles = 0;
	src->received = 0;
	src->received_prior = 0;
	src->expected_prior = 0;

	/* other initialization */
	src->transit = 0;
	src->jitter = 0;
	src->lsr = 0;
	src->dlsr = 0;
	src->last_recv_lsr = 0;
	src->roundtrip_delay = 0;
	src->last_recv_sr_time.tv_sec = 0; 
	src->last_recv_sr_time.tv_usec = 0; 
#if 1 /*Jason , rtp statistic*/
	src->totalJitter = 0;
	src->totalrtt = 0;
#endif	
	return;
}

/*______________________________________________________________________________
**	rtcpUpdateSeq
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int rtcpUpdateSeq(
	rtp_rr_t *src,
	icf_uint16_t seq
) {
	icf_uint16_t	udelta;

	udelta = seq - src->max_seq;

	/*
	* Source is not valid until MIN_SEQUENTIAL packets with
	* sequential sequence numbers have been received.
	*/
	if (src->probation) {
		/* packet is in sequence */
		if (seq == src->max_seq + 1) {
			src->probation--;
			src->max_seq = seq;
			if (src->probation == 0) {
				rtcpInitSeq(src, seq);
				src->received++;
				return 0;	/* permission */
			}
		} else {
			src->probation = RTP_MIN_SEQUENTIAL - 1;
			src->max_seq = seq;
		}
		return -1;	/* invalid */
	} else if (udelta < RTP_MAX_DROPOUT) {
		/* in order, with permissible gap */
		if (seq < src->max_seq) {
			/*
			* Sequence number wrapped - count another 64K cycle.
			*/
			src->cycles += RTP_SEQ_MOD;
		}
		src->max_seq = seq;
	} else if (udelta <= RTP_SEQ_MOD - RTP_MAX_MISORDER) {
		/* the sequence number made a very large jump */
		if (seq == src->bad_seq) {
			/*
			* Two sequential packets -- assume that the other side
			* restarted without telling us so just re-sync
			* (i.e., pretend this was the first packet).
			*/
			 rtcpInitSeq(src, seq);
		} else {
			src->bad_seq = (seq + 1) & (RTP_SEQ_MOD-1);
			return -1;	/* bad */
		}
	} else {
		/* duplicate or reordered packet */
	}
	src->received++;
	return 0;
}

#if 1 /*Jason , syslog */
/*______________________________________________________________________________
**	rtcpPktFractionlost
**
**	descriptions:	Determining Number of Packets Expected and Lost
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

void rtcpPktFractionlost(
	rtp_rr_t *src, 
	icf_uint32_t* input_fraction, 
	icf_uint32_t* input_lost)
{
	icf_uint32_t	extended_max, expected;
	icf_uint32_t	lost;
	icf_uint32_t	expected_interval, received_interval, lost_interval;
	icf_uint8_t	fraction;

	if (src->probation)
	{
		lost = 0;
	}
	else
	{
		extended_max = src->cycles + src->max_seq;
		expected = extended_max - src->base_seq + 1;

		/*    
		The number of packets lost is defined to be the number of packets
		expected less the number of packets actually received: 
		*/
		lost = expected - src->received;
	}

	extended_max = src->cycles + src->max_seq;
	expected = extended_max - src->base_seq + 1;
	lost = expected - src->received;

	expected_interval = expected - src->expected_prior;
	received_interval = src->received - src->received_prior;

	src->expected_prior = expected;
	src->received_prior = src->received;

	lost_interval = expected_interval - received_interval;

	if (expected_interval == 0 || lost_interval <= 0){
		fraction = 0;
	}
	else{
		fraction = ((lost_interval << 8) / expected_interval);
	}

	*input_fraction = fraction;
	*input_lost = lost;
	return;
}
#endif

#if !defined(RTP_BCM_RTCP_DRIVER_TASK)

/*______________________________________________________________________________
**	rtcp_common_header
**
**	descriptions:	Determining Number of Packets Expected and Lost
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/

void rtcp_common_header(
 	rtcp_common_t *p_common,
 	int type,
	int count, 
	int bytes_len
){
  	p_common->version = RTP_VERSION;
  	p_common->p       = 0;
  	p_common->count   = count;
	p_common->pt      = type;
	p_common->length  = htons((bytes_len/4)-1);

	return;
}

/*______________________________________________________________________________
**	rtcpWriteSR
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpWriteSR( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
){
	rtcp_t *r = (rtcp_t *)p_msg;
	rtcp_rr_t	*rr;
	struct timeval now;
	icf_uint16_t	msg_len= 0;
	icf_uint32_t     lost= 0, fraction = 0;
	icf_uint32_t	nowtime = 0;
#ifdef DUMP_RTCP_PKT
	icf_uint32_t  begin = msg_len;
#endif

	gettimeofday(&now, NULL);
	nowtime = (icf_uint32_t)((double)now.tv_usec*(double)(1LL << 32)*1.0e-6);	

	/* Sender report block */
  	r->r.sr.ssrc = htonl(table->ssrc);
	r->r.sr.ntp_sec = htonl(now.tv_sec + SECOND_FROM_1900);
	r->r.sr.ntp_frac = htonl(nowtime);
  	r->r.sr.rtp_ts = htonl(table->tx_timestamp);
  	r->r.sr.psent = htonl(table->rtcp_stats.tx_rtp_pkt_cnt);
  	r->r.sr.osent = htonl(table->rtcp_stats.tx_rtp_octet_cnt);

	msg_len += RTCP_SR_HDR_BYTE_LEN;

	/*
	* Reception report block 	
	*/
	rr = (rtcp_rr_t *)(p_msg + msg_len);

	rtcpPktFractionlost(&table->rtcp_stats.rtp_rr, &fraction, &lost);

	rr->ssrc = htonl(table->ssrc);
	rr->fraction = fraction;
	rr->lost = lost;
	rr->last_seq = htonl(table->rtcp_stats.rtp_rr.cycles + table->rtcp_stats.rtp_rr.max_seq);
	rr->jitter = htonl(table->rtcp_stats.rtp_rr.jitter >> 4);
	rr->lsr = htonl(table->rtcp_stats.rtp_rr.last_recv_lsr);
	rr->dlsr = htonl(nowtime - table->rtcp_stats.rtp_rr.last_recv_lsr);

	msg_len += REPORT_BLOCK_BYTE_SIZE;

  	rtcp_common_header(&r->common, RTCP_SR, 1, msg_len);	/* count=1: we don't support multi source receiving yet */

#ifdef DUMP_RTCP_PKT
	RTCP_PRINTF("SR pkt\n");
	rtcp_debug_display(begin, msg_len, p_msg);
	RTCP_PRINTF("\n");
#endif
  	return msg_len;
}

/*______________________________________________________________________________
**	rtcpWriteRR
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpWriteRR( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
){
	rtcp_t *r = (rtcp_t *)p_msg;
	rtcp_rr_t	*rr;
	struct timeval now;
	icf_uint16_t	msg_len= 0;
	icf_uint32_t     lost= 0, fraction = 0;
	icf_uint32_t	nowtime = 0;
#ifdef DUMP_RTCP_PKT
	icf_uint32_t  begin = msg_len;
#endif

	gettimeofday(&now, NULL);
	nowtime = (icf_uint32_t)((double)now.tv_usec*(double)(1LL << 32)*1.0e-6);

   	r->r.rr.ssrc = htonl(table->ssrc);

	msg_len += RTCP_RR_HDR_BYTE_LEN;

	/*
	* Reception report block 	
	*/
	rr = (rtcp_rr_t *)(p_msg + msg_len);

	rtcpPktFractionlost(&table->rtcp_stats.rtp_rr, &fraction, &lost);

	rr->ssrc = htonl(table->ssrc);
	rr->fraction = fraction;
	rr->lost = lost;
	rr->last_seq = htonl(table->rtcp_stats.rtp_rr.cycles + table->rtcp_stats.rtp_rr.max_seq);
	rr->jitter = htonl(table->rtcp_stats.rtp_rr.jitter >> 4);
	rr->lsr = htonl(table->rtcp_stats.rtp_rr.last_recv_lsr);
	rr->dlsr = htonl(nowtime - table->rtcp_stats.rtp_rr.last_recv_lsr);

	msg_len += REPORT_BLOCK_BYTE_SIZE;

 	rtcp_common_header(&r->common, RTCP_RR, 1, msg_len);	/* count=1: we don't support multi source receiving yet */

#ifdef DUMP_RTCP_PKT
	RTCP_PRINTF("RR pkt\n");
	rtcp_debug_display(begin, msg_len, p_msg);
	RTCP_PRINTF("\n");
#endif
  	return msg_len;
}
/*______________________________________________________________________________
**	rtcpWriteApp
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpWriteApp( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
){
	icf_uint16_t	msg_len= 0;

	table = table;
	p_msg = p_msg;
	return msg_len;
}
/*______________________________________________________________________________
**	rtcpWriteXR
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpWriteXR(
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
){
	icf_uint16_t	msg_len= 0;

	table = table;
	p_msg = p_msg;
	return msg_len;
}

/*______________________________________________________________________________
**	rtcpWriteSdes
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpWriteSdes( 
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
){
	rtcp_t *r = (rtcp_t *)p_msg;
	struct rtcp_sdes *sdes;
	icf_uint32_t len = 0;
	icf_uint32_t total = 8;
	icf_uint32_t padding = 0;
	icf_uint16_t msg_len= 0;	
#ifdef DUMP_RTCP_PKT
	icf_uint32_t  begin = msg_len;
#endif

	msg_len += RTCP_SDES_HDR_BYTE_LEN;

	sdes = (struct rtcp_sdes *)(p_msg + msg_len);
	sdes->src = htonl(table->ssrc);

	msg_len += 4; /* shift 4 octets */
        /* may need to add other item in the future */
	len = rtcpSdesItem(RTCP_SDES_CNAME, (char *)(p_msg + msg_len), table->rtcp_stats.cname);
	msg_len += len;
 	total += len;

	*(p_msg + msg_len) = RTCP_SDES_END;
	msg_len += 1;
	total += 1;

	len = (total + 3) & 0xfffc;
	padding = len - total;
  	zyMemset(p_msg + msg_len, 0, padding);
  	msg_len += padding;
  	*(p_msg + msg_len -1) = 0x00;

  	rtcp_common_header(&r->common, RTCP_SDES, 1, msg_len);

#ifdef DUMP_RTCP_PKT
	RTCP_PRINTF("SDES pkt\n");
	rtcp_debug_display(begin, msg_len, p_msg);
	RTCP_PRINTF("\n");
#endif
  	return msg_len;
}
/*______________________________________________________________________________
**	rtcpWriteBye
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
icf_uint16_t rtcpWriteBye(
	rtpTable_t	*table,
	icf_uint8_t	*p_msg
){
	rtcp_t *r = (rtcp_t *)p_msg;
	icf_uint32_t  *ssrc;
	icf_uint16_t msg_len= 0;
#ifdef DUMP_RTCP_PKT
	icf_uint32_t  begin = msg_len;
#endif

	msg_len += RTCP_BYE_HDR_BYTE_LEN;
 
	ssrc  = (icf_uint32_t*)(p_msg + msg_len);
	*ssrc = htonl(table->ssrc);
	msg_len += 4;   /* shift 4 octets */

 	rtcp_common_header(&r->common, RTCP_BYE, 1, msg_len);

#ifdef DUMP_RTCP_PKT
	RTCP_PRINTF("BYE pkt\n");
	rtcp_debug_display(begin, msg_len, p_msg);
	RTCP_PRINTF("\n");
#endif
	return msg_len;
}

static double
rtcpDrand48()
{
    return (double)random() / (double)0x7fffffffL; /* 2**31-1 */
}

/*______________________________________________________________________________
**	rtcpInterval
**
**	descriptions: 
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
/*Computing the RTCP Transmission Interval*/
double  rtcpInterval(
	int rtcp_min_time, 
	int initial
){
	int const RTCP_MIN_TIME = 5000; /*mini-Seconds, referenced in RFC3550*/
	double const COMPENSATION = 2.71828 - 1.5; /*referenced in RFC3550*/
	int  t, r;

	if(0 == rtcp_min_time){
		rtcp_min_time = RTCP_MIN_TIME;
	}

	if(initial){
		rtcp_min_time /= 2;
	}

	/*assume all sender*/
	t = rtcp_min_time;	
       t = t * (rtcpDrand48() + 0.5);
       t = t / COMPENSATION;

	RTCP_PRINTF("\r\n rtcpInterval=%d",t); /*return msec*/
	return t;
	
}
/*______________________________________________________________________________
**	rtcpTxTimer
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
int
rtcpOnExpire(
	tm_id_t	timer_id,
	void	*bp
){
	rtpTable_t *table = (rtpTable_t *)bp;

	if(MM_NULL != table)
	{
		table->rtcp_tx_timer.tm_id = 0;
		if((1 == table->rtcp_enabled) && (0 != table->rtcp_socket.remote_addr))
		{
			rtcpSend(table, 0, MM_NULL, 0);

			table->rtcp_tx_timer.rty_count++;
			table->rtcp_tx_timer.tm_intv = rtcpInterval(table->rtcp_tx_timer.interval, 1);
			table->rtcp_tx_timer.tm_id = timerStartS(table->rtcp_tx_timer.tm_intv, rtcpOnExpire, table);
		}
	}

	return 0;
}
/*______________________________________________________________________________
**	rtcpEnableTxTimer
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpEnableTxTimer(
	rtpTable_t *table
){
	if((MM_NULL != table) && (1 == table->rtcp_enabled))
	{
		if (0 != table->rtcp_tx_timer.tm_id)
		{
			timerStop(table->rtcp_tx_timer.tm_id);
			table->rtcp_tx_timer.tm_id = 0;
		}
		table->rtcp_tx_timer.tm_intv = rtcpInterval(table->rtcp_tx_timer.interval, 1);
		table->rtcp_tx_timer.tm_id = timerStartS(table->rtcp_tx_timer.tm_intv, rtcpOnExpire, table);
	}
}
/*______________________________________________________________________________
**	rtcpDisableTxTimer
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void rtcpDisableTxTimer(
	rtpTable_t *table
){
	if(MM_NULL != table)
	{
		if(0 != table->rtcp_tx_timer.tm_id)
		{
			timerStop(table->rtcp_tx_timer.tm_id);
			table->rtcp_tx_timer.tm_id = 0;
			table->rtcp_tx_timer.rty_count = 0;
			table->rtcp_tx_timer.tm_intv = 0;

			if(0 != table->rtcp_socket.remote_addr)
			{
				rtcpSend(table, RTCP_BYE, MM_NULL, 0);	/* Send BYE event */
			}
		}
		table->rtcp_enabled = 0;
	}
}

#endif

