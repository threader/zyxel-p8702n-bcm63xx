/*****************************************************************************
 ** FUNCTION:
 **                     This file contains the implementation for all the 
 **                     SIP UA toolkit related common callbacks.
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_commoncallbacks.c
 **
 ** DESCRIPTION:        This file contains the implementation for the
 **                     SIP UA toolkit related callbacks that both the
 **                     UAC and the UAS have to implement.
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 29-JAN-2005     Aashish Suchdev
 **
 ** 24-MAY-2005     Umang Singh        SPR 7506
 ** 07-JUL-2005     Aashish Suchdev							TCP checkin            
 ** 07-DEC-2005		Jyoti Narula		ICF 4.0 LLD			Ecode setting for 
 **															timeout failure
 **															in case of REGISTER 
 **															request
 ** 01-Dec-2005		Mohan Dhawan		ICF 4.0 LLD			code merged for UT
 **															changes
 ** 02-Mar-2006     Amit Sharma                         ICF merging with ICF2.0
 ** 23-Mar-2006     Amit Sharma                         Code merged with ICF 4.2
 ** 30-Jun-2006     Umang Singh		Rel 5.2		        TLS support
 ** 03-Aug-2007     Tarun Gupta     CSR_1-4966233       Added a callback
 **                                                     sdf_cbk_uaGetIpFromNonSipUrl
 ** 23-Dec-2007     Abhishek Dhammawat SPR17855         Modified sdf_cbk_uaFreeApplicationData
 **                                                     sip_indicateMessageRetransmission 
 ** 24-Dec-2007     Abhishek Dhammawat SPR 17862        Modified 
 **                                                     sip_indicateMessageRetransmission
 ** 25-Jun-2008     Rajiv Kumar        SPR - 18722      IPTK not compilable with gcc
 **                                                     version 4.2.3 
 ** 17-Jul-2009		Rohit Aggarwal	SPR 19864	Do not abort the call on 
 **												stack's no answer timer expiry
 **
 ** 28-Jul-2009		Anurag					 		Fix for CSR 7407479 
 ** 05-Feb-2010     Pavneet Bajaj    SPR 20251      Fix for CSR 7945729 
 **
 *****************************************************************************
 **                 Copyrights 2006, Aricent.
 *****************************************************************************/

/*ICF Common inclusions*/
#include "icf_feature_flags.h"

#include "uatk_headers.h"
#include "stack_headers.h"

/*ICF Common inclusions*/
#include "icf_common_types.h"
#include "icf_macro.h"
#include "icf_internal_api_id.h"



#include "icf_dbm_prototypes.h"

/*ICF SSA inclusions*/
#include "icf_ssa_types.h"
#include "icf_ssa_macro.h"
#include "icf_ssa_prototypes.h"
#include "icf_ssa_defs.h"
#include "sdf_lookupport.h"
#ifdef ICF_DNS_LOOKUP_ENABLED
/* UATK inclusions */
#include "sdf_lookup.h"
#endif
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif



extern Sdf_ty_retVal sdf_fn_uaDnsQuery (Sdf_ty_s8bit  *pDomainName,
    Sdf_st_dnsEventContext   *pDnsEventContext,
    Sdf_st_error             *pErr);

