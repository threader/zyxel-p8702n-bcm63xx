#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "global.h"
#include "voiceApp_main.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "Voipccm_sip_itf.h"
#include "commands.h"
#include "tr104_init.h"
#include "tr104.h"
#include "voice_dsp_common_api.h"	/* send packed msg to MM */

/*____________________________________________________________________________*/
/*	function declaration                                                                                                     */
/*____________________________________________________________________________*/
static int  cmd_show_valid (cmds_t cmds[]);
int doSip(int argc, char *argv[]);
int doMm(int argc, char *argv[]);
int doFsm(int argc, char *argv[]);
int doFxoTest(int argc, char *argv[]);
int doFsmPhone(int argc, char *argv[]);
int doFsmCcm(int argc, char *argv[]);
int doFsmConvert(int argc, char *argv[]);
int doCallHistory(int argc, char *argv[]);
int doActive(int argc, char *argv[]);
int doDebug(int argc, char *argv[]);
int doZyimsShow(int argc, char *argv[]);
int doSipSysShow(int argc, char *argv[]);
int doSipLineShow(int argc, char *argv[]);
int doSipRegShow(int argc, char *argv[]);
int doSipRegister(int argc, char *argv[]);
int doSipDeregister(int argc, char *argv[]);
int doCallHistoryShow(int argc, char *argv[]);
int doCallHistoryClear(int argc,char* argv[]);
int doMMSetDebug(int argc, char *argv[]);
int doMMStatusShow(int argc, char *argv[]);
int doMMKSocketSet(int argc, char	*argv[]);
int doDsp(int argc, char	*argv[]);
int doJBSet(int argc, char	*argv[]);
int doDspInit(int argc, char	*argv[]);
int doDspDeinit(int argc, char	*argv[]);
int doPhone(int argc, char	*argv[]);
int doFlashInterval(int argc, char	*argv[]);
int doPhoneToneTest(int argc, char	*argv[]);
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
int doPhoneAutoTest(int argc, char	*argv[]);
static int fxsRingTimeout(tm_id_t timer_id, void *arg);
#endif
/*Add some debug message in voiceApp for future debugging.,Mitrastar, Jason,20120913*/
int doSipAppShow(int	argc, char	*argv[]);


extern int dspReinitNeeded;
extern sipCCMConvert_t sipCCMConvert;
#if KSOCKET_RTP_SUPPORT
int mmKSocketEnable = KSOCKET_RTP_SUPPORT;
#endif
/*______________________________________________________________________*/
/*      Commands Declaration
**______________________________________________________________________*/
const cmds_t zyimsCmds[] =
{
   { "sip",	doSip,  CAT1|SUBCMD, 0,	"SIP related commands",	NULL},
   { "mm",	doMm,  CAT1|SUBCMD, 0,	"MM related commands",	NULL},
   { "phone",	doPhone,  CAT1|SUBCMD, 0,	"physical phone related commands",	NULL},
   { "fsm",	doFsm,  CAT1|SUBCMD, 0,	"FSM related commands",	NULL},
   { "active",	doActive,  CAT1, 1,	"activate/deactivate VoIP service",	"<on/off>"},
   { "debug",	doDebug,  CAT1, 2,	"enable/disable debug messages",	"<voiceApp/ccm/phone/sip/tr104/icf/all> <on:1/off:0>"},
   { "show",	doZyimsShow,  CAT1, 0,	"dump TR-104 object config",	NULL},   
   { "fxotest",	doFxoTest,  CAT1, 0,	"Fxo Test",	"<onhook/offhook number/setrelay [0|1]>"},
   { NULL, NULL, NULL, NULL, NULL, NULL }
};

const cmds_t sipCmds[] =
{
   { "showsys",	doSipSysShow,  CAT1, 0,	"dump IPTK system settings",	NULL},
   { "showline",	doSipLineShow,  CAT1, 1,	"dump IPTK line settings",	"<lineId>"},
   { "showreg",	doSipRegShow,  CAT1, 1,	"dump IPTK register status",	"<lineId>"},
   { "register",	doSipRegister,  CAT1, 1,	"send SIP register",	"<lineId>"},
   { "deregister",	doSipDeregister,  CAT1, 1,	"send SIP deregister",	"<lineId>"},
   { "callhistory", doCallHistory, CAT1, 1, "show Call History", "<show/clear>"},
   { "showapp",	doSipAppShow,  CAT1, 0,	"dump status of Voice Apprication",NULL},   
   { NULL, NULL, NULL, NULL, NULL, NULL }
};

const cmds_t fsmCmds[] =
{
   { "phone",	doFsmPhone,  CAT1, 0,	"dump Phone Fsm status",	NULL},
   { "ccm",	doFsmCcm,  CAT1, 0,	"dump CCM Fsm status",	NULL},
   { "convert",	doFsmConvert,  CAT1, 0,	"dump sipCCMConvert status",	NULL},
   { NULL, NULL, NULL, NULL, NULL, NULL }
};

const cmds_t phoneCmds[] =
{
   { "flashInterval",	 doFlashInterval,  CAT1, 1,  "set Flash Intervale",  "show <line> | set <line> <min> <max>"},
   { "toneTest",	 doPhoneToneTest,  CAT1, 1,  "set Tone Test",  "<line> <tone_id> <caller_id> | show"},
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
   { "autoTest",	 doPhoneAutoTest,  CAT1, 1,  "start VoIP Auto Test",  "<test_digit> | show"},
#endif   
   { NULL, NULL, NULL, NULL, NULL, NULL }
};



const cmds_t mmCmds[] =
{
   { "status",	doMMStatusShow,  CAT1, 1,	"dump MM related status",	"<rm/rtp/dsp/ivr>"},
   { "debug",	doMMSetDebug,  CAT1, 2,	"enable/disable MM debug messages",	"<rm/rtp/dsp> <on:1/off:0>"},
   #if KSOCKET_RTP_SUPPORT
   { "ksocket",	doMMKSocketSet,  CAT1, 1,	"dump MM related status",	"<show/enable/disable>"},
   #endif
   { "dsp",	doDsp,  CAT1|SUBCMD, 0,	"DSP related commands",	NULL},
   { NULL, NULL, NULL, NULL, NULL, NULL }
};

const cmds_t dspCmds[] =
{
   { "dspInit",	doDspInit,  CAT1, 0,	"Init dsp",	 NULL},
   { "dspDeinit",  doDspDeinit,  CAT1, 0,  "deinit dsp",	NULL},
   { "jitter",	doJBSet,  CAT1, 1,	"set Dsp Jitter Buffer size",	"line <fixed:1/adaptive:0> min max voiceTarget dataTarget"},
   { NULL, NULL, NULL, NULL, NULL, NULL }
};


const cmds_t callHistoryCmds[] =
{
   { "show",	doCallHistoryShow,  CAT1, 1,	"show call history",	"<calls/summary>"},
   { "clear",	doCallHistoryClear,  CAT1, 1,	"clear call history",	"<incalls/outcalls/allcalls/summary>"},
   { NULL, NULL, NULL, NULL, NULL, NULL }
};

/*******************************************************************************
 *
 * FUNCTION NAME : rip
 *
 * DESCRIPTION   : replace terminating end of line marker(s) with null
 *
 * RETURNS       : void
 *
 ******************************************************************************/
void
rip(
	char	*s
)
{
	char	*cp;

	if((cp = strchr(s,'\n')) != NULL) {
		*cp = '\0';
		if ( cp != s && *(--cp) == '\r' ) {
			*cp = '\0';
		}
	}
} /* rip */

