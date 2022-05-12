#include <stdio.h>
#include "endpt.h"
#include "Endpoint_itf.h"
#if 0
#include <qm.h>
#include <bm.h>
#include <bosInit.h>
#include <bosTask.h>
#include <bosTime.h>
#include <bosSem.h>
#include <bosSleep.h>
#endif
#include <event.h>
#include <switch.h>
#include <global.h>
#include "sys_msg.h"
#include "call_mgnt.h"
#include <time.h>
#include "phfsm.h"
#include <signal.h>
#include <syslog.h>
#include "config_sys.h"

/* toneprofile is defined on \xchange\dslx_common\voice_res_gw\hdsp\ */
/* ringprofile is defined on \xChange\dslx_common\voice_res_gw\casCtl\src\profiles\ */



/* ---- Private Function Prototypes for Phfsm ------------------------------------- */
int PhoneInitEventCb(void);
int PhoneStartRing(phoneCh_t *phoneCB);
int PhoneStartDialTone(phoneCh_t *phoneCB);
int PhoneStopRing(phoneCh_t *phoneCB);
int PhoneStartRingBackTone( phoneCh_t *phoneCB);
int PhoneStopTone(phoneCh_t *phoneCB);
int PhonePSTNoutgoing(phoneCh_t *phoneCB);
int PhonePSTNonhook(phoneCh_t *phoneCB);
int PhonePSTNanswer(phoneCh_t *phoneCB);
void PhoneDisableToneDetection(phoneCh_t *phoneCB);
int PhoneEnableToneDetection(phoneCh_t *phoneCB);
int PhoneStartDtmfTone( phoneCh_t *phoneCB);
#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
int PhoneStartRingForAutoTest(phoneCh_t *phoneCB);
#endif

void PhoneEventCb( ENDPT_STATE *endptState, int cxid, EPEVT event, void *pdata, int length, int data);
#ifdef BUILD_TDC_CUSTOMIZATION//_MSTC_, JuWen, replace phone number replace the + with 00
char *phoneNum_replace (char *source, char find, char *rep, char *result);
#endif
/* ---- Public Variables ------------------------------------------------- */
CMPHYSENDPT cmPhysEndpt[MAX_ENDPT];
EPZCNXPARAM parm;
UINT16 cmNumEndpts = 0;          /* Number of endpoints as queried from endpoint */
static int ringcnt=1;
/*Added by Eason, move from vodsl/voip/prov/sipCli.c(4.02L.02), to support endpoint console commands */
#if 1 
#define FIVE_HUNDRED_MSEC  500
#define STRTOK_R_MACRO( retVal, arg1, arg2, arg3 ) \
{                                                  \
   retVal = strtok_r( arg1, arg2, arg3 );          \
   if ( retVal == NULL )                           \
   {                                               \
      ZyPrintf("Wrong number of arguments \n");\
      return;                                      \
   }                                               \
}
typedef struct cmd cmd_t;
typedef int (cmd_func_t)(cmd_t* cmd, int argc, char **argv);

/* generic command line argument handler */
struct cmd {
   char *name;
   cmd_func_t *func;
   char *help;
   char *syntax;
};

#define MAX_EPTCMD_STRING_LEN       20
#define MAX_EPTCMDHELP_STRING_LEN   100
typedef struct EPTCMDTABLE
{
   EPCONSOLECMD   eptCmd;
   char           eptCmdString[MAX_EPTCMD_STRING_LEN];
   char           eptCmdHelpString[MAX_EPTCMDHELP_STRING_LEN];
   int            nrArguments; /* exludes the line number */
} EPTCMDTABLE;

