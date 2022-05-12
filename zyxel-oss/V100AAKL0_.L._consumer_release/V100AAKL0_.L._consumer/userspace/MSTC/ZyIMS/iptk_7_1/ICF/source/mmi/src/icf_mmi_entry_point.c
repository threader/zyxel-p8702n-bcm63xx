/******************************************************************************
 * 
 * FILE NAME:   icf_mmi_entry_point.c
 * 
 * DESCRIPTION: This file contains the entry point function and the utitlity 
 *              function called by the entry point function to call the API 
 *              handlers.
 *              
 * REVISION HISTORY:    
 * Date            Name            Reference       Comments
 * 20-Dec-2004    Shefali Aggarwal       LLD         Initial Version
 *          
 * 28-Mar-2005    Shefali Aggarwal                   Updations
 * 28-Mar-2005    Shefali Aggarwal    SPR 7488       mmi was not initializ
 *                                                            ing context 
 * 04-Jul-2005    Jalaj Negi          IMS CLIENT     SDP block was
 *                                                   allocated from 
 *                                                   common pool instead
 *                                                            of SDP pool
 * 05-Jul-2005    Sumit Gupta                        Allocation of SDP 
 *                                                   from DBM rather than
 *                                                   through memget
 * 29-Nov-2005    Kirti Dhingra                      QOS support for ICF
 * 09-Mar-2006    Amit Sharma                        ICF merging with ICF2.0
 * 04-Jul-2006	   Mohan Dhawan			ICF			  QoS Support for ICF 6.0
 * 26-Jul-2006    Aman/Puneet		ICF Rel 6.0		modifications for multiple 
 * 													m line
 * 6-Dec-2006      Deepti Goyal     ICF Rel 6.2     NAt Feature Support.
 *
 * Copyright (c) 2006, Aricent.  
 *
 ******************************************************************************/     
#include "icf_internal_api_id.h"
#include "icf_mmi_macro.h"
#include "icf_mmi_defs.h"
#include "icf_mmi_types.h"
#include "icf_mmi_prototypes.h"
#include "icf_dbm_prototypes.h"
ICF_MM_ROUTING_START
#include "icf_defs.h"
ICF_MM_ROUTING_END
#ifdef ICF_NAT_MEDIA_TRAVERSAL
#include "icf_cfg_defs.h"
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_mmi_init
 *
 * DESCRIPTION:     This is the initialiozation function of MMI
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/
icf_return_t icf_mmi_init(
        INOUT   icf_glb_pdb_st       *p_glb_pdb,
        OUT     icf_void_t           **p_p_glb_data,
        OUT     icf_error_t          *p_ecode)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_mmi_data_st                  *p_mmi_data = ICF_NULL;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    nat_stun_init_req_st            *p_msg_payload = ICF_NULL;
    icf_msg_st                      *p_msg = ICF_NULL;
    icf_uint8_t                     api_len= ICF_NULL;
#endif
    
    /* Allocate memory to table */
    ICF_STATIC_MEMGET(p_glb_pdb,sizeof(icf_mmi_data_st),
            p_mmi_data, ICF_RET_ON_FAILURE,p_ecode, ret_val)
    
    /* Initilize the table */
    /* Assign the function names in the FSM table for the events recieved when 
     * the system is in IDLE state
     */  
  
    /* CMS req handler will be called */      
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_CMS_REQ] = 
        icf_mmi_00_cms_request;
    
    /* MMS req handler will be called */
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_MMS_REQ] = 
        icf_mmi_00_mms_request;
    
    /* DMS req handler will be called */
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_DMS_REQ] = 
        icf_mmi_00_dms_request;
    
    /* Merge req handler will be called */
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_MERGE_REQ] = 
        icf_mmi_00_merge_request;
    
    /* Call clear indication in IDLE state */
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_CALL_CLEAR] = 
        icf_mmi_call_clear;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_NM_QOS_RELEASE_IND] = 
        icf_mmi_xx_nm_deact_ind;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_NM_QOS_MODIFY_IND] = 
        icf_mmi_xx_nm_modify_ind;
