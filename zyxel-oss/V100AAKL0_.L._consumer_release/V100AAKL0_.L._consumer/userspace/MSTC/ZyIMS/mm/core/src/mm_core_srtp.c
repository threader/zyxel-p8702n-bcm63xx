#include"mm_core_fsm_types.h"
#include"mm_macros.h"
#include "mm_glb.h"
#include"mm_port_wrapper.h"

#include "global.h"
#include "switch.h"
#include "srtp.h"
#include "crypto.h"

#define BUF_SIZE 1024
#define MAX_BASE64_STR_SIZE 256
#define MAX_KEY_LENGTH 60
#define SDP_STR_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_80 "AES_CM_128_HMAC_SHA1_80"
#define SDP_STR_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_32 "AES_CM_128_HMAC_SHA1_32"
#define SDP_STR_CRYPTO_SUITE_AES_CM_128_NULL "AES_CM_128_NULL"
#define SDP_STR_CRYPTO_SUITE_NULL_CIPHER_HMAC_SHA1_80 "NULL_CIPHER_HMAC_SHA1_80"

typedef enum{
	SRTP_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_80 = 0,
	SRTP_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_32,
	SRTP_CRYPTO_SUITE_AES_CM_128_NULL,
	SRTP_CRYPTO_SUITE_NULL_CIPHER_HMAC_SHA1_80
}SRTP_CRYPTO_SUITE_TYPE_t;

const char * srtpSupportedCryptoSuiteList[] = {
	{SDP_STR_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_80}, /*default*/
	{SDP_STR_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_32},
	{SDP_STR_CRYPTO_SUITE_AES_CM_128_NULL},
	{SDP_STR_CRYPTO_SUITE_NULL_CIPHER_HMAC_SHA1_80},
	NULL /*end*/
};


srtpInit(void){
	int ret = 0;
	ret = srtp_init();
	ZyPrintf("%s....ret = %d\n",__FUNCTION__, ret);
}

static const unsigned char map[256] = {
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
 52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
  7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
 19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
 37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
 49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
255, 255, 255, 255 };

/**
   base64 decode a block of memory
   @param in       The base64 data to decode
   @param inlen    The length of the base64 data
   @param out      [out] The destination of the binary decoded data
   @param outlen   [in/out] The max size and resulting size of the decoded data
   @return CRYPT_OK if successful
*/
int base64Decode(const unsigned char *in,  unsigned long inlen, 
                        unsigned char *out, unsigned long *outlen)
{
   unsigned long t, x, y, z;
   unsigned char c;
   int           g;

   if( in == NULL || out == NULL || outlen == NULL){
   		ZyPrintf("%s, argument error\n",__FUNCTION__);
   		return -1;
   }

   g = 3;
   for (x = y = z = t = 0; x < inlen; x++) {
       c = map[in[x]&0xFF];
       if (c == 255) continue;
       /* the final = symbols are read and used to trim the remaining bytes */
       if (c == 254) { 
          c = 0; 
          /* prevent g < 0 which would potentially allow an overflow later */
          if (--g < 0) {
             return -1;//CRYPT_INVALID_PACKET;
          }
       } else if (g != 3) {
          /* we only allow = to be at the end */
          return -1;//CRYPT_INVALID_PACKET;
       }

       t = (t<<6)|c;

       if (++y == 4) {
          if (z + g > *outlen) { 
             return -1;//CRYPT_BUFFER_OVERFLOW; 
          }
          out[z++] = (unsigned char)((t>>16)&255);
          if (g > 1) out[z++] = (unsigned char)((t>>8)&255);
          if (g > 2) out[z++] = (unsigned char)(t&255);
          y = t = 0;
       }
   }
   if (y != 0) {
       return -1;//CRYPT_INVALID_PACKET;
   }
   *outlen = z;
   ZyPrintf("outlen = %d\n",z);
   ZyPrintf("%s, outlen addr = 0x%x\n", __FUNCTION__, outlen);
   return 0;//CRYPT_OK;
}