static EPTCMDTABLE endpointCommandTable[] =
{
   {EPCMD_PRINT_VHDS,            "vhd_info",          "vhd_info   <line> ..................................display info for all VHDs", 0},
   {EPCMD_VHD_STATS,             "vhd_stats",         "vhd_stats  <line> <handle(dec)>.....................display VHD stats", 1},
   {EPCMD_VHD_JBREGS,            "vhd_jbregs",        "vhd_jbregs <line> <handle(dec)> ....................display cnx VHD JB parms", 1},
   {EPCMD_RTP_STATS,             "rtp_stats",         "rtp_stats  <line> <cnxId>...........................display rtp stats", 1},   
   {EPCMD_FAXR_STATS,            "faxr_stats",        "faxr_stats <line> <handle(dec)> ....................display fax relay stats", 1},
   {EPCMD_IPC_STATS,             "ipc_stats",         "ipc_stats <line> <handle(dec)> <mode:0|1>..................display IPC stats", 2},
   {EPCMD_DSP_BUFFERS,           "buf",               "buf <line> .........................................display DSP buffer pool stats", 0},
   {EPCMD_DSP_GETCNT,            "getcnt",            "getcnt <line>.......................................display DSP debug counters", 0},
   {EPCMD_DSP_FORCEASSERT,       "fassert",           "fassert <line>......................................force DSP assert (DSP crasj)", 0},
   {EPCMD_DSP_CHECKASSERT,       "checkassert",       "checkassert <line>..................................check DSP assert stack pointer", 0},
   {EPCMD_DSP_GETREGS,           "getregs",           "getregs <line>......................................get a snapshot of DSP registers", 0},
   {EPCMD_DSP_READDMEM,          "readmem",           "readmem <line> <addr(dec)> <len>....................read internal DSP data memory", 2},
   {EPCMD_APM_STATUS,            "apmsts",            "apmsts <line>.......................................display APM status information", 0},
   {EPCMD_PRINT_DSP_CAPABILITIES,"caps",              "caps <line>.........................................display DSP capabilities", 0},
   {EPCMD_DSP_STACK_SHOW,        "stackshow",         "stackshow <line>....................................display DSP stack usage stats.", 0},
   {EPCMD_HAPINET_CONFIG_SETREG, "configreg",         "configreg <line> <VHD num (dec)> <offset> <value>...set cfg registers of the NET Srv", 3},
   {EPCMD_DSP_ICP_CALIBRATE,     "icpcalibrate",      "icpcalibrate <line>.................................calibrate DSP idle profiler", 0},
   {EPCMD_DSP_ICP_START,         "icpstart",          "icpstart <line> <verbose 0|1> <stat interval>.......start DSP idle profiler", 2},
   {EPCMD_DSP_ICP_STOP,          "icpstop",           "icpstop <line>......................................stop DSP idle profiler", 0},
   {EPCMD_DSP_DPR_START,         "dprstart",          "dprstart <line> <VHD num (dec)>.....................start development DSP profiler", 1},
   {EPCMD_HEC_BURST_ENABLE,      "hec_burstenable",   "hec_burstenable <line>..............................ECAN debugging: enable burst", 0},
   {EPCMD_HEC_NOISE_ENABLE,      "hec_noiseenable",   "hec_noiseenable <line>..............................ECAN debugging: enable noise", 0},
   {EPCMD_HEC_DEFAULT,           "hec_default",       "hec_default <line>..................................ECAN debugging: set default", 0},
   {EPCMD_HEC_GET_STATE,         "hec_getstate",      "hec_getstate <line>.................................ECAN debugging: get ECAN state", 0},
   {EPCMD_CASEVT_GENERATE,       "casevtgen",         "casevtgen <line> <CAS event>........................generate CAS event", 1},
   {EPCMD_TONEEVT_GENERATE,      "toneevtgen",        "toneevtgen <line> <tone on/off> <tone ID>...........generate tone event", 2},
   {EPCMD_EGRESSGAIN_SET,        "setgain_egress",    "setgain_egress <line> <gaindB>......................set egress gain (dB)", 1},
   {EPCMD_INGRESSGAIN_SET,       "setgain_ingress",   "setgain_ingress <line> <gaindB>.....................set ingress gain (dB)", 1},
   {EPCMD_CONNECTSB,             "sbconnect",         "sbconnect <line1> <line2>...........................switchboard connect line1 to line2", 1},
   {EPCMD_SET_ECAN_STATE,        "setecan",           "setecan <line> <1 - on/ 0 -off>.....................set new ecan state", 1},
#if DECT_SUPPORTED
   {EPCMD_DECT_REG_START,        "dectstartreg",      "dectstartreg ...................................... start DECT registration", 0},
   {EPCMD_DECT_SET_ACCESS_CODE,  "dectsetac",         "dectsetac <access code 4 digits>....................set DECT access code", 1},
   {EPCMD_DECT_GET_ACCESS_CODE,  "dectgetac",         "dectgetac ..........................................get DECT access code", 0},
   {EPCMD_DECT_DEL_HANDSET,      "dectdelhs",         "dectdelhs <handset #>...............................delete DECT handset", 1},
   {EPCMD_DECT_GET_HANDSET_COUNT, "decthscnt",        "decthscnt ..........................................get DECT handset count", 0},
#endif
#ifdef SUPPORT_412_VOICE_L06
   {EPCMD_SET_SLIC_STATE,		"sistate",			 "sistate <line> <1 - active/0 -standby/3 - ring>.....set SLIC mode", 1},
#else
   {EPCMD_SET_SLIC_STATE,		"sistate",			 "sistate <line> <1 - active/0 -standby/2 - ring>.....set SLIC mode", 1},
#endif
   {EPCMD_SET_HDSP_TONE, 		"dsptone",			 "dsptone <line> <tone type>..........................DSP generate tone", 1},
   {EPCMD_SLIC_MODE_CTL, 		"setmode",			 "setmode <line> <mode 0:LCFO 1:STANDBY 2:OHT 3:OHTR 4:TIPOPEN 5:RING 6:LCF 7:RLCF 8:RINGOPEN>..SLIC mode change", 1},
   {EPCMD_SLIC_IS_OFFHOOK,		"isoffhook",		 "isoffhook <line> ...................................check if slic is offhook", 0},
   {EPCMD_SLIC_LINE_STATE,		"getlinestate", 	 "getlinestate <line> ................................get line state", 0},
   {EPCMD_SLIC_SHOW_DEV_INFO,	"showdevinfo",		 "showdevinfo <line> .................................show the device info", 0},
   {EPCMD_SLIC_SHOW_LINE_INFO,	"showlineinfo", 	 "showlineinfo <line> ................................show the line info", 0},
//   {EPCMD_SET_SLIC_TONE,      "sitone",        "sitone <line> <tone type 0:DIAL 1:BUSY 2:RINGBACK 3:REORDER 4:CONGEST 5:STOP>..SLIC gen tone", 1},
   {EPCMD_SET_SLIC_RING,      "siring",        "siring <line> <1 - on/0 -off> <ring ID>...........SLIC generate ring", 2},
//   {EPCMD_SLIC_REG_WRITE,      "siregw",        "siregw <channel> <register address> <value>...........SLIC register write", 2},
//   {EPCMD_SLIC_REG_READ,      "siregr",        "siregr <channel> <register address>...........SLIC register read", 1},
//   {EPCMD_SLIC_REG_DUMP,      "siregd",        "siregd <channel>...........SLIC register dump", 0},
//   {EPCMD_SLIC_RAM_WRITE,      "siramw",        "siramw <channel> <RAM address> <valueHI> <valueLO>...........SLIC RAM write", 3},
//   {EPCMD_SLIC_RAM_READ,      "siramr",        "siramr <channel> <RAM address>...........SLIC RAM read", 1},
//   {EPCMD_SLIC_RAM_DUMP,      "siramd",        "siramd <channel>...........SLIC RAM dump", 0},
//   {EPCMD_SLIC_INIT,	      "siinit",        "siinit <channel>...........SLIC channel init", 0},
//   {EPCMD_SLIC_DEINIT,	      "sideinit",        "sideinit <channel>...........SLIC channel deinit", 0},
	{EPCMD_MPI_CMD, 			 "mpicmd",			  "mpicmd <channel> <deviceId> <ecVal> <cmd> <cmdLen> <data 1-cmdLen (when 'write')> ..........MPI command", 4},
	{EPCMD_SLIC_GET_GAIN_RESULT, "getgain", 		  "getgain <line> .....................................get the Tx and Rx gain", 0},
	{EPCMD_SLIC_SET_GAIN,		 "setgain", 		  "setgain <line> <txLevel> <rxLevel> .................set the Tx and Rx gain", 2},
#ifdef MSTC_VOICE_GR909 /*MitraStar_PeiEn_Support GR909 Line Test Diagnostic_2012.02.22*/
	{EPCMD_SLIC_LINE_TEST,	"linetest",	"linetest <line> <testID 1:LINE_V 2:ROH 3:REN 4:Resistive 5:GR-909>.....line test option",1},
#endif
    {EPCMD_VRG_DEBUG,	      "vrg_debug",        "vrg_debug <don't care> <1 -on/0 -off>...........vrg_debug on/off", 1},
//   {EPCMD_BUZZER,	      "buzzer",        "buzzer <don't care> <1 -on/0 -off>...........buzzer on/off", 1},
   {EPCMD_MAX,                   "",                  "", 0}
};

#define LOGERROR(fmt) ZyPrintf fmt
#endif /*Added by Eason, move from vodsl/voip/prov/sipCli.c(4.02L.02), to support endpoint console commands */


/* ---- Public Functions for Phfsm-------------------------------------------------------- */

int PhoneDataInit(void *data, int data_len){	
	if(data_len==sizeof(cmPhysEndpt)){
		zyMemcpy(cmPhysEndpt,data,data_len);			
		return 0;
	}
	PH_FSM_DBG(" PhoneDataInit FAILED!!");
	return -1;
}

#define MAX_USERNAME_LEN                    40
#define MAX_CLID_NUMBER             MAX_USERNAME_LEN
#define MAX_CLID_NAME               MAX_USERNAME_LEN
#define MAX_CLID_DATE               14


int PhoneStartRing(
	phoneCh_t *phoneCB
) {
	int index;

	struct tm *local_time = NULL;
	time_t calendar_time = 0;
	int ret = -1;
	char formatstr[MAX_CLID_DATE + MAX_CLID_NUMBER + MAX_CLID_NAME + 4];

	char date[MAX_CLID_DATE];
	char number[MAX_CLID_NUMBER];
#ifdef BUILD_TDC_CUSTOMIZATION//_MSTC_, JuWen, replace phone number replace the + with 00	
	char replace_num[MAX_CLID_NUMBER+1];
#endif
	char name[MAX_CLID_NAME];
	  
	ret = time(&calendar_time);	
	if (-1 != ret)	
	{
	
		local_time = localtime(&calendar_time);
		snprintf(date, MAX_CLID_DATE,
	                     "%02d/%02d/%02d/%02d",           // Do not change the length of this string
	                     local_time->tm_mon + 1,
	                     local_time->tm_mday,
	                     local_time->tm_hour,
	                     local_time->tm_min);
	
	
		
	}
	else
	{	
		sprintf(date, MAX_CLID_DATE, "01/01/00/00"); // Do not change the length of this string
	}

	
	if(zyStrlen(phoneCB->dialInform.dialDigitNumber.cname)>0){
		zyStrncpy(name, phoneCB->dialInform.dialDigitNumber.cname, MAX_CLID_NAME);		
	}
	else{
		zyStrncpy(name, " ", MAX_CLID_NAME);
	}

	if(zyStrlen(phoneCB->dialInform.dialDigitNumber.dial_digits)>0){
		zyStrncpy(number, phoneCB->dialInform.dialDigitNumber.dial_digits, MAX_CLID_NUMBER);		

#ifdef BUILD_TDC_CUSTOMIZATION//_MSTC_, JuWen, replace phone number replace the + with 00
              PH_FSM_PRINTF("!!PhoneStartRing:number = %s!!\n",number);
		if(phoneNum_replace(number, '+',"00", replace_num) != NULL){
			strcpy(number,replace_num);
		}		
#endif
		
	}
	else{
		zyStrncpy(number, " ", MAX_CLID_NUMBER);
	}
	
	snprintf(formatstr, MAX_CLID_DATE + MAX_CLID_NUMBER + MAX_CLID_NAME + 4, "%s, %s, %s", date, number, name);

	PH_FSM_PRINTF("********CLID : %s\n", formatstr);
	index=phoneCB->physicalId;
	/* Platform porting: send event or ... to physical driver to send ring and caller id */
	/* END of Platform porting */	
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_CALLID_RINGING, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Ring!!");
		return (-1);
	}
	vrgEndptSignal( &cmPhysEndpt[index].endptObjState, -1, EPSIG_CALLID, (uint32)&formatstr, -1, -1, -1 );	
	return (0);
}


