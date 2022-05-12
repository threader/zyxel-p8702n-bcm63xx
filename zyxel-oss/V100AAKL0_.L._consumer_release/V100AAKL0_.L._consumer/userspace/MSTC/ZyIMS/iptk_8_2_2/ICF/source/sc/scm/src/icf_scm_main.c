/******************************************************************************
 *
 * FILE NAME    : icf_scm_main.c
 *
 * DESCRIPTION  : Contains the definitions of SCM service entry point functions
 *
 *
 * Revision History :
 *    DATE            NAME       REFERENCE       REASON
 * --------    ------------      ---------      ---------------
 * 11-Jan-2005    Navneet Gupta   None            Initial
 * 12-Apr-2005    Amit Sharma                     Updation in process mesg for 
 *                                                timer expiry 
 *
 * 21-Sep2005     Shefali Aggarwal              Rel 2.0 changes
 * 27-Feb-2006  Amit Sharma                  Merged code for ICF
 * 13-Jul-2007  Amit Sharma                  Code updation for XCONF feature 
 *                                           in IPTK Rel 7.0
 * 13-Jul-2007  Abhishek Dhammawat Rel7.0 CDD Modified function
 *                                            icf_scm_process_mesg for XCONF
 *                                            feature.  
 * 07-Nov-2008  Rajiv Kumar     SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                            18911)
 * 07-Nov-2008  Rajiv Kumar     SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                            18909)
 *
 * Copyright 2007,Aricent.
 ******************************************************************************/
#include "icf_map_extif_feature_flags.h"

#ifdef ICF_SERVICE_ENABLE
#include "icf_scm_types.h"
#include "icf_scm_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_scm_macro.h"
#include "icf_internal_api_id.h"

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_init
 *
 * DESCRIPTION   : This is the initialization function of SCM module
 *                 which is called by DBM at the time of system startup
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t   icf_scm_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_scm_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_scm_data_st         *p_scm_data = ICF_NULL;
   
    /* Map of Module id -> (Message Handler + Service Module Bitmask)*/
    icf_sl_info_st scm_sl_info[ICF_SCM_MAX_SL]= 
    {
        {icf_chr_process_mesg,ICF_SERVICE_CHR},
        {icf_rnw_process_mesg,ICF_SERVICE_RNW},
        {icf_twc_process_mesg,ICF_SERVICE_TWC},
        {icf_cta_process_mesg,ICF_SERVICE_CTA},
        {icf_ctu_process_mesg,ICF_SERVICE_CTU},
        {icf_cfw_process_mesg, ICF_SERVICE_CFW},
        {icf_xconf_process_mesg, ICF_SERVICE_XCONF}
    };
    ICF_FUNCTION_ENTER(p_glb_pdb) 

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    /* Allocate and assign SCM Data */
    ICF_STATIC_MEMGET(p_glb_pdb,
            sizeof(icf_scm_data_st),
            p_scm_data, ICF_RET_ON_FAILURE, p_ecode, ret_val)

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_scm_data))

    /* SCM SL information */
    icf_port_memcpy(
            p_scm_data->scm_sl_info, 
            scm_sl_info, 
            (icf_uint32_t)sizeof(icf_scm_data_st));

    /*Assign global data pointer */
    *p_p_scm_data = (icf_void_t *)p_scm_data;
    ICF_FUNCTION_EXIT(p_glb_pdb) 

    return ret_val;

}

/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_deinit
 *
 * DESCRIPTION   : This is the cleanup function of SCM module
 *                 which is called by DBM at the time of system shutdown
 *
 * RETURNS       : icf_void_t
 *
 ******************************************************************************/
icf_void_t icf_scm_deinit(
        INOUT icf_glb_pdb_st *p_glb_pdb,
        INOUT icf_void_t     *p_data,
        OUT   icf_error_t    *p_ecode)
{
    icf_return_t ret_val = ICF_SUCCESS;
    ICF_FUNCTION_ENTER(p_glb_pdb)
    ICF_STATIC_MEMFREE(p_glb_pdb,
            p_data,  p_ecode, ret_val)
    ICF_FUNCTION_EXIT(p_glb_pdb) 
}


/*******************************************************************************
 *
 * FUNCTION NAME : icf_scm_process_mesg
 *
 * DESCRIPTION   : This is the entry point function of SCM. 
 *                 It forwards the message to the appropriate service 
 *                 logic module. 
 *                 If the service context and service logic context do not 
 *                 exist, it allocates them. 
 *                 If any service is already active, then it checks if the 
 *                 new service can be handled and returns appropriate response.
 *
 * RETURNS       : icf_return_t
 *
 ******************************************************************************/
icf_return_t   icf_scm_process_mesg(
        INOUT   icf_internal_msg_st   *p_msg
        )

