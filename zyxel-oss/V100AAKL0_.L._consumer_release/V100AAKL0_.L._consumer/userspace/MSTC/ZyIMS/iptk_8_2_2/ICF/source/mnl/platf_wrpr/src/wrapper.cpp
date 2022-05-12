/*-----------------------------------------------------------------------------

*

* File name        : wrapper.cpp

*

* Purpose          : This file contains implementation of all exported 

*						functions

*						

*

* Revision History :

*

* Date         Name            Ref#            Comments

* --------     ------------    ----            ---------------

* 22-Aug-2005  Irfan Khan						Initial

*

* 06-Jan-2006  Jyoti Narula	  SPR 8063/ 9043	changes in mimRunAka() for AKA

*												readIpsecKeys() for reading 

*                                               IPSec Keys												

* 14-Jan-2006  Aman Arora     ICF rel 4.1       IPSEC changes for Symbian

* Copyright (c) 2006, Aricent.

*---------------------------------------------------------------------------*/



/* Includes*/



#include "wrp_prototypes.h"

#include <stdio.h>

#include <string.h>

#include <stdlib.h>



#ifdef __SYMBIAN32__

#include <unistd.h>

#endif



/*Constants*/

/*const unsigned int KIpAddressSize = 256;*/

const char KPCSCFS[] = "<PCSCF>";

const char KPCSCFE[] = "</PCSCF>";

const char KPRIDS[] = "<privateid>";

const char KPRIDE[] = "</privateid>";

const char KPBIDS[] = "<publicid>";

const char KPBIDE[] = "</publicid>";

const char KHMNWIPS[] = "<hnwip>";

const char KHMNWIPE[] = "</hnwip>";

const char KIMSIS[]  ="<imsi>";

const char KIMSIE[]  = "</imsi>";

const char KSIMS[]  ="<simType>";

const char KSIME[]  = "</simType>";

const char KSLFIPS[] = "<selfip>";

const char KSLFIPE[]  = "</selfip>";

const char KAPNS[] = "<apnlist>";

const char KAPNE[] = "</apnlist>";

const char KRESS[] = "<AKAres>";	

const char KRESE[] = "</AKAres>";

const char KAUTSS[] = "<AKAauts>";

const char KAUTSE[] = "</AKAauts>";

const char KENCALGO1S[] = "<encalgo1>";

const char KENCALGO1E[] = "</encalgo1>";

const char KENCALGO2S[] = "<encalgo2>";

const char KENCALGO2E[] = "</encalgo2>";

const char KAUTHALGO1S[] = "<authalgo1>";

const char KAUTHALGO1E[] = "</authalgo1>";

const char KAUTHALGO2S[] = "<authalgo2>";

const char KAUTHALGO2E[] = "</authalgo2>";

const char KIKS[] = "<IK>";

const char KIKE[] = "</IK>";

const char KCKS[] = "<CK>";;

const char KCKE[] = "</CK>";



#ifdef ICF_P_CSCF_DISCOVERY_ENABLED

const char KPCSCFDHCPS[] = "<PCSCF-DHCP>";

const char KPCSCFDHCPE[] = "</PCSCF-DHCP>";

const char KDHCPS[] = "<DHCP>";

const char KDHCPE[] = "</DHCP>";

const char KDNSPDPS[] = "<DNS-PDP>";

const char KDNSPDPE[] = "</DNS-PDP>";

const char KDNSDHCPS[] = "<DNS-DHCP>";

const char KDNSDHCPE[] = "</DNS-DHCP>";

#endif



/* # defines for wrapper.cfg params */

#define WRPR_PCSCF_PDP_ADDR   1

#define WRPR_PCSCF_DHCP_ADDR  2

#define WRPR_DHCP_ADDR        3

#define WRPR_DNS_PDP_ADDR     4

#define WRPR_DNS_DHCP_ADDR    5

#define WRPR_SELF_ADDR        6





/*Defines*/

#ifndef __SYMBIAN32__

#define EXPORT_C

#endif

/*Forward declarations*/

int ParseInputL(char*,int*,char*,char*, char*);



#ifndef ICF_QOS_STUBBED_OUT

icf_return_t (*smcWrprSend)(void * msg);



EXPORT_C icf_return_t icf_port_pltfm_init_send(icf_return_t (*asmcWrprSend)(void *msg))

{

	smcWrprSend=asmcWrprSend;



	return ICF_SUCCESS;

}

#endif



#ifdef ICF_VERIZON

char		 filename[100] ; 

#endif





#ifdef __SYMBIAN32__



#include "wrp_internal.h"



/*

#define ICF_NW_ACTIVATE		 1

#define ICF_QOS_SUPPORT		 1

#define ICF_QOS_STUBBED_OUT      1

#define ICF_IPSEC_STUBBED_OUT    1

*/



#include "icf_feature_flags.h"

#include "icf_defs.h"

#include "icf_common_types.h"

#include "icf_macro.h"

#include "icf_api.h"

#include "wrp_types.h"

#include "wrp_prototypes.h"





icf_return_t    icf_port_pltfm_pdp_addr_get(icf_string_st * p_pdp_addr);



icf_return_t    readIpsecKeys (icf_string_st *imsIk, icf_string_st *imsCk);



icf_return_t icf_port_pltfm_ipv4_sscanf(IN      icf_uint8_t      *pIP,

                                        OUT   icf_uint8_t  *p_octet_1,

                                        OUT   icf_uint8_t  *p_octet_2,

                                        OUT   icf_uint8_t  *p_octet_3,

                                        OUT     icf_uint8_t  *p_octet_4);

 

/* -----------------------------------------------------------------------------*/

/* init wrapper*/

/* Initializes the platform wrapper.*/

/* -----------------------------------------------------------------------------*/

/**/

EXPORT_C icf_return_t icf_port_pltfm_init(icf_msg_st	*p_port_req,

		 					 icf_port_pltfm_ret_val_t	*p_port_ret_val,

							 icf_msg_st					* p_port_resp,

							 icf_return_t (*aRsporIndCallBack)(icf_msg_st *pMsg))

{

    /*Creates the object of CMNLWrapper which will be global to the wrapper*/

    /*Object created in getGprsConnectorfromWrp returned, CMNLWrapper being a singleton class*/

    CMNLWrapper* wrapper = CMNLWrapper::NewL();

    if(wrapper != NULL)

	{

		/*smcRsporIndCallBack=aRsporIndCallBack;*/

		wrapper->smcRsporIndCallBack = aRsporIndCallBack;

        return ICF_SUCCESS;

	}

    else

	{

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

	}

}



/* -----------------------------------------------------------------------------*/

/* getGprsConnectorfromWrp*/

/* Initializes the platform wrapper.*/

/* -----------------------------------------------------------------------------*/

/**/

EXPORT_C icf_return_t icf_port_pltfm_getGprsConnectorfromWrp(CGPRSConnector **aGprsConnectorPtr)

{

    /*Creates the object of CMNLWrapper which will be global to the wrapper*/

    CMNLWrapper* wrapper = CMNLWrapper::NewL();

	if(	NULL != wrapper)

	{

		*aGprsConnectorPtr = wrapper->iGprsConnectorPtr;

		return ICF_SUCCESS;

	}

	else

	{



		return ICF_FAILURE;

	}

}



/* -----------------------------------------------------------------------------*/

/* Deinit wrapper*/

/* De Initializes the platform wrapper(Cleans up the memory allocated by init).*/

/* -----------------------------------------------------------------------------*/

/**/

EXPORT_C void icf_port_pltfm_deinit(void)

{

    /*Getting the pointer of CMNLWrapper's object from the TLS*/

    CMNLWrapper* wrapper = static_cast<CMNLWrapper*>(Dll::Tls());

    delete wrapper;

    Dll :: SetTls ( NULL );

}



/* ----------------------------------------------------------------------------*/

/* icf_port_pltfm_get_access_nw_info*/









/* Gets the P-NW Access information from the device*/

/* ----------------------------------------------------------------------------*/

/**/

EXPORT_C  icf_return_t icf_port_pltfm_get_access_nw_info(

							icf_msg_st					*p_port_req,

		 					icf_port_pltfm_ret_val_t	*p_port_ret_val,

							icf_msg_st					*p_port_resp)

{



	icf_port_pltfm_access_network_resp_st * p_access_nw_info=\

		(icf_port_pltfm_access_network_resp_st *)p_port_resp->payload;



    /*Getting the pointer of CNwMonitor's object from the TLS*/

    CNwMonitor* nwMonitor = static_cast<CNwMonitor*>((static_cast<CMNLWrapper*>

        (Dll::Tls()))->iLocMonitor);

    /*Network information			*/

    MBasicGsmPhoneNetwork::TCurrentNetworkInfo  nwinfo;

    /*Call to Basic GSM phone to get the info*/

    nwMonitor->iBPhone.GetCurrentNetworkInfo(nwinfo);

    

    /*Mobile Country Code*/

    p_access_nw_info->mcc = (TUint16)nwinfo.iNetworkInfo.iId.iMCC;

    /*Mobile Network Code*/

    p_access_nw_info->mnc = (TUint16)nwinfo.iNetworkInfo.iId.iMNC;

    /*Location area code*/

    p_access_nw_info->lac = (TUint16)nwinfo.iLocationAreaCode;



	/*Cell id*/

    TInt *cellid;

    cellid = (TInt*) p_access_nw_info->cell_id.str;

	p_access_nw_info->cell_id.str_len=2;



    *cellid = nwinfo.iCellId;	

    /*for GPRS 0 HARD CODE*/

    p_access_nw_info->nw_type = 0;

	

    return ICF_SUCCESS;

}



/* ----------------------------------------------------------------------------*/

/* smcPdpDefine*/

/* AT command based interface for PDP definition*/

/* NOTE : Not supported */

/* ----------------------------------------------------------------------------*/

/**/

EXPORT_C  icf_return_t smcPdpDefine(

								  icf_msg_st * 	/*pdpReq*/)

{

        return ICF_SUCCESS;



}




/* ----------------------------------------------------------------------------*/

/* smcPdpAddrGet*/

/* To get the IP Address corresponding to the PDP context Activated*/

/* ----------------------------------------------------------------------------*/

/**/

EXPORT_C icf_return_t icf_port_pltfm_pdp_addr_get(icf_string_st * p_pdp_addr)



{

	icf_return_t ret_val = ICF_FAILURE;

#ifdef IMS_CLIENT
	/*Getting the pointer of CMNLWrapper's object from the TLS*/

	CMNLWrapper* wrapper = static_cast<CMNLWrapper*>(Dll::Tls());

    HBufC* lHostName = HBufC::NewL(KIpAddressSize);

	TInt lResult = wrapper->iGprsConnectorPtr->RetrieveLocalIPAddress(lHostName->Des());

	_LIT(KNullIpAddress,"0.0.0.0");



	while(!lHostName->Compare(KNullIpAddress))

	{

		/*to confirm connection, i.e. retrieval of IP address*/

		ret_val=wrapper->iGprsConnectorPtr->RetrieveLocalIPAddress(lHostName->Des());

	}

    

   	if (HSS_SUCCESS == ret_val)

	{

		/*Allocating the memory to pass the IP addr to the caller*/

		TChar* ipAdd= new TChar[lHostName->Des().Size()];

		Mem::Copy(ipAdd,lHostName->Des().Ptr(),lHostName->Des().Size());

		p_pdp_addr->str_len=lHostName->Des().Size()/2+1;

		wcstombs((char *) p_pdp_addr->str,

			(const wchar_t *)ipAdd,

			lHostName->Des().Size());

		p_pdp_addr->str[p_pdp_addr->str_len-1]='\0';

        ret_val = ICF_SUCCESS;

	}

    return ret_val;
#else
    return ret_val;
#endif

}


#ifdef ICF_NW_ACTIVATE

/* ----------------------------------------------------------------------------*/

/* smcPdpActivate*/

/* Activates the PDP context*/

/* NOTE: Due to unavailability of RpacketContext and RpacketService,RConnection*/

/*			is used which ignores any input*/

/* ----------------------------------------------------------------------------*/

/**/

EXPORT_C icf_return_t icf_port_pltfm_pdp_activate_req (

						icf_msg_st					*p_port_req,

						icf_port_pltfm_ret_val_t	*p_port_ret_val,

						icf_msg_st					*p_port_resp)                                

                  

