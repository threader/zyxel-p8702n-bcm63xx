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

/* unix socket */
#include <sys/socket.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
/* unix pipe*/
#include <unistd.h>
#include "sys_itf.h"
#include "icf_types.h"
#include "mm_glb.h"
#include "mm_core_fsm_types.h"
#include "mm_macros.h"
#include "mm_port_wrapper.h"
#include "mm_core_fsm_defs.h"
#include "mm_core_rtp.h"
#include "mm_port_prototypes.h"
#include "switch.h"
#include "voice_dsp_common_api.h"

#ifdef NORWAY_VOIP_CUSTOMIZATION
#define BeUsed_Port "/tmp/BeUsed_Port"
#define NF_UDP_port "/proc/net/nf_udp_port"
#define NF_UDP_port_expect "/proc/net/nf_udp_port_expect"
#define NF_LEN 25
#endif

static icf_int32_t rtpFDpipe[2];
static icf_int32_t rtcpFDpipe[2];
rtpTablePool_t  *rtp_table;
extern int mmKSocketEnable;
#if 1  /*Jason , rtp static*/
SipLineStats_t sipLineStats[MAX_SIP_LINE_COUNT];
#endif

#ifdef NORWAY_VOIP_CUSTOMIZATION
char rtp_IPAddr[33];
#endif

extern int
RtpSendToDsp(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type,
	icf_uint32_t null_pload_type, 
	icf_uint8_t *InMessage, 
	icf_uint16_t MessageLen, 
	icf_uint32_t TimeStamp, 
	void* handle
);
extern int
RtcpSendToDsp(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type,
	icf_uint32_t null_pload_type, 
	icf_uint8_t *InMessage, 
	icf_uint16_t MessageLen, 
	icf_uint32_t TimeStamp, 
	void* handle
);
/*______________________________________________________________________________
**	rtpTask
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
void rtpTask(void)
{
	icf_uint32_t fd_list[MAX_DSP_CHAN_PER_DSP];
	icf_uint32_t numbytes = 0;
	icf_uint32_t num_message = 0;
	icf_uint32_t fdsize = 0;

	fd_set	readfds; 
	fd_set	rset;
	icf_int32_t maxfd = 0;
	rtpTable_t *table = MM_NULL;
	icf_uint8_t      RTPStackPacket[1024];
	icf_int32_t result, j, k;

	RTP_PRINTF(" RTP task started\n");

	fdsize = sizeof(fd_set);
	numbytes =  sizeof(icf_uint32_t);
	FD_ZERO(&readfds);
	FD_ZERO(&rset);

	if( pipe(rtpFDpipe) ) {
		RTP_PRINTF("Pipe failed\n");
		return;
	}
	FD_SET(rtpFDpipe[0], &readfds);
	maxfd = rtpFDpipe[0];

	while (1)
	{
		#if KSOCKET_RTP_SUPPORT
		if(!mmKSocketEnable){
		#endif
			zyMemcpy(&rset, &readfds, fdsize);
			if( result = select(maxfd+1, &rset, MM_NULL, MM_NULL, MM_NULL) > 0) 
			{
				if (FD_ISSET(rtpFDpipe[0], &rset)) 
				{
					result = read(rtpFDpipe[0], (char*) fd_list, MAX_DSP_CHAN_PER_DSP * numbytes);
					num_message = result / numbytes;
					for (k = 0; k < num_message; k++) 
					{
						FD_SET((icf_int32_t)fd_list[k], &readfds);
						if((icf_int32_t)fd_list[k] > maxfd)
						{
							maxfd = (icf_int32_t)fd_list[k];
						}
					}
				}			
				table = rtp_table->front;
				while(table != MM_NULL) 
				{
					if(table->rtp_socket.s != -1) 
					{
						if (FD_ISSET(table->rtp_socket.s, &rset)) 
						{
							struct sockaddr fromAddr = {0};
							icf_int32_t fromLen = sizeof(fromAddr);
							icf_int32_t rc = -1;
							rc = recvfrom(table->rtp_socket.s, (char *)RTPStackPacket, sizeof(RTPStackPacket), 0, &fromAddr, &fromLen);
							if(rc > 0 )
							{
								rtpRxProc(table, RTPStackPacket, rc);
							}
							else if (rc == 0)
							{
								/*
								** Close RTP recv socket
								*/
								RTP_PRINTF("rtpTask: Closing rtp recv socket\n");
								FD_CLR(table->rtp_socket.s, &readfds);
								if (maxfd == table->rtp_socket.s) 
								{
									for(j = table->rtp_socket.s; j > 0; j--) 
									{
										if (FD_ISSET(j, &readfds)) 
										{
											maxfd = j;
											break;
										}
									}
								}
								close(table->rtp_socket.s);
								table->rtp_socket.s = -1;
							}  /* else if */
						}
					}
					
					table = table->next;
				}			

			}
		#if KSOCKET_RTP_SUPPORT
		}		
		else{
			sleep(1);
		}
		#endif
	}

	RTP_PRINTF("rtpTask ENDS");
}
/*______________________________________________________________________________
**	rtcpTask
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
void rtcpTask(void)
{
	icf_uint32_t fd_list[MAX_DSP_CHAN_PER_DSP];
	icf_uint32_t numbytes = 0;
	icf_uint32_t num_message = 0;
	icf_uint32_t fdsize = 0;

	fd_set	readfds; 
	fd_set	rset;
	icf_int32_t maxfd = 0;
	rtpTable_t *table = MM_NULL;
	icf_uint8_t      RTCPStackPacket[1024];
	icf_int32_t result, j, k;

	RTP_PRINTF(" RTCP task started\n");

	fdsize = sizeof(fd_set);
	numbytes =  sizeof(icf_uint32_t);
	FD_ZERO(&readfds);
	FD_ZERO(&rset);

	if( pipe(rtcpFDpipe) ) {
		RTP_PRINTF("rtcpFDpipe failed\n");
		return;
	}
	FD_SET(rtcpFDpipe[0], &readfds);
	maxfd = rtcpFDpipe[0];

	while (1)
	{
		#if KSOCKET_RTP_SUPPORT
		if(!mmKSocketEnable){	
		#endif
			zyMemcpy(&rset, &readfds, fdsize);
			if( result = select(maxfd+1, &rset, MM_NULL, MM_NULL, MM_NULL) > 0) 
			{
				if (FD_ISSET(rtcpFDpipe[0], &rset)) 
				{
					result = read(rtcpFDpipe[0], (char*) fd_list, MAX_DSP_CHAN_PER_DSP * numbytes);
					num_message = result / numbytes;
					for (k = 0; k < num_message; k++) 
					{
						FD_SET((icf_int32_t)fd_list[k], &readfds);
						if((icf_int32_t)fd_list[k] > maxfd)
						{
							maxfd = (icf_int32_t)fd_list[k];
						}
					}
				}

				table = rtp_table->front;
				while(table != MM_NULL) 
				{
					if(table->rtcp_socket.s != -1) 
					{
						if (FD_ISSET(table->rtcp_socket.s, &rset)) 
						{
							struct sockaddr fromAddr = {0};
							icf_int32_t fromLen = sizeof(fromAddr);
							icf_int32_t rc = -1;
							rc = recvfrom(table->rtcp_socket.s, (char *)RTCPStackPacket, sizeof(RTCPStackPacket), 0, &fromAddr, &fromLen);
							if(rc > 0 )
							{
								rtcpRxProc(table, RTCPStackPacket, rc);
							}
							else if (rc == 0)
							{
								/*
								** Close RTCP recv socket
								*/
								RTP_PRINTF("rtcpTask: Closing rtcp recv socket\n");
								FD_CLR(table->rtcp_socket.s, &readfds);
								if (maxfd == table->rtcp_socket.s) 
								{
									for(j = table->rtcp_socket.s; j > 0; j--) 
									{
										if (FD_ISSET(j, &readfds)) 
										{
											maxfd = j;
											break;
										}
									}
								}
								close(table->rtcp_socket.s);
								table->rtcp_socket.s = -1;
							}  /* else if */
						}
					}
					
					table = table->next;
				}
			}
		#if KSOCKET_RTP_SUPPORT
		}
		else{
			sleep(1);
		}
		#endif
	}

	RTP_PRINTF("rtcpTask ENDS");
}