/*******************************************************************************
 *
 * FUNCTION NAME : cmdparse
 *
 * DESCRIPTION   : This function is to parse a command line
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
cmdparse(
	cmds_t	cmds[],
	char	*line
)
{
	const cmds_t	*cmdp;
	char	*argv[NARG];
	char	*cp;
	int	argc;
	int	cmd_len;
	int i;
	
#ifdef ZyNOS
	char	PNCDbgStr[128];
	int	dbg_mode;
	int	qflag = FALSE;

	extern uint8 TelnetDbgFlag;
#endif
	
	/* Remove cr/lf */
	rip(line);

#ifdef ZyNOS
	if( sessionFlag == GUI_SESSION ) /* #dchen PNC debug */
	{
		memset(PNCDbgStr, 0, 128);
		strcpy(PNCDbgStr, "PNCCMD in:");
		strncpy(PNCDbgStr + strlen(PNCDbgStr), line, 127-strlen(PNCDbgStr) );
		sdcmdSyslogSend(SYSLOG_PNCLOG ,SYSLOG_DEBU, PNCDbgStr ); 
	}
#endif

	for ( argc = 0; argc < NARG; argc++ )
		argv[argc] = NULL;

	for ( argc = 0; argc < NARG; )
	{
	
#ifdef ZyNOS
		qflag = FALSE;
#endif

		/* Skip leading white space */
		while ( *line == ' ' || *line == '\t' )
			line++;
		if ( *line == '\0' )
			break;
		/* return if comment character first non-white */
		if ( argc == 0  &&  *line == '#' )
			return 0;
		
#ifdef ZyNOS
		/* Check for quoted token */
		if ( *line == '\"' )
		{
			line++; /* Suppress quote */
			qflag = TRUE;
		}
#endif

		/* Beginning of token */
		argv[argc++] = line;

#ifdef ZyNOS
		if ( qflag )
		{
			/* Find terminating delimiter */
			if ( (line = string_parse(line)) == NULL )
			{
				return ERR_STR_PARSE;
			}
		}
		else
		{
#endif
			/* Find space or tab. If not present,
			 * then we've already found the last
			 * token.
			 */
			if((cp = strchr(line,' ')) == NULL
			 && (cp = strchr(line,'\t')) == NULL){
				break;
			}
			*cp++ = '\0';
			line = cp;
#ifdef ZyNOS
		}
#endif

	}

	if (argc < 1)
	{
		/* empty command line */
		argc = 1;
		argv[0] = "";
	}

	for ( i = argc; (( i < (argc+5) ) && ( i < NARG )); i++ ) {
		argv[i]="";
	}

#ifdef ZyNOS
	/* Look up command in table; prefix matches are OK */
	dbg_mode = GetBootModDebug() && 0x01;
#endif

	if ( (cmd_len = zyStrlen(argv[0])) == 0 ) {
		ZyPrintf("Valid commands are:\n");
		cmd_show_valid(cmds);		
		return -1;
	}

	/*--------------------------------------------------------------*/
	for ( cmdp = cmds; cmdp->name != NULL; cmdp++ )
	{

		if ( strncmp(argv[0],cmdp->name,cmd_len) != 0 )
			continue;

#ifdef ZyNOS
#ifdef MULTI_USER_CONFIGURATION
        if(p != NULL && currentUserID == USER_CLIENT && (strcmp((char*)p,"Client Mode") == 0)){
	#ifdef TELENOR_2ACCOUNT_SIMLOGIN
            if (cmdp->flags & CAT_MODE_CLIENT)
	#else
            if (!(cmdp->flags & CAT_MODE_CLIENT))
	#endif
		{
				ZyPrintf("This command can not be used in client mode.\n");
				return -1;
			}
        }
#endif /* MULTI_USER_CONFIGURATION */
		if ( dbg_mode )
			break;
		if ( cliCmdCb.flags.oem )
		{
			if ( !(cmdp->flags & CAT3) )
				continue;
			break;
		}
		if ( cmdp->flags & CAT4 ) {
			if (!( cliCmdCb.flags.protect ) )
				break;
			continue;
		}
#endif
		
		if ( !(cmdp->flags & CAT2) )
			break;
	}

	if ( cmdp->name == NULL )
	{
	
#ifdef ZyNOS
		/* search extend ci-command table */
		if ( cmdStrucFind( cmd_root, argv[0], &cmdp ) != 0 )
		{
#if	defined(CMD_DEBUG)
if ( cliCmdCb.flags.debug )
  ZyPrintf("cmdparse#3: extend ci-command is not found\n" );
#endif
			if ( cmdp->argc_errmsg != NULL ) 
				ZyPrintf("%s\n",cmdp->argc_errmsg);
			else
				ZyPrintf("Valid commands are:\n");
			cmd_show_valid (cmd_root, cmds);		
			return 1;
		}
#ifdef MULTI_USER_CONFIGURATION
        if(p != NULL && currentUserID == USER_CLIENT && (strcmp((char*)p,"Client Mode") == 0)){
	#ifdef TELENOR_2ACCOUNT_SIMLOGIN
            if (cmdp->flags & CAT_MODE_CLIENT)
	#else
            if (!(cmdp->flags & CAT_MODE_CLIENT))
	#endif
			{
				ZyPrintf("This command can not be used in client mode.\n");
				return -1;
			}
        }
#endif /* MULTI_USER_CONFIGURATION */
#else
		ZyPrintf("Valid commands are:\n");
		cmd_show_valid(cmds);		
		return -1;
#endif
	}



	if( ((argc-1 < cmdp->argcmin) || 
		 ((argv[1]!= NULL ) && (!zyStrcmp(argv[1], "?"))) ||
		 ((argv[1]!= NULL ) && (!zyStrcmp(argv[1], "help")))) && 
		(!(cmdp->flags & SUBCMD)) && 
		(cmdp->flags & CAT1) )
	{
        	if ( cmdp->argc_errmsg != NULL ) 
		        ZyPrintf("Usage: %s\n",cmdp->argc_errmsg);
		else    ZyPrintf("Usage: no help\n");
       
		return -1;
	}
	if (cmdp->func == NULL)
	{
		ZyPrintf("Can't find command: argv [%s] cmdp->name [%s]\n", 
			argv[0],cmdp->name); 
		return -1;
	}

	return (*cmdp->func)(argc,argv);
} /* cmdparse */

/*******************************************************************************
 *
 * FUNCTION NAME : subcmd
 *
 * DESCRIPTION   : Call a subcommand based on the first token in an already-parsed line
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
subcmd(
    const cmds_t tab[],
	int	argc,
	char	*argv[]
)
{
	char	*error_msg = NULL;
	const cmds_t	*cmdp  = tab;
	int	found  = FALSE;
#ifdef ZyNOS
	char	*cmd0 = argv[0];	/* current command		*/
	int	dbg_mode;