{
    icf_error_t         *p_ecode = ICF_NULL;
    icf_scm_pdb_st      pdb;
    icf_result_t        ret_val = ICF_SUCCESS;
    icf_scm_data_st     *p_scm_data = ICF_NULL;
    icf_glb_pdb_st      *p_glb_pdb = ICF_NULL;
    icf_timer_data_st   *p_timer_data = ICF_NULL;

    /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18911)*/
    /* SPR 18911 : Added NULL checks */
    if((ICF_NULL == p_msg) ||
       (ICF_NULL == &(p_msg->msg_hdr).ecode) ||
       (ICF_NULL == p_msg->msg_hdr.p_glb_pdb))
    {
        /* Return as not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"\n[SCM] p_msg is NULL.\n"))
        return ICF_FAILURE;
    }

    p_ecode = &((p_msg->msg_hdr).ecode);
    p_glb_pdb = p_msg->msg_hdr.p_glb_pdb;    

    ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                             p_glb_pdb->p_glb_data,
                             ret_val)

    if(ICF_FAILURE == ret_val)    
    {
        /* Return as not enough data to proceed. */
        ICF_FUNCTION_EXIT(p_glb_pdb)
        ICF_RESET_MODULE(p_glb_pdb)
        return ret_val;
    }    

#ifdef ICF_UT_TEST
    /* p_ecode declared in icf_glb_pdb_st will point to 
       ecode in icf_internal_msg_st. To change this ecode, 
       value of pointer will be changed */
    p_msg->msg_hdr.p_glb_pdb->p_ecode = &(p_msg->msg_hdr.ecode);
#endif

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_SCM)
    ICF_FUNCTION_ENTER(p_glb_pdb)

    icf_port_memset(&pdb, '\0', sizeof(icf_scm_pdb_st));
    
    /* Assign Values in PDB */
    /*If timer expiry is received then p_glb_pdb->p_call_ctx will be NULL*/
    if (ICF_TIMER_EXPIRY_IND == p_msg->msg_hdr.msg_id)
    {
        /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18909)*/
        /* SPR 18909 */
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_glb_pdb->p_recv_msg,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            ICF_RESET_MODULE(p_glb_pdb)
            return ret_val;
        }          
        p_timer_data =
                *(icf_timer_data_st **)p_glb_pdb->p_recv_msg->payload;
        
        ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_glb_pdb,
                                            p_timer_data,
                                            p_timer_data->p_timer_buffer,
                                            ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            ICF_RESET_MODULE(p_glb_pdb)
            return ret_val;
        }        

        p_glb_pdb->p_call_ctx =
                    ((icf_call_ctx_st *)p_timer_data->p_timer_buffer);
    } /* if (ICF_TIMER_EXPIRY_IND ==... */
    
    pdb.p_glb_data = p_glb_pdb->p_glb_data;
    pdb.p_msg = p_msg;
    pdb.msg_id = p_msg->msg_hdr.msg_id;
    pdb.call_waiting_fwd = ICF_FALSE;

    /*If INITIATE_EXTERNAL_CONF_REQ is received then in this case 
     * p_glb_pdb->p_call_ctx will be NULL so this will not be populated in
     * pdb, in this case call context will be allocated by XCONF module
     * and it will populate it in p_glb_pdb*/
    if (ICF_USER_INITIATE_EXTERNAL_CONF_REQ != p_msg->msg_hdr.msg_id)
    {
        ICF_CHECK_IF_VAR_IS_NULL(p_glb_pdb,
                                 p_glb_pdb->p_call_ctx,
                                 ret_val)
        if(ICF_FAILURE == ret_val)
        {
            ICF_FUNCTION_EXIT(p_glb_pdb)
            ICF_RESET_MODULE(p_glb_pdb)
            return ret_val;
        }
        
        pdb.p_service_ctx = p_glb_pdb->p_call_ctx->p_service_ctx;

        /* If p_msg_data is not null and the msg id is sc int twc resp
         * then copy the second service ctx in the pdb and set the p msg data to 
         * null
         */

        if ((ICF_NULL != p_msg->p_msg_data) && 
                ((ICF_SC_INTERNAL_TWC_RESP == pdb.msg_id)
                 || (ICF_SC_INTERNAL_RNW_RESP == pdb.msg_id)))
        {
            pdb.p_second_call_ctx = 
                (icf_call_ctx_st *)(p_msg->p_msg_data);
            p_msg->p_msg_data = ICF_NULL;
        }
        else
        {
            pdb.p_second_call_ctx = ICF_NULL;
        }
    }

    /*Call DBM function to get SCM global data pointer*/
    ret_val = icf_dbm_get_module_glb_data(
                    p_msg->msg_hdr.p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_SCM),
                    (icf_void_t *)&p_scm_data, p_ecode);
    if(ICF_FAILURE == ret_val)
    {
        ICF_SCM_TRACE(&pdb, ICF_TRACE_GET_GLB_DATA_FAILED)
        ret_val = ICF_SUCCESS;
    }
    else
    {
        pdb.p_scm_data = p_scm_data;
        /* Route message to appropriate module */
        ICF_SCM_TRACE(&pdb, ICF_TRACE_API_RECVD)
        ret_val=icf_scm_route_message(&pdb,p_ecode);
    }

    ICF_FUNCTION_EXIT(p_glb_pdb)
    ICF_RESET_MODULE(p_glb_pdb)
    return ret_val;
}
#endif
