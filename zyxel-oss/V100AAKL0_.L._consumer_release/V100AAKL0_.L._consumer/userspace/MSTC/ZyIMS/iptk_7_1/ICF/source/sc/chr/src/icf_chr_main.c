/*******************************************************************************
*
* FILE NAME    : icf_chr_main.c
*
* DESCRIPTION  : Contains the definitions of CHR service entry point function
*
*
* Revision History :
*    DATE            NAME       REFERENCE       REASON
* --------    ------------      ---------      ---------------
* 13-Dec-2004    Amit Sharma     None            Initial
* 25-Jan-2005    Amit Sharma                    Changes to replace TIMER_MSG 
*                                               with TIMER_EXPIRY_IND
* 25-Feb-2005    Amit Sharma                    Added handling for forceful
*                                               Call Clear event
* 12-Apr-2005    Amit Sharma                    Updated handling of timer expiry
* 19-May-2005    Amit Sharma   SPR:7461         Handling for ecode generic error
* 19-May-2005    Amit Sharma   SPR:7568         Change in handling of ret_val 0
* 29-Sep-2005    Shefali Aggarwal               Coding for rel 2.0 
* 4-Oct-2005     Shefali Aggarwal               Changed the FSM table to remove
*                                               the function calls to the 
*                                               state 3.
*
* Copyright 2006,Aricent.
*******************************************************************************/
#include "icf_map_extif_feature_flags.h"
#ifdef ICF_SERVICE_ENABLE
#include "icf_chr_types.h"
#include "icf_chr_defs.h"
#include "icf_chr_macro.h"
#include "icf_chr_prototypes.h"
#include "icf_sc_common_types.h"
#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif 

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_init 
*
* DESCRIPTION  : This is the initialization function of CHR service module
*                which is called by DBM at the time of system startup
*
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_chr_init(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        OUT     icf_void_t            **p_p_chr_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_chr_data_st         *p_chr_data = ICF_NULL;
    icf_return_t            ret_val = ICF_SUCCESS;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*Allocate memory for FSM table*/
    ICF_STATIC_MEMGET(p_glb_pdb, 
             sizeof(icf_chr_data_st),
            p_chr_data, ICF_RET_ON_FAILURE, p_ecode, ret_val)

    if(ICF_FAILURE == ret_val)
    {
        return ret_val;
    } /* if(ICF_FAILURE == ret_val) */
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_chr_data))

    /*Assignment of function pointers of FSM*/
    /*ICF_CHR_00_IDLE*/
    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_CALL_HOLD] =
    icf_chr_00_call_hold; 

    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_MMS_RESP] =
    icf_chr_unexpected_event;      /*ICF_CHR_MMS_RESP*/
    
    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_MEDIA_ERROR] =
    icf_chr_unexpected_event;      /*ICF_CHR_MEDIA_ERROR*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_00_IDLE][ICF_CHR_NW_HLD_RSM_SUC_RESP] =
    icf_chr_unexpected_event;      /*ICF_CHR_NW_HLD_RSM_SUC_RESP*/

    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_CALL_RESUME] =
    icf_chr_00_call_resume;        /*ICF_CHR_CALL_RESUME*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_00_IDLE][ICF_CHR_HLD_RSM_REQ_FROM_NW] =
    icf_chr_unexpected_event;      /*ICF_CHR_HLD_RSM_REQ_FROM_NW*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_00_IDLE][ICF_CHR_NW_ACK_FOR_REQ_CNL] =
    icf_chr_unexpected_event;      /*ICF_CHR_NW_REQ_CANCEL_RESP*/
    
    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_NW_OG_SETUP_FAIL] =
    icf_chr_unexpected_event;      /*ICF_CHR_NW_OG_SETUP_FAIL*/
    
    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_TIMER_EXPIRY] =
    icf_chr_unexpected_event;      /*ICF_CHR_TIMER_EXPIRY*/
    
    p_chr_data->chr_fsm_table
        [ICF_CHR_00_IDLE][ICF_CHR_CALL_RELEASED] =    
    icf_chr_00_call_release;      /*ICF_CHR_CALL_RELEASED*/
    
    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_ST_PROC_SIG] =
    icf_chr_unexpected_event;      /*ICF_CHR_ST_PROC_SIG*/
    
    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE][ICF_CHR_ST_PROC_MEDIA] =
    icf_chr_unexpected_event;       /*ICF_CHR_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table[ICF_CHR_00_IDLE]
        [ICF_CHR_FORCEFUL_CALL_CLEAR] = icf_chr_xx_forceful_call_clear;

    /*State: ICF_CHR_01_AWT_MMS_RESP*/
    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_CALL_HOLD] = 
        icf_chr_xx_call_hold;          /*ICF_CHR_CALL_HOLD*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_MMS_RESP] = 
        icf_chr_01_mms_resp;           /*ICF_CHR_MMS_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_MEDIA_ERROR] = 
        icf_chr_01_media_error;        /*ICF_CHR_MEDIA_ERROR*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_NW_HLD_RSM_SUC_RESP]
        = icf_chr_unexpected_event;      /*ICF_CHR_NW_HLD_RSM_SUC_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_CALL_RESUME] = 
        icf_chr_xx_call_resume;        /*ICF_CHR_CALL_RESUME*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_HLD_RSM_REQ_FROM_NW] =
        icf_chr_01_hldrsm_req_frm_nw;  /*ICF_CHR_HLD_RSM_REQ_FROM_NW*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_NW_ACK_FOR_REQ_CNL] 
        = icf_chr_unexpected_event;      /*ICF_CHR_NW_REQ_CANCEL_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_NW_OG_SETUP_FAIL] = 
        icf_chr_unexpected_event;      /*ICF_CHR_NW_OG_SETUP_FAIL*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_TIMER_EXPIRY] = 
        icf_chr_xx_timer_expiry;       /*ICF_CHR_TIMER_EXPIRY*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_CALL_RELEASED] = 
        icf_chr_xx_call_release;       /*ICF_CHR_CALL_RELEASED*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_ST_PROC_SIG] = 
        icf_chr_unexpected_event;      /*ICF_CHR_ST_PROC_SIG*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_01_AWT_MMS_RESP][ICF_CHR_ST_PROC_MEDIA] = 
        icf_chr_unexpected_event;       /*ICF_CHR_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table[ICF_CHR_01_AWT_MMS_RESP]
        [ICF_CHR_FORCEFUL_CALL_CLEAR] = icf_chr_xx_forceful_call_clear;

    /*State: ICF_CHR_02_AWT_NW_ACK*/
    p_chr_data->chr_fsm_table[ICF_CHR_02_AWT_NW_ACK][ICF_CHR_CALL_HOLD] = 
        icf_chr_xx_call_hold;          /*ICF_CHR_CALL_HOLD*/

    p_chr_data->chr_fsm_table[ICF_CHR_02_AWT_NW_ACK][ICF_CHR_MMS_RESP] = 
        icf_chr_unexpected_event;      /*ICF_CHR_MMS_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_MEDIA_ERROR] = 
        icf_chr_02_media_error;        /*ICF_CHR_MEDIA_ERROR*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_NW_HLD_RSM_SUC_RESP] = 
        icf_chr_02_nw_hldrsm_suc_resp; /*ICF_CHR_NW_HLD_RSM_SUC_RESP*/

    p_chr_data->chr_fsm_table[ICF_CHR_02_AWT_NW_ACK][ICF_CHR_CALL_RESUME]
        = icf_chr_xx_call_resume;        /*ICF_CHR_CALL_RESUME*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_HLD_RSM_REQ_FROM_NW] = 
        icf_chr_xx_hldrsm_req_frm_nw;  /*ICF_CHR_HLD_RSM_REQ_FROM_NW*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_NW_ACK_FOR_REQ_CNL] = 
        icf_chr_unexpected_event;      /*ICF_CHR_NW_REQ_CANCEL_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_NW_OG_SETUP_FAIL] = 
        icf_chr_02_nw_og_setup_fail;   /*ICF_CHR_NW_OG_SETUP_FAIL*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_TIMER_EXPIRY] = 
        icf_chr_02_timer_expiry;       /*ICF_CHR_TIMER_EXPIRY*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_CALL_RELEASED] =    
        icf_chr_02_call_release;       /*ICF_CHR_CALL_RELEASED*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_ST_PROC_SIG] = 
        icf_chr_unexpected_event;      /*ICF_CHR_ST_PROC_SIG*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_02_AWT_NW_ACK][ICF_CHR_ST_PROC_MEDIA] = 
        icf_chr_unexpected_event;       /*ICF_CHR_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table[ICF_CHR_02_AWT_NW_ACK]
        [ICF_CHR_FORCEFUL_CALL_CLEAR] = icf_chr_xx_forceful_call_clear;

    /*State: ICF_CHR_03_AWT_CANCEL_RESP*/
    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_CALL_HOLD] =
        icf_chr_unexpected_event;      /*ICF_CHR_CALL_HOLD*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_MMS_RESP] =
        icf_chr_unexpected_event;      /*ICF_CHR_MMS_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_MEDIA_ERROR] =
        icf_chr_unexpected_event;      /*ICF_CHR_MEDIA_ERROR*/

    p_chr_data->chr_fsm_table[ICF_CHR_03_AWT_CANCEL_RESP]
        [ICF_CHR_NW_HLD_RSM_SUC_RESP] =
        icf_chr_unexpected_event;      /*ICF_CHR_NW_HLD_RSM_SUC_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_CALL_RESUME] =
        icf_chr_unexpected_event;      /*ICF_CHR_CALL_RESUME*/

    p_chr_data->chr_fsm_table[ICF_CHR_03_AWT_CANCEL_RESP]
        [ICF_CHR_HLD_RSM_REQ_FROM_NW] =
        icf_chr_xx_hldrsm_req_frm_nw;  /*ICF_CHR_HLD_RSM_REQ_FROM_NW*/

    p_chr_data->chr_fsm_table[ICF_CHR_03_AWT_CANCEL_RESP]
        [ICF_CHR_NW_ACK_FOR_REQ_CNL] =
        icf_chr_unexpected_event;    /*ICF_CHR_NW_REQ_CANCEL_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_NW_OG_SETUP_FAIL]
        = icf_chr_unexpected_event;      /*ICF_CHR_NW_OG_SETUP_FAIL*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_TIMER_EXPIRY] =
        icf_chr_xx_timer_expiry;       /*ICF_CHR_TIMER_EXPIRY*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_CALL_RELEASED] =
        icf_chr_xx_call_release;       /*ICF_CHR_CALL_RELEASED*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_ST_PROC_SIG] =
        icf_chr_unexpected_event;      /*ICF_CHR_ST_PROC_SIG*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_03_AWT_CANCEL_RESP][ICF_CHR_ST_PROC_MEDIA] =
        icf_chr_unexpected_event;       /*ICF_CHR_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table[ICF_CHR_03_AWT_CANCEL_RESP]
        [ICF_CHR_FORCEFUL_CALL_CLEAR] = icf_chr_xx_forceful_call_clear;

    /*State: ICF_CHR_04_AWT_ST_PROC_SIG*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_CALL_HOLD] = 
        icf_chr_xx_call_hold;          /*ICF_CHR_CALL_HOLD*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_MMS_RESP] = 
        icf_chr_unexpected_event;      /*ICF_CHR_MMS_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_MEDIA_ERROR] = 
        icf_chr_unexpected_event;      /*ICF_CHR_MEDIA_ERROR*/

    p_chr_data->chr_fsm_table[ICF_CHR_04_AWT_ST_PROC_SIG]
        [ICF_CHR_NW_HLD_RSM_SUC_RESP] = 
        icf_chr_unexpected_event;      /*ICF_CHR_NW_HLD_RSM_SUC_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_CALL_RESUME] = 
        icf_chr_xx_call_resume;        /*ICF_CHR_CALL_RESUME*/

    p_chr_data->chr_fsm_table[ICF_CHR_04_AWT_ST_PROC_SIG]
        [ICF_CHR_HLD_RSM_REQ_FROM_NW] = 
        icf_chr_unexpected_event;      /*ICF_CHR_HLD_RSM_REQ_FROM_NW*/

    p_chr_data->chr_fsm_table[ICF_CHR_04_AWT_ST_PROC_SIG]
        [ICF_CHR_NW_ACK_FOR_REQ_CNL] = 
        icf_chr_unexpected_event;      /*ICF_CHR_NW_REQ_CANCEL_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_NW_OG_SETUP_FAIL]
        = icf_chr_unexpected_event;      /*ICF_CHR_NW_OG_SETUP_FAIL*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_TIMER_EXPIRY] = 
        icf_chr_xx_timer_expiry;       /*ICF_CHR_TIMER_EXPIRY*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_CALL_RELEASED] = 
        icf_chr_xx_call_release;       /*ICF_CHR_CALL_RELEASED*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_ST_PROC_SIG] = 
        icf_chr_04_st_proc_sig;        /*ICF_CHR_ST_PROC_SIG*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_04_AWT_ST_PROC_SIG][ICF_CHR_ST_PROC_MEDIA] = 
        icf_chr_unexpected_event;       /*ICF_CHR_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table[ICF_CHR_04_AWT_ST_PROC_SIG]
        [ICF_CHR_FORCEFUL_CALL_CLEAR] = icf_chr_xx_forceful_call_clear;

    /*State: ICF_CHR_05_AWT_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_CALL_HOLD] =
        icf_chr_xx_call_hold;          /*ICF_CHR_CALL_HOLD*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_MMS_RESP] =
        icf_chr_unexpected_event;      /*ICF_CHR_MMS_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_MEDIA_ERROR] =
        icf_chr_unexpected_event;      /*ICF_CHR_MEDIA_ERROR*/

    p_chr_data->chr_fsm_table[ICF_CHR_05_AWT_ST_PROC_MEDIA]
        [ICF_CHR_NW_HLD_RSM_SUC_RESP] =
        icf_chr_unexpected_event;      /*ICF_CHR_NW_HLD_RSM_SUC_RESP*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_CALL_RESUME] =
        icf_chr_xx_call_resume;        /*ICF_CHR_CALL_RESUME*/

    p_chr_data->chr_fsm_table[ICF_CHR_05_AWT_ST_PROC_MEDIA]
        [ICF_CHR_HLD_RSM_REQ_FROM_NW] =
        icf_chr_unexpected_event;      /*ICF_CHR_HLD_RSM_REQ_FROM_NW*/

    p_chr_data->chr_fsm_table[ICF_CHR_05_AWT_ST_PROC_MEDIA]
        [ICF_CHR_NW_ACK_FOR_REQ_CNL] =
        icf_chr_unexpected_event;      /*ICF_CHR_NW_REQ_CANCEL_RESP*/

    p_chr_data->chr_fsm_table[ICF_CHR_05_AWT_ST_PROC_MEDIA]
        [ICF_CHR_NW_OG_SETUP_FAIL] =
        icf_chr_unexpected_event;      /*ICF_CHR_NW_OG_SETUP_FAIL*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_TIMER_EXPIRY] =
        icf_chr_xx_timer_expiry;       /*ICF_CHR_TIMER_EXPIRY*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_CALL_RELEASED] =
        icf_chr_xx_call_release;       /*ICF_CHR_CALL_RELEASED*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_ST_PROC_SIG] =
        icf_chr_unexpected_event;      /*ICF_CHR_ST_PROC_SIG*/

    p_chr_data->chr_fsm_table
        [ICF_CHR_05_AWT_ST_PROC_MEDIA][ICF_CHR_ST_PROC_MEDIA] =
        icf_chr_05_st_proc_media;       /*ICF_CHR_ST_PROC_MEDIA*/

    p_chr_data->chr_fsm_table[ICF_CHR_05_AWT_ST_PROC_MEDIA]
        [ICF_CHR_FORCEFUL_CALL_CLEAR] = icf_chr_xx_forceful_call_clear;

    /*********End of Function pointer assignment*************/
    
   
    /*Assign fsm table pointer to global data pointer*/
    *p_p_chr_data = (icf_void_t *)p_chr_data;

    ICF_FUNCTION_EXIT(p_glb_pdb)

    return ret_val;
} /* icf_chr_init */

            
            

