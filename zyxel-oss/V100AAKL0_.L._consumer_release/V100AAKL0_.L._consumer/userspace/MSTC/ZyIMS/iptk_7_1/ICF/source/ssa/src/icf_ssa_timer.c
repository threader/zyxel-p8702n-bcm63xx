/*****************************************************************************
 ** FUNCTION:
 **                     This file contains utility functions invoked by the SSA
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_timer.c
 **
 ** DESCRIPTION:        This file contains timer implementation for SSA and 
 **                     UATK
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 20Feb2005           Aashish                             Initial
 ** 07-JUL-2005         Aashish Suchdev                     TCP checkin  
 ** 03-Mar-2006         Amit Sharma                     ICF merging with ICF2.0
 ** 30-Jan-2007         Mohit Sharma      SPR 14407     SSA context not freed 
 **                                                     after timer expiry.
 ** 30-Aug-2007         Amit Sharma      SPR:16031
 *****************************************************************************
 **                             Copyrights 2006, Aricent .
 *****************************************************************************/


#include "stack_headers.h"
#include "uatk_headers.h"

#include "icf_common_interface_types.h"
#include "icf_dbm_prototypes.h"
#include "icf_common_types.h"
#include "icf_common_interface_types.h"
#include "icf_common_defs.h"
#include "icf_internal_api_id.h"
#include "icf_macro.h"
#include "icf_port_defs.h"


#include "sdf_lookupport.h"

#include "icf_ssa_macro.h"
#include "icf_ssa_defs.h"
#include "icf_ssa_types.h"
#include "icf_ssa_prototypes.h"

extern icf_glb_pdb_st 	*p_persistent_glb_pdb;
extern icf_return_t icf_es_rmv_ares_fd(icf_global_port_info_st* p_glb_port_data);

/*****************************************************************************
 ** FUNCTION: 		sdf_cbk_uaStopTimer
 **
 * DESCRIPTION:	   Stop the specified timer. 
 *                 1.UA toolkit gives this callback to the application to stop
 *                 the timer of the value specified. 
 *                 2.The Toolkit passes the same timer handle(SSA timer data)
 *                 that the application had returned from the callback
 *                 sdf_ivk_uaStartTimer.
 *                 3. The context information that had been stored with the 
 *                 application (as an input to sdf_ivk_uaStartTimer) 
 *                 must be returned by the application to the Toolkit if 
 *                 the timer is succesfully stopped. The context 
 *                 information will be de-allocated by the Toolkit upon 
 *                 return from the sdf_ivk_uaStopTimer callback. 
 *
 *****************************************************************************/
Sdf_ty_retVal sdf_cbk_uaStopTimer
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid	 pVoidTimerHandle,
	 Sdf_ty_pvoid	*ppContextInfo, 
	 Sdf_st_error	*pError)
#else
	(pVoidTimerHandle, ppContextInfo, pError)
	 Sdf_ty_pvoid	 pVoidTimerHandle;
	 Sdf_ty_pvoid	*ppContextInfo; 
	 Sdf_st_error	*pError;
#endif
{
    icf_return_t             ret_val = ICF_SUCCESS;

    icf_ssa_timer_data_st    *p_ssa_timer_data =
        (icf_ssa_timer_data_st*)pVoidTimerHandle;

    icf_timer_data_st        *p_timer_data =
        p_ssa_timer_data->p_timer_data;

    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL; 

    icf_error_t              ecode;

    Sdf_ty_retVal               sdf_ret_val = Sdf_co_success;

    if(ICF_NULL == p_timer_data)
    {
        return Sdf_co_fail;
    }
    else
    {
        p_glb_pdb = p_timer_data->p_glb_pdb;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

        ICF_STOP_TIMER(p_glb_pdb, p_ssa_timer_data->timer_id, \
                &ecode, ret_val)
		ICF_PRINT(((icf_uint8_t *)"[SSA] Timer Id=%d stopped \n",
				p_ssa_timer_data->timer_id));
        if ( ret_val == ICF_FAILURE)
        {
#ifdef SDF_ERROR
            sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
                    (Sdf_ty_s8bit *)"sdf_cbk_uaStopTimer(): "
                    "Failed to stop timer",pError);
#endif
            pError->errCode = Sdf_en_systemError;
            /*Raise/Set Error here*/
            sdf_ret_val = Sdf_co_fail;
        }

    /*Return the context info to the UATK*/
    *ppContextInfo = p_ssa_timer_data->p_uatk_ctxt_info;

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
        return sdf_ret_val;
}