{

	icf_port_pltfm_pdp_def_req_st * p_pdp_def_req = \

		(icf_port_pltfm_pdp_def_req_st *) p_port_req->payload;



	icf_port_pltfm_pdp_def_resp_st * p_pdp_def_resp = \

		(icf_port_pltfm_pdp_def_resp_st *) p_port_resp->payload;

	/*Getting the pointer of CMNLWrapper's object from the TLS*/

    CMNLWrapper* wrapper = static_cast<CMNLWrapper*>(Dll::Tls());

	icf_return_t ret_val;



	if (ICF_PORT_PLTFM_PRIMARY_PDP_CNTXT == p_pdp_def_req->pdp_context_type)

	{

		icf_uint32_t apn_length = p_pdp_def_req->pdp_context_info.primary.apn.str_len;

		icf_uint8_t *apn = (uint8*)p_pdp_def_req->pdp_context_info.primary.apn.str;

		/*printf("going to call CreateConnection\n");*/

		/*sleep(10);*/

		

#ifdef __WINS__

		ret_val = wrapper->iGprsConnectorPtr->CreateConnection();

#else

		if( !apn_length )

			ret_val = wrapper->iGprsConnectorPtr->CreateConnection();

		else

		{

			TBuf8<32>		apn8( (const TUint8*)apn);

			TBuf16<32>      apn16;

			

			apn16.Copy(apn8);

			ret_val = wrapper->iGprsConnectorPtr->CreateConnection(apn16);

		}

#endif 

		if (HSS_SUCCESS == ret_val)

		{

			p_pdp_def_resp->context_id=p_pdp_def_req->pdp_context_info.primary.context_id;

			icf_port_pltfrm_get_pdp_addresses(p_pdp_def_resp);

			*p_port_ret_val=ICF_SUCCESS;

			printf("GPRS IP is %s \n",p_pdp_def_resp->pdp_addr.str);

			return ICF_SUCCESS;

		}

        else

		{

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

		}

	}

	else

	{

#ifdef ICF_QOS_SUPPORT

#ifndef  ICF_QOS_STUBBED_OUT

  			/* This is a secondary pdp context */

			icf_return_t                         ret_val = ICF_SUCCESS;

			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

			icf_gprs_sec_pdp_activate_req_st       *p_msg_payload = ICF_NULL;			

			

			api_len = sizeof(icf_api_header_st) + 

				sizeof(icf_gprs_sec_pdp_activate_req_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			if (ICF_NULL == p_msg)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				return ICF_FAILURE;

			}



				

			/* Fill the header of the message to be send */

			p_msg->hdr.api_id = ICF_GPRS_SEC_PDP_ACTIVATE_REQ;

			p_msg->hdr.source_id = ICF_MODULE_ICF;

			p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

			

			/* Fill the payload */

			p_msg_payload = (icf_gprs_sec_pdp_activate_req_st *)

				(p_msg->payload);

			p_msg_payload->context_id = p_pdp_def_req->pdp_context_info.secondary.context_id;

				

			if(ICF_PORT_PLTFM_PDP_PARAM_IDENT_AUTH_TOKEN==\

				p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter_identifier)

			{

				memcpy(p_msg_payload->auth_token.str,

					p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter.auth_token.octets,

					p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter.auth_token.num_of_octets);\

				p_msg_payload->auth_token.str_len=\

				p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter.auth_token.num_of_octets;

			}

			else 

				p_msg_payload->auth_token.str_len=0;

			

			/* Send to GPRS network */

			ret_val = smcWrprSend(p_msg);

			

			if(ICF_SUCCESS == ret_val)

			{

				/* Free memory */

				icf_wrpr_memfree(p_msg);

				return ICF_SUCCESS;

			}          

			else

			{

				/* Free memory */

				icf_wrpr_memfree(p_msg);

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				return ICF_FAILURE;

			}

#else

			/* This is a secondary pdp context */

			icf_return_t                         ret_val = ICF_SUCCESS;

			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

						

			api_len = sizeof(icf_api_header_st) + \

				sizeof(icf_port_pltfm_pdp_def_resp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			p_msg->hdr.api_id = ICF_PORT_PLTFM_SEC_PDP_ACT_RSP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_pdp_def_resp = (icf_port_pltfm_pdp_def_resp_st *) p_msg->payload;

			p_pdp_def_resp->context_id=p_pdp_def_req->pdp_context_info.secondary.context_id;

			p_pdp_def_resp->status=ICF_PORT_PLTFM_GPRS_NOERR;

			(((CMNLWrapper*)(Dll::Tls()))->smcRsporIndCallBack)(p_msg);

			icf_wrpr_memfree(p_msg);

#endif

#endif

	}

		*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

		return ret_val;

}





/* ----------------------------------------------------------------------------*/

/* smcPdpDeactivate*/

/* DeActivates the PDP context*/

/* ----------------------------------------------------------------------------*/

/**/

EXPORT_C icf_return_t icf_port_pltfm_pdp_deactivate_req (

					icf_msg_st* p_port_req,

					icf_port_pltfm_ret_val_t	*p_port_ret_val,

					icf_msg_st* p_port_resp)     

                                

{

	icf_port_pltfm_pdp_deact_rsp_st			*p_pdp_deact_rsp=ICF_NULL;

	icf_port_pltfm_pdp_deact_req_st			*p_pdp_deact_req=ICF_NULL;



	p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_req->payload;

	p_pdp_deact_rsp=(icf_port_pltfm_pdp_deact_rsp_st *) p_port_req->payload;



	/* If primary is getting deleted then remove network too */

	if(1==p_pdp_deact_req->context_id)

	{

		/*Getting the pointer of CMNLWrapper's object from the TLS*/

		CMNLWrapper* wrapper = static_cast<CMNLWrapper*>(Dll::Tls());



		/*Call to Terminate connection*/

	    wrapper->iGprsConnectorPtr->TerminateConnection();

	}

	else 

    {

#ifdef ICF_QOS_SUPPORT

#ifndef  ICF_QOS_STUBBED_OUT

		icf_return_t                         ret_val = ICF_SUCCESS;

		icf_msg_st                           *p_msg = ICF_NULL;

		icf_uint8_t                          api_len;

		icf_error_t							ecode = ICF_NULL;

		icf_gprs_pdp_deactivate_req_st			*p_msg_payload = ICF_NULL;

		

		api_len = sizeof(icf_api_header_st) + 

			sizeof(icf_gprs_pdp_deactivate_req_st);

		

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);

		

		if (ICF_NULL == p_msg)

		{

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

			return ICF_FAILURE;

		}

		

		

		/* Fill the header of the message to be send */

		p_msg->hdr.api_id = ICF_GPRS_PDP_DEACTIVATE_REQ;

		p_msg->hdr.source_id = ICF_MODULE_ICF;

		p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

		

		/* Fill the payload */

		p_msg_payload = (icf_gprs_pdp_deactivate_req_st *)

			(p_msg->payload);

		p_msg_payload->context_id = p_pdp_deact_req->context_id;

		

		/* Send to GPRS network */

		ret_val = smcWrprSend(p_msg);

		

		if(ICF_SUCCESS == ret_val)

		{

			/* Free memory */

			icf_wrpr_memfree(p_msg);

			return ICF_SUCCESS;

		}          

		else

		{

			/* Free memory */

			icf_wrpr_memfree(p_msg);

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

			return ICF_FAILURE;

		}

#else

			/* This is a deactivation request */

			icf_return_t                         ret_val = ICF_SUCCESS;



#ifdef ICF_REL_OLD

			/* ICF does not wait for de-activate response now - changed in Rel 6.0*/



			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

								

			api_len = sizeof(icf_api_header_st) + \

					  sizeof(icf_port_pltfm_pdp_deact_rsp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			p_msg->hdr.api_id = ICF_PORT_PLTFM_SEC_PDP_ACT_RSP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_req->payload;

			p_pdp_deact_rsp=(icf_port_pltfm_pdp_deact_rsp_st *) p_msg->payload;

						

			p_pdp_deact_rsp->context_id=p_pdp_deact_req->context_id;

			p_pdp_deact_rsp->status=ICF_PORT_PLTFM_GPRS_NOERR;



			(((CMNLWrapper*)(Dll::Tls()))->smcRsporIndCallBack)(p_msg);

			icf_wrpr_memfree(p_msg);

#endif

#endif

#endif

	}

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ICF_SUCCESS;

}



/* ----------------------------------------------------------------------------*/

/* icf_port_pltfm_get_host_ip_on_symbian*/

/* DNS service- Gets the IP addr from FQDN */

/* ----------------------------------------------------------------------------*/

/**/



EXPORT_C icf_return_t icf_port_pltfm_get_host_ip_on_symbian(

											   icf_msg_st	*p_port_req,

											   icf_msg_st	*p_port_resp)

{

	icf_port_pltfm_host_to_ip_addr_resp_st *p_host_to_ip_rsp=\

		(icf_port_pltfm_host_to_ip_addr_resp_st*) p_port_resp->payload;



	icf_port_pltfm_host_to_ip_addr_req_st *p_host_to_ip_req=\

		(icf_port_pltfm_host_to_ip_addr_resp_st*) p_port_req->payload;



	/*Getting the pointer to the CGPRSConnector object from TLS	*/

    CGPRSConnector *connector = static_cast<CGPRSConnector*>((static_cast

							 <CMNLWrapper*>(Dll::Tls()))->iGprsConnectorPtr);

  

    TBuf<KIpAddressSize> hostName,ipAddr;

    hostName.FillZ();

    ipAddr.FillZ();

    hostName.Copy((TUint16*)p_host_to_ip_req->str);

    hostName.SetLength(p_host_to_ip_req->str_len/2);

    TInetAddr addr;

    /*Get the IP addr in addr*/

    if(0==connector->HostnmtoIPAddrL(hostName,addr))

	{

		p_host_to_ip_rsp->str_len=0;

		return ICF_FAILURE;

	}

    addr.Output(ipAddr);

    /*Allocating memory and setting the values to pass to the caller*/

    TChar* ipaddr= new TChar[ipAddr.Size()];

	p_host_to_ip_rsp->str_len=ipAddr.Size()/2+1;

	wcstombs((char *) p_host_to_ip_rsp->str,

			 (const wchar_t *)ipAddr.Ptr(),

			 ipAddr.Size());

	p_host_to_ip_rsp->str[p_host_to_ip_rsp->str_len]='\0';



	return ICF_SUCCESS;

}

#endif/*#ifdef ICF_NW_ACTIVATE*/



#ifdef ICF_QOS_SUPPORT

EXPORT_C icf_return_t icf_port_pltfm_qos_reserve_req_non_ims(

						icf_msg_st					*p_port_req,

						icf_port_pltfm_ret_val_t	*p_port_ret_val,

						icf_msg_st					*p_port_resp)                                

                  

{

	icf_return_t                         ret_val = ICF_SUCCESS;

	icf_port_pltfm_qos_res_non_ims_req_st * p_qos_res_req = \

		(icf_port_pltfm_qos_res_non_ims_req_st *) p_port_req->payload;



	icf_port_pltfm_qos_res_non_ims_resp_st * p_qos_res_resp = \

		(icf_port_pltfm_qos_res_non_ims_resp_st *) p_port_resp->payload;



	/*Getting the pointer of CMNLWrapper's object from the TLS*/

    CMNLWrapper* wrapper = static_cast<CMNLWrapper*>(Dll::Tls());

			

#ifdef  ICF_QOS_STUBBED_OUT

  			

			/* This is a secondary pdp context */

			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							 ecode = ICF_NULL;

						

			api_len = sizeof(icf_api_header_st) + \

				sizeof(icf_port_pltfm_qos_res_non_ims_resp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			p_msg->hdr.api_id = ICF_PORT_PLTFM_QOS_RES_NON_IMS_RESP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_qos_res_resp = (icf_port_pltfm_qos_res_non_ims_resp_st *) p_msg->payload;

			p_qos_res_resp->id=p_qos_res_req->id;

			p_qos_res_resp->status=ICF_PORT_PLTFM_GPRS_NOERR;

			(((CMNLWrapper*)(Dll::Tls()))->smcRsporIndCallBack)(p_msg);

			icf_wrpr_memfree(p_msg);

#endif

	

		*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

		return ret_val;

}





EXPORT_C icf_return_t icf_port_pltfm_qos_release_req_non_ims (

					icf_msg_st* p_port_req,

					icf_port_pltfm_ret_val_t	*p_port_ret_val,

					icf_msg_st* p_port_rsp)     

                                

{

	icf_port_pltfm_qos_rel_non_ims_rsp_st			*p_qos_rel_rsp=ICF_NULL;

	icf_port_pltfm_qos_rel_non_ims_req_st			*p_qos_rel_req=ICF_NULL;



	p_qos_rel_req=(icf_port_pltfm_qos_rel_non_ims_req_st *) p_port_req->payload;

	p_qos_rel_rsp=(icf_port_pltfm_qos_rel_non_ims_rsp_st *) p_port_rsp->payload;



	/* This is a deactivation request */

	/* ICF does not wait for de-activate response */



	*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

	return ICF_SUCCESS;

}

#endif /*#ifdef ICF_QOS_SUPPORT*/





/* ************************************************************************** */

/* IP Security related functions end */

/* ************************************************************************** */



#ifdef ICF_IPSEC_ENABLED

/* Function invoked by ICF to create SA for encryption & authentication */

EXPORT_C icf_return_t	icf_port_pltfm_sa_create_req(

				icf_msg_st					*p_port_req,

				icf_port_pltfm_ret_val_t	*p_port_ret_val,

				icf_msg_st					*p_port_resp)   

{

	icf_port_pltfm_sa_cmd_resp_st			*p_sa_cmd_rsp=ICF_NULL;

	icf_port_pltfm_sa_create_req_st			*p_sa_create_req=ICF_NULL;



	p_sa_create_req=(icf_port_pltfm_sa_create_req_st *) p_port_req->payload;

	p_sa_cmd_rsp=(icf_port_pltfm_sa_cmd_resp_st *)	p_port_resp->payload;



#ifndef ICF_IPSEC_STUBBED_OUT

    icf_return_t					ret_val = ICF_SUCCESS;

    icf_msg_st					*p_msg = ICF_NULL;

    icf_uint32_t					api_len = ICF_NULL;

    icf_error_t					ecode = ICF_NULL;

	icf_port_pltfm_sa_create_req_st *p_msg_payload;

        

    api_len = sizeof(icf_api_header_st) + sizeof(icf_port_pltfm_sa_create_req_st);

				

	p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);



    if (ICF_NULL == p_msg)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

				

    /* Fill the header of the message to be send */

    p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_CREATE_REQ;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

    p_msg->hdr.api_length = api_len;

    p_msg->hdr.call_id = 0;

    p_msg->hdr.version = ICF_VERSION_1_0;

    p_msg->hdr.app_id = 0;

    

    /* Fill the payload */

    p_msg_payload = (icf_port_pltfm_sa_create_req_st *)(p_msg->payload);

    p_msg_payload->spi=p_sa_create_req->spi;

	p_msg_payload->lifetime = p_sa_create_req->lifetime;

    

    /* Send to GPRS network */

    smcWrprSend(p_msg);

    /* This sleep is necessary otherwise ICF will send 4 requests to IAS 

     * quickly and manual operation of IAS may not work correctly */

    _sleep(1000);

    if(ICF_SUCCESS == ret_val)

    {

        /* Free memory */

        icf_wrpr_memfree(p_msg);

        return ICF_SUCCESS;

    }          

    else

    {

        /* Free memory */

        icf_wrpr_memfree(p_msg);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

	icf_return_t                         ret_val = ICF_SUCCESS;

		icf_msg_st                           *p_msg = ICF_NULL;

		icf_uint8_t                          api_len;

		icf_error_t							ecode = ICF_NULL;

	

						

		api_len = sizeof(icf_api_header_st) + \

				  sizeof(icf_port_pltfm_sa_cmd_resp_st);

			

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);



			



		p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_CREATE_RESP;

		p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.destination_id = ICF_MODULE_ICF;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;

		

		p_sa_cmd_rsp->spi=p_sa_create_req->spi;

		p_sa_cmd_rsp->response=ICF_SUCCESS;

		(*smcRsporIndCallBack)(p_msg);

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ICF_SUCCESS;

}



/* Function invoked by ICF to modify duration of an SA */

EXPORT_C icf_return_t	icf_port_pltfm_sa_modify_req(

								icf_msg_st					*p_port_req,

								icf_port_pltfm_ret_val_t	*p_port_ret_val,

								icf_msg_st					*p_port_resp)

{



icf_port_pltfm_sa_cmd_resp_st			*p_sa_cmd_rsp=ICF_NULL;

	icf_port_pltfm_sa_modify_req_st			*p_sa_modify_req=ICF_NULL;



	p_sa_modify_req=(icf_port_pltfm_sa_modify_req_st *) p_port_req->payload;

	p_sa_cmd_rsp=(icf_port_pltfm_sa_cmd_resp_st *)	    p_port_resp->payload;



#ifndef ICF_IPSEC_STUBBED_OUT

    icf_return_t					ret_val = ICF_SUCCESS;

    icf_msg_st					*p_msg = ICF_NULL;

    icf_uint32_t					api_len = ICF_NULL;

    icf_error_t					ecode = ICF_NULL;

    icf_port_pltfm_sa_modify_req_st	*p_msg_payload = ICF_NULL;

    

    api_len = sizeof(icf_api_header_st) + sizeof(icf_port_pltfm_sa_modify_req_st);

				

	p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);

    

    if (ICF_NULL == p_msg)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

				

    /* Fill the header of the message to be send */

    p_msg->hdr.api_id = ICF_WRPR_SA_MODIFY_REQ;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

    p_msg->hdr.api_length = api_len;

    p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

    p_msg->hdr.version = ICF_VERSION_1_0;

    p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

    

    /* Fill the payload */

    p_msg_payload = (icf_port_pltfm_sa_modify_req_st *)(p_msg->payload);

    p_msg_payload->spi = p_sa_modify_req->spi;

    p_msg_payload->lifetime = p_msg_payload->lifetime;



    /* Send to GPRS network */

    smcWrprSend(p_msg);

    /* This sleep is necessary otherwise ICF will send 4 requests to IAS 

     * quickly and manual operation of IAS may not work correctly */

    _sleep(1000);

    if(ICF_SUCCESS == ret_val)

    {

        /* Free memory */

        icf_wrpr_memfree(p_msg);

        return ICF_SUCCESS;

    }          

    else

    {

        /* Free memory */

        icf_wrpr_memfree(p_msg);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

#else

		icf_return_t                         ret_val = ICF_SUCCESS;

		icf_msg_st                           *p_msg = ICF_NULL;

		icf_uint8_t                          api_len;

		icf_error_t							ecode = ICF_NULL;

		

						

		api_len = sizeof(icf_api_header_st) + \

				  sizeof(icf_port_pltfm_sa_cmd_resp_st);

			

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);

				

		p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_MODIFY_RESP;

		p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.destination_id = ICF_MODULE_ICF;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;

		

		p_sa_cmd_rsp->spi=p_sa_modify_req->spi;

		p_sa_cmd_rsp->response=ICF_SUCCESS;

		(((CMNLWrapper*)(Dll::Tls()))->smcRsporIndCallBack)(p_msg);

		icf_wrpr_memfree(p_msg);

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ICF_SUCCESS;

}