/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_process_mesg
*
* DESCRIPTION  : This is the Entry Point function of CHR service module
*
*
* RETURNS:
*
*******************************************************************************/
icf_return_t   icf_chr_process_mesg(
        INOUT   icf_internal_msg_st   *p_internal_msg
        )
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_chr_pdb_st          chr_pdb = {0,0,0,0,0,0,0,0
				#ifdef ICF_TRACE_ENABLE
					,0,0
				#endif
					};
	icf_chr_pdb_st			*p_pdb = ICF_NULL;
	icf_glb_pdb_st          *p_glb_pdb = ICF_NULL;
    icf_error_t             *p_ecode = ICF_NULL;
    icf_chr_data_st         *p_chr_data = ICF_NULL;
    icf_uint8_t             event=0;
    /*icf_chr_fsm_func_ptr_t  **p_chr_fsm_table;*/
    p_pdb = &chr_pdb;

    /*
     * Copy internal msg pointer to p_pdb
     */
    p_pdb->p_internal_msg = p_internal_msg;

    /*
     * Copy p_glb_pdb
     */
    p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_glb_pdb))

    ICF_SET_MODULE(p_glb_pdb, ICF_INT_MODULE_CHR)

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    /*
     * Copy ecode pointer
     */
    p_ecode = &(p_internal_msg->msg_hdr.ecode);
