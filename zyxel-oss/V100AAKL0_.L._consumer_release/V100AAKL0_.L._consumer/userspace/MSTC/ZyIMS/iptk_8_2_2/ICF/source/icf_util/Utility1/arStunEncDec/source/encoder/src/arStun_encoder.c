/******************************************************************************
*                                    
* FILE NAME    : arStun_encoder.c                
*                                    
* DESCRIPTION  : Contains the arStun encoding routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial
*           
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "arStunEncDec_api.h"
#include "arStunEncDec_types.h"
#include "arStun_enc_prototypes.h"

/****************************************************************************
 * API Name: arStun_create_binding_req
 * 
 * Description: This API is invoked by the application create a STUN 
 *               :binding request message structure. 
 *		 :The message structure will be used for encoding the binding request message.
 * INPUT       :arStun_string_st is the user name.
 *             :isChangePortReq is the change port req.
 *             :isChangeIpReq is the change ip req.
 *             :pResponseAddr is the response address.
 * OUTPUT      :arStun_msg_st  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_create_binding_req(
             OUT arStun_msg_st          *pStunMessage,
             IN  arStun_string_st       *pUsername,
             IN  util_boolean_t         isChangePortReq,
             IN  util_boolean_t         isChangeIpReq,
             IN  arStun_ip_port_address_st *pResponseAddr,
             OUT arStun_ecode_et           *pError)
{
	/*checking for error*/
	if(UTIL_NULL == pError)

	{
		ARSTUN_PRINT(("\nError pointer is NULL"));
		return UTIL_FAILURE;
	}
    /*Check for stun message*/
    if(UTIL_NULL == pStunMessage)

    {
        ARSTUN_PRINT(("\nStun Message is NULL"));
        *pError = ARSTUN_ERROR_INVALID_STUN_MSG_STRUCT;
        return UTIL_FAILURE;
    }
   
    
    UTIL_MEMSET(pStunMessage, 0, 
            UTIL_SIZEOF(arStun_msg_st));

    /*form the stun message header*/
    pStunMessage->msg_hdr.msg_type = ARSTUN_BIND_REQ_MSG;
    pStunMessage->msg_hdr.msg_length = UTIL_INIT_VAL;
    pStunMessage->msg_attrib_presence = UTIL_INIT_VAL;

    /*Generate the randon transaction id*/
    arStun_generate_transactionID (
            &(pStunMessage->msg_hdr.transaction_id));

    /*Check for change port request and change port ip*/
    if(isChangePortReq == UTIL_TRUE || isChangeIpReq == UTIL_TRUE)
    {
        /*Setting the bit mask for change request*/
        pStunMessage->msg_attrib_presence |= ARSTUN_CHANGE_REQUEST_PRESENT;
        pStunMessage->change_request = 0;

        /*Setting the change request value in stun message */
        if(UTIL_TRUE == isChangePortReq)
        {
            pStunMessage->change_request |= ARSTUN_CHANGE_PORTFLAG_VALUE;
        }

        if(UTIL_TRUE == isChangeIpReq)
        {
            pStunMessage->change_request |= ARSTUN_CHANGE_IPFLAG_VALUE;
        }
    }

    if((UTIL_NULL != pUsername) && (UTIL_INIT_VAL != pUsername->str_len))
    {
        /*Settng the bit mask for user name*/
        pStunMessage->msg_attrib_presence |= ARSTUN_USER_NAME_PRESENT;

        /*Setting the user name value in stun message */
        UTIL_MEMCPY(&pStunMessage->username,
                pUsername,
                UTIL_SIZEOF(arStun_string_st));

    }

    if((UTIL_NULL != pResponseAddr) &&  (UTIL_INIT_VAL !=  pResponseAddr->port))
    {
        /*Setting the bit mask for response address*/
        pStunMessage->msg_attrib_presence |= ARSTUN_RESPONSE_ADDRESS_PRESENT;

        /*Setting the response address value in stun message*/
        UTIL_MEMCPY(&pStunMessage->response_addr.ipaddr,
                pResponseAddr,
                UTIL_SIZEOF(arStun_ip_port_address_st));
    }
    return UTIL_SUCCESS;
}