int PhoneStartDialTone(
 	phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start dial tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DIAL, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Dial!!");
		return (-1);
   	}

	return (0);
}

int PhoneStartSpecialDialTone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start call waiting tone */
	/* END of Platform porting */
	PH_FSM_PRINTF("Special Dial Tone for %d\n", index);
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_SPECIAL_DIAL, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Gen SpecialDialTone!!");
		return (-1);
   	}

	return (0);
}

int PhoneStartConfirmTone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start call waiting tone */
	/* END of Platform porting */
	PH_FSM_PRINTF("Confirm Tone for %d\n", index);
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_CONF, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Gen Confirm Tone!!");
		return (-1);
   	}

	return (0);
}


int PhoneStopRing(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to stop ring */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_CALLID_RINGING, 0,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Stop ring!!");
		return (-1);
   	}

	return (0);
}

#ifdef MSTC_VOIP_AUTO_TEST //__MSTC__, marcus, voip auto test
int PhoneStartRingForAutoTest(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to stop ring */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_RINGING, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Ring!!");
		return (-1);
   	}
	return (0);
}
#endif

int PhoneStartRingBackTone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start ring back tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_RINGBACK, 1,-1,-1,-1))!=EPSTATUS_SUCCESS){
		PH_FSM_DBG("Can't Gen RingBackTone!!");
		return (-1);
   	}

	return (0);
}
int PhoneStartMWITone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start MWI tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_MSGW, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Gen MWITone!!");
		return (-1);
   	}

	return (0);
}
int PhoneStartBusyTone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start busy tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_BUSY, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Gen RingBackTone!!");
		return (-1);
	}

	return (0);
}
int PhoneStartCongestTone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start Congestion tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_REORDER, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Gen RingBackTone!!");
		return (-1);
   	}

	return (0);
}

int PhoneStartTone(
     phoneCh_t *phoneCB,
     int toneindex
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to start call waiting tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, toneindex, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_PRINTF("Can't Gen Tone %d!!\n", toneindex);
		return (-1);
   	}

	return (0);
}


int PhoneStartCallWaitingTone(
     phoneCh_t *phoneCB
){
	int index;
	
	struct tm *local_time = NULL;
	time_t calendar_time = 0;
	int ret = -1;
	char formatstr[MAX_CLID_DATE + MAX_CLID_NUMBER + MAX_CLID_NAME + 4];

	char date[MAX_CLID_DATE];
	char number[MAX_CLID_NUMBER];
	char name[MAX_CLID_NAME];
	  
	ret = time(&calendar_time); 
	if (-1 != ret)	
	{
	
		local_time = localtime(&calendar_time);
		snprintf(date, MAX_CLID_DATE,
						 "%02d/%02d/%02d/%02d", 		  // Do not change the length of this string
						 local_time->tm_mon + 1,
						 local_time->tm_mday,
						 local_time->tm_hour,
						 local_time->tm_min);
	
	
		
	}
	else
	{	
		sprintf(date, MAX_CLID_DATE, "01/01/00/00"); // Do not change the length of this string
	}

	
	if(zyStrlen(phoneCB->dialInform.dialDigitNumber.cname)>0){
		zyStrncpy(name, phoneCB->dialInform.dialDigitNumber.cname, MAX_CLID_NAME);		
	}
	else{
		zyStrncpy(name, " ", MAX_CLID_NAME);
	}

	if(zyStrlen(phoneCB->dialInform.dialDigitNumber.dial_digits)>0){
		zyStrncpy(number, phoneCB->dialInform.dialDigitNumber.dial_digits, MAX_CLID_NUMBER);		
	}
	else{
		zyStrncpy(number, " ", MAX_CLID_NUMBER);
	}
	
	snprintf(formatstr, MAX_CLID_DATE + MAX_CLID_NUMBER + MAX_CLID_NAME + 4, "%s, %s, %s", date, number, name);

	PH_FSM_PRINTF("********CLID : %s\n", formatstr);
	index=phoneCB->physicalId;
	/* Platform porting: send event or ... to physical driver to send ring and caller id */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_SAS_CAS, 1,-1,-1,-1))!=EPSTATUS_SUCCESS) {
		PH_FSM_DBG("Can't Ring!!");
		return (-1);
   	}
	vrgEndptSignal( &cmPhysEndpt[index].endptObjState, -1, EPSIG_CALLID, (uint32)&formatstr, -1, -1, -1 );	
	return (0);


	return (0);
}


int PhoneStartDtmfTone(
     phoneCh_t *phoneCB
){
	int index = 0;

	index=phoneCB->physicalId;
	PH_FSM_DBG2("In ringing test ,cmPhysEndpt[", index, "].endptObjState.lineId=",cmPhysEndpt[index].endptObjState.lineId);
	PH_FSM_DBG("Nick test :START to  Gen DtmfTone \n");

	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x30){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 0 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF0, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
   		}
 	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x31){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 1 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF1, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x32){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 2 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF2, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
 	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x33){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 3 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF3, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x34){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 4 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF4, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x35){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 5 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF5, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x36){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 6 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF6, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x37){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 7 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF7, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x38){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 8 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF8, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x39){
		/* Platform porting: send event or ... to physical driver to send DTMF tone 9 */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMF9, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x2a){
		/* Platform porting: send event or ... to physical driver to send DTMF tone * */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMFS, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x23){
		/* Platform porting: send event or ... to physical driver to send DTMF tone # */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMFH, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x41){
		/* Platform porting: send event or ... to physical driver to send DTMF tone A */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMFA, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x42){
		/* Platform porting: send event or ... to physical driver to send DTMF tone B */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMFB, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x43){
		/* Platform porting: send event or ... to physical driver to send DTMF tone C */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMFC, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	if(phoneCB->dialInform.dialDigitNumber.dial_digits[phoneCB->dialInform.dialDigitNumber.dialout_length] == 0x44){
		/* Platform porting: send event or ... to physical driver to send DTMF tone D */
		/* END of Platform porting */
		if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_DTMFD, 250,-1,-1,-1))!=EPSTATUS_SUCCESS) // DTMF Tone 
		{
			PH_FSM_DBG("Can't Stop  DTMF Tone!!");
			return (-1);
		}
	}
	PH_FSM_DBG("Nick test :Gen DtmfTone end");
	return (0);
}
int PhoneStopTone(
     phoneCh_t *phoneCB
){
	int index;
	index=phoneCB->physicalId;

	/* Platform porting: send event or ... to physical driver to stop Tone */
	/* END of Platform porting */
	if((vrgEndptSignal(&cmPhysEndpt[index].endptObjState, -1, EPSIG_BUSY, 0,-1,-1,-1))!=EPSTATUS_SUCCESS) //BUSY_TONE
	{
		PH_FSM_DBG("Can't Stop BUSY TONE!");
		return (-1);
	}

	return (0);
}