static const char *codes =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64Encode(const unsigned char *in,  unsigned long len,
                        unsigned char *out, unsigned long *outlen)
{
   unsigned long i, len2, leven;
   unsigned char *p;

   //_ARGCHK(in     != NULL);
   //_ARGCHK(out    != NULL);
   //_ARGCHK(outlen != NULL);

   /* valid output size ? */
   len2 = 4 * ((len + 2) / 3);
   if (*outlen < len2 + 1) {
      return -1;//CRYPT_BUFFER_OVERFLOW;
   }
   p = out;
   leven = 3*(len / 3);
   for (i = 0; i < leven; i += 3) {
       *p++ = codes[(in[0] >> 2) & 0x3F];
       *p++ = codes[(((in[0] & 3) << 4) + (in[1] >> 4)) & 0x3F];
       *p++ = codes[(((in[1] & 0xf) << 2) + (in[2] >> 6)) & 0x3F];
       *p++ = codes[in[2] & 0x3F];
       in += 3;
   }
   /* Pad it if necessary...  */
   if (i < len) {
       unsigned a = in[0];
       unsigned b = (i+1 < len) ? in[1] : 0;

       *p++ = codes[(a >> 2) & 0x3F];
       *p++ = codes[(((a & 3) << 4) + (b >> 4)) & 0x3F];
       *p++ = (i+1 < len) ? codes[(((b & 0xf) << 2)) & 0x3F] : '=';
       *p++ = '=';
   }

   /* append a NULL byte */
   *p = '\0';

   /* return ok */
   *outlen = p - out;
   return 0;//CRYPT_OK;
}

int srtpGetCryptoSuiteType(char *cryptoSuite){
	char *iter = NULL;
	int i=0;
	RTP_PRINTF("entering %s\n",__FUNCTION__);

	if(NULL == cryptoSuite){
		ZyPrintf("%s cryptoSuite NULL\n",__FUNCTION__);
		return MM_FAILURE;
	}	

	for(i=0;srtpSupportedCryptoSuiteList[i] != NULL; i++){
		ZyPrintf("%d => %s\n", i, srtpSupportedCryptoSuiteList[i]);
		if(!strncmp(srtpSupportedCryptoSuiteList[i], cryptoSuite, strlen(srtpSupportedCryptoSuiteList[i]))){
			ZyPrintf("crypto suite:%s supported", cryptoSuite);
			break;
		}
	}
	
	if(srtpSupportedCryptoSuiteList[i] == NULL){
		ZyPrintf("crypto %s not supported, return NULL\n", cryptoSuite);
		return -1;
	}

	return i;

}

err_status_t
srtp_session_print_policy(srtp_t srtp) {
  char *serv_descr[4] = {
    "none",
    "confidentiality",
    "authentication",
    "confidentiality and authentication"
  };
  char *direction[3] = {
    "unknown",
    "outbound",
    "inbound"
  };
  srtp_stream_t stream;

  /* sanity checking */
  if (srtp == NULL)
    return err_status_fail;

  /* if there's a template stream, print it out */
  if (srtp->stream_template != NULL) {
    stream = srtp->stream_template;
    RTP_PRINTF("# SSRC:          any %s\r\n"
	   "# rtp cipher:    %s\r\n"
	   "# rtp auth:      %s\r\n"
	   "# rtp services:  %s\r\n" 
           "# rtcp cipher:   %s\r\n"
	   "# rtcp auth:     %s\r\n"
	   "# rtcp services: %s\r\n",
	   direction[stream->direction],
	   stream->rtp_cipher->type->description,
	   stream->rtp_auth->type->description,
	   serv_descr[stream->rtp_services],
	   stream->rtcp_cipher->type->description,
	   stream->rtcp_auth->type->description,
	   serv_descr[stream->rtcp_services]);
  }

  /* loop over streams in session, printing the policy of each */
  stream = srtp->stream_list;
  while (stream != NULL) {
    if (stream->rtp_services > sec_serv_conf_and_auth)
      return err_status_bad_param;
    
    RTP_PRINTF("# SSRC:          0x%08x\r\n"
	   "# rtp cipher:    %s\r\n"
	   "# rtp auth:      %s\r\n"
	   "# rtp services:  %s\r\n" 
           "# rtcp cipher:   %s\r\n"
	   "# rtcp auth:     %s\r\n"
	   "# rtcp services: %s\r\n",
	   stream->ssrc,
	   stream->rtp_cipher->type->description,
	   stream->rtp_auth->type->description,
	   serv_descr[stream->rtp_services],
	   stream->rtcp_cipher->type->description,
	   stream->rtcp_auth->type->description,
	   serv_descr[stream->rtcp_services]);

    /* advance to next stream in the list */
    stream = stream->next;
  } 
  return err_status_ok;
}

/*************************************************************************
 *
 *  FUNCTION NAME  : srtpGetCryptoAttributes
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : return key if success, or NULL on failed
 *************************************************************************/