/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaFreeApplicationData
 **
 ** DESCRIPTION: This callback is issued by the toolkit when
 **              sdf_ivk_uaFreeAppData is invoked and the reference count of
 **              the Sdf_st_appData structure is 0. It indicates that the
 **              toolkit no longer needs access to the structure. The
 **              application is passed the pData member of the Sdf_st_appData
 **              structure and may free the member at this stage if it
 **              wants to.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaFreeApplicationData(
        Sdf_ty_pvoid *ppData)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_error_t          ecode;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    icf_ssa_pdb_st       ssa_pdb;
    Sdf_st_error         sdf_error = {0,0,0,"\0"};

    p_ssa_ctxt = (icf_ssa_ctxt_st*)(*ppData);
    if ( ICF_NULL != p_ssa_ctxt)
    {
        p_glb_pdb = p_ssa_ctxt->p_glb_pdb;

        icf_ssa_init_pdb(&ssa_pdb, p_glb_pdb);

        ssa_pdb.p_ssa_glb_data = p_ssa_ctxt->p_ssa_glb_data;
        ssa_pdb.p_glb_pdb = p_glb_pdb;
        ssa_pdb.p_ecode = &ecode;
        ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;

        if(p_ssa_ctxt->p_call_obj)
        {
            if (Sdf_en_PermanentCallObject == p_ssa_ctxt->p_call_obj->dCallObjectType)
            {
#ifdef ICF_LOAD_DEBUG
                ICF_PRINT(("\nFreeAppData: Permanent CallObj\n"));
#endif
                if(p_ssa_ctxt->p_call_ctxt)
                {
                    if (p_ssa_ctxt->p_call_ctxt->p_sip_ctx == p_ssa_ctxt) 
                    {
#ifdef ICF_LOAD_DEBUG 
                        ICF_PRINT(("\n %d: Sip Ctx Matched\n", 
                               p_ssa_ctxt->call_id));
#endif
                        p_ssa_ctxt->p_call_ctxt->p_sip_ctx=ICF_NULL;
                    }
                    else
                    {
#ifdef ICF_LOAD_DEBUG 
                        ICF_PRINT(("\n %d Sip Ctx did not match\n",
                                p_ssa_ctxt->call_id));
#endif
                    }
                }
		    /*
             * Iterate thru all the open connections in conn_db and close
             * all connections that are for the given SSA Conteext and then
             * free SSA Context.
             */
                /* SPR 17855: Moved flush_alive_connections function above
                   to remove klocworks warning and redundant for callobj
                   type*/
                 icf_ssa_flush_alive_connections(&ssa_pdb);
             }    
        }


        /*
         * In this routine we will release the memory for SSA context 
         * whose pointer is the pData pointer and we will also reset the 
         * entry for this intance of ssa ctxt and the nw call id in the 
         * SSA-maintained map.
         */

		/*ICF_SSA_RESET_CALLID_CTXT_MAP(p_ssa_ctxt)*/

		if ( ICF_NULL != p_ssa_ctxt->p_hash_key)
		{
            icf_ssa_remove_ctxt_list_entry(p_glb_pdb,
                    p_ssa_ctxt);
#if 0
			icf_dbm_hash_remove_ssa_ctxt(p_glb_pdb,
				p_ssa_ctxt->p_hash_key,
				p_tmp_ssa_ctxt,&ecode);
#endif
	    	ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctxt->p_hash_key,
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
			p_ssa_ctxt->p_hash_key = ICF_NULL;
		}

        if (ICF_NULL != p_ssa_ctxt->p_auth_params_list)
        {
            Sdf_st_error     sdf_error;
            sdf_listDeleteAll(p_ssa_ctxt->p_auth_params_list, &sdf_error);
            ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctxt->p_auth_params_list,
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
        }
        
        /* Clear the contents of old nonce in ssa context */

        if(ICF_NULL != p_ssa_ctxt->p_old_nonce)
        {
		    /*Fix done for SPR-18722 : CSR-1-6068965*/
            /* memfree the nonce string */
            ICF_MEMFREE(
                p_glb_pdb,
                p_ssa_ctxt->p_old_nonce,
                ICF_MEM_COMMON,
                &ecode,
                ret_val)
         }

#ifdef ICF_SIGCOMP_SUPPORT
		if(ICF_SUCCESS == icf_dbm_get_module_glb_data(
                    ssa_pdb.p_glb_pdb,
                    (icf_int_module_id_t)ICF_INT_MODULE_CFG,
                    (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                    ssa_pdb.p_ecode))
		{
			/*
			 * Free the reference Counter of SIGSA CONTEXT as we are going to
			 * delete the SSA CONTEXT
			 */
			if (ICF_TRUE == ssa_pdb.p_glb_cfg_data->sigcomp_required)
			{
#ifdef ICF_IPSEC_ENABLED

				if (ICF_TRUE == ssa_pdb.p_glb_cfg_data->ipsec_enabled)
				{
					icf_rgm_glb_data_st		 *p_rgm_glb_data  = ICF_NULL;

					if (ICF_FAILURE == \
							icf_dbm_get_module_glb_data(ssa_pdb.p_glb_pdb,\
							ICF_INT_MODULE_REGM, 
							(icf_void_t **)(p_rgm_glb_data),
							ssa_pdb.p_ecode))
					{
						/*ICF_SIGSA_TRACE(("\nFailure while getting RGM Global Data in SIGSA\n"));*/

						return ICF_FAILURE;
					}

					/* Free the reference Counter of SIGSA CONTEXT */
					icf_sigsa_decr_ref_count_to_sigsa_ctxt(
											p_rgm_glb_data->p_sigsa_ctxt);
				}
				else
				{
					/*
					 * Free the reference Counter of SIGSA CONTEXT as we are going to
					 * delete the SSA CONTEXT
					 */
					if(ICF_NULL != p_ssa_ctxt->p_sigsa_ctxt)
					{
						icf_sigsa_decr_ref_count_to_sigsa_ctxt(
										p_ssa_ctxt->p_sigsa_ctxt);
					}
				}
#else
				if(ICF_NULL != p_ssa_ctxt->p_sigsa_ctxt)
				{
					icf_sigsa_decr_ref_count_to_sigsa_ctxt(
									p_ssa_ctxt->p_sigsa_ctxt);
				}
#endif 
			} /* if sigcomp_required */
		}/* if cfg_data */
#endif
   /*Fix for CSr 7945729-SPR 20251*/
if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
    {
        p_ssa_ctxt->p_sic_ctxt->p_ssa_ctx = ICF_NULL;
        p_ssa_ctxt->p_sic_ctxt = ICF_NULL;
    }
    if(ICF_NULL != p_ssa_ctxt->p_rgm_ctxt)
    {
        p_ssa_ctxt->p_rgm_ctxt->p_ssa_ctxt = ICF_NULL;
		p_ssa_ctxt->p_rgm_ctxt = ICF_NULL;
    }

/* sic_ctx should be cleared in SIC module only Decision by Pankaj/Pavai */
#ifdef ICF_REL_OLD
		if(ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
        {
            if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt->p_user_address)
			{
				ICF_MEMFREE(
						ssa_pdb.p_glb_pdb,
						p_ssa_ctxt->p_sic_ctxt->p_user_address,
						ICF_MEM_COMMON,
						ssa_pdb.p_ecode,
						ret_val)
			}

			if( p_ssa_ctxt->p_sic_ctxt->p_preferred_identity)
			{
				ICF_MEMFREE(
						 ssa_pdb.p_glb_pdb,
						 p_ssa_ctxt->p_sic_ctxt->p_preferred_identity,
						 ICF_MEM_COMMON,
						 ssa_pdb.p_ecode,
						 ret_val)
			}

			icf_dbm_dealloc_sic_cntxt_blk(
				ssa_pdb.p_glb_pdb,
				p_ssa_ctxt->p_sic_ctxt->call_id,
				ssa_pdb.p_ecode);
        }
#endif
        /*if (ICF_NULL != p_ssa_ctxt->p_call_ctxt)
	{
		p_ssa_ctxt->p_call_ctxt->p_sip_ctx = ICF_NULL;
	}*/

#ifdef ICF_DNS_LOOKUP_ENABLED
        if((p_ssa_ctxt->bitmask & ICF_SSA_CONTACT_IP_PRESENT) &&
            p_ssa_ctxt->p_dns_ip_addr != ICF_NULL)
    {
        ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt->p_dns_ip_addr, \
                ICF_MEM_COMMON, &ecode, ret_val)
    }
#endif
 /* stop all the running timers and return the allocated memory to timer pool
      */
/*Fix for CSR 7945729 SPR 20251*/
if (ICF_TRUE == icf_ssa_check_if_any_timers_exist(p_ssa_ctxt))
     {
        icf_ssa_timer_list_st      *p_timer_list = ICF_NULL;
        icf_ssa_timer_list_st      *p_temp_timer_list = ICF_NULL;

        p_timer_list = p_ssa_ctxt->p_timer_list;

        while (0 != p_ssa_ctxt->timer_count)
        {

            if (ICF_NULL != p_timer_list)
            {
                if(ICF_NULL != p_timer_list->p_timer_data)
                {
#ifdef ICF_SESSION_TIMER
                    icf_ssa_timer_data_st      *p_ssa_timer_data = 
                        (icf_ssa_timer_data_st *)p_timer_list->p_timer_data->p_timer_buffer;

                    if(Sdf_en_sessionTimer == p_ssa_timer_data->timer_type)  
                    {
                        if(ICF_NULL == p_ssa_ctxt->p_call_obj)                                               
                        {                                                                      
                        ICF_PRINT(((icf_uint8_t *)"\n"  "p_ssa_ctxt->p_call_obj is null \n"));   
                         ret_val = ICF_FAILURE;                              
                         }                                                          
                        else if(ICF_NULL ==p_ssa_ctxt->p_call_obj->pSessionTimer)
                          
                        {                                                          
                        ICF_PRINT(((icf_uint8_t *)"\n" "p_ssa_ctxt->p_call_obj->pSessionTimer \
                             is null \n"));   
                         ret_val = ICF_FAILURE;                              
                        }      
     
                        if (ICF_FAILURE == ret_val)
                        {
                            /* session timer in call object does not exist, just stop
                             * the timer and free the timer data
                             */ 
                            ICF_STOP_TIMER(p_glb_pdb, p_timer_list->timer_id, 
                                &ecode, ret_val)

                            /*Free the allocated ssa timer block here*/
                            icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                                    p_timer_list->p_timer_data->p_timer_buffer,&ecode); 

                            icf_dbm_ret_mem_to_pool(p_glb_pdb,
                                ICF_MEM_TIMER,p_timer_list->p_timer_data,  
                                &ecode);
                        }
                        else
                        {
                            sdf_fn_uaStopSessionTimer(\
                                (Sdf_ty_pvoid *)&(p_ssa_ctxt->p_call_obj->pSessionTimer->pTimerId), \
                                &sdf_error);
                        }
                    } 
                    else
                    {
#endif
                        ICF_STOP_TIMER(p_glb_pdb, p_timer_list->timer_id, 
                                &ecode, ret_val)
  
                        /*Free the allocated ssa timer block here*/
                        icf_dbm_ret_mem_to_pool(p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                                    p_timer_list->p_timer_data->p_timer_buffer,&ecode);    

                        icf_dbm_ret_mem_to_pool(p_glb_pdb,
                                ICF_MEM_TIMER,p_timer_list->p_timer_data,  
                                &ecode);    
#ifdef ICF_SESSION_TIMER
                    } 
#endif
                } 
                p_temp_timer_list = p_timer_list;
                p_timer_list = p_timer_list->p_next;
                ICF_MEMFREE(
                    p_glb_pdb,
                    p_temp_timer_list,
                    ICF_MEM_COMMON,
                    &ecode,
                    ret_val)

            } /* if (ICF_NULL != p_timer_lis... */   
            else
            {
                /* timer data list is NULL */
                break;
            }         
            p_ssa_ctxt->timer_count--;
        }       
    }

     p_ssa_ctxt->p_timer_list = ICF_NULL;
     p_ssa_ctxt->timer_count = ICF_NULL;
    if ((ICF_NULL != p_ssa_ctxt->p_call_ctxt) &&
		(p_ssa_ctxt->p_call_ctxt->p_sip_ctx == p_ssa_ctxt))
	{
		p_ssa_ctxt->p_call_ctxt->p_sip_ctx = ICF_NULL;
		p_ssa_ctxt->p_call_ctxt = ICF_NULL;
	}

    
    p_persistent_glb_pdb->no_of_call_obj--;
    ICF_MEMFREE(p_glb_pdb,p_ssa_ctxt, \
                ICF_MEM_COMMON, &ecode, ret_val)

        *ppData = Sdf_co_null;
    }

    return Sdf_co_success;
}


/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaCloneAppData
 **
 ** DESCRIPTION: This is issued by the toolkit to the application when
 **              the CallObject is to be cloned. The application should
 **              clone the pData member of the AppData structure
 **              to copy the source to the destination.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCloneAppData(
        Sdf_st_appData *pDestInfo,
        Sdf_st_appData *pSrcInfo,
        Sdf_st_error *pErr)
{
    /* To remove compiler warning */
    (void)pErr;

    /* Simply copy the source ptr into the destination ptr */
    pDestInfo = pSrcInfo;

    return Sdf_co_success;
}

/*****************************************************************************
 ** FUNCTION:    sdf_cbk_uaCloneEventContextpData
 **
 ** DESCRIPTION: This is issued by the toolkit to the application when
 **              the CallObject is to be cloned. The application should
 **              clone the pData member of the Event Context structure
 **              to copy the source to the destination.
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaCloneEventContextpData(
        Sdf_st_eventContext *pDestInfo,
        Sdf_st_eventContext *pSrcInfo,
        Sdf_st_error *pErr)
{
    /* To remove compiler warning */
    (void)pErr;

    /* Simply copy the source ptr into the destination ptr */
    pDestInfo->pData = pSrcInfo->pData;

    return Sdf_co_success;
}


/*****************************************************************************
** FUNCTION: sdf_cbk_uaFreeEventContextData
**
**
** DESCRIPTION: This callback is issued by the toolkit when
**              sdf_ivk_uaFreeEventContext is invoked and the
**              reference count of the Sdf_st_eventContext structure
**              is 0. It indicates that the toolkit no longer needs
**              access to the structure. The application is passed
**              the pData member of the Sdf_st_eventContext
**              structure and may free the member at this stage if it wants to.
**
**  PARAMETERS:
**              ppData(IN)      : Pointer to the void* member of the
**                                Sdf_st_eventContext structure
******************************************************************************/
Sdf_ty_retVal sdf_cbk_uaFreeEventContextData(
        Sdf_ty_pvoid *ppData)
{
    /* To remove compiler warning */
    (void)ppData;

    return Sdf_co_success;
}

#ifdef SIP_AUTHENTICATE
/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaAuthenticateUser
 **
 ** DESCRIPTION:        This callback is issued by the toolkit when it needs to
 **                     verify the credentials of a user. It is issued by the
 **                     sdf_ivk_uaAuthorize API when the request contains an
 **                     Authorization header, and the toolkit needs to verify if
 **                     the credentials in the header are valid. The application
 **                     should return the clear-text password (in case of Basic)
 **                     or H(A1) (in case of Digest) from its database.
 **
 *****************************************************************************/