/***************************************************************************
 ** FUNCTION: 		sdf_cbk_uaStartTimer
 **
 * DESCRIPTION:    UA toolkit gives this callback to the application to start
 *                 the timer of the value specified.
 *                 1. UATK expects the application to store the pContextInfo 
 *                 passed as input and return this in case of corresponding 
 *                 Stop Timer or Timer expiry. Application need not interpret 
 *                 this pContext information.
 *                 2. The application specific data 
 *                 i.e SSA ctxt is passed through pAppData field. If this 
 *                 value is not Null, then it is equal to the application data
 *                 passed before calling sdf_ivk_uaDecodeMessage or 
 *                 sdf_ivk_uaHandleCall or sdf_ivk_uaUpdateCallDetails.
 *                 3. ppTimerHandle is a structure that application understands.
 *                 Application needs to allocate memory to it.
 *                 This pTimerHandle will be passed by UATK while calling 
 *                 Stop Timer callback.This pTimerHandle is the SSA timer data 
 *
 ***************************************************************************/
Sdf_ty_retVal sdf_cbk_uaStartTimer
#ifdef ANSI_PROTO
	(Sdf_ty_u32bit		dDuration,
	 Sdf_ty_timerType	dTimerType,
	 Sdf_ty_pvoid		pContextInfo,
	 Sdf_ty_pvoid		pAppData,
	 Sdf_ty_TimertimeOutFunc timeoutFunc,
	 Sdf_ty_pvoid		*ppTimerHandle,
	 Sdf_st_error		*pError)	
#else
	(dDuration, dTimerType, pContextInfo, pAppData, timeoutFunc, ppTimerHandle, pError)
	 Sdf_ty_u32bit		dDuration;
	 Sdf_ty_timerType	dTimerType;
	 Sdf_ty_pvoid		pContextInfo;
	 Sdf_ty_pvoid		pAppData;
	 Sdf_ty_TimertimeOutFunc timeoutFunc;
	 Sdf_ty_pvoid		*ppTimerHandle;
	 Sdf_st_error		*pError;
#endif
{
    icf_return_t             ret_val = ICF_SUCCESS;

    icf_timer_id_t           timer_id = 
        ICF_TIMER_ID_INVALID;

    icf_ssa_ctxt_st           *p_ssa_ctxt = ICF_NULL;

    icf_error_t              ecode;

   icf_glb_pdb_st		*p_glb_pdb;

    /* Extract the pointer to SSA PDB from the event context */

    icf_ssa_timer_data_st        *p_ssa_timer_data =
        ICF_NULL;

    icf_timer_data_st            *p_timer_data =
        ICF_NULL;

    Sdf_ty_retVal               sdf_ret_val = Sdf_co_success;
#ifdef ICF_SESSION_TIMER
    if ( Sdf_en_sessionTimer == dTimerType)
	{
	     p_ssa_ctxt = (icf_ssa_ctxt_st*)(((Sdf_st_sessionTimerContextInfo *)
			pContextInfo)->pCallObject->pAppData->pData);
             if (ICF_FALSE == p_ssa_ctxt->p_call_ctxt->session_timer_enable)
             {
                 /*sdf_ivk_uaFreeCallObject(&(pContextInfo)->p_call_obj));*/
                 (void)sdf_fn_uaFreeSessionTimerContextInfo(pContextInfo);
                 ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer, Session Timer is Disabled, no need to start timer.\n"))
                 return ICF_SUCCESS;
             }   
	}
	else
	{
#endif
		p_ssa_ctxt = (icf_ssa_ctxt_st *)pAppData;
#ifdef ICF_SESSION_TIMER
	}
#endif
    /* Assumption: here I have assumed that we will get the pAppData non NULL
     * always*/
p_glb_pdb = p_persistent_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

        /*Pseudocode*/
        /* We need to store pContextInfo and dTimerType in the timer
         * data structure.  These are to be passed back to the timeoutFunc.
         * So we define a timer_data_st for ssa
         */
    if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_glb_pdb, ICF_MEM_TIMER, (icf_void_t **)&p_timer_data, 
                &ecode))
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaStartTimer(): "
				"Failed to start timer",pError);
#endif
		pError->errCode = Sdf_en_systemError;
        ret_val = ICF_FAILURE;
        sdf_ret_val = Sdf_co_fail;
    }
    else if ( ICF_FAILURE == icf_dbm_get_mem_from_pool(\
                p_glb_pdb, ICF_MEM_SSA_TIMER, 
                (icf_void_t **)&p_ssa_timer_data,&ecode))
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaStartTimer(): "
				"Failed to start timer",pError);