#endif

	/* Strip off first token and pass rest of line to subcommand */
	if (argc <= 1)
	{				
		/* no subcommand */
		error_msg = "Missing subcommand. valid subcommands are:";
	}
	else if (argc > 1)
	{	/* subcommand */	
		argc--;
		argv++;
#ifdef ZyNOS
		dbg_mode = GetBootModDebug() && 0x01;
#endif
		for ( cmdp = tab; cmdp->name != NULL;cmdp++ )
		{
			if ( strncmp(argv[0],cmdp->name,zyStrlen(argv[0])) != 0 )
				continue;
			
#ifdef ZyNOS
#ifdef MULTI_USER_CONFIGURATION
            if(p != NULL && currentUserID == USER_CLIENT && (strcmp((char*)p,"Client Mode") == 0)){
	#ifdef TELENOR_2ACCOUNT_SIMLOGIN
            if (cmdp->flags & CAT_MODE_CLIENT)
	#else
            if (!(cmdp->flags & CAT_MODE_CLIENT))
	#endif
			    {
				    ZyPrintf("This command can not be used in client mode.\n");
				    return -1;
			    }
            }
#endif /* MULTI_USER_CONFIGURATION */
			if ( dbg_mode )
			{
				found = TRUE;
				break;
			}
			if ( cliCmdCb.flags.oem )
			{
				if ( cmdp->flags & CAT3 )
					found = TRUE;
				break;
			}
			if ( cmdp->flags & CAT4 ) {
				if (!( cliCmdCb.flags.protect ) ) {
					found = TRUE;
					break;
				}
				continue;
			}
#endif

			if ( !(cmdp->flags & CAT2) )
			{
				found = TRUE;
				break;
			}
		}
		/* invalid subcommand */
		if (cmdp->name == NULL && cmdp->argc_errmsg == NULL)
		{
		
#ifdef ZyNOS
			/* search extend ci-command table */
			if ( cmdStrucFind( cmd0, argv[0], &cmdp ) == OK )
			{
				found = TRUE;
			}
			else	
#endif

				error_msg = "Valid subcommands are:";
		}
	}
	if ( !found )
	{
#if 0
		/* Error message similar to cmdparse () */
		if( argc > 1)
		{
			/* search extend ci-command table */
			if ( cmdStrucFind( cmd0, argv[0], &cmdp ) == OK )
			{
				goto CMD_FOUND;
			}
		}
#endif	
		if ( error_msg != NULL ) 
		{
			ZyPrintf("%s\n", error_msg);
#ifdef ZyNOS
			cmd_show_valid(cmd0, tab);
#else
			cmd_show_valid(tab);
#endif
		}
		else if (cmdp->argc_errmsg != NULL)
			ZyPrintf ("%s\n", cmdp->argc_errmsg);
		return -1;
	}

CMD_FOUND:
	if( ((argc-1 < cmdp->argcmin) || 
		 ((argv[1]!= NULL ) && (!zyStrcmp(argv[1], "?"))) ||
		 ((argv[1]!= NULL ) && (!zyStrcmp(argv[1], "help")))) && 
		(!(cmdp->flags & SUBCMD)) && 
		(cmdp->flags & CAT1) )
	{
        	if ( cmdp->argc_errmsg != NULL ) 
		        ZyPrintf("Usage: %s\n",cmdp->argc_errmsg);
		else    ZyPrintf("Usage: no help\n");
		return -1;
	}
	if ( cmdp->func == NULL )
		return -1;

	return (*cmdp->func)(argc,argv);
} /* subcmd */

/*******************************************************************************
 *
 * FUNCTION NAME : cmd_show_valid
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
static int
cmd_show_valid(
	cmds_t	cmds[]
)
{
	cmds_t* cmd = NULL;

	for(cmd = cmds; cmd->name; cmd++){
		ZyPrintf("%s		%s\n", cmd->name, cmd->help);
	}
	return 0;
}

/*******************************************************************************
 *
 * FUNCTION NAME : doZyims
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : void
 *
 ******************************************************************************/
void
doZyims(
	char	*cmdLine
) {
	cmdparse(zyimsCmds, cmdLine);
} /* doZyims */

/*******************************************************************************
 *
 * FUNCTION NAME : doSip
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doSip(
	int	argc,
	char	*argv[]
) {
	return subcmd(sipCmds, argc, argv);
} /* doSip */

/*******************************************************************************
 *
 * FUNCTION NAME : doMm
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doMm(
	int	argc,
	char	*argv[]
) {
	return subcmd(mmCmds, argc, argv);
} /* doMm */

/*******************************************************************************
 *
 * FUNCTION NAME : doDsp
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doDsp(
	int	argc,
	char	*argv[]
) {
	return subcmd(dspCmds, argc, argv);
} /* doDsp */

/*******************************************************************************
 *
 * FUNCTION NAME : doFsm
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doFsm(
	int	argc,
	char	*argv[]
) {
	return subcmd(fsmCmds, argc, argv);
} /* doFsm */

/*******************************************************************************
 *
 * FUNCTION NAME : doPhone
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doPhone(
	int	argc,
	char	*argv[]
) {
	return subcmd(phoneCmds, argc, argv);
} /* doFsm */


/*******************************************************************************
 *
 * FUNCTION NAME : doCallHistory
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doCallHistory(
	int	argc,
	char	*argv[]
) {
	return subcmd(callHistoryCmds, argc, argv);
} /* doCallHistory */

/*******************************************************************************
 *
 * FUNCTION NAME : doFsmPhone
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doFsmPhone(
	int	argc,
	char	*argv[]
) {
	phoneCh_t *tmpPhoneCB = NULL;	
	tmpPhoneCB = voicePhoneCh.phoneChEntry;
	
	while ( tmpPhoneCB != NULL ) {
		ZyPrintf(" Physical-ID = %d,	 Logical-ID = %d,	Call-Id = %d \n\r", tmpPhoneCB->physicalId, tmpPhoneCB->logicalId, tmpPhoneCB->callId);
		ZyPrintf(" Type=%d,   Ph-St=%d,	 Link-St=%d  ", tmpPhoneCB->dspPortType,	tmpPhoneCB->phStatus, tmpPhoneCB->linkSt);
		ZyPrintf("		   flags=%x   \n\r", tmpPhoneCB->phoneFlags);

		
		ZyPrintf("           peerMediaChId = %d\n\r", tmpPhoneCB->peerMediaChId );
		ZyPrintf("		   firstDigitTimerId=%x   dialDigitTimer=%x   connConfTimer=%x\n\r",  tmpPhoneCB->callProTimer.firstDigitTimer, tmpPhoneCB->callProTimer.dialDigitTimer, tmpPhoneCB->callProTimer.connConfTimer);
		ZyPrintf("		   fxsCallerIDRingTimer=%x	 fxsflashRingTimer=%x\n\r",  tmpPhoneCB->callProTimer.fxsCallerIDRingTimer, tmpPhoneCB->callProTimer.fxsflashRingTimer);
		ZyPrintf("		   fxsFeatureKeyTimer=%x   fxoNonFlashCheckTimer=%x\n\r", tmpPhoneCB->callProTimer.fxsFeatureKeyTimer, tmpPhoneCB->callProTimer.fxoNonFlashCheckTimer);
		ZyPrintf("		   fxoDialDigitTimer1=%x   fxoDialDigitTimer2=%x\n", tmpPhoneCB->callProTimer.fxoDialDigitTimer1, tmpPhoneCB->callProTimer.fxoDialDigitTimer2);
		ZyPrintf("		   fxoConnCheckTimer=%x   \n\r",  tmpPhoneCB->callProTimer.fxoConnCheckTimer);

		ZyPrintf("		   dial_digits=%s\n\r", tmpPhoneCB->dialInform.dialDigitNumber.dial_digits);
		ZyPrintf("		   dial_length=%d  dialout_length=%d\n\r",	tmpPhoneCB->dialInform.dialDigitNumber.current_length, tmpPhoneCB->dialInform.dialDigitNumber.dialout_length);
		tmpPhoneCB = tmpPhoneCB->next;
	}

	return 0;
} /* doFsmPhone */