/* Function invoked by ICF to delete an SA */

EXPORT_C icf_return_t	icf_port_pltfm_sa_delete_req(

		icf_msg_st					*p_port_req,

		icf_port_pltfm_ret_val_t	*p_port_ret_val,

		icf_msg_st					* p_port_resp)

{

	icf_port_pltfm_sa_cmd_resp_st			*p_sa_cmd_rsp=ICF_NULL;

	icf_port_pltfm_sa_delete_req_st			*p_sa_delete_req=ICF_NULL;

	

	p_sa_delete_req=(icf_port_pltfm_sa_delete_req_st *) p_port_req->payload;

	p_sa_cmd_rsp=(icf_port_pltfm_sa_cmd_resp_st *)	p_port_resp->payload;



#ifndef ICF_IPSEC_STUBBED_OUT

    icf_return_t					ret_val = ICF_SUCCESS;

    icf_msg_st					*p_msg = ICF_NULL;

    icf_uint32_t					api_len = ICF_NULL;

    icf_error_t					ecode = ICF_NULL;

    icf_port_pltfm_sa_delete_req_st	*p_msg_payload = ICF_NULL;

    

    api_len = sizeof(icf_api_header_st) + sizeof(icf_port_pltfm_sa_delete_req_st);

				

	p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);

    

    if (ICF_NULL == p_msg)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

				

    /* Fill the header of the message to be send */

    p_msg->hdr.api_id = ICF_WRPR_SA_DELETE_REQ;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

    p_msg->hdr.api_length = api_len;

    p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

    p_msg->hdr.version = ICF_VERSION_1_0;

    p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

    

    /* Fill the payload */

    p_msg_payload = (icf_port_pltfm_sa_delete_req_st *)(p_msg->payload);

    p_msg_payload->spi = p_sa_delete_req->spi;

    

    /* Send to GPRS network */

    smcWrprSend(p_msg);

    /* This sleep is necessary otherwise ICF will send 4 requests to IAS 

     * quickly and manual operation of IAS may not work correctly */

    _sleep(1000);

    if(ICF_SUCCESS == ret_val)

    {

        /* Free memory */

        icf_wrpr_memfree(p_msg);

        return ICF_SUCCESS;

    }          

    else

    {

        /* Free memory */

        icf_wrpr_memfree(p_msg);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

#else

  	icf_return_t                         ret_val = ICF_SUCCESS;

		icf_msg_st                           *p_msg = ICF_NULL;

		icf_uint8_t                          api_len;

		icf_error_t							ecode = ICF_NULL;

		

						

		api_len = sizeof(icf_api_header_st) + \

				  sizeof(icf_port_pltfm_sa_cmd_resp_st);

			

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);

		p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_DELETE_RESP;

		p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.destination_id = ICF_MODULE_ICF;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;

	

		p_sa_cmd_rsp->spi=p_sa_delete_req->spi;

		p_sa_cmd_rsp->response=ICF_SUCCESS;

		(((CMNLWrapper*)(Dll::Tls()))->smcRsporIndCallBack)(p_msg);

		icf_wrpr_memfree(p_msg);

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ICF_SUCCESS;

}



#endif/*#ifdef ICF_IPSEC_ENABLED*/



/* ************************************************************************** */

/* IP Security related functions end */

/* ************************************************************************** */







/* -----------------------------------------------------------------------------*/

/* E32Dll()*/

/* Entry point function for Symbian Apps.*/

/* -----------------------------------------------------------------------------*/

/**/

GLDEF_C TInt E32Dll( TDllReason /*aReason*/ )

{

    /* DLL entry point, return that everything is ok*/

    return KErrNone;

}





#endif /*__SYMBIAN32__*/



/*For window build*/

#if defined(WRAPPER_WINDOWS)|| defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

#include "icf_feature_flags.h"

#include "icf_defs.h"

#include "icf_common_types.h"

#include "icf_macro.h"

#include "icf_api.h"

#include "icf_port_prototypes.h"

#ifndef WRAPPER_VXWORKS    

#include <memory.h>

#endif

/*#include <iostream.h>*/

#if !defined(WRAPPER_LINUX) && !defined(WRAPPER_VXWORKS) 

#include <new>

#endif

#include "wrp_types.h"

#include "wrp_prototypes.h"

    

#ifdef ICF_PORT_WINMOB



/*#include "stdafx.h"*/

#include "winsock.h"



/*/The following Defines are only on Smartphone and Pocket PC Phone Edition*/

#include <objbase.h>

#include <initguid.h>

#include <connmgr.h>   /*This Include contains all defines for the */

/*connection manager*/



#endif



icf_return_t (*smcRsporIndCallBack)(icf_msg_st *pMsg);



icf_return_t    icf_port_pltfm_pdp_addr_get(icf_string_st * p_pdp_addr);



icf_return_t    readIpsecKeys (icf_string_st *imsIk, icf_string_st *imsCk);



icf_return_t icf_port_pltfm_ipv4_sscanf(IN      icf_uint8_t      *pIP,

                                        OUT   icf_uint8_t  *p_octet_1,

                                        OUT   icf_uint8_t  *p_octet_2,

                                        OUT   icf_uint8_t  *p_octet_3,

                                        OUT     icf_uint8_t  *p_octet_4);





#if !defined(WRAPPER_LINUX) && !defined(WRAPPER_VXWORKS) 

extern "C"

{

#endif

	extern icf_glb_pdb_st   *p_persistent_glb_pdb;

#if !defined(WRAPPER_LINUX) && !defined(WRAPPER_VXWORKS)

}

#endif



#ifdef ICF_PORT_WINMOB

icf_return_t	icf_port_pltfm_pdp_addr_get(

						icf_string_st * p_pdp_addr)

{

#ifdef IMS_CLIENT
	HANDLE phWebConnection = p_persistent_glb_pdb->connection_handle;

	icf_uint8_t    szHostname[255];      

	icf_uint8_t   IP[17];

	HOSTENT *pHostEnt=NULL;

	icf_uint16_t      nAdapter = 0;

	IN_ADDR *tsim=NULL;

	icf_boolean_t    tried2Connect=ICF_FALSE;



    IP[0]=0;   /* Clear the IP Address*/



	GUID guid;



    if (p_pdp_addr != ICF_NULL) p_pdp_addr->str[0]=0;



tryagain:

	nAdapter=0;

    gethostname( (icf_int8_t *)szHostname, sizeof( szHostname ));

    pHostEnt = gethostbyname( (icf_int8_t *)szHostname );

    while ( pHostEnt!=NULL && pHostEnt->h_addr_list[nAdapter] )      

	{

		/* in case a device has multiple ethernet cards*/

		/* i.e. 802.11, Bluetooth, USB-Cradle*/

		/* we need to go though all pHostEnt->h_addr_list[nAdapter]*/

		tsim=(IN_ADDR *)pHostEnt->h_addr_list[nAdapter];

		if (/*tsim->S_un.S_un_b.s_b1==192 || */

			/*tsim->S_un.S_un_b.s_b1==169 || */

			tsim->S_un.S_un_b.s_b1==127 || 

			tsim->S_un.S_un_b.s_b1==255)

		{

			/* If you want to make sure you have a real Internet*/

			/* connection you cannot bet on IpAddresses starting with*/

			/* 127 and 255. 192 and 169 are local IP addresses and*/

			/* might be routed or proxied*/

			nAdapter++;      

		}

		else

		{

			sprintf((icf_int8_t *)IP,"%d.%d.%d.%d",

				tsim->S_un.S_un_b.s_b1,

				tsim->S_un.S_un_b.s_b2,

				tsim->S_un.S_un_b.s_b3,

				tsim->S_un.S_un_b.s_b4);



			if (p_pdp_addr != ICF_NULL)

				sprintf((icf_int8_t *)p_pdp_addr->str,"%s",IP);   /*Return the IP address*/

			tried2Connect = ICF_TRUE;

			ICF_PRINT((" \n IP IS %s",p_pdp_addr->str));

			break;

		}

	}



	/* the next lines only work with Pocket PC Phone */

	/* and Smartphone*/

/*#if (WIN32_PLATFORM_PSPC>300 || WIN32_PLATFORM_WFSP )*/

/* Pocket PC Phone Edition has set WIN32_PLATFORM_PSPC to 310*/

   if (IP[0]==0 && tried2Connect == ICF_FALSE)

   {



	   icf_port_printf(" \n ### INSIDE CONNECTION ");

		   /* Here comes the main code:*/

		   /* First we check if we might have a connection*/

		   DWORD  pdwStatus;

		   ConnMgrConnectionStatus(&phWebConnection,&pdwStatus);



		   if (pdwStatus==CONNMGR_STATUS_CONNECTED)

		   {

			   icf_port_printf(" \n ### STALE CONNECTION ");

			   /*We are already connected!*/

			   /*This code should never run since we should*/

			   /*have a valid IP already.*/

			   /*If you still get here, you probably have*/

			   /*stale connection. */

		   }

		   else

		   {

			   icf_port_printf(" \n ### TRYING FOR CONNECTION ");



			   /*We are not connected, so lets try:*/

			   /*The CONNECTIONINFO is the structure that */

			   /*tells Connection Manager how we want*/

			   /*to connect*/

			   CONNMGR_CONNECTIONINFO sConInfo;

			   memset(&sConInfo,0,

				   sizeof(CONNMGR_CONNECTIONINFO));

			   sConInfo.cbSize=sizeof(CONNMGR_CONNECTIONINFO);

			   /* We want to use the "guidDestNet" parameter*/

			   sConInfo.dwParams=CONNMGR_PARAM_GUIDDESTNET;

			   /* This is the highest data priority.*/

			   sConInfo.dwPriority=

				   CONNMGR_PRIORITY_USERINTERACTIVE;

			   sConInfo.dwFlags=0;

			   /* Lets be nice and share the connection with*/

			   /* other applications*/

			   sConInfo.bExclusive=FALSE;

			   sConInfo.bDisabled=FALSE;



			   if (S_OK == ConnMgrMapConRef ( ConRefType_NAP, _T("Access point name"), &guid))

				{

					sConInfo.guidDestNet = guid;

				}

				else

				{

					sConInfo.guidDestNet = IID_DestNetInternet;

				}



			   /*sConInfo.guidDestNet=IID_DestNetInternet;*/



			   if (ConnMgrEstablishConnectionSync (

				   &sConInfo,&phWebConnection,60000,&pdwStatus)

				   ==S_OK)

			   {

				   if(CONNMGR_STATUS_CONNECTED == pdwStatus)

				   {

					   icf_port_printf(" \n ### CONNECTION SUCCESSFUL");

						p_persistent_glb_pdb->connection_handle = phWebConnection;



					   /*We are successfully connected!*/

					   /*Now lets try to get the new IP address:*/

					   tried2Connect=TRUE;

					   goto tryagain;

				   }

			   }

			   else

			   {

				   icf_port_printf(" \n ### CONNECTION FAILED");



				   tried2Connect= ICF_FALSE;

				   /*Doh! Connection failed!*/

			   }

		   }

	   

   }

   else

   {

	   icf_port_printf(" \n ### INSIDE Alredy CONNECTION ");

		   /* Here comes the main code:*/

		   /* First we check if we might have a connection*/

		   DWORD  pdwStatus;

		   ConnMgrConnectionStatus(&phWebConnection,&pdwStatus);

		   if(NULL != phWebConnection)
			icf_port_printf(" \n NonNull ConnectionHandle  ");
		   else 
			icf_port_printf(" \n Null ConnectionHandle  ");

		   if (pdwStatus==CONNMGR_STATUS_CONNECTED)

		   {

			   p_persistent_glb_pdb->connection_handle = phWebConnection;

			   /*We are already connected!*/

			   /*This code should never run since we should*/

			   /*have a valid IP already.*/

			   /*If you still get here, you probably have*/

			   /*stale connection. */

		   }

		   else

		   {

			   icf_port_printf(" \n ### No CONNECTION ");

		   }

   }

/*#endif*/

   if(ICF_FALSE == tried2Connect)

   {

	   	return ICF_FAILURE;

   }

   else

   {

	   	return ICF_SUCCESS;

   }
#else
  return ICF_SUCCESS; 
#endif
}