mm_return_t srtpGetCryptoAttributes(
	icf_list_st			*p_input_enc_attri_list, /*input is a list of icf_string_st*/
	int *cryptoSuiteType, /*output*/
	uint8_t 			*key, /*output*/
	uint32_t			*keylen
){
	icf_list_st		*p_enc_attrib_list = MM_NULL;
	icf_string_st	*p_string = MM_NULL;	
	char tag[10]={0}, cryptoSuite[50]={0};
	char keyStr[MAX_KEY_LENGTH] = {0};
	int matchNum = 0;
	char *iter = NULL;
	//int cryptoSuiteType = 0;	
	int i=0;
	p_enc_attrib_list = p_input_enc_attri_list;	
	while(MM_NULL != p_enc_attrib_list)
	{		
		p_string = p_enc_attrib_list->p_data;
		MM_PRINTF("attribute >> %s\n", p_string->str);		
		matchNum = sscanf(p_string->str, "crypto:%s %s inline:%s", tag, cryptoSuite, keyStr);
		if(matchNum == 3){
			MM_PRINTF("%s, all matched\n",__FUNCTION__);
			MM_PRINTF("tag = %s\n", tag);
			MM_PRINTF("cryptoSuite = %s\n", cryptoSuite);
			*cryptoSuiteType = srtpGetCryptoSuiteType(cryptoSuite);
			MM_PRINTF("cryptoSuiteType = %d\n", *cryptoSuiteType);
			if(*cryptoSuiteType <0 ){
				MM_PRINTF("srtpGetCryptoSuiteType failed\n");
				return MM_FAILURE;
			}
			
			MM_PRINTF("base64 key = %s\n", keyStr);
			if(NULL != key){
				MM_PRINTF("base64 keylen = %d\n", strlen(keyStr));
				if(base64Decode(keyStr, strlen(keyStr), key, keylen) == 0){
					MM_PRINTF("decode keylen = %d\n", *keylen);
					for(i=0;i<*keylen;i++){
						MM_PRINTF("key[%d] = 0x%x\n", i , key[i]);
					}
					return MM_SUCCESS;
				}
				else{
					MM_PRINTF("base64_decode failed\n");
					return MM_FAILURE;
				}
			}
			else{
				return MM_SUCCESS; /*no output for key*/
			}
			
		}
		
		
		p_enc_attrib_list = p_enc_attrib_list->p_next;
		
	}


	MM_PRINTF("\n\tMM >Exit %s, failed\n", __FUNCTION__);
	return MM_FAILURE;
}


/*______________________________________________________________________________
**	srtpAddCryptoAtrribute
**
**	descriptions: add crypto attribute according to offer or config
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/



mm_return_t  srtpAddCryptoAtrribute(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream,
	uint16 key_size, /*bits*/
	char *cryptoSuiteName
){
	mm_return_t ret_val = MM_FAILURE;
	uint16 keyByteLength = key_size/8;
	unsigned char *key = NULL, *key2 = NULL, *keyDecimal = NULL;
	uint8 attributes[BUF_SIZE]={0};//"crypto:1 AES_CM_128_HMAC_SHA1_80 inline:";
	char base64[MAX_BASE64_STR_SIZE]={0};
	char digit[5]={0};
	int i = 0, ret = 0, outlen = MAX_BASE64_STR_SIZE;
	char *cryptoSuiteStr = NULL;

	RTP_PRINTF("entering %s\n",__FUNCTION__);

	#if 1
	int cryptoSuiteType = -1;
	if( MM_SUCCESS == srtpGetCryptoAttributes(p_ctx_stream->remote_sdp.p_encoded_attributes, &cryptoSuiteType, NULL, NULL)){
		if(cryptoSuiteType >= 0){
			RTP_PRINTF("remote_sdp found cryptoSuiteType = %d\n", cryptoSuiteType);
			cryptoSuiteStr = srtpSupportedCryptoSuiteList[cryptoSuiteType];
		}
	}

	if(cryptoSuiteType < 0)
	{
			/*use X_ZyXEL_COM_CryptoSuiteName */
			RTP_PRINTF("remote_sdp not found, use config cryptoSuiteName %s\n", cryptoSuiteName);
			cryptoSuiteType = -1;			
			if(zyStrlen(cryptoSuiteName)>0){
				RTP_PRINTF("config cryptoSuite = %s\n", cryptoSuiteName);
				cryptoSuiteType = srtpGetCryptoSuiteType(cryptoSuiteName);
			}
			
			if(cryptoSuiteType >= 0){
				RTP_PRINTF("remote_sdp not found, use config cryptoSuiteType = %d\n", cryptoSuiteType);
				cryptoSuiteStr = srtpSupportedCryptoSuiteList[cryptoSuiteType];
			}
			else{
				RTP_PRINTF("remote_sdp not found, use default cryptoSuiteType = %d\n", 0);
				cryptoSuiteStr = srtpSupportedCryptoSuiteList[0];
			}
			
			
	}
	#endif

	snprintf(attributes, BUF_SIZE, "crypto:1 %s inline:", cryptoSuiteStr);
	
	key = (unsigned char *)zyMalloc(keyByteLength);
	//key2 = (unsigned char *)zyMalloc(keyByteLength);
	//keyDecimal = (unsigned char *)zyMalloc(keyByteLength*2);
	//key[keyByteLength]=0;
	ret = crypto_get_random(key, keyByteLength);
	RTP_PRINTF("crypto_get_random ret = %d\n", ret);
	for(i = 0; i<keyByteLength; i++){
		RTP_PRINTF("key[%d] = 0x%x\n", i, key[i]);
	}
	
	base64Encode(key, keyByteLength, base64, &outlen);

	zyFree(key);

	RTP_PRINTF("key base64 encoded string = %s\n", base64);


	zyStrcat(attributes, base64);

	RTP_PRINTF("srtp attributes = %s\n", attributes);
	mmAddMediaAttributes(&p_ctx_stream->local_sdp.p_encoded_attributes, attributes);	
}

