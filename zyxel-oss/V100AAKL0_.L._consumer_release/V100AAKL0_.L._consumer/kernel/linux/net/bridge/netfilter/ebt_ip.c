/*
 *  ebt_ip
 *
 *	Authors:
 *	Bart De Schuymer <bdschuym@pandora.be>
 *
 *  April, 2002
 *
 *  Changes:
 *    added ip-sport and ip-dport
 *    Innominate Security Technologies AG <mhopf@innominate.com>
 *    September, 2002
 */
#include <linux/ip.h>
#include <net/ip.h>
#include <linux/in.h>
#include <linux/module.h>
#include <linux/netfilter/x_tables.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_ip.h>
#if 1 //__MSTC__, FuChia, QoS
#include <linux/tcp.h>
#include <linux/udp.h>
#endif //__MSTC__, FuChia, QoS
struct tcpudphdr {
	__be16 src;
	__be16 dst;
};

static bool
ebt_ip_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
	const struct ebt_ip_info *info = par->matchinfo;
	const struct iphdr *ih;
	struct iphdr _iph;
	const struct tcpudphdr *pptr;
	struct tcpudphdr _ports;
#if 1 //__MSTC__, FuChia, QoS
	struct tcphdr _tcph, *th;
#endif //__MSTC__, FuChia, QoS

	ih = skb_header_pointer(skb, 0, sizeof(_iph), &_iph);
	if (ih == NULL)
		return false;
	if (info->bitmask & EBT_IP_TOS &&
	   FWINV(info->tos != ih->tos, EBT_IP_TOS))
		return false;
   /* brcm */
	if (info->bitmask & EBT_IP_DSCP &&
	   FWINV(info->dscp != (ih->tos & 0xFC), EBT_IP_DSCP))
		return false;
	if (info->bitmask & EBT_IP_SOURCE &&
	   FWINV((ih->saddr & info->smsk) !=
	   info->saddr, EBT_IP_SOURCE))
		return false;
	if ((info->bitmask & EBT_IP_DEST) &&
	   FWINV((ih->daddr & info->dmsk) !=
	   info->daddr, EBT_IP_DEST))
		return false;
#if 1 //__MSTC__, FuChia, QoS
	if(info->bitmask & EBT_IP_LENGTH) { /* IP Length */
		u16 len = ntohs(ih->tot_len);
			if (FWINV(len < info->length[0] ||
					  len > info->length[1],
					  EBT_IP_LENGTH))
		return false;
	}
#endif //__MSTC__, FuChia, QoS
	if (info->bitmask & EBT_IP_PROTO) {
		if (FWINV(info->protocol != ih->protocol, EBT_IP_PROTO))
			return false;
#if 0 //__MSTC__, FuChia, QoS
		if (!(info->bitmask & EBT_IP_DPORT) &&
		    !(info->bitmask & EBT_IP_SPORT))
#else
		if (!(info->bitmask & EBT_IP_DPORT) &&
		    !(info->bitmask & EBT_IP_SPORT) &&
		    !(info->bitmask & EBT_IP_TCP_FLAGS))
#endif //__MSTC__, FuChia, QoS
			return true;
		if (ntohs(ih->frag_off) & IP_OFFSET)
			return false;
		pptr = skb_header_pointer(skb, ih->ihl*4,
					  sizeof(_ports), &_ports);
		if (pptr == NULL)
			return false;
		if (info->bitmask & EBT_IP_DPORT) {
			u32 dst = ntohs(pptr->dst);
			if (FWINV(dst < info->dport[0] ||
				  dst > info->dport[1],
				  EBT_IP_DPORT))
			return false;
		}
		if (info->bitmask & EBT_IP_SPORT) {
			u32 src = ntohs(pptr->src);
			if (FWINV(src < info->sport[0] ||
				  src > info->sport[1],
				  EBT_IP_SPORT))
			return false;
		}
#if 1 //__MSTC__, FuChia, QoS
                if (info->bitmask & EBT_IP_TCP_FLAGS) {
			th = skb_header_pointer(skb, ih->ihl*4, sizeof(_tcph), &_tcph);
			if (th == NULL) {
				/* We've been asked to examine this packet, and we
				   can't.  Hence, no choice but to drop. */
				printk("Dropping evil TCP offset=0 tinygram.\n");
				return 0;
			}
			if (FWINV((((unsigned char *)th)[13] & info->tcp_flg_mask) != info->tcp_flg_cmp, EBT_IP_TCP_FLAGS))
				return false;
		}
#endif //__MSTC__, FuChia, QoS
	}
	return true;
}

static bool ebt_ip_mt_check(const struct xt_mtchk_param *par)
{
	const struct ebt_ip_info *info = par->matchinfo;
	const struct ebt_entry *e = par->entryinfo;

	if (e->ethproto != htons(ETH_P_IP) ||
	   e->invflags & EBT_IPROTO)
		return false;
	if (info->bitmask & ~EBT_IP_MASK || info->invflags & ~EBT_IP_MASK)
		return false;
	if (info->bitmask & (EBT_IP_DPORT | EBT_IP_SPORT)) {
		if (info->invflags & EBT_IP_PROTO)
			return false;
		if (info->protocol != IPPROTO_TCP &&
		    info->protocol != IPPROTO_UDP &&
		    info->protocol != IPPROTO_UDPLITE &&
		    info->protocol != IPPROTO_SCTP &&
		    info->protocol != IPPROTO_DCCP)
			 return false;
	}
	if (info->bitmask & EBT_IP_DPORT && info->dport[0] > info->dport[1])
		return false;
	if (info->bitmask & EBT_IP_SPORT && info->sport[0] > info->sport[1])
		return false;
#if 1 //__MSTC__, FuChia, QoS
        if (info->bitmask & EBT_IP_LENGTH && info->length[0] > info->length[1])
		return false;
#endif //__MSTC__, FuChia, QoS
	return true;
}

static struct xt_match ebt_ip_mt_reg __read_mostly = {
	.name		= "ip",
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_ip_mt,
	.checkentry	= ebt_ip_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_ip_info)),
	.me		= THIS_MODULE,
};

static int __init ebt_ip_init(void)
{
	return xt_register_match(&ebt_ip_mt_reg);
}

static void __exit ebt_ip_fini(void)
{
	xt_unregister_match(&ebt_ip_mt_reg);
}

module_init(ebt_ip_init);
module_exit(ebt_ip_fini);
MODULE_DESCRIPTION("Ebtables: IPv4 protocol packet match");
MODULE_LICENSE("GPL");