/*______________________________________________________________________________
**	rtpTaskCreate
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

static icf_int32_t rtpTaskCreate( void )
{
	int  status;
   
	status = taskCreate( "rtp",
				8*1024,
				TASK_CLASS_HIGH,
				(void *)rtpTask);

	if ( status != RET_SUCCESS ) {
		ZyPrintf("!!%s fail !!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	return MM_SUCCESS;  
}

/*______________________________________________________________________________
**	rtcpTaskCreate
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

static icf_int32_t rtcpTaskCreate( void )
{
	int  status;
   
	status = taskCreate( "rtcp",
				4*1024,
				TASK_CLASS_MED_LOW,
				(void *)rtcpTask);

	if ( status != RET_SUCCESS ) {
		ZyPrintf("!!%s fail !!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	return MM_SUCCESS;  
}


/*______________________________________________________________________________
**	rtpInit
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
mm_return_t rtpInit(
	void
) {
	#if SRTP_SUPPORT
	srtpInit();
	#endif

	if(MM_FAILURE == rtpTableListInit()){
		return MM_FAILURE;
	}		
	if(MM_FAILURE == rtpTaskCreate()){
		return MM_FAILURE;
	}	
	if(MM_FAILURE == rtcpTaskCreate()){
		return MM_FAILURE;
	}	

#if 1  /*Jason , rtp statistic*/
	SipLineStatsResetAll();
#endif
	return MM_SUCCESS;
} /* rtpInit */


/*______________________________________________________________________________
**	rtpRxProc
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
static icf_int32_t rtpRxProc( 
       rtpTable_t *table,
	void *packet, 
	icf_int32_t length 
){
	if((ICF_STREAM_TYPE_AUDIO == table->stream_type) || (ICF_STREAM_TYPE_VIDEO == table->stream_type))
	{
		if( (table->media_mode == ICF_MEDIA_MODE_INVALID) 
			|| (table->media_mode == ICF_MEDIA_MODE_INACTIVE) 
			|| (table->media_mode == ICF_MEDIA_MODE_SEND_ONLY) )
		{
			return 0;
		}
	}

	if (RTP_VERSION == ((rtp_header_t*)packet)->version)
	{
		table->remote_ssrc = ntohl(((rtp_header_t*)packet)->ssrc);
		if(1 == table->rtcp_enabled)
		{
			rtcpStoreRtpInfo(table, packet, length);
		}
	}
	else
	{
		//RTP_PRINTF("\n\r Unknown RTP version");
	}
	
	/* Send the packet */
	if (table->voipRtpCallBack != NULL)
	{
		table->voipRtpCallBack(table->call_id, table->stream_id, table->stream_type, 0, packet, length, 0, NULL);
        usleep(1);
	}

	return 0;
}


/*______________________________________________________________________________
**	rtpTableAlloc
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
static rtpTable_t *
rtpTableAlloc(
	void
) {
	rtpTable_t *bp;

	if( ( bp = (rtpTable_t *)zyMalloc(sizeof(rtpTable_t))) == MM_NULL ){
		return MM_NULL;
	}
	zyMemset(bp, 0, sizeof(rtpTable_t));
	return bp;
}


/*______________________________________________________________________________
**	rtpTableInit
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
static void rtpTableInit(
	rtpTable_t *table
){
	table->ssrc = 0;
	table->seq = 0;

	rtcpGetLocalCname(&table->rtcp_stats, 0);
	table->rtcp_stats.tx_last_rtp_pktcnt = 0;
	table->rtcp_stats.tx_rtp_pkt_cnt = 0;
	table->rtcp_stats.tx_last_rtp_pktcnt = 0;
	table->rtcp_stats.tx_rtp_octet_cnt = 0;
	table->rtcp_stats.tx_rtp_pkt_failcnt = 0;
	table->rtcp_stats.tx_rtcp_cnt = 0;
	table->rtcp_stats.tx_rtcp_failcnt =0;
	table->rtcp_stats.rx_rtp_pkt_cnt =0;
	table->rtcp_stats.rx_rtp_octet_cnt = 0;
	table->rtcp_stats.tx_rtcp_cnt = 0;
	table->rtcp_stats.rx_rtcp_bye_recvd =0;
	table->rtcp_stats.rx_rtcp_rr_recvd =0;
	table->rtcp_stats.rx_rtcp_sr_recvd = 0;

	table->rtcp_stats.tx_rtcp_sr_sent = 0;
	table->rtcp_stats.tx_rtcp_bye_sent = 0;
#if 1  /*Jason , rtp statistic*/
	table->rtcp_stats.remoteLossRate = 0;
	table->rtcp_stats.remoteJitter = 0;
	table->rtcp_stats.totalRemoteJitter = 0;
	table->rtcp_stats.totalRemoteCnt = 0;
#endif
	/* RTCP timer control block*/
	rtcpGetEnable(table, 0);
	rtcpGetTxInterval(table, 0);
	table->rtcp_tx_timer.tm_id = 0;
	table->rtcp_tx_timer.tm_intv = 0;
	table->rtcp_tx_timer.rty_count = 0;

	table->next = NULL;
}
/*______________________________________________________________________________
**	rtpTableListInit
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
static icf_int32_t rtpTableListInit(
	void
) {
	if( ( rtp_table = ( rtpTablePool_t *)zyMalloc(sizeof(rtpTablePool_t))) == MM_NULL ){
		RTP_PRINTF("%s fail!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	rtp_table->front = MM_NULL;
	rtp_table->rear = MM_NULL;
	rtp_table->count = 0;
	
	return MM_SUCCESS;
}

/*______________________________________________________________________________
**	rtpTableGetById
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
rtpTable_t *rtpTableGetById(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type
) {
	rtpTable_t *bp;
	icf_uint32_t count;

	count = rtp_table->count;
	if(  count == 0 )   /* no element in this table */
		return NULL;

	bp = rtp_table->front;

	while( bp != NULL) 
	{
		if(( bp->call_id == call_id )
			&& ( bp->stream_id == stream_id )
				&& ( bp->stream_type == stream_type )) 
		{
			return bp;
		} 
		else 
		{
			bp = bp->next;
		}
	}

	return NULL;   /* not found */
}

/*______________________________________________________________________________
**	rtpTableInsert
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
static icf_int32_t  rtpTableInsert(
	rtpTable_t *table
) {

	if( rtp_table->count == 0 ){
		rtp_table->rear = table;
		rtp_table->front = table;
		++rtp_table->count;
	} else {
		rtp_table->rear->next = table;
		rtp_table->rear = table;
		++rtp_table->count;
	}

#ifdef VOIP_SYSLOG /*Jason , syslog*/
	table->initTimeStamp = hwTick();
#endif
	return MM_SUCCESS;
}