#else



icf_return_t	icf_port_pltfm_pdp_addr_get(

						icf_string_st * p_pdp_addr)

{
#ifdef IMS_CLIENT
	int len = 0;

	FILE *config;

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	char *ipadd = ICF_NULL;

	ipadd = malloc(20 * sizeof(char));

#else

    char* ipadd= new char[20];

#endif



    memset(ipadd,0,20);



#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

	if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

    {

		while (!feof(config))

        {

                char buf[100];

		int res = 0;

                memset(&buf,0,100);

                if( fgets( buf, 100, config ) == NULL)

                    return ICF_FAILURE;

                

                res = ParseInputL(ipadd,&len,(char*)KSLFIPS,(char*)KSLFIPE,buf);

                if(res == 0)

                    break;

        }

            

            fclose( config );

    }



	p_pdp_addr->str_len=len;



	memcpy(p_pdp_addr->str,

			ipadd,

			len);		

	p_pdp_addr->str[p_pdp_addr->str_len]='\0';



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(ipadd);

#else

	delete [] ipadd;

#endif



	return ICF_SUCCESS;
#else
        p_pdp_addr = p_pdp_addr;
        return ICF_SUCCESS;
#endif
}

#endif





EXPORT_C icf_return_t icf_port_pltfm_init(icf_msg_st*p_port_req,

										icf_port_pltfm_ret_val_t	*p_port_ret_val,

										icf_msg_st* p_port_resp,

										icf_return_t (*aRsporIndCallBack)(icf_msg_st *pMsg))

{

   /*To remove warnings */

     

   p_port_req=p_port_req;

   p_port_resp=p_port_resp;

  		smcRsporIndCallBack=aRsporIndCallBack;

		*p_port_ret_val=ICF_PORT_PLTFM_SUCCESS;



#ifdef ICF_VERIZON

	{

		HKEY key;

		unsigned char				filename_unicode[256],filepath[256];

		unsigned long				len=sizeof(filename_unicode);

		if( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"SOFTWARE\\FSS_Microphone",0,KEY_READ, &key ))

		{

			return FALSE;

		}



		RegQueryValueEx( key, L"Path", NULL, NULL, filename_unicode, &len );

		len = 0;

		len = wcslen((WCHAR*)filename_unicode);

		wcstombs((char*)filepath,(WCHAR*)filename_unicode,len + 1);

		

		strcpy(filename,(char*)filepath);

		strcat(filename,"/cfg/wrapper.cfg");

	}

#endif

	

        return ICF_SUCCESS;

}

    

    /* -----------------------------------------------------------------------------*/

    /* Deinit wrapper*/

    /* De Initializes the platform wrapper(Cleans up the memory allocated by init).*/

    /* -----------------------------------------------------------------------------*/

    /**/

    void icf_port_pltfm_deinit(void)

    {



    }

    

      /************************************************************************************/

    

	icf_return_t icf_port_pltfm_get_access_nw_info(

				 icf_msg_st* p_port_req,

				 icf_port_pltfm_ret_val_t	*p_port_ret_val,

				 icf_msg_st* p_port_resp)

    {

		icf_port_pltfm_access_network_resp_st * p_access_nw_info=\

			(icf_port_pltfm_access_network_resp_st *)(p_port_resp->payload);

        int *cellid = (int*) p_access_nw_info->cell_id.str;

		p_access_nw_info->cell_id.str_len=2;



 	/*To remove warnings */

	   p_port_req=p_port_req;

	   p_port_resp=p_port_resp;



           *cellid = 71;

           p_access_nw_info->lac	 =	11;

	   p_access_nw_info->mcc	 =	248;

	   p_access_nw_info->mnc	 =	323;

	   p_access_nw_info->nw_type =	0;

           *p_port_ret_val=ICF_SUCCESS;



	return ICF_SUCCESS;

    }

    

    

/************************************************************************************/

#ifdef ICF_NW_ACTIVATE

icf_return_t icf_port_pltfm_pdp_activate_req (

							icf_msg_st					*p_port_req,

							icf_port_pltfm_ret_val_t	*p_port_ret_val,

							icf_msg_st					*p_port_resp)                                

 {

	icf_port_pltfm_pdp_def_req_st * p_pdp_def_req = \

		(icf_port_pltfm_pdp_def_req_st *) p_port_req->payload;



	icf_port_pltfm_pdp_def_resp_st * p_pdp_def_resp = \

		(icf_port_pltfm_pdp_def_resp_st *) p_port_resp->payload;





	if (ICF_PORT_PLTFM_PRIMARY_PDP_CNTXT == p_pdp_def_req->pdp_context_type)

	{

		p_pdp_def_resp->context_id=p_pdp_def_req->pdp_context_info.primary.context_id;

		icf_port_pltfrm_get_pdp_addresses(p_pdp_def_resp);

		*p_port_ret_val=ICF_SUCCESS;

		return ICF_SUCCESS;

	}

	else

	{

#ifdef ICF_QOS_SUPPORT			

#ifndef  ICF_QOS_STUBBED_OUT

  			/* This is a secondary pdp context */

			icf_return_t                         ret_val = ICF_SUCCESS;

			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint32_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

			icf_gprs_sec_pdp_activate_req_st       *p_msg_payload = ICF_NULL;			

			

			api_len = sizeof(icf_api_header_st) + 

				sizeof(icf_gprs_sec_pdp_activate_req_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			if (ICF_NULL == p_msg)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				return ICF_FAILURE;

			}



				

			/* Fill the header of the message to be send */

			p_msg->hdr.api_id = ICF_GPRS_SEC_PDP_ACTIVATE_REQ;

			p_msg->hdr.source_id = ICF_MODULE_ICF;

			p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

			

			/* Fill the payload */

			p_msg_payload = (icf_gprs_sec_pdp_activate_req_st *)

				(p_msg->payload);

			p_msg_payload->context_id = p_pdp_def_req->pdp_context_info.secondary.context_id;

				

			if(ICF_PORT_PLTFM_PDP_PARAM_IDENT_AUTH_TOKEN==\

				p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter_identifier)

			{

				memcpy(p_msg_payload->auth_token.str,

					p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter.auth_token.octets,

					p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter.auth_token.num_of_octets);\

				p_msg_payload->auth_token.str_len=\

				p_pdp_def_req->pdp_context_info.secondary.tft.params_list[0].parameter.auth_token.num_of_octets;

			}

			else 

				p_msg_payload->auth_token.str_len=0;

			

			/* Send to GPRS network */

			ICF_SEND(p_persistent_glb_pdb,p_msg,ret_val,&ecode)

			

			if(ICF_SUCCESS == ret_val)

			{

				/* Free memory */

				icf_wrpr_memfree(p_msg);

				return ICF_SUCCESS;

			}          

			else

			{

				/* Free memory */

				icf_wrpr_memfree(p_msg);

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				return ICF_FAILURE;

			}

#else

			/* This is a secondary pdp context */

			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint32_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

						

			api_len = sizeof(icf_api_header_st) + \

				sizeof(icf_port_pltfm_pdp_def_resp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			p_msg->hdr.api_id = ICF_PORT_PLTFM_SEC_PDP_ACT_RSP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_pdp_def_resp = (icf_port_pltfm_pdp_def_resp_st *) p_msg->payload;

			p_pdp_def_resp->context_id=p_pdp_def_req->pdp_context_info.secondary.context_id;

			p_pdp_def_resp->status=ICF_PORT_PLTFM_GPRS_NOERR;

/* for failure scenario in stubbed mode */

/*p_pdp_def_resp->status=ICF_PORT_PLTFM_GPRS_SYTEM_ERR;*/

			(*smcRsporIndCallBack)(p_msg);



			/* Free memory */

			icf_wrpr_memfree(p_msg);

#endif

#endif

	}

		*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

		return ICF_SUCCESS;

}

    

    /************************************************************************************/



#if defined ICF_PORT_WINMOB

icf_return_t	icf_port_pltfm_pdp_deactivate_req (

				icf_msg_st* p_port_req,

				icf_port_pltfm_ret_val_t	*p_port_ret_val,

				icf_msg_st* p_port_resp)     

                                

{

	icf_port_pltfm_pdp_deact_rsp_st			*p_pdp_deact_rsp=ICF_NULL;

	icf_port_pltfm_pdp_deact_req_st			*p_pdp_deact_req=ICF_NULL;



	p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_req->payload;

	p_pdp_deact_rsp=(icf_port_pltfm_pdp_deact_rsp_st *) p_port_req->payload;



	/* If primary is getting deleted then remove network too */

	if(1==p_pdp_deact_req->context_id)

	{

		HANDLE pWebConnection = p_persistent_glb_pdb->connection_handle;



		ConnMgrReleaseConnection(pWebConnection,ICF_TRUE);

	}

#ifdef ICF_QOS_SUPPORT

#ifndef  ICF_QOS_STUBBED_OUT

		icf_return_t                         ret_val = ICF_SUCCESS;

		icf_msg_st                           *p_msg = ICF_NULL;

		icf_uint8_t                          api_len;

		icf_error_t							ecode = ICF_NULL;

		icf_gprs_pdp_deactivate_req_st			*p_msg_payload = ICF_NULL;

		

		api_len = sizeof(icf_api_header_st) + 

			sizeof(icf_gprs_pdp_deactivate_req_st);

		

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);

		

		if (ICF_NULL == p_msg)

		{

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

			return ICF_FAILURE;

		}

		

		

		/* Fill the header of the message to be send */

		p_msg->hdr.api_id = ICF_GPRS_PDP_DEACTIVATE_REQ;

		p_msg->hdr.source_id = ICF_MODULE_ICF;

		p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

		

		/* Fill the payload */

		p_msg_payload = (icf_gprs_pdp_deactivate_req_st *)

			(p_msg->payload);

		p_msg_payload->context_id = p_pdp_deact_req->context_id;

		

		/* Send to GPRS network */

		ICF_SEND(p_persistent_glb_pdb,p_msg,ret_val,&ecode)

		

		if(ICF_SUCCESS == ret_val)

		{

			/* Free memory */

			icf_wrpr_memfree(p_msg);

			return ICF_SUCCESS;

		}          

		else

		{

			/* Free memory */

			icf_wrpr_memfree(p_msg);

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

			return ICF_FAILURE;

		}



#else

			/* This is a deactivation request */

			icf_return_t                         ret_val = ICF_SUCCESS;

#ifdef ICF_REL_OLD

			/* ICF does not wait for de-activate response now - changed in Rel 6.0*/



			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

								

			api_len = sizeof(icf_api_header_st) + \

					  sizeof(icf_port_pltfm_pdp_deact_rsp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);



			p_msg->hdr.api_id = ICF_PORT_PLTFM_PDP_DEACT_RSP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_req->payload;

			p_pdp_deact_rsp=(icf_port_pltfm_pdp_deact_rsp_st *) p_msg->payload;

						

			p_pdp_deact_rsp->context_id=p_pdp_deact_req->context_id;

			p_pdp_deact_rsp->status=ICF_PORT_PLTFM_GPRS_NOERR;



			(*smcRsporIndCallBack)(p_msg);



			/* Free memory */

			icf_wrpr_memfree(p_msg);

#endif

#endif

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ICF_SUCCESS;

}



#else 



icf_return_t	icf_port_pltfm_pdp_deactivate_req (

				icf_msg_st* p_port_req,

				icf_port_pltfm_ret_val_t	*p_port_ret_val,

				icf_msg_st* p_port_resp)     

                                

{

	icf_port_pltfm_pdp_deact_rsp_st			*p_pdp_deact_rsp=ICF_NULL;

	icf_port_pltfm_pdp_deact_req_st			*p_pdp_deact_req=ICF_NULL;

	icf_return_t		ret_val = ICF_SUCCESS;



	p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_req->payload;

	p_pdp_deact_rsp=(icf_port_pltfm_pdp_deact_rsp_st *) p_port_resp->payload;



#ifdef ICF_QOS_SUPPORT			

#ifndef  ICF_QOS_STUBBED_OUT

		icf_msg_st                           *p_msg = ICF_NULL;

		icf_uint8_t                          api_len;

		icf_error_t							ecode = ICF_NULL;

		icf_gprs_pdp_deactivate_req_st			*p_msg_payload = ICF_NULL;

		

		api_len = sizeof(icf_api_header_st) + 

			sizeof(icf_gprs_pdp_deactivate_req_st);

		

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);

		

		if (ICF_NULL == p_msg)

		{

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

			return ICF_FAILURE;

		}

		

		

		/* Fill the header of the message to be send */

		p_msg->hdr.api_id = ICF_GPRS_PDP_DEACTIVATE_REQ;

		p_msg->hdr.source_id = ICF_MODULE_ICF;

		p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

		

		/* Fill the payload */

		p_msg_payload = (icf_gprs_pdp_deactivate_req_st *)

			(p_msg->payload);

		p_msg_payload->context_id = p_pdp_deact_req->context_id;

		

		/* Send to GPRS network */

		ICF_SEND(p_persistent_glb_pdb,p_msg,ret_val,&ecode)

		

		if(ICF_SUCCESS == ret_val)

		{

			/* Free memory */

			icf_wrpr_memfree(p_msg);

			return ICF_SUCCESS;

		}          

		else

		{

			/* Free memory */

			icf_wrpr_memfree(p_msg);

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

			return ICF_FAILURE;

		}