Sdf_ty_authResult sdf_cbk_uaAuthenticateUser(
        Sdf_st_callObject **ppCallObj,
        SipAddrSpec *pAddrSpec,
        Sdf_st_uasAuthInfo *pUasAuthInfo,
        Sdf_ty_pvoid *ppContext,
        Sdf_st_error *pErr)
{
    /* WILL BE IMPLEMENTED LATER */
    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pAddrSpec;
    (void)pUasAuthInfo;
    (void)ppContext;
    (void)pErr;

    return Sdf_co_success;
}

/*****************************************************************************
** FUNCTION: sdf_cbk_uaChallengeUser
**
** DESCRIPTION: This callback is issued by the toolkit under
**              two scenarios:
**              a) If a request arrives without Authorization header.
**                 The application should look up it's database, and
**                 inform the toolkit if the user needs to be challenged
**                 and if so, with what parameters.
**              b) If a request arrives with a Authorization header
**                 but the credentials do not match with that
**                 present in the application. In this case, the
**                 application is issued the callback to re-challenge
**                 the user with new/same parameters
**                 The application should indicate whether the user is to be
**                 challenged, and if so, return the realm (in case of Basic)
**                 OR the {realm, opaque, qop, algorithm, domain, stale,
**                 nonce} (in case of Digest)
**
*****************************************************************************/
Sdf_ty_authResult sdf_cbk_uaChallengeUser(
        Sdf_st_callObject **ppCallObj,
        SipAddrSpec *pAddrSpec,
        Sdf_st_authenticationParams *pAuthParams,
        Sdf_ty_pvoid *ppContext,
        Sdf_st_error *pErr)
{
    /* WILL BE IMPLEMENTED LATER */
    /* To remove compiler warning */
    (void)ppCallObj;
    (void)pAddrSpec;
    (void)pAuthParams;
    (void)ppContext;
    (void)pErr;

    return Sdf_co_success;

}

#endif /* SIP_AUTHENTICATE */

#ifdef SDF_TXN
/*****************************************************************************
 ** FUNCTION:           sdf_cbk_uaIndicateTxnTimeOut
 **
 ** DESCRIPTION:        This callback is issued by the toolkit when
 **
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaIndicateTxnTimeOut(
        Sdf_st_callObject                      *pCallObj,
        Sdf_st_overlapTransInfo                *pOverlapTxn,
        Sdf_ty_txnTimerType                    dTimerType,
        Sdf_ty_pvoid                           pContext,
        Sdf_st_error                           *pError)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_rgm_nw_resp_st               *p_rgm_msg = ICF_NULL;
	icf_nw_inc_refer_resp_st         *p_refer_resp = ICF_NULL;
	icf_nw_inc_msg_resp_st			*p_msg_resp = ICF_NULL;
	icf_nw_inc_subs_resp_st			*p_subs_resp = ICF_NULL;
	icf_nw_inc_publish_resp_st       *p_publish_resp = ICF_NULL;
	icf_nw_inc_opt_resp_st			*p_opt_resp = ICF_NULL;
        Sdf_st_error                   sdf_error;
#ifdef ICF_DNS_LOOKUP_ENABLED
    icf_error_t                     err;
#endif
     Sdf_st_transaction *pTransaction=Sdf_co_null;
    icf_ssa_ctxt_st       *p_ssa_ctxt = 
        (icf_ssa_ctxt_st*)((Sdf_st_appData*)pContext);

    /*Normally we do not access GLB PDB  through p_ssa_ctxt.
     * This is special handling for timer exprie */
    icf_glb_pdb_st       *p_glb_pdb =
        ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_glb_pdb;

    icf_internal_msg_st  *p_internal_msg =
        ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_internal_msg;

    icf_int_module_id_t      dest_module_id =
        ICF_INT_MODULE_INVALID;
#ifdef ICF_IPSEC_ENABLED
	icf_ssa_pdb_st		*p_ssa_pdb = (icf_ssa_pdb_st*)p_ssa_ctxt->p_ssa_pdb;
#endif

    SipOptions dOptions;
    SipMessage	*p_sip_msg;	

	Sdf_ty_s8bit *p_method=Sdf_co_null;
    Sdf_ty_messageType 	dMsgType		= Sdf_en_unknownRequest;
    en_SipMessageType 	dReqRespType = SipMessageAny;
    en_SipMessageType 	dReqRespTypeTemp = SipMessageAny;
    Sdf_ty_u16bit             resp_code;

    ICF_FUNCTION_ENTER(p_glb_pdb)   
    ret_val = ret_val; 

    /* check if the record was picked up from the DNS buffer if 
     * yes then delete the DNS buffer entry*/
    if (Sdf_co_null == pOverlapTxn)
    {
        if((dTimerType == Sdf_en_timerB) || (dTimerType == Sdf_en_timerF) ||\
                (dTimerType == Sdf_en_timerNoAnswer))
        {
            p_sip_msg = pCallObj->pUacTransaction->pSipMsg;
            pTransaction =  pCallObj->pUacTransaction;
        }
        else
        {
            p_sip_msg = pCallObj->pUasTransaction->pSipMsg;
            pTransaction =  pCallObj->pUasTransaction;
        }
    }
    else
    {
        p_sip_msg = pOverlapTxn->pSipMsg;
    }
#ifdef ICF_DNS_LOOKUP_ENABLED
    if (p_ssa_ctxt->bitmask & ICF_DNS_RECORD_FETCHED)
    {
        /*Reset the TxnDNSState to send DNS query again*/
        if(Sdf_co_null == pOverlapTxn)
        {
            if(Sdf_en_Idle != pTransaction->dTxnDnsState)
            {
                pTransaction->dTxnDnsState = Sdf_en_Idle;  
            }
        }
        else
        {
            if(Sdf_en_Idle != pOverlapTxn->dTxnDnsState)
            {
               pOverlapTxn->dTxnDnsState = Sdf_en_Idle;
            }   
        }

        sip_getMessageType(p_sip_msg, &dReqRespTypeTemp, (SipError*)
                    &(pError->stkErrCode));

        if ((SipMessageRequest == dReqRespTypeTemp) &&
                (p_ssa_ctxt->p_conn_table_req_entry != ICF_NULL))
        {
            icf_nw_interface_info_st nw_interface_info;
            nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
            nw_interface_info.receiver_fd = 
            p_ssa_ctxt->p_conn_table_req_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
            if (p_ssa_ctxt->p_conn_table_req_entry->prot_conn_bitmask &
                    ICF_TRANSPORT_TLS)
            {
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
            }
#endif
            ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
            icf_es_close_tcp_conn(
                    p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                    &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);

            icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                    p_ssa_ctxt->p_conn_table_req_entry->conn_fd);
            pCallObj->pUacTransaction->dTxnDnsState = Sdf_en_Idle;

        }
        else if ((SipMessageResponse == dReqRespTypeTemp) &&
                (p_ssa_ctxt->p_conn_table_resp_entry != ICF_NULL))
        {
            icf_nw_interface_info_st nw_interface_info;
            nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
            nw_interface_info.receiver_fd = 
            p_ssa_ctxt->p_conn_table_resp_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
            if (p_ssa_ctxt->p_conn_table_resp_entry->prot_conn_bitmask &
                    ICF_TRANSPORT_TLS)
            {
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
            }
#endif
            ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
            icf_es_close_tcp_conn(
                    p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                    &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);
            icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                    p_ssa_ctxt->p_conn_table_resp_entry->conn_fd);
            pCallObj->pUasTransaction->dTxnDnsState = Sdf_en_Idle;
        }

        p_ssa_ctxt->bitmask &= ~ICF_DNS_RECORD_FETCHED;
        icf_ssa_dns_delete_fqdn_entry(&p_ssa_ctxt->fqdn, &err);
        /* 7.0 DNS enhancement . New DNS query is fired and earlier record is deleted*/
        if ( Sdf_co_fail == sdf_ivk_uaHandleLookup(pCallObj,
                    pOverlapTxn, p_sip_msg,
                   p_ssa_ctxt->p_ssa_pdb->p_event_ctxt,&sdf_error) &&
                ( Sdf_en_dnsInvState != sdf_error.errCode))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:FailureIn HandleLookUp API"));
            ret_val = ICF_FAILURE;
        }
        else if ((Sdf_co_false == sdf_ivk_uaIsDnsQueryState(pCallObj,
                        pOverlapTxn, &sdf_error)))
           /* ||                   (ICF_NULL == (p_ssa_ctxt->bitmask & 
                        ICF_SSA_DNS_FQDN_PRESENT )))*/
        {
            ret_val = ICF_FAILURE;
        }
        else
            ret_val =  ICF_SUCCESS;
        return ret_val;
        /* We return from here as now next processing shall be done when response
         * will be received from DNS query */
    }