/*______________________________________________________________________________
**	rtpTableFree
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
static icf_int32_t rtpTableFree(
	rtpTable_t *bp
) {

	if ( bp != MM_NULL ) {
		zyFree(bp);
		bp = MM_NULL;
	}

	return MM_SUCCESS;
}

/*______________________________________________________________________________
**	rtpTableDelete
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
static icf_int32_t rtpTableDelete(
       icf_call_id_t call_id,
	icf_stream_id_t stream_id,
	icf_media_type_t	stream_type
) {
	rtpTable_t *bp, *pre_bp = MM_NULL, *tmp;
	icf_uint32_t  count;

	count = rtp_table->count;
	if(  count == 0 )   /* no element in this table */
		return MM_FAILURE;

	bp = rtp_table->front;

	while( bp != MM_NULL )
	{
		if ((bp->call_id == call_id)	
			&& (bp->stream_id == stream_id)
				&& (bp->stream_type == stream_type))
		{
			if( count == 1 ){ /* contain only 1 element */
				rtpTableFree(bp);
				rtp_table->front = MM_NULL;
				rtp_table->rear = MM_NULL;
				rtp_table->count = 0;
				return MM_SUCCESS;
			}
			else if(pre_bp == MM_NULL ){  /* the first element */
				tmp = bp->next;
				rtp_table->front = tmp;
				--rtp_table->count;
				rtpTableFree(bp);
				return MM_SUCCESS;
			}
			else if(pre_bp != MM_NULL){
				tmp = bp->next;
				pre_bp->next = tmp;
				if( tmp == MM_NULL)
					rtp_table->rear = pre_bp;

				--rtp_table->count;
				rtpTableFree(bp);
				return MM_SUCCESS;
			}
		}
		pre_bp = bp;
		bp = bp->next;
	}
	
	return MM_FAILURE;

}


/*______________________________________________________________________________
**	rtpTableDisplay
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:	0: successful,		-1: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
void
rtpTableDisplay(
	void
){
	rtpTable_t	*bp = MM_NULL;

	if( rtp_table != NULL && rtp_table->count != 0 )
	{
		bp = rtp_table->front;

		while( bp != NULL)
		{
			RTP_PRINTF("\n\tRTP >  rtp call_id=%d, stream_id=%d, stream_type=%d", bp->call_id, bp->stream_id, bp->stream_type);
			RTP_PRINTF("\n\tRTP >  rtp socket=%d", bp->rtp_socket.s);
			RTP_PRINTF("\n\tRTP >  rtp local addr/port=%x/%d", bp->rtp_socket.local_addr, bp->rtp_socket.local_port);
			RTP_PRINTF("\n\tRTP >  rtp remote addr/port=%x/%d", bp->rtp_socket.remote_addr, bp->rtp_socket.remote_port);
			RTP_PRINTF("\n\tRTP >  rtp media_mode=%d", bp->media_mode);
			RTP_PRINTF("\n\tRTP >  ");
			bp = bp->next;
		}
	}

}


/*______________________________________________________________________________
**	isRemoteAddrExistInRtpTable
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:	0: FALSE,		1: TRUE
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static bool
isRemoteAddrExistInRtpTable(
       icf_call_id_t	call_id,
	icf_uint32_t	remote_addr
	#if 1 /* MSTC porting by Andylin, bind interface , Iproute2 */
	, char *type
	#endif
){
	rtpTable_t *bp;
	icf_uint32_t count;
        /* MSTC porting by Andylin, bind interface , Iproute2 */
	icf_uint32_t bp_remote_addr;

	count = rtp_table->count;
	if(  count == 0 )   /* no element in this table */
		return FALSE;

	bp = rtp_table->front;

	while( bp != NULL) 
	{
                /* MSTC porting by Andylin, bind interface , Iproute2 */
		if(zyStrcmp(type,"Rtp") == 0)
			bp_remote_addr = bp->rtp_socket.remote_addr;
		else if(zyStrcmp(type,"Rtcp") == 0)
			bp_remote_addr = bp->rtcp_socket.remote_addr;
		else
			return FALSE;
		
		if(( bp->call_id != call_id ) && (bp_remote_addr == remote_addr))
		{
			return TRUE;
		} 
		else 
		{
			bp = bp->next;
		}
	}

	return FALSE;   /* not found */
}

/* MSTC porting by Andylin, bind interface , Iproute2 */
static mm_return_t rtpGetIfName(icf_uint32_t ipaddr, char *ifname)
{
	int socketfd;
	struct ifconf ifc;
	struct ifreq *p_ifq;
	struct sockaddr_in *addr;
	char buf[sizeof(struct ifreq) * 10];
	int i, n;
	
	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		ZyPrintf("Open socket failure\n");
		return MM_FAILURE;
	}
	
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = (caddr_t)buf;

	if (ioctl(socketfd, SIOCGIFCONF, (caddr_t)&ifc) < 0)
	{
		ZyPrintf("ioctl for SIOCGIFCONF failure\n");
		close(socketfd);
		return MM_FAILURE;
	}
	close(socketfd);

	p_ifq = ifc.ifc_req;
	//ZyPrintf("len = %d, sizeof(ifreq) = %d\n", ifc.ifc_len, sizeof(struct ifreq));
	n = ifc.ifc_len / sizeof(struct ifreq);
	for (i = 0; i < n; ++i)
	{
		addr = (struct sockaddr_in *)&(p_ifq->ifr_addr);
		if (addr->sin_addr.s_addr == ipaddr)
		{
			strcpy(ifname, p_ifq->ifr_name);
			return MM_SUCCESS;
		}
		++p_ifq;
	}
	
	return MM_FAILURE;
}


/*______________________________________________________________________________
**	udpSend
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
icf_uint32_t udpSend(
	icf_int32_t	socket, 
	icf_uint8_t	*packet, 
	icf_uint16_t	len,
	icf_uint16_t	remote_port,
	icf_uint32_t	remote_addr
){
 	struct sockaddr_in  sin = {0};     /* socket address information */
	icf_int32_t nsent;

   	sin.sin_family = AF_INET;
	sin.sin_port = htons(remote_port);
	sin.sin_addr.s_addr = htonl(remote_addr);

	nsent = sendto(socket, (char *)packet, len, 0, &sin, sizeof(sin));
	if ((nsent < 0) || (nsent != len))
	{
		RTP_PRINTF ("ERROR udpSend: nsent=%u, addr/port=%x/%d \n", nsent, remote_addr,remote_port);
		return -1;
	}

	return nsent;
}

/*______________________________________________________________________________
**	rtpSocketOpen
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
static icf_uint32_t rtpSocketOpen(
	udpSocket_t	*p_soket
){
	icf_int32_t    rtpSock = -1;
	struct sockaddr_in sin = {0};  /* socket address information */
	
	#if KSOCKET_RTP_SUPPORT
	if(mmKSocketEnable){
		ZyPrintf("%s, kernel socket enabled, skip rtp socket open, return SUCCESS\n",__FUNCTION__);
		return MM_SUCCESS;
	}
	#endif
	
	rtpSock = socket(AF_INET, SOCK_DGRAM, 0);	
	if (rtpSock < 0) {
		RTP_PRINTF("Error: open rtpUDPSock fail\n");
		return MM_FAILURE;
	}
	sin.sin_family = AF_INET;
#if SUPPORT_412 /*Jennifer, when not using kernel socket, socket can't be close*/
	sin.sin_addr.s_addr = htonl(p_soket->local_addr);
#else
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
#endif
	sin.sin_port = htons(p_soket->local_port);
	
	if (bind(rtpSock, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
		RTP_PRINTF("Error during rtpUDPSock bind\n");
		close(rtpSock);
		return MM_FAILURE;
	}
	p_soket->s = rtpSock;
	

	return MM_SUCCESS;
}