#else

			/* This is a deactivation request */



#ifdef ICF_REL_OLD

			/* ICF does not wait for de-activate response now - changed in Rel 6.0*/



			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							ecode = ICF_NULL;

								

			api_len = sizeof(icf_api_header_st) + \

					  sizeof(icf_port_pltfm_pdp_deact_rsp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);



			p_msg->hdr.api_id = ICF_PORT_PLTFM_PDP_DEACT_RSP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_pdp_deact_req=(icf_port_pltfm_pdp_deact_req_st *) p_port_req->payload;

			p_pdp_deact_rsp=(icf_port_pltfm_pdp_deact_rsp_st *) p_msg->payload;

						

			p_pdp_deact_rsp->context_id=p_pdp_deact_req->context_id;

			p_pdp_deact_rsp->status=ICF_PORT_PLTFM_GPRS_NOERR;



			(*smcRsporIndCallBack)(p_msg);



			/* Free memory */

			icf_wrpr_memfree(p_msg);

#endif

#endif

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ret_val;

}

#endif/*#ifdef ICF_NW_ACTIVATE*/

#endif 



#ifdef ICF_QOS_SUPPORT

icf_return_t icf_port_pltfm_qos_reserve_req_non_ims(

						icf_msg_st					*p_port_req,

						icf_port_pltfm_ret_val_t	*p_port_ret_val,

						icf_msg_st					*p_port_resp)                                

                  

{

	icf_port_pltfm_qos_res_non_ims_req_st * p_qos_res_req = \

		(icf_port_pltfm_qos_res_non_ims_req_st *) p_port_req->payload;



	icf_port_pltfm_qos_res_non_ims_resp_st * p_qos_res_resp = \

		(icf_port_pltfm_qos_res_non_ims_resp_st *) p_port_resp->payload;



			icf_return_t                         ret_val = ICF_SUCCESS;

#ifdef  ICF_QOS_STUBBED_OUT

			/* This is a secondary pdp context */

			icf_msg_st                           *p_msg = ICF_NULL;

			icf_uint8_t                          api_len;

			icf_error_t							 ecode = ICF_NULL;

						

			api_len = sizeof(icf_api_header_st) + \

				sizeof(icf_port_pltfm_qos_res_non_ims_resp_st);

			

			p_msg = (icf_msg_st *)icf_wrpr_memget(

					(icf_uint32_t)api_len,

					&ecode);

            if(ICF_NULL == p_msg)
				return ICF_FAILURE;

			p_msg->hdr.api_id = ICF_PORT_PLTFM_QOS_RES_NON_IMS_RESP;

			p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

			p_msg->hdr.destination_id = ICF_MODULE_ICF;

			p_msg->hdr.api_length = api_len;

			p_msg->hdr.call_id = 0;

			p_msg->hdr.version = ICF_VERSION_1_0;

			p_msg->hdr.app_id = 0;

			p_qos_res_resp = (icf_port_pltfm_qos_res_non_ims_resp_st *) p_msg->payload;

			p_qos_res_resp->id=p_qos_res_req->id;

			p_qos_res_resp->status=ICF_PORT_PLTFM_GPRS_NOERR;

			(smcRsporIndCallBack)(p_msg);

			icf_wrpr_memfree(p_msg);

#endif

	

		*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

		return ret_val;

}



icf_return_t icf_port_pltfm_qos_release_req_non_ims (

					icf_msg_st* p_port_req,

					icf_port_pltfm_ret_val_t	*p_port_ret_val,

					icf_msg_st* p_port_rsp)     

                                

{

	icf_port_pltfm_qos_rel_non_ims_rsp_st			*p_qos_rel_rsp=ICF_NULL;

	icf_port_pltfm_qos_rel_non_ims_req_st			*p_qos_rel_req=ICF_NULL;



	p_qos_rel_req=(icf_port_pltfm_qos_rel_non_ims_req_st *) p_port_req->payload;

	p_qos_rel_rsp=(icf_port_pltfm_qos_rel_non_ims_rsp_st *) p_port_rsp->payload;



	/* This is a deactivation request */

	/* ICF does not wait for de-activate response */

	

	*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

	return ICF_SUCCESS;

}



#endif



 

    /************************************************************************************/

    



    

    

#if 0 

    /************************************************************************************/

    

    NQIRetVal_et nqiDnsService (NQIDnsService_st * 	/*dnsReq*/)

    {

        return ICF_SUCCESS;

    }

    

#endif







/*####################################################################################################*/

/*####################################################################################################*/









/* ************************************************************************** */

/* IP Security related functions start */

/* ************************************************************************** */





#ifdef ICF_IPSEC_ENABLED

#if 0



/* Function invoked by ICF to create SA for encryption */

IPSecRetVal_et	ipsecCreateHtHEnc(

	void				        *aWrGlbCntxt,

	IPSecSaCreateEncReq_st		*pSaCreateEncReq)

{

    return ICF_SUCCESS;

}





/* Function invoked by ICF to create SA for authentication */

IPSecRetVal_et	ipsecCreateHtHAuth(

	void				        *aWrGlbCntxt,

	IPSecSaCreateAuthReq_st		*pSaCreateAuthReq)

{

    return ICF_SUCCESS;

}



#endif

/* Function invoked by ICF to create SA for encryption & authentication */



icf_return_t	icf_port_pltfm_sa_create_req(

				icf_msg_st					*p_port_req,

				icf_port_pltfm_ret_val_t	*p_port_ret_val,

				icf_msg_st					* p_port_resp)   

{

	icf_port_pltfm_sa_cmd_resp_st			*p_sa_cmd_rsp=ICF_NULL;

	icf_port_pltfm_sa_create_req_st			*p_sa_create_req=ICF_NULL;

    	icf_return_t					ret_val = ICF_SUCCESS;

    	icf_msg_st					*p_msg = ICF_NULL;

    	icf_uint32_t					api_len = ICF_NULL;

    	icf_error_t					ecode = ICF_NULL;



	p_sa_create_req=(icf_port_pltfm_sa_create_req_st *) p_port_req->payload;

	p_sa_cmd_rsp=(icf_port_pltfm_sa_cmd_resp_st *)	p_port_resp->payload;



#ifndef ICF_IPSEC_STUBBED_OUT

	icf_port_pltfm_sa_create_req_st *p_msg_payload;

        

    api_len = sizeof(icf_api_header_st) + sizeof(icf_port_pltfm_sa_create_req_st);

				

	p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);

    

    if (ICF_NULL == p_msg)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

				

    /* Fill the header of the message to be send */

    p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_CREATE_REQ;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

    p_msg->hdr.api_length = api_len;

    p_msg->hdr.call_id = 0;

    p_msg->hdr.version = ICF_VERSION_1_0;

    p_msg->hdr.app_id = 0;

    

    /* Fill the payload */

    p_msg_payload = (icf_port_pltfm_sa_create_req_st *)(p_msg->payload);

    p_msg_payload->spi=p_sa_create_req->spi;

	p_msg_payload->lifetime = p_sa_create_req->lifetime;

    

    /* Send to GPRS network */

    ICF_SEND(p_persistent_glb_pdb,p_msg,ret_val,&ecode)

    /* This sleep is necessary otherwise ICF will send 4 requests to IAS 

     * quickly and manual operation of IAS may not work correctly */

#ifndef ICF_PORT_WINCE

    _sleep(1000);

#else

	Sleep(1000);

#endif



    if(ICF_SUCCESS == ret_val)

    {

        /* Free memory */

		icf_wrpr_memfree(p_msg);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }          

    else

    {

        /* Free memory */

		icf_wrpr_memfree(p_msg);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

#else



						

		api_len = sizeof(icf_api_header_st) + \

				  sizeof(icf_port_pltfm_sa_cmd_resp_st);

			

		p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);



		p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_CREATE_RESP;

		p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.destination_id = ICF_MODULE_ICF;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;

		

		p_sa_cmd_rsp->spi=p_sa_create_req->spi;

		p_sa_cmd_rsp->response=ICF_SUCCESS;

		(*smcRsporIndCallBack)(p_msg);

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ret_val;

}



icf_return_t	icf_port_pltfm_sa_modify_req(

				icf_msg_st					*p_port_req,

				icf_port_pltfm_ret_val_t	*p_port_ret_val,

				icf_msg_st					* p_port_resp)

{

	icf_port_pltfm_sa_cmd_resp_st			*p_sa_cmd_rsp=ICF_NULL;

	icf_port_pltfm_sa_modify_req_st			*p_sa_modify_req=ICF_NULL;

	    icf_return_t					ret_val = ICF_SUCCESS;

	    icf_msg_st					*p_msg = ICF_NULL;

	    icf_uint32_t					api_len = ICF_NULL;

	    icf_error_t					ecode = ICF_NULL;



	p_sa_modify_req=(icf_port_pltfm_sa_modify_req_st *) p_port_req->payload;

	p_sa_cmd_rsp=(icf_port_pltfm_sa_cmd_resp_st *)	    p_port_resp->payload;



#ifndef ICF_IPSEC_STUBBED_OUT

    icf_port_pltfm_sa_modify_req_st	*p_msg_payload = ICF_NULL;

    

    api_len = sizeof(icf_api_header_st) + sizeof(icf_port_pltfm_sa_modify_req_st);

				

	p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);

    

    if (ICF_NULL == p_msg)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

				

    /* Fill the header of the message to be send */

    p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_MODIFY_REQ;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

    p_msg->hdr.api_length = api_len;

    p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

    p_msg->hdr.version = ICF_VERSION_1_0;

    p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

    

    /* Fill the payload */

    p_msg_payload = (icf_port_pltfm_sa_modify_req_st *)(p_msg->payload);

    p_msg_payload->spi = p_sa_modify_req->spi;

    p_msg_payload->lifetime = p_msg_payload->lifetime;

    

    /* Send to GPRS network */

    ICF_SEND(p_persistent_glb_pdb,p_msg,ret_val,&ecode)

    /* This sleep is necessary otherwise ICF will send 4 requests to IAS 

     * quickly and manual operation of IAS may not work correctly */

#ifndef ICF_PORT_WINCE

    _sleep(1000);

#else

	Sleep(1000);

#endif

    

    if(ICF_SUCCESS == ret_val)

    {

        /* Free memory */

		icf_wrpr_memfree(p_msg);

		*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

		return ICF_SUCCESS;

    }          

    else

    {

        /* Free memory */

		icf_wrpr_memfree(p_msg);

        *p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

		return ICF_SUCCESS;

    }

#else

						

		api_len = sizeof(icf_api_header_st) + \

				  sizeof(icf_port_pltfm_sa_cmd_resp_st);

			

		p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);



		p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_MODIFY_RESP;

		p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.destination_id = ICF_MODULE_ICF;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;

		

		p_sa_cmd_rsp->spi=p_sa_modify_req->spi;

		p_sa_cmd_rsp->response=ICF_SUCCESS;

		(*smcRsporIndCallBack)(p_msg);

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ret_val;

}



icf_return_t	icf_port_pltfm_sa_delete_req(

		icf_msg_st					*p_port_req,

		icf_port_pltfm_ret_val_t	*p_port_ret_val,

		icf_msg_st					* p_port_resp)

