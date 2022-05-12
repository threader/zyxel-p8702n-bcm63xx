/*****************************************************************************
 ** 		This file contains the integration of OpenSSL function calls.
 **
 *****************************************************************************
 **
 ** FILENAME:		openssl_wrapper.c
 **
 ** DESCRIPTION: This file contains the integration of OpenSSL function calls.	 	
 ** DATE 		    NAME		       REFERENCE		          REASON
 ** ---- 		    ----		       ---------		          ------
 ** 06/02/09	Rajiv Kumar			Initial Creation    integration of OpenSSL
 ** 08/11/09    Anuradha Gupta      SPR 19776           Prototype modified
 ** 08/20/09    Anuradha Gupta      SPR 19927           GCC Warning removal
 ** 
 *****************************************************************************
 **				 Copyright (C) 2009 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include "openssl_wrapper.h"


/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_new
 **
 ** DESCRIPTION:	This wrapper will call the SSL_new() Openssl function call. 
 **
 *****************************************************************************/

icf_pvoid_t wrapper_SSL_new(icf_pvoid_t pSslCtx)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_new((SSL_CTX* )pSslCtx);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_new((SSL_CTX* )pSslCtx);
#else
    (icf_void_t)pSslCtx;
    return ICF_NULL;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_set_fd
 **
 ** DESCRIPTION:	This wrapper will call the SSL_set_fd() Openssl function call. **
 *****************************************************************************/

icf_void_t wrapper_SSL_set_fd(icf_pvoid_t pSsl, icf_int32_t dSockfd)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_set_fd((SSL *)pSsl,dSockfd);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_set_fd((SSL *)pSsl,dSockfd);
#else
    (icf_void_t)pSsl;
    (icf_void_t)dSockfd;
    return;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_set_connect_state
 **
 ** DESCRIPTION:	This wrapper will call the SSL_set_connect_state() Openssl 
 **                 function call.
 *****************************************************************************/

icf_void_t wrapper_SSL_set_connect_state(icf_pvoid_t pSsl)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_set_connect_state((SSL *)pSsl);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_set_connect_state((SSL *)pSsl);
#else
    (icf_void_t)pSsl;
    return;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_connect
 **
 ** DESCRIPTION:	This wrapper will call the SSL_connect() Openssl function call. **
 *****************************************************************************/

icf_int32_t wrapper_SSL_connect(icf_pvoid_t pSsl)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_connect((SSL *) pSsl);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_connect((SSL *) pSsl);
#else
    (icf_void_t)pSsl;
    return ICF_SUCCESS;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_free
 **
 ** DESCRIPTION:	This wrapper will call the SSL_free() Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_free(icf_pvoid_t pSsl)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_free((SSL *)pSsl);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_free((SSL *)pSsl);