/*******************************************************************************
 *
 * FUNCTION NAME : doFsmCcm
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doFsmCcm(
	int	argc,
	char	*argv[]
) {
	ccmConn_t *tmpCcmConn_p = NULL;
	ZyPrintf("ccmConnCb.fxsCcmConn:\n\r");
	tmpCcmConn_p = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn_p != NULL ) {
		ZyPrintf("	memAddr=%x	 connSt=%d	 origPortId=%d	 origPortType=%d\n\r", tmpCcmConn_p, tmpCcmConn_p->connSt, tmpCcmConn_p->origPortId, tmpCcmConn_p->origPortType);
		ZyPrintf("		   flags=%x   connRole=%d	connType=%d   SipFxsMap=%d\n\r", tmpCcmConn_p->flags, tmpCcmConn_p->connRole, tmpCcmConn_p->connType, tmpCcmConn_p->SipFxsMap );	
		ZyPrintf("		   dial_digits=%s\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.dial_digits);
		ZyPrintf("		   dial_length=%d	dial_flags=%x\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.current_length, tmpCcmConn_p->dialInform.dialDigitNumber.dialDigitFlags);
		ZyPrintf("		   func_id=%d	  peer=%x\n\r			bundle.prev=%x	 bundle.next=%x\n\r", tmpCcmConn_p->origFuncId, tmpCcmConn_p->peerCcmConn,
			(tmpCcmConn_p->bundleCcmList.bundleCcmPrev==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmPrev, (tmpCcmConn_p->bundleCcmList.bundleCcmNext==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmNext );
		ZyPrintf("		   callFeatures=%x\n\r", tmpCcmConn_p->callFeatures);
		if ( tmpCcmConn_p->callTransferCB != NULL ) {
			ZyPrintf("		   callTransferCB=%x\n\r", tmpCcmConn_p->callTransferCB);
		}
		if ( tmpCcmConn_p->ConfCallCB != NULL ) {		/* CALL_CONFERENCE */
			ZyPrintf("		   ConfCallCB=%x\n\r", tmpCcmConn_p->ConfCallCB);
		}
		tmpCcmConn_p = tmpCcmConn_p->next;
	}

	ZyPrintf("ccmConnCb.fxoCcmConn:\n\r");
	tmpCcmConn_p = ccmConnCb.fxoCcmConn;
	while ( tmpCcmConn_p != NULL ) {
		ZyPrintf("	memAddr=%x	 connSt=%d	 origPortId=%d	 origPortType=%d\n\r", tmpCcmConn_p, tmpCcmConn_p->connSt, tmpCcmConn_p->origPortId, tmpCcmConn_p->origPortType);
		ZyPrintf("		   flags=%x   connRole=%d	connType=%d   SipFxsMap=%d\n\r", tmpCcmConn_p->flags, tmpCcmConn_p->connRole, tmpCcmConn_p->connType, tmpCcmConn_p->SipFxsMap );
		ZyPrintf("		   dial_digits=%s\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.dial_digits);
		ZyPrintf("		   dial_length=%d	dial_flags=%x\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.current_length, tmpCcmConn_p->dialInform.dialDigitNumber.dialDigitFlags);
		ZyPrintf("		   func_id=%d	  peer=%x\n\r			bundle.prev=%x	  bundle.next=%x\n\r", tmpCcmConn_p->origFuncId, tmpCcmConn_p->peerCcmConn,
			(tmpCcmConn_p->bundleCcmList.bundleCcmPrev==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmPrev, (tmpCcmConn_p->bundleCcmList.bundleCcmNext==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmNext );
		ZyPrintf("		   callFeatures=%x\n\r", tmpCcmConn_p->callFeatures);
		if ( tmpCcmConn_p->callTransferCB != NULL ) {
			ZyPrintf("		   callTransferCB=%x\n\r", tmpCcmConn_p->callTransferCB);
		}
		if ( tmpCcmConn_p->ConfCallCB != NULL ) {	/* CALL_CONFERENCE */
			ZyPrintf("		   ConfCallCB=%x\n\r", tmpCcmConn_p->ConfCallCB);
		}
		tmpCcmConn_p = tmpCcmConn_p->next;
	}

	ZyPrintf("ccmConnCb.zyxelCcmConn:\n\r");
	tmpCcmConn_p = ccmConnCb.zyxelCcmConn;
	while ( tmpCcmConn_p != NULL ) {
		ZyPrintf("	memAddr=%x	 connSt=%d	 origPortId=%d	 origPortType=%d\n\r", tmpCcmConn_p, tmpCcmConn_p->connSt, tmpCcmConn_p->origPortId, tmpCcmConn_p->origPortType);
		ZyPrintf("		   flags=%x   connRole=%d	connType=%d   SipFxsMap=%d\n\r", tmpCcmConn_p->flags, tmpCcmConn_p->connRole, tmpCcmConn_p->connType, tmpCcmConn_p->SipFxsMap );
		ZyPrintf("		   dial_digits=%s\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.dial_digits);
		ZyPrintf("		   dial_length=%d	dial_flags=%x\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.current_length, tmpCcmConn_p->dialInform.dialDigitNumber.dialDigitFlags);
		ZyPrintf("		   func_id=%d	  peer=%x\n\r			bundle.prev=%x	  bundle.next=%x\n\r", tmpCcmConn_p->origFuncId, tmpCcmConn_p->peerCcmConn,
			(tmpCcmConn_p->bundleCcmList.bundleCcmPrev==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmPrev, (tmpCcmConn_p->bundleCcmList.bundleCcmNext==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmNext );
		ZyPrintf("		   callFeatures=%x\n\r", tmpCcmConn_p->callFeatures);
		if ( tmpCcmConn_p->callTransferCB != NULL ) {
			ZyPrintf("		   callTransferCB=%x\n\r", tmpCcmConn_p->callTransferCB);
		}
		if ( tmpCcmConn_p->ConfCallCB != NULL ) {	/* CALL_CONFERENCE */
			ZyPrintf("		   ConfCallCB=%x\n\r", tmpCcmConn_p->ConfCallCB);
		}
		tmpCcmConn_p = tmpCcmConn_p->next;
	}

	ZyPrintf("ccmConnCb.sipCcmConn:\n\r");
	tmpCcmConn_p = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn_p != NULL ) {
		ZyPrintf("	memAddr=%x	 connSt=%d	 origPortId=%d	 origPortType=%d\n\r", tmpCcmConn_p, tmpCcmConn_p->connSt, tmpCcmConn_p->origPortId, tmpCcmConn_p->origPortType);
		ZyPrintf("		   flags=%x   connRole=%d	connType=%d   SipFxsMap=%d\n\r", tmpCcmConn_p->flags, tmpCcmConn_p->connRole, tmpCcmConn_p->connType, tmpCcmConn_p->SipFxsMap);
		ZyPrintf("		   dial_digits=%s\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.dial_digits);
		ZyPrintf("		   dial_length=%d	dial_flags=%x\n\r", tmpCcmConn_p->dialInform.dialDigitNumber.current_length, tmpCcmConn_p->dialInform.dialDigitNumber.dialDigitFlags);
		ZyPrintf("		   func_id=%d	  peer=%x\n\r			bundle.prev=%x	 bundle.next=%x\n\r", tmpCcmConn_p->origFuncId, tmpCcmConn_p->peerCcmConn,
			(tmpCcmConn_p->bundleCcmList.bundleCcmPrev==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmPrev, (tmpCcmConn_p->bundleCcmList.bundleCcmNext==NULL)?0:tmpCcmConn_p->bundleCcmList.bundleCcmNext );
		ZyPrintf("		   callFeatures=%x\n\r", tmpCcmConn_p->callFeatures);
		if ( tmpCcmConn_p->callTransferCB != NULL ) {
			ZyPrintf("		   callTransferCB=%x\n\r", tmpCcmConn_p->callTransferCB);
		}
		if ( tmpCcmConn_p->ConfCallCB != NULL ) {	/* CALL_CONFERENCE */
			ZyPrintf("		   ConfCallCB=%x\n\r", tmpCcmConn_p->ConfCallCB);
		}
		tmpCcmConn_p = tmpCcmConn_p->next;
	}

	return 0;
} /* doFsmCcm */

/*******************************************************************************
 *
 * FUNCTION NAME : doFsmConvert
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doFsmConvert(
	int	argc,
	char	*argv[]
) {
	sipCCMConvertEntry_t *tmpEntry_p = NULL;
	ZyPrintf("sipCCMConvert.totalNum = %d\n\r", sipCCMConvert.sipCCMConvertNum);

	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		tmpEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpEntry_p != NULL ) {
			ZyPrintf("sipCcmPortId=%d, RoleSt=%d, flags=%x, callId=%d, SipAccountId=%d, VpItr=%d\n\r", 
				tmpEntry_p->sipCcmPortId, tmpEntry_p->RoleSt, tmpEntry_p->flags, tmpEntry_p->callId, tmpEntry_p->SipAccountId, tmpEntry_p->VpItr);
			tmpEntry_p = tmpEntry_p->next;
		}
	}

	return 0;
} /* doCallHistory */


/*******************************************************************************
 *
 * FUNCTION NAME : doActive
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doActive(
	int	argc,
	char	*argv[]
) {
	char activeChar[] = "on";
	char deactiveChar[] = "off";

	if(argc == 2) {
		if ( strncmp(argv[1], &activeChar[0], 2) == 0 ) {
			if(TRUE == voipActivate()) {
				ZyPrintf("VoIP service activated\n");
			}
		} else if( strncmp(argv[1], &deactiveChar[0], 3) == 0 ) {
			if(TRUE == voipDeactivate()) {
				ZyPrintf("VoIP service deactivated\n");
			}
		} else {
			ZyPrintf("Command usage: on/off\n");
		}
	}
	return 0;
} /* doActive */

/*******************************************************************************
 *
 * FUNCTION NAME : doDebug
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doDebug(
	int	argc,
	char	*argv[]
) {
	int icfdbg=0;
	if(argc == 3) {
		/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
		if(strncmp(argv[1], "dest", sizeof("dest")) == 0){
			log_dest = atoi(argv[2]);
		} else if(strncmp(argv[1], "voiceApp", sizeof("voiceApp")) == 0){
			voiceAppDebug = atoi(argv[2]);
		} else if(strncmp(argv[1], "ccm", sizeof("ccm")) == 0){
			ccmDebug = atoi(argv[2]);
		} else if(strncmp(argv[1], "phone", sizeof("phone")) == 0){
			phoneDebug = atoi(argv[2]);
		} else if(strncmp(argv[1], "sip", sizeof("sip")) == 0){
			sipDebug = atoi(argv[2]);
  		} else if(strncmp(argv[1], "tr104", sizeof("tr104")) == 0){
			Tr104Debug = atoi(argv[2]);
  		}else if(strncmp(argv[1], "all", sizeof("all")) == 0){
			voiceAppDebug = atoi(argv[2]);
			ccmDebug = atoi(argv[2]);
			phoneDebug = atoi(argv[2]);
			sipDebug = atoi(argv[2]);
			Tr104Debug = atoi(argv[2]);
		}else if(strncmp(argv[1], "icf", sizeof("icf")) == 0){
			icfdbg = atoi(argv[2]);
			voiceAppSetTrace(icfdbg);				
		}
	}
	ZyPrintf("debug flag status:\n");	
	ZyPrintf("log_dest: %d\n", log_dest);
	ZyPrintf("voiceAppDebug: %d\n", voiceAppDebug);
	ZyPrintf("ccmDebug: %d\n", ccmDebug);
	ZyPrintf("phoneDebug: %d\n", phoneDebug);
	ZyPrintf("sipDebug: %d\n", sipDebug);
	ZyPrintf("Tr104Debug: %d\n", Tr104Debug);
	ZyPrintf("icfDebug: %d\n", icfdbg);	

	return 0;
} /* doDebug */

/*******************************************************************************
 *
 * FUNCTION NAME : doZyimsShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doZyimsShow(
	int	argc,
	char	*argv[]
) {
	tr104DumpAll();
	return 0;
} /* doZyimsShow */

/*******************************************************************************
 *
 * FUNCTION NAME : doTest
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/

int
doFxoTest(
	int	argc,
	char	*argv[]
) {
	phoneCh_t *phoneCB = NULL;
	phoneCB=find_phoneCB(2,1);
	ZyPrintf("doFxoTest argc = %d\n", argc);
	if(NULL!=phoneCB){		
		
		if(!strncmp(argv[1], "onhook",strlen("onhook"))){
			ZyPrintf("Test onhook\n");
			PhonePSTNOnhook(phoneCB);
		}
		else if(!strncmp(argv[1], "offhook",strlen("offhook"))){
			ZyPrintf("Test offhook");
			if(argv>2){
				ZyPrintf("number %s\n",argv[2]);
				memcpy(phoneCB->dialInform.dialDigitNumber.dial_digits, argv[2], strlen(argv[2]));
				PhonePSTNOutgoing(phoneCB);
			}
			else{
				PhonePSTNOffhook(phoneCB);		
			}
		
		}
		else if(!strncmp(argv[1], "setrelay",strlen("setrelay"))){
			ZyPrintf("set relay");
			if(argv>=2){
				ZyPrintf("parm %s\n",argv[2]);
				PhonePSTNSetRelay(atoi(argv[2]));
			}					
			
		}
	}
	else{
		ZyPrintf("cannot find phoneCB\n");
	}

	return 0;
} /* doTest */

/*******************************************************************************
 *
 * FUNCTION NAME : doSipSysShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doSipSysShow(
	int	argc,
	char	*argv[]
) {
	if (FALSE == voiceAppGetSysSettings()) {
		ZyPrintf("GET_SYSTEM_SETTINGS_REQ towards IP Phone Toolkit Failed\n");
		return -1;
	}
	return 0;
} /* doSipShow */

/*******************************************************************************
 *
 * FUNCTION NAME : doSipLineShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doSipLineShow(
	int	argc,
	char	*argv[]
) {
	uint16 lineId = 0;
	
	if(argc == 2) {
		lineId = atoi(argv[1]);
		if (FALSE == voiceAppGetLineSettings(lineId)) {
			ZyPrintf("GET_LINE_SETTINGS_REQ towards IP Phone Toolkit Failed\n");
			return -1;
		}
	}
	return 0;
} /* doSipLineShow */

/*******************************************************************************
 *
 * FUNCTION NAME : doSipRegShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doSipRegShow(
	int	argc,
	char	*argv[]
) {
	uint16 lineId = 0;
	
	if(argc == 2) {
		lineId = atoi(argv[1]);
		if (FALSE == voiceAppGetRegisterStatus(lineId)) {
			ZyPrintf("GET_REGISTER_STATUS_REQ towards IP Phone Toolkit Failed\n");
			return -1;
		}
	}
	return 0;
} /* doSipRegShow */

//Ssu-Ying, support Multiple Profile
/*******************************************************************************
 *
 * FUNCTION NAME : doSipRegister
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doSipRegister(
	int	argc,
	char	*argv[]
) {
	uint16 SipAccountId = 0;
	uint16 LineItr = 0, VpItr=0;		
	if(argc == 2) {
		SipAccountId = atoi(argv[1]);
		VpItr = mapSipAccountIdToVpItr(SipAccountId);
		LineItr = mapSipAccountIdToLineItr(SipAccountId);		
		VoiceProfLineObj_t* lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		VOICEAPP_PRINTF("doSipRegister: VoIPIsActive is %u ,SipAccountId (%d)\n",VoIPIsActive ,SipAccountId);	
		/*[RD Internal] HGW shouldn't register sip account if account is "changeme".,__CHT__,Mitrastar Jason,20120611*/	
		if(lineObj!=NULL && lineObj->enable && strcmp(lineObj->directoryNumber,"changeme")&& ICFLineInitDone[SipAccountId]){		
			/*Stop Timer*/
			if(reg_timer[SipAccountId].regRetryTimer!= 0){
				VOICEAPP_PRINTF("Stop regRetryTimer %d for SipAccountId %d.\n",reg_timer[SipAccountId].regRetryTimer, SipAccountId);						
				timerStop(reg_timer[SipAccountId].regRetryTimer);
				reg_timer[SipAccountId].regRetryTimer = 0;
			}
			
			if(reg_timer[SipAccountId].subscribeTimer!= 0){
				VOICEAPP_PRINTF("Stop subscribeTimer %d for SipAccountId %d.\n",reg_timer[SipAccountId].subscribeTimer, SipAccountId);						
				timerStop(reg_timer[SipAccountId].subscribeTimer);
				reg_timer[SipAccountId].subscribeTimer = 0;
			}			
		
		if (FALSE == voiceAppSendRegister(SipAccountId)) {
			ZyPrintf("REGISTER_REQ towards IP Phone Toolkit Failed\n");
			return -1;
		}
		}else{
			if(!ICFLineInitDone[SipAccountId]){
				VOICEAPP_PRINTF("ICFLineInitDone[%d] is 0!!\n",SipAccountId);			
			}if(lineObj==NULL){
				VOICEAPP_PRINTF("lineObj is NULL!!\n");
			}else{
				VOICEAPP_PRINTF("enable : %d, directoryNumber is %s\n", lineObj->enable, lineObj->directoryNumber);
			} 
			VOICEAPP_PRINTF("Don't send register req in this case!!\n");		
		}
	}
	return 0;
} /* doSipRegister */

/*******************************************************************************
 *
 * FUNCTION NAME : doSipDeregister
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doSipDeregister(
	int	argc,
	char	*argv[]
) {
	uint16 SipAccountId = 0;
	uint16 LineItr = 0, VpItr=0;
	
	if(argc == 2) {
		SipAccountId = atoi(argv[1]);
		VpItr = mapSipAccountIdToVpItr(SipAccountId);
		LineItr = mapSipAccountIdToLineItr(SipAccountId);
		VoiceProfLineObj_t* lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
		VOICEAPP_PRINTF("doSipDeregister: SipAccountId = %d\n", SipAccountId);	
		/*[RD Internal] HGW shouldn't register sip account if account is "changeme".,__CHT__,Mitrastar Jason,20120611*/	
		if(lineObj!=NULL && lineObj->enable && lineObj->status == TR104I_STATUS_UP){
			/*Stop Timer*/
			if(reg_timer[SipAccountId].regRetryTimer!= 0){
				VOICEAPP_PRINTF("Stop regRetryTimer %d for SipAccountId %d.\n",reg_timer[SipAccountId].regRetryTimer, SipAccountId);						
				timerStop(reg_timer[SipAccountId].regRetryTimer);
				reg_timer[SipAccountId].regRetryTimer = 0;
			}
			
			if(reg_timer[SipAccountId].subscribeTimer!= 0){
				VOICEAPP_PRINTF("Stop subscribeTimer %d for SipAccountId %d.\n",reg_timer[SipAccountId].subscribeTimer, SipAccountId);						
				timerStop(reg_timer[SipAccountId].subscribeTimer);
				reg_timer[SipAccountId].subscribeTimer = 0;
			}			
					
			if (FALSE == voiceAppSendDeRegister(SipAccountId)){
				ZyPrintf("DEREGISTER_REQ towards IP Phone Toolkit Failed\n");
				return -1;
			}
		}else{
			if(lineObj==NULL){
				VOICEAPP_PRINTF("lineObj is NULL!!\n");
			}else{
				VOICEAPP_PRINTF("enable : %d, directoryNumber is %s\n", lineObj->enable, lineObj->directoryNumber);
			} 
			VOICEAPP_PRINTF("Don't send de-register req in this case!!\n");	
		}	
	}
	return 0;
} /* doSipDeregister */

/*******************************************************************************
 *
 * FUNCTION NAME : doCallHistoryShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doCallHistoryShow(
	int	argc,
	char	*argv[]
) {
	char * target = NULL;
	
	if(argc == 2) {
		target = argv[1];
		if(!strncmp(target, "calls",5)){
			if(0 != webCallHistoryDisplayEntry()){
				ZyPrintf("webCallHistoryDisplayEntry Failed\n");
			}
		}
		else if(!strncmp(target, "summary", 7)){
			if(0 != webCallHistoryDisplaySummary()){
				ZyPrintf("webCallHistoryDisplaySummary Failed\n");
			}
		}
	}
	return 0;
} /* doCallHistoryShow */

/*******************************************************************************
 *
 * FUNCTION NAME : doCallHistoryClear
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doCallHistoryClear(
	int	argc,
	char	*argv[]
) {
	char * target = NULL;
	
	if(argc == 2) {
		target = argv[1];
		if(!strncmp(target, "incalls", 7)){
			if(0 != webCallHistoryClearInCalls()){
				ZyPrintf("webCallHistoryClearInCalls Failed\n");
			}
			else{
				ZyPrintf("clear complete\n");
			}
		}
		else if(!strncmp(target, "outcalls", 8)){
			if(0 != webCallHistoryClearOutCalls()){
				ZyPrintf("webCallHistoryClearInCalls Failed\n");
			}
			else{
				ZyPrintf("clear complete\n");
			}
		}
		else if(!strncmp(target, "allcalls", 8)){
			if(0 != webCallHistoryClearInCalls()){
				ZyPrintf("webCallHistoryClearInCalls Failed\n");
			}
			else{
				ZyPrintf("incalls clear complete\n");
			}
			if(0 != webCallHistoryClearOutCalls()){
				ZyPrintf("webCallHistoryClearInCalls Failed\n");
			}
			else{
				ZyPrintf("outcalls clear complete\n");
			}
		}
		else if(!strncmp(target, "summary", 7)){
			if(0 != webCallHistorySummaryClearAll()){
				ZyPrintf("webCallHistorySummaryClearAll Failed\n");
			}
			else{
				ZyPrintf("clear complete\n");
			}
		}
	}
	return 0;
} /* doCallHistoryClear */



/*******************************************************************************
 *
 * FUNCTION NAME : doMMStatusShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doMMStatusShow(
	int	argc,
	char	*argv[]
) {
	voiceMsg_t *p_msg,*p_rsp_msg = NULL;
	mm_debug_status_req_st *p_payload = NULL;
	int ret_val = 0;

	if(argc == 2) {
		p_msg = voiceMsgAlloc(VOICE_MM_DEBUG_STATUS_REQ,sizeof(mm_debug_status_req_st));
		p_payload=(mm_debug_status_req_st *)p_msg->payload;


		if(strncmp(argv[1], "rm", sizeof("rm")) == 0){
			p_payload->status_rm = 1;
			ret_val = 1;
		}
		else if(strncmp(argv[1], "rtp", sizeof("rtp")) == 0){
			p_payload->status_rtp = 1;
			ret_val = 1;
		}
		else if(strncmp(argv[1], "dsp", sizeof("dsp")) == 0){
			p_payload->status_dsp = 1;
			ret_val = 1;
		}
		else if(strncmp(argv[1], "ivr", sizeof("ivr")) == 0){
			p_payload->status_ivr = 1;
			ret_val = 1;
		}

		if(1 == ret_val){
			if(eventVoiceDspDataSend((uint8*)p_msg ,(uint8 **)&p_rsp_msg) != 0){
				ZyPrintf("eventVoiceDspDataSend failed\n");
			}

			voiceMsgFree(p_msg);	
			voiceMsgFree(p_rsp_msg); 
		}
	}

	return 0;
} /* doMMStatusShow */

/*******************************************************************************
 *
 * FUNCTION NAME : doMMDebugShow
 *
 * DESCRIPTION   : 
 *
 * RETURNS       : int
 *
 ******************************************************************************/
int
doMMSetDebug(
	int	argc,
	char	*argv[]
) {
	voiceMsg_t *p_msg,*p_rsp_msg = NULL;
	mm_set_debug_req_st *p_payload = NULL;
	int ret_val = 0;

	if(argc == 3) {

		p_msg = voiceMsgAlloc(VOICE_MM_SET_DEBUG_REQ,sizeof(mm_set_debug_req_st));
		p_payload=(mm_set_debug_req_st *)p_msg->payload;

		/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
		if(strncmp(argv[1], "dest", sizeof("dest")) == 0){
			p_payload->dest = atoi(argv[2]);
			ret_val = 1;
		}

		if(strncmp(argv[1], "rm", sizeof("rm")) == 0){
			p_payload->iptk_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_IPTK_DEBUG;
			ret_val = 1;
		}

		if(strncmp(argv[1], "rtp", sizeof("rtp")) == 0){
			p_payload->rtp_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_RTP_DEBUG;
			ret_val = 1;
		}

		if(strncmp(argv[1], "dsp", sizeof("dsp")) == 0){
			p_payload->dsp_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_DSP_DEBUG;
			ret_val = 1;
		}

		if(strncmp(argv[1], "ivr", sizeof("ivr")) == 0){
			p_payload->ivr_debug= atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_IVR_DEBUG;
			ret_val = 1;
		}

		if(strncmp(argv[1], "all", sizeof("all")) == 0){
			p_payload->iptk_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_IPTK_DEBUG;
			p_payload->rtp_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_RTP_DEBUG;
			p_payload->dsp_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_DSP_DEBUG;
			p_payload->ivr_debug = atoi(argv[2]);
			p_payload->bit_mask |= MM_SET_IVR_DEBUG;
			ret_val = 1;
		}

		if(1 == ret_val){
			if(eventVoiceDspDataSend((uint8*)p_msg ,(uint8 **)&p_rsp_msg) != 0){
				ZyPrintf("eventVoiceDspDataSend failed\n");
		}

		voiceMsgFree(p_msg);	
		voiceMsgFree(p_rsp_msg); 
	}
	}

	return 0;
} /* doMMDebugShow */

#if KSOCKET_RTP_SUPPORT

/*******************************************************************************
 *
 * FUNCTION NAME : doMMKSocketSet
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
int
doMMKSocketSet(
	int argc,
	char	*argv[]
) {
	voiceMsg_t *p_msg,*p_rsp_msg = NULL;
	mm_ksocket_set_req_st *p_payload = NULL;
	
	if(argc == 2) {
		p_msg = voiceMsgAlloc(VOICE_MM_KSOCKET_SET_REQ,sizeof(mm_ksocket_set_req_st));
		p_payload=(mm_ksocket_set_req_st *)p_msg->payload;

		if(strncmp(argv[1], "enable", sizeof("enable")) == 0){
			p_payload->enable = 1;			
		}
		else if(strncmp(argv[1], "disable", sizeof("disable")) == 0){
			p_payload->enable = 0;			
		}
		else if(strncmp(argv[1], "show", sizeof("show")) == 0){
			ZyPrintf("KSocket enable = %d\n", mmKSocketEnable);
			PhoneShowKSocket();
			return 0;
		}
		else{
			ZyPrintf("wrong argument\n");
			return 0;
		}
		
		ZyPrintf("set KSocket enable = %d\n", p_payload->enable);

		if(!isVoIPinUsed()){
			if(eventVoiceDspDataSend((uint8*)p_msg ,(uint8 **)&p_rsp_msg) != 0){
				ZyPrintf("eventVoiceDspDataSend failed\n");
			}
			else{
				mmKSocketEnable = p_payload->enable;
				ZyPrintf("set Ksocket success\n");
			}
			
			voiceMsgFree(p_msg);	
			voiceMsgFree(p_rsp_msg); 
		}
		else{
			ZyPrintf("VoIP is in used, could not set KSocket enable\n");
		}
	}

	return 0;
} /* doMMKSocketSet */

#endif

/*******************************************************************************
 *
 * FUNCTION NAME : doDspInit
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
int
doDspInit(
	int argc,
	char	*argv[]
) {	
	ZyPrintf("execute DSP init\n");
	voiceDspInit();

	return 0;
} /* doJBSet */

/*******************************************************************************
 *
 * FUNCTION NAME : doDspDeinit
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
int
doDspDeinit(
	int argc,
	char	*argv[]
) {	
	ZyPrintf("execute DSP deinit\n");
	voiceDspDeinit();
	return 0;
} /* doJBSet */



/*******************************************************************************
 *
 * FUNCTION NAME : doJBSet
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
int
doJBSet(
	int argc,
	char	*argv[]
) {	
	int line = 0, type = 0, min = 0, max = 0, voiceTarget = 0, dataTarget = 0;
	
	if(argc == 7) {
		line = atoi(argv[1]);
		type = atoi(argv[2]);
		min = atoi(argv[3]);
		max = atoi(argv[4]);
		voiceTarget = atoi(argv[5]);
		dataTarget = atoi(argv[6]);
		ZyPrintf("line = %d\n", line);
		if(type){
			ZyPrintf("type = FIXED\n", line);
		}
		else{
			ZyPrintf("type = ADAPTIVE\n");
		}
		ZyPrintf("min = %d\n", min);
		ZyPrintf("max = %d\n", max);
		ZyPrintf("voiceTarget = %d\n", voiceTarget);
		ZyPrintf("dataTarget = %d\n", dataTarget);
		
		PhoneJBSet(line, type, min, max, voiceTarget, dataTarget);
	}
	else{
		ZyPrintf("argument number not match, %d\n", argc);
	}

	return 0;
} /* doJBSet */

/*******************************************************************************
 *
 * FUNCTION NAME : doFlashInterval
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
int
doFlashInterval(
	int argc,
	char	*argv[]
) {	
	int phoneId = 0;
	uint32 min = 0, max = 0 ;
	VoicePhyIntfObj_t *phyIntfObj = NULL;
	if(argc < 2) {
		ZyPrintf("usage: flashInterval show <phoneId>\n");
		ZyPrintf("       flashInterval set <phoneId> <min> <max>\n");
		fflush(stdout);
	}

	if(strcmp(argv[1], "show")==0){
		if(argc<3){
			ZyPrintf("usage: flashInterval show <phoneId>\n");
			fflush(stdout);
			return 0;
		}
		phoneId = atoi(argv[2]);

		phyIntfObj = tr104VoicePhyIntfObjPtrGet(phoneId);		
		if(NULL != phyIntfObj){			
			ZyPrintf("flash interval for phoneId %d is %d-%d\n", phoneId, phyIntfObj->ZyXEL_FlashMinInterval, phyIntfObj->ZyXEL_FlashMaxInterval);

		}
		else{
			ZyPrintf("fail to save to tr104 phyIntf, cannot get phyIntf obj \n");
		}

		if(PhoneFlashIntervalGet(phoneId, &min, &max)==0){
			ZyPrintf("Provision get from driver, flash interval for phoneId %d is %d-%d ms.\n", phoneId, min, max);
		}
		else{
			ZyPrintf("fail to get flash interval for phoneId %d\n", phoneId);
		}
		
	}
	else if(strcmp(argv[1], "set")==0){
		if(argc<5){
			ZyPrintf("usage: flashInterval set <phoneId> <min> <max>\n");
			return 0;
		}

		phoneId = atoi(argv[2]);		
		min = atoi(argv[3]);
		max = atoi(argv[4]);
		if(min > max){
			ZyPrintf("min value(%d) should bigger than the max value(%d) \n", min, max);
			fflush(stdout);
			return 0;
		}

		if(max!=0){
			ZyPrintf("set for phoneId %d, flash interval %d-%d ms\n", phoneId, min, max);	
			if(PhoneFlashIntervalSet(phoneId, min, max) == 0){
				ZyPrintf("flash interval set successfully.\n");
			}
			else{
				ZyPrintf("Can not set flash interval for phoneId %d\n", phoneId);
			}
		}
		
		phyIntfObj = tr104VoicePhyIntfObjPtrGet(phoneId);
		if(NULL != phyIntfObj){
			/*max and min to 0, need to reboot to use country code flash interval*/
			if( (max!= 0) && ((phyIntfObj->ZyXEL_FlashMaxInterval!=max) || (phyIntfObj->ZyXEL_FlashMinInterval!=min)) ){
				dspReinitNeeded = 1;
			}
			phyIntfObj->ZyXEL_FlashMaxInterval = max;
			phyIntfObj->ZyXEL_FlashMinInterval = min;
			if((max == min) && (min == 0)){
				ZyPrintf("Set the user-defined flash interval to 0, please reboot to use country-code flash interval.\n");
			}
			tr104VoicePhyIntfObjPtrSet(phoneId, phyIntfObj);
			
		}
		else{
			ZyPrintf("fail to save to tr104 phyIntf, cannot get phyIntf obj \n");
		}
	}
	else{
		ZyPrintf("usage: flashInterval show <phoneId>\n");
		ZyPrintf("       flashInterval set <phoneId> <min> <max>\n");
	}
		
	fflush(stdout);
	return 0;
} /* doFlashInterval */

#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
/*******************************************************************************
 *
 * FUNCTION NAME : doPhoneAutoTest
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
int
doPhoneAutoTest(
	int argc,
	char	*argv[]
) {	
	int phoneIdOne = 0;
	int phoneIdTwo = 1;
	
	phoneCh_t *phoneCBOne = NULL;
	phoneCh_t *phoneCBTwo = NULL;
	
	if(argc < 2) {
		ZyPrintf("usage: autoTest <test_digit>\n");
		fflush(stdout);
	}

	if(strcmp(argv[1], "show")==0){
		ZyPrintf("autoTest 1234\n");
	}
	else{
		/* save test_digit */
		memset(VoIP_autoTest.autoTest_digits,0,MAX_DIAL_STRING_LENGTH);
		if( NULL != argv[1] )
			strcpy(VoIP_autoTest.autoTest_digits,argv[1]);
		else
			strcpy(VoIP_autoTest.autoTest_digits,"1234");
		
		phoneCBOne = find_phoneCB(phoneIdOne, 1);
		if(NULL == phoneCBOne){
			ZyPrintf("Can not find physical phone %d\n", phoneIdOne);
			return 0;
		}else{
			/* change state to autotest */
			phoneCBOne->phStatus = PHONE_STATUS_TEST;
		
			/* start timer - ring detect timeout */
			VoIP_autoTest.fxsAutoTestTimerOne = timerStartS(10000, fxsRingTimeout, phoneCBOne);
			VoIP_autoTest.isFXSONESucc = TRUE;
			
			/* Start Ring */
			PhoneStartRingForAutoTest(phoneCBOne);
		}
		
		phoneCBTwo = find_phoneCB(phoneIdTwo, 1);
		if(NULL != phoneCBTwo){
			/* change state to autotest */
			phoneCBTwo->phStatus = PHONE_STATUS_TEST;
		
			/* start timer - ring detect timeout */
			VoIP_autoTest.fxsAutoTestTimerTwo = timerStartS(10000, fxsRingTimeout, phoneCBTwo);
			VoIP_autoTest.isFXSTWOSucc = TRUE;
		
			/* Start Ring */
			PhoneStartRingForAutoTest(phoneCBTwo);
		}else{
			ZyPrintf("Can not find physical phone %d, skip\n", phoneIdTwo);
		}
		
	}	
		
	fflush(stdout);
	return 0;
} /* doAutoTest */

