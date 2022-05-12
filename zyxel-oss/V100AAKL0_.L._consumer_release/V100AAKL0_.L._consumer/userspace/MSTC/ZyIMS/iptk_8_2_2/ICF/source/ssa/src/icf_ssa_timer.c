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
 ** 23-Jan-2008         Shruti Thakral                  Enhancements for Rel 8.0
 ** 06-Apr-2008         Tarun Gupta      SPR 18355      Merged Windows Porting Changes
 ** 18-Apr-2008         Alok Tiwari      SPR 18456      Klocwork warning resolved
 ** 01-May-2008         Amit Sharma      SPR 18390     Fix for IPTK crash in TLS case
 **                                                    when Pri-server is not responding
 ** 17-May-2008         Amit Sharma      SPR 18577     Assigning APP ctxt to glb_pdb
 ** 24-June-2008        Jagmohan Chauhan SPR 18684     CSR Merge 1-6048221
 ** 16-Jul-2008         Anuradha Gupta   SPR 18787     NULL check added for call context
 ** 04-Nov-2008         Tarun Gupta      SPR 19189     Merged Rel 7.1.2 SPR 18880
 ** 07-Nov-2008         Rajiv Kumar      SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                                     18909) 
 ** 25-Nov-2008         Rajiv Kumar      SPR 19215     CSR Merge 1-6259119    
 ** 04-Dec-2008         Rajiv Kumar      SPR 19218     CSR-1-6291842 Merged
 ** 08-Dec-2008       Abhishek Dhammawat SPR 19223     Klocwork warning
 **                                                    removal 
 ** 08-Dec-2008       Rajiv Kumar        SPR 19218     Merged CSR 1-6431768
 ** 02-Mar-2009     Alok Tiwari      IPTK Rel8.1 IPTK open source replacement:
 **                                              Data related to ares has
 **                                              been removed.
 ** 13-Mar-2009     Rajiv Kumar      IPTK Rel8.1 IPTK open source
 **                                              replacement:DNS CLIENT
 ** 4-Apr-2009      Rajiv Kumar      SPR 19423   Klocwork Warning Fix.  
 ** 11-Jun-2009 Ashutosh Mohan       SPR 19737   Changes done for GCC 4.1 warnings removal
 ** 25-Jul-2009    Abhishek Dhammawat SPR 19875  Modified
 **                                              icf_ssa_process_timer_expiry 
 ** 14-Sep-2009     Preksha          SPR 19999   Merged CSR 1-7694021
 ** 27-Oct-2009    Anuradha Gupta    SPR 20174   Modified
 **                                              icf_ssa_process_timer_expiry 
 *****************************************************************************
 **                             Copyrights 2009, Aricent .
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
    icf_ssa_timer_data_st    *p_ssa_timer_data = ICF_NULL;
    icf_timer_data_st        *p_timer_data = ICF_NULL;
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL; 
    icf_error_t              ecode = ICF_ERROR_NONE;
    Sdf_ty_retVal            sdf_ret_val = Sdf_co_success;

    /* SPR 18880 : Added NULL checks */
    if(Sdf_co_null == pVoidTimerHandle)
    {
        ICF_PRINT(((icf_uint8_t *)("[SSA] pVoidTimerHandle is NULL.\n")))
        return Sdf_co_fail;
    }
    p_ssa_timer_data = (icf_ssa_timer_data_st*)pVoidTimerHandle;
    p_timer_data = p_ssa_timer_data->p_timer_data;
    if(ICF_NULL == p_timer_data)
    {
        ICF_PRINT(((icf_uint8_t *)("[SSA] p_timer_data is NULL.\n")))
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
    if(ICF_FAILURE == ret_val)
    {
#ifdef SDF_ERROR
        sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
                (Sdf_ty_s8bit *)"sdf_cbk_uaStopTimer(): "
                "Failed to stop timer",pError);
#endif
        if(Sdf_co_null != pError)
        {
            pError->errCode = Sdf_en_systemError;
        }
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
    icf_timer_id_t           timer_id = ICF_TIMER_ID_INVALID;
    icf_ssa_ctxt_st          *p_ssa_ctxt = ICF_NULL;
    icf_error_t              ecode = ICF_ERROR_NONE;
    icf_glb_pdb_st		     *p_glb_pdb = ICF_NULL;
    icf_ssa_timer_data_st    *p_ssa_timer_data = ICF_NULL;
    icf_timer_data_st        *p_timer_data = ICF_NULL;
    Sdf_ty_retVal            sdf_ret_val = Sdf_co_success;

    p_glb_pdb = p_persistent_glb_pdb;
 
    /* SPR 18880 */
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA]: p_glb_pdb is NULL.n")))
        return Sdf_co_fail;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb) /* function entry trace */

#ifdef ICF_SESSION_TIMER
    if(Sdf_en_sessionTimer == dTimerType)
	{
        /* SPR 18880 : Added Null checks */
        if((Sdf_co_null == pContextInfo) ||
           (Sdf_co_null == ((Sdf_st_sessionTimerContextInfo *)pContextInfo)->pCallObject) ||
           (Sdf_co_null == ((Sdf_st_sessionTimerContextInfo *)pContextInfo)->pCallObject->pAppData))
        {
            ICF_PRINT(((icf_uint8_t *)("\n[SSA]: Input data to the callback is NULL.\n")))
            return Sdf_co_fail;
        }

	    p_ssa_ctxt = (icf_ssa_ctxt_st*)(((Sdf_st_sessionTimerContextInfo *)
			pContextInfo)->pCallObject->pAppData->pData);
        if((ICF_NULL == p_ssa_ctxt) ||
           (ICF_NULL == p_ssa_ctxt->p_call_ctxt))
        {
            /*SPR 18787: No need to start the session timer as the call context
             * does not exist
             */
            (void)sdf_fn_uaFreeSessionTimerContextInfo(pContextInfo);
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer, call context not present, no need to start timer.\n"))
            return Sdf_co_fail; 
        }
        else if(ICF_FALSE == p_ssa_ctxt->p_call_ctxt->session_timer_enable)
        {
            /*sdf_ivk_uaFreeCallObject(&(pContextInfo)->p_call_obj));*/
            (void)sdf_fn_uaFreeSessionTimerContextInfo(pContextInfo);
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer, Session Timer is Disabled, no need to start timer.\n"))
            return Sdf_co_success;
        } 
	}
	else
	{
#endif
        if(Sdf_co_null == pAppData)
        {
            ICF_PRINT(((icf_uint8_t *)("\n[SSA]: pAppData is NULL.\n")))
            return Sdf_co_fail;
        }
		p_ssa_ctxt = (icf_ssa_ctxt_st *)pAppData;
#ifdef ICF_SESSION_TIMER
	}
