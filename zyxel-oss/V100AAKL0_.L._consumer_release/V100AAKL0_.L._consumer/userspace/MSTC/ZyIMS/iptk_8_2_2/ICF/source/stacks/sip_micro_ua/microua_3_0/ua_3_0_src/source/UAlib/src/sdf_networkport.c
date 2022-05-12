/*****************************************************************************
 ** FUNCTION		: This file has all the network related funtions
 **
 *****************************************************************************
 **
 ** FILENAME		: sdf_network.c
 **
 ** DESCRIPTION		: This file has all the network related funtions
 **
 ** DATE		NAME		REFERENCE		REASON
 ** ----		----		---------		--------
 ** 27-Oct-00	Subhash						Creation
 ** 19-Jul-02	Siddharth					Added transaction layer 
 **											integration.
 ** 22/04/03    Anil Kumar     ---          Changes for TLS feature.
 ** 29/01/09   Alok Tiwari  ICF Rel-8.1     Changes done for 
 **                               flag ICF_UATK_PORT.This flag filters
 **                               the ICF related code/changes from this file. 
 ** 12-Jun-09  Anuradha Gupta SPR 19737      Klocworks warning removal
 *****************************************************************************
 **				COPYRIGHT , Aricent, 2009
 *****************************************************************************/


#include "ALheader.h"
#include "sdf_struct.h"
#include "sdf.h"
#include "sdf_network.h"
#include "sdf_accessor.h"
#include "sdf_init.h"
#ifndef SDF_LINT
#include "sdf_free.h"
#include "sdf_list.h"
#endif
#include "sdf_internal.h"
#include "sdf_sdpinternal.h"
#include "sdf_reqresp.h"
#include "sdf_debug.h"
#include "sdf_trace.h"
#include "sdf_hash.h"
#include "sdf_portlayer.h"
#include "sdf_callbacks.h"
#include "sdf_sdp.h"
#include "sdf_sdpaccessor.h"
#include "sdf_sdpfree.h"

#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif

#ifdef ICF_UATK_PORT
#ifdef ICF_PORT_WINDOWS
#include<ws2tcpip.h>
#include<winsock2.h>

#if defined(ICF_IPV6_ENABLED) || defined(SDF_IPV6)
#include <tpipv6.h>
#endif
#endif/*ICF_PORT_WINDOWS*/

#ifdef ICF_PORT_SYMBIAN
#include<netinet/in.h>
#include<sys/socket.h>
#endif
#endif /****ICF_UATK_PORT******/

#ifndef SDF_NETWORKCALLBACKS
extern Sdf_st_fdSetInfo	dGlbFdSet;
#endif

/* klocworks : checking for memeory allocation failure has been added and 
 * on NULL check for pointer , failure is returned from function
 */ 

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaInitSocket
 **
 ** DESCRIPTION: This function initializes a socket. It returns 
 **				 a sockfd with the initialized socket.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaInitSocket
#ifdef ANSI_PROTO
	(Sdf_st_socketProperties *pSocketProperties,
	 Sdf_ty_s32bit *pSockfd, 
	 Sdf_st_error *pErr)
#else
	(pSocketProperties,pSockfd,pErr)
	 Sdf_st_socketProperties *pSocketProperties;
	 Sdf_ty_s32bit *pSockfd;
	 Sdf_st_error  *pErr;
#endif
{
#ifdef ICF_UATK_PORT
#ifndef ICF_PORT_SYMBIAN
	Sdf_ty_socketId tempSocket = Sdf_co_InvalidSocketFd;
	Sdf_st_socket   dSocket;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_s32bit dDomain, dType;

	Sdf_ty_u32bit reuseOptInt = 1;
	Sdf_ty_bool dIsIpv6=Sdf_co_false;

#ifdef SDF_IPV6	
    struct sockaddr_in6 servAddr6;
#endif	
	
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#ifdef SDF_IPV6	
#ifdef SDF_LINT
	struct addrinfo *pAddrInfo={0};
#else
	struct addrinfo *pAddrInfo={0};
#endif
	struct addrinfo	Hints;
	Sdf_ty_s32bit	RetVal;
	Sdf_ty_s8bit  dPort[Sdf_co_characterBufferSize];
#endif	
#else
#ifdef SDF_IPV6	
    struct ipv6_mreq    multicastMreq6;
#endif	
#endif
	struct sockaddr_in 	servAddr;
	struct ip_mreq 		multicastMreq;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaInitSocket\n");

	if(Sdf_mc_strcasecmp(pSocketProperties->pProtocol,"UDP")==0)
		dSocket.dProtocol = Sdf_en_protoUdp;
	else if(Sdf_mc_strcasecmp(pSocketProperties->pProtocol, "TCP")==0)
		dSocket.dProtocol = Sdf_en_protoTcp;
	else
	{
		/* 
		 * Invalid protocol requested. Trace error and return fail
		 */
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
			"Invalid socket protocol specified", pErr);
#endif
		pErr->errCode=Sdf_en_socketOpenError;
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(Sdf_mc_strstr(pSocketProperties->pIpAddress,":") !=Sdf_co_null)
		dIsIpv6=Sdf_co_true;

#ifdef SDF_IPV6
	if(dIsIpv6==Sdf_co_true)
		dDomain = AF_INET6;
	else
#endif
		dDomain = AF_INET;

	if(dSocket.dProtocol == Sdf_en_protoUdp)
		dType = SOCK_DGRAM;
	else
		dType = SOCK_STREAM;



	if ((tempSocket = sdf_fn_uaSocketCreate(dDomain, dType,0)) < 0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSocket( ): "
			"Could not open socket for initiating call",pErr) ;
#endif
		pErr->errCode=Sdf_en_socketOpenError;
		return Sdf_co_fail;
	}

	dSocket.dSockfd = tempSocket;
	/*
	 * SPR 2221 Fix.
	 * We should set the socket fd here as it not set in the 
	 * sdf_fn_uaSocketCreate function.
	 */
#ifdef SDF_LINT
/* #ifndef SDF_NETWORKCALLBACKS: This LINT fix is removed as the
 * NETWORKCALLBACKS will not be defined by default
 */
    /*w534*/ (void)sdf_fn_uaSetSocketFd(&dGlbFdSet, dSocket.dSockfd);
/* #endif */
#else
#ifndef SDF_NETWORKCALLBACKS
    sdf_fn_uaSetSocketFd(&dGlbFdSet, dSocket.dSockfd);
