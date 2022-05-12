/*
   $Id: zynos_misc.c 1.145 2007/07/03 05:31:00 AlexSo Exp $
*/
/******************************************************************************/
/*                                                    
 *  Copyright (C) 2004 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, dIsclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/

/*
  $Log: Phfsm.c $
*/
#include <stdio.h>
#include <stdarg.h>
#if 0
#include <qm.h>
#include <bm.h>
#include <bosInit.h>
#include <bosTask.h>
#include <bosTime.h>
#include <bosSem.h>
#include <bosSleep.h>
#include <event.h>
#include <call_mgnt.h>
#endif
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/if.h>
#include <global.h>
#include <sys/times.h>

static tm_id_t tmIdIndex=0;
#define MAX_IFACE_NUM 20
#define MAX_IFNAME_LENGTH 32
typedef struct iface_ipv4_address{
	char ifName[32];
	struct in_addr ipaddr;	
}iface_ipv4_address_t;
static iface_ipv4_address_t iface_ipv4_address_list[MAX_IFACE_NUM]={0};

void dbg_pline_1(
	const char *s
) {
	ZyPrintf(s);
	fflush(stdout);
}

void dbg_plineb_1(
	char *s,
	unsigned char var
)
{
	ZyPrintf(s);
	ZyPrintf("%c\n", var);
}

void dbg_plinel_1(
	char *s,
	unsigned long var
)
{
	ZyPrintf(s);
	ZyPrintf("0x%x\n", var);
}

int dbg_printf(
	char *fmt, ...
) {
	va_list argptr;
	char temp_buf[256];
	int ret;

	va_start(argptr, fmt);
	ret=vsprintf(temp_buf, fmt, argptr);
	ZyPrintf(temp_buf);
	va_end(argptr);
	return(ret);
}


void DBG_pline_1(
	const char *s
) {
	dbg_pline_1(s);
}

void DBG_plineb_1(
	char *s,
	unsigned char var
)
{
	dbg_plineb_1(s, var);
}

void DBG_plinel_1(
	char *s,
	unsigned long var
)
{
	dbg_plinel_1(s, var);
}

void sysreset(
	void
) {
	ZyPrintf("sysreset, not implement yet\n");
}

tm_id_t
timerStartS(
	uint32	tm_out,			/* time out value, unit is ms	*/
	int	(*func)(tm_id_t,void *),/* timer call back function	*/
	void	*tm_data_p
)
{
	tm_id_t id=++tmIdIndex;
	int res;
	
	res =UIAddTimer(id, tm_out, func, tm_data_p);
	if (res < 0) {
		ZyPrintf("Error: Can't start timer\n");
		tmIdIndex--;
		id = res;
	}
	return id;
} /* timerHiResStart */

int
timerStop(
	int timer_id
)
{
	int res;
	
	res =UIDelTimer(timer_id);
	if (res < 0) {
		ZyPrintf("Error: Can't stop timer\n");
	}
	return res;
}

tm_id_t
timerPeriodicStartS(
	uint32	tm_out,			/* time out value, unit is ms	*/
	int	(*func)(tm_id_t,void *),/* timer call backk function	*/
	void	*tm_data_p		/* timer data pointer		*/
)
{
	tm_id_t id=++tmIdIndex;
	int res;
	
	res =UIAddPeriodicTimer(id, tm_out, func, tm_data_p);
	if (res < 0) {
		ZyPrintf("Error: Can't start periodic timer\n");
		tmIdIndex--;
		id = res;
	}
	return id;
}

int
timerPeriodicStop(
	int timer_id
)
{
	int res;
	
	res =UIDelPeriodicTimer(timer_id);
	if (res < 0) {
		ZyPrintf("Error: Can't stop timer\n");
	}
	return res;
}

unsigned int hwTick(
	void
) {
	struct tms time;

	return times( &time );
}

unsigned int msclock(
	void
) {
	ZyPrintf("msclock, not implement yet\n");
	return 0;
}