#endif

    if (ICF_NULL != p_sip_msg)
    {
      if( SipFail != sip_getMessageType(p_sip_msg, &dReqRespType, (SipError*)
            &(pError->stkErrCode)))
      {
        /* 
         * If the call context is not null fetch the response code which
         * would be sent to the application
         */
        if(ICF_NULL != p_ssa_ctxt->p_ssa_pdb->p_ssa_ctxt->p_call_ctxt && 
            SipMessageResponse == dReqRespType)
        {
          if (Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                p_sip_msg, &resp_code, pError))
          {
            p_ssa_ctxt->p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->\
              inc_response_code = resp_code;
          }
          else
          {
            pError->errCode = Sdf_en_noUaError;
            ICF_FUNCTION_EXIT(p_glb_pdb)
              return Sdf_co_fail;
          }
        }

        if(Sdf_co_fail == 
            sdf_ivk_uaGetMethodFromSipMessage(p_sip_msg, &p_method, pError))

        {
          pError->errCode = Sdf_en_noUaError;

          ICF_FUNCTION_EXIT(p_glb_pdb)    
            return Sdf_co_fail;
        }
      }
      else
      {
        pError->errCode = Sdf_en_noUaError;
        ICF_FUNCTION_EXIT(p_glb_pdb)
          return Sdf_co_fail;
      }
    }
#ifdef ICF_TRACE_ENABLE    
    p_ssa_ctxt->p_ssa_pdb->p_method = (icf_uint8_t *)p_method;
    ICF_SSA_APP_TRACE(p_ssa_ctxt->p_ssa_pdb, 
                   ICF_TRACE_SIP_MSG_RETRANSMISSION_TIMEOUT)
#endif 
	if(p_method)
		sdf_fn_uaGetMethodNameAsEnum(p_method, &dMsgType, pError);

	dOptions.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    if(dTimerType == Sdf_en_timerH)
    {
        /* ACK wait timer expired */
        if((Sdf_en_finalSuccessSent == 
                    pCallObj->pCallInfo->dState) ||
                (Sdf_en_reInviteFinalResponseSent == 
                 pCallObj->pCallInfo->dState)) 
        {
            dest_module_id = ICF_INT_MODULE_CM;
        }
        /*thie code was merged from cert branch caused crashes in LOAD testing 
          for ICF_6_5_PQT_SUPPORT branch*/
        /*Free call onject only if timers are not expired because of free call 
         * object*/
        else if ((Sdf_en_finalFailureSent == pCallObj->pCallInfo->dState) &&
                 (ICF_FALSE == p_ssa_ctxt->no_retrans_req))
        {
           	/*Fix for CSR 7407479*/
		   if(1 < pCallObj->dRefCount)
            {
            sdf_ivk_uaFreeCallObject(&pCallObj);
        }
    }
    }
    else if((dTimerType == Sdf_en_timerB) || (dTimerType == Sdf_en_timerF))
    {
        /* Request Timeout.  Sdf_en_timerB for Invite Transactions and
         * Sdf_en_timerF for Regular Transactions */

#ifdef ICF_IPSEC_ENABLED
		
		if ((ICF_TRUE == p_ssa_pdb->p_glb_cfg_data->ipsec_enabled) &&
			(dReqRespType == SipMessageRequest) &&
			(dMsgType != Sdf_en_register))
		{
			/* decrement the active transaction count */
			p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans -= 1;
			ICF_PRINT(("[SSA]: num_active_trans decremented, current value = %d", 
				p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans));
			
			/* check if the active transaction count == 0. if yes and the
			* timeout is for a non Register transaction, so send a message
			* to REGM for deleting the SA if the transaction comp. on the 
			* old SA
			*/

			p_ssa_pdb->p_rgm_context = p_ssa_ctxt->ssa_sa_context.p_rgm_context;
			p_ssa_pdb->p_sa_context = p_ssa_ctxt->ssa_sa_context.p_sa_context;

			if (ICF_NULL == 
				p_ssa_ctxt->ssa_sa_context.p_sa_context->num_active_trans)
			{
				ret_val = icf_ssa_rgm_delete_old_sa_ind(p_ssa_pdb);
			}
		}
#endif

        if ((SipMessageRequest == dReqRespType) &&
                (p_ssa_ctxt->p_conn_table_req_entry != ICF_NULL))
        {
            icf_nw_interface_info_st nw_interface_info;
            nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
            nw_interface_info.receiver_fd = 
            p_ssa_ctxt->p_conn_table_req_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
            if (p_ssa_ctxt->p_conn_table_req_entry->prot_conn_bitmask &
                    ICF_TRANSPORT_TLS)
            {
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
            }
#endif
            ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
            icf_es_close_tcp_conn(
                    p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                    &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);

            icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                    p_ssa_ctxt->p_conn_table_req_entry->conn_fd);

        }
        else if ((SipMessageResponse == dReqRespType) &&
                (p_ssa_ctxt->p_conn_table_resp_entry != ICF_NULL))
        {
            icf_nw_interface_info_st nw_interface_info;
            nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TCP;
            nw_interface_info.receiver_fd = 
            p_ssa_ctxt->p_conn_table_resp_entry->conn_fd;
#ifdef ICF_SECURE_SUPPORT
            if (p_ssa_ctxt->p_conn_table_resp_entry->prot_conn_bitmask &
                    ICF_TRANSPORT_TLS)
            {
                nw_interface_info.bitmap = ICF_INTF_INFO_PROTOCOL_TLS;
            }
#endif
            ICF_PRINT(((icf_uint8_t *)"Closing the Request connection\n"))
            icf_es_close_tcp_conn(
                    p_ssa_ctxt->p_ssa_pdb->p_glb_pdb->p_glb_data->p_glb_port_info, 
                    &nw_interface_info, p_ssa_ctxt->p_ssa_pdb->p_ecode);
            icf_ssa_remove_conn_entry(p_ssa_ctxt->p_ssa_pdb,
                    p_ssa_ctxt->p_conn_table_resp_entry->conn_fd);
        }
        

        if((dReqRespType == SipMessageRequest || 
                    (dReqRespType == SipMessageResponse)) &&
                ((dMsgType == Sdf_en_update) || (dMsgType == Sdf_en_invite) ||
                 (dMsgType == Sdf_en_info) || (dMsgType == Sdf_en_prack) ||
                 (dMsgType == Sdf_en_cancel)))
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
							dOptions))
	        {
		        return ICF_SUCCESS;
	        }
#endif
            dest_module_id = ICF_INT_MODULE_CM;
        }
        else if (dMsgType == Sdf_en_bye)
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
	    						dOptions))
    	    {
		        return ICF_SUCCESS;
	        }
#endif
           /* Other than Request Timeouts, and final Response timeouts */
            sdf_ivk_uaCloseAllSocketsForCall(pCallObj, pError);
            sdf_ivk_uaAbortCall(pCallObj, pContext, pError);

        }
        else if ( Sdf_en_register == dMsgType)
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
            if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
							dOptions))
	        {
		        return ICF_SUCCESS;
	        }
#endif
            dest_module_id = ICF_INT_MODULE_CFG;
        }
ICF_SIC_START
	else if (p_method)
	{
        if (Sdf_en_refer == dMsgType)
        {
#ifdef ICF_DNS_LOOKUP_ENABLED
			if(ICF_SUCCESS == 
                    icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
					dOptions))
			{
				return ICF_SUCCESS;
			}
#endif
            if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
                dest_module_id = ICF_INT_MODULE_SIC;
            else
                dest_module_id = ICF_INT_MODULE_CM;
        }
		if(
			( Sdf_en_subscribe == dMsgType)||
			( Sdf_en_options == dMsgType)||
			/*( Sdf_en_refer == dMsgType)||*/
			!( icf_port_strcmp( (icf_uint8_t *)p_method,(icf_uint8_t *)"MESSAGE" ) ) ||
			!( icf_port_strcmp((icf_uint8_t *) p_method,(icf_uint8_t *)"OPTIONS" ) ) ||
			!( icf_port_strcmp( (icf_uint8_t *)p_method,(icf_uint8_t *) "PUBLISH") )
		)
		{
#ifdef ICF_DNS_LOOKUP_ENABLED
			if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
					dOptions))
			{
				return ICF_SUCCESS;
			}
#endif
			dest_module_id = ICF_INT_MODULE_SIC;
		}
	}
	
    	/* code added */
	    else if (Sdf_en_refer == dMsgType)
	    {
#ifdef ICF_DNS_LOOKUP_ENABLED
			if(ICF_SUCCESS == icf_ssa_resend_ongoing_txn(pCallObj, p_sip_msg, pOverlapTxn,\
						dOptions))
			{
				return ICF_SUCCESS;
			}
#endif
   	     	if (ICF_NULL != p_ssa_ctxt->p_sic_ctxt)
	        {
	            dest_module_id = ICF_INT_MODULE_SIC;
   	     	}
	        else
	        {
	            dest_module_id = ICF_INT_MODULE_CM;
	        }
   		 }