{

	icf_port_pltfm_sa_cmd_resp_st			*p_sa_cmd_rsp=ICF_NULL;

	icf_port_pltfm_sa_delete_req_st			*p_sa_delete_req=ICF_NULL;



	    icf_return_t					ret_val = ICF_SUCCESS;

    	icf_msg_st					*p_msg = ICF_NULL;

    	icf_uint32_t					api_len = ICF_NULL;

    	icf_error_t					ecode = ICF_NULL;

 	

	p_sa_delete_req=(icf_port_pltfm_sa_delete_req_st *) p_port_req->payload;

	p_sa_cmd_rsp=(icf_port_pltfm_sa_cmd_resp_st *)	p_port_resp->payload;



#ifndef ICF_IPSEC_STUBBED_OUT

   icf_port_pltfm_sa_delete_req_st	*p_msg_payload = ICF_NULL;

    

    api_len = sizeof(icf_api_header_st) + sizeof(icf_port_pltfm_sa_delete_req_st);

				

	p_msg = (icf_msg_st *)icf_wrpr_memget(

			(icf_uint32_t)api_len,

			&ecode);

    

    if (ICF_NULL == p_msg)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

				

    /* Fill the header of the message to be send */

    p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_DELETE_REQ;

    p_msg->hdr.source_id = ICF_MODULE_ICF;

    p_msg->hdr.destination_id = ICF_MODULE_PLATFORM;

    p_msg->hdr.api_length = api_len;

    p_msg->hdr.call_id = 0;/*p_persistent_glb_pdb->p_call_ctx->call_id;*/

    p_msg->hdr.version = ICF_VERSION_1_0;

    p_msg->hdr.app_id = 0;/*p_persistent_glb_pdb->p_call_ctx->app_id;*/

    

    /* Fill the payload */

    p_msg_payload = (icf_port_pltfm_sa_delete_req_st *)(p_msg->payload);

    p_msg_payload->spi = p_sa_delete_req->spi;

    

    /* Send to GPRS network */

    ICF_SEND(p_persistent_glb_pdb,p_msg,ret_val,&ecode)

    /* This sleep is necessary otherwise ICF will send 4 requests to IAS 

     * quickly and manual operation of IAS may not work correctly */



#ifndef ICF_PORT_WINCE

    _sleep(1000);

#else

	Sleep(1000);

#endif

    

    if(ICF_SUCCESS == ret_val)

    {

        /* Free memory */

		icf_wrpr_memfree(p_msg);

        return ICF_SUCCESS;

    }          

    else

    {

        /* Free memory */

		icf_wrpr_memfree(p_msg);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

#else



						

		api_len = sizeof(icf_api_header_st) + \

				  sizeof(icf_port_pltfm_sa_cmd_resp_st);

			

		p_msg = (icf_msg_st *)icf_wrpr_memget(

				(icf_uint32_t)api_len,

				&ecode);



		p_msg->hdr.api_id = ICF_PORT_PLTFM_SA_DELETE_RESP;

		p_msg->hdr.source_id = ICF_MODULE_PLATFORM;

		p_msg->hdr.destination_id = ICF_MODULE_ICF;

		p_msg->hdr.api_length = api_len;

		p_msg->hdr.call_id = 0;

		p_msg->hdr.version = ICF_VERSION_1_0;

		p_msg->hdr.app_id = 0;

	

		p_sa_cmd_rsp->spi=p_sa_delete_req->spi;

		p_sa_cmd_rsp->response=ICF_SUCCESS;

		(*smcRsporIndCallBack)(p_msg);

#endif

			*p_port_ret_val= ICF_PORT_PLTFM_PENDING;

			return ret_val;

}



#endif/*#ifdef ICF_IPSEC_ENABLED*/



/* ************************************************************************** */

/* IP Security related functions end */

/* ************************************************************************** */

   

/*}//extern C*/

#endif  /*(WRAPPER_WINDOWS)||(WRAPPER_LINUX)*/



EXPORT_C icf_return_t	icf_port_pltfm_get_param_string_from_wrapper(

				icf_string_st *p_param,

				icf_uint8_t    param)

{

    FILE *config;

    char buf[100]="";

    char start_token[ICF_MAX_STR_LEN],end_token[ICF_MAX_STR_LEN];

    int len = 0;

    p_param->str_len=0;

    switch(param)

    {

	    case WRPR_PCSCF_PDP_ADDR: 

		   /* p_start_token = KPCSCFS; */

			strcpy(start_token,KPCSCFS);

		  /*  p_end_token = KPCSCFE; */

			strcpy(end_token,KPCSCFE);

		    break;



#ifdef ICF_P_CSCF_DISCOVERY_ENABLED

	    case WRPR_PCSCF_DHCP_ADDR:  

		    /*p_start_token = KPCSCFDHCPS;*/

			strcpy(start_token,KPCSCFDHCPS);

		    /*p_end_token = KPCSCFDHCPE;*/

			strcpy(end_token,KPCSCFDHCPE);

		    break;

            case WRPR_DHCP_ADDR:

			/*p_start_token = KDHCPS;*/

			strcpy(start_token,KDHCPS);

			/*p_end_token = KDHCPE;*/

			strcpy(end_token,KDHCPE);

		    break;

            case WRPR_DNS_PDP_ADDR:

			/*p_start_token = KDNSPDPS;*/

			strcpy(start_token,KDNSPDPS);

		    /*p_end_token = KDNSPDPE;*/

			strcpy(end_token,KDNSPDPE);

		    break;

            case WRPR_DNS_DHCP_ADDR:

			/*p_start_token = KDNSDHCPS;*/

			strcpy(start_token,KDNSDHCPS);

		    /*p_end_token = KDNSDHCPE;*/

			strcpy(end_token,KDNSDHCPE);

		    break;

#endif

            case WRPR_SELF_ADDR:    

		    /*p_start_token = KSLFIPS;*/ 

			strcpy(start_token,KSLFIPS);

		    /*p_end_token = KSLFIPE;*/ 

			strcpy(end_token,KSLFIPE);

		    break;

            default : return ICF_FAILURE;

    }

#ifdef ICF_PORT_WINMOB

	config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" );
#else
    config = fopen( "wrapper.cfg", "r" );
#endif

    if(NULL != config)

    {

        while (!feof(config))

        {

		if(NULL == fgets( buf, 100, config ))

		{

			return ICF_FAILURE;

		}            

		if(0 == ParseInputL((char *)p_param->str,&len,start_token,end_token,buf))

		{

			break;

		}

	}

	fclose( config );

    }

    p_param->str_len=len;

    p_param->str[len]='\0';



    return ICF_SUCCESS;

}




#if 0 
/*Common to all builds */

/* ----------------------------------------------------------------------------*/

/* icf_port_pltfm_get_device_params*/

/* Gets the device parameters from the device*/

/* returns ICF_SUCCESS/ICF_FAILURE*/

/* NOTE: Except IMSI all the parameters are hard coded for now*/

/* ----------------------------------------------------------------------------*/

/**/

EXPORT_C icf_return_t

		  icf_port_pltfm_get_device_params (icf_msg_st					*p_port_req,

											icf_port_pltfm_ret_val_t	*p_port_ret_val,

											icf_msg_st					*p_port_resp) 

                                              

                                              

{

	icf_port_pltfm_device_params_resp_st *p_device_params= \

		(icf_port_pltfm_device_params_resp_st *) p_port_resp->payload;

    



    /*Opening file for reading config data*/

    FILE *config = ICF_NULL;

#if !defined(WRAPPER_LINUX) && !defined(WRAPPER_VXWORKS)

    fpos_t pos = 0;

#endif    

    int len = 0;

	int res = 0;

    int count=0;

    /*not more then 5 APNs can be specified thourh cfg file*/

    char apnlist[5][80];

    int count_pub=0;

    /*not more then 10 Pulic user ids  can be specified thourh cfg file*/

    char pub_idlist[5][80];



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	char *sim;

    char *imsi = ICF_NULL;

	Data_st *apnl = ICF_NULL;

	Data_st *pub_ids = ICF_NULL;

	char *pr_uri = ICF_NULL;

	char *hnd = ICF_NULL;



#endif

/*To remove warnings */

   p_port_req=p_port_req;



#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) == NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" )) == NULL )

#else

  

    if( (config = fopen( "wrapper.cfg", "r" )) == NULL )

#endif

#endif

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }	



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	imsi = malloc(20 * sizeof(char));

#else

    char* imsi = new char[20];

#endif

    memset(imsi,0,20);

    

    while (!feof(config))

	   {

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(imsi);

#else

			delete imsi;

#endif



			fclose(config);

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

		}

        res = ParseInputL(imsi,&len,(char*)KIMSIS,(char*)KIMSIE,buf);

        if(res == 0)

        {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			fseek(config,0,SEEK_SET);

#else

            fsetpos(config,&pos);

#endif

            break;

        }

	   }



	p_device_params->imsi.str_len=len;

	p_device_params->mncLen = 2;/*ICF_MNC_LEN_TWO; //ICF_MNC_LEN_THREE for mncLen = 3 (only two values allowed)*/



	memcpy(p_device_params->imsi.str,

					   imsi,

					   len);

	p_device_params->imsi.str[len]='\0';



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(imsi);

#else

	delete [] imsi;

#endif



	/********************* Getting SIM TYPE *************************/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	sim = malloc(7 * sizeof(char));

#else

	char* sim = new char[7];

#endif



    memset(sim,0,7);

    

    while (!feof(config))

	{

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(sim);

#else

			delete []sim;

#endif

			fclose(config);

            return ICF_FAILURE;

		}

        res = ParseInputL(sim,&len,(char*)KSIMS,(char*)KSIME,buf);

        if(res == 0)

        {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			fseek(config,0,SEEK_SET);

#else

            fsetpos(config,&pos);

#endif

            break;

        }

	}



	if ((strcmp(sim, "USIM")) == 0)

		p_device_params->sim_type = ICF_PORT_PLTFM_EMIM3GUSIM;

	else if ((strcmp(sim, "ISIM")) == 0)

		p_device_params->sim_type = ICF_PORT_PLTFM_EMIM3GISIM;

	else

		p_device_params->sim_type=0;	/*Unknown SIM Type*/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(sim);

#else

	delete []sim;

#endif

	

	/********************* Getting SIM TYPE Ends ********************/

	

    /*Setting APN*/

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	apnl = malloc(5 * sizeof(Data_st));

#else

    Data_st *apnl= new Data_st[5];

#endif



    memset(apnl,0,sizeof(Data_st)*5);

    memset(apnlist,0,400);



    while (!feof(config))

    {

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(apnl);

#else

			delete [] apnl;

#endif

			/* Memory leak exists here */

			fclose(config);

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

		}

        if(*buf != '\n')

        {

            res = ParseInputL(apnlist[count],&len,(char*)KAPNS,(char*)KAPNE,buf);

            if(res == 0)

            {

                count++;

                apnl[count-1].format= EASCIISTRING;

                apnl[count-1].length	=   len;

                apnl[count-1].value	=	apnlist[count-1];

		    }

            else if((res != 0)&& (count!= 0))

            {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

                fsetpos(config,&pos);

#endif

                break;

            }

        }

    }

    if(feof(config) && (count==0))

		{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(apnl);

#else

			delete [] apnl;

#endif

			/* Memory leak exists here */

			fclose(config);

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

		}

        

    p_device_params->num_of_apns=count;



	if(count>ICF_PORT_PLTFM_MAX_APNS)

	{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

		free(apnl);

#else

		delete [] apnl;

#endif

		fclose(config);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

	}

	else

	{

		while(0 <= --count)

		{

			p_device_params->apn_list[count].str_len=apnl[count].length;

			memcpy (p_device_params->apn_list[count].str,

								apnl[count].value,

								p_device_params->apn_list[count].str_len);

			p_device_params->apn_list[count].str[p_device_params->apn_list[count].str_len]='\0';

		}

	}

    

    /*All the pointers of APNs ahas been assigned so apnlist[5] can be deleted*/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(apnl);

#else

    delete [] apnl;

#endif

 



	/**************************** Setting Multiple Public IDs ********************************/

	/*Setting PUBLIC IDs*/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	pub_ids = malloc(5 * sizeof(Data_st));

#else

    Data_st *pub_ids= new Data_st[5];

#endif



    memset(pub_ids,0,sizeof(Data_st)*5);

    memset(pub_idlist,0,400);

    while (!feof(config))

    {

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

			if(p_device_params->sim_type==ICF_PORT_PLTFM_EMIM3GISIM)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				fclose(config);

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				free(pub_ids);

#else

				delete [] pub_ids;

#endif

				return ICF_FAILURE;

			}

			break;

		}

        if(*buf != '\n')

        {

            res = ParseInputL(pub_idlist[count_pub],&len,(char*)KPBIDS,(char*)KPBIDE,buf);

            if(res == 0)

            {

                count_pub++;

                pub_ids[count_pub-1].format= EASCIISTRING;

                pub_ids[count_pub-1].length	=   len;

                pub_ids[count_pub-1].value	=	pub_idlist[count_pub-1];

				          

            }

            else if((res != 0)&& (count_pub!= 0))

            {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

                fsetpos(config,&pos);

#endif

                break;

            }

        }

    }

    if(feof(config) && (count_pub==0))

	{

			/* Memory leak exists here */

			if(p_device_params->sim_type==ICF_PORT_PLTFM_EMIM3GISIM)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				fclose(config);

				return ICF_FAILURE;

			}

	}

        

    p_device_params->num_public_uris=count_pub;



	if(p_device_params->num_public_uris>ICF_PORT_PLTFM_MAX_PUBLIC_USER_IDS)

	{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

		free(pub_ids);

#else

		delete [] pub_ids;

#endif

		/* Memory leak exists here */

		fclose(config);

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

	}

	else

	{

		while(0 <= --count_pub)

		{

			p_device_params->public_user_id[count_pub].str_len=pub_ids[count_pub].length;

			memcpy (p_device_params->public_user_id[count_pub].str,

								pub_ids[count_pub].value,

								p_device_params->public_user_id[count_pub].str_len);

			p_device_params->public_user_id[count_pub].str[p_device_params->public_user_id[count_pub].str_len]='\0';

		}

	}

    

    /*All the pointers of APNs ahas been assigned so pub_idlist[5] can be deleted*/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(pub_ids);

#else

    delete [] pub_ids;

#endif



	/******************* Finished Public IDs *****************************************/



    /*Setting private URI*/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	pr_uri = malloc(45 * sizeof(char));

#else

    char* pr_uri= new char[45];

#endif



    memset(pr_uri,0,45);

	len=0;

    while (!feof(config))

	   {

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

			

			if(p_device_params->sim_type==ICF_PORT_PLTFM_EMIM3GISIM)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				fclose(config);



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				free(pr_uri);

#else

				delete [] pr_uri;

#endif

				return ICF_FAILURE;

			}

			break;

		}

        

        res = ParseInputL(pr_uri,&len,(char*)KPRIDS,(char*)KPRIDE,buf);

        if(res == 0)

        {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			fseek(config,0,SEEK_SET);

#else

            fsetpos(config,&pos);

#endif

            break;

        }

	   }



	if(feof(config) && (len==0))

	{

		if(p_device_params->sim_type==ICF_PORT_PLTFM_EMIM3GISIM)

		{

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(pr_uri);

#else

			delete [] pr_uri;

#endif

			fclose(config);

			return ICF_FAILURE;

		}



		p_device_params->private_user_id.str_len=0;

	

	}

	else

	{

		p_device_params->private_user_id.str_len=len; 



		memcpy (p_device_params->private_user_id.str,

							    pr_uri,

								len);

		p_device_params->private_user_id.str[p_device_params->private_user_id.str_len]='\0';

	

	}



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(pr_uri);

#else

	delete [] pr_uri;

#endif

       

    /*Setting Home network domain IP*/



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	hnd = malloc(45 *sizeof(char));

#else

    char* hnd = new char[45];

#endif



    memset(hnd,0,45);

	len=0;

    while (!feof(config))

	   {

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

			if(p_device_params->sim_type==ICF_PORT_PLTFM_EMIM3GISIM)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				free(hnd);

#else

				delete hnd;

#endif

				fclose(config);

				return ICF_FAILURE;

			}	

			break;

		}

        

        res = ParseInputL(hnd,&len,(char*)KHMNWIPS,(char*)KHMNWIPE,buf);

        if(res == 0)

        {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			fseek(config,0,SEEK_SET);

#else 

            fsetpos(config,&pos);

#endif

            break;

        }

	   }



	if(feof(config) && (len==0))

	{

		if(p_device_params->sim_type==ICF_PORT_PLTFM_EMIM3GISIM)

		{

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(hnd);

#else

			delete [] hnd;

#endif

			fclose(config);

			return ICF_FAILURE;

		}



		p_device_params->home_nw_domain.str_len=0;



	}

	else

	{

    

		p_device_params->home_nw_domain.str_len = len;

			

		memcpy(p_device_params->home_nw_domain.str,

			   hnd, 

			   len);



		p_device_params->home_nw_domain.str[p_device_params->home_nw_domain.str_len]='\0';

	}



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(hnd);