#endif    
#endif    
	/* 
	 * Socket created. Now set the socket options in the property structure
	 */
	(void)sdf_listInitIterator(&(pSocketProperties->slOptions), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_socketOptions *pOption;

		pOption = (Sdf_st_socketOptions*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_fn_uaSetSockOpt(tempSocket, pOption->level, pOption->optName,\
			pOption->pOptVal, pOption->optlen) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
				"(): Error in setting socket option", pErr);
#endif
			pErr->errCode=Sdf_en_socketOpenError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	/* 
	 * Socket now created and options set. Set sockopt for re-using the 
	 * socket after app exit. No error check here since its not critical
	 */
	/*w534*/ (void)sdf_fn_uaSetSockOpt(tempSocket, SOL_SOCKET, SO_REUSEADDR, \
		(Sdf_ty_s8bit *) &reuseOptInt, 	sizeof(Sdf_ty_u32bit));
#ifdef SDF_LOOKUP_ICMP
#endif
	/* 
	 * Bind the socket to the interface given in the property structure
	 */

	if(dIsIpv6 == Sdf_co_true)
	{
#ifdef SDF_IPV6		
		Sdf_mc_bzero((void*)&servAddr6,sizeof(servAddr6));
		servAddr6.sin6_family = AF_INET6;
		servAddr6.sin6_port = Sdf_mc_htons(pSocketProperties->dPort);

		if(pSocketProperties->dType != Sdf_en_sockMulticast)
		{
			if (pSocketProperties->pIpAddress != Sdf_co_null)
			{
				memset(&Hints, 0, sizeof(Hints));
				Hints.ai_flags = AI_NUMERICHOST;
				Hints.ai_family =PF_INET6;
				if(dSocket.dProtocol == Sdf_en_protoUdp)
				{
					Hints.ai_socktype = SOCK_DGRAM;
					Hints.ai_protocol = IPPROTO_UDP;
				}
				else
				{
					Hints.ai_socktype = SOCK_STREAM;
					Hints.ai_protocol = IPPROTO_TCP;
				}
				(void)sdf_fn_uaSprintf(dPort, "%hu", pSocketProperties->dPort);
				RetVal = getaddrinfo(pSocketProperties->pIpAddress, dPort, \
					&Hints, &pAddrInfo);

				if(RetVal != 0)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
						(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
						"(): Failed to resolve IP address into numeric form", pErr);
#endif
					pErr->errCode=Sdf_en_systemError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
					return Sdf_co_fail;
				}
			}
		}
		else
		{
			if (pSocketProperties->pMulticastGroup != Sdf_co_null)
			{
				memset(&Hints, 0, sizeof(Hints));
				Hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
				Hints.ai_family =PF_INET6;
				if(dSocket.dProtocol == Sdf_en_protoUdp)
				{
					Hints.ai_socktype = SOCK_DGRAM;
					Hints.ai_protocol = IPPROTO_UDP;
				}
				else
				{
					Hints.ai_socktype = SOCK_STREAM;
					Hints.ai_protocol = IPPROTO_TCP;
				}
				(void)sdf_fn_uaSprintf(dPort, "%hu", pSocketProperties->dPort);
				RetVal = getaddrinfo(pSocketProperties->pIpAddress, dPort, \
					&Hints, &pAddrInfo);
				if(RetVal != 0)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
						(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
						"(): Failed to resolve IP address into numeric form", pErr);
#endif
					pErr->errCode=Sdf_en_systemError;
					(void)sdf_fn_uaCloseSocket(&dSocket);
					return Sdf_co_fail;
				}
			}
			else
			{
				/*
				 * Multicast option specified but no multicast group specified
				 */
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Multicast option requested without specifying"
					" multicast group to join", pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}
		}
#else
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
			"(): Wrong IP Address IPV6 Support, Please compile with IPV6"
			" multicast group to join", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		(void)sdf_fn_uaCloseSocket(&dSocket);
		return Sdf_co_fail;
#endif		
	}
	else
	{
		Sdf_mc_bzero((void*)&servAddr,sizeof(servAddr));
		servAddr.sin_family = AF_INET;	
		servAddr.sin_port = Sdf_mc_htons(pSocketProperties->dPort);
		if(pSocketProperties->dType != Sdf_en_sockMulticast)
		{
			if (pSocketProperties->pIpAddress != Sdf_co_null)
			{
				servAddr.sin_addr.s_addr = inet_addr(pSocketProperties->pIpAddress);
			}
			else
			{
				servAddr.sin_addr.s_addr = Sdf_mc_htonl(INADDR_ANY);
			}
		}
		else
		{
			if (pSocketProperties->pMulticastGroup != Sdf_co_null)
			{
				servAddr.sin_addr.s_addr = inet_addr(pSocketProperties->\
					pMulticastGroup);
			}
			else
			{
				/* 
				 * Multicast option specified but no multicast group specified
				 */
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Multicast option requested without specifying"
					" multicast group to join", pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}
		}
	}

	/* Bind name to socket */
	if(dIsIpv6 == Sdf_co_true)
	{
#ifdef SDF_IPV6	
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)    
		if ((sdf_fn_uaBindSocket(tempSocket, pAddrInfo->ai_addr, \
				pAddrInfo->ai_addrlen)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Could not bind to address", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
			(void)sdf_fn_uaCloseSocket (&dSocket);
			freeaddrinfo(pAddrInfo);

			return Sdf_co_fail;
		}
		freeaddrinfo(pAddrInfo);
#else
        if ((sdf_fn_uaBindSocket(tempSocket, (Sdf_ty_pvoid) &servAddr6, \
			sizeof(servAddr6))) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Could not bind to address", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
		 (void)sdf_fn_uaCloseSocket (&dSocket);
			return Sdf_co_fail;
		}
#endif
#else
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
			"Wrong IP address, please compile with SDF_IPV6 flag", pErr);
#endif
		pErr->errCode=Sdf_en_socketBindError;
	 (void)sdf_fn_uaCloseSocket (&dSocket);
		return Sdf_co_fail;
