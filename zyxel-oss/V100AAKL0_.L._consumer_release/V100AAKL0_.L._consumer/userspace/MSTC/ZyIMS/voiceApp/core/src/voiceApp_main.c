#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "icf_types.h"
#include "icf_defs.h"
#include "icf_api.h"
#include "icf_clib.h"
#include "global.h"
#include "sys_msg.h"
#include "sys_itf.h"
#include "config_sys.h"
#include "call_mgnt.h"
#include "phfsm.h"
#include "voiceApp_main.h"
#include "voiceApp_iptk_api.h"
#include "voiceApp_iptk_MsgHandler.h"
#include "icf_clib.h"
#include "dialplan.h"
#include "tr104.h"
#include "tr104_global.h"
/*sipAccountNumber is a global variable in TR104 Module which is stored the total sip account number */
#include "tr104_init.h"

/*____________________________________________________________________________*/
/*	function declaration                                                                                                     */
/*____________________________________________________________________________*/

/*____________________________________________________________________________*/
/*	data declaration                                                                                                          */
/*____________________________________________________________________________*/
/*_____ global variables _______________________________________________*/
int voiceAppDebug = 0;
/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
uint8 log_dest = 0;
/* Indicates whether VoIP was active */
bool VoIPIsActive = FALSE;
uint8 *ICFLineInitDone = NULL;

/*initialize according to line number*/
webSipStatus_t *webSipStatus;

extern uint32 sipAccountNumber;	
int APICFMInitflag = 0;
int IPTKCfgInitTimer = 0;
#if 1 /*[RD Internal] Register retry timer, Jason*/
voiceAppRegTimer_t* reg_timer=NULL;
extern tm_id_t reg_status_timer;
extern int voiceChangedFlag;
extern int voiceCriticalChangedFlag;
extern int *reg_check_cnt;
extern int dspReinitNeeded;
#endif
/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/	
extern voiceAppICFErrRec_t voiceAppICFErrRec;

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
/*____________________________________________________________________________*/
/*	Functions                                                                                                                    */
/*____________________________________________________________________________*/

/*************************************************************************
*
*  FUNCTION NAME  : voiceAppInit
*
*  DESCRIPTION    : Initialize struct globalVoiceApp
*
*  RETURNS		  :
*************************************************************************/
static void voiceAppInit(){
	globalVoiceApp.appId = 1;
	globalVoiceApp.voiceApp_ccm_fd = -1;
	globalVoiceApp.voiceApp_iptk_fd = -1;
	globalVoiceApp.ccm_fd = -1;
	globalVoiceApp.phone_fd = -1;
	globalVoiceApp.voiceApp_mm_fd = -1;
	globalVoiceApp.config_fd = -1;
	
}