#if KSOCKET_RTP_SUPPORT
int PhoneShowKSocket(void){

	/* Platform porting: send event or ... to physical driver to start busy tone */
	/* END of Platform porting */
	if(vrgEndptShowSession()) {
		PH_FSM_DBG("Can't show KSocket session!");
		return (-1);
	}

	return (0);
}
#endif

int PhoneJBSet(int line, int type, int min, int max, int voiceTarget, int dataTarget){
	PH_FSM_PRINTF("%s line = %d\n", __FUNCTION__, line);
	if(vrgEndptProvSet(line, EPPROV_VoiceJitterBuffFixed, &type, sizeof(type))) {
		PH_FSM_DBG("Set EPPROV_VoiceJitterBuffFixed failed");	
	}

	if(vrgEndptProvSet(line, EPPROV_VoiceJitterBuffMin, &min, sizeof(min))) {
		PH_FSM_DBG("Set EPPROV_VoiceJitterBuffMin failed");
	}

	if(vrgEndptProvSet(line, EPPROV_VoiceJitterBuffMax, &max, sizeof(max))) {
		PH_FSM_DBG("Set EPPROV_VoiceJitterBuffMin failed");
	}

	if(vrgEndptProvSet(line, EPPROV_VoiceJitterBuffTarget, &voiceTarget, sizeof(voiceTarget))) {
		PH_FSM_DBG("Set EPPROV_VoiceJitterBuffTarget failed");
	}

	if(vrgEndptProvSet(line, EPPROV_DataJitterBuffTarget, &dataTarget, sizeof(dataTarget))) {
		PH_FSM_DBG("Set EPPROV_DataJitterBuffTarget failed");
	}

	return (0);
}

int PhoneFlashIntervalSet(int line, uint32 min, uint32 max){
	if(vrgEndptProvSet(line, EPPROV_MinHookFlash, &min, sizeof(min))) {
		ZyPrintf("Set EPPROV_MinHookFlash failed");
		return -1;
	}

	if(vrgEndptProvSet(line, EPPROV_MaxHookFlash, &max, sizeof(max))) {
		ZyPrintf("Set EPPROV_MaxHookFlash failed");
		return -1;
	}
	return 0;
}

int PhoneFlashIntervalGet(int line, uint32 *min, uint32 *max){
	if(vrgEndptProvGet(line, EPPROV_MinHookFlash, min, sizeof(min))) {
		ZyPrintf("Set EPPROV_MinHookFlash failed");
		return -1;
	}

	if(vrgEndptProvGet(line, EPPROV_MaxHookFlash, max, sizeof(max))) {
		ZyPrintf("Set EPPROV_MaxHookFlash failed");
		return -1;
	}
	return 0;
}


int PhonePSTNSetRelay(bool cmd)
{
	ZyPrintf("***In PhonePSTNSetRelay::set to %d\n", cmd);

	/* Platform porting: send event or ... to physical driver for PSTN outgoing, offhook then PSTN callsetup*/
	/* END of Platform porting */
	vrgEndptActiveRelay(cmd);

	return (0);
}

int PhonePSTNOffhook(phoneCh_t *phoneCB)
{
	int index;
	EPSTATUS retVal;

	index=phoneCB->physicalId;
	PH_FSM_DBG2("***In PhonePSTNoutgoing::index=", index, " ,phoneCB->peerMediaChId=", phoneCB->peerMediaChId);

	/* Platform porting: send event or ... to physical driver for PSTN outgoing, offhook then PSTN callsetup*/
	/* END of Platform porting */
	retVal=vrgEndptSignal( &cmPhysEndpt[phoneCB->physicalId].endptObjState, -1, EPSIG_OFFHOOK, 1, -1, -1, -1 );

	return (0);
}


int PhonePSTNOutgoing(phoneCh_t *phoneCB)
{
	int index;
	EPSTATUS retVal;

	index=phoneCB->physicalId;
	PH_FSM_DBG2("***In PhonePSTNoutgoing::index=", index, " ,phoneCB->peerMediaChId=", phoneCB->peerMediaChId);

	/* Platform porting: send event or ... to physical driver for PSTN outgoing, offhook then PSTN callsetup*/
	/* END of Platform porting */
	retVal=vrgEndptSignal( &cmPhysEndpt[phoneCB->physicalId].endptObjState, -1, EPSIG_OFFHOOK, 1, -1, -1, -1 );

	PH_FSM_PRINTF("*** digits: %s\n", phoneCB->dialInform.dialDigitNumber.dial_digits);
	/* [2] Signal the destination for this call.*/
	retVal=vrgEndptSignal( &cmPhysEndpt[phoneCB->physicalId].endptObjState, -1, EPSIG_PSTNCALLSETUP,
		phoneCB->dialInform.dialDigitNumber.dial_digits, strlen(phoneCB->dialInform.dialDigitNumber.dial_digits), -1, -1 );

	return (0);
}

int PhonePSTNOnhook(phoneCh_t *phoneCB)
{
	int index;
	EPSTATUS retVal;

	index=phoneCB->physicalId;
	PH_FSM_DBG2("***In PhonePSTNonhook::index=", index, " ,phoneCB->peerMediaChId=", phoneCB->peerMediaChId);

	/* Platform porting: send event or ... to physical driver to send PSTN onhook */
	/* END of Platform porting */
	retVal=vrgEndptSignal( &cmPhysEndpt[phoneCB->physicalId].endptObjState, -1, EPSIG_ONHOOK, 1, -1, -1, -1 );
	ringcnt=1;
	return (0);
}

int PhonePSTNanswer(phoneCh_t *phoneCB)
{
	int index;
	EPSTATUS retVal;

	index=phoneCB->physicalId;
	PH_FSM_DBG2("***In PhonePSTNanswer::index=", index, " ,phoneCB->peerMediaChId=", phoneCB->peerMediaChId);

	/* Platform porting: send event or ... to physical driver to send PSTN answer */
	/* END of Platform porting */
	retVal=vrgEndptSignal( &cmPhysEndpt[index].endptObjState, -1, EPSIG_ANSWER, 1, -1, -1, -1 );
	return (0);
}

/******************************************************
  * value : gain value (db)
  *****************************************************/
int PhoneSetTxGain(phoneCh_t *phoneCB, int value)
{
	int index = 0;
	if(NULL != phoneCB){
		index=phoneCB->physicalId;
		PH_FSM_PRINTF("***In PhoneSetTxGain::index = %d, value = %d\n", index, value);

		/* Platform porting: send event or ... to physical driver to set Tx Gain */
		/* END of Platform porting */
		if(vrgEndptSignal( &cmPhysEndpt[index].endptObjState, -1, EPSIG_TXGAIN, value, -1, -1, -1 ) != EPSTATUS_SUCCESS){
			PH_FSM_DBG("BroadcomPhoneSetTxGain failed");
			return -1;
		}
	} else {
	   	PH_FSM_DBG("***In PhoneSetTxGain phoneCB NULL");
   		return -1;
	}
	return (0);
}

