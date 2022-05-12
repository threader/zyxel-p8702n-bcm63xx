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
*    Filename: net.c
*    Creation Date: 18 Aug 2000 (v0.00)
*    VSS Info:
*        $Revision: 1.0 $
*        $Date: 2001-10-26 14:21:44-07 $
*
****************************************************************************
*    Description:
*
*      Network utility functions
*
****************************************************************************/

/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/

#include <env.h>
#include <log.h>

#include <net.h>
#include <string.h>
#include <stdlib.h>
#include <bcmos.h>

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <bcmnet.h>
#include <ctype.h>

/*
*****************************************************************************
** LOCAL FUNCTIONS
*****************************************************************************
*/
static IPADDR  IpAddrNumber( const char *string );

#ifdef LINUX
static int readFirstLine(char *file, char *data, int len);
#endif 


/*
*****************************************************************************
** FUNCTION:   IpAddrNumber()
**
** PURPOSE:    Converts character represented IP address (dotted decimal notation)
**             to a four byte integer. e.g. converts "10.1.0.5" to 0x0a010005.
**
** PARAMETERS: string - Character represented IP adress (dottted decimal notion).
**
** RETURNS:    "IPADDR" IP address or 0 if error
**
** NOTE:
*****************************************************************************
*/
static IPADDR  IpAddrNumber( const char *string )
{
   char     ipstring[256];  /* It might be FQDN with the MAX length = 256 (as per spec) */
   char     *s = ipstring;
   IPADDR   address = 0L;
   int      i, j;
   char     *cp, *b1, *b2, *cl;

   if (!string)
   {
      LOG(("*PANIC* IpAddrNumber() - NULL String as an input parameter!\n"));
      return 0;
   }
   if (!strlen(string))
   {
      LOG(("*PANIC* IpAddrNumber() - EMPTY String as an input parameter!\n"));
      return 0;
   }

   strcpy(ipstring, string);  /* Just make the string possible to manipulate/change with. */

   /*
   ** According to the PacketCable conventions, explicit IP addresses
   ** are presented in the squuare brackets - [xx.xx.xx.xx]
   ** Let's get rid of these (if they are present)
   ** Another convention is that the FQDN can contain port number in it,
   ** like this: <FQDN>:<port_num>. We need to get rid of it as well.
   */
   b1 = strchr( ipstring, '[');
   b2 = strchr( ipstring, ']');
   cl = strchr( ipstring, ':');

   if ( b1 )
   {
      b1++;    /* skip to the next character */
   }

   if (b1  && (b1 >= b2) )
   {
      LOG(("*PANIC* IpAddrNumber() - WRONG Format of an input parameter (%s)!\n", string));
      return 0;
   }

   if (b2  && !b1 )
   {
      LOG(("*PANIC* IpAddrNumber() - WRONG Format of an input parameter (%s)!\n", string));
      return 0;
   }

   if ( cl && (cl < b2) )
   {
      LOG(("*PANIC* IpAddrNumber() - WRONG Format of an input parameter (%s)!\n", string));
      return 0;
   }

   if (cl)
   {
      *cl = 0; /* Cut off everything starting from ':' sign. */
   }

   if (b2)     /* Cut off everything starting from ']' sign. */
   {
      *b2 = 0;
   }

   if (!b1)
   {
      b1 = s;
   }

   for (j=24; j>=0; j-=8)
   {
      cp = b1;

      while(( *b1 ) && ( *b1 != '.' ))
      {

         if ( (*b1 < '0') || (*b1 > '9') )
         {
            address = 0L;
            return(address);
         }

         b1++;
      }

      if( *b1 )
      {
         *b1++ = '\0';
      }

      i = atoi(cp);
      address |= (i &0xff) << j;

      if ( !(*b1) && (j != 0) )
      {
         LOG(("*PANIC* IpAddrNumber() - WRONG Format of an input parameter (%s)!\n", string));
         return 0;
      }

   }

   if ( *b1 )
   {
      LOG(("*PANIC* IpAddrNumber() - WRONG Format of an input parameter (%s)!\n", string));
      return 0;
   }

   return(address);
}


