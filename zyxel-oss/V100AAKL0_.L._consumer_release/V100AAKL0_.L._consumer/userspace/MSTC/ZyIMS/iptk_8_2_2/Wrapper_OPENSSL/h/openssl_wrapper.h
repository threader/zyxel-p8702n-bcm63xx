#ifndef __OPENSSL_WRAPPER_H_
#define __OPENSSL_WRAPPER_H_

/*****************************************************************************
 ** 		This file contains the integration of OpenSSL function calls.
 **
 *****************************************************************************
 **
 ** FILENAME:		openssl_wrapper.h
 **
 ** DESCRIPTION: This file contains the wrappers for OpenSSL function calls.	 	
 ** DATE 		    NAME		       REFERENCE		          REASON
 ** ---- 		    ----		       ---------		          ------
 ** 06/02/09	Rajiv Kumar			Initial Creation    integration of OpenSSL
 ** 08/11/09    Anuradha Gupta      SPR 19776           Prototype modified
 ** 25-Aug-2009 Rajiv Kumar         SPR: 19982          OPENSSL include Path
 **                                                     removed for Linux
 ** 
 *****************************************************************************
 **				 Copyright (C) 2009 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#include "icf_types.h"
#include "icf_defs.h"
#include "stdio.h"

#if  defined(ICF_PORT_LINUX)  

#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/x509.h"
#include "openssl/dh.h"
#include "openssl/pem.h"
#include "openssl/rand.h"

#define ICF_SSL_ERROR_NONE          SSL_ERROR_NONE
#define ICF_SSL_ERROR_WANT_READ     SSL_ERROR_WANT_READ
#define ICF_SSL_ERROR_WANT_WRITE    SSL_ERROR_WANT_WRITE
#define ICF_SSL_ERROR_WANT_ACCEPT   SSL_ERROR_WANT_ACCEPT

#endif

#if  (defined(ICF_PORT_VXWORKS) && defined(arUSE_OPENSSL_OPEN_SOURCE))
#include "openssl/ssl.h"
#include "openssl/err.h"
#include "openssl/x509.h"
#include "openssl/dh.h"
#include "openssl/pem.h"
#include "openssl/rand.h"


#define ICF_SSL_ERROR_NONE          SSL_ERROR_NONE
#define ICF_SSL_ERROR_WANT_READ     SSL_ERROR_WANT_READ
#define ICF_SSL_ERROR_WANT_WRITE    SSL_ERROR_WANT_WRITE
#define ICF_SSL_ERROR_WANT_ACCEPT   SSL_ERROR_WANT_ACCEPT
#endif    

#ifdef __cplusplus
extern "C" {
#endif
 
    
/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_new
 **
 ** DESCRIPTION:	This wrapper will call the SSL_new() Openssl function call. 
 **
 *****************************************************************************/

icf_pvoid_t wrapper_SSL_new(icf_pvoid_t pSslCtx);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_set_fd
 **
 ** DESCRIPTION:	This wrapper will call the SSL_set_fd() Openssl function call.
 *****************************************************************************/

icf_void_t wrapper_SSL_set_fd(icf_pvoid_t pSsl, icf_int32_t dSockfd);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_set_connect_state
 **
 ** DESCRIPTION:	This wrapper will call the SSL_set_connect_state() 
 **                 Openssl function call. 
 *****************************************************************************/

icf_void_t wrapper_SSL_set_connect_state(icf_pvoid_t pSsl);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_connect
 **
 ** DESCRIPTION:	This wrapper will call the SSL_connect() Openssl function call.
 *****************************************************************************/

