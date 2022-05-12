
/* Ensure Names are not mangled by C++ compilers */

#ifndef __ALHEADER_H
#define __ALHEADER_H

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

#ifdef SDF_TXN
#include "microsip_txntimer.h"
#include "microsip_txnstruct.h"
#include "microsip_txnfree.h"
#include "microsip_txninit.h"
#include "microsip_txnclone.h"
#include "microsip_txndecode.h"
#include "microsip_txndecodeintrnl.h"
#else
#include "microsip_timer.h"
#endif

#ifdef SDF_TEL
#include "microsip_telinit.h"
#include "microsip_telfree.h"
#include "microsip_telstruct.h"
#include "microsip_telparser.h"
#include "microsip_telapi.h"
#endif

#ifdef SIP_IMPP
#include "microsip_imurl.h"
#include "microsip_imapi.h"
#include "microsip_imurlfree.h"
#endif

#endif /* __ALHEADER_H */