#else
    (icf_void_t)pSsl;
    return;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_load_error_strings
 **
 ** DESCRIPTION:	This wrapper will call the SSL_load_error_strings() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_load_error_strings(icf_void_t)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_load_error_strings();
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_load_error_strings();
#else
    return;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_free
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_free() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_free(icf_pvoid_t pData)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_CTX_free((SSL_CTX *)pData);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_CTX_free((SSL_CTX *)pData);
#else
    (icf_void_t)pData;
    return;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_new
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_new() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_pvoid_t wrapper_SSL_CTX_new(icf_void_t)
{
#if  defined(ICF_PORT_LINUX)    
    return (icf_pvoid_t)SSL_CTX_new(TLSv1_method());
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return (icf_pvoid_t)SSL_CTX_new(TLSv1_method());
#else
    return ICF_NULL;    
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_use_certificate_file
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_use_certificate_file() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_use_certificate_file(icf_pvoid_t pSslCtx,
        icf_int8_t* pCertFile,icf_int32_t dCertFormat)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_CTX_use_certificate_file((SSL_CTX *)pSslCtx, pCertFile,dCertFormat);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_use_certificate_file((SSL_CTX *)pSslCtx, pCertFile,dCertFormat);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)pCertFile;
    (icf_void_t)dCertFormat;
    return ICF_SUCCESS;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_default_passwd_cb
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_default_passwd_cb() 
 **               Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_default_passwd_cb(icf_pvoid_t pSslCtx,
        icf_pvoid_t dUaParams)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_CTX_set_default_passwd_cb((SSL_CTX *)pSslCtx,
            (pem_password_cb *)dUaParams);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_CTX_set_default_passwd_cb((SSL_CTX *)pSslCtx,
            (pem_password_cb *)dUaParams);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)dUaParams;
    return;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_default_passwd_cb_userdata
 **
 ** DESCRIPTION:	This wrapper will call the 
 **                 SSL_CTX_set_default_passwd_cb_userdata() Openssl function
 **                 call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_default_passwd_cb_userdata(icf_pvoid_t pSslCtx,
        icf_pvoid_t pPrivateKeyPasswd)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_CTX_set_default_passwd_cb_userdata((SSL_CTX *)pSslCtx,
            pPrivateKeyPasswd);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_CTX_set_default_passwd_cb_userdata((SSL_CTX *)pSslCtx,
            pPrivateKeyPasswd);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)pPrivateKeyPasswd;
    return;    
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_use_PrivateKey_file
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_use_PrivateKey_file() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_use_PrivateKey_file(icf_pvoid_t pSslCtx,
        icf_pvoid_t pPrivateKeyFile,icf_int32_t dCertFormat)    
{
#if  defined(ICF_PORT_LINUX)   
    return SSL_CTX_use_PrivateKey_file((SSL_CTX *)pSslCtx,
            (const icf_int8_t *)pPrivateKeyFile,dCertFormat);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_use_PrivateKey_file((SSL_CTX *)pSslCtx,
            (const icf_int8_t *)pPrivateKeyFile,dCertFormat);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)pPrivateKeyFile;
    (icf_void_t)dCertFormat;
    return ICF_SUCCESS;    
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_check_private_key
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_check_private_key() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_check_private_key(icf_pvoid_t pSslCtx)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_CTX_check_private_key((SSL_CTX *)pSslCtx);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_check_private_key((SSL_CTX *)pSslCtx);
#else
    (icf_void_t)pSslCtx;
    return ICF_SUCCESS;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_load_verify_locations
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_load_verify_locations() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_load_verify_locations(icf_pvoid_t pSslCtx,
        icf_pvoid_t pCACertFile,icf_pvoid_t pCACertDirPath)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_CTX_load_verify_locations((SSL_CTX *)pSslCtx,
            (icf_int8_t*)pCACertFile,(icf_int8_t*)pCACertDirPath);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_load_verify_locations((SSL_CTX *)pSslCtx,
            (icf_int8_t*)pCACertFile,(icf_int8_t*)pCACertDirPath);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)pCACertFile;
    (icf_void_t)pCACertDirPath;
    return ICF_SUCCESS;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_default_verify_paths
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_default_verify_paths() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_set_default_verify_paths(icf_pvoid_t pSslCtx)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_CTX_set_default_verify_paths((SSL_CTX *)pSslCtx);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_set_default_verify_paths((SSL_CTX *)pSslCtx);
#else
    (icf_void_t)pSslCtx;
    return ICF_SUCCESS;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_verify
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_verify() 
 **                 Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_verify(icf_pvoid_t pSslCtx,
        icf_uint8_t dCertVerifyMode,icf_pvoid_t dUaParams)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_CTX_set_verify((SSL_CTX *)pSslCtx,dCertVerifyMode,
            (int (*)(int, X509_STORE_CTX*))dUaParams);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_CTX_set_verify((SSL_CTX *)pSslCtx,dCertVerifyMode,
            (int (*)(int, X509_STORE_CTX*))dUaParams);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)dCertVerifyMode;
    (icf_void_t)dUaParams;
#endif
#endif
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_cipher_list
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_cipher_list() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_CTX_set_cipher_list(icf_pvoid_t pSslCtx,
        icf_int8_t* pCipherList)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_CTX_set_cipher_list((SSL_CTX *)pSslCtx,pCipherList);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_set_cipher_list((SSL_CTX *)pSslCtx,pCipherList);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)pCipherList;
    return ICF_SUCCESS;
