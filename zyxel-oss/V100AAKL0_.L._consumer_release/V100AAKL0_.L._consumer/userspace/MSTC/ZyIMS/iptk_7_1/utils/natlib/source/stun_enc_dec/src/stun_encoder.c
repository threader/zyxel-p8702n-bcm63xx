/******************************************************************************
 * 
 * FILE NAME:   stun_encoder.c
 * 
 * DESCRIPTION: This file contains the encoding routines of STUN Messages
 *              
 * REVISION HISTORY:    
 *        Date            Name                Reference       Comments
 *	10-July-2006	Anuradha Gupta		    HLD	            Initial version
 *  21-Nov-2006     Anuradha Gupta          HLD             Coding
 ******************************************************************************/ 
 
#include "stun_enc_dec_types.h"
#include "nat_port_pltfm_prototypes.h"
#include "stun_enc_dec_prototypes.h"
#include "stun_enc_dec_sha.h"
#include "nat_types.h"

#define SHAInit		SHA1Reset
#define SHAUpdate	SHA1Input
#define SHAFinal	SHA1Result 
#define SHA_CTX		SHA1Context

#ifndef SHA_DIGESTSIZE
#define SHA_DIGESTSIZE  40
#endif

#ifndef SHA_BLOCKSIZE
#define SHA_BLOCKSIZE   64
#endif

#ifndef MD5_DIGESTSIZE
#define MD5_DIGESTSIZE  16
#endif

#ifndef MD5_BLOCKSIZE
#define MD5_BLOCKSIZE   64
#endif

static nat_int8_t* 
encode16(nat_int8_t* buf, nat_uint16_t data)
{
   nat_uint16_t ndata = nat_port_htons(data);
   nat_port_memcpy(buf, (nat_void_t *)(&ndata), nat_port_sizeof(nat_uint16_t));
   return buf + nat_port_sizeof(nat_uint16_t);
}

static nat_int8_t* 
encode32(nat_int8_t* buf, nat_uint32_t data)
{
   nat_uint32_t ndata = nat_port_htonl(data);
   nat_port_memcpy(buf, (nat_void_t *)(&ndata), nat_port_sizeof(nat_uint32_t));
   return buf + nat_port_sizeof(nat_uint32_t);
}


static nat_int8_t* 
encode(nat_int8_t* buf, const nat_int8_t* data, nat_uint32_t length)
{
   nat_port_memcpy((nat_void_t *)buf, (nat_void_t *)data, length);
   return buf + length;
}



static nat_int8_t* 
encodeAtrAddress4(nat_int8_t* ptr, nat_uint16_t type, const stun_attr_address_st atr)
{
   nat_uint32_t ip_data;
   ptr = encode16(ptr, type);
   ptr = encode16(ptr, 8);
   *ptr++ = atr.pad;
   *ptr++ = IPV4FAMILY;
   ptr = encode16(ptr, atr.addr.port);
   stun_convert_ipaddr_to_data(atr.addr.ipv4_addr,&ip_data);
   ptr = encode32(ptr, ip_data);
	
   return ptr;
}

static nat_int8_t* 
encodeAtrChangeRequest(nat_int8_t* ptr, const stun_attr_change_req_st atr)
{
   ptr = encode16(ptr, CHANGE_REQUEST);
   ptr = encode16(ptr, 4);
   ptr = encode32(ptr, atr.value);
   return ptr;
}

static nat_int8_t* 
encodeAtrError(nat_int8_t* ptr, const stun_attr_error_st atr)
{
   ptr = encode16(ptr, ERROR_CODE);
   ptr = (nat_int8_t *)encode16(ptr, (nat_uint16_t )(4 + atr.reason.str_len));
   ptr = encode16(ptr, atr.pad);
   *ptr++ = atr.error_class;
   *ptr++ = atr.number;
   ptr = encode(ptr, (const nat_int8_t *)atr.reason.str, atr.reason.str_len);
   return ptr;
}


static nat_int8_t* 
encodeAtrUnknown(nat_int8_t* ptr, const stun_attr_unknown_st atr)
{
   nat_uint8_t i =0;
   ptr = encode16(ptr, UNKNOWN_ATTRIBUTE);
   ptr = (nat_int8_t *)encode16(ptr, (nat_uint16_t)(2+2*atr.num_attributes));
   for (i=0; i<atr.num_attributes; i++)
   {
      ptr = encode16(ptr, atr.attr_type[i]);
   }
   return ptr;
}


