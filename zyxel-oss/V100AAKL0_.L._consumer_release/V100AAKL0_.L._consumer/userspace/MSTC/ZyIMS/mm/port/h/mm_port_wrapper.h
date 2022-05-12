/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/

#ifndef __ICF_MM_PORT_WRAPPER_H__
#define __ICF_MM_PORT_WRAPPER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/un.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <semaphore.h>
#include <sys/msg.h>
#include <errno.h>
#include <sys/types.h>
#include <time.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include "icf_types.h"
#include "icf_api.h"

#define MM_PRINTF	if (mmRmDebug & 1)	ZyPrintf
#define RTP_PRINTF	if (mmRtpDebug & 1)	ZyPrintf
#define RTCP_PRINTF	if (mmRtpDebug & 1)	ZyPrintf
#define VDSP_PRINTF	if (mmDspDebug & 1)	ZyPrintf
#define MM_NULL NULL

typedef icf_return_t mm_return_t;
typedef icf_error_t  mm_error_t;




#endif

