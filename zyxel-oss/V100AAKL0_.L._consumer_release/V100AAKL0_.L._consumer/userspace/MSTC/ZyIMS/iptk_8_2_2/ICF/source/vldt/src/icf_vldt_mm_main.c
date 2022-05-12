/*------------------------------------------------------------------------------
 *
 * File name        : icf_vldt_mm_main.c
 *
 * Purpose          : This file contains the funstions to validate the various API'
 *                    called by PA towards ICF.
 *
 * Revision History :
 *
 * Date         Name              Ref#            Comments
 * --------     ------------      ----            ---------------
 * 13-JUN-2007  Jagmohan Chauhan   No              Initial
                Rishi Saxena
 * 14-Feb-2007  Ashutosh Mohan     Rel8.1       Added code for Tunnelled APIs
 * 07-May-2010  Aman Aggarwal     SPR 20391       Merged fixes in CSR 1-8561947
 * Copyright (c) 2010, Aricent.
 *---------------------------------------------------------------------------*/

#include "icf_vldt_prototypes.h"
#include "icf_vldt_defs.h"

/**********************************************************************************************
*
* FUNCTION:        icf_vldt_mm_api
*
* DESCRIPTION:     This function is invoked to validate each API field depending on the api-id 
*
*
***********************************************************************************************/
 
icf_return_t icf_vldt_mm_api(
                              icf_msg_st* p_msg  ,
                              icf_vldt_err_st *p_vldt_err)
{
        
    icf_uint8_t*  p_payload = ICF_NULL;
    icf_return_t ret_val = ICF_SUCCESS;
       
    /* the message payload*/
    p_payload = p_msg->payload;
        
    /* Validate the Payload according to API Types*/
    switch (p_msg->hdr.api_id) {
       

    case ICF_GET_CODEC_LIST_RESP:
         ret_val = icf_vldt_icf_get_codec_list_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;


    case ICF_CODEC_COMMIT_RESP:
         ret_val  = icf_vldt_icf_codec_commit_resp_st(
                                              p_payload ,
                                              p_vldt_err);
         break;



    case ICF_OPEN_MEDIA_CHANNEL_RESP:
         ret_val = icf_vldt_icf_open_media_channel_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;


    case ICF_CREATE_MEDIA_SESSION_RESP:
         ret_val = icf_vldt_icf_create_media_session_resp_st(
                                             p_payload ,
                                             p_vldt_err);
   
         break;

    case ICF_MODIFY_MEDIA_SESSION_RESP:
         ret_val = icf_vldt_icf_modify_media_session_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;

    case ICF_DELETE_MEDIA_SESSION_IND:
         ret_val = icf_vldt_icf_delete_media_session_ind_st(
                                              p_payload ,
                                              p_vldt_err);
         break;


    case ICF_MEDIA_CAPABILITIES_RESP:
         ret_val = icf_vldt_icf_media_capabilities_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;

    case ICF_RECV_UDP_DATA_IND:
         ret_val = icf_vldt_icf_recv_udp_data_ind_st(
                                             p_payload ,
                                             p_vldt_err);
         break;

    case ICF_MERGE_MEDIA_SESSION_RESP:
        /*validation for merge_media_session_resp */
        ret_val =  icf_vldt_icf_msg_resp_st(
                                     p_payload,
                                     p_vldt_err);
        break;

    case ICF_DELETE_MEDIA_SESSION_RESP:
        /*validation for delete_media_session_resp */
        ret_val =  icf_vldt_icf_msg_resp_st(
                                    p_payload,
                                    p_vldt_err);
        break;
    
    case ICF_TUNNELLED_MEDIA_CAPABILITIES_RESP:
         ret_val = icf_vldt_icf_tunnelled_media_capabilities_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;

    case ICF_TUNNELLED_MODIFY_MEDIA_SESSION_RESP:
         ret_val = icf_vldt_icf_tunnelled_modify_media_session_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;

    case ICF_TUNNELLED_CREATE_MEDIA_SESSION_RESP:
         ret_val = icf_vldt_icf_tunnelled_create_media_session_resp_st(
                                             p_payload ,
                                             p_vldt_err);
         break;

    default :
/*ZyXEL porting*/
#if 0	
        /* Merged CSR 1-7931204. Fix for CSR 1-8561947.*/
        ICF_VLDT_PRINT(("Invalid  API\n")); /* If no matching api found */
        ret_val = ICF_FAILURE;
#endif
        break;
/*ZyXEL porting end*/

    } /* End of switch-case */

    return ret_val;

} /* End of function icf_vldt_api_msg */