#endif		
	}
	else
	{
		if ((sdf_fn_uaBindSocket(tempSocket, (Sdf_ty_pvoid) &servAddr, \
			sizeof(servAddr))) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Could not bind to address", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
			(void)sdf_fn_uaCloseSocket (&dSocket);
			return Sdf_co_fail;
		}
	}

	/* 
	 * Add to multicast group in case the socket is for multicast
	 */
	if (pSocketProperties->dType == Sdf_en_sockMulticast)
	{
		if(dIsIpv6 == Sdf_co_true)
		{
#ifdef SDF_IPV6		
#else
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Wrong IP address, please compile with SDF_IPV6 flag", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
			(void)sdf_fn_uaCloseSocket (&dSocket);
			return Sdf_co_fail;
#endif		
		}
		else
		{
			multicastMreq.imr_multiaddr.s_addr= inet_addr\
				(pSocketProperties->pMulticastGroup);

			if (pSocketProperties->pIpAddress != Sdf_co_null)		
				multicastMreq.imr_interface.s_addr= \
						inet_addr(pSocketProperties->pIpAddress);
			else	
				multicastMreq.imr_interface.s_addr = Sdf_mc_htonl(INADDR_ANY);

			if (sdf_fn_uaSetSockOpt(tempSocket,IPPROTO_IP, \
				IP_ADD_MEMBERSHIP,&multicastMreq,sizeof(multicastMreq)) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Failed to join multicast group", pErr);
#endif
				pErr->errCode=Sdf_en_socketOpenError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}	
		}
	}

	/* 
	 * Bind also through. Now set the maximum pending connections if it is a 
	 * TCP socket
	 */
	if(dSocket.dProtocol == Sdf_en_protoTcp)
	{
		/*w534*/ (void)sdf_fn_uaListenSocket(tempSocket, pSocketProperties->dMaxPendConns);
	}

	/*
	 * Set pSockfd to the socket created.
	 */
	*pSockfd = tempSocket;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaInitSocket\n");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
#endif /* ifndef ICF_PORT_SYMBIAN*/
#else /***ifndef ICF_UATK_PORT****/

	Sdf_ty_socketId tempSocket = Sdf_co_InvalidSocketFd;
	Sdf_st_socket   dSocket;
	Sdf_st_listIterator dListIterator;
	Sdf_ty_s32bit dDomain, dType;

	struct linger lingerOpt;
	lingerOpt.l_onoff=0;
	lingerOpt.l_linger=0;

	Sdf_ty_u32bit reuseOptInt = 1;
	
	Sdf_ty_bool dIsIpv6=Sdf_co_false;

#ifdef SDF_IPV6	
    struct sockaddr_in6 servAddr6;
#endif	
	
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#ifdef SDF_IPV6	
#ifdef SDF_LINT
	struct addrinfo *pAddrInfo={0};
#else
	struct addrinfo *pAddrInfo;
#endif
	struct addrinfo	Hints;
	Sdf_ty_s32bit	RetVal;
	Sdf_ty_s8bit  dPort[Sdf_co_characterBufferSize];
#endif	

#else
#ifdef SDF_IPV6	
    struct ipv6_mreq    multicastMreq6;
#endif	
#endif
	struct sockaddr_in 	servAddr;
	struct ip_mreq 		multicastMreq;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Entering sdf_ivk_uaInitSocket\n");

	if(Sdf_mc_strcasecmp(pSocketProperties->pProtocol,"UDP")==0)
		dSocket.dProtocol = Sdf_en_protoUdp;
	else if(Sdf_mc_strcasecmp(pSocketProperties->pProtocol, "TCP")==0)
		dSocket.dProtocol = Sdf_en_protoTcp;
	else
	{
		/* 
		 * Invalid protocol requested. Trace error and return fail
		 */
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
			"Invalid socket protocol specified", pErr);
#endif
		pErr->errCode=Sdf_en_socketOpenError;
		pErr->errCode = Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}
	if(Sdf_mc_strstr(pSocketProperties->pIpAddress,":") !=Sdf_co_null)
		dIsIpv6=Sdf_co_true;

#ifdef SDF_IPV6
	if(dIsIpv6==Sdf_co_true)
		dDomain = AF_INET6;
	else
#endif
		dDomain = AF_INET;

	if(dSocket.dProtocol == Sdf_en_protoUdp)
		dType = SOCK_DGRAM;
	else
		dType = SOCK_STREAM;



	if ((tempSocket = sdf_fn_uaSocketCreate(dDomain, dType,0)) < 0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
			(Sdf_ty_s8bit *)"sdf_ivk_uaInitSocket( ): "
			"Could not open socket for initiating call",pErr) ;
#endif
		pErr->errCode=Sdf_en_socketOpenError;
		return Sdf_co_fail;
	}

	dSocket.dSockfd = tempSocket;
	/*
	 * SPR 2221 Fix.
	 * We should set the socket fd here as it not set in the 
	 * sdf_fn_uaSocketCreate function.
	 */
#ifdef SDF_LINT
/* #ifndef SDF_NETWORKCALLBACKS: This LINT fix is removed as the
 * NETWORKCALLBACKS will not be defined by default
 */
    /*w534*/ (void)sdf_fn_uaSetSocketFd(&dGlbFdSet, dSocket.dSockfd);
/* #endif */
#else
#ifndef SDF_NETWORKCALLBACKS
    sdf_fn_uaSetSocketFd(&dGlbFdSet, dSocket.dSockfd);
#endif    
#endif    
	/* 
	 * Socket created. Now set the socket options in the property structure
	 */
	(void)sdf_listInitIterator(&(pSocketProperties->slOptions), \
		&dListIterator, pErr);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_socketOptions *pOption;

		pOption = (Sdf_st_socketOptions*)\
			(dListIterator.pCurrentElement->pData);

		if(sdf_fn_uaSetSockOpt(tempSocket, pOption->level, pOption->optName,\
			pOption->pOptVal, pOption->optlen) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
				"(): Error in setting socket option", pErr);
#endif
			pErr->errCode=Sdf_en_socketOpenError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
			return Sdf_co_fail;
		}
		(void)sdf_listNext(&dListIterator, pErr);
	}
	/* 
	 * Socket now created and options set. Set sockopt for re-using the 
	 * socket after app exit. No error check here since its not critical
	 */
	/*w534*/ (void)sdf_fn_uaSetSockOpt(tempSocket, SOL_SOCKET, SO_REUSEADDR, \
		(Sdf_ty_s8bit *) &reuseOptInt, 	sizeof(Sdf_ty_u32bit));


	         (void)sdf_fn_uaSetSockOpt(tempSocket, SOL_SOCKET, SO_LINGER, \
		(Sdf_ty_pvoid)&lingerOpt, sizeof(lingerOpt));