#endif
		pError->errCode = Sdf_en_systemError;
        ret_val = ICF_FAILURE;
        sdf_ret_val = Sdf_co_fail;

        /*Free the allocated timer block here*/
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                p_timer_data, &ecode);
    }
    else
    {
        ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)

        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_SSA_UATK_TIMER_TYPE;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;

        /*In case of timer expiry, eshell will pass the p_timer_data
         * to SSA.SSA will then get the p_ssa_timer_data and invoke
         * UATK callback
         */
        p_timer_data->p_timer_buffer = 
            (icf_void_t*)p_ssa_timer_data;
        p_timer_data->p_glb_pdb = p_glb_pdb;


        ICF_START_TIMER(p_glb_pdb, p_timer_data, timer_id,
                dDuration, &ecode, ret_val)

            if ( ICF_FAILURE == ret_val)
            {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaStartTimer(): "
				"Failed to start timer",pError);
#endif
		pError->errCode = Sdf_en_systemError;
                sdf_ret_val = Sdf_co_fail;
                /*Free the allocated timer block here*/
                icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                        p_timer_data, &ecode);
                /*Free the allocated ssa timer block here*/
                icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                        p_ssa_timer_data, 
                        &ecode);
            }
            else
            {
				ICF_PRINT(((icf_uint8_t *)"[SSA] Timer Id=%d started for %dms \n",
							timer_id,dDuration));
        
                p_ssa_timer_data->p_ssa_ctxt = p_ssa_ctxt;

                p_ssa_timer_data->timer_type = dTimerType;
                p_ssa_timer_data->timer_id = timer_id;
                p_ssa_timer_data->p_timer_data = p_timer_data;
                p_ssa_timer_data->p_uatk_timer_cbk =
                    (icf_ssa_uatk_timeout_func)timeoutFunc;
                p_ssa_timer_data->p_uatk_ctxt_info =
                    pContextInfo;

                /*The address of the ssa timer data block is assigned
                 * to the handle.  The UATK will pass it in stop timer
                 * So we will be able to get the icf_timer_data_st
                 * from there and stop the port timer.
                 */
                *ppTimerHandle  = (Sdf_ty_pvoid)p_ssa_timer_data;
                /*Insert timer in the timer list*/

                ret_val = icf_ssa_insert_timer_in_list(p_glb_pdb,
                        &(p_ssa_ctxt->p_timer_list),
                        timer_id, (icf_void_t*)p_timer_data);

				if (ICF_SUCCESS == ret_val)
            	{
                	p_ssa_ctxt->timer_count++;
               	}
            }
            
    }

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
        return sdf_ret_val;
}

/*****************************************************************************
 **  FUNCTION    : sdf_cbk_uaFreeTimerHandle
 **
 **  DESCRIPTION : 1.UA toolkit gives this callback to the application to free 
 **                the Timer handle (timer blocks ).
 **                2.This handle is same as what provided by application during 
 **                starting of timer.
 **
 ****************************************************************************/
void sdf_cbk_uaFreeTimerHandle
#ifdef ANSI_PROTO
	(Sdf_ty_pvoid 	pTimerHandle)
#else
	(pTimerHandle)
	 Sdf_ty_pvoid	pTimerHandle;
#endif
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_timer_data_st    *p_ssa_timer_data = ICF_NULL;
    icf_timer_data_st        *p_timer_data = ICF_NULL;
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;

    icf_error_t               ecode;

    /* Need to take a local ssa_pdb */
    icf_ssa_pdb_st            ssa_pdb;

      p_ssa_timer_data = (icf_ssa_timer_data_st*)pTimerHandle;
    if(ICF_NULL == p_ssa_timer_data)
    {
        return;
    }
    p_timer_data = p_ssa_timer_data->p_timer_data;
    p_glb_pdb = p_ssa_timer_data->p_timer_data->p_glb_pdb;
    ret_val = ret_val;
    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

    icf_ssa_init_pdb(&ssa_pdb,p_glb_pdb);

    /* crash fix when p_ecode is null and is accessed later */
    ssa_pdb.p_ecode = &ecode;

    /*Delete timer from timer list*/
    icf_ssa_delete_timer_frm_list(&ssa_pdb,
            p_ssa_timer_data->p_ssa_ctxt,
            p_ssa_timer_data->timer_id);

        /*Free the allocated ssa timer block here*/
    ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
    icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                p_ssa_timer_data, 
                &ecode);

    /*Free the allocated timer block here*/
    icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
            p_timer_data, &ecode);

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_timer_expiry
 **
 ** DESCRIPTION:        This function process the timer expiry event
 **                     from MRM.
 **                     1. Only UATK timer expiry's are handled here .
 **                     TCP related timer expiry is handled separately in the 
 **                     function icf_ssa_purge_timer_expiry_handler
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_timer_expiry(
        icf_internal_msg_st *p_internal_msg)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    
    icf_ssa_pdb_st   ssa_pdb;
    icf_glb_pdb_st       *p_glb_pdb = ICF_NULL;
    
    icf_msg_st           *p_recv_msg = ICF_NULL;
    
    icf_timer_data_st    *p_timer_data =
        ICF_NULL;
    
    icf_ssa_timer_data_st    *p_ssa_timer_data =
        ICF_NULL;
    
    icf_ssa_glb_data_st      *p_ssa_glb_data = ICF_NULL;
    
    Sdf_st_error            sdf_error;

    icf_ssa_intrnl_intf_ut   internal_msg_payload;
    icf_ssa_ctxt_st*         p_ssa_ctxt = ICF_NULL;
    icf_uint32_t             msg_id = ICF_NULL;
    icf_uint32_t             ares_qid = ICF_NULL;
