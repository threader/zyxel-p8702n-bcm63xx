/*
 *	xt_mark - Netfilter module to match NFMARK value
 *
 *	(C) 1999-2001 Marc Boucher <marc@mbsi.ca>
 *	Copyright © CC Computer Consultants GmbH, 2007 - 2008
 *	Jan Engelhardt <jengelh@medozas.de>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/xt_mark.h>
#include <linux/netfilter/x_tables.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Marc Boucher <marc@mbsi.ca>");
MODULE_DESCRIPTION("Xtables: packet mark operations");
MODULE_ALIAS("ipt_mark");
MODULE_ALIAS("ip6t_mark");
MODULE_ALIAS("ipt_MARK");
MODULE_ALIAS("ip6t_MARK");

#ifdef CONFIG_MSTC_MODIFY /*support MARK v0, v1 for old 1.3.8 iptables target compatible*/
static unsigned int
target_v0(struct sk_buff *skb, const struct xt_action_param *par)
{
    	const struct xt_mark_target_info *markinfo = par->targinfo;

	skb->mark = markinfo->mark;
	return XT_CONTINUE;
}

static unsigned int
target_v1(struct sk_buff *skb, const struct xt_action_param *par)
{
    	const struct xt_mark_target_info_v1 *markinfo = par->targinfo;
    	//printk("%s: %8x %8x\n", __FUNCTION__, markinfo->mode, markinfo->mark);

	int mark = 0;
	switch (markinfo->mode) {
	case XT_MARK_SET:
		mark = markinfo->mark;
		break;

	case XT_MARK_AND:
		mark = skb->mark & markinfo->mark;
		break;

	case XT_MARK_OR:
		mark = skb->mark | markinfo->mark;
		break;

#ifdef CONFIG_MSTC_MODIFY //__MSTC__, ZyXEL richard, QoS
	// for iptable vtag_set command
	case XT_MARK_VTAG_SET:
		mark = skb->mark;
		skb->vtag = (unsigned short)(markinfo->mark);
		break;
#endif //__MSTC__, ZyXEL richard, QoS
        
	}

	skb->mark = mark;
	return XT_CONTINUE;
}

static int checkentry_v0(const struct xt_tgchk_param *par)
{
	const struct xt_mark_target_info *markinfo = par->targinfo;

	if (markinfo->mark > 0xffffffff) {
		printk(KERN_WARNING "MARK: Only supports 32bit wide mark\n");
		return -EINVAL;
	}
	return 0;

}
static int checkentry_v1(const struct xt_tgchk_param *par)
{
    	const struct xt_mark_target_info_v1 *markinfo = par->targinfo;
    	//printk("%s: %8x %8x\n", __FUNCTION__, markinfo->mode, markinfo->mark);

	if (markinfo->mode != XT_MARK_SET
	    && markinfo->mode != XT_MARK_AND
#ifdef CONFIG_MSTC_MODIFY //__MSTC__, ZyXEL richard, QoS
	    && markinfo->mode != XT_MARK_VTAG_SET
#endif //__MSTC__, ZyXEL richard, QoS
	    && markinfo->mode != XT_MARK_OR) {
		printk(KERN_WARNING "MARK: unknown mode %u\n",
		       markinfo->mode);
		return -EINVAL;
	}
	if (markinfo->mark > 0xffffffff) {
		printk(KERN_WARNING "MARK: Only supports 32bit wide mark\n");
		return -EINVAL;
	}
	return 0;

}


#ifdef CONFIG_COMPAT
struct compat_xt_mark_target_info_v1 {
	compat_ulong_t	mark;
	u_int8_t	mode;
	u_int8_t	__pad1;
	u_int16_t	__pad2;
};

static void compat_from_user_v1(void *dst, void *src)
{
	struct compat_xt_mark_target_info_v1 *cm = src;
	struct xt_mark_target_info_v1 m = {
		.mark	= cm->mark,
		.mode	= cm->mode,
	};
	memcpy(dst, &m, sizeof(m));
}

static int compat_to_user_v1(void __user *dst, void *src)
{
	struct xt_mark_target_info_v1 *m = src;
	struct compat_xt_mark_target_info_v1 cm = {
		.mark	= m->mark,
		.mode	= m->mode,
	};
	return copy_to_user(dst, &cm, sizeof(cm)) ? -EFAULT : 0;
}
#endif /* CONFIG_COMPAT */
#endif

