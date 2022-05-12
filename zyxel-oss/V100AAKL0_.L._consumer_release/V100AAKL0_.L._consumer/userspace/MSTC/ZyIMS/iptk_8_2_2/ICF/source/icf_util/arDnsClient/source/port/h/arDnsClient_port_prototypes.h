/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_port_prototypes.h                
*                                    
* DESCRIPTION  : Contains the definitions for the interfaces. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta           None          Initial
* 10-Feb-2009    Anuradha Gupta           LLD           Rel 1.0
*                                                                
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARDNSCLIENT_PORT_PROTOTYPES_H__
#define __ARDNSCLIENT_PORT_PROTOTYPES_H__

#include "arDnsClient_types.h"
#include "arDnsClient_port_types.h"

/* Prevent Name Mangling By C++ Compilers */

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_create_ipc
 *
 * Description: This function is used to craete an IPC.
 *
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t   arDnsClient_port_create_ipc (
			dnscl_void_t  *pDns_glb_data,
			arDnsClient_address_st    address,
			dnscl_int32_t             *p_fd,	  
			dnscl_error_t             *p_Error);

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_send_msg_to_dnscl
 *
 * Description: This function is used to pack and send message from CLIB to DNS Client.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t    arDnsClient_port_send_msg_to_dnscl(
	                   dnscl_void_t   *pDns_glb_data,
                       arDnsClient_request_st      *pRequest,
                       dnscl_error_t                *pError);


/*****************************************************************************
 * FUNCTION :   arDnsClient_port_send_msg_to_app
 *
 * Description: This function is used to pack and send message from DNS Client to App.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t    arDnsClient_port_send_msg_to_app(
	                   dnscl_void_t                 *pDns_glb_data,
                       arDnsClient_response_st      *pResponse,
                       dnscl_error_t                *pError);

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_recv_msg_frm_app
 *
 * Description: This function is used to receive and unpack message from application.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful
 *             else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t   arDnsClient_port_recv_msg_frm_app (
		               dnscl_void_t               *pDns_glb_data,
                       arDnsClient_request_st     **ppRequest,
		               dnscl_error_t              *pError);

/*****************************************************************************
 * FUNCTION :   arDnsClient_port_recv_msg_frm_dnscl
 *
 * Description: This function is used to receive and unpack message from Dns Clinet.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful
 *             else return ARDNSCL_FAILURE.
 ********************************************************************************/
dnscl_return_t   arDnsClient_port_recv_msg_frm_dnscl (
		               dnscl_void_t               *pDns_glb_data,
                       arDnsClient_response_st    **ppResponse,
		               dnscl_error_t              *pError);

/*****************************************************************************
 * FUNCTION :   arDnsClient_send_dns_query
 *
 * Description: This function is used to send DNS query.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t   arDnsClient_port_send_dns_query (
                        dnscl_void_t              *pDnscl_glb_data,
                        arDnsClient_query_st      *pDns_Query,
                        dnscl_buff_ptr            *ppOutBuff,
                        dnscl_uint32_t           *pResp_Length, 
                        dnscl_error_t             *pError);

 /*****************************************************************************
 * FUNCTION :   arDnsClient_port_set_dns_server
 *
 * Description: This function is used to configure DNS server dynamically.
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t  arDnsClient_port_set_dns_server(
                        dnscl_void_t              *pDnscl_glb_data,
                        arDnsClient_address_st    *pServer_addr,
                        dnscl_error_t             *pError);


/*****************************************************************************
 * FUNCTION :   arDnsClient_port_close_ipc
 *
 * Description: This function is used to close an IPC. 
 *                            
 * Return:      It will return ARDNSCL_SUCCESS if successful 
 *              else return ARDNSCL_FAILURE.
 ********************************************************************************/
 dnscl_return_t   arDnsClient_port_close_ipc (
                        dnscl_void_t 		 *pDns_glb_data,
                        dnscl_int32_t         fd,
                        dnscl_error_t        *pError);

/****************************************************************************
 * API Name: arDnsClient_port_get_timestamp
 * 
 * Description: This API is invoked to get time stamp value 
 *              request. 
 * INPUT       :pTimeStamp - The pointer to time stamp.
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_get_timestamp (
        dnscl_void_t 		 *pDns_glb_data,
        dnscl_uint32_t *pTimeStamp);

/****************************************************************************
 * API Name: arDnsClient_port_send_message
 * 
 * Description:    This function shall be to send message on IPC  
 * 
 * INPUT       : pDns_glb_data is the global data.
 *       :      p_fd is the  FD
 *       :      buffer is the  buffer to be sent
 *       :      msg_len is the  length of message to be sent
 *       :      send_addr is the address
 * OUTPUT:		pError is the returned error
 *
 * Return :    ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.

 ******************************************************************************/