#endif
#endif    
}

/*****************************************************************************
 ** FUNCTION:		wrapper_PEM_read_DHparams
 **
 ** DESCRIPTION:	This wrapper will call the PEM_read_DHparams() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_pvoid_t wrapper_PEM_read_DHparams(FILE *pDHParamfp,
        icf_pvoid_t var1,icf_pvoid_t var2,icf_pvoid_t var3)
{
    /*Warning Removal*/
    var1 = var1;
    var2 = var2;
    var3 = var3;
#if  defined(ICF_PORT_LINUX)    
    return (icf_pvoid_t)PEM_read_DHparams((FILE*)pDHParamfp,
            ICF_NULL,ICF_NULL,ICF_NULL);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return (icf_pvoid_t)PEM_read_DHparams((FILE*)pDHParamfp,
            ICF_NULL,ICF_NULL,ICF_NULL);
#else
    (icf_void_t)pDHParamfp;
    (icf_void_t)var1;
    (icf_void_t)var2;
    (icf_void_t)var3;
    return ICF_NULL;
#endif
#endif  
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_tmp_dh
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_tmp_dh() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_tmp_dh(icf_pvoid_t pSslCtx,icf_pvoid_t  dh)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_CTX_set_tmp_dh((SSL_CTX *)pSslCtx,(DH* )dh);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_CTX_set_tmp_dh((SSL_CTX *)pSslCtx,(DH* )dh);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)dh;
    return;