#else

	delete [] hnd;

#endif



    /*Setting the function pointer, to free the memory allocated for device*/

    /*parameters*/



    /*closing the file*/

    fclose( config );

	*p_port_ret_val= ICF_PORT_PLTFM_SUCCESS;



    return ICF_SUCCESS;

}
#endif




/* ----------------------------------------------------------------------------*/

/* nqiPcscfAddrGet*/

/* Gets the address of P-CSCF SIP server*/

/* NOTE: values are har coded*/

/* ----------------------------------------------------------------------------*/

/**/







/*##############################################################################*/








#if 0
/*--------------------------------------------------------------------------------------*/

/* readIpsecKeys*/

/*This function reads the keys needed by IPSec section to create the SAs.*/

/**/

/*--------------------------------------------------------------------------------------*/



icf_return_t	readIpsecKeys (icf_string_st *imsIk, icf_string_st *imsCk)

{

	/*Setting the res value*/

    FILE *config = ICF_NULL;

	int ikFound = 0;

	int ckFound = 0;

	int ikLen = 0;

	int ckLen = 0;

	int ik1 = 0;

	int ck1 = 0;   

#if !defined(WRAPPER_LINUX) && !defined(WRAPPER_VXWORKS)

    fpos_t pos = 0;

#endif    



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	char *Ik = ICF_NULL;

	char *Ck = ICF_NULL;



	Ik = malloc(100 * sizeof(char));



	Ck = malloc(100 * sizeof(char));

#else	

	char *Ik = new char[100];

	char *Ck = new char[100];

#endif



	

    memset(Ik,0,100);

	memset(Ck, 0, 100);



	ikLen = 0;

#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" )) != NULL )

#else

       	

    if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

#endif



    {

        while (!feof(config))

        {

            char buf[100];

            memset(&buf,0,100);

            if( fgets( buf, 100, config ) == NULL)

			{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

				fsetpos(config,&pos);

#endif

                break;

			}

            

            ik1 = ParseInputL(Ik,&ikLen,(char*)KIKS,(char*)KIKE,buf);

            if(ik1 == 0)

            {

				ikFound = 1;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

				fsetpos(config,&pos);

#endif

                break;

            }

        }

    }



	

    

    ckLen = 0;

    while (!feof(config))

	{

        char buf[100];

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

		{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			fseek(config,0,SEEK_SET);

#else

			fsetpos(config,&pos);

#endif

			break;

		}

        

        ck1 = ParseInputL(Ck,&ckLen,(char*)KCKS,(char*)KCKE,buf);

        if(ck1 == 0)

		{

			ckFound = 1;

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			fseek(config,0,SEEK_SET);

#else

			fsetpos(config,&pos);

#endif

            break;



		}

	 }



	if ((1 == ikFound) && (1 == ckFound))

	{

		imsIk->str_len=ikLen;

		memcpy(imsIk->str,Ik,ikLen);

		imsIk->str[ikLen]='\0';



		imsCk->str_len=ckLen;

		memcpy(imsCk->str,Ck,ckLen);

		imsCk->str[ikLen]='\0';

				

		return ICF_SUCCESS;



	}

	else

	{

		return ICF_FAILURE;

	}

}/* End function: readIpsecKeys*/
#endif

/* ----------------------------------------------------------------------------*/

/* mimRunAka*/

/* Runs the Authentication and Key Agrement procedure*/

/* NOTE: All values are hard coded for now*/

/* ----------------------------------------------------------------------------*/

/**/



EXPORT_C icf_return_t

		  icf_port_pltfm_run_aka_procedure (icf_msg_st					*p_port_req,

											icf_port_pltfm_ret_val_t	*p_port_ret_val,

											icf_msg_st					*p_port_resp)

{

#ifdef IMS_CLIENT

    /*Setting the res value*/

    FILE *config = ICF_NULL;

	int res_found = 0;

	int auts_found = 0;

	int len = 0;

	int auts_len = 0;

#if !defined(WRAPPER_LINUX) && !defined(WRAPPER_VXWORKS)

    fpos_t pos = 0;

#endif    



	icf_port_pltfm_aka_response_st * p_aka_chall_resp=\

		(icf_port_pltfm_aka_response_st *)p_port_resp->payload;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	char *res = ICF_NULL;

	char *auts = ICF_NULL;



	res = malloc(100 *sizeof(char));

	

	auts = malloc(100 * sizeof(char));

#else

    char *res = new char[100];

	char *auts = new char[100];

#endif



  /* To remove warnings */

  p_port_req=p_port_req;

	

    memset(res,0,100);

	memset(auts, 0, 100);



    

#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\cfg\\wrapper.cfg", "r" )) != NULL )

#else

   

    if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

#endif

    {

        while (!feof(config))

        {

            char buf[100];

			int res1 = 0;

            memset(&buf,0,100);

            if( fgets( buf, 100, config ) == NULL)

			{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

				fsetpos(config,&pos);

#endif

                break;

			}

            

            res1 = ParseInputL(res,&len,(char*)KRESS,(char*)KRESE,buf);

            if(res1 == 0)

            {

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

                fsetpos(config,&pos);

#endif

				res_found = 1;

                break;

            }

        }

    }



	    



   auts_len = 0;

    while (!feof(config))

	{

        char buf[100];

		int auts1 = 0;

        memset(&buf,0,100);

        if( fgets( buf, 100, config ) == NULL)

			{

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

				fseek(config,0,SEEK_SET);

#else

				fsetpos(config,&pos);

#endif

                break;

			}

        

        auts1 = ParseInputL(auts,&auts_len,(char*)KAUTSS,(char*)KAUTSE,buf);

        if(auts1 == 0)

		{

			auts_found = 1;

            break;



		}

	 }



	if (1 == auts_found)

	{

		if (0 == res_found)

		{

			/* This is SQN error */

			p_aka_chall_resp->status=ICF_PORT_PLTFM_AKA_SEQRANGEERR;

			p_aka_chall_resp->auts.str_len=auts_len;

			memcpy(p_aka_chall_resp->auts.str,

				   auts,

				   auts_len);

			p_aka_chall_resp->auts.str[auts_len]='\0';





		}

		else

		{

			/* failure scenario: both AUTS and RES cannot be present at the same time */

			/* both AUTS and RES should be made null here */

			p_aka_chall_resp->status= ICF_PORT_PLTFM_AKA_AKASYSTEMERR;

			p_aka_chall_resp->auts.str_len=0;

			p_aka_chall_resp->res.str_len=0;



		}

	}

	else

	{

		if (1 == res_found)

		{

			/*Setting the res*/

			p_aka_chall_resp->res.str_len=len;

			memcpy(p_aka_chall_resp->res.str,res,len);

			p_aka_chall_resp->res.str[len]='\0';

			p_aka_chall_resp->status = ICF_PORT_PLTFM_AKA_SUCCESSFUL;



			/* Over here other members that is CK and IK will also be filled. */

			if (ICF_SUCCESS ==

						readIpsecKeys(&(p_aka_chall_resp->imsIk),

									&(p_aka_chall_resp->imsCk)))

			{

				/* nothing to be done, status left as SUCCESS ful*/

			}

			else

			{

				p_aka_chall_resp->imsIk.str_len = 0;

				p_aka_chall_resp->imsCk.str_len = 0;

			}

#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

			free(auts);

#else

			delete auts;

#endif

		}

		else

		{			

			/* This will be taken as MAC failure: neither RES nor AUTS present */

			p_aka_chall_resp->status = ICF_PORT_PLTFM_AKA_UNAUTHNWERR; 

			p_aka_chall_resp->auts.str_len = 0;

			p_aka_chall_resp->res.str_len = 0;



		}

	}/* AUTS: absent: ends */



	*p_port_ret_val= ICF_SUCCESS;

    return ICF_SUCCESS;
#else

    *p_port_ret_val= ICF_SUCCESS;

    p_port_req = p_port_req;
    p_port_resp = p_port_resp;
    return ICF_SUCCESS;

#endif 

}


/*ParseInputL */

int ParseInputL(char* buf,int* len,char* startpos, char* endpos, char* stream)

{

    char* fpos;

	int spos;

	int epos;

	char* space;

	int start;

    fpos = strstr(stream,startpos);

    

    if(fpos == NULL)

        return 1;

    

    spos = (fpos-stream+1)+strlen(startpos)-1;

    

    fpos = strstr(stream,endpos);

    if(fpos == NULL)

        return 1;

    

    epos = (fpos-stream+1);

    

    *len = epos-spos-1;

    /*removing spaces from the end*/

    space= (char*)stream;

    space+=spos;

    while((space[(*len)-1] ==' ')||(space[(*len)-1])=='\t')

        (*len)--;

    /*removing space from the front*/

    start=0;

    while((space[start] == ' ')|| (space[start]=='\t'))

        start++;

    (*len)-=start;

    

    memcpy(buf,stream+spos+start,*len);

    return 0;

}



#if 0

/*##############################################################################################*/

/* Function invoked by ICF to fetch the list of locally supported 

 * encryption algorithms

 */



EXPORT_C  icf_return_t

		  icf_port_pltfm_fetch_enc_algo_req (icf_msg_st					*p_port_req,

											 icf_port_pltfm_ret_val_t	*p_port_ret_val,

											 icf_msg_st					*p_port_resp)

{

    FILE *config;

    int len;

    icf_port_pltfm_fetch_algo_resp_st   * p_fetch_algo_rsp = \

		(icf_port_pltfm_fetch_algo_resp_st *) p_port_resp->payload;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	char *str1 = ICF_NULL;

	char *str2 = ICF_NULL;



	str1 = malloc(20 * sizeof(char));

	str2 = malloc(20 * sizeof(char));

#else

    char* str1= new char[20];

    char* str2= new char[20];

#endif

    

   /*To Remove warnings */

    p_port_req=p_port_req;



										  

	if(ICF_NULL == str1 || ICF_NULL == str2)

	{

		return ICF_FAILURE;

	}



    /** read the first algorithm **/



    len = 0;

    memset(str1,0,20);



#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" )) != NULL )

#else

    if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

#endif

    {

        while (!feof(config))

        {

            char buf[100];

			int res = 0;

            memset(&buf,0,100);

            if( fgets( buf, 100, config ) == NULL)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

                return ICF_FAILURE;

			}

            

            res = ParseInputL(str1,&len,(char*)KENCALGO1S,(char*)KENCALGO1E,buf);

            if(res == 0)

                break;

        }

        

        

        fclose( config );

    }



	p_fetch_algo_rsp->encryption_algo[0].str_len=len;

	memcpy(p_fetch_algo_rsp->encryption_algo[0].str,

		   str1,

		   len);

	p_fetch_algo_rsp->encryption_algo[0].str[len]='\0';

    /** read the second algorithm **/



    len = 0;

    memset(str2,0,20);

#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" )) != NULL )

#else

    if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

#endif

    {

        while (!feof(config))

        {

            char buf[100];

			int res = 0;

            memset(&buf,0,100);

            if( fgets( buf, 100, config ) == NULL)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

                return ICF_FAILURE;

			}

            

            res = ParseInputL(str2,&len,(char*)KENCALGO2S,(char*)KENCALGO2E,buf);

            if(res == 0)

                break;

        }

        

        

        fclose( config );

    }



	p_fetch_algo_rsp->encryption_algo[1].str_len=len;

	memcpy(p_fetch_algo_rsp->encryption_algo[1].str,

		   str2,

		   len);

	p_fetch_algo_rsp->encryption_algo[1].str[len]='\0';



	p_fetch_algo_rsp->num_enc_algo=2;



   *p_port_ret_val= ICF_SUCCESS;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(str1);

	free(str2);

#else

	delete [] str1;

	delete [] str2;

#endif



    return ICF_SUCCESS;

}
#endif



#if 0 
/* Function invoked by ICF to fetch the list of locally supported 

 * authentication algorithms

 */



EXPORT_C icf_return_t

		  icf_port_pltfm_fetch_auth_algo_req (icf_msg_st					*p_port_req,

											  icf_port_pltfm_ret_val_t		*p_port_ret_val,

											  icf_msg_st					*p_port_resp)

{

    FILE *config;

    int len;

    icf_port_pltfm_fetch_algo_resp_st   * p_fetch_algo_rsp = \

		(icf_port_pltfm_fetch_algo_resp_st *) p_port_resp->payload;  



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	char *str1 = ICF_NULL;

	char *str2 = ICF_NULL;



	str1 = malloc(20 *sizeof(char));

	str2 = malloc(20 *sizeof(char));

#else

    char* str1= new char[20];

    char* str2= new char[20];

#endif



/* To remove warnings */

    p_port_req=p_port_req;



	if(ICF_NULL == str1 || ICF_NULL == str2)

	{

		return ICF_FAILURE;

	}



    /** read the first algorithm **/



    len = 0;

    memset(str1,0,20);

#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" )) != NULL )

#else

    if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

#endif

    {

        while (!feof(config))

        {

            char buf[100];

			int res = 0;

            memset(&buf,0,100);

            if( fgets( buf, 100, config ) == NULL)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

                return ICF_FAILURE;

			}

            

            res = ParseInputL(str1,&len,(char*)KAUTHALGO1S,(char*)KAUTHALGO1E,buf);

            if(res == 0)

                break;

        }

        

        

        fclose( config );

    }

	

	p_fetch_algo_rsp->integrity_algo[0].str_len=len;

	memcpy(p_fetch_algo_rsp->integrity_algo[0].str,

		   str1,

		   len);

	p_fetch_algo_rsp->integrity_algo[0].str[len]='\0';

	

    /** read the second algorithm **/



    len = 0;

    memset(str2,0,20);