/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
int PhoneSetEptTxGain(phoneCh_t *phoneCB, int value)
{
	int index = 0;
	if(NULL != phoneCB){
		index=phoneCB->physicalId;
		PH_FSM_PRINTF("***In PhoneSetEptTxGain::index = %d, value = %d\n", index, value);
		EPCMD_PARMS parm;
		parm.arg[0]=value;
		/* Platform porting: send event or ... to physical driver to set Tx Gain */
		/* END of Platform porting */
		if(vrgEndptConsoleCmd( &cmPhysEndpt[index].endptObjState,EPCMD_EGRESSGAIN_SET, &parm) != EPSTATUS_SUCCESS){
			PH_FSM_DBG("BroadcomPhoneSetEptTxGain failed");
			return -1;
		}
	} else {
	   	PH_FSM_DBG("***In PhoneSetEptTxGain phoneCB NULL");
   		return -1;
	}
	return (0);
}

/******************************************************
  * value : gain value (db)
  *****************************************************/

int PhoneSetRxGain(phoneCh_t *phoneCB, int value)
{
	int index = 0;
	if(NULL != phoneCB){
		index=phoneCB->physicalId;
		PH_FSM_PRINTF("***In PhoneSetRxGain::index = %d, value = %d\n", index, value);
		/* Platform porting: send event or ... to physical driver to set Rx Gain */
		/* END of Platform porting */
		if(vrgEndptSignal( &cmPhysEndpt[index].endptObjState, -1, EPSIG_RXGAIN, value, -1, -1, -1 ) != EPSTATUS_SUCCESS) {
			PH_FSM_DBG("PhoneSetRxGain failed");
			return -1;
		}
	} else {
	   	PH_FSM_DBG("***In PhoneSetRxGain phoneCB NULL");
		return -1;
	}
	return (0);
}

/* __MSTC__, Klose,merge from P876 feature, Set egress/ingress gain to control the Speaking/Listening volume, 20121107 */
int PhoneSetEptRxGain(phoneCh_t *phoneCB, int value)
{
	int index = 0;
	if(NULL != phoneCB){
		index=phoneCB->physicalId;
		EPCMD_PARMS parm;
		parm.arg[0]=value;
		PH_FSM_PRINTF("***In PhoneSetEptRxGain::index = %d, value = %d\n", index, value);
		/* Platform porting: send event or ... to physical driver to set Rx Gain */
		/* END of Platform porting */
		if(vrgEndptConsoleCmd( &cmPhysEndpt[index].endptObjState,EPCMD_INGRESSGAIN_SET, &parm) != EPSTATUS_SUCCESS) {
			PH_FSM_DBG("PhoneSetEptRxGain failed");
			return -1;
		}    
	} else {
   		PH_FSM_DBG("***In PhoneSetEptRxGain phoneCB NULL");
   		return -1;
	}
	return (0);
}

void PhoneDisableToneDetection(phoneCh_t *phoneCB){
	int index = -1;
	index = phoneCB->physicalId;

	PH_FSM_DBG1("***In PhoneDisableToneDetection::index=", index);
	cmPhysEndpt[index].toneDetection = 0;
}

int PhoneEnableToneDetection(phoneCh_t *phoneCB){
	int index = -1;
	index = phoneCB->physicalId;

	PH_FSM_DBG1("***In PhoneEnableToneDetection::index=", index);
	cmPhysEndpt[index].toneDetection = 1;

	return 0;
}


int PhoneDeinit(void)
{	
	if(isEndptInitialized()){
		ZyPrintf("%s, deinit DSP..., vrgEndptDeinit()\n",__FUNCTION__);
		vrgEndptDeinit();
	}
	return 0;
}

pthread_mutex_t mutexDspDeinit = PTHREAD_MUTEX_INITIALIZER; 

void AppSignalHandler(int sig, siginfo_t* siginfo, void* notused ){
	
	switch(sig){
		case SIGTERM:
		case SIGSEGV:	
		case SIGHUP:
		case SIGKILL:
			#if 1
			if(!pthread_mutex_trylock(&mutexDspDeinit)){
			#else
				pthread_mutex_lock(&mutexDspDeinit);
			#endif
				ZyPrintf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				ZyPrintf("voiceApp %d: Signal %d received, TERMINATING voiceApp \n",getpid(),  sig);
				ZyPrintf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				PhoneDeinit();
				pthread_mutex_unlock(&mutexDspDeinit);
			#if 1 /*Below is the symptom of the bug fix of the unregister issue, __P876__,Ailsa, 20120326 */
			} else {
				syslog(LOG_ALERT, "<<<voiceApp(%d) received signal %d TERMINATING>>>", getpid(), sig);
				ZyPrintf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
				ZyPrintf("voiceApp %d: Signal %d received, TERMINATING voiceApp \n", getpid(), sig);
				ZyPrintf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			}
			#endif
			exit(0);
			break;
		default:
			ZyPrintf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			ZyPrintf("voiceApp: Signal %d received\n", sig);
			ZyPrintf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			break;
			
	}
}



void AppRegisterSignal(void){
	struct sigaction sa;

   sa.sa_sigaction = AppSignalHandler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;
   ZyPrintf("%s, voiceApp pid = %d\n",__FUNCTION__, getpid());
#if 0
   sigaction( SIGINT ,  &sa,  NULL );     /* Interrupt (ANSI).  */
   sigaction( SIGQUIT,  &sa,  NULL );     /* Quit (POSIX).  */
   sigaction( SIGILL ,  &sa,  NULL );     /* Illegal instruction (ANSI).  */
   sigaction( SIGTRAP,  &sa,  NULL );     /* Trace trap (POSIX).  */
   sigaction( SIGABRT,  &sa,  NULL );     /* Abort (ANSI).  */
   sigaction( SIGEMT,   &sa,  NULL );
   sigaction( SIGFPE ,  &sa,  NULL );     /* Floating-point exception (ANSI).  */
   sigaction( SIGBUS,   &sa,  NULL );     /* BUS error (4.2 BSD).  */
   sigaction( SIGSYS,   &sa,  NULL );
   sigaction( SIGPIPE,  &sa,  NULL );     /* Broken pipe (POSIX).  */
   sigaction( SIGALRM,  &sa,  NULL );     /* Alarm clock (POSIX).  */
   sigaction( SIGUSR1,  &sa,  NULL );     /* User-defined signal 1 (POSIX).  */
   sigaction( SIGUSR2,  &sa,  NULL );     /* User-defined signal 2 (POSIX).  */
   sigaction( SIGCHLD,  &sa,  NULL );     /* Child status has changed (POSIX).  */
   sigaction( SIGPWR,   &sa,  NULL );     /* Power failure restart (System V).  */
   sigaction( SIGWINCH, &sa,  NULL );     /* Window size change (4.3 BSD, Sun).  */
   sigaction( SIGURG,   &sa,  NULL );     /* Urgent condition on socket (4.2 BSD).  */
   sigaction( SIGIO,    &sa,  NULL );     /* I/O now possible (4.2 BSD).  */
   sigaction( SIGTSTP,  &sa,  NULL );     /* Keyboard stop (POSIX).  */
   sigaction( SIGCONT,  &sa,  NULL );     /* Continue (POSIX).  */
   sigaction( SIGTTIN,  &sa,  NULL );     /* Background read from tty (POSIX).  */
   sigaction( SIGTTOU,  &sa,  NULL );     /* Background write to tty (POSIX).  */
   sigaction( SIGVTALRM,&sa,  NULL );     /* Virtual alarm clock (4.2 BSD).  */
   sigaction( SIGPROF,  &sa,  NULL );     /* Profiling alarm clock (4.2 BSD).  */
   sigaction( SIGXCPU,  &sa,  NULL );     /* CPU limit exceeded (4.2 BSD).  */
   sigaction( SIGXFSZ,  &sa,  NULL );     /* File size limit exceeded (4.2 BSD).  */   
#endif   
   sigaction( SIGHUP ,	&sa,  NULL );	  /* Hangup (POSIX).  */
   sigaction( SIGKILL , &sa,  NULL );	 /* Hangup (POSIX).  */
   sigaction( SIGSEGV,  &sa,  NULL );     /* Segmentation violation (ANSI).  */   
   sigaction( SIGTERM,  &sa,  NULL );     /* Termination (ANSI).  */

}