ICF_SIC_END    
    }
	/* Fix for CSR 1-7493306: SPR 19864 */
	/* On receiving 1xx response, UATK runs a No Answer timer of 180sec.
	 * When this timer expires, we should not abort the call. Instead,
 	 * we should let IPTK guard timer expire and handle the clearance
	 * gracefully.
	 */
    else if (dTimerType != Sdf_en_timerNoAnswer)
    {
        /* Other than Request Timeouts, and final Response timeouts */
        sdf_ivk_uaCloseAllSocketsForCall(pCallObj, pError);
        sdf_ivk_uaAbortCall(pCallObj, pContext, pError);

    }

    /*SPR 7743 Starts*/
    if (( ICF_SSA_APP_CTXT_TERMINATED ==
    (ICF_SSA_APP_CTXT_TERMINATED & p_ssa_ctxt->bitmask))
    || (ICF_SSA_APP_CTX_DELINKED == 
                (ICF_SSA_APP_CTX_DELINKED & p_ssa_ctxt->bitmask)))
    {
        dest_module_id = ICF_INT_MODULE_INVALID;
    }
    /*SPR 7743 Ends*/
    else
    {
        if (ICF_INT_MODULE_CM == dest_module_id)
        {
	   #ifdef ICF_SESSION_TIMER
	        if((pCallObj->pSessionTimer != ICF_NULL) && 
			(pCallObj->pSessionTimer->pTimerId != ICF_NULL))
		{
			if(sdf_fn_uaStopSessionTimer((Sdf_ty_pvoid *)\
				&(pCallObj->pSessionTimer->pTimerId) ,pError)==Sdf_co_fail)
			{
			    ret_val = ICF_FAILURE;
			}
		}
	  #endif

            /*Fill internal message for other module*/
			p_internal_msg->p_msg_data = ICF_NULL;/*no payload*/
			p_internal_msg->msg_hdr.msg_id = 
				    ICF_GENERIC_CALL_CLEAR;
			p_internal_msg->msg_hdr.payload_length = 0;
			p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;
			((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->dest_module_id =
			    dest_module_id;
        }
        else if (ICF_INT_MODULE_CFG == dest_module_id)
        {
            /* Populate the internal message header */
            p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_REG_RESP;
            p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_rgm_nw_resp_st);

            /* Populate the internal message payload */
            p_rgm_msg = (icf_rgm_nw_resp_st *)p_internal_msg->p_msg_data;

            p_rgm_msg->p_rgm_context = p_ssa_ctxt->p_rgm_ctxt;
            ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_rgm_msg->p_rgm_context))

                p_rgm_msg->status = ICF_RGM_SSA_NW_FAILURE;

            p_rgm_msg->ecode = ICF_RGM_SSA_FAIL_TIMEOUT;

            p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;
            ICF_ASSERT(p_glb_pdb, 
                    (ICF_NULL != p_internal_msg->msg_hdr.p_glb_pdb))

                ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->dest_module_id = 
                dest_module_id;
        }
		else if (ICF_INT_MODULE_SIC == dest_module_id)
        {
			
			
			if( Sdf_en_refer == dMsgType)
			{
				/* Populate the internal message header */
				p_internal_msg->msg_hdr.msg_id = ICF_SIC_REFER_STATUS;
				p_internal_msg->msg_hdr.payload_length = 
					ICF_PORT_SIZEOF(icf_nw_inc_refer_resp_st);

				/* Populate the internal message payload */
				p_refer_resp = (icf_nw_inc_refer_resp_st *)p_internal_msg->p_msg_data;

				p_refer_resp->response = 0;
				p_refer_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                /*KLOCWORKS */
                if (ICF_NULL != p_refer_resp->p_sic_ctx)
                {
                    p_refer_resp->p_sic_ctx->error_code = ICF_CAUSE_TIMER_EXPIRY;
                    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_refer_resp->p_sic_ctx))
                }
            
			}
			else if( Sdf_en_subscribe == dMsgType)
			{
				if (ICF_NULL == p_ssa_ctxt->p_sic_ctxt)
				{
					/* In this case application has delinked itself
					 * and hence it should ignore this response 
					 * and return SUCCESS */
					ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Application delinked: Txn Timeout for SUBSCRIBE ignored "));
					dest_module_id = ICF_INT_MODULE_INVALID;
				}
				else
				{
					/* Populate the internal message header */
					p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_SUBSCRIBE_RESP;
					p_internal_msg->msg_hdr.payload_length = 
						ICF_PORT_SIZEOF(icf_nw_inc_subs_resp_st);

					/* Populate the internal message payload */
					p_subs_resp = (icf_nw_inc_subs_resp_st *)p_internal_msg->p_msg_data;

					p_subs_resp->bitmask = 0;
					p_subs_resp->response = ICF_NW_INC_SUBS_RESP_NO_RETRY;
					p_subs_resp->resp_code=0;
					p_subs_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
					ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_subs_resp->p_sic_ctx))
				}
			}
			else if( Sdf_en_options == dMsgType)
			{
							/* Populate the internal message header */
							p_internal_msg->msg_hdr.msg_id = ICF_SIC_OPTION_STATUS;
							p_internal_msg->msg_hdr.payload_length = 
											ICF_PORT_SIZEOF(icf_nw_inc_opt_resp_st);

							/* Populate the internal message payload */
							p_opt_resp = (icf_nw_inc_opt_resp_st *)p_internal_msg->p_msg_data;

							p_opt_resp->response = 0;
							p_opt_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
							ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_opt_resp->p_sic_ctx))
											p_opt_resp->p_sic_ctx->error_code = ICF_CAUSE_TIMER_EXPIRY;
			}
			else if(p_method)
			{
							if( icf_port_strcmp( (icf_uint8_t *)p_method,(icf_uint8_t *)"MESSAGE" ) == 0 )
							{
											/* Populate the internal message header */
											p_internal_msg->msg_hdr.msg_id = ICF_SIC_MESSAGE_STATUS;
											p_internal_msg->msg_hdr.payload_length = 
															ICF_PORT_SIZEOF(icf_nw_inc_msg_resp_st);

											/* Populate the internal message payload */
											p_msg_resp = (icf_nw_inc_msg_resp_st *)p_internal_msg->p_msg_data;

											p_msg_resp->response = 0;
											p_msg_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                                            if (ICF_NULL != p_msg_resp->p_sic_ctx)
                                            {
                                                p_msg_resp->p_sic_ctx->error_code =
                                                    ICF_CAUSE_TIMER_EXPIRY;
                                                ICF_ASSERT(p_glb_pdb,
                                                        (ICF_NULL != p_msg_resp->p_sic_ctx))
                                            }
							}
							else if( icf_port_strcmp((icf_uint8_t *) p_method, (icf_uint8_t *)"PUBLISH") == 0)
							{
											/* Populate the internal message header */
											p_internal_msg->msg_hdr.msg_id = ICF_SIC_PUBLISH_STATUS;
											p_internal_msg->msg_hdr.payload_length = 
															ICF_PORT_SIZEOF(icf_nw_inc_publish_resp_st);

											/* Populate the internal message payload */
											p_publish_resp = (icf_nw_inc_publish_resp_st *)p_internal_msg->p_msg_data;

											p_publish_resp->response = 0;
											p_publish_resp->duration = 0;
											p_publish_resp->p_sic_ctx = p_ssa_ctxt->p_sic_ctxt;
                                            if (ICF_NULL != p_publish_resp->p_sic_ctx)
                                            {
                                                p_publish_resp->p_sic_ctx->error_code =
                                                    ICF_CAUSE_TIMER_EXPIRY;
                                                ICF_ASSERT(p_glb_pdb,
                                                        (ICF_NULL != p_publish_resp->p_sic_ctx))
                                            }
							}
			}
			p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;
			ICF_ASSERT(p_glb_pdb, 
											(ICF_NULL != p_internal_msg->msg_hdr.p_glb_pdb))

							((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->dest_module_id = 
							dest_module_id;
				}
		}

    if((ICF_NULL == p_ssa_ctxt->p_call_ctxt)||
       (p_ssa_ctxt->p_call_ctxt->p_sip_ctx == p_ssa_ctxt))
    {
        ICF_SSA_SET_ECODE(p_ssa_ctxt->p_ssa_pdb,p_method,ICF_ERROR_RETRANSMISSION_TIMEOUT)
    }
    else if(ICF_INT_MODULE_CM == dest_module_id)
    {   /*This leg is for the case when forceful call has been called for clearing the 
              forked callobjs and timer expiry is being simulated. In such scenario we should
              ignore the timer expiry*/
        dest_module_id = ICF_INT_MODULE_INVALID;
    }
		pError->errCode = Sdf_en_noUaError;

		ICF_FUNCTION_EXIT(p_glb_pdb)    
		return Sdf_co_success;

}

#endif /* SDF_TXN */

#ifdef SDF_TEL
/*****************************************************************************
 ** FUNCTION:           sdf_fn_uaGetIpFromTelUrl
 **
 ** DESCRIPTION:        This function resolves a TelUrl into a valid IP and
 **                     port. It fills up the pDestInfo structure passed to it.
 **
 *****************************************************************************/