/*______________________________________________________________________________
**	rtp_open
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
mm_return_t  rtpOpen(
       icf_call_id_t call_id,
       icf_line_id_t SipAccountId,
	mmCtxStream_t	*p_ctx_stream
) {
	rtpTable_t*	table = MM_NULL;
	RTP_PRINTF("\n\tMM >Enter %s...\n", __FUNCTION__);

	if( (table = rtpTableAlloc()) == MM_NULL) 
	{
		RTP_PRINTF("\n\rrtp table alloc fail");
		return MM_FAILURE;
	}

	rtpTableInit(table);
	
	table->call_id = call_id;
	table->stream_id = p_ctx_stream->stream_id;	
	table->stream_type = p_ctx_stream->stream_type;
	table->line_id = SipAccountId;

	/* local SDP*/
	if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_ctx_stream->local_sdp.stream_address.addr.addr_type)
	{
		table->rtp_socket.local_addr = MM_MAKE_32BIT_INT(
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1,
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2,
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3,
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4);
#ifdef NORWAY_VOIP_CUSTOMIZATION
		sprintf(rtp_IPAddr, "%d.%d.%d.%d", (table->rtp_socket.local_addr >> 24) & 0xFF,
	                                       (table->rtp_socket.local_addr >> 16) & 0xFF,
	                                       (table->rtp_socket.local_addr >> 8) & 0xFF,
	                                       (table->rtp_socket.local_addr) & 0xFF);							
#endif								
		p_ctx_stream->local_sdp.stream_address.port_num = rtpPortPoolAllocate(SipAccountId , p_ctx_stream->stream_type);
		
		table->rtp_socket.local_port = p_ctx_stream->local_sdp.stream_address.port_num;

		/* RTCP local*/
		table->rtcp_socket.local_addr = table->rtp_socket.local_addr;
		table->rtcp_socket.local_port = table->rtp_socket.local_port + 1;
	}

	/* remote SDP*/
	if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_ctx_stream->remote_sdp.stream_address.addr.addr_type)
	{
		table->rtp_socket.remote_addr = MM_MAKE_32BIT_INT(
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_1,
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_2,
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_3,
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_4);
		table->rtp_socket.remote_port = p_ctx_stream->remote_sdp.stream_address.port_num;

		/* RTCP remote*/
		table->rtcp_socket.remote_addr = table->rtp_socket.remote_addr;
		table->rtcp_socket.remote_port = table->rtp_socket.remote_port + 1;
	}
	
	table->media_mode = p_ctx_stream->media_mode;
        /* MSTC porting by Andylin, bind interface , Iproute2 */
	rtpGetIfName(htonl(table->rtp_socket.local_addr), table->ifname);

	mmRouteQosFirewallControl(table->rtp_socket.remote_addr,table->rtp_socket.remote_port, table->rtp_socket.local_port, 1 , table->line_id);
	mmRouteQosFirewallControl(table->rtcp_socket.remote_addr,table->rtcp_socket.remote_port, table->rtcp_socket.local_port, 1 , table->line_id);
	
		if(MM_FAILURE == rtpSocketOpen(&table->rtp_socket))
		{
			rtpTableFree(table);
			return MM_FAILURE;
		}

		if(MM_FAILURE == rtpSocketOpen(&table->rtcp_socket))
		{
			RTP_PRINTF("MM> rtcp open fail \n");
			rtpTableFree(table);
			return MM_FAILURE;
		}
	

	table->voipRtpCallBack = RtpSendToDsp;
	table->voipRtcpCallBack = RtcpSendToDsp;
	
	#if KSOCKET_RTP_SUPPORT
	if(!mmKSocketEnable){
	#endif
		if (setsockopt(table->rtp_socket.s, SOL_IP, IP_TOS, &p_ctx_stream->qos_mark, sizeof(icf_qos_mark_t)) != 0 )
		{
			RTP_PRINTF("MM> setsockopt fail \n");
		}
		#if 1 /* MSTC porting by Andylin, bind interface , Iproute2 */
		if (setsockopt(table->rtp_socket.s, SOL_SOCKET, SO_BINDTODEVICE, table->ifname, MAX_IFNAME_LEN) != 0 )
		{
			RTP_PRINTF("MM> setsockopt fail \n");
		}
		#endif
	#if KSOCKET_RTP_SUPPORT
	}
	#endif
	/* MSTC porting by Andylin, bind interface , Iproute2 */
	if (setsockopt(table->rtcp_socket.s, SOL_SOCKET, SO_BINDTODEVICE, table->ifname, MAX_IFNAME_LEN) != 0 )
	{
		RTP_PRINTF("MM> setsockopt fail \n");
	}
	

	table->qos_mark = p_ctx_stream->qos_mark;

	if(MM_FAILURE == rtpTableInsert(table))
	{
		rtpTableFree(table);
		RTP_PRINTF("MM> rtpTableInsert fail \n");
		return MM_FAILURE;
	}
/*Jason , syslog*/	
#ifndef VOIP_SYSLOG  
#if !defined(RTP_BCM_RTCP_DRIVER_TASK)
	if(0 != table->rtcp_tx_timer.interval) 
	{
		RTP_PRINTF("MM> rtcpEnableTxTimer  \n");
		rtcpEnableTxTimer(table);
	}
#endif
#endif
	#if KSOCKET_RTP_SUPPORT
	if(!mmKSocketEnable){
	#endif
		write(rtpFDpipe[1], (char*) &table->rtp_socket.s, sizeof(icf_int32_t));
		write(rtcpFDpipe[1], (char*) &table->rtcp_socket.s, sizeof(icf_int32_t));
	#if KSOCKET_RTP_SUPPORT
	}
	#endif
	

RTP_PRINTF("\n\tMM >Exist %s Succ\n", __FUNCTION__);
	return MM_SUCCESS;

}

/*______________________________________________________________________________
**	rtpSend
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

void rtpSend(
	icf_call_id_t call_id, 
	icf_stream_id_t	stream_id, 
	icf_media_type_t	stream_type,
	uint16 logical_id,
	icf_uint32_t pload_type, 
	icf_uint8_t *packet, 
	icf_uint16_t len, 
	icf_uint32_t timestamp, 
	void* handle
)
{
	rtpTable_t *table;
	
	icf_int32_t nsent = 0;
	#if SRTP_SUPPORT
	int err= 0;
	#endif
	
	if( (table = rtpTableGetById(call_id, stream_id, stream_type)) == NULL ) {
		return 0;
	}

	if((ICF_STREAM_TYPE_AUDIO == table->stream_type) || (ICF_STREAM_TYPE_VIDEO == table->stream_type))
	{
		if((table->rtp_socket.remote_addr == 0) 
			|| (table->media_mode == ICF_MEDIA_MODE_INVALID) 
			|| (table->media_mode == ICF_MEDIA_MODE_INACTIVE) 
			|| (table->media_mode == ICF_MEDIA_MODE_RECV_ONLY) )
		{
			return 0;
		}
	}

	table->ssrc = ntohl(((rtp_header_t*)packet)->ssrc);
	table->seq = ntohs(((rtp_header_t*)packet)->seq);
	table->tx_timestamp = ntohl(((rtp_header_t*)packet)->ts);
	
	#if SRTP_SUPPORT
	if(NULL != table->srtpSendSession){
		int length = len;		
		err = srtp_protect(table->srtpSendSession, packet, &length);
		if(err != 0){
			ZyPrintf("srtp_protect ret = %d\n", err);
		}
		len = length;		
	}
	#endif
	nsent = udpSend(table->rtp_socket.s, (char *)packet, len, table->rtp_socket.remote_port, table->rtp_socket.remote_addr);

#if !defined(RTP_BCM_RTCP_DRIVER_TASK)
	if((nsent < 0) || (nsent != len))	
	{
		++table->rtcp_stats.tx_rtp_pkt_failcnt;
   	} 
	else 
	{
		table->rtcp_stats.tx_last_rtp_pktcnt = table->rtcp_stats.tx_rtp_pkt_cnt;
		++table->rtcp_stats.tx_rtp_pkt_cnt;
		table->rtcp_stats.tx_rtp_octet_cnt += len;
   	}
#endif
}

/*______________________________________________________________________________
**	rtpClose
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
mm_return_t  rtpClose(
       icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type
) {
	rtpTable_t *table = MM_NULL;

#ifdef VOIP_SYSLOG /*Jason , syslog*/
	uint32 calltime = 0;
	uint32 fraction, lost, jitter;
#endif	
	RTP_PRINTF("entering %s, call_id: %d\n", __FUNCTION__, call_id);
	if( (table = rtpTableGetById(call_id, stream_id, stream_type)) == MM_NULL ){
		RTP_PRINTF("port not open yet!");
		return MM_FAILURE;
	}

#if !defined(RTP_BCM_RTCP_DRIVER_TASK)
	rtcpDisableTxTimer(table);