#endif

    /* Pseudocode*/
    /* We need to store pContextInfo and dTimerType in the timer
     * data structure.  These are to be passed back to the timeoutFunc.
     * So we define a timer_data_st for ssa
     */
    if(ICF_FAILURE == icf_dbm_get_mem_from_pool(
                p_glb_pdb, ICF_MEM_TIMER, (icf_void_t **)&p_timer_data, 
                &ecode))
    {
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaStartTimer(): "
				"Failed to start timer",pError);
#endif
        if(Sdf_co_null != pError)
        {
            pError->errCode = Sdf_en_systemError;
        }
        ret_val = ICF_FAILURE;
        sdf_ret_val = Sdf_co_fail;
        /*Klocwork Warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_timer_data)
    }
    else if(ICF_FAILURE == icf_dbm_get_mem_from_pool(\
                p_glb_pdb, ICF_MEM_SSA_TIMER, 
                (icf_void_t **)&p_ssa_timer_data,&ecode))
    {
        /*Klocwork Warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_timer_data)
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_ssa_timer_data)
#ifdef SDF_ERROR
		sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
			(Sdf_ty_s8bit *)"sdf_cbk_uaStartTimer(): "
				"Failed to start timer",pError);
#endif
        if(Sdf_co_null != pError)
        {
            pError->errCode = Sdf_en_systemError;
        }
        ret_val = ICF_FAILURE;
        sdf_ret_val = Sdf_co_fail;

        /*Free the allocated timer block here*/
        if(ICF_NULL != p_timer_data)
        {
            ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                                    p_timer_data, &ecode);
            if(ICF_SUCCESS == ret_val)
            {
                p_timer_data = ICF_NULL;
            }
            /* Timer block freeing failure. Raise a alarm and proceed. */
            else
            {
                ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
            }
        }
    }
    else if((ICF_NULL != p_timer_data) &&
            (ICF_NULL != p_ssa_timer_data))
    {
        /*Klocwork Warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_timer_data)
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_ssa_timer_data)

        ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)

        /* fill the timer data structure */
        p_timer_data->timer_type = ICF_SSA_UATK_TIMER_TYPE;
        p_timer_data->module_id = ICF_INT_MODULE_SSA;

        /*In case of timer expiry, eshell will pass the p_timer_data
         * to SSA.SSA will then get the p_ssa_timer_data and invoke
         * UATK callback
         */
        p_timer_data->p_timer_buffer = (icf_void_t*)p_ssa_timer_data;
        p_timer_data->p_glb_pdb = p_glb_pdb;


        ICF_START_TIMER(p_glb_pdb, p_timer_data, timer_id,
                dDuration, &ecode, ret_val)

        if(ICF_FAILURE == ret_val)
        {
#ifdef SDF_ERROR
	    	sdf_fn_setError(Sdf_en_minorErrors,Sdf_en_systemError,\
		    	(Sdf_ty_s8bit *)"sdf_cbk_uaStartTimer(): "
			    	"Failed to start timer",pError);
#endif
            if(Sdf_co_null != pError)
            {
                pError->errCode = Sdf_en_systemError;
            }
            sdf_ret_val = Sdf_co_fail;
            /*Free the allocated timer block here */
            if(ICF_NULL != p_timer_data)
            {
                ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                                        p_timer_data, &ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    p_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }
            /*Free the allocated ssa timer block here */
            if(ICF_NULL != p_ssa_timer_data)
            {
                ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
                ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                                        p_ssa_timer_data, &ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    p_ssa_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }
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
            p_ssa_timer_data->p_uatk_ctxt_info = pContextInfo;

            /*The address of the ssa timer data block is assigned
             * to the handle.  The UATK will pass it in stop timer
             * So we will be able to get the icf_timer_data_st
             * from there and stop the port timer.
             */
			if(Sdf_co_null != ppTimerHandle)
                *ppTimerHandle  = (Sdf_ty_pvoid)p_ssa_timer_data;

            /*Insert timer in the timer list*/

            ret_val = icf_ssa_insert_timer_in_list(p_glb_pdb,
                          &(p_ssa_ctxt->p_timer_list),
                          timer_id, (icf_void_t*)p_timer_data);

	    	if(ICF_SUCCESS == ret_val)
            {
                p_ssa_ctxt->timer_count++;
            }
            /* klocwork warning removal*/
            else
            {
                /*Free the allocated timer block here */
                if(ICF_NULL != p_timer_data)
                {
                    ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                                        p_timer_data, &ecode);
                    if(ICF_SUCCESS == ret_val)
                    {
                        p_timer_data = ICF_NULL;
                    }
                    /* Timer block freeing failure. Raise a alarm and proceed. */
                    else
                    {
                        ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                    }
                }
                /*Free the allocated ssa timer block here */
                if(ICF_NULL != p_ssa_timer_data)
                {
                    ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                                        p_ssa_timer_data, &ecode);
                    if(ICF_SUCCESS == ret_val)
                    {
                        p_ssa_timer_data = ICF_NULL;
                    }
                    /* Timer block freeing failure. Raise a alarm and proceed. */
                    else
                    {
                        ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                    }
                }
            }
        }   
    }
    else
    {
        /*SPR 19223 Klocwork Warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_timer_data)
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,p_ssa_timer_data)
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

    icf_error_t              ecode = ICF_ERROR_NONE;

    /* Need to take a local ssa_pdb */
    icf_ssa_pdb_st           ssa_pdb;

    p_ssa_timer_data = (icf_ssa_timer_data_st*)pTimerHandle;

    if((ICF_NULL == p_ssa_timer_data) ||
       (ICF_NULL == p_ssa_timer_data->p_timer_data) ||
       (ICF_NULL == p_ssa_timer_data->p_timer_data->p_glb_pdb) ||
       (ICF_NULL == p_ssa_timer_data->p_ssa_ctxt))
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA]: Input data to the callback is NULL.\n")))
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

    /*Free the allocated ssa timer block here */
    ICF_SSA_RESET_SSA_TIMER_BLK(p_ssa_timer_data)
    ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SSA_TIMER,
                            p_ssa_timer_data, 
                            &ecode);
    if(ICF_SUCCESS == ret_val)
    {
        p_ssa_timer_data = ICF_NULL;
    }
    /* Timer block freeing failure. Raise a alarm and proceed. */
    else
    {
        ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
    }

    /*Free the allocated timer block here and assign default values */
    ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                            p_timer_data, &ecode);
    if(ICF_SUCCESS == ret_val)
    {
        p_timer_data = ICF_NULL;
    }
    /* Timer block freeing failure. Raise a alarm and proceed. */
    else
    {
        ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
    }

    ICF_FUNCTION_EXIT(p_glb_pdb) /* function exit trace */
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_process_timer_expiry
 **
 ** DESCRIPTION:        This function process the timer expiry event
 **                     from MRM.
 **                     1. Only UATK timer expiry's are handled here .
 **                     TCP related timer expiry is handled separately in the 
 **                     function icf_ssa_purge_timer_expiry_handler.
 **
 **                     DNS request is retransmitted to same DNS Server until 
 **                     all retransmissions (and failure is sent to concerned 
 **                     module. After all retransmissions, DNS request is sent 
 **                     to secondary DNS server
 **
 *****************************************************************************/
icf_return_t icf_ssa_process_timer_expiry(
        icf_internal_msg_st *p_internal_msg)
{
    icf_return_t             ret_val = ICF_SUCCESS;
    icf_ssa_pdb_st           ssa_pdb;
    icf_glb_pdb_st           *p_glb_pdb = ICF_NULL;
    icf_msg_st               *p_recv_msg = ICF_NULL;
    icf_timer_data_st        *p_timer_data = ICF_NULL;
    icf_ssa_timer_data_st    *p_ssa_timer_data = ICF_NULL;
    icf_ssa_glb_data_st      *p_ssa_glb_data = ICF_NULL;
    Sdf_st_error             sdf_error = {0,0,0,"\0"};
    /* Local variable is replaced with pointer variable to resolve the klocwork
     * warning*/    
    icf_ssa_intrnl_intf_ut   *p_internal_msg_payload = ICF_NULL;
    icf_ssa_ctxt_st*         p_ssa_ctxt = ICF_NULL;
    icf_uint32_t             msg_id = ICF_NULL;
#ifdef IMS_CLIENT
    SipOptions               options;
    options.dOption =        SIP_OPT_CLEN|SIP_OPT_FULLFORM;
#endif
    Sdf_st_dnsEventContext   *p_dns_evt_ctxt = Sdf_co_null;
    icf_global_port_info_st* p_glb_port_data = ICF_NULL;
    icf_boolean_t            dns_failure   = ICF_TRUE;
    /* Boolean to check whether some session timer expiry happens .Initialised
     * to FLASE*/
    icf_boolean_t            session_timer_exp = ICF_FALSE;
    icf_dns_buffer_st        *p_dns_record = ICF_NULL;
    icf_error_t              err = ICF_ERROR_NONE;
    icf_return_t             return_val = ICF_SUCCESS;
#ifdef ICF_DNS_LOOKUP_ENABLED
	icf_boolean_t			dns_map_change_ind_sent = ICF_FALSE;
#endif
    Sdf_st_txnContextData *pTxnContextData = Sdf_co_null;
    Sdf_st_overlapTransInfo *pOverlapTransInfo = Sdf_co_null;
    Sdf_st_msgInfo		  dMessageInfo;

    if(ICF_NULL == p_internal_msg)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_internal_msg is NULL\n.")))
        return ICF_FAILURE;
    }
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    if(ICF_NULL == p_glb_pdb)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_glb_pdb is NULL\n.")))
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18909)*/
    /* SPR 18909 : Removed redundant NULL check for p_recv_msg as it 
       is being done later in the code wherein p_timer_data is being
       extracted from it and being checked for NULL.
    */
    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                                          p_glb_pdb->p_glb_data,
                                          p_glb_pdb->p_glb_data->p_glb_port_info,
                                          return_val)        
   
    if(ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    p_glb_port_data = p_glb_pdb->p_glb_data->p_glb_port_info;
    p_recv_msg = p_glb_pdb->p_recv_msg;

    /*Check if timer expiry is internally generated by SSA then
     * fetch timer data from internal_msg otherwise fetch timer
     * data from p_recv_msg payload*/
    if(ICF_SSA_INTERNAL_TIMER_EXPIRY == p_internal_msg->msg_hdr.msg_id)
    {
        p_timer_data = (icf_timer_data_st*)(p_internal_msg->p_msg_data);
        msg_id = ICF_SSA_INTERNAL_TIMER_EXPIRY;
    }
    else
    {
        p_timer_data = *(icf_timer_data_st**)(p_recv_msg->payload);
    }
    if(ICF_NULL == p_timer_data)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_timer_data is NULL\n.")))
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }
    /* SPR 20174: In case timer_buffer is NULL then no use of proceeding
     * further as the tiemr related information is present in the 
     * timer buffer only. This is an invalid case but in case it happens
     * then failure should be returned from this function.
     */
    else if(ICF_NULL == p_timer_data->p_timer_buffer)
    {
        ret_val = icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_TIMER,
                                        p_timer_data, &err);

        ICF_PRINT(((icf_uint8_t *)("\n[SSA] p_timer_buffer is NULL\n.")))
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    }

    icf_ssa_init_pdb(&ssa_pdb,p_glb_pdb);

    ssa_pdb.p_internal_msg = p_internal_msg;
    ssa_pdb.p_ecode = &(p_internal_msg->msg_hdr.ecode);	

    /* SPR 19189 */
    icf_dbm_get_module_glb_data(p_glb_pdb,
                                ICF_INT_MODULE_SSA,
                                (icf_void_t **)&p_ssa_glb_data, 
                                ssa_pdb.p_ecode);
    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_ssa_glb_data,
                             return_val)
    if(ICF_FAILURE == return_val)
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA]:Failure in SSA GLB Cfg data retreival!!!")))
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ret_val;
    }