dnscl_return_t    arDnsClient_port_send_message (
		dnscl_void_t   *pDns_glb_data,
		dnscl_int32_t                fd,
		dnscl_uint8_t                *pBuffer,
		dnscl_uint16_t                msg_len,
		arDnsClient_address_st       *pSend_addr,
		dnscl_error_t                *pError);

/****************************************************************************
 * API Name: arDnsClient_port_recv_message
 * 
 * Description:    This function shall be  used recv message on IPC 
 * 
 * INPUT       : pDns_glb_data is the global data.
 *       :      fd is the  FD
 *       :      buffer is the  buffer to be recv
 *       :      msg_len is the  length of message to be recv
 *       :      recv_addr is the address
 * OUTPUT:		pError is the returned error
 *
 * Return :    ARDNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.

 ******************************************************************************/
dnscl_return_t
arDnsClient_port_recv_message(
		dnscl_void_t              *pDns_glb_data,
		dnscl_int32_t 			   fd,
		dnscl_uint8_t 			  *pBuffer,
		dnscl_uint32_t 			  *pMsg_len,
		dnscl_error_t 			  *pError);

/****************************************************************************
 * API Name: arDnsClient_port_generate_alarm
 * 
 * Description: This function is used to generate alarm 
 * INPUT       :pDnsCl_glb_data - The pointer to global data structure. 
 *             :pReq_data  -      Request data structure
 *             :error  -          error code
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_void_t
arDnsClient_generate_port_alarm (
		dnscl_void_t            *pDns_glb_data,
		arDnsClient_request_st  *pReq_data,
		dnscl_error_t            error);

 /******************************************************************************
  *   PACK / UNPACK interfaces
 ******************************************************************************/
/****************************************************************************
 * Function Name: arDnsClient_port_pack_U8 
 * 
 * Description: This Function is invoked to convert U8 data type to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_U8
(
    dnscl_uint8_t    *p_dest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_U16 
 * 
 * Description: This Function is invoked to convert U16 data type to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_U16
(
    dnscl_uint8_t    *p_dest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_U32 
 * 
 * Description: This Function is invoked to convert U32 data type to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_U32
(
    dnscl_uint8_t    *p_dest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_string 
 * 
 * Description: This Function is invoked to convert string data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_string(
        dnscl_uint8_t *pDest,
        arDnsClient_string_st *pStr,
        dnscl_uint32_t *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_short_string 
 * 
 * Description: This Function is invoked to convert short string data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_short_string(
        dnscl_uint8_t *pDest,
        arDnsClient_short_string_st *pStr,
        dnscl_uint32_t *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_address 
 * 
 * Description: This Function is invoked to convert address data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_address(
     dnscl_uint8_t    *pDest,
     arDnsClient_address_st    *p_dnscl_address,
 	 dnscl_uint32_t   *p_length);


/****************************************************************************
 * Function Name: arDnsClient_port_pack_conf 
 * 
 * Description: This Function is invoked to convert conf data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_conf(
        dnscl_uint8_t       *pDest, 
        arDnsClient_conf_st *pConfData,
        dnscl_uint32_t    *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_query 
 * 
 * Description: This Function is invoked to convert query type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_query(
        dnscl_uint8_t 		*pDest,
        arDnsClient_query_st *pDnsQuery,
        dnscl_uint32_t       *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_clear_cache 
 * 
 * Description: This Function is invoked to convert clear cache data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_clear_cache (
	  dnscl_uint8_t        *pDest,
   	  arDnsClient_clear_cache_st *pClearCache,
      dnscl_uint32_t       *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_rr_list
 * 
 * Description: This Function is invoked to convert Resource Record list 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_rr_list(
			dnscl_uint8_t          *pDest,
			dnscl_uint8_t          query_type,
			arDnsClient_dns_rr_list_st *pList,
            dnscl_uint32_t          *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_pack_query_resp
 * 
 * Description: This Function is invoked to convert query resp data type 
 *              to stream buffer. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_pack_query_resp(
	dnscl_uint8_t          *pDest,
    dnscl_query_t           query_type,
	arDnsClient_query_resp_st *pQuery_resp,
    dnscl_uint32_t          *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_U8 
 * 
 * Description: This Function is invoked to convert stream buffer to U8 data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_U8
(
    dnscl_uint8_t    *p_dest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_U16 
 * 
 * Description: This Function is invoked to convert stream buffer to U16 data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_U16
(
    dnscl_uint8_t    *p_dest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_U32
 * 
 * Description: This Function is invoked to convert stream buffer to U32 data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_U32
(
    dnscl_uint8_t    *p_dest,
    dnscl_uint8_t    *p_src,
    dnscl_uint32_t   *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_string 
 * 
 * Description: This Function is invoked to convert stream buffer to string
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_string
(
    arDnsClient_string_st    *pDest,
    dnscl_uint8_t    		*pSrc,
    dnscl_uint32_t 		*p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_string 
 * 
 * Description: This Function is invoked to convert stream buffer to short  string
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_short_string
(
    arDnsClient_short_string_st    *pDest,
    dnscl_uint8_t    		*pSrc,
    dnscl_uint32_t 		*p_length);


/****************************************************************************
 * Function Name: arDnsClient_port_unpack_address
 * 
 * Description: This Function is invoked to convert stream buffer to address 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_address(
    arDnsClient_address_st  *pDest,
    dnscl_uint8_t           *pSrc,
    dnscl_uint32_t          *p_length);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_conf
 * 
 * Description: This Function is invoked to convert stream buffer to conf 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_conf(
	arDnsClient_conf_st **ppConfData,
	dnscl_uint8_t        *pSrc,
	dnscl_uint32_t        *pLength,
	dnscl_error_t        *pError);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_query
 * 
 * Description: This Function is invoked to convert stream buffer to query 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_query(
	arDnsClient_query_st **ppDnsQuery,
	dnscl_uint8_t 	     *pSrc,
	dnscl_uint32_t        *pLength,
	dnscl_error_t        *pError);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_query
 * 
 * Description: This Function is invoked to convert stream buffer to query 
 *              data type. 
 * 
 ******************************************************************************/