#endif
#if KSOCKET_RTP_SUPPORT /*KSOCKET_RTP_SUPPORT*/
	if(mmKSocketEnable){		
		dspPoolAcquireMutex();
		dspChan_t *dspChan_p = dspPoolFindDspChanByCallId(call_id);
		if(dspChan_p != NULL){
			//dspDriverKSocketDeleteSession(dspChan_p, table, &lost, &jitter);
			dspDriverGetRtpStatistics(dspChan_p, table, &lost, &jitter);
		}
		dspPoolReleaseMutex();
		dspDriverKSocketDeleteSession(call_id, table, &lost, &jitter);
	}
#endif
	/* Remove RTP static route & dynamic QoS & firewall rules */
//Ssu-Ying, support Multiple Profile
/* MSTC porting by Andylin, bind interface , Iproute2 */
	if(isRemoteAddrExistInRtpTable(table->call_id, table->rtp_socket.remote_addr, "Rtp")) {
		mmRouteQosFirewallControl(0,table->rtp_socket.remote_port, table->rtp_socket.local_port, 0 , table->line_id);
	} else {
		mmRouteQosFirewallControl(table->rtp_socket.remote_addr,table->rtp_socket.remote_port, table->rtp_socket.local_port, 0 , table->line_id);
	}
	/* Remove RTCP dynamic QoS & firewall rules */
	if(isRemoteAddrExistInRtpTable(table->call_id, table->rtcp_socket.remote_addr, "Rtcp")) {
		mmRouteQosFirewallControl(0, table->rtcp_socket.remote_port ,table->rtcp_socket.local_port, 0 , table->line_id);
	}else{
		mmRouteQosFirewallControl(table->rtcp_socket.remote_addr, table->rtcp_socket.remote_port ,table->rtcp_socket.local_port, 0 , table->line_id);
	}
	#if KSOCKET_RTP_SUPPORT
	if(!mmKSocketEnable){
	#endif
	   	/*
	   	** Send a zero-length packet to the receiver thread to kick it out of the
	   	** socket blocked read loop
	   	*/
#if SUPPORT_412 /*Jennifer, when not using kernel socket, socket can't be close*/
		udpSend(table->rtp_socket.s, "", 0, table->rtp_socket.local_port, table->rtp_socket.local_addr);
		udpSend(table->rtcp_socket.s, "", 0, table->rtcp_socket.local_port, table->rtcp_socket.local_addr);
#else
		udpSend(table->rtp_socket.s, "", 0, table->rtp_socket.local_port, RTP_LOOPBACK);
		udpSend(table->rtcp_socket.s, "", 0, table->rtcp_socket.local_port, RTP_LOOPBACK);
#endif
	#if KSOCKET_RTP_SUPPORT
	}
	#endif

   	/*
   	** Receiver sockets are closed at the receiver thread's context.
   	** Wait until they are closed.
   	*/
   	#if KSOCKET_RTP_SUPPORT
	if(!mmKSocketEnable){
	#endif
	   	while ((table->rtp_socket.s != -1) || (table->rtcp_socket.s != -1))
	   	{
		   	RTP_PRINTF("usleep(100000)");
	   	   	usleep(100000);
	   	}
	#if KSOCKET_RTP_SUPPORT
	}
	#endif
	

	rtpPortPoolRelease(table->rtp_socket.local_port, table->stream_type);

#ifdef VOIP_SYSLOG /*Jason , syslog*/
	/* tr104 - Update cumulative statistics for this line */
	calltime = (hwTick() - table->initTimeStamp) / 100;
	sipLineStats[table->line_id].total_tx_pkt_cnt += table->rtcp_stats.tx_rtp_pkt_cnt;
	sipLineStats[table->line_id].total_tx_oct_cnt += table->rtcp_stats.tx_rtp_octet_cnt;
	sipLineStats[table->line_id].total_rx_pkt_cnt += table->rtcp_stats.rx_rtp_pkt_cnt;
	sipLineStats[table->line_id].total_rx_oct_cnt += table->rtcp_stats.rx_rtp_octet_cnt;
	sipLineStats[table->line_id].total_call_time += calltime;

	sipLineStats[table->line_id].tx_pkt_cnt = table->rtcp_stats.tx_rtp_pkt_cnt;
	sipLineStats[table->line_id].tx_oct_cnt = table->rtcp_stats.tx_rtp_octet_cnt;
	sipLineStats[table->line_id].rx_pkt_cnt = table->rtcp_stats.rx_rtp_pkt_cnt;
	sipLineStats[table->line_id].rx_oct_cnt = table->rtcp_stats.rx_rtp_octet_cnt;

#if KSOCKET_RTP_SUPPORT /*KSOCKET_RTP_SUPPORT*/
	if(!mmKSocketEnable){			
#endif
		if(table->rtcp_stats.rx_rtp_pkt_cnt>0){
			rtcpPktFractionlost(&table->rtcp_stats.rtp_rr, &fraction, &lost);
			sipLineStats[table->line_id].pkt_lost = lost;
			sipLineStats[table->line_id].avgjitter = (table->rtcp_stats.rtp_rr.totalJitter * 125) / (table->rtcp_stats.rx_rtp_pkt_cnt);
		}
#if KSOCKET_RTP_SUPPORT /*KSOCKET_RTP_SUPPORT*/		
	}
	else{
		sipLineStats[table->line_id].pkt_lost = lost;		
		sipLineStats[table->line_id].avgjitter = jitter;
		
	}
#endif
		

	RTP_PRINTF("syslog : call id is %d\n",call_id);
	RTP_PRINTF("syslog : line id is %d\n",table->line_id);
	RTP_PRINTF("\r\n rx_rtp_pkt_cnt is %d\n",table->rtcp_stats.rx_rtp_pkt_cnt);
	
	RTP_PRINTF("[rtp_close]tx: %d, %d\n", sipLineStats[table->line_id].total_tx_pkt_cnt, sipLineStats[table->line_id].total_tx_oct_cnt);
	RTP_PRINTF("[rtp_close]rx: %d, %d\n", sipLineStats[table->line_id].total_rx_pkt_cnt, sipLineStats[table->line_id].total_rx_oct_cnt);
	RTP_PRINTF("[rtp_close]time: %d\n", sipLineStats[table->line_id].total_call_time);

#endif
	rtpTableDelete(call_id, stream_id, stream_type);

		
	return MM_SUCCESS;
}