static unsigned int
mark_tg(struct sk_buff *skb, const struct xt_action_param *par)
{
	const struct xt_mark_tginfo2 *info = par->targinfo;
    
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG_FEATURE)
	skb->ipt_check |= IPT_TARGET_MARK;
	skb->ipt_log.u32[BLOG_ORIGINAL_MARK_INDEX] = skb->mark;
	skb->ipt_log.u32[BLOG_TARGET_MARK_INDEX] = (skb->mark & ~info->mask) ^
	   info->mark;
	if ( skb->ipt_check & IPT_TARGET_CHECK )
		return XT_CONTINUE;
#endif

	skb->mark = (skb->mark & ~info->mask) ^ info->mark;
	return XT_CONTINUE;
}

static bool
mark_mt(const struct sk_buff *skb, struct xt_action_param *par)
{
	const struct xt_mark_mtinfo1 *info = par->matchinfo;

	return ((skb->mark & info->mask) == info->mark) ^ info->invert;
}

#ifdef CONFIG_MSTC_MODIFY/*support MARK v0, v1 for old 1.3.8 iptables target compatible*/
static struct xt_target mark_tg_regs[] __read_mostly = {
	{
		.name		= "MARK",
		.family		= NFPROTO_UNSPEC,
		.revision	= 0,
		.checkentry	= checkentry_v0,
		.target		= target_v0,
		.targetsize	= sizeof(struct xt_mark_target_info),
		.table		= "mangle",
		.me		= THIS_MODULE,
	},
	{
		.name		= "MARK",
		.family		= NFPROTO_UNSPEC,
		.revision	= 1,
		.checkentry	= checkentry_v1,
		.target		= target_v1,
		.targetsize	= sizeof(struct xt_mark_target_info_v1),
#ifdef CONFIG_COMPAT
		.compatsize	= sizeof(struct compat_xt_mark_target_info_v1),
		.compat_from_user = compat_from_user_v1,
		.compat_to_user	= compat_to_user_v1,
#endif
		.table		= "mangle",
		.me		= THIS_MODULE,
	},
	{
		.name           = "MARK",
		.revision       = 2,
		.family         = NFPROTO_UNSPEC,
		.target         = mark_tg,
		.targetsize     = sizeof(struct xt_mark_tginfo2),
		.me             = THIS_MODULE,
	}
};
#else
static struct xt_target mark_tg_reg __read_mostly = {
	.name           = "MARK",
	.revision       = 2,
	.family         = NFPROTO_UNSPEC,
	.target         = mark_tg,
	.targetsize     = sizeof(struct xt_mark_tginfo2),
	.me             = THIS_MODULE,
};
#endif
static struct xt_match mark_mt_reg __read_mostly = {
	.name           = "mark",
	.revision       = 1,
	.family         = NFPROTO_UNSPEC,
	.match          = mark_mt,
	.matchsize      = sizeof(struct xt_mark_mtinfo1),
	.me             = THIS_MODULE,
};

static int __init mark_mt_init(void)
{
	int ret;
#ifdef CONFIG_MSTC_MODIFY /*support MARK v0, v1 for old 1.3.8 iptables target compatible*/
	ret = xt_register_targets(mark_tg_regs, ARRAY_SIZE(mark_tg_regs));
	if (ret < 0)
		return ret;
	ret = xt_register_match(&mark_mt_reg);
	if (ret < 0) {
		xt_unregister_targets(mark_tg_regs, ARRAY_SIZE(mark_tg_regs));
		return ret;
	}
#else
	ret = xt_register_target(&mark_tg_reg);
	if (ret < 0)
		return ret;
	ret = xt_register_match(&mark_mt_reg);
	if (ret < 0) {
		xt_unregister_target(&mark_tg_reg);
		return ret;
	}
#endif    	
	return 0;
}

static void __exit mark_mt_exit(void)
{
	xt_unregister_match(&mark_mt_reg);
#ifdef CONFIG_MSTC_MODIFY /*support MARK v0, v1 for old 1.3.8 iptables target compatible*/
	xt_unregister_targets(mark_tg_regs, ARRAY_SIZE(mark_tg_regs));
#else    	
	xt_unregister_target(&mark_tg_reg);
#endif
}

module_init(mark_mt_init);
module_exit(mark_mt_exit);