Sdf_ty_retVal sdf_cbk_uaGetIpFromTelUrl(
        Sdf_st_callObject *pCallobj,
        TelUrl *pTelUrl,
        Sdf_st_transportInfo *pDestInfo,
        Sdf_st_error *pErr)
{
    icf_return_t          ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st       *p_ssa_ctxt = 
        (icf_ssa_ctxt_st*)((Sdf_st_appData*)pCallobj->pAppData->pData);
    icf_ssa_pdb_st        *p_ssa_pdb = (icf_ssa_pdb_st *)
        (p_ssa_ctxt->p_ssa_pdb);
    
    icf_line_data_st      *p_line_data = p_ssa_pdb->p_line_data ;

    /* To remove compiler warning */
    (void)pTelUrl;
    (void)pErr;

    /*
     * Allocate memory for pDestInfo->pIp
     * This memory will be freed by UATK
     */
    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
            ICF_SSA_IP_MAX_LEN,
            ICF_MEM_COMMON, pDestInfo->pIp,
            ICF_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)

    /* Extract the proxy address and return as pDestInfo */

    if ( (ICF_TRANSPORT_TYPE_TCP == p_line_data->transport_mode) ||
            (ICF_TRANSPORT_MODE_BOTH == p_line_data->transport_mode))
    {
        pDestInfo->dScheme = Sdf_en_protoTcp ;
    }
#ifdef ICF_SECURE_SUPPORT    
    else if (ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
    {
        pDestInfo->dScheme = Sdf_en_protoTls ;
    }
#endif    
    else
    {
        pDestInfo->dScheme = Sdf_en_protoUdp ;
    }

    if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
            p_line_data->proxy_addr.addr.addr_type)
    {
        icf_port_strcpy((icf_uint8_t *)pDestInfo->pIp,
                (icf_uint8_t *)p_line_data->proxy_addr.addr.addr.domain.str);
    } /* p_line_data->proxy_addr.addr.a... */
    else
    {
        icf_ssa_convert_to_IP_addr(&(p_line_data->proxy_addr),
               (icf_uint8_t *) pDestInfo->pIp);
    }
    pDestInfo->dPort = p_line_data->proxy_addr.port_num;
/*    ICF_SSA_TRACE(p_ssa_pdb,ICF_TRACE_TEL_URL_TO_IP_ADDR)*/


    return Sdf_co_success;

}
#endif
#ifdef SDF_SESSION_TIMER
/*****************************************************************************
 ** FUNCTION:	 sdf_cbk_uaHandleSTTimeout
 **
 ** DESCRIPTION: This callback is issued to the application on expiration
 **				 of session timer. In this callback application can perform
 **				 his own sets of operation to refresh/terminate the session 
 **				 and gives an indication to the UATK through pRefresh enum. 
 **				 If return value of pRefresh enum is Sdf_en_applicaton then
 **				 UATK will not perform refresh on behalf of application. 
 **				 If the value is Sdf_en_uatoolkit then UATK will 
 **				 UATK will perform refresh/terminate on behalf of
 **				 application 
 **
 ** PARAMETERS:
 **		pCallObj(IN)	: Call Object that has Session Timer times out
 **		pRefresh(OUT)	: The application is expected to set this enum 
 **						  for performing the refresh/terminated by the UATK 
 **		pErr(OUT)		: Error structure in case the function fails
 **
 ************************************************************************/


Sdf_ty_retVal sdf_cbk_uaHandleSTTimeout(
	Sdf_st_callObject *pCallObj,Sdf_ty_handleEvent *pRefresh,
	Sdf_ty_sessionRefreshRequest *pRefreshReq, Sdf_st_error  *pErr )
{
        icf_return_t  ret_val = ICF_SUCCESS;
     
#ifdef ICF_SESSION_TIMER	
	Sdf_ty_refresher dRefresher = Sdf_en_refresherNone;
	Sdf_st_appData *pAppData = ICF_NULL;
	icf_ssa_ctxt_st *p_ssa_ctxt = ICF_NULL;
	icf_ssa_pdb_st *p_ssa_pdb = ICF_NULL;
	icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
        icf_nw_otg_session_refresh_req_st  *p_session_refresh_req;
	icf_internal_msg_st  *p_internal_msg = ICF_NULL;
	if(sdf_ivk_uaGetAppDataFromCallObject(pCallObj, &pAppData, pErr) == Sdf_co_fail){
		return Sdf_co_fail;
	}
        if (ICF_NULL == pAppData)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: Unable to fetch App Data.\n"))
           return ICF_FAILURE;
        }
	p_ssa_ctxt = (icf_ssa_ctxt_st *)pAppData->pData;
        if (ICF_NULL == p_ssa_ctxt)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: Unable to fetch SSA Ctxt.\n"))
           return ICF_FAILURE;
        }
	p_ssa_pdb = (icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb);
        if (ICF_NULL == p_ssa_pdb)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: Unable to fetch SSA Pdb.\n"))
           return ICF_FAILURE;
        }
	    p_glb_pdb = p_ssa_pdb->p_glb_pdb;
        if (ICF_NULL == p_glb_pdb)
        {
           ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer: p_glb_pdb is NULL.\n"))
           return ICF_FAILURE;
        }
        /* Moved FUNCTION_ENTER macro below to avoid klocworks warning 
           Also it should be call only if p_glb_pdb is non-null */
        ICF_FUNCTION_ENTER(p_glb_pdb)   
        if (ICF_TRUE == p_ssa_ctxt->p_call_ctxt->session_timer_enable)
        {
	  p_internal_msg = ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_internal_msg;
   	  p_session_refresh_req = (icf_nw_otg_session_refresh_req_st*)
		(p_internal_msg->p_msg_data);
    

	if(pCallObj->pMediaInfo != ICF_NULL){
		pCallObj->pMediaInfo->dMediaState = Sdf_en_idleMedia;
	}
	/* get the ICF session refresher role */

	if(sdf_ivk_uaGetRefresher(pCallObj, &dRefresher,pErr) == Sdf_co_fail){
		sdf_ivk_uaFreeAppData(pAppData);
		return Sdf_co_fail;
	}
	*pRefresh = Sdf_en_applicaton;
	/*if remote UA is the refresher then send a message to CC to terminate the call*/
	if(dRefresher == Sdf_en_refresherRemote){
		ICF_PRINT(((icf_uint8_t *)"\n [SSA] Sending Bye when Session Timer Expired"));
		
		/* this reason code indiates to CC that session timer had expired when */

		/* session refresher is remote UA.*/

		p_internal_msg->p_msg_data = ICF_NULL;/*no payload*/
			p_internal_msg->msg_hdr.msg_id = 
				    ICF_GENERIC_CALL_CLEAR;
			p_internal_msg->msg_hdr.payload_length = 0;
			p_internal_msg->msg_hdr.p_glb_pdb = p_glb_pdb;    
		p_ssa_pdb->dest_module_id = ICF_INT_MODULE_CM;
	}
	else{
		/* Send an event ICF_NW_OTG_REFRESH_SEND_OR_ACK to CC to set
		 *  signalling ongoing to CC and send Session refresh request
		 *  using UATK */

		if(p_ssa_ctxt->refresher_method_type == Sdf_en_refresherInvite){
			ICF_PRINT(((icf_uint8_t *)"\n [SSA] Sending Re-Invite when Session Timer Expired"));
			p_session_refresh_req->refresher_req_type = ICF_SSA_REFRESHER_METHOD_INVITE;
		}
		else if(p_ssa_ctxt->refresher_method_type == Sdf_en_refresherUpdate){
			ICF_PRINT(((icf_uint8_t *)"\n [SSA] Sending Update when Session Timer Expired"));
			p_session_refresh_req->refresher_req_type = ICF_SSA_REFRESHER_METHOD_UPDATE;
		}
		else
			ret_val = ICF_FAILURE;
		if ( ICF_SUCCESS == ret_val)
		{
			p_ssa_pdb->dest_module_id = ICF_INT_MODULE_INVALID;
			ICF_SSA_SET_COMMON_BITMASK((p_ssa_pdb->common_bitmask), 
				ICF_SSA_PENDING_SESSION_REFRESH)
			p_ssa_ctxt->session_timer_expired_refresh = ICF_TRUE;
		}
/*		p_ssa_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&session_refresh_req;
		if(icf_ssa_process_mesg(p_ssa_pdb->p_internal_msg) == Sdf_co_fail){
			sdf_ivk_uaFreeAppData(pAppData);
			return Sdf_co_fail;
		}*/
	}
	sdf_ivk_uaFreeAppData(pAppData);
	pRefreshReq = pRefreshReq;
       }
       else
       {
	  pCallObj = pCallObj;
	  pRefresh = pRefresh;
	  pErr = pErr;
   	  pRefreshReq = pRefreshReq;         
       }
    ICF_FUNCTION_EXIT(p_glb_pdb)
#else
	pCallObj = pCallObj;
	pRefresh = pRefresh;
	pErr = pErr;
   	pRefreshReq = pRefreshReq;
#endif


	if ( ICF_SUCCESS == ret_val)
	{
		return Sdf_co_success;
	}
	else
	{
		return Sdf_co_fail;
	}
}
#endif


#ifdef SDF_TLS