#ifdef SDF_LOOKUP_ICMP
#endif
	/* 
	 * Bind the socket to the interface given in the property structure
	 */

	if(dIsIpv6 == Sdf_co_true)
	{
#ifdef SDF_IPV6		
		Sdf_mc_bzero((void*)&servAddr6,sizeof(servAddr6));
		servAddr6.sin6_family = AF_INET6;
		servAddr6.sin6_port = Sdf_mc_htons(pSocketProperties->dPort);

		if(pSocketProperties->dType != Sdf_en_sockMulticast)
		{
			if (pSocketProperties->pIpAddress != Sdf_co_null)
			{
				if (0 == inet_pton(AF_INET6, pSocketProperties->pIpAddress, 
								   (Sdf_ty_pvoid)&(servAddr6.sin6_addr)))
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
						(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
						"(): Failed to resolve IP address into numeric form", pErr);
#endif
					pErr->errCode=Sdf_en_systemError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
					return Sdf_co_fail;
				}
			}
		}
		else
		{
			if (pSocketProperties->pMulticastGroup != Sdf_co_null)
			{
				if (0 == inet_pton(AF_INET6, pSocketProperties->pMulticastGroup, 
								   (Sdf_ty_pvoid)&(servAddr6.sin6_addr)))
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
						(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
						"(): Failed to resolve IP address into numeric form", pErr);
#endif
					pErr->errCode=Sdf_en_systemError;
			  	(void)sdf_fn_uaCloseSocket(&dSocket);
					return Sdf_co_fail;
				}
			}
			else
			{
				/*
				 * Multicast option specified but no multicast group specified
				 */
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Multicast option requested without specifying"
					" multicast group to join", pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}
		}
#else
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
			"(): Wrong IP Address IPV6 Support, Please compile with IPV6"
			" multicast group to join", pErr);
#endif		
		pErr->errCode=Sdf_en_invalidParamError;
		(void)sdf_fn_uaCloseSocket(&dSocket);
		return Sdf_co_fail;
#endif		
	}
	else
	{
		Sdf_mc_bzero((void*)&servAddr,sizeof(servAddr));
		servAddr.sin_family = AF_INET;	
		servAddr.sin_port = Sdf_mc_htons(pSocketProperties->dPort);
		if(pSocketProperties->dType != Sdf_en_sockMulticast)
		{
			if (pSocketProperties->pIpAddress != Sdf_co_null)
			{
				servAddr.sin_addr.s_addr = inet_addr(pSocketProperties->pIpAddress);
			}
			else
			{
				servAddr.sin_addr.s_addr = Sdf_mc_htonl(INADDR_ANY);
			}
		}
		else
		{
			if (pSocketProperties->pMulticastGroup != Sdf_co_null)
			{
				servAddr.sin_addr.s_addr = inet_addr(pSocketProperties->\
					pMulticastGroup);
			}
			else
			{
				/* 
				 * Multicast option specified but no multicast group specified
				 */
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_invalidParamError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Multicast option requested without specifying"
					" multicast group to join", pErr);
#endif
				pErr->errCode=Sdf_en_invalidParamError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}
		}
	}

	/* Bind name to socket */
	if(dIsIpv6 == Sdf_co_true)
	{
#ifdef SDF_IPV6		
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
		if ((sdf_fn_uaBindSocket(tempSocket, pAddrInfo->ai_addr, \
				pAddrInfo->ai_addrlen)) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Could not bind to address", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
			(void)sdf_fn_uaCloseSocket (&dSocket);
			freeaddrinfo(pAddrInfo);

			return Sdf_co_fail;
		}
		freeaddrinfo(pAddrInfo);
#else
		if ((sdf_fn_uaBindSocket(tempSocket, (Sdf_ty_pvoid) &servAddr6, \
			sizeof(servAddr6))) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Could not bind to address", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
		 (void)sdf_fn_uaCloseSocket (&dSocket);
			return Sdf_co_fail;
		}
#endif
#else
#ifdef SDF_ERROR		
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
			"Wrong IP address, please compile with SDF_IPV6 flag", pErr);
#endif
		pErr->errCode=Sdf_en_socketBindError;
	 (void)sdf_fn_uaCloseSocket (&dSocket);
		return Sdf_co_fail;
#endif		
	}
	else
	{
		if ((sdf_fn_uaBindSocket(tempSocket, (Sdf_ty_pvoid) &servAddr, \
			sizeof(servAddr))) ==Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Could not bind to address", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
			(void)sdf_fn_uaCloseSocket (&dSocket);
			return Sdf_co_fail;
		}
	}

	/* 
	 * Add to multicast group in case the socket is for multicast
	 */
	if (pSocketProperties->dType == Sdf_en_sockMulticast)
	{
		if(dIsIpv6 == Sdf_co_true)
		{
#ifdef SDF_IPV6		
			if (0 == inet_pton(AF_INET6, pSocketProperties->pMulticastGroup, 
						   (Sdf_ty_pvoid)&(multicastMreq6.ipv6mr_multiaddr)))
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Failed to resolve IP address into numeric form", pErr);
#endif
				pErr->errCode=Sdf_en_systemError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}

			/*
			 * ipv6mr_interface should be 0 to choose the default multicast 
			 * interface. 
			 */
			multicastMreq6.ipv6mr_interface = 0;

			if (sdf_fn_uaSetSockOpt(tempSocket,IPPROTO_IPV6, \
				IPV6_ADD_MEMBERSHIP,&multicastMreq6,sizeof(multicastMreq6)) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Failed to join multicast group", pErr);
#endif
				pErr->errCode=Sdf_en_socketOpenError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}
#else
#ifdef SDF_ERROR		
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketBindError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket(): "
				"Wrong IP address, please compile with SDF_IPV6 flag", pErr);
#endif
			pErr->errCode=Sdf_en_socketBindError;
			(void)sdf_fn_uaCloseSocket (&dSocket);
			return Sdf_co_fail;
#endif		
		}
		else
		{
			multicastMreq.imr_multiaddr.s_addr= inet_addr\
				(pSocketProperties->pMulticastGroup);

			if (pSocketProperties->pIpAddress != Sdf_co_null)		
				multicastMreq.imr_interface.s_addr= \
						inet_addr(pSocketProperties->pIpAddress);
			else	
				multicastMreq.imr_interface.s_addr = Sdf_mc_htonl(INADDR_ANY);

			if (sdf_fn_uaSetSockOpt(tempSocket,IPPROTO_IP, \
				IP_ADD_MEMBERSHIP,&multicastMreq,sizeof(multicastMreq)) \
				== Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError, \
					(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
					"(): Failed to join multicast group", pErr);
#endif
				pErr->errCode=Sdf_en_socketOpenError;
				(void)sdf_fn_uaCloseSocket(&dSocket);
				return Sdf_co_fail;
			}	
		}
	}

	/* 
	 * Bind also through. Now set the maximum pending connections if it is a 
	 * TCP socket
	 */
	if(dSocket.dProtocol == Sdf_en_protoTcp)
	{
		/*w534*/ (void)sdf_fn_uaListenSocket(tempSocket, pSocketProperties->dMaxPendConns);
	}

	/*
	 * Set pSockfd to the socket created.
	 */
	*pSockfd = tempSocket;

	sdf_fn_debug((Sdf_ty_s8bit*)"SDF_DEBUG - Exiting sdf_ivk_uaInitSocket\n");
	pErr->errCode = Sdf_en_noUaError;
	return Sdf_co_success;