#endif


    
    /* Following events are unexpected in IDLE state so a common function will 
     * be called which will generate ecode that the event is unexpected.
     */
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_GCL_RESP] = 
        icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_COMMIT_RESP] =
        icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_CMS_RESP] = 
        icf_mmi_unexpected_event;    
            
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_MMS_RESP] = 
        icf_mmi_unexpected_event;         
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_DMS_RESP] = 
        icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_OPEN_CHANNEL_RESP] = 
        icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE]
			[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_NM_QOS_RESERVE_RESP] = 
        icf_mmi_unexpected_event;
#endif
	

        
    /* DMS IND handler is called */
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_DMS_IND] = 
        icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_00_IDLE][ICF_MMI_MERGE_RESP] =
        icf_mmi_unexpected_event;
    
    /* The function to handle the events in ICF_MMI_01_AWT_GCL_RESP
     * state are assigned.
     */
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_01_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_01_gcl_resp;

    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
	p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;

    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =   icf_mmi_unexpected_event;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_unexpected_event;
#endif
    
    /* Call clear indication in IDLE state */
    p_mmi_data->mmi_fsm_table[ICF_MMI_01_AWT_GCL_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;
            
    /* Functions to handle events in state ICF_MMI_02_AWT_CMS_RESP
     * are assigned.
     */
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_02_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_02_cms_resp;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_xx_nm_deact_ind;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_02_AWT_CMS_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_xx_nm_modify_ind;;
#endif

    
    /* Functions to handle events in state ICF_MMI_03_AWT_COMMIT_RESP
     * are assigned.
     */
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_03_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_03_commit_resp;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_unexpected_event;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_03_AWT_COMMIT_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_unexpected_event;
#endif

    
    /* Functions to handle events in State ICF_MMI_04_AWT_MMS_RESP */
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_04_dms_request;

    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_04_mms_resp;

    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;
#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_04_nm_deact_ind;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_04_AWT_MMS_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_04_nm_modify_ind;
#endif

    
    /* Functions to handle events in state ICF_MMI_05_AWT_DMS_RESP */
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_05_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_05_dms_resp;

    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;
    
	p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_unexpected_event;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_05_AWT_DMS_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_unexpected_event;	
#endif


    
    /* Functions to handle events in state ICF_MMI_06_AWT_OPEN_CHANNEL_RESP */
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_06_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;
    
	p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_06_open_channel_resp;

    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_xx_nm_deact_ind;;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_06_AWT_OPEN_CHANNEL_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_xx_nm_modify_ind;;
#endif

    
    /* Functions to handle events in State ICF_MMI_07_AWT_MERGE_RESP */
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_07_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_07_merge_resp;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;

#ifdef ICF_QOS_SUPPORT
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_unexpected_event;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_xx_nm_deact_ind;;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_07_AWT_MERGE_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_xx_nm_modify_ind;;
#endif


#ifdef ICF_QOS_SUPPORT

	/* Functions to handle events in State ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP */

	p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_08_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;

    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
		[ICF_MMI_NM_QOS_RESERVE_RESP] = icf_mmi_08_nm_qos_reserve_resp;
	
    p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
		[ICF_MMI_NM_QOS_RELEASE_IND] =  icf_mmi_xx_nm_deact_ind;
	
	p_mmi_data->mmi_fsm_table[ICF_MMI_08_AWT_NM_QOS_RESERVE_RESP]
		[ICF_MMI_NM_QOS_MODIFY_IND] = icf_mmi_xx_nm_modify_ind;

#endif

#ifdef ICF_NAT_MEDIA_TRAVERSAL
	    /* Functions to handle events in State 
		 * ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP */

    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_CMS_REQ] = icf_mmi_unexpected_event;
        
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_MMS_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_DMS_REQ] = icf_mmi_10_dms_request;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_MERGE_REQ] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_GCL_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_COMMIT_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_CMS_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_MMS_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_DMS_RESP] = icf_mmi_unexpected_event;

	p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
		[ICF_MMI_OPEN_CHANNEL_RESP] = icf_mmi_unexpected_event;

    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
		[ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP] = 
		icf_mmi_10_nat_public_ip_port_resp;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_DMS_IND] = icf_mmi_any_state_dms_ind;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_MERGE_RESP] = icf_mmi_unexpected_event;
    
    p_mmi_data->mmi_fsm_table[ICF_MMI_10_AWT_NAT_PUBLIC_IP_PORT_RESP]
        [ICF_MMI_CALL_CLEAR] = icf_mmi_call_clear;