#if 0
    if (ICF_TIMER_EXPIRY_IND == p_internal_msg->msg_hdr.msg_id)
    {
        ret_val = icf_chr_convert_api_to_event(p_pdb, &event);

        if ((ICF_SUCCESS == ret_val)
                && (ICF_EVENT_UNDEFINED != event))
        {
            ret_val = icf_chr_init_pdb_data(p_pdb, p_internal_msg);
        } /* if (ICF_SUCCESS == ret_val) */
    } /* if (ICF_TIMER_EXPIRY_IND ==... */
    else
    {
#endif
        ret_val = icf_chr_init_pdb_data(p_pdb, p_internal_msg);

        if (ICF_SUCCESS == ret_val)
        {
            ret_val = icf_chr_convert_api_to_event(p_pdb, &event);
        }
#if 0
    } /* else */    
#endif
    
    /*
     * Convert the incoming API to the Event
     */
    if((ICF_EVENT_UNDEFINED != event)
            && (ICF_FAILURE != ret_val))
    {
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)

        /*Assign converted event to CHR context*/
        p_pdb->p_chr_ctx->event = event;

        /* Check if immidiate response for the internal call hold or resume is
         * to be send then set the status and there is no need to call the 
         * FSM
         */
        if (ICF_TRUE == p_pdb->imm_internal_req_resp)
        {
            if (ICF_CHR_CALL_HOLD == p_pdb->p_chr_ctx->event)
            {
                ICF_CHR_SET_STATUS(p_pdb,
                        ICF_STATUS_SERVICE_CALL_HOLD_RESP)      
            }
            else
            {
                ICF_CHR_SET_STATUS(p_pdb,
                        ICF_STATUS_SERVICE_CALL_RESUME_RESP)         
            }
        }/* if (ICF_TRUE == */

        else
        {
            /*Call DBM function to get CHR global data pointer*/
            ret_val = 
                icf_dbm_get_module_glb_data(
                    p_internal_msg->msg_hdr.p_glb_pdb,
                    (icf_module_id_t)(ICF_INT_MODULE_CHR),
                    (icf_void_t *)&p_chr_data, p_ecode);

            if(ICF_FAILURE != ret_val)
            {
                ICF_CHR_TRACE(p_pdb, ICF_TRACE_CHR_STATE)
                
                ICF_CHR_TRACE(p_pdb, ICF_TRACE_CHR_EVENT)
    
                /* Call appropriate State machine function based on
                 * State and event */
                ret_val = p_chr_data->chr_fsm_table[p_pdb->p_chr_ctx->state]
                    [p_pdb->p_chr_ctx->event](p_pdb);
            } /* if(ICF_FAILURE != ret_val) */    
        }/* else */
    } /* if(ICF_FAILURE != ret_val) */
    
    /*
     * Check the return value of FSM function
     */
    if (ICF_EVENT_UNDEFINED == event)
    {
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
    }
    else if(ICF_FAILURE == ret_val)
    {
        /*
         * Set status as service ended error Reset service continue
         * status
         */
        ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE) 
        
        ICF_CHR_SET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR)

        ICF_CHR_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)

        /*icf_chr_deinit_ctx(p_pdb);*/

    }
    else if(ICF_CHR_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_ERROR))
    {
        /*
         * Reset service continue and normal status if already set
         */
        ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL)

        ICF_CHR_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_ERROR)
        
        icf_chr_deinit_ctx(p_pdb);
    } /* else if(ICF_STATUS_SERVICE_... */    
    else if(ICF_CHR_CHECK_STATUS(p_pdb, ICF_STATUS_SERVICE_ENDED_NORMAL))
    { 
        /*
         * If normal then reset service continue 
         */
        ICF_CHR_RESET_STATUS(p_pdb, ICF_STATUS_SERVICE_CONTINUE)
        
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_SERVICE_TERM_NORMAL)
        
        icf_chr_deinit_ctx(p_pdb);        
    } /* else if(ICF_STATUS_SERVICE_... */    
    else
    {
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CURR_RET_STATUS)
        
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CHR_STATE)
        
        ICF_CHR_TRACE(p_pdb, ICF_TRACE_CHR_EVENT)
    } /* else */    

    ICF_FUNCTION_EXIT(p_glb_pdb)

    ICF_RESET_MODULE(p_glb_pdb)
    
    return ret_val;
} /* icf_chr_process_msg */