/*************************************************************************
*
*  FUNCTION NAME  : voiceAppIpcInit
*
*  DESCRIPTION    : Initialize IPC sockets for VoiceApp usage
*
*  RETURNS		  : bool
*************************************************************************/
static bool voiceAppIpcInit(void) {
	struct sockaddr_un addr;
	bool ret_val = TRUE;

	/* init ipc socket for IPTK */
	globalVoiceApp.voiceApp_iptk_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(globalVoiceApp.voiceApp_iptk_fd < 0) {
		globalVoiceApp.voiceApp_iptk_fd = -1;
		ret_val = FALSE;
	} else {
		bzero(&addr, sizeof(addr));
		addr.sun_family = PF_LOCAL;
		zyStrncpy(addr.sun_path, VOICEAPP_IPTK_CHANNEL_PATH, sizeof(VOICEAPP_IPTK_CHANNEL_PATH));
		unlink(VOICEAPP_IPTK_CHANNEL_PATH);
		if ( bind( globalVoiceApp.voiceApp_iptk_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
			ZyPrintf("voiceApp : Can't bind IPC socket for IPTK, start application fail.\n");
			close(globalVoiceApp.voiceApp_iptk_fd);
			ret_val = FALSE;
		}
	}


	/* init ipc socket for CCM */
	globalVoiceApp.voiceApp_ccm_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(globalVoiceApp.voiceApp_ccm_fd < 0) {
		globalVoiceApp.voiceApp_ccm_fd = -1;
		ret_val = FALSE;
	} else {
		bzero(&addr, sizeof(addr));
		addr.sun_family = PF_LOCAL;
		zyStrncpy(addr.sun_path, VOICEAPP_CCM_CHANNEL_PATH, sizeof(VOICEAPP_CCM_CHANNEL_PATH));
		unlink(VOICEAPP_CCM_CHANNEL_PATH);
		if ( bind( globalVoiceApp.voiceApp_ccm_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
			ZyPrintf("voiceApp : Can't bind IPC socket for CCM, start application fail.\n");
			close(globalVoiceApp.voiceApp_ccm_fd);
			ret_val = FALSE;
		}
	}

	/* init ipc socket for MM */
		globalVoiceApp.voiceApp_mm_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
		if(globalVoiceApp.voiceApp_mm_fd < 0) {
			globalVoiceApp.voiceApp_mm_fd = ICF_FAILURE;
			ret_val = ICF_FAILURE;
		} else {
			bzero(&addr, sizeof(addr));
			addr.sun_family = PF_LOCAL;
			zyStrncpy(addr.sun_path, VOICEAPP_MM_CHANNEL_PATH, sizeof(VOICEAPP_MM_CHANNEL_PATH));
			unlink(VOICEAPP_MM_CHANNEL_PATH);
			if ( bind( globalVoiceApp.voiceApp_mm_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
				ZyPrintf("voiceApp : Can't bind IPC socket for CCM, start application fail.\n");
				close(globalVoiceApp.voiceApp_mm_fd);
				ret_val = ICF_FAILURE;
		}
	}

	/* init ipc socket for config */
		globalVoiceApp.config_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
		if(globalVoiceApp.config_fd < 0) {
			globalVoiceApp.config_fd = ICF_FAILURE;
			ret_val = ICF_FAILURE;
		} else {
			bzero(&addr, sizeof(addr));
			addr.sun_family = PF_LOCAL;
			zyStrncpy(addr.sun_path, CONFIG_CHANNEL_PATH, sizeof(CONFIG_CHANNEL_PATH));
			unlink(CONFIG_CHANNEL_PATH);
			if ( bind( globalVoiceApp.config_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
				ZyPrintf("voiceApp : Can't bind IPC socket for config, start application fail.\n");
				close(globalVoiceApp.config_fd);
				ret_val = ICF_FAILURE;
		}
	}

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : voiceAppTask
*
*  DESCRIPTION    : This function receives events from IPTK, CCM, phone
*
*  RETURNS		  :
*************************************************************************/
void voiceAppTask( void )
{
	int maxfd = 0;
	uint8 *recvBuf = NULL;
	int	recvLen = 0;
	fd_set 	allset;
	
	#if 1/*ZyXEL, rt_priority_Eason*/
	nice(-10); /*increase voiceApp priority*/
	ZyPrintf("%s, getpid = %d\n", __FUNCTION__, getpid());
    #endif
	/*Eason, register application signal handler*/
	AppRegisterSignal();
	/* IPC socket handler */
	FD_ZERO(&allset);

	while(1) {
		FD_SET(globalVoiceApp.voiceApp_iptk_fd, &allset);
		FD_SET(globalVoiceApp.voiceApp_ccm_fd, &allset);
		FD_SET(globalVoiceApp.ccm_fd, &allset);
		FD_SET(globalVoiceApp.phone_fd, &allset);
		FD_SET(globalVoiceApp.config_fd, &allset);
		
		if (globalVoiceApp.voiceApp_iptk_fd > maxfd)
			maxfd = globalVoiceApp.voiceApp_iptk_fd;
		if (globalVoiceApp.voiceApp_ccm_fd > maxfd)
			maxfd = globalVoiceApp.voiceApp_ccm_fd;
		if (globalVoiceApp.ccm_fd > maxfd)
			maxfd = globalVoiceApp.ccm_fd;
		if (globalVoiceApp.phone_fd > maxfd)
			maxfd = globalVoiceApp.phone_fd;
		if (globalVoiceApp.config_fd > maxfd)
			maxfd = globalVoiceApp.config_fd;
		
		if((select(maxfd+1, &allset, NULL, NULL,NULL)) < 0 ) {
			ZyPrintf("select error!!!\n");
		} else {		
			if(FD_ISSET(globalVoiceApp.voiceApp_iptk_fd, &allset)) {
				/* event from IPTK to voiceApp*/
				if((recvBuf = zyMalloc(MAX_MTU_SIZE)) != NULL) {
					recvLen = recvfrom(globalVoiceApp.voiceApp_iptk_fd, recvBuf, MAX_MTU_SIZE, 0, NULL, NULL);
					if(recvLen > 0) {
						voiceAppIPTKMsgProcess(recvBuf, recvLen);
					}
					zyFree(recvBuf);
				}
			} else if(FD_ISSET(globalVoiceApp.voiceApp_ccm_fd, &allset)) {
				/* event from CCM to voiceApp*/
				if((recvBuf = zyMalloc(sizeof(evtMsg_t))) != NULL) {
					recvLen = recvfrom(globalVoiceApp.voiceApp_ccm_fd, recvBuf, sizeof(evtMsg_t), 0, NULL, NULL);
					if (recvLen == sizeof(evtMsg_t)) {
						CCMEvtToSip((evtMsg_t*)recvBuf);
					}
					zyFree(recvBuf);
				}
			} else if(FD_ISSET(globalVoiceApp.ccm_fd, &allset)) {
				/* event to CCM */
				if((recvBuf = zyMalloc(sizeof(evtMsg_t))) != NULL) {
					recvLen = recvfrom(globalVoiceApp.ccm_fd, recvBuf, sizeof(evtMsg_t), 0, NULL, NULL);
					if (recvLen == sizeof(evtMsg_t)) {
						ccmConnFsmProc((evtMsg_t *)recvBuf);
					}
					zyFree(recvBuf);
				}
			} else if(FD_ISSET(globalVoiceApp.phone_fd, &allset)) {
				/* event to phone */
				if((recvBuf = zyMalloc(sizeof(evtMsg_t))) != NULL) {
					recvLen = recvfrom(globalVoiceApp.phone_fd, recvBuf, sizeof(evtMsg_t), 0, NULL, NULL);
					if (recvLen == sizeof(evtMsg_t)) {
						phoneFsmProc((evtMsg_t *)recvBuf);
					}
					zyFree(recvBuf);
				}
			} else if(FD_ISSET(globalVoiceApp.config_fd, &allset)) {
				/* event from MM to voiceApp for config */
				if((recvBuf = zyMalloc(sizeof(rtpPortMsg_t))) != NULL) {
					recvLen = recvfrom(globalVoiceApp.config_fd, recvBuf, sizeof(rtpPortMsg_t), 0, NULL, NULL);
					if (recvLen == sizeof(rtpPortMsg_t)) {
						processMMConfigMsg((rtpPortMsg_t *)recvBuf);
					}
					zyFree(recvBuf);
				}
			} 

		}
	}
}

/*************************************************************************
*
*  FUNCTION NAME  : voiceAppTaskCreate
*
*  DESCRIPTION    : This function create voiceApp task
*
*  RETURNS		  : int
*************************************************************************/
static int voiceAppTaskCreate( void )
{
	int  status;
   
	status = taskCreate( "voiceApp",
				64*1024,
				#if 1/*ZyXEL, rt_priority_Eason*/
				TASK_CLASS_HIGH,
				#else
				TASK_CLASS_MED_HIGH,
				#endif
				(void *)voiceAppTask);

	if ( status != RET_SUCCESS ) {
		ZyPrintf("!!%s fail !!\n", __FUNCTION__);
		return RET_FAILURE;
	}
	return RET_SUCCESS;  
}

#ifdef BUILD_IGNORE_DIRECT_IP
void initFirewallRule()
{
#ifdef NORWAY_VOIP_CUSTOMIZATION
	setVoIPFilterRule();     // Because bootup time is too long,  move voipfilterrule setting form bootup to this.  
#endif

	system("iptables -A VOIP_INPUT");
	system("iptables -N VOIP_INPUT");
	system("iptables -I INPUT -p udp -j VOIP_INPUT;iptables -I INPUT -p tcp -j VOIP_INPUT");
#ifdef NORWAY_VOIP_CUSTOMIZATION
	rutSys_setVoIPFilterRule(NULL, NULL, 0);
#endif
}
#endif
/*************************************************************************
*
*  FUNCTION NAME  : voiceApp
*
*  DESCRIPTION    : main function to start voiceApp application
*
*  RETURNS		  :
*************************************************************************/
/* main function to start voiceApp application */
static void voiceApp(void){
        /*Eason, register application signal handler*/
	AppRegisterSignal();
	voiceAppInit();
	vendorOsInit();
	UITimer_Init();
	configSysInit();

#ifdef BUILD_IGNORE_DIRECT_IP
	initFirewallRule();     
#endif
	
	if( TR104_ERROR == tr104ModuleInit()){
		ZyPrintf("TR104 Module init fail, exit application\n");
		return;
	};
	/*initialize webSipStatus according to line number*/
	webSipStatus = (webSipStatus_t *)zyMalloc(sizeof(webSipStatus_t)*tr104GetMaxLine());
	zyMemset(webSipStatus, 0 , sizeof(webSipStatus_t)*tr104GetMaxLine());
	/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/	
	zyMemset(&voiceAppICFErrRec, 0 ,sizeof(voiceAppICFErrRec_t));
#ifdef DIAL_PLAN
	initDialPlan();
#endif
	callCtrlInit();
	phoneInit();
	#ifdef CALLHISTORY_SUPPORT
	webCallHistoryInit();
	#endif
	if(voiceAppIpcInit() == FALSE) {
		ZyPrintf("IPC socket init fail, exit application\n");
		return;
	}
	if(0 != voiceDspInit()){
		ZyPrintf("VOICE DSP init fail, exit application");
		return;
	}

	if (FALSE == voiceAppAddNewApp()) {
		ZyPrintf("voiceApp > APP_ADD_REQ towards IP Phone Toolkit Failed.\n");
		return;
	}
	initSipCCMConvert();
#ifdef ICF_TRACE_ENABLE
	iptkTraceInit();
#endif
	if(RET_FAILURE == voiceAppTaskCreate()) {
		return;
	}
	configSysTask();
	
}

/*************************************************************************
*
*  FUNCTION NAME  : main
*
*  DESCRIPTION      : Main function of voiceApp
*
*  RETURNS		  : int
*************************************************************************/
int main(int argc, char *argv[])
{	
	voiceApp();
	return 1;
}

/*************************************************************************
*
*  FUNCTION NAME  : ccmIpcInit
*
*  DESCRIPTION    : Initialize IPC sockets for CCM usage
*
*  RETURNS		  : bool
*************************************************************************/
bool ccmIpcInit(void) {
	struct sockaddr_un addr;
	bool ret_val = TRUE;

	/* init ipc socket for ccm */
	globalVoiceApp.ccm_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(globalVoiceApp.ccm_fd < 0) {
		globalVoiceApp.ccm_fd = -1;
		ret_val = FALSE;
	} else {
		bzero(&addr, sizeof(addr));
		addr.sun_family = PF_LOCAL;
		zyStrncpy(addr.sun_path, CCM_CHANNEL_PATH, sizeof(CCM_CHANNEL_PATH));
		unlink(CCM_CHANNEL_PATH);
		if ( bind( globalVoiceApp.ccm_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
			ZyPrintf("ccmIpcInit : Can't bind IPC socket for CCM\n");
			close(globalVoiceApp.ccm_fd);
			ret_val = FALSE;
		}
	}

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : phoneIpcInit
*
*  DESCRIPTION    : Initialize IPC sockets for phone usage
*
*  RETURNS		  : bool
*************************************************************************/
bool phoneIpcInit(void) {
	struct sockaddr_un addr;
	bool ret_val = TRUE;

	/* init ipc socket for phone */
	globalVoiceApp.phone_fd = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if(globalVoiceApp.phone_fd < 0) {
		globalVoiceApp.phone_fd = -1;
		ret_val = FALSE;
	} else {
		bzero(&addr, sizeof(addr));
		addr.sun_family = PF_LOCAL;
		zyStrncpy(addr.sun_path, PHONE_CHANNEL_PATH, sizeof(PHONE_CHANNEL_PATH));
		unlink(PHONE_CHANNEL_PATH);
		if ( bind( globalVoiceApp.phone_fd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
			ZyPrintf("ccmIpcInit : Can't bind IPC socket for PHONE\n");
			close(globalVoiceApp.phone_fd);
			ret_val = FALSE;
		}
	}

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : voipActivate
*
*  DESCRIPTION    : This function will activate VoIP service
*
*  RETURNS		  :
*************************************************************************/
bool voipActivate(void)
{
	VOICEAPP_PRINTF("%s > ENTER \n",__FUNCTION__);

	if(!VoIPIsActive) {
		int SipAccountId = 0 , VpItr = 0 , numofProf=0;	
		int markIdx=0;
		VoiceProfObj_t* pProf = NULL;	

		/*reset Timer and Flag*/
		zyMemset((void *)reg_timer, 0, sipAccountNumber*sizeof(voiceAppRegTimer_t));			
		if(reg_status_timer != 0){
			VOICEAPP_PRINTF("%s,Stop reg_status_timer (%d) \n",__FUNCTION__,reg_status_timer);			
			timerPeriodicStop(reg_status_timer);
			reg_status_timer = 0;
		}	
		voiceAppIPTKAcquireMutex();
		APICFMInitflag = 0;
		VOICEAPP_PRINTF("%s,%d : voiceapp-acif(reset)APICFMInitflag!!\n" , __FUNCTION__,APICFMInitflag);									
		voiceAppIPTKReleaseMutex();
		
		if(FALSE == mmCfgInit()) {
			VOICEAPP_PRINTF("mmCfgInit > FALSE \n");			
			return FALSE;
		}
	
		numofProf = tr104GetNumProfile();	
		for(VpItr=0; VpItr<numofProf;VpItr++){
			pProf = tr104VoiceProfObjPtrGet(VpItr);
			if(pProf!=NULL && pProf->enable){
				VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
				if(pProfSip == NULL){
					ZyPrintf("%s > pProf(%d)Sip is NULL \n",__FUNCTION__,VpItr);						
				}else{
					#if 0  /* MSTC porting by Andylin, bind interface , Iproute2 */
					/* add static route for SIP */
					addVoipStaticRoute(pProfSip->proxyServer , SipAccountId);
					addVoipStaticRoute(pProfSip->registrarServer , SipAccountId);
					#else
					addVoipSIPIproute2(pProf->ZyXEL_BoundIfName,pProf->ZyXEL_BoundIfList ,pProf->ZyXEL_ActiveBoundIfName,pProfSip->registrarServer, pProfSip->userAgentPort, pProfSip->userAgentTransport);
					#endif
					
					#if 0
					delVoipFirewallAclAll();
					#endif
					/* add ACL rule for SIP */
					#if 1 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
	                /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
						#ifdef MSTC_VOICE_AUTO_QOS
					        addVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->userAgentTransport , pProf->ZyXEL_ActiveBoundIfName);
					    #elif defined(MSTC_VOICE_VLAN_ID_MARK)
						addVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr, (pProfSip!=NULL)?pProfSip->VLANIDMark:-1, (pProfSip!=NULL)?pProfSip->ethernetPriorityMark:-1);
					        #else
						addVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr);
						#endif
					#else
					addVoipFirewallAcl(pProfSip->userAgentPort, pProfSip->proxyServerTransport , SipAccountId);
					addVoipQosClass(pProfSip->userAgentPort, pProfSip->proxyServerTransport);
					#endif
					if( TR104I_TRANSPORT_TLS == pProfSip->proxyServerTransport){
						#if 1 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
						/* add 5061 port for TLS*/
	                    /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
							#ifdef MSTC_VOICE_AUTO_QOS
							addVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->userAgentTransport , pProf->ZyXEL_ActiveBoundIfName);
							#elif defined(MSTC_VOICE_VLAN_ID_MARK)
							addVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr, (pProfSip!=NULL)?pProfSip->VLANIDMark:-1, (pProfSip!=NULL)?pProfSip->ethernetPriorityMark:-1);
							#else
							addVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr);
							#endif
						#else
						addVoipFirewallAcl(pProfSip->proxyServerPort, pProfSip->proxyServerTransport , SipAccountId);
						#endif
					}
					#if 0
					delVoipQosClassAll();
					#endif
					/* add QoS classification for SIP */		

#ifdef BUILD_IGNORE_DIRECT_IP
					/* When SIP config was changed: 2. add rules into VOIP_INPUT chain */
					VoiceProfObj_t* pProfObj =tr104VoiceProfObjPtrGet(VpItr);
					if(pProfObj->numberOfLines > 0 && pProfObj->ZyXEL_ignoreDirectIP==1) {
						VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
						if(strcmp(pProfSip->proxyServer,"0.0.0.0") != 0){
							addVoipFirewallDrop(pProfObj->ZyXEL_ActiveBoundIfName,pProfObj->ZyXEL_BoundIfList, pProfSip->proxyServer, pProfSip->proxyServerPort, pProfSip->proxyServerTransport);
						}
					}
#endif

				
				}
			}
		}				
#ifdef NORWAY_VOIP_CUSTOMIZATION	
		rutSys_setVoIPFilterRule(NULL,NULL,0);
#endif
	
		#if 1 /*Jason, test*/
		IPTKCfgInitTimer = timerStartS(150, iptkCfgInit,NULL);
		#endif
		VoIPIsActive = TRUE;
		VOICEAPP_PRINTF("voipActivate end \n");
	}
	VOICEAPP_PRINTF("%s > EXIT \n",__FUNCTION__);	
	return TRUE;
}

/*************************************************************************
*
*  FUNCTION NAME  : voipDeactivate
*
*  DESCRIPTION    : This function will deactivate VoIP service
*
*  RETURNS		  :
*************************************************************************/
bool voipDeactivate(void)
{
	VOICEAPP_PRINTF("%s Enter\n",__FUNCTION__);
	int SipAccountId = 0 , VpItr = 0, numofProf=0;	
	VoiceProfObj_t* pProf = NULL; 

	while(IPTKCfgInitTimer > 0) {	
		VOICEAPP_PRINTF("%s> IPTKCfgInitTimer = %d, sleep 100s\n",__FUNCTION__,IPTKCfgInitTimer);			
		usleep(100000);
	}
	/* MSTC , fix IPTKCfgInitTimer looping whan iptkCfgInit return false, by Andylin 20120605 */
	if(IPTKCfgInitTimer != 0){
		ZyPrintf("voiceApp > iptkcfgInit Failed. Error return %d\n", IPTKCfgInitTimer);
	}
	
	if(VoIPIsActive) {

		/*Stop Timer*/
		if(reg_status_timer != 0){
			VOICEAPP_PRINTF("%s,Stop reg_status_timer (%d). \n",__FUNCTION__,reg_status_timer);			
			timerPeriodicStop(reg_status_timer);
			reg_status_timer = 0;
		}
		
		for(SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++){
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
			reg_check_cnt[SipAccountId]=0;
		}
		
		iptkCfgReset();

		numofProf = tr104GetNumProfile();
		for(VpItr=0; VpItr<numofProf;VpItr++){
			pProf = tr104VoiceProfObjPtrGet(VpItr);
			if(pProf!=NULL && pProf->enable){
				VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
				if(pProfSip == NULL){
					ZyPrintf("%s > pProf(%d)Sip is NULL \n",__FUNCTION__,VpItr);						
				}else{
					VOICEAPP_PRINTF("%s > (%d),del ip route2 setting for SIP \n",__FUNCTION__,VpItr);				
#if 0  /* MSTC porting by Andylin, bind interface , Iproute2 */
					/* delete static route for SIP */
					delVoipStaticRoute(pProfSip->proxyServer , SipAccountId);
					delVoipStaticRoute(pProfSip->registrarServer , SipAccountId);
#else
					resetVoipSIPIproute2();
#endif
					/* delete ACL rule for SIP */
					VOICEAPP_PRINTF("%s > (%d),del ACL rule for SIP \n",__FUNCTION__,VpItr);					
#if 1 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
						 /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
					#ifdef MSTC_VOICE_AUTO_QOS			
					delVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->userAgentTransport, pProf->ZyXEL_ActiveBoundIfName);
                    #elif defined(MSTC_VOICE_VLAN_ID_MARK)
					delVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr, (pProfSip!=NULL)?pProfSip->VLANIDMark:-1, (pProfSip!=NULL)?pProfSip->ethernetPriorityMark:-1);
                                        #else
					delVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr);
					#endif
#else
					delVoipFirewallAcl(pProfSip->userAgentPort, pProfSip->proxyServerTransport , SipAccountId);
					/* delete QoS classification for SIP */
					delVoipQosClass(pProfSip->userAgentPort, pProfSip->proxyServerTransport);
#endif
					if( TR104I_TRANSPORT_TLS == pProfSip->proxyServerTransport){
	#if 1 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
						/* add 5061 port for TLS*/
							  /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
						#ifdef MSTC_VOICE_AUTO_QOS			
						delVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->userAgentTransport, pProf->ZyXEL_ActiveBoundIfName);
						#elif defined(MSTC_VOICE_VLAN_ID_MARK)
						delVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr, (pProfSip!=NULL)?pProfSip->VLANIDMark:-1, (pProfSip!=NULL)?pProfSip->ethernetPriorityMark:-1);
						#else
						delVoIPQoSFirewall(pProfSip->userAgentPort, pProfSip->proxyServerTransport , VpItr);
						#endif
	#else
						delVoipFirewallAcl(pProfSip->proxyServerPort, pProfSip->proxyServerTransport , SipAccountId);
	#endif
					}


#ifdef BUILD_IGNORE_DIRECT_IP
					/* When SIP config was changed: 2. add rules into VOIP_INPUT chain */
					VoiceProfObj_t* pProfObj =tr104VoiceProfObjPtrGet(VpItr);
					if(pProfObj->numberOfLines > 0 && pProfObj->ZyXEL_ignoreDirectIP==1) {
						VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
						if(strcmp(pProfSip->proxyServer,"0.0.0.0") != 0){
								delVoipFirewallDrop(pProfObj->ZyXEL_ActiveBoundIfName,pProfObj->ZyXEL_BoundIfList, pProfSip->proxyServer, pProfSip->proxyServerPort, pProfSip->proxyServerTransport);
						}
					}
#endif
	
				
				}
			}
		}

		VOICEAPP_PRINTF("%s > Clear ipv4 address \n",__FUNCTION__);			
		clear_iface_ipv4_address_list();
		VoIPIsActive = FALSE;
		VOICEAPP_PRINTF("voipDeactivate end \n");
	}	
	VOICEAPP_PRINTF("%s EXIT\n",__FUNCTION__);	
	return TRUE;
}