#endif
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    /* Send the STUN Init request to NAT Client Library.
     */
    api_len = ICF_PORT_SIZEOF(icf_api_header_st) + 
        ICF_PORT_SIZEOF(nat_stun_init_req_st);
    
    ICF_MEMGET(p_glb_pdb,api_len,ICF_MEM_COMMON,p_msg,
            ICF_DONOT_RET_ON_FAILURE,p_ecode,ret_val)

    if (ICF_FAILURE == ret_val || ICF_NULL == p_msg)
    {
       ICF_MEMFREE(p_glb_pdb,p_mmi_data,ICF_MEM_COMMON,p_ecode,ret_val)
       return ICF_FAILURE;      
    } 

        
    /* Fill the header of the message to be send */
    p_msg->hdr.api_id = NAT_STUN_INIT_REQ;
    p_msg->hdr.source_id = ICF_MODULE_ICF;
    p_msg->hdr.destination_id = ICF_MODULE_NAT;
    p_msg->hdr.api_length = api_len;
    p_msg->hdr.call_id = ICF_INVALID_CALL_ID;
	p_msg->hdr.version = ICF_VERSION_1_0;
	p_msg->hdr.app_id = ICF_NULL;
    
    /* Fill the payload */
    p_msg_payload = (nat_stun_init_req_st*)(p_msg->payload);

	/* fill the payload */
    p_msg_payload->start_procedures = ICF_NULL;
    p_msg_payload->max_contexts = ICF_CFG_NAT_STUN_MAX_CONTEXTS;
    ICF_SEND(p_glb_pdb,p_msg,ret_val,p_ecode);
    /* Free memory */
    ICF_MEMFREE(p_glb_pdb,p_msg,ICF_MEM_COMMON,p_ecode,ret_val)


#endif

#ifndef FSS_EMUZED_MEDIA_MANAGER
	p_mmi_data->init_complete = ICF_TRUE;
    p_mmi_data->mm_resp_awaited = ICF_FALSE;
    p_mmi_data->p_mm_msg_buf = ICF_NULL;
#else
    p_mmi_data->init_complete = ICF_FALSE;
#endif

#ifdef FSS_EMUZED_MEDIA_MANAGER
   	icf_mmi_init_mm_cfg_params(p_mmi_data, p_glb_pdb);
#endif

    /* The FSM table pointer needs to be assigned to Global pointer.
     * This will be present in p_glb_data.
     */
    
    *p_p_glb_data = (icf_void_t *)p_mmi_data;
    
    return ret_val;    

}