void PhoneEventCb( ENDPT_STATE *endptState, int cxid, EPEVT event, void *pdata, int length, int data)
{
	phoneCh_t *phoneCB=NULL;
	void  *ref = NULL;
	int val = 0;

	if( pdata != NULL && length > 0 ) {
		char *ptr = (char *)zyMalloc( length * sizeof(char));
		if( ptr != NULL ) {
			zyMemcpy( (void *)ptr, pdata, length );
			ref = (void *)ptr;
			val = length;
		}
	} else {
		ref = NULL;
		val = 0;
	}
	
	PH_FSM_DBG2("cmEndptEventCb: endptState->lineId = ", (int)endptState->lineId, ", event = ", (int)event);
	PH_FSM_DBG2("data = ", (int)data, ", ref = ", ref);
	PH_FSM_DBG2( "val =", (int)val, " ,cxid = ", cxid);
	switch(event) {
		case EPEVT_DTMF0 :
		case EPEVT_DTMF1 :
		case EPEVT_DTMF2 :
		case EPEVT_DTMF3 :
		case EPEVT_DTMF4 :
		case EPEVT_DTMF5 :
		case EPEVT_DTMF6 :
		case EPEVT_DTMF7 :
		case EPEVT_DTMF8 :
		case EPEVT_DTMF9 :
		case EPEVT_DTMFH :
		case EPEVT_DTMFS :
		case EPEVT_DTMFA :
		case EPEVT_DTMFB :
		case EPEVT_DTMFC :
		case EPEVT_DTMFD :
		{
			char asciiChar;
			dialDigitBuf_t dialDigitBuf;
			/* We process DTMF events on the 'off' action, that is after the user releases
			** the key from the keypad.  However, there is one case that we want to handle
			** properly and that is to terminate dial tone on the first DTMF detection, which
			** would happen as soon as the DTMF 'on' event is reported.
			*/
			if( data == EPDTMFACT_TONEOFF || data == 0xffff /* ZyXEL_VOICE, P876, add for dial pulse */)
			{
				PH_FSM_DBG1(" DTMF:sizeof(dialDigitBuf_t) == ",sizeof(dialDigitBuf_t));
				switch (event){
					case 0x1 :
						asciiChar = '0';
						break;
					case 0xB:
						asciiChar = '*';
						break;
					case 0xC:
						asciiChar = '#';
						break;
					case 0xD:
						asciiChar = 'A';
						break;
					case 0xE:
						asciiChar = 'B';
						break;
					case 0xF:
						asciiChar = 'C';
						break;
					case 0x10:
						asciiChar = 'D';
						break;
					default:
						asciiChar= '0' + (event-1);
						break;
				}
				PH_FSM_PRINTF(" Digit:%c\n ",asciiChar);
				zyMemset(&dialDigitBuf, 0, sizeof(dialDigitBuf_t));
				dialDigitBuf.digitStr = asciiChar;
				phoneCB = find_phoneCB(endptState->lineId, 1);
				if ( phoneCB != NULL ) {
					PH_FSM_DBG2("DTMF TONE::phoneCB->logicalId = ", phoneCB->logicalId , ", phoneCB->phoneFlags = ",phoneCB->phoneFlags);
					eventVoiceDataSend(EVT_PHONE_FXS_DIGITS, MSG_SEND_TO_PHONE, phoneCB->logicalId, &dialDigitBuf, sizeof(dialDigitBuf_t));					
				}
			}
		
		}
		break;
	case EPEVT_OFFHOOK:
		phoneCB = find_phoneCB(endptState->lineId, 1);
		if ( phoneCB != NULL ) {
			PH_FSM_PRINTF("EPEVT_OFFHOOK::phoneCB=%x\n",phoneCB);
			voipLedBlinking(phoneCB->physicalId);
			eventVoiceSimpleSend(EVT_PHONE_FXS_OFFHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
		}
		break;			
	case EPEVT_ONHOOK:
		phoneCB = find_phoneCB(endptState->lineId, 1);
		if ( phoneCB != NULL ) {
			PH_FSM_PRINTF("EPEVT_ONHOOK::phoneCB=%x\n",phoneCB);
			eventVoiceSimpleSend(EVT_PHONE_FXS_ONHOOK, MSG_SEND_TO_PHONE, phoneCB->logicalId);
		}		
		break;		
	case EPEVT_FSK_CLIDRX:
	case EPEVT_DTMF_CLIDRX:			  /* FSK CLIDRX */
		{
			callerIDInfo_t temp_info;
			VRG_CLIDRX_DATA clid;
			PH_FSM_PRINTF("#####EPEVT_CLIDRX,endptState->lineId=%d\n",endptState->lineId);
			phoneCB = find_phoneCB(endptState->lineId, 1);			

			if(phoneCB!=NULL){
				zyMemset( (void *)&clid, 0, sizeof(VRG_CLIDRX_DATA) );
				if( ref != NULL ) {
					zyMemcpy( (void *)&clid, ref, sizeof(VRG_CLIDRX_DATA) );
					zyFree(ref );
					ref = NULL;
				}
				zyMemset(&temp_info,0,sizeof(callerIDInfo_t));
				zyMemcpy(temp_info.month, &clid.date[0], 2);
				zyMemcpy(temp_info.day, &clid.date[2], 2);
				zyMemcpy(temp_info.hour, &clid.date[4], 2);
				zyMemcpy(temp_info.minute, &clid.date[6], 2);
				zyMemcpy(temp_info.digits, &clid.number, zyStrlen(clid.number));
				temp_info.digitsFlag=zyStrlen(temp_info.digits);
				zyMemcpy(temp_info.name, &clid.name, zyStrlen(clid.name));
				temp_info.nameFlag=zyStrlen(temp_info.name);
				eventVoiceDataSend(EVT_PHONE_FXO_RECVCID, MSG_SEND_TO_PHONE, phoneCB->logicalId,  (void *)&temp_info, sizeof(callerIDInfo_t)); 
			} else {
				PH_FSM_PRINTF("#####EPEVT_CLIDRX FAIL!!!!\n");
			}
		}
		break;
   
	case EPEVT_RINGDET:
      		{
			PH_FSM_PRINTF("#####EPEVT_RINGDET,endptState->lineId=%d\n",endptState->lineId);
			phoneCB = find_phoneCB(endptState->lineId, 1);
			
			if(ringcnt==1){
				PH_FSM_PRINTF("#####EPEVT_RINGDET==>FIRSTRING\n");
				if ( phoneCB != NULL ) {
					eventVoiceSimpleSend(EVT_PHONE_FXO_DETECTCID, MSG_SEND_TO_PHONE, phoneCB->logicalId);
					ringcnt=0;
				}
			} else {
				PH_FSM_PRINTF("#####EPEVT_RINGDET==>RINGRINGRING~~~~~~~~~~~\n");
				if ( phoneCB != NULL ) {
					eventVoiceSimpleSend(EVT_PHONE_FXO_RINGING, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
			}
		}
		break;
	case EPEVT_RINGEND:
		{
			PH_FSM_PRINTF("#####EPEVT_RINGEND,endptState->lineId=%d\n",endptState->lineId);
			phoneCB = find_phoneCB(endptState->lineId, 1);
			if ( phoneCB != NULL ) {
				PH_FSM_PRINTF("EPEVT_RINGEND::phoneCB->logicalId=%x\n",phoneCB->logicalId);
				eventVoiceSimpleSend(EVT_PHONE_FXO_STOP_RING, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				ringcnt=1;
			}
		}
		break;
	case EPEVT_FLASH:
		phoneCB = find_phoneCB(endptState->lineId, 1);
		if ( phoneCB != NULL ) {
			PH_FSM_PRINTF("EPEVT_FLASH::phoneCB->logicalId=%x\n",phoneCB->logicalId);
			eventVoiceSimpleSend(EVT_PHONE_FLASH, MSG_SEND_TO_PHONE, phoneCB->logicalId);
		}
		break;
	case EPEVT_FAX:
		if(endptState->lineId >=0 && endptState->lineId<MAX_ENDPT){
			if(cmPhysEndpt[endptState->lineId].toneDetection){
				phoneCB = find_phoneCB(endptState->lineId, 1);
				PH_FSM_PRINTF("send EVT_PHONE_FAX_DETECT event\n");
				if ( phoneCB != NULL ) {
					eventVoiceSimpleSend(EVT_PHONE_FAX_DETECT, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
			}
		}
		break;
	case EPEVT_MODEM:
		if(endptState->lineId >=0 && endptState->lineId<MAX_ENDPT){
			if(cmPhysEndpt[endptState->lineId].toneDetection){
				phoneCB = find_phoneCB(endptState->lineId, 1);
				PH_FSM_PRINTF("send EVT_PHONE_FAX_DETECT event\n");
				if ( phoneCB != NULL ) {
					eventVoiceSimpleSend(EVT_PHONE_FAX_DETECT, MSG_SEND_TO_PHONE, phoneCB->logicalId);
				}
			}
		}
		break;
	case EPEVT_VBD_START:
		PH_FSM_PRINTF("#####EPEVT_VBD_START\n");
		break;
	case EPEVT_VBD_STOP:
		PH_FSM_PRINTF("#####EPEVT_VBD_STOP\n");
		break;	
	case EPEVT_T38_FAILURE:
	case EPEVT_T38_STOP:
		break;

	default:
		PH_FSM_PRINTF("Undefined Event,event=%d\n",event);
		break;
	
	}

	if( ref != NULL ) {
		zyFree(ref );
	}

	(void)(cxid);
}



int GetHooKstatus(
     phoneCh_t *phoneCB
){
	int index;
	EPCASSTATE status=0;
	index=phoneCB->physicalId;
	/* Platform porting: send event or ... to physical driver to get FXS hook status */
	/* END of Platform porting */
	vrgEndptGetFxsHookStatus( &cmPhysEndpt[index].endptObjState,&status);
	return (status);
}


int PhoneInitEventCb(void){
	int rc;
	/* Platform porting: send event or ... to physical driver to register PhoneEventCb */
	/* END of Platform porting */	
	rc = vrgEndptInitEvent(PhoneEventCb);
	return rc;
}

int PhoneOpenDriver(void){
	
	/* Platform porting: send event or ... to physical driver to register PhoneEventCb */
	/* END of Platform porting */	
	vrgEndptOpenDriver();
	return 0;
}


#if 1 /*for xChange 4.02.L03*/
int PhoneRegionToCountryCode(char *region){
	/* Platform porting: set contry code mapping to dsp driver */
	/* END of Platform porting */
	int countryCode = 17; /*default to NORTHAMERICA*/
	if(region != NULL){
		if(!strncmp(region, "AU", 2)){ /*AUSTRAILIA*/
			countryCode=0;		
		}
		else if(!strncmp(region, "BE", 2)){ /*BELIGIUM*/
			countryCode=1;		
		}
		else if(!strncmp(region, "BR", 2)){ /*BRAZIL*/
			countryCode=2;		
		}
		else if(!strncmp(region, "CL", 2)){ /*CHILE*/
			countryCode=3;		
		}
		else if(!strncmp(region, "CN", 2)){ /*CHINA*/
			countryCode=4;		
		}
		else if(!strncmp(region, "CZ", 2)){ /*CZECH*/
			countryCode=5;		
		}
		else if(!strncmp(region, "DK", 2)){ /*DENMARK*/
			countryCode=6;		
		}
		else if(!strncmp(region, "XE", 2)){ /*ETSI*/
			countryCode=7;		
		}
		else if(!strncmp(region, "FI", 2)){ /*FINLAND*/
			countryCode=8;		
		}
		else if(!strncmp(region, "FR", 2)){ /*FRANCE*/
			countryCode=9;		
		}
		else if(!strncmp(region, "DE", 2)){ /*GERMANY*/
			countryCode=10;		
		}
		else if(!strncmp(region, "HU", 2)){ /*HUNGARY*/
			countryCode=11;		
		}
		else if(!strncmp(region, "IN", 2)){ /*INDIA*/
			countryCode=12;		
		}
		else if(!strncmp(region, "IT", 2)){ /*ITALY*/
			countryCode=13;		
		}
		else if(!strncmp(region, "JP", 2)){ /*JAPAN*/
			countryCode=14;			
		}
		else if(!strncmp(region, "NL", 2)){ /*NETHERLANDS*/
			countryCode=15;			
		}
		else if(!strncmp(region, "NZ", 2)){ /*NEWZEALAND*/
			countryCode=16;			
		}
		else if(!strncmp(region, "US", 2)){ /*NORTHAMERICA*/
			countryCode=17;			
		}
		else if(!strncmp(region, "ES", 2)){ /*SPAIN*/
			countryCode=18;			
		}
		else if(!strncmp(region, "SE", 2)){ /*SWEDEN*/
			countryCode=19;			
		}
		else if(!strncmp(region, "NO", 2)){ /*NORWAY*/
			countryCode=20;			
		}
		else if(!strncmp(region, "CH", 2)){ /*SWITZERLAND*/
			countryCode=21;			
		}
		else if(!strncmp(region, "XT", 2)){ /*TR57*/
			countryCode=22;			
		}
		else if(!strncmp(region, "GB", 2)){ /*UK*/
			countryCode=23;			
		}
		else if(!strncmp(region, "TW", 2)){ /*TAIWAN*/
			countryCode=24;			
		}
		else if(!strncmp(region, "PL", 2)){ /*POLAND*/
			countryCode=25;			
		}
		else if(!strncmp(region, "AE", 2)){ /*UNITEDARABEMIRATES*/
			countryCode=26;			
		}
		else if(!strncmp(region, "IR", 2)){ /*IRELAND*/
			countryCode=27;			
		}
		
		
	}

	return countryCode;
}
#else /*for xChange 4.02.L02*/
int PhoneRegionToCountryCode(char *region){
	/* Platform porting: set contry code mapping to dsp driver */
	/* END of Platform porting */
	int countryCode = 16; /*default to NORTHAMERICA*/
	if(region != NULL){
		if(!strncmp(region, "AU", 2)){ /*AUSTRAILIA*/
			countryCode=0;		
		}
		else if(!strncmp(region, "BE", 2)){ /*BELIGIUM*/
			countryCode=1;		
		}
		else if(!strncmp(region, "BR", 2)){ /*BRAZIL*/
			countryCode=2;		
		}
		else if(!strncmp(region, "CL", 2)){ /*CHILE*/
			countryCode=3;		
		}
		else if(!strncmp(region, "CN", 2)){ /*CHINA*/
			countryCode=4;		
		}
		else if(!strncmp(region, "CZ", 2)){ /*CZECH*/
			countryCode=5;		
		}
		else if(!strncmp(region, "DK", 2)){ /*DENMARK*/
			countryCode=6;		
		}
		else if(!strncmp(region, "XE", 2)){ /*ETSI*/
			countryCode=7;		
		}
		else if(!strncmp(region, "FI", 2)){ /*FINLAND*/
			countryCode=8;		
		}
		else if(!strncmp(region, "FR", 2)){ /*FRANCE*/
			countryCode=9;		
		}
		else if(!strncmp(region, "DE", 2)){ /*GERMANY*/
			countryCode=10;		
		}
		else if(!strncmp(region, "HU", 2)){ /*HUNGARY*/
			countryCode=11;		
		}
		else if(!strncmp(region, "IT", 2)){ /*ITALY*/
			countryCode=12;		
		}
		else if(!strncmp(region, "JP", 2)){ /*JAPAN*/
			countryCode=13;			
		}
		else if(!strncmp(region, "NL", 2)){ /*NETHERLANDS*/
			countryCode=14;			
		}
		else if(!strncmp(region, "NZ", 2)){ /*NEWZEALAND*/
			countryCode=15;			
		}
		else if(!strncmp(region, "US", 2)){ /*NORTHAMERICA*/
			countryCode=16;			
		}
		else if(!strncmp(region, "ES", 2)){ /*SPAIN*/
			countryCode=17;			
		}
		else if(!strncmp(region, "SE", 2)){ /*SWEDEN*/
			countryCode=18;			
		}
		else if(!strncmp(region, "NO", 2)){ /*NORWAY*/
			countryCode=19;			
		}
		else if(!strncmp(region, "CH", 2)){ /*SWITZERLAND*/
			countryCode=20;			
		}
		else if(!strncmp(region, "XT", 2)){ /*TR57*/
			countryCode=21;			
		}
		else if(!strncmp(region, "GB", 2)){ /*UK*/
			countryCode=22;			
		}
		else if(!strncmp(region, "TW", 2)){ /*TAIWAN*/
			countryCode=23;			
		}
		
		
	}

	return countryCode;
}

#endif
/*Added by Eason, move from vodsl/voip/prov/sipCli.c(4.02L.02), to support endpoint console commands */
#if 1 

/*************************************************************************
*
*  FUNCTION NAME  : displayEndptCmdHelp
*
*  DESCRIPTION    : 
*
*  RETURNS		  :
*************************************************************************/

void displayEndptCmdHelp()
{
   EPTCMDTABLE*   pEptCmdTable = endpointCommandTable;

   ZyPrintf("syntax(CLI) : voice eptcmd endpointCmd lineId [param1] [param2]\n");
   ZyPrintf(" \n");
   ZyPrintf("Available endpoint commands:\n");

   while ( pEptCmdTable->eptCmd != EPCMD_MAX )
   {
      ZyPrintf(" %s\n", pEptCmdTable->eptCmdHelpString);
      pEptCmdTable++;
   }
}


/*************************************************************************
*
*  FUNCTION NAME  : sip_endptcmd
*
*  DESCRIPTION    : This function handles Broadcom CMS message endpoint commands
*
*  RETURNS		  :
*************************************************************************/

void sip_endptcmd( char * cmdLine )
{
   ENDPT_STATE    endptState;
   char           endptCmd[MAX_EPTCMD_STRING_LEN];
   EPCMD_PARMS    consoleCmdParams;
   EPTCMDTABLE*   pEptCmdTable = endpointCommandTable;
   int executeCommand = 0;
   int i, j;
   char buffer[256];
   char *pToken = NULL, *pLast = NULL;
   int fxsEpts = MAX_PHYS_ENDPT;
   int fxoEpts = 0;
   int idx16 = 0, idx1 = 0, hex = 16;
   char hexDump[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
				 'A', 'B', 'C', 'D', 'E', 'F'
				};
   /* for use of mpicmd, Eva added*/    
   int mpi_nrArgs_def = 4;
   int mpilen_idx = 3;	 
   int rw_idx = 2;

   memset( &consoleCmdParams, 0, sizeof(consoleCmdParams) );

   strncpy(buffer, cmdLine, 256);

   STRTOK_R_MACRO( pToken, buffer, " ", &pLast );

   strncpy( endptCmd, pToken, sizeof(endptCmd) );

   if ( strncmp(endptCmd, "help", sizeof(endptCmd)) == 0 )
   {
      displayEndptCmdHelp();
      return;
   }

   while ( (strncmp(pEptCmdTable->eptCmdString, endptCmd, sizeof(endptCmd)) != 0) && (pEptCmdTable->eptCmd < EPCMD_MAX) )
   {
      pEptCmdTable++;
   }
   
   if ( pEptCmdTable->eptCmd >= EPCMD_MAX )
   {
      /* eptcmd name not matched */
      LOGERROR(("Invalid endptcmd name"));
      displayEndptCmdHelp();
      return;
   }

   if ( pLast == NULL || pLast == '\0')
   {
      LOGERROR(("Missing line number"));
      displayEndptCmdHelp();
      return;
   }
   
   STRTOK_R_MACRO( pToken, NULL, " ", &pLast );
   
   endptState.lineId = atoi(pToken);   

   
   if ( endptState.lineId >= (fxsEpts + fxoEpts) )
   {
      LOGERROR(("Line id out of bounds (larger than %d)", (fxsEpts + fxoEpts - 1) ));
      return;
   }
   
   if ( strncmp(pEptCmdTable->eptCmdString, endptCmd, sizeof(endptCmd)) == 0 )
   {
      for ( i = 0; i < pEptCmdTable->nrArguments; i++)
      {
         if ( pLast == NULL || pLast == '\0')
         {
            LOGERROR(("Invalid number of arguments"));
            LOGERROR(("%s",pEptCmdTable->eptCmdHelpString));
            return;
         }
         STRTOK_R_MACRO( pToken, NULL, " ", &pLast );


		 if ( strstr(pToken, "0x") != 0 ){ /* for hex format, Eva added */
		 	for ( j=0 ; j<hex ; j++){
				if ( *(pToken+2) == hexDump[j] ) idx16=j; 
				if ( *(pToken+3) == hexDump[j] ) idx1=j;
			}
			consoleCmdParams.arg[i] = idx16*hex + idx1;
		 }
		 else consoleCmdParams.arg[i] = atoi(pToken);

		 /* for mpicmd, the number of data to write into the register is variable, Eva added */	
		 if ( i == mpilen_idx && pEptCmdTable->eptCmd == EPCMD_MPI_CMD ){
		 	if ( (consoleCmdParams.arg[rw_idx]%2) == 0 ){
				pEptCmdTable->nrArguments += consoleCmdParams.arg[mpilen_idx];	
			}
			else{ 
				pEptCmdTable->nrArguments = mpi_nrArgs_def;	
			}
	     } 


      }
      executeCommand = 1;
   }

   ZyPrintf("Sending endpt console command: %s %d",endptCmd,endptState.lineId);
   for( i = 0; i<pEptCmdTable->nrArguments; i++)
   {
      ZyPrintf(" %d",consoleCmdParams.arg[i]);
   }
   ZyPrintf("\n");
   
   bosSleep( FIVE_HUNDRED_MSEC );
   
   if ( executeCommand == 1 )
   {
      vrgEndptConsoleCmd( &endptState, pEptCmdTable->eptCmd, &consoleCmdParams );
   }
   else
   {
      LOGERROR(("Endpoint command not executed"));
   }

   return;
}
#endif
#ifdef BUILD_TDC_CUSTOMIZATION//_MSTC_, JuWen, replace phone number replace the + with 00
char *phoneNum_replace (char *source, char find, char *rep, char *result){  	 
	       
	    if(source[0] == find){		
			sprintf(result,"%s%s",rep,source+1);
			return result;
	   }
	   else{
			return NULL;
	   }
}  
#endif
