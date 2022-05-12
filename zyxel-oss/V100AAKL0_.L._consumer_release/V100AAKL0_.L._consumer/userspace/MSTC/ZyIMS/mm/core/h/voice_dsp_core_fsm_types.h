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
 * shall not be reproduced, copied, dIsclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/
#ifndef __VOICE_DSP_CORE_FSM_TYPES_H__
#define __VOICE_DSP_CORE_FSM_TYPES_H__


typedef mm_return_t (*voiceDspCoreStateEventFprtTy)
						(void *input_param,                 
                         mm_error_t	*err);


#endif