#ifdef IMS_CLIENT
    SipOptions              options;
    options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;
#endif

   Sdf_st_dnsEventContext   *p_dns_evt_ctxt = Sdf_co_null;
   icf_global_port_info_st*  p_glb_port_data = ICF_NULL;

   icf_boolean_t dns_failure  = ICF_TRUE;
   icf_void_t                *p_ares_ctxt = ICF_NULL;

   if (ICF_NULL == p_internal_msg)
   {
       return ICF_FAILURE;
   }
    p_glb_pdb =
        p_internal_msg->msg_hdr.p_glb_pdb;
   if (ICF_NULL == p_glb_pdb )
   {
       return ICF_FAILURE;
   }

   p_glb_port_data =  
                       p_glb_pdb->p_glb_data->p_glb_port_info;

   p_recv_msg =
        p_glb_pdb->p_recv_msg;

    ICF_FUNCTION_ENTER(p_glb_pdb)
        ICF_ASSERT(p_glb_pdb, p_internal_msg)

    /*Check if timer expiry is internally generated by SSA then
     * fetch timer data from internal_msg otherwise fetch timer
     * data from p_recv_msg payload*/
    if (ICF_SSA_INTERNAL_TIMER_EXPIRY == p_internal_msg->msg_hdr.msg_id)
    {
        p_timer_data = (icf_timer_data_st*)(p_internal_msg->p_msg_data);
        msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;
    }
    else
    {
        p_timer_data = *(icf_timer_data_st**)
            (p_recv_msg->payload);
#if 0
        p_timer_data = (icf_timer_data_st*)
            (p_recv_msg->payload);
#endif
    }
    if (ICF_NULL == p_timer_data)
    {
        return ICF_FAILURE;
    }
    icf_ssa_init_pdb(&ssa_pdb,p_glb_pdb);

    ssa_pdb.p_internal_msg = p_internal_msg;
    ssa_pdb.p_ecode = &(p_internal_msg->msg_hdr.ecode);	