#ifdef ICF_DNS_LOOKUP_ENABLED

    /* Process only if no pointer has been found as NULL so far.
       The NULL check for p_timer_data->p_timer_buffer has been put
       for timer type ICF_TIMER_TYPE_DNS_BUFFER_TIMER 
    */
       
    if(ICF_NULL != p_timer_data->p_timer_buffer)
	{
        if(ICF_TIMER_TYPE_DNS_BUFFER_TIMER == p_timer_data->timer_type)
        {
            icf_error_t                     err = ICF_ERROR_NONE;
            icf_ssa_dns_purge_timer_expire(p_glb_pdb, &err);
            return ICF_SUCCESS;
        }
        else if(ICF_TIMER_TYPE_DNS_RESP_TIMER == p_timer_data->timer_type)
        {
            /* SPR 18880 : Check for NULL pointer values accessed while processing
               timer type ICF_TIMER_TYPE_DNS_RESP_TIMER 
            */
            ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
            ((icf_ssa_timer_data_st *)p_timer_data->p_timer_buffer)->p_uatk_ctxt_info,
            return_val)

            if((ICF_FAILURE == return_val) ||
              (ICF_NULL == ((icf_ssa_timer_data_st *)p_timer_data->p_timer_buffer)->p_ssa_ctxt))
            {
                ICF_PRINT(((icf_uint8_t *)("\n[SSA] Input data to function is NULL\n.")))
                ret_val = ICF_FAILURE;
                ICF_FUNCTION_EXIT(p_glb_pdb)
                return ICF_FAILURE;
            }

            /* Retreiving dns event ctxt info from ssa timer buffer. */
            p_ssa_timer_data = (icf_ssa_timer_data_st *)p_timer_data->p_timer_buffer;
            p_dns_evt_ctxt = (Sdf_st_dnsEventContext *)p_ssa_timer_data->p_uatk_ctxt_info;

            /* Initialising SSA pdb with the ssa_ctxt retreving it from ssa
             * timer buffer.*/
            ssa_pdb.p_ssa_ctxt = (icf_ssa_ctxt_st*)(p_ssa_timer_data->p_ssa_ctxt);
            ssa_pdb.p_ssa_ctxt->p_ssa_pdb = &ssa_pdb;
            /* CSR 1-6259119 */
            /* Set the glb_pdb pointer in SSA ctxt */
            ssa_pdb.p_ssa_ctxt->p_glb_pdb = p_glb_pdb;            
           
            ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;

            ssa_pdb.p_glb_cfg_data = ICF_NULL;

            /*Allocate module context into p_glb_pdb*/
            icf_ssa_alloc_app_ctx_in_glb_pdb(&ssa_pdb);

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

            /*Free the allocated ssa timer block here */
            if(ICF_NULL != p_timer_data->p_timer_buffer)
            {
                ICF_SSA_RESET_SSA_TIMER_BLK(((icf_ssa_timer_data_st *)
                                            p_timer_data->p_timer_buffer))
                ret_val = icf_dbm_ret_mem_to_pool(ssa_pdb.p_ssa_ctxt->p_glb_pdb,
                                        ICF_MEM_SSA_TIMER,
                                        p_timer_data->p_timer_buffer,
                                        ssa_pdb.p_ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    p_timer_data->p_timer_buffer = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }
                  
            if(ICF_NULL != p_timer_data)
            {
                /*Free the allocated timer block here and assign default values */
                ret_val = icf_dbm_ret_mem_to_pool(ssa_pdb.p_ssa_ctxt->p_glb_pdb,
                          ICF_MEM_TIMER,
                          p_timer_data,
                          ssa_pdb.p_ecode);
                if(ICF_SUCCESS == ret_val)
                {
                    ssa_pdb.p_ssa_ctxt->dns_resp_timer_id = ICF_TIMER_ID_INVALID;
                    ssa_pdb.p_ssa_ctxt->p_dns_resp_timer_data = ICF_NULL;
                    p_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }

            /* Fix for CSR 1-6291842 */
            /* DNS query should not be retried if call is being cleared forcefully */
            if ((ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt) &&
                    (ICF_TRUE == ssa_pdb.p_ssa_ctxt->p_call_ctxt->force_clr_rcvd))
            {
                ICF_PRINT(((icf_uint8_t*)"\n[SSA]: DNS query not retried for forceful call clear"));
                /* Free DnsEventContext of the Dns Query */
                (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
            }
            else    
            {
                /*CSR1-5414215 If IPTK receives response for DNS query, 
                  then donot retransmit it*/ 
                if(ICF_SUCCESS != icf_ssa_dns_get_ip_from_fqdn(ssa_pdb.p_ssa_ctxt->fqdn.str,
                            &p_dns_record,
                            &err))
                {
                    /* if more retransmissions required for this DNS request */
                    if(ssa_pdb.p_glb_cfg_data->dns_max_retrans > 
                       ssa_pdb.p_ssa_ctxt->dns_retrans_timer)
                    {
                        /* Changes done after DNS CLIENT replacement */
                        /* Previously we are retransmitting the DNS query from
                         * here,however DNS CLIENT will take care of this now
                         * as retransmission of DNS query will be taken care
                         * by DNS CLIENT.As a result if upper limit of DNS
                         * retrans timer is no achieved we are just restarting
                         * the DNS timer here which is acting as a guard timer
                         * for DNS response. */
                        if (ICF_SUCCESS == icf_ssa_dns_start_dns_resp_timer(\
                                    ssa_pdb.p_ssa_ctxt,p_dns_evt_ctxt,\
                                    ssa_pdb.p_ssa_ctxt->dns_retrans_timer,\
                                    &err))
                        {
                            /* Set the value of dns_failure as false which are
                             * indicating we are still going to wait for DNS
                             * response so do not clear the call.*/
                            dns_failure = ICF_FALSE;
                        }
                    }
                    else /* else no more retransmission required for this 
                          *  DNS request */
                    {
                        ret_val = icf_ssa_send_dns_map_change_ind(
                                    p_glb_pdb,
                                    ssa_pdb.p_ssa_ctxt->fqdn.str,
                                    ICF_NULL,
                                    (icf_uint8_t)ICF_DNS_MAP_CHANGE_QUERY_INVOCATION_TIMEOUT);
                        dns_map_change_ind_sent = ICF_TRUE;                        
                        /* if both DNS Servers have been tried */
                        if (ICF_SSA_MAX_DNS_SERVERS_TRIED == 
                                ssa_pdb.p_ssa_ctxt->dns_servers_tried)
                        {
                            /* set dns_failure to call 
                             * icf_ssa_process_dns_fail_resp() */
                            dns_failure = ICF_TRUE;
                            /*Mark secondary server inactive and raise trace for
                             *Corresponding primary or secondary DNS server not responding*/
                            if (ICF_TRUE == ssa_pdb.p_glb_cfg_data->sec_dns_server_active)
                            {
                                ssa_pdb.p_glb_cfg_data->sec_dns_server_active = 
                                    ICF_FALSE;
                                ICF_SSA_APP_TRACE(&ssa_pdb,
                                        ICF_TRACE_SECONDARY_DNS_SERVER_NOT_RESPONDING)
                            }
                            else
                            {
                                ICF_SSA_APP_TRACE(&ssa_pdb,
                                        ICF_TRACE_PRIMARY_DNS_SERVER_NOT_RESPONDING)
                            }
                            /* return ERROR cause */
                            *(ssa_pdb.p_ecode) = 
                                ICF_CAUSE_DNS_SERVER_NOT_RESPONDING;

                            /*If call context is not NULL then copy 
                             *call clear reason in release_cause */
                            if (ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt)
                            {
                                ssa_pdb.p_ssa_ctxt->p_call_ctxt->release_cause = 
                                    ICF_CALL_CLEAR_REASON_DNS_SERVER_NOT_RESPONDING;
                            }
                        }/* end if - both DNS Servers have been tried */
                        else /* request is to be sent to secondary DNS Server 
                              * (if configured) */
                        {
                            /*If sec_dns_server is active then raise trace for sec not responding
                             *else raise for primary not responding*/
                            if (ICF_TRUE == ssa_pdb.p_glb_cfg_data->sec_dns_server_active)
                            {
                                ICF_SSA_APP_TRACE(&ssa_pdb,
                                        ICF_TRACE_SECONDARY_DNS_SERVER_NOT_RESPONDING)
                            }
                            else
                            {
                                ICF_SSA_APP_TRACE(&ssa_pdb,
                                        ICF_TRACE_PRIMARY_DNS_SERVER_NOT_RESPONDING)
                            }
                            /* if secondary DNS Server is configured */
                            if (ICF_NULL != ssa_pdb.p_glb_cfg_data->
                                    p_sec_dns_server_addr)
                            {
                                /* increment dns_servers_tried to 2 */
                                ssa_pdb.p_ssa_ctxt->dns_servers_tried++;
                                /* Mark secondary DNS Server active */
                                if (ICF_FALSE == ssa_pdb.p_glb_cfg_data->sec_dns_server_active)
                                {
                                    ssa_pdb.p_glb_cfg_data->sec_dns_server_active = 
                                        ICF_TRUE;
                                    /* generate trace-secondary DNS Server is active */
                                    ICF_SSA_APP_TRACE(&ssa_pdb,
                                            ICF_TRACE_SECONDARY_DNS_SERVER_ACTIVE)
                                }
                                else
                                {
                                    /*Toggle DNS Server active flag*/
                                    ssa_pdb.p_glb_cfg_data->sec_dns_server_active = 
                                        ICF_FALSE;
                                }
                                /* send DNS query */
                                if(Sdf_co_success == 
                                        sdf_fn_uaDnsQuery((icf_int8_t *)ssa_pdb.p_ssa_ctxt->fqdn.str,
                                            p_dns_evt_ctxt,
                                            &sdf_error))
                                { 
                                    dns_failure = ICF_FALSE;

                                    /* Free DnsEventContext of the Dns Query */
                                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (
                                                                                p_dns_evt_ctxt);
                                }
                                /* Reset ssa_pdb.p_ssa_ctxt->dns_retrans_timer 
                                 * for the DNS request to secondary DNS Server */
                                ssa_pdb.p_ssa_ctxt->dns_retrans_timer = 0;
                            }/* end if - secondary DNS Server configured */
                            else /* secondary DNS Server not configured */
                            {
                                /* set dns_failure to call 
                                 * icf_ssa_process_dns_fail_resp() */
                                dns_failure = ICF_TRUE;
                                /* return ERROR cause */
                                *(ssa_pdb.p_ecode) = 
                                    ICF_CAUSE_DNS_SERVER_NOT_RESPONDING;

                                /*If call context is not NULL then copy 
                                 *call clear reason in release_cause */
                                if (ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt)
                                {
                                    ssa_pdb.p_ssa_ctxt->p_call_ctxt->release_cause
                                        = ICF_CALL_CLEAR_REASON_DNS_SERVER_NOT_RESPONDING;
                                }
                            }/* end else - secondary DNS server not configured */
                        }/* end else -request to be sent through sec. DNS server */
                    }/* end else - no more retransmissions required */
                }
                if(ICF_TRUE == dns_failure) 
                {
                    /* Since the scope of local variable is lost as soon as flow enter some
                     * other function, instead of saving the reference to local
                     * variable,we are allocating the memory for the same. This is done to
                     * resolve the klocwork warning.*/                    
                    ICF_MEMGET(p_glb_pdb,
                            ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut),ICF_MEM_COMMON,\
                            p_internal_msg_payload,\
                            ICF_DONOT_RET_ON_FAILURE,\
                            ssa_pdb.p_ecode, return_val)          

                    if (ICF_FAILURE == return_val)
                    {
                        ret_val = ICF_FAILURE;
                        ICF_PRINT(((icf_uint8_t *)"Memory allocation failure for internal message\n"));
                        ICF_FUNCTION_EXIT(p_glb_pdb)
                        return ICF_FAILURE;
                    }                    
                    p_internal_msg->p_msg_data  = (icf_uint8_t*)p_internal_msg_payload;
                    p_internal_msg->msg_hdr.payload_length = ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);

#ifdef ICF_STAT_ENABLE
                    /*Update DNS STATS for query response as failure, as TImer expiry means no 
                     *no response received for a DNS query*/
                    icf_ssa_stat_dns_resp_type(p_dns_evt_ctxt->dDnsRecordType, ICF_FALSE);
#endif
                    /*Store p_call_ctxt, p_rgm_ctxt and p_sic_ctxt in p_ssa_pdb
                     *from p_ssa_ctxt, as these are required in function
                     *icf_ssa_process_dns_fail_resp, to send response to CC/REGM/SIC
                     *in case of failure and icf_ssa_ctxt_st is already freed, SSA context
                     *can be freed when inavalid DNS response received for primary server address and
                     *some failure occurs while trying on sec server address*/

                    /* SPR 19189 : Added Null checks */
                    if(ICF_NULL != ssa_pdb.p_ssa_ctxt->p_call_ctxt)
                    {
                        ssa_pdb.p_call_ctxt = ssa_pdb.p_ssa_ctxt->p_call_ctxt;
                    }
                    if(ICF_NULL != ssa_pdb.p_ssa_ctxt->p_rgm_ctxt)
                    {
                        ssa_pdb.p_rgm_context = ssa_pdb.p_ssa_ctxt->p_rgm_ctxt;
                    }
                    if(ICF_NULL != ssa_pdb.p_ssa_ctxt->p_sic_ctxt)
                    {
                        ssa_pdb.p_sic_ctxt = ssa_pdb.p_ssa_ctxt->p_sic_ctxt;
                    }
                    /* SPR 19189 end */

                    ret_val = icf_ssa_process_dns_fail_resp(&ssa_pdb,
                            p_dns_evt_ctxt, 
                            dns_failure,
                            dns_map_change_ind_sent);
                    /* Free DnsEventContext of the Dns Query */
                    (Sdf_ty_void)sdf_ivk_uaFreeDnsEventContext (p_dns_evt_ctxt);
                }
            } /* (p_call_ctxt != ICF_NULL && force_clr_rcvd = ICF_TRUE) */
            /*** Update statistics for DNS query timeout ***/

            if(ICF_TRUE == dns_failure)
            {
                ICF_MEMFREE(p_glb_pdb,
                        p_internal_msg_payload,
                        ICF_MEM_COMMON,
                        ssa_pdb.p_ecode,ret_val)
                p_internal_msg->p_msg_data = ICF_NULL; 
            }
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ret_val;
        } /*Block end for DNS Response Timer expiry*/
    }/*Block End for p_timer_data->p_timer_buffer non-NULL*/