/*************************************************************************
 *
 *  FUNCTION NAME  : srtpGetCryptoAttributes
 *
 *  DESCRIPTION    :  
 *                   
 *
 *  RETURNS:       : return key if success, or NULL on failed
 *************************************************************************/
mm_return_t srtpSetPolicy(int cryptoSuiteType, srtp_policy_t *policy){
	MM_PRINTF("%s, cryptoSuiteType = %d\n",__FUNCTION__,cryptoSuiteType);
	switch(cryptoSuiteType){
		case SRTP_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_80:
			crypto_policy_set_rtp_default(&policy->rtp);
			crypto_policy_set_rtcp_default(&policy->rtcp);
			break;
		case SRTP_CRYPTO_SUITE_AES_CM_128_HMAC_SHA1_32:
			crypto_policy_set_aes_cm_128_hmac_sha1_32(&policy->rtp);
			crypto_policy_set_aes_cm_128_hmac_sha1_32(&policy->rtcp);
			break;
		case SRTP_CRYPTO_SUITE_AES_CM_128_NULL:
			crypto_policy_set_aes_cm_128_null_auth(&policy->rtp);
			crypto_policy_set_aes_cm_128_null_auth(&policy->rtcp);
			break;
		case SRTP_CRYPTO_SUITE_NULL_CIPHER_HMAC_SHA1_80:
			crypto_policy_set_null_cipher_hmac_sha1_80(&policy->rtp);
			crypto_policy_set_null_cipher_hmac_sha1_80(&policy->rtcp);
			break;
		default:
			ZyPrintf("cannot find support crypto suite, return NULL\n");
			return MM_FAILURE;					
	}

	return MM_SUCCESS;
}