#ifdef ICF_DNS_LOOKUP_ENABLED
    if(p_timer_data->p_timer_buffer)
	{
        /* 6_4_Nortel merge */
#if 0
			ICF_PRINT(("[SSA] Timer expired Id:%d\n",
			((icf_ssa_timer_data_st*)
            (p_timer_data->p_timer_buffer))->timer_id));	
#endif
      /*  if(p_glb_pdb->p_glb_data->p_dns_info->dns_buffer_timer_id ==((icf_ssa_timer_data_st*)
                                                (p_timer_data->p_timer_buffer))->timer_id)*/
        if(ICF_TIMER_TYPE_DNS_BUFFER_TIMER == p_timer_data->timer_type)
        {
            icf_error_t                     err;

            icf_ssa_dns_purge_timer_expire(p_glb_pdb, &err);
            return ICF_SUCCESS;
        }
         else if(ICF_TIMER_TYPE_DNS_RESP_TIMER == p_timer_data->timer_type)
         { 
              /* Retreiving dns event ctxt info from ssa timer buffer. */
              p_ssa_timer_data = (icf_ssa_timer_data_st *)p_timer_data->p_timer_buffer;
              p_dns_evt_ctxt = (Sdf_st_dnsEventContext *)p_ssa_timer_data->p_uatk_ctxt_info;
              p_ares_ctxt = p_ssa_timer_data->p_ares_ctxt;
              ares_qid = p_ssa_timer_data->ares_qid;

              /* Initialising SSA pdb with the ssa_ctxt retreving it from ssa
               * timer buffer.*/
              ssa_pdb.p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_ssa_timer_data->p_ssa_ctxt);
              ssa_pdb.p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
              icf_dbm_get_module_glb_data(p_glb_pdb,ICF_INT_MODULE_SSA,
                                          (icf_void_t **)&p_ssa_glb_data, ssa_pdb.p_ecode);
              ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;

              ssa_pdb.p_glb_cfg_data = ICF_NULL;
               
              icf_port_set_ares_fd(p_glb_port_data, ssa_pdb.p_ssa_ctxt->ares_fd);
              icf_es_rmv_ares_fd(p_glb_port_data);
              ssa_pdb.p_ssa_ctxt->ares_fd = ICF_NULL;
              if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_glb_pdb,
                            ICF_INT_MODULE_CFG,
                            (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                            ssa_pdb.p_ecode))
              {
                 ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in GLB Cfg data retreival!!!"));
                 return ICF_FAILURE;
              }
              else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                                                p_glb_pdb,
                                                (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                                                (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
                                                ssa_pdb.p_ecode))
              {
                 ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in GLB Regm data retreival!!!"));
                 return ICF_FAILURE;
              }

              /*Free the allocated ssa timer block here*/
              icf_dbm_ret_mem_to_pool(ssa_pdb.p_ssa_ctxt->p_glb_pdb, ICF_MEM_SSA_TIMER,
                                         p_timer_data->p_timer_buffer,ssa_pdb.p_ecode);
                  
              /*Free the allocated timer block here*/
              icf_dbm_ret_mem_to_pool(ssa_pdb.p_ssa_ctxt->p_glb_pdb, ICF_MEM_TIMER,
                                         p_timer_data, ssa_pdb.p_ecode);
         
              ssa_pdb.p_ssa_ctxt->dns_resp_timer_id = ICF_TIMER_ID_INVALID;
              ssa_pdb.p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;

              /*Free AresEventContext based on the ares_qid*/
              sdf_ivk_uaFreeAresContext(p_ares_ctxt, (icf_uint16_t) ares_qid);
              ssa_pdb.p_ssa_ctxt->ares_qid = ICF_NULL;              

              /*If this bitmask is set,then do not retransmit the DNS query*/ 
              if(!(ICF_SSA_DNS_RESP_RECEIVED & ssa_pdb.p_ssa_ctxt->bitmask_1)) 
              {
                  if(ssa_pdb.p_glb_cfg_data->dns_max_retrans > 
                                ssa_pdb.p_ssa_ctxt->dns_retrans_timer)
                  {
                            if(Sdf_co_success == 
                               sdf_fn_uaDnsQuery((icf_int8_t *)ssa_pdb.p_ssa_ctxt->fqdn.str,
                                                 p_dns_evt_ctxt,
                                                 &sdf_error))
                            { 
                                   dns_failure = ICF_FALSE;

                                   /* Free DnsEventContext of the Dns Query */
                                  (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
                            }
                 }
                 if(ICF_TRUE == dns_failure) 
                 {
                    p_internal_msg->p_msg_data  = (icf_uint8_t*)(&internal_msg_payload);
                    p_internal_msg->msg_hdr.payload_length = ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);
               
                    ret_val = icf_ssa_process_dns_fail_resp(&ssa_pdb,p_dns_evt_ctxt);
                      /* Free DnsEventContext of the Dns Query */
                     (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
                 }
       
                 if (ICF_FAILURE == ret_val)
	         {
	             ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Failure in handling DNS FAIL RSP"));
	        }
              }
              return ret_val;
         }
      }
