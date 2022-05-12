#if defined(CONFIG_BCM_KF_NETFILTER)

/*
* <:copyright-BRCM:2012:DUAL/GPL:standard
* 
*    Copyright (c) 2012 Broadcom Corporation
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
:>
*/

#ifndef _NF_CONNTRACK_RTSP_H
#define _NF_CONNTRACK_RTSP_H

#ifdef __KERNEL__
#ifdef CONFIG_MSTC_MODIFY
#define FIX_ACK_ERROR_ISSUE 1
#endif
/* This structure exists only once per master */
struct nf_ct_rtsp_master {
	/* The client has sent PAUSE message and not replied */
	int paused;
	int teardown;
};

/* Single data channel */
extern int (*nat_rtsp_channel_hook) (struct sk_buff *skb,
				     struct nf_conn *ct,
				     enum ip_conntrack_info ctinfo,
				     unsigned int matchoff,
				     unsigned int matchlen,
				     struct nf_conntrack_expect *exp,
				     int *delta);

/* A pair of data channels (RTP/RTCP) */
extern int (*nat_rtsp_channel2_hook) (struct sk_buff *skb,
				      struct nf_conn *ct,
				      enum ip_conntrack_info ctinfo,
				      unsigned int matchoff,
				      unsigned int matchlen,
				      struct nf_conntrack_expect *rtp_exp,
				      struct nf_conntrack_expect *rtcp_exp,
				      char dash, int *delta);

#ifdef CONFIG_MSTC_TELENOR_NORWAY_PORT_CHANGE
/* A pair of data channels (RTP/RTCP/retrasm) */
extern int (*nat_rtsp_channel3_hook) (struct sk_buff *skb,
				      struct nf_conn *ct,
				      enum ip_conntrack_info ctinfo,
				      unsigned int matchoff,
				      unsigned int matchlen,
				      struct nf_conntrack_expect *rtp_exp,
				      struct nf_conntrack_expect *rtcp_exp,
					  struct nf_conntrack_expect *rtcp2_exp,
				      struct nf_conntrack_expect *rertp_exp,
					  struct nf_conntrack_expect *rertp2_exp,
				      char dash, int *delta);
#endif

/* Modify parameters like client_port in Transport for single data channel */
extern int (*nat_rtsp_modify_port_hook) (struct sk_buff *skb,
					 struct nf_conn *ct,
			      	  	 enum ip_conntrack_info ctinfo,
			      	  	 unsigned int matchoff,
					 unsigned int matchlen,
			      	  	 __be16 rtpport, int *delta);

/* Modify parameters like client_port in Transport for multiple data channels*/
extern int (*nat_rtsp_modify_port2_hook) (struct sk_buff *skb,
					  struct nf_conn *ct,
			       	   	  enum ip_conntrack_info ctinfo,
			       	   	  unsigned int matchoff,
					  unsigned int matchlen,
			       	   	  __be16 rtpport, __be16 rtcpport,
				   	  char dash, int *delta);


#ifdef CONFIG_MSTC_TELENOR_NORWAY_PORT_CHANGE
/* Modify parameters like client_port in Transport for multiple data channels*/
extern int (*nat_rtsp_modify_port3_hook) (struct sk_buff *skb,
					  struct nf_conn *ct,
			       	   	  enum ip_conntrack_info ctinfo,
			       	   	  unsigned int matchoff,
					  unsigned int matchlen,
			       	   	  __be16 rtpport, __be16 rtcpport,
				   	  char dash, int *delta);
#endif
#if FIX_ACK_ERROR_ISSUE
extern void (*clean_ipport_buf_hook) (void);
#endif

/* Modify parameters like destination in Transport */
extern int (*nat_rtsp_modify_addr_hook) (struct sk_buff *skb,
					 struct nf_conn *ct,
				 	 enum ip_conntrack_info ctinfo,
					 int matchoff, int matchlen,
					 int *delta);


#if FIX_ACK_ERROR_ISSUE
extern int (*modify_setup_mesg_hook) (struct sk_buff *skb, struct nf_conn *ct,
			 	  enum ip_conntrack_info ctinfo, const char *tpdate, int matchoff, int matchlen,
			 	  int *delta);

extern int (*modify_reply_mesg_hook) (struct sk_buff *skb, struct nf_conn *ct,
			 	  enum ip_conntrack_info ctinfo, const char *tpdate, int matchoff, int matchlen,
			 	  int *delta);
#endif

#endif /* __KERNEL__ */

#endif /* _NF_CONNTRACK_RTSP_H */

#endif