/*
*****************************************************************************
** FUNCTION:   bcmNetNumToIpAddr()
**
** PURPOSE:    Converts IPADDR type (four byte int) to a dotted decimal
**             notation IP address string. e.g. converts 0x0a010005 to
**             "10.1.0.5".
**
** PARAMETERS: ipaddr - IPADDR IP address
**             buf    - string containing IP Address (output)
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************
*/
void bcmNetNumToIpAddr( IPADDR ipaddr, char *buf )
{
   sprintf(buf, "%lu.%lu.%lu.%lu", (ipaddr>>24)&0xff,
                               (ipaddr>>16)&0xff,
                               (ipaddr>>8)&0xff,
                               (ipaddr&0xff));
}


/*
*****************************************************************************
** FUNCTION:   bcmNetIpAddrToNum()
**
** PURPOSE:    Converts character represented IP address (dotted decimal notation)
**             or fully qualified domain name (FQDN) to an IPADDR type (four byte
**             integer).
**
** PARAMETERS: string - character string with IP address/FQDN
**
** RETURNS:    IPADDR IP address or 0 if error
**
** NOTE:       This function will first try to convert the passed in string
**             assuming that it is a dotted decimal notion IP address. If
**             this conversion fails, it will perform a DNS lookup. If this
**             fails, it will then return an error.
**
**             Use of this function is deprecated, and is only provided for
**             backwards compatibility. bcmNetGetHostAddrs should be used instead.
**
*****************************************************************************
*/
IPADDR  bcmNetIpAddrToNum( const char *string )
{
   IPADDR address;
   struct hostent* phostent;
   struct in_addr hostaddr;

   /* Validate input parameter */
   if ( string == NULL )
   {
      return ( 0 );
   }

   /* Check if domain name is in IP dotted notation, e.g. [10.1.100.1] */
   address = IpAddrNumber( string );

   if ( !address )
   {
      /* Domain name not in IP dotted notation, perform DNS lookup */
      phostent = gethostbyname( string );

      if ( phostent != NULL )
      {
         hostaddr = *((struct in_addr*)phostent->h_addr);
         LOG(( "Address for host %s resolved to %s \n", string, inet_ntoa(hostaddr) ));
         address = ntohl( (IPADDR)(hostaddr.s_addr) );
      }
      else
      {
         address = 0;
         LOG(( "ERROR resolving address ""%s"" by DNS Server.", string ));
      }
   }

   return(address);
}


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
int bcmNetGetHostAddrs( char *hostName, IPADDR *ipAddr, int *numIpAddrs )
{
   struct hostent* phostent;
   struct in_addr hostaddr;
   IPADDR address;
   int    status = SUCCESS;
   int    nrResolvedAddrs = 0;
   char **addrlist;

   /* Validate input parameters */
   BCMOS_ASSERT( hostName && ipAddr && numIpAddrs );

   /* Check if domain name is in IP dotted notation, e.g. [10.1.100.1] */
   address = IpAddrNumber( hostName );
   if ( address )
   {
      BCMOS_ASSERT( *numIpAddrs >= 1);

      /* Domain name is in IP dotted notation */
      ipAddr[0] = address;
      *numIpAddrs = 1;
   }
   else
   {
      /* Domain name not in IP dotted notation, perform DNS lookup */
      phostent = gethostbyname( hostName );

      if ( phostent != NULL )
      {
         addrlist = phostent->h_addr_list;
         while (*addrlist) 
         {
            hostaddr = *((struct in_addr*)*addrlist++);
            nrResolvedAddrs++;
            if ( nrResolvedAddrs <= *numIpAddrs )
            {
               LOG(( "Address for host %s resolved to %s \n", hostName, inet_ntoa(hostaddr) ));
               /* nrResolvedAddrs is the number of resolved addresses (starts from 1)
               ** The index for the array of resolved addresses starts from 0 */
               ipAddr[nrResolvedAddrs-1] = ntohl( (IPADDR)(hostaddr.s_addr) );
            }
            else
            {
               break;
            }
         }

         *numIpAddrs = nrResolvedAddrs;
      }
      else
      {
         ipAddr[0] = 0;
         *numIpAddrs = 0;
         LOG(( "ERROR resolving address ""%s"" by DNS Server.", hostName ));
         status = FAILURE;
      }
   }

   return( status );
}


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
UINT32 bcmNetTakeServerPort(const char* ipAddrStr)
{
   char  *p;

   if (!ipAddrStr) /* Sanity check */
   {
      return 0;
   }

   p = strchr(ipAddrStr, ':');
   if (p)
   {
      return atol(++p);
   }

   return 0;

}