#endif

    icf_dbm_get_module_glb_data(p_glb_pdb,ICF_INT_MODULE_SSA,
            (icf_void_t **)&p_ssa_glb_data, ssa_pdb.p_ecode);
    ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;
	ssa_pdb.p_conn_table_entry = ICF_NULL;
	ssa_pdb.common_bitmask = 0;
   
    ssa_pdb.p_glb_cfg_data = ICF_NULL;
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_glb_pdb,
                            ICF_INT_MODULE_CFG,
                            (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                            ssa_pdb.p_ecode))
    {
        return ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                p_glb_pdb,
                (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
                ssa_pdb.p_ecode))
    {
        return ICF_FAILURE;
    }

    p_internal_msg->p_msg_data  = (icf_uint8_t*)(&internal_msg_payload);
    p_internal_msg->msg_hdr.payload_length  = 
        ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);
	if ((p_timer_data->p_timer_buffer) && 
		(ICF_SSA_PURGE_TIMER_TYPE != p_timer_data->timer_type))
	{
		ICF_PRINT(((icf_uint8_t *)"[SSA] Timer expired Id:%d\n",
			((icf_ssa_timer_data_st*)
            (p_timer_data->p_timer_buffer))->timer_id));
	}

    ICF_ASSERT(p_glb_pdb, p_timer_data)

    if ( ICF_SSA_UATK_TIMER_TYPE == 
            p_timer_data->timer_type)
    {
             p_ssa_timer_data = (icf_ssa_timer_data_st*)
            (p_timer_data->p_timer_buffer);

        ICF_ASSERT(p_glb_pdb, p_ssa_timer_data)
 
        p_ssa_ctxt = (icf_ssa_ctxt_st*)p_ssa_timer_data->p_ssa_ctxt;
        
        /*FORKING:- If This is not the call object on which last forked
         * response received then ignore the message*/
        if ((ICF_NULL != p_ssa_ctxt &&
	     ICF_NULL != p_ssa_ctxt->p_call_ctxt) && (ICF_NULL != 
                p_ssa_ctxt->p_call_ctxt->p_sip_ctx) 
                &&(p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx) &&
                (ICF_SSA_INTERNAL_TIMER_EXPIRY != msg_id))
        {
            /*Discuss: if we have to send ACK, also check memory
             * leak in this case*/
            return ICF_SUCCESS;
        }
        if (ICF_NULL == p_ssa_ctxt)
        {
            /*
              * Return SUCCESS as SSA ctxt is not available
              * This is a temporary fix.Revisit in 2.0
              */
              ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Timer expiry received when \
                 SSA ctxt is NULL !!!!\n\n"));
              ICF_FUNCTION_EXIT(p_glb_pdb)
              return ICF_SUCCESS;
         } 

    
        /* Copy the call context in glb pdb from timer data */
        if (ICF_NULL != p_ssa_ctxt->p_call_ctxt)
        {
            p_glb_pdb->p_call_ctx = 
                ((icf_ssa_ctxt_st*)(p_ssa_timer_data->p_ssa_ctxt))->p_call_ctxt;
        }

        p_ssa_ctxt->p_ssa_pdb = 
            (struct icf_ssa_pdb_st*)(&ssa_pdb);
    	ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;

        /*Used in Indicate txn timeout callback
         */
        ((icf_ssa_pdb_st*)
         (p_ssa_ctxt->p_ssa_pdb))->p_internal_msg =
            p_internal_msg;

        /*(Sdf_ty_TimertimeOutFunc)*/(p_ssa_timer_data->p_uatk_timer_cbk)
            ( ((Sdf_ty_timerType) p_ssa_timer_data->timer_type),
              ((Sdf_ty_pvoid) p_ssa_timer_data->p_uatk_ctxt_info),
              &sdf_error);

       /*
        * If the triggering of UATK callback resulted in triggering 
        * of IndicateTxnTimeout
        * callback in SSA, then the callback will fill the 
        * dest_module_id in the ssa_pdb
        * passed through ssa_ctxt in the callobject
        */
        if (( ICF_INT_MODULE_INVALID != 
                ssa_pdb.dest_module_id)  && 
                (ICF_SSA_INTERNAL_TIMER_EXPIRY != msg_id))\
        {

            /*Right now I am filling the destination module ID as that od
             * CM since we have only this timer.When Registartion comes
             * in, we will have REG TIMER also. So then we will use the timer
             * type given by UATK stored in the SSA timer data block
             * to distinguish between the two
             */
            ret_val = icf_ssa_process_otg_intrnl_msg(&ssa_pdb);

        }
#ifdef ICF_SESSION_TIMER
        else if ((ICF_NULL != p_glb_pdb->p_call_ctx) && \
                 (ICF_TRUE == p_glb_pdb->p_call_ctx->session_timer_enable) 
                 )
        {
		if (ICF_SSA_PENDING_SESSION_REFRESH == 
			(ICF_SSA_PENDING_SESSION_REFRESH & (ssa_pdb.common_bitmask)))
		{

			/* If session timer expires and ICF acts as session refresher
			 * then SSA will send RE-Invite or UPDATE. So ICF needs
			 * Application profile to make ALLOW header List
			 * FIX FOR CRASH
			 */
                        if (ICF_NULL == p_glb_pdb->p_call_ctx)
                        {
                                 ICF_PRINT(((icf_uint8_t *)"\n[SSA] :Session Timer Expired: Unable to Fetch Call Ctxt\n"))
                                 ret_val = ICF_FAILURE;
                        }
		        else if	(ICF_FAILURE == icf_cfg_fetch_app_conf(ssa_pdb.p_glb_pdb,
				p_glb_pdb->p_call_ctx->app_id,&(ssa_pdb.p_app_conf),ssa_pdb.p_ecode))
			{
				ICF_PRINT(((icf_uint8_t *)"\n[SSA] :Session Timer Expired: Unable to Fetch App Conf"
					"For App-ID = %d\n\n",p_glb_pdb->p_call_ctx->app_id));
				
				ret_val = ICF_FAILURE;
			}
			/* END-- FIX FOR CRASH */
			else
			{
				ret_val = icf_ssa_process_session_refresh_req(&ssa_pdb);
			}

		}
        }
        else if (ICF_NULL == p_glb_pdb->p_call_ctx)
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer, p_glb-pdb->p_call_ctxt is NULL.\n"))
            ret_val = ICF_FAILURE; 
        }
