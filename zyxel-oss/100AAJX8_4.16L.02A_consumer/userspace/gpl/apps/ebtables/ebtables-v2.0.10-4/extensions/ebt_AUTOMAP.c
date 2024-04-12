/* 
 * Description: EBTables auto priority mapping module for userspace.
 *  Authors:  Jeff Liu <Jeff.Liu@mitrastar.com.tw>
 *           The following is the original disclaimer.
 *
 * Shared library add-on to ebtables for AUTOMAP
 *
 * (C) 2011 by Jeff Liu <Jeff.Liu@mitrastar.com.tw>
 *
 * This program is distributed under the terms of GNU GPL v2, 1991
 *
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>

#include "../include/ebtables_u.h"
#include "../include/linux/netfilter_bridge/ebt_automap_t.h"

static int automapType_supplied;

#define AUTOMAP_TYPE  '1'

static struct option opts[] =
{
	{ "automap-type" , required_argument, 0, AUTOMAP_TYPE },
	{ 0 }
};

static void print_help()
{
	printf(
	"AUTOMAP target options:\n"
	" --automap-type :\n"
	"			 Auto priority mapping by defined type(1:802.1P 2:DSCP 4:IP Length)\n");
}

static void init(struct ebt_entry_target *target)
{
	struct ebt_automap_t_info *automapinfo =
	   (struct ebt_automap_t_info *)target->data;

	automapinfo->type = AUTOMAP_TYPE_PKTLEN;
	automapinfo->marktable[0]=0x0;   /* Queue priority 0*/
	automapinfo->marktable[1]=0x1;   /* Queue priority 1*/
	automapinfo->marktable[2]=0x2;   /* Queue priority 2*/
	automapinfo->marktable[3]=0x3;   /* Queue priority 3*/
	automapinfo->marktable[4]=0x4;   /* Queue priority 4*/
	automapinfo->marktable[5]=0x5;   /* Queue priority 5*/
	automapinfo->marktable[6]=0x6;   /* Queue priority 6*/
	automapinfo->marktable[6]=0x7;   /* Queue priority 7*/
	automapType_supplied = 0;
}

static int
parse(int c, char **argv, int argc,
   const struct ebt_u_entry *entry, unsigned int *flags,
   struct ebt_entry_target **target)
{
	struct ebt_automap_t_info *automapinfo =
	   (struct ebt_automap_t_info *)(*target)->data;
	int type;

	switch (c) {
	case AUTOMAP_TYPE:
		ebt_check_option2(flags, AUTOMAP_TYPE);
		type = strtol(argv[optind - 1], NULL, 0);
		if((type!=AUTOMAP_TYPE_DSCP)&&
				(type!=AUTOMAP_TYPE_8021P)&&
				(type!=AUTOMAP_TYPE_PKTLEN)){
			ebt_print_error2("Invalid mapping type (1:802.1P 2:DSCP 4:IP Length)");
		}
		automapinfo->type = type;
		automapType_supplied = 1;
		break;
	 default:
		return 0;
	}
	return 1;
}

static void
final_check(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target, const char *name,
   unsigned int hookmask, unsigned int time)
{
	if (time == 0 && automapType_supplied == 0)
		printf("No automap type supplied");
}


/* Prints out the targinfo. */
static void 
print(const struct ebt_u_entry *entry,
   const struct ebt_entry_target *target)
{
	const struct ebt_automap_t_info *automapinfo = (const struct ebt_automap_t_info*)target->data;
	if(automapinfo->type == AUTOMAP_TYPE_DSCP)
		printf("automap type is DSCP");
	else if(automapinfo->type == AUTOMAP_TYPE_8021P)
		printf("automap type is 8021P");
	else if(automapinfo->type == AUTOMAP_TYPE_PKTLEN)
		printf("automap type is PKTLEN");
}

static int 
compare(const struct ebt_entry_target *t1,
  	 const struct ebt_entry_target *t2)
{
	struct ebt_automap_t_info *automapinfo1 =
	   (struct ebt_automap_t_info *)t1->data;
	struct ebt_automap_t_info *automapinfo2 =
	   (struct ebt_automap_t_info *)t2->data;

	return automapinfo1->type == automapinfo2->type;
}

static
struct  ebt_u_target automap_target = 
{
    .name          =EBT_AUTOMAP_TARGET,
    .size          =sizeof(struct ebt_automap_t_info),
    .help          =print_help,
    .init          =init,
    .parse         =parse,
    .final_check	=final_check,
    .print		=print,
    .compare	= compare,
    .extra_ops	= opts,
};

//static void _init(void) __attribute__ ((constructor));
//static void _init(void)
void _init(void)
{
	ebt_register_target(&automap_target);
}