/*******************************************************************************
*
* FUNCTION NAME    : icf_chr_convert_api_to_event
* 
* DESCRIPTION  : This function converts incoming APIs to internal EVENTs
*
* RETURNS:
*******************************************************************************/
icf_return_t  icf_chr_convert_api_to_event(
        INOUT  icf_chr_pdb_st   *p_pdb,
        INOUT  icf_uint8_t      *p_event
        )
{
    icf_return_t       ret_val = ICF_SUCCESS;
    icf_glb_pdb_st     *p_glb_pdb = ICF_NULL;

    /*Copy p_glb_pdb*/
    p_glb_pdb = p_pdb->p_internal_msg->msg_hdr.p_glb_pdb;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
    ICF_ASSERT(p_glb_pdb, (ICF_NULL != p_event))
    
    switch(p_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        case ICF_SC_INTERNAL_CALL_HOLD_REQ:
            if (ICF_TRUE == icf_chr_internal_req_handler(p_pdb, 
                        (icf_uint32_t)(ICF_SC_INTERNAL_CALL_HOLD_REQ)))
            {
                /* Added to send traces */

                ICF_CHR_APP_TRACE(p_pdb, ICF_TRACE_CALL_HOLD_REQ) 
                *p_event = ICF_CHR_CALL_HOLD;
            } /* ICF_SC_INTERNAL_CALL_RESUME... */
            else
            {
                *p_event = ICF_EVENT_UNDEFINED;
            }
            
            break;

        case ICF_SC_INTERNAL_CALL_RESUME_REQ:
            if (ICF_TRUE == icf_chr_internal_req_handler(p_pdb, 
                        (icf_uint32_t)(ICF_SC_INTERNAL_CALL_RESUME_REQ)))
            {
                ICF_CHR_APP_TRACE(p_pdb, ICF_TRACE_CALL_RESUME_REQ)
                *p_event = ICF_CHR_CALL_RESUME;
            } /* ICF_SC_INTERNAL_CALL_RESUME... */
            else
            {
                *p_event = ICF_EVENT_UNDEFINED;
            }

            break;
            
        case ICF_MODIFY_MEDIA_RESP: 
            ret_val = icf_chr_check_mms_resp(p_pdb, p_event);
            break;

        case ICF_MEDIA_ERROR_IND:
            p_pdb->mm_err_recvd = ICF_TRUE;
            *p_event = ICF_CHR_MEDIA_ERROR;
            break;

        case ICF_NW_INC_MEDIA_UPDATE_RESP:
            /*Check for the response received, failure and success mapped
             * on different events*/
            if (ICF_TRUE == 
                    ((icf_nw_inc_media_change_resp_st *)
                    (p_pdb->p_internal_msg->p_msg_data))->status)
            {
                *p_event = ICF_CHR_NW_HLD_RSM_SUC_RESP;
            } /* (p_pdb->p_internal_msg->payloa... */
            else
            {
                *p_event = ICF_CHR_NW_OG_SETUP_FAIL;
            }
            break;

        case ICF_NW_INC_MEDIA_UPDATE:
            *p_event = ICF_CHR_HLD_RSM_REQ_FROM_NW;
            break;

        /*To DO: Remove it*/
        case ICF_NW_INC_CALL_TERMINATE_RESP:
            *p_event = ICF_CHR_NW_ACK_FOR_REQ_CNL;
            break;

        /*TO DO: remove*/
        case ICF_NW_INC_CALL_REJECT:
            *p_event = ICF_CHR_NW_OG_SETUP_FAIL;
            break;

        case ICF_TIMER_EXPIRY_IND:
             /* Added to send traces */

                ICF_CHR_APP_TRACE(p_pdb, ICF_TRACE_CALL_HOLD_RESUME_TIMER_EXPIRE)

                *p_event = ICF_CHR_TIMER_EXPIRY;
            break;

        case ICF_USER_CALL_HOLD:
            p_pdb->p_chr_ctx->internal_hold_resp = ICF_FALSE;
            ICF_CHR_APP_TRACE(p_pdb, ICF_TRACE_CALL_HOLD_REQ)
            *p_event = ICF_CHR_CALL_HOLD;
            break;

        case ICF_USER_CALL_RESUME:
            p_pdb->p_chr_ctx->internal_resume_resp = ICF_FALSE;
            *p_event = ICF_CHR_CALL_RESUME;
            break;
        
        case ICF_CC_CALL_RELEASED:
            *p_event = ICF_CHR_CALL_RELEASED;
            break;

        case ICF_START_PROC_SIGNALING:
            *p_event = ICF_CHR_ST_PROC_SIG;
            break;

        case ICF_START_PROC_MEDIA:
            *p_event = ICF_CHR_ST_PROC_MEDIA;
            break;

        case ICF_FORCEFUL_CALL_CLEAR_IND:
             *p_event = ICF_CHR_FORCEFUL_CALL_CLEAR;
            break;

        /* If API id is int TWC resp or int RNW resp or int CHR resp then 
         * check the pending event. If the pending event is call hold then 
         * the event is user call hold, if the pending event is call resume
         * then the event is call resume. If there is no pending event then
         * set event undefined. Alse reset the pending event flag
         */ 
            
        case ICF_SC_INTERNAL_TWC_RESP:
        case ICF_SC_INTERNAL_RNW_RESP:
        case ICF_SC_INTERNAL_CHR_RESP:
            if (ICF_CHR_PE_CALL_HOLD == p_pdb->p_chr_ctx->pending_event)
            {
                p_pdb->p_chr_ctx->internal_hold_resp = ICF_FALSE;
                
                ICF_CHR_RESET_PEND_EVT(p_pdb)
                    
                *p_event = ICF_CHR_CALL_HOLD;
            }
            else if (ICF_CHR_PE_CALL_RESUME == p_pdb->p_chr_ctx->
                    pending_event)
            {
                p_pdb->p_chr_ctx->internal_resume_resp = ICF_FALSE;
                
                ICF_CHR_RESET_PEND_EVT(p_pdb)

                *p_event = ICF_CHR_CALL_RESUME;
            }
            else
            {
                *p_event = ICF_EVENT_UNDEFINED;
            }
            break;
            
        default:
            *p_event = ICF_EVENT_UNDEFINED;
            break;
    } /* switch(p_pdb->p_internal_msg->... */

    ICF_FUNCTION_EXIT(p_glb_pdb)
    
    return ret_val;
} /* icf_chr_convert_api_to_event */


/*******************************************************************************
*
* FUNCTION NAME : icf_chr_deinit
*
* DESCRIPTION   : This is the deinit function of CHR service module
*                 which is called by DBM at the time of system stop
*
* RETURNS       : icf_void_t
*
*******************************************************************************/
icf_void_t   icf_chr_deinit(
        INOUT   icf_glb_pdb_st        *p_glb_pdb,
        INOUT   icf_void_t            *p_data,
        OUT     icf_error_t           *p_ecode
        )
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_glb_pdb)
    
     ICF_STATIC_MEMFREE(
            p_glb_pdb,
            p_data,
            p_ecode,
            ret_val)

     ICF_FUNCTION_EXIT(p_glb_pdb)
} /* icf_chr_deinit */
#endif
