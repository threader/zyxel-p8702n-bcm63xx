/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bart.de.schuymer@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */

#ifndef __LINUX_BRIDGE_EBT_IP_H
#define __LINUX_BRIDGE_EBT_IP_H

#include <linux/types.h>

#define EBT_IP_SOURCE 0x01
#define EBT_IP_DEST 0x02
#define EBT_IP_TOS 0x04
#define EBT_IP_PROTO 0x08
#define EBT_IP_SPORT 0x10
#define EBT_IP_DPORT 0x20
#if defined(CONFIG_BCM_KF_NETFILTER) || !defined(CONFIG_BCM_IN_KERNEL)
#define EBT_IP_DSCP  0x40
#ifdef CONFIG_MSTC_MODIFY //__MSTC__, FuChia, QoS
#define EBT_IP_LENGTH 0x80
#define EBT_IP_TCP_FLAGS 0x100
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT | EBT_IP_DSCP | EBT_IP_LENGTH | EBT_IP_TCP_FLAGS)
#else 
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT | EBT_IP_DSCP )
#endif //__MSTC__, FuChia, QoS
#else 
#define EBT_IP_MASK (EBT_IP_SOURCE | EBT_IP_DEST | EBT_IP_TOS | EBT_IP_PROTO |\
 EBT_IP_SPORT | EBT_IP_DPORT )
#endif
#define EBT_IP_MATCH "ip"

/* the same values are used for the invflags */
#ifdef CONFIG_MSTC_MODIFY //__MSTC__, FuChia, QoS
struct ebt_ip_info
{
	__be32 saddr;
	__be32 daddr;
	__be32 smsk;
	__be32 dmsk;
	uint8_t  tos;
	uint8_t  dscp; /* brcm */
	uint8_t  protocol;
	uint16_t  bitmask;
	uint16_t  invflags;
	uint8_t  tcp_flg_mask;
	uint8_t  tcp_flg_cmp;
	uint16_t sport[2];
	uint16_t dport[2];
	uint16_t length[2];
};
#else
struct ebt_ip_info {
	__be32 saddr;
	__be32 daddr;
	__be32 smsk;
	__be32 dmsk;
	__u8  tos;
#if defined(CONFIG_BCM_KF_NETFILTER) || !defined(CONFIG_BCM_IN_KERNEL)
	__u8  dscp;
#endif
	__u8  protocol;
	__u8  bitmask;
	__u8  invflags;
	__u16 sport[2];
	__u16 dport[2];
};
#endif //__MSTC__, FuChia, QoS

#endif