dnscl_return_t
arDnsClient_port_unpack_clear_cache(
	arDnsClient_clear_cache_st **ppClearCache,
	dnscl_uint8_t 	     *pSrc,
	dnscl_uint32_t        *pLength,
	dnscl_error_t        *pError);

/****************************************************************************
 * Function Name: arDnsClient__unpack_query_resp
 * 
 * Description: This Function is invoked to unpack query response. 
 * 
 ******************************************************************************/
dnscl_uint32_t
arDnsClient_port_unpack_query_resp(
	arDnsClient_query_resp_st   **ppQuery_resp,
	dnscl_query_t    	    query_type,
	dnscl_uint8_t    	    *pSrc,
	dnscl_uint32_t              *pLength,
        dnscl_error_t               *pError);

/****************************************************************************
 * Function Name: arDnsClient_port_unpack_query_rr_data
 * 
 * Description: This Function is invoked to unpack query resource record 
 * 
 ******************************************************************************/
dnscl_uint32_t
arDnsClient_port_unpack_query_rr_data(
		arDnsClient_rr_data_un  *pQuery_rr_data,
		dnscl_uint8_t    	query_type,
		dnscl_uint8_t    	*pSrc,
		dnscl_uint32_t          *pLength);

/****************************************************************************
 * API Name: arDnsClient_port_pack_request
 * 
 * Description: This API is invoked by the application for packing the request 
 *              ie converting the request structure to stream buffer. 
 *  PUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pRequest - The pointer to request structure.  
 * PUT      :ppDest - The pointer to the stream buffer
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_pack_request (
          dnscl_void_t             *pDnsCl_glb_data,
          arDnsClient_request_st   *pRequest,
        dnscl_uint8_t            **ppDest,
        dnscl_uint16_t           *pLength,
        dnscl_error_t            *pError);

/****************************************************************************
 * API Name: arDnsClient_port_pack_response
 * 
 * Description: This API is invoked by the application for packing the response 
 *              ie converting the response structure to stream buffer. 
 *  PUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pResponse - The pointer to response structure.  
 * PUT      :ppDest - The pointer to the stream buffer
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_pack_response (
          dnscl_void_t             *pDnsCl_glb_data,
          arDnsClient_response_st  *pResponse,
        dnscl_uint8_t            **ppDest,
        dnscl_uint16_t           *pLength,
        dnscl_error_t            *pError);

/****************************************************************************
 * API Name: arDnsClient_port_unpack_request
 * 
 * Description: This API is invoked by the application for unpacking the request 
 *              ie converting the stream buffer to request structure. 
 *  PUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pSrc - The pointer to the stream buffer
 * PUT      :ppRequest - The pointer to request structure
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_unpack_request (
         dnscl_void_t             *pDnsCl_glb_data,
         dnscl_uint8_t            *pSrc,
       arDnsClient_request_st   **ppRequest,
       dnscl_uint16_t           *pLength,
       dnscl_error_t            *pError);

/****************************************************************************
 * API Name: arDnsClient_port_unpack_response
 * 
 * Description: This API is invoked by the application for unpacking the response 
 *              ie converting the stream buffer to responset structure. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pSrc - The pointer to the stream buffer
 * OUTPUT      :ppResponse - The pointer to response structure
 *             :pLength - Pointer to the lenght of the stream buffer.
 *             :pError - Error pointer to be populated in case of error. 
 *
 * Return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_port_unpack_response (
       dnscl_void_t             *pDnsCl_glb_data,
       dnscl_uint8_t            *pSrc,
       arDnsClient_response_st  **ppResponse,
       dnscl_uint16_t           *pLength,
       dnscl_error_t            *pError);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARDNSCLIENT_PORT_PROTOTYPES_H__ */