/*______________________________________________________________________________
**	srtpCreateSession
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
mm_return_t  srtpCreateSession(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream

) {
	mm_return_t	ret_val = MM_FAILURE;
	rtpTable_t *table = MM_NULL;
	unsigned char *key = NULL;
	uint32 keylen = 0;
	int ret = 0;
	int cryptoSuiteType = 0;
	RTP_PRINTF("entering %s\n",__FUNCTION__);

	if( (table = rtpTableGetById(call_id, p_ctx_stream->stream_id, p_ctx_stream->stream_type)) == MM_NULL ){
		RTP_PRINTF("rtp_table not found!");
		return MM_FAILURE;
	}

	if((p_ctx_stream->local_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_SAVP)&& 
		(p_ctx_stream->remote_sdp.trans_mode == ICF_TRANSPORT_MODE_RTP_SAVP)){
		RTP_PRINTF(">local_sdp ICF_TRANSPORT_MODE_RTP_SAVP, get key\n");
		keylen = MAX_KEY_LENGTH;
		key = zyMalloc(keylen);
		zyMemset(key, 0, keylen);
		
		if(MM_SUCCESS == srtpGetCryptoAttributes(p_ctx_stream->local_sdp.p_encoded_attributes, &cryptoSuiteType, key, &keylen)){		
			if(MM_FAILURE == srtpSetPolicy(cryptoSuiteType, &table->srtpSendPolicy)){
				return MM_FAILURE;
			}
			table->srtpSendPolicy.ssrc.type = ssrc_any_outbound;
			table->srtpSendPolicy.ssrc.value = 0;
			table->srtpSendPolicy.key = key;
			table->srtpSendPolicy.next = NULL;
			RTP_PRINTF(">local_sdp key exist, start srtp send session\n");
			RTP_PRINTF("srtpSendSession = 0x%x, srtp_create\n", table->srtpSendSession);
			if(table->srtpSendSession == NULL){
				ret = srtp_create(&(table->srtpSendSession), &(table->srtpSendPolicy));
				if(ret != 0){
					RTP_PRINTF("error srtp_create ret = %d\n", ret);
					return MM_FAILURE;
				}
			}
			else{
				RTP_PRINTF("NULL srtpSendSession\n");
			}

			srtp_session_print_policy(table->srtpSendSession);
		}
		else{
			zyFree(key);
			ZyPrintf("mmEncAttribGetCryptoKey cannot get local_sdp srtp key, srtcpCreateSession failed\n");
			return MM_FAILURE;
		}
		
	
		RTP_PRINTF(">remote_sdp ICF_TRANSPORT_MODE_RTP_SAVP, get key\n");
		keylen = MAX_KEY_LENGTH;
		key = zyMalloc(keylen);
		zyMemset(key, 0, keylen);
		if(MM_SUCCESS == srtpGetCryptoAttributes(p_ctx_stream->remote_sdp.p_encoded_attributes, &cryptoSuiteType, key, &keylen)){
			if(MM_FAILURE == srtpSetPolicy(cryptoSuiteType, &table->srtpRecvPolicy)){
				return MM_FAILURE;
			}
			table->srtpRecvPolicy.ssrc.type = ssrc_any_inbound;
			table->srtpRecvPolicy.ssrc.value = 0;
			table->srtpRecvPolicy.key = key;
			table->srtpRecvPolicy.next = NULL;
			RTP_PRINTF(">remote_sdp key exist, start srtp recv session\n");
			RTP_PRINTF("srtpSendSession = 0x%x, srtp_create\n", table->srtpRecvSession);
			if(NULL == table->srtpRecvSession){
				ret = srtp_create(&(table->srtpRecvSession), &(table->srtpRecvPolicy));
				if(ret != 0){
					RTP_PRINTF("error srtp_create ret = %d\n", ret);
					return MM_FAILURE;
				}
			}
			else{
				RTP_PRINTF("NULL srtpRecvSession\n");
			}
			
			srtp_session_print_policy(table->srtpRecvSession);
		}
		else{
			zyFree(key);
			ZyPrintf("mmEncAttribGetCryptoKey cannot get remote_sdp srtp key, srtcpCreateSession failed\n");
			return MM_FAILURE;
		}
	}
	
	ret_val = MM_SUCCESS;
	RTP_PRINTF("\n\tMM >Exist %s ret_val=%d\n", __FUNCTION__, ret_val); 
	
	return ret_val;

}

/*______________________________________________________________________________
**	srtpDeleteSession
**
**	descriptions:
**	parameters:
**	local:
**	global:
**	return:
**	called by:
**	call:
**	revision:
**____________________________________________________________________________*/
mm_return_t  srtpDeleteSession(
       icf_call_id_t call_id,
	mmCtxStream_t	*p_ctx_stream){
	mm_return_t	ret_val = MM_FAILURE;
	rtpTable_t *table = MM_NULL;

	RTP_PRINTF("entering %s...\n",__FUNCTION__);
	if( (table = rtpTableGetById(call_id, p_ctx_stream->stream_id, p_ctx_stream->stream_type)) == MM_NULL ){
		RTP_PRINTF("rtp_table not found!");
		return MM_FAILURE;
	}
	if(NULL!=table->srtpSendSession){
		srtp_dealloc(table->srtpSendSession);
		table->srtpSendSession = NULL;
	}

	if(NULL!=table->srtpRecvSession){
		srtp_dealloc(table->srtpRecvSession);
		table->srtpRecvSession = NULL;		
	}

	if(NULL != table->srtpSendPolicy.key){
		zyFree(table->srtpSendPolicy.key);
	}

	if(NULL != table->srtpRecvPolicy.key){
		zyFree(table->srtpRecvPolicy.key);
	}
	ret_val = MM_SUCCESS;
RTP_PRINTF("\n\tMM >Exist %s ret_val=%d\n", __FUNCTION__, ret_val); 
	return ret_val;
}