/*Add some debug message in voiceApp for future debugging.,Mitrastar, Jason,20120913*/
void VoiceAppShowStatus(void)
{
	int idx=0;
	voiceAppICF_Fail_rereg_t *ReRegErr_p =NULL;
	voiceAppICF_Err_ind_t * ErrInd_p=NULL;
	voiceAppICF_Dnsmapchg_ind_t *DNSMapChgInd_p=NULL;	

	/*Show Flag*/
	ZyPrintf("=== Show Flag ===\n");		
	ZyPrintf("VoIPIsActive = %d\n",VoIPIsActive);	
	ZyPrintf("voiceChangedFlag = %d\n",voiceChangedFlag);
	ZyPrintf("voiceCriticalChangedFlag = %d\n",voiceCriticalChangedFlag);
	ZyPrintf("APICFMInitflag = %d\n",APICFMInitflag);
	ZyPrintf("dspReinitNeeded = %d\n",dspReinitNeeded);	
	ZyPrintf("sipAccountNumber = %d\n",sipAccountNumber);
	if(ICFLineInitDone!=NULL){	
		for(idx=0;idx<sipAccountNumber;idx++){	
			ZyPrintf("ICFLineInitDone[%d] = %u\n",idx,ICFLineInitDone[idx]);				
		}	
	}else{
		ZyPrintf("ICFLineInitDone is NULL\n");	
	}
	/*Show last registration*/
	for(idx=0;idx<sipAccountNumber;idx++){	
		ZyPrintf("lastRegisteration[%d] = %ld\n",idx,webSipStatus[idx].lastRegisteration);				
	}

	/*Show Timer*/
	ZyPrintf("=== Show Timer ===\n");
	if(reg_check_cnt!=NULL){
		for(idx=0;idx<sipAccountNumber;idx++){
			ZyPrintf("reg_check_cnt[%u]:%d\n",idx,reg_check_cnt[idx]);	
		}
	}else{
		ZyPrintf("reg_check_cnt is NULL\n");	
	}

	if(reg_timer != NULL){
		for(idx=0;idx<sipAccountNumber;idx++){
			ZyPrintf("reg_timer[%u]= reg [%d],sub [%d]\n",idx,reg_timer[idx].regRetryTimer,reg_timer[idx].subscribeTimer);	
		}
	}else{
		ZyPrintf("reg_timer is NULL\n");	
	}
	ZyPrintf("reg_status_timer=%d \n",reg_status_timer);
	ZyPrintf("CfgInitTimer=%d \n",IPTKCfgInitTimer);	

	/*[RD Internal] Support IP Route2 for P876,Show IP route2 Setting and status.,__CHT__,Jason,20120905*/
	ZyPrintf("=== Show SIP route ===\n");	
	if(ipmark_list_H!=NULL){
		ipmarklist_t *tmpentry = NULL;	
		ipmarklist_t *preventry = NULL;

		tmpentry = ipmark_list_H;	
		while(tmpentry !=NULL){	
			ZyPrintf("ifname=%s , mark =%d \n",tmpentry->ifname,tmpentry->mark_index);			
			tmpentry = tmpentry->next;									
		}			
	}
	ZyPrintf("=== Show RTP route ===\n");		
	if(voipRtp_list_H!=NULL){
		Rtplist_t *tmpentry = NULL;	
		Rtplist_t *preventry = NULL;

		tmpentry = voipRtp_list_H;	
		while(tmpentry !=NULL){	
			ZyPrintf("bindIfname=%s , sport =%u ,dport=%u\n",tmpentry->bindIfname,tmpentry->sport,tmpentry->dport);			
			tmpentry = tmpentry->next;									
		}			
	}

	ZyPrintf("=== Show Err Ind ===\n");
	ZyPrintf("Next Idx : %u \n",voiceAppICFErrRec.errIndNext);
	for(idx=0;idx<VOICEAPP_ICF_MAX_ERR_REC;idx++){
		ErrInd_p = &(voiceAppICFErrRec.lastErrInd[idx]);
		if(ErrInd_p->error_id != 0){
			ZyPrintf("%d : %u/%s \n",idx,ErrInd_p->error_id,ErrInd_p->error_str);
		}
	}
	ZyPrintf("=== Show Re-Reg Ind ===\n");
	ZyPrintf("Next Idx : %u \n",voiceAppICFErrRec.failreRegNext);	
	for(idx=0;idx<VOICEAPP_ICF_MAX_REREG_REC;idx++){
		ReRegErr_p = &(voiceAppICFErrRec.lastReRegErr[idx]);
		if(ReRegErr_p->request_type!=0){
			ZyPrintf("%d : %u/%u/%u/%u/%ld \n",idx,
				ReRegErr_p->line_id,
				ReRegErr_p->request_type,
				ReRegErr_p->error_cause,
				ReRegErr_p->response_code,
				ReRegErr_p->timerec);
		}
	}
	ZyPrintf("=== Show DNS chg Mapping ===\n");	
	ZyPrintf("Next Idx : %u",voiceAppICFErrRec.DNSMapChgNext);	
	for(idx=0;idx<VOICEAPP_ICF_MAX_DNS_REC;idx++){
		DNSMapChgInd_p = &(voiceAppICFErrRec.lastDNSMapChgInd[idx]);
		if(DNSMapChgInd_p->record_type!=0){
			ZyPrintf("%d : %u/%u/%s/%s \n",idx,DNSMapChgInd_p->record_type,DNSMapChgInd_p->cause,DNSMapChgInd_p->fqdn,DNSMapChgInd_p->ip);
		}
	}


	ZyPrintf("\n === rt_tables ===\n");		
	system("cat /var/iproute2/rt_tables");
	ZyPrintf("\n === iptables mangle ===\n");		
	system("iptables -t mangle -L -v");
	ZyPrintf("\n === ip rule ===\n");	
	system("ip rule");	
	ZyPrintf("\n === ip route ===\n");	
	system("ip route");
	
}