int dirps(
	void
) {
	/* not implement yet */
	return 0;
}

void restore(
	int x
) {
	/* not implement yet */
}

void clear_iface_ipv4_address_list(void){
	memset(iface_ipv4_address_list, 0, sizeof(iface_ipv4_address_t)*MAX_IFACE_NUM);
}
/*
*****************************************************************************
** FUNCTION:   get_iface_ipv4_address
**
** PURPOSE:    Extracts IP-Address of an ATM or Ethernet interface.
**
** PARAMETERS: ifname      [IN]  - name of the interface
**             	     ipaddr      [OUT] - pointer to the ip address of the interface
**
** RETURNS:    0 if successful,
**
** NOTE:
*****************************************************************************
*/
int get_iface_ipv4_address(char *ifname, struct in_addr *ipaddr)
{
	int socketfd;
	struct ifreq ifrequest;
	int i=0;

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		ZyPrintf("Error openning socket when getting IP address info\n");
		//return -1;
		goto fail;
	}

	zyStrncpy(ifrequest.ifr_name, ifname, min(IFNAMSIZ, zyStrlen(ifname)+1));
	if (ioctl(socketfd,SIOCGIFADDR,&ifrequest) < 0) {
		close(socketfd);
		//return -1;
		ZyPrintf("Error ioctl socket SIOCGIFADDR\n");
		goto fail;
	}
	zyMemcpy(ipaddr, &(((struct sockaddr_in *)&(ifrequest.ifr_addr))->sin_addr), sizeof(struct in_addr));
	for(i=0;i<MAX_IFACE_NUM;i++){
		if( (strlen(iface_ipv4_address_list[i].ifName)==0) || (strcmp(iface_ipv4_address_list[i].ifName, ifname)==0)){
			/*add to iface_ipv6_address_list*/
			zyStrcpy(&(iface_ipv4_address_list[i].ifName), ifname);
			zyMemcpy(&(iface_ipv4_address_list[i].ipaddr), ipaddr, sizeof(struct in_addr));
			//ZyPrintf("ifname %s address add/set to iface_ipv4_address_list %d\n", ifname, i);
			break;
		}
	}

	close(socketfd);
	return 0;
	
fail:
	/*find interface list*/
	if(strlen(ifname)){
		for(i=0;i<MAX_IFACE_NUM;i++){
			if(strcmp(iface_ipv4_address_list[i].ifName, ifname)==0){
				/*add to iface_ipv6_address_list*/
				zyMemcpy(ipaddr,&(iface_ipv4_address_list[i].ipaddr), sizeof(struct in_addr));
				//ZyPrintf("ifname %s found\n", ifname);
				return 0;
			}
		}
	}

	ZyPrintf("can't find ifName in iface_ipv4_address_list");
	return -1;
	
}

/*
*****************************************************************************
** FUNCTION:   get_iface_ipv4_netmask
**
** PURPOSE:    Extracts IP-Netmask of an ATM or Ethernet interface.
**
** PARAMETERS: ifname      [IN]  - name of the interface
**             	     ipmask      [OUT] - pointer to the ip netmask of the interface
**
** RETURNS:    0 if successful,
**
** NOTE:
*****************************************************************************
*/
int get_iface_ipv4_netmask(char *ifname, struct in_addr *ipmask)
{
	int socketfd;
	struct ifreq ifrequest;

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		ZyPrintf("Error openning socket when getting IP address info\n");
		return -1;
	}

	zyStrncpy(ifrequest.ifr_name, ifname, min(IFNAMSIZ, zyStrlen(ifname)+1));
	if (ioctl(socketfd,SIOCGIFNETMASK,&ifrequest) < 0) {
		close(socketfd);
		return -1;
	}
	*ipmask = ((struct sockaddr_in *)&(ifrequest.ifr_addr))->sin_addr;

	close(socketfd);
	return 0;
}