static nat_int8_t* 
encodeXorOnly(nat_int8_t* ptr)
{
   ptr = encode16(ptr, XOR_ONLY );
   return ptr;
}


static nat_int8_t* 
encodeAtrString(nat_int8_t* ptr, nat_uint16_t type, const nat_string_st atr)
{
   nat_port_assert(atr.str_len % 4 == 0);
    
   ptr = encode16(ptr, type);
   ptr = encode16(ptr, atr.str_len);
   ptr = encode(ptr, (const nat_int8_t *)atr.str, atr.str_len);
   return ptr;
}


static nat_int8_t* 
encodeAtrIntegrity(nat_int8_t* ptr, const stun_attr_integrity_st atr)
{
   ptr = encode16(ptr, MESSAGE_INTEGRITY);
   ptr = encode16(ptr, 20);
   ptr = encode(ptr, (const nat_int8_t *) atr.hash, nat_port_sizeof(atr.hash)-1);//himanshu
   return ptr;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_convert_ipaddr_to_data
 *
 * DESCRIPTION:     This function converts IPV4 address to packed uint32 data
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE              
 *
 ******************************************************************************/
nat_return_t stun_convert_ipaddr_to_data(
             const nat_ipv4_address_st ip_addr, nat_uint32_t *p_data)
{
    *p_data    = 0;

    *p_data    |= (nat_uint32_t)ip_addr.octet_1 << 24;
    *p_data    |= (nat_uint32_t)ip_addr.octet_2 << 16;
    *p_data    |= (nat_uint32_t)ip_addr.octet_3 << 8;
    *p_data    |= (nat_uint32_t)ip_addr.octet_4;

	return NAT_RETURN_SUCCESS;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_encode_message
 *
 * DESCRIPTION:     This function encodes the stun message
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_encode_message( stun_message_st 		stun_message, 
                     nat_octet_string_st	*p_buffer, 
                     const nat_string_st	password,
			         nat_error_t		    error,
		     nat_uint8_t		*p_msg_integrity
			        )
{

    nat_return_t    ret_val = NAT_RETURN_SUCCESS;
	
    nat_int8_t* lengthp = NAT_NULL;

    nat_int8_t* ptr = (nat_int8_t *)p_buffer->str;
    nat_int8_t* pBuff = (nat_int8_t *)p_buffer->str;


    stun_attr_integrity_st integrity;
		error = NAT_NULL;
/*
    if(p_buffer->octet_len >= nat_port_sizeof(stun_msg_hdr))
       return NAT_RETURN_FAILURE;
  */  
    ptr = encode16(ptr, stun_message.msg_hdr.msg_type);
    lengthp = ptr;
    ptr = encode16(ptr, 0);
    ptr = encode(ptr, (nat_int8_t*)(stun_message.msg_hdr.transaction_id.octet), 
        nat_port_sizeof(stun_message.msg_hdr.transaction_id));
    
    
    
    if (NAT_TRUE == stun_message.hasMappedAddress)
    {
        ptr = encodeAtrAddress4 (ptr, MAPPED_ADDRESS, stun_message.mapped_address);
    }
    
    if (NAT_TRUE == stun_message.hasResponseAddress)
    {
        
        ptr = encodeAtrAddress4(ptr, RESPONSE_ADDRESS, stun_message.response_address);
    }
    if (NAT_TRUE == stun_message.hasChangeRequest)
    {
        
        ptr = encodeAtrChangeRequest(ptr, stun_message.change_request);
    }
    if (NAT_TRUE == stun_message.hasSourceAddress)
    {
        
        ptr = encodeAtrAddress4(ptr, SOURCE_ADDRESS, stun_message.source_address);
    }
    if (NAT_TRUE == stun_message.hasChangedAddress)
    {
        
        ptr = encodeAtrAddress4(ptr, CHANGED_ADDRESS, stun_message.changed_address);
    }
    if (NAT_TRUE == stun_message.hasUsername)
    {
        
        ptr = encodeAtrString(ptr, USERNAME, stun_message.username);
    }
    if (NAT_TRUE == stun_message.hasPassword)
    {
        
        ptr = encodeAtrString(ptr, PASSWORD, stun_message.password);
    }
    if (NAT_TRUE == stun_message.hasErrorCode)
    {
      		
        ptr = encodeAtrError(ptr, stun_message.error_code);
    }
    if (NAT_TRUE == stun_message.hasUnknownAttributes)
    {
        
        ptr = encodeAtrUnknown(ptr, stun_message.unknown_attributes);
    }
    if (NAT_TRUE == stun_message.hasReflectedFrom)
    {
        
        ptr = encodeAtrAddress4(ptr, REFLECTED_FROM, stun_message.reflected_from);
    }
    if (NAT_TRUE == stun_message.hasXorMappedAddress)
    {
        
        ptr = encodeAtrAddress4 (ptr, XOR_MAPPED_ADDRESS, stun_message.xor_mapped_address);
    }
    if (NAT_TRUE == stun_message.xorOnly)
    {
        
        ptr = encodeXorOnly( ptr );
    }
    if (NAT_TRUE == stun_message.hasServerName)
    {
        
        ptr = encodeAtrString(ptr, SERVER_NAME, stun_message.server_name);
    }
    if (NAT_TRUE == stun_message.hasSecondaryAddress)
    {
        ptr = encodeAtrAddress4 (ptr, SECONDARY_ADDRESS, stun_message.secondary_address);
    }
    
    if (password.str_len > 0) //himanshu
    {
        
       nat_uint8_t hash[50];
        
        stun_computeHmac((nat_uint8_t *)&hash[0], (const nat_uint8_t *)pBuff, 
            (nat_uint32_t)(ptr-pBuff), 
            (const nat_uint8_t *)password.str,(nat_uint8_t)password.str_len);

		nat_port_memcpy(&integrity.hash[0],&hash[0],
		(NAT_STUN_HMAC_DIGEST_LENGTH * nat_port_sizeof(nat_uint8_t))); //Himanshu

		integrity.hash[NAT_STUN_HMAC_DIGEST_LENGTH]='\0';

       
	if(NAT_NULL != p_msg_integrity)
	{ 
        	nat_port_strcpy( p_msg_integrity,integrity.hash);
	}

        ptr = encodeAtrIntegrity(ptr, integrity);
    }
    
    
    encode16(lengthp, (nat_uint16_t)(ptr - pBuff - nat_port_sizeof(stun_msg_hdr)));
 
	p_buffer->octet_len = (nat_uint16_t)(ptr - pBuff);
   
    return ret_val;
}




/*******************************************************************************
 *
 * FUNCTION:        stun_create_bind_req
 *
 * DESCRIPTION:     This function creates binding request message in encode
 *                  routine format
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_create_bind_req(
                     stun_message_st 		*p_stun_message,
	                 nat_string_st	username,
                     nat_boolean_t			change_port,
			         nat_boolean_t			change_ip,
			         nat_ip_port_address_st	*p_response_addr
			   )
{
    nat_return_t    ret_val = NAT_RETURN_SUCCESS;
    nat_uint8_t     count = 0;
    nat_uint32_t    rand = 0;
    
    nat_port_memset(p_stun_message,0,nat_port_sizeof(stun_message_st));
    
    /* fill the message header */
    p_stun_message->msg_hdr.msg_type = BindRequestMsg;
    p_stun_message->msg_hdr.msg_length = 0;
    
    for ( count=0; count<16; count=(nat_uint8_t)(count+4 ))
    {
        nat_port_assert(count+3<16);
        rand = stunRand();
        p_stun_message->msg_hdr.transaction_id.octet[count+0]= (nat_uint8_t)(rand>>0);
        p_stun_message->msg_hdr.transaction_id.octet[count+1]= (nat_uint8_t)(rand>>8);
        p_stun_message->msg_hdr.transaction_id.octet[count+2]= (nat_uint8_t)(rand>>16);
        p_stun_message->msg_hdr.transaction_id.octet[count+3]= (nat_uint8_t)(rand>>24);
    }
    
    /* fill the message */
    if(NAT_TRUE == change_ip || NAT_TRUE == change_port)
    {
        p_stun_message->hasChangeRequest = NAT_TRUE;
        p_stun_message->change_request.value =(change_ip?CHANGE_IPFLAG:0) | 
            (change_port?CHANGE_PORTFLAG:0);
    }
    
    if ( username.str_len > 0 )
    {
        p_stun_message->hasUsername = NAT_TRUE;
		nat_port_memcpy(&p_stun_message->username,&username,
			nat_port_sizeof(nat_string_st));
    }

    if ( NAT_NULL != p_response_addr->port )
    {
        p_stun_message->hasResponseAddress = NAT_TRUE;
		nat_port_memcpy(&p_stun_message->response_address.addr,
			p_response_addr,
			nat_port_sizeof(nat_ip_port_address_st));
    }

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_create_shsec_req
 *
 * DESCRIPTION:     This function creates shared secret request message in 
 *                  encode routine format
 *                  
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE                
 *
 ******************************************************************************/
nat_return_t
stun_create_shsec_req( stun_message_st 		*p_stun_message)
{
    nat_return_t    ret_val = NAT_RETURN_SUCCESS;

#ifdef NAT_PORT_WINDOWS

    p_stun_message = p_stun_message;
#endif

    return ret_val;
}

/*******************************************************************************
 *
 * FUNCTION:        stunRand
 *
 * DESCRIPTION:     This function is for generating a random number.
 *                  
 * RETURNS:         The generated random number               
 *
 ******************************************************************************/
nat_uint32_t stunRand()
{

   // return 32 bits of random stuff
#if defined(NAT_PORT_WINDOWS) || defined(NAT_PORT_WINCE)
	nat_uint32_t r1;
	nat_uint32_t r2;
	nat_uint32_t ret;
   nat_port_assert( RAND_MAX == 0x7fff );
   r1 = rand();
   r2 = rand();
	
   ret = (r1<<16) + r2;
	
   return ret;
#else
/*   time_t seed;
   time(&seed);
   srandom(seed);
   return seed; */
   nat_uint32_t seed = random();
   return seed;
#endif
}

/*******************************************************************************
 *
 * FUNCTION:        stun_computeHmac
 *
 * DESCRIPTION:     This function is for computing HMAC.
 *                  
 * RETURNS:         nothing               
 *
 ******************************************************************************/
nat_void_t
stun_computeHmac(nat_uint8_t* hmac, 
                 const nat_uint8_t* input, 
                 nat_uint32_t length, 
                 const nat_uint8_t* key, 
                 nat_uint8_t sizeKey)
{
    SHA_CTX *p_ictx, *p_octx ;
    nat_uint8_t    isha[SHA_DIGESTSIZE +1];
    nat_uint8_t    buf[SHA_BLOCKSIZE + 1] ;
    nat_uint8_t		temp[1000];
    nat_uint8_t     i ;
    nat_error_t    ecode = NAT_NULL;
   
    p_ictx = (SHA_CTX *)nat_port_static_memget(nat_port_sizeof(SHA_CTX),&ecode);
    p_octx = (SHA_CTX *)nat_port_static_memget(nat_port_sizeof(SHA_CTX),&ecode);

    /**** Inner Digest ****/
    
    SHAInit(p_ictx) ;
    
    /* Pad the key for inner digest */
    for (i = 0 ; i < sizeKey ;++i) buf[i] = (nat_uint8_t)(key[i] ^ 0x36);
    for (i = sizeKey ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x36 ;
    
    buf[SHA_BLOCKSIZE] = '\0'; 
    nat_port_strcpy(temp,buf);
    nat_port_strcat(temp,(nat_uint8_t *)input);
    
    temp[SHA_BLOCKSIZE + length]='\0';
    
    SHAUpdate(p_ictx, temp, SHA_BLOCKSIZE + sizeKey) ;
    
    
    if (!SHAFinal(p_ictx))
    {
        fprintf(stderr, "ERROR-- could not compute message digest\n");
    }
    
    /**** Outter Digest ****/
    
    SHAInit(p_octx) ;
    
    /* Pad the key for outter digest */
    
    for (i = 0 ; i < sizeKey ;++i) buf[i] = (nat_uint8_t)(key[i] ^ 0x5C);
    for (i = sizeKey ; i < SHA_BLOCKSIZE ; ++i) buf[i] = 0x5C ;
    
    
    nat_port_sprintf((nat_int8_t *)isha,"%08x%08x%08x%08x%08x",p_ictx->Message_Digest[0],
        p_ictx->Message_Digest[1],p_ictx->Message_Digest[2],
        p_ictx->Message_Digest[3],p_ictx->Message_Digest[4]);
    
    
    nat_port_strcpy(temp,buf);
    nat_port_strcat(temp,isha);
    
    temp[SHA_BLOCKSIZE + SHA_DIGESTSIZE]='\0';
    
    SHAUpdate(p_ictx, temp, SHA_BLOCKSIZE + SHA_DIGESTSIZE) ;
    
    
    if (!SHAFinal(p_octx))
    {
        fprintf(stderr, "ERROR-- could not compute message digest\n");
    }

    /*
    printf("\t");
    for(i = 0; i < 5 ; i++)
    {
        printf("%X ", octx.Message_Digest[i]);
    }
    printf("\n");
    */

    nat_port_sprintf((nat_int8_t *)hmac,"%4x%4x%4x%4x%4x",p_octx->Message_Digest[0],
        p_octx->Message_Digest[1],p_octx->Message_Digest[2],
        p_octx->Message_Digest[3],p_octx->Message_Digest[4]);
    
    nat_port_static_memfree(p_ictx,&ecode);
    nat_port_static_memfree(p_octx,&ecode);

    return;
}

/*******************************************************************************
 *
 * FUNCTION:        stun_create_binding_resp
 *
 * DESCRIPTION:     This function creates binding response message in encode
 *                  routine format
 *
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE
 *
 ******************************************************************************/
nat_return_t
stun_create_binding_resp(nat_octet_string_st   **p_buffer,
                         nat_uint128_t         transaction_id,
                         stun_attr_address_st  *changed_address,
                         stun_attr_address_st  *source_address,
                         stun_attr_address_st  *mapped_address,
                         stun_attr_address_st  *reflected_from,
                         nat_string_st         password)
{
    stun_message_st         stun_message;
    nat_error_t             ecode = 0;
    nat_return_t        ret_val = NAT_RETURN_SUCCESS;

    nat_port_memset(&stun_message,0,nat_port_sizeof(stun_message_st));

    /* fill the message header */
    stun_message.msg_hdr.msg_type = BindResponseMsg;
    stun_message.msg_hdr.msg_length = 0;
        stun_message.msg_hdr.transaction_id = transaction_id;

    if (changed_address)
    { 
        stun_message.hasChangedAddress = NAT_TRUE;
        stun_message.changed_address = *changed_address;
    }
    else
    {
        stun_message.hasChangedAddress = NAT_FALSE;
    }
   
    if (source_address)
    { 
        /* Source address is also mandatory parameter in the binding response.*/
        stun_message.hasSourceAddress = NAT_TRUE;
        stun_message.source_address = *source_address;
    }
    else
    {
        stun_message.hasSourceAddress = NAT_FALSE;
    }

    if (mapped_address)
    {
        /* Mapped address is also mandatory parameter in the binding response.*/
        stun_message.hasMappedAddress = NAT_TRUE;
        stun_message.mapped_address = *mapped_address;
    }
    else
    {
        stun_message.hasMappedAddress = NAT_FALSE;
    } 

    if (reflected_from)
    {
        stun_message.hasReflectedFrom = NAT_TRUE;
        stun_message.reflected_from = *reflected_from;
    }
    else
    {
        stun_message.hasReflectedFrom = NAT_FALSE;
    }

    *p_buffer = (nat_octet_string_st *)nat_port_static_memget(
                          nat_port_sizeof(nat_octet_string_st),&ecode);
    /* Function call below will encode the stun message. */
    ret_val =  stun_encode_message(stun_message,*p_buffer,password,ecode,NAT_NULL);
  
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        stun_create_binding_err_resp
 *
 * DESCRIPTION:     This function creates binding error response message in encode
 *                  routine format
 *
 * RETURNS:         NAT_RETURN_SUCCESS / NAT_RETURN_FAILURE
 *
 ******************************************************************************/
nat_return_t
stun_create_binding_err_resp(nat_octet_string_st  **p_buffer,
                             nat_uint128_t         transaction_id,
                             stun_attr_error_st    *error_code,
                         stun_attr_unknown_st  *unknown_attributes,
                             nat_string_st         password)
{
	stun_message_st            stun_message;
           nat_error_t                ecode = 0;
	nat_return_t        ret_val = NAT_RETURN_SUCCESS;
#ifdef NAT_PORT_WINDOWS
	unknown_attributes = unknown_attributes;
#endif 

    	nat_port_memset(&stun_message,0,nat_port_sizeof(stun_message_st));

           /* fill the message header */
       stun_message.msg_hdr.msg_type = BindErrorResponseMsg;
       stun_message.msg_hdr.msg_length = 0;
           stun_message.msg_hdr.transaction_id = transaction_id;

           /* Error code a mandatory parameter in the binding error response.*/
           if (error_code)
           {
                   stun_message.hasErrorCode = NAT_TRUE;
           stun_message.error_code = *error_code;
           }
           else
                   return NAT_RETURN_FAILURE;

        *p_buffer = (nat_octet_string_st *)nat_port_static_memget(
                          nat_port_sizeof(nat_octet_string_st),&ecode);

           /* Function call below will encode the stun message. */
        ret_val = stun_encode_message(stun_message,*p_buffer,password,ecode,NAT_NULL);

	return ret_val;
}