#endif
    }
	else if ( ICF_SSA_PURGE_TIMER_TYPE == p_timer_data->timer_type)
    {
      /*  ICF_PRINT(("\n[SSA]:PurgeTimerExpired!!")); */
        icf_ssa_purge_timer_expiry_handler(&ssa_pdb);
    }
#ifdef ICF_SSA_UT_NR_CODE
    else if ( ICF_SSA_OPTIONS_TIMER_TYPE == p_timer_data->timer_type)
    {
        ICF_SSA_DEBUG(("\n[SSA]:OptionsTimerExpired!!"));
        icf_ssa_options_timer_expiry_handler(&ssa_pdb);
        /*Free the allocated timer block here*/
        icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                p_timer_data, 
                &(p_internal_msg->msg_hdr.ecode));
    }
#endif
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_glb_pdb,ICF_TRUE)
#if 0
    else if((ICF_SSA_TIMER_RETRY_AFTER == (*p_p_timer_data)->timer_type)
		|| (ICF_SSA_TIMER_RE_SUBSCRIBE == (*p_p_timer_data)->timer_type))
    {
        
		p_ssa_ctxt = (icf_ssa_ctxt_st*)((*p_p_timer_data)->p_timer_buffer);

		ssa_pdb.p_call_obj = p_ssa_ctxt->p_call_obj;

		(icf_ssa_pdb_st *)(p_ssa_ctxt->p_ssa_pdb) = (icf_ssa_pdb_st*)&ssa_pdb;

        if (Sdf_co_fail == icf_ssa_send_subscribe_req((icf_ssa_pdb_st*)&ssa_pdb))
	    {
			/*Raise/Set Error/Alarm here UATK PROCESSING FAILURE*/
		    ret_val = ICF_FAILURE;
		}

    }	
