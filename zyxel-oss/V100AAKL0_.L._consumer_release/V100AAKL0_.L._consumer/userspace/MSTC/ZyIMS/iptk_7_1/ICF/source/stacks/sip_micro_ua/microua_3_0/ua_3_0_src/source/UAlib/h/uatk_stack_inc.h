#ifndef __UATK_STACK_INC_H_
#define __UATK_STACK_INC_H_
#include "icf_feature_flags.h"
#ifdef ICF_PORT_WINDOWS
#define SIP_WINDOWS
#define SDF_WINDOWS
#endif

#ifdef ICF_PORT_LINUX
#define SIP_LINUX
#define SDF_LINUX
#endif


#ifdef ICF_PORT_VXWORKS
#define SIP_VXWORKS
#define SDF_VXWORKS
#endif

#ifndef MICROUA
#define MICROUA
#endif

#ifndef ANSI_PROTO
#define ANSI_PROTO
#endif

/*Sip Flags*/
#define SIP_RPR
#define SIP_NO_CALLBACK
#define SIP_IMPP
#define SIP_TEL
#define SIP_CLONE
#define SIP_ACC
#define SIP_OVERRIDE_SNPRINTF
#define SIP_REFER
#define SIP_REPLACES
#define SIP_AUTHENTICATE
#define SIP_MIME_PARSING
#define SIP_ISUP
#define SIP_SIPMSGBODY
#define SIP_CORE_STACK_COMPATIBLE
#define SIP_DATE
#define SIP_BY_REFERENCE
#define SIP_TXN_LAYER
#define SIP_IMURL
#define SIP_SESSIONTIMER
#define SIP_PRIVACY


#undef SIP_CRITICAL_SECTION
#define SIP_BADMESSAGE_PARSING
#undef SIP_THREAD_SAFE
#undef SIP_3GPP
#define SIP_3GPP
#undef SIP_TRACE
#undef SIP_STATISTICS
#undef SIP_DEBUG
#undef SIP_FNDEBUG
#undef SIP_ERROR
#undef SIP_LOCKEDREFCOUNT
#undef SIP_INCREMENTAL_PARSING
#define SIP_TIMESTAMP
#define SIP_WARNING
#define SIP_RETRANSCALLBACK


/*UATK Flags*/
#define SDF_REFER
#define SDF_TEL
#define SDF_TXN
#define SDF_PARAMVALIDATION
#define SDF_IM
#define SDF_SESSION_TIMER
//#define SDF_PRIVACY

#undef SDF_IPV6
#define SDF_NETWORKCALLBACKS
#define SDF_AUTHENTICATE
#ifdef ICF_SECURE_SUPPORT
#define SDF_TLS
#endif
#ifdef ICF_DNS_LOOKUP_ENABLED
#define SDF_LOOKUP
#endif

#undef SDF_OPTIMIZATION_0
#undef SDF_SERVICE
/*#undef SDF_TRACE*/
#undef SDF_STATISTICS 
#undef SDF_DEBUG 
/*#undef SDF_ERROR */
#undef SDF_CALLFLOW 

#ifndef ICF_QOS_SUPPORT
#undef SDF_QOS
#endif

#undef SDF_THREAD_SAFE
#undef SDF_USE_PREALLOCBUFFERS 
#undef SDF_USE_INTERNALTHREADS

#endif
