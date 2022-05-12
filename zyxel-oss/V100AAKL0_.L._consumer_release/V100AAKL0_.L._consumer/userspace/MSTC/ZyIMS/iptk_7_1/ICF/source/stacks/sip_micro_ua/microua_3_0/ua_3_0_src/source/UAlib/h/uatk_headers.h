/* Ensure Names are not mangled by C++ compilers */

#ifndef __UATKHEADER_H
#define __UATKHEADER_H

#include "stack_headers.h"
#if 0
#if !defined(SDF_WINDOWS) && !defined(SDF_WINCE)
#include "uatk_config.h"
#endif
#endif
#include "basic.h"
#include "digcalc.h"
#include "global.h"
#include "md5.h"
#include "sdf_common.h"
#include "sdf_basictypes.h"
#include "sdf_struct.h"
#include "sdf_tables.h"
#include "sdf_portlayer.h"
#include "sdf_accessor.h"
#include "sdf_free.h"
#include "sdf.h"
#include "sdf_init.h"
#include "sdf_hash.h"
#include "sdf_callapis.h"
#include "sdf_reqresp.h"
#include "sdf_sdp.h"
#include "sdf_debug.h"
#include "sdf_network.h"
#include "sdf_configs.h"
#include "sdf_statistics.h"
#include "sdf_trace.h"
#include "sdf_list.h"
#include "sdf_internal.h"
#include "sdf_authorization.h"
#include "sdf_callbacks.h"
#include "sdf_sdpstruct.h"
#include "sdf_sdpinit.h"
#include "sdf_sdpfree.h"
#include "sdf_sdpinternal.h"
#include "sdf_sdpaccessor.h"
#include "sdf_mempool.h"

#ifdef SDF_TXN
#include "sdf_txn.h"
#endif
	
#ifdef SDF_SESSION_TIMER
#include "sdf_sessTimer.h"
#endif

#ifdef SDF_SUBSCRIPTION_LAYER
#include "sdf_subslayerstruct.h"
#endif

#ifdef SDF_SUBS_PACKAGE_LAYER
#include "sdf_packageinfostruct.h"
#endif

#ifdef SDF_HA
#include "sdf_hacommon.h"
#include "sdf_haapi.h"
#include "sdf_haactive.h"
#include "sdf_haapiint.h"
#include "sdf_haint.h"
#include "sdf_hadeserstruct.h"
#include "sdf_haserstruct.h"
#include "sdf_haserint.h"
#include "sdf_haser.h"
#include "sdf_hastandby.h"
#endif


#ifdef SDF_SERVICE
#include "sdf_servicestruct.h"
#include "sdf_service.h"
#include "sdf_servicecallbacks.h"
#include "sdf_servicecallobjectassoc.h"
#include "sdf_servicefree.h"
#include "sdf_serviceinit.h"
#include "sdf_serviceinternal.h"
#include "sdf_ctstruct.h"
#include "sdf_ct.h"
#include "sdf_ctfree.h"
#include "sdf_ctinit.h" 
#include "sdf_ctinternal.h"
#endif



#endif