#endif
ICF_CHECK_IMS_END
#endif
    ICF_FUNCTION_EXIT(p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_purge_timer_expiry_handler
 **
 ** DESCRIPTION:        This function travereses through the conn DB
 **                     database in global data and invokes the SM
 **                     for each valid Conn DB block with PurgeTimer
 **                     Expiry Event
 *****************************************************************************/
icf_return_t icf_ssa_purge_timer_expiry_handler(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_ssa_glb_data_st  *p_ssa_glb_data = 
        p_ssa_pdb->p_ssa_glb_data;
    icf_uint32_t          counter = 0;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    for ( counter = 0; counter < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS;
            counter++)
    {
        if ( ICF_SSA_TCPM_IDLE != 
         p_ssa_glb_data->p_ssa_tcpm_conn_db[counter].tcpm_sm_state)
        {
            p_ssa_pdb->p_conn_table_entry = &(p_ssa_glb_data->p_ssa_tcpm_conn_db[counter]);
            ICF_SSA_IVK_TCPSM(p_ssa_pdb, 
                    ICF_SSA_TCPM_PURGE_TIMER_XPRY, 
                    ret_val)
        }
    }

    if (ICF_NULL != p_ssa_pdb->p_ssa_glb_data->p_purge_timer)
    {
        if (ICF_SUCCESS == icf_dbm_ret_mem_to_pool (p_ssa_pdb->p_glb_pdb, 
                                            ICF_MEM_TIMER,
					    p_ssa_pdb->p_ssa_glb_data->p_purge_timer, 
                                            p_ssa_pdb->p_ecode))
        {
             p_ssa_pdb->p_ssa_glb_data->p_purge_timer = ICF_NULL;
             p_ssa_pdb->p_ssa_glb_data->purge_timer_id = ICF_NULL;
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    }

    if (ICF_FAILURE == ret_val)
    {}
    else if ( ICF_FAILURE == icf_ssa_start_purge_timer(p_ssa_pdb))
    {
        ICF_PRINT(((icf_uint8_t *)"[SSA]:FailureInRestartingPurgeTimer\n"));
    }
    else
    {
     /*   ICF_PRINT(("[SSA]:PurgeTimerRestartedSuccessfully!!\n")); */
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
}


#ifdef IMS_CLIENT
/***************************************************************************
 ** FUNCTION:           icf_ssa_start_timer
 **
 ** DESCRIPTION:        This function will be used to start
 **                     the timer. 
 ***************************************************************************/
icf_return_t icf_ssa_start_timer(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT    icf_timer_type_t timer_type,
        INOUT    icf_duration_t   timer_dur,
        INOUT icf_ssa_ctxt_st  *p_ssa_ctxt,
        INOUT icf_error_t *p_ecode)
{
    icf_timer_data_st *p_timer_data = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;

    if ( ICF_FAILURE == icf_dbm_create_timer_block(
                p_glb_pdb,(icf_void_t **)&p_timer_data,
                p_ecode))
    {
        ret_val = ICF_FAILURE;
	return ret_val;
    }

    /* fill the timer data structure */
    p_timer_data->timer_type = timer_type;
    p_timer_data->module_id = ICF_INT_MODULE_SSA;
   
    p_timer_data->p_timer_buffer = p_ssa_ctxt;

    p_timer_data->p_glb_pdb = p_glb_pdb;

    ICF_START_TIMER(p_glb_pdb, p_timer_data, p_ssa_ctxt->timer_id,
                timer_dur, p_ecode, ret_val)

    if ( ICF_FAILURE == ret_val)
    {
	icf_dbm_free_timer_block(p_glb_pdb, p_timer_data, p_ecode);
    }
    else
    {
	p_ssa_ctxt->p_timer_data = p_timer_data;
    }	

    return ret_val;
  
} 
#endif
#ifdef ICF_SSA_UT_NR_CODE
/*****************************************************************************
 * FUNCTION:        icf_ssa_options_timer_expiry_handler
 *
 * DESCRIPTION:     This function handles the timer expiry of the
 *                  OPTIONS timer and sends failure response to peer.
 *                  1. Fetch SSA ctxt from the timer buffer
 *                  2. Flush entry from the global map
 *                  3. Sne dfailure resp to peer
 * 
 *****************************************************************************/
icf_return_t icf_ssa_options_timer_expiry_handler(
        icf_ssa_pdb_st *p_ssa_pdb)
{
    icf_return_t         ret_val=ICF_SUCCESS;
    icf_glb_pdb_st       *p_glb_pdb =
        p_ssa_pdb->p_internal_msg->msg_hdr.p_glb_pdb;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_msg_st           *p_recv_msg =
        p_glb_pdb->p_recv_msg;

    icf_timer_data_st    **p_p_timer_data =
        ICF_NULL;
    Sdf_st_overlapTransInfo *p_ovtxn = Sdf_co_null;
    SipOptions              options;
    Sdf_st_error            sdf_error;

    /* Extract the timer buffer which stores the SSA ctxt */
    p_p_timer_data = (icf_timer_data_st**)
       (p_recv_msg->payload);

    /* Extract SSA ctxt from the timer buffer */
    p_ssa_ctxt = (icf_ssa_ctxt_st*)
         ((*p_p_timer_data)->p_timer_buffer); 

    /* Update the SSA ctxt pointer into the SSA PDB */
    p_ssa_pdb->p_ssa_ctxt = p_ssa_ctxt;
    
    /* Specify the stack options */
    options.dOption = SIP_OPT_CLEN|SIP_OPT_FULLFORM;

    /*
     * Fetch the overlapped transaction pOverlapTransInfo 
     * stored in the call object corresponding to OPTIONS
     * request received
     */ 
    if( ICF_FAILURE == icf_ssa_fetch_matching_overlap_txn(
                p_ssa_pdb,Sdf_en_options,Sdf_en_transactionReceived,
                p_ssa_ctxt->p_call_obj,&p_ovtxn))
    {
        /*Raise/Set an error here - UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (Sdf_co_fail == sdf_ivk_uaFormResponse(500, "OPTIONS", 
                p_ssa_ctxt->p_call_obj,
                p_ovtxn,
                Sdf_co_false,
                &sdf_error))
    {
        /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    else if (ICF_FAILURE == icf_ssa_send_to_peer(p_ssa_pdb,
                p_ssa_ctxt->p_call_obj,
                p_ovtxn->pSipMsg,
                &options,&sdf_error))
    {
        /*Raise/Set Error here:UATK_PROCESSING_FAILURE*/
        ret_val = ICF_FAILURE;
    }
    if (ICF_NULL != 
            (p_ssa_ctxt->bitmask & ICF_SSA_OPTIONS_RECVD_OUT_OF_DIALOG))
    {
        /* If OPTIONS is recvd out of dialog free the callobject */
        sdf_ivk_uaFreeCallObject(&(p_ssa_ctxt->p_call_obj));
    }
    /* 
     * Reset the mapping in the map maintained 
     * in the SSA global data .This functions also resets 
     * the bit indicating that OPTIONS is ongoing
     */
    if (ICF_NULL != p_ssa_pdb->p_ssa_ctxt)
    {
        icf_ssa_flush_entry_frm_map(p_ssa_pdb,p_ssa_ctxt);
    }
    return ret_val;
}
#endif


