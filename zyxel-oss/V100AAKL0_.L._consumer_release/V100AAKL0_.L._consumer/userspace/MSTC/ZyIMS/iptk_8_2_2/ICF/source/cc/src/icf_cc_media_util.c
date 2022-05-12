/*------------------------------------------------------------------------------
 * File name        : icf_cc_media_util.c
 *
 * Purpose          : This file contains the utility function required by CC.
 *
 * Revision History :
 *
 * Date         Name            Ref#              Comments
 * --------     ------------    ----              ---------------
 * 01-Aug-2006  Anuradha Gupta  ICF 6.0        Code changes for Multiple m-lines
 *
 * 31-Aug-2006	Deepti Goyal	SPR 12693		Bitmask for media stream param
 *												in call modify resp. to peer set
 * 19-Jan-2007  Aman Aggarwal	SPR 13998	Reset the bitmask call type in cal *                                              l context
 * 3-Feb-2007   Abhishek Dhammawat SPR 14459    Copying codecs from line for default 
 *                                              audio call
 * 06-March-2007 Abhishek Dhammawat SPR14647   Before call to icf_cc_copy_internal_sdp_stream
 *                                             Added the null check for source stream.
 * 26-Mar-2007  Deepti Goyal        ICF 6.4     Changes for media connect reque
 *                                              st API from pa.
 * 26-Mar-2007  Rajat Chandna                   Changes in func. icf_cc_check_if_update_to_be_sent
 *                                              for update/reinvite enhancement.
 * 19-Apr-2007  Rajat Chandna                   Changes in func. icf_cc_check_if_update_to_be_sent
 *                                              for handling update/reinvite sending in
 *                                              Non Fast Start call. 
 * 09-Jul-2007  Neha Choraria       ICF 7.0     Changes for Forking &
 *                                              Multiple Answer.  
 * 10-Jul-2007  Neha Choraria       ICF 7.0     Changes for Re-Invite w/o SDP
 *
 * 10-Jul-2007  Abhishek Dutta      ICF 7.0     Changes for cancel call modify
 *                                              req API from pa.
 * 22-Jul-07    Neha                            Fixed UT issues	
 * 24-Jul-2007  Vidyut Gupta                    Merging the SPR 14905 from Alcatel branch
 * 7-Aug-2007   Amit Mahajan                    Klocwork warning removal
 * 10-Dec-2007  Alok Tiwari        ICF 7.0      spr-17634 fixed.
 * 19-Mar-2008  Shruti Thakral     SPR 18247    Klocwork warning removal
 * 28-Mar-2008  Sumant Gupta       ICF 8.0      KlockWork Warning Removal
 * 3-Apr-2008   Sumant Gupta    ICF 8.0         Rel7.1 Windows porting Merging		
 * 12-Apr-2008  Alok Tiwari      SPR-18359      Klocwork Warning removal
 * 28-Apr-2008  Kamini Gangwani  SPR-18490      ICF_CHECK_IMS_START macro check
 *                                              removed from fax code
 * 27-May-2008  Tarun Gupta      SPR 18585      Modified icf_cc_check_ssa_offer_change                                             
 * 30-May-2008  Tarun Gupta      SPR 18585      Modified icf_cc_set_remote_call_hold_status
 * 05-Jun-2008  Tarun Gupta      SPR 18585      Modified icf_cc_pa_connect_send_mms_req
 * 06-Jun-2008  Tarun Gupta      SPR 18585      Merged CSR:1-5943186
 * 06-Jun-2008  Tarun Gupta      SPR 18585      Remote resume and remote hold 
 *                                              bitmask interaction
 * 07-Jun-2008  Amit Sharma       IPTK 8.0 SRTP LLD      SRTP Enhancements
 * 26-jun-2008  Anurag Khare    SPR 18684       CSR 1-6074322 Merge 
 * 30-Jun-2008  Anurag Khare    SPR 18684       CSR 1-6082332 Merge 
 * 02-July-2008 Anurag Khare    SPR 18684       CSR 1-6088455 Merge
 * 04-July-2008 Anurag Khare    SPR 18684       CSR 1-6096114 Merge
 * 04-Nov-2008  Abhishek Dhammawat SPR 19189    Merge For SPR19015
 * 04-Nov-2008  Anuradha Gupta    SPR 19188     Rel 7.1.2 SPR Merging
 * 05-Nov-2008  Rajiv Kumar       SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                              18877) 
 * 01-Dec-2008  Rajiv Kumar       SPR 19215     CSR-1-6212448 Merged
 * 01-dec-2008  Rajiv Kumar       SPR 19188     Rel 7.1.2 SPR merging (SPR
 *                                              18991) 
 * 08-Dec-2008  Rajiv Kumar       SPR 19218     Merged CSR 1-6131201 
 * 16-Dec-2008  Anurag Khare      SPR 19218     CSR 1-6449001 Merged
 * 10-Feb-2009  Tarun Gupta       Rel 8.1 LLD   SDP Tunnelling Enhancements
 * 11-Feb-2009  Anurag Khare      ICF Rel 8.1   changes done for call
 *                                              modify enhancements
 * 12-Feb-2009  Ashutosh Mohan    Rel8.1        Added handling, to free icf_tunnelled_sdp_st
 * 18-Feb-2009  Tarun Gupta       Rel 8.1       Added new function definition:
 *                                              icf_cc_fetch_and_delete_sdp_stream_nodes_in_call_ctx
 * 04-Mar-2009 Saurabh Gupta	   SPR 19349       Klocworks fix
 * 04-Mar-2009  Tarun Gupta       Rel 8.1       Modified icf_cc_create_audio_call_by_default 
 * 06-Mar-2009  Tarun Gupta       Rel 8.1       Modified icf_cc_validate_ith_level_m_line_check,
 *                                              icf_cc_make_mms_type_for_offer_in_tunnel_mode and
 *                                              icf_cc_make_mms_type_for_answer_in_tunnel_mode 
 * 01-Apr-2009 Kamal Ashraf       Rel 8.2       Klocworks fix
 * 07-Apr-2008 Rajiv Kumar        SPR 19428     Merged CSR-1-6767492
 * 19-Apr-2009 Abhishek Dhammawat SPR 19450     Modified icf_cc_clear_all_stream_sdp
 * 23-Apr-2009 Abhishek Dhammawat SPR 19502     Modified 
 *                                        icf_cc_fetch_and_delete_sdp_stream_nodes_in_call_ctx
 * 18-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6347417 merging                                        
 * 26-May-2009  Anuradha Gupta    SPR 19672     CSR_1-6727350 merging
 * 28-May-2009  Ashutosh Mohan    SPR 19672     CSR-1-6976670 Merged
 * 11-Jun-2009 Ashutosh Mohan     SPR 19737     Changes done for GCC 4.1 warnings removal
 * 29-Jul-2009  Tarun Gupta       SPR 19886     Merged CSR 1-7438489
 * 06-Aug-2009  Tarun Gupta       SPR 19886     Merged CSR 1-7245222
 * 06-Aug-2009  Tarun Gupta       SPR 19886     Merged CSR 1-7402396
 * 07-Sep-2009  Anuradha Gupta    SPR 19999     Merged CSR 1-7574801
 * 10-Sep-2009  Anuradha Gupta    SPR 19999     Merged CSR 1-7643774
 * 25-May-2010  Preksha           SPR 20412     Merged CSR 1-7792265
 * 25-May-2010  Udit Gattani      SPR 20407     Modified function
                                               icf_cc_check_mms_req_for_forked_resp 
 * 
 * Copyright (c) 2009, Aricent.
 *----------------------------------------------------------------------------*/

#include "icf_common_interface_types.h"
#include "icf_cc_prototypes.h"
#include "icf_cc_macro.h"
#include "icf_dbm_prototypes.h"
#include "icf_port_macros.h"
#include "icf_internal_api_id.h"

#ifdef ICF_PORT_WINDOWS
#pragma warning ( disable : 4702 )
#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_local_call_modify_offer
 *
 * DESCRIPTION:     This function processes the local call modify offer
 ******************************************************************************/
icf_return_t     icf_cc_process_local_call_modify_offer(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    icf_call_modify_req_st *p_call_modify_req = (icf_call_modify_req_st *)
        p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

    icf_internal_list_st *p_list = ICF_NULL;
    icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL;
    icf_call_modify_media_data_st *p_call_modify_req_data = ICF_NULL;
    icf_list_st *p_modify_list = p_call_modify_req-> p_media_modify_list;
    icf_uint8_t ctr=0;

    ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_SEND_OFFER_TO_NW)
        ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_NULL;

    if(ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT ==
            (ICF_CALL_MOD_REQ_MEDIA_MOD_LIST_PRESENT & p_call_modify_req->bit_mask))
    {
        while(ICF_NULL != p_modify_list)
        {
            icf_uint32_t stream_id = 0;

            p_call_modify_req_data  = (icf_call_modify_media_data_st *)
                p_modify_list-> p_data;

            stream_id = p_call_modify_req_data->stream_id;

            /* get the stream SDP info corresponding to the stream ID */
            p_list = icf_cmn_get_node_in_internal_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info,&stream_id);

            if(ICF_NULL != p_list)
            {
                p_stream_sdp_info = (icf_stream_sdp_info_st *)p_list->p_payload;

                /* Copy p_stream_sdp_info in cc pdb */
                p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;

                p_cc_pdb->p_generic_ptr_1 = p_call_modify_req_data;
            }

            /* check if the steram is active, if not then this is a call modify 
               request for new stream addition. Reject such request with proper
               error cause set */
            if(ICF_NULL == p_list || (ICF_MEDIA_STREAM_ACTIVE !=
                        (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp_info->stream_status)))
            {
                /* reject the request*/
                p_cc_pdb->p_call_ctx->app_failure_cause =
                    ICF_CAUSE_STREAM_ADDITION_NOT_ALLOWED;
                return ICF_SUCCESS;

            }
            else if(p_stream_sdp_info->stream_type !=
                    p_call_modify_req_data->stream_type)
            {
                /* reject the request*/
                p_cc_pdb->p_call_ctx->app_failure_cause =
                    ICF_CAUSE_INVALID_CALL_MODIFY_REQUEST;
                return ICF_SUCCESS;

            }
            else if(ICF_FAILURE == 
                    icf_cc_make_mms_type_for_local_offer_change(p_cc_pdb))
            {
                return ICF_FAILURE;
            }


            p_modify_list = p_modify_list->p_next;
        }
    }

    if((ICF_SUCCESS == ret_val) && (ICF_NULL == p_cc_pdb->p_call_ctx->app_failure_cause) &&
            (ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT ==
             (ICF_CALL_MOD_REQ_DELETED_STREAM_PRESENT & 
              p_call_modify_req->bit_mask)))
    {
        icf_uint32_t stream_id = 0;

        for(ctr=0; (ctr < p_call_modify_req->count) && (ctr < ICF_MAX_STREAM_PER_CALL);ctr++)
        {
            stream_id = p_call_modify_req->deleted_media_streams[ctr].stream_id;

            /* get the stream SDP info corresponding to the stream ID */
            p_list = icf_cmn_get_node_in_internal_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info, &stream_id);

            if(ICF_NULL != p_list)
            {

                p_stream_sdp_info = (icf_stream_sdp_info_st *)p_list->p_payload;

                if(ICF_MEDIA_STREAM_ACTIVE ==
                        (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE))
                {
                    p_stream_sdp_info->media_modify_type = ICF_STREAM_DELETE;

                    ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,
                            ICF_CC_SEND_OFFER_TO_NW)
                }
            }
        }
    }

    if(ICF_CC_SEND_OFFER_TO_NW != 
            (ICF_CC_SEND_OFFER_TO_NW & p_cc_pdb->common_bitmask))
    {
        /*no SDP related change. Reset flag so that the version number is
          not increased */
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_FALSE;
    }
    else
    {
        p_cc_pdb->p_call_ctx->sdp_changed = ICF_TRUE;
    }


    return ret_val;
}




/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_local_offer_change
 *
 * DESCRIPTION:     This function processes the local call modify offer
 ******************************************************************************/
icf_return_t     icf_cc_make_mms_type_for_local_offer_change(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_list_st             *p_pref_profile_node = ICF_NULL;
    icf_stream_capab_st     *p_pref_profile_data = ICF_NULL;


    icf_call_modify_media_data_st *p_call_modify_req_data =
        (icf_call_modify_media_data_st *)p_cc_pdb->p_generic_ptr_1;


    /* get the stream SDP info corresponding to the stream ID */
    icf_stream_sdp_info_st	*p_stream_sdp_info = p_cc_pdb->p_stream_sdp_info;

    /* applicable only for audio and video streams*/
    if(ICF_STREAM_TYPE_AUDIO ==
            p_call_modify_req_data->stream_type ||
            ICF_STREAM_TYPE_VIDEO ==
            p_call_modify_req_data->stream_type)
    {
        if(ICF_CALL_MOD_CODEC_INFO_PRESENT ==
                (ICF_CALL_MOD_CODEC_INFO_PRESENT &
                 p_call_modify_req_data->bit_mask))
        {    
            p_cc_pdb->p_list_1 = p_call_modify_req_data->p_codec_info_list;

            p_cc_pdb->p_list_2 = p_stream_sdp_info->p_applied_sdp->sdp_stream.\
                                 p_codec_info_list;

            if(ICF_TRUE != icf_cc_compare_codec_lists(p_cc_pdb))
            {
                /* copy the codec info in local sdp */
                if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                            p_cc_pdb->p_glb_pdb,
                            &(p_stream_sdp_info->p_local_sdp->
                                sdp_stream.p_codec_info_list),
                            p_call_modify_req_data->p_codec_info_list))
                {
                    return ICF_FAILURE;
                }

                p_stream_sdp_info->media_modify_type |=
                    ICF_LOCAL_SDP_CODEC_CHANGE;

                /* spr 14476 */
                /*  THe call_dir check is removed because
                    THis copying the codec list in pref_med_prod 
                    should always be done either it is an 
                    incoming call or outgoing call
                    THis code change came for 17634 in 7.0
                    PQT Support. This code change was incorrect
                    THe proper fix was done in this CSR
                 */
                if (ICF_NULL ==  
                        (p_pref_profile_node = 
                         icf_cmn_get_node_from_stream_capab_list(		
                             p_cc_pdb->p_call_ctx->p_pref_media_profile,
                             p_stream_sdp_info->stream_type)))
                {
                    /* CSR 1-6082332 Merge  */
                    /* No need to return failure from here. If pref media 
                     * profile is empty in call ctx, simply ignore it and 
                     * continue. This will happen in case of incoming call when
                     * App has not specified any media profile in app_add_req.
                     */
                     ICF_PRINT(((icf_uint8_t*)"\nPreferred profile empty in call context\n"));
                }
                else
                {
                    p_pref_profile_data = p_pref_profile_node->p_data;
                    p_pref_profile_data->bit_mask |= 
                        ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
                    if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                                p_cc_pdb->p_glb_pdb,
                                &(p_pref_profile_data->p_codec_info_list), 
                                p_call_modify_req_data->p_codec_info_list))
                    {
                             if (p_pref_profile_data->p_codec_info_list != ICF_NULL)
                        {
                            icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
						    &(p_pref_profile_data->p_codec_info_list),
						    p_cc_pdb->p_ecode);
                        }
 
         			    return ICF_FAILURE;
                    }
                   /*Klocwork Warning Removal*/
                   ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                               p_pref_profile_data->p_codec_info_list)
                 } 
                /* end of fix corresponding to spr 14476.*/

                ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_SEND_OFFER_TO_NW)

                    if(ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT == 
                            (ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT & 
                             p_call_modify_req_data->bit_mask))
                    {

                        p_stream_sdp_info->p_local_sdp->
                            sdp_stream.media_stream_param =
                            p_call_modify_req_data->media_stream_param;

                        p_stream_sdp_info->media_modify_type |=
                            ICF_LOCAL_SDP_MEDIA_CHANGE;
                    }

                if(ICF_CALL_MOD_ENC_ATTRIB_PRESENT == 
                        (ICF_CALL_MOD_ENC_ATTRIB_PRESENT & 
                         p_call_modify_req_data->bit_mask))
                {

                    icf_cmn_copy_string_list(
                            p_cc_pdb->p_glb_pdb,
                            &(p_stream_sdp_info->p_local_sdp->sdp_stream.
                                p_encoded_attributes),
                            p_call_modify_req_data->p_encoded_attributes,
                            p_cc_pdb->p_ecode);

                    p_stream_sdp_info->media_modify_type |=
                        ICF_LOCAL_SDP_MEDIA_CHANGE;
                }
                /* CSR 1-6074322  Merge*/
                /* Clear the stream level encoded attributes if App has not
                 * given in ICF_CALL_MODIFY_REQ. The previously stored list
                 * should not be sent in media APIs.
                 */
                else if ((ICF_NULL == (ICF_CALL_MOD_ENC_ATTRIB_PRESENT &
                                        p_call_modify_req_data->bit_mask)) &&
                        (ICF_CC_PA_CALL_MODIFY_REQ == p_cc_pdb->current_event))
                {
                    icf_cmn_delete_list(
                        p_cc_pdb->p_glb_pdb,
                        (&(p_stream_sdp_info->p_local_sdp->sdp_stream.\
                            p_encoded_attributes)),
                        ICF_MEM_COMMON);
                    p_stream_sdp_info->p_local_sdp->sdp_stream.\
                        p_encoded_attributes = ICF_NULL;
                }
            } 		
        }

        if(ICF_CALL_MOD_MEDIA_MODE_PRESENT ==
                (ICF_CALL_MOD_MEDIA_MODE_PRESENT &
                 p_call_modify_req_data->bit_mask))
        {

            /* Copy mode from call modify req in cc pdb */
            p_cc_pdb->application_mode
                = p_call_modify_req_data->media_mode;



            icf_cc_calc_mode_app_change(p_cc_pdb);

        }
    }
    /* applicable only for TBCP streams */
    else if((ICF_CALL_MOD_ENC_ATTRIB_PRESENT ==
                (ICF_CALL_MOD_ENC_ATTRIB_PRESENT &
                 p_call_modify_req_data->bit_mask)) &&
            ICF_STREAM_TYPE_TBCP == p_call_modify_req_data->stream_type)
    {
        if(ICF_FAILURE == icf_cmn_copy_string_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_stream_sdp_info->p_local_sdp->sdp_stream.
                        p_encoded_attributes),
                    p_call_modify_req_data->p_encoded_attributes,
                    p_cc_pdb->p_ecode))
        {
            return ICF_FAILURE;
        }

        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_SEND_OFFER_TO_NW)

    }

  if ((ret_val == ICF_FAILURE) && (p_pref_profile_data->p_codec_info_list != ICF_NULL))
    {
        icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                &(p_pref_profile_data->p_codec_info_list),
                p_cc_pdb->p_ecode);
    }
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_fax_req
 *
 * DESCRIPTION:     This function checks if the incoming offer was for starting
 *					a fax request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_fax_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_FAILURE;
    icf_nw_inc_media_change_req_st  *p_update_req = ICF_NULL;
    icf_inc_media_sdp_info_st   *p_fax_req = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

#ifdef NON_IMS_CLIENT

#ifdef ICF_FAX_SUPPORT

        p_update_req = (icf_nw_inc_media_change_req_st *)
        p_cc_pdb->p_internal_msg->p_msg_data;
    /*
     * If SDP is present  and media is committed then copy it in current call
     * context
     */
    if(ICF_NW_INC_MEDIA_ADD_STREAM ==
            (ICF_NW_INC_MEDIA_ADD_STREAM & p_update_req->bitmask))
    {
        p_fax_req = (icf_inc_media_sdp_info_st *)
            p_update_req->p_added_sdp_list->p_data;

        if(ICF_STREAM_TYPE_FAX == p_fax_req->stream_type)
            ret_val = ICF_SUCCESS;
        else
            ret_val = ICF_FAILURE;
    }
    else
    {
        ret_val = ICF_FAILURE;
    }

#endif /* ICF_FAX_SUPPORT */
#endif /* NON_IMS_CLIENT */

        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_fax_resp
 *
 * DESCRIPTION:     This function checks if the incoming answer was a fax response
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_fax_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_FAILURE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */


#ifdef ICF_FAX_SUPPORT


        if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
        {
            /* this means , fax call request have been initiated but
               it is yet to be established. So the reinvite response
               is a response for fax call offer */
            ret_val = ICF_SUCCESS;
        }

#endif /* ICF_FAX_SUPPORT */

        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

#ifdef ICF_FAX_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_local_fax_req
 *
 * DESCRIPTION:     This function validates and processes the local fax request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_local_fax_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;

    icf_start_fax_req_st *p_start_fax_req = (icf_start_fax_req_st *)
        p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

    /* get the audio stream info */
    icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL; 

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

        p_stream_sdp_info = (icf_stream_sdp_info_st *)p_cc_pdb->p_call_ctx
        ->p_stream_sdp_info->p_payload;

    /* if it is not an audio call, fax can not start, reject the request */
    if(ICF_CALL_TYPE_AUDIO !=
            (0xFF & p_cc_pdb->p_call_ctx->call_type))
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_CALL_NOT_ALLOWED;
        return ICF_SUCCESS;
    }
    else if(ICF_FAX_PT_CALL != p_start_fax_req->fax_type &&
            ICF_T38_FAX_CALL != p_start_fax_req->fax_type &&
            ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL != p_start_fax_req->fax_type)
    {
        p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_INVALID_FAX_REQUEST;
        return ICF_SUCCESS;
    }
    else if(ICF_FAX_PT_CALL == p_start_fax_req->fax_type ||
            ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == p_start_fax_req->fax_type)
    {

        if(ICF_PREF_CODEC_FAX_PT_PCMA != p_start_fax_req->fax_pt_codec &&
                ICF_PREF_CODEC_FAX_PT_PCMU != p_start_fax_req->fax_pt_codec)
        {
            p_cc_pdb->p_call_ctx->app_failure_cause =
                ICF_CAUSE_INVALID_FAX_REQUEST;
            return ICF_SUCCESS;
        }
        if(ICF_START_FAX_REQ_PT_PARAM_PRESENT & p_start_fax_req->bitmask)
        {
            p_cc_pdb->p_call_ctx->pt_param = p_start_fax_req->pt_param;
            p_cc_pdb->p_call_ctx->common_bitmask_2 |= ICF_NEGOTIATE_PT_PARAM_PRESENT;
        }
    }

    /* take a copy of the local sdp in the last committed sdp
       on per stream basis */
    /* SPR14647: Added stream_status active check and null check */
    if((p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE) &&
            (ICF_NULL != p_stream_sdp_info->p_local_sdp))
    {

        if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb,
                    p_stream_sdp_info->p_local_sdp, /*Source*/
                    &(p_stream_sdp_info->p_last_commited_sdp) /*Destination*/))
        {
            ret_val = ICF_FAILURE;
        }
        else if(ICF_FAX_PT_CALL == p_start_fax_req->fax_type)
        {
            p_cc_pdb->p_call_ctx->pref_codec = p_start_fax_req->fax_pt_codec;
            p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
            icf_cc_process_fax_passthrough_req(p_cc_pdb);

        }
        else if(ICF_T38_FAX_CALL == p_start_fax_req->fax_type ||
                ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL == p_start_fax_req->fax_type)
        {
            /* add the new T38 stream */
            ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_LOCAL_FAX_REQ)
                if(ICF_SUCCESS == icf_cc_add_new_fax_stream(p_cc_pdb))
                {
                    /* set audio stream mode as inacvtive */
                    p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
                    p_cc_pdb->application_mode = ICF_MEDIA_MODE_INACTIVE;
                    icf_cc_calc_mode_app_change(p_cc_pdb);
                    p_stream_sdp_info->stream_status |= ICF_MEDIA_STREAM_CLINE_ZERO;
                    /* ANU : this mode should not be inactive, rather the previously applied mode */
                    p_stream_sdp_info->application_mode = 
                           p_stream_sdp_info->p_applied_sdp->media_mode;
                   /*
                    * Update the fax method in call context if it has been
    	            * received from Application
                    */
              	    if((p_start_fax_req->bitmask & ICF_START_FAX_REQ_METHOD_PRESENT))
               	    {
	         	p_cc_pdb->p_call_ctx->fax_method = 
		           p_start_fax_req->fax_method;
            	    }
                }
                else
                {
                    ret_val = ICF_FAILURE;
                }
        }
    }/*SPR14647*/   


    if(ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->p_call_ctx->fax_type = p_start_fax_req->fax_type;
        if(ICF_FAX_PT_CALL != p_start_fax_req->fax_type)
        {
            p_cc_pdb->p_call_ctx->pref_codec = p_start_fax_req->fax_pt_codec;
        }
        ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_START_FAX_FROM_APP)
    }
#ifdef ICF_LOAD_STAT
    if(ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->p_glb_pdb->load_data.otg_start_fax_req++;
    }
#endif


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_req
 *
 * DESCRIPTION:     This function validates and processes the remote fax request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;

    icf_uint8_t invalid_fax_stream = ICF_FALSE;
    icf_nw_inc_media_change_req_st *p_start_fax_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL; 

    icf_internal_sdp_stream_st *p_fax_sdp_info = ICF_NULL;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

    if(ICF_NW_INC_MEDIA_NW_REJECT_STREAM & p_start_fax_req->bitmask)
    {
        p_cc_pdb->p_list_1 = p_start_fax_req->p_nw_reject_stream_list;
        icf_cc_process_nw_rejected_stream_list(p_cc_pdb);
    }
   
    p_stream_sdp_info = (icf_stream_sdp_info_st *)p_cc_pdb->p_call_ctx
        ->p_stream_sdp_info->p_payload;

    /* if it is not an audio call, fax can not start, reject the request */
    if(ICF_CALL_TYPE_AUDIO !=
            (0xFF & p_cc_pdb->p_call_ctx->call_type))
    {
        if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_start_fax_req->bitmask))
        {
            icf_cc_delete_inc_media_sdp_info_list(
                    p_cc_pdb,&(p_start_fax_req->p_sdp_list));
        }

        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_FAX_ERR_CALL_NOT_ALLOWED;
        return ICF_SUCCESS;

    }
    else if(ICF_NW_INC_MEDIA_ADD_STREAM ==
            (ICF_NW_INC_MEDIA_ADD_STREAM & p_start_fax_req->bitmask))
    {
        if(ICF_FALSE ==  ((icf_inc_media_sdp_info_st *)p_start_fax_req->
                    p_added_sdp_list->p_data)->stream_active)
        {
            if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                    (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_start_fax_req->bitmask))
            {
                icf_cc_delete_inc_media_sdp_info_list(
                        p_cc_pdb,&(p_start_fax_req->p_sdp_list));
            }

            /* rejected list is present, that means remote has asked to add
               any new stream other than T38 , reject the offer */ 
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_FAX_ERR_INVALID_REQUEST;
            return ICF_SUCCESS;
        }
    }

    if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
            (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_start_fax_req->bitmask))
    {
        p_fax_sdp_info = ((icf_inc_media_sdp_info_st *)
                p_start_fax_req->p_sdp_list->p_data)->p_int_sdp_stream;


        p_cc_pdb->p_list_1 = p_start_fax_req->p_sdp_list;

        if(ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
        {
            /* ith level validation fails , reject the offer */
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_FAX_ERR_INVALID_REQUEST;
            return ICF_SUCCESS;
        }

        if (ICF_STREAM_DELETE != (p_stream_sdp_info->media_modify_type
                    & ICF_STREAM_DELETE))
        {
            icf_transport_address_st stream_address;
            icf_port_memset(&stream_address,0,
                    ICF_PORT_SIZEOF(icf_transport_address_st));
            stream_address.port_num = p_fax_sdp_info->sdp_stream.
                stream_address.port_num;

            p_cc_pdb->p_generic_ptr_2 = &stream_address;
            p_cc_pdb->p_generic_ptr_1 = &(p_fax_sdp_info->sdp_stream.
                    stream_address);
            icf_cc_compare_ip_and_port(p_cc_pdb);

            p_cc_pdb->p_generic_ptr_2 = ICF_NULL; /* klocworks */
            p_cc_pdb->p_generic_ptr_1 = ICF_NULL; /* klocworks */

            if((ICF_MEDIA_MODE_INACTIVE == p_fax_sdp_info->media_mode) ||
                    (ICF_CC_REM_IP_NULL == (ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)))
            {
                p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;
                p_stream_sdp_info->mm_applied_mode = ICF_MEDIA_MODE_INACTIVE;
            }
            else if(p_start_fax_req->bitmask & ICF_NW_INC_MEDIA_ADD_STREAM)
            {
                invalid_fax_stream=ICF_TRUE;
            }
            else
            {
                /* rejected list is present, that means remote has asked to add
                   any new stream other than T38 , reject the offer */
                p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_FAX_ERR_INVALID_REQUEST;
                return ICF_SUCCESS;
            }
        }

    }
    else
    {
        /* no SDP */
        /* Assumtion that remote want to replace fax with Audio

           Mark AUDIO stream as NW_DISALLOW , PRIVATE
           and remote MODE AS INACTIVE

           p_cc_pdb->p_call_ctx->media_state &= (~(bit));

         */
        p_stream_sdp_info->p_remote_received_sdp->media_mode =
            ICF_MEDIA_MODE_INACTIVE;
        p_stream_sdp_info->stream_status &= ~ICF_MEDIA_STREAM_NW_ALLOW;
        p_stream_sdp_info->stream_status |= ICF_MEDIA_STREAM_PRIVATE;

        p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;
        p_stream_sdp_info->mm_applied_mode = ICF_MEDIA_MODE_INACTIVE;
    }

    /* take a copy of the committed sdp in the last committed sdp
       on per stream basis, set the flag to FALSE */
    /* SPR14647: Added stream_status active check and null check */
    if((p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE) &&
            (ICF_NULL != p_stream_sdp_info->p_applied_sdp))
    {
        if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb, 
                    p_stream_sdp_info->p_applied_sdp,
                    &(p_stream_sdp_info->p_last_commited_sdp)))
        {
            ret_val = ICF_FAILURE;
        }  
        else if((ICF_NW_INC_MEDIA_ADD_STREAM ==
                (ICF_NW_INC_MEDIA_ADD_STREAM & p_start_fax_req->bitmask)) &&
                 (ICF_FALSE == invalid_fax_stream))
        {
            /*ADD T.38 stream sdp info and set mms type as ICF_STREAM_ADD*/
            /*take it from app profile if it is there for FAX*/

            if(ICF_FAILURE == icf_cc_add_new_fax_stream(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
            }
            else if(ICF_NULL != p_cc_pdb->p_call_ctx->offer_failure_reason)
            {
                ret_val = ICF_SUCCESS;         
            }
            else
            {
                /* copy the info in the added list to remote received SDP
                   of new FAX request */

                p_cc_pdb->p_int_sdp_stream = 
                    ((icf_inc_media_sdp_info_st *)p_start_fax_req->
                     p_added_sdp_list->p_data)->p_int_sdp_stream;

                ((icf_inc_media_sdp_info_st *)p_start_fax_req->
                 p_added_sdp_list->p_data)->p_int_sdp_stream = ICF_NULL;

                icf_cc_copy_int_sdp_stream_to_remote(p_cc_pdb);

                p_cc_pdb->p_stream_sdp_info->p_applied_sdp = ICF_NULL;
            }

            p_cc_pdb->p_call_ctx->fax_type = ICF_T38_FAX_CALL;
        }
        else if(ICF_TRUE == invalid_fax_stream)
        {
        
            p_cc_pdb->p_list_1 = p_start_fax_req->p_added_sdp_list;
            icf_cc_process_nw_added_invalid_stream_list(p_cc_pdb);
        
            p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
            icf_cc_compare_sdp_n_populate_mod_fileds(p_cc_pdb);
            p_cc_pdb->current_event = ICF_CC_SSA_OFFER_CHANGE;
        }

    }/*SPR14647*/   

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_resp
 *
 * DESCRIPTION:     This function validates and processes the remote fax
 *					success resposne.
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;

    icf_nw_inc_media_change_resp_st *p_fax_resp =
        (icf_nw_inc_media_change_resp_st *)
        p_cc_pdb->p_internal_msg->p_msg_data;

    icf_inc_media_sdp_info_st *p_fax_sdp_info = ICF_NULL;
    icf_internal_sdp_stream_st *p_stream_info = ICF_NULL;
    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;
    icf_internal_list_st	*p_list           = ICF_NULL;

    icf_uint32_t stream_id = 0;

    icf_list_st *p_temp_list = p_fax_resp->p_sdp_list;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        p_cc_pdb->p_list_1 = p_fax_resp->p_sdp_list;

    while(ICF_NULL != p_temp_list)
    {
        p_fax_sdp_info = (icf_inc_media_sdp_info_st *)p_temp_list->p_data;

        p_stream_info  = p_fax_sdp_info->p_int_sdp_stream;

        if(ICF_STREAM_TYPE_AUDIO == p_fax_sdp_info->stream_type)
        {
            stream_id = p_fax_sdp_info->stream_id;

            if(ICF_FALSE == p_fax_sdp_info->stream_active)
            {
                /*
                   there is a problem
                   clear the call
                 */
                p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                ret_val = ICF_FAILURE;
                break;
            }



            p_list = icf_cmn_get_node_in_internal_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                    &stream_id);

            if(ICF_NULL == p_list)
            {
                p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                ret_val = ICF_FAILURE;
                break;
            }
            else
            {
                p_stream_sdp_info = 
                    (icf_stream_sdp_info_st *)p_list->p_payload;
            }

            if(ICF_FAX_PT_CALL == p_cc_pdb->p_call_ctx->fax_type)
            {
                if(ICF_LOCAL_SDP_CODEC_CHANGE == 
                        (ICF_LOCAL_SDP_CODEC_CHANGE & 
                         p_stream_sdp_info->media_modify_type))
                {
                    p_stream_sdp_info->media_modify_type =
                        ICF_REMOTE_ANSWER_CODEC_CHANGE;
                }
                else if(ICF_LOCAL_SDP_MEDIA_CHANGE == 
                        (ICF_LOCAL_SDP_MEDIA_CHANGE & 
                         p_stream_sdp_info->media_modify_type))
                {
                    p_stream_sdp_info->media_modify_type =
                        ICF_REMOTE_SDP_MEDIA_CHANGE;
                }
            }
            else
            {
                icf_boolean_t is_valid = ICF_FALSE;

                icf_transport_address_st stream_address;

                icf_port_memset(&stream_address,0,
                        ICF_PORT_SIZEOF(icf_transport_address_st));

                stream_address.port_num =
                    p_fax_sdp_info->p_int_sdp_stream->sdp_stream.
                    stream_address.port_num;

                p_cc_pdb->p_generic_ptr_2 = &stream_address;
                p_cc_pdb->p_generic_ptr_1 = &(p_fax_sdp_info->p_int_sdp_stream->
                        sdp_stream.stream_address);
                /* In case of T.38 method reset the media modify type for audio stream*/
                p_stream_sdp_info->media_modify_type = ICF_NULL;

                icf_cc_compare_ip_and_port(p_cc_pdb);

                if((ICF_MEDIA_MODE_INACTIVE == p_stream_info->media_mode)
                        || (ICF_CC_REM_IP_NULL == (ICF_CC_REM_IP_NULL & 
                                p_cc_pdb->common_bitmask)))
                {
                    is_valid = ICF_TRUE;
                }

                if(ICF_FALSE == is_valid)
                {
                    /*
                       there is a problem
                       clear the call
                     */
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    break;
                }

            }

        }
        else if((ICF_STREAM_TYPE_FAX == p_fax_sdp_info->stream_type) &&
                (ICF_FAX_PT_CALL != p_cc_pdb->p_call_ctx->fax_type))
        {
            if(ICF_TRUE != p_fax_sdp_info->stream_active)
            {
                /*
                   there is a problem
                   clear the call
                 */
                p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                ret_val = ICF_FAILURE;
                break;
            }
            else
            {
                /*If the fax method type was replace type then SSA wouldn't have
                  sent the correct  stream id with the stream so we shouldnt use
                  it for fetching the stream info*/
                if(ICF_FAX_METHOD_REPLACE_STREAM == 
                       p_cc_pdb->p_call_ctx->fax_method)
                {
                    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

                    while(p_list)
                    {
                        if(((icf_stream_sdp_info_st*)p_list->p_payload)->\
                             stream_type == ICF_STREAM_TYPE_FAX)
                        {
                            break;
                        }
                        else
                        {
                            p_list = p_list->p_next;
                        }
                    }
                }
                else
                {
                stream_id = p_fax_sdp_info->stream_id;
                p_list = icf_cmn_get_node_in_internal_list(
                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                        &stream_id);
                }
                if(ICF_NULL == p_list)
                {
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    break;
                }
                else
                {
                    p_stream_sdp_info = 
                        (icf_stream_sdp_info_st *)p_list->p_payload;

                    p_stream_sdp_info->media_modify_type =
                        ICF_REMOTE_ANSWER_CODEC_CHANGE;
                }

            }
        }

        p_temp_list = p_temp_list->p_next;
    }

    if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
    {
        /* if ith level check has failed */
        return ICF_FAILURE;
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_err_resp
 *
 * DESCRIPTION:     This function validates and processes the remote fax
 *					error resposne.
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_err_resp(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;

    icf_internal_list_st	*p_list =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    icf_internal_list_st    *p_sdp_list = ICF_NULL;
    icf_stream_sdp_info_st *p_audio_stream_info = ICF_NULL;
    icf_stream_sdp_info_st *p_t38_stream_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        /* get the audio and T38 stream info*/

        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_AUDIO);

    if(ICF_NULL == p_sdp_list)
    {	
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ICF_FAILURE;
    }

    p_audio_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
    p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;
   
    if(ICF_FAX_METHOD_REPLACE_STREAM ==
           p_cc_pdb->p_call_ctx->fax_method)
    {
        /*To allow the audio stream that was disallowed on network*/
        p_audio_stream_info->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;

        /*To disalloww the rejected fax stream*/
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_FAX);

        if(ICF_NULL == p_sdp_list)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
             return ICF_FAILURE;
        }

        p_t38_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;

        p_t38_stream_info->stream_status &= ~ICF_MEDIA_STREAM_NW_ALLOW;
    }

    if(ICF_FAX_PT_CALL != p_cc_pdb->p_call_ctx->fax_type)
    {
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_FAX);

        if(ICF_NULL == p_sdp_list)
        {  
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ICF_FAILURE;
        }

        p_t38_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
    }

    switch(p_cc_pdb->p_call_ctx->fax_type)
    {
        case ICF_FAX_PT_CALL:
            if(ICF_LOCAL_SDP_CODEC_CHANGE == 
                    (ICF_LOCAL_SDP_CODEC_CHANGE & 
                     p_audio_stream_info->media_modify_type))
            {
                p_audio_stream_info->media_modify_type =
                    ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE;
            }
            else if(ICF_LOCAL_SDP_MEDIA_CHANGE == 
                    (ICF_LOCAL_SDP_MEDIA_CHANGE & 
                     p_audio_stream_info->media_modify_type))
            {
                p_audio_stream_info->media_modify_type =
                    ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE;
            }
            break;

        case ICF_T38_FAX_CALL:
            p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;

            p_audio_stream_info->stream_status &= (~(ICF_MEDIA_STREAM_CLINE_ZERO));

            icf_cc_calc_mode_remote_reject_change(p_cc_pdb);

            p_t38_stream_info->media_modify_type = ICF_STREAM_DELETE;

            break;

        case ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL:
            {
                p_audio_stream_info->stream_status &= (~(ICF_MEDIA_STREAM_CLINE_ZERO));
                icf_cc_process_fax_passthrough_req(p_cc_pdb);

                icf_cc_calc_mode_remote_reject_change(p_cc_pdb);

                p_t38_stream_info->media_modify_type = ICF_STREAM_DELETE;
              /* SPR 17399: M line for image with port 0 not sent in case of 
                 * fax_type 3.
                 */
                p_t38_stream_info->stream_status &=(~(ICF_MEDIA_STREAM_NW_ALLOW));

            }
            break;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;

    /* if fax_type = PT , revert
       if fax_type = T38, delete T38 and revert audio
       if fax_type = T38 failover, delete T38 and
       modify audio forG711
     */
}

#endif


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_call_modify_offer
 *
 * DESCRIPTION:     This function processes the remote call modify offer
 ******************************************************************************/
icf_return_t     icf_cc_process_remote_call_modify_offer(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_internal_list_st             *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st           *p_stream_sdp_info_data = ICF_NULL;

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

        icf_nw_inc_media_change_req_st *p_call_modify_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
    
    /* The common bitmask set for checking of Reinvite without sdp
       in future use of this transaction.set the modify media type
       through the function.Check if the session is stablished on
       FAX stream then return failure*/

   /*********** Changes for Re-Invite w/o SDP Start (Rel7.0) ***********/
    /* SPR 19188: SPR 18979: Reinvite wo SDP should be handled when Neither SDP list 
	 * not added / rejected list of reject bitmaks is present.
	 */
    if((ICF_NW_INC_MEDIA_CHG_SDP_PRESENT != ((p_call_modify_req->bitmask) &
                                  ICF_NW_INC_MEDIA_CHG_SDP_PRESENT)) && 
      (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM != ((p_call_modify_req->bitmask) &
                                  ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM)) &&
      (ICF_NW_INC_MEDIA_ADD_STREAM != ((p_call_modify_req->bitmask) &
                                  ICF_NW_INC_MEDIA_ADD_STREAM)) &&
	  (ICF_NW_INC_MEDIA_NW_REJECT_STREAM != (p_call_modify_req->bitmask & 
	                             ICF_NW_INC_MEDIA_NW_REJECT_STREAM)))
    {
       ICF_CC_SET_COMMON_BITMASK(p_cc_pdb, ICF_CALL_MOD_REQ_WO_SDP) 

       /* empty reinvite */

       if(ICF_NULL != p_cc_pdb->p_call_ctx->fax_type)
       {
          p_cc_pdb->p_call_ctx->offer_failure_reason =
                             ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

          return ret_val;
       }
       else    
       {
          ret_val = icf_cc_make_mms_type_for_remote_offer_change_wo_sdp(p_cc_pdb);

          if(ICF_FAILURE == ret_val)
          {
               ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: icf_cc_make_mms_type_for_remote_offer_change_wo_sdp returned Failure"));
          }    

          return ret_val;
       }

    }
    /* SPR 19188: SPR 18979: In case SDP list not present but reject list is present then
	 * reject the offer with 488.
	 */
    else if((ICF_NW_INC_MEDIA_CHG_SDP_PRESENT != ((p_call_modify_req->bitmask) &
                                  ICF_NW_INC_MEDIA_CHG_SDP_PRESENT)) && 
      ((ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM == ((p_call_modify_req->bitmask) &
                                  ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM)) ||
	  (ICF_NW_INC_MEDIA_NW_REJECT_STREAM == (p_call_modify_req->bitmask & 
	                             ICF_NW_INC_MEDIA_NW_REJECT_STREAM))))
    {
	    if(ICF_NW_INC_MEDIA_NW_REJECT_STREAM & p_call_modify_req->bitmask)
        {
             icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                        &(p_call_modify_req->p_nw_reject_stream_list),
                        ICF_MEM_COMMON);
        }
	    p_cc_pdb->p_call_ctx->offer_failure_reason = 
		                    ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;
		return ret_val;
	}

    /************ Changes for Re-Invite w/o SDP End (Rel7.0) ************/

    if(ICF_NW_INC_MEDIA_NW_REJECT_STREAM == (ICF_NW_INC_MEDIA_NW_REJECT_STREAM & p_call_modify_req->bitmask))
    {
        p_cc_pdb->p_list_1 = p_call_modify_req->p_nw_reject_stream_list;
        icf_cc_process_nw_rejected_stream_list(p_cc_pdb);
    }

        p_cc_pdb->p_list_1 = p_call_modify_req->p_sdp_list;

    if(ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM ==
            (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM & p_call_modify_req->bitmask))
    {
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

        if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
                (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_call_modify_req->bitmask))
        {
            icf_cc_delete_inc_media_sdp_info_list(
                    p_cc_pdb,&(p_call_modify_req->p_sdp_list));
        }
    }
    else if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
            (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_call_modify_req->bitmask))
    {
        if(ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
        {
            p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_INVALID_REQUEST;
        }
        else
        {
            ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);
        }
    }

    if(ICF_NULL != p_cc_pdb->p_call_ctx->offer_failure_reason)
    {
        /* spr 14476 */
        p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        while(ICF_NULL != p_stream_sdp_info_node)
        {
           p_stream_sdp_info_data = p_stream_sdp_info_node->p_payload;
           if(ICF_REMOTE_SDP_CODEC_CHANGE == 
                (p_stream_sdp_info_data->media_modify_type & ICF_REMOTE_SDP_CODEC_CHANGE))
           {
               p_cc_pdb->p_call_ctx->common_bitmask |= 
                   ICF_OFFER_CODECS_IN_GCL;
               break;
           }
           p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
        }

        /* if ICF_NO_LOCAL_CODEC_IN_GCL is set that means there are no codecs
	in the preferecd media profile so no need to send the local codecs in 
	the GCL request */
	if(p_cc_pdb->p_call_ctx->common_bitmask & ICF_NO_LOCAL_CODEC_IN_GCL)
	{
	    p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_OFFER_CODECS_IN_GCL;
	    p_cc_pdb->p_call_ctx->common_bitmask &= ~ICF_NO_LOCAL_CODEC_IN_GCL;
	}
     }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)
    /* Rel 8.1
     * if SDP is not received then set the mms type
     * as Local sdp required because in 200 OK
     * local SDP need to be sent 
     */
    if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT != ((p_call_modify_req->bitmask) &
                                  ICF_NW_INC_MEDIA_CHG_SDP_PRESENT))
    {
        p_stream_sdp_info_data =
                      (icf_stream_sdp_info_st *)
                      (p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload);
        p_stream_sdp_info_data->media_modify_type = ICF_NULL;
        p_stream_sdp_info_data->media_modify_type |=
                                 ICF_LOCAL_SDP_REQD;

    }
    else
    {
        p_cc_pdb->p_list_1 = p_call_modify_req->p_sdp_list;
        /* Rel 8.1
         * if SDP is received the copy it into remote received SDP
         */
        if(ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
        {
            p_cc_pdb->p_call_ctx->offer_failure_reason = 
                        ICF_CALL_MODIFY_REASON_INVALID_REQUEST;
        }
        else
        {
            /* Rel 8.1
             * if SDP is received then set the mms type
             * as Local sdp required and Tunnelled remote sdp
             * present because in 200 OK
             * local SDP need to be sent and remote sdp need to
             * convey to application.
             */
            p_stream_sdp_info_data =
                      (icf_stream_sdp_info_st *)
                      (p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_payload);
            p_stream_sdp_info_data->media_modify_type = ICF_NULL;
            p_stream_sdp_info_data->media_modify_type |=
                                 ICF_LOCAL_SDP_REQD;
            p_stream_sdp_info_data->media_modify_type |=
                                 ICF_TUNNELLED_REMOTE_SDP_PRESENT;
        }
    }
ICF_CHECK_TUNNEL_MODE_END
    return ret_val;
}

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_call_modify_resp
 *
 * DESCRIPTION:     This function processes the remote call modify success
 *                  response
 ******************************************************************************/
icf_return_t     icf_cc_process_remote_call_modify_resp(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_nw_inc_media_change_resp_st *p_call_modify_resp =
        (icf_nw_inc_media_change_resp_st *)
        p_cc_pdb->p_internal_msg->p_msg_data;

    p_cc_pdb->p_list_1 = p_call_modify_resp->p_sdp_list;

    ret_val = icf_cc_make_mms_type_for_nth_rem_succ_ans(p_cc_pdb);

    return ret_val;
}
#endif


#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_call_modify_err_resp
 *
 * DESCRIPTION:     This function processes the remote call modify error
 *                  response
 ******************************************************************************/
icf_return_t     icf_cc_process_remote_call_modify_err_resp(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    /* need to verify what to copy in the p_cc_pdb structure */
    ret_val = icf_cc_derive_mms_remote_rejected(p_cc_pdb);

    return ret_val;
}
#endif


#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_local_call_modify_err_resp
 *
 * DESCRIPTION:     This function processes the local call modify error
 *                  response
 ******************************************************************************/
icf_return_t     icf_cc_process_local_call_modify_err_resp(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    ret_val = icf_cc_derive_mms_type_for_revert_remote_change(p_cc_pdb);

    return ret_val;
}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_remote_sdp_in_all_streams
 *
 * DESCRIPTION:     This function checks in all the streams that remote SDP
 *                  is received or not
 *
 ******************************************************************************/
icf_return_t  icf_cc_check_remote_sdp_in_all_streams(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t             ret_val = ICF_FAILURE;
    icf_internal_list_st     *p_list = ICF_NULL;
    icf_stream_sdp_info_st   *p_stream_sdp = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    if (ICF_NULL != p_list)
    {
        while (ICF_NULL != p_list)
        {
            p_stream_sdp = p_list->p_payload;

            if (ICF_NULL != p_stream_sdp->p_remote_received_sdp)
            {
                ret_val = ICF_SUCCESS;
                break;
            }

            p_list = p_list->p_next;
        }
    }

    return ret_val;
}

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_mm_applied_mode_to_rem_mode
 *
 * DESCRIPTION:     This function copies remote_mode from remote_received_sdp
 *                  to mm_applied_mode for that stream
 ******************************************************************************/
icf_void_t     icf_cc_copy_mm_applied_mode_to_rem_mode(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;
    icf_internal_list_st        *p_list = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_sdp =
            (icf_stream_sdp_info_st*)(p_list->p_payload);

        p_sdp->p_remote_received_sdp->media_mode =
            p_sdp->mm_applied_mode;

        p_list = (icf_internal_list_st *)p_list->p_next;
    }
}

#endif

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_memfree_internal_sdp_stream
 *
 * DESCRIPTION:     This function frees internal SDP stream
 *
 ******************************************************************************/
icf_return_t   icf_cc_memfree_internal_sdp_stream(
        icf_glb_pdb_st                *p_glb_pdb,
        icf_internal_sdp_stream_st    *p_int_sdp)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_error_t      ecode = ICF_ERROR_NONE;

    if (ICF_SDP_STREAM_ENCPARAMS_PRESENT & p_int_sdp->sdp_stream.bit_mask)
    {
        ret_val = icf_cmn_delete_list(p_glb_pdb,
                &(p_int_sdp->sdp_stream.p_encoded_attributes),
                ICF_MEM_COMMON);
    }

    /*
       if (ICF_NULL != p_int_sdp->sdp_stream.p_media_streaim_param)

       ICF_MEMFREE(p_glb_pdb,
       (icf_void_t *)(p_int_sdp->sdp_stream.p_media_stream_param),
       ICF_MEM_COMMON,
       &ecode,
       ret_val)*/

    icf_dbm_ret_mem_to_pool(p_glb_pdb, ICF_MEM_SDP, p_int_sdp, &ecode);

    return ret_val;
}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_clear_all_stream_sdp
 *
 * DESCRIPTION:     This function to free all memory related to all streams
 *
 ******************************************************************************/

icf_return_t   icf_cc_clear_all_stream_sdp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_return_t    dummy_ret_val = ICF_SUCCESS;
    icf_internal_list_st        *p_list = ICF_NULL;
    icf_internal_list_st        *p_media_sdp_list = ICF_NULL;
    icf_inc_media_sdp_info_st   *p_media_sdp = ICF_NULL;
    
    if((ICF_NULL == p_cc_pdb) || (ICF_NULL == p_cc_pdb->p_glb_pdb))
    {
        ICF_PRINT(((icf_uint8_t*)"\n[CC]: Pdb or Glb Pdb is NULL"))
        return ICF_FAILURE;
    }
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
             p_cc_pdb->p_call_ctx, dummy_ret_val)
    if(ICF_FAILURE == dummy_ret_val)
    {
       return dummy_ret_val;
    }
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    ret_val = icf_cmn_free_stream_sdp_info_list(p_cc_pdb->p_glb_pdb,
            &p_list,
            p_cc_pdb->p_ecode);

    p_cc_pdb->p_call_ctx->p_stream_sdp_info = ICF_NULL;

    if (ICF_NULL != p_cc_pdb->p_call_ctx->mmi_context.p_remote_recvd_sdp)
    {
        /*SPR 19450 The tunnelled SDP memory inside the remote_recv_sdp
          is already freed by icf_cmn_free_stream_sdp_info_list which
          internally invokes icf_cmn_delete_internal_sdp_stream 
          but the pointer in mmi context for tunnelled SDP is not reset
          therefore before invocation to icf_cmn_free_media_sdp_info_internal_list
          we shall be initializing the p_tunnelled_sdp to avoid attempt
          to free the memory again*/
        p_media_sdp_list =
            p_cc_pdb->p_call_ctx->mmi_context.p_remote_recvd_sdp;
        p_media_sdp = 
			(icf_inc_media_sdp_info_st *)(p_media_sdp_list->p_payload);
        p_media_sdp->p_int_sdp_stream->p_tunnelled_sdp = ICF_NULL;

        icf_cmn_free_media_sdp_info_internal_list(p_cc_pdb->p_glb_pdb,
                &p_cc_pdb->p_call_ctx->mmi_context.p_remote_recvd_sdp,
                p_cc_pdb->p_ecode);
    }

    p_cc_pdb->p_call_ctx->mmi_context.p_remote_recvd_sdp = ICF_NULL;

    if (ICF_NULL != p_cc_pdb->p_call_ctx->mmi_context.p_mms_req)
    {
        icf_mmi_mms_request_st	*p_mms_req = ICF_NULL;

        p_mms_req = (icf_mmi_mms_request_st *)
            (p_cc_pdb->p_call_ctx->mmi_context.p_mms_req);

        if (ICF_MMI_MMS_ACTION_ADD & p_mms_req->bit_mask)
        {
            ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                    p_mms_req->p_add_media,
                    ICF_MEM_COMMON, 
                    p_cc_pdb->p_ecode, 
                    ret_val)
        }

        if (ICF_MMI_MMS_ACTION_DELETE & p_mms_req->bit_mask)
        {
            ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                    p_mms_req->p_delete_media,
                    ICF_MEM_COMMON, 
                    p_cc_pdb->p_ecode, 
                    ret_val)
        }

        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                p_cc_pdb->p_call_ctx->mmi_context.p_mms_req,
                ICF_MEM_COMMON, 
                p_cc_pdb->p_ecode, 
                ret_val)
    }

    if (ICF_NULL != p_cc_pdb->p_call_ctx->mmi_context.p_mmi_resp)
    {
        /* MEMFREE p_mmi_pdb->p_mmi_ctx->p_mmi_resp */
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
                p_cc_pdb->p_call_ctx->mmi_context.p_mmi_resp,
                ICF_MEM_COMMON, 
                p_cc_pdb->p_ecode, 
                ret_val)
    }


    /*SPR 13998*/
    p_cc_pdb->p_call_ctx->call_type = ICF_NULL;
    return ret_val;
}


#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_stream_sdp_info
 *
 * DESCRIPTION:     This function frees stream SDP info
 *
 ******************************************************************************/
icf_return_t    icf_cc_free_stream_sdp_info(
        icf_cc_pdb_st          *p_cc_pdb,
        icf_stream_sdp_info_st *p_sdp)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    if (ICF_NULL != p_sdp->p_remote_received_sdp)
    {
        ret_val = icf_cc_memfree_internal_sdp_stream(
                p_cc_pdb->p_glb_pdb,
                p_sdp->p_remote_received_sdp);
    }

    if (ICF_NULL != p_sdp->p_local_sdp)
    {
        ret_val = icf_cc_memfree_internal_sdp_stream(
                p_cc_pdb->p_glb_pdb,
                p_sdp->p_local_sdp);
    }

    if (ICF_NULL != p_sdp->p_applied_sdp)
    {
        ret_val = icf_cc_memfree_internal_sdp_stream(
                p_cc_pdb->p_glb_pdb,
                p_sdp->p_applied_sdp);
    }

    if (ICF_NULL != p_sdp->p_last_commited_sdp)
    {
        ret_val = icf_cc_memfree_internal_sdp_stream(
                p_cc_pdb->p_glb_pdb,
                p_sdp->p_last_commited_sdp);
    }
#if 0 /* this should not be done here */
    ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
            (icf_void_t *)(p_sdp),
            ICF_MEM_COMMON,
            p_cc_pdb->p_ecode,
            ret_val)
#endif
        return ret_val;
}
#endif

#if 0
icf_void_t     icf_cc_free_inc_media_sdp(
        INOUT  icf_cc_pdb_st  *p_cc_pdb,
        INOUT  icf_list_st    *p_list)
{
    icf_return_t                 ret_val = ICF_SUCCESS;
    icf_inc_media_sdp_info_st    *p_nw_inc_sdp = ICF_NULL;

    while(ICF_NULL != p_list)
    {
        p_nw_inc_sdp = p_list->p_data;

        icf_cc_memfree_internal_sdp_stream(
                p_cc_pdb->p_glb_pdb,
                p_nw_inc_sdp->p_int_sdp_stream);

        p_list = p_list->p_next;
    }

    ret_val = icf_cmn_delete_list(p_cc_pdb->p_glb_pdb, &p_list, ICF_MEM_COMMON);
}
#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_mm_applied_mode
 *
 * DESCRIPTION:     This function copies remote_mode from remote_received_sdp
 *                  to mm_applied_mode for that stream
 ******************************************************************************/
icf_void_t     icf_cc_set_mm_and_app_mode(
        INOUT   icf_cc_pdb_st     *p_cc_pdb,
        IN      icf_media_mode_t  media_mode)
{
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;
    icf_internal_list_st        *p_list = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_sdp =
            (icf_stream_sdp_info_st*)(p_list->p_payload);
        if(!(ICF_MEDIA_STREAM_ACTIVE & p_sdp->stream_status))
        {
          p_list = (icf_internal_list_st *)p_list->p_next;
          continue;
        }

        if(ICF_CALL_DIR == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR))
        {
            /* For incoming call derive media mode assuming application mode
             * as inactive */
            p_sdp->application_mode = media_mode;

            if (ICF_CALL_MEDIA_SETUP_TYPE == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_MEDIA_SETUP_TYPE))
            {
                /* It is a FS call.*/
                p_cc_pdb->remote_mode = p_sdp->p_remote_received_sdp->media_mode;
                p_cc_pdb->application_mode = p_sdp->application_mode;
                icf_cc_derive_mode(p_cc_pdb);
                p_sdp->mm_applied_mode = p_cc_pdb->derived_mode;
            }
            else
            {
                /* It is not fast start call. remote sdp is not avilable */
                p_sdp->mm_applied_mode = ICF_MEDIA_MODE_INACTIVE;
            }
        }
        else
        {
            p_sdp->application_mode = ICF_MEDIA_MODE_SEND_RECV;

            p_sdp->mm_applied_mode = media_mode;
        }
        p_list = (icf_internal_list_st *)p_list->p_next;
    }
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_local_sdp_media_mode
 *
 * DESCRIPTION:     This function copies given mode in local_sdp
 ******************************************************************************/
icf_void_t     icf_cc_set_local_sdp_media_mode(
        INOUT   icf_cc_pdb_st     *p_cc_pdb,
        IN      icf_media_mode_t  media_mode)
{
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;
    icf_internal_list_st        *p_list = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_sdp =
            (icf_stream_sdp_info_st*)(p_list->p_payload);

        if (ICF_NULL != p_sdp->p_local_sdp)
            p_sdp->p_local_sdp->media_mode = media_mode;

        p_list = (icf_internal_list_st *)p_list->p_next;
    }
}


#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_preferred_profile_with_app_media_profile
 *
 * DESCRIPTION:     This function compares preferred media profile with
 *                  media profile received in app_add
 *
 ******************************************************************************/
icf_return_t   icf_cc_check_preferred_profile_with_app_media_profile(
        icf_list_st    *p_pref_profile,
        icf_list_st    *p_app_profile)
{
    return ICF_SUCCESS;
}


icf_return_t    icf_cc_revert_remote_sdp_stream(
        INOUT icf_cc_pdb_st  *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    return ret_val;
}


icf_void_t   icf_cc_copy_rem_mode_to_mm_applied_mode(
        INOUT   icf_cc_pdb_st    *p_cc_pdb)
{
}



icf_void_t   icf_cc_copy_applied_mode_to_rem_mode(
        INOUT   icf_cc_pdb_st    *p_cc_pdb)
{
}


icf_return_t   icf_cc_check_if_remote_received_sdp_present(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;

    return ret_val;

}
#endif



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_ith_level_m_line_check
 *
 * DESCRIPTION:     This function shall validate the ith level
 *                  check for the mlines received from the network
 *                  and also copy the SDP streams into the call context
 ******************************************************************************/
icf_return_t icf_cc_validate_ith_level_m_line_check(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_return_t                    ret_value = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_list_st                    *p_rem_recvd_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;
    icf_inc_media_sdp_info_st      *p_nw_inc_sdp =ICF_NULL;
    icf_uint8_t                     virtual_ctr = 1;
    
    if(ICF_NULL == p_cc_pdb->p_glb_pdb) 
    {
        ICF_PRINT(((icf_uint8_t *)"[CC]:GLB PDB is null"));
        return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */
    /*Start SPR 19015 */    
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
            p_cc_pdb->p_call_ctx, ret_value)
    if(ICF_FAILURE == ret_value)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ICF_FAILURE;    
    }/*End SPR 19015 */

    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    p_rem_recvd_list = p_cc_pdb->p_list_1;

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_list))
        /* Check the stream id corresponding to virtual counter in the
         * in the SDP received from SSA and if the stream types don't
         * match ,then return failure.If the corresponding stream is not
         * received in the SDP from SSA,it means that the peer has
         * rejected that stream in the offer. In this case ,delete this stream
         */
        while(ICF_NULL != p_cc_list)
        {
            p_stream_sdp_info = p_cc_list->p_payload;
            /*Start SPR 19015 */    
            ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                         p_stream_sdp_info, ret_value)
            if(ICF_FAILURE == ret_value)
            {
               ret_val = ICF_FAILURE;
               ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
               return ICF_FAILURE;    
            }/*End SPR 19015 */
            /* Compare the streams only if it is nw_allowed */
            /* In tunnel mode, we will not check for the stream status */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)            
            if (ICF_MEDIA_STREAM_NW_ALLOW ==
                    (ICF_MEDIA_STREAM_NW_ALLOW &
                     p_stream_sdp_info->stream_status))
            {
                if (ICF_NULL != p_rem_recvd_list)
                {
                    p_nw_inc_sdp = p_rem_recvd_list->p_data;
                    /*Start SPR 19015 */    
                    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                         p_nw_inc_sdp, ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                       ret_val = ICF_FAILURE;
                       ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                       return ICF_FAILURE;    
                    }/*End SPR 19015 */
                    if ((virtual_ctr == p_nw_inc_sdp->stream_id) &&
                            (p_stream_sdp_info->stream_type ==
                             p_nw_inc_sdp->stream_type))
                    {
                        /*
                         * Copy if stream status is active and stream_active is true
                         */
                        if ((ICF_MEDIA_STREAM_ACTIVE == (
                                        p_stream_sdp_info->stream_status &
                                        ICF_MEDIA_STREAM_ACTIVE)) &&
                                (ICF_TRUE ==p_nw_inc_sdp->stream_active))
                        {
                            p_cc_pdb->p_int_sdp_stream = 
                                p_nw_inc_sdp->p_int_sdp_stream;

                            p_nw_inc_sdp->p_int_sdp_stream = ICF_NULL;
                            p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
                            ret_val = icf_cc_copy_int_sdp_stream_to_remote(p_cc_pdb);
                            if(ICF_FAILURE == ret_val)
                            {
                                 ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: icf_cc_copy_int_sdp_stream_to_remote\
                                           returned Failure "));
                                 p_cc_pdb->common_bitmask |=
                                         ICF_CC_STREAM_POS_VALIDATION_FAIL;
                                 break;
                            }
                        }
                        /*
                         * If stream_active is false, and stream status is active
                         * mark it as ICF_STREAM_DELETE
                         */
                        else if((ICF_MEDIA_STREAM_ACTIVE == (
                                        p_stream_sdp_info->stream_status &
                                        ICF_MEDIA_STREAM_ACTIVE)) &&
                                (ICF_FALSE ==p_nw_inc_sdp->stream_active))
                        {
                            /* Mark the media_modify_type as ICF_STREAM_DELETE */
                          p_stream_sdp_info->media_modify_type |=
                                ICF_STREAM_DELETE;
              
                             /* SPR 19999 - Fix for CSR 1-7643774 */
                            /*  If peer has deleted any stream them copy the
                             *  codec list */
                            if(ICF_FAILURE ==
                                    icf_cmn_clone_codec_attrib_list(
                                        p_cc_pdb->p_glb_pdb, 
                                        &(p_stream_sdp_info->p_remote_recvd_codec_list),
                                        p_nw_inc_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list))
                            {
                                ret_val = ICF_FAILURE;
                            }
 
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
                            if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR)))
                            {
                                /******** Changes for Forking & Multiple Answer Start 
                                 (Rel7.0) ********/
                                if(ICF_CC_SSA_CALL_CONNECT != p_cc_pdb->current_event)
                                {
                                   /* SPR 18979: Created empty node in forked response list, only when the
                                    * call is not connected.
                                    */ 
                                   if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE))
                                   {
                                      p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
 
                                      ret_val = icf_cc_create_empty_node_forked_resp(p_cc_pdb);
                                      if(ICF_FAILURE == ret_val)
                                      {
                                         ICF_PRINT(((icf_uint8_t*)"\n[CC]: icf_cc_create_empty_node_forked_resp returned Failure "));
                                         p_cc_pdb->common_bitmask |=
                                            ICF_CC_STREAM_POS_VALIDATION_FAIL;
                                         break;
                                      }
                                    }
                                 }
                                 /******** Changes for Forking & Multiple Answer End 
                                 (Rel7.0) ********/
                            }
ICF_CHECK_MULT_ANS_ENBL_END
 
                        }
                        else if(ICF_MEDIA_STREAM_ACTIVE != (
                            p_stream_sdp_info->stream_status &
                            ICF_MEDIA_STREAM_ACTIVE))
                        {
                            /* SPR 19999 - Fix for CSR 1-7643774 */
                            /*  If peer has changed the codecs in a stream
                             *  that is inactive the copy the codec list. 
                             */
                            if(ICF_FAILURE ==
                                    icf_cmn_clone_codec_attrib_list(
                                        p_cc_pdb->p_glb_pdb,
                                        &(p_stream_sdp_info->p_remote_recvd_codec_list),
                                        p_nw_inc_sdp->p_int_sdp_stream->sdp_stream.p_codec_info_list))
                            {
                                ret_val = ICF_FAILURE;
                            }
                        }
                    }
                    else
                    {
                        p_cc_pdb->p_generic_ptr_2 = p_stream_sdp_info;

                        ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_ITH_LEVEL_VALIDATION_FAILED)
                            /* Raise alarm */
                            ret_val = ICF_FAILURE;
                        p_cc_pdb->common_bitmask |= 
                            ICF_CC_STREAM_POS_VALIDATION_FAIL;
                        break ;
                    }
                    p_rem_recvd_list = p_rem_recvd_list->p_next;
                }
                else
                {
                    /* This means that the peer has deleted an mline
                     * in the offer/answer .If this particular stream
                     * is active,then mark the same for deletion,else
                     * ignore
                     */
                    if (ICF_MEDIA_STREAM_ACTIVE == (ICF_MEDIA_STREAM_ACTIVE &
                                p_stream_sdp_info->stream_status))
                    {
                        /* Mark the media_modify_type as ICF_STREAM_DELETE */
                        p_stream_sdp_info->media_modify_type |=
                            ICF_STREAM_DELETE;
                    }
                }
                virtual_ctr++;
            }
ICF_CHECK_TUNNEL_MODE_END
            /* In tunnel mode, we will just copy the SDP received into 
               remote received SDP of the call context.
            */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
           if (ICF_NULL != p_rem_recvd_list)
           {
                p_nw_inc_sdp = p_rem_recvd_list->p_data;
                    
                ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, 
                                         p_nw_inc_sdp,
                                         ret_value)
                if(ICF_FAILURE == ret_value)
                {
                    /* Even if the failure is due to p_nw_inc_sdp being NULL, we will
                       set the following bit. In tunnel mode, failure from this function
                       due to any reason is considered equivalent. The calling function
                       takes decisions on the basis of the bit ICF_CC_STREAM_POS_VALIDATION_FAIL
                       in CC PDB.
                    */
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    /* Freeing the memory acquired for the incoming SDP */
                    icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ret_val;    
                }
                p_cc_pdb->p_int_sdp_stream = p_nw_inc_sdp->p_int_sdp_stream;
                p_nw_inc_sdp->p_int_sdp_stream = ICF_NULL;
                p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
                ret_val = icf_cc_copy_int_sdp_stream_to_remote(p_cc_pdb);
                if(ICF_FAILURE == ret_val)
                {
                    ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: \
                                icf_cc_copy_int_sdp_stream_to_remote\
                                returned Failure "));
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    /* 
                    Freeing internal SDP stream, the following pointer
                    assignment is done so that the function icf_cc_delete_inc_media_sdp_info_list
                    frees the memory for the incoming SDP received.
                    */
                    p_nw_inc_sdp->p_int_sdp_stream = p_cc_pdb->p_int_sdp_stream;
                    break;
                }
                p_rem_recvd_list = p_rem_recvd_list->p_next;
            }
ICF_CHECK_TUNNEL_MODE_END   
            p_cc_list = p_cc_list->p_next;
        }

    icf_cc_delete_inc_media_sdp_info_list(p_cc_pdb,&p_cc_pdb->p_list_1);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function entry trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_int_sdp_stream_to_remote
 *
 * DESCRIPTION:     This function shall delete the memory for the
 * 		    p_remote_received_sdp member of the p_stream_sdp_info
 * 		    passed in the PDB and assign the p_int_sdp_stream ptr
 * 		    in the PDB to the same
 ******************************************************************************/
icf_return_t icf_cc_copy_int_sdp_stream_to_remote(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;
    icf_forked_resp_data_st *p_payload = ICF_NULL;
    icf_list_st *p_node_returned = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
    icf_internal_sdp_stream_st* p_old_sdp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
    
    if (ICF_NULL != p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp)
    {
          icf_cmn_delete_internal_sdp_stream(
                    p_cc_pdb->p_glb_pdb,
                    &p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp,
                    p_cc_pdb->p_ecode);
    }
    p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp =
             p_cc_pdb->p_int_sdp_stream;

ICF_CHECK_MULT_ANS_DISBL_END

ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)

    if(ICF_CALL_CONNECT_DONE != (p_cc_pdb->p_call_ctx->common_bitmask 
             & ICF_CALL_CONNECT_DONE))
    {
         if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,ICF_CALL_DIR)))
         {
            /*********** Changes for Forking & Multiple Answer Start (Rel7.0) ***********/

    /* Checks if it is a forked response */

    p_stream_sdp_info = p_cc_pdb->p_stream_sdp_info;
               
    /* The function below returns the node (if available) based on the identifier else 
      returns NULL */
         
    p_node_returned = icf_cmn_get_node_in_list(p_stream_sdp_info->p_forked_resp_list,
                                p_cc_pdb->forked_resp_id);
    
    /* Checks if the node returned based on the identifier is NULL and if so adds 
      a new node */

    if(ICF_NULL == p_node_returned)
    {
        ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: p_node_returned is ICF_NULL"));
        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_forked_resp_data_st),
                   ICF_MEM_COMMON,
                   p_payload,
                   ICF_DONOT_RET_ON_FAILURE,
                   p_cc_pdb->p_ecode,
                   ret_val)
        /*klocwork warning removal*/ 
        ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,p_payload);

        if(ICF_SUCCESS == ret_val)
        {
              icf_return_t dummy_ret_val = ICF_SUCCESS;

              if (ICF_NULL == p_payload)
              {
                 return ICF_FAILURE;
              }
              p_payload->p_sdp_info = p_cc_pdb->p_int_sdp_stream;
              p_payload->forked_resp_id = p_cc_pdb->forked_resp_id;

              ret_val = icf_cmn_add_node_to_list(p_cc_pdb->p_glb_pdb,
                         &p_stream_sdp_info->p_forked_resp_list,
                         p_payload);

              if(ICF_FAILURE == ret_val)
              {
                    ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: icf_cmn_add_node_to_list returned Failure "));
                    ICF_MEMFREE(p_cc_pdb->p_glb_pdb, 
						p_payload, 
						ICF_MEM_COMMON, 
						p_cc_pdb->p_ecode, 
						dummy_ret_val) 
              }
        }
        else
        {
              ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: ICF_MEMGET in icf_cc_copy_int_sdp_stream_to_remote returned Failure "));
        }
 
    }

    /* if the node returned based on the identifier is not NULL, replaces the earlier
      data with the new one */

    else
    {
         ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: p_node_returned is not NULL"));
         /* delete the existing payload of the node returned*/

         p_old_sdp = ((icf_forked_resp_data_st *)
                           (p_node_returned->p_data))->p_sdp_info;

         ((icf_forked_resp_data_st *)(p_node_returned->p_data))->p_sdp_info = 
                             p_cc_pdb->p_int_sdp_stream;
         ((icf_forked_resp_data_st *)(p_node_returned->p_data))->\
                             forked_resp_id = p_cc_pdb->forked_resp_id;

		/* Fix for CSR 1-6767492: SPR 19251 */
		/* Before freeing the SDP pointer stored in forked_resp_list, check if
		 * the same SDP pointer is also stored in p_stream_sdp_info. If yes,
		 * reset the cross-reference in p_stream_sdp_info to NULL.
		 */
		if (p_old_sdp == p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp)
		{
			p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp = ICF_NULL;
		}
         
         /* delete the p_sdp_info */
         ret_val = icf_cmn_delete_internal_sdp_stream(
                    p_cc_pdb->p_glb_pdb,
                    &p_old_sdp,
                    p_cc_pdb->p_ecode);
        
         icf_dbm_ret_mem_to_pool(
                                 p_cc_pdb->p_glb_pdb,
                                 ICF_MEM_SDP,
                                 p_old_sdp,
                                 p_cc_pdb->p_ecode);
 
         if(ICF_FAILURE == ret_val)
         {
                ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: ICF_MEMGET in icf_cc_copy_int_sdp_stream_to_remote returned Failure "));
         }
     }
     
        p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp =
                   p_cc_pdb->p_int_sdp_stream;     
     }
     else
     {
         if (ICF_NULL != p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp)
         {
             /* Fix for CSR 1-6767492: SPR 19251 */
             /* Remove any cross references stored for remote sdp in forked 
              * response list.
              */ 
             if (ICF_SUCCESS == icf_cc_free_forked_resp_list_node_for_sdp(
                         p_cc_pdb,
                         p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp))
             {
                 /* Now delete the remote received sdp */             
                 icf_cmn_delete_internal_sdp_stream(
                         p_cc_pdb->p_glb_pdb,
                         &p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp,
                         p_cc_pdb->p_ecode);
             }
         }
 
        p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp =
                   p_cc_pdb->p_int_sdp_stream;
      }
    }
     else
     {
         if (ICF_NULL != p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp)
         {
             /* Fix for CSR 1-6767492: SPR 19251 */
             /* Remove any cross references stored for remote sdp in forked 
              * response list.
              */ 
             if (ICF_SUCCESS == icf_cc_free_forked_resp_list_node_for_sdp(
                         p_cc_pdb,
                         p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp))
             {
                 /* Now delete the remote received sdp */             
                 icf_cmn_delete_internal_sdp_stream(
                         p_cc_pdb->p_glb_pdb,
                         &p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp,
                         p_cc_pdb->p_ecode);
             }
         }
         p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp =
             p_cc_pdb->p_int_sdp_stream;
     }


     /*********** Changes for Forking & Multiple Answer End (Rel7.0) ***********/

ICF_CHECK_MULT_ANS_ENBL_END
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_no_of_nodes_in_list
 *
 * DESCRIPTION:    This function shall return failure if number of nodes present in
 * 		   the  list passed as argument and the one present in the CC PDB
 * 		   are different.Internally it calls icf_cmn_get_list_count
 ******************************************************************************/
icf_boolean_t icf_cc_compare_no_of_nodes_in_list(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_uint8_t                     list_count1 = ICF_NULL;
    icf_uint8_t                     list_count2 = ICF_NULL;

    icf_cmn_get_list_count(p_cc_pdb->p_list_1,&list_count1);
    icf_cmn_get_list_count(p_cc_pdb->p_list_2,&list_count2);

    if (list_count1 != list_count2)
    {
        return ICF_FALSE;
    }
    else
    {
        return ICF_TRUE;
    }
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_media_modify_type_for_all_streams
 * DESCRIPTION:     This function resets media_modify_type bitmask for all 
 *					streams
 *
 ******************************************************************************/
icf_return_t icf_cc_reset_media_modify_type_for_all_streams(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;


    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_list))

        ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        while(ICF_NULL != p_cc_list)
        {
            p_stream_sdp_info = p_cc_list->p_payload;
ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
            if ((ICF_STREAM_DELETE == (p_stream_sdp_info->media_modify_type & 
                                ICF_STREAM_DELETE)) || (ICF_STREAM_PENDING_DELETE 
                                == (p_stream_sdp_info->media_modify_type & 
                                ICF_STREAM_PENDING_DELETE)));
            else
            { 
                  p_stream_sdp_info->media_modify_type = ICF_NULL;
            }
ICF_CHECK_MULT_ANS_ENBL_END
ICF_CHECK_MULT_ANS_DISBL_START(p_cc_pdb->p_glb_pdb)
            p_stream_sdp_info->media_modify_type = ICF_NULL;
ICF_CHECK_MULT_ANS_DISBL_END
            p_cc_list = p_cc_list->p_next;
        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_remote_offer_change
 * DESCRIPTION:     This function sets the media_modify_type bitmask which
 *		    shall indicate which fields of the SDP have changed
 *		    and shall aid the icf_cc_send_mms_req for populating the
 *		    payload for MMS request sent to MMI.
 *
 ******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_remote_offer_change(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;
    icf_list_st                    *p_pref_profile_node = ICF_NULL;
    icf_stream_capab_st            *p_pref_profile_data = ICF_NULL;


    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_list))

        ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */


        /*
         * The added(FAX) and deleted streams have already been checked
         * and the media_modify_type corresponding to those streams
         * have been populated.Just compare the codecs,mode and
         * other SDP attributes.
         */

        while(ICF_NULL != p_cc_list)
        {
            p_stream_sdp_info = p_cc_list->p_payload;



            /* Compare the streams only if it is nw_allowed and active*/
            if ((ICF_MEDIA_STREAM_NW_ALLOW ==
                        (ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp_info->stream_status)) &&
                    (ICF_MEDIA_STREAM_ACTIVE ==
                     (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp_info->stream_status)))

            {
                ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)
        
               /***** Changes for Forking & Multiple Answer Start (Rel7.0) *****/
                /* if stream is asked for deletion , mark it pending
                   do not delete now, it will be on the basis of final sdp */

                if ( ICF_STREAM_DELETE ==
                        (p_stream_sdp_info->media_modify_type & 
                                ICF_STREAM_DELETE)
                    )
                {
                   if(ICF_CC_SSA_CALL_CONNECT != p_cc_pdb->current_event)
                   {
                        /* SPR 19999 - In case PRACK is received with offer that has
                         * stream deleted then the media stream should be deleted and it
                         * should not be marked as PENDING DELETE. 
                         */
                        if((ICF_CC_ALERT_PRACK == p_cc_pdb->current_event) ||
                           (ICF_CC_MEDIA_PARAM_PRACK == p_cc_pdb->current_event))
                        {
                            p_stream_sdp_info->media_modify_type &= 
                             ~(ICF_STREAM_PENDING_DELETE);
                        }
                       /* SPR 19188: SPR 19140: Mark the steram as pending delete only when call is
                        * not connected and multiple answers are being processed in forking scenario.
                        */
                       else if(ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_CONNECT_DONE))
                       {

                           ICF_PRINT(((icf_uint8_t*)"\n[CC]: media_modify_type marked ICF_STREAM_PENDING_DELETE"));

                           p_stream_sdp_info->media_modify_type |=
                               ICF_STREAM_PENDING_DELETE;

                           p_stream_sdp_info->media_modify_type &= 
                               ~(ICF_STREAM_DELETE);
                        }
                   }
	             else if(ICF_STREAM_PENDING_DELETE ==
                        (p_stream_sdp_info->media_modify_type & 
                                 ICF_STREAM_PENDING_DELETE)
                   )
                   {
                        ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:ICF_STREAM_PENDING_DELETE reset"));

                        p_stream_sdp_info->media_modify_type &= 
                             ~(ICF_STREAM_PENDING_DELETE);
                   }
                }
                
                else if(ICF_STREAM_PENDING_DELETE == (p_stream_sdp_info->media_modify_type &
                           ICF_STREAM_PENDING_DELETE)) 
                {
                        if(ICF_NULL == p_stream_sdp_info->p_applied_sdp)
                        {
                              p_stream_sdp_info->media_modify_type |=
                              ICF_REMOTE_ANSWER_CODEC_CHANGE;
                        }
                        else
                        {
                              p_stream_sdp_info->media_modify_type |=
                              ICF_REMOTE_SDP_CODEC_CHANGE;
                        }
                        p_stream_sdp_info->media_modify_type &= 
                             ~(ICF_STREAM_PENDING_DELETE);
                }

                if(ICF_CALL_CONNECT_DONE == 
                    (p_cc_pdb->p_call_ctx->common_bitmask & ICF_CALL_CONNECT_DONE))
                {
                      /* Reset the ICF_STREAM_PENDING_DELETE bitmask if active in
                         the new offer */

                       if(ICF_STREAM_PENDING_DELETE == (ICF_STREAM_PENDING_DELETE
                                      & p_stream_sdp_info->media_modify_type))
                       {
                            ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:ICF_STREAM_PENDING_DELETE reset"));
                            p_stream_sdp_info->media_modify_type &= ~(ICF_STREAM_PENDING_DELETE);
                       } 
                 }

               /***** Changes for Forking & Multiple Answer End (Rel7.0) *****/

               ICF_CHECK_MULT_ANS_ENBL_END
 
                /* If media_modify_type is set to ICF_STREAM_ADD
                 * or ICF_STREAM_DELETE,don't compare the SDP
                 * stream
                 */
                if ((ICF_NULL == (ICF_STREAM_ADD & 
                                p_stream_sdp_info->media_modify_type)) &&
                   (ICF_NULL == (ICF_STREAM_DELETE &
                                      p_stream_sdp_info->media_modify_type)) &&
                   (ICF_NULL == (ICF_STREAM_PENDING_DELETE &
                                      p_stream_sdp_info->media_modify_type)) &&
                   (ICF_NULL == (ICF_REMOTE_ANSWER_CODEC_CHANGE &
                                      p_stream_sdp_info->media_modify_type)))
                {
                    if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb, 
                                p_stream_sdp_info->p_applied_sdp,
                                &(p_stream_sdp_info->p_last_commited_sdp)))
                    {
                        ret_val = ICF_FAILURE;
                        break;
                    }
                    /*
                     * Now invoke a function which shall compare the 
                     * p_remote_received_sdp
                     * and the p_applied_sdp and populate the media_modify_type
                     */
                    p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
                    /* here call function to set remote hold status */
                    icf_cc_set_remote_call_hold_status(p_cc_pdb);
                    icf_cc_compare_sdp_n_populate_mod_fileds(p_cc_pdb);

                    /* Fix for spr 14476.*/
                  if (ICF_REMOTE_SDP_CODEC_CHANGE == 
                            (p_stream_sdp_info->media_modify_type & 
                                             ICF_REMOTE_SDP_CODEC_CHANGE))
                    {
                        if (ICF_NULL != p_stream_sdp_info->p_offered_sdp)
                        {
                            if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                                        p_cc_pdb->p_glb_pdb,
                                        &(p_stream_sdp_info->p_local_sdp->
                                                      sdp_stream.p_codec_info_list),
                                        p_stream_sdp_info->p_offered_sdp->
                                                        sdp_stream.p_codec_info_list))
                            {
                                ret_val = ICF_FAILURE;
                                break;
                            }
                            /*Klocwork Warning Removal*/
                            ICF_KLOCWORK_FALSE_POSITIVE(
                                     p_cc_pdb->p_glb_pdb,\
                                     p_stream_sdp_info->p_local_sdp->\
                                     sdp_stream.p_codec_info_list)
                        }
                        else if (ICF_NULL != p_stream_sdp_info->p_local_sdp)
                        {
                            if (ICF_NULL ==  
                                    (p_pref_profile_node = 
                                       icf_cmn_get_node_from_stream_capab_list(		
                                            p_cc_pdb->p_call_ctx->p_pref_media_profile,
                                            p_stream_sdp_info->stream_type)))
                            {
                                 ret_val = ICF_SUCCESS;
                                 p_cc_pdb->p_call_ctx->common_bitmask |= ICF_NO_LOCAL_CODEC_IN_GCL; 
                            }
                            else
                            {
                                p_pref_profile_data = p_pref_profile_node->p_data;
                                if (ICF_STREAM_CAPAB_CODEC_INFO_PRESENT == 
                                        (ICF_STREAM_CAPAB_CODEC_INFO_PRESENT & 
                                             p_pref_profile_data->bit_mask))
                                { 
                                    if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                                                p_cc_pdb->p_glb_pdb,
                                                &(p_stream_sdp_info->p_local_sdp->
                                                     sdp_stream.p_codec_info_list),
                                                p_pref_profile_data->p_codec_info_list))
                                    {
                                        ret_val = ICF_FAILURE;
                                        break;
                                    }                        
                               /*Klocwork Warning Removal*/
                               ICF_KLOCWORK_FALSE_POSITIVE(
                                          p_cc_pdb->p_glb_pdb,\
                                          p_stream_sdp_info->p_local_sdp->\
                                          sdp_stream.p_codec_info_list)
                                }
                            }
                        }    
                    }
                    /* End of fix for spr 14476.*/ 
                }
            }
            p_cc_list = p_cc_list->p_next;
        }



    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_sdp_n_populate_mod_fileds
 * DESCRIPTION :    This function shall compare the 2 SDP?¢s
 *		    (p_applied_sdp and p_remote_sdp) present in the
 *		    p_stream_sdp_info in p_cc_pdb and set the  media_modify_type
 *		    bitmask which shall indicate which fields of the SDP have changed.
 *		    If the stream type is FAX,it shall be ignored
 *		    In the case of audio and video only shall the codecs be compared.
 *		    Silence supp shall be compared only in the case of audio.
 *		    RTP and RTCP(if present) IP and port shall be compared for
 *		    audio,video and TBCP.
 *		    Mode shall be compared only in the case of sudio and video
 *
 ******************************************************************************/
icf_return_t icf_cc_compare_sdp_n_populate_mod_fileds(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_stream_sdp_info_st         *p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        /* If stream_type is FAX,ignore */
        if(ICF_STREAM_TYPE_FAX != p_stream_sdp_info->stream_type)
        {
            /* If stream type is audio or video,compare the codecs */
            if((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type) ||
               (ICF_STREAM_TYPE_VIDEO == p_stream_sdp_info->stream_type))
            {
                p_cc_pdb->p_list_1 = p_stream_sdp_info->p_remote_received_sdp->
                    sdp_stream.p_codec_info_list;
                p_cc_pdb->p_list_2 = p_stream_sdp_info->p_applied_sdp->
                    sdp_stream.p_codec_info_list;
                if (ICF_FALSE == icf_cc_compare_codec_lists(p_cc_pdb))
                {
                    /* Set bit for modification of remote SDP
                     * Remaining SDP doesn't need to be compared
                     */
                    p_stream_sdp_info->media_modify_type |=
                        ICF_REMOTE_SDP_CODEC_CHANGE;
                }


                /* CSR-1-6976670: check codec change and set media_modify_type */
                else if (ICF_TRUE==p_cc_pdb->p_config_data->check_remote_codec_list)
                {
                    p_cc_pdb->p_list_2 = p_stream_sdp_info->\
                        p_lastcommitted_codeclist_from_remote;
                    if (ICF_FALSE == icf_cc_compare_codec_lists(p_cc_pdb))
                    {
                        /* Set bit for modification of remote SDP
                   	 * Remaining SDP doesn't need to be compared
                    	 */
                        p_stream_sdp_info->media_modify_type |=
             	            ICF_REMOTE_SDP_CODEC_CHANGE;
                    }
                }
            
                /******** Changes for Forking & Multiple Answer Start (Rel7.0) ********/

                ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)

                if(ICF_CALL_CONNECT_DONE == 
                    (p_cc_pdb->p_call_ctx->common_bitmask & ICF_CALL_CONNECT_DONE))
                {

                        if(ICF_STREAM_PENDING_DELETE == (ICF_STREAM_PENDING_DELETE
                              & p_stream_sdp_info->media_modify_type))
                        {
                              if(ICF_CC_SSA_CALL_CONNECT == p_cc_pdb->current_event)
                              {
                                    /* we should be commiting this codec as final
                                       answer has not deleted the stream line */
                                    if(ICF_REMOTE_SDP_CODEC_CHANGE == (
                                         ICF_REMOTE_SDP_CODEC_CHANGE & 
                                         p_stream_sdp_info->media_modify_type))
                                    {
                                         p_stream_sdp_info->media_modify_type |=
                                         ICF_REMOTE_ANSWER_CODEC_CHANGE;
                                         p_stream_sdp_info->media_modify_type &=
                                             ~(ICF_REMOTE_SDP_CODEC_CHANGE);
                                    }
                                }
                                else if(ICF_CC_SSA_ALERT_WITH_MEDIA == p_cc_pdb->current_event ||
                                         ICF_CC_SSA_MEDIA_PARAM == p_cc_pdb->current_event)
                                {
                                     /* we will not be committing this teram till final
                                         response has come */
                                     if(ICF_REMOTE_SDP_CODEC_CHANGE == (
                                        ICF_REMOTE_SDP_CODEC_CHANGE & 
                                        p_stream_sdp_info->media_modify_type))
                                     {
                                               p_stream_sdp_info->media_modify_type &=
                                               ~(ICF_REMOTE_SDP_CODEC_CHANGE);
                                     }
                                 }
                          }
                } 

                ICF_CHECK_MULT_ANS_ENBL_END

                /********* Changes for Forking & Multiple Answer End (Rel7.0) *********/
            }
            /* Now comapre the remaining media paramters */
            if ((ICF_NULL == (ICF_REMOTE_SDP_CODEC_CHANGE &
                            p_stream_sdp_info->media_modify_type)) &&
                    (ICF_FALSE == icf_cc_compare_rem_media_params(p_cc_pdb)))
            {
                /* Set bit for modification of remote SDP
                 * Remaining SDP doesn't need to be compared
                 */
                p_stream_sdp_info->media_modify_type |=
                    ICF_REMOTE_SDP_MEDIA_CHANGE;
            }
            if((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type) ||
                    (ICF_STREAM_TYPE_VIDEO == p_stream_sdp_info->stream_type))
            {
                if((ICF_FALSE == icf_cc_check_remote_mode_change(p_cc_pdb)))
                {
                    /* Set bit for modification of remote SDP
                     * Remaining SDP doesn't need to be compared
                     */
                    p_stream_sdp_info->media_modify_type |=
                        ICF_MODE_CHANGE;
                }
            }
        }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_codec_lists
 * DESCRIPTION:     This function shall compare the codec lists passed as
 * 		    paramters in the CC PDB.
 * 		    First it shall compare the number of codecs
 * 		    The it shall compare the codec name and num(if present)
 * 		    in both the lists
 *
 ******************************************************************************/
icf_boolean_t icf_cc_compare_codec_lists(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_list_st                    	*p_codec_list_1 = ICF_NULL;
    icf_list_st                    	*p_codec_list_2 = ICF_NULL;
    icf_boolean_t			is_codec_match = ICF_TRUE;

    /* First compare the number of codecs */

    /* CSR-1-6976670: NULL check for remote_codec_list */

    if(ICF_NULL == p_cc_pdb->p_config_data->check_remote_codec_list)
    {
        if (ICF_FALSE == icf_cc_compare_no_of_nodes_in_list(p_cc_pdb))
        {
            return ICF_FALSE;
        }
    }

    p_codec_list_1 = p_cc_pdb->p_list_1;

    while(ICF_NULL != p_codec_list_1)
    {
        p_cc_pdb->p_generic_ptr_1 = p_codec_list_1->p_data;
        p_codec_list_2 = p_cc_pdb->p_list_2;

        while(ICF_NULL != p_codec_list_2)
        {
            p_cc_pdb->p_generic_ptr_2 = p_codec_list_2->p_data;
            if (ICF_TRUE == icf_cc_compare_codec_attrib(p_cc_pdb))
            {
                break;
            }
            p_codec_list_2 = p_codec_list_2->p_next;
        }

        if ((ICF_NULL == p_codec_list_2))
        {
            /* The codec was not found in the other list */
            is_codec_match = ICF_FALSE;
            break ;
        }
        p_codec_list_1 = p_codec_list_1->p_next;
    }
    return is_codec_match;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_codec_attrib
 * DESCRIPTION:     This function shall compare the codec attributes
 *		    passed as generic ptrs through the PDB.
 *		    It shall compare the codec name and the codec number
 *		    (if present)
 *
 ******************************************************************************/
icf_boolean_t icf_cc_compare_codec_attrib(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_codec_attrib_st		*p_codec_attr_1 =
        (icf_codec_attrib_st *)p_cc_pdb->p_generic_ptr_1;
    icf_codec_attrib_st		*p_codec_attr_2 =
        (icf_codec_attrib_st *)p_cc_pdb->p_generic_ptr_2;
    icf_boolean_t			is_same = ICF_TRUE;

    if ((ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT ==
                (ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_attr_1->bit_mask ) &&
                (ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT ==
                 (ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_attr_2->bit_mask )))
            && (p_codec_attr_1->codec_num != p_codec_attr_2->codec_num))
    {
        is_same = ICF_FALSE;
    }
    /* If codec number is not same check there name only if codec numbers are greater 
     * then 95 */
    else if(p_codec_attr_1->codec_num > 95)
    {

        if (ICF_NULL != (icf_port_strcmp(p_codec_attr_1->codec_name.str,
                        p_codec_attr_2->codec_name.str)))
        {
            is_same = ICF_FALSE;
        }
    }
    /*  SPR 17993 Merge */
    else if(ICF_AUDIO_CODEC_PARAM_VBD_VAL &  p_codec_attr_1->codec_params.audio_param.bit_mask)
        {
           if(ICF_NULL == 
               (ICF_AUDIO_CODEC_PARAM_VBD_VAL & p_codec_attr_2->codec_params.audio_param.bit_mask))
            {
                is_same = ICF_FALSE;
            }
            else if(p_codec_attr_1->codec_params.audio_param.vbd_val !=
                  p_codec_attr_2->codec_params.audio_param.vbd_val)
            {
                is_same = ICF_FALSE;
            }
        } 
    return is_same;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_rem_media_params
 *
 * DESCRIPTION:     This function checks if mms is for remote SDP change
 *                  Also checks the silence suppression for audio stream
 *
 ******************************************************************************/

icf_boolean_t icf_cc_compare_rem_media_params(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_boolean_t          is_same = ICF_TRUE;
    icf_sdp_stream_st      *p_sdp_stream_1 =
        &(p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp->sdp_stream);
    icf_sdp_stream_st      *p_sdp_stream_2 =
        &(p_cc_pdb->p_stream_sdp_info->p_applied_sdp->sdp_stream);

    /* To remove warning*/
    ret_val=ret_val;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace*/

        p_cc_pdb->p_generic_ptr_1 = &(p_sdp_stream_1->stream_address);
    p_cc_pdb->p_generic_ptr_2 = &(p_sdp_stream_2->stream_address);

    /* Invoke function to compare the IP,port */
    if (ICF_FALSE == icf_cc_compare_ip_and_port(p_cc_pdb))
    {
        is_same = ICF_FALSE;
#if 0
        /* Check if the IP received was NULL.In this case mark the
         * remote mode as inactive
         */
        if (ICF_CC_REM_IP_NULL == (ICF_CC_REM_IP_NULL & (
                        p_cc_pdb->common_bitmask)))
        {
            p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp->
                media_mode = ICF_MEDIA_MODE_SEND_RECV;
        }
#endif
    }
    /* Now check if RTCP address and port are present */
    else if ((ICF_FALSE != is_same) && (ICF_SDP_STREAM_RTCP_INFO_PRESENT  ==
                (ICF_SDP_STREAM_RTCP_INFO_PRESENT & p_sdp_stream_1->bit_mask)))
    {
        /* If RTP information is not present in the applied SDP,then
         * the comparsion fails
         */
        if (ICF_NULL == (ICF_SDP_STREAM_RTCP_INFO_PRESENT &
                    p_sdp_stream_2->bit_mask))
        {
            is_same = ICF_FALSE;
        }
        else
        {
            p_cc_pdb->p_generic_ptr_1 = &(p_sdp_stream_1->rtcp_address);
            p_cc_pdb->p_generic_ptr_2 = &(p_sdp_stream_2->rtcp_address);
            /* Invoke function to compare the IP,port */
            if (ICF_FALSE == icf_cc_compare_ip_and_port(p_cc_pdb))
            {
                is_same = ICF_FALSE;
            }
        }
    }
    /* If this is an audio stream ,then check the silence supp */
    if (ICF_STREAM_TYPE_AUDIO == p_cc_pdb->p_stream_sdp_info->stream_type)
    {
        if (ICF_AUDIO_PROF_SILN_SUPP_PRESENT ==
                (ICF_AUDIO_PROF_SILN_SUPP_PRESENT &
                 p_sdp_stream_1->media_stream_param.audio_profile.bit_mask))
        {
            /* If silence supp information is not present in the applied SDP,then
             * the comparsion fails
             */
            if (ICF_NULL == (ICF_AUDIO_PROF_SILN_SUPP_PRESENT &
                        p_sdp_stream_2->media_stream_param.audio_profile.bit_mask))
            {
                is_same = ICF_FALSE;
            }
            else if (p_sdp_stream_1->media_stream_param.audio_profile.
                    silence_suppression != p_sdp_stream_2->media_stream_param.
                    audio_profile.silence_suppression)
            {
                is_same = ICF_FALSE;
            }
        }
        /* SPR 16526: Check if echo_cancellation is present then 
         * set is_same to FALSE
         */
        if(ICF_TRUE == is_same)
        {
            if(ICF_SDP_STREAM_ENCPARAMS_PRESENT == 
                   (ICF_SDP_STREAM_ENCPARAMS_PRESENT & p_sdp_stream_1->bit_mask)\
                   && (ICF_NULL != p_sdp_stream_1->p_encoded_attributes))
            {
                is_same = icf_cc_check_ecan_present_in_encoded_attribs(p_cc_pdb);
            }
        }
        if (ICF_AUDIO_PROF_ECHO_CANC_PRESENT ==
		(ICF_AUDIO_PROF_ECHO_CANC_PRESENT &
		 p_sdp_stream_1->media_stream_param.audio_profile.bit_mask))
      {
	    /* If echo canc information is not present in the applied SDP,then
	     * the comparsion fails
	     */
	    if (ICF_NULL == (ICF_AUDIO_PROF_ECHO_CANC_PRESENT &
			p_sdp_stream_2->media_stream_param.audio_profile.bit_mask))
	    {
		is_same = ICF_FALSE;
	    }
	    else if (p_sdp_stream_1->media_stream_param.audio_profile.
		    echo_canc != p_sdp_stream_2->media_stream_param.
		    audio_profile.echo_canc)
	    {
		is_same = ICF_FALSE;
	    }
      }
      /* Fix for CSR 1-7438489 : SPR-19787*/
      /* Check if ptime is present in received SDP,
       * then do comparison for ptime present in applied SDP and
       * received SDP.If any difference is found, set the
       * the value of is_same with ICF_FALSE.
       */  
      if((ICF_TRUE == is_same) &&
            (ICF_AUDIO_PROF_PTIME_PRESENT ==
                 (ICF_AUDIO_PROF_PTIME_PRESENT &
                  p_sdp_stream_1->media_stream_param.audio_profile.bit_mask)))
      {
        /* If ptime information is not present in the applied SDP,then
         * the comparsion fails
         */
        if (ICF_NULL == (ICF_AUDIO_PROF_PTIME_PRESENT &
                    p_sdp_stream_2->media_stream_param.audio_profile.bit_mask))
        {
            is_same = ICF_FALSE;
        }
        /* Comparison will be failed if the ptime value in recived SDP
         * is different from ptime value in applied SDP.
         */  
        else if (p_sdp_stream_1->media_stream_param.audio_profile.pref_ptime !=
                    p_sdp_stream_2->media_stream_param.audio_profile.pref_ptime)
        {
            is_same = ICF_FALSE;
        }
      }
    }
    
    return is_same;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_ip_and_port
 *
 * DESCRIPTION:     Assumption:
 *                  remote_rtp_addr != commited_rtp_addr &&
 *                  remote_rtp_addr != ICF_NULL)
 *                  ( For ip and port )
 *
 ******************************************************************************/
icf_boolean_t icf_cc_compare_ip_and_port(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
#ifdef ICF_TRACE_ENABLE
    icf_return_t           ret_val = ICF_SUCCESS;
#endif
    icf_boolean_t          is_same = ICF_TRUE;


    icf_transport_address_st    *p_rem_rtp_adr =
        (icf_transport_address_st *)p_cc_pdb->p_generic_ptr_1;
    icf_transport_address_st    *p_com_rtp_adr =
        (icf_transport_address_st *)p_cc_pdb->p_generic_ptr_2;
    icf_uint32_t                rem_ip_addr = 0;
    icf_uint32_t                com_ip_addr = 0;

    /* Check for port: port number is not zero and remote port is not same
     * as committed port */
    if(p_rem_rtp_adr->port_num !=  p_com_rtp_adr->port_num)
    {
        is_same = ICF_FALSE;
    }

    if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == p_rem_rtp_adr->addr.addr_type)
    {
        rem_ip_addr = ICF_MAKE_32BIT_INT(
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_1,
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_2,
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_3,
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_4);

        com_ip_addr = ICF_MAKE_32BIT_INT(
                p_com_rtp_adr->addr.addr.ipv4_addr.octet_1,
                p_com_rtp_adr->addr.addr.ipv4_addr.octet_2,
                p_com_rtp_adr->addr.addr.ipv4_addr.octet_3,
                p_com_rtp_adr->addr.addr.ipv4_addr.octet_4);

        if(ICF_NULL == rem_ip_addr)
        {
            p_cc_pdb->common_bitmask |= ICF_CC_REM_IP_NULL;
            is_same = ICF_FALSE;
        }

        if(rem_ip_addr != com_ip_addr)
        {
            is_same = ICF_FALSE;
        }
    } /* End of check for rtp addr */

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return is_same;
}





/******************************************************************************
 *
 * FUNCTION:        icf_cc_initiate_call_clear
 *
 * DESCRIPTION:     This function initiates normal call clear
 *
 *****************************************************************************/
icf_return_t icf_cc_initiate_call_clear(
        icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_CC_SET_STATUS(p_cc_pdb, ICF_STATUS_CALL_R_SRV_TERMINATE)    

        return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_update_local_sdp
 *
 * DESCRIPTION:     This function updates local SDP codec from applied
 *                  SDP for all streams
 *
 *****************************************************************************/
icf_return_t icf_cc_update_local_sdp(
        icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;
        if ((ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status) && 
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) && 
            (ICF_STREAM_TYPE_TBCP != p_stream_sdp->stream_type)&&
            (ICF_STREAM_PENDING_DELETE !=
            (p_stream_sdp->media_modify_type & ICF_STREAM_PENDING_DELETE)))

        {
           /* 
               ret_val = icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
               &(p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list),
               p_cc_pdb->p_ecode);

               if (ICF_SUCCESS == ret_val) 
               {*/
            ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                    &(p_stream_sdp->p_local_sdp->sdp_stream.
                        p_codec_info_list),
                    p_stream_sdp->p_applied_sdp->sdp_stream.p_codec_info_list);
            /*klocwork warning removal*/ 
            ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                     p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list)
           /* }*/
            if (ICF_FAILURE == ret_val)
                break;
        }
        p_list = p_list->p_next;
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_check_if_update_to_be_sent
 *
 * DESCRIPTION:     This function checks if UPDATE is required to be sent
 *                  to peer if media is changed in MMS response
 *
 *****************************************************************************/
icf_return_t icf_cc_check_if_update_to_be_sent(
        icf_cc_pdb_st      *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_FAILURE;
    icf_return_t            ret_value = ICF_SUCCESS;
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_uint8_t             local_codec_count = ICF_NULL;
    icf_uint8_t             remote_codec_count = ICF_NULL;
    icf_config_data_st      *p_cfg_data = ICF_NULL;
    if(ICF_NULL == p_cc_pdb->p_glb_pdb) 
    {
        ICF_PRINT(((icf_uint8_t *)"[CC]:GLB PDB is null"));
        return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        if (ICF_FAILURE == icf_dbm_get_module_glb_data(p_cc_pdb->p_glb_pdb,
                    ICF_INT_MODULE_CFG,
                    (icf_void_t *)&p_cfg_data, p_cc_pdb->p_ecode))
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ICF_FAILURE;
        }
    /*
     * If offer change is pending event we would not be
     * initiating UPDATE/REINVITE. The offer/anwer would
     * go in 200 OK of UPDATE/REINVITE
     */
     if ((ICF_NULL != (ICF_CC_CHECK_PENDING_EVENT(p_cc_pdb,
                    ICF_PENDING_EVENT_OFFER_CHANGE_REQ))) ||
                    (ICF_NULL != (ICF_CC_CHECK_PENDING_EVENT (p_cc_pdb,
                    ICF_PENDING_EVENT_OFFER_CHANGE_WO_SDP_REQ))))
      {
         ret_val = ICF_FAILURE;
      }
    else
      {
        /*Start SPR 19015 */    
         ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
            p_cc_pdb->p_call_ctx, ret_value)
         if(ICF_FAILURE == ret_value)
         {
            ret_val = ICF_FAILURE; 
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
            return ICF_FAILURE;    
         }/*End SPR 19015 */ 


    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;
        /*Start SPR 19015 */    
        ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
           p_stream_sdp, ret_value)
        if(ICF_FAILURE == ret_value)
        {
            ret_val = ICF_FAILURE; 
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
            return ICF_FAILURE;    
        }/*End SPR 19015 */

        if (ICF_STREAM_TYPE_TBCP == p_stream_sdp->stream_type)
        {
            p_list = p_list->p_next;
            continue;
        }

        /* Compare the codec lists only when stream is 
         * active and allow */
        if ((ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status) && 
                (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) &&
                (ICF_STREAM_TYPE_TBCP != p_stream_sdp->stream_type) &&
                (ICF_STREAM_PENDING_DELETE !=
                (p_stream_sdp->media_modify_type & ICF_STREAM_PENDING_DELETE)))
        {
            /* Fix for CSR 1-7402396: SPR-19822*/
            icf_boolean_t codec_count_match = ICF_TRUE;
            /* Fix corresponding to spr 14476.*/
            /* Earlier we were comparing local sdp with applied irrespective of whether
             * is incoming or outgoing. */
            /* p_cc_pdb->p_list_1 =
               p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list;*/

            /*Fix for CSR 1-7245222 SPR 19716 */
			/*Removed if part so that now whenever check_if_update_to_be_sent
			function is called IPTK generates auto Re-INVITE depending on the 
			configuration of update_reqd in set_call_params irrespective of 
			direction of call */

               /*Start SPR 19015 */    
               ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                   p_cfg_data,
                   ret_value)
               if(ICF_FAILURE == ret_value)
               {
                   ret_val = ICF_FAILURE; 
                   ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                   return ICF_FAILURE;    
               }/*End SPR 19015 */
                /*Outgoing call OR Non Fast Start incoming call*/
                if(ICF_NO_UPDATE == p_cfg_data->update_reqd)
                { 
                    ret_val = ICF_FAILURE;
                    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                        return ret_val;
                }

                /* SPR 19886 : Additional NULL checks put while merging 
                   CSR 1-7402396.
                */    
                ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                        p_stream_sdp->p_local_sdp,
                        ret_value)
                if(ICF_FAILURE == ret_value)
                {
                   ret_val = ICF_FAILURE; 
                   ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                   return ICF_FAILURE;    
                }/* End additional fix of SPR 19886  */

                /* Fix for CSR 1-7402396: SPR-19822*/
                /*Copy pointer of local SDP*/
                p_cc_pdb->p_list_1 =
                     p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list;

				/*Copy Pointer of applied SDP*/
				p_cc_pdb->p_list_2 =
					p_stream_sdp->p_applied_sdp->sdp_stream.p_codec_info_list;

				/* getting and compare the number of codecs in offer and applied*/
  
                /*Getting the number of codecs present in the stream of local sdp*/ 
                icf_cmn_get_codec_attrib_list_count(
				             	p_cc_pdb->p_list_1,
								&local_codec_count);

                /*Getting the number of codecs present in the stream of committed sdp*/ 
                icf_cmn_get_codec_attrib_list_count(
								p_cc_pdb->p_list_2,
								&remote_codec_count);
 
				if (local_codec_count != remote_codec_count)
				{
                    codec_count_match = ICF_FALSE;
                }
      
                /* CSR 1-6088455 Merge */ 
                /* Outgoing call. if the cofiguration is no update if answer with 
                 * single codec,look for remote codes only. 
                 * If configuration is update if muliple offered codecs, 
                 * look for local codec only 
                 */

                /* Fix for CSR 1-7402396: SPR-19822*/
                /* If update_reqd is set with bitmasks 
                 * ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC or
                 * ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC, the required processing 
                 * is done only if codec_count_match is FALSE i.e.number of committed
                 * codecs are not equal to the number of offered codecs.
                 *
                 * If update_reqd is set with bitmasks 
                 * ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC_COMMIT or 
                 * ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC_COMMIT, the required processing
                 * is done as per there specification.These bitmasks are added to maintain the
                 * backward compatibility.  
                 */

                if(((ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC == \
                                p_cfg_data->update_reqd) &&
                   (ICF_FALSE == codec_count_match)) ||
                   (ICF_NO_UPDATE_WHN_ANSWER_SDP_WITH_SINGLE_CODEC_COMMIT == \
                                p_cfg_data->update_reqd))
                {
                    /*Start SPR 19015 */    
                    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                      p_stream_sdp->p_remote_received_sdp,
                      ret_value)
                    if(ICF_FAILURE == ret_value)
                    {
                       ret_val = ICF_FAILURE; 
                       ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                       return ICF_FAILURE;    
                    }/*End SPR 19015 */
                    /*  CSR 1-6088455 Merge */
                    p_cc_pdb->p_list_2 =
                       p_stream_sdp->p_remote_received_sdp->sdp_stream.p_codec_info_list;
                    /*Getting the number of codecs present in the stream of remote sdp*/ 
                    icf_cmn_get_codec_attrib_list_count(
								p_cc_pdb->p_list_2,
								&remote_codec_count);
					ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: remote codec count = %d\n", 
								remote_codec_count));
                    if(remote_codec_count > 1) 
                    {
                        ret_val = ICF_SUCCESS;
                        break;
                    }
                }
                else if(((ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC == \
                                p_cfg_data->update_reqd) &&
                        (ICF_FALSE == codec_count_match)) ||
                        (ICF_UPDATE_WHN_OFFER_SDP_WITH_MULTIPLE_CODEC_COMMIT == \
                                p_cfg_data->update_reqd))
                {
                    /* Outgoing call. Now for Outgoing call codec 
                     * comparison for UPDATE will be done between the offer 
                     * sent in initial INVITE and applied SDP.*/
                    /*Copy pointer of local SDP*/

                    if(ICF_NULL != p_stream_sdp->p_offered_sdp)
                    {
                       p_cc_pdb->p_list_1 =
                          p_stream_sdp->p_offered_sdp->sdp_stream.p_codec_info_list;
                    }
                    else
                    {
                       /*Start SPR 19015 */    
                       ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                           p_stream_sdp->p_local_sdp,
                           ret_value)
                       if(ICF_FAILURE == ret_value)
                       {
                           ret_val = ICF_FAILURE; 
                           ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                           return ICF_FAILURE;    
                       }/*End SPR 19015 */
                       p_cc_pdb->p_list_1 =
                          p_stream_sdp->p_local_sdp->sdp_stream.p_codec_info_list;
                    }

                    /*Getting the number of codecs present in the stream of local sdp*/
                    icf_cmn_get_codec_attrib_list_count(
				             	p_cc_pdb->p_list_1,
								&local_codec_count);

                    /*  CSR 1-6088455 Merge*/
                    ICF_PRINT(((icf_uint8_t *)"\n[CC]: local codec count = %d", 
                                                                  local_codec_count));
                    /* If the number of offered codecs are more than 1, then
                     * REINVITE should be sent
                     */
                    if (local_codec_count > 1) 
                    {
                        ret_val = ICF_SUCCESS;
                        break;
                    }
                }
        } /*End of a stream */
        p_list = p_list->p_next;
    }
    p_cc_pdb->p_list_1 = ICF_NULL;
    p_cc_pdb->p_list_2 = ICF_NULL;
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_set_reject_stream_status
 *
 * DESCRIPTION:     This function iterates thru all the streams in the
 *                  icf_stream_sdp_info_st in call context and set the
 *                  given status in the rejected stream in MMS response
 *
 *****************************************************************************/
icf_void_t    icf_cc_set_reject_stream_status(
        icf_cc_pdb_st    *p_cc_pdb,
        icf_uint8_t      stream_status)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_mmi_mms_resp_st     *p_mms_resp = ICF_NULL;
    icf_uint8_t             cnt = 0,ongoing_event =0 ;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        if (ICF_NULL == p_cc_pdb->p_internal_msg)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return;
        }
    p_mms_resp =
        (icf_mmi_mms_resp_st*)p_cc_pdb->p_internal_msg->p_msg_data;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

    /*Iterate thru all the modified stream*/
    for (cnt = 0; cnt < p_mms_resp->mod_stream_count; cnt++)
    {
        /*If stream modification request is failed*/
        if ( (cnt < ICF_MAX_STREAM_PER_CALL) && (ICF_FALSE == p_mms_resp->modified_stream_resp[cnt].status_success))
        {

            p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

            /*Search the corresponding node in call context for
             * that stream ID*/
            while (ICF_NULL != p_list)
            {
                p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;

                /*If stream found then assign the stream status*/
                if ((cnt < ICF_MAX_STREAM_PER_CALL) && (p_mms_resp->modified_stream_resp[cnt].stream_id ==
                        p_stream_sdp->stream_id))
		{
			p_stream_sdp->stream_status |= stream_status;

			/* SPR 19999: CSR 1-7574801 
             * In case a stream is rejected by media manager in mid call then the 
             * received payload list should be restored sp that while rejecting the
             * stream with port 0, the payload list in m-line is same as received
             * in the offer.
             */
            /* Fix for CSR 1-7643774 : copy the codec list for offers received
             * in PRACK so that if the offer is rejected then correct codec list
             * is populated along with port 0 for rejected stream.
             */
			if ((ICF_CC_SSA_OFFER_CHANGE == ongoing_event) ||
                (ICF_CC_ALERT_PRACK == ongoing_event) ||
                (ICF_CC_MEDIA_PARAM_PRACK == ongoing_event))
			{
				if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
							p_cc_pdb->p_glb_pdb,
							&(p_stream_sdp->p_remote_recvd_codec_list),
							p_stream_sdp->p_remote_received_sdp->
							sdp_stream.p_codec_info_list))
				{
					ret_val = ICF_FAILURE;
				}
			}
			break;
		}
                p_list = p_list->p_next;
            }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
}

#if  0
/******************************************************************************
 *
 * FUNCTION:        icf_cc_reset_reject_stream_status
 *
 * DESCRIPTION:     This function iterates thru all the streams in the
 *                  icf_stream_sdp_info_st in call context and reset the
 *                  given status in the rejected stream in MMS response
 *
 *****************************************************************************/
icf_void_t    icf_cc_reset_reject_stream_status(
        icf_cc_pdb_st    *p_cc_pdb,
        icf_uint8_t      stream_status)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_mmi_mms_resp_st     *p_mms_resp = ICF_NULL;
    icf_uint8_t             cnt = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
        if (ICF_NULL == p_cc_pdb->p_internal_msg)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return;
        }
    p_mms_resp =
        (icf_mmi_mms_resp_st*)p_cc_pdb->p_internal_msg->p_msg_data;

    /*Iterate thru all the modified stream*/
    for (cnt = 0; cnt < p_mms_resp->mod_stream_count; cnt++)
    {
        /*If stream modification request is failed*/
        if (ICF_FALSE == p_mms_resp->modified_stream_resp[cnt].status_success)
        {

            p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

            /*Search the corresponding node in call context for
             * that stream ID*/
            while (ICF_NULL != p_list)
            {
                p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;

                /*If stream found then assign the stream status*/
                if (p_mms_resp->modified_stream_resp[cnt].stream_id ==
                        p_stream_sdp->stream_id)
                {
                    p_stream_sdp->stream_status &= ~stream_status;
                    break;
                }
                p_list = p_list->p_next;
            }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
}
#endif

/***************************************************************************

 ***
 *
 * FUNCTION:        icf_cc_analyse_mms_resp
 *
 * DESCRIPTION:     This function checks the MMS response received from MMI
 *                  and return the result as if MMS resp is successful or
 *                  rejected
 *


 ****************************************************************************

 */
icf_void_t icf_cc_analyse_mms_resp(
        INOUT  icf_cc_pdb_st        *p_cc_pdb)
{
#ifdef ICF_TRACE_ENABLE    
    icf_return_t            ret_val = ICF_SUCCESS;
#endif
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_mmi_mms_resp_st     *p_mms_resp = ICF_NULL;
    icf_uint8_t             cnt = 0;
    icf_boolean_t           stream_deleted = ICF_FALSE;
    icf_uint8_t             mod_rej_cnt = 0;
    icf_uint8_t             mod_acc_cnt = 0;
    icf_uint8_t             add_rej_cnt = 0;
    icf_uint8_t             add_acc_cnt = 0;

    /* CSR-1-6976670 */
    icf_stream_sdp_info_st  *p_stream_sdp_info =ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        if (ICF_NULL == p_cc_pdb->p_internal_msg)
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return;
        }
    p_mms_resp =
        (icf_mmi_mms_resp_st*)p_cc_pdb->p_internal_msg->p_msg_data;

    /* SPR 18991: Update the trace information after the MMS Response
       type is set, if the response type is success only then the
       applied SDP mode will be updated.
     */  

    if (ICF_FAILURE == p_mms_resp->result)
    {
        p_cc_pdb->mms_resp_type = ICF_CC_MMS_RESP_OFFER_REJECTED;
/* REl 8.1
 * if tunnel mode is ON then
 * bypass the old stream sdp
 * handling
*/
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        /*Search the corresponding node in call context for
         * that stream ID*/
        while (ICF_NULL != p_list)
        {
            p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;

            if (ICF_STREAM_DELETE & p_stream_sdp->media_modify_type)
            {
                p_stream_sdp->media_modify_type &= ~ICF_STREAM_DELETE;
                p_stream_sdp->stream_status |= ICF_MEDIA_STREAM_ACTIVE;
                p_stream_sdp->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;

            }
            p_list = p_list->p_next;
        }
        /* SPR 18991: Update the trace specific information before returning
           from the function.
         */  
#ifdef ICF_TRACE_ENABLE    
        ret_val = icf_cc_update_trace_info_aftr_mms_resp(p_cc_pdb);
#endif   
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)    
    /* Call this macro to issue interface trace */
    ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_MMS_RESP_RECVD)
#ifdef ICF_TRACE_ENABLE                
    /* Tunnel mode specific trace */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_TUNNELLED_MMS_RESP_RECVD)  
#endif  
ICF_CHECK_TUNNEL_MODE_END
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return;
    }
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_TRUE)

    /* Call this macro to issue interface trace */
    ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_MMS_RESP_RECVD)
#ifdef ICF_TRACE_ENABLE                
    /* Tunnel mode specific trace */
    ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_TUNNELLED_MMS_RESP_RECVD)  
#endif    

    if(ICF_MMI_MMS_ACTION_LOCAL_SDP_RCVD != 
       (ICF_MMI_MMS_ACTION_LOCAL_SDP_RCVD & p_mms_resp->bit_mask))
    {
        p_cc_pdb->mms_resp_type = ICF_CC_MMS_RESP_LOCAL_SDP_NOT_RECV;
    }
ICF_CHECK_TUNNEL_MODE_END
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb, ICF_FALSE)

    /* CSR-1-6976670: Backup codec received from remote */

    if ((ICF_TRUE == p_cc_pdb->p_config_data->check_remote_codec_list) && 
        (p_cc_pdb->mms_resp_type != ICF_CC_MMS_RESP_OFFER_REJECTED ))
    {	
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        while (ICF_NULL != p_list)
        {
            p_stream_sdp_info = (icf_stream_sdp_info_st *)p_list->p_payload;
            if((ICF_NULL != p_stream_sdp_info) && 
                (ICF_NULL != (p_stream_sdp_info->p_bckup_codec_from_remote)))
            {
                if(ICF_NULL!= (p_stream_sdp_info->p_lastcommitted_codeclist_from_remote))
                {
                    icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb, 
                        &p_stream_sdp_info->p_lastcommitted_codeclist_from_remote,
                        p_cc_pdb->p_ecode);
                    p_stream_sdp_info->p_lastcommitted_codeclist_from_remote = ICF_NULL;
                }
                icf_cmn_clone_codec_attrib_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_stream_sdp_info->p_lastcommitted_codeclist_from_remote),
                    p_stream_sdp_info->p_bckup_codec_from_remote);
            }
        p_list = p_list->p_next;
        }
    }

    /*Check thru the MMS response, if any stream were deleted
      then it can not be offer rejected*/
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    /*Search the corresponding node in call context for
     * that stream ID*/
    while (ICF_NULL != p_list)
    {
        p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;

        if (ICF_STREAM_DELETE & p_stream_sdp->media_modify_type)
        {
            /* p_stream_sdp->media_modify_type &= ~ICF_STREAM_DELETE; */
            if((ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_MEDIA_SETUP_TYPE)) &&
                    (ICF_FALSE == p_cc_pdb->p_call_ctx->media_committed)
                    && (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_DIR)))
            {
                p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_ACTIVE;
                p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_NW_ALLOW;
            }

            stream_deleted = ICF_TRUE;
            break;
        }
        p_list = p_list->p_next;
    }

    /*count modify streams success and failures*/
    if (ICF_MMI_MMS_ACTION_MODIFY_RESP & p_mms_resp->bit_mask)
    {
        /* flag will be False if it was modification.*/
        p_cc_pdb->flag = ICF_FALSE;
        for (cnt = 0; (cnt < p_mms_resp->mod_stream_count) && (cnt < ICF_MAX_STREAM_PER_CALL); cnt++)
        {	
            p_cc_pdb->count = cnt;
            ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_MMS_RESP_RECVD)
                /*If stream modification request is failed*/
                if (ICF_FALSE == 
                        p_mms_resp->modified_stream_resp[cnt].status_success)
                {	
                    icf_internal_list_st		*p_tmp_node = ICF_NULL;
                    icf_internal_sdp_stream_st	*p_tmp_sdp = ICF_NULL;	
                    icf_uint32_t				rej_stream_id = 
                        p_mms_resp->modified_stream_resp[cnt].stream_id;
                    p_tmp_node = icf_cmn_get_node_in_internal_list(
                            p_cc_pdb->p_glb_pdb->p_call_ctx->p_stream_sdp_info,
                            &rej_stream_id);

                    if(ICF_NULL != p_tmp_node)
                    {
                        if(ICF_STREAM_TYPE_TBCP != 
				((icf_stream_sdp_info_st *)p_tmp_node->p_payload)->stream_type)
                        {
                           p_tmp_sdp = ((icf_stream_sdp_info_st *)p_tmp_node->p_payload)->p_local_sdp;
                           p_tmp_sdp->media_mode = 
                            ((icf_stream_sdp_info_st *)p_tmp_node->p_payload)->backup_mode;
                        }
                    }

                    mod_rej_cnt++;
                }
                else
                {
                    mod_acc_cnt++;
                }
        }
    }
    /*count add streams success and failures*/
    if (ICF_MMI_MMS_ACTION_ADD_RESP & p_mms_resp->bit_mask)
    {
        /* flag will be ICF_TRUE if it this porticular stream was 
           added.*/
        p_cc_pdb->flag = ICF_TRUE;
        for (cnt = 0; (cnt < p_mms_resp->add_stream_count) && (cnt < ICF_MAX_STREAM_PER_CALL); cnt++)
        {
            p_cc_pdb->count = cnt;
            ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_MMS_RESP_RECVD)
                /*If stream addition request is failed*/
                if (ICF_FALSE == 
                        p_mms_resp->added_stream_resp[cnt].status_success)
                {
                    add_rej_cnt++;

                }
                else
                {
                    add_acc_cnt++;
                }
        }
    }

    p_cc_pdb->mms_resp_type = ICF_NULL;

    /*If some stream has been deleted and modify is failed
     *for some or all streams mark mod rejected*/
    if ((ICF_TRUE == stream_deleted) &&
            (mod_acc_cnt != p_mms_resp->mod_stream_count))
    {
        p_cc_pdb->mms_resp_type = ICF_CC_MMS_RESP_MOD_REJECTED;
    }
    /*If no stream has been deleted and all modified streams
     *are rejected set offer rejected*/
    else if ((mod_rej_cnt == p_mms_resp->mod_stream_count) &&
            (ICF_FALSE == stream_deleted)&&
             (add_acc_cnt == 0))
    {
        p_cc_pdb->mms_resp_type = ICF_CC_MMS_RESP_OFFER_REJECTED;
    }
    /*If no stream has been deleted and modify for some streams
     *is rejected but not all set mod rejected*/
    else if ((mod_acc_cnt != p_mms_resp->mod_stream_count) &&
            (mod_acc_cnt != 0) && (ICF_FALSE == stream_deleted))
    {
        p_cc_pdb->mms_resp_type = ICF_CC_MMS_RESP_MOD_REJECTED;
    }

    /*If above all is not set and some added stream is failure
     *then set add rejected*/
    if ((ICF_NULL == p_cc_pdb->mms_resp_type) &&
            (0 != add_rej_cnt))
    {
        p_cc_pdb->mms_resp_type = ICF_CC_MMS_RESP_ADD_REJECTED;
    }

    /* SPR 18991: Update the trace specific information after updating
       the MMS response type in PDB so that applied mode is not updated
       in case of rejection.
     */ 
#ifdef ICF_TRACE_ENABLE    
       ret_val = icf_cc_update_trace_info_aftr_mms_resp(p_cc_pdb);
#endif
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return;
}





/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_streams_for_reoccurence
 *
 * DESCRIPTION:     This function will check if a stream is occuring
 *					multiple times then it will return ICF_TRUE otherwise
 *					ICF_FALSE.And if it is outgoing call and a stream is
 *					recurring then we have to clear the call.
 ******************************************************************************/
icf_boolean_t icf_cc_check_streams_for_reoccurence
(
 icf_cc_pdb_st *p_cc_pdb
 )
{
    icf_boolean_t			ret_val=ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        if (ICF_NULL != (p_cc_pdb->p_call_ctx->call_type
                    & p_cc_pdb->call_type))
        {
            /* Stream is reoccuring.*/
            if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_DIR))
            {
                /* It is an outgoing call.*/

                /* We have to clear the call, only if pref media profile exists,
                 * just set the bit in common_bitmask of p_cc_pdb. In icf_cc_populate
                 * _local_sdp() if pref profile was absent then we will just reset this
                 * bit in case of recurrence of streams.
                 */
                p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
            }
            /* Suppose it would have been an incoming call and stream was occuring
             * multiple times then we have to just made this stream INACTIVE and keeping
             * it NWALLOW, but this will be catered in the function written for incoming
             * call check.
             */
            ret_val = ICF_TRUE;
        }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_local_sdp_from_stream_capab
 *
 * DESCRIPTION:     This function fills local SDP in call context from
 *                  		media profile received in app_add
 *
 *					Before calling this function we need to set
 *					p_cc_pdb->p_strea_sdp_info with the streams p_stream_sdp_info
 *					whose local sdp is to be created from stream capab list.
 *
 *					Also we need to initialise p_cc_pdb->p_generic_ptr_1 with
 *					appropriate stream capab from which local sdp is to be
 *					created.This generic pointer will be type casted to icf_stream_
 *					capab_st pointer.
 ******************************************************************************/
icf_return_t    icf_cc_fill_local_sdp_from_stream_capab
(
 icf_cc_pdb_st                 *p_cc_pdb
 )
{
    icf_internal_sdp_stream_st	*p_local_sdp = ICF_NULL;

    icf_stream_sdp_info_st		*p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;

    icf_stream_capab_st			*p_stream_capab =
        (icf_stream_capab_st *)
        (p_cc_pdb->p_generic_ptr_1);

    icf_return_t ret_val = ICF_SUCCESS;
    icf_line_data_st             *p_line_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        if (ICF_NULL != p_stream_capab)
        {
            /*Assigning memory of type icf_internal_sdp_stream.*/
            ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
                     ICF_MEM_SDP,
                    p_local_sdp,
                    ICF_RET_ON_FAILURE,
                    p_cc_pdb->p_ecode,
                    ret_val);

            icf_port_memset(p_local_sdp, 0, 
                    ICF_PORT_SIZEOF(icf_internal_sdp_stream_st));

            if (ICF_STREAM_CAPAB_CODEC_INFO_PRESENT == 
                    (p_stream_capab->bit_mask & ICF_STREAM_CAPAB_CODEC_INFO_PRESENT))
            {
                /*Start for Per Line feature addition  - codec*/
                /*
                if ( (ICF_NULL==p_stream_capab->p_codec_info_list) ||
                        ((ICF_NULL!=p_stream_capab->p_codec_info_list) && !(p_cc_pdb->flag)))
                */
                if(ICF_NULL==p_stream_capab->p_codec_info_list)
                {
                    if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk( p_cc_pdb->p_glb_pdb,
                                p_cc_pdb->p_call_ctx->line_id,
                                &p_line_data, 
                                p_cc_pdb->p_ecode))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    if(  p_stream_capab->stream_type == ICF_STREAM_TYPE_AUDIO )
                    {
                        if(p_line_data->p_codec_attributes_audio != ICF_NULL)
                            p_stream_capab->p_codec_info_list = (icf_list_st*)p_line_data->p_codec_attributes_audio;
                    }
                    else if(p_stream_capab->stream_type == ICF_STREAM_TYPE_VIDEO)
                    {
                        if(p_line_data->p_codec_attributes_video != ICF_NULL)
                            p_stream_capab->p_codec_info_list = (icf_list_st*)p_line_data->p_codec_attributes_video;
                    }     
                }   
                if(ICF_STREAM_TYPE_TBCP != p_stream_capab->stream_type)
                {
                ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                        &(p_local_sdp->sdp_stream.p_codec_info_list),
                        p_stream_capab->p_codec_info_list);
                 /*klocwork warning removal*/ 
                 ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                             p_local_sdp->sdp_stream.p_codec_info_list)
                }
            }
            else
            {
                if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(p_cc_pdb->p_glb_pdb,
                            p_cc_pdb->p_call_ctx->line_id,
                            &p_line_data,
                            p_cc_pdb->p_ecode))
                {
                    ret_val = ICF_FAILURE;
                }
                if( p_stream_capab->stream_type == ICF_STREAM_TYPE_AUDIO )
                {
                    if(p_line_data->p_codec_attributes_audio != ICF_NULL)
                     {
                        /*p_stream_capab->p_codec_info_list = (icf_list_st *)p_line_data->p_codec_attributes_audio;*/
                        ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                                &(p_local_sdp->sdp_stream.p_codec_info_list),
                                p_line_data->p_codec_attributes_audio);
                        /*klocwork warning removal*/ 
                         ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                               p_local_sdp->sdp_stream.p_codec_info_list)
                       }

                }
                else if (p_stream_capab->stream_type == ICF_STREAM_TYPE_VIDEO)
                {
                    if(p_line_data->p_codec_attributes_video != ICF_NULL)
                        /*p_stream_capab->p_codec_info_list = (icf_list_st *)p_line_data->p_codec_attributes_video;*/
                        ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                                &(p_local_sdp->sdp_stream.p_codec_info_list),
                                p_line_data->p_codec_attributes_video);
                    /*klocwork warning removal*/ 
                    ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                  p_local_sdp->sdp_stream.p_codec_info_list)

                }
            }  /* End Per Line Addition - codec*/ 

            if (ICF_FAILURE != ret_val)
            {
                if (ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT	== 
                        (p_stream_capab->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
                {
                    ret_val = icf_cmn_copy_string_list(p_cc_pdb->p_glb_pdb,
                            &((p_local_sdp->sdp_stream).p_encoded_attributes),
                            p_stream_capab->p_encoded_attributes,
                            p_cc_pdb->p_ecode);
                    /*klocwork warning removal*/ 
                    ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                           (p_local_sdp->sdp_stream).p_encoded_attributes)

                    (p_local_sdp->sdp_stream).bit_mask |= 
                        ICF_SDP_STREAM_ENCPARAMS_PRESENT;
                }
            }
            if (ICF_FAILURE != ret_val)
            {
                if (ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT ==
                        (p_stream_capab->bit_mask & ICF_STREAM_CAPAB_MEDIA_STREAM_PARAM_PRESENT))
                {
                    (p_local_sdp->sdp_stream).media_stream_param =
                        p_stream_capab->media_stream_param;
                }
                p_stream_sdp_info->p_local_sdp = p_local_sdp;
            }
            else
            {
                ret_val = icf_cmn_delete_internal_sdp_stream(
                        p_cc_pdb->p_glb_pdb,
                        &p_local_sdp,
                        p_cc_pdb->p_ecode);
                ret_val = ICF_FAILURE;
            }
        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

        return ret_val;
}




/***************************************************************************

 ****
 *
 * FUNCTION:        icf_cc_fill_local_sdp_from_profile
 *
 * DESCRIPTION:     This function fills local Sdp in call context either 

 from
 *                  preferred media profile or app_add profile depending 

 upon the
 *					preferred_profile_exists boolean 

 flag.
 *
 *					Before calling this function we need 

 to set following things:
 *					1) p_cc_pdb->stream_id, if it pref 

 profile does NOT exists.
 *					2) p_cc_pdb->flag, with ICF_TRUE if 

 pref profile exists
 *					   otherwise ICF_FALSE.
 * 					3) p_cc_pdb->p_generic_ptr_1 with 

 appropriate stream's stream
 *					   capab pointer either from pref 

 profile or from
 *					   app_add profile.
 *
 * 					Most importantly pointer to 

 structure of type
 *					icf_stream_sdp_info will be returned 

 in p_cc_pdb->
 *					p_stream_sdp_info


 ****************************************************************************

 **/
icf_return_t    icf_cc_fill_local_sdp_from_profile
(
 icf_cc_pdb_st                 *p_cc_pdb
 )
{
    icf_return_t                  ret_val = ICF_SUCCESS;
    icf_stream_id_t               stream_id = p_cc_pdb->stream_id;
    icf_boolean_t				  preferred_profile_exists = 
        p_cc_pdb->flag;
    icf_stream_capab_st			  *p_profile_data =
        (icf_stream_capab_st *)
        (p_cc_pdb->p_generic_ptr_1);

    /* Basically newly created node will of type icf_stream_sdp_info 
     * will be extracted in calling function through 
     * p_cc_pdb->p_stream_sdp_info.
     */
    icf_stream_sdp_info_st        **p_p_stream_sdp_info =
        &(p_cc_pdb->p_stream_sdp_info);

    /*Also mode has to be assigned value yet;*/

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
                ICF_MEM_COMMON,
                *p_p_stream_sdp_info,
                ICF_RET_ON_FAILURE,
                p_cc_pdb->p_ecode,
                ret_val);

    icf_port_memset(*p_p_stream_sdp_info, 0,
            ICF_PORT_SIZEOF(icf_stream_sdp_info_st));

    /* if preferred profile exists then directly copy the stream id from
     * pref profile, otherwise copy stream id passed to the above function
     */
    if (ICF_TRUE == preferred_profile_exists)
    {
        if (ICF_NULL != p_profile_data)
        {
            (*p_p_stream_sdp_info)->stream_id = 
                p_profile_data->stream_id;
        }
    }
    else
    {
        (*p_p_stream_sdp_info)->stream_id = stream_id;
    }

    if (ICF_NULL != p_profile_data)
    {
        (*p_p_stream_sdp_info)->stream_type = 
            p_profile_data->stream_type;
        (*p_p_stream_sdp_info)->stream_status = 
            ICF_MEDIA_STREAM_ACTIVE;
        (*p_p_stream_sdp_info)->stream_status |= 
            ICF_MEDIA_STREAM_NW_ALLOW;

        /* Here NO need to initialise p_cc_pdb->p_stream_sdp_info 
         * And p_cc_pdb->p_list_1(which are requirements before calling 
         * this function),
         * because they are already initialised with suitable values.
         */
        ret_val = icf_cc_fill_local_sdp_from_stream_capab(p_cc_pdb);
        if (ICF_FAILURE != ret_val)
        {
            if ((*p_p_stream_sdp_info)->stream_type == 
                    ICF_STREAM_TYPE_AUDIO
                    || (*p_p_stream_sdp_info)->stream_type == 
                    ICF_STREAM_TYPE_VIDEO)
            {
                ((*p_p_stream_sdp_info)->p_local_sdp->sdp_stream).trans_mode
                    = ICF_TRANSPORT_MODE_RTP_AVP;
            }
            if ((*p_p_stream_sdp_info)->stream_type == 
                    ICF_STREAM_TYPE_TBCP)
            {
                ((*p_p_stream_sdp_info)->p_local_sdp->sdp_stream).trans_mode
                    = ICF_TRANSPORT_MODE_UDP;
            }
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_audio_call_by_default
 *
 * DESCRIPTION:     This function creates icf_stream_sdp_info node for
 *					Audio stream by default.
 *                  In tunnel mode, this function will create a single node
 *                  in the call context of type icf_stream_sdp_info and
 *                  initialise it with default vales.
 ******************************************************************************/
icf_return_t icf_cc_create_audio_call_by_default
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_stream_sdp_info_st     *p_temp_stream_sdp_info = ICF_NULL;
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_internal_sdp_stream_st *p_local_sdp = ICF_NULL;
    icf_line_data_st           *p_line_data = ICF_NULL;
    icf_return_t               dummy_retval = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /* Assigning memory of type icf_internal_sdp_stream.*/
    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
               ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
               ICF_MEM_SDP,
               p_local_sdp,
               ICF_RET_ON_FAILURE,
               p_cc_pdb->p_ecode,
               ret_val)

    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
               ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
               ICF_MEM_COMMON,
               p_temp_stream_sdp_info,
               ICF_DONOT_RET_ON_FAILURE,
               p_cc_pdb->p_ecode,
               ret_val)

    if (ICF_FAILURE == ret_val)
    {
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_local_sdp,
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val) 
        ret_val = ICF_FAILURE;    
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ICF_FAILURE;
    }

    icf_port_memset(p_temp_stream_sdp_info, 0, 
                ICF_PORT_SIZEOF(icf_stream_sdp_info_st));

     /* SDP Tunnelling Enhancements : Initialising icf_internal_sdp_stream_st */
    icf_port_memset(p_local_sdp, 0, 
                ICF_PORT_SIZEOF(icf_internal_sdp_stream_st));

    if (ICF_SUCCESS == ret_val)
    {
        /* In tunnel mode, the following code snippet is not applicable as
           we do not understand codecs and their attributes. 
        */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
        /* SPR14459:-for copying the line audio codecs in create call (audio) */
        icf_dbm_fetch_line_cntxt_blk( p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->line_id,
                &p_line_data,
                p_cc_pdb->p_ecode);

        if(p_line_data->p_codec_attributes_audio != ICF_NULL)
        {

            ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                    &(p_local_sdp->sdp_stream.p_codec_info_list),
                    (icf_list_st*)p_line_data->p_codec_attributes_audio);
        }        

        p_temp_stream_sdp_info->stream_id=1;
        p_temp_stream_sdp_info->stream_type=ICF_STREAM_TYPE_AUDIO;
        p_temp_stream_sdp_info->stream_status=ICF_MEDIA_STREAM_ACTIVE;
        p_temp_stream_sdp_info->stream_status|=ICF_MEDIA_STREAM_NW_ALLOW;
ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
        
        /* Initialisations of members of icf_stream_sdp_info_st in tunnel mode */
        p_temp_stream_sdp_info->stream_id = 1;
        p_temp_stream_sdp_info->stream_type = ICF_STREAM_TYPE_INVALID;
        p_temp_stream_sdp_info->stream_status = ICF_MEDIA_STREAM_ACTIVE;
        p_temp_stream_sdp_info->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;
        p_temp_stream_sdp_info->application_mode = ICF_MEDIA_MODE_INACTIVE;
        p_temp_stream_sdp_info->backup_mode = ICF_MEDIA_MODE_INACTIVE;
        p_temp_stream_sdp_info->mm_applied_mode = ICF_MEDIA_MODE_INACTIVE;
        /* The remaining pointers and fields in the structure are already
           taken care of by the call to icf_port_memset above.
        */

        /* Initialising media_mode field of icf_internal_sdp_stream_st */
        p_local_sdp->media_mode = ICF_MEDIA_MODE_INACTIVE;
        /* The remaining pointers and fields in the structure are already
           taken care of by the call to icf_port_memset above.
        */

ICF_CHECK_TUNNEL_MODE_END

        p_cc_pdb->p_stream_sdp_info = p_temp_stream_sdp_info;
        /* Now a node has been created and init with default values. Assigning
           the same to the call context pointer.
        */   
        p_cc_pdb->p_stream_sdp_info->p_local_sdp = p_local_sdp;
    }

    
    /* In tunnel mode, after successful creation of the node of type
       icf_stream_sdp_info_st, we will add it to the call context.
    */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)        
    if(ICF_SUCCESS == ret_val)
    {
        ret_val = icf_cmn_add_node_to_internal_list(
                        p_cc_pdb->p_glb_pdb,
                        &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                        p_cc_pdb->p_stream_sdp_info,
                        &(p_cc_pdb->p_stream_sdp_info->stream_id),
                        ICF_NULL);
        if (ICF_FAILURE == ret_val)
        {
            p_cc_pdb->common_bitmask |=
                        ICF_CC_STREAM_POS_VALIDATION_FAIL;
            ret_val = ICF_FAILURE;
            
            /* Free the memory acquired for the nodes above */
            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                        p_local_sdp,
                        ICF_MEM_COMMON,
                        p_cc_pdb->p_ecode,
                        dummy_retval)
            p_local_sdp = ICF_NULL;

            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                        p_temp_stream_sdp_info,
                        ICF_MEM_COMMON,
                        p_cc_pdb->p_ecode,
                        dummy_retval)
            p_temp_stream_sdp_info = ICF_NULL;
        }
    }
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}




/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_audio_call
 *
 * DESCRIPTION:     This function is invoked if outgoing call is initiated
 *					by some service NOT by PA.
 *					This function will create Audio stream from pref(if exists),
 *                  otherwise from app_add, if app_add does not contain Audio then
 *					it will create default Audio stream.
 ******************************************************************************/
icf_return_t
icf_cc_create_audio_call(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;

    icf_list_st				*p_pref_profile_node = ICF_NULL;

    icf_internal_list_st	*p_app_profile_node = ICF_NULL;

    icf_app_conf_st         *p_app_conf = ICF_NULL;

    icf_stream_id_t         stream_id = 1;

    icf_media_type_t		stream_type = ICF_STREAM_TYPE_AUDIO;

    icf_uint32_t			identifier = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        p_pref_profile_node = p_cc_pdb->p_call_ctx->p_pref_media_profile;

    if (ICF_SUCCESS == icf_cfg_fetch_app_conf(
                p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_call_ctx->app_id,
                &p_app_conf,
                p_cc_pdb->p_ecode)
       )
    {
        p_app_profile_node = p_app_conf->p_app_media_profile;
    }
    if (ICF_NULL != p_pref_profile_node)
    {
        /* Pref profile exists.*/
        /* Get Pointer to Audio node from the pref profile,*/
        p_pref_profile_node = icf_cmn_get_node_from_stream_capab_list(
                p_pref_profile_node,
                stream_type);

        if (ICF_NULL == p_pref_profile_node)
        {
            /* Pref profile is not containing Audio profile in it,so we
             * will clear the call.
             */
            p_cc_pdb->common_bitmask |=
                ICF_CC_STREAM_POS_VALIDATION_FAIL;
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Control will be here if pref profile exists and it is containing
             * Audio profile in it as well.
             */

            if (ICF_NULL != p_app_profile_node)
            {
                /* app_add exists.*/
                /* Now check whether Audio stream exists in app_add or not.*/
                identifier = stream_type;
                p_app_profile_node = icf_cmn_get_node_in_internal_list(
                        p_app_profile_node,
                        &identifier);
                if (ICF_NULL == p_app_profile_node)
                {
                    /* Audio stream is Not present in app_add profile.*/
                    p_cc_pdb->common_bitmask |=
                        ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                }
            }
            if (ICF_FAILURE != ret_val) 
            {
                /* Either app_add does not exists and if it exists then 
                 * it is supporting Audio,now create Audio from pref profile.
                 */

                /* We have to set these variables before calling 
                 * this function.But we need not to set stream id 
                 * because pref profile exists.
                 */

                /* flag is True because pref profile exists.*/
                p_cc_pdb->flag = ICF_TRUE;

                /* This is the node of list of type stream capab from 
                 * where Audio parameters will be extracted.
                 */
                p_cc_pdb->p_generic_ptr_1 = p_pref_profile_node->p_data;
                ret_val = icf_cc_fill_local_sdp_from_profile(p_cc_pdb);
            }

        }
    }
    else
    {
        /* Control will be here if pref profile does NOT exists.
         * Now search for Audio stream in app_add profile.
         */

        if (ICF_NULL != p_app_profile_node)
        {
            /* Control will be here if app_add profile exists,
             * Now search for Audio profile in it.
             */
            identifier = stream_type;
            p_app_profile_node = icf_cmn_get_node_in_internal_list(
                    p_app_profile_node,
                    &identifier);
            if (ICF_NULL == p_app_profile_node)
            {
                /* Control will be here if app_add profile exists but 
                 * it is NOT containing Audio profile, we will clear the call.
                 */
                p_cc_pdb->common_bitmask |=
                    ICF_CC_STREAM_POS_VALIDATION_FAIL;
                ret_val = ICF_FAILURE;
            }
            else
            {
                /* Control will be here only if Audio profile exists in the
                 * app_add profile.
                 */
                /* Now create audio stream from the app_add profile.*/

                /* We have to set these variables before calling this function.*/
                p_cc_pdb->stream_id = stream_id;

                /* flag is false because pref profile does not exist or it does
                 * Not contain audio profile.
                 */
                p_cc_pdb->flag = ICF_FALSE;

                /* This is the pointer to data of type stream capab from where Audio
                 * parameters will be extracted.
                 */
                p_cc_pdb->p_generic_ptr_1 = p_app_profile_node->p_payload;

                ret_val = icf_cc_fill_local_sdp_from_profile(p_cc_pdb);
            }
        }
        else
        {
            /* Control will be here if neither pref profile exists nor 
             * app_add profile exists, now we will create default Audio call.
             */
            ret_val = icf_cc_create_audio_call_by_default(p_cc_pdb);
        }
        if (ICF_FAILURE != ret_val)
        {
            /* Newly added fuction which will create node 
             * of preferred profile from local sdp recently       
             * created and add that node to p_cc_pdb-
             * >p_call_ctx->p_pref_media_profile.*/                          
            ret_val =  
                icf_cc_create_pref_profile_node_from_local_sdp                                                                   
                (p_cc_pdb);
            if (ICF_FAILURE == ret_val)
            {
                p_cc_pdb->common_bitmask |=
                    ICF_CC_STREAM_POS_VALIDATION_FAIL;
                ret_val = ICF_FAILURE;            
            } 
        }
    }
    /* Now if Audio profile is successfully created then add this node
     * to the list of streams i.e. p_cc_pdb->p_call_ctx->p_stream_sdp_info.
     */
    if (ICF_SUCCESS == ret_val)
    {
        p_cc_pdb->p_call_ctx->call_type |= ICF_CALL_TYPE_AUDIO;
        identifier = stream_id;
        ret_val = icf_cmn_add_node_to_internal_list(
                p_cc_pdb->p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                p_cc_pdb->p_stream_sdp_info,
                &identifier,
                ICF_NULL					);
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}




/*******************************************************************************
 *
 * FUNCTION:        icf_cc_populate_local_sdp
 *
 * DESCRIPTION:     This function populates local SDP from preferred media
 *                  profile if exists, else if media profile exists, populate
 *                  from it, otherwise fill default audio stream. This function
 *                  returns failure if preferred media profile is not subset of
 *                  media profile received in app_add.
 *
 ******************************************************************************/
icf_return_t
icf_cc_populate_local_sdp(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_list_st                    *p_pref_media_profile = ICF_NULL;
    icf_stream_capab_st			   *p_profile_data = ICF_NULL;
    icf_app_conf_st                *p_app_conf = ICF_NULL;
    icf_internal_list_st           *p_app_profile = ICF_NULL;
    icf_internal_list_st           *p_temp_node_internal = ICF_NULL;
    icf_stream_id_t				   stream_id = 1;
    icf_media_type_t			   stream_type = 1;
    icf_boolean_t				   at_least_one_hit = ICF_FALSE;
    icf_uint32_t                   identifier = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /*This variable will be containing pointer to list node of containing
          data part of type icf_stream_capab_st.*/
        p_pref_media_profile = p_cc_pdb->p_call_ctx->p_pref_media_profile;


    if (ICF_FALSE != p_cc_pdb->refer_init_call)
    {
        /* Fix for CSR 1-6727350: SPR 19316 */
		/* In case APP profile is configured and NFS call with Replaces is 
		 * received, then IPTK should create audio call by default; hence added
		 * the check the call media setup type.
		 * Without this check, there will be a loop in functions 
		 * icf_cc_populate_local_sdp & icf_cc_check_recvd_media_with_profile.
		 */
        if ((ICF_INVALID_CALL_ID != 
				p_cc_pdb->p_call_ctx->replace_initiated_call_id) &&
            (ICF_CALL_MEDIA_SETUP_TYPE == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        ICF_CALL_MEDIA_SETUP_TYPE)))
        {
           ret_val = icf_cc_check_recvd_media_with_profile(p_cc_pdb);
        }
        else
        {
           /* This call is initiated by some service.*/
           ret_val = icf_cc_create_audio_call(p_cc_pdb);
        }   
    }
    else
    {
        /* This call is initiated by PA.*/

        if (ICF_SUCCESS ==
                icf_cfg_fetch_app_conf(p_cc_pdb->p_glb_pdb,
                    p_cc_pdb->p_call_ctx->app_id, &p_app_conf,
                    p_cc_pdb->p_ecode) && ICF_NULL != p_app_conf)
        {
            p_app_profile = p_app_conf->p_app_media_profile;
        }

        /*Check if preferred media profile exists*/
        if (ICF_NULL != p_pref_media_profile)
        {
            /*Pref media profile exists.*/

            /*Traverse all the nodes present in pref profile.*/
            while (ICF_NULL != p_pref_media_profile)
            {
                p_profile_data = (icf_stream_capab_st *)
                    (p_pref_media_profile->p_data);

                /* Here we are just setting this variable call_type to
                 * accces the type of call in the function which we will call
                 * for checking if a stream is recurring.
                 */
                icf_cc_get_call_type(p_cc_pdb,p_profile_data->stream_type);

                if (ICF_TRUE == icf_cc_check_streams_for_reoccurence(p_cc_pdb))
                {
                    p_cc_pdb->common_bitmask |=
                        ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    break;
                }
                else
                {
                    /* Now we need to set some variables in p_cc_pdb, before
                     * calling icf_cc_fill_local_sdp_from_profile().
                     */

                    /* No need to set stream id because pref profile exists.*/

                    /* Initialise p_generic_ptr_1 with pref media profile's
                     * data part which will be of type icf_stream_capab_st.
                     */
                    p_cc_pdb->p_generic_ptr_1 = p_profile_data;

                    /* Because pref profile exists, therefore flg is TRUE.*/
                    p_cc_pdb->flag = ICF_TRUE;

                    /* Set the call type.*/
                    p_cc_pdb->p_call_ctx->call_type
                        |= p_cc_pdb->call_type;

                    /* if stream type is Fax then set p_generic_ptr_1 with ICF_NULL
                     * i.e. there is no need to create local sdp for it.But we will
                     * create icf_stream_sdp_info_st
                     * node for it, but mark it Inactive and NW_DISALLOW within the
                     * function icf_cc_fill_local_sdp_from_profile().
                     */
                    if (p_profile_data->stream_type == ICF_STREAM_TYPE_FAX)
                    {
                        /* set p_generic_ptr_1 as ICF_NULL because we need not to
                         * create local sdp for it.
                         */
                        p_cc_pdb->p_generic_ptr_1 = ICF_NULL;

                        /* Reset the call type bit.*/
                        p_cc_pdb->p_call_ctx->call_type
                            &= ( ~(p_cc_pdb->call_type));
                    }			
                    else if (ICF_NULL != p_app_profile)
                    {
                        /* App_add profile exists for validtion, otherwise if it
                         * would not have existed then we should have allowed all the
                         * streams i.e. we need not to do anything.
                         */

                        /* Now check if this stream exists then app_add profile, if it
                         * exists then need not to do anything,because node for this
                         * stream is already created.
                         * otherwise if this stream does not exists in app_add
                         * this stream INACTIVE and NW_DISALLOW.
                         */
                        identifier = p_profile_data->stream_type;
                        if (ICF_NULL == icf_cmn_get_node_in_internal_list(
                                    p_app_profile,&identifier)
                           )

                        {
                            /* This particular stream is NOT present in
                             * app_add profile.Set p_generic_ptr_1 as ICF_NULL.
                             */

                            p_cc_pdb->p_generic_ptr_1 = ICF_NULL;

                            /* Reset the call type bit.*/
                            p_cc_pdb->p_call_ctx->call_type
                                &= ( ~(p_cc_pdb->call_type));
                        }
                    }/*End of else if (ICF_NULL != p_app_profile) part*/


                    if(ICF_FAILURE ==
                            icf_cc_fill_local_sdp_from_profile(p_cc_pdb))
                    {
                        p_cc_pdb->common_bitmask |=
                            ICF_CC_STREAM_POS_VALIDATION_FAIL;
                        break;
                    }
                    else 
                    {
                        if (ICF_NULL == p_cc_pdb->p_generic_ptr_1)
                        {
                            p_cc_pdb->p_stream_sdp_info->stream_id
                                = p_profile_data->stream_id;

                            p_cc_pdb->p_stream_sdp_info->stream_type
                                = p_profile_data->stream_type;
                        }
                        else
                        {
                           if (ICF_FAILURE ==
                                icf_cc_update_pref_profile_node_from_local_sdp(p_cc_pdb))
                            {
                               p_cc_pdb->common_bitmask |=
                                    ICF_CC_STREAM_POS_VALIDATION_FAIL;
                               break;     
                            }
                        }
                        identifier = p_cc_pdb->p_stream_sdp_info->stream_id;

                        if(ICF_FAILURE == icf_cmn_add_node_to_internal_list(
                                    p_cc_pdb->p_glb_pdb,
                                    &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                                    p_cc_pdb->p_stream_sdp_info,
                                    &identifier,
                                    ICF_NULL))
                        {
                            p_cc_pdb->common_bitmask |=
                                ICF_CC_STREAM_POS_VALIDATION_FAIL;
                            break;
                        }
                        /* Fix corresponding to spr 14476.*/
                        /* Following function will access values contained in  
                         * p_cc_pdb->p_generic_ptr_1 and p_cc_pdb->p_stream_sdp_info.*/
                        /*if (ICF_FAILURE == icf_cc_update_pref_profile_node_from_local_sdp(p_cc_pdb))
                        {
                            p_cc_pdb->common_bitmask |=
                                ICF_CC_STREAM_POS_VALIDATION_FAIL;
                            break;
                        }*/
                        /* End of Fix corresponding to spr  14476.*/
                    }
                } /* End of if (ICF_TRUE == icf_cc_check_stre...*/

                p_pref_media_profile = p_pref_media_profile->p_next;

            }/* End of while (ICF_NULL != p_pref_media_profile)*/

        }   /* END of if (ICF_NULL != p_pref_media_profile)*/
        /* Pref profile does not exists, check if app_add exists or not.*/
        else if (ICF_NULL != p_app_profile)
        {
            /* app_add profile exists, now create nodes corresponding to
             * media streams Audio, Video and TBCP by extracting corresponding
             * parameters from app profile's stream capab list.
             */

            /* Traverse the loop for filling in these three streams
             * whichever exists.
             */
            while (stream_type <= ICF_STREAM_TYPE_TBCP)
            {
                /* Get node from app_add profile corresponding
                 * to stream_type.
                 */
                identifier = stream_type;
                p_temp_node_internal = icf_cmn_get_node_in_internal_list(
                        p_app_profile,
                        &identifier);


                if (ICF_NULL != p_temp_node_internal)
                {
                    /* This particular type of stream exists.*/

                    /* Now we need to set some variables in p_cc_pdb,
                     * before calling icf_cc_fill_local_sdp_
                     * from_profile().
                     */

                    /* Here we need to set stream id because pref
                     * profile does NOT exists.
                     */
                    p_cc_pdb->stream_id = stream_id;

                    /* Initialise p_generic_ptr_1 with pref
                     * media profile node.
                     */
                    p_cc_pdb->p_generic_ptr_1 = p_temp_node_internal->p_payload;

                    /* Because pref profile does NOT exists,therefore
                     * flag is FALSE.
                     */
                    p_cc_pdb->flag = ICF_FALSE;


                    if (ICF_SUCCESS ==
                            icf_cc_fill_local_sdp_from_profile(p_cc_pdb))
                    {
                        identifier = p_cc_pdb->stream_id; 
                        if (ICF_FAILURE == icf_cmn_add_node_to_internal_list(
                                    p_cc_pdb->p_glb_pdb,
                                    &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                                    p_cc_pdb->p_stream_sdp_info,
                                    &identifier,
                                    ICF_NULL))
                        {
                            p_cc_pdb->common_bitmask |=
                                ICF_CC_STREAM_POS_VALIDATION_FAIL;
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        /* Fix corresponding to spr  14476.*/

                        /* Newly added fuction which will create node 
                         * of preferred profile from local sdp recently       
                         * created and add that node to p_cc_pdb-
                         * >p_call_ctx->p_pref_media_profile.*/                         
                        ret_val =  
                            icf_cc_create_pref_profile_node_from_local_sdp
                            (p_cc_pdb);
                        if (ICF_FAILURE == ret_val)
                        {
                            p_cc_pdb->common_bitmask |=
                                ICF_CC_STREAM_POS_VALIDATION_FAIL;
                            break;
                        }
                        /* End of Fix corresponding to spr  14476.*/
                        stream_id++;

                        icf_cc_get_call_type(p_cc_pdb,((icf_stream_sdp_info_st *)
                                    p_temp_node_internal->p_payload)->stream_type);
                        /* Set the call type here.*/
                        p_cc_pdb->p_call_ctx->call_type
                            |= p_cc_pdb->call_type; 

                        at_least_one_hit = ICF_TRUE;


                    }
                    else /*Else part of if (ICF_SUCCESS == ret_val)*/
                    {
                        p_cc_pdb->common_bitmask |=
                            ICF_CC_STREAM_POS_VALIDATION_FAIL;
                        ret_val = ICF_FAILURE;
                        break;
                    }

                }/*End of if (ICF_NULL != p_temp_node_internal)*/

                stream_type++;
            }/*End of while (stream_type != ICF_STREAM_TYPE_TBCP)*/
            if (ICF_TRUE == at_least_one_hit && ICF_FAILURE != ret_val)
            {
                ret_val = ICF_SUCCESS;
            }
            else if (ICF_FALSE == at_least_one_hit)
            {
                p_cc_pdb->common_bitmask |=
                    ICF_CC_STREAM_POS_VALIDATION_FAIL;
            }
        }
        else /* Else part of if (ICF_NULL != p_app_profile)*/
        {
            /* Control will be here only if Neither pref profile exists
             * nor app_add profile exists, so simply create default audio
             * call.
             */
            if (ICF_SUCCESS == icf_cc_create_audio_call_by_default(p_cc_pdb))
            {
                identifier = p_cc_pdb->p_stream_sdp_info->stream_id;

                ret_val = icf_cmn_add_node_to_internal_list(
                        p_cc_pdb->p_glb_pdb,
                        &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                        p_cc_pdb->p_stream_sdp_info,
                        &(identifier),
                        ICF_NULL);

                if (ICF_FAILURE == ret_val)
                {
                    p_cc_pdb->common_bitmask |=
                        ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                }
                else
                {
                    /* Set the call type here.*/
                    p_cc_pdb->p_call_ctx->call_type |= ICF_CALL_TYPE_AUDIO;

                    /* Fix corresponding to spr 14476.*/

                    /* Newly added fuction which will create node 
                     * of preferred profile from local sdp recently       
                     * created and add that node to p_cc_pdb-
                     * >p_call_ctx->p_pref_media_profile.*/                          
                    ret_val = icf_cc_create_pref_profile_node_from_local_sdp(p_cc_pdb);
                    if (ICF_FAILURE == ret_val)
                    {
                        p_cc_pdb->common_bitmask |=
                            ICF_CC_STREAM_POS_VALIDATION_FAIL;
                        ret_val = ICF_FAILURE;
                    }
                    /* End of Fix corresponding to spr 14476.*/
                }

            }
        }	/* End of if (ICF_NULL != p_pref_media_profile)*/

    }/*End of if (ICF_FALSE != p_cc_pdb->refer_init_call)*/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cc_check_for_streams_deleted_by_user
 *
 * DESCRIPTION:    This function is invoked when Alert or connect is received 
 * 				   from the PA for incoming call and it will be used to mark 
 * 				   if application has deleted any streams then we need to mark 
 * 				   those streams deleted in the media_modify_type of their respective 
 * 				   icf_stream_sdp_info structure.
 *				   Before calling this function we will set p_cc_pdb->p_generic_ptr_1
 *				   with the pointer to array of deleted streams and p_cc_pdb->count
 *				   should be set with the no. of deleted streams count.
 *****************************************************************************/
icf_return_t icf_cc_check_for_streams_deleted_by_user
(
 icf_cc_pdb_st		*p_cc_pdb
 )
{
    icf_stream_list_st			*deleted_media_streams =
        (icf_stream_list_st *)(p_cc_pdb->p_generic_ptr_1);

    icf_internal_list_st		*p_stream_sdp_info_node = ICF_NULL;

    icf_uint8_t					counter = 0;

    icf_uint8_t					no_of_streams = 0;

    icf_return_t				ret_val = ICF_SUCCESS;

    icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

    icf_uint32_t			identifier = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


        /* This while loop will look for the streams deleted by the appn and will
         * mark them deleted in their corresponding icf_stream_sdp_info structure.
         */
        while (counter < p_cc_pdb->count)
        {
            identifier = deleted_media_streams[counter].stream_id;
            if (
                    (p_stream_sdp_info_node = icf_cmn_get_node_in_internal_list (
                                                                                 p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                                                                 &identifier))
                    != ICF_NULL
               )
            {
                p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
                    (p_stream_sdp_info_node->p_payload);
                if (ICF_MEDIA_STREAM_ACTIVE ==
                        (p_stream_sdp_info_data->stream_status & ICF_MEDIA_STREAM_ACTIVE))
                {
                    /* Trace for the stream rejected by user.*/
                    p_cc_pdb->p_generic_ptr_2 = p_stream_sdp_info_data;
                    ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_APP_REJECTED_STREAMS)

                        p_stream_sdp_info_data->media_modify_type
                        = ICF_STREAM_DELETE;

                    /* Resetting call type bitmask in call ctx.*/
                    icf_cc_get_call_type(p_cc_pdb,
                            p_stream_sdp_info_data->stream_type); 

                    p_cc_pdb->p_call_ctx->call_type &= ~(p_cc_pdb->call_type); 

                }
            }
            counter++;
        }/*End of while (counter < p_user_alerted->count)*/

    /* Following while loop will traverse each stream node in call ctx
     * and check if there is Not any active stream left, then we will return
     * ICF_CC_STREAM_POS_VALIDATION_FAIL bit ON in p_cc_pdb->common_bitmask
     * to issue generic error.
     */
    counter = 0;
    p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    while (ICF_NULL != p_stream_sdp_info_node)
    {
        no_of_streams++;
        p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
            (p_stream_sdp_info_node->p_payload);
        if ((ICF_MEDIA_STREAM_ACTIVE != (p_stream_sdp_info_data->stream_status
                        & ICF_MEDIA_STREAM_ACTIVE)) 
                || (ICF_STREAM_DELETE == (p_stream_sdp_info_data->media_modify_type
                        & ICF_STREAM_DELETE)))
        {
            counter++;
        }
        p_stream_sdp_info_node = (icf_internal_list_st *)
            (p_stream_sdp_info_node->p_next);
    }
    if (counter == no_of_streams)
    {
        p_cc_pdb->common_bitmask |=
            ICF_CC_ALL_STREAMS_DELETED;
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


/******************************************************************************
 *
 * FUNCTION:		icf_cc_update_stream_sdps_aftr_cms_resp
 *
 * DESCRIPTION:		This function is invoked to to mark streams Active, Deleted, NW
 *			allow, Disallow and free memory assigned to local and remote
 * 			sdps depending upon cms response for each stream.
 * *****************************************************************************/
icf_result_t icf_cc_update_stream_sdps_aftr_cms_resp
(
 icf_cc_pdb_st *p_cc_pdb
 )
{
    icf_return_t			ret_val = ICF_SUCCESS;

    /* This variable will be containing pointer to the structure which is
     * holding cms_resp from MMI.
     */
    icf_mmi_cms_resp_st		*p_mmi_cms_resp = ICF_NULL;

    icf_uint8_t				counter = 0;

    icf_uint8_t				no_of_failures = 0;

    icf_stream_id_t			stream_id;

    icf_uint32_t            identifier = 0;

    icf_internal_list_st	*p_stream_sdp_info_node=
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        p_mmi_cms_resp = (icf_mmi_cms_resp_st *)p_cc_pdb->
        p_internal_msg->p_msg_data;

    if (ICF_FALSE == p_mmi_cms_resp->result)
    {
#ifdef ICF_TRACE_ENABLE        
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)
        /* Tunnel mode specific trace */
        ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_TUNNELLED_CMS_RESP_RECVD)  
ICF_CHECK_TUNNEL_MODE_END
#endif

        ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_CMS_RESP_RECVD);
        p_cc_pdb->common_bitmask |= ICF_CC_ALL_STREAMS_DELETED;

        /* In tunnel mode, this trace should not be issued to the application
           as it is related to stream types and their status.
        */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
#ifdef ICF_TRACE_ENABLE
        p_cc_pdb->result = ICF_FAILURE;
        p_cc_pdb->stream_type = 0;
        ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_CREATE)
#endif
ICF_CHECK_TUNNEL_MODE_END
    }
    /* Issuing success CMS response trace in tunnel mode */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)  
    if (ICF_TRUE == p_mmi_cms_resp->result)
    {
#ifdef ICF_TRACE_ENABLE        
        /* Tunnel mode specific trace */
        ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_TUNNELLED_CMS_RESP_RECVD)  
#endif            
        ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_CMS_RESP_RECVD);
    }
ICF_CHECK_TUNNEL_MODE_END

    /* In tunnel mode, if the result is success, no further stream-wise
       processing is required. Thus bypassing the following code snippet.
    */   
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)
    if (p_mmi_cms_resp->result == ICF_TRUE)
    {
        /* Following while loop is just checking the appropriate stream
         * ids for which cms was issued and if issued then whether it is
         * successful or not and act accordingly.
         */
        while ( (counter != p_mmi_cms_resp->count) && (counter < ICF_MAX_STREAM_PER_CALL) )
        {
            p_cc_pdb->count = counter;
            ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_CMS_RESP_RECVD);
            stream_id = (p_mmi_cms_resp->stream_status[counter]).stream_id;

            identifier = stream_id;

            p_stream_sdp_info_node = icf_cmn_get_node_in_internal_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                    &identifier);

            if(ICF_NULL == p_stream_sdp_info_node)
            {
                ret_val = ICF_FAILURE;
                break;
            }

            p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
                (p_stream_sdp_info_node->p_payload);

            /* cms req for this particular stream was given. */
            if (ICF_FALSE == (p_mmi_cms_resp->
                        stream_status[counter]).status_success)
            {
#ifdef ICF_TRACE_ENABLE
        p_cc_pdb->result = ICF_FAILURE;
        p_cc_pdb->stream_type = p_stream_sdp_info_data->stream_type;
        ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_CREATE)
#endif
                no_of_failures++;

                /* cms for this stream is not successfull.*/
                /* Mark this stream as INACTIVE.*/
                p_stream_sdp_info_data->stream_status &= ~ICF_MEDIA_STREAM_ACTIVE;

                /* Reset the call type bit in call context for this 
                 * particular stream.
                 */

                icf_cc_get_call_type(p_cc_pdb,
                        p_stream_sdp_info_data->stream_type); 

                p_cc_pdb->p_call_ctx->call_type &= ~(p_cc_pdb->call_type);

                if (ICF_NULL != p_stream_sdp_info_data->p_local_sdp)
                {
                    ret_val = icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb,
                            &(p_stream_sdp_info_data->p_local_sdp),
                            p_cc_pdb->p_ecode);
                }
                if (ICF_NULL != p_stream_sdp_info_data->p_remote_received_sdp)
                {
                    /*  Fix Merge for CSR 1-6449001 */                    
                    /*  Copy the remote receivied codec list */
                    if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                             p_cc_pdb->p_glb_pdb,
                             &(p_stream_sdp_info_data->p_remote_recvd_codec_list),
                             p_stream_sdp_info_data->p_remote_received_sdp->
                             sdp_stream.p_codec_info_list))
 
                    {
                        ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        ret_val = icf_cmn_delete_internal_sdp_stream(
                             p_cc_pdb->p_glb_pdb,
                             &(p_stream_sdp_info_data->p_remote_received_sdp),
                             p_cc_pdb->p_ecode);
                    }
                }
                if (ICF_FAILURE == ret_val)
                    break;

                if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                            ICF_CALL_DIR))
                {
                    /* It is an outgoing call.*/
                    /* Marking stream as NW-DISALLOW by resetting this NW_ALLOW bit.*/
                    p_stream_sdp_info_data->stream_status &= 
                        ~ICF_MEDIA_STREAM_NW_ALLOW;
                }

            }/*End of if (ICF_FALSE == (p_mmi_cms_resp->....*/

            else
            {
#ifdef ICF_TRACE_ENABLE
                 p_cc_pdb->result = ICF_SUCCESS;
                 p_cc_pdb->stream_type = p_stream_sdp_info_data->stream_type;
#endif
                /* For incoming call Applied SDP to be needed */
                if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb, ICF_CALL_DIR) &&
                        ICF_NULL != p_stream_sdp_info_data->p_remote_received_sdp 
                                     && ICF_STREAM_TYPE_TBCP != p_stream_sdp_info_data->stream_type )
                {
                    p_stream_sdp_info_data->p_applied_sdp->media_mode =
                        p_stream_sdp_info_data->p_remote_received_sdp->media_mode;
#ifdef ICF_TRACE_ENABLE
                    p_cc_pdb->derived_mode = 
                       p_stream_sdp_info_data->p_remote_received_sdp->media_mode;

                   p_cc_pdb->remote_rtp_addr =
                       p_stream_sdp_info_data->p_remote_received_sdp->sdp_stream.
                           stream_address;
                   p_cc_pdb->local_rtp_addr =
                       p_stream_sdp_info_data->p_local_sdp->sdp_stream.
                           stream_address;

                   p_cc_pdb->p_codec_info_list = 
                        p_stream_sdp_info_data->p_remote_received_sdp->sdp_stream.
                            p_codec_info_list;
#endif
                } 
                else 
                {
#ifdef ICF_TRACE_ENABLE
                   p_cc_pdb->derived_mode = 
                       p_stream_sdp_info_data->p_local_sdp->media_mode;

                   p_cc_pdb->local_rtp_addr =
                       p_stream_sdp_info_data->p_local_sdp->sdp_stream.
                           stream_address;

                   p_cc_pdb->p_codec_info_list = 
                        p_stream_sdp_info_data->p_local_sdp->sdp_stream.
                            p_codec_info_list;
#endif
                }
                 ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_CREATE)
            }
            counter++;
        }/*End of while (counter != p_mmi_cms_resp->count)...*/
        if (no_of_failures == p_mmi_cms_resp->count)
        {
            p_cc_pdb->common_bitmask |= ICF_CC_ALL_STREAMS_DELETED;
        }
    }
ICF_CHECK_TUNNEL_MODE_END

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

#ifdef ICF_TRACE_ENABLE

/******************************************************************************
 *
 * FUNCTION:		icf_cc_update_trace_info_aftr_mms_resp
 *
 * DESCRIPTION:		This function is invoked to to mark streams Active, Deleted, NW
 *			allow, Disallow and free memory assigned to local and remote
 * 			sdps depending upon cms response for each stream.
 * *****************************************************************************/
icf_result_t icf_cc_update_trace_info_aftr_mms_resp
(
 icf_cc_pdb_st *p_cc_pdb
 )
{
    icf_return_t			ret_val = ICF_SUCCESS;

    /* This variable will be containing pointer to the structure which is
     * holding cms_resp from MMI.
     */
    icf_mmi_mms_resp_st		*p_mmi_mms_resp = ICF_NULL;

    icf_uint8_t				counter = 0;

    icf_stream_id_t			stream_id;

    icf_uint32_t            identifier = 0;

    icf_internal_list_st	*p_stream_sdp_info_node=
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        p_mmi_mms_resp = (icf_mmi_mms_resp_st *)p_cc_pdb->
        p_internal_msg->p_msg_data;

    if (p_mmi_mms_resp->result == ICF_FALSE)
    {
        p_cc_pdb->result = ICF_FAILURE;
        p_cc_pdb->stream_type = 0;
        ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_MODIFY)
    }
    else
    {
        /* SPR 18991: MMS response type set as NULL means complete
           offer is accepted. Update the applied SDP mode only if 
           the complete offer is accepted.
         */  
        if(ICF_NULL == p_cc_pdb->mms_resp_type)
        {
            /* CSR 1-6212448: SPR 18891: 
               Update the applied sdp mode in applied sdp*/
		    icf_cc_update_applied_sdp_mode(p_cc_pdb);
        }        
        /* Following while loop is just checking the appropriate stream
         * ids for which cms was issued and if issued then whether it is
         * successful or not and act accordingly.
         */
        while (counter != p_mmi_mms_resp->mod_stream_count)
        {
            p_cc_pdb->count = counter;
            if (counter < ICF_MAX_STREAM_PER_CALL ) 
				stream_id = (p_mmi_mms_resp->modified_stream_resp[counter]).stream_id;

            identifier = stream_id;

            p_stream_sdp_info_node = icf_cmn_get_node_in_internal_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                    &identifier);

            if(ICF_NULL == p_stream_sdp_info_node)
            {
                ret_val = ICF_FAILURE;
                break;
            }

            p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
                (p_stream_sdp_info_node->p_payload);
            
            if(ICF_MEDIA_STREAM_ACTIVE !=
                (p_stream_sdp_info_data->stream_status & ICF_MEDIA_STREAM_ACTIVE))
            {
                counter++;
                continue;
            }

            if(ICF_MEDIA_STREAM_ACTIVE !=
                (p_stream_sdp_info_data->stream_status & ICF_MEDIA_STREAM_ACTIVE))
            {
                counter++;
                continue;
            }

            if(ICF_MEDIA_STREAM_ACTIVE !=
                (p_stream_sdp_info_data->stream_status & ICF_MEDIA_STREAM_ACTIVE))
            {
                counter++;
                continue;
            }
 
            /* cms req for this particular stream was given. */
            if (( counter < ICF_MAX_STREAM_PER_CALL) && (ICF_FALSE == (p_mmi_mms_resp->
                        modified_stream_resp[counter]).status_success))
            {
                p_cc_pdb->result = ICF_FAILURE;
                p_cc_pdb->stream_type = p_stream_sdp_info_data->stream_type;
                ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_MODIFY)

            }/*End of if (ICF_FALSE == (p_mmi_cms_resp->....*/
            else
            {
                 p_cc_pdb->result = ICF_SUCCESS;
                 p_cc_pdb->stream_type = p_stream_sdp_info_data->stream_type;
                 
                 if(ICF_NULL != p_stream_sdp_info_data->p_remote_received_sdp)
                 {     
                 p_cc_pdb->remote_rtp_addr =
                      p_stream_sdp_info_data->p_remote_received_sdp->sdp_stream.
                                        stream_address;
                 }
                 if(ICF_NULL != p_stream_sdp_info_data->p_local_sdp)
                 {
                 p_cc_pdb->local_rtp_addr =
                       p_stream_sdp_info_data->p_local_sdp->sdp_stream.
                           stream_address;
                 }

                 if(ICF_STREAM_TYPE_TBCP != p_stream_sdp_info_data->stream_type)
                 {
                    if(ICF_NULL != p_stream_sdp_info_data->p_applied_sdp)
                    {
                    p_cc_pdb->p_codec_info_list =
                        p_stream_sdp_info_data->p_applied_sdp->sdp_stream.
                            p_codec_info_list;
                    p_cc_pdb->derived_mode = 
                       p_stream_sdp_info_data->p_applied_sdp->media_mode;
                    }
                 }
                 
                 ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_MODIFY)
            }
            counter++;
        }/*End of while (counter != p_mmi_cms_resp->count)...*/

        counter = 0;

        while ((counter != p_mmi_mms_resp->add_stream_count) && (counter < ICF_MAX_STREAM_PER_CALL))
        {
            p_cc_pdb->count = counter;
            stream_id = (p_mmi_mms_resp->added_stream_resp[counter]).stream_id;

            identifier = stream_id;

            p_stream_sdp_info_node = icf_cmn_get_node_in_internal_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                    &identifier);

            if(ICF_NULL == p_stream_sdp_info_node)
            {
                ret_val = ICF_FAILURE;
                break;
            }

            p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
                (p_stream_sdp_info_node->p_payload);

            /* cms req for this particular stream was given. */
            if (ICF_FALSE == (p_mmi_mms_resp->
                        added_stream_resp[counter]).status_success)
            {
                p_cc_pdb->result = ICF_FAILURE;
                p_cc_pdb->stream_type = p_stream_sdp_info_data->stream_type;
                ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_MODIFY)

            }/*End of if (ICF_FALSE == (p_mmi_cms_resp->....*/
            else
            {
                 p_cc_pdb->result = ICF_SUCCESS;
                 p_cc_pdb->stream_type = p_stream_sdp_info_data->stream_type;
                 
                 if(ICF_NULL != p_stream_sdp_info_data->p_applied_sdp)
                 {
                 p_cc_pdb->derived_mode = 
                       p_stream_sdp_info_data->p_applied_sdp->media_mode;
                 p_cc_pdb->p_codec_info_list = 
                        p_stream_sdp_info_data->p_applied_sdp->sdp_stream.
                            p_codec_info_list;
                 }
		 else
                 {
                 p_cc_pdb->derived_mode = 
                       p_stream_sdp_info_data->p_local_sdp->media_mode;
                 p_cc_pdb->p_codec_info_list = 
                        p_stream_sdp_info_data->p_local_sdp->sdp_stream.
                            p_codec_info_list;
                 }

                 if(ICF_NULL != p_stream_sdp_info_data->p_remote_received_sdp)
                 {
                 p_cc_pdb->remote_rtp_addr =
                       p_stream_sdp_info_data->p_remote_received_sdp->sdp_stream.
                           stream_address;
                 }
                 
                 p_cc_pdb->local_rtp_addr =
                       p_stream_sdp_info_data->p_local_sdp->sdp_stream.
                           stream_address;
              
                 ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_STREAM_MODIFY)
            }
            counter++;
        }/*End of while (counter != p_mmi_cms_resp->count)...*/


    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


#endif

#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_stream_from_profile
 *
 * DESCRIPTION:     This function validates whether this particular stream_type
 *					passed in the function exists in app_add profile or not.
 ******************************************************************************/
icf_boolean_t icf_cc_validate_stream_from_profile
(
 icf_cc_pdb_st		*p_cc_pdb,
 icf_media_type_t	stream_type,
 icf_internal_list_st			*p_app_profile
 )
{
    icf_return_t	ret_val = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        while (ICF_NULL != p_app_profile)
        {
            if (((icf_stream_capab_st *)(p_app_profile->p_payload))->stream_type
                    == stream_type)
            {
                ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                    return ICF_TRUE;
            }
            p_app_profile = (icf_internal_list_st *)(p_app_profile->p_next);
        }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}
#endif


/******************************************************************************
 *
 * FUNCTION:      icf_cc_fill_local_sdp_checking_incoming_and_appadd_profile
 *
 * DESCRIPTION:   This function is invoked to validate the incoming streams with
 *		 app_add profile that which streams are allowed and which are
 *		not.
 ******************************************************************************/

icf_boolean_t icf_cc_fill_local_sdp_checking_incoming_and_appadd_profile
(
 icf_cc_pdb_st		*p_cc_pdb,
 icf_app_conf_st	*p_app_conf
 )
{
    /*This variable will contain pointer to node of list p_stream_sdp_info. */
    icf_internal_list_st		*p_stream_sdp_info_node=p_cc_pdb->
        p_call_ctx->p_stream_sdp_info;

    /* This list will be pointing to app_add profile, which is list of
     * type icf_stream_capab_st.
     */
    icf_internal_list_st		*p_app_profile_node =
        p_app_conf->p_app_media_profile;

    icf_internal_list_st		*p_temp_node = ICF_NULL;

    icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;

    icf_return_t				ret_val = ICF_SUCCESS;

    icf_uint32_t				identifier = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        p_cc_pdb->flag = ICF_FALSE;

    /* Following while loop will check one by one whether the media stream
     * obtained from SSA in an incoming call is existing in app_add profile or not,
     * if it is existing then need not to do anything and if stream is not 
     * present in app_add profile then simply Reset stream_status bitmask with bit
     * ICF_MEDIA_STREAM_ACTIVE, but NW_ALLOW bit ON and also reset the call type 
     * bit for this stream in call context.
     */

    while (p_stream_sdp_info_node != NULL)
    {
        p_stream_sdp_info = (icf_stream_sdp_info_st *)
            (p_stream_sdp_info_node->p_payload);
        if (ICF_MEDIA_STREAM_ACTIVE == 
                (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE))
        {
            /* Now validate if stream exists in app_add profile. */
            identifier = p_stream_sdp_info->stream_type;
            if ((p_temp_node = icf_cmn_get_node_in_internal_list(
                            p_app_profile_node,&identifier))
                    == ICF_NULL
               )
            {
                /* Control will be here if this particular stream is not present in
                 * app_add profile then simply mark this stream as rejected but it is
                 * NWALLOW.
                 */

                p_stream_sdp_info->stream_status &= (~ICF_MEDIA_STREAM_ACTIVE);

                icf_cc_get_call_type(p_cc_pdb,p_stream_sdp_info->stream_type);
                p_cc_pdb->p_call_ctx->call_type &= ~(p_cc_pdb->call_type);
            }
            else
            {
                /* Fill local sdp from app_add.*/

                /* This pointer will be type casted to get icf_stream_capab_st
                 * to get data.
                 */
                p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
                p_cc_pdb->p_generic_ptr_1 = p_temp_node->p_payload;
                ret_val = icf_cc_fill_local_sdp_from_stream_capab(p_cc_pdb);

                /* Fix corresponding to spr 14476.*/
                if (ICF_FAILURE != ret_val)
                {
                    /* Newly added fuction which will create node 
                     * of preferred profile from local sdp recently       
                     * created and add that node to p_cc_pdb-
                     * >p_call_ctx->p_pref_media_profile.*/                          
                    ret_val =  
                        icf_cc_create_pref_profile_node_from_local_sdp(p_cc_pdb);
                    if (ICF_SUCCESS != ret_val )
                    {
                        break;
                    }   
                }
                /* End of Fix corresponding to spr 14476.*/
            }

        }/*End of if (ICF_MEDIA_STREAM_ACTIVE ==... */


        p_stream_sdp_info_node = (icf_internal_list_st *)
            (p_stream_sdp_info_node->p_next);
    }/*End of while (p_stream_sdp_info_node != NULL)*/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

        return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_recvd_media_with_profile
 *
 * DESCRIPTION:     This function compares remote received media streams with
 *                  media profile received in app_add request.
 ******************************************************************************/
icf_return_t   icf_cc_check_recvd_media_with_profile
(
 INOUT   icf_cc_pdb_st     *p_cc_pdb
 )
{
    icf_return_t ret_val = ICF_SUCCESS;
    icf_internal_list_st   *p_app_profile = ICF_NULL;
    icf_uint32_t			identifier = 0;

    /* This variable is used to store pointer to
     * application's configuration.*/
    icf_app_conf_st  *p_app_conf = ICF_NULL;

    /* This variable will contain pointer to node
     * of list p_stream_sdp_info. */
    icf_internal_list_st	*p_stream_sdp_info_node=p_cc_pdb->
        p_call_ctx->p_stream_sdp_info;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Fetch the app_conf for this app */
        if (ICF_SUCCESS == icf_cfg_fetch_app_conf(
                    p_cc_pdb->p_glb_pdb,
                    p_cc_pdb->p_call_ctx->app_id,
                    &p_app_conf,
                    p_cc_pdb->p_ecode) && ICF_NULL != p_app_conf)
        {
            p_app_profile = p_app_conf->p_app_media_profile;
        }

    if (ICF_NULL != p_app_profile)
    {
        /* Control will be here if Application configuration exist
         * and now we will check whether it is a Fast start call
         * or Non Fast start call.
         */

        if (ICF_NULL == p_stream_sdp_info_node)
        {
            /* It is a Non Fast start call and app_add profile exists. */

            ret_val=icf_cc_populate_local_sdp(p_cc_pdb);
        }
        else
        {
            /* it is a Fast start call and app_add profile exists. */

            ret_val=icf_cc_fill_local_sdp_checking_incoming_and_appadd_profile
                (p_cc_pdb,p_app_conf);

            /* Fix corresponding to spr 14476.*/
            /* This particular change will make sure that in  
             * incoming call, we will send local codecs in gcl 
             * request.*/
            p_cc_pdb->p_call_ctx->common_bitmask |= ICF_OFFER_CODECS_IN_GCL;

            /* End of Fix corresponding to spr 14476.*/
        }
    }
    else
    {
        /*
         * Application configuration does not exist, now check
         * whether it is Non Fast start call, if it is Non FS call
         * then simply create audio stream by default otherwise nothing
         * to do because we will allow all streams.
         */
        if (p_stream_sdp_info_node == ICF_NULL)
        {
            /*it is a Non Fast start call, then simply create Audio stream. */
            ret_val = icf_cc_create_audio_call_by_default(p_cc_pdb);
            if (ICF_FAILURE != ret_val)
            {
                identifier = p_cc_pdb->p_stream_sdp_info->stream_id;
                ret_val = icf_cmn_add_node_to_internal_list(
                        p_cc_pdb->p_glb_pdb,
                        &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                        p_cc_pdb->p_stream_sdp_info,
                        &(identifier),
                        ICF_NULL);
                p_cc_pdb->p_call_ctx->call_type |= ICF_CALL_TYPE_AUDIO;        

                /* Fix corresponding to spr 14476.*/
                if (ICF_FAILURE != ret_val)
                {
                    /* Newly added fuction which will create node 
                     * of preferred profile from local sdp recently       
                     * created and add that node to p_cc_pdb-
                     * >p_call_ctx->p_pref_media_profile.*/                          
                    ret_val =  
                        icf_cc_create_pref_profile_node_from_local_sdp(p_cc_pdb);
                }
                /* End of Fix corresponding to spr 14476.*/
            }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}



/******************************************************************************
 *
 * FUNCTION:        	icf_cc_make_mms_type_for_rem_succ_ans
 *
 * DESCRIPTION:     This function is invoked to derive appropriate MMS type after
 *				   receiving Answer to our INVITE from remote i.e. it is First
 *				   answer And that answer will be 200 ok or 183, NOT 4xx.
 * This Function:
 *			we need not to do anything in case our offer is completely rejected
 *			by remote, we will just take care of the success response.
 *
 * 			Here every stream is a new stream.
 *
 *			And we will check only those streams which are ACTIVE.
 *
 * 			In case of stream got accepted by remote, we will set remote codec
 *			commit bit in media_modify_type of icf_stream_sdp_info.
 *
 *			In case stream is rejected by remote then we set Delete bit for this
 *			particular stream(and reset others) in media_modify_type bitmask
 *			of structure icf_stream_sdp_info ,but don't mark that stream as inactive
 *			, because icf_cc_set_media_modify_type() function will take care of that.
 *			Release the memory associated with Local sdp.
 *
 *			Before calling this function, p_cc_pdb->p_list_1 must be initialised
 *			with the remote list's answer.
 *****************************************************************************/

icf_return_t icf_cc_make_mms_type_for_rem_succ_ans
(
 icf_cc_pdb_st 	*p_cc_pdb
 )
{
    icf_internal_list_st	*p_stream_sdp_info_node=p_cc_pdb->
        p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

    icf_return_t			ret_val=ICF_SUCCESS;

    icf_boolean_t			success_for_atleast_one_stream
        = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* Function entry trace */

        while(p_stream_sdp_info_node != NULL)
        {
            p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
                (p_stream_sdp_info_node->p_payload);

            ICF_CHECK_MULT_ANS_ENBL_START(p_cc_pdb->p_glb_pdb)

             /* if stream is asked for deletion , mark it pending
                do not delete now, it will be on the basis of final sdp */
               if ( ICF_STREAM_DELETE ==
                        (p_stream_sdp_info_data->media_modify_type & 
                                ICF_STREAM_DELETE)
                   )
                {
                   /*In case of precondition call do not set PENDING DELETE
                    *as in this case we need to send updated SDP in UPDATE
                    *after 183. So when call is not connected and PRECOND
                    *call is not set then only set PENDING_DELETE*/
                   if((ICF_PRECONDITION_REQD != 
                      (p_cc_pdb->p_call_ctx->common_bitmask_2 & ICF_PRECONDITION_REQD)) && 
                      (ICF_CC_SSA_CALL_CONNECT != p_cc_pdb->current_event))
                   {
                       p_stream_sdp_info_data->media_modify_type |=
                             ICF_STREAM_PENDING_DELETE;

                       p_stream_sdp_info_data->media_modify_type &= 
                             ~(ICF_STREAM_DELETE);
                   }
	        }

            ICF_CHECK_MULT_ANS_ENBL_END

            if (ICF_MEDIA_STREAM_ACTIVE ==
                    (p_stream_sdp_info_data->stream_status &
                     ICF_MEDIA_STREAM_ACTIVE)
               )
            {
                /* Here we will check offer response only for those
                 * streams which are ACTIVE.
                 */

                if ( ICF_STREAM_DELETE ==
                        (p_stream_sdp_info_data->media_modify_type & ICF_STREAM_DELETE)
                   )
                {
                    /*Control will be here if offor this stream offer is rejected. */
                    /* We will set the Bit for deletion here.*/
                    p_stream_sdp_info_data->media_modify_type = ICF_STREAM_DELETE;

                    ret_val = icf_cmn_delete_internal_sdp_stream
                        (p_cc_pdb->p_glb_pdb,
                         &(p_stream_sdp_info_data->p_local_sdp),
                         p_cc_pdb->p_ecode);
                }
                else if(ICF_STREAM_PENDING_DELETE !=
                        (p_stream_sdp_info_data->media_modify_type & ICF_STREAM_PENDING_DELETE))
                {
                    /* Control will be here if offer for this stream has
                     * been accepted.
                     */

                    /* we need not to Assign pointer of remote received sdp
                     * from icf_inc_media_sdp_info to the remote_received_sdp
                     * of  p_stream_sdp_info, because function which is
                     * validating ith line matching will take care of that.
                     */
                    success_for_atleast_one_stream = ICF_TRUE;
                    p_stream_sdp_info_data->media_modify_type |=
                        ICF_REMOTE_ANSWER_CODEC_CHANGE;

                    p_cc_pdb->application_mode = 
                        p_stream_sdp_info_data->application_mode;
                    p_cc_pdb->remote_mode =
                        p_stream_sdp_info_data->p_remote_received_sdp->media_mode;

                    if(ICF_STREAM_TYPE_TBCP != 
                            p_stream_sdp_info_data->stream_type)
                    {
                        p_cc_pdb->application_mode = 
                            p_stream_sdp_info_data->application_mode;
                        p_cc_pdb->remote_mode =
                            p_stream_sdp_info_data->p_remote_received_sdp->
                            media_mode;

                        /*If the p_early mode is present, then simply apply that mode.*/
                       if(ICF_EARLY_MEDIA_HDR_SUPPORTED & p_cc_pdb->p_call_ctx->common_bitmask_2)
                       {
                         p_cc_pdb->derived_mode = p_cc_pdb->remote_mode;
                         if (ICF_MEDIA_MODE_SEND_ONLY == p_cc_pdb->remote_mode)
                         {
                             p_cc_pdb->derived_mode = ICF_MEDIA_MODE_RECV_ONLY; 
                         }
                         else if (ICF_MEDIA_MODE_RECV_ONLY == p_cc_pdb->remote_mode)
                         {
                             p_cc_pdb->derived_mode = ICF_MEDIA_MODE_SEND_ONLY;
                         }
                       }
                       else
                       {
                         icf_cc_derive_mode(p_cc_pdb);
                       }

                        if(p_stream_sdp_info_data->mm_applied_mode != 
                                p_cc_pdb->derived_mode)
                        {
                            p_stream_sdp_info_data->mm_applied_mode = 
                                p_cc_pdb->derived_mode;
                            p_stream_sdp_info_data->media_modify_type |=
                                ICF_MODE_CHANGE;
                        }
                    }

                }/*End of if ( ICF_INC_MEDIA_STREAM_DELETED ==...)*/

            }/*End of if (ICF_MEDIA_STREAM_ACTIVE ==..*/
            p_stream_sdp_info_node = (icf_internal_list_st *)
                (p_stream_sdp_info_node->p_next);

        }/* End of while(p_stream_sdp_info_node != NULL)....*/
    if (ICF_FALSE == success_for_atleast_one_stream)
    {
        ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_NO_ACTIVE_STREAMS_LEFT)
            p_cc_pdb->common_bitmask |= ICF_CC_ALL_STREAMS_DELETED;
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:    icf_cc_remote_revert_modify_type
 *
 * DESCRIPTION: This function is invoked to revert to previous stream parameters,
 *	           if any modification was done on this stream. Before invoking this
 *	           function we need to set p_cc_pdb->p_stream_sdp_info
 ********************************************************************************/
icf_return_t icf_cc_remote_revert_modify_type
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_return_t			ret_val = ICF_SUCCESS;

    icf_stream_sdp_info_st	*p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
        if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                    & ICF_LOCAL_SDP_CODEC_CHANGE))
        {
            /* Codec change was offered for this stream.*/

            /* Reset the Local sdp codec change bit.*/
            p_stream_sdp_info->media_modify_type
                &= (~ICF_LOCAL_SDP_CODEC_CHANGE);

            /* Set the Remote answer revert codec change bit.*/
            p_stream_sdp_info->media_modify_type
                |= ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE;
        }
        else if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                    & ICF_LOCAL_SDP_MEDIA_CHANGE))
        {
            /* Local sdp change was offered. */

            /* Reset the Local sdp media change bit.*/
            p_stream_sdp_info->media_modify_type
                &= (~ICF_LOCAL_SDP_MEDIA_CHANGE);

            /* Set the Local answer revert sdp change bit.*/
            p_stream_sdp_info->media_modify_type |= ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE;
        }
    if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                & ICF_MODE_CHANGE))
    {
        /* Mode change was sent in the offer.*/

        /* No need to Reset Mode change bit, because we will have to revert
         * to previous mode in mms, so mms should indicate that again mode
         * change has to be done.
         */

        ret_val = icf_cc_calc_mode_remote_reject_change(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}




/***********************************************************************
 * FUNCTION:		icf_cc_modification_accepted_for_stream
 *
 * DESCRIPTION:	This is valid in case our modification offer for this particular
 *				stream has been accepted.
 *				if in our offer we sent local codec change then
 *					set the bit remote answer Codec change
 *					And Reset the bit local codec change.
 *				else
 *					call the function which will compare remote sdp with applied sdp
 *				call the function remote_mode_change()
 * NOTE:			Before calling this function we need to set p_cc_pdb->p_stream_sdp_info
 *				with structure of this stream.
 ************************************************************************/
icf_return_t icf_cc_modification_accepted_for_stream
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_return_t	ret_val = ICF_SUCCESS;
    icf_return_t	ret_value = ICF_SUCCESS;

    icf_stream_sdp_info_st	*p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;
    if(ICF_NULL == p_cc_pdb->p_glb_pdb) 
    {
        ICF_PRINT(((icf_uint8_t *)"[CC]:GLB PDB is null"));
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */
    /*Start SPR 19015 */    
    ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
            p_stream_sdp_info, ret_value)
    if(ICF_FAILURE == ret_value)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ICF_FAILURE;    
    }/*End SPR 19015 */

        if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                    & ICF_LOCAL_SDP_MEDIA_CHANGE))
        {
            /* Reset the Local sdp media change bit as remote media is applied over here.*/
            p_stream_sdp_info->media_modify_type
                &= (~ICF_LOCAL_SDP_MEDIA_CHANGE);
       }
 
        if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                    & ICF_LOCAL_SDP_CODEC_CHANGE))
        {
            /* Codec change was offered for this stream.*/

            /* Reset the Local sdp codec change bit.*/
            p_stream_sdp_info->media_modify_type
                &= (~ICF_LOCAL_SDP_CODEC_CHANGE);

            /* Set the Remote answer codec change bit.*/
            p_stream_sdp_info->media_modify_type
                |= ICF_REMOTE_ANSWER_CODEC_CHANGE;
        }
        else
        {
            p_stream_sdp_info->media_modify_type = 0;
#ifdef ICF_SESSION_TIMER
            /*Start SPR 19015 */    
            ICF_CHECK_IF_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                 p_cc_pdb->p_glb_pdb->p_call_ctx, ret_value)
            if(ICF_FAILURE == ret_value)
            {
               ret_val = ICF_FAILURE;
               ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
               return ICF_FAILURE;    
            } /*End SPR 19015 */
            /*  SPR 17993 Merge */
            if ((ICF_TRUE == p_cc_pdb->p_glb_pdb->p_call_ctx->session_timer_enable)&&
            (ICF_CC_SESSION_REFRESH_RESP == ICF_CC_GET_ONGOING_EVENT(p_cc_pdb)))
            {
                 if(ICF_FALSE == icf_cc_compare_rem_media_params(p_cc_pdb))
                  {
                      p_stream_sdp_info->media_modify_type =
                                     ICF_REMOTE_SDP_MEDIA_CHANGE;
                  }
            }
         else
            {
#endif
               if((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type) ||
                    (ICF_STREAM_TYPE_VIDEO == p_stream_sdp_info->stream_type))
               {
                  /*Start SPR 19015 */    
                  ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb, \
                          p_stream_sdp_info->p_remote_received_sdp, \
                          p_stream_sdp_info->p_applied_sdp,
                          ret_value)
                  if(ICF_FAILURE == ret_value)
                  {
                       ret_val = ICF_FAILURE;
                       ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
                       return ICF_FAILURE;    
                  }/*End SPR 19015 */
                  p_cc_pdb->p_list_1 = p_stream_sdp_info->p_remote_received_sdp->
                     sdp_stream.p_codec_info_list;
                  p_cc_pdb->p_list_2 = p_stream_sdp_info->p_applied_sdp->
                    sdp_stream.p_codec_info_list;
                  if (ICF_FALSE == icf_cc_compare_codec_lists(p_cc_pdb))
                   {
                     /* Set bit for modification of remote SDP
                      * Remaining SDP doesn't need to be compared
                      */
                      p_stream_sdp_info->media_modify_type |=
                        ICF_REMOTE_ANSWER_CODEC_CHANGE;
                   }
               }
               if(ICF_REMOTE_ANSWER_CODEC_CHANGE != 
                    (p_stream_sdp_info->media_modify_type &  ICF_REMOTE_ANSWER_CODEC_CHANGE))
                {
                  if(ICF_FALSE == icf_cc_compare_rem_media_params(p_cc_pdb))
                   {
                      p_stream_sdp_info->media_modify_type =
                        ICF_REMOTE_SDP_MEDIA_CHANGE;
                   }
                }
#ifdef ICF_SESSION_TIMER
            }    
#endif
        }


    if (ICF_SUCCESS == ret_val && 
            ICF_STREAM_TYPE_TBCP != p_stream_sdp_info->stream_type)
    {
        ret_val = icf_cc_check_remote_mode_change(p_cc_pdb);
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_for_modify_type
 *
 * DESCRIPTION:     This function is invoked to check if modification(if there was any)
 *				   has been accepted or rejected, if it was rejected then call function
 *				   for reverting to older parameters And if it was accepted then call
 *				   function for acting accordingly.
 *				   Before invoking this function we need to set p_cc_pdb->p_stream_sdp_info
 ********************************************************************************/
icf_return_t icf_cc_check_for_modify_type
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_return_t	ret_val = ICF_SUCCESS;

    icf_stream_sdp_info_st	*p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        if (ICF_STREAM_DELETE == (p_stream_sdp_info->media_modify_type
                    & ICF_STREAM_DELETE))
        {
            /* Offer change for this stream has been rejected by remote,
             * in case if there was any modification for this stream.
             */
            /* No need to set p_cc_pdb->p_stream_sdp_info here because
             * it is already set.
             */
            p_stream_sdp_info->media_modify_type &= ~(ICF_STREAM_DELETE);
            ret_val = icf_cc_remote_revert_modify_type(p_cc_pdb);
           
        }
        else
        {
            /* Offer change for this stream has been accepted by remote,
             * in case if there was any modification for this stream.
             */

            ret_val = icf_cc_modification_accepted_for_stream(p_cc_pdb);
        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        	icf_cc_make_mms_type_for_nth_rem_succ_ans
 *
 * DESCRIPTION:      This function is invoked to derive appropriate MMS type after
 *					receiving Answer to our reINVITE offer from remote i.e. after
 *					receiving 200 OK.
 *					This is for nth answer from remote.
 *
 *					This function will receive remote answer list from
 *					p_cc_pdb->p_list_1.
 *******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_nth_rem_succ_ans
(
 icf_cc_pdb_st *p_cc_pdb
 )
{
    icf_return_t	ret_val = ICF_SUCCESS;

    /* containing pointer to the first node of type icf_stream_sdp_info.*/
    icf_internal_list_st  *p_stream_sdp_info_node= p_cc_pdb->p_call_ctx
        ->p_stream_sdp_info;

    icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* This Function is checking if there is performing ith
         * level validation on the order of media streams as they
         * were previously sent.
         */
        /* Here No need to set p_cc_pdb->p_list_1 with remote answer list
         * because i am already assuming that this function will also be requiring
         * that variable with the remote answer list.
         */
        if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
        {
            /* if ith level check has failed */
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* ith level valiation has been successful.*/

            /* Following while loop will iterate stream by stream and will check
             * whether offer for this particular stream has been accepted by remote
             * or not.
             */
            while (ICF_NULL != p_stream_sdp_info_node)
            {
                p_stream_sdp_info = (icf_stream_sdp_info_st *)
                    (p_stream_sdp_info_node->p_payload);
                if (ICF_NULL != (p_stream_sdp_info->stream_status
                            & ICF_MEDIA_STREAM_ACTIVE))
                {
                    /* Control will be here if this stream was NOT deleted in
                     * Offer sent.
                     */

                    if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                                & ICF_STREAM_ADD))
                    {
                        /* This stream was added in our offer.*/
                        if (ICF_NULL != (p_stream_sdp_info->media_modify_type
                                    & ICF_STREAM_DELETE))
                        {
                            /* Offer for addition of this stream has been
                             * rejected.
                             */

                            /* Reset the ADD bit, and no need to set Delete bit
                             * because it is already added.
                             */
                            p_stream_sdp_info->media_modify_type
                                &= (~ICF_STREAM_ADD);
                        }
                        else
                        {
                            /* Offer for addition of this stream has been accepted.*/

                            p_stream_sdp_info->media_modify_type =
                                ICF_REMOTE_ANSWER_CODEC_CHANGE;
                        }
                    }
                    else
                    {
                        /* May be Stream was modified.*/

                        p_cc_pdb->p_stream_sdp_info=p_stream_sdp_info;
                        ret_val = icf_cc_check_for_modify_type(p_cc_pdb);
                    }/*End of if (ICF_NULL != (p_stream_sdp_info->media_modify_type..*/
                    /*Fix of CSR:1-5943186 */
                    /*Reset the bitmask ICF_MEDIA_STREAM_REMOTE_HOLD if it is set and
                     *SEND_RECV mode is received in either 200 OK of Re-Invite
                     *or in ACK of Re-Invite without sdp.
                     */ 
                    if((ICF_MEDIA_STREAM_REMOTE_HOLD ==
                               (p_stream_sdp_info->stream_status &
                               ICF_MEDIA_STREAM_REMOTE_HOLD)) &&
                               (ICF_MEDIA_MODE_SEND_RECV ==
                               p_stream_sdp_info->p_remote_received_sdp->media_mode))
                    {
                        p_stream_sdp_info->stream_status &= 
                                  ~ICF_MEDIA_STREAM_REMOTE_HOLD;
                        p_stream_sdp_info->stream_status &= 
                                  ~ICF_MEDIA_STREAM_REMOTE_RESUME;
                    }

                }/*End of if (ICF_NULL != (p_stream_sdp_info->stream_status...)*/

                p_stream_sdp_info_node = (icf_internal_list_st *)
                    (p_stream_sdp_info_node->p_next);

            }/*End of while (ICF_NULL != p_stream_sdp_info_node)*/

        }/*End of if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))*/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}




/******************************************************************************
 *
 * FUNCTION:        icf_cc_derive_mms_remote_rejected
 *
 * DESCRIPTION:     This function is invoked when get 4xx response to our offer.
 *******************************************************************************/

icf_return_t icf_cc_derive_mms_remote_rejected
(
 icf_cc_pdb_st *p_cc_pdb
 )
{
    icf_internal_list_st	*p_stream_sdp_info_node=p_cc_pdb->p_call_ctx
        ->p_stream_sdp_info;

    icf_return_t ret_val=ICF_SUCCESS;

    icf_stream_sdp_info_st	*p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* This while loop is traversing streams node by node and
         * setting appropriate bits in media_modify_type bitmask depending
         * upon the bit set in this same bitmask, which was set at the time
         * when offer was sent.
         */
        while(ICF_NULL != p_stream_sdp_info_node)
        {
            p_stream_sdp_info = (icf_stream_sdp_info_st *)
                (p_stream_sdp_info_node->p_payload);

            if (ICF_NULL != (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE))
            {
                /* Stream was NOT deleted in the offer sent, then we will check if
                 * stream was added or modified and then act accordingly.
                 */
                if (ICF_NULL != (p_stream_sdp_info->media_modify_type & ICF_STREAM_ADD))
                {
                    /* In our offer this stream was newly added.*/

                    /* Because offer was rejected, we will set delete bit for this
                     * stream in media_modify_type bitmask.
                     */

                    /* Reset the Add bit.*/
                    p_stream_sdp_info->media_modify_type &= (~ICF_STREAM_ADD);

                    /* Set the Delete bit.*/
                    p_stream_sdp_info->media_modify_type = ICF_STREAM_DELETE;
                }
                else
                {
                    /* Check if Modification was sent in offer for this stream.*/
                    p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
                    ret_val = icf_cc_remote_revert_modify_type(p_cc_pdb);
                }
            }
            else
            {
                if( ICF_NULL != (p_stream_sdp_info->media_modify_type 
                            & ICF_STREAM_DELETE))
                {
                    /* stream was deleted in the offer, we need to clear the call */
                    ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_REMOTE_REJ_STREAM_DELETE)
                }
            }
            p_stream_sdp_info_node = (icf_internal_list_st *)
                (p_stream_sdp_info_node->p_next);
        }/*End of while(ICF_NULL != p_stream_sdp_info_node)*/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}




#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_replicate_pref_media_profile
 *
 * DESCRIPTION:     This function makes a deep copy of pref media profile received
 *					in icf_create_call_req_st structure.
 *
 ******************************************************************************/
icf_return_t	icf_cc_replicate_pref_media_profile
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_return_t	ret_val = ICF_SUCCESS;

    /* This variable is containing pointer to structure of type
     * icf_pa_cc_create_call_req_st.
     */

    icf_list_st *p_stream_capab_st_node = 
        ((icf_create_call_req_st *)(p_cc_pdb->p_glb_pdb->p_recv_msg->payload))->
        p_pref_media_profile;


    icf_list_st	**p_p_stream_capab_st_node =
        &(p_cc_pdb->p_call_ctx->p_pref_media_profile);

    icf_stream_capab_st		*p_stream_capab_data = ICF_NULL;

    icf_stream_capab_st		*p_stream_capab_new_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */


        /* Following while loop will be iterating through nodes of pref media 
         * profile and be creating deep copy of each node.
         */
        while (p_stream_capab_st_node != NULL)
        {
            p_stream_capab_data = (icf_stream_capab_st *)
                (p_stream_capab_st_node->p_data);

            p_stream_capab_new_data = ICF_NULL;
            if (ICF_NULL != p_stream_capab_st_node->p_data)
            {
                ICF_MEMGET(
                        p_cc_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_stream_capab_st),
                        ICF_MEM_COMMON,
                        (icf_void_t *)(p_stream_capab_new_data),
                        ICF_RET_ON_FAILURE,
                        p_cc_pdb->p_ecode,
                        ret_val
                        )

                    /*Copying memory of type icf_stream_capab_st.*/
                    icf_port_memcpy(
                            p_stream_capab_new_data,
                            p_stream_capab_data,
                            (icf_uint32_t)sizeof(icf_internal_sdp_stream_st)
                            );

                if (p_stream_capab_data->p_codec_info_list != NULL)
                {
                    /*Copying codec list present inside icf_stream_capab_st.*/

                    ret_val = icf_cmn_clone_codec_attrib_list(
                            p_cc_pdb->p_glb_pdb,
                            &(p_stream_capab_new_data->p_codec_info_list),
                            p_stream_capab_data->p_codec_info_list);
                }
                else if (ICF_FAILURE != ret_val)
                {
                    if (p_stream_capab_data->p_encoded_attributes != NULL)
                    {
                        /*This function will be making deep copy of list of type 
                          icf_stream_list_st.*/
                        ret_val=icf_cmn_copy_string_list(
                                p_cc_pdb->p_glb_pdb,
                                &(p_stream_capab_new_data->p_encoded_attributes),
                                p_stream_capab_data->p_encoded_attributes,
                                p_cc_pdb->p_ecode);
                    }
                }
                else if (ICF_FAILURE == ret_val)
                {
                    break;
                }
            }/*End of if (p_stream_capab_st_node->p_data != NULL)*/

            ret_val = icf_cmn_add_node_to_list(p_cc_pdb->p_glb_pdb,
                    p_p_stream_capab_st_node,
                    p_stream_capab_new_data);

            if (ICF_FAILURE == ret_val)
                break;

            p_stream_capab_st_node=p_stream_capab_st_node->p_next;

        }/*End of while (p_stream_capab_st_node != NULL)*/

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */

        return ret_val;
}
#endif


/*****************************************************************************
 ** FUNCTION:     icf_cc_delete_inc_media_sdp_info_list
 **
 ** DESCRIPTION:  This function frees a list of type icf_inc_media_sdp_info.
 *****************************************************************************/
icf_return_t  icf_cc_delete_inc_media_sdp_info_list
(
 icf_cc_pdb_st	*p_cc_pdb,
 icf_list_st	    **p_p_inc_media_sdp
 )
{
    icf_list_st	*p_inc_media_sdp= *p_p_inc_media_sdp;
    icf_return_t ret_val=ICF_SUCCESS;
    icf_inc_media_sdp_info_st *p_inc_media_sdp_info_data = ICF_NULL;

    /*Following while loop is deleting first the payload part
      of the node and then deleting the node itself.*/
    while(p_inc_media_sdp != ICF_NULL)
    {
        /*Freeing the payload part.*/
        p_inc_media_sdp_info_data=(icf_inc_media_sdp_info_st *)
            (p_inc_media_sdp->p_data);
        if (p_inc_media_sdp_info_data != ICF_NULL)
        {
            if (p_inc_media_sdp_info_data->p_int_sdp_stream != ICF_NULL)
            {
                /*Deleting p_int_sdp_stream present in icf_inc_media_sdp_info*/
                ret_val=icf_cmn_delete_internal_sdp_stream(
                        p_cc_pdb->p_glb_pdb,
                        &(p_inc_media_sdp_info_data->p_int_sdp_stream),
                        p_cc_pdb->p_ecode);
            }

        }
        p_inc_media_sdp= p_inc_media_sdp->p_next;
    }

    ret_val = icf_cmn_delete_list(
            p_cc_pdb->p_glb_pdb,p_p_inc_media_sdp,ICF_MEM_COMMON);
    return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_internal_sdp_stream
 *
 * DESCRIPTION:     This function will be copying internal_sdp_stream from
 *					p_src to *p_p_target.
 ******************************************************************************/
icf_return_t
icf_cc_copy_internal_sdp_stream(
        icf_cc_pdb_st	*p_cc_pdb,
        icf_internal_sdp_stream_st   *p_src,
        icf_internal_sdp_stream_st   **p_p_target)
{
    icf_return_t	ret_val = ICF_SUCCESS;

    icf_internal_sdp_stream_st	*p_internal_sdp_stream = ICF_NULL;

    /*	icf_internal_sdp_stream_st	*p_internal_sdp_stream_to_be_copied = ICF_NULL;*/

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    /*	p_internal_sdp_stream_to_be_copied = p_src;*/
    if(ICF_NULL == p_src)
       return ret_val;

    /* Delete the already existing internal sdp stream, if it exists.*/
    if (ICF_NULL != *p_p_target)
    {
        ret_val = icf_cmn_delete_internal_sdp_stream(
                                     p_cc_pdb->p_glb_pdb,
                                     p_p_target,
                                     p_cc_pdb->p_ecode);
    }
    /*Assigning memory of type icf_internal_sdp_stream.*/
    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
            ICF_MEM_SDP,
            (*p_p_target),
            ICF_RET_ON_FAILURE,
            p_cc_pdb->p_ecode,
            ret_val);


    if(ICF_SUCCESS == ret_val)
    {
        p_internal_sdp_stream = *p_p_target;
        /*Copying local or remote sdp into the newly assigned memory.*/
        icf_port_memcpy(p_internal_sdp_stream,
                p_src,
                (icf_uint32_t)sizeof(icf_internal_sdp_stream_st));

        /*This function will be used to create new list of
          codecs from the existing codec list
          into newly creates memory for icf_internal_sdp_stream.*/
        p_internal_sdp_stream->sdp_stream.p_codec_info_list = ICF_NULL;
        p_internal_sdp_stream->sdp_stream.p_encoded_attributes = ICF_NULL;
        p_internal_sdp_stream->p_tunnelled_sdp = ICF_NULL;



ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)


        if (ICF_FAILURE == icf_cmn_copy_string_list(
                    p_cc_pdb->p_glb_pdb,
                    &((p_internal_sdp_stream->sdp_stream).p_encoded_attributes),
                    (p_src->sdp_stream).p_encoded_attributes,p_cc_pdb->p_ecode))	
        {
            ret_val = ICF_FAILURE;
        }
        else if (ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                    p_cc_pdb->p_glb_pdb,
                    &((p_internal_sdp_stream->sdp_stream).p_codec_info_list),
                    (p_src->sdp_stream).p_codec_info_list))
        {
            ret_val = ICF_FAILURE;
        }	

ICF_CHECK_TUNNEL_MODE_END

ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_TRUE)

               if(ICF_NULL != p_src->p_tunnelled_sdp)
               {
                   ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                       ICF_PORT_SIZEOF(icf_tunnelled_sdp_info_st),
                       ICF_MEM_COMMON,
                       (*p_p_target)->p_tunnelled_sdp,
                       ICF_RET_ON_FAILURE,
                       p_cc_pdb->p_ecode,
                       ret_val);
                   icf_port_memcpy( (*p_p_target)->p_tunnelled_sdp,
                                     p_src->p_tunnelled_sdp,
                                     (icf_uint32_t)sizeof(icf_tunnelled_sdp_info_st));
               }

ICF_CHECK_TUNNEL_MODE_END

        /*klocwork warning removal*/ 
        ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                (p_internal_sdp_stream->sdp_stream).p_encoded_attributes)  
        ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                (p_internal_sdp_stream->sdp_stream).p_codec_info_list)  

#if 0
        if(ICF_SUCCESS == ret_val)
        {
            /* Delete the already existing internal sdp stream, if it exists.*/
            if (ICF_NULL != *p_p_target)
            {
                ret_val = icf_cmn_delete_internal_sdp_stream(
                        p_cc_pdb->p_glb_pdb,
                        p_p_target,
                        p_cc_pdb->p_ecode);
            }
            *p_p_target = p_internal_sdp_stream;
        }
        else
#endif
        if(ICF_FAILURE == ret_val)
        {
            ret_val = icf_cmn_delete_internal_sdp_stream(
                    p_cc_pdb->p_glb_pdb,
                    &p_internal_sdp_stream,
                    p_cc_pdb->p_ecode);
            ret_val = ICF_FAILURE;
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_stream_capab_from_applied_sdp
 *
 * DESCRIPTION:     This Function will be invoked to create new stream capab
 *					structure from applied sdp of p_cc_pdb->p_stream_sdp_info
 *					->p_applied_sdp and return pointer to newly created structure
 *					in p_cc_pdb->p_generic_ptr_1.
 ******************************************************************************/
icf_return_t icf_cc_fill_stream_capab_from_applied_sdp
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_return_t				ret_val = ICF_SUCCESS;

    icf_stream_capab_st			*p_stream_capab = ICF_NULL;

    icf_stream_sdp_info_st		*p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;

    icf_sdp_stream_st			*p_sdp_stream = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        if(ICF_NULL == p_cc_pdb->p_stream_sdp_info->p_applied_sdp)
        {
            p_sdp_stream = &(p_cc_pdb->p_stream_sdp_info->p_local_sdp->sdp_stream);
        }
        else
        {
            p_sdp_stream = &(p_cc_pdb->p_stream_sdp_info->p_applied_sdp->sdp_stream);
        }


    /*Assigning memory of type icf_internal_sdp_stream.*/
    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_stream_capab_st),
            ICF_MEM_COMMON,
            p_stream_capab,
            ICF_RET_ON_FAILURE,
            p_cc_pdb->p_ecode,
            ret_val)

        icf_port_memset(p_stream_capab, 0, ICF_PORT_SIZEOF(icf_stream_capab_st));

    p_stream_capab->stream_id = p_stream_sdp_info->stream_id;
    p_stream_capab->stream_type = p_stream_sdp_info->stream_type;

    if (ICF_NULL != p_sdp_stream->p_codec_info_list)
    {
        /* Codec attrib list is present in applied sdp.*/
        ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                &(p_stream_capab->p_codec_info_list),
                p_sdp_stream->p_codec_info_list);

        p_stream_capab->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;
        /*Klocwork Warning Removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                    p_stream_capab->p_codec_info_list);
    }
    if (ICF_FAILURE != ret_val)
    {
        if(ICF_NULL != p_sdp_stream->p_encoded_attributes)
        {
            ret_val = icf_cmn_copy_string_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_stream_capab->p_encoded_attributes),
                    p_sdp_stream->p_encoded_attributes,
                    p_cc_pdb->p_ecode);

            p_stream_capab->bit_mask |= ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT;
           /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                        p_stream_capab->p_encoded_attributes);
        }
    }
    if (ICF_FAILURE != ret_val)
    {
        p_stream_capab->media_stream_param = p_sdp_stream->media_stream_param;
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* if successfull in every task then set p_stream_capab.*/
        p_cc_pdb->p_generic_ptr_1 = p_stream_capab;
    }
    else
    {
        /*Free p_codec_info_list before free the p_stream_capab*/
        icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                         &(p_stream_capab->p_codec_info_list),
                                            p_cc_pdb->p_ecode);

        /*Free p_encoded_attributes before free the p_stream_capab*/
        icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                         &(p_stream_capab->p_encoded_attributes),
                           ICF_MEM_COMMON);     

        /* In case of unsuccess free the memory assigned to p_stream_capab.*/
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_stream_capab,
                ICF_MEM_COMMON,
                p_cc_pdb->p_ecode,
                ret_val)
        ret_val=ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_call_modify_media_from_applied_sdp
 *
 * DESCRIPTION:     This Function will be invoked to create new stream capab
 *					structure from applied  sdp of p_cc_pdb->p_stream_sdp_info
 *					->p_local_sdp and return pointer to newly created structure
 *					in p_cc_pdb->p_generic_ptr_1.
 ******************************************************************************/
icf_return_t icf_cc_fill_call_modify_media_from_applied_sdp
(
 icf_cc_pdb_st	*p_cc_pdb
 )
{
    icf_return_t							ret_val = ICF_SUCCESS;

    icf_call_modify_media_data_st			*p_call_modify_data = ICF_NULL;

    icf_stream_sdp_info_st					*p_stream_sdp_info =
        p_cc_pdb->p_stream_sdp_info;

    icf_sdp_stream_st						*p_sdp_stream = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        if (ICF_STREAM_TYPE_TBCP == p_stream_sdp_info->stream_type)
        {
            p_sdp_stream = &(p_cc_pdb->p_stream_sdp_info
                    ->p_remote_received_sdp->sdp_stream);
        }
        else
        {
            p_sdp_stream = &(p_cc_pdb->p_stream_sdp_info
                    ->p_applied_sdp->sdp_stream);
        }

    /*Assigning memory of type icf_internal_sdp_stream.*/
    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_call_modify_media_data_st),
            ICF_MEM_COMMON,
            p_call_modify_data,
            ICF_RET_ON_FAILURE,
            p_cc_pdb->p_ecode,
            ret_val)

        icf_port_memset(p_call_modify_data, 0, sizeof(icf_stream_capab_st));

    p_call_modify_data->stream_id = p_stream_sdp_info->stream_id;
    p_call_modify_data->stream_type = p_stream_sdp_info->stream_type;

    if (ICF_NULL != p_sdp_stream->p_codec_info_list)
    {
        /* Codec attrib list is present in local sdp.*/
        ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                &(p_call_modify_data->p_codec_info_list),
                p_sdp_stream->p_codec_info_list);

        p_call_modify_data->bit_mask |= ICF_CALL_MOD_CODEC_INFO_PRESENT;
        /*Klocwork Warning Removal*/
        ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                    p_call_modify_data->p_codec_info_list);
    }
    if (ICF_FAILURE != ret_val)
    {
        if(ICF_NULL != p_sdp_stream->p_encoded_attributes)
        {
            ret_val = icf_cmn_copy_string_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_call_modify_data->p_encoded_attributes),
                    p_sdp_stream->p_encoded_attributes,
                    p_cc_pdb->p_ecode);

            p_call_modify_data->bit_mask |= ICF_CALL_MOD_ENC_ATTRIB_PRESENT;
           /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                              p_call_modify_data->p_encoded_attributes)
        }
    }
    if ((ICF_FAILURE != ret_val) && (ICF_STREAM_TYPE_TBCP != p_stream_sdp_info->stream_type))
    {
        p_call_modify_data->media_stream_param = p_sdp_stream->media_stream_param;
        p_call_modify_data->bit_mask |= ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT;
    }

    if (ICF_SUCCESS == ret_val)
    {
        icf_transport_address_st dummy_stream_address = {{0,{{0,"\0"}}},0};
        dummy_stream_address.port_num = p_sdp_stream->stream_address.port_num;

        p_cc_pdb->p_generic_ptr_1 = &(p_sdp_stream->stream_address);
        p_cc_pdb->p_generic_ptr_2 = &dummy_stream_address;

        if(ICF_TRUE == icf_cc_compare_ip_and_port(p_cc_pdb))
        {
            p_call_modify_data->media_mode = ICF_MEDIA_MODE_INACTIVE;
        }
        else if(ICF_CC_REM_IP_NULL == 
                (ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask))
        {
            p_call_modify_data->media_mode = ICF_MEDIA_MODE_INACTIVE;
        }
        else
        {
            /* Copying media mode from mm_applied_mode.*/
            p_call_modify_data->media_mode = p_stream_sdp_info->mm_applied_mode; 
        }

		p_cc_pdb->p_generic_ptr_2 = ICF_NULL; /* Klocworks: LOCRET */
        p_cc_pdb->common_bitmask &= ~(ICF_CC_REM_IP_NULL);
        p_call_modify_data->bit_mask |= ICF_CALL_MOD_MEDIA_MODE_PRESENT;

        /* if successfull in every task then set assign p_call_modify_data.
         * to p_generic_ptr_1.
         */
        p_cc_pdb->p_generic_ptr_1 = p_call_modify_data;
    }
    else
    {
        /*Free p_codec_info_list before free the p_stream_capab*/
        icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                         &(p_call_modify_data->p_codec_info_list),
                                            p_cc_pdb->p_ecode);

        /*Free p_encoded_attributes before free the p_stream_capab*/
        icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                         &(p_call_modify_data->p_encoded_attributes),
                           ICF_MEM_COMMON);     
        /* In case of unsuccess free the memory assigned to p_call_modify_data.*/
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                p_call_modify_data,
                ICF_MEM_COMMON,
                p_cc_pdb->p_ecode,
                ret_val)
        ret_val=ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to create list of stream capab
 *					corresponding to applied sdps and also for creating list of deleted
 *					streams from the p_cc_pdb->p_call_ctx->p_stream_sdp_info
 *
 *					This function will do following things:
 *					1)Traverse the p_cc_pdb->p_call_ctx->p_stream_sdp_info list
 *					2)Check from the stream_status bitmask whether this particular
 *					  stream is Acitive or not.
 *					  if stream is INACTIVE then
 *						increase the count field in p_cc_pdb by one
 *						AND fill deleted_media_streams[count].stream_id,
 *						deleted_media_streams[count].stream_type for this stream
 *						kept in p_cc_pdb.
 *					  else
 *						create data of type stream capab from the applied sdp of
 * 						this stream and add the pointer to newly created stream
 *						capab data into the p_cc_pdb->p_accepted_streams list.
 ******************************************************************************/
icf_return_t
icf_cc_create_app_media_data(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;

    icf_internal_list_st		*p_stream_sdp_info_node =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;

    p_cc_pdb->count = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        p_cc_pdb->p_list_1 = ICF_NULL;

    /* Following while loop traverses the each stream node by node and count
     * for rejected streams and make list of stream capabs from the applied
     * sdps of Active streams.
     */
    while (ICF_NULL != p_stream_sdp_info_node)
    {
        p_stream_sdp_info = (icf_stream_sdp_info_st *)
            (p_stream_sdp_info_node->p_payload);

        if ((ICF_MEDIA_STREAM_ACTIVE ==
                (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE)) &&
           (ICF_STREAM_PENDING_DELETE !=
                (p_stream_sdp_info->media_modify_type & ICF_STREAM_PENDING_DELETE)))
        {
            /* This particular media stream is ACTIVE.*/

            /* Before calling this function we need to set p_cc_pdb->p_stream
             * _sdp_info with this particular streams p_stream_sdp_info data.
             * And also after returning from this function extract newly created
             * stream capab pointer from p_cc_pdb->p_generic_ptr_1.
             * p_cc_pdb->count must be initialised to 0 before calling
             * this function.
             */
            p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
            p_cc_pdb->p_generic_ptr_1 = ICF_NULL;

            if (ICF_CC_FILL_CALL_MODIFY_DATA == 
                    (p_cc_pdb->common_bitmask & ICF_CC_FILL_CALL_MODIFY_DATA))
            {
                if(ICF_FAILURE == 
                        icf_cc_fill_call_modify_media_from_applied_sdp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                    break;
                }
            }
            else
            {
                if(ICF_FAILURE == icf_cc_fill_stream_capab_from_applied_sdp(p_cc_pdb))
                {
                    ret_val = ICF_FAILURE;
                    break;
                }
            }

            if(ICF_NULL == p_cc_pdb->p_generic_ptr_1)
            {
                /* do nothing */	
            }
            else if(ICF_FAILURE == icf_cmn_add_node_to_list(
                        p_cc_pdb->p_glb_pdb,
                        &(p_cc_pdb->p_list_1),
                        p_cc_pdb->p_generic_ptr_1))
            {
                ret_val = ICF_FAILURE;
                break;
            }

        }
        else if(ICF_STREAM_PENDING_DELETE !=
                (p_stream_sdp_info->media_modify_type & ICF_STREAM_PENDING_DELETE))
        {
            /* This particular stream is INACTIVE.*/
			if ( p_cc_pdb->count < ICF_MAX_STREAM_PER_CALL ) {
            (p_cc_pdb->deleted_media_streams[p_cc_pdb->count]).stream_id
                = p_stream_sdp_info->stream_id;
            (p_cc_pdb->deleted_media_streams[p_cc_pdb->count]).stream_type
                = p_stream_sdp_info->stream_type;
			}
            (p_cc_pdb->count)++;
        }
        p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_delete_app_media_data
 *
 * DESCRIPTION:     This Function will be invoked to delete the app media data
 ******************************************************************************/
icf_return_t
icf_cc_delete_app_media_data(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        if (ICF_CC_FILL_CALL_MODIFY_DATA == 
                (p_cc_pdb->common_bitmask & ICF_CC_FILL_CALL_MODIFY_DATA))
        {
            icf_cc_free_call_modify_data_list(p_cc_pdb);
        }
        else
        {
            icf_cc_free_stream_capab_list(p_cc_pdb);
        }

    p_cc_pdb->p_list_1 = ICF_NULL;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_call_modify_data_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the call modify data
 ******************************************************************************/
icf_return_t
icf_cc_free_call_modify_data_list(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t	ret_val = ICF_SUCCESS;
    icf_list_st		*p_call_modify_data_list =  p_cc_pdb->p_list_1;
    icf_call_modify_media_data_st  *p_data = ICF_NULL;
    icf_list_st     *p_temp_list = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        while(ICF_NULL != p_call_modify_data_list)
        {
            p_data = (icf_call_modify_media_data_st *)
                p_call_modify_data_list->p_data;

            if(ICF_NULL != p_data)
            {

                if(ICF_NULL != p_data->p_codec_info_list)
                {
                    ret_val = icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                            &(p_data->p_codec_info_list),p_cc_pdb->p_ecode);
                }

                if(ICF_NULL != p_data->p_encoded_attributes)
                {
                    ret_val = icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                            (&(p_data->p_encoded_attributes)),
                            ICF_MEM_COMMON);
                }
            }

            p_temp_list = p_call_modify_data_list;

            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_data,
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val)

                p_call_modify_data_list = p_call_modify_data_list->p_next;

            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_temp_list,
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val)
        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_stream_capab_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the stream capab data
 ******************************************************************************/
icf_return_t
icf_cc_free_stream_capab_list(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;

    icf_list_st		*p_stream_capab_data_list =  p_cc_pdb->p_list_1;
    icf_stream_capab_st  *p_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        while(ICF_NULL != p_stream_capab_data_list)
        {
            p_data = (icf_stream_capab_st *)p_stream_capab_data_list->p_data;

            if(ICF_NULL != p_data->p_codec_info_list)
            {
                ret_val = icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                        &(p_data->p_codec_info_list),p_cc_pdb->p_ecode);
            }

            if(ICF_NULL != p_data->p_encoded_attributes)
            {
                ret_val = icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                        (&(p_data->p_encoded_attributes)),
                        ICF_MEM_COMMON);
            }

            /*ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
              (icf_void_t *)(p_data),
              ICF_MEM_COMMON,
              p_cc_pdb->p_ecode,
              ret_val)

              p_temp_list = p_stream_capab_data_list;*/
            p_stream_capab_data_list = p_stream_capab_data_list->p_next;

            /*      ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    (icf_void_t *)(p_temp_list),
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val)*/
        }

    icf_cmn_delete_list(p_cc_pdb->p_glb_pdb, &(p_cc_pdb->p_list_1), ICF_MEM_COMMON);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

#if 0

/* Following while loop traverses the each stream node by node and count
 * for rejected streams and make list of stream capabs from the applied
 * sdps of Active streams.
 */
while (ICF_NULL != p_stream_sdp_info_node)
{
    p_stream_sdp_info = (icf_stream_sdp_info_st *)
        (p_stream_sdp_info_node->p_payload);

    if (ICF_MEDIA_STREAM_ACTIVE ==
            (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE))
    {
        /* This particular media stream is ACTIVE.*/

        /* Before calling this function we need to set p_cc_pdb->p_stream
         * _sdp_info with this particular streams p_stream_sdp_info data.
         * And also after returning from this function extract newly created
         * stream capab pointer from p_cc_pdb->p_generic_ptr_1.
         * p_cc_pdb->count must be initialised to 0 before calling
         * this function.
         */
        p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;


        if (ICF_CC_FILL_CALL_MODIFY_DATA == 
                (p_cc_pdb->common_bitmask & ICF_CC_FILL_CALL_MODIFY_DATA))
        {
            if(ICF_FAILURE == 
                    icf_cc_fill_call_modify_media_from_local_sdp(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
                break;
            }
        }
        else
        {
            if(ICF_FAILURE == icf_cc_fill_stream_capab_from_local_sdp(p_cc_pdb))
            {
                ret_val = ICF_FAILURE;
                break;
            }
        }
        if(ICF_FAILURE == icf_cmn_add_node_to_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_cc_pdb->p_list_1),
                    p_cc_pdb->p_generic_ptr_1))
        {
            ret_val = ICF_FAILURE;
            break;
        }

    }
    else
    {
        /* This particular stream is INACTIVE.*/

        (p_cc_pdb->deleted_media_streams[p_cc_pdb->count]).stream_id
            = p_stream_sdp_info->stream_id;
        (p_cc_pdb->deleted_media_streams[p_cc_pdb->count]).stream_type
            = p_stream_sdp_info->stream_type;
        (p_cc_pdb->count)++;
    }
    p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
}

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_copy_deleted_list
 *
 * DESCRIPTION:     This function copies the deleted stream list info from
 *                  PDB structure to message struture.
 ******************************************************************************/
icf_return_t
icf_cc_copy_deleted_list(
        icf_cc_pdb_st *p_cc_pdb,
        icf_stream_list_st *p_deleted_list)
{
    icf_return_t		ret_val = ICF_SUCCESS;
    icf_uint8_t         ctr=0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        for(ctr = 0; (ctr < p_cc_pdb->count) && (ctr < ICF_MAX_STREAM_PER_CALL ); ctr++)
        {
            (p_deleted_list[ctr]).stream_id =
                p_cc_pdb->deleted_media_streams[ctr].stream_id;
            (p_deleted_list[ctr]).stream_type =
                p_cc_pdb->deleted_media_streams[ctr].stream_type;
        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_generate_stream_id
 *
 * DESCRIPTION:     This function generates a new stream ID for the newly added
 *                  stream
 ******************************************************************************/
icf_return_t
icf_cc_generate_stream_id(
        icf_cc_pdb_st * p_cc_pdb,
        icf_uint8_t    *p_stream_id)
{
    icf_return_t		ret_val = ICF_SUCCESS;
    icf_uint8_t         max_id =0;

    icf_internal_list_st	*p_stream_sdp_info_list = 
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        while(ICF_NULL != p_stream_sdp_info_list)
        {
            p_stream_sdp_info = (icf_stream_sdp_info_st *)
                p_stream_sdp_info_list->p_payload;

            if(p_stream_sdp_info->stream_id > max_id)
            {
                max_id = p_stream_sdp_info->stream_id;
            }

            p_stream_sdp_info_list = p_stream_sdp_info_list->p_next;
        }


    *p_stream_id = (icf_uint8_t)max_id + 1;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_add_new_fax_stream
 *
 * DESCRIPTION:     This function adds new FAX stream
 ******************************************************************************/
icf_return_t
icf_cc_add_new_fax_stream(
        icf_cc_pdb_st * p_cc_pdb)

{
    icf_return_t		ret_val = ICF_SUCCESS;
    icf_stream_capab_st     *p_stream_capab_info = ICF_NULL;
    icf_app_conf_st         *p_app_conf = ICF_NULL;
    icf_internal_list_st	*p_app_profile_node = ICF_NULL;
    icf_uint32_t             stream_type = ICF_STREAM_TYPE_FAX;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        /*take it from app profile if it is there for FAX*/
        ret_val = icf_cfg_fetch_app_conf(
                p_cc_pdb->p_glb_pdb,
                p_cc_pdb->p_glb_pdb->p_call_ctx->app_id,
                &p_app_conf,
                p_cc_pdb->p_ecode);

    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_app_conf));

    /* add new steram for T38 */
    /* only check the app profile, need not chek for pref media profile*/
    if(ICF_NULL != p_app_conf->p_app_media_profile)
    {

        p_app_profile_node = icf_cmn_get_node_in_internal_list(
                p_app_conf->p_app_media_profile,
                &stream_type);

        if (ICF_NULL == p_app_profile_node)
        {
            /* No FAX Profile
               return error
             */
            if(ICF_CC_LOCAL_FAX_REQ == 
                    (ICF_CC_LOCAL_FAX_REQ & p_cc_pdb->common_bitmask))
            {
                p_cc_pdb->p_call_ctx->app_failure_cause = ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED;
            }
            else
            {
                p_cc_pdb->p_call_ctx->offer_failure_reason = 
                    ICF_CAUSE_FAX_PROFILE_NOT_CONFIGURED;
            }
            return ICF_SUCCESS;
        }
        else
        {
            p_stream_capab_info = (icf_stream_capab_st *)
                p_app_profile_node->p_payload;

            /* Control will be here only if FAX profile exists in the
             * app_add profile.
             */
            /* Now create fax stream from the app_add profile.*/

            /* We have to set these variables before calling this
               function.*/
            /*p_cc_pdb->stream_id = p_stream_capab_info->stream_id;*/

            if(ICF_CC_LOCAL_FAX_REQ == 
                    (ICF_CC_LOCAL_FAX_REQ & p_cc_pdb->common_bitmask))
            {
                icf_internal_list_st        *p_sdp_list = ICF_NULL;

                /* flag is false because pref profile does not exist or it
                 * does Not contain audio profile.
                 */

                p_sdp_list = 
                    icf_cc_get_node_from_stream_list(
                            p_cc_pdb->p_call_ctx->p_stream_sdp_info, 
                            ICF_STREAM_TYPE_FAX);

                if(ICF_NULL != p_sdp_list)
                {
                    ret_val = 
                        icf_cmn_free_stream_sdp_info_list(p_cc_pdb->p_glb_pdb,
                                &p_sdp_list,
                                p_cc_pdb->p_ecode);

                    p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_next = ICF_NULL;
                }
             
                /* Stream id should be generated after deleting the Fax stream
                 * if it was already there but deleted.
                 */ 
                icf_cc_generate_stream_id(p_cc_pdb,&p_cc_pdb->stream_id);
                p_cc_pdb->flag = ICF_FALSE;

                /* This is the pointer to data of type stream capab from
                 * where Audio parameters will be extracted.
                 */
                p_cc_pdb->p_generic_ptr_1 = p_stream_capab_info;

                ret_val = icf_cc_fill_local_sdp_from_profile(p_cc_pdb);

                ICF_CC_RESET_PDB_COMMON_BITMASK(p_cc_pdb,
                        ICF_CC_LOCAL_FAX_REQ)

                    if(ICF_SUCCESS == ret_val && ICF_NULL != p_cc_pdb->p_stream_sdp_info)
                    {
                        icf_uint32_t stream_id = p_cc_pdb->stream_id;

                        p_cc_pdb->p_stream_sdp_info->media_modify_type =
                            ICF_STREAM_ADD;

                        /* add the newly created node to internal sdp list*/
                        if(ICF_FAILURE == icf_cmn_add_node_to_internal_list(
                                    p_cc_pdb->p_glb_pdb,
                                    &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                                    p_cc_pdb->p_stream_sdp_info,
                                    &stream_id,
                                    ICF_NULL))
                        {
                            icf_cmn_delete_internal_sdp_stream(
                                    p_cc_pdb->p_glb_pdb,
                                    &(p_cc_pdb->p_stream_sdp_info->p_local_sdp),
                                    p_cc_pdb->p_ecode);


                            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                                    p_cc_pdb->p_stream_sdp_info,
                                    ICF_MEM_COMMON,
                                    p_cc_pdb->p_ecode,
                                    ret_val)

                                ret_val = ICF_FAILURE;
                        }
                        else
                        {
                            p_cc_pdb->p_stream_sdp_info->p_applied_sdp = ICF_NULL;
                            p_cc_pdb->p_stream_sdp_info->
                                p_remote_received_sdp = ICF_NULL;
                        }

                    }
                    else
                    {
                        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                                p_cc_pdb->p_stream_sdp_info,
                                ICF_MEM_COMMON,
                                p_cc_pdb->p_ecode,
                                ret_val)

                            ret_val = ICF_FAILURE;
                    }
            }
            else
            {
                /* Remote FAX request
                 */


                icf_stream_sdp_info_st *p_new_stream_sdp_info = ICF_NULL;
                icf_internal_list_st        *p_sdp_list = ICF_NULL;

                /* flag is false because pref profile does not exist or it
                 * does Not contain audio profile.
                 */

                p_sdp_list = 
                    icf_cc_get_node_from_stream_list(
                            p_cc_pdb->p_call_ctx->p_stream_sdp_info, 
                            ICF_STREAM_TYPE_FAX);

                if(ICF_NULL != p_sdp_list)
                {
                    ret_val = 
                        icf_cmn_free_stream_sdp_info_list(p_cc_pdb->p_glb_pdb,
                                &p_sdp_list,
                                p_cc_pdb->p_ecode);

                    p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_next = ICF_NULL;
                }

                ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
                        ICF_MEM_COMMON,
                        p_new_stream_sdp_info,
                        ICF_RET_ON_FAILURE,
                        p_cc_pdb->p_ecode,
                        ret_val)

                    icf_port_memset(p_new_stream_sdp_info,0,
                            ICF_PORT_SIZEOF(icf_stream_sdp_info_st));

                if(ICF_SUCCESS == ret_val)
                {
                    icf_uint32_t stream_id=0;
                    icf_cc_generate_stream_id(p_cc_pdb,
                            &p_new_stream_sdp_info->stream_id);

                    p_new_stream_sdp_info->stream_type = ICF_STREAM_TYPE_FAX;

                    p_new_stream_sdp_info->stream_status |=
                        ICF_MEDIA_STREAM_ACTIVE;

                    p_new_stream_sdp_info->stream_status |=
                        ICF_MEDIA_STREAM_NW_ALLOW;


                    p_new_stream_sdp_info->media_modify_type =
                        ICF_STREAM_ADD;
                    stream_id =p_new_stream_sdp_info->stream_id; 

                    /* add the newly created node to internal sdp list*/
                    if(ICF_FAILURE == icf_cmn_add_node_to_internal_list(
                                p_cc_pdb->p_glb_pdb,
                                &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                                p_new_stream_sdp_info,
                                &stream_id,
                                ICF_NULL))
                    {
                        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                                p_new_stream_sdp_info,
                                ICF_MEM_COMMON,
                                p_cc_pdb->p_ecode,
                                ret_val)

                            ret_val = ICF_FAILURE;
                    }
                    else
                    {
                        p_cc_pdb->p_stream_sdp_info = p_new_stream_sdp_info;
                    }


                }
            }      
        }
    }
    else
    {
        icf_uint32_t stream_id =0;
        /* Now create default Stream node with only strema ID and
         * stream type set
         */

        icf_stream_sdp_info_st *p_new_stream_sdp_info = ICF_NULL;

        icf_internal_list_st        *p_sdp_list = ICF_NULL;

        p_sdp_list = 
            icf_cc_get_node_from_stream_list(
                    p_cc_pdb->p_call_ctx->p_stream_sdp_info, 
                    ICF_STREAM_TYPE_FAX);


        if(ICF_NULL != p_sdp_list)
        {
            ret_val = 
                icf_cmn_free_stream_sdp_info_list(p_cc_pdb->p_glb_pdb,
                        &p_sdp_list,
                        p_cc_pdb->p_ecode);

            p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_next = ICF_NULL;
        }

        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
                ICF_MEM_COMMON,
                p_new_stream_sdp_info,
                ICF_RET_ON_FAILURE,
                p_cc_pdb->p_ecode,
                ret_val)

            icf_port_memset(p_new_stream_sdp_info,0,
                    ICF_PORT_SIZEOF(icf_stream_sdp_info_st));

        icf_cc_generate_stream_id(p_cc_pdb,
                &p_new_stream_sdp_info->stream_id);

        p_new_stream_sdp_info->stream_type = ICF_STREAM_TYPE_FAX;


        p_new_stream_sdp_info->stream_status |=
            ICF_MEDIA_STREAM_ACTIVE;

        p_new_stream_sdp_info->stream_status |=
            ICF_MEDIA_STREAM_NW_ALLOW;

        p_new_stream_sdp_info->media_modify_type = ICF_STREAM_ADD;

        stream_id = p_new_stream_sdp_info->stream_id;

        /*add it to the stream list in call context */
        if(ICF_FAILURE == icf_cmn_add_node_to_internal_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_cc_pdb->p_call_ctx->p_stream_sdp_info),
                    p_new_stream_sdp_info,
                    &stream_id,
                    ICF_NULL))
        {
            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_new_stream_sdp_info,
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val)

                ret_val = ICF_FAILURE;
        }
        else
        {
            p_cc_pdb->p_stream_sdp_info = p_new_stream_sdp_info;
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}



/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_media_modify_type
 *
 * DESCRIPTION:     This function marks the MMS type on per stream basis in MMI
 *                  payload.
 ******************************************************************************/
icf_return_t
icf_cc_set_media_modify_type(
        icf_cc_pdb_st * p_cc_pdb,
        icf_mmi_mms_request_st *p_mms_payload)
{
    icf_return_t		        ret_val = ICF_SUCCESS;
    icf_internal_list_st		*p_stream_sdp_info_node =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;

    icf_uint8_t         active_list_count =0;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        p_cc_pdb->mms_sent = ICF_FALSE;

    while (ICF_NULL != p_stream_sdp_info_node)
    {
        p_stream_sdp_info = (icf_stream_sdp_info_st *)
            (p_stream_sdp_info_node->p_payload);

        if ((ICF_MEDIA_STREAM_ACTIVE ==
                (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE)) &&
           (ICF_STREAM_PENDING_DELETE !=
                        (ICF_STREAM_PENDING_DELETE & p_stream_sdp_info->media_modify_type)))  
        {
            /* This particular media stream is ACTIVE.*/
            active_list_count++;

            /* Now check the modify_media_type and set the appropriate type
               in MMS payload
             */
            if(p_stream_sdp_info->media_modify_type)
            {
                p_cc_pdb->mms_sent = ICF_TRUE;

                if(ICF_STREAM_ADD ==
                        (ICF_STREAM_ADD & p_stream_sdp_info->media_modify_type))
                {
                    p_mms_payload->bit_mask |= ICF_MMI_MMS_ACTION_ADD;

                    if(ICF_NULL == p_mms_payload->p_add_media)
                    {
                        /* allocate memory and put information*/
                        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_stream_id_list_st),
                                ICF_MEM_COMMON,
                                p_mms_payload->p_add_media,
                                ICF_RET_ON_FAILURE,
                                p_cc_pdb->p_ecode,
                                ret_val)

                            p_mms_payload->p_add_media->count = 1;

                    }
                    else
                    {
                        p_mms_payload->p_add_media->count++;
                    }

                    if ((p_mms_payload->p_add_media->count -1) < ICF_MAX_STREAM_PER_CALL) 
					{
						p_mms_payload->p_add_media->stream_id
                        [p_mms_payload->p_add_media->count -1] =
                        p_stream_sdp_info->stream_id;
					}
                }
                else if(ICF_STREAM_DELETE ==
                        (ICF_STREAM_DELETE & p_stream_sdp_info->media_modify_type))
                {

                    p_mms_payload->bit_mask |= ICF_MMI_MMS_ACTION_DELETE;

                    if(ICF_NULL == p_mms_payload->p_delete_media)
                    {
                        /* allocate memory and put information*/
                        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                                ICF_PORT_SIZEOF(icf_stream_id_list_st),
                                ICF_MEM_COMMON,
                                p_mms_payload->p_delete_media,
                                ICF_RET_ON_FAILURE,
                                p_cc_pdb->p_ecode,
                                ret_val)

                            p_mms_payload->p_delete_media->count = 1;
                        p_stream_sdp_info->stream_status 
                            &= ~ICF_MEDIA_STREAM_ACTIVE;
                    }
                    else
                    {
                        p_mms_payload->p_delete_media->count++;
                    }

                    if ((p_mms_payload->p_delete_media->count -1) < ICF_MAX_STREAM_PER_CALL) 
					{
                    p_mms_payload->p_delete_media->stream_id
                        [p_mms_payload->p_delete_media->count -1] =
                        p_stream_sdp_info->stream_id;
					}
                    p_stream_sdp_info->stream_status &= ~ICF_MEDIA_STREAM_ACTIVE;
                    icf_cc_get_call_type(p_cc_pdb,
                            p_stream_sdp_info->stream_type);

                    p_stream_sdp_info->stream_status &= (~ICF_MEDIA_STREAM_ACTIVE);

                    p_cc_pdb->p_call_ctx->call_type &= (~(p_cc_pdb->call_type)); 
                }
                else
                {
                    icf_uint8_t count = 0;

                    if(ICF_MEDIA_REVERT_NULL !=
                        (ICF_MEDIA_REVERT_NULL & p_stream_sdp_info->media_modify_type))
                    {
                          p_mms_payload->bit_mask |= ICF_MMI_MMS_ACTION_MODIFY;

                          if(p_mms_payload->modify_stream_count)
                          {
                              p_mms_payload->modify_stream_count++;
                          }
                          else
                          {
                              p_mms_payload->modify_stream_count = 1;
                          }

                          count = (icf_uint8_t)p_mms_payload->modify_stream_count - 1;
						if ( count < ICF_MAX_STREAM_PER_CALL ) 
						{
                        p_mms_payload->modify_media[count].bitmask =
                            p_stream_sdp_info->media_modify_type;

                        p_mms_payload->modify_media[count].stream_id =
                            p_stream_sdp_info->stream_id;

                       p_mms_payload->modify_media[count].stream_type =
                            p_stream_sdp_info->stream_type;
						}	
                     }      
                     else
                     {
                        p_stream_sdp_info->media_modify_type &= ~(ICF_MEDIA_REVERT_NULL);
                     }     
                }
            }
        }

        p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
    }

    if(ICF_NULL != p_mms_payload->p_delete_media)
    {
        if(p_mms_payload->p_delete_media->count == active_list_count)
        {
            /* All the active streams have been marked for deletion
               do not send MMS request, send generic call clear so that
               DMS request is sent
             */
            ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_NO_ACTIVE_STREAMS_LEFT)
                p_cc_pdb->mms_sent = ICF_FALSE;
            ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_ALL_STREAMS_DELETED)

                ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                        p_mms_payload->p_delete_media,
                        ICF_MEM_COMMON,
                        p_cc_pdb->p_ecode,
                        ret_val)
        }
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_derive_mms_type_for_revert_remote_change
 *
 * DESCRIPTION:     This function derives the MMS type for reverting the remote
 *                  media change
 ******************************************************************************/
icf_return_t
icf_cc_derive_mms_type_for_revert_remote_change(
        icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t		ret_val = ICF_SUCCESS;
    icf_internal_list_st *p_sdp_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;


    icf_stream_sdp_info_st *p_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        while(ICF_NULL != p_sdp_list)
        {
            p_sdp_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
            if(ICF_STREAM_DELETE & p_sdp_info->media_modify_type)
            {
                    /*Case when the incoming offer is for media line delete
                      and the request is rejected by application*/
                    ret_val = ICF_FAILURE;
                    return ret_val ;
            }

            if(ICF_REMOTE_SDP_MEDIA_CHANGE ==
                    (ICF_REMOTE_SDP_MEDIA_CHANGE & p_sdp_info->media_modify_type))
            {
                p_sdp_info->media_modify_type |=
                    ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE;

                p_sdp_info->media_modify_type &= ~(ICF_REMOTE_SDP_MEDIA_CHANGE);
            }
            else if(ICF_REMOTE_SDP_CODEC_CHANGE ==
                    (ICF_REMOTE_SDP_CODEC_CHANGE & p_sdp_info->media_modify_type))
            {
                p_sdp_info->media_modify_type |=
                    ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE;
                p_sdp_info->media_modify_type &= ~(ICF_REMOTE_SDP_CODEC_CHANGE);
            }
            else if(ICF_MODE_CHANGE ==
                    (ICF_MODE_CHANGE & p_sdp_info->media_modify_type))
            {
                p_cc_pdb->p_stream_sdp_info = p_sdp_info;
                icf_cc_calc_mode_app_reject_change(p_cc_pdb);
                /* copy the mode in mm_applied_mode after calculation*/
            }
            else if(ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE ==
                    (ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE & p_sdp_info->media_modify_type) ||
                    (ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE ==
                    (ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE & p_sdp_info->media_modify_type)))
            {
               p_sdp_info->media_modify_type |= ICF_MEDIA_REVERT_NULL;
            }
            
            /*********** Changes for Re-Invite w/o SDP Start (Rel7.0) ***********/

            /* If the common bitmask is set as ICF_CALL_MOD_REQ_WO_SDP, set the
            * modify_media_type as ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE */

            else if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                          ICF_CALL_MOD_REQ_WO_SDP))
            {
                p_sdp_info->media_modify_type |=
                                   ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE;
                p_sdp_info->media_modify_type &= ~(ICF_LOCAL_SDP_CODEC_CHANGE);
                ICF_CC_RESET_COMMON_BITMASK(p_cc_pdb,ICF_CALL_MOD_REQ_WO_SDP)

                /* The p_last_commited_sdp is copied back in the p_local_sdp
                 * in media revert back processing in case of ReINVITE received
                 * without SDP.
                 */
                if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb,
                               p_sdp_info->p_last_commited_sdp, /*Source*/
                               &(p_sdp_info->p_local_sdp) /*Destination*/))

                 {
                        ret_val = ICF_FAILURE;
                        break;
                 }
            }

            /************ Changes for Re-Invite w/o SDP End (Rel7.0) ************/
            
            /* SPR 18589 : If the steam was on remote hold and the new request was 
            * remote resume, reset the resume bitmask as it has been rejected by application
            */
            if((ICF_MEDIA_STREAM_REMOTE_HOLD ==
                     (p_sdp_info->stream_status & ICF_MEDIA_STREAM_REMOTE_HOLD)) &&
                  (ICF_MEDIA_STREAM_REMOTE_RESUME ==
                     (p_sdp_info->stream_status & ICF_MEDIA_STREAM_REMOTE_RESUME)))
            {
                p_sdp_info->stream_status &= ~ICF_MEDIA_STREAM_REMOTE_RESUME;
            }
            p_sdp_list = p_sdp_list->p_next;
        }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}


#if 0
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_audio_stream_params
 *
 * DESCRIPTION:     This function compares two audio profiles
 ******************************************************************************/
icf_boolean_t   icf_cc_compare_audio_stream_params(
        icf_audio_profile_st	new_profile,
        icf_audio_profile_st   existing_profile)
{
    if((ICF_AUDIO_PROF_BNDWDTH_PRESENT  ==
                (new_profile.bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)) &&
            (ICF_AUDIO_PROF_BNDWDTH_PRESENT  !=
             (existing_profile.bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)))
    {
        return ICF_FALSE;
    }
    else if((ICF_AUDIO_PROF_BNDWDTH_PRESENT  ==
                (new_profile.bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)) &&
            (ICF_AUDIO_PROF_BNDWDTH_PRESENT  ==
             (existing_profile.bit_mask & ICF_AUDIO_PROF_BNDWDTH_PRESENT)))
    {
        if(new_profile.bandwidth != existing_profile.bandwidth)
            return ICF_FALSE;
    }
    else if((ICF_AUDIO_PROF_PTIME_PRESENT  ==
                (new_profile.bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)) &&
            (ICF_AUDIO_PROF_PTIME_PRESENT  !=
             (existing_profile.bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)))
    {
        return ICF_FALSE;
    }
    else if((ICF_AUDIO_PROF_PTIME_PRESENT  ==
                (new_profile.bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)) &&
            (ICF_AUDIO_PROF_PTIME_PRESENT  ==
             (existing_profile.bit_mask & ICF_AUDIO_PROF_PTIME_PRESENT)))
    {
        if(new_profile.pref_ptime != existing_profile.pref_ptime)
            return ICF_FALSE;
    }
    else if((ICF_AUDIO_PROF_SILN_SUPP_PRESENT  ==
                (new_profile.bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)) &&
            (ICF_AUDIO_PROF_SILN_SUPP_PRESENT  !=
             (existing_profile.bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)))
    {
        return ICF_FALSE;
    }
    else if((ICF_AUDIO_PROF_SILN_SUPP_PRESENT  ==
                (new_profile.bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)) &&
            (ICF_AUDIO_PROF_SILN_SUPP_PRESENT  ==
             (existing_profile.bit_mask & ICF_AUDIO_PROF_SILN_SUPP_PRESENT)))
    {
        if(new_profile.silence_suppression != existing_profile.silence_suppression)
            return ICF_FALSE;
    }

    return ICF_TRUE;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_compare_video_stream_params
 *
 * DESCRIPTION:     This function compares two video profiles
 ******************************************************************************/
icf_boolean_t   icf_cc_compare_video_stream_params(
        icf_video_profile_st	new_profile,
        icf_video_profile_st   existing_profile)
{
    if((ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT  ==
                (new_profile.bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)) &&
            (ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT  !=
             (existing_profile.bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)))
    {
        return ICF_FALSE;
    }
    else if((ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT  ==
                (new_profile.bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)) &&
            (ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT  ==
             (existing_profile.bit_mask & ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT)))
    {
        if(new_profile.encoding_quality != existing_profile.encoding_quality)
            return ICF_FALSE;
    }
    else if((ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT  ==
                (new_profile.bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)) &&
            (ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT  !=
             (existing_profile.bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)))
    {
        return ICF_FALSE;
    }
    else if((ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT  ==
                (new_profile.bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)) &&
            (ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT  ==
             (existing_profile.bit_mask & ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT)))
    {
        if(new_profile.frame_rate != existing_profile.frame_rate)
            return ICF_FALSE;
    }
    else if((ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT  ==
                (new_profile.bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)) &&
            (ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT  !=
             (existing_profile.bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)))
    {
        return ICF_FALSE;
    }
    else if((ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT  ==
                (new_profile.bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)) &&
            (ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT  ==
             (existing_profile.bit_mask & ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT)))
    {
        if(new_profile.bandwidth != existing_profile.bandwidth)
            return ICF_FALSE;
    }

    return ICF_TRUE;
}

#endif


/*****************************************************************************
 ** FUNCTION:     icf_cc_send_mms_req_for_fax_failover
 **
 ** DESCRIPTION:  This function is for sending MMS request for fax failover
 *****************************************************************************/
icf_return_t
icf_cc_send_mms_req_for_fax_failover(
        icf_cc_pdb_st	*p_cc_pdb)
{    
    icf_return_t            ret_val = ICF_SUCCESS;    
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_stream_sdp_info_st  *p_audio_stream_info = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        /*Search Active AUDIO stream*/
        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type) &&
                (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
        {
            p_audio_stream_info = p_stream_sdp;
            p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_CLINE_ZERO;
        }
        /*Delete all SDPs in FAX stream and mark it as inactive and
         *nw_disallow*/
        else if (ICF_STREAM_TYPE_FAX == p_stream_sdp->stream_type)
        {
            p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_ACTIVE;
            p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_NW_ALLOW;
            p_stream_sdp->media_modify_type = ICF_NULL;

            if (ICF_NULL != p_stream_sdp->p_local_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_local_sdp), 
                        p_cc_pdb->p_ecode);

            if (ICF_NULL != p_stream_sdp->p_applied_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_applied_sdp), 
                        p_cc_pdb->p_ecode);

            if (ICF_NULL != p_stream_sdp->p_remote_received_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_remote_received_sdp), 
                        p_cc_pdb->p_ecode);

            if (ICF_NULL != p_stream_sdp->p_last_commited_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_last_commited_sdp), 
                        p_cc_pdb->p_ecode);
        }
        p_list = p_list->p_next;

    }

    /*If active audio stream is not found send failure*/
    if (ICF_NULL == p_audio_stream_info)
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
    }

    p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;
    icf_cc_process_fax_passthrough_req(p_cc_pdb);

    p_audio_stream_info->mm_applied_mode = ICF_MEDIA_MODE_SEND_RECV;
    p_audio_stream_info->p_local_sdp->media_mode = ICF_MEDIA_MODE_SEND_RECV;
    p_audio_stream_info->media_modify_type |= ICF_MODE_CHANGE;

    p_cc_pdb->current_event=ICF_CC_PA_FAX_REQ;

    if (ICF_TRUE == p_cc_pdb->mms_sent)
    {
        if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
        {
           ret_val = ICF_FAILURE;
        }
        else
        {
           ret_val = icf_cc_start_media_timer(p_cc_pdb);
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}


/*****************************************************************************
 ** FUNCTION:     icf_cc_send_mms_to_revert_audio_mode
 **
 ** DESCRIPTION:  This function is for reverting audio mode.
 *****************************************************************************/
icf_return_t
icf_cc_send_mms_to_revert_audio_mode(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_stream_sdp_info_st  *p_audio_stream_info = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        /*Reset media modify type */ 
        p_stream_sdp->media_modify_type = ICF_NULL;



        /*Search Active AUDIO stream*/
        if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp->stream_type) &&
                (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
        {
            p_audio_stream_info = p_stream_sdp;
        }
        /*Delete all SDPs in FAX stream and mark it as inactive and
         *nw_disallow*/
        else if (ICF_STREAM_TYPE_FAX == p_stream_sdp->stream_type)
        {
            p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_ACTIVE;
            p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_NW_ALLOW;


            if (ICF_NULL != p_stream_sdp->p_local_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_local_sdp), 
                        p_cc_pdb->p_ecode);

            if (ICF_NULL != p_stream_sdp->p_applied_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_applied_sdp), 
                        p_cc_pdb->p_ecode);

            if (ICF_NULL != p_stream_sdp->p_remote_received_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_remote_received_sdp), 
                        p_cc_pdb->p_ecode);

            if (ICF_NULL != p_stream_sdp->p_last_commited_sdp)
                icf_cmn_delete_internal_sdp_stream(p_cc_pdb->p_glb_pdb, 
                        &(p_stream_sdp->p_last_commited_sdp), 
                        p_cc_pdb->p_ecode);
        }
        p_list = p_list->p_next;
    }

    /*If active audio stream is not found send failure*/
    if (ICF_NULL == p_audio_stream_info)
    {
        ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_ALL_STREAMS_DELETED)
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ret_val;
    }

    p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;
    icf_cc_calc_mode_local_fax_fail(p_cc_pdb);

    if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    } 
    else if(ICF_TRUE == p_cc_pdb->mms_sent)
    {
        p_cc_pdb->p_call_ctx->ongoing_event = 
            ICF_CC_ONGOING_EVENT_LOCAL_MEDIA_REVERT;
        ret_val = icf_cc_start_media_timer(p_cc_pdb);
    } 
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:     icf_cc_calc_mode_app_change
 **
 ** DESCRIPTION:  This function is called when Application send 
 **               request to change the mode for stream. This function identify
 **               if MMS is required for this stream and do we need to send 
 **               update on the network for same.
 **               On getting Connect from PA, App mode should be changed to
 **               send recv for audio-video stream 
 *****************************************************************************/

icf_return_t icf_cc_calc_mode_app_change(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        if(ICF_STREAM_TYPE_TBCP != p_cc_pdb->p_stream_sdp_info->stream_type)
        {

            /* Application mode is set by Calling function */ 
            p_cc_pdb->remote_mode =
                p_cc_pdb->p_stream_sdp_info->p_applied_sdp->media_mode;

            /* Call function to derive the mode to be applied to MM for stream */
            icf_cc_derive_mode(p_cc_pdb);

            /*If derived mode is not same as already applied mode toward media manager
             * copy the mode and set mode change bitmask for stream */
            if(p_cc_pdb->derived_mode != 
                    p_cc_pdb->p_stream_sdp_info->mm_applied_mode)
            {
                /* Copy the mode and set media modify type as mode change */
                p_cc_pdb->p_stream_sdp_info->mm_applied_mode = 
                    p_cc_pdb->derived_mode;
                p_cc_pdb->p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;

            }
            /* if app_mode has changed from local_sdp_mode,
               set send_offer_to_nw flag as true */
            if(p_cc_pdb->application_mode != p_cc_pdb->p_stream_sdp_info->
                    p_local_sdp->media_mode)
            {
                /* Invoke function to take backup in last commited SDP */
                p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp->media_mode =
                    p_cc_pdb->p_stream_sdp_info->application_mode;

                ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,ICF_CC_SEND_OFFER_TO_NW);
                /* Local mode to be sent on network needs to be updated */
                p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode =
                    p_cc_pdb->application_mode;
            }

            /* Update application mode in stream sdp :TODOD revert mode in case of 
               MMS failure */
            p_cc_pdb->p_stream_sdp_info->application_mode = 
                p_cc_pdb->application_mode;

        }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;

}



/*****************************************************************************
 ** FUNCTION:     icf_cc_calc_mode_remote_reject_change
 **
 ** DESCRIPTION:  This function is called when remote  rejects media 
 **                modify request from Application
 *****************************************************************************/

icf_return_t icf_cc_calc_mode_remote_reject_change(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Remote rejected our change. Application mode backup is in
         * last_commited SDP Update application mode from last_commited SDP*/

     if(ICF_NULL == p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp)
     {
          return ICF_FAILURE;
     }

        p_cc_pdb->application_mode =
        p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp->media_mode;

    p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode =
        p_cc_pdb->application_mode;
    p_cc_pdb->remote_mode = 
        p_cc_pdb->p_stream_sdp_info->p_applied_sdp->media_mode;

    p_cc_pdb->p_stream_sdp_info->application_mode = p_cc_pdb->application_mode;
    /*Call function to derive mode to be applied */
    icf_cc_derive_mode(p_cc_pdb);

    /*If derived mode is not same as already applied mode toward media manager
     * copy the mode and send MMS toward network */
    if(p_cc_pdb->derived_mode != p_cc_pdb->p_stream_sdp_info->mm_applied_mode)
    {
        /* Copy the mode and set media modify type as mode change */
        p_cc_pdb->p_stream_sdp_info->mm_applied_mode = p_cc_pdb->derived_mode;
        p_cc_pdb->p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;

    }
    else
    {
        /* Raise unexpected event alarm */
        ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
            /* reset media modify type bitmask */
            p_cc_pdb->p_stream_sdp_info->media_modify_type &= (~(ICF_MODE_CHANGE));

    }



    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;



}

/*****************************************************************************
 ** FUNCTION:     icf_cc_calc_mode_app_reject_change
 **
 ** DESCRIPTION:  This function is called when application  rejects media 
 **                modify request from remote party
 *****************************************************************************/

icf_return_t icf_cc_calc_mode_app_reject_change(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Application has rejected the Change of MODE proposed by Remote
         * Copy backup from last commited_sdp */

    if(ICF_NULL == p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp)
    {
        return ICF_FAILURE;
    }

    p_cc_pdb->p_stream_sdp_info->p_applied_sdp->media_mode =
    p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp->media_mode;


    p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp->media_mode =
        p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp->media_mode;


    p_cc_pdb->application_mode = p_cc_pdb->p_stream_sdp_info->application_mode;
    p_cc_pdb->remote_mode = 
        p_cc_pdb->p_stream_sdp_info->p_applied_sdp->media_mode;

    /* Call function to derive mode to be applied to media manager */
    icf_cc_derive_mode(p_cc_pdb);

    /*If derived mode is not same as already applied mode toward media manager
     * copy the mode and send MMS toward network */
    if(p_cc_pdb->derived_mode != p_cc_pdb->p_stream_sdp_info->mm_applied_mode)
    {
        /* Copy the mode and set media modify type as mode change */
        p_cc_pdb->p_stream_sdp_info->mm_applied_mode = p_cc_pdb->derived_mode;
        p_cc_pdb->p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;

    }
    else
    {
        /* Raise unexpected event alarm */
        ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
            /* reset media modify type bitmask */
            p_cc_pdb->p_stream_sdp_info->media_modify_type &= (~(ICF_MODE_CHANGE));

    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;

}


/*****************************************************************************
 ** FUNCTION:     icf_cc_check_remote_mode_change
 **
 ** DESCRIPTION:  This function is called when remote request mode change in 
 offer or answer sdp 
 *****************************************************************************/
icf_return_t icf_cc_check_remote_mode_change(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Copy media mode in applied Sdp and last commited sdp */
        if(ICF_NULL != p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp)
        { 
            p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp->media_mode =
                p_cc_pdb->p_stream_sdp_info->p_applied_sdp->media_mode;
        }

    p_cc_pdb->application_mode = 
        p_cc_pdb->p_stream_sdp_info->application_mode;

    p_cc_pdb->remote_mode =
        p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp->media_mode;

    p_cc_pdb->p_stream_sdp_info->backup_mode = 
        p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode;

    if (ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                ICF_CALL_DIR))
    {
        if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,\
                    ICF_CALL_CONNECT_DONE))
        {
            p_cc_pdb->application_mode = ICF_MEDIA_MODE_SEND_RECV;
            /* Copy derived mode in local sdp */
            icf_cc_derive_mode(p_cc_pdb);
            p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode =
                p_cc_pdb->derived_mode;
            /* For incoming call before connect mode can 
             * be INACTIVE or SEND ONLY depending on media mode */
            if(ICF_MEDIA_MODE_SEND_ONLY == p_cc_pdb->remote_mode ||
                    ICF_MEDIA_MODE_INACTIVE == p_cc_pdb->remote_mode)
            {
                p_cc_pdb->derived_mode = ICF_MEDIA_MODE_INACTIVE;
            }
            else
            {
                p_cc_pdb->derived_mode = ICF_MEDIA_MODE_SEND_ONLY;
            }
        }
        else
        {
            /* it is incoming call in connected state. */
            /* Call function to derive the mode */
            icf_cc_derive_mode(p_cc_pdb);
            /*Copy the mode to be shared on network in local sdp */
            p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode =
                p_cc_pdb->derived_mode;

        }
    }
    else
    {
        /* it is outgoing call or incoming call in connected state. */
        /* Call function to derive the mode */
        icf_cc_derive_mode(p_cc_pdb);
        /*Copy the mode to be shared on network in local sdp */
        p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode =
            p_cc_pdb->derived_mode;

    }
    /* If derived mode is not same as currently applied mode, set bitmask
     * for mms change */
    if(p_cc_pdb->derived_mode != p_cc_pdb->p_stream_sdp_info->mm_applied_mode)
    {
        p_cc_pdb->p_stream_sdp_info->mm_applied_mode = p_cc_pdb->derived_mode;
        p_cc_pdb->p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;
    }


    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;

}

/*****************************************************************************
 ** FUNCTION:     icf_cc_derive_mode
 **
 ** DESCRIPTION:  This function is called to deruice te media mode on basis 
 **               of remote mode and applicatio mode
 *****************************************************************************/
icf_return_t icf_cc_derive_mode(icf_cc_pdb_st *p_cc_pdb)
{
    icf_media_mode_t   derived_mode = ICF_MEDIA_MODE_INVALID;
    icf_return_t       ret_val = ICF_SUCCESS;
    /* Copy remote mode from applied SDP */
    icf_media_mode_t   remote_mode = p_cc_pdb->remote_mode;
    icf_media_mode_t   app_mode = p_cc_pdb->application_mode;


    switch(app_mode)
    {
        case ICF_MEDIA_MODE_INACTIVE:
            switch(remote_mode)
            {
                /* If App mode is inactive, derived mdoe will
                 * be always inactive */
                case ICF_MEDIA_MODE_INACTIVE :
                case ICF_MEDIA_MODE_SEND_ONLY :
                case ICF_MEDIA_MODE_RECV_ONLY :
                case ICF_MEDIA_MODE_SEND_RECV :
                    derived_mode = ICF_MEDIA_MODE_INACTIVE;
                    break;
                default:
                    derived_mode = ICF_MEDIA_MODE_INVALID;
                    break;
            }

            break;
        case ICF_MEDIA_MODE_SEND_ONLY:
            switch(remote_mode)
            {
                case ICF_MEDIA_MODE_INACTIVE:
                    derived_mode = ICF_MEDIA_MODE_INACTIVE;
                    break;

                case ICF_MEDIA_MODE_SEND_ONLY:
                    derived_mode = ICF_MEDIA_MODE_INACTIVE;
                    break;
                case ICF_MEDIA_MODE_RECV_ONLY:
                    derived_mode = ICF_MEDIA_MODE_SEND_ONLY;
                    break;
                case ICF_MEDIA_MODE_SEND_RECV:
                    derived_mode = ICF_MEDIA_MODE_SEND_ONLY;
                    break;
                default:
                    derived_mode = ICF_MEDIA_MODE_INVALID;
                    break;
            }

            break;

        case ICF_MEDIA_MODE_RECV_ONLY:
            switch(remote_mode)
            {
                case ICF_MEDIA_MODE_INACTIVE:
                    derived_mode = ICF_MEDIA_MODE_INACTIVE;
                    break;

                case ICF_MEDIA_MODE_SEND_ONLY:
                    derived_mode = ICF_MEDIA_MODE_RECV_ONLY;
                    break;
                case ICF_MEDIA_MODE_RECV_ONLY:
                    derived_mode = ICF_MEDIA_MODE_INACTIVE;
                    break;
                case ICF_MEDIA_MODE_SEND_RECV:
                    derived_mode = ICF_MEDIA_MODE_RECV_ONLY;
                    break;
                default:
                    derived_mode = ICF_MEDIA_MODE_INVALID;
                    break;

            }
            break;

        case ICF_MEDIA_MODE_SEND_RECV:
            switch(remote_mode)
            {
                case ICF_MEDIA_MODE_INACTIVE:
                    derived_mode = ICF_MEDIA_MODE_INACTIVE;
                    break;

                case ICF_MEDIA_MODE_SEND_ONLY:
                    derived_mode = ICF_MEDIA_MODE_RECV_ONLY;
                    break;
                case ICF_MEDIA_MODE_RECV_ONLY:
                    derived_mode = ICF_MEDIA_MODE_SEND_ONLY;
                    break;
                case ICF_MEDIA_MODE_SEND_RECV:
                    derived_mode = ICF_MEDIA_MODE_SEND_RECV;
                    break;
                default:
                    derived_mode = ICF_MEDIA_MODE_INVALID;
                    break;

            }
            break;

        default:
            derived_mode = ICF_MEDIA_MODE_INVALID;
            break;
    }
    p_cc_pdb->derived_mode = derived_mode;
    if(derived_mode == ICF_MEDIA_MODE_INVALID)
    {
        ICF_CC_TRACE(p_cc_pdb, ICF_UNEXPECTED_EVENT)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;

}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_back_local_sdp
 *
 * DESCRIPTION:     This function takes backup of local SDP from last
 *                  committed SDP
 *
 *****************************************************************************/
icf_return_t icf_cc_back_local_sdp(
        INOUT  icf_cc_pdb_st        *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    icf_mmi_mms_resp_st     *p_mms_resp = ICF_NULL;
    icf_uint8_t             cnt = 0;

    p_mms_resp =
        (icf_mmi_mms_resp_st*)p_cc_pdb->p_internal_msg->p_msg_data;

    /*count modify streams success and failures*/
    if (ICF_MMI_MMS_ACTION_MODIFY_RESP & p_mms_resp->bit_mask)
    {
        for (cnt = 0; (cnt < p_mms_resp->mod_stream_count) && (cnt < ICF_MAX_STREAM_PER_CALL ); cnt++)
        {
            /*If stream modification request is failed*/
            if (ICF_FALSE == p_mms_resp->modified_stream_resp[cnt].status_success)
            {
                p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

                while (ICF_NULL != p_list)
                {
                    p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;
                    /* SPR14647: Added stream_status active check and null check */
                    if ((p_stream_sdp->stream_id == 
                                p_mms_resp->modified_stream_resp[cnt].stream_id) &&
                            (p_stream_sdp->stream_status & ICF_MEDIA_STREAM_ACTIVE) &&
                            (ICF_NULL != p_stream_sdp->p_last_commited_sdp))
                    {                        
                        ret_val = icf_cc_copy_internal_sdp_stream(p_cc_pdb, 
                                p_stream_sdp->p_last_commited_sdp, /*Source*/
                                &(p_stream_sdp->p_local_sdp) /*Destination*/);

                        break;
                    }
                    if (ICF_FAILURE == ret_val)
                        break;
                    p_list = p_list->p_next;
                }
            }
        }
    }

    return ret_val;    
}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_get_call_type
 *
 * DESCRIPTION:     This function converts stream type to call type
 *
 *****************************************************************************/
    icf_void_t 
icf_cc_get_call_type(INOUT  icf_cc_pdb_st        *p_cc_pdb,
        IN  icf_media_type_t			stream_type)
{
    switch(stream_type)
    {
        case ICF_STREAM_TYPE_AUDIO:
            p_cc_pdb->call_type = ICF_CALL_TYPE_AUDIO;
            break;

        case ICF_STREAM_TYPE_VIDEO:
            p_cc_pdb->call_type = ICF_CALL_TYPE_VIDEO;
            break;

        case ICF_STREAM_TYPE_TBCP:
            p_cc_pdb->call_type = ICF_CALL_TYPE_TBCP;
            break;

        case ICF_STREAM_TYPE_FAX:
            p_cc_pdb->call_type = ICF_CALL_TYPE_T38;
            break;
    }

    return;

}



/******************************************************************************
 *
 * FUNCTION:        icf_cc_update_applied_sdp_mode
 *
 * DESCRIPTION:     This function updates the media mode in applied sdp
 *
 *****************************************************************************/

icf_void_t icf_cc_update_applied_sdp_mode(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{

    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;


    /*Check thru the MMS response, if any stream were deleted
      then it can not be offer rejected*/
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    /*Search the corresponding node in call context for
     * that stream ID*/
    while (ICF_NULL != p_list)
    {
        p_stream_sdp = (icf_stream_sdp_info_st*)p_list->p_payload;

        if ( ICF_MEDIA_STREAM_ACTIVE == (p_stream_sdp->stream_status & \
                    ICF_MEDIA_STREAM_ACTIVE) &&
            (ICF_STREAM_TYPE_TBCP != p_stream_sdp->stream_type) &&
            (ICF_STREAM_PENDING_DELETE !=
               (p_stream_sdp->media_modify_type & ICF_STREAM_PENDING_DELETE)))
        {
            /* SPR 18891: While resolving Trace issues, on basic start fax, crash 
             * was observed here as the applied sdp and remote received sdp is not
             * present here. Therefore the NULL check has been added so that correct
             * mode is updated when the required SDPs are present.
             */ 
            if((ICF_NULL != p_stream_sdp->p_applied_sdp) &&
                    (ICF_NULL != p_stream_sdp->p_remote_received_sdp))
            {
               p_stream_sdp->p_applied_sdp->media_mode =
                 p_stream_sdp->p_remote_received_sdp->media_mode;
            }            
        }
        p_list = p_list->p_next;
    }

}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_pa_connect_send_mms_req
 *
 * DESCRIPTION:     This function checks for rejected streams present
 *                  in the call context and marks the rejected streams 
 *                  for deletion.This function also updates the application 
 *                  mode and invokes function to derive the mode.
 *
 *****************************************************************************/
icf_return_t icf_cc_pa_connect_send_mms_req(
        INOUT  icf_cc_pdb_st        *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_internal_list_st	*p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st	*p_stream_sdp_info_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* This while loop will look for the streams deleted by the appn and will
         * mark them deleted in their corresponding icf_stream_sdp_info structure.
         */

        ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
        p_stream_sdp_info_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    while (ICF_NULL != p_stream_sdp_info_node)
    {
        p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
            (p_stream_sdp_info_node->p_payload);
        if (ICF_MEDIA_STREAM_ACTIVE ==
                (p_stream_sdp_info_data->stream_status & ICF_MEDIA_STREAM_ACTIVE))
        {
            p_cc_pdb->stream_id = p_stream_sdp_info_data->stream_id;
            if (ICF_TRUE == icf_cc_chk_if_stream_is_rejected(p_cc_pdb))
            {
                p_stream_sdp_info_data->media_modify_type
                    = ICF_STREAM_DELETE;
            }
            if ((ICF_STREAM_DELETE != (ICF_STREAM_DELETE & 
                            p_stream_sdp_info_data->media_modify_type)))
            {
                if(ICF_STREAM_TYPE_TBCP != p_stream_sdp_info_data->stream_type)
                {
                   if ( ICF_NULL != p_cc_pdb->p_call_ctx->recvd_media_attrib_backup)
                    {
                        p_cc_pdb->remote_mode = 
                            p_cc_pdb->p_call_ctx->recvd_media_attrib_backup;
                        /*CSR_1_5919138 fix start*/
                        p_stream_sdp_info_data->p_remote_received_sdp->media_mode =
                            p_cc_pdb->remote_mode;
                        /*CSR_1_5919138 fix end*/
                        /*CSR_1-6131201 fix*/
                        /*In case the P-Early-Media header was received, IPTK shall
                          change the remote+applied mode in its database during 
                          processing 
                          the call CONNECT to the mode received in SDP of the 
                          initial INVITE*/
                        p_stream_sdp_info_data->p_applied_sdp->media_mode=p_cc_pdb->remote_mode;                        
                        p_stream_sdp_info_data->p_local_sdp->media_mode =
                            icf_cc_get_remote_mode(p_cc_pdb);
                        p_cc_pdb->p_call_ctx->recvd_media_attrib_backup = ICF_NULL;
                    }
                   else if(ICF_NULL != p_stream_sdp_info_data->p_applied_sdp)
                    {
                    p_cc_pdb->remote_mode = 
                        p_stream_sdp_info_data->p_applied_sdp->media_mode;
                    }
                    else
                    {
                        /* it is a NFS call */
                        p_cc_pdb->remote_mode = ICF_MEDIA_MODE_SEND_RECV;
                    } 
 
                    p_cc_pdb->application_mode = ICF_MEDIA_MODE_SEND_RECV;
                    p_stream_sdp_info_data->application_mode =
                        ICF_MEDIA_MODE_SEND_RECV;
                    if (ICF_FAILURE == icf_cc_derive_mode(p_cc_pdb))
                    {
                        ret_val = ICF_FAILURE;
                    }
                    /*If derived mode is not same as already applied mode 
                     * toward media manager
                     * copy the mode and set mode change bitmask for stream */
                    else if(p_cc_pdb->derived_mode != 
                            p_stream_sdp_info_data->mm_applied_mode)
                    {
                        /* Copy the mode and set media modify type as mode change */
                        p_stream_sdp_info_data->mm_applied_mode = 
                            p_cc_pdb->derived_mode;
                        p_stream_sdp_info_data->media_modify_type |=
                            ICF_MODE_CHANGE;
                    }
                }
            }
        }
        p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
    }
    if (ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    return ret_val;
}

/******************************************************************************
 *
 * FUNCTION:        icf_cc_chk_if_stream_is_rejected
 *
 * DESCRIPTION:     This function checks for rejected streams present
 *                  in the call context
 *
 *****************************************************************************/
icf_boolean_t icf_cc_chk_if_stream_is_rejected(
        INOUT  icf_cc_pdb_st        *p_cc_pdb)
{
    icf_uint8_t             counter = 0;
    icf_boolean_t           is_present = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        for (counter=0;(counter<p_cc_pdb->p_call_ctx->rejected_stream_count) && (counter < ICF_MAX_STREAM_PER_CALL);counter++)
        {
            if(p_cc_pdb->stream_id == 
                    p_cc_pdb->p_call_ctx->rejected_stream_list[counter].stream_id)
            {
                is_present = ICF_TRUE;
                break;
            }
        }
    return is_present;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_set_local_sdp_media_mode_from_rem_mode
 *
 * DESCRIPTION:     This function copies remote received media mode in local_sdp
 ******************************************************************************/
icf_void_t     icf_cc_set_local_sdp_media_mode_from_rem_mode(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;
    icf_internal_list_st        *p_list = ICF_NULL;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_sdp =
            (icf_stream_sdp_info_st*)(p_list->p_payload);
        if(ICF_MEDIA_STREAM_ACTIVE == (p_sdp->stream_status & 
                    ICF_MEDIA_STREAM_ACTIVE))
        {

            if (ICF_NULL != p_sdp->p_remote_received_sdp)
            {
                p_cc_pdb->remote_mode = p_sdp->p_remote_received_sdp->media_mode;
                p_sdp->p_local_sdp->media_mode = icf_cc_get_remote_mode(p_cc_pdb);
            }
            else
            {
                p_sdp->p_local_sdp->media_mode = ICF_MEDIA_MODE_SEND_RECV;
            }

        }
        p_list = (icf_internal_list_st *)p_list->p_next;
    }


}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_get_remote_mode
 *
 * DESCRIPTION:     This function sets the remote mode as required byMMI
 *                  Inactive and send recv remain same
 *                  while send_only change to recv_only and
 *                  recv_only changes to send_only
 *
 ******************************************************************************/

icf_media_mode_t icf_cc_get_remote_mode(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
#ifdef ICF_TRACE_ENABLE    
    icf_return_t           ret_val = ICF_SUCCESS;
#endif

    /* THis variable store the media mode received from remote */
    icf_media_mode_t       remote_media_mode = ICF_MEDIA_MODE_INVALID;

    /* This variable store the meida mode which
     * should be applied to MMI*/
    icf_media_mode_t       applied_media_mode = ICF_MEDIA_MODE_INVALID;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace*/

        remote_media_mode = p_cc_pdb->remote_mode;

    if(ICF_MEDIA_MODE_SEND_ONLY == remote_media_mode)
    {
        applied_media_mode = ICF_MEDIA_MODE_RECV_ONLY;
    }
    else if(ICF_MEDIA_MODE_RECV_ONLY == remote_media_mode)
    {
        applied_media_mode = ICF_MEDIA_MODE_SEND_ONLY;
    }
    else if(ICF_MEDIA_MODE_INVALID == remote_media_mode)
    {
        applied_media_mode = ICF_MEDIA_MODE_SEND_RECV;
    }
    else
    {
        applied_media_mode = remote_media_mode;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return applied_media_mode;

}


/******************************************************************************
 *
 * FUNCTION:        icf_cc_process_app_stream_capab_req
 *
 * DESCRIPTION:     This Function will be getting pointer to list of type
 *					icf_stream_capab_st through p_cc_pdb->p_list_1.
 *					Now this function will get the stream capab list's 
 *                  node one by one and extract the corresponding stream from
 *                  p_cc_pdb->p_call_ctx->p_stream_sdp_info.
 *                  if the corresponding stream does not exist then we have to 
 *                  clear the call by setting some bit in p_cc_pdb->common_bitmask.
 *                  if the corresponding stream exists then copy encoded attributes's
 *                  pointer from stream capab list into the local sdp's encoded
 *                  attributes.
 *****************************************************************************/
icf_return_t icf_cc_process_app_stream_capab_req(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;
    icf_list_st				*p_stream_capab_node = p_cc_pdb->p_list_1;
    icf_stream_capab_st		*p_stream_capab_data = ICF_NULL;
    icf_uint32_t			identifier = 0;
    icf_stream_sdp_info_st  *p_stream_sdp_info = ICF_NULL;
    icf_sdp_stream_st		*p_sdp_stream = ICF_NULL;
    icf_internal_list_st	*p_stream_sdp_info_node = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Following while loop will traverse each stream capab node 
         * and extract its encoded attribute list pointer(if exists) and 
         * place that pointer into encoded attribute pointer of local sdp
         * of corresponding stream, if corresponding stream does not exist
         * then set the common_bitmask in p_cc_pdb for clearing the call.
         */
        while(ICF_NULL != p_stream_capab_node)
        {
            p_stream_capab_data = (icf_stream_capab_st*)
                (p_stream_capab_node->p_data);
            identifier = p_stream_capab_data->stream_id;
            if (ICF_NULL == (p_stream_sdp_info_node = 
                        icf_cmn_get_node_in_internal_list(p_cc_pdb->p_call_ctx->
                            p_stream_sdp_info,&identifier)))
            {
                p_cc_pdb->common_bitmask |= ICF_STREAM_ID_VALIDATION_FAIL;
                ret_val = ICF_FAILURE;
                break;
            }
            else
            {
                p_stream_sdp_info = (icf_stream_sdp_info_st*)
                    (p_stream_sdp_info_node->p_payload) ;
                if (ICF_MEDIA_STREAM_ACTIVE != 
                        (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE))
                {
                    p_cc_pdb->common_bitmask |= ICF_STREAM_ID_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    break;
                }
                p_sdp_stream = &(p_stream_sdp_info->p_local_sdp->sdp_stream); 
                if (ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT == 
                        (p_stream_capab_data->bit_mask & ICF_STREAM_CAPAB_ENC_ATTRIB_PRESENT))
                {
                    /* Encoded attributes are present in this stream capab data.
                     * copy them into local sdp and free the previous encoded attributes
                     * (if exists).
                     */
                    if (ICF_SDP_STREAM_ENCPARAMS_PRESENT == 
                            (p_sdp_stream->bit_mask & ICF_SDP_STREAM_ENCPARAMS_PRESENT))
                    {
                        /* if already there were encoded parameters then free that list.*/
                        icf_cmn_delete_list(p_cc_pdb->p_glb_pdb, 
                                &(p_sdp_stream->p_encoded_attributes), 
                                ICF_MEM_COMMON);
                    }
                    p_sdp_stream->bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
                    p_sdp_stream->p_encoded_attributes = 
                        p_stream_capab_data->p_encoded_attributes;
                    p_stream_capab_data->p_encoded_attributes = ICF_NULL;
                }
            }
            p_stream_capab_node = p_stream_capab_node->p_next;
        }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_fax_passthrough_req
 *
 * DESCRIPTION:     This function validates and processes the fax passthrough 
 *                  request
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_fax_passthrough_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t			ret_val = ICF_SUCCESS;

    icf_short_string_st   applied_codec_name;
    icf_fax_pt_codec_t    applied_pt_codec = 0;
    icf_fax_pt_codec_t    requested_pt_codec = p_cc_pdb->p_call_ctx->pref_codec;
    icf_stream_sdp_info_st	*p_stream_sdp_info = p_cc_pdb->p_stream_sdp_info;
    icf_uint8_t             pt_param = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        icf_port_memcpy(&applied_codec_name,
                &((icf_codec_attrib_st *)p_stream_sdp_info->p_applied_sdp->
                    sdp_stream.p_codec_info_list->p_data)->codec_name,
                ICF_PORT_SIZEOF(icf_short_string_st));

    if(ICF_NEGOTIATE_PT_PARAM_PRESENT &
                  p_cc_pdb->p_call_ctx->common_bitmask_2)
    {
        pt_param = p_cc_pdb->p_call_ctx->pt_param;
    } 


    if(0 == icf_port_strcmp((icf_uint8_t *)"PCMA",applied_codec_name.str))
    {
        applied_pt_codec = ICF_PREF_CODEC_FAX_PT_PCMA;
    }
    else if(0 == icf_port_strcmp((icf_uint8_t *)"PCMU",applied_codec_name.str))
    {
        applied_pt_codec = ICF_PREF_CODEC_FAX_PT_PCMU;
    }

    if(requested_pt_codec == applied_pt_codec)
    {
        if(ICF_AUDIO_PROF_SILN_SUPP_PRESENT ==
                (ICF_AUDIO_PROF_SILN_SUPP_PRESENT & p_stream_sdp_info->
                 p_applied_sdp->sdp_stream.media_stream_param.audio_profile.bit_mask))
        {
            /* commited codec is G711*/
            /*now check for silence suppression */
            icf_boolean_t silence_supp = p_stream_sdp_info->p_applied_sdp->
                sdp_stream.media_stream_param.audio_profile.
                silence_suppression;

            if(ICF_TRUE == silence_supp)
            {
                p_stream_sdp_info->p_local_sdp->
                    sdp_stream.media_stream_param.audio_profile.
                    silence_suppression = ICF_FALSE;

                p_stream_sdp_info->media_modify_type =
                    ICF_LOCAL_SDP_MEDIA_CHANGE;

                p_cc_pdb->mms_sent = ICF_TRUE;
            }
            else
            {
                p_cc_pdb->mms_sent = ICF_FALSE;
            }
        }
        else
        {
            p_stream_sdp_info->p_local_sdp->sdp_stream.media_stream_param.
                audio_profile.bit_mask |= ICF_AUDIO_PROF_SILN_SUPP_PRESENT;

            p_stream_sdp_info->p_local_sdp->sdp_stream.media_stream_param.
                audio_profile.silence_suppression = ICF_FALSE;

            p_stream_sdp_info->media_modify_type =
                ICF_LOCAL_SDP_MEDIA_CHANGE;

            p_cc_pdb->mms_sent = ICF_TRUE;
        }
    }
    else
    {

        /* comitted codec is not G7111 */
        if(ICF_PREF_CODEC_FAX_PT_PCMA == requested_pt_codec)
        {
            icf_port_strcpy(((icf_codec_attrib_st *)p_stream_sdp_info->
                        p_local_sdp->sdp_stream.p_codec_info_list->
                        p_data)->codec_name.str,(icf_uint8_t *)"PCMA");

            ((icf_codec_attrib_st *)p_stream_sdp_info->p_local_sdp->
             sdp_stream.p_codec_info_list->p_data)->codec_num = 8;
        }
        else if(ICF_PREF_CODEC_FAX_PT_PCMU == requested_pt_codec)
        {
            icf_port_strcpy(((icf_codec_attrib_st *)p_stream_sdp_info->
                        p_local_sdp->sdp_stream.p_codec_info_list->
                        p_data)->codec_name.str,(icf_uint8_t *)"PCMU");

            ((icf_codec_attrib_st *)p_stream_sdp_info->p_local_sdp->
             sdp_stream.p_codec_info_list->p_data)->codec_num = 0;
        }


        p_stream_sdp_info->p_local_sdp->sdp_stream.media_stream_param.
            audio_profile.bit_mask |= ICF_AUDIO_PROF_SILN_SUPP_PRESENT;

        p_stream_sdp_info->p_local_sdp->
            sdp_stream.media_stream_param.audio_profile.
            silence_suppression = ICF_FALSE;

        p_stream_sdp_info->media_modify_type =
            ICF_LOCAL_SDP_CODEC_CHANGE;
        p_cc_pdb->mms_sent = ICF_TRUE;
    }


    /*If pt_param is recieved in ICF_START_FAX_REQ */ 
    if(ICF_NEGOTIATE_PT_PARAM_PRESENT &
            p_cc_pdb->p_call_ctx->common_bitmask_2)   
    {
       if(!(ICF_NEGOTIATE_PARAM_SILEN_SUPP & pt_param))
       {
            p_stream_sdp_info->p_local_sdp->sdp_stream.media_stream_param.
				    audio_profile.bit_mask &= ~(ICF_AUDIO_PROF_SILN_SUPP_PRESENT);
       } 
       if(ICF_NEGOTIATE_PARAM_VBD & pt_param)
       {
           ((icf_codec_attrib_st *)p_stream_sdp_info->p_local_sdp->
                     sdp_stream.p_codec_info_list->p_data)->codec_params.audio_param.bit_mask |= 
                                 ICF_AUDIO_CODEC_PARAM_VBD_VAL;
         
           ((icf_codec_attrib_st *)p_stream_sdp_info->p_local_sdp->
                  sdp_stream.p_codec_info_list->p_data)->codec_params.audio_param.vbd_val = ICF_TRUE;

           p_stream_sdp_info->media_modify_type =
				    ICF_LOCAL_SDP_CODEC_CHANGE;
           p_cc_pdb->mms_sent = ICF_TRUE;
       }
       

       /*If pt_param bit is ON in ICF_START_FAX_REQ API then it means application wants
         to send ICF_MODIFY_MEDIA_SESSION_REQ with Echo cancellation as ON */

      if(ICF_NEGOTIATE_PARAM_ECHO_CANC & pt_param)
      {
          
           if (ICF_FALSE == p_cc_pdb->p_call_ctx->echo_canc)
           {
               p_cc_pdb->p_call_ctx->common_bitmask |= ICF_ECHO_CANC_PRESENT;
               p_cc_pdb->p_call_ctx->echo_canc = ICF_TRUE;


               if(ICF_FALSE == p_cc_pdb->mms_sent)
               {
                   p_stream_sdp_info->media_modify_type =
                       ICF_LOCAL_SDP_MEDIA_CHANGE;
                   p_cc_pdb->mms_sent = ICF_TRUE;
               }
           }
       }

       /*If pt_param bit is OFF in ICF_START_FAX_REQ API then it means application wants
         to send ICF_MODIFY_MEDIA_SESSION_REQ with Echo cancellation as OFF */
       else
       {
           if ((ICF_TRUE == p_cc_pdb->p_call_ctx->echo_canc) || 
                   (p_cc_pdb->p_call_ctx->common_bitmask !=
                    (p_cc_pdb->p_call_ctx->common_bitmask &
                     ICF_ECHO_CANC_PRESENT)))
           {
               p_cc_pdb->p_call_ctx->echo_canc = ICF_FALSE;
               p_cc_pdb->p_call_ctx->common_bitmask |= ICF_ECHO_CANC_PRESENT;

           if(ICF_FALSE == p_cc_pdb->mms_sent)
           {
               p_stream_sdp_info->media_modify_type =
            		  	     ICF_LOCAL_SDP_MEDIA_CHANGE;
               p_cc_pdb->mms_sent = ICF_TRUE;
           }
       }
    }
    }
    /*If pt_param is not recieved in ICF_START_FAX_REQ then as per draft
      of fax Echo cancellation should be sent as ON in ICF_MODIFY_MEDIA_
      SESSION_REQ */ 
    else
    {
        if(ICF_TRUE == p_cc_pdb->mms_sent)
        {
            p_cc_pdb->p_call_ctx->common_bitmask |= ICF_ECHO_CANC_PRESENT;
            p_cc_pdb->p_call_ctx->echo_canc = ICF_TRUE;
        }
        else if(ICF_FALSE == p_cc_pdb->p_call_ctx->echo_canc)
        {
            p_cc_pdb->p_call_ctx->common_bitmask |= ICF_ECHO_CANC_PRESENT;
            p_stream_sdp_info->media_modify_type =
                ICF_LOCAL_SDP_MEDIA_CHANGE;

            p_cc_pdb->p_call_ctx->echo_canc = ICF_TRUE;
            p_cc_pdb->mms_sent = ICF_TRUE;
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_get_node_from_stream_list
 *
 * DESCRIPTION:     This function will return pointer node of list of type
 *					icf_stream_capab_st, if corresponding matching stream type
 *					is found in the list.
 ******************************************************************************/
icf_internal_list_st* icf_cc_get_node_from_stream_list
(
 icf_internal_list_st	*p_list_node,
 icf_media_type_t	stream_type
 )
{
    icf_stream_sdp_info_st	*p_node_data = ICF_NULL;

    while (ICF_NULL != p_list_node)
    {
        p_node_data = (icf_stream_sdp_info_st *)
            (p_list_node->p_payload);

        if (p_node_data->stream_type == stream_type)
        {
            break;
        }
        p_list_node = p_list_node->p_next;
    }

    return p_list_node;
}



/*****************************************************************************
 ** FUNCTION:     icf_cc_calc_mode_local_fax_fail
 **
 ** DESCRIPTION:  This function is called when remote  rejects media 
 **                modify request from Application
 *****************************************************************************/

icf_return_t icf_cc_calc_mode_local_fax_fail(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Remote rejected our change. Application mode backup is in
         * last_commited SDP Update application mode from last_commited SDP*/


        p_cc_pdb->application_mode =
        p_cc_pdb->p_stream_sdp_info->p_last_commited_sdp->media_mode;

    p_cc_pdb->p_stream_sdp_info->p_local_sdp->media_mode =
        p_cc_pdb->application_mode;
    p_cc_pdb->remote_mode = 
        p_cc_pdb->p_stream_sdp_info->p_applied_sdp->media_mode;

    /*Call function to derive mode to be applied */
    icf_cc_derive_mode(p_cc_pdb);

    /*If derived mode is not same as already applied mode toward media manager
     * copy the mode and send MMS toward network */
    if(p_cc_pdb->derived_mode != p_cc_pdb->p_stream_sdp_info->mm_applied_mode)
    {
        /* Copy the mode and set media modify type as mode change */
        p_cc_pdb->p_stream_sdp_info->mm_applied_mode = p_cc_pdb->derived_mode;
        p_cc_pdb->p_stream_sdp_info->media_modify_type |= ICF_MODE_CHANGE;

    }
    else
    {
        /* Raise unexpected event alarm */
        ICF_CC_TRACE(p_cc_pdb,ICF_UNEXPECTED_EVENT)
            /* reset media modify type bitmask */
            p_cc_pdb->p_stream_sdp_info->media_modify_type &= (~(ICF_MODE_CHANGE));

    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_cc_fill_remote_call_hold_status
 **
 ** DESCRIPTION:  This function is called to fill remote call hold status 
 **               in call modify request
 *****************************************************************************/

icf_return_t
icf_cc_fill_remote_call_hold_status(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;

    icf_internal_list_st		*p_stream_sdp_info_node =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;

    icf_list_st                 *p_temp_data = ICF_NULL;

    icf_call_modify_media_data_st  *p_call_modify_data = ICF_NULL;	

    p_cc_pdb->count = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)


        /* Copy this data from p_list_1  */
        p_temp_data =p_cc_pdb->p_list_1 ;

    if(ICF_NULL == p_temp_data)
    {
        return ret_val;
    }
    /* Following while loop traverses the each stream node by node and count
     * for rejected streams and make list of stream capabs from the applied
     * sdps of Active streams.
     */
    while (ICF_NULL != p_temp_data)
    {
        p_call_modify_data = (icf_call_modify_media_data_st *)
            (p_temp_data->p_data);

        while(ICF_NULL!= p_stream_sdp_info_node)
        {
            p_stream_sdp_info = (icf_stream_sdp_info_st*)
                p_stream_sdp_info_node->p_payload;
            if(p_stream_sdp_info->stream_id !=
                    p_call_modify_data->stream_id)
            {
                p_stream_sdp_info_node = p_stream_sdp_info_node->p_next; 
                /*Stream ids don't match. Continue inner while loop */
                continue;
            }
            /* SPR 18589 : if the media stream status is aet as remote
             * resume then send the trace accordingly */
            else if(ICF_MEDIA_STREAM_REMOTE_RESUME == (p_stream_sdp_info->
                        stream_status & ICF_MEDIA_STREAM_REMOTE_RESUME))
            {
#ifdef ICF_TRACE_ENABLE
                p_cc_pdb->call_hold_status = 0;
                p_cc_pdb->stream_type = p_stream_sdp_info->stream_type;
                ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_MODIFY_IND);
#endif
                /* Now we can exit from this inner while loop*/
                break;

            }
            /* Stream id is found in call context. 
             * Set the bitmask in call modify data */
            else if(ICF_MEDIA_STREAM_REMOTE_HOLD == (p_stream_sdp_info->
                        stream_status & ICF_MEDIA_STREAM_REMOTE_HOLD))
            {
                p_call_modify_data->bit_mask |=
                    ICF_CALL_MOD_STREAM_ON_REMOTE_HOLD;

#ifdef ICF_TRACE_ENABLE
                p_cc_pdb->call_hold_status = 1;
                p_cc_pdb->stream_type = p_stream_sdp_info->stream_type;
                ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_MODIFY_IND);
#endif
                /* Now we can exit from this inner while loop*/
                break;

            }
            else
            {
                /* Added to send trace information to SM */
#ifdef ICF_TRACE_ENABLE
                p_cc_pdb->call_hold_status=0;
                p_cc_pdb->stream_type = p_stream_sdp_info->stream_type;
                ICF_CC_APP_TRACE(p_cc_pdb,ICF_TRACE_CALL_MODIFY_IND);
#endif
                p_stream_sdp_info_node = p_stream_sdp_info_node->p_next; 
                /*Stream ids don't match. Continue inner while loop */
                continue;
            }
        }
        p_temp_data = p_temp_data->p_next;

    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}



/*****************************************************************************
 ** FUNCTION:     icf_cc_set_set_remote_call_hold_status
 **
 ** DESCRIPTION:  This function is called to set remote call hold status 
 **               in stream data
 *****************************************************************************/

icf_return_t
icf_cc_set_remote_call_hold_status(
        icf_cc_pdb_st	*p_cc_pdb)
{
    icf_return_t				ret_val = ICF_SUCCESS;

    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;

    icf_transport_address_st    *p_rem_rtp_adr = ICF_NULL;

    icf_uint32_t                rem_ip_addr = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        p_stream_sdp_info = p_cc_pdb->p_stream_sdp_info;


    if(ICF_NULL == p_stream_sdp_info)
    {
        return ret_val;
    }
    else
    {
        p_rem_rtp_adr = &(p_stream_sdp_info->p_remote_received_sdp->
                sdp_stream.stream_address); 

        rem_ip_addr = ICF_MAKE_32BIT_INT(
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_1,
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_2,
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_3,
                p_rem_rtp_adr->addr.addr.ipv4_addr.octet_4);

        /* Reset call hold mode in call context */
        if((ICF_MEDIA_MODE_INACTIVE == p_stream_sdp_info->p_remote_received_sdp
                    ->media_mode) || (ICF_MEDIA_MODE_SEND_ONLY == 
                        p_stream_sdp_info->p_remote_received_sdp->media_mode))
        {
            /* Set the bitmask for local hold in p_stream_sdp_info */
            p_stream_sdp_info->stream_status |= 
                ICF_MEDIA_STREAM_REMOTE_HOLD;
        }
        else if( ICF_NULL == rem_ip_addr)
        {
            /* Set the bitmask for local hold in p_stream_sdp_info */
            p_stream_sdp_info->stream_status |= 
                ICF_MEDIA_STREAM_REMOTE_HOLD;
        }
        /* SPR 18589 : if it is not remote hold then mark it remote resume if
         * it is already marked as remote hold , but do not reset remote resume here
         */
        else if(ICF_MEDIA_STREAM_REMOTE_HOLD & p_stream_sdp_info->stream_status)
        {
            p_stream_sdp_info->stream_status |= 
                ICF_MEDIA_STREAM_REMOTE_RESUME;
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}

#ifdef ICF_PATCH_FOR_OFFER_IN_200
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_handle_non_fs_reinvite
 *
 * DESCRIPTION:     This function handles non FS reinvite as a special
 *                  case by sending mode as sendrecv in 200 OK
 ******************************************************************************/
icf_void_t     icf_cc_handle_non_fs_reinvite(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;
    icf_internal_list_st        *p_list = ICF_NULL;
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_boolean_t               is_other_call_active = ICF_FALSE;

#if 0
    ret_val = icf_dbm_check_other_call_act(p_cc_pdb->p_glb_pdb,
                                           &is_other_call_active,
                                           p_cc_pdb->p_ecode);
    /* If there is no other call active on the line*/
    if(ICF_SUCCESS == ret_val &&  ICF_FALSE == is_other_call_active)
    {
#endif
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

        while (ICF_NULL != p_list)
        {
           p_sdp =
               (icf_stream_sdp_info_st*)(p_list->p_payload);

           if (ICF_NULL != p_sdp->p_local_sdp)
           {
              p_sdp->p_local_sdp->media_mode = 
                p_sdp->application_mode;
           }

           p_list = (icf_internal_list_st *)p_list->p_next;
        }
#if 0
    }
#endif    
}

#endif

/*****************************************************************************
 ** FUNCTION:     icf_cc_create_pref_profile_node_from_local_sdp
 **
 ** DESCRIPTION:  This Function will be creating node of type icf_stream_capab_st
 **               from the local sdp of the stream whose icf_stream_sdp_info_st
 **               block is being passed to this function through p_cc_pdb->p_stream_sdp_info.
 **               After making this node, it will be added to the 
 **               p_cc_pdb->p_call_ctx->p_pref_media_profile.
 *****************************************************************************/
    icf_return_t 
icf_cc_create_pref_profile_node_from_local_sdp(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_stream_sdp_info_st            *p_stream_sdp_info = ICF_NULL;
    icf_stream_capab_st               *p_stream_capab = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Access p_cc_pdb->p_stream_sdp_info in local p_stream_sdp_info,
         * bcoz this will be further used for creating stream capab node.
         */ 
        p_stream_sdp_info = p_cc_pdb->p_stream_sdp_info;

    ICF_MEMGET(p_cc_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_stream_capab_st),
            ICF_MEM_COMMON,
            p_stream_capab,
            ICF_RET_ON_FAILURE,
            p_cc_pdb->p_ecode,
            ret_val)
    /*Klocwork Warning Removal*/
    ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,p_stream_capab)

        icf_port_memset(p_stream_capab,0,ICF_PORT_SIZEOF(icf_stream_capab_st)); 

    /* Fill stream id and stream type from p_stream_sdp_info received.*/
    p_stream_capab->stream_id = p_stream_sdp_info->stream_id;			 
    p_stream_capab->stream_type = p_stream_sdp_info->stream_type;

    if (ICF_NULL != p_stream_sdp_info->p_local_sdp)
    {
        if (ICF_NULL != p_stream_sdp_info->p_local_sdp->sdp_stream.p_codec_info_list)
        {
            ret_val = icf_cmn_clone_codec_attrib_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_stream_capab->p_codec_info_list),		
                    p_stream_sdp_info->p_local_sdp->sdp_stream.p_codec_info_list);

            p_stream_capab->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT;         
           /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                        p_stream_capab->p_codec_info_list)
        }
    }
    if (ICF_FAILURE != ret_val)
    {
        ret_val = icf_cmn_add_node_to_list(p_cc_pdb->p_glb_pdb,
                &(p_cc_pdb->p_call_ctx->p_pref_media_profile),
                p_stream_capab);
    }
    else
    {
        /*Free p_codec_info_list before free the p_stream_capab*/
        icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                         &(p_stream_capab->p_codec_info_list),
                                            p_cc_pdb->p_ecode);

        /*Free p_encoded_attributes before free the p_stream_capab*/
        icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,
                         &(p_stream_capab->p_encoded_attributes),
                           ICF_MEM_COMMON);     
        ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
                    p_stream_capab,
                    ICF_MEM_COMMON,
                    p_cc_pdb->p_ecode,
                    ret_val)
        ret_val = ICF_FAILURE; 
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_cc_update_pref_profile_node_from_local_sdp
 **
 ** DESCRIPTION:  This Function will be updating nodes pref_media_profile from local sdp
 **               only for Audio and Video streams and moreover only if codecs for that
 **               particular stream are NULL in the preffered profile. 
 *****************************************************************************/
    icf_return_t 
icf_cc_update_pref_profile_node_from_local_sdp(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                      ret_val = ICF_SUCCESS;
    icf_stream_sdp_info_st            *p_stream_sdp_info = ICF_NULL;
    icf_stream_capab_st               *p_stream_capab = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        /* Access p_cc_pdb->p_stream_sdp_info in local p_stream_sdp_info,
         * bcoz this will be further used for creating stream capab node.
         */ 
        p_stream_sdp_info = p_cc_pdb->p_stream_sdp_info;
    p_stream_capab = (icf_stream_capab_st *)(p_cc_pdb->p_generic_ptr_1);

    if ((ICF_STREAM_TYPE_AUDIO == p_stream_sdp_info->stream_type)
            || (ICF_STREAM_TYPE_VIDEO == p_stream_sdp_info->stream_type))
    {
        if (ICF_NULL==p_stream_capab->p_codec_info_list)
        {
            if (ICF_NULL != p_stream_sdp_info->p_local_sdp)
            {  
                ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                        &(p_stream_capab->p_codec_info_list),
                        p_stream_sdp_info->p_local_sdp->sdp_stream.p_codec_info_list);
                if (ICF_FAILURE != ret_val)
                {
                    p_stream_capab->bit_mask |= ICF_STREAM_CAPAB_CODEC_INFO_PRESENT; 
                }
            }               
        }
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}


/*****************************************************************************
 ** FUNCTION:     icf_cc_make_duplicate_of_offered_sdp_per_stream
 **
 ** DESCRIPTION:  This Function will be creating deep copy of p_local_sdp
 **               in p_offer_sdp on per stream basis.
 *****************************************************************************/
    icf_return_t 
icf_cc_make_duplicate_of_offered_sdp_per_stream(icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t                       ret_val = ICF_SUCCESS;
    icf_stream_sdp_info_st             *p_stream_sdp_info_data = ICF_NULL;
    icf_internal_list_st	           *p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

        p_stream_sdp_info = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_stream_sdp_info)
    {
        p_stream_sdp_info_data = (icf_stream_sdp_info_st *)
            (p_stream_sdp_info->p_payload);
        if (ICF_NULL != p_stream_sdp_info_data->p_local_sdp)
        {
            ret_val = icf_cc_copy_internal_sdp_stream(p_cc_pdb,
                    p_stream_sdp_info_data->p_local_sdp,
                    &(p_stream_sdp_info_data->p_offered_sdp));
        }
        if (ICF_FAILURE == ret_val)
            break;

        p_stream_sdp_info = p_stream_sdp_info->p_next;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */    
        return ret_val;
}

/*****************************************************************************
 ** FUNCTION:     icf_cc_validate_n_add_rejected_streams_in_call_ctxt
 **
 ** DESCRIPTION:  This Function will validate the rejected streams stored in 
 **               the call context and if the stream is already stored as 
 **               rejected then that stream will not be saved. Else the stream
 **               will be stored in the call context as rejected.
 *****************************************************************************/
icf_return_t    icf_cc_validate_n_add_rejected_streams_in_call_ctxt(
        INOUT icf_cc_pdb_st *p_cc_pdb,
        IN    icf_stream_list_st rejected_stream_list[ICF_MAX_STREAM_PER_CALL],
        IN    icf_uint8_t   rej_count)
{

    icf_return_t        ret_val = ICF_SUCCESS;
    icf_uint8_t count =0 , count1=0;
    icf_boolean_t   match_found = ICF_FALSE;

    /* Some deleted streams are present */
    for ( count =0;(count< rej_count) && (count < ICF_MAX_STREAM_PER_CALL) ;count ++)
    {
        match_found = ICF_FALSE;
        for(count1 =0;(count1 < p_cc_pdb->p_call_ctx->rejected_stream_count) && (count1 < ICF_MAX_STREAM_PER_CALL);count1++)
        {

            if(p_cc_pdb->p_call_ctx->rejected_stream_list[count1].stream_id == rejected_stream_list[count].stream_id )
            {
                /* the stream was already rejected so no need to store it again in the
                 * rejected stream list.
                 */ 
                match_found = ICF_TRUE;
                break;

            }
        }
        if((ICF_FALSE == match_found) && (count < ICF_MAX_STREAM_PER_CALL ) && (p_cc_pdb->p_call_ctx->rejected_stream_count < ICF_MAX_STREAM_PER_CALL ))
        {
            p_cc_pdb->p_call_ctx->rejected_stream_list
            [p_cc_pdb->p_call_ctx->rejected_stream_count++]=
             rejected_stream_list[count];
        }
       
    }


    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_revert_mms_fax_cancel
 *
 * DESCRIPTION:     This function validates and processes the remote fax
 *					error resposne.
 *
 ******************************************************************************/
icf_return_t  icf_cc_make_revert_mms_fax_cancel(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;

    icf_internal_list_st	*p_list =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    icf_internal_list_st    *p_sdp_list = ICF_NULL;
    icf_stream_sdp_info_st *p_audio_stream_info = ICF_NULL;
    icf_stream_sdp_info_st *p_t38_stream_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        /* get the audio and T38 stream info*/

        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_AUDIO);

    if(ICF_NULL == p_sdp_list)
    {	
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ICF_FAILURE;
    }

    p_audio_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
    p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;

    if(ICF_FAX_PT_CALL != p_cc_pdb->p_call_ctx->fax_type)
    {
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_FAX);

        if(ICF_NULL == p_sdp_list)
        {  
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ICF_FAILURE;
        }

        p_t38_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
    }

    switch(p_cc_pdb->p_call_ctx->fax_type)
    {
        case ICF_FAX_PT_CALL:
            if(ICF_LOCAL_SDP_CODEC_CHANGE == 
                    (ICF_LOCAL_SDP_CODEC_CHANGE & 
                     p_audio_stream_info->media_modify_type))
            {
                p_audio_stream_info->media_modify_type =
                    ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE;
            }
            else if(ICF_LOCAL_SDP_MEDIA_CHANGE == 
                    (ICF_LOCAL_SDP_MEDIA_CHANGE & 
                     p_audio_stream_info->media_modify_type))
            {
                p_audio_stream_info->media_modify_type =
                    ICF_REMOTE_ANSWER_REVERT_SDP_CHANGE;
            }
            break;

        case ICF_T38_FAX_CALL:
        case ICF_T38_FAX_CALL_FAILURE_SWITCH_PT_CALL:
            p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;

            p_audio_stream_info->stream_status &= (~(ICF_MEDIA_STREAM_CLINE_ZERO));
            /*if fax call has not ben established then call icf_cc_calc_mode_remote_reject_changed .
              Else call else leg where stop fax req.has been done by app. after remote has initiated a start fax request */               
            if (ICF_NULL == ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                                        ICF_FAX_CALL_ESTABLISHED))
            {
               icf_cc_calc_mode_remote_reject_change(p_cc_pdb);
            }
            else
            {
               /*  CSR 1-6096114 Merge */ 
               /*When processing STOP_FAX , IPTK will apply mode as
                 RECV_ONLY instead of SEND_RECV. The mode should be RECV_ONLY
                 as we are not yet aware of the peer capability and port. */
               p_audio_stream_info->mm_applied_mode = ICF_MEDIA_MODE_RECV_ONLY; 
               p_audio_stream_info->media_modify_type = ICF_MODE_CHANGE;
               p_audio_stream_info->p_local_sdp->media_mode = ICF_MEDIA_MODE_SEND_RECV;
            }

            /* In case fax_method was REPLACE then we will mark Audio stream as NW_ALLOW
             * and T38 stream as NW_DISALLOW.
             */ 
            if (ICF_FAX_METHOD_REPLACE_STREAM == p_cc_pdb->p_call_ctx->fax_method)
            {
               p_audio_stream_info->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;
               p_t38_stream_info->stream_status &= ~(ICF_MEDIA_STREAM_NW_ALLOW);
            }
            p_t38_stream_info->media_modify_type = ICF_STREAM_DELETE;


            break;

    }

    if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        if(ICF_TRUE == p_cc_pdb->mms_sent)
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                        ICF_CC_PA_STOP_FAX_REQ)
                ICF_CC_RESET_PENDING_EVENT(p_cc_pdb,ICF_PENDING_EVENT_PA_STOP_FAX_REQ)
                /*p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;*/
                /* Start the media timer  */
                ret_val = icf_cc_start_media_timer(p_cc_pdb);
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_revert
 *
 * DESCRIPTION:     This function validates and processes the reverting of 
 *                  remote fax request.     
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_revert(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;
    icf_internal_list_st   *p_list =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    icf_internal_list_st    *p_sdp_list = ICF_NULL;
    icf_stream_sdp_info_st *p_temp_stream_info = ICF_NULL;
    icf_stream_sdp_info_st *p_audio_stream_info = ICF_NULL;
    icf_stream_sdp_info_st *p_t38_stream_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        /* get the audio and T38 stream info*/
        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_AUDIO);
    if(ICF_NULL == p_sdp_list)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
            return ICF_FAILURE;
    }
    p_temp_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
    if(ICF_STREAM_TYPE_FAX == p_temp_stream_info->stream_type)
    {
        /* this means audio line was deleted 
           we should release call here*/
    }
    else
    {
        /*
           1. get the audio sdp and t38 sdp.
           2. t38 line is to be deleted
           3. audio line mode is to be made as before
           4. audio stream status needs to be modified to NW_ALLOW
         */
        p_audio_stream_info = p_temp_stream_info;
        p_cc_pdb->p_stream_sdp_info = p_audio_stream_info;
        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, 
                ICF_STREAM_TYPE_FAX);
        if(ICF_NULL == p_sdp_list)                                              
        {
            ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
                return ICF_FAILURE;
        }
        p_t38_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
        p_audio_stream_info->stream_status &= (~(ICF_MEDIA_STREAM_CLINE_ZERO));
        p_audio_stream_info->stream_status &= ~ICF_MEDIA_STREAM_PRIVATE;
        p_audio_stream_info->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;
        p_audio_stream_info->media_modify_type |= ICF_MODE_CHANGE;
        p_audio_stream_info->mm_applied_mode = ICF_MEDIA_MODE_SEND_RECV;
        p_t38_stream_info->media_modify_type = ICF_STREAM_DELETE;
    }

    if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        if(ICF_TRUE == p_cc_pdb->mms_sent)
        {
            /*
             * Update the states in the call context
             */
            ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
                ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                        ICF_CC_SSA_STOP_FAX_REQ)
                /*p_cc_pdb->p_call_ctx->fax_type = ICF_NULL;*/
                /* Start the media timer  */
                ret_val = icf_cc_start_media_timer(p_cc_pdb);
        }
    }
    /* send the mms requets here
       make next state as 06
       ongoing event as SSA_CALL_CANCEL_REQ
       set fax type as NULL,
       reset the call cancel pending event*/
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}     

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_audio_stream_status
 *
 * DESCRIPTION:     This function checks if audio line is present in local sdp
 *
 ******************************************************************************/
icf_return_t icf_cc_check_audio_stream_status(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_SUCCESS;

    icf_internal_list_st    *p_list =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    icf_internal_list_st    *p_sdp_list = ICF_NULL;
    icf_stream_sdp_info_st *p_audio_stream_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        /* get the audio info*/
        p_sdp_list = icf_cc_get_node_from_stream_list(p_list, ICF_STREAM_TYPE_AUDIO);

    if(ICF_NULL == p_sdp_list)
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        p_audio_stream_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;
        if ( ICF_NULL == 
                (ICF_MEDIA_STREAM_ACTIVE & p_audio_stream_info->stream_status))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            ret_val = ICF_SUCCESS;
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
        return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_is_fax_stop_req
 *
 * DESCRIPTION:     This function checks whether incomming message is of stop
 *                   fax request or not     
 ******************************************************************************/
icf_return_t  icf_cc_check_is_fax_stop_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t  ret_val = ICF_FAILURE;
    icf_nw_inc_media_change_req_st  *p_update_req = ICF_NULL;
    icf_inc_media_sdp_info_st   *p_media_sdp_info = ICF_NULL;
    icf_list_st *p_temp = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)
        p_update_req = (icf_nw_inc_media_change_req_st *)
        p_cc_pdb->p_internal_msg->p_msg_data;

    if(ICF_NW_INC_MEDIA_ADD_STREAM ==
            (ICF_NW_INC_MEDIA_ADD_STREAM & p_update_req->bitmask))
    {
        return ICF_FAILURE;
    }
    else if(ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM  ==
            (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM & p_update_req->bitmask))
    {
        return ICF_FAILURE;
    }
    else if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT  ==
            (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_update_req->bitmask))
    {
        p_temp = p_update_req->p_sdp_list;
        p_media_sdp_info = (icf_inc_media_sdp_info_st *)p_temp->p_data;
        if (ICF_NULL == p_media_sdp_info )
        {
            ret_val = ICF_FAILURE;
        }
        else if (p_media_sdp_info->stream_type == ICF_STREAM_TYPE_AUDIO )
        {
            p_temp = p_temp->p_next;
            if (p_temp != ICF_NULL)
            {
                p_media_sdp_info = (icf_inc_media_sdp_info_st *)p_temp->p_data;
                if ((p_media_sdp_info != ICF_NULL) && 
                        (p_media_sdp_info->stream_type == ICF_STREAM_TYPE_FAX))
                {
                    if (ICF_FALSE == p_media_sdp_info->stream_active )
                        ret_val = ICF_SUCCESS;
                    else
                        ret_val = ICF_FAILURE;
                }
                else 
                    ret_val = ICF_SUCCESS;
            }
            else
                ret_val = ICF_SUCCESS;
        }
        else
            ret_val = ICF_FAILURE;
    }
    else
        ret_val = ICF_FAILURE ;

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb); /* function exit trace */

    if (ret_val == ICF_SUCCESS)
    {
        return icf_cc_check_audio_stream_status(p_cc_pdb);
    }
    else
        return ICF_FAILURE;

}
                                                                           
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_revert_mms_for_call_mod_cancel_req
 *
 * DESCRIPTION:     This function reverts the mms to last committed SDP
 *                  for call modify cancel request 
 *
 ******************************************************************************/
icf_return_t  icf_cc_revert_mms_for_call_mod_cancel_req(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;
    icf_uint8_t      ongoing_event = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    if(ICF_FAILURE == icf_cc_derive_mms_remote_rejected(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else if(ICF_CC_REMOTE_REJ_STREAM_DELETE ==
      ( ICF_CC_REMOTE_REJ_STREAM_DELETE & p_cc_pdb->common_bitmask))
    {
        icf_cc_send_pa_call_modify_resp(p_cc_pdb);
        ICF_CC_SET_RELEASE_TYPE(p_cc_pdb,ICF_REL_TYPE_INTERNAL_INITIATED)
        ret_val = icf_cc_clear_call_at_call_modify(p_cc_pdb);
    }
    else if(ICF_FAILURE == icf_cc_send_mms_req(p_cc_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        if(ICF_TRUE == p_cc_pdb->mms_sent)
        {
            if(ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK_2(p_cc_pdb,ICF_CALL_MOD_CANCEL_REQ)) 
            {
                ongoing_event =  ICF_CC_GET_ONGOING_EVENT(p_cc_pdb);

                if (ICF_CC_SSA_CALL_MODIFY_CANCEL_RESP == ongoing_event)
                {
                    /*Case when Re-Invite is not sent and call mod cancel req comes*/
                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                }
                else
                {
                    /*Normal case when call modify cancel req comes and Re-Invite is sent 
                      to the n/w*/
                    /*Update the states in the call context*/

                    ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                    ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_15_AWT_CALL_MODIFY_ALERT_RESP)
                    ICF_CC_SET_ONGOING_EVENT(p_cc_pdb,
                      ICF_CC_PA_CALL_MODIFY_CANCEL_REQ)
                    /*Check*/
                    /*ICF_CC_SET_PENDING_EVENT(p_cc_pdb, 
                      ICF_PENDING_EVENT_PA_CALL_MOD_CANCEL_REQ)*/
                 }
            }
            else
            {
                /*If this bitmask is already reset it means 200 OK of Re-Invite is received
                  before 200 OK of CANCEL*/

                ICF_CC_SET_CALL_STATE(p_cc_pdb,ICF_CC_04_AWT_MMS_RESP)
                ICF_CC_SET_NEXT_STATE(p_cc_pdb,ICF_CC_06_CONNECTED)
            }
            /*Start the media timer*/
            ret_val = icf_cc_start_media_timer(p_cc_pdb);
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
}



/******************************************************************
 * FUNCTION:        icf_cc_process_nw_rejected_stream_list
 *
 * DESCRIPTION:     this function processes the new streams that are
 *                  recvd with port 0 from remote. We will add the
 *                  stream node temporarily to be removed after
 *                  sending the answer
 *****************************************************************/
 icf_return_t   icf_cc_process_nw_rejected_stream_list
 (
    INOUT   icf_cc_pdb_st     *p_cc_pdb
 )
 {
     icf_return_t              ret_val = ICF_SUCCESS;
     icf_list_st               *p_recvd_list = ICF_NULL;
     icf_internal_list_st      *p_stream_sdp_list = ICF_NULL;
     icf_stream_sdp_info_st    *p_stream_sdp_node = ICF_NULL;
     icf_inc_media_sdp_info_st *p_recvd_node = ICF_NULL;
     icf_uint32_t              loc_stream = ICF_NULL;
     icf_boolean_t             node_present = ICF_FALSE;

     p_recvd_list = p_cc_pdb->p_list_1;

     while(p_recvd_list)
     {
        p_recvd_node = p_recvd_list->p_data;
        loc_stream = p_recvd_node->stream_id;
        p_stream_sdp_list = icf_cmn_get_node_in_internal_list(
                                 p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                 &loc_stream);
        if((ICF_NULL != p_stream_sdp_list) && 
                (ICF_NULL != p_stream_sdp_list->p_payload))
        {
            p_stream_sdp_node = p_stream_sdp_list->p_payload;
            node_present = ICF_TRUE;
        }
        else
        {
            ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                    ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
                    ICF_MEM_COMMON,
                    p_stream_sdp_node,
                    ICF_RET_ON_FAILURE,
                    p_cc_pdb->p_ecode,
                    ret_val);
            /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,p_stream_sdp_node)
        }
        p_stream_sdp_node->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;
        p_stream_sdp_node->stream_status |= ICF_MEDIA_STREAM_PRIVATE;
        p_stream_sdp_node->stream_id = p_recvd_node->stream_id;
        p_stream_sdp_node->stream_type = p_recvd_node->stream_type;

        /* SPR 19999 - Fix for CSR 1-7643774 */
        /* The rejected stream node populated by SSA was not being freed.
         * Hence it is being stored here in call ctxt so that it can be
         * freed when the entire SDP is freed.
         */
        if (ICF_NULL != p_stream_sdp_node->p_remote_received_sdp)
        {
            icf_cmn_delete_internal_sdp_stream(
                    p_cc_pdb->p_glb_pdb,
                    &p_stream_sdp_node->p_remote_received_sdp,
                    p_cc_pdb->p_ecode);
        }
        p_stream_sdp_node->p_remote_received_sdp =
            p_recvd_node->p_int_sdp_stream;
        /* CSR 1-7643774 */
        /*  Copy the remote received codec list
         */
        /*  This is the network rejected stream list so copying the codec
         *  numbers into remote_recvd_codec_list to send it in the response
         */
        if (ICF_NULL != p_stream_sdp_node->p_remote_received_sdp)
        {
            if(ICF_FAILURE ==
                    icf_cmn_clone_codec_attrib_list(
                        p_cc_pdb->p_glb_pdb,
                        &(p_stream_sdp_node->p_remote_recvd_codec_list),
                        p_stream_sdp_node->p_remote_received_sdp->
                        sdp_stream.p_codec_info_list))

            {
                /*  Do Nothing */
                ICF_PRINT(((icf_uint8_t *)"\n[CC]: Not able to clone remote received codec list"));

            }
        }

        if(ICF_FALSE == node_present)
         {
            icf_cmn_add_node_to_internal_list(p_cc_pdb->p_glb_pdb,
                      &p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                      p_stream_sdp_node,
                      &loc_stream,
                      ICF_NULL);
         }
         p_recvd_list = p_recvd_list->p_next;
         p_stream_sdp_node = ICF_NULL;
    }
    icf_cmn_delete_list(p_cc_pdb->p_glb_pdb,&p_cc_pdb->p_list_1,
                        ICF_MEM_COMMON);
 
    return ret_val;
}

#ifdef ICF_TRACE_ENABLE


/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fill_sdp_info_data
 *
 * DESCRIPTION:     This Function will be invoked to create SDP info for Traces
 *					in p_cc_pdb->p_generic_ptr_1.
 ******************************************************************************/
icf_return_t icf_cc_fill_sdp_info_data
(
       icf_cc_pdb_st	*p_cc_pdb,
       icf_uint8_t      sdp_type
 )
{
    icf_return_t		ret_val = ICF_SUCCESS;

    icf_sdp_info_st		*p_sdp_info =  ICF_NULL;

    icf_stream_sdp_info_st     *p_stream_sdp_info = 
              p_cc_pdb->p_stream_sdp_info;

    icf_sdp_stream_st          *p_sdp_stream = ICF_NULL;
    
    /* CSR 1-6212448:SPR 18873: The field defines the mode of the media stream */
    icf_media_mode_t	media_mode;
    
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)
    
	/* CSR 1-6212448: SPR 18873: Also check if not local sdp offer along with remote_received_sdp
	 * NULL Check
	 */
    if( ((p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_ACTIVE) != ICF_MEDIA_STREAM_ACTIVE) ||
         ((ICF_CC_LOCAL_SDP_OFFER != sdp_type) && (ICF_NULL == p_stream_sdp_info->p_remote_received_sdp)))
    {
         /* this is an inactive stream , we will not inform about it */
         return ICF_SUCCESS;
    }
    else
    {
    switch(sdp_type)
    {
    case ICF_CC_LOCAL_SDP_OFFER:
    case ICF_CC_LOCAL_SDP_ANSWER:
         p_sdp_stream =  
             &(p_stream_sdp_info->p_local_sdp->sdp_stream); 
         /* CSR 1-6212448: SPR 18873: Extract the media mode from local sdp*/
         media_mode = p_stream_sdp_info->p_local_sdp->media_mode;
         break;
    case ICF_CC_REMOTE_SDP_OFFER:
    case ICF_CC_REMOTE_SDP_ANSWER:
         p_sdp_stream =  
             &(p_stream_sdp_info->p_remote_received_sdp->sdp_stream); 
         /* CSR 1-6212448: SPR 18873: Extract the media mode from remote sdp*/
         media_mode = p_stream_sdp_info->p_remote_received_sdp->media_mode;
         break;
    default:
        return ICF_FAILURE;

    }

/*     ICF_MEMGET(p_cc_pdb->p_glb_pdb,
          ICF_PORT_SIZEOF(icf_sdp_info_st),
          ICF_MEM_COMMON,
          p_sdp_info,
          ICF_RET_ON_FAILURE,
          p_cc_pdb->p_ecode,ret_val) 
*/
   { 
       p_sdp_info = icf_cmn_memget(p_cc_pdb->p_glb_pdb, (icf_uint32_t)sizeof(icf_sdp_info_st), (icf_pool_id_t)0, p_cc_pdb->p_ecode); 
       if (0 == p_sdp_info) 
       { 
           ret_val = 0; 
           if (0 == ret_val) 
           { 
              { 
                  icf_trace_data_st trace_data_func_ext;
                  /* initalize trace_data_func_ext variable
                     for klocwork warning removal(Klocworks fix Rel 8.2) */
                  icf_cmn_init_trace_data_st(&trace_data_func_ext);
                  /* Klocworks fix Rel 8.2 end */
                  trace_data_func_ext.num_of_integer_values = 1; 
              
#if defined(ICF_PORT_VXWORKS) || defined(ICF_PORT_LINUX) 

                  trace_data_func_ext.number_of_string_values = 1; 
                  trace_data_func_ext.int_val[0] = ret_val; 
                  icf_port_strcpy(trace_data_func_ext.string_val[0].str, 
                  (icf_void_t *)__func__);
                  trace_data_func_ext.string_val[0].str_len = 
                      icf_port_strlen(trace_data_func_ext.string_val[0].str); 
#endif 


#if defined(ICF_PORT_WINDOWS)
				  trace_data_func_ext.number_of_string_values = 1;
                  
				  icf_port_strcpy(trace_data_func_ext.string_val[0].str,
                                         (icf_void_t *)__FILE__); 
				  trace_data_func_ext.string_val[0].str_len = (icf_uint16_t)
                      icf_port_strlen(trace_data_func_ext.string_val[0].str); 
				  trace_data_func_ext.int_val[0]  = ret_val ; 

#endif 

                  { 
                     icf_cmn_trace(p_cc_pdb->p_glb_pdb, (icf_trace_id_t)4, &trace_data_func_ext); 
                  } 
               } 
               return (ret_val); 
           } 
       } 
       else 
       { 
            ret_val = 1; 
       } 
    }

    p_sdp_info->stream_id = p_stream_sdp_info->stream_id;
    p_sdp_info->stream_type = p_stream_sdp_info->stream_type; 
    
    
    if (ICF_NULL != p_sdp_stream->p_codec_info_list)
    {
        /* Codec attrib list is present in local sdp.*/
        ret_val = icf_cmn_clone_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                &(p_sdp_info->p_codec_info_list),
                p_sdp_stream->p_codec_info_list);

        p_sdp_info->bit_mask |= ICF_CALL_MOD_CODEC_INFO_PRESENT;
    }
    
    if (ICF_FAILURE != ret_val)
    {
        if(ICF_NULL != p_sdp_stream->p_encoded_attributes)
        {
            ret_val = icf_cmn_copy_string_list(
                    p_cc_pdb->p_glb_pdb,
                    &(p_sdp_info->p_encoded_attributes),
                    p_sdp_stream->p_encoded_attributes,
                    p_cc_pdb->p_ecode);

            p_sdp_info->bit_mask |= ICF_CALL_MOD_ENC_ATTRIB_PRESENT;
        }
    }

    if (ICF_FAILURE != ret_val)
    {
        p_sdp_info->media_stream_param = p_sdp_stream->media_stream_param;
        p_sdp_info->bit_mask |= ICF_CALL_MOD_MEDIA_STREAM_PARAM_PRESENT;
    }

    if (ICF_SUCCESS == ret_val)
    {
        icf_transport_address_st dummy_stream_address = {{0,{{0,"\0"}}},0};
        dummy_stream_address.port_num = p_sdp_stream->stream_address.port_num;

        p_cc_pdb->p_generic_ptr_1 = &(p_sdp_stream->stream_address);
        p_cc_pdb->p_generic_ptr_2 = &dummy_stream_address;

        if(ICF_TRUE == icf_cc_compare_ip_and_port(p_cc_pdb))
        {
            p_sdp_info->media_mode = ICF_MEDIA_MODE_INACTIVE;
        }
        else if(ICF_CC_REM_IP_NULL == 
                (ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask))
        {
            p_sdp_info->media_mode = ICF_MEDIA_MODE_INACTIVE;
        }
        else
        {
            /* CSR 1-6212448: SPR 18873:Copying media mode from extracted mode.*/
            p_sdp_info->media_mode = media_mode;             
        }
		p_cc_pdb->p_generic_ptr_2 = ICF_NULL; /* Klocworks: LOCRET */
        p_cc_pdb->common_bitmask &= ~(ICF_CC_REM_IP_NULL);
        p_sdp_info->bit_mask |= ICF_CALL_MOD_MEDIA_MODE_PRESENT;
    }
    }
    /* if successfull in every task then set assign p_call_modify_data.
     * to p_generic_ptr_1.
     */
    p_cc_pdb->p_generic_ptr_1 = p_sdp_info;      
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_sdp_info_list
 *
 * DESCRIPTION:     This Function will be invoked to create 
 *                  offer/answer SDP information
 ******************************************************************************/
icf_return_t
icf_cc_create_sdp_info_list(
        icf_cc_pdb_st	*p_cc_pdb,
        icf_uint8_t      sdp_type)
{
    icf_return_t				ret_val = ICF_SUCCESS;

    icf_internal_list_st		*p_stream_sdp_info_node =
        p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_stream_sdp_info_st		*p_stream_sdp_info = ICF_NULL;

    p_cc_pdb->count = 0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    /* In tunnel mode, we do not understand streams and their status. Thus
       no trace data should be populated.
    */
ICF_CHECK_TUNNEL_MODE_START(p_cc_pdb->p_glb_pdb,ICF_FALSE)        

    p_cc_pdb->p_list_1 = ICF_NULL;

    /* Following while loop traverses the each stream node by node and count
     * for rejected streams and make list of stream capabs from the applied
     * sdps of Active streams.
     */
    while (ICF_NULL != p_stream_sdp_info_node)
    {
            p_stream_sdp_info = (icf_stream_sdp_info_st *)
                (p_stream_sdp_info_node->p_payload);

            /* Before calling this function we need to set p_cc_pdb->p_stream
             * _sdp_info with this particular streams p_stream_sdp_info data.
             * And also after returning from this function extract newly created
             * stream capab pointer from p_cc_pdb->p_generic_ptr_1.
             * p_cc_pdb->count must be initialised to 0 before calling
             * this function.
             */
            p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;
            p_cc_pdb->p_generic_ptr_1 = ICF_NULL;

            icf_cc_fill_sdp_info_data(p_cc_pdb,sdp_type);

            if(ICF_NULL == p_cc_pdb->p_generic_ptr_1)
            {
                /* do nothing */	
            }
            else if(ICF_FAILURE == icf_cmn_add_node_to_list(
                        p_cc_pdb->p_glb_pdb,
                        &(p_cc_pdb->p_list_1),
                        p_cc_pdb->p_generic_ptr_1))
            {
                ret_val = ICF_FAILURE;
                break;
            }

            p_stream_sdp_info_node = p_stream_sdp_info_node->p_next;
    }
ICF_CHECK_TUNNEL_MODE_END
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

    return ret_val;
}

#ifndef ICF_UNIT_TEST
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_free_stream_data_list
 *
 * DESCRIPTION:     This Function will be invoked to delete the stream data list
 ******************************************************************************/
icf_return_t
icf_cc_free_stream_data_list(
        icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                                ret_val = ICF_SUCCESS;

    icf_list_st         *p_stream_data_list =  p_cc_pdb->p_list_1;
    icf_stream_data_st  *p_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

        while(ICF_NULL != p_stream_data_list)
        {
            p_data = (icf_stream_data_st *)p_stream_data_list->p_data;

            if(ICF_NULL != p_data->p_offered_codec)
            {
                ret_val = icf_cmn_delete_codec_attrib_list(p_cc_pdb->p_glb_pdb,
                        &(p_data->p_offered_codec),p_cc_pdb->p_ecode);
            }

            ICF_MEMFREE(p_cc_pdb->p_glb_pdb,
              p_data->p_negotiated_codec,
              ICF_MEM_COMMON,
              p_cc_pdb->p_ecode,
              ret_val)

            p_stream_data_list = p_stream_data_list->p_next;

        }

    icf_cmn_delete_list(p_cc_pdb->p_glb_pdb, &(p_cc_pdb->p_list_1), ICF_MEM_COMMON);

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

        return ret_val;
}
#endif

#endif

/************ Changes for Re-Invite w/o SDP Start (Rel7.0) ************/

/*****************************************************************************
 * FUNCTION:        icf_cc_make_mms_type_for_remote_offer_change_wo_sdp
 * DESCRIPTION:     This function sets the media_modify_type bitmask which
 *                  shall indicate which fields of the SDP have changed
 *                  and shall aid the icf_cc_send_mms_req for populating the
 *                  payload for MMS request sent to MMI.
 *
******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_remote_offer_change_wo_sdp(
                               INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;
    icf_list_st                    *p_pref_profile_node = ICF_NULL;
    icf_stream_capab_st            *p_stream_capab_data = ICF_NULL;

    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_list))

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

    /* check for the streams if it is nw_allowed and active then
     * set the bitmask media_modify_type from the bitmask
     * ICF_LOCAL_SDP_CODEC_CHANGE which is used to do gcl request.
     */

    while(ICF_NULL != p_cc_list)
    {
         p_stream_sdp_info = p_cc_list->p_payload;

         /* Take the backup of p_local_sdp in p_last_commited_sdp */

         if(ICF_FAILURE == icf_cc_copy_internal_sdp_stream(p_cc_pdb,
                          p_stream_sdp_info->p_local_sdp,
                          &(p_stream_sdp_info->p_last_commited_sdp)))
         {
               ret_val = ICF_FAILURE;
               ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: icf_cc_copy_internal_sdp_stream returned failure"));
               break;
         }


        /* Compare the streams only if it is nw_allowed and active*/
         if ((ICF_MEDIA_STREAM_NW_ALLOW ==
            (ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp_info->stream_status)) &&
            (ICF_MEDIA_STREAM_ACTIVE ==
            (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp_info->stream_status)))

         {
             /* SPR 18589 : if the reinvite wo sdp is received on remote hold call
              * then mark the new reinvite as remote resume 
              */
            if(ICF_MEDIA_STREAM_REMOTE_HOLD & p_stream_sdp_info->stream_status)
            {
                  p_stream_sdp_info->stream_status |= 
                        ICF_MEDIA_STREAM_REMOTE_RESUME;
            }
            /* copy codec list info of p_pref_media_profile in p_local_sdp */

            p_pref_profile_node = icf_cmn_get_node_from_stream_capab_list(
                                  p_cc_pdb->p_call_ctx->p_pref_media_profile,
                                            p_stream_sdp_info ->stream_type);   

            if(ICF_NULL == p_pref_profile_node)
            {
                /* no codec information present. The GCL request should go
                          without codec list */
                p_stream_sdp_info->media_modify_type |=
                                     ICF_LOCAL_SDP_CODEC_CHANGE;

                if(ICF_FAILURE == icf_cmn_delete_codec_attrib_list(
                                 p_cc_pdb->p_glb_pdb,
                                 &( p_stream_sdp_info ->p_local_sdp->
                                 sdp_stream.p_codec_info_list),
                                 p_cc_pdb->p_ecode))
                {
                     ret_val = ICF_FAILURE;
                     ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:icf_cmn_delete_codec_attrib_list returned failure"));
                     break;
                }
            }
            else
            { 
                p_stream_capab_data = (icf_stream_capab_st *)
                                              p_pref_profile_node->p_data;
                if(ICF_NULL != p_stream_capab_data->p_codec_info_list)
                {
                    if(p_stream_sdp_info -> stream_type != ICF_STREAM_TYPE_TBCP)      
                    {

                         if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                                    p_cc_pdb->p_glb_pdb,
                                    &(p_stream_sdp_info->p_local_sdp->
                                    sdp_stream.p_codec_info_list),
                                    p_stream_capab_data->p_codec_info_list))
                         {
                                ret_val = ICF_FAILURE;
                                ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:icf_cmn_clone_codec_attrib_list returned failure"));
                                break;
                         }
                         else
                         {

                              p_stream_sdp_info->media_modify_type |=
                                               ICF_LOCAL_SDP_CODEC_CHANGE;
                             /*Klocwork Warning Removal*/
                              ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                           p_stream_sdp_info->p_local_sdp->
                                           sdp_stream.p_codec_info_list)
                         }

                      }
                  }
                  else
                  {
                      /* no codec information present. The GCL request should go
                          without codec list */
                      p_stream_sdp_info->media_modify_type |=
                                               ICF_LOCAL_SDP_CODEC_CHANGE;

                      if(ICF_FAILURE == icf_cmn_delete_codec_attrib_list(
                                     p_cc_pdb->p_glb_pdb,
                                     &( p_stream_sdp_info ->p_local_sdp->
                                     sdp_stream.p_codec_info_list),
                                     p_cc_pdb->p_ecode))
                      {
                            ret_val = ICF_FAILURE;
                            ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:icf_cmn_delete_codec_attrib_list returned failure"));
                            break;
                       }
                   }
               }
            }

          p_cc_list = p_cc_list->p_next;
     }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function exit trace */
        return ret_val;
}

/************* Changes for Re-Invite w/o SDP End (Rel7.0) *************/

/************* Changes for Forking & Multiple Answer Start (Rel 7.0) *************/
 
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_mms_req_for_forked_resp
 *
 * DESCRIPTION:     This function checks if mms is required.
 *                  It copies the forked_resp into the remote_sdp when 200 ok
 *                  recieved with out SDP and incase remote_sdp do not match
 *                  forked_resp
 *
 ******************************************************************************/

icf_boolean_t icf_cc_check_mms_req_for_forked_resp(
	INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
      icf_boolean_t                   ret_val = ICF_FALSE;
      icf_boolean_t                   forked_resp_exists = ICF_FALSE;
      icf_internal_list_st            *p_cc_list = ICF_NULL;
      icf_stream_sdp_info_st          *p_stream_sdp_info = ICF_NULL;

      p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
      ICF_ASSERT(p_cc_pdb->p_glb_pdb, (ICF_NULL != p_cc_list))

      while(ICF_NULL != p_cc_list)
      {
            p_stream_sdp_info = p_cc_list->p_payload;

            /* Now we check if an SDP already exists corresponding to the current forked_id
               If yes, returns TRUE else FALSE */

            p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info;

            forked_resp_exists = 
                             icf_cc_check_if_forked_resp_id_exists(p_cc_pdb);

            /* If an SDP already exists, we check if it is same as remote_sdp */
            if(forked_resp_exists)
            {
                  ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: forked respnse exists"));
                  /* If remote_sdp is same as forked_sdp_info implies mms not required */

                  if(p_stream_sdp_info->p_remote_received_sdp == ((icf_internal_sdp_stream_st*)
                            p_cc_pdb->p_generic_ptr_1))
                  {
                        ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: MMS not req as remote SDP and forked resp are same"));
                        ret_val = ICF_FALSE;
                  }
                  /* If remote_sdp is not same as forked_sdp_info implies mms required */
                 /* CSR 1-8608777 SPR 20407 */
                 /* Fix to apply proper MMS after receiving 200 OK of INVITE
                    in case when remote SDP and forked resp are different
                  */
                  else 
                  {
                        ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: MMS req as remote SDP and forked resp are different"));
                       p_stream_sdp_info->p_remote_received_sdp = 
                          ((icf_internal_sdp_stream_st *) p_cc_pdb->p_generic_ptr_1);
                        ret_val = ICF_TRUE;
                  } 
            }
            else
            {
                   ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: Forked Response does not exist"));
                   ret_val = ICF_FALSE;
            }

      p_cc_list = p_cc_list->p_next;
      }

      return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_empty_node_forked_resp
 *
 * DESCRIPTION:     This function is called to create an empty node in the
 *                  forked_resp_list when a media stream is inactive/deleted
 *
 ******************************************************************************/

icf_return_t icf_cc_create_empty_node_forked_resp(
	INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
      icf_forked_resp_data_st *p_payload = ICF_NULL;
      icf_list_st *p_forked_resp_list = ICF_NULL;
      icf_return_t ret_val = ICF_SUCCESS;
      icf_list_st *p_node_returned = ICF_NULL;
      icf_internal_sdp_stream_st *p_old_sdp = ICF_NULL;

      ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

       p_forked_resp_list = p_cc_pdb->p_stream_sdp_info->p_forked_resp_list;
       
       p_node_returned = icf_cmn_get_node_in_list(p_forked_resp_list,
                                        p_cc_pdb->forked_resp_id);
    
       /* Checks if the node returned based on the identifier is NULL and if so adds 
          a new node */

       if(ICF_NULL == p_node_returned)
       {
             ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: p_node_returned is equal to ICF_NULL"));
             ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                 ICF_PORT_SIZEOF(icf_forked_resp_data_st),
                 ICF_MEM_COMMON,
                 p_payload,
                 ICF_RET_ON_FAILURE,
                 p_cc_pdb->p_ecode,
                 ret_val)
             /*Klocwork Warning Removal*/
             ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,p_payload)

             if(ICF_SUCCESS == ret_val)
             {
                   p_payload->p_sdp_info = ICF_NULL;
                   p_payload->forked_resp_id = p_cc_pdb->forked_resp_id;

                   /* creating a node for the above forked_resp_list */
                   ret_val = icf_cmn_add_node_to_list(p_cc_pdb->p_glb_pdb,
                         &(p_cc_pdb->p_stream_sdp_info->p_forked_resp_list),
                         p_payload);
                   if(ICF_FAILURE == ret_val)
                   {
                         ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: icf_cmn_add_node_to_list returned Failure"));
                   }
             }
             else
             {
                    ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: ICF_MEMGET in icf_cc_create_empty_node_forked_resp returned Failure"));
             }
         }
         else
         {
               p_old_sdp = ((icf_forked_resp_data_st *)(p_node_returned->p_data))->p_sdp_info;
              
               /* update the payload of the node returned with NULL*/
 
               ((icf_forked_resp_data_st *)(p_node_returned->p_data))->p_sdp_info = 
                             ICF_NULL;
               ((icf_forked_resp_data_st *)(p_node_returned->p_data))->\
                             forked_resp_id = p_cc_pdb->forked_resp_id;

               /* delete the p_sdp_info */

               ret_val = icf_cmn_delete_internal_sdp_stream(
                    p_cc_pdb->p_glb_pdb,
                    &p_old_sdp,
                    p_cc_pdb->p_ecode);
        
               icf_dbm_ret_mem_to_pool(
                                 p_cc_pdb->p_glb_pdb,
                                 ICF_MEM_SDP,
                                 p_old_sdp,
                                 p_cc_pdb->p_ecode);
 
              if(ICF_FAILURE == ret_val)
              {
                     ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: ICF_MEMGET in icf_cc_copy_int_sdp_stream_to_remote returned Failure "));
              }
          }
    
         return ret_val; 
 
         ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
         return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_if_forked_resp_id_exists
 *
 * DESCRIPTION:     This function checks if an SDP already exists for a given
 *                  forked_resp_id. Incase an SDP is present this function 
 *                  returns TRUE else it returns FALSE
 *
 ******************************************************************************/

icf_boolean_t icf_cc_check_if_forked_resp_id_exists(
	INOUT   icf_cc_pdb_st           *p_cc_pdb)
{
     icf_list_st *p_forked_resp_list = ICF_NULL;
     icf_boolean_t ret_val = ICF_FALSE;
     /* Checks for the forked_resp_id through the forked_resp_list and if found 
        returns TRUE  */
     
     for(p_forked_resp_list = p_cc_pdb->p_stream_sdp_info->p_forked_resp_list;
                     p_forked_resp_list != ICF_NULL;p_forked_resp_list = 
                     p_forked_resp_list->p_next)
     {
            if(p_cc_pdb->forked_resp_id == ((icf_forked_resp_data_st *)
                (p_forked_resp_list->p_data))->forked_resp_id)
            {
                    /* If the sdp_info of the forked_resp_list is NULL then assign
                       media_modify_type as ICF_STREAM_DELETE */
                    ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:forked resp id exists"));
                    p_cc_pdb->p_generic_ptr_1 = (icf_internal_sdp_stream_st*)
                             (((icf_forked_resp_data_st *)
                             (p_forked_resp_list->p_data))->p_sdp_info);
          
                    if(ICF_NULL == ((icf_forked_resp_data_st *)
                            (p_forked_resp_list->p_data))->p_sdp_info)
                    {
                            ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]: media_modify_type marked as ICF_STREAM_DELETE"));
                            p_cc_pdb->p_stream_sdp_info->media_modify_type = 
                                     ICF_STREAM_DELETE; 
                    }
                    ret_val = ICF_TRUE;
                    break;
             }
      }
      if(ICF_FALSE == ret_val)
      {
          ICF_PRINT(((icf_uint8_t*)(icf_uint8_t*)"\n[CC]:forked resp id does not exist"));
          p_cc_pdb->common_bitmask |= ICF_CC_CALL_CLEAR;
      }

      return ret_val;
}

/************** Changes for Forking & Multiple Answer End (Rel 7.0) **************/

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_derive_mms_type_app_partial_reject
 *
 * DESCRIPTION:     This function derives the MMS type for reverting the remote
 *                  media change on partial reject from application
 ******************************************************************************/
icf_return_t
icf_cc_derive_mms_type_app_partial_reject(
        icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t		ret_val = ICF_SUCCESS;
    icf_internal_list_st *p_sdp_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    icf_pa_cc_call_modify_cfm_st *p_call_modify_cfm =
         (icf_pa_cc_call_modify_cfm_st *)
                        p_cc_pdb->p_glb_pdb->p_recv_msg->payload;

    icf_stream_sdp_info_st *p_sdp_info = ICF_NULL;
    icf_uint8_t   actual_stream_count = 0;
    icf_uint8_t   counter=0;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb)

    while(ICF_NULL != p_sdp_list)
    {
        p_sdp_info = (icf_stream_sdp_info_st *)p_sdp_list->p_payload;

        if(ICF_STREAM_DELETE & p_sdp_info->media_modify_type)
        {
            /*Case when the incoming offer is for media line delete
              and the request is rejected by application in rejected list*/
            ret_val = ICF_FAILURE;
            return ret_val ;
        }
        actual_stream_count ++;

        for(counter = 0; (counter < p_call_modify_cfm->count) && (counter < ICF_MAX_STREAM_PER_CALL ); counter ++)
        {
           /* match found so we should revert the media change for the stream */
            if(p_sdp_info->stream_id == 
                 p_call_modify_cfm->deleted_stream_list[counter].stream_id)
            {
               break;
            }
        }

        if(counter == p_call_modify_cfm->count)
        {
           /*
            if(ICF_LOCAL_SDP_CODEC_CHANGE ==
                   (ICF_LOCAL_SDP_CODEC_CHANGE & p_sdp_info->media_modify_type))
            {
                p_sdp_info->media_modify_type &= ~(ICF_LOCAL_SDP_CODEC_CHANGE);
            }
           */
             p_sdp_info->media_modify_type |= ICF_MEDIA_REVERT_NULL;
             /*p_sdp_info->media_modify_type = ICF_NULL;*/
             p_sdp_list = p_sdp_list->p_next;
             /* no match found in reject list, no need to revert the changes */
             continue;
        }

        p_sdp_info->stream_status |=
              ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT;

        if (ICF_NULL != p_sdp_info->p_remote_received_sdp) 
        {
            /*  Fix merge for CSR 1-6449001 */
            /*  Copy the remote received codec list for sending the codec nums
             *  in m line */
            if(ICF_FAILURE == icf_cmn_clone_codec_attrib_list(
                                p_cc_pdb->p_glb_pdb,
                                &(p_sdp_info->p_remote_recvd_codec_list),
                                p_sdp_info->p_remote_received_sdp->
                                sdp_stream.p_codec_info_list))

            {
                ret_val = ICF_FAILURE;
            }
        }

        if(ICF_REMOTE_SDP_MEDIA_CHANGE ==
              (ICF_REMOTE_SDP_MEDIA_CHANGE & p_sdp_info->media_modify_type))
        {
            p_sdp_info->media_modify_type |=
                    ICF_LOCAL_ANSWER_REVERT_SDP_CHANGE;
           
            p_sdp_info->media_modify_type &= ~ICF_REMOTE_SDP_MEDIA_CHANGE;
        }
        else if(ICF_REMOTE_SDP_CODEC_CHANGE ==
               (ICF_REMOTE_SDP_CODEC_CHANGE & p_sdp_info->media_modify_type))
        {
           p_sdp_info->media_modify_type |=
                 ICF_LOCAL_ANSWER_REVERT_CODEC_CHANGE;

           p_sdp_info->media_modify_type &= ~ICF_REMOTE_SDP_CODEC_CHANGE;
        }
        else if(ICF_MODE_CHANGE ==
                (ICF_MODE_CHANGE & p_sdp_info->media_modify_type))
        {
             p_cc_pdb->p_stream_sdp_info = p_sdp_info;
             icf_cc_calc_mode_app_reject_change(p_cc_pdb);
             /* copy the mode in mm_applied_mode after calculation*/
         }
        else if(ICF_LOCAL_SDP_CODEC_CHANGE ==
               (ICF_LOCAL_SDP_CODEC_CHANGE & p_sdp_info->media_modify_type))
        {
           /* when reinvite wo sdp is received */
           p_sdp_info->media_modify_type |=
                 ICF_REMOTE_ANSWER_REVERT_CODEC_CHANGE;

           p_sdp_info->media_modify_type &= ~(ICF_LOCAL_SDP_CODEC_CHANGE);
           p_sdp_info->stream_status &=
              ~(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
        }
        /* SPR 18589 : If the steam was on remote hold and the new request was 
         * remote resume, reset the resume bitmask as it has been rejected by application
         */
        if((ICF_MEDIA_STREAM_REMOTE_HOLD ==
                     (p_sdp_info->stream_status & ICF_MEDIA_STREAM_REMOTE_HOLD)) &&
                  (ICF_MEDIA_STREAM_REMOTE_RESUME ==
                     (p_sdp_info->stream_status & ICF_MEDIA_STREAM_REMOTE_RESUME)))
        {
            p_sdp_info->stream_status &= ~ICF_MEDIA_STREAM_REMOTE_RESUME;
        }

        p_sdp_list = p_sdp_list->p_next;
     }

    if(actual_stream_count == p_call_modify_cfm->count)
    {
       ICF_CC_SET_PDB_COMMON_BITMASK(p_cc_pdb,
               ICF_CC_ALL_STREAMS_DELETED)
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)

    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:       icf_cc_apply_pa_media_mode
 *
 * DESCRIPTION:     The funciton assigs the mm applied mode to all the streams in the
 *                  given list of streams
 ******************************************************************************/

icf_return_t
icf_cc_apply_pa_media_mode(
        icf_internal_list_st  *p_stream_sdp_info,
        icf_media_mode_t        media_mode)
{
    icf_internal_list_st        *p_temp_stream_sdp_info = ICF_NULL;
    icf_return_t                ret_val = ICF_SUCCESS;

    if (ICF_NULL == p_stream_sdp_info)
       return ICF_FAILURE;

    /* check if remote SDP is there else retrun the failure
     * as there is no need to do the mms till the time remote SDP is 
     * received */
    if(ICF_NULL == p_stream_sdp_info->p_payload ||
            ICF_NULL== ((icf_stream_sdp_info_st*)p_stream_sdp_info->p_payload)->p_remote_received_sdp)
    {
        ret_val =  ICF_FAILURE;
    }
    else
    {
        for(p_temp_stream_sdp_info = p_stream_sdp_info;
            p_temp_stream_sdp_info != ICF_NULL;  p_temp_stream_sdp_info = p_temp_stream_sdp_info->p_next )
        {
            /*If stream type is TBCP media mode change will not be done
             *This change is done while running the case for CSR_1_5420263*/
            if(((icf_stream_sdp_info_st*)p_temp_stream_sdp_info->p_payload 
                  != ICF_NULL) &&
               (ICF_STREAM_TYPE_TBCP !=
                  ((icf_stream_sdp_info_st*)p_temp_stream_sdp_info->p_payload)\
                  ->stream_type)) 
            {
                ((icf_stream_sdp_info_st*)p_temp_stream_sdp_info->p_payload)->mm_applied_mode = media_mode;
                ((icf_stream_sdp_info_st*)p_temp_stream_sdp_info->p_payload)->media_modify_type |= ICF_MODE_CHANGE;
            }
        }
    }
    
    return ret_val;
}

/******************************************************************
 * FUNCTION:        icf_cc_process_nw_added_invalid_stream_list
 *
 * DESCRIPTION:     this function processes the new fax streams that 
 *                  are added without holding/removing the audio
 *                  stream. We will send port 0 for this fax stream.
 *****************************************************************/
icf_return_t  icf_cc_process_nw_added_invalid_stream_list
(
    INOUT   icf_cc_pdb_st     *p_cc_pdb
)
{
    icf_return_t              ret_val = ICF_SUCCESS;
    icf_list_st               *p_recvd_list = ICF_NULL;
    icf_internal_list_st      *p_stream_sdp_list = ICF_NULL;
    icf_stream_sdp_info_st    *p_stream_sdp_node = ICF_NULL;
    icf_inc_media_sdp_info_st *p_recvd_node = ICF_NULL;
    icf_uint32_t              loc_stream = ICF_NULL;
    icf_boolean_t             node_present = ICF_FALSE;

    p_recvd_list = p_cc_pdb->p_list_1;

    while(p_recvd_list)
    {
        p_recvd_node = p_recvd_list->p_data;
        loc_stream = p_recvd_node->stream_id;

        p_stream_sdp_list = icf_cmn_get_node_in_internal_list(
                            p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                            &loc_stream);

        if((ICF_NULL != p_stream_sdp_list) && 
                (ICF_NULL != p_stream_sdp_list->p_payload))
        {
            p_stream_sdp_node = p_stream_sdp_list->p_payload;
            node_present = ICF_TRUE;
        }
        if((ICF_TRUE == node_present) && (ICF_NULL != p_stream_sdp_list))
        {
                p_stream_sdp_node = p_stream_sdp_list->p_payload;
        }
        else
        {
            ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                        ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
                        ICF_MEM_COMMON,
                        p_stream_sdp_node,
                        ICF_RET_ON_FAILURE,
                        p_cc_pdb->p_ecode,
                        ret_val);
            /*Klocwork Warning Removal*/
            ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,p_stream_sdp_node)
        }

        /*Klocwork Warning Removal*/
        if(ICF_NULL != p_stream_sdp_node)
        {
            p_stream_sdp_node->stream_status = ICF_MEDIA_STREAM_NW_ALLOW;
            p_stream_sdp_node->stream_status |= ICF_MEDIA_STREAM_PRIVATE;
            p_stream_sdp_node->stream_id = p_recvd_node->stream_id;
            p_stream_sdp_node->stream_type = p_recvd_node->stream_type;
        }

        if(ICF_FALSE == node_present)
        {
            icf_cmn_add_node_to_internal_list(p_cc_pdb->p_glb_pdb,
                                 &p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                 p_stream_sdp_node,
                                 &loc_stream,
                                 ICF_NULL);
        }
        p_recvd_list = p_recvd_list->p_next;
        p_stream_sdp_node = ICF_NULL;
    }
    return ret_val;
}
/* SPR 16526: End to End Negotiation for echo canc */
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_ecan_present_in_encoded_attribs
 *
 * DESCRIPTION:     This function checks if ecan is received from remote then do
 *                  mms for this
 *
 ******************************************************************************/

icf_boolean_t icf_cc_check_ecan_present_in_encoded_attribs(
        INOUT icf_cc_pdb_st *p_cc_pdb)
{
    icf_return_t           ret_val = ICF_SUCCESS;
    icf_boolean_t          is_same = ICF_TRUE;
    icf_sdp_stream_st      *p_sdp_stream_1 =
        &(p_cc_pdb->p_stream_sdp_info->p_remote_received_sdp->sdp_stream);
    icf_list_st             *p_list = ICF_NULL;
    icf_string_st        *pString = ICF_NULL;
    icf_string_st        	enc_attrib_string;
    icf_int8_t           *p_temp_str = ICF_NULL,*p_rem = ICF_NULL;
    
    /* To remove warning*/
    ret_val=ret_val;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace*/
    
    for(p_list = p_sdp_stream_1->p_encoded_attributes; ICF_NULL != p_list;\
           p_list=p_list->p_next)
    {
       	/* Fix for CSR 5233036 */
        icf_port_memset(
 	    (icf_void_t *)&enc_attrib_string,
	     0,
	    ICF_PORT_SIZEOF(icf_string_st));    
	icf_port_memcpy(
	    &enc_attrib_string,
	    (icf_string_st *)p_list->p_data,
	    ICF_PORT_SIZEOF(icf_string_st));
      
        pString = &enc_attrib_string;
     
        if(ICF_NULL != pString->str_len)
        {
            p_temp_str = icf_port_strtokr((icf_int8_t*)pString->str,":",&p_rem);      
            if((ICF_NULL != p_temp_str) && 
               (ICF_NULL == icf_port_strcmp((icf_uint8_t *)p_temp_str,(icf_uint8_t*)"ecan")))
            {
                is_same = ICF_FALSE;
                break;
            }
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
    return is_same;
}

/*****************************************************************************
 ** FUNCTION:     icf_cc_update_nw_allow_for_stream(
 **
 ** DESCRIPTION:  This function would search the stream of type stream_type and
 **               would set/unset the network allow flag for the same
 **               based on the value of input parameter set_value
*****************************************************************************/
icf_void_t
icf_cc_update_nw_allow_for_stream(
	        icf_cc_pdb_st	*p_cc_pdb,
                icf_uint8_t     stream_type,
                icf_boolean_t   set_value)
{    
    icf_internal_list_st    *p_list = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp = ICF_NULL;
    #ifdef ICF_TRACE_ENABLE
    icf_return_t           ret_val = ICF_SUCCESS;
    #endif
    
    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    while (ICF_NULL != p_list)
    {
        p_stream_sdp = p_list->p_payload;

        /* Search the stream of type stream_type */
        if (stream_type == p_stream_sdp->stream_type)
        {
            if(ICF_TRUE == set_value)
            {
                p_stream_sdp->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;
	    }
            else
	    {
	        p_stream_sdp->stream_status &= ~ICF_MEDIA_STREAM_NW_ALLOW;
	    }
        }
        p_list = p_list->p_next;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb)
}


/* CSR 1-5390593 Merge */
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_ssa_offer_change
 *
 * DESCRIPTION:     This function processes checks the ssa offer change received from
 *                   remote.This is needs to be checked only in case of T.38 Fax Call.
 *                   If T.38 Fax call is established and IPTK receives ReInvite From
 *                   network, then CC shall check for any change in SDP for ReInvite.
 *                   If there is any change, then CC shall send 488 else sends 200ok.
 *********************************************************************************/

icf_boolean_t   icf_cc_check_ssa_offer_change(
         INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
     icf_boolean_t      is_same = ICF_TRUE;
     icf_nw_inc_media_change_req_st  *p_update_req = ICF_NULL;
     icf_internal_list_st           *p_cc_list = ICF_NULL;
     icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL; 
     icf_sdp_stream_st               sdp_stream_1, sdp_stream_2;
     icf_return_t                   ret_val = ICF_SUCCESS;     

     p_update_req = (icf_nw_inc_media_change_req_st *)
                          p_cc_pdb->p_internal_msg->p_msg_data;

     if(ICF_NULL == p_update_req->bitmask)
     {
	  /* empty reinvite */
	  return ICF_FALSE;
     }

     p_cc_pdb->p_list_1 = p_update_req->p_sdp_list;
 
     if ((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                      ICF_FAX_CALL_ESTABLISHED)) && 
          (ICF_T38_FAX_CALL == p_cc_pdb->p_call_ctx->fax_type))
     {
         if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
         { 
	      /* if ith level check has failed */
             is_same = ICF_FALSE; 
         }
         else if (ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
         {
             is_same = ICF_FALSE; 
         }
         else
         {
             p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
             while(ICF_NULL != p_cc_list)
             {
                 p_stream_sdp_info = p_cc_list->p_payload;
                 if(ICF_STREAM_TYPE_FAX != p_stream_sdp_info->stream_type) 
                 {
                     if((ICF_NULL != p_stream_sdp_info->media_modify_type)
                     && (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_NW_ALLOW))
                     {
                         /*Check if REMOTE SDP MEDIA is the only bitmask set.
                          * If yes, then check for PORT or IP address change
                          * If no, then it means that there is another change in received SDP.
                          */ 
                         if(ICF_NULL == 
                            ((~ICF_REMOTE_SDP_MEDIA_CHANGE) & p_stream_sdp_info->media_modify_type)) 
                         {
                             if(ICF_CC_REM_PORT_CHNG & p_cc_pdb->common_bitmask)
                             {
                                is_same = ICF_FALSE;
                                break; 
                             }

                             /*If the remote offer may be different  
                             * 1) If IPTK sends ReINVITE/UPDATE with c-line zero,but recevied
                             * c-line without zero
                             * 2) If IPTK sends ReINVITE/UPDATE with IP, but received c-line with zero.
                             * 3) If IPTK sends ReINVITE/UPDATE with IP and received c-line with different IP. 
                             */ 
                             if(((ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp_info->stream_status) &&  
                                !(ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)) || 
                                 (!(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp_info->stream_status)
                                  && (ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)) ||
                                 (!(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp_info->stream_status) &&  
                                !(ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)))
                             {
                                  is_same = ICF_FALSE;
                                  break; 
                             }
                          }
                          else
                          {
                               is_same = ICF_FALSE;
                               break; 
                          }    
                     }
                  } 
                  else
                  {
                      /*Check for PORT or IP address change in case of FAX stream.*/
                     
                      /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18877)*/
                      /* SPR 18877 Fix : Added protective NULL checks. Proceed with 
                       the comparision only if all pre-requisite pointers have
                       been successfully populated. 
                      */
                      ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                   p_stream_sdp_info->p_remote_received_sdp,
                                   p_stream_sdp_info->p_applied_sdp,
                                   ret_val)
                      if(ICF_FAILURE == ret_val)
                      {
                          /* return from the function as not enough data to proceed */
                          return ICF_FALSE;
                      }                      
                      sdp_stream_1 = p_stream_sdp_info->p_remote_received_sdp->sdp_stream;
                      sdp_stream_2 = p_stream_sdp_info->p_applied_sdp->sdp_stream;

                      p_cc_pdb->p_generic_ptr_1 = &(sdp_stream_1.stream_address);
                      p_cc_pdb->p_generic_ptr_2 = &(sdp_stream_2.stream_address);
                      
                      if (ICF_FALSE == icf_cc_compare_ip_and_port(p_cc_pdb))
                      {
                          is_same = ICF_FALSE;
                          break; 
                      }
					p_cc_pdb->p_generic_ptr_1 = ICF_NULL; /* Klocworks: LOCRET */
					p_cc_pdb->p_generic_ptr_2 = ICF_NULL; /* Klocworks: LOCRET */
                  }
                p_cc_list = p_cc_list->p_next;
             }
          }
     }
     else if(ICF_NULL != p_cc_pdb->p_call_ctx->p_service_ctx)
     {
         ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
         if (ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
         { 
	      /* if ith level check has failed */
             is_same = ICF_FALSE; 
         }
         else if (ICF_FAILURE == icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb))
         {
             is_same = ICF_FALSE; 
         }
         else
         {
             p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
             while(ICF_NULL != p_cc_list)
             {
                 p_stream_sdp_info = p_cc_list->p_payload;
                     
                 if((ICF_NULL != p_stream_sdp_info->media_modify_type)
                     && (p_stream_sdp_info->stream_status & ICF_MEDIA_STREAM_NW_ALLOW))
                 {
                         /*Check if REMOTE SDP MEDIA is the only bitmask set.
                          * If yes, then check for PORT or IP address change
                          * If no, then it means that there is another change in received SDP.
                          */ 
                         if(ICF_NULL == 
                            ((~ICF_REMOTE_SDP_MEDIA_CHANGE) & p_stream_sdp_info->media_modify_type)) 
                         {
                             if(ICF_CC_REM_PORT_CHNG & p_cc_pdb->common_bitmask)
                             {
                                is_same = ICF_FALSE;
                                break; 
                             }

                             /*If the remote offer may be different  
                             * 1) If IPTK sends ReINVITE/UPDATE with c-line zero,but recevied
                             * c-line without zero
                             * 2) If IPTK sends ReINVITE/UPDATE with IP, but received c-line with zero.
                             * 3) If IPTK sends ReINVITE/UPDATE with IP and received c-line with different IP. 
                             */ 
                             if(((ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp_info->stream_status) &&  
                                !(ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)) || 
                                 (!(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp_info->stream_status)
                                  && (ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)) ||
                                 (!(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp_info->stream_status) &&  
                                !(ICF_CC_REM_IP_NULL & p_cc_pdb->common_bitmask)))
                             {
                                  is_same = ICF_FALSE;
                                  break; 
                             }
                          }
                          else
                          {
                               is_same = ICF_FALSE;
                               break; 
                          }    
                     }
                     p_cc_list = p_cc_list->p_next;
                }
          }
     }
     else
     {
         p_cc_pdb->p_internal_msg->msg_hdr.msg_id = ICF_NW_INC_MEDIA_UPDATE;
         icf_cc_free_ssa_msg(p_cc_pdb);
         is_same = ICF_FALSE;
     }
	p_cc_pdb->p_generic_ptr_1 = ICF_NULL; /* Klocworks: LOCRET */
	p_cc_pdb->p_generic_ptr_2 = ICF_NULL; /* Klocworks: LOCRET */
     return is_same;
}

#ifdef ICF_FAX_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_ssa_stop_fax_req
 *
 * DESCRIPTION:     This function processes the stop fax req received from remote.
 ******************************************************************************/
icf_return_t     icf_cc_process_ssa_stop_fax_req(
         INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_return_t    ret_val = ICF_SUCCESS;
    icf_internal_list_st    *p_stream_sdp_info_node = ICF_NULL;
    icf_stream_sdp_info_st  *p_stream_sdp_info_data = ICF_NULL;

        ICF_ASSERT(p_cc_pdb->p_glb_pdb, p_cc_pdb->p_internal_msg->p_msg_data)

	icf_nw_inc_media_change_req_st *p_call_modify_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

	if(ICF_NULL == p_call_modify_req->bitmask)
	{
		/* empty reinvite */
		return ICF_SUCCESS;
	}

    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)

    if(ICF_NW_INC_MEDIA_NW_REJECT_STREAM & p_call_modify_req->bitmask)
    {
        p_cc_pdb->p_list_1 = p_call_modify_req->p_nw_reject_stream_list;
        icf_cc_process_nw_rejected_stream_list(p_cc_pdb);
    }

    p_cc_pdb->p_list_1 = p_call_modify_req->p_sdp_list;

	if(ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM ==
       (ICF_NW_INC_MEDIA_CHG_REJECTED_STREAM & p_call_modify_req->bitmask))
    {
        p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_REJECT_MEDIA_CHANGE;

		if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
			(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_call_modify_req->bitmask))
		{
			icf_cc_delete_inc_media_sdp_info_list(
				p_cc_pdb,&(p_call_modify_req->p_sdp_list));
		}
    }
    else if(ICF_NW_INC_MEDIA_CHG_SDP_PRESENT ==
        (ICF_NW_INC_MEDIA_CHG_SDP_PRESENT & p_call_modify_req->bitmask))
    {
         if ((ICF_NULL != ICF_CC_CHECK_COMMON_BITMASK(p_cc_pdb,
                        		ICF_FAX_CALL_ESTABLISHED)) &&
             (((icf_inc_media_sdp_info_st *)(p_call_modify_req->p_sdp_list->p_data))->stream_type \
             == ICF_STREAM_TYPE_AUDIO) &&
             (ICF_NULL == p_call_modify_req->p_sdp_list->p_next) &&
             (ICF_NULL == (ICF_NW_INC_MEDIA_NW_REJECT_STREAM & p_call_modify_req->bitmask)))
         {
             p_stream_sdp_info_node = icf_cc_get_node_from_stream_list(
                                                p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                                ICF_STREAM_TYPE_FAX);

             if ((ICF_NULL != p_stream_sdp_info_node) &&
                  ((p_stream_sdp_info_data = p_stream_sdp_info_node->p_payload) != ICF_NULL))
             {
                 p_stream_sdp_info_data->media_modify_type = ICF_STREAM_DELETE;
                 p_stream_sdp_info_data->stream_status &= ~(ICF_MEDIA_STREAM_NW_ALLOW);
                 
                 p_stream_sdp_info_node = ICF_NULL;
                 p_stream_sdp_info_node = icf_cc_get_node_from_stream_list(
                                                p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                                ICF_STREAM_TYPE_AUDIO);

                 if ((ICF_NULL != p_stream_sdp_info_node) &&
                     ((p_stream_sdp_info_data = p_stream_sdp_info_node->p_payload) != ICF_NULL))
                 {
                    p_stream_sdp_info_data->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;
                    
                    p_cc_pdb->p_stream_sdp_info = p_stream_sdp_info_data;
                    p_cc_pdb->p_int_sdp_stream = ((icf_inc_media_sdp_info_st*)
                                                 (p_call_modify_req->p_sdp_list->p_data))->p_int_sdp_stream;
                                                                              
                    icf_cc_copy_int_sdp_stream_to_remote(p_cc_pdb);

                    ((icf_inc_media_sdp_info_st*)
                    (p_call_modify_req->p_sdp_list->p_data))->p_int_sdp_stream = ICF_NULL;
 
                    ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);

                    icf_cc_delete_inc_media_sdp_info_list(
				p_cc_pdb,&(p_call_modify_req->p_sdp_list));
                 }
                 else
                 {
                    ret_val = ICF_FAILURE;
                 }
             }
             else
             {
                 ret_val = ICF_FAILURE;
             }
         }
         else if(ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
         {
             p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_CALL_MODIFY_REASON_INVALID_REQUEST;
         }
         else
         {
	     ret_val = icf_cc_make_mms_type_for_remote_offer_change(p_cc_pdb);
         }
         
    }
	return ret_val;
}


/*  SPR 17993 Merge */

/*Adding New Function for VBD negotiation*/
/*******************************************************************************
 * FUNCTION:        icf_cc_negotiate_vbd_parameter
 * DESCRIPTION:     This function negotiates and checks if the OFFER sent
 *                  by IPTK had vbd=yes and answer has vbd=no then treat this
 *                  response as a failure response.
 ******************************************************************************/
icf_return_t  icf_cc_negotiate_vbd_parameter
(
    INOUT  icf_cc_pdb_st *p_cc_pdb
)
{
  

    icf_nw_inc_media_change_resp_st *p_fax_resp =
                (icf_nw_inc_media_change_resp_st *)
                    p_cc_pdb->p_internal_msg->p_msg_data;


    icf_inc_media_sdp_info_st *p_fax_sdp_info = ICF_NULL;
    icf_stream_sdp_info_st *p_stream_sdp_info = ICF_NULL;
    icf_internal_list_st    *p_list           = ICF_NULL;
    icf_uint32_t stream_id = 0;
    icf_list_st *p_temp_list = p_fax_resp->p_sdp_list;


    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */


    while(ICF_NULL != p_temp_list)
    {
        p_fax_sdp_info = (icf_inc_media_sdp_info_st *)p_temp_list->p_data;

        if(ICF_STREAM_TYPE_AUDIO == p_fax_sdp_info->stream_type)
        {
            stream_id = p_fax_sdp_info->stream_id;

            if(ICF_FALSE == p_fax_sdp_info->stream_active)
            {
                return ICF_SUCCESS;
            }

            p_list = icf_cmn_get_node_in_internal_list(
                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                        &stream_id);
            if(ICF_NULL == p_list)
            {
                return ICF_SUCCESS;
            }
            else
            {
                p_stream_sdp_info =
                         (icf_stream_sdp_info_st *)p_list->p_payload;
            }

            if(ICF_FAX_PT_CALL == p_cc_pdb->p_call_ctx->fax_type)
            {
                icf_codec_attrib_st     *p_codec_attrib=ICF_NULL;

                p_codec_attrib = p_stream_sdp_info->p_local_sdp->\
                                     sdp_stream.p_codec_info_list->p_data;

                if ((ICF_AUDIO_CODEC_PARAM_VBD_VAL &
                        p_codec_attrib->codec_params.audio_param.bit_mask)&&
                    (ICF_TRUE == p_codec_attrib->codec_params.audio_param.\
                        vbd_val))
                {
                    p_codec_attrib = p_fax_sdp_info->p_int_sdp_stream->\
                                      sdp_stream.p_codec_info_list->p_data;
                    if ((ICF_AUDIO_CODEC_PARAM_VBD_VAL &
                            p_codec_attrib->codec_params.audio_param.bit_mask)&&
                        (ICF_TRUE == p_codec_attrib->codec_params.audio_param.\
                            vbd_val))
                    {
                        /*If both VBD is true then its fine*/
                    }
                    else
                    {
                        icf_cc_free_ssa_msg(p_cc_pdb);
                        return ICF_FAILURE;
                    }
                }
            }
        }
        p_temp_list = p_temp_list->p_next;
    }
    return ICF_SUCCESS;
}

#endif

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_check_for_srtp_streams_in_local_sdp
 *
 * DESCRIPTION:     This function checks for SRTP streams in local SDP and update
 *                  srtp_fallback_reqd in call context
 ******************************************************************************/
icf_void_t     icf_cc_check_for_srtp_streams_in_local_sdp(
        INOUT   icf_cc_pdb_st     *p_cc_pdb)
{
    icf_stream_sdp_info_st      *p_sdp = ICF_NULL;
    icf_internal_list_st        *p_list = ICF_NULL;
    icf_boolean_t               srtp_present = ICF_FALSE;

    p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

    /*Iterate thru all streams and find if SRTP is the transport
     *mode for any active media stream. If no such stream found 
     *then mark srtp_fallback_reqd as ICF_FALSE. As no SRTP fallback
     *is required in this case*/
    while (ICF_NULL != p_list)
    {
        p_sdp =
            (icf_stream_sdp_info_st*)(p_list->p_payload);

        if ((ICF_NULL != p_sdp->p_local_sdp) &&
            (ICF_MEDIA_STREAM_ACTIVE & p_sdp->stream_status))
        {
            if (ICF_TRANSPORT_MODE_RTP_SAVP == 
                p_sdp->p_local_sdp->sdp_stream.trans_mode)
            {
                srtp_present = ICF_TRUE;
                break;
            }
        }
        p_list = (icf_internal_list_st *)p_list->p_next;
    }
    if (ICF_FALSE == srtp_present)
    {
        p_cc_pdb->p_call_ctx->srtp_fallback_reqd = ICF_FALSE;
    }
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_offer_in_tunnel_mode
 * DESCRIPTION:     This function sets the media_modify_type bitmask which
 *		            shall determine whether tunnelled SDP will be present in
 *                  tunnelled MMS request and response.Also it copies the local
 *                  SDP to the last committed SDP in the call context, whenever
 *                  a new offer is received from remote.
 *
 ******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_offer_in_tunnel_mode(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                        p_cc_pdb->p_call_ctx,
                                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        /* Even if the failure is due to p_stream_sdp_info being NULL, we will
           set the following bit. In tunnel mode, failure from this function
           due to any reason is considered equivalent. The calling function
           takes decisions on the basis of the bit ICF_CC_ALL_STREAMS_DELETED
           in CC PDB.
        */
        p_cc_pdb->common_bitmask |= ICF_CC_ALL_STREAMS_DELETED;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }

    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    p_stream_sdp_info = (icf_stream_sdp_info_st*) p_cc_list->p_payload;

    if(ICF_FAILURE == icf_cc_copy_committed_sdp(p_cc_pdb))
    {
        /* Even if the failure is due to failure of icf_cc_copy_committed_sdp, we will
           set the following bit. In tunnel mode, failure from this function
           due to any reason is considered equivalent. The calling function
           takes decisions on the basis of the bit ICF_CC_ALL_STREAMS_DELETED
           in CC PDB.
        */
        p_cc_pdb->common_bitmask |= ICF_CC_ALL_STREAMS_DELETED;
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* Set bits ICF_LOCAL_SDP_REQD and ICF_TUNNELLED_REMOTE_SDP_PRESENT 
           in the media_modify_type field of the call context.
         
           Meaning of these two bits:

           ICF_LOCAL_SDP_REQD is set : It means that local SDP is sought from 
           the application in tunnelled MMS response. This is indicated to the
           application by setting the boolean flag local_sdp_reqd in tunnelled
           MMS request to ICF_TRUE. 

           ICF_LOCAL_SDP_REQD is not set : It means that local SDP is not sought
           from the application in tunnelled MMS response. This is indicated to the
           application by setting the boolean flag local_sdp_reqd in tunnelled
           MMS request to ICF_FALSE.

           ICF_TUNNELLED_REMOTE_SDP_PRESENT is not set : It means that remote SDP is not 
           being given to the application in tunnelled MMS request.
    
           ICF_TUNNELLED_REMOTE_SDP_PRESENT is set : It means that remote SDP is being given 
           to the application in tunnelled MMS request.

           On the basis of these two bits, CC will send a MMS request to MMI in tunnel mode.
        */

        if(ICF_NULL != p_stream_sdp_info)
        {
            /* First setting media_modify_type to NULL. */
            p_stream_sdp_info->media_modify_type = ICF_NULL;
            p_stream_sdp_info->media_modify_type |= ICF_LOCAL_SDP_REQD;
            p_stream_sdp_info->media_modify_type |= ICF_TUNNELLED_REMOTE_SDP_PRESENT;
        }
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_make_mms_type_for_answer_in_tunnel_mode
 * DESCRIPTION:     This function sets the media_modify_type bitmask which
 *		            shall determine whether tunnelled SDP will be present in
 *                  tunnelled MMS request and response.
 *
 ******************************************************************************/
icf_return_t icf_cc_make_mms_type_for_answer_in_tunnel_mode(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                   ret_val = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                        p_cc_pdb->p_call_ctx,
                                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        /* Even if the failure is due to p_stream_sdp_info being NULL, we will
           set the following bit. In tunnel mode, failure from this function
           due to any reason is considered equivalent. The calling function
           takes decisions on the basis of the bit ICF_CC_ALL_STREAMS_DELETED
           in CC PDB.
        */
        p_cc_pdb->common_bitmask |= ICF_CC_ALL_STREAMS_DELETED;
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
        return ret_val;
    }

    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    p_stream_sdp_info = (icf_stream_sdp_info_st*) p_cc_list->p_payload;

    /* Reset bit ICF_LOCAL_SDP_REQD and set bit ICF_TUNNELLED_REMOTE_SDP_PRESENT 
       in the media_modify_type field of the call context.
         
       Meaning of these two bits:

       ICF_LOCAL_SDP_REQD is set : It means that local SDP is sought from 
       the application in tunnelled MMS response. This is indicated to the
       application by setting the boolean flag local_sdp_reqd in tunnelled
       MMS request to ICF_TRUE. 

       ICF_LOCAL_SDP_REQD is not set : It means that local SDP is not sought
       from the application in tunnelled MMS response. This is indicated to the
       application by setting the boolean flag local_sdp_reqd in tunnelled
       MMS request to ICF_FALSE.

       ICF_TUNNELLED_REMOTE_SDP_PRESENT is not set : It means that remote SDP is not 
       being given to the application in tunnelled MMS request.

       ICF_TUNNELLED_REMOTE_SDP_PRESENT is set : It means that remote SDP is being given 
       to the application in tunnelled MMS request.

       On the basis of these two bits, CC will send a MMS request to MMI in tunnel mode.
    */

    if(ICF_NULL != p_stream_sdp_info)
    {
        /* First setting media_modify_type to NULL. */
        p_stream_sdp_info->media_modify_type = ICF_NULL;
        p_stream_sdp_info->media_modify_type &= ~ICF_LOCAL_SDP_REQD;
        p_stream_sdp_info->media_modify_type |= ICF_TUNNELLED_REMOTE_SDP_PRESENT;
    }
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_fetch_and_delete_sdp_stream_nodes_in_call_ctx
 * DESCRIPTION:     This function fetches and frees all the SDP stream nodes
 *                  in the remote received SDP of the call context except
 *                  the first node
 ******************************************************************************/

icf_return_t icf_cc_fetch_and_delete_sdp_stream_nodes_in_call_ctx(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_internal_list_st	*p_stream_sdp_node = ICF_NULL;
  
    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* Function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                        p_cc_pdb->p_call_ctx,
                                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    /* Extracting the stream SDP node in the call context */
    p_stream_sdp_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    
    /* Moving to the second node in the list */
    p_stream_sdp_node = p_stream_sdp_node->p_next;

    /* Iterating through the list and freeing the nodes */
    if(ICF_NULL != p_stream_sdp_node)
    {
        /*SPR 19502 instead of freeing the remote recv SDP we should
         free the entire node because in tunnel mode we do have only
         single applicable node.*/
        ret_val = icf_cmn_free_stream_sdp_info_list(p_cc_pdb->p_glb_pdb,
            &p_stream_sdp_node,
            p_cc_pdb->p_ecode);
    }
    /*SPR 19502 in call ctx mark the p_next of first node as null
      so that when traversing the list of nodes we do not process
      beyond first in any case */
    p_cc_pdb->p_call_ctx->p_stream_sdp_info->p_next = ICF_NULL;
    
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* Function exit trace */
    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        icf_cc_create_local_sdp
 *
 * DESCRIPTION:     This function allocates memory for the local SDP in the
 *                  call context in case of incoming calls.
 ******************************************************************************/
icf_return_t icf_cc_create_local_sdp
(       INOUT icf_cc_pdb_st	*p_cc_pdb)
{

    icf_stream_sdp_info_st     *p_stream_sdp = ICF_NULL;
    icf_return_t               ret_val = ICF_SUCCESS;
    icf_internal_list_st       *p_stream_sdp_node = ICF_NULL;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /* function entry trace */

    ICF_CHECK_IF_ANY_OF_TWO_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                        p_cc_pdb->p_call_ctx,
                                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }

    /* Extracting the stream SDP node in the call context */
    p_stream_sdp_node = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    p_stream_sdp = (icf_stream_sdp_info_st *)(p_stream_sdp_node->p_payload);
    
    if(ICF_NULL != p_stream_sdp)
    {
        /* Assigning memory of type icf_internal_sdp_stream.*/
        ICF_MEMGET(p_cc_pdb->p_glb_pdb,
                   ICF_PORT_SIZEOF(icf_internal_sdp_stream_st),
                   ICF_MEM_SDP,
                   p_stream_sdp->p_local_sdp,
                   ICF_RET_ON_FAILURE,
                   p_cc_pdb->p_ecode,
                   ret_val)

        /* Memory successfully allocated */
        /* Initialising icf_internal_sdp_stream_st */
        icf_port_memset(p_stream_sdp->p_local_sdp, 0, 
                    ICF_PORT_SIZEOF(icf_internal_sdp_stream_st));

        /* The following macro has been called to remove a false postive
           klokwork warning about the memory acquired for 
           p_stream_sdp->p_local_sdp. This memory is freed through 
           the following function calls while the call context
           is being cleared:
           icf_cc_clear_call_context -> icf_cc_clear_all_stream_sdp ->
           icf_cmn_free_stream_sdp_info_list.
        */
        ICF_KLOCWORK_FALSE_POSITIVE(p_cc_pdb->p_glb_pdb,
                                    p_stream_sdp->p_local_sdp)
    }
 
    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* Function exit trace */
    return ret_val;
}

/*CSR_1-6347417 : SPR-19129 fixed.*/
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_validate_inc_offer_change_on_fax_retry
 *
 * DESCRIPTION:     This function will validate the ith level
 *                  check for the mlines received in Re-INVITE/UPDATE
 *                  from the network when retry after timer is 
 *                  running for start fax req.
 ******************************************************************************/
icf_return_t icf_cc_validate_inc_offer_change_on_fax_retry(
        INOUT   icf_cc_pdb_st   *p_cc_pdb)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_internal_list_st           *p_cc_list = ICF_NULL;
    icf_list_st                    *p_rem_recvd_list = ICF_NULL;
    icf_stream_sdp_info_st         *p_stream_sdp_info = ICF_NULL;
    icf_inc_media_sdp_info_st      *p_nw_inc_sdp =ICF_NULL;
    icf_uint8_t                     virtual_ctr = 1;

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    p_cc_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;
    p_rem_recvd_list = p_cc_pdb->p_list_1;

        /* Check the stream id corresponding to virtual counter
         * in the SDP received from SSA and if the stream types don't
         * match ,then return failure.
         */
        while((ICF_NULL != p_cc_list) && (ICF_NULL != p_rem_recvd_list))
        {
            p_stream_sdp_info = p_cc_list->p_payload;

            p_nw_inc_sdp = p_rem_recvd_list->p_data;

            /*If the fax method type was replace type then SSA wouldn't have
             *sent the correct  stream id with the stream so we shouldnt use
             *it for fetching the stream info*/

            if(ICF_FAX_METHOD_REPLACE_STREAM == 
                       p_cc_pdb->p_call_ctx->fax_method)
             {
                while(p_cc_list)
                    {
                      if((ICF_NULL != p_cc_list->p_payload) &&
                         (((icf_stream_sdp_info_st*)p_cc_list->p_payload)->\
                             stream_type == ICF_STREAM_TYPE_FAX))
                       {
                          p_stream_sdp_info = p_cc_list->p_payload;
                          break;
                       }
                      else
                       {
                          p_cc_list = p_cc_list->p_next;
                       }
                     }
             }

            /* Return failure if :-
             * 1. Stream_id or stream_type mismatch occurs.
             * 2. If incoming Re-INVITE offer contains any m-line with port 0.
             */

            if ((virtual_ctr == p_nw_inc_sdp->stream_id) &&
                       (p_stream_sdp_info->stream_type ==
                                p_nw_inc_sdp->stream_type) &&
                (ICF_TRUE == p_nw_inc_sdp->stream_active))
            {
                ret_val = ICF_SUCCESS;
            }
            else
            {
               ICF_CC_TRACE(p_cc_pdb,ICF_TRACE_ITH_LEVEL_VALIDATION_FAILED)
               /* Raise alarm */
               ret_val = ICF_FAILURE;
               p_cc_pdb->common_bitmask |= 
               ICF_CC_STREAM_POS_VALIDATION_FAIL;
               break ;
            }
            p_rem_recvd_list = p_rem_recvd_list->p_next;

            virtual_ctr++;
            if(ICF_NULL != p_cc_list)
            { 
                p_cc_list = p_cc_list->p_next;
            }
        }

    /* Return failure if the number of m-lines in SDP of outgoing offer is
     * not equal to the number of m-lines in SDP of received offer.
     */
      
    if(((ICF_NULL == p_cc_list) && (ICF_NULL != p_rem_recvd_list)) ||
       ((ICF_NULL != p_cc_list) && (ICF_NULL == p_rem_recvd_list)))
    {
        ret_val = ICF_FAILURE;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /*function entry trace */
        return ret_val;
}
/*******************************************************************************
 *
 * FUNCTION:        icf_cc_process_remote_fax_req_when_retry
 *
 * DESCRIPTION:     This function validates and processes the remote fax request
 *                  when retry after timer is running after receiving the 491
 *                  of Re-INVITE/UPDATE for start fax.  
 *
 *
 ******************************************************************************/
icf_return_t  icf_cc_process_remote_fax_req_when_retry(
        INOUT  icf_cc_pdb_st *p_cc_pdb)
{

    icf_return_t  ret_val = ICF_SUCCESS;

    icf_nw_inc_media_change_req_st *p_start_fax_req =
        (icf_nw_inc_media_change_req_st *)p_cc_pdb->p_internal_msg->p_msg_data;

    icf_stream_sdp_info_st     *p_stream_sdp_info = ICF_NULL; 
    icf_internal_sdp_stream_st *p_stream_info     = ICF_NULL;
    icf_inc_media_sdp_info_st  *p_fax_sdp_info    = ICF_NULL;
    icf_internal_list_st       *p_list            = ICF_NULL;
    icf_uint32_t stream_id = 0;

    icf_list_st *p_temp_list = ICF_NULL;
    

    ICF_FUNCTION_ENTER(p_cc_pdb->p_glb_pdb) /*function entry trace */

    ICF_CHECK_IF_ANY_OF_THREE_VAR_IS_NULL(p_cc_pdb->p_glb_pdb,
                                        p_cc_pdb->p_call_ctx,
                                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                        p_start_fax_req,
                                        ret_val)
    if(ICF_FAILURE == ret_val)
    {
        ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* Function exit trace */
        return ret_val;
    }    

    ICF_CC_RESET_MODIFY_MEDIA_TYPE(p_cc_pdb)
    p_stream_sdp_info = (icf_stream_sdp_info_st *)p_cc_pdb->p_call_ctx
        ->p_stream_sdp_info->p_payload;

    p_cc_pdb->p_list_1 = p_start_fax_req->p_sdp_list;
    p_temp_list = p_start_fax_req->p_sdp_list;

    while(ICF_NULL != p_temp_list)
    {
        p_fax_sdp_info = (icf_inc_media_sdp_info_st *)p_temp_list->p_data;

        if(ICF_NULL != p_fax_sdp_info)
        {
            p_stream_info  = p_fax_sdp_info->p_int_sdp_stream;

            if(ICF_STREAM_TYPE_AUDIO == p_fax_sdp_info->stream_type)
            {
                stream_id = p_fax_sdp_info->stream_id;

                /* Call will be cleared if audio stream has been 
                 * deleted and audio line is received from remote in SDP.
                 */
                if(ICF_FALSE == p_fax_sdp_info->stream_active)
                {
                    /*
                       there is a problem
                       clear the call
                     */
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    break;
                }

                p_list = icf_cmn_get_node_in_internal_list(
                        p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                        &stream_id);

                if(ICF_NULL == p_list)
                {
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    break;
                }
                else
                {
                    p_stream_sdp_info = 
                        (icf_stream_sdp_info_st *)p_list->p_payload;
                }

            }
            else if((ICF_STREAM_TYPE_FAX == p_fax_sdp_info->stream_type) &&
                    (ICF_FAX_PT_CALL != p_cc_pdb->p_call_ctx->fax_type))
            {
                if(ICF_FALSE == p_fax_sdp_info->stream_active)
                {
                    /*
                       there is a problem
                       clear the call
                     */
                    p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                    ret_val = ICF_FAILURE;
                    break;
                }
                else
                {
                    /*If the fax method type was replace type then SSA wouldn't have
                      sent the correct  stream id with the stream so we shouldnt use
                      it for fetching the stream info*/
                    if(ICF_FAX_METHOD_REPLACE_STREAM == 
                            p_cc_pdb->p_call_ctx->fax_method)
                    {
                        p_list = p_cc_pdb->p_call_ctx->p_stream_sdp_info;

                        while(p_list)
                        {
                            if(((icf_stream_sdp_info_st*)p_list->p_payload)->\
                                    stream_type == ICF_STREAM_TYPE_FAX)
                            {
                                break;
                            }
                            else
                            {
                                p_list = p_list->p_next;
                            }
                        }
                    }
                    else
                    {
                        stream_id = p_fax_sdp_info->stream_id;
                        p_list = icf_cmn_get_node_in_internal_list(
                                p_cc_pdb->p_call_ctx->p_stream_sdp_info,
                                &stream_id);
                    }
                    if(ICF_NULL == p_list)
                    {
                        p_cc_pdb->common_bitmask |= ICF_CC_STREAM_POS_VALIDATION_FAIL;
                        ret_val = ICF_FAILURE;
                        break;
                    }
                    else
                    {
                        p_stream_sdp_info = 
                            (icf_stream_sdp_info_st *)p_list->p_payload;

                        p_stream_sdp_info->media_modify_type =
                            ICF_REMOTE_ANSWER_CODEC_CHANGE;
                    }

                }
            }
        }

        p_temp_list = p_temp_list->p_next;
    }

   if(ICF_FAILURE == icf_cc_validate_ith_level_m_line_check(p_cc_pdb))
    {
       /* ith level validation fails , reject the offer */
       p_cc_pdb->p_call_ctx->offer_failure_reason = ICF_FAX_ERR_INVALID_REQUEST;
       ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
       return ICF_SUCCESS;
    }

    ICF_FUNCTION_EXIT(p_cc_pdb->p_glb_pdb) /* function exit trace */
    return ret_val;
}