#endif /***************ICF_UATK_PORT*********************/ 
}


/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitiateConnection
 **
 ** DESCRIPTION: The function is called internally by the toolkit to initiate
 **				 a new connection at the time of sending a message towards
 **				 network
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaInitiateConnection
#ifdef ANSI_PROTO
	(Sdf_st_transaction *pTransaction,
	Sdf_st_overlapTransInfo *pOverlapTransInfo,
	Sdf_st_initData *pInitData,
	Sdf_st_error *pError)
#else 	
	(pTransaction, pOverlapTransInfo, pInitData, pError)
	Sdf_st_transaction *pTransaction;
	Sdf_st_overlapTransInfo *pOverlapTransInfo;
	Sdf_st_initData *pInitData;
	Sdf_st_error *pError;
#endif
{
	Sdf_ty_socketId 	sockfd=0;
	Sdf_st_socket 		dSocket;
	Sdf_st_transportInfo *pDestInfo = Sdf_co_null;
	
#ifdef SDF_IPV6	
    struct sockaddr_in6 dServaddr6;
#endif	
	struct sockaddr_in  dServaddr;

#if defined (SDF_WINDOWS) || defined (SDF_WINCE)
#ifdef SDF_IPV6	
#ifdef SDF_LINT
	struct addrinfo *pAddrInfo={0};
#else
	struct addrinfo *pAddrInfo;
#endif
	struct addrinfo	Hints;
	Sdf_ty_s32bit	RetVal;
	Sdf_ty_s8bit  dPort[Sdf_co_characterBufferSize];
#endif
#endif
#ifdef SDF_TLS
	Sdf_st_sslData	*pSslData = Sdf_co_null;
#else
	Sdf_ty_pvoid	pSslData = Sdf_co_null;
#endif
	Sdf_ty_bool 	dIsIpv6=Sdf_co_false;
	Sdf_ty_s32bit 	dDomain;
	Sdf_st_socket	*pSocket = Sdf_co_null;
	
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_fn_uaInitiateConnection\n");

	(void)pInitData;

	if(pOverlapTransInfo==Sdf_co_null)
  {
    if(pTransaction != Sdf_co_null )   
		  pSocket = &(pTransaction->dSocket);
  }
	else
		pSocket = &(pOverlapTransInfo->dSocket);
		
	if(pSocket != Sdf_co_null)
  if (sdf_fn_uaIsConnectedTcpSocket(pSocket->dSockfd,pError)== Sdf_co_success)
	{
		/* 
		 * No new connection required since the existing TCP 
		 * channel can be reused
		 */
		sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
			"Exiting sdf_fn_uaInitiateConnection\n");
		pError->errCode = Sdf_en_noUaError;
		return Sdf_co_success;
	}	
	
	if (sdf_ivk_uaGetDestInfoFromTransaction(pTransaction,pOverlapTransInfo,\
			&pDestInfo, pError) == Sdf_co_fail)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_callObjectAccessError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
			"Error getting DestInfo from Transaction Object",pError);
#endif
		pError->errCode=Sdf_en_callObjectAccessError;
		return Sdf_co_fail;
	}
#ifdef SDF_IPV6
	if(Sdf_mc_strstr(pDestInfo->pIp,":") !=Sdf_co_null)
	{
		dIsIpv6=Sdf_co_true;
		dDomain=AF_INET6;
	}
	else
#endif
	{
		dIsIpv6=Sdf_co_false;
		dDomain=AF_INET;
	}

	if ((sockfd = sdf_fn_uaSocketCreate(dDomain, SOCK_STREAM, 0)) < 0)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketOpenError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
			"Could not open TCP socket for initiating call",pError) ;
#endif
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
		pError->errCode=Sdf_en_socketOpenError;
		return Sdf_co_fail;
	}

	/* Set the socket descriptor in the socket structure. */
	dSocket.dSockfd = sockfd;

	if(dIsIpv6 == Sdf_co_true)
	{	
#ifdef SDF_IPV6		
		Sdf_mc_bzero((Sdf_ty_pvoid)&dServaddr6,sizeof(dServaddr6));
#ifdef ICF_UATK_PORT
			memset(&Hints, 0, sizeof(Hints));
			Hints.ai_flags = AI_NUMERICHOST;
			Hints.ai_family =PF_INET6;
			Hints.ai_socktype = SOCK_STREAM;
			Hints.ai_protocol = IPPROTO_TCP;
			(void)sdf_fn_uaSprintf(dPort, "%hu", pDestInfo->dPort);
			RetVal = getaddrinfo(pDestInfo->pIp, dPort, &Hints, &pAddrInfo);
			if(RetVal != 0)
#else  /******ICF_UATK_PORT******/
		dServaddr6.sin6_family = AF_INET6;
		dServaddr6.sin6_port = Sdf_mc_htons((Sdf_ty_u16bit)pDestInfo->dPort);
		if (0 == inet_pton(AF_INET6, pDestInfo->pIp, 
						   (Sdf_ty_pvoid)&(dServaddr6.sin6_addr)))
#endif /******ICF_UATK_PORT******/

			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
				(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
				"(): Failed to resolve IP address into numeric form", pError);
#endif
				pError->errCode=Sdf_en_systemError;
				 (void)sdf_fn_uaCloseSocket(&dSocket);
				(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
				return Sdf_co_fail;
			}
#else
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_minorErrors, Sdf_en_systemError, \
			(Sdf_ty_s8bit*)"SDF_ERROR - sdf_ivk_uaInitSocket"
			"(): Wrong IP-Address, compile with IPv6 flag", pError);
#endif
			pError->errCode=Sdf_en_systemError;
		 (void)sdf_fn_uaCloseSocket(&dSocket);
		 (void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			return Sdf_co_fail;
#endif			
	}
	else
	{
		Sdf_mc_bzero((void*)&dServaddr,sizeof(dServaddr));
		dServaddr.sin_family = AF_INET;
		dServaddr.sin_port = Sdf_mc_htons((Sdf_ty_u16bit)pDestInfo->dPort);
		dServaddr.sin_addr.s_addr = inet_addr(pDestInfo->pIp);
	}