/*______________________________________________________________________________
**	rtpModify
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
mm_return_t  rtpModify(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream
) {
	mm_return_t	ret_val = MM_FAILURE;
	rtpTable_t *table = MM_NULL;
	#if 1 /* MSTC porting by Andylin, bind interface , Iproute2 */
	uint32 rtp_remote_addr = 0, rtcp_remote_addr = 0;
	uint32 rtp_remote_port = 0, rtcp_remote_port = 0;
	uint32 rtp_local_port =0, rtcp_local_port = 0;
	uint16 sipAccntIdx = 0;
	uint8 flag = 0; // DISABLE
	#endif

	if( (table = rtpTableGetById(call_id, p_ctx_stream->stream_id, p_ctx_stream->stream_type)) == MM_NULL ){
		RTP_PRINTF("rtp_table not found!");
		return MM_FAILURE;
	}
	
	#if 1 /* MSTC porting by Andylin, bind interface , Iproute2 */
	rtp_remote_addr = table->rtp_socket.remote_addr;
	rtp_remote_port =  table->rtp_socket.remote_port;
	rtp_local_port = table->rtp_socket.local_port;
	sipAccntIdx = table->line_id;
	rtcp_remote_addr = table->rtcp_socket.remote_addr;
	rtcp_remote_port =  table->rtcp_socket.remote_port;
	rtcp_local_port = table->rtcp_socket.local_port;
	#endif
	
	/* local SDP*/
	if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_ctx_stream->local_sdp.stream_address.addr.addr_type)
	{
		table->rtp_socket.local_addr = MM_MAKE_32BIT_INT(
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_1,
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_2,
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_3,
									p_ctx_stream->local_sdp.stream_address.addr.addr.ipv4_addr.octet_4);
		
		table->rtp_socket.local_port = p_ctx_stream->local_sdp.stream_address.port_num;

		/* RTCP local*/
		table->rtcp_socket.local_addr = table->rtp_socket.local_addr;
		table->rtcp_socket.local_port = table->rtp_socket.local_port + 1;
		#if KSOCKET_RTP_SUPPORT
		if(!mmKSocketEnable){
		#endif
#if 1 /*Jason , [SPRID # 090618210]*/
			if (setsockopt(table->rtp_socket.s, SOL_IP, IP_TOS, &p_ctx_stream->qos_mark, sizeof(icf_qos_mark_t)) != 0 )
			{
				RTP_PRINTF("MM> setsockopt fail \n");
			}
#endif	
		#if KSOCKET_RTP_SUPPORT
		}
		#endif
	}

	/* remote SDP*/
	if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_ctx_stream->remote_sdp.stream_address.addr.addr_type)
	{
		table->rtp_socket.remote_addr = MM_MAKE_32BIT_INT(
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_1,
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_2,
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_3,
									p_ctx_stream->remote_sdp.stream_address.addr.addr.ipv4_addr.octet_4);
		table->rtp_socket.remote_port = p_ctx_stream->remote_sdp.stream_address.port_num;

		/* RTCP remote*/
		table->rtcp_socket.remote_addr = table->rtp_socket.remote_addr;
		table->rtcp_socket.remote_port = table->rtp_socket.remote_port + 1;

		/* MSTC porting by Andylin, bind interface , Iproute2 */
		/* local SIP calls each other, do not bind to interface */
		if (table->rtp_socket.remote_addr == table->rtp_socket.local_addr)
		{
			strcpy(table->ifname, "");          

			if (setsockopt(table->rtp_socket.s, SOL_SOCKET, SO_BINDTODEVICE, table->ifname, MAX_IFNAME_LEN) != 0 )
			{
				RTP_PRINTF("MM> setsockopt fail \n");
			}

			if (setsockopt(table->rtcp_socket.s, SOL_SOCKET, SO_BINDTODEVICE, table->ifname, MAX_IFNAME_LEN) != 0 )
			{
				RTP_PRINTF("MM> setsockopt fail \n");
			}

		}
	}
	
	table->media_mode = p_ctx_stream->media_mode;
	RTP_PRINTF("\n %s(): media_mode = %d\n",__FUNCTION__,table->media_mode);
	
	/** this will cause T38 failed due to media mode remains inactive
	* refer detailed info from http://172.23.56.144/trac/VoIPTeam/ticket/612
	* cindy modify 20100927 */
	if(table->stream_type == ICF_STREAM_TYPE_FAX ){ //ICF_STREAM_TYPE_FAX
		RTP_PRINTF("\n %s(): stream type is 4 (FAX)\n",__FUNCTION__);
		table->media_mode = ICF_MEDIA_MODE_SEND_RECV;
		RTP_PRINTF("\n %s(): table->media_mode = 4 (SEND_RECV)\n",__FUNCTION__);
	}else{
		RTP_PRINTF("\n %s(): stream type is %d ( NOT FAX)\n",__FUNCTION__,table->stream_type);
	}
	
#ifdef VOIP_SYSLOG /*Jason , syslog */
	#if !defined(RTP_BCM_RTCP_DRIVER_TASK)
		if ( (0 != table->rtcp_tx_timer.interval) && (0 == table->rtcp_tx_timer.tm_id) )
		{
			RTP_PRINTF("MM> rtcpEnableTxTimer  \n");
			rtcpEnableTxTimer(table);
		}
	#endif
#endif

	/* add route & QoS & firewall rules for RTP */
#if 1 /* MSTC porting by Andylin, bind interface , Iproute2 */	
	if( table->rtp_socket.local_port == rtp_local_port){
		flag |= 1 <<1; // NOT_REMOVE
	}
	/* Remove RTP dynamic QoS, firewall, and NAT rules */
	mmRouteQosFirewallControl(rtp_remote_addr, rtp_remote_port, rtp_local_port, flag, sipAccntIdx);
	/* Add RTP dynamic QoS, firewall, and NAT rules */
	flag |= 1 << 0; // ENABLE
	mmRouteQosFirewallControl(table->rtp_socket.remote_addr, table->rtp_socket.remote_port, table->rtp_socket.local_port, flag, table->line_id);

	flag = 0; // DISABLE
	if( table->rtcp_socket.local_port == rtcp_local_port){
		flag |= 1 <<1; // NOT_REMOVE
	}
	
	/* sleep 100 ms to avoid call transfer fail */
	usleep(100000);
	
	/* Remove RTCP dynamic QoS, firewall, and NAT rules */
	mmRouteQosFirewallControl(rtcp_remote_addr, rtcp_remote_port, rtcp_local_port, flag, sipAccntIdx);
	flag |= 1 << 0; // ENABLE
	/* Add RTCP dynamic QoS, firewall, and NAT rules */
	mmRouteQosFirewallControl(table->rtcp_socket.remote_addr, table->rtcp_socket.remote_port, table->rtcp_socket.local_port, flag, table->line_id);
#else //Ssu-Ying, support Multiple Profile
	mmRouteQosFirewallControl(table->rtp_socket.remote_addr, table->rtp_socket.local_port, 1 , table->line_id);
	mmRouteQosFirewallControl(table->rtcp_socket.remote_addr, table->rtcp_socket.local_port, 1 , table->line_id);
#endif
	
	#if KSOCKET_RTP_SUPPORT
	if(mmKSocketEnable){
		if(table->rtp_socket.remote_addr!=0)
		{
			#if 0	/* aircheng mpdify for kernel socket */
			dspChan_t *dspChan_p=NULL;
			dspChan_p = dspPoolFindDspChanByCallId(call_id);
			if(NULL != dspChan_p ){			
				dspDriverKSocketMakeSession(dspChan_p, 
											table->rtp_socket.local_addr,
											table->rtp_socket.local_port,
											table->rtp_socket.remote_addr,
											table->rtp_socket.remote_port,
											table->media_mode,
											p_ctx_stream->qos_mark
											);
			}
			#else
				dspDriverKSocketMakeSession(call_id, 
								table->stream_id,
								table->rtp_socket.local_addr,
								table->rtp_socket.local_port,
								table->rtp_socket.remote_addr,
								table->rtp_socket.remote_port,
								table->media_mode,
								p_ctx_stream->qos_mark,
								table->ifname
								);
			#endif
		}
	}
	#endif

	ret_val = MM_SUCCESS;

RTP_PRINTF("\n\tMM >Exist %s ret_val=%d\n", __FUNCTION__, ret_val); 
	return ret_val;

}