#endif

    ssa_pdb.p_ssa_glb_data = p_ssa_glb_data;
	ssa_pdb.p_conn_table_entry = ICF_NULL;
	ssa_pdb.common_bitmask = 0;
   
    ssa_pdb.p_glb_cfg_data = ICF_NULL;
    if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                          p_glb_pdb,
                          ICF_INT_MODULE_CFG,
                          (icf_void_t **)&(ssa_pdb.p_glb_cfg_data),
                          ssa_pdb.p_ecode))
    {
        return ICF_FAILURE;
    }
    else if(ICF_FAILURE == icf_dbm_get_module_glb_data(
                               p_glb_pdb,
                               (icf_int_module_id_t)ICF_INT_MODULE_REGM,
                               (icf_void_t *)&(ssa_pdb.p_glb_rgm_data),
                               ssa_pdb.p_ecode))
    {
        return ICF_FAILURE;
    }
    /* Since the scope of local variable is lost as soon as flow enter some
     * other function, instead of saving the reference to local
     * variable,we are allocating the memory for the same. This is done to
     * resolve the klocwork warning.*/
    ICF_MEMGET(p_glb_pdb,
            ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut),ICF_MEM_COMMON,\
            p_internal_msg_payload,\
            ICF_DONOT_RET_ON_FAILURE,\
            ssa_pdb.p_ecode, return_val)   
        
    if (ICF_FAILURE == return_val)
    {
        ret_val = ICF_FAILURE;
        ICF_PRINT(((icf_uint8_t *)"Memory allocation failure for internal message\n"));
        ICF_FUNCTION_EXIT(p_glb_pdb)
        return ICF_FAILURE;
    } 
    
    p_internal_msg->p_msg_data  = (icf_uint8_t*)p_internal_msg_payload;
    p_internal_msg->msg_hdr.payload_length  = 
        ICF_PORT_SIZEOF(icf_ssa_intrnl_intf_ut);

	if((ICF_NULL != p_timer_data) &&
       (ICF_NULL != p_timer_data->p_timer_buffer) && 
       (ICF_SSA_PURGE_TIMER_TYPE != p_timer_data->timer_type))
	{
		ICF_PRINT(((icf_uint8_t *)"[SSA] Timer expired Id:%d\n",
			((icf_ssa_timer_data_st*)(p_timer_data->p_timer_buffer))->timer_id));
	}


    if((ICF_NULL != p_timer_data) &&
       (ICF_NULL != p_timer_data->p_timer_buffer) &&     
       (ICF_SSA_UATK_TIMER_TYPE == p_timer_data->timer_type))
    {
        p_ssa_timer_data = (icf_ssa_timer_data_st*)
            (p_timer_data->p_timer_buffer);
          
        ICF_ASSERT(p_glb_pdb, p_ssa_timer_data)

        /* SPR 19189 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_ssa_timer_data,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_MEMFREE(p_glb_pdb,
                    p_internal_msg_payload,
                    ICF_MEM_COMMON,
                    ssa_pdb.p_ecode,return_val)
            p_internal_msg->p_msg_data = ICF_NULL;     

            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
        }
        /* SPR 19189 end */

        /*If there is some session timer expiry ,set the boolean
         * session_timer_exp as true*/

        if(Sdf_en_sessionTimer == p_ssa_timer_data->timer_type)  
        {
             session_timer_exp = ICF_TRUE;
        }
              
        p_ssa_ctxt = (icf_ssa_ctxt_st*)p_ssa_timer_data->p_ssa_ctxt;

        /*FORKING:- If This is not the call object on which last forked
         * response received then ignore the message*/
        if ((ICF_NULL != p_ssa_ctxt &&
             ICF_NULL != p_ssa_ctxt->p_call_ctxt) && (ICF_NULL != 
                         p_ssa_ctxt->p_call_ctxt->p_sip_ctx) 
             &&(p_ssa_ctxt != p_ssa_ctxt->p_call_ctxt->p_sip_ctx) &&
                        (ICF_SSA_INTERNAL_TIMER_EXPIRY != msg_id))
        {
            ICF_MEMFREE(p_glb_pdb,
                    p_internal_msg_payload,
                    ICF_MEM_COMMON,
                    ssa_pdb.p_ecode,return_val)
            p_internal_msg->p_msg_data = ICF_NULL;              
            /*Discuss: if we have to send ACK, also check memory
             * leak in this case*/
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_SUCCESS;
        }
       
        if(ICF_NULL == p_ssa_ctxt)
        {
            /*
             * Return SUCCESS as SSA ctxt is not available
             * This is a temporary fix.Revisit in 2.0
             */
            ICF_MEMFREE(p_glb_pdb,
                    p_internal_msg_payload,
                    ICF_MEM_COMMON,
                    ssa_pdb.p_ecode,return_val)
            p_internal_msg->p_msg_data = ICF_NULL;              
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]:Timer expiry received when \
                        SSA ctxt is NULL !!!!\n\n"));
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_SUCCESS;
        }

        /*
        At this moment we have a Non NULL p_ssa_ctxt.Proceed to validate the
        timer expiry received.

        Now call this function to do validation of the timer block pointers.
        We have extracted p_ssa_ctx from the timer expiry that is being received
        from MRM. This p_ssa_ctx also contains a pointer to the list of timer
        blocks that are running currently.This list should contain a pointer to
        the timer block which is received when this function is hit.We should
        handle the timer expiry received from MRM only if both the pointers are
        equal i.e. they point to the same timer block.If the pointers are
        different, we will free the timer block received from MRM. We will
        not free the timer block in p_ssa_ctx list as its timer expiry will
        happen sometime in future. On receiving the same from MRM,the
        pointers would match and thus the timer expiry would be handled.
        */  
       
        /* Filling p_ssa_ctxt in ssa_pdb. It would be used by the
           following function for timer validations. 
        */   

        ssa_pdb.p_ssa_ctxt = p_ssa_ctxt;

        if(ICF_FAILURE == icf_ssa_is_valid_timer_blk(&ssa_pdb,
                            p_ssa_timer_data))
        {
            ICF_PRINT(((icf_uint8_t *)"[SSA]:Invalid timer expiry received.\n"))

            /* Pointers did not match.Free the timer block received from MRM */
            if(ICF_NULL != p_timer_data)
            {
                ret_val = icf_dbm_free_timer_block(p_glb_pdb,
                          (icf_void_t *)(p_timer_data),ssa_pdb.p_ecode); 
                if(ICF_SUCCESS == ret_val)
                {
                    p_timer_data = ICF_NULL;
                }
                /* Timer block freeing failure. Raise a alarm and proceed. */
                else
                {
                    ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
                }
            }
            ret_val = ICF_FAILURE;
            ICF_MEMFREE(p_glb_pdb,
                    p_internal_msg_payload,
                    ICF_MEM_COMMON,
                    ssa_pdb.p_ecode,return_val)
            p_internal_msg->p_msg_data = ICF_NULL;              
            ICF_FUNCTION_EXIT(p_glb_pdb)
            return ICF_FAILURE;
        }

        /* Valid timer expiry received. Proceed to handle it */

        /* CSR Merge:1-6048221. Bit mask to identify that this is a timer of UATK. 
         *  This is used to inform SSA on return from UAK that message send on network 
         *  is a new mesasge or a re-transmission  */
        p_ssa_ctxt->bitmask_1 |= ICF_SSA_UATK_TIMER_TYPE_EXPIRE;

        /* Copy the call context in glb pdb from timer data */
        if (ICF_NULL != p_ssa_ctxt->p_call_ctxt)
        {
            p_glb_pdb->p_call_ctx = 
                ((icf_ssa_ctxt_st*)(p_ssa_timer_data->p_ssa_ctxt))->p_call_ctxt;
        }

        p_ssa_ctxt->p_ssa_pdb = (struct icf_ssa_pdb_st*)(&ssa_pdb);

        /*Used in Indicate txn timeout callback
         */
        ((icf_ssa_pdb_st*)(p_ssa_ctxt->p_ssa_pdb))->p_internal_msg = p_internal_msg;

        /*Allocate module context into p_glb_pdb*/
        icf_ssa_alloc_app_ctx_in_glb_pdb(&ssa_pdb);
        if(Sdf_co_null != p_ssa_timer_data->p_uatk_ctxt_info)
        {    
             pTxnContextData = (Sdf_st_txnContextData *) \
		       ((SipTxnTimeoutData *)(p_ssa_timer_data->p_uatk_ctxt_info))->pContextData;
        }     
        /*SPR 19875 - In case of TCP transport if remote party closes the TCP
         * connection after 200OK for INVITE being send by IPTK then there is
         * no provision for opening a new TCP connection for the
         * retransmission of 200OK and call establishment does not gets
         * happened because peer has closed TCP connection before receiving
         * the 200OK but IPTK acting as incoming end receives that close
         * indication after the 200OK was sent. Now to fix the issue if the
         * timer expired is Sdf_entxnLayer that is case when retranmission
         * message to be sent and protocol is TCP/TLS we shall check if the
         * transaction in retransmitted message does have fd invalid then
         * trigger new TCP connection establishment.*/
        if((Sdf_en_txnLayerTimer == p_ssa_timer_data->timer_type) &&
           (Sdf_co_null != pTxnContextData) &&
           (Sdf_co_null != pTxnContextData->pTransaction))
        {
#ifdef SDF_TLS            
           if((Sdf_en_protoTcp == ((Sdf_st_transaction*)(pTxnContextData->pTransaction))
                                  ->dSocket.dProtocol) ||
              (Sdf_en_protoTls == ((Sdf_st_transaction*)(pTxnContextData->pTransaction))
                                  ->dSocket.dProtocol))
#else
           if(Sdf_en_protoTcp == ((Sdf_st_transaction*)(pTxnContextData->pTransaction))
                                  ->dSocket.dProtocol) 
#endif     
           {             
                if(ICF_INVALID_SOCKFD == ((Sdf_st_transaction*)  
                         (pTxnContextData->pTransaction))->dSocket.dSockfd)     
                {
                    if(Sdf_co_success != sdf_fn_uaGetMessageInfo(
                       ((Sdf_st_transaction*)(pTxnContextData->pTransaction))->pSipMsg, 
                         &dMessageInfo, \
                         &sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else if (Sdf_co_fail == sdf_fn_uaDoSendMessageProcs(p_ssa_ctxt->p_call_obj, 
                             ((Sdf_st_transaction*)(pTxnContextData->pTransaction))->pSipMsg, 
                             &pOverlapTransInfo,\
                             dMessageInfo, &sdf_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else if(Sdf_co_success != sdf_fn_uaEstablishChannel(p_ssa_ctxt->p_call_obj, 
                                pOverlapTransInfo,
                                dMessageInfo.dReqRespType, 
                                dMessageInfo.dMsgType,
                                &sdf_error))
                    {
                        sdf_ivk_uaFreeOverlapTransInfo(pOverlapTransInfo);
                        ret_val = ICF_FAILURE;
                    }
                }
            }
        }
            
        (p_ssa_timer_data->p_uatk_timer_cbk)(
             ((Sdf_ty_timerType) p_ssa_timer_data->timer_type),
             ((Sdf_ty_pvoid) p_ssa_timer_data->p_uatk_ctxt_info),
             &sdf_error);
                
        /*
         * If the triggering of UATK callback resulted in triggering 
         * of IndicateTxnTimeout
         * callback in SSA, then the callback will fill the 
         * dest_module_id in the ssa_pdb
         * passed through ssa_ctxt in the callobject
         */
        if ((ICF_INT_MODULE_INVALID != ssa_pdb.dest_module_id)  && 
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
                 (ICF_TRUE == p_glb_pdb->p_call_ctx->session_timer_enable))
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
        /*Here we are checking whether the session_timer_exp is set to true or
         * false.If true it specify that there is some session timer expiry and
         * in this case we check whether the call context in glb_pdb in NULL
         * or not .If the call context is NULL in this case we must return
         * failure from this function because in that case based on failure
         * check we are taking some decision(in session timer handling).However 
         * we need not to return failure in other scenario based on ICF_NULL ==
         * p_glb_pdb->p_call_ctx check .*/
         
        else if ((ICF_TRUE == session_timer_exp) &&
                 (ICF_NULL == p_glb_pdb->p_call_ctx))
        {
            ICF_PRINT(((icf_uint8_t *)"\n[SSA]: Session Timer, p_glb-pdb->p_call_ctxt is NULL.\n"))
            ret_val = ICF_FAILURE; 
        }
#endif
    }
	else if ( ICF_SSA_PURGE_TIMER_TYPE == p_timer_data->timer_type)
    {
        icf_ssa_purge_timer_expiry_handler(&ssa_pdb);
        /*Klocwork Warning removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_glb_pdb,
                   ssa_pdb.p_ssa_glb_data->p_purge_timer)
    }

    /*This bit is set in ssa_pdb when primary server is not responding
     *and SSA sent request on secondary server, so call object with primary server
     *will be freed in this case*/
    if (ICF_SSA_DEALLOCATE_CALL_OBJECT & ssa_pdb.common_bitmask)
    {
        Sdf_st_callObject *p_call_obj = ssa_pdb.p_call_obj_to_dealloc;
        sdf_ivk_uaFreeCallObject(&p_call_obj);
    }
    ICF_MEMFREE(p_glb_pdb,
            p_internal_msg_payload,
            ICF_MEM_COMMON,
            ssa_pdb.p_ecode,return_val)
    p_internal_msg->p_msg_data = ICF_NULL;    
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
    icf_ssa_glb_data_st  *p_ssa_glb_data = ICF_NULL;
    icf_uint32_t          counter = 0;

    /* SPR 18880 : Added Null checks */
    if((ICF_NULL == p_ssa_pdb) ||
       (ICF_NULL == p_ssa_pdb->p_glb_pdb) ||
       (ICF_NULL == p_ssa_pdb->p_ssa_glb_data))
    {
        ICF_PRINT(((icf_uint8_t *)("\n[SSA]: Input data to the function is NULL.\n")))
        return ICF_FAILURE;
    }

    p_ssa_glb_data = p_ssa_pdb->p_ssa_glb_data;
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    for (counter = 0; counter < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS;
         counter++)
    {
        if(ICF_SSA_TCPM_IDLE != 
                p_ssa_glb_data->p_ssa_tcpm_conn_db[counter].tcpm_sm_state)
        {
            p_ssa_pdb->p_conn_table_entry = &(p_ssa_glb_data->p_ssa_tcpm_conn_db[counter]);
            ICF_SSA_IVK_TCPSM(p_ssa_pdb, 
                    ICF_SSA_TCPM_PURGE_TIMER_XPRY, 
                    ret_val)
            /*Fix for CSR 7694021 - SPR 20007*/
			/* IPTK was not closing connection opened for INVITE
				if 200 OK of INVITE was recieved on new port
				and new connection was used for ACK and BYE .
				Fix not specific to this CSR*/
	    	if (ICF_SSA_TCPM_CONNECTED == 
				p_ssa_glb_data->p_ssa_tcpm_conn_db[counter].tcpm_sm_state)	
	 	    {
			p_ssa_glb_data->p_ssa_tcpm_conn_db[counter].tcpm_sm_state	=
				ICF_SSA_TCPM_NOT_INUSE;
	        }
        }
    }

    if(ICF_NULL != p_ssa_pdb->p_ssa_glb_data->p_purge_timer)
    {
        if(ICF_SUCCESS == icf_dbm_ret_mem_to_pool (p_ssa_pdb->p_glb_pdb, 
                           ICF_MEM_TIMER,
				           p_ssa_pdb->p_ssa_glb_data->p_purge_timer, 
                           p_ssa_pdb->p_ecode))
        {
            p_ssa_pdb->p_ssa_glb_data->p_purge_timer = ICF_NULL;
            p_ssa_pdb->p_ssa_glb_data->purge_timer_id = ICF_NULL;
        }
        else
        {
            /* Timer block freeing failure. Raise a alarm and proceed. */
            ICF_PRINT(((icf_uint8_t *)"[SSA]: Failure in freeing timer block.\n"))
            ret_val = ICF_FAILURE;
        }
    }

    if(ICF_FAILURE == ret_val)
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


