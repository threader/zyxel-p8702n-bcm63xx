/***********************************************************************************
 *
 * File name        : iptkapp_cfg_msg_handler.c
 *
 * Purpose          : The files provide the functions to perform various operations
 *					  for messages recv/send from/to IP Phone Toolkit.	
 *
 * Date         Name            Ref#            Comments
 * --------     ------------    ----            ---------------
 * 07-Dec-2007  Amit Sikka   					Initial version
 *
 * Copyright (c) 2007 Aricent Inc . All Rights Reserved
 ****************************************************************************************/

#include "iptkapp_macros.h"
#include "icf_clib.h"
#include "iptkapp_cfg_types.h"
#include "iptkapp_port_prototypes.h"
#include "iptkapp_feature_flags.h"

/* This is universal application id*/
icf_uint32_t g_app_id = 1;

/*******************************************************************************
 *
 * FUNCTION NAME : iptkapp_cfg_fill_hdr_and_send_iptk
 *
 * DESCRIPTION   : The function to fill the header of the message and send to ICF
 *
 * RETURNS       : iptkapp_return_t
 *
 ******************************************************************************/
iptkapp_return_t iptkapp_cfg_fill_hdr_and_send_iptk(icf_msg_st *p_msg,
												icf_api_id_t api_id,
												icf_call_id_t call_id,
												iptkapp_error_t *p_err)
{

	iptkapp_return_t	ret_val = 0;
	icf_uint8_t     * p_buffer=NULL;
	icf_uint32_t	buf_len=0;
#ifdef IPTK_REL_7_0
    icf_vldt_err_st  vldt_err;
#endif

	/* Every message towards IPTK needs a header.
     * This function fills the values in the header */
	if(ICF_APP_ADD_REQ == api_id) /* network activate need to be checked */
	{
		IPTKAPP_MSG_FILL_HDR(p_msg,api_id,IPTKAPP_MODULE_PA,
						ICF_MODULE_ICF,ICF_INVALID_CALL_ID,ICF_GENERIC_APP_ID);
	}
	else
	{

		IPTKAPP_MSG_FILL_HDR(p_msg,api_id,IPTKAPP_MODULE_PA,
						ICF_MODULE_ICF,call_id,g_app_id);
	}

	/* Message to be send towards IPTK will be encoded and validated by following
     * function. This is performed by clib library of IPTK which is linked with
     * IPTK Application.
     */
#ifdef IPTK_REL_7_0
    ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,p_err,&vldt_err);
#else
    ret_val = icf_encode_msg((icf_msg_st*)p_msg, &p_buffer, &buf_len,p_err);
#endif


	
	if( ret_val !=  ICF_SUCCESS)
	{
			/* add the trace */
		return(IPTKAPP_FAILURE);
	}

	/* Call the macro to send it to the IPTK */
	IPTKAPP_MSG_SEND(p_buffer,buf_len,ret_val,p_err);
	
	/* Error in sending */
	if(ret_val==IPTKAPP_FAILURE)
	{
		icf_free_encoded_msg(p_buffer,p_err);  /* ICF - free message encoded */
		return(IPTKAPP_FAILURE);
	}

    /* memory taken by this buffer needs to be freed after sending to IPTK
     */
	
	icf_free_encoded_msg(p_buffer,p_err);   /* ICF - free encoded message*/

	return(IPTKAPP_SUCCESS);
}