#ifdef ICF_VERIZON

	if( (config = fopen( filename, "r" )) != NULL )

#else

#ifdef ICF_PORT_WINMOB

	if( (config = fopen( "\\ims_apps\\cfg\\wrapper.cfg", "r" )) != NULL )

#else

    if( (config = fopen( "wrapper.cfg", "r" )) != NULL )

#endif

#endif

	    

    {

        while (!feof(config))

        {

            char buf[100];

			int res = 0;

            memset(&buf,0,100);

            if( fgets( buf, 100, config ) == NULL)

			{

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

                return ICF_FAILURE;

			}

            

            res = ParseInputL(str2,&len,(char*)KAUTHALGO2S,(char*)KAUTHALGO2E,buf);

            if(res == 0)

                break;

        }

        

        

        fclose( config );

    }    



	p_fetch_algo_rsp->integrity_algo[1].str_len=len;

	memcpy(p_fetch_algo_rsp->integrity_algo[1].str,

		   str2,

		   len);

	p_fetch_algo_rsp->integrity_algo[1].str[len]='\0';



	p_fetch_algo_rsp->num_auth_algo=2;

    

    *p_port_ret_val= ICF_SUCCESS;



#if defined(WRAPPER_LINUX) || defined(WRAPPER_VXWORKS)

	free(str1);

	free(str2);

#else

	delete [] str1;

    delete [] str2;

#endif



    return ICF_SUCCESS;

}

#endif

/*************************************************************************

*

*  FUNCTION NAME  :  icf_port_pltfm_ipv4_sscanf 

*

*  DESCRIPTION    : This function fixes the sscanf problem for IPV4 addrs 

*					in symbian

*

*  RETURNS        :   NONE

*

*************************************************************************/



icf_return_t icf_port_pltfm_ipv4_sscanf(IN	icf_uint8_t	 *pIP,

								  OUT	icf_uint8_t  *p_octet_1,

								  OUT	icf_uint8_t  *p_octet_2,

								  OUT	icf_uint8_t  *p_octet_3,

								  OUT	icf_uint8_t  *p_octet_4)

{

    icf_return_t							ret_val = ICF_SUCCESS;

	char *p_str=(char*)pIP;

	char *p_ch=p_str;

	int num_digits = 0;

	int current_val = 0;

	int num_octets = 0;

	int i;

	

	unsigned char octets[4]={0,0,0,0};

	

	if((NULL == p_ch) || (*p_ch == '\0'))

		return ICF_FAILURE;

	

	p_ch = p_str;

	while(*p_ch!='\0')

		p_ch++;

	p_ch--;

	

	

	while(p_ch>=p_str)

	{

		if('.' == *p_ch)

		{

			if(0==num_digits)

			{

				ret_val = ICF_FAILURE;

				break;

			}

			else

			{

				if(current_val > 255)

				{

					ret_val = ICF_FAILURE;

					break;

				}

				octets[3-num_octets]=current_val;

				num_digits = 0;

				current_val = 0;

				num_octets++;

				if(3 < num_octets)

				{

					ret_val = ICF_FAILURE;

					break;

				}

			}

		}

		else if(('0' <= *p_ch) && ('9' >= *p_ch))

		{

			int num;

			

			/* Valid digit */

			num_digits ++;

			if(num_digits >3)

			{

				ret_val = ICF_FAILURE;

				break;

			}

			num = *p_ch - '0';

			for(i = 1;i<num_digits;i++)

				num*=10;

			current_val +=num;

			octets[3-num_octets]=current_val;

		}

		else

		{

			ret_val = ICF_FAILURE;

			break;

		}

		p_ch--;

	}



	*p_octet_1=octets[0];

	*p_octet_2=octets[1];

	*p_octet_3=octets[2];

	*p_octet_4=octets[3];

	return ret_val;

}


EXPORT_C  icf_return_t icf_port_pltfm_get_host_ip(icf_msg_st					*p_port_req,

										icf_port_pltfm_ret_val_t	*p_port_ret_val,

										icf_msg_st					*p_port_resp)

{

	FILE *fp=NULL;

    char temp_buf1[100] = {"\0"},temp_buf2[32] = {"\0"};

    icf_uint8_t temp1=0,temp2=0,temp3=0,temp4=0;

	icf_uint8_t	*p_temp = ICF_NULL;



	icf_port_pltfm_host_to_ip_addr_req_st *p_host_to_ip_req=\

		(icf_port_pltfm_host_to_ip_addr_req_st*) p_port_req->payload;



	icf_port_pltfm_host_to_ip_addr_resp_st *p_host_to_ip_rsp=\

		(icf_port_pltfm_host_to_ip_addr_resp_st*) p_port_resp->payload;



	p_host_to_ip_rsp->str_len=0;

    

    if(0 == p_host_to_ip_req->str_len)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }



    if(ICF_SUCCESS == icf_port_pltfm_ipv4_sscanf(p_host_to_ip_req->str,&temp1,&temp2,&temp3,&temp4) )

    {

		*p_host_to_ip_rsp=*p_host_to_ip_req;

		*p_port_ret_val=ICF_PORT_PLTFM_SUCCESS;

        return ICF_SUCCESS;

    }

    else

	{

		p_temp = (icf_uint8_t *)strstr((char *)p_host_to_ip_req->str,":");



		if(p_temp != ICF_NULL)

		{

			*p_host_to_ip_rsp=*p_host_to_ip_req;

			*p_port_ret_val=ICF_PORT_PLTFM_SUCCESS;

			return ICF_SUCCESS;

		}

	}

#ifdef ICF_PORT_SYMBIAN

#ifdef ICF_NW_ACTIVATE

	if(ICF_SUCCESS==icf_port_pltfm_get_host_ip_on_symbian(p_port_req,

														  p_port_resp))

	{

		*p_port_ret_val=ICF_PORT_PLTFM_SUCCESS;

		return ICF_SUCCESS;

	}

#endif

#endif

fp=fopen("\\ims_apps\\cfg\\domainmap.cfg" ,"r");

    

if(fp==NULL)

{

#ifdef ICF_PORT_SYMBIAN

	*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

#endif

    }

    else

    {

        while( !feof(fp) )

        {

            fscanf(fp,"%s %s\n",temp_buf1, temp_buf2);

            if(strcmp(temp_buf1,(char *)p_host_to_ip_req->str )==0 )

                break;

            temp_buf1[0]=0;

            temp_buf2[0]=0;

        }

        

        if(temp_buf1[0]==0)

        {

            fclose(fp);

#ifdef ICF_PORT_SYMBIAN

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

#endif

        }

        else

        {

            strcpy((char *)p_host_to_ip_rsp->str,

				   temp_buf2);

			p_host_to_ip_rsp->str_len=strlen(temp_buf2);

            fclose(fp);

			*p_port_ret_val=ICF_PORT_PLTFM_SUCCESS;

            return ICF_SUCCESS;

        }

    }

    

#ifdef ICF_PORT_WINDOWS

    {

        

#ifdef ICF_IPV6_ENABLED

		struct addrinfo *p_ai_response =ICF_NULL;

		struct addrinfo ai_hints;

#endif	

        

        icf_int32_t 	dErrNo=0;

        struct hostent 	*pHostIp = ICF_NULL;

        icf_boolean_t isIpv6= ICF_FALSE;

        (void)isIpv6;



        /* First invoke the ipv4 resolver gethostbyname_r to get the host entry.

        * if it returns null and the error valuse is HOST_NOT_FOUND or 

        * TRY_AGAIN then it will invoke the IPV6 resolver getipnodebyname

        */

        

        pHostIp = gethostbyname((char *)p_host_to_ip_req->str);

        

        /* if pHostIp = NULL and dErrNo=HOST_NOT_FOUND  or TRY_AGAIN then

        * try for IPV6 resolver

        */

        if(((dErrNo == HOST_NOT_FOUND) || (dErrNo == TRY_AGAIN)) \

            && (pHostIp == ICF_NULL))

            isIpv6=ICF_TRUE;

        

        else if (pHostIp == NULL)

        {

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

        }

        

        if(isIpv6 == ICF_TRUE)

        {

#ifdef ICF_IPV6_ENABLED		

			memset((void *)&ai_hints,0,sizeof(struct addrinfo));

			ai_hints.ai_family = AF_INET6;

            if(getaddrinfo(pHost,NULL,&ai_hints,&p_ai_response) != 0){

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				return ICF_FAILURE;

            }

#else

			*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

            return ICF_FAILURE;

#endif		

        }

        

        if(isIpv6 == ICF_TRUE)

        {

#ifdef ICF_IPV6_ENABLED

			struct sockaddr_storage inaddr_storage;

			memcpy((void *)&inaddr_storage,

			(void *)(p_ai_response->ai_addr),p_ai_response->ai_addrlen);

			if(icf_port_ntop(&inaddr_storage,p_host_to_ip_rsp->str) == ICF_FAILURE){

				p_host_to_ip_rsp->str_len=\

					strlen(p_host_to_ip_rsp->str);

				freeaddrinfo(p_ai_response);

				p_ai_response = ICF_NULL;

				*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

				return ICF_FAILURE;

			}

			freeaddrinfo(p_ai_response);

			p_ai_response = ICF_NULL;

#endif		

        }

        else

        {

            struct in_addr  *inaddrptr;

            inaddrptr = (struct in_addr *)*pHostIp->h_addr_list;

			strcpy((char *)p_host_to_ip_rsp->str,(icf_int8_t *) inet_ntoa(*inaddrptr));

            p_host_to_ip_rsp->str_len=strlen((char *)p_host_to_ip_rsp->str);

        }

        

    }

#endif

    if(0==p_host_to_ip_rsp->str_len)

    {

		*p_port_ret_val=ICF_PORT_PLTFM_FAILURE;

        return ICF_FAILURE;

    }

	*p_port_ret_val=ICF_PORT_PLTFM_SUCCESS;

    return ICF_SUCCESS;

}





/*************************************************************************

*

*  FUNCTION NAME  :   icf_wrpr_memget

*

*  DESCRIPTION    :   This function is called to allocate buffers in the system.

*  Arguments      :   size      - Size of memory buffer required

*                     p_eocde   - pointer to variable to return error code

*

*  RETURNS        :   Pointer to the allocated buffer if there is available 

*                     memory in the system, else ICF_NULL with ecode as 

*                     ICF_ERROR_MEM_ALLOC_FAILURE.

*

 *************************************************************************/

icf_void_t* 

icf_wrpr_memget(icf_uint32_t size, icf_error_t *p_ecode)

{

   icf_void_t  *p_buff = ICF_NULL;

   p_buff = malloc(size); 

   if (ICF_NULL == p_buff) 

   { 

       *p_ecode = ICF_ERROR_MEM_ALLOC_FAILURE;

   } 

   return p_buff;   

}



/*************************************************************************

*

*  FUNCTION NAME  :   icf_wrpr_memfree

*

*  DESCRIPTION    :   This function is called to deallocate buffers.

*  RETURNS        :   void.

*

 *************************************************************************/

icf_return_t

icf_wrpr_memfree(icf_void_t* p_buf)

{

    icf_return_t ret_val = ICF_SUCCESS;

    if(p_buf != ICF_NULL)

    {

        free((void *)p_buf);

		p_buf = ICF_NULL;

        ret_val = ICF_SUCCESS;

    }

    else

    {

        ret_val = ICF_FAILURE;

    }



    return ret_val;

}

icf_void_t icf_port_pltfrm_get_pdp_addresses(icf_port_pltfm_pdp_def_resp_st *p_pdp_def_resp)

{

#ifdef ICF_PORT_WINMOB

	icf_port_pltfm_pdp_addr_get(&p_pdp_def_resp->pdp_addr);

#else

	icf_port_pltfm_get_param_string_from_wrapper(&p_pdp_def_resp->pdp_addr,WRPR_SELF_ADDR);

#endif



	if(0 < p_pdp_def_resp->pdp_addr.str_len)

		p_pdp_def_resp->pdp_addr_present=ICF_TRUE;

	icf_port_pltfm_get_param_string_from_wrapper(&p_pdp_def_resp->pcscf_addr,WRPR_PCSCF_PDP_ADDR);

	if(0 < p_pdp_def_resp->pcscf_addr.str_len)

		p_pdp_def_resp->pcscf_addr_present=ICF_TRUE;



#ifdef ICF_P_CSCF_DISCOVERY_ENABLED

	icf_port_pltfm_get_param_string_from_wrapper(&p_pdp_def_resp->dhcp_addr,WRPR_DHCP_ADDR);

	if(0 < p_pdp_def_resp->dhcp_addr.str_len)

		p_pdp_def_resp->dhcp_addr_present=ICF_TRUE;

	icf_port_pltfm_get_param_string_from_wrapper(&p_pdp_def_resp->dns_addr,WRPR_DNS_PDP_ADDR);

	if(0 < p_pdp_def_resp->dns_addr.str_len)

		p_pdp_def_resp->dns_addr_present=ICF_TRUE;

#endif

}



#ifdef ICF_P_CSCF_DISCOVERY_ENABLED

EXPORT_C icf_return_t icf_port_pltfm_dhcp_query(

		icf_transport_list_st *p_dhcp_addr,

		icf_transport_list_st *p_dns_addr,

		icf_transport_list_st *p_pcscf_addr)

{

       icf_string_st str;

       p_dns_addr->num_of_elements = 0;

       p_pcscf_addr->num_of_elements = 0;



      icf_port_pltfm_get_param_string_from_wrapper(&str,WRPR_DNS_DHCP_ADDR);

      if(0 < str.str_len)

      {

           p_dns_addr->num_of_elements = 1;

           icf_cmn_convert_ip_string_to_transport_addr(

			str.str,

                        &(p_dns_addr->addr[0]));

      }

 

      icf_port_pltfm_get_param_string_from_wrapper(&str,WRPR_PCSCF_DHCP_ADDR);

       

      if(0 < str.str_len)

      {

           p_pcscf_addr->num_of_elements = 1;

           icf_cmn_convert_ip_string_to_transport_addr(

			str.str,

                        &(p_pcscf_addr->addr[0]));

      }

      return ICF_SUCCESS;

}

#endif



/*End of file*/