/*******************************************************************************
 *
 * FUNCTION:        icf_mmi_init_mmi_ctx
 *
 * DESCRIPTION:     This function initalize the mmi context parameters
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

icf_return_t icf_mmi_init_mmi_ctx(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
    p_mmi_pdb->p_mmi_ctx->mmi_bitmask = ICF_NULL;
    p_mmi_pdb->p_mmi_ctx->merge_call_id = ICF_NULL;
    p_mmi_pdb->p_mmi_ctx->mmi_state = ICF_MMI_00_IDLE;
	p_mmi_pdb->p_mmi_ctx->p_media_session_req = ICF_NULL;
	p_mmi_pdb->p_mmi_ctx->stream_count = ICF_NULL;
	p_mmi_pdb->p_mmi_ctx->p_mmi_resp = ICF_NULL;
	p_mmi_pdb->p_mmi_ctx->p_mms_req = ICF_NULL;
	p_mmi_pdb->p_mmi_ctx->ongoing_event = ICF_NULL;
#ifdef ICF_QOS_SUPPORT
	p_mmi_pdb->p_mmi_ctx->pdp_count = ICF_NULL;
#endif

	p_mmi_pdb->stream_id = ICF_NULL;
	p_mmi_pdb->stream_type = ICF_NULL;

	if(ICF_NULL != p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp)
	{
		ret_val = icf_cmn_free_media_sdp_info_internal_list(p_mmi_pdb->p_glb_pdb,
			&(p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp),p_mmi_pdb->p_ecode);
	}
    p_mmi_pdb->p_mmi_ctx->p_remote_recvd_sdp = ICF_NULL;

	p_mmi_pdb->p_mmi_ctx->p_local_stream_addr = ICF_NULL;

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_mmi_api_to_event
 *
 * DESCRIPTION:     This function maps the messages recieved from CC to events.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

icf_return_t icf_mmi_api_to_event(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
       
    switch(p_mmi_pdb->p_internal_msg->msg_hdr.msg_id)
    {
        /* CMS request is recvd from CC */
        case ICF_CREATE_MEDIA_REQ:
            icf_mmi_init_mmi_ctx(p_mmi_pdb);
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_CMS_REQ)
                break;
                
        /* MMS request is recvd from CC */
        case ICF_MODIFY_MEDIA_REQ:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_MMS_REQ)
                break;

        /* DMS request is recvd from CC */
        case ICF_DELETE_MEDIA_REQ:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_DMS_REQ)
                break;

        /* Merge request is recvd from CC */
        case ICF_CREATE_CONF_REQ:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_MERGE_REQ)
                break;
            
        /* Forceful call clear can also be recvd by MMI */            
        case ICF_FORCEFUL_CALL_CLEAR_IND:    
           ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_CALL_CLEAR)
            break;          

#ifdef ICF_QOS_SUPPORT
		/* These messages can be received from NM and contain the expected 
		 * responses for Secondary PDP Context Activation/Deactivation requests
         */
        case ICF_NM_MMI_SEC_PDP_ACT_RESP:
		   ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_NM_QOS_RESERVE_RESP)
            break;          

		case ICF_NM_MMI_SEC_PDP_DEACT_IND:
		   ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_NM_QOS_RELEASE_IND)
            break;   
		   
        case ICF_NM_MMI_SEC_PDP_MODIFY_IND:
		   ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_NM_QOS_MODIFY_IND)
            break;

		case ICF_NM_MMI_QOS_RESERVE_NON_IMS_RESP:
		   ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_NM_QOS_RESERVE_RESP)
            break;          

#endif
		/* If the msg is recvd from MRM, then the function to map API's recvd 
         * from mrm is called.This function will map the external msg recvd 
         * from MM/RM to the corresponding event.
         */
        case ICF_MSG_FROM_MRM:
            ret_val = icf_mmi_mrm_event(p_mmi_pdb);
                break;
                
        /* Any other msg id is not expected. Thus error is raised and failure 
         * is returned by the function.
         */
        default:
            ICF_MMI_SET_ECODE(p_mmi_pdb,ICF_INVALID_MSG_ID_RECD)
            break;
    }
    
    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_mmi_mrm_event
 *
 * DESCRIPTION:     This function maps the API ids recvd from MRM to event.
 *                  
 * RETURNS:         ICF_SUCCESS in case of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

icf_return_t icf_mmi_mrm_event(
        INOUT icf_mmi_pdb_st             *p_mmi_pdb)
{
    icf_return_t                         ret_val = ICF_SUCCESS;
    
    ICF_FUNCTION_ENTER(p_mmi_pdb->p_glb_pdb)
        
    switch(p_mmi_pdb->p_glb_pdb->p_recv_msg->hdr.api_id)
    {
        case ICF_GET_CODEC_LIST_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_GCL_RESP)
            break;

        /* Codec commit response is recvd from RM */
        case ICF_CODEC_COMMIT_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_COMMIT_RESP)
            break;

        /* CMS Response is recvd from MM */
        case ICF_CREATE_MEDIA_SESSION_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_CMS_RESP)
            break;

        /* MMS response is recvd from MM */
        case ICF_MODIFY_MEDIA_SESSION_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_MMS_RESP)
            break;

        /* DMS response is recvd from MM */
        case ICF_DELETE_MEDIA_SESSION_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_DMS_RESP)
            break;

        /* DMS indication is recvd from MM */
        case ICF_DELETE_MEDIA_SESSION_IND:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_DMS_IND)
            /*ICF_MMI_APP_TRACE(p_mmi_pdb, ICF_TRACE_DMS_IND)*/
            break;                

        /* Merge response is recvd from MM */
        case ICF_MERGE_MEDIA_SESSION_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_MERGE_RESP)
            break;

		case ICF_OPEN_MEDIA_CHANNEL_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_OPEN_CHANNEL_RESP)
            break; 

        /* Any other API is not expected from MM/RM. So the function will 
         * return failure
         */