#ifdef SDF_TLS
	if (pDestInfo->dScheme == Sdf_en_protoTls)
	{
		/* Establish a connection to the remote IP/port */
		if (sdf_ivk_uaGetSslDataFromInitData(pInitData, &pSslData, \
				pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_socketInfoMapError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
				"Failed to get SSL data from the InitData.",pError);
#endif
			pError->errCode=Sdf_en_socketInfoMapError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
			(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
			return Sdf_co_fail;
		}
		dSocket.dProtocol = Sdf_en_protoTls;
	}
	else
#endif /* SDF_TLS */
	{
		dSocket.dProtocol = Sdf_en_protoTcp;
	}

	if(dIsIpv6 == Sdf_co_true)
	{
#ifdef SDF_IPV6		
#if defined (SDF_WINDOWS) || defined (SDF_WINCE)

		if (sdf_fn_uaConnectSocket(&dSocket, pAddrInfo->ai_addr, \
			pAddrInfo->ai_addrlen, (Sdf_ty_pvoid)pSslData) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_tcpConnectError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
				"Failed to connect to remote entity on TCP",pError);
#endif
			pError->errCode=Sdf_en_tcpConnectError;
		 (void)sdf_fn_uaCloseSocket(&dSocket);
			(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#ifdef SDF_TLS
		    (void)sdf_ivk_uaFreeSslData(pSslData);
#endif
			return Sdf_co_fail;
		}
#else
		if (sdf_fn_uaConnectSocket(&dSocket, (Sdf_ty_pvoid)&dServaddr6, \
			sizeof(dServaddr6), (Sdf_ty_pvoid)pSslData) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_tcpConnectError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
				"Failed to connect to remote entity on TCP",pError);
#endif
			pError->errCode=Sdf_en_tcpConnectError;
			 (void)sdf_fn_uaCloseSocket(&dSocket);
			 (void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#ifdef SDF_TLS
		   (void)sdf_ivk_uaFreeSslData(pSslData);
#endif
			return Sdf_co_fail;
		}
#endif
#else
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_tcpConnectError,\
			(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
			"Wrong IP Address: please compile with IPV6 flag",pError);
#endif
		pError->errCode=Sdf_en_tcpConnectError;
		(void)sdf_fn_uaCloseSocket(&dSocket);
		(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#ifdef SDF_TLS
		(void)sdf_ivk_uaFreeSslData(pSslData);
#endif
		return Sdf_co_fail;

#endif

	}
	else
	{
		if (sdf_fn_uaConnectSocket(&dSocket, (Sdf_ty_pvoid)&dServaddr,	\
			sizeof(dServaddr), (Sdf_ty_pvoid)pSslData) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_tcpConnectError,\
				(Sdf_ty_s8bit *)"sdf_fn_uaInitiateConnection( ): "
				"Failed to connect to remote entity on TCP",pError);
#endif
			pError->errCode=Sdf_en_tcpConnectError;
			(void)sdf_fn_uaCloseSocket(&dSocket);
			(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#ifdef SDF_TLS
		   (void)sdf_ivk_uaFreeSslData(pSslData);
#endif
			return Sdf_co_fail;
		}
	}
	
	/* Free the local reference */
	(void)sdf_ivk_uaFreeTransportInfo(pDestInfo);
#ifdef SDF_TLS
	(void)sdf_ivk_uaFreeSslData(pSslData);
#endif
		
	/*
	 * Set this as the socket file descriptor for the call
	 */
	if(pOverlapTransInfo ==Sdf_co_null) 
  { 
    if (pTransaction != Sdf_co_null)  
		  pTransaction->dSocket = dSocket;
  }
	else
		pOverlapTransInfo->dSocket = dSocket;

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_fn_uaInitiateConnection\n");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:	sdf_ivk_uaSetFdSetToAllTransactions
 **
 ** DESCRIPTION: This API sets the fd_set to all the transaction of the
 **				call object ( including overlap transactions)
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaSetFdSetToAllTransactions
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObject,
	Sdf_ty_fdSet *pFdSet,
	Sdf_st_error *pError)
#else 	
	(pCallObject, pFdSet, pError)
	Sdf_st_callObject	*pCallObject;
	Sdf_ty_fdSet *pFdSet;
	Sdf_st_error *pError;
#endif
{

	Sdf_st_listIterator dListIterator;

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaSetFdSetToAllTransactions( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	

	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetFdSetToAllTransactions( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pFdSet == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaSetFdSetToAllTransactions( ): "
			" FdSet parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_ivk_uaSetFdSetToAllTransactions \n");

	(void)sdf_listInitIterator(&(pCallObject->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_overlapTransInfo *pOverlapTransInfo;

		pOverlapTransInfo = (Sdf_st_overlapTransInfo *) \
				(dListIterator.pCurrentElement->pData);
#ifdef SDF_LINT
		if(pOverlapTransInfo->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
			FD_SET((unsigned)pOverlapTransInfo->dSocket.dSockfd, pFdSet);
#else        
		if(pOverlapTransInfo->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
			FD_SET(pOverlapTransInfo->dSocket.dSockfd, pFdSet);
#endif
        
		(void)sdf_listNext(&dListIterator, pError);
	}	
#ifdef SDF_LINT
    if(pCallObject->pUacTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
		FD_SET((unsigned)pCallObject->pUacTransaction->dSocket.dSockfd, pFdSet);

	if(pCallObject->pUasTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
		FD_SET((unsigned)pCallObject->pUasTransaction->dSocket.dSockfd, pFdSet);
#else    
    if(pCallObject->pUacTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
		FD_SET(pCallObject->pUacTransaction->dSocket.dSockfd, pFdSet);

	if(pCallObject->pUasTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
		FD_SET(pCallObject->pUasTransaction->dSocket.dSockfd, pFdSet);
#endif
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetFdSetToAllTransactions\n");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaGetListOfSockFd
 **
 ** DESCRIPTION: This API returns the list of SockFds from the call object
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetListOfSockFd
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObject,
	Sdf_ty_slist *pList,
	Sdf_st_error *pError)
#else 	
	(pCallObject,pList, pError)
	Sdf_st_callObject	*pCallObject;
	Sdf_ty_fdSet *pList;
	Sdf_st_error *pError;
#endif
{

	Sdf_st_listIterator dListIterator;

	Sdf_ty_socketId *pElem=Sdf_co_null;

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaGetListOfSockFd( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	

	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
			" pList parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_ivk_uaGetListOfSockFd \n");

	(void)sdf_listInitIterator(&(pCallObject->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_overlapTransInfo *pOverlapTransInfo;

		pOverlapTransInfo = (Sdf_st_overlapTransInfo *) \
				(dListIterator.pCurrentElement->pData);

		if(pOverlapTransInfo->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
		{
			pElem=(Sdf_ty_socketId *)sdf_memget(Sdf_mc_callHandlerMemId,\
				sizeof(Sdf_ty_socketId),pError);
			if(Sdf_co_null == pElem)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
						"pElem - memory allocation failure",pError);
#endif
				pError->errCode = Sdf_en_noMemoryError;
				return Sdf_co_fail;
			}
			
			*pElem=pOverlapTransInfo->dSocket.dSockfd;
			if (sdf_listAppend(pList,(Sdf_ty_pvoid)pElem,pError) == Sdf_co_fail)
			{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors, \
					Sdf_en_callObjectAccessError,\
				(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
				"Error in List Append",pError);
#endif
				pError->errCode=Sdf_en_callObjectAccessError;
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pElem,pError);
				return Sdf_co_fail;
			}
		}
		(void)sdf_listNext(&dListIterator, pError);
	}	
	if(pCallObject->pUacTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
	{
		pElem=(Sdf_ty_socketId *)sdf_memget(Sdf_mc_callHandlerMemId,\
			sizeof(Sdf_ty_socketId),pError);
		if(Sdf_co_null == pElem)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
					"pElem - memory allocation failure",pError);
#endif
			pError->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}
		
		*pElem=pCallObject->pUacTransaction->dSocket.dSockfd;
		if (sdf_listAppend(pList,(Sdf_ty_pvoid)pElem,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
				"Error in List Append",pError);
#endif
				pError->errCode=Sdf_en_callObjectAccessError;
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pElem,pError);
				return Sdf_co_fail;
		}
	}
	if(pCallObject->pUasTransaction->dSocket.dSockfd != Sdf_co_InvalidSocketFd)
	{
		pElem=(Sdf_ty_socketId *)sdf_memget(Sdf_mc_callHandlerMemId,\
			sizeof(Sdf_ty_socketId),pError);
		if(Sdf_co_null == pElem)
		{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
					"pElem - memory allocation failure",pError);
#endif
			pError->errCode = Sdf_en_noMemoryError;
			return Sdf_co_fail;
		}

		*pElem=pCallObject->pUasTransaction->dSocket.dSockfd;
		if (sdf_listAppend(pList,(Sdf_ty_pvoid)pElem,pError) == Sdf_co_fail)
		{
#ifdef SDF_ERROR
				sdf_fn_setError(Sdf_en_minorErrors,\
					Sdf_en_callObjectAccessError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetListOfSockFd( ): "
				"Error in List Append",pError);
#endif
				pError->errCode=Sdf_en_callObjectAccessError;
				(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
					&pElem,pError);
				return Sdf_co_fail;
		}

	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaSetFdSetToAllTransactions\n");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:	 sdf_ivk_uaGetFdIsSetSockFds
 **
 ** DESCRIPTION: This API returns the List of Sdf_st_socket structures, 
 **              for all the dSockFds that are active by doing FD_ISSET on the 
 **              SockFds of each transaction associated with the Call Object.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_ivk_uaGetFdIsSetSockFds
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObject,
	Sdf_ty_slist *pList,
	Sdf_ty_fdSet *pFdSet,
	Sdf_st_error *pError)
#else 	
	(pCallObject,pList,pFdSet pError)
	Sdf_st_callObject	*pCallObject;
	Sdf_ty_fdSet *pList;
	Sdf_ty_fdSet *pFdSet;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_listIterator dListIterator;

	Sdf_st_socket *pElem=Sdf_co_null;


#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaGetFdIsSetSockFds( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	

	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pList == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
			" pList parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
	if (pFdSet == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
			" pFdSet parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	

#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_ivk_uaGetFdIsSetSockFds \n");

	(void)sdf_listInitIterator(&(pCallObject->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_overlapTransInfo *pOverlapTransInfo;

		pOverlapTransInfo = (Sdf_st_overlapTransInfo *) \
				(dListIterator.pCurrentElement->pData);

		if((pOverlapTransInfo->dSocket.dProtocol != Sdf_en_protoUdp) && \
		   (pOverlapTransInfo->dSocket.dSockfd != Sdf_co_InvalidSocketFd))
		{
#ifdef SDF_LINT
            if (FD_ISSET((unsigned)pOverlapTransInfo->dSocket.dSockfd,pFdSet))
#else            
            if (FD_ISSET(pOverlapTransInfo->dSocket.dSockfd,pFdSet))
#endif                
			{
				pElem=(Sdf_st_socket *)sdf_memget(Sdf_mc_callHandlerMemId,\
					sizeof(Sdf_st_socket),pError);
				if(Sdf_co_null == pElem)
				{
#ifdef SDF_ERROR
			sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
					(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
					"pElem - memory allocation failure",pError);
#endif
					pError->errCode = Sdf_en_noMemoryError;
					return Sdf_co_fail;
				}

				*pElem=pOverlapTransInfo->dSocket;
				if (sdf_listAppend(pList,(Sdf_ty_pvoid)pElem,pError)\
					== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
						"Error in List Append",pError);
#endif
					pError->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pElem,pError);
					return Sdf_co_fail;
				}
			}
		}
		(void)sdf_listNext(&dListIterator, pError);
	}
	if(pCallObject->pUacTransaction !=Sdf_co_null)
	{
		if((pCallObject->pUacTransaction->dSocket.dProtocol != Sdf_en_protoUdp)\
				&& (pCallObject->pUacTransaction->dSocket.dSockfd != 
				Sdf_co_InvalidSocketFd))			
		{
#ifdef SDF_LINT
            if (FD_ISSET((unsigned)pCallObject->pUacTransaction->dSocket.dSockfd,pFdSet))
#else            
            if (FD_ISSET(pCallObject->pUacTransaction->dSocket.dSockfd,pFdSet))
#endif                
			{
				pElem=(Sdf_st_socket *)sdf_memget(Sdf_mc_callHandlerMemId,\
					sizeof(Sdf_st_socket),pError);
				if(Sdf_co_null == pElem)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
							"pElem - memory allocation failure",pError);
#endif
					pError->errCode = Sdf_en_noMemoryError;
					return Sdf_co_fail;
				}

				*pElem=pCallObject->pUacTransaction->dSocket;
				if (sdf_listAppend(pList,(Sdf_ty_pvoid)pElem,pError)\
						== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
						"Error in List Append",pError);
#endif
					pError->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pElem,pError);
					return Sdf_co_fail;
				}
			}
		}
	}
	if(pCallObject->pUasTransaction !=Sdf_co_null)
	{
		if((pCallObject->pUasTransaction->dSocket.dProtocol != Sdf_en_protoUdp)\
			&& (pCallObject->pUasTransaction->dSocket.dSockfd != \
			Sdf_co_InvalidSocketFd))
		{
#ifdef SDF_LINT
            if (FD_ISSET((unsigned)pCallObject->pUasTransaction->dSocket.dSockfd,pFdSet))
#else            
            if (FD_ISSET(pCallObject->pUasTransaction->dSocket.dSockfd,pFdSet))
#endif                
			{

				pElem=(Sdf_st_socket *)sdf_memget(Sdf_mc_callHandlerMemId,\
					sizeof(Sdf_st_socket),pError);
				if(Sdf_co_null == pElem)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_majorErrors, Sdf_en_noMemoryError,\
							(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
							"pElem - memory allocation failure",pError);
#endif
					pError->errCode = Sdf_en_noMemoryError;
					return Sdf_co_fail;
				}

				*pElem=pCallObject->pUasTransaction->dSocket;
				if (sdf_listAppend(pList,(Sdf_ty_pvoid)pElem,pError)\
						== Sdf_co_fail)
				{
#ifdef SDF_ERROR
					sdf_fn_setError(Sdf_en_minorErrors,\
						Sdf_en_callObjectAccessError,\
						(Sdf_ty_s8bit *)"sdf_ivk_uaGetFdIsSetSockFds( ): "
						"Error in List Append",pError);
#endif
					pError->errCode=Sdf_en_callObjectAccessError;
					(void)sdf_memfree(Sdf_mc_callHandlerMemId,(Sdf_ty_pvoid *)\
						&pElem,pError);
					return Sdf_co_fail;
				}
			}
		}
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaGetFdIsSetSockFds\n");
	pError->errCode = Sdf_en_noUaError;
	return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaResetSockFd
**
** DESCRIPTION: This API reset the SockFd map to the particular transaction
**				by setting corresponding map SockFd to Sdf_co_InvalidSocketFd
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaResetSockFd
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObject,
	Sdf_ty_socketId dSockfd,
	Sdf_st_error *pError)
#else 	
	(pCallObject,dSockfd, pError)
	Sdf_st_callObject	*pCallObject;
	Sdf_ty_socketId		dSockfd;
	Sdf_st_error *pError;
#endif
{
	Sdf_st_listIterator dListIterator;
	Sdf_ty_bool found=Sdf_co_false;

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaResetSockFd( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	

	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaResetSockFd( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_ivk_uaResetSockFd \n");

	(void)sdf_listInitIterator(&(pCallObject->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
                Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
		pOverlapTransInfo = (Sdf_st_overlapTransInfo *) \
				(dListIterator.pCurrentElement->pData);

		if((pOverlapTransInfo != Sdf_co_null) &&
		   (pOverlapTransInfo->dSocket.dSockfd == dSockfd))
                {
			pOverlapTransInfo->dSocket.dSockfd = Sdf_co_InvalidSocketFd;
			found=Sdf_co_true;
			break;
		}
		(void)sdf_listNext(&dListIterator, pError);
	}
        if ((pCallObject->pUacTransaction != Sdf_co_null) &&\
	(pCallObject->pUacTransaction->dSocket.dSockfd == dSockfd) &&\
		(found ==Sdf_co_false))
	{
		pCallObject->pUacTransaction->dSocket.dSockfd = Sdf_co_InvalidSocketFd;
		found=Sdf_co_true;
	}
	if ((pCallObject->pUasTransaction != Sdf_co_null) &&\
	(pCallObject->pUasTransaction->dSocket.dSockfd == dSockfd) &&\
    (found==Sdf_co_false))
	{
		pCallObject->pUasTransaction->dSocket.dSockfd = Sdf_co_InvalidSocketFd;
		found=Sdf_co_true;
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaResetSockFd\n");
	pError->errCode = Sdf_en_noUaError;

	if(found == Sdf_co_true)	
		return Sdf_co_success;
	else
		return Sdf_co_fail;
}

/*****************************************************************************
** FUNCTION: sdf_ivk_uaCloseAllSocketsForCall
**
** DESCRIPTION: This API closes the all the socket associated with the call
**				and will re-initialize the SockFd to Sdf_co_InvalidSocketFd
**
******************************************************************************/
Sdf_ty_retVal sdf_ivk_uaCloseAllSocketsForCall
#ifdef ANSI_PROTO
	(Sdf_st_callObject	*pCallObject,
	Sdf_st_error *pError)
#else 	
	(pCallObject, pError)
	Sdf_st_callObject	*pCallObject;
	Sdf_st_error *pError;
#endif
{

	Sdf_st_listIterator dListIterator;

#ifdef SDF_PARAMVALIDATION
	if (pError == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_dumpError((Sdf_ty_s8bit*)"SDF_ERROR - "
			"sdf_ivk_uaCloseAllSocketForCall( ): Invalid Error parameter");
#endif
		return Sdf_co_fail;
	}	

	if (pCallObject == Sdf_co_null)
	{
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_majorErrors,Sdf_en_invalidParamError, \
			(Sdf_ty_s8bit *)"sdf_ivk_uaCloseAllSocketForCall( ): "
			" Call object parameter passed to function is invalid",pError);
#endif
		pError->errCode=Sdf_en_invalidParamError;
		return Sdf_co_fail;
	}	
#endif

	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG -  "
		"Entering sdf_ivk_uaCloseAllSocketForCall \n");

	(void)sdf_listInitIterator(&(pCallObject->slOverlapTransInfo), \
		&dListIterator, pError);

	while (dListIterator.pCurrentElement != Sdf_co_null)
	{
		Sdf_st_overlapTransInfo *pOverlapTransInfo;

		pOverlapTransInfo = (Sdf_st_overlapTransInfo *) \
				(dListIterator.pCurrentElement->pData);

		if(pOverlapTransInfo->dSocket.dProtocol != Sdf_en_protoUdp)
		{
		(void)sdf_fn_uaCloseSocket(&pOverlapTransInfo->dSocket);
		}
		(void)sdf_listNext(&dListIterator, pError);
	}
	if(pCallObject->pUacTransaction->dSocket.dProtocol != \
			Sdf_en_protoUdp)
	{
		(void)sdf_fn_uaCloseSocket(&pCallObject->pUacTransaction->dSocket);
	}
	
	if(pCallObject->pUasTransaction->dSocket.dProtocol != Sdf_en_protoUdp)
	{
		(void)sdf_fn_uaCloseSocket(&pCallObject->pUasTransaction->dSocket);
	}
	sdf_fn_debug((Sdf_ty_s8bit *)"SDF_DEBUG - "
		"Exiting sdf_ivk_uaCloseAllSocketForCall\n");
	pError->errCode = Sdf_en_noUaError;

	return Sdf_co_success;
}
