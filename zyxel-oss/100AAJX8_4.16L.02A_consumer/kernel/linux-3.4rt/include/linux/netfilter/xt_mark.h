#ifndef _XT_MARK_H
#define _XT_MARK_H

#include <linux/types.h>

#ifdef CONFIG_MSTC_MODIFY
/* Version 0 */
struct xt_mark_target_info {
	unsigned long mark;
};

/* Version 1 */
enum {
	XT_MARK_SET=0,
	XT_MARK_AND,
	XT_MARK_OR,
	XT_MARK,
	XT_MARK_VTAG_SET,
};

struct xt_mark_target_info_v1 {
	unsigned long mark;
	u_int8_t mode;
};
#endif

struct xt_mark_tginfo2 {
	__u32 mark, mask;
};

struct xt_mark_mtinfo1 {
	__u32 mark, mask;
	__u8 invert;
};

#endif /*_XT_MARK_H*/