/*______________________________________________________________________________
**	rtpConnect
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
mm_return_t  rtpConnect(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream
) {
	mm_return_t	ret_val = MM_FAILURE;
	dspChan_t	*dspChan_p = MM_NULL;

	voiceDsp_t *voiceDsp_p;
	int i,j;

	dspPoolAcquireMutex();

#if 1
	dspPoolModifyDspChanCallBack(rtpSend, rtcpSendPacket);
	ret_val = MM_SUCCESS;
#else

	dspChan_p = dspPoolFindDspChanById(call_id, p_ctx_stream->stream_id, p_ctx_stream->stream_type);
	if(MM_NULL != dspChan_p) 
	{
		#if KSOCKET_RTP_SUPPORT
		if(!mmKSocketEnable){
		#endif

			dspChan_p->voipDspCallBack = rtpSend;

#ifdef RTP_BCM_RTCP_DRIVER_TASK
			dspChan_p->voipDspRtcpCallBack = rtcpSendPacket;
#endif
		#if KSOCKET_RTP_SUPPORT
		}
		#endif


		ret_val = MM_SUCCESS;
	}
#endif

	dspPoolReleaseMutex();


RTP_PRINTF("\n\tMM >Exist %s ret_val=%d\n", __FUNCTION__, ret_val); 
	return ret_val;
}



/*______________________________________________________________________________
**	rtpPortSearchInUseLinkList
**
**	descriptions: Find if the free RTP port existed between the application setting range.
**	parameters:
**	local:
**	global:
**	return: 0: Not existed.	1: existed freeRtpPort
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static icf_uint16_t
rtpPortSearchInUseLinkList(
	icf_line_id_t SipAccountId,
	icf_uint32_t rtpPortPoolSize
){
	mmRtpPortRangePool_t* current_p;
#ifdef NORWAY_VOIP_CUSTOMIZATION
	icf_uint16_t rtpPortBase=0, rtpPortStart, rtpPortEnd=0, freeRtpPort=0, freePoolSize=0, inUsePort=0,MS_t=0;
#else
	icf_uint16_t rtpPortBase=0, rtpPortStart, rtpPortEnd=0, freeRtpPort=0, freePoolSize=0, inUsePort=0;
#endif	
	icf_cfg_set_port_range_req_st	*p_cfg_port_range = MM_NULL;
	
	p_cfg_port_range = &gMmCfgDb_t.port_range[SipAccountId];
	rtpPortStart = p_cfg_port_range->start_rtp_port;
	rtpPortEnd = p_cfg_port_range->end_rtp_port;

#ifdef NORWAY_VOIP_CUSTOMIZATION
	FILE *fp;
	icf_uint16_t MS_PortRange=0,MS_RanValue=0;
	srand( time(NULL));
	char used_port[8]="\0";
	
	MS_PortRange = rtpPortEnd - rtpPortStart;
	rtpPortBase=rtpPortStart;
	RTP_PRINTF("\n\tMM >rtpPortStart=[%d]\n",rtpPortStart);
	RTP_PRINTF("\n\tMM >rtpPortEnd=[%d]\n",rtpPortEnd);	
	RTP_PRINTF("\n\tMM >rtpPortPoolSize=[%d]\n",rtpPortPoolSize);
	
	do {

		if((MS_PortRange >= 20) && (freePoolSize ==0)){ 
			/*if interval bigger than 20, use random value.*/
			
			MS_RanValue = rand()%MS_PortRange;
			rtpPortBase=rtpPortStart + MS_RanValue;
			
		}else if (freePoolSize !=0){
			/*if freePoolSize don't be zero, check next port.*/
			rtpPortBase++;
		}else{
			rtpPortBase=rtpPortStart+MS_t;
			MS_t++;
		}
		
		RTP_PRINTF("\n\tMM >rtpPortBase=[%d]\n",rtpPortBase);
		if ( ( freePoolSize == 0 ) && ( ( rtpPortBase % 2 ) != 0 ) ) {		/* Select even port as the RTP port */
			continue;
		}		
		current_p = gMmCfgDb_t.p_portRangePool;
		
		
		if ( current_p == NULL ) {

				//add this part,let it will check port(x+1).
				if ( freePoolSize == 0 ) {
					freeRtpPort = rtpPortBase;
				}
				freePoolSize += 1;
				//2. check port be used in process.(netstat)
				if(freeRtpPort){
					if(!rtpPortSearchInnetstat(rtpPortBase)){
						freePoolSize = 0;
						freeRtpPort = 0;
						RTP_PRINTF("\n\tMM > Port be used in netstat!\n");
					}
				}

				//3. check port be used in NAT.
				if(freeRtpPort){
					if(!rtpPortSearchInconntrack(rtpPortBase,NF_UDP_port)){
						freePoolSize = 0;
						freeRtpPort = 0;
						RTP_PRINTF("\n\tMM > Port be used in conntrack!\n");
					}
				}
				
				if(freeRtpPort){
					if(!rtpPortSearchInconntrack(rtpPortBase,NF_UDP_port_expect)){
						freePoolSize = 0;
						freeRtpPort = 0;
						RTP_PRINTF("\n\tMM > Port be used in conntrack!\n");
					}
				}
			
		} else {
			inUsePort = 0;
			while( current_p != NULL) {
				//1. check others call.
				if (current_p->portNum == rtpPortBase) {
					inUsePort = 1;
					break;
				}
				current_p = current_p->next;
			}
			if( inUsePort == 0 )
			{
				//2. check port be used in process.(netstat)
				if(!inUsePort){
					if(!rtpPortSearchInnetstat(rtpPortBase)){
						inUsePort = 1;
						RTP_PRINTF("\n\tMM > Port be used in netstat!\n");
					}
				}

				//3. check port be used in NAT.
				if(!inUsePort){
					if(!rtpPortSearchInconntrack(rtpPortBase,NF_UDP_port)){
						inUsePort = 1;
						RTP_PRINTF("\n\tMM > Port be used in conntrack!\n");
					}
				}
				
				if(!inUsePort){
					if(!rtpPortSearchInconntrack(rtpPortBase,NF_UDP_port_expect)){
						inUsePort = 1;
						RTP_PRINTF("\n\tMM > Port be used in conntrack!\n");
					}
				}
			}
			
			if ( inUsePort == 0 ) {
				if ( freePoolSize == 0 ) {
					freeRtpPort = rtpPortBase;
				}
				freePoolSize += 1;
			} else {
				freePoolSize = 0;
				freeRtpPort = 0;
			}
		}
		if ( freePoolSize >= rtpPortPoolSize ) {
			break;
		}
	}while( rtpPortBase < rtpPortEnd );

	if ( freePoolSize >= rtpPortPoolSize ) {
		return freeRtpPort;
	} else {
		return MM_SUCCESS; //protect from no freeRtpPort, return fail.
	}
#else
	for ( rtpPortBase = rtpPortStart; rtpPortBase <= rtpPortEnd; rtpPortBase++)
	{
		if ( ( freePoolSize == 0 ) && ( ( rtpPortBase % 2 ) != 0 ) ) {		/* Select even port as the RTP port */
			continue;
		}
		current_p = gMmCfgDb_t.p_portRangePool;
		if ( current_p == NULL ) {
			freeRtpPort = rtpPortBase;
			freePoolSize = rtpPortPoolSize;
		} else {
			inUsePort = 0;
			while( current_p != NULL) {
				if (current_p->portNum == rtpPortBase) {
					inUsePort = 1;
					break;
				}
				current_p = current_p->next;
			}
			if ( inUsePort == 0 ) {
				if ( freePoolSize == 0 ) {
					freeRtpPort = rtpPortBase;
				}
				freePoolSize += 1;
			} else {
				freePoolSize = 0;
				freeRtpPort = 0;
			}
		}
		if ( freePoolSize >= rtpPortPoolSize ) {
			break;
		}
	}

	if ( freePoolSize >= rtpPortPoolSize ) {
		return freeRtpPort;
	} else {
		return MM_SUCCESS;
	}
#endif	
}

#ifdef NORWAY_VOIP_CUSTOMIZATION
static icf_uint16_t
rtpPortSearchInnetstat(
	icf_uint16_t rtpPort
){
	FILE *fp;
	char used_port[6]="";
	char cmd[256]="";
	sprintf(cmd, "netstat -na |grep	udp| sed 's/::/0/g' | cut -d ':' -f 2 | cut -d ' ' -f 1 | sed -e 's/'\\n'//g' > %s",BeUsed_Port);
	system(cmd);

	fp = fopen(BeUsed_Port, "r");
	if ( fp == NULL ) {
			/*if can not open, do not to check the port be use or not */
			RTP_PRINTF("\n\tMM >%s can NOT open %s\n", __FUNCTION__,BeUsed_Port);
			return 1;
	}else{
		while( !feof(fp) )
		{
			memset(used_port, 0, sizeof(used_port));
			if( !fgets(used_port,sizeof(used_port), fp) )		break;
								
			if(atoi(used_port) == rtpPort ){
				goto find;
			}	
		}
							
		fclose(fp);
	}
	sprintf(cmd,"rm -rf %s",BeUsed_Port);
    system(cmd);
	return 1;
find:
	fclose(fp);
	return 0;

}

