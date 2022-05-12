/******************************************************************************
*                                    
* FILE NAME    : arStun_enc_util.c                
*                                    
* DESCRIPTION  : Contains the arStun Encoding utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Jan-2009    Anuradha Gupta         None          Initial encode utility
* 07-Jan-2009    Abbas Haque            None          Utility for stun encoder added
*           
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "common_defs.h"
#include "common_prototypes.h"
#include "arStun_enc_prototypes.h"
#include "arStun_enc_macro.h"

/****************************************************************************
 * API Name: arStun_encode_U8
 * 
 * Description:    This function shall be  used to encode 1 byte data 
 * 
 * INPUT       : data is the data to be encoded .
 *             :p_length  length of data that is to be encoded
 * OUTPUT      :p_dest is the varible to copy encoded data
 *
 * Return :    It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.

 ******************************************************************************/

util_void_t
arStun_encode_U8(
   		util_uint8_t     *p_dest,
		util_uint8_t     data,
		util_uint32_t    *p_length)
{
	/*copy the data to destination */ 
	UTIL_MEMCPY(p_dest, &data, UTIL_SIZEOF(util_uint8_t));

	/*Assign value of length as size of util_uint8_t */
	*p_length  = UTIL_SIZEOF(util_uint8_t);

	return ;


}

/****************************************************************************
 * API Name: arStun_encode_U16
 * 
 * Description:    This function shall be  used to encode 2 byte data 
 * 
 * INPUT       : data is the data to be encoded .
 * OUTPUT      :p_length  length of data that has been encoded. 
 *             :p_dest is the varible to copy encoded data
 *
 * Return :It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.

 ******************************************************************************/

util_void_t
arStun_encode_U16(
    		util_uint8_t    *p_dest,
   		util_uint16_t     data,
    		util_uint32_t    *p_length)
{
        util_uint16_t hns_data = UTIL_INIT_VAL;
 
	/*Convert data to host to network short byte order ie big 
	  endian*/
	hns_data = UTIL_HTONS(data);

	/*copy the converted data to buffer */
	UTIL_MEMCPY(p_dest, (util_void_t *)(& hns_data),   
			UTIL_SIZEOF(util_uint16_t));


	/*Assign value of length as size of util_uint16_t */
	*p_length  = UTIL_SIZEOF(util_uint16_t);

	return ;
}

/****************************************************************************
 * API Name: arStun_encode_U32
 * 
 * Description:    This function shall be  used to encode 4 byte data 
 * INPUT       : data is the data to be encoded .
 * OUTPUT      :p_length  length of data that has been encoded. 
 *             :p_dest is the varible to copy encoded data
 *
 * Return :It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

 util_void_t
arStun_encode_U32(
    		util_uint8_t     *p_dest,
    		util_uint32_t     data,
    		util_uint32_t    *p_length)
{
	util_uint32_t hnl_data = UTIL_INIT_VAL;
	
         /*Convert data to host to network long byte order ie big 
	  endian*/
	 hnl_data = UTIL_HTONL(data);

	/*copy the converted data to buffer */
	UTIL_MEMCPY(p_dest, (util_void_t *)(& hnl_data),   
			UTIL_SIZEOF(util_uint32_t));



	/*Assign value of length as size of util_uint32_t */
	*p_length  = UTIL_SIZEOF(util_uint32_t);

	return ;
}

/****************************************************************************
 * API Name: arStun_convert_ipaddr_to_data
 * 
 * Description :This function shall be  used to convert ip address into 4 bytes.  
 * INPUT       : ipv4_addr IP address.
 * OUTPUT      :ip_data 4 bytes converted ip ddress. 
 *
 * Return : 
 ******************************************************************************/

util_void_t  
arStun_convert_ipaddr_to_data(
    			     arStun_ipv4_address_st	  ipv4_addr, 
    			     util_uint32_t 	          *p_ip_data )
{
        /*Setting value of p_ip_data as zero*/
        *p_ip_data = UTIL_INIT_VAL;

	/*Right shift the ip address by 24 bit */
	*p_ip_data    |= (util_uint32_t)ipv4_addr.octet_1 << 24;

	/*Right shift the ip address by 16 bit */
	*p_ip_data    |= (util_uint32_t)ipv4_addr.octet_2 << 16;

	/*Right shift the ip address by 8 bit */
	*p_ip_data    |= (util_uint32_t)ipv4_addr.octet_3 << 8;

	/*store the value of ip address in ip_data */
	*p_ip_data    |= (util_uint32_t)ipv4_addr.octet_4;

}


