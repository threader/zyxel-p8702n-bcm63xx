/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Laguna Canyon Road
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: net.h
*    Creation Date: 18 Aug 2000 (v0.00)
*    VSS Info:
*        $Revision: 1.0 $
*        $Date: 2001-10-25 15:00:55-07 $
*
****************************************************************************
*    Description:
*
*     This header file contains prototypes for NET utililty functions
*
****************************************************************************/

#ifndef _BCMNET_H
#define _BCMNET_H

#include <env.h>

#ifdef LINUX
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#endif /* LINUX */

#if defined(__cplusplus)
extern "C"
{
#endif


/*
*****************************************************************************
** TYPEDEFS AND CONSTANTS
*****************************************************************************
*/

typedef  UINT32   IPADDR;     /* IP4 Ip Address data type */

#define  SOCKET_ERROR  (-1)
#define  NET_LINKUP     6
#define  NET_LINKDOWN   1


/*
*****************************************************************************
** FUNCTION:   bcmNetNumToIpAddr
**
** PURPOSE:    Converts IPADDR presented IP address to a Character string
**
** PARAMETERS: ipaddr - IPADDR IP address
**             buf    - string containing IP Address (output)
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************
*/
void bcmNetNumToIpAddr( IPADDR ipaddr, char *buf );


/*
*****************************************************************************
** FUNCTION:   bcmNetIpAddrToNum
**
** PURPOSE:    Converts Character presented IP address or FQDN to IPADDR number
**
** PARAMETERS: string - character string with an IP address/FQDN
**
** RETURNS:    IPADDR IP address or 0 if error
**
** NOTE:       if IP address is FQDN then function will try to resolve this
**             it via DNS (if active).
*****************************************************************************
*/
IPADDR bcmNetIpAddrToNum( const char *string );

/*
****************************************************************************
** FUNCTION:   bcmNetGetHostAddrs
**
** PURPOSE:    Given the hostname, this function finds corresponding IP
**             addresses and returns them.
**
** PARAMETERS: hostName [IN]        - host name to resolve into IP addresses
**             ipAddr   [OUT]       - array of resolved IP addresses (in
**                                     host byte order). Memory must be
**                                     allocated by caller.
**             numIpAddrs [IN/OUT]  - On input, points to the array length
**                                     of 'ipAddr'. On output, returns the
**                                     number of resolved IP addresses contained
**                                     in the 'ipAddr' array.
**
** RETURNS:    SUCCESS or FAILURE
**
** NOTE:       The 'hostName' may be either a fully qualified domain name or
**             a dotted decimal notation IP address string.
**
*****************************************************************************
*/
int bcmNetGetHostAddrs( char *hostName, IPADDR *ipAddr, int *numIpAddrs );


/*
*****************************************************************************
** FUNCTION:   bcmNetTakeServerPort
**
** PURPOSE:    Extracts the Port Number part from the IP-Address string.
**             IP-Address string should be like <IP-str>:<port_num>
**
** PARAMETERS: ipAddrStr (I)  - IP Addr string (or FQDN)
**
** RETURNS:    Port Number or 0 if an error or not given.
**
** NOTE:
*****************************************************************************
*/
UINT32 bcmNetTakeServerPort(const char* ipAddrStr);


#ifdef LINUX
int bcmGetIpAddress(char *ifname, struct in_addr *ipaddr, struct in_addr *subnetmask);
int bcmIsLinkUp( char *ifname );
#endif

#if defined(__cplusplus)
}
#endif

#endif /* _BCMNET_H */

