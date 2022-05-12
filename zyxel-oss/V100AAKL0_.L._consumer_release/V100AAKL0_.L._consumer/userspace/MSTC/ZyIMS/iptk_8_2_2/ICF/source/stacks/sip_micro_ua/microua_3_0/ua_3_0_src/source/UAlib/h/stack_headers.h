/*
 *
 * All the stack headers are included in this file
 *
 */

#ifndef __STACK_HEADERS_H__
#define __STACK_HEADERS_H__

#include "uatk_stack_inc.h"

/* Micor-Sip stack files*/
#include "microsip_common.h"
#include "microsip_struct.h"
#include "microsip_list.h"
#include "microsip_bcptinit.h"
#include "microsip_bcptfree.h"
#include "microsip_msgbody.h"
#include "microsip_accessors.h"
#include "microsip_subapi.h"
#include "microsip_portlayer.h"
#include "microsip_bcptfree.h"
#include "microsip_bcptinit.h"
#include "microsip_clone.h"
#include "microsip_cloneintrnl.h"
#include "microsip_common.h"
#include "microsip_decode.h"
#include "microsip_decodeinternal.h"
#include "microsip_encode.h"
#include "microsip_encodeinternal.h"
#include "microsip_hdrencoders.h"
#include "microsip_free.h"
#include "microsip_init.h"
#include "microsip_list.h"
#include "microsip_sendmessage.h"
#include "microsip_statistics.h"
#include "microsip_trace.h"
#include "microsip_header.h"
#include "microsip_subparsers.h"

#include "microsip_startline.h"
#include "microsip_callbacks.h"
#include "microsip_hdrparsers.h"
#include "microsip_msgbodyparser.h"


#ifdef SIP_IMPP
#include "microsip_imurlfree.h"
#include "microsip_imurlinit.h"
#include "microsip_imurlstruct.h"
#include "microsip_imurl.h"
#include "microsip_imapi.h"
#endif

#ifdef SIP_TXN_LAYER
#include "microsip_txntimer.h"
#include "microsip_txnstruct.h"
#include "microsip_txnfree.h"
#include "microsip_txninit.h"
#include "microsip_txnclone.h"
#include "microsip_txndecode.h"
/*#include "microsip_txndecodeintrnl.h"
#include "microsip_txnmidway.h"*/
#else
#include "microsip_timer.h"
#endif

#ifdef SIP_TEL
#include "microsip_telinit.h"
#include "microsip_telfree.h"
#include "microsip_telstruct.h"
#include "microsip_telparser.h"
#include "microsip_telapi.h"
#endif

#endif