/***********************************************************************
** FUNCTION:  sdf_cbk_uaTLSConnectionEstdNtfn
**
** DESCRIPTION: This is a notification callback thrown on the application 
**              to notify that a new TLS connection has been established. 
**
** PARAMETERS:
** 		dSockfd(IN)	: The Socket Descriptor. 
** 		pSockInfo(IN)	: The socket related information corresponding to
**                        dSockfd.
**	 	pErr(OUT)		: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTLSConnectionEstdNtfn(
    Sdf_ty_socketId	dSockfd,
    Sdf_st_sockInfo	*pSockInfo,
    Sdf_st_error *pErr)
{
	(void)pErr;
    (void)pSockInfo;
    dSockfd = dSockfd;

	return Sdf_co_success;
}


/***********************************************************************
** FUNCTION:  sdf_cbk_uaTLSConnectionClosedNtfn
**
** DESCRIPTION: This is a notification callback thrown on the application 
**              to notify that an existing TLS connection has been closed. 
**
** PARAMETERS:
** 		dSockfd(IN)	: The Socket Descriptor that was closed. 
**	 	pErr(OUT)	: Error structure in case the function fails
**
************************************************************************/
Sdf_ty_retVal sdf_cbk_uaTLSConnectionClosedNtfn
(Sdf_ty_socketId	dSockfd,
 Sdf_st_error *pErr)
{
	(void)pErr;
    dSockfd = dSockfd;
	return Sdf_co_success;
}

#endif /* SDF_TLS */
#ifdef SDF_LOOKUP

/***********************************************************************
** FUNCTION: sdf_cbk_uaSendCallToPeer 
**
** DESCRIPTION:  This callback is thrown when the UATK forms the message
**                       and application needs to send the message in the network
************************************************************************/

Sdf_ty_retVal sdf_cbk_uaSendCallToPeer  (Sdf_st_callObject  \
                *pObject, Sdf_st_overlapTransInfo *pOverlapTransInfo,
                SipMessage *pSipMsg, SipOptions dOptions, \
                Sdf_ty_s8bit transptype, Sdf_st_eventContext *pContext, \
                Sdf_st_error *pErr)
{
        Sdf_ty_s8bit *pMethod = Sdf_co_null;
        Sdf_ty_retVal   sdf_ret_val = Sdf_co_success;

        if (Sdf_co_fail == sdf_ivk_uaHandleLookup(pObject, pOverlapTransInfo, pSipMsg,
                                pContext,pErr))
        {
                if ( Sdf_en_dnsInvState != pErr->errCode)
                {
                        ICF_PRINT(((icf_uint8_t *)"\n[SSA]:DNS lookup API failed \n"));
                        sdf_ret_val = Sdf_co_fail;
                }
        }
        else if (Sdf_co_fail == sdf_ivk_uaGetMethodFromSipMessage(pSipMsg,&pMethod,pErr))
        {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:failure In SendcalLToPeer CBK\n"));
                sdf_ret_val = Sdf_co_fail;
        }
        else if (Sdf_co_fail == sdf_ivk_uaEndTransaction(pObject, pOverlapTransInfo, \
                                pMethod, pErr))
        {
                ICF_PRINT(((icf_uint8_t *)"\n[SSA]:failure In Ending txn SendCalLToPeer CBK\n"));
                sdf_ret_val = Sdf_co_fail;
        }

#ifdef ICF_DNS_LOOKUP_ENABLED
        dOptions = dOptions;
        transptype = transptype;
#endif

        return sdf_ret_val;
}

/****************************************************************************
 ** FUNCTION: sdf_cbk_uaGetIpFromNonSipUrl
 **
 ** DESCRIPTION: This Callback resolves a Non Sip URl into a valid IP and port.
 **				 It fills up the pDestInfo structure passed to it.
 **
 ****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaGetIpFromNonSipUrl
(Sdf_st_dnsEventContext   *pDnsEventContext,
 Sdf_ty_void              *pUrl,
 Sdf_ty_UrlType           dUrlType,	
 Sdf_st_error *pErr)
{
	Sdf_ty_DnsRecordType	     dDnsRecordType = Sdf_en_unKnownAddr;
	Sdf_ty_bool		             dUpdateState =	Sdf_co_false;
    Sdf_st_transportInfo         *pDestInfo = Sdf_co_null;

    icf_return_t          ret_val = ICF_SUCCESS,
                          dummy_ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st       *p_ssa_ctxt = 
        (icf_ssa_ctxt_st*)((Sdf_st_appData*)pDnsEventContext->pCallObj->pAppData->pData);
    icf_ssa_pdb_st        *p_ssa_pdb = (icf_ssa_pdb_st *)
        (p_ssa_ctxt->p_ssa_pdb);
    
    icf_line_data_st      *p_line_data = p_ssa_pdb->p_line_data ;

    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
        sizeof(Sdf_st_transportInfo),
        ICF_MEM_COMMON, pDestInfo,
        ICF_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)

    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
        ICF_MAX_STR_LEN,
        ICF_MEM_COMMON, pDestInfo->pIp,
        ICF_DONOT_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)

    if (ICF_FAILURE == ret_val || ICF_NULL == pDestInfo->pIp)
    {
       ICF_MEMFREE(
                p_ssa_pdb->p_glb_pdb,
                pDestInfo,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                ret_val)
        return ICF_FAILURE;        
    }

	pDestInfo->dPort = p_line_data->proxy_addr.port_num;
      
   if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
        pDnsEventContext->pTransaction->pDestInfo->dPort =pDestInfo->dPort; 
   else
        pDnsEventContext->pOverlappedTransaction->pDestInfo->dPort =pDestInfo->dPort;	
 
	
    /* Extract the proxy address and return as pDestInfo */
    if ( (ICF_TRANSPORT_TYPE_TCP == p_line_data->transport_mode) ||
            (ICF_TRANSPORT_MODE_BOTH == p_line_data->transport_mode))
    {
        pDestInfo->dScheme = Sdf_en_protoTcp ;
    }
