#ifndef __LINUX_BRIDGE_EBT_MARK_M_H
#define __LINUX_BRIDGE_EBT_MARK_M_H

#include <linux/types.h>

#define EBT_MARK_AND 0x01
#define EBT_MARK_OR 0x02
#ifdef CONFIG_MSTC_MODIFY //__MSTC__, FuChia,
#define EBT_MARK_CLASSID_CMP 0x04
#endif
#define EBT_MARK_MASK (EBT_MARK_AND | EBT_MARK_OR)
struct ebt_mark_m_info {
	unsigned long mark, mask;
	__u8 invert;
	__u8 bitmask;
};
#define EBT_MARK_MATCH "mark_m"

#endif