icf_int32_t wrapper_SSL_connect(icf_pvoid_t pSsl);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_free
 **
 ** DESCRIPTION:	This wrapper will call the SSL_free() Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_free(icf_pvoid_t pSsl);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_load_error_strings
 **
 ** DESCRIPTION:	This wrapper will call the SSL_load_error_strings() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_load_error_strings(icf_void_t);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_free
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_free() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_free(icf_pvoid_t pData);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_new
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_new() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_pvoid_t wrapper_SSL_CTX_new(icf_void_t);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_use_certificate_file
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_use_certificate_file() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_use_certificate_file(icf_pvoid_t pSslCtx,
        icf_int8_t* pCertFile,icf_int32_t dCertFormat);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_default_passwd_cb
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_default_passwd_cb() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_default_passwd_cb(icf_pvoid_t pSslCtx,
        icf_pvoid_t dUaParams);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_default_passwd_cb_userdata
 **
 ** DESCRIPTION:	This wrapper will call the 
 **                 SSL_CTX_set_default_passwd_cb_userdata() Openssl function
 **                 call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_default_passwd_cb_userdata(icf_pvoid_t pSslCtx,
        icf_pvoid_t pPrivateKeyPasswd);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_use_PrivateKey_file
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_use_PrivateKey_file() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_use_PrivateKey_file(icf_pvoid_t pSslCtx,
        icf_pvoid_t pPrivateKeyFile,icf_int32_t dCertFormat);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_check_private_key
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_check_private_key() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_check_private_key(icf_pvoid_t pSslCtx);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_load_verify_locations
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_load_verify_locations() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_load_verify_locations(icf_pvoid_t pSslCtx,
        icf_pvoid_t pCACertFile,icf_pvoid_t pCACertDirPath);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_default_verify_paths
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_default_verify_paths() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_set_default_verify_paths(icf_pvoid_t pSslCtx);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_verify
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_verify() 
 **                 Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_verify(icf_pvoid_t pSslCtx,
        icf_uint8_t dCertVerifyMode,icf_pvoid_t dUaParams);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_cipher_list
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_cipher_list() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_set_cipher_list(icf_pvoid_t pSslCtx,
        icf_int8_t* pCipherList);

/*****************************************************************************
 ** FUNCTION:		wrapper_PEM_read_DHparams
 **
 ** DESCRIPTION:	This wrapper will call the PEM_read_DHparams() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_pvoid_t wrapper_PEM_read_DHparams(FILE *pDHParamfp,
        icf_pvoid_t a,icf_pvoid_t b,icf_pvoid_t c);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_tmp_dh
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_tmp_dh() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_tmp_dh(icf_pvoid_t pSslCtx,icf_pvoid_t  dh);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_options
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_options() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_uint64_t wrapper_SSL_CTX_set_options(icf_pvoid_t pSslCtx,icf_uint64_t
        a);

/*****************************************************************************
 ** FUNCTION:		wrapper_DH_free
 **
 ** DESCRIPTION:	This wrapper will call the DH_free() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_DH_free(icf_pvoid_t dh);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_quiet_shutdown
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_quiet_shutdown() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_quiet_shutdown(icf_pvoid_t pSslCtx, icf_int32_t
        a);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSLeay_add_ssl_algorithms
 **
 ** DESCRIPTION:	This wrapper will call the SSLeay_add_ssl_algorithms() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSLeay_add_ssl_algorithms(icf_void_t);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_set_accept_state
 **
 ** DESCRIPTION:	This wrapper will call the SSL_set_accept_state() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_set_accept_state(icf_pvoid_t pSsl);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_accept
 **
 ** DESCRIPTION:	This wrapper will call the SSL_accept() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_accept(icf_pvoid_t pSsl);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_get_error
 **
 ** DESCRIPTION:	This wrapper will call the SSL_get_error() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_get_error(icf_pvoid_t pSsl,icf_int32_t dRetVal);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_shutdown
 **
 ** DESCRIPTION:	This wrapper will call the SSL_shutdown() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_shutdown(icf_pvoid_t pSsl);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_read
 **
 ** DESCRIPTION:	This wrapper will call the SSL_read() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_read(icf_pvoid_t pSsl,icf_pvoid_t
        message,icf_int32_t a);

/*****************************************************************************
 ** FUNCTION:		wrapper_ERR_print_errors_fp
 **
 ** DESCRIPTION:	This wrapper will call the ERR_print_errors_fp() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_ERR_print_errors_fp(FILE *dStdErr);

/*****************************************************************************
 ** FUNCTION:	 wrapper_SSL_write
 **
 ** DESCRIPTION:	This wrapper will call the SSL_write() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_write(icf_pvoid_t pSsl,icf_pvoid_t pBuffer,
        icf_int32_t dBuflen);

/*****************************************************************************
 ** FUNCTION:	 wrapper_d2i_DHparams_fp
 **
 ** DESCRIPTION:	This wrapper will call the d2i_DHparams_fp() 
 ** Openssl function call. 
 **
 *****************************************************************************/
icf_pvoid_t wrapper_d2i_DHparams_fp(FILE *pDHParamfp,icf_pvoid_t pVar);

/*****************************************************************************
 ** FUNCTION:	 wrapper_RAND_egd
 **
 ** DESCRIPTION:	This wrapper will call the RAND_egd() 
 ** Openssl function call. 
 **
 *****************************************************************************/
icf_int32_t wrapper_RAND_egd(icf_int8_t *pEGDRandFile);

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_peek
 **
 ** DESCRIPTION:	This wrapper will call the SSL_peek() 
 **                 Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_peek(icf_pvoid_t pSsl,
        icf_pvoid_t message,icf_int32_t a);

#ifdef __cplusplus
}
#endif 
#endif