static icf_uint16_t 
rtpPortSearchInconntrack(
	icf_uint16_t rtpPort,
	char *filename
){
	FILE *fp;
	char IP_port[NF_LEN]="";
	int i=0;
	char buffer[NF_LEN]="";
	sprintf(buffer, "%s:%d",rtp_IPAddr,rtpPort);

	fp = fopen(filename, "r");
	if ( fp == NULL ) {
		/*if can not open, do not to check the port be use or not */
		RTP_PRINTF("\n\tMM >Exist %s can NOT open %s\n", __FUNCTION__,filename);
		return 1;
	}
	while( !feof(fp) )
	{
		memset(IP_port, 0, sizeof(IP_port));
		if( !fgets(IP_port,sizeof(IP_port), fp) )		break;

		for(i = 0; i < NF_LEN; i++){		//Because the "echo" will bring "\n" in the buf, so clean to "".
			if(IP_port[i] == '\n')
				IP_port[i] = 0 ;
	    }
		if(!strcmp(IP_port,buffer)){
			goto find;
		}	
	}	
	fclose(fp);

	return 1;
find:
	fclose(fp);
	return 0;

}
#endif

/*______________________________________________________________________________
**	rtpPortPoolAllocate
**
**	descriptions:
**	parameters:	stream_type
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static icf_uint16_t
rtpPortPoolAllocate(
	icf_line_id_t SipAccountId,
	icf_media_type_t	stream_type
) {
	icf_uint32_t count;
	icf_uint16_t freeRtpPort = 0;
	mmRtpPortRangePool_t* current_p;
	icf_int32_t	rtpPortPoolSize = 0;
	
	if(ICF_STREAM_TYPE_AUDIO == stream_type){
		rtpPortPoolSize = NUM_RTPRTCP_PORT;
	}
	else{
		rtpPortPoolSize = 1;
	}
	
	if ( ( freeRtpPort = rtpPortSearchInUseLinkList(SipAccountId , rtpPortPoolSize) ) > 0 ) {
		for ( count=0; count<rtpPortPoolSize; count++) {
			current_p = zyMalloc( sizeof(mmRtpPortRangePool_t) );
			current_p->portNum = freeRtpPort + count;
			current_p->next = gMmCfgDb_t.p_portRangePool;
			gMmCfgDb_t.p_portRangePool = current_p;
		}
	}
	
RTP_PRINTF("\n\tMM >Exist %s freeRtpPort=%d\n", __FUNCTION__, freeRtpPort);

	return freeRtpPort;
}


/*______________________________________________________________________________
**	rtpPortPoolRelease
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:	0: successful,		-1: failed
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
static icf_int32_t
rtpPortPoolRelease(
	icf_uint16_t port,
	icf_media_type_t	stream_type
) {
	icf_uint16_t limitPort, beFreePort=0;
	mmRtpPortRangePool_t* pri_p;
	mmRtpPortRangePool_t* current_p;

	if(0 == port)
	{
		RTP_PRINTF("\n\tMM >rtpPortPoolRelease() fail");
		return MM_FAILURE;
	}

	if(ICF_STREAM_TYPE_AUDIO == stream_type){
		limitPort = port + NUM_RTPRTCP_PORT;
	}
	else{
		limitPort = port + 1;
	}

	if(0 == limitPort)
	{	/*limitPort ---> 65534+ 2= 0*/
		limitPort=0XFFFF;
	}

	/* Check if the port is at the head of the useListPtr */
	while (MM_NULL != gMmCfgDb_t.p_portRangePool) 
	{
		pri_p = gMmCfgDb_t.p_portRangePool;
		if ( ( pri_p->portNum >= port  ) && ( pri_p->portNum <= limitPort ) ) 
		{
			gMmCfgDb_t.p_portRangePool = pri_p->next;
			zyFree(pri_p);
			pri_p=NULL;
			beFreePort += 1;
		} 
		else 
		{
			break;
		}
	}

	if (MM_NULL != gMmCfgDb_t.p_portRangePool)
	{
		/* If the port is not at the head of the useListPtr */
		pri_p = gMmCfgDb_t.p_portRangePool;
		current_p = pri_p->next;
		while (MM_NULL != current_p) 
		{
			if ( ( current_p->portNum >= port  ) && ( current_p->portNum < limitPort ) ) 
			{
				pri_p->next = current_p->next;
				zyFree(current_p);
				current_p = NULL;
				beFreePort += 1;
				current_p = pri_p->next;
			} 
			else 
			{
				pri_p = pri_p->next;
				current_p = pri_p->next;
			}
		}
	}

RTP_PRINTF("\n\tMM >Exist %s, beFreePort=%d", __FUNCTION__, beFreePort);
	return MM_SUCCESS;
}

/*______________________________________________________________________________
**	rtpTableGetByLineId
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
rtpTable_t *rtpTableGetByLineId(
       uint16	line_id
) {
	rtpTable_t *bp;
	icf_uint32_t count;
	RTP_PRINTF("\n rtpTableGetByLineId > ENTER \n");
	count = rtp_table->count;
	if(  count == 0 )   /* no element in this table */
		return NULL;

	bp = rtp_table->front;

	while( bp != NULL) 
	{
		if( bp->line_id == line_id ){
			return bp;
		} 
		else 
		{
			bp = bp->next;
		}
	}

	return NULL;   /* not found */
}

void SipLineStatsResetAll(
	void
){
	int i;
	RTP_PRINTF("\n SipLineStatsResetAll > ENTER \n");
	for (i = 0; i < 2; i++) {
		memset(&sipLineStats[i], 0, sizeof(SipLineStats_t));
	}
}


#ifdef VOIP_SYSLOG /*Jason , syslog*/
/*______________________________________________________________________________
**	rtpGetStreamStats
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
int rtpGetStreamStats(
       icf_uint16_t	line_id,
       voiceDspMmStatsRsp_t *stats
) {
	rtpTable_t *rtp;
	int err = 0;
	int ret = MM_SUCCESS;
	uint32 fraction, lost;
	RTP_PRINTF("\n rtpGetStreamStats > ENTER \n");
	
	rtp = rtpTableGetByLineId(line_id);
	if( NULL == rtp ){

		stats->txPktCnt = sipLineStats[line_id].tx_pkt_cnt;
		stats->rxPktCnt = sipLineStats[line_id].rx_pkt_cnt;
		stats->txOctetCnt = sipLineStats[line_id].tx_oct_cnt;
		stats->rxOctetCnt = sipLineStats[line_id].rx_oct_cnt;
		stats->lostPktCnt = sipLineStats[line_id].pkt_lost;
		stats->avgRcvJitter =sipLineStats[line_id].avgjitter;		
		goto fx_exit;
	}
					
	stats->txPktCnt = rtp->rtcp_stats.tx_rtp_pkt_cnt;
	stats->rxPktCnt = rtp->rtcp_stats.rx_rtp_pkt_cnt;
	stats->txOctetCnt = rtp->rtcp_stats.tx_rtp_octet_cnt;
	stats->rxOctetCnt = rtp->rtcp_stats.rx_rtp_octet_cnt;


	if(rtp->rtcp_stats.rx_rtp_pkt_cnt>0){
		rtcpPktFractionlost(&rtp->rtcp_stats.rtp_rr, &fraction, &lost);
		stats->lostPktCnt = lost;
		stats->rcvJitter = rtp->rtcp_stats.rtp_rr.jitter * 125;		
 	    stats->avgRcvJitter = (rtp->rtcp_stats.rtp_rr.totalJitter * 125) / (rtp->rtcp_stats.rx_rtp_pkt_cnt);
	}
	
fx_exit:
	return ret;
}

#endif