#ifdef ICF_SECURE_SUPPORT    
    else if (ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
    {
        pDestInfo->dScheme = Sdf_en_protoTls ;
    }
#endif    
    else
    {
        pDestInfo->dScheme = Sdf_en_protoUdp ;
    }

    if(ICF_TRANSPORT_ADDRESS_DOMAIN_NAME == 
            p_line_data->proxy_addr.addr.addr_type)
    {
        icf_port_strcpy((icf_uint8_t *)pDestInfo->pIp,
                (icf_uint8_t *)p_line_data->proxy_addr.addr.addr.domain.str);

        if ((pDestInfo->dPort) == 0)
            dDnsRecordType = Sdf_en_SRVrecord;
        else
            dDnsRecordType = Sdf_en_IPAddress;	

        pDnsEventContext->dDnsRecordType  = dDnsRecordType;
      
	    if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
	        pDnsEventContext->pTransaction->dDnsRecordType  = \
					dDnsRecordType;
	    else
	        pDnsEventContext->pOverlappedTransaction->dDnsRecordType = \
					dDnsRecordType;	
	
	  /* Query the portlayer code */
      if(sdf_fn_uaDnsQuery(pDestInfo->pIp, pDnsEventContext,pErr) == Sdf_co_fail)
	  {
#ifdef SDF_TEL      
		if (dUrlType == Sdf_en_TelUrl)
		    sip_freeTelUrl((TelUrl*)pUrl);
#endif
#ifdef SDF_IM
        if (dUrlType == Sdf_en_ImUrl)
		    sip_freeImUrl((ImUrl*)pUrl);
#endif
#ifdef SDF_PRES
        if (dUrlType == Sdf_en_PresUrl)
            sip_freePresUrl((PresUrl*)pUrl);
#endif
        (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext(pDnsEventContext);
	    pErr->errCode=Sdf_en_destinationInfoError;
	    ret_val =  Sdf_co_fail;
	  }
      ICF_MEMFREE(
                p_ssa_pdb->p_glb_pdb,
                pDestInfo->pIp,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                dummy_ret_val)
    }
    else
    {
        icf_ssa_convert_to_IP_addr(&(p_line_data->proxy_addr),
                (icf_uint8_t *)pDestInfo->pIp);

	    dUpdateState = Sdf_co_true;	

        if (pDnsEventContext->pOverlappedTransaction == Sdf_co_null)
        {
            if(pDnsEventContext->pTransaction->pDestInfo->pIp)
            {
                ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    pDnsEventContext->pTransaction->pDestInfo->pIp,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    dummy_ret_val)
            }
            pDnsEventContext->pTransaction->pDestInfo->pIp = pDestInfo->pIp;
              
            if(pDestInfo->dPort == 0)
            {
#ifdef SDF_TLS
                if(ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
                {
                    pDnsEventContext->pTransaction->pDestInfo->dPort = 
                        ICF_DEFAULT_SIPS_SERVER_PORT;
                }
                else
#endif
                {
                    pDnsEventContext->pTransaction->pDestInfo->dPort = 
                        ICF_DEFAULT_SIP_SERVER_PORT;
                }
            }
        }
        else
        {
            if(pDnsEventContext->pOverlappedTransaction->pDestInfo->pIp)
            {
                ICF_MEMFREE(
                    p_ssa_pdb->p_glb_pdb,
                    pDnsEventContext->pOverlappedTransaction->pDestInfo->pIp,
                    ICF_MEM_COMMON,
                    p_ssa_pdb->p_ecode,
                    dummy_ret_val)
            }

            pDnsEventContext->pOverlappedTransaction->pDestInfo->pIp = pDestInfo->pIp;  

            if(pDestInfo->dPort == 0)
            {      
#ifdef SDF_TLS
            
                if(ICF_TRANSPORT_TYPE_TLS == p_line_data->transport_mode)
                {
                    pDnsEventContext->pOverlappedTransaction->pDestInfo->dPort =
                        ICF_DEFAULT_SIPS_SERVER_PORT;
                }
                else
#endif
                {
		            pDnsEventContext->pOverlappedTransaction->pDestInfo->dPort =
                        ICF_DEFAULT_SIP_SERVER_PORT;
                }
            }
        }
	
          
       if (dUpdateState == Sdf_co_true)
       {
	       if(pDnsEventContext->pOverlappedTransaction==Sdf_co_null)
	       {
		       if(pDnsEventContext->pTransaction != Sdf_co_null)
               {

                   switch (pDnsEventContext->pTransaction->dTxnDnsState)  
                   {
		           case Sdf_en_dnsQuery:
                       pDnsEventContext->pTransaction->dTxnDnsState = 
                           Sdf_en_dnsFirstRecordFilled;	
                   break;

                   case Sdf_en_dnsIpUsed:
                       pDnsEventContext->pTransaction->dTxnDnsState = 
                           Sdf_en_dnsFilled;
                   break;

                   default: ; 
                       /* To avoid warning, default is added */
                   }
               }
               else
               {
                   ret_val = Sdf_co_fail;
               }
		   }
		   else
		   {
			   switch (pDnsEventContext->pOverlappedTransaction->dTxnDnsState)  
               {
                   case Sdf_en_dnsQuery:
                       pDnsEventContext->pOverlappedTransaction->dTxnDnsState = 
                           Sdf_en_dnsFirstRecordFilled;
                   break;
                   
                   case Sdf_en_dnsIpUsed:
                       pDnsEventContext->pOverlappedTransaction->dTxnDnsState = Sdf_en_dnsFilled;
                   break;
                   
                   default: break ;
                       /* To avoid warning, default is added */
               }
		   }
	   }		
    }
    /*Fix done for SPR-18722 : CSR-1-6068965*/
	ICF_MEMFREE(
                p_ssa_pdb->p_glb_pdb,
                pDestInfo,
                ICF_MEM_COMMON,
                p_ssa_pdb->p_ecode,
                dummy_ret_val)

    return ret_val;
}

#endif

/***********************************************************************
** FUNCTION: sip_indicateMessageRetransmission
**
** DESCRIPTION:  This callback is thrown when the Stack retramsmits the message
**               on the network.
************************************************************************/

icf_void_t sip_indicateMessageRetransmission(SipEventContext *pEventContext, 
                                       SipTxnKey *pTxnKey,
                                       SIP_S8bit *pBuffer,
                                       SIP_U32bit dBufferLength, 
                                       SipTranspAddr *pTranspAddr, 
                                       SIP_U8bit dRetransCount, 
                                       SIP_U32bit dDuration)
{
  Sdf_ty_s8bit              *p_method = Sdf_co_null;
  Sdf_st_error              Error;
  SipMessage                *p_sip_msg = Sdf_co_null;
  icf_ssa_ctxt_st           *p_ssa_ctxt = ICF_NULL;
  icf_glb_pdb_st            *p_glb_pdb = ICF_NULL;
  icf_ssa_pdb_st            ssa_pdb;
  Sdf_ty_messageType        dMsgType = Sdf_en_unknownRequest;
  Sdf_st_txnContextData	  *pContextData	= \
                                 (Sdf_st_txnContextData *)pEventContext->pData;
  Sdf_st_callObject        *pCallObj = pContextData->pRefCallObject;
  en_SipMessageType         dReqRespType = SipMessageAny;
  Sdf_st_overlapTransInfo   *pTransInfo = Sdf_co_null;
  Sdf_st_transaction        *pTransaction = Sdf_co_null;
  Sdf_st_overlapTransInfo   *pOverlapTxn = Sdf_co_null;
  Sdf_ty_u16bit             resp_code;

  pTxnKey = pTxnKey;
  pBuffer = pBuffer;
  dBufferLength = dBufferLength;
  pTranspAddr = pTranspAddr;
  dDuration = dDuration;

  /*p_ssa_ctxt = (icf_ssa_ctxt_st *)((Sdf_st_appData*)(pContextData->pApplicationData));*/
  p_ssa_ctxt = (icf_ssa_ctxt_st *)(pCallObj->pAppData->pData);

  if ( ICF_NULL != p_ssa_ctxt)
  {
    p_glb_pdb = p_ssa_ctxt->p_glb_pdb;

    icf_ssa_init_pdb(&ssa_pdb, p_glb_pdb);

    ssa_pdb.p_ssa_glb_data = p_ssa_ctxt->p_ssa_glb_data;
    ssa_pdb.p_glb_pdb = p_glb_pdb;
    p_ssa_ctxt->p_ssa_pdb =
      (struct icf_ssa_pdb_st*)(&ssa_pdb);
    ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;
    ssa_pdb.retrans_count = dRetransCount;

    icf_dbm_get_module_glb_data(
        p_glb_pdb,
        ICF_INT_MODULE_CFG,
        (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
        ssa_pdb.p_ecode);

    icf_dbm_get_module_glb_data(
        p_glb_pdb,
        (icf_int_module_id_t)ICF_INT_MODULE_REGM,
        (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
        ssa_pdb.p_ecode);

    if ((pContextData->dType == Sdf_en_overlapTransaction) ||
        (pContextData->dType == Sdf_en_RegularUasTransaction) ||
        (pContextData->dType == Sdf_en_RegularUacTransaction) )
    {
      pOverlapTxn = (Sdf_st_overlapTransInfo *)pContextData->pTransaction;
    }

    if (Sdf_co_null == pOverlapTxn)
    {
      if(ICF_NULL != pCallObj->pUacTransaction->pSipMsg)
         p_sip_msg = pCallObj->pUacTransaction->pSipMsg;
       else
         p_sip_msg = pCallObj->pUasTransaction->pSipMsg;
    }
    else
    {
      p_sip_msg = pOverlapTxn->pSipMsg;
    }

    if(sdf_ivk_uaGetTransactionForSipMsg(\
          (pCallObj), p_sip_msg, &pTransaction, \
          &pTransInfo, &Error) == Sdf_co_success)
    {
      if(pTransInfo == Sdf_co_null)
      {
        p_sip_msg = pTransaction->pSipMsg;
        sdf_ivk_uaFreeTransaction(pTransaction);
      }
      else
      {
        p_sip_msg = pTransInfo->pSipMsg;
        sdf_ivk_uaFreeOverlapTransInfo( \
            pTransInfo);
      }
    }


    if (ICF_NULL != p_sip_msg)
    {
      if( SipFail != sip_getMessageType(p_sip_msg, &dReqRespType, (SipError*)
            &(Error.stkErrCode))) 
      {
        if(SipMessageResponse == dReqRespType)
        {
          /* move the call context NULL check to here as the call context
          will be NULL in non INVITE related transactions and we have to 
          fill the ERROR code for these messages too */
          if (ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt &&
            Sdf_co_fail != sdf_ivk_uaGetRespCodeFromSipMessage(
                p_sip_msg, &resp_code, &Error))
          {
            ssa_pdb.p_ssa_ctxt->p_call_ctxt->\
              inc_response_code = resp_code;
          }
          else
          {
            Error.errCode = Sdf_en_noUaError;
          }  
        }

        if(Sdf_co_fail ==
            sdf_ivk_uaGetMethodFromSipMessage(p_sip_msg, &p_method, &Error))

        {
          Error.errCode = Sdf_en_noUaError;
        }
      }
      else
      {
        Error.errCode = Sdf_en_noUaError;
      }
    }

    if(p_method)
      sdf_fn_uaGetMethodNameAsEnum(p_method, &dMsgType, &Error);

    ICF_SSA_SET_ECODE(p_ssa_ctxt->p_ssa_pdb,p_method,ICF_ERROR_INTERMIDIATE_RETRANSMISSION_TIMEOUT)
      Error.errCode = Sdf_en_noUaError;
    if(ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt)
    {
      ssa_pdb.p_ssa_ctxt->p_call_ctxt->\
        inc_response_code = ICF_NULL;
    }
  /* SPR17855: Added null check for klocworks warning removal*/   
  if(Sdf_co_null != p_method)
   {    
     if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INVITE")== 0)
     {
         ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
             stat_requests.stat_outbound_retrans_req.stats_INVITE)
     }
     else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"PRACK")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_PRACK)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"UPDATE")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_UPDATE)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"INFO")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_INFO)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"OPTIONS")== 0)
     {
       /* SPR 17862: Modified the method name as OPTIONS instead of 
          OPTION in strcmp */
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_OPTION)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"CANCEL")== 0)
     {
       ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_CANCEL)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"BYE")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_BYE)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"REGISTER")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_REGISTER)
     }

    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"SUBSCRIBE")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_SUBSCRIBE)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"REFER")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_REFER)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t *)"NOTIFY")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_NOTIFY)
     }
    else if(icf_port_strcmp((icf_uint8_t *)p_method,(icf_uint8_t*)"PUBLISH")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_PUBLISH)
     }
    else if(icf_port_strcmp((icf_uint8_t*)p_method,(icf_uint8_t*)"MESSAGE")== 0)
     {
        ICF_STATS(ssa_pdb.p_glb_pdb->p_stats_info->
           stat_requests.stat_outbound_retrans_req.stats_MESSAGE)
     }
   } 
 
  }/* if ( ICF_NULL != p_ssa_ctxt ... */

}

