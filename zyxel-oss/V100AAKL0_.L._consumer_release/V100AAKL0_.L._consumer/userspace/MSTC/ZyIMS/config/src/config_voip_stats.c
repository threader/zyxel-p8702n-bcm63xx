/*
   $Id: config_sys.c 0.00.0.1 2008/11/12 14:51 ctpan Exp $
*/
/************************************************************************
 *
 *	Copyright (C) 2008 ZyXEL Communications, Corp.
 *	All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Corp. and shall
 * not be reproduced, copied, disclosed, or used in whole or in part
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Corp.
 *
 *************************************************************************/

/*
 * $Log: config_sys.c $
*/
#include "global.h"
#include "tr104_object.h"
#include "config_sys.h"
#include "syscall_wrapper.h"
#include "config_voip_stats.h"

#define PROFILE_MAX 1
#define LINE_MAX 2

static int act_voice_service = 0;
static int act_voice_prof_number = 0;
static int* act_voice_line_number;
static Config_VoIP_Stats_t* statsBuff[PROFILE_MAX][LINE_MAX];

static const char STR_NONE[12] = "N/A";