/****************************************************************************
 * API Name: arStun_encode_ipv4_addr
 * 
 * Description:    This function shall be  used to encode ip address attribute type 
 * INPUT       : type is the attribute type.
 *             : atr is the IP address 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_ipv4_addr(
    			util_uint8_t 	    **pp_msg_buffer, 
    			util_uint16_t 	    type, 
    			arStun_attr_addr_st atr)
{
	/*Length of data encoded*/
	util_uint32_t    length = UTIL_INIT_VAL;

	/*Value of IPV4 Address*/ 
	util_uint32_t    ip_data = UTIL_INIT_VAL;

    util_uint8_t 	    *p_msg_buffer = *pp_msg_buffer;

	/*Encode the value of type*/
	arStun_encode_U16(p_msg_buffer, type, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of address length*/
	arStun_encode_U16(p_msg_buffer, ARSTUN_ATTRIB_ADDR_LEN, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*Encode the value of padding */
        atr.padding = UTIL_INIT_VAL;
	arStun_encode_U8(p_msg_buffer, atr.padding, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;


	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*Encode the value of ipfamily */
        atr. ipfamily= ARSTUN_IPV4_FAMILY;
	arStun_encode_U8(p_msg_buffer, atr.ipfamily, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of port*/
	arStun_encode_U16(p_msg_buffer, atr.ipaddr.port, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

	/*Convert ip address to 4 byte data*/
	arStun_convert_ipaddr_to_data(atr.ipaddr.ipv4_address,&ip_data);

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of converted ip address*/
	arStun_encode_U32(p_msg_buffer , ip_data, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

    *pp_msg_buffer = p_msg_buffer;
	return;
}

/****************************************************************************
 * API Name: arStun_encode_change_req
 * 
 * Description:    This function shall be  used to encode change req attribute type 
 * INPUT       : type is the attribute type.
 *             : atr is the change attribute value.
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_change_req(
    			 util_uint8_t 		     **pp_msg_buffer, 
    			 util_uint16_t 	             type, 
    			 arStun_attr_change_req_t     atr)
{
	/*Length of data encoded*/
	util_uint32_t    length = UTIL_INIT_VAL;

    util_uint8_t 	    *p_msg_buffer = *pp_msg_buffer;

	/*Encode the value of type*/
	arStun_encode_U16(p_msg_buffer , type, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of length*/
	arStun_encode_U16(p_msg_buffer  , ARSTUN_ATTRIB_CHANGE_REQ_LEN, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of 4 byte value */
	arStun_encode_U32(p_msg_buffer, atr, &length);

	/*Increment the ptr by value of length*/
	p_msg_buffer  += length;
    *pp_msg_buffer = p_msg_buffer;

	return;

}

/****************************************************************************
 * API Name: arStun_encode_string
 * 
 * Description:    This function shall be  used to encode string attribute type 
 * INPUT       : type is the attribute type.
 *             : str is the string attribute 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_string(
		util_uint8_t 		  **pp_msg_buffer, 
		util_uint16_t 	          type, 
		arStun_string_st              str)
{
	/*Length of data encoded*/
	util_uint32_t    length = UTIL_INIT_VAL;
 
	/*count for string length*/	
	util_uint32_t    count = UTIL_INIT_VAL;

	/*Number of space */	
	util_uint32_t    num_of_space = UTIL_INIT_VAL;


	util_uint8_t 	    *p_msg_buffer = *pp_msg_buffer;


	/*Encode the value of type*/
	arStun_encode_U16(p_msg_buffer, type, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/
	length = UTIL_INIT_VAL;
	
	num_of_space= str.str_len  % 4;

	if(UTIL_INIT_VAL != num_of_space)
	{
        ARSTUN_PRINT(("\n Length of Attribute is not multiple of 4. Appending spaces"));
		num_of_space = 4 - num_of_space;
	}
	

	/*encode the value of string length*/
	arStun_encode_U16(p_msg_buffer  , str.str_len + num_of_space, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;
     
        /* added to remove klockworks warning*/ 
	if(ARSTUN_MAX_STR_LEN<=str.str_len)
	{
		str.str_len=ARSTUN_MAX_STR_LEN;
	}

	for(count=0; count < str.str_len; count ++)
	{
		/*Setting the value of length as 0*/                  
		length = UTIL_INIT_VAL;

		/*encode the value of string */
		arStun_encode_U8(p_msg_buffer, str.str_val[count], &length);

		/*Increment the p_msg_buffer by value of length*/
		p_msg_buffer  += length;
	}
        
        /*encoding number of spaces*/
	for(count=0; count < num_of_space; count ++)
	{
		/*Setting the value of length as 0*/                  
		length = UTIL_INIT_VAL;

		/*encode the value of string */
		arStun_encode_U8(p_msg_buffer, 0x20 ,&length);

		/*Increment the p_msg_buffer by value of length*/
		p_msg_buffer  += length;
	}


    *pp_msg_buffer = p_msg_buffer;
	return;
}

/****************************************************************************
 * API Name: arStun_encode_unknown_attr
 * 
 * Description:    This function shall be  used to encode unknown attribute type 
 * INPUT       : type is the attribute type.
 *             : str is the unknown attribute 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_unknown_attr (
		util_uint8_t 		  **pp_msg_buffer, 
		util_uint16_t 	          type, 
		arStun_attr_unknown_st    attr)
{
	/*Length of data encoded*/
	util_uint32_t    length = UTIL_INIT_VAL;

        /*count for number of attributes */	
        util_uint32_t    count = UTIL_INIT_VAL;
    util_uint8_t 	    *p_msg_buffer = *pp_msg_buffer;


	/*Encode the value of type*/
	arStun_encode_U16(p_msg_buffer, type, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

    if(0 == attr.num_attributes % 2)
    {
       /* even number of attributes */
	   arStun_encode_U16(p_msg_buffer, 2 * attr.num_attributes, &length);
    }
    else
    {
        /* odd number of attributes */
	    arStun_encode_U16(p_msg_buffer, 2 + 2 * attr.num_attributes, &length);
    }

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;
      
        /*added to remove klockwork warning*/
	if(ARSTUN_MAX_UNKNOWN_ATTRIBUTES<=attr.num_attributes)
	{
		attr.num_attributes=ARSTUN_MAX_UNKNOWN_ATTRIBUTES;
	}

	for (count =0; count <attr.num_attributes; count++)
	{
		/*Setting the value of length as 0*/                  
		length = UTIL_INIT_VAL;

		arStun_encode_U16 (p_msg_buffer,  attr.attr_type[count], &length);
		p_msg_buffer  += length;
	}

    if(0 != attr.num_attributes % 2)
    {
       /* even number of attributes */
		arStun_encode_U16 (p_msg_buffer,  attr.attr_type[count -1], &length);
		p_msg_buffer  += length;
    }

    *pp_msg_buffer = p_msg_buffer;
	return;

}


/****************************************************************************
 * API Name: arStun_encode_error_attr
 * 
 * Description: This function shall be  used to encode error attribute type 
 * INPUT       : type is the attribute type.
 *             : atr is the error attribute 
 * OUTPUT      :p_msg_buffer  buffer conatining encoded error attribute. 
 *
 * Return :
 ******************************************************************************/

util_void_t  
arStun_encode_error_attr(
    			util_uint8_t 		  **pp_msg_buffer, 
    			util_uint16_t 	          type, 
    			arStun_attr_error_st      attr)
{
	/*Length of data encoded*/
	util_uint32_t    length = UTIL_INIT_VAL;

	/*count for error string */	
	util_uint32_t    count = UTIL_INIT_VAL;
  
	/*Number of space */	
	util_uint32_t    num_of_space = UTIL_INIT_VAL;

    util_uint8_t 	    *p_msg_buffer = *pp_msg_buffer;


	/*Encode the value of type*/
	arStun_encode_U16(p_msg_buffer , type, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	num_of_space= attr.reason.str_len % 4;
     
	if(UTIL_INIT_VAL != num_of_space)
	{
		num_of_space = 4 - num_of_space;
	}


	/*encode the value of length*/
	arStun_encode_U16(p_msg_buffer  ,4 + attr.reason.str_len + num_of_space, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of pad*/
	arStun_encode_U16(p_msg_buffer  , attr. padding, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of error class*/
	arStun_encode_U8(p_msg_buffer  , attr. error_class, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;

	/*Setting the value of length as 0*/                  
	length = UTIL_INIT_VAL;

	/*encode the value of error number*/
	arStun_encode_U8(p_msg_buffer  , attr. number, &length);

	/*Increment the p_msg_buffer by value of length*/
	p_msg_buffer  += length;
       
       /*added to remove klockwork warning*/
	if(ARSTUN_MAX_STR_LEN<=attr. reason.str_len) 
	{
		attr. reason.str_len=ARSTUN_MAX_STR_LEN;
	}

	/*encode the error string*/
	for(count=0; count < attr. reason.str_len; count ++)
	{
		/*Setting the value of length as 0*/                  
		length = UTIL_INIT_VAL;

		/*encode the value of string */
		arStun_encode_U8(p_msg_buffer, attr.reason.str_val [count], &length);

		/*Increment the p_msg_buffer by value of length*/
		p_msg_buffer  += length;
	}
    
	/*encoding number of spaces*/
	for(count=0; count < num_of_space; count ++)
	{
		/*Setting the value of length as 0*/                  
		length = UTIL_INIT_VAL;

		/*encode the value of string */
		arStun_encode_U8(p_msg_buffer, 0x20 ,&length);

		/*Increment the p_msg_buffer by value of length*/
		p_msg_buffer  += length;
	}

    *pp_msg_buffer = p_msg_buffer;
	return;

}

/****************************************************************************
 * API Name: arStun_generate_transactionID
 * 
 * Description:    This function shall be  used to generate transaction id 
 * INPUT       : .
 * OUTPUT      :pTransactionID  is transaction id. 
 *
 * Return : It will return UTIL_SUCCESS if successful else return UTIL_FAILURE.
 ******************************************************************************/

util_void_t
arStun_generate_transactionID (
		INOUT arStun_uint128_t      *pTransactionID)
{
	/*count for transaction id */	
	util_uint32_t    count = UTIL_INIT_VAL;

	/*Random number for transaction id*/	
	util_uint32_t    rand_num = UTIL_INIT_VAL;


	for ( count=0; count < 4; count++)
	{
		/*Genearte a randon number*/
		rand_num = util_port_random();

		pTransactionID->octet[4* count+0] =( rand_num >>0);
		pTransactionID->octet[4* count+1] =( rand_num >>8);
		pTransactionID->octet[4* count+2] =( rand_num >>16);
		pTransactionID->octet[4* count+3] =( rand_num >>24);
	}
}