#ifdef ICF_NAT_MEDIA_TRAVERSAL
        case ICF_NAT_STUN_INIT_RESP:
            ICF_PRINT(("\n Init Response from NAT Client Library received\n"));
            break;
        case ICF_NAT_GET_PUBLIC_IP_PORT_RESP:
            ICF_MMI_SET_EVENT(p_mmi_pdb,ICF_MMI_NAT_GET_PUBLIC_IP_PORT_RESP)
            break; 

        case ICF_NAT_ERROR_IND:
             ICF_PRINT(("\n Error Indication from NAT Client Library received\n"));
             break;

        case ICF_NAT_STUN_EXEC_PROC_RESP:
             ICF_PRINT(("\n Execute Procedure response from NAT Client Library received\n"));
             break;

        case ICF_NAT_STUN_PROC_STATUS_IND:
             ICF_PRINT(("\n Execute Procedure Indication from NAT Client Library received\n"));
             break;
#endif
            
        default:
            ICF_MMI_SET_ECODE(p_mmi_pdb,ICF_INVALID_API_ID_RECD)
            break;                
    }

    ICF_FUNCTION_EXIT(p_mmi_pdb->p_glb_pdb)
    return ret_val;        
}    
        


/*******************************************************************************
 *
 * FUNCTION:        icf_mmi_process_mesg
 *
 * DESCRIPTION:     This is the entry point function of MMI that is invoked by 
 *                  the respective module invoking the request towards the MMI.  
 *					The function checks the API Id in the message 
 *                  received and invokes appropriate handler based on current 
 *                  call state and the event (mapped from API Id).
 *                  
 * RETURNS:         ICF_SUCCESS in acse of success and ICF_FAILURE 
 *                  in case of failure.                 
 *
 ******************************************************************************/