/****************************************************************************
 * API Name: arStun_create_binding_resp
 * 
 * Description: This API is invoked to to create a STUN binding response structure. 
 *      	:The message structure will be used for encoding the success binding 
 *              :response message.
 * INPUT       :pTransactionID is the transaction id.
 *             :pChangedAddress is the change address.
 *             :pSourceAddress is the source address.
 *             :pMappedAddress is the mapped address.
 *             :pReflectedFrom is the reflected from address.
 * OUTPUT      :arStun_msg_st  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_create_binding_resp(
		OUT    arStun_msg_st          *pStunMessage,
		IN     arStun_uint128_t       *pTransactionID,
		IN     arStun_attr_addr_st    *pChangedAddress,
		IN     arStun_attr_addr_st    *pSourceAddress,
		IN     arStun_attr_addr_st    *pMappedAddress,
		IN     arStun_attr_addr_st    *pReflectedFrom,
		OUT    arStun_ecode_et        *pError)
{
	/*Check for error*/
	if(UTIL_NULL == pError)

	{
		ARSTUN_PRINT(("\nError pointer is NULL"));
		return UTIL_FAILURE;
	}


	/*Check for stun message*/
	if(UTIL_NULL == pStunMessage)

	{
		ARSTUN_PRINT(("\nStun Message is NULL"));
		*pError = ARSTUN_ERROR_INVALID_STUN_MSG_STRUCT;
		return UTIL_FAILURE;
	}

	UTIL_MEMSET(pStunMessage, 0, 
			UTIL_SIZEOF(arStun_msg_st));


	/*Setting bit mask for stun message as zero*/
	pStunMessage->msg_attrib_presence=UTIL_INIT_VAL;

	/*form the stun message header*/
	pStunMessage->msg_hdr.msg_type = ARSTUN_BIND_RESP_MSG;
	pStunMessage->msg_hdr.msg_length = 0;
	pStunMessage->msg_hdr.transaction_id =*pTransactionID;

	/*Check for changed address*/
	if((UTIL_NULL!= pChangedAddress) && 
			(UTIL_INIT_VAL != pChangedAddress->ipaddr.port))
	{
		/*Set the bit mask for change address*/
		pStunMessage->msg_attrib_presence |= ARSTUN_CHANGED_ADDRESS_PRESENT;

		/*Set the value of changed address*/
		UTIL_MEMCPY(&pStunMessage->changed_addr,
				pChangedAddress,
				UTIL_SIZEOF(arStun_attr_addr_st));
	}
	else
	{
		ARSTUN_PRINT(("\nMandatory attribute Changed Address is not present"));
	}



	/*Check for source address*/
	if((UTIL_NULL!= pSourceAddress) && (UTIL_INIT_VAL != pSourceAddress->ipaddr.port))
	{
		/*Setting the bit mask for source address*/
		pStunMessage->msg_attrib_presence |= ARSTUN_SOURCE_ADDRESS_PRESENT;

		/*Setting  the value of source address*/
		UTIL_MEMCPY(&pStunMessage->source_addr,
				pSourceAddress,
				UTIL_SIZEOF(arStun_attr_addr_st));
	}
	else
	{
		ARSTUN_PRINT(("\nMandatory attribute Source Address is not present"));
	}


	/*Check for mapped address*/
	if((UTIL_NULL!= pMappedAddress) && (UTIL_INIT_VAL != pMappedAddress->ipaddr.port))
	{
		/*Setting the bit mask for mapped address*/
		pStunMessage->msg_attrib_presence |= ARSTUN_MAPPED_ADDRESS_PRESENT;

		/*Setting  the value of mapped address*/
		UTIL_MEMCPY(&pStunMessage->mapped_addr,
				pMappedAddress,
				UTIL_SIZEOF(arStun_attr_addr_st));
	}
	else
	{
		ARSTUN_PRINT(("\nMandatory attribute Mapped Address is not present"));
	}


	/*Check for reflected from address*/
	if((UTIL_NULL!= pReflectedFrom) && (UTIL_INIT_VAL != pReflectedFrom->ipaddr.port))
	{
		/*Setting the bit mask for source address*/
		pStunMessage->msg_attrib_presence |= ARSTUN_REFLECTED_FROM_PRESENT;

		/*Setting  the value of mapped address*/
		UTIL_MEMCPY(&pStunMessage->reflected_from,
				pReflectedFrom,
				UTIL_SIZEOF(arStun_attr_addr_st));
	}

	return UTIL_SUCCESS;

}