#endif
#endif  
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_options
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_options() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_uint64_t wrapper_SSL_CTX_set_options(icf_pvoid_t pSslCtx,icf_uint64_t var1)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_CTX_set_options((SSL_CTX *)pSslCtx,var1);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_CTX_set_options((SSL_CTX *)pSslCtx,var1);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)var1;
    return ICF_SUCCESS;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_DH_free
 **
 ** DESCRIPTION:	This wrapper will call the DH_free() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_DH_free(icf_pvoid_t dh)
{
#if  defined(ICF_PORT_LINUX)    
    DH_free((DH *) dh);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    DH_free((DH *) dh);
#else
    (icf_void_t)dh;
    return;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_CTX_set_quiet_shutdown
 **
 ** DESCRIPTION:	This wrapper will call the SSL_CTX_set_quiet_shutdown() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_CTX_set_quiet_shutdown(icf_pvoid_t pSslCtx, 
        icf_int32_t      var1)
{
#if  defined(ICF_PORT_LINUX) 
    SSL_CTX_set_quiet_shutdown((SSL_CTX *)pSslCtx,var1);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_CTX_set_quiet_shutdown((SSL_CTX *)pSslCtx,var1);
#else
    (icf_void_t)pSslCtx;
    (icf_void_t)var1;
    return;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSLeay_add_ssl_algorithms
 **
 ** DESCRIPTION:	This wrapper will call the SSLeay_add_ssl_algorithms() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSLeay_add_ssl_algorithms(icf_void_t)
{
#if  defined(ICF_PORT_LINUX)  
    SSLeay_add_ssl_algorithms();
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSLeay_add_ssl_algorithms();
#else
    return;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_set_accept_state
 **
 ** DESCRIPTION:	This wrapper will call the SSL_set_accept_state() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_SSL_set_accept_state(icf_pvoid_t pSsl)
{
#if  defined(ICF_PORT_LINUX)    
    SSL_set_accept_state((SSL *)pSsl);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    SSL_set_accept_state((SSL *)pSsl);
#else
    (icf_void_t)pSsl;
    return;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_accept
 **
 ** DESCRIPTION:	This wrapper will call the SSL_accept() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_accept(icf_pvoid_t pSsl)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_accept((SSL *)pSsl);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_accept((SSL *)pSsl);
#else
    (icf_void_t)pSsl;
    return ICF_SUCCESS;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_get_error
 **
 ** DESCRIPTION:	This wrapper will call the SSL_get_error() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_get_error(icf_pvoid_t pSsl,icf_int32_t dRetVal)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_get_error((SSL *)pSsl,dRetVal);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_get_error((SSL *)pSsl,dRetVal);
#else
    (icf_void_t)pSsl;
    (icf_void_t)dRetVal;
    return ICF_SUCCESS;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_shutdown
 **
 ** DESCRIPTION:	This wrapper will call the SSL_shutdown() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_shutdown(icf_pvoid_t pSsl)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_shutdown((SSL *)pSsl);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_shutdown((SSL *)pSsl);
#else
    (icf_void_t)pSsl;
    return;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_read
 **
 ** DESCRIPTION:	This wrapper will call the SSL_read() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_read(icf_pvoid_t pSsl,icf_pvoid_t message,
        icf_int32_t      var1)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_read((SSL *)pSsl,message,var1);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_read((SSL *)pSsl,message,var1);
#else
    (icf_void_t)pSsl;
    (icf_void_t)message;
    (icf_void_t)var1;
    return ICF_SUCCESS;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_ERR_print_errors_fp
 **
 ** DESCRIPTION:	This wrapper will call the ERR_print_errors_fp() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_void_t wrapper_ERR_print_errors_fp(FILE *dStdErr)
{
#if  defined(ICF_PORT_LINUX)    
    return ERR_print_errors_fp(dStdErr);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return ERR_print_errors_fp(dStdErr);
#else
    (icf_void_t)dStdErr;
    return;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:	 wrapper_SSL_write
 **
 ** DESCRIPTION:	This wrapper will call the SSL_write() 
 ** Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_write(icf_pvoid_t pSsl,icf_pvoid_t pBuffer,
        icf_int32_t dBuflen)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_write((SSL *)pSsl,pBuffer,dBuflen);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_write((SSL *)pSsl,pBuffer,dBuflen);
#else
    (icf_void_t)pSsl;
    (icf_void_t)pBuffer;
    (icf_void_t)dBuflen;
    return ICF_SUCCESS;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:	 wrapper_d2i_DHparams_fp
 **
 ** DESCRIPTION:	This wrapper will call the d2i_DHparams_fp() 
 ** Openssl function call. 
 **
 *****************************************************************************/
icf_pvoid_t wrapper_d2i_DHparams_fp(FILE *pDHParamfp,icf_pvoid_t pVar)
{
#if  defined(ICF_PORT_LINUX)    
    return d2i_DHparams_fp(pDHParamfp, &pVar);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return d2i_DHparams_fp(pDHParamfp, &pVar);
#else
    (icf_void_t)pDHParamfp;
    (icf_void_t)pVar;
    return ICF_NULL;
#endif
#endif 
}
/*****************************************************************************
 ** FUNCTION:	 wrapper_RAND_egd
 **
 ** DESCRIPTION:	This wrapper will call the RAND_egd() 
 ** Openssl function call. 
 **
 *****************************************************************************/
icf_int32_t wrapper_RAND_egd(icf_int8_t *pEGDRandFile)
{
#if  defined(ICF_PORT_LINUX)    
    return RAND_egd(pEGDRandFile);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return RAND_egd(pEGDRandFile);
#else
    (icf_void_t)pEGDRandFile;
    return -1;
#endif
#endif 
}

/*****************************************************************************
 ** FUNCTION:		wrapper_SSL_peek
 **
 ** DESCRIPTION:	This wrapper will call the SSL_peek() 
 **                 Openssl function call. 
 **
 *****************************************************************************/

icf_int32_t wrapper_SSL_peek(icf_pvoid_t pSsl,
        icf_pvoid_t message,icf_int32_t len)
{
#if  defined(ICF_PORT_LINUX)    
    return SSL_peek((SSL *)pSsl,message,len);
#elif defined(ICF_PORT_VXWORKS)
#ifdef arUSE_OPENSSL_OPEN_SOURCE
    return SSL_peek((SSL *)pSsl,message,len);
#else
    (icf_pvoid_t)pSsl;
    (icf_pvoid_t)message;
    (icf_pvoid_t)len;
    return ICF_SUCCESS;
#endif
#endif 
}