static int
fxsRingTimeout(
	tm_id_t timer_id,
	void	*arg
) {
	phoneCh_t *tmpPhoneCB = (phoneCh_t *)arg;
	
	ZyPrintf("phoneCB->physicalId %d Ring timeout!!!!\n",tmpPhoneCB->physicalId);
	
	if( tmpPhoneCB->physicalId == 0 )
	{
		system("echo \"FXS1 Ring timeout!!\"> /tmp/autofxs1");
	}else{
		system("echo \"FXS2 Ring timeout!!\"> /tmp/autofxs2");
	}
	
	/* stop ring reset state*/
	tmpPhoneCB->phStatus = PHONE_STATUS_IDLE;
	slicStopRing(tmpPhoneCB);
	
	return 0;
}
#endif

/*******************************************************************************
 *
 * FUNCTION NAME : doPhoneToneTest
 *
 * DESCRIPTION	 : 
 *
 * RETURNS		 : int
 *
 ******************************************************************************/
void printPhoneToneTestUsage(void){
	ZyPrintf("usage: toneTest show -- show tone id\n");
	ZyPrintf("	   toneTest <phoneId> <tone id> <caller id - for call waiting>\n");
}

int
doPhoneToneTest(
	int argc,
	char	*argv[]
) {	
	int phoneId = 0, toneId =0;
	phoneCh_t *phoneCB = NULL;
	if(argc < 2) {
		printPhoneToneTestUsage();
		fflush(stdout);
	}

	if(strcmp(argv[1], "show")==0){
		ZyPrintf("Tone Name       Tone ID    \n");
		ZyPrintf("   Busy           1    \n");
		ZyPrintf("   Dial           3    \n");
		ZyPrintf("   MWI            4    \n");
		ZyPrintf("   Offhook Warn   5    \n");
		ZyPrintf("   Congestion     24    \n");
		ZyPrintf("   Ringback       26    \n");
		ZyPrintf("   CallWaiting    31    \n");
		
	}
	else{
		if(argc<3){
			printPhoneToneTestUsage();
			return 0;
		}

		phoneId = atoi(argv[1]);		
		toneId = atoi(argv[2]);
		phoneCB = find_phoneCB(phoneId, 1);
		if(NULL == phoneCB){
			ZyPrintf("Can not find physical phone %d\n", phoneId);
			return 0;
		}
		
		if(toneId == 31){ /*Call Waiting with CallId*/
			if(argc>=4){
				strcpy(phoneCB->dialInform.dialDigitNumber.dial_digits, argv[3]);				
				phoneCB->dialInform.dialDigitNumber.current_length = strlen(phoneCB->dialInform.dialDigitNumber.dial_digits);
				phStartCallWaitingTone(phoneCB);
			}
			else{
				PhoneStartTone(phoneCB, toneId);
			}
		}
		else{
			PhoneStartTone(phoneCB, toneId);
		}
		
	}	
		
	fflush(stdout);
	return 0;
} /* doFlashInterval */

/*Add some debug message in voiceApp for future debugging.,Mitrastar, Jason,20120913*/
int
doSipAppShow(
	int	argc,
	char	*argv[]
) {
	VoiceAppShowStatus();
	return 0;
} /* doSipAppShow */