/****************************************************************************
 * API Name: arStun_create_binding_err_resp
 * 
 * Description: This API is invoked to create the STUN Binding Error response message.
 * INPUT       :pTransactionID is the transcation id.
 *             :pErrorCode is the error response in binding message.
 *             :pUnknownAttribs is the unknown attributes.
 * OUTPUT      :pStunMessage  is stun message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
util_return_t
arStun_create_binding_err_resp(
     	OUT arStun_msg_st                *pStunMessage,
     	IN   arStun_uint128_t            *pTransactionID,
     	IN   arStun_attr_error_st        *pErrorCode,
     	IN   arStun_attr_unknown_st      *pUnknownAttribs,
     	OUT  arStun_ecode_et             *pError)
{
	/*Check for error*/
	if(UTIL_NULL == pError)

	{
		ARSTUN_PRINT(("\nError pointer is NULL"));
		return UTIL_FAILURE;
	}

	/*Check for stun message*/
	if(UTIL_NULL == pStunMessage)
	{
		ARSTUN_PRINT(("\nStun Message is NULL"));
		*pError = ARSTUN_ERROR_INVALID_STUN_MSG_STRUCT;
		return UTIL_FAILURE;
	}

	UTIL_MEMSET(pStunMessage, 0, 
			UTIL_SIZEOF(arStun_msg_st));


	/*form the stun message header*/
	pStunMessage->msg_hdr.msg_type = ARSTUN_BIND_ERR_RESP_MSG;
	pStunMessage->msg_hdr.msg_length = UTIL_INIT_VAL;
	pStunMessage->msg_hdr.transaction_id =*pTransactionID;
	pStunMessage->msg_attrib_presence = UTIL_INIT_VAL;

	/*Check for error code*/
	if((UTIL_NULL != pErrorCode) && (UTIL_INIT_VAL != pErrorCode->error_class))
	{
		/*Setting the bit mask for error code address*/
		pStunMessage->msg_attrib_presence |= ARSTUN_ERROR_CODE_PRESENT;

		/*Setting the value of error code*/
		UTIL_MEMCPY(&pStunMessage->error_code,
				pErrorCode,
				UTIL_SIZEOF(arStun_attr_error_st));
	} 
	else
	{
		ARSTUN_PRINT(("\nMandatory attribute Error Code is not present"));
	}


	/*Check for unknown attribs */
	if((UTIL_NULL != pUnknownAttribs) && (UTIL_INIT_VAL != pUnknownAttribs->num_attributes))
	{
		/*Setting the bit mask for error code address*/
		pStunMessage->msg_attrib_presence |= ARSTUN_UNKNOWN_ATTR_PRESENT;

		/*Setting the value of unknown attributes*/
		UTIL_MEMCPY(&pStunMessage->unknown_attributes,
				pUnknownAttribs,
				UTIL_SIZEOF(arStun_attr_unknown_st));
	}
	return UTIL_SUCCESS;
}
/****************************************************************************
 * API Name: arStun_encode_message
 * 
 * Description: This API is invoked to encode the stun message.
 * INPUT       :pStunMessage is the stun message.
 * OUTPUT      :pMsgBuffer  is encoded message. 
 *             :pError is the error code. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/
 util_return_t
 arStun_encode_message(
		 IN   arStun_msg_st         *pStunMessage,
                 IN   arStun_string_st   *pPassword,  
		 OUT  arStun_msg_buffer_st  *pMsgBuffer, 
		 OUT  arStun_ecode_et       *pError)
{
	/*tempory buffer*/
	util_uint8_t   *p_temp_buffer = UTIL_NULL;
	util_uint8_t   *p_store_buffer = UTIL_NULL;

	/*buffer length*/
	util_uint8_t   * pBuffLength = UTIL_NULL;

	/*Length of data encoded*/
	util_uint32_t    length = UTIL_INIT_VAL;

	/*count for transaction id */	
	util_uint32_t    count = UTIL_INIT_VAL;

	/*to remove warning*/
	pPassword = pPassword;

	/*Check for error*/
	if(UTIL_NULL == pError)

	{
		ARSTUN_PRINT(("\nError pointer is NULL"));
		return UTIL_FAILURE;
	}

	/*Check for stun message*/
	if(UTIL_NULL == pStunMessage)

	{
		ARSTUN_PRINT(("\nStun Message is NULL"));
		*pError = ARSTUN_ERROR_INVALID_STUN_MSG_STRUCT;
		return UTIL_FAILURE;
	}
	/*Check for message buffer*/
	if(UTIL_NULL == pMsgBuffer)

	{
		ARSTUN_PRINT(("\nMessage buffer is NULL"));
		*pError = ARSTUN_ERROR_INVALID_MSG_BUFFER;
		return UTIL_FAILURE;
	}

	/*Assigning the address of message buffer to temp buffer*/
	p_temp_buffer = pMsgBuffer->msg_val;
	p_store_buffer = pMsgBuffer->msg_val;

	/*Encode the Message header*/
	arStun_encode_U16(p_temp_buffer, pStunMessage->msg_hdr.msg_type,&length);

	/*Increment the p_temp_buffer by value of length*/
	p_temp_buffer += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;


	pBuffLength = p_temp_buffer;

	/*Encode the value of length as 0*/
	arStun_encode_U16(p_temp_buffer, 0,&length);

	/*Increment the ptr by value of length*/
	p_temp_buffer += length;


	/*Encode the value of transaction id*/
	for(count = 0 ; count < 16; count ++)
	{
		/*Setting the value of length as 0*/                  
		length = UTIL_INIT_VAL;

		arStun_encode_U8(p_temp_buffer, 
				pStunMessage->msg_hdr.transaction_id.octet[count],                 
				&length);

		/*Increment the ptr by value of length*/
		p_temp_buffer ++;
	}

	/*Check for bit mask of Mapped address*/
	if(ARSTUN_MAPPED_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence )
	{
		/*encode the mapped address*/
		arStun_encode_ipv4_addr (
                &p_temp_buffer, 
				ARSTUN_ATTRIB_MAPPED_ADDRESS,          
				pStunMessage->mapped_addr);
	}

	/*Check for bit mask of Response address*/
	if(ARSTUN_RESPONSE_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence)
	{
		/*encode the response address address*/
		arStun_encode_ipv4_addr (
                &p_temp_buffer, 
				ARSTUN_ATTRIB_RESPONSE_ADDRESS, 
				pStunMessage->response_addr);

	}

	/*Check for bit mask of change request*/
	if(ARSTUN_CHANGE_REQUEST_PRESENT & pStunMessage->msg_attrib_presence)
	{
		/*encode the change request*/
		arStun_encode_change_req (
                &p_temp_buffer, 
				ARSTUN_ATTRIB_CHANGE_REQUEST, 
				pStunMessage-> change_request);

	}

	/*Check for bit mask of source address*/
	if(ARSTUN_SOURCE_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence)
	{
		/*encode the source address*/
		arStun_encode_ipv4_addr (
                &p_temp_buffer, 
				ARSTUN_ATTRIB_SOURCE_ADDRESS, 
				pStunMessage->source_addr);
	}

	/* Check for bit mask of changed address*/
	if(ARSTUN_CHANGED_ADDRESS_PRESENT & pStunMessage->msg_attrib_presence) 
	{
		/*encode the changed address*/
		arStun_encode_ipv4_addr (
                &p_temp_buffer, 
				ARSTUN_ATTRIB_CHANGED_ADDRESS, 
				pStunMessage->changed_addr);
	}

	/*Check for bit mask of User name*/ 
	if(ARSTUN_USER_NAME_PRESENT & pStunMessage->msg_attrib_presence)
	{
			/*encode the user name*/
			arStun_encode_string (
                &p_temp_buffer, 
				ARSTUN_ATTRIB_USERNAME,     
				pStunMessage-> username);
    }

	/* Check for bit mask of error code*/
	if(ARSTUN_ERROR_CODE_PRESENT & pStunMessage->msg_attrib_presence)
	{
	     /*encode the error code*/
	     arStun_encode_error_attr (
                &p_temp_buffer, 
	            ARSTUN_ATTRIB_ERROR_CODE,     
				pStunMessage->error_code);
     }

	/* Check for bit mask of unknown attributes*/
	if(ARSTUN_UNKNOWN_ATTR_PRESENT & pStunMessage->msg_attrib_presence)
	{
		/*encode the unknown attributes*/
		arStun_encode_unknown_attr(
                &p_temp_buffer, 
				ARSTUN_ATTRIB_UNKNOWN_ATTRIBUTE ,     
				pStunMessage-> unknown_attributes);
	}

	/* Check for bit mask of reflected from attributes*/
	if(ARSTUN_REFLECTED_FROM_PRESENT & pStunMessage->msg_attrib_presence)
	{
		/*encode the reflected from*/
		  arStun_encode_ipv4_addr (
                  &p_temp_buffer, 
		  		  ARSTUN_ATTRIB_REFLECTED_FROM, 
		          pStunMessage-> reflected_from);
	}

	arStun_encode_U16(
            pBuffLength,  
            (p_temp_buffer - p_store_buffer - UTIL_SIZEOF(arStun_msg_hdr_st)),  
			&length);

	pMsgBuffer->msg_len = (util_uint16_t)( p_temp_buffer - p_store_buffer);

	/*added to remove klockwork warning*/
	if(ARSTUN_MAX_MSG_LEN <= pMsgBuffer->msg_len)
	{
		pMsgBuffer->msg_len=ARSTUN_MAX_MSG_LEN-1;
	}

	pMsgBuffer->msg_val[pMsgBuffer->msg_len] = '\0';

	return UTIL_SUCCESS;
}


