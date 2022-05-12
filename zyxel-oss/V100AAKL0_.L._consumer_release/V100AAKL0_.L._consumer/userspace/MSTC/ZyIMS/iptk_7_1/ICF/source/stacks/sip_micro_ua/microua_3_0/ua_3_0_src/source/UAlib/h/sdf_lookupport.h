/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
extern "C" {
#endif 

/******************************************************************************
 ** FUNCTION:
 **	Has function declarations for Lookup portlayer structure and functions
 **				
 ******************************************************************************
 **
 ** FILENAME:  		sdf_lookupport.h
 **
 ** DESCRIPTION:	This file contains function declarations for Lookup 
 **					porlayer structure and functions
 **
 **  DATE        NAME                    REFERENCE           REASON
 **  ----        ----                    ---------          --------
 ** 10-Jun-05	R.Parthasarathi	 			   CREATION
 **
 ******************************************************************************
 **    		COPYRIGHT , Aricent, 2006
 ******************************************************************************/

#ifndef __SDF_LOOKUPPORT_H_
#define __SDF_LOOKUPPORT_H_

#ifdef SDF_LOOKUP
/*  lookup specific header files */
#include "ares.h"
#include "ares_dns.h"

typedef struct Sdf_tg_aresData {
	/* Channel is the main data structure of ARES */
	ares_channel  		pChannel;

	/* Socket created for the ARES reading operation */
	Sdf_ty_fdSet 		readfd;

	/* Socket created for the ARES writing operation */
	Sdf_ty_fdSet 		writefd;
} Sdf_st_aresData;

typedef struct Sdf_st_aresEventContext {
	Sdf_st_dnsEventContext		*pDnsEventContext; 
	/*  The following type is given to show the application
	 *  that EventContext can be  exteneded. The following variable
     *  is not used inside */
	Sdf_ty_s8bit 			*pDnsType;
    /* Domain name stored by IPTK for the scenario when SRV query fails 
     * and A query needs to be initiated */
    Sdf_ty_s8bit            *pDomainName; 

} Sdf_st_aresEventContext;

#ifdef SDF_ASYNC_DNS
typedef struct
{
	/*
	 * Update DNS Record In Txn using these values
	 */
	Sdf_ty_u32bit IPRecCount;
	Sdf_ty_u32bit IPRecIndex;

	/*
	 * To be freed after sdf_fn_uaDnsQuery returns
	 */
	Sdf_ty_s8bit		 *pHost;
	SipUrl				 *pUrl;
	Sdf_st_transportInfo *pDestInfo;
	Sdf_st_destDetails   *pDestDetails;

	/*
	 * To be used for closing TCP sockets after sdf_fn_uaDnsQuery returns
	 */
	Sdf_ty_u16bit dOldPort;
	Sdf_ty_s8bit  dOldIp[Sdf_co_characterBufferSize];

	/*
	 * The type of Message - Request or Response
	 */
	en_SipMessageType   dMessageType;
	/*
	 * To know the Url Type after the DNS response is received
	 */
	Sdf_ty_UrlType           dUrlType;

	/*
	 * Ares Data pointer for this DNS query
	 */
	Sdf_st_aresData   *pAresData;

	Sdf_ty_refCount		dRefCount;

	Sdf_ty_socketId	*pAresHashKey;

} Sdf_st_asyncDNSContextInfo;
#endif /*SDF_ASYNC_DNS*/


/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaDnsQuery
 **
 ** DESCRIPTION: This function has the routine to query the DNS server 
 **
 **	PARAMETERS:
 ** 	pDomainName(IN)       	    : The given Domain Name
 **   pDnsEventContext(IN)        : Sdf_st_dnsEventContext Structure
 **	 	pErr(OUT)										: Error structure in case the function fails

 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaDnsQuery _ARGS_ ((Sdf_ty_s8bit *pDomainName,
		Sdf_st_dnsEventContext *pDnsEventContext,
		Sdf_st_error *pErr));

/*****************************************************************************
 ** FUNCTION:	  sdf_fn_uaAppendSrvRecord
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **		 Lookup module
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **   pSrvRec(IN)            : Srv Record which is to append in the list.
 **	 	pErr(OUT)						   : Error structure in case the function fails
 **  
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendSrvRecord _ARGS_ ((Sdf_ty_slist *pslDnsRecords, 
		Sdf_st_dnsSrvRecord *pSrvRec, 
		Sdf_st_error *pErr));
/*****************************************************************************
 ** FUNCTION:	  sdf_fn_uaAppendIpRecord
 ** DESCRIPTION: The function use to update the Sorted SRV records into the 
 **		 Lookup module
 **	PARAMETERS:
 ** 	pslDnsRecords(IN)      : List of Dns Records
 **   pAddrString(IN)        : The IP Address.
 **	 	pErr(OUT)						   : Error structure in case the function fails
 **  
 *****************************************************************************/
Sdf_ty_retVal sdf_fn_uaAppendIpRecord _ARGS_((Sdf_ty_slist *pslDnsRecords,  
    Sdf_ty_s8bit *pAddrString, 
		Sdf_st_error *pErr));


/*****************************************************************************
 ** FUNCTION:	 Ares_cbk_uaQueryResult 
 ** DESCRIPTION: The callback thrown by the ARES for the SRV query result
 **	PARAMETERS:
 ** 	pArescontext(IN)      : List of Dns Records
 **   dstatus(IN)           : Message Status Type 
 **   pabuf(IN)							: Address Buffer
 **	 	pErr(OUT)						  : Error structure in case the function fails
 **  
 *****************************************************************************/
void Ares_cbk_uaQueryResult _ARGS_ ((void *pArescontext,
	 Sdf_ty_s32bit dstatus, 
	 Sdf_ty_u8bit *abuf, 
	 Sdf_ty_s32bit dlen));

/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaParseRR
 ** DESCRIPTION: The function  parse the RR record and store it in the List
 **	PARAMETERS:
 ** 	paptr(IN)      : List of Dns Records
 **   pabuf(IN)      : Address Buffer
 **   dalen(IN)			 : Length of the address
 **   pAppData(IN)   : Sdf_st_appData Structure
 **	 	pErr(OUT)			 : Error structure in case the function fails
 **  
 *****************************************************************************/
Sdf_ty_u8bit*  sdf_fn_uaParseRR	_ARGS_ ((\
		Sdf_ty_u8bit *paptr,
		Sdf_ty_u8bit *pabuf,
		Sdf_ty_s32bit dalen,
		Sdf_ty_pvoid pAppData,
		Sdf_ty_bool *p_query_flag,
                Sdf_ty_u8bit *pcount,
		Sdf_st_error *pErr));

Sdf_ty_u8bit sdf_ivk_uaExpandString(
                Sdf_ty_s8bit *p_src_str,
                Sdf_ty_s8bit **p_p_dest_str, 
                Sdf_ty_u32bit dlength);


/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitLookup
 ** DESCRIPTION: This function is used to initialize the lookup
 **  component.
 **  
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaInitLookup _ARGS_ ((void));   

/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaFreeLookup
 ** DESCRIPTION: This function is used to free the lookup
 **  component.
 **  
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaFreeLookup _ARGS_ ((Sdf_ty_pvoid pVoidData));


/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitAresData
 ** DESCRIPTION: It Creates the Thread Key for Gloable to all thread
 **	PARAMETERS:
 **   pAresData(IN)   : Sdf_st_aresData Structure
 **   pErr(OUT)			  : Error structure in case the function fails 
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaInitAresData _ARGS_ ((Sdf_st_aresData **pAresData, \
														Sdf_st_error *pErr));

/*****************************************************************
** FUNCTION:sdf_fn_uaFreeAresData()
** DESCRIPTION: It Frees the Ares Data
** PARAMETERS:
**   pAresData(IN)   : Sdf_st_aresData Structure
*******************************************************************/
void sdf_fn_uaFreeAresData _ARGS_((Sdf_st_aresData *pAresData));


/*****************************************************************
** FUNCTION:sdf_fn_uaFreeAresContext()
** DESCRIPTION: It Frees the Ares Context 
** PARAMETERS:
**   p_ares_ctxt(IN)   : Sdf_st_aresEventContext Structure
*******************************************************************/ 

Sdf_ty_retVal sdf_ivk_uaFreeAresContext _ARGS_ ((Sdf_ty_pvoid p_ares_ctxt, Sdf_ty_u16bit id));


#ifdef SDF_ASYNC_DNS
/*****************************************************************************
 ** FUNCTION:	 sdf_fn_uaInitAsyncDnsContextInfo
 ** DESCRIPTION: Inits the Sdf_st_asyncDNSContextInfo structure for storing 
 **              context information
 **	PARAMETERS:
 **   pAsyncDnsCxtInfo(IN)   : Sdf_st_asyncDNSContextInfo Structure
 **   pErr(OUT)			  : Error structure in case the function fails 
 *****************************************************************************/
Sdf_ty_retVal  sdf_fn_uaInitAsyncDnsContextInfo _ARGS_ \
				((Sdf_st_asyncDNSContextInfo **pAsyncDnsCxtInfo, \
					  Sdf_st_error *pErr));

/******************************************************************
** FUNCTION:  sdf_ivk_uaFreeAsyncDnsContextInfo
** DESCRIPTION: This function frees Async DNS Event Info structure
** PARAMETERS:
**   pAsyncDnsCxtInfo(IN)   : Sdf_st_asyncDNSContextInfo Structure
******************************************************************/
Sdf_ty_retVal sdf_fn_uaFreeAsyncDnsContextInfo _ARGS_ \
				((Sdf_st_asyncDNSContextInfo *pAsyncDnsCxtInfo));

/*****************************************************************************
 ** FUNCTION:		sdf_fn_computeAresDataHash
 **
 ** DESCRIPTION:	This function takes a Void Pointer and returns it after 
 **					casting it to U32bit
 ** 
 *****************************************************************************/
Sdf_ty_u32bit sdf_fn_computeAresDataHash
(Sdf_ty_pvoid	pData);

/*****************************************************************************
 ** FUNCTION:		sdf_fn_aresHashKeyCompare
 ** 
 ** DESCRIPTION: 	This is function to compare the hash 
 **					keys of the ares data pointers hash table.
 **
 *****************************************************************************/
Sdf_ty_u8bit sdf_fn_aresHashKeyCompare 
(Sdf_ty_pvoid	pKey1, 
 Sdf_ty_pvoid	pKey2);
/*****************************************************************************
 ** FUNCTION:		sdf_fn_removeAresDataFromHash
 ** 
 ** DESCRIPTION:	Function to remove ares data pointer from hash table .
 **
 *****************************************************************************/
void sdf_fn_removeAresDataFromHash
(Sdf_ty_pvoid	pAresData);
/*****************************************************************************
 ** FUNCTION:		sdf_fn_aresKeyFree
 ** 
 ** DESCRIPTION:	This is the hash function to free the ares hash key
 **
 *****************************************************************************/
void sdf_fn_aresKeyFree
(Sdf_ty_pvoid	pKey);
#endif /*SDF_ASYNC_DNS*/
#endif
#endif 

/* Ensure Names are not mangled by C++ compilers */
#ifdef __cplusplus
}
#endif