icf_return_t icf_mmi_process_mesg(
        INOUT icf_internal_msg_st        *p_internal_msg)
{
    icf_return_t                         ret_val=ICF_SUCCESS;
    icf_mmi_pdb_st                       mmi_pdb;
    icf_mmi_data_st                      *p_mmi_data = ICF_NULL;
	icf_internal_list_st				*p_src_list = ICF_NULL;
	icf_boolean_t                      remote_sdp = ICF_FALSE;
    icf_error_t                           ecode = ICF_ERROR_NONE;
  
    ICF_SET_MODULE(p_internal_msg->msg_hdr.p_glb_pdb,ICF_INT_MODULE_MMI)
    ICF_FUNCTION_ENTER(p_internal_msg->msg_hdr.p_glb_pdb)        
        
	/*Initialize the mmi_pdb structure*/

	icf_port_memset((icf_void_t*)&mmi_pdb,
	(icf_uint8_t)0,(icf_uint32_t)ICF_PORT_SIZEOF(icf_mmi_pdb_st));

    mmi_pdb.p_glb_pdb = p_internal_msg->msg_hdr.p_glb_pdb;
    mmi_pdb.p_internal_msg = p_internal_msg;
    mmi_pdb.p_ecode =  &(mmi_pdb.p_internal_msg->msg_hdr.ecode);

ICF_MM_ROUTING_START;
    /* Routing is needed if dest_module is MM and it is an App api */
    if(ICF_NULL != mmi_pdb.p_glb_pdb->p_recv_msg)
    {
#ifdef ICF_MM_REQUEST_BUFFERING
        if(ICF_TRUE == icf_mmi_check_mm_resp_msg(mmi_pdb.p_glb_pdb->p_recv_msg))
        {
            icf_mmi_data_st *p_mmi_data=ICF_NULL;
            
            ret_val = icf_dbm_get_module_glb_data(mmi_pdb.p_glb_pdb,
                (icf_module_id_t)(ICF_INT_MODULE_MMI),
                (icf_void_t *)&p_mmi_data,
                mmi_pdb.p_ecode);
            if(ICF_SUCCESS == ret_val)
            {
                icf_msg_st *p_msg = ICF_NULL;
                p_mmi_data->mm_resp_awaited = ICF_FALSE;
                icf_mmi_pop_buffered_msg(mmi_pdb.p_glb_pdb,p_mmi_data,
                    &p_msg,
                    mmi_pdb.p_ecode);
                if(ICF_NULL != p_msg)
                {
                    ret_val = icf_mmi_send(&mmi_pdb,p_msg,mmi_pdb.p_ecode);

                    ICF_MEMFREE(mmi_pdb.p_glb_pdb,
								p_msg,
								ICF_MEM_COMMON,
                        		mmi_pdb.p_ecode,
								ret_val);

                }
            }
                
        }
#endif

        if ((ICF_MODULE_PA == mmi_pdb.p_glb_pdb->p_recv_msg->hdr.source_id) &&
            (ICF_MODULE_MM == 
					mmi_pdb.p_glb_pdb->p_recv_msg->hdr.destination_id))
        {
            ret_val = icf_mmi_proc_icf_imsc_msg(&mmi_pdb);
            ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                return ret_val;
        }
        
        /* Routing is needed if dest_module is App and it is an MM api */
        if ((ICF_MODULE_MM == mmi_pdb.p_glb_pdb->p_recv_msg->hdr.source_id) &&
            (ICF_MODULE_PA == 
			 		mmi_pdb.p_glb_pdb->p_recv_msg->hdr.destination_id))
        {
            ret_val = icf_mmi_proc_imsc_icf_msg(&mmi_pdb);
            ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                return ret_val;
        }

#ifdef ICF_NAT_MEDIA_TRAVERSAL
        if ((ICF_MODULE_NAT == mmi_pdb.p_glb_pdb->p_recv_msg->hdr.source_id) &&
            (ICF_MODULE_MM == 
					mmi_pdb.p_glb_pdb->p_recv_msg->hdr.destination_id))
        {
            ret_val = icf_mmi_proc_icf_imsc_msg(&mmi_pdb);
            ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                return ret_val;
        }
        
        /* Routing is needed if dest_module is App and it is an MM api */
        if ((ICF_MODULE_MM == mmi_pdb.p_glb_pdb->p_recv_msg->hdr.source_id) &&
            (ICF_MODULE_NAT == 
			 		mmi_pdb.p_glb_pdb->p_recv_msg->hdr.destination_id))
        {
            ret_val = icf_mmi_proc_imsc_icf_msg(&mmi_pdb);
            ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                return ret_val;
        }
       
        if(ICF_NAT_STUN_INIT_RESP == mmi_pdb.p_glb_pdb->p_recv_msg->hdr.api_id) 
        {
            ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                return ICF_SUCCESS;
        }
#endif

    }
ICF_MM_ROUTING_END

    if(ICF_GET_MEDIA_CAP_REQ == mmi_pdb.p_internal_msg->msg_hdr.msg_id) 
    {
        /* Call the handler for media capabilities request*/
        ret_val = icf_mmi_xx_med_cap_request(&mmi_pdb);
        ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
        ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)					
        return ret_val;
    }
    else if(ICF_CONFIG_SELF_IP_REQ == mmi_pdb.p_internal_msg->msg_hdr.msg_id) 
    {
	/* Call the handler for media capabilities request*/
        ret_val = icf_send_self_ip_req_to_mm(&mmi_pdb);
        ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
        ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)					
        return ret_val;
    }

    /* 
     * If this was media capabilities response dont enter the FSM
     * Send media capabilities response to SSA 
     */
    else if(ICF_MSG_FROM_MRM == mmi_pdb.p_internal_msg->msg_hdr.msg_id &&
            (ICF_NULL != mmi_pdb.p_glb_pdb->p_recv_msg)
            && ICF_MEDIA_CAPABILITIES_RESP == 
            mmi_pdb.p_glb_pdb->p_recv_msg->hdr.api_id)
    {
        /* Call the handler for media capabilities request*/
        ret_val = icf_mmi_xx_med_cap_resp(&mmi_pdb);
        ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
        ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)               
        return ret_val;
    }

    if((ICF_NULL ==  mmi_pdb.p_glb_pdb->p_call_ctx)
       && (ICF_NULL != mmi_pdb.p_glb_pdb->p_recv_msg))
    {
        if(mmi_pdb.p_glb_pdb->p_recv_msg->hdr.call_id < ICF_INVALID_CALL_ID)
        {
            /* Message is from MMI AL so ask dbm to fetch call context */
            /* Request DBM for the ctx with the call id in the recvd msg */
    
           if(ICF_FAILURE == icf_dbm_find_call_cntxt_blk(
                       mmi_pdb.p_glb_pdb,
                       mmi_pdb.p_glb_pdb->p_recv_msg->hdr.call_id,
                       &(mmi_pdb.p_glb_pdb->p_call_ctx),
                       mmi_pdb.p_ecode))
            {
                ecode = ICF_CAUSE_INVALID_CALL_ID;
                ret_val = icf_mmi_send_unexpected_msg_ind(&mmi_pdb, &ecode);                
                ICF_MMI_TRACE(&mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)
                
                ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)               
                return ret_val;
            }
           
            else if(mmi_pdb.p_glb_pdb->p_call_ctx->call_id != 
                    mmi_pdb.p_glb_pdb->p_recv_msg->hdr.call_id)
            {
                /* Invalid pointer returned by DBM */
                ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
                ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)
                return ret_val;
            }           
       }
        
        else
        {
            ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
            ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)
            return ret_val;
        }        
    }
    mmi_pdb.p_call_ctx =  mmi_pdb.p_glb_pdb->p_call_ctx;
    mmi_pdb.p_mmi_ctx =  &(mmi_pdb.p_call_ctx->mmi_context);

    ICF_MMI_TRACE(&mmi_pdb,ICF_MMI_TRACE_CURRENT_STATE)
    
    ret_val = icf_dbm_get_module_glb_data(mmi_pdb.p_glb_pdb,
        (icf_module_id_t)(ICF_INT_MODULE_MMI),
        (icf_void_t *)&p_mmi_data,
        mmi_pdb.p_ecode);
    
    if(ICF_FAILURE == ret_val)
    {
        ICF_MMI_TRACE(&mmi_pdb,ICF_TRACE_FAILURE_BY_DBM)

        ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
        ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)
        return ret_val;
    }        

    ret_val = icf_mmi_api_to_event(&mmi_pdb);

	p_src_list = mmi_pdb.p_call_ctx->p_stream_sdp_info;

	while(ICF_NULL != p_src_list)
	{
		if(ICF_NULL != ((icf_stream_sdp_info_st *)p_src_list->p_payload)->p_remote_received_sdp)
		{
			remote_sdp = ICF_TRUE;
			break;
		}
		p_src_list = p_src_list->p_next;
	}

	if(ICF_TRUE == remote_sdp && 
                       ((mmi_pdb.p_internal_msg->msg_hdr.msg_id == ICF_CREATE_MEDIA_REQ)
                        ||(mmi_pdb.p_internal_msg->msg_hdr.msg_id == ICF_MODIFY_MEDIA_REQ)
                        ||(mmi_pdb.p_internal_msg->msg_hdr.msg_id == ICF_CREATE_CONF_REQ)))
	{
		icf_mmi_copy_remote_recvd_sdp_in_mmi_ctxt(&mmi_pdb);
	}	
	   

    if(ICF_SUCCESS == ret_val)
    {
        ret_val = p_mmi_data->mmi_fsm_table[mmi_pdb.p_call_ctx->
            mmi_context.mmi_state][mmi_pdb.p_call_ctx->
            mmi_context.mmi_event](&mmi_pdb);
    }

        
    ICF_FUNCTION_EXIT(mmi_pdb.p_glb_pdb)
    ICF_RESET_MODULE(mmi_pdb.p_glb_pdb)

    return ret_val;
}/* End function */


                      