#ifdef LINUX
/*
*****************************************************************************
** FUNCTION:   bcmGetIpAddress
**
** PURPOSE:    Extracts IP-Address of an ATM or Ethernet interface.
**
** PARAMETERS: ifname      [IN]  - name of the interface
**             ipaddr      [OUT] - pointer to the ip address of the interface
**             subnetmask  [OUT] - pointer to the subnetmask of the interface
**
** RETURNS:    0 if successful,
**
** NOTE:
*****************************************************************************
*/
int bcmGetIpAddress(char *ifname, struct in_addr *ipaddr, struct in_addr *subnetmask)
{
   int socketfd;
   struct ifreq ifrequest;

   if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
     printf("Error openning socket when getting IP address info\n");
     return -1;
   }

   strcpy(ifrequest.ifr_name,ifname);
   if (ioctl(socketfd,SIOCGIFADDR,&ifrequest) < 0) {
     printf("Error getting IP address\n");
     close(socketfd);
     return -1;
   }
   *ipaddr = ((struct sockaddr_in *)&(ifrequest.ifr_addr))->sin_addr;

   if (ioctl(socketfd,SIOCGIFNETMASK,&ifrequest) < 0) {
     printf("Error getting subnet address\n");
     close(socketfd);
     return -1;
   }

   *subnetmask = ((struct sockaddr_in *)&(ifrequest.ifr_netmask))->sin_addr;

   close(socketfd);
   return 0;
}


/*
*****************************************************************************
** FUNCTION:   bcmIsLinkUp
**
** PURPOSE:    Get the status of a WAN/LAN iterface
**
** PARAMETERS: ifname      [IN]  - name of the interface
**
** RETURNS:    NET_LINKUP if interface is up
**             NET_LINKDOWN if interface is down
**
** NOTE:
*****************************************************************************
*/
int bcmIsLinkUp( char *ifname ) 
{
   int status = 0;
   char device[16], file[256], data[256];

   device[0] = '\0'; file[0] = '\0'; data[0] = '\0';

   /* We will assume that the br0 interface is always up */
   if ( strncmp(ifname, "br0", sizeof("br0")) == 0 )
   {
      return( NET_LINKUP );
   }

   sprintf(file, "/proc/var/fyi/wan/%s/status", ifname);
   readFirstLine(file, data, 256);
   if ( data[0] != '\0' )
      status = atoi(data);

   return status;
}

static int readFirstLine(char *file, char *data, int len) 
{
   int ret = -1;
   FILE* fs = fopen(file, "r");

   if ( fs != NULL ) {
      fgets(data, len, fs);
      fclose(fs);
      if ( data != NULL ) {
         int i = 0;
         // remove trailing space characters
         while ( data[i] != '\0' && isspace(data[i]) == 0 )
            i++;
         data[i] = '\0';
      }
      ret = 0;
   }

   return ret;
}
#endif
