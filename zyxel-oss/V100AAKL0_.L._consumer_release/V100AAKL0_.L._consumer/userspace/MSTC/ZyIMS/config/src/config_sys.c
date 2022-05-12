#include <netdb.h>
#include <sys/socket.h>
#include "global.h"
#include "call_mgnt.h"
#include "Voipccm_sip_itf.h"
#include "voiceApp_main.h"
#include "commands.h"
#include "tr104.h"
#include "tr104_voicemdm.h"
#include "switch.h"
#include "dialplan.h"
#include "config_sys.h"
#include "phfsm.h"
#include "tr104_init.h"
#include <errno.h>

#ifdef CALLHISTORY_SUPPORT
#include "webCallHistory.h"
#endif
#include <time.h>

#if 1	/* Broadcom private */
/* Broadcom CMS */
#include <cms.h>
#include <cms_util.h>
#include <cms_core.h>
#include <cms_msg.h>
#include <cms_log.h>
#include <dal_voice.h> 


/*____________________________________________________________________________*/
/*	data declaration                                                                                                          */
/*____________________________________________________________________________*/
/*_____ global variables _______________________________________________*/

/* Indicates whether a change in voice parameters in mdm was recorded */
int voiceChangedFlag = 0;
int voiceCriticalChangedFlag = 0;
void *msgHandle=NULL;
#ifndef MSTC_VOICE_AUTO_QOS /* MSTC , Andy add compiler flag */
/*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
typedef struct qosFirewallPort{
	uint32 port;
	uint32 protocol;
}qosFirewallPort_t;
qosFirewallPort_t voipQosFirewallPort[10] = {0};
#endif
#ifdef CALLHISTORY_SUPPORT
extern webCallHistoryBuffer_t *webCallHistory_pool;
#endif
extern webSipStatus_t *webSipStatus;
tm_id_t checkVoipInUseTimerId = -1;
extern sipCCMConvert_t sipCCMConvert;
#if 1 /*Eason, check if country code changed*/
int dspReinitNeeded = 0;
#endif

extern uint32 sipAccountNumber;	

#ifdef MSTC_VOICE_GR909	/* klose, support GR909 */
typedef unsigned short LtLineVMaskType;
typedef unsigned short LtRohMaskType;
typedef unsigned short LtRingersMaskType;
typedef unsigned short LtResFltMaskType;
typedef unsigned short LtMsrmntStatusMaskType;

/* Line test voltage type; In units of milli Volts */
typedef signed long LtVoltageType;
/* Line test Ringer Equivalence Number (REN) type; In units of milli REN */
typedef signed long LtRenType;
/* Line test impedance type; In units of tenth of a Ohm */
typedef signed long LtImpedanceType;

typedef struct ENDPOINTDRV_LT_STATISTICS_PARM
{
   int lineId;
   int mode;
   
   LtLineVMaskType fltMask_1; /* Can be any or'ed comb of this type */
   LtVoltageType vAcTip;
   LtVoltageType vAcRing;
   LtVoltageType vAcDiff;
   LtVoltageType vDcTip;
   LtVoltageType vDcRing;
   LtVoltageType vDcDiff;
   LtMsrmntStatusMaskType measStatus_1;
   
   LtRohMaskType fltMask_2;
   LtImpedanceType rLoop1;
   LtImpedanceType rLoop2;
   LtMsrmntStatusMaskType measStatus_2;
   
   LtRingersMaskType fltMask_3;
   LtRenType ren;      /* REN from the Tip to Ring */
   LtRenType rentg;    /* REN from the Tip lead to ground */
   LtRenType renrg;    /* REN from the Ring lead to ground */
   LtMsrmntStatusMaskType measStatus_3;

   LtResFltMaskType fltMask_4;
   LtImpedanceType rtg;    /* Resistance from the Tip lead to ground */
   LtImpedanceType rrg;    /* Resistance from the Ring lead to ground */
   LtImpedanceType rtr;    /* Resistance between Tip and Ring leads */
   /* The following resistance is measured only if a fault (or low resistance)
    * exists between the Tip and Ring leads */
   LtImpedanceType rGnd;   /* Resistance to ground */
   LtMsrmntStatusMaskType measStatus_4;
} ENDPOINTDRV_LT_STATISTICS_PARM, *PENDPOINTDRV_LT_STATISTICS_PARM;
#endif

extern void sip_endptcmd( char * cmdLine ); /*broadcom endpoint driver console commands*/

/*************************************************************************
*
*  FUNCTION NAME  : registerInterestInEvent
*
*  DESCRIPTION    : This function register/unregister the interested event to Broadcom CMS
*
*  RETURNS		  :
*************************************************************************/
static void registerInterestInEvent(CmsMsgType msgType, UBOOL8 positive, void *msgData, UINT32 msgDataLen)
{
	CmsMsgHeader *msg = NULL;
	char *data = NULL;
	void *msgBuf = NULL;
	CmsRet ret;

	if (msgData != NULL && msgDataLen != 0) {
		/* for msg with user data */
		msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader) + msgDataLen, ALLOC_ZEROIZE);
	} else {
		msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
	}

	msg = (CmsMsgHeader *)msgBuf;

	/* fill in the msg header */
	msg->type = (positive) ? CMS_MSG_REGISTER_EVENT_INTEREST : CMS_MSG_UNREGISTER_EVENT_INTEREST;
	msg->src = EID_ZYIMS;
	msg->dst = EID_SMD;
	msg->flags_request = 1;
	msg->wordData = msgType;

	if (msgData != NULL && msgDataLen != 0) {
		data = (char *) (msg + 1);
		msg->dataLength = msgDataLen;
		zyMemcpy(data, (char *)msgData, msgDataLen);
	}

	ret = cmsMsg_sendAndGetReply(msgHandle, msg);
	if (ret != CMSRET_SUCCESS) {
		if(positive) {
			cmsLog_error("REGISTER_EVENT_INTEREST failed, msgType = %d, ret = %d", msgType, ret);
		} else {
			cmsLog_error("UNREGISTER_EVENT_INTEREST failed, msgType = %d, ret = %d", msgType, ret);
		}
	} else {
		if(positive) {
			cmsLog_error("REGISTER_EVENT_INTEREST succeeded, msgType = %d", msgType);
		} else {
			cmsLog_error("UNREGISTER_EVENT_INTEREST succeeded, msgType = %d", msgType);
		}
	}

#if 1 /* Fix Leakage issue, __CHT__, MitraStar Kid, 20111125. */
	cmsMem_free(msgBuf);
#endif
	return;
}

/*************************************************************************
*
*  FUNCTION NAME  : cmsInit
*
*  DESCRIPTION    : This function register ZyIMS user application to Broadcom CMS
*
*  RETURNS		  :
*************************************************************************/
static void cmsInit(void)
{
	/* UNINITIALIZED_SHM_ID */
	SINT32 shmId= 0; 
	CmsRet ret;

	/* Set CMS logLevels */
	cmsLog_init(EID_ZYIMS);
	cmsLog_setLevel(LOG_LEVEL_ERR);

	cmsLog_debug("We entered cmsInit function\n");

	/* Initialize Messaging for ZYIMS */ 
	if ((ret = cmsMsg_init(EID_ZYIMS, &msgHandle)) != CMSRET_SUCCESS) {
		cmsLog_error("cmsMsg_init failed, ret=%d", ret);
		exit(-1);
	}

	/* Initialize Mdm layer for ZYIMS */
	if ((ret = cmsMdm_init(EID_ZYIMS, msgHandle, &shmId)) != CMSRET_SUCCESS) {
		cmsLog_error("cmsMdm_init failed, ret=%d", ret);
		cmsMsg_cleanup(&msgHandle);
		exit(-1);
	}

	/* Register interest in flash update event */
	registerInterestInEvent(CMS_MSG_CONFIG_WRITTEN, TRUE, NULL, 0);
	
	/* MSTC porting by Andylin, bind interface , Iproute2 */
	registerInterestInEvent(CMS_MSG_WAN_CONNECTION_UP, TRUE, NULL, 0);
	registerInterestInEvent(CMS_MSG_WAN_CONNECTION_DOWN, TRUE, NULL, 0);
	
  	initiproute2();
	voipqosFirewall_list_H = NULL;
	
}

/****************************************************************************
* FUNCTION: voicePerformRouteOperation   
*
* PURPOSE:  API function used  to add or delete a static route. 
*                  This API function is used to bind SIP and RTP packets on one specific interface.   
*
* PARAMETERS:
*             enable   - Flag to enable/disable static route
*             ifName   - Interface name
*             ipAddr   - Destination IP address
*             mask     - Subnet mask
*
* RETURNS:    Nothing  
****************************************************************************/
//Ssu-Ying, support Multiple Profile
static void voicePerformRouteOperation(UINT32 enable, char* ifName, char* ipAddr, char* mask)
{
   DAL_VOICE_ROUTE_CTL_BLK rtCtlBlk;
   struct      in_addr ifAddr;
   struct      in_addr ifMask;

   VOICEAPP_PRINTF("Entering %s\n", __FUNCTION__);

   if(get_iface_ipv4_address(ifName, &ifAddr) != 0) {
      return;
   }
   ZyPrintf("ifAddr %lx\n", ifAddr.s_addr);
   if(get_iface_ipv4_netmask(ifName, &ifMask) != 0) {
      return;
   }
   ZyPrintf("ifMask %lx\n", ifMask.s_addr);
   ZyPrintf("interface subnet %lx, dest ip subnet %lx\n", ifAddr.s_addr & ifMask.s_addr, inet_addr(ipAddr) & ifMask.s_addr);
   if( (ifAddr.s_addr & ifMask.s_addr) == (inet_addr(ipAddr) & ifMask.s_addr) ) {
      return;
   }
   
   /* Populate route control block */
   rtCtlBlk.enable = enable;

   strncpy(rtCtlBlk.destIPAddress, ipAddr, STRMAXSIZE);
   strncpy(rtCtlBlk.destSubnetMask, mask, STRMAXSIZE);
   strncpy(rtCtlBlk.gatewayIPAddress, "0.0.0.0", STRMAXSIZE);
   strncpy(rtCtlBlk.ifName, ifName, STRMAXSIZE);

   /* Aquire Lock */
   cmsLck_acquireLock();

   /* 
   ** Invoke CMS DAL function to set/reset static route. The value of
   ** the enable field set in the control block above decides whether the
   ** the static route is added or deleted.
   */

   if( !(cmsUtl_strcmp( ifName, MDMVS_LAN )) || !(cmsUtl_strcmp( ifName, MDMVS_ANY_WAN )) )
   {
      ZyPrintf( "No need for route operations\n" );
   }
   else {
   	if ( dalVoice_performRouteOperation(NULL, &rtCtlBlk) != CMSRET_SUCCESS )
       {
          ZyPrintf("%s: Failed to perform static route operation!!\n", __FUNCTION__);   
       }
   }   

   /* Release Lock */
   cmsLck_releaseLock();
}

/*************************************************************************
*
*  FUNCTION NAME  : addVoipStaticRoute
*
*  DESCRIPTION    : This function add static route to bind VoIP on one specific interface
*
*  RETURNS		  :
*************************************************************************/
//Ssu-Ying, support Multiple Profile
void addVoipStaticRoute(char* dest , uint16 SipAccountId)
{
	char addr[STRMAXSIZE];
	char mask[STRMAXSIZE];
	
	struct hostent* phostent;
	struct in_addr ipAddr;

	int VpItr = mapSipAccountIdToVpItr(SipAccountId);
	VoiceProfObj_t * pProf = tr104VoiceProfObjPtrGet(VpItr);
	
	phostent = gethostbyname( dest );
	if ( phostent != NULL ) {
		ipAddr = *((struct in_addr*)phostent->h_addr);
	} else {
		return;
	}

	sprintf( addr, inet_ntoa(ipAddr) );
	sprintf( mask, "255.255.255.255" );
	
	VOICEAPP_PRINTF("addVoipStaticRoute: destAddr %s, SipAccountId %d\n", addr, SipAccountId);
	voicePerformRouteOperation(1, pProf->ZyXEL_ActiveBoundIfName, addr, mask);
}


/*************************************************************************
*
*  FUNCTION NAME  : delVoipStaticRoute
*
*  DESCRIPTION    : This function delete static route which bind VoIP on one specific interface
*
*  RETURNS		  :
*************************************************************************/
//Ssu-Ying, support Multiple Profile
void delVoipStaticRoute(char* dest , uint16 SipAccountId)
{
	char addr[STRMAXSIZE];
	char mask[STRMAXSIZE];

	struct hostent* phostent;
	struct in_addr ipAddr;
	int VpItr = mapSipAccountIdToVpItr(SipAccountId);
	VoiceProfObj_t * pProf = tr104VoiceProfObjPtrGet(VpItr);
	
	phostent = gethostbyname( dest );
	if ( phostent != NULL ) {
		ipAddr = *((struct in_addr*)phostent->h_addr);
	} else {
		return;
	}

	sprintf( addr, inet_ntoa(ipAddr) );
	sprintf( mask, "255.255.255.255" );

	VOICEAPP_PRINTF("delVoipStaticRoute: destAddr %s, SipAccountId %d\n", addr, SipAccountId);
	voicePerformRouteOperation(0, pProf->ZyXEL_ActiveBoundIfName, addr, mask);
}

/****************************************************************************
* FUNCTION: voicePerformFilterOperation   
*
* PURPOSE:  API function used  to enable or disable a firewall filter. 
*                  This API function is used to block/unblock SIP and RTP ports.   
*
* PARAMETERS: id       - Unique ID for the filter 
*             enable   - Flag to enable/disable filter
*             ifName   - Interface name
*             ipaddr   - Destination IP address for incoming packets
*             port     - Destination port for incoming packets
*             mask     - Subnet mask
*             protocol - Transport protocol (TCP/UDP/TCP or UDP)  
*
* RETURNS:    Nothing  
****************************************************************************/
static void voicePerformFilterOperation(UINT32 id, UINT32 enable, char* ifName, char* ipAddr, UINT32 port, char* mask, UINT32 protocol)
{
   DAL_VOICE_FIREWALL_CTL_BLK fwCtlBlk;
   const char* transportStrings[] = { "UDP", "TCP", "TCP or UDP" };
   #if 0
   const char signature[] = { 225, 226, 227, 228, '\0' }; 
   char fid[10]; 
   #endif

   VOICEAPP_PRINTF("Entering %s\n", __FUNCTION__);
   /* Populate firewall control block */
   fwCtlBlk.enable = enable;

   #if 0
   sprintf(fid, "%x", id);
   sprintf(fwCtlBlk.filterName, "%sVoiceFilter%s", signature, (id == DELETE_ALL) ? "" : fid);
   #endif
   strncpy(fwCtlBlk.ifName, ifName, STRMAXSIZE);
   strncpy(fwCtlBlk.protocol, transportStrings[protocol], STRMAXSIZE);
   fwCtlBlk.sourcePort = 0; 
   fwCtlBlk.destinationPort = port;
   strncpy(fwCtlBlk.sourceIPAddress, "", STRMAXSIZE);
   strncpy(fwCtlBlk.sourceNetMask, "", STRMAXSIZE);
   strncpy(fwCtlBlk.destinationIPAddress, ipAddr, STRMAXSIZE);
   strncpy(fwCtlBlk.destinationNetMask, mask, STRMAXSIZE);


   /* 
   ** Invoke CMS DAL function to set/reset firewall filter. The value of
   ** the enable field set in the control block above decides whether the
   ** the filter is added or deleted.
   */

	if( !(cmsUtl_strcmp( ifName, MDMVS_LAN )) )
	{
	   ZyPrintf( "Interface is LAN. No need for filter operations\n" );
	}
	else {
		if ( dalVoice_performFilterOperation(NULL, &fwCtlBlk) != CMSRET_SUCCESS )
		{
		  ZyPrintf("%s: Failed to perform firewall filter operation!!\n", __FUNCTION__);   
		}
	}   
}

/* MSTC porting by Andylin, bind interface , Iproute2 */
static void forkAndExecvp(char **argv)
{
	pid_t spid, gpid, wpid;
	int status=0;

	VOICEAPP_PRINTF("%s > ENTER ,	",__FUNCTION__);

	spid=fork();
	
	if (spid < 0 ) {
		ZyPrintf("voiceApp:-   fork FAIL!\n");
		return;
	} else if (spid == 0) { /* son process */
		gpid = fork();

		if (gpid < 0 ) {
			ZyPrintf("voiceApp:-   fork FAIL!\n");
			exit(0);
		} else if (gpid == 0) { /* grandson process */
			
			if (execvp(argv[0], argv) < 0) {
				ZyPrintf("voiceApp:-   %s,%s(%d) execvp fail, error:%s(%d)\n",__FILE__,__FUNCTION__,__LINE__,strerror(errno),errno);
				exit(0);
			}				
		}

		exit(0);			
	} 
	else { /* main process */
		wpid = waitpid(spid, &status, 0); 
		if( wpid < 0) {			
			ZyPrintf("voiceApp:-   waitpid errno is %s \n", strerror(errno));			
		}
	}
	VOICEAPP_PRINTF("%s > EXIT\n",__FUNCTION__);		

	return;
}

#if 0 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/

/*************************************************************************
*
*  FUNCTION NAME  : addVoipFirewallAcl
*
*  DESCRIPTION    : This function add VoIP related port to apply firewall ACL rule
*
*  RETURNS		  :
*************************************************************************/
//Ssu-Ying, support Multiple Profile
void addVoipFirewallAcl(uint32 port, uint32 protocol , uint16 SipAccountId)
{
	int VpItr = 0;
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	VoiceProfObj_t* pProf = tr104VoiceProfObjPtrGet(VpItr);
	
	char addr[STRMAXSIZE];
	char mask[STRMAXSIZE];

	struct in_addr ipAddr;
	if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &ipAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}

	sprintf( addr, inet_ntoa(ipAddr) );
	sprintf( mask, "255.255.255.255" );
	
	VOICEAPP_PRINTF("addVoipFirewallAcl: addr %s, port %d, protocol %d\n", addr, port, protocol);
	voicePerformFilterOperation(port, 1, pProf->ZyXEL_ActiveBoundIfName, addr, port, mask, (protocol == TR104I_TRANSPORT_UDP)?0:1);
}


/*************************************************************************
*
*  FUNCTION NAME  : delVoipFirewallAcl
*
*  DESCRIPTION    : This function delete VoIP related port from firewall ACL rule
*
*  RETURNS		  :
*************************************************************************/
//Ssu-Ying, support Multiple Profile
void delVoipFirewallAcl(uint32 port, uint32 protocol , uint16 SipAccountId)
{
	int VpItr = 0;
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	VoiceProfObj_t * pProf = tr104VoiceProfObjPtrGet(VpItr);
	
	char addr[STRMAXSIZE];
	char mask[STRMAXSIZE];

	struct in_addr ipAddr;
	if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &ipAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}

	sprintf( addr, inet_ntoa(ipAddr) );
	sprintf( mask, "255.255.255.255" );

	VOICEAPP_PRINTF("delVoipFirewallAcl: addr %s, port %d, protocol %d\n", addr, port, protocol);
	voicePerformFilterOperation(port, 0, pProf->ZyXEL_ActiveBoundIfName, addr, port, mask, (protocol == TR104I_TRANSPORT_UDP)?0:1);
}
#endif
#if 0
/*************************************************************************
*
*  FUNCTION NAME  : delVoipFirewallAclAll
*
*  DESCRIPTION    : This function delete all VoIP related ports from firewall ACL rules
*
*  RETURNS		  :
*************************************************************************/
void delVoipFirewallAclAll(void)
{
	/* Delete all voice filters */
	VOICEAPP_PRINTF("delVoipFirewallAclAll\n");
	voicePerformFilterOperation(DELETE_ALL, 0, "", 0, "", 0);
}
#endif

/****************************************************************************
* FUNCTION: voicePerformQosOperation   
*
* PURPOSE:  API function used  to enable or disable a Qos classification. 
*                  This API function is used to put SIP and RTP packets into high priority queue.   
*
* PARAMETERS: id       - Unique ID for the classification 
*             enable   - Flag to enable/disable classification
*             port     - Source port for outgoing packets
*             protocol - Transport protocol (TCP/UDP)  
*             clsKey     - Class key for iptables
*
* RETURNS:    Nothing  
****************************************************************************/
#ifdef MSTC_VOICE_AUTO_QOS /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
static void voicePerformQosOperation(UINT32 id, UINT32 enable, UINT32 port, UINT32 protocol, UINT32 clsKey, char* ifName)
#elif defined(MSTC_VOICE_VLAN_ID_MARK)
static void voicePerformQosOperation(UINT32 id, UINT32 enable, UINT32 port, UINT32 protocol, UINT32 clsKey, int VLANIDMark, int ethernetPriorityMark)
#else
static void voicePerformQosOperation(UINT32 id, UINT32 enable, UINT32 port, UINT32 protocol, UINT32 clsKey)
#endif
{
   DAL_VOICE_QOS_CTL_BLK qosCtlBlk;
   const SINT32 transportValues[] = { 17, 6 };
   #ifndef MSTC_VOICE_AUTO_QOS /* MSTC , Andy add compiler flag */
   const char signature[] = { 225, 226, 227, 228, '\0' }; 
   char fid[10]; 
   #endif
   VOICEAPP_PRINTF("entering %s\n",__FUNCTION__);

   /* Populate firewall control block */
   qosCtlBlk.enable = enable;

   #ifndef MSTC_VOICE_AUTO_QOS /* MSTC , Andy add compiler flag */
   sprintf(fid, "%x", id);
   sprintf(qosCtlBlk.clsName, "%sVoiceClass%s", signature, (id == DELETE_ALL) ? "" : fid);
   #else /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
   sprintf(qosCtlBlk.clsName, "VoiceClass%d", id);
   #endif
   qosCtlBlk.clsKey = 127 - clsKey;
   qosCtlBlk.protocol = transportValues[protocol];
   qosCtlBlk.destPort= port; 
   qosCtlBlk.dscpMark = -1;
   qosCtlBlk.dscpCheck = -1;
#ifdef MSTC_VOICE_AUTO_QOS /* MSTC , Andy add compiler flag */
   sprintf(qosCtlBlk.ifName,"%s",ifName);
#endif
#ifdef MSTC_VOICE_VLAN_ID_MARK
	qosCtlBlk.VLANIDMark= VLANIDMark;
    qosCtlBlk.ethernetPriorityMark= ethernetPriorityMark;	
#endif

   /* Aquire Lock */
   cmsLck_acquireLock();

   /* 
   ** Invoke CMS DAL function to set/reset QoS classification. The value of
   ** the enable field set in the control block above decides whether the
   ** the classification is added or deleted.
   */
   if ( dalVoice_performQosOperation(NULL, &qosCtlBlk) != CMSRET_SUCCESS )
   {
       ZyPrintf("%s: Failed to perform QoS classification operation!!\n", __FUNCTION__);   
   }

   /* Release Lock */
   cmsLck_releaseLock();
}
#if 0 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
/*************************************************************************
*
*  FUNCTION NAME  : addVoipQosClass
*
*  DESCRIPTION    : This function add VoIP related port to apply QoS classification
*
*  RETURNS		  :
*************************************************************************/
void addVoipQosClass(uint32 port, uint32 protocol)
{
	VOICEAPP_PRINTF("addVoipQosClass: port %d, protocol %d\n", port, protocol);
	int i = 0;
	int found = FALSE;
	while( (!found) && (i < 10) ) {
		if(voipQosClassPort[i] == 0) {
			found = TRUE;
			voipQosClassPort[i] = port;
			voicePerformQosOperation(port, 1, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, i);
		}
		i++;
	}
	if(!found) {
		ZyPrintf("addVoipQosClass fail! ClassKey is out of resource.\n");
	}
}

/*************************************************************************
*
*  FUNCTION NAME  : delVoipQosClass
*
*  DESCRIPTION    : This function delete VoIP related port from QoS classification
*
*  RETURNS		  :
*************************************************************************/
void delVoipQosClass(uint32 port, uint32 protocol)
{
	VOICEAPP_PRINTF("delVoipQosClass: port %d, protocol %d\n", port, protocol);
	int i = 0;
	int found = FALSE;
	while( (!found) && (i < 10) ) {
		if(voipQosClassPort[i] == port) {
			found = TRUE;
			voipQosClassPort[i] = 0;
			voicePerformQosOperation(port, 0, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, i);			
		}
		i++;
	}
	if(!found) {
		ZyPrintf("delVoipQosClass fail! Cannot find classkey.\n");
        }
}
#endif

#if 1 /*Eason, check qos and firewall rules before set, prevent iptables rule leakage*/
/*************************************************************************
*
*  FUNCTION NAME  : addVoIPQoSFirewall
*
*  DESCRIPTION    : This function add VoIP related port to apply QoS classification and firewall rules
*
*  RETURNS		  :
*************************************************************************/
#ifdef MSTC_VOICE_AUTO_QOS /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
void addVoIPQoSFirewall(uint32 port, uint32 protocol, char *ifName)
#elif defined(MSTC_VOICE_VLAN_ID_MARK)
void addVoIPQoSFirewall(uint32 port, uint32 protocol , uint16 VpItr, int vlanIdMark, int ethernetPriorityMark )
#else
void addVoIPQoSFirewall(uint32 port, uint32 protocol , uint16 VpItr)
#endif
{
	qosFirewallList_t *newentry = NULL;
	qosFirewallList_t *tmpentry = NULL;	
	
	char addr[STRMAXSIZE];
	char mask[STRMAXSIZE];
	struct in_addr ipAddr;

	#ifndef MSTC_VOICE_AUTO_QOS /* MSTC , Andy add compiler flag */
	int i = 0;
	int found = FALSE;

	VoiceProfObj_t* pProf = tr104VoiceProfObjPtrGet(VpItr);	

        if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &ipAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}
        #else
	if(get_iface_ipv4_address(ifName, &ipAddr) != 0) {
		ZyPrintf("voiceApp:-	 GET IP ADDRESS FAIL!\n");
		return;
	}
        #endif	
	sprintf( addr, inet_ntoa(ipAddr) );
	sprintf( mask, "255.255.255.255" );

	#ifdef MSTC_VOICE_AUTO_QOS /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
	
	if(voipqosFirewall_list_H!=NULL){
		tmpentry = voipqosFirewall_list_H;
		while(tmpentry!=NULL){
			cmsLog_debug("ifname : %s , port : %d , protocol : %d ",tmpentry->ifname, tmpentry->port, tmpentry->protocol);
			if((strcmp(tmpentry->ifname, ifName) == 0)&&
				(tmpentry->port == port)&&(tmpentry->protocol== protocol))	{
				cmsLog_error("voipqosFirewall List Exist, ifname : %s , port : %d , protocol : %d ",tmpentry->ifname, tmpentry->port, tmpentry->protocol);
				return;
			}
			tmpentry = tmpentry->next;
		}
	}

	newentry =(Rtplist_t *) malloc(sizeof(Rtplist_t));	
	if(newentry == NULL){
		cmsLog_error("newlist is NULL");
		return;
	}

	if(strlen(ifName) >= MAX_IFNAME_LEN){
		cmsLog_error("ifName length is too long");	
		free(newentry);
		return;
	}
	strcpy(newentry->ifname, ifName);
	
	newentry->port = port;
	newentry->protocol = protocol;	
	
	cmsLog_debug("add voip qos firewall list , ifname = %s, port = %d, protocol =%d",
		newentry->ifname, newentry->port, newentry->protocol);
	
	if(voipqosFirewall_list_H == NULL){
		newentry->next = NULL;			
	}
	else{
		tmpentry = voipqosFirewall_list_H;
		newentry->next = tmpentry;
	}
	voipqosFirewall_list_H = newentry;	

	voicePerformQosOperation(port, 1, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, 0, ifName);	
	voicePerformFilterOperation(port, 1, ifName, addr, port, mask, (protocol == TR104I_TRANSPORT_UDP)?0:1);
	
	#else	
	VOICEAPP_PRINTF("%s: addr %s, port %d, protocol %d, VpItr %d\n",__FUNCTION__, addr, port, protocol, VpItr);
	for(i=0;i<10;i++){
		if((voipQosFirewallPort[i].port == port) && voipQosFirewallPort[i].protocol == protocol ) {			
			VOICEAPP_PRINTF("addVoipQosClass found [%d] port %d, protocol %d, do nothing\n", i, port, protocol);
			return;
		}
	}

	/*find rule*/
	i=0;	
	while( (!found) && (i < 10) ) {
		if(voipQosFirewallPort[i].port == 0) {
			found = TRUE;
			voipQosFirewallPort[i].port = port;
			voipQosFirewallPort[i].protocol = protocol;
			VOICEAPP_PRINTF("%s, added to [%d] port %d, protocol %d\n",__FUNCTION__, i, port, protocol);
			#ifdef MSTC_VOICE_VLAN_ID_MARK
			voicePerformQosOperation(port, 1, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, i, vlanIdMark, ethernetPriorityMark);
			#else
			voicePerformQosOperation(port, 1, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, i);
			#endif
			VOICEAPP_PRINTF("%s: addr %s, port %d, protocol %d\n",__FUNCTION__, addr, port, protocol);
			voicePerformFilterOperation(port, 1, pProf->ZyXEL_ActiveBoundIfName, addr, port, mask, (protocol == TR104I_TRANSPORT_UDP)?0:1);
		}
		i++;
	}
	if(!found) {
		ZyPrintf("%s failed!\n", __FUNCTION__);
	}
	for(i =0; i< 10; i++) {
		VOICEAPP_PRINTF("\t[%d] port = %d, protocol = %d\n", i, voipQosFirewallPort[i].port, voipQosFirewallPort[i].protocol);
	}
	#endif
}
/*************************************************************************
*
*  FUNCTION NAME  : delVoIPQoSFirewall
*
*  DESCRIPTION    : This function delete VoIP related port from QoS classification and firewall rules
*
*  RETURNS		  :
*************************************************************************/
#ifdef MSTC_VOICE_AUTO_QOS /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
void delVoIPQoSFirewall(uint32 port, uint32 protocol, char *ifName)
#elif defined(MSTC_VOICE_VLAN_ID_MARK)
void delVoIPQoSFirewall(uint32 port, uint32 protocol, uint16 VpItr, int vlanIdMark, int ethernetPriorityMark)
#else
void delVoIPQoSFirewall(uint32 port, uint32 protocol, uint16 VpItr)
#endif
{
	qosFirewallList_t *preventry = NULL;
	qosFirewallList_t *tmpentry = NULL;	
	UBOOL8 found = FALSE;

	char addr[STRMAXSIZE];
	char mask[STRMAXSIZE];
        struct in_addr ipAddr;

	#ifndef MSTC_VOICE_AUTO_QOS /* MSTC , Andy add compiler flag */
	int i = 0;
	VoiceProfObj_t * pProf = tr104VoiceProfObjPtrGet(VpItr);
        if(get_iface_ipv4_address(pProf->ZyXEL_ActiveBoundIfName, &ipAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}
	#else	
	if(get_iface_ipv4_address(ifName, &ipAddr) != 0) {
		ZyPrintf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}
        #endif

	sprintf( addr, inet_ntoa(ipAddr) );
	sprintf( mask, "255.255.255.255" );

	#ifdef MSTC_VOICE_AUTO_QOS /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */

	if(voipqosFirewall_list_H!=NULL){
		tmpentry = voipqosFirewall_list_H;			
		while(tmpentry!=NULL){							
			if((strcmp(tmpentry->ifname, ifName) == 0)&&
				(tmpentry->port == port)&&(tmpentry->protocol==protocol)){				
				cmsLog_debug("Remove voip Rtp rule , ifname = %s, port = %d, protocol = %d",
					tmpentry->ifname, tmpentry->port, tmpentry->protocol);
				voicePerformQosOperation(port, 0, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, 0, ifName);	
				voicePerformFilterOperation(port, 0, ifName, addr, port, mask, (protocol == TR104I_TRANSPORT_UDP)?0:1);
				
				if(tmpentry == voipqosFirewall_list_H){		
					voipqosFirewall_list_H = tmpentry->next;
				}
				else if(preventry != NULL){
					preventry->next = tmpentry->next;
				}	
				else{
					cmsLog_error("preventry is NULL");
				}
				found = TRUE;
				break;
			}	
			else{
				preventry = tmpentry;	
				tmpentry = tmpentry->next;		
			}										
		}		
	}	
	if(found){
		free(tmpentry);
	}
	#else
	VOICEAPP_PRINTF("%s: addr %s, port %d, protocol %d, VpItr %d\n",__FUNCTION__, addr, port, protocol, VpItr);
	while( (!found) && (i < 10) ) {
		if((voipQosFirewallPort[i].port == port) && (voipQosFirewallPort[i].protocol == protocol)) {			
			ZyPrintf("%s found[%d].\n", __FUNCTION__, i);			
			#ifdef MSTC_VOICE_VLAN_ID_MARK
			voicePerformQosOperation(port, 0, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, i, vlanIdMark, ethernetPriorityMark);	
			#else
			voicePerformQosOperation(port, 0, port, (protocol == TR104I_TRANSPORT_UDP)?0:1, i);	
			#endif
			voicePerformFilterOperation(port, 0, pProf->ZyXEL_ActiveBoundIfName, addr, port, mask, (protocol == TR104I_TRANSPORT_UDP)?0:1);
			voipQosFirewallPort[i].port = 0;
			voipQosFirewallPort[i].protocol = 0;
			found = TRUE;
		}
		i++;
	}
	if(!found) {
		ZyPrintf("%s, fail! Cannot find classkey.\n", __FUNCTION__);
		
	}
	
	for(i =0; i< 10; i++) {
		VOICEAPP_PRINTF("\t[%d] port = %d, protocol = %d\n", i, voipQosFirewallPort[i].port, voipQosFirewallPort[i].protocol);
	}
	#endif
}
#endif
#if 0
/*************************************************************************
*
*  FUNCTION NAME  : delVoipQosClassAll
*
*  DESCRIPTION    : This function delete all VoIP related ports from QoS classifications
*
*  RETURNS		  :
*************************************************************************/
void delVoipQosClassAll(void)
{
	VOICEAPP_PRINTF("entering %s\n",__FUNCTION__);
	/* Delete all voice related QoS classifications */
	voicePerformQosOperation(DELETE_ALL, 0, 0, 0);
}
#endif

/****************************************************************************
* FUNCTION: voicePerformLedOperation   
*
* PURPOSE:  API function used  to control the LED.  
*
* PARAMETERS: phid       - Phone ID
*             type   - LED type
*             behavior     - LED behavior
*
* RETURNS:    Nothing  
****************************************************************************/
static void voicePerformLedOperation(DAL_VOICE_LED_TYPE type, DAL_VOICE_LED_BEHAVIOR behavior)
{
	VOICEAPP_PRINTF("%s: ENTER\n", __FUNCTION__);
	DAL_VOICE_LED_CTL_BLK ledCtlBlk;

   ledCtlBlk.led = type;
   ledCtlBlk.behavior = behavior;



   /* Invoke CMS DAL function to on/off VoIP LED. */
   if ( dalVoice_performLedOperation(NULL, &ledCtlBlk) != CMSRET_SUCCESS )
   {
      ZyPrintf("%s: Failed to perform LED operation!!\n", __FUNCTION__);   
   }
    

	VOICEAPP_PRINTF("%s: EXIT\n", __FUNCTION__);	
}

/*************************************************************************
*
*  FUNCTION NAME  : voipLedOnOff
*
*  DESCRIPTION    : This function makes the LED on/off/blink.
*
*  RETURNS		  :
*************************************************************************/
void voipLedOnOff(uint16 phid)
{
//Ssu-Ying, support Multiple Profile
	int i = 0;
	int j = 0;
	uint16 NumOfPhyIntf = 0;
	VoicePhyIntfObj_t*	phyInterface = NULL;
	VoiceProfLineObj_t *pLine = NULL;
	VoiceProfLineCallingFeaturesObj_t *pCallfeatures = NULL;
	uint8 regStatus = 0;
	uint8 messageWaiting = 0;
	phoneCh_t *phoneCB = NULL;
	int hookStatus = PHONE_HOOK_ST_ON;
	VOICEAPP_PRINTF("%s, ENTER",__FUNCTION__);

	VOICEAPP_DBG1("voipLedOnOff phid=",phid);
	NumOfPhyIntf = tr104GetNumOfPhyIntf();
	int SipAccountId = 0, LineItr = 0, VpItr=0;
	uint32 NumOfProfile = 0;
	uint32 NumOfLine = 0;
	/* If DAL_VOICE_LED_TYPE abd DAL_VOICE_LED_BEHAVIOR is not definded, */
	/* please merge dal_voice.h and dal_voice.c from Telefonica_Common trunk */
	DAL_VOICE_LED_TYPE type1 = DAL_VOICE_LED_UNKNOWN_TYPE;
	DAL_VOICE_LED_TYPE type2 = DAL_VOICE_LED_UNKNOWN_TYPE;
	uint8 lineEnable = 0;

	if(phid == NumOfPhyIntf) {	/* check all FXS phone */
		for(i = 0; i < NumOfPhyIntf; i++){
			phyInterface = tr104VoicePhyIntfObjPtrGet(i);
			regStatus = 0;
			messageWaiting = 0;
			
			if(NULL != phyInterface){
				for ( SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++ ) {
					if((phyInterface->ZyXEL_LineSelect& (1 << SipAccountId)) > 0) {	/* FXS outgoing line */
						LineItr = mapSipAccountIdToLineItr(SipAccountId);
						VpItr = mapSipAccountIdToVpItr(SipAccountId);
						pLine = tr104VoiceLineObjPtrGet(VpItr,LineItr);
						if(NULL != pLine){
							pCallfeatures = pLine->callingFeatures;
							if(pLine->status == TR104I_STATUS_UP) {
									VOICEAPP_PRINTF("pLine->status = %d\n" , pLine->status);
								regStatus = 1;
								if(pCallfeatures->messageWaiting) {
										VOICEAPP_PRINTF("pCallfeatures->messageWaiting = %d \n" , pCallfeatures->messageWaiting);
									messageWaiting = 1;
								}
							}
						}
					}
				}
			}

			lineEnable = 0;
			phoneCB = find_phoneCB(i, 1);
			if(phoneCB != NULL) {
				hookStatus = phHookStatus(phoneCB);
				pLine = getAssociatedRegLineObj(phoneCB);
				if(pLine) {
					lineEnable = pLine->enable;
				}
			}

			type1 = type2 = DAL_VOICE_LED_UNKNOWN_TYPE;
			if(i == 0) {
				type1 = DAL_VOICE_LED_VOIP_1;
				type2 = DAL_VOICE_LED_MWI_1;
			} else if(i == 1) {
				type1 = DAL_VOICE_LED_VOIP_2;
				type2 = DAL_VOICE_LED_MWI_2;
			}
			if(lineEnable == 0) {
				voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);
				voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);
			} else if(messageWaiting) {
				if(PHONE_HOOK_ST_OFF == hookStatus) {
					voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);	/* PHONE_REGISTRATION_LED OFF */
					voicePerformLedOperation(type2, DAL_VOICE_LED_SLOW_BLINK);	/* PHONE_MWI_LED BLINK */
				} else {
					voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);		/* PHONE_REGISTRATION_LED OFF */
					voicePerformLedOperation(type2, DAL_VOICE_LED_ON);		/* PHONE_MWI_LED ON */
				}
			} else if(regStatus) {
				if(PHONE_HOOK_ST_OFF == hookStatus) {
					voicePerformLedOperation(type1, DAL_VOICE_LED_SLOW_BLINK);	/* PHONE_REGISTRATION_LED BLINK */
					voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);	/* PHONE_MWI_LED OFF */
				} else {
					voicePerformLedOperation(type1, DAL_VOICE_LED_ON);		/* PHONE_REGISTRATION_LED ON */
					voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
				}
			} else {
				#if 1
				voicePerformLedOperation(type1, DAL_VOICE_LED_FAIL);	/* PHONE_REGISTRATION_LED FAIL */
				voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
				#else
				voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);		/* PHONE_REGISTRATION_LED OFF */
				voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
				#endif
			}
		}
	} else {	/* check FXS phone with phid */
		VOICEAPP_DBG("phid != NumOfPhyIntf");
		phyInterface = tr104VoicePhyIntfObjPtrGet(phid);
	
		if(NULL != phyInterface){
			for ( SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++ ) {
				if((phyInterface->ZyXEL_LineSelect& (1 << SipAccountId)) > 0) {	/* FXS outgoing line */
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
					if(NULL != pLine){
						pCallfeatures = pLine->callingFeatures;
						if(pLine->status == TR104I_STATUS_UP) {
							regStatus = 1;
							if(pCallfeatures->messageWaiting) {
								messageWaiting = 1;
							}
						}
					}
				}
			}
		}

		lineEnable = 0;
		phoneCB = find_phoneCB(phid, 1);
		if(phoneCB != NULL) {
			hookStatus = phHookStatus(phoneCB);
			pLine = getAssociatedRegLineObj(phoneCB);
			if(pLine) {
				lineEnable = pLine->enable;
			}
		}

		type1 = type2 = DAL_VOICE_LED_UNKNOWN_TYPE;
		if(phid == 0) {
			type1 = DAL_VOICE_LED_VOIP_1;
			type2 = DAL_VOICE_LED_MWI_1;
		} else if(phid == 1) {
			type1 = DAL_VOICE_LED_VOIP_2;
			type2 = DAL_VOICE_LED_MWI_2;
		}
		if(lineEnable == 0) {
			voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);
			voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);
		} else if(messageWaiting) {
			if(PHONE_HOOK_ST_OFF == hookStatus) {
				voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);		/* PHONE_REGISTRATION_LED OFF */
				voicePerformLedOperation(type2, DAL_VOICE_LED_SLOW_BLINK);		/* PHONE_MWI_LED BLINK */
			} else {
				voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);		/* PHONE_REGISTRATION_LED OFF */
				voicePerformLedOperation(type2, DAL_VOICE_LED_ON);		/* PHONE_MWI_LED ON */
			}
		} else if(regStatus) {
			if(PHONE_HOOK_ST_OFF == hookStatus) {
				voicePerformLedOperation(type1, DAL_VOICE_LED_SLOW_BLINK);		/* PHONE_REGISTRATION_LED BLINK */
				voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
			} else {
				voicePerformLedOperation(type1, DAL_VOICE_LED_ON);		/* PHONE_REGISTRATION_LED ON */
				voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
			}
		} else {
			#if 1
			voicePerformLedOperation(type1, DAL_VOICE_LED_FAIL);	/* PHONE_REGISTRATION_LED FAIL */
			voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
			#else
			voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);		/* PHONE_REGISTRATION_LED OFF */
			voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
			#endif
		}
	}
	VOICEAPP_PRINTF("%s, Exit \n",__FUNCTION__);

}

/*************************************************************************
*
*  FUNCTION NAME  : voipLedBlinking
*
*  DESCRIPTION    : This function makes the LED blinking.
*
*  RETURNS		  :
*************************************************************************/
void voipLedBlinking(uint16 phid)
{
//Ssu-Ying, support Multiple Profile
	int j = 0;
	VoicePhyIntfObj_t*	phyInterface = NULL;
	VoiceProfLineObj_t *pLine = NULL;
	VoiceProfLineCallingFeaturesObj_t *pCallfeatures = NULL;
	uint8 regStatus = 0;
	uint8 messageWaiting = 0;
	/* If DAL_VOICE_LED_TYPE abd DAL_VOICE_LED_BEHAVIOR is not definded, */
	/* please merge dal_voice.h and dal_voice.c from Telefonica_Common trunk */
	DAL_VOICE_LED_TYPE type1 = DAL_VOICE_LED_UNKNOWN_TYPE;
	DAL_VOICE_LED_TYPE type2 = DAL_VOICE_LED_UNKNOWN_TYPE;
	ccmConn_t *ccmConn = NULL;
	phoneCh_t *phoneCB = NULL;
	uint8 lineEnable = 0;

	VOICEAPP_DBG("voipLedBlinking");

	int LineItr = 0, VpItr=0 , SipAccountId = 0;
	uint32 NumOfProfile = 0;
	uint32 NumOfLine = 0;
	
	phyInterface = tr104VoicePhyIntfObjPtrGet(phid);

	if(NULL != phyInterface){
		for ( SipAccountId=0; SipAccountId<sipAccountNumber; SipAccountId++ ) {
			if((phyInterface->ZyXEL_LineSelect& (1 << SipAccountId)) > 0) {	/* FXS outgoing line */
				VpItr = mapSipAccountIdToVpItr(SipAccountId);
				LineItr = mapSipAccountIdToLineItr(SipAccountId);
				pLine = tr104VoiceLineObjPtrGet(VpItr, LineItr);
				if(NULL != pLine){
					pCallfeatures = pLine->callingFeatures;
					if(pLine->status == TR104I_STATUS_UP) {
						regStatus = 1;
						if(pCallfeatures->messageWaiting) {
							messageWaiting = 1;
						}
					}
				}
			}
		}
	}

	regStatus = 0;
	lineEnable = 0;
	phoneCB = find_phoneCB(phid, 1);
	if(phoneCB) {
		ccmConn = findOrigCcmConn(phoneCB->dspPortType, phoneCB->logicalId);
		if(ccmConn) {
			regStatus = check_associateCCMRegSt(ccmConn);
		}
		pLine = getAssociatedRegLineObj(phoneCB);
		if(pLine) {
			lineEnable = pLine->enable;
		}
	}

	type1 = type2 = DAL_VOICE_LED_UNKNOWN_TYPE;
	if(phid == 0) {
		type1 = DAL_VOICE_LED_VOIP_1;
		type2 = DAL_VOICE_LED_MWI_1;
	} else if(phid == 1) {
		type1 = DAL_VOICE_LED_VOIP_2;
		type2 = DAL_VOICE_LED_MWI_2;
	}
	if(lineEnable == 0) {
		voicePerformLedOperation(type1, DAL_VOICE_LED_SLOW_BLINK);/* PHONE_DISABLE_UNREGISTRATION_LED BLINK */ 
		voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);
	} else if(messageWaiting) {
		voicePerformLedOperation(type1, DAL_VOICE_LED_OFF);		/* PHONE_REGISTRATION_LED OFF */
		voicePerformLedOperation(type2, DAL_VOICE_LED_SLOW_BLINK);		/* PHONE_MWI_LED BLINK */
	} else {
		if(regStatus) {
			voicePerformLedOperation(type1, DAL_VOICE_LED_SLOW_BLINK);		/* PHONE_REGISTRATION_LED BLINK */
			voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);		/* PHONE_MWI_LED OFF */
		} else {
			voicePerformLedOperation(type1, DAL_VOICE_LED_SLOW_BLINK); /* PHONE_ENABLE_UNREGISTRATION_LED BLINK */ 
			voicePerformLedOperation(type2, DAL_VOICE_LED_OFF);
		}
	}
}

void voipSetLed(uint16 phid, eVoIPLedType type, eVoIPLedState state)
{
	/* If DAL_VOICE_LED_TYPE abd DAL_VOICE_LED_BEHAVIOR is not definded, */
	/* please merge dal_voice.h and dal_voice.c from Telefonica_Common trunk */
	DAL_VOICE_LED_TYPE dalLedType = DAL_VOICE_LED_UNKNOWN_TYPE;
	DAL_VOICE_LED_BEHAVIOR behavior = DAL_VOICE_LED_UNKNOWN_BEHAVIOR;
	
	if(phid == 0) {
		switch(type) {
			case LED_REG:
				dalLedType = DAL_VOICE_LED_VOIP_1;
				break;
			case LED_MWI:
				dalLedType = DAL_VOICE_LED_MWI_1;
				break;
			default:
				dalLedType = DAL_VOICE_LED_UNKNOWN_TYPE;
				break;
		}
	} else if(phid == 1) {
		switch(type) {
			case LED_REG:
				dalLedType = DAL_VOICE_LED_VOIP_2;
				break;
			case LED_MWI:
				dalLedType = DAL_VOICE_LED_MWI_2;
				break;
			default:
				dalLedType = DAL_VOICE_LED_UNKNOWN_TYPE;
				break;
		}
	}

	switch(state) {
		case VOIP_LED_STATE_OFF:
			behavior = DAL_VOICE_LED_OFF;
			break;
		case VOIP_LED_STATE_ON:
			behavior = DAL_VOICE_LED_ON;
			break;
		case VOIP_LED_STATE_ALL_ON:
			behavior = DAL_VOICE_LED_ALL_ON;
			break;
		case VOIP_LED_STATE_ALL_OFF:
			behavior = DAL_VOICE_LED_ALL_OFF;
			break;
		case VOIP_LED_STATE_FAIL:
			behavior = DAL_VOICE_LED_FAIL;
			break;
		case VOIP_LED_STATE_FAIL_OFF:
			behavior = DAL_VOICE_LED_FAIL_OFF;
			break;
		case VOIP_LED_STATE_SLOW_BLINK:
			behavior = DAL_VOICE_LED_SLOW_BLINK;
			break;
		case VOIP_LED_STATE_FAST_BLINK:
			behavior = DAL_VOICE_LED_FAST_BLINK;
			break;
		case VOIP_LED_STATE_BLINK_ONECE:
			behavior = DAL_VOICE_LED_BLINK_ONECE;
			break;
		default:
			behavior = DAL_VOICE_LED_UNKNOWN_BEHAVIOR;
			break;
	}

	voicePerformLedOperation(dalLedType, behavior);
	return;
}

/*************************************************************************
*
*  FUNCTION NAME  : tr104UpdateFXSLineSelect
*
*  DESCRIPTION    : This function will reload ZyXEL_VoiceFXS_t lineSelect to FXS ccm
*
*  RETURNS		  :
*************************************************************************/

void tr104UpdateFXSLineSelect(void){
//Ssu-Ying, support Multiple Profile
	ccmConn_t *tmpCcmConn_p = NULL;
	VoicePhyIntfObj_t*	phyInterface = NULL;
	phoneCh_t *phoneCB = NULL;
	TR104_PRINTF("entering %s\n", __FUNCTION__);
	tmpCcmConn_p = ccmConnCb.fxsCcmConn;
	while ( tmpCcmConn_p != NULL ) {
		phoneCB = find_phoneCB(tmpCcmConn_p->origPortId, 0);
		if(NULL != phoneCB){
			phyInterface = tr104VoicePhyIntfObjPtrGet(phoneCB->physicalId);
			if(NULL != phyInterface){
		 		tmpCcmConn_p->SipFxsMap = phyInterface->ZyXEL_LineSelect;
				TR104_PRINTF("%s, update Phone %d, lineSelect to %d\n", __FUNCTION__, phoneCB->physicalId+1, phyInterface->ZyXEL_LineSelect);
			}
		}
		tmpCcmConn_p = tmpCcmConn_p->next;
	}
}
/*************************************************************************
*
*  FUNCTION NAME  : tr104UpdateLinePhyReferenceFXSList
*
*  DESCRIPTION    : This function will reload ZyXEL_phyReferenceFXSList to SIP ccm
*
*  RETURNS		  :
*************************************************************************/

void tr104UpdateLinePhyReferenceFXSList(void){
	ccmConn_t *tmpCcmConn_p = NULL;
	sipCCMConvertEntry_t * tmpConvertEntry_p = NULL;
	VoiceProfLineObj_t *lineObj = NULL;
	TR104_PRINTF("entering %s\n", __FUNCTION__);
	tmpCcmConn_p = ccmConnCb.sipCcmConn;
	while ( tmpCcmConn_p != NULL ) {
		tmpConvertEntry_p = findSipCCMConvertByPortId(tmpCcmConn_p->origPortId);
		if(NULL != tmpConvertEntry_p){
//Ssu-Ying, support Multiple Profile
			int LineItr = 0;
			LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
			lineObj = tr104VoiceLineObjPtrGet(tmpConvertEntry_p->VpItr,LineItr);
			if(NULL != lineObj){
				tmpCcmConn_p->SipFxsMap = lineObj->ZyXEL_phyReferenceFXSList;
				TR104_PRINTF("%s, update SipAccountId %d, phoneSelect to %d\n", __FUNCTION__, tmpConvertEntry_p->SipAccountId, tmpCcmConn_p->SipFxsMap);
			}
		}
		tmpCcmConn_p = tmpCcmConn_p->next;
	}
	
}

/*************************************************************************
*
*  FUNCTION NAME  : tr104UpdateSipCallStatus
*
*  DESCRIPTION    : This function will reload sipCallStatus of sipCCMConvertEntry_t
*
*  RETURNS		  :
*************************************************************************/

void tr104UpdateSipCallStatus(void){
	ccmConn_t *tmpCcmConn_p = NULL;
	sipCCMConvertEntry_t * tmpConvertEntry_p = NULL;
	VoiceProfLineObj_t *lineObj = NULL;
	int i=0;
	TR104_PRINTF("entering %s\n", __FUNCTION__);
	
	if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
		i = 0;
		tmpConvertEntry_p = sipCCMConvert.sipCCMConvertEntry;
		while ( tmpConvertEntry_p != NULL ) {
//Ssu-Ying, support Multiple Profile
			int LineItr = 0;
			LineItr = mapSipAccountIdToLineItr(tmpConvertEntry_p->SipAccountId);
			lineObj = tr104VoiceLineObjPtrGet(tmpConvertEntry_p->VpItr, LineItr);
			if(NULL != lineObj){
				zyStrncpy(tmpConvertEntry_p->sipCallStatus.selfNumber, lineObj->directoryNumber, sizeof(tmpConvertEntry_p->sipCallStatus.selfNumber));
				tmpConvertEntry_p->sipCallStatus.selfNumber[sizeof(tmpConvertEntry_p->sipCallStatus.selfNumber)-1]=0;
				TR104_PRINTF("%s, update SipAccountId %d, selfNumber to %d\n", __FUNCTION__, tmpConvertEntry_p->SipAccountId, tmpConvertEntry_p->sipCallStatus.selfNumber);
			}
			tmpConvertEntry_p = tmpConvertEntry_p->next;
			
		}
	}
	
}


/*************************************************************************
*
*  FUNCTION NAME  : resetIPTimer
*
*  DESCRIPTION    : This function will send CMS_MSG_ZYIMS_SELF_IP_CHANGE to ZyIMS when timeout
*
*  RETURNS		  :
*************************************************************************/
int
resetIPTimer(
	tm_id_t unused, /* timerid */
	void	*vp
)
{
	VOICEAPP_PRINTF("%s Enter\n",__FUNCTION__);
	CmsMsgHeader msg=EMPTY_MSG_HEADER;
	CmsRet ret = CMSRET_SUCCESS;

	msg.type = CMS_MSG_ZYIMS_SELF_IP_CHANGE;
	msg.src = EID_ZYIMS;
	msg.dst = EID_ZYIMS;
	msg.flags_event = 1;

	if ((ret = cmsMsg_send(msgHandle, &msg)) != CMSRET_SUCCESS) {
		cmsLog_error("could not send CMS_MSG_ZYIMS_SELF_IP_CHANGE msg to zyims, ret=%d", ret);
	} else {
		cmsLog_debug("sent CMS_MSG_ZYIMS_SELF_IP_CHANGE msg to zyims, ret=%d", ret);
	}
 	VOICEAPP_PRINTF("%s Exit\n",__FUNCTION__);   
	return 1;
}


/*************************************************************************
*
*  FUNCTION NAME  : checkVoipInUseTimer
*
*  DESCRIPTION    : This function will send CMS_MSG_REBOOT_SYSTEM to ZyIMS when voip not in use
*
*  RETURNS		  :
*************************************************************************/
int
checkVoipInUseTimer(
	tm_id_t timerid, /* timerid */
	void	*vp
)
{
	CmsMsgHeader msg=EMPTY_MSG_HEADER;
	CmsRet ret = CMSRET_SUCCESS;
	
	
	if(!isVoIPinUsed()){
		VOICEAPP_PRINTF("%s, Voip is Not In Use\n", __FUNCTION__);

		msg.type = CMS_MSG_REBOOT_SYSTEM;		
		msg.src = EID_ZYIMS;
		msg.dst = EID_SMD;
		msg.flags_request = 1;

		VOICEAPP_PRINTF("%s, voipDeactivate\n",__FUNCTION__);
		voipDeactivate();

		ZyPrintf("%s, prepare to reboot, de-init DSP\n",__FUNCTION__);		
		PhoneDeinit();

		VOICEAPP_PRINTF("%s, Sending cms msg type 0x%x\n",__FUNCTION__,  msg.type);
		if ((ret = cmsMsg_send(msgHandle, &msg)) != CMSRET_SUCCESS) {
			cmsLog_error("could not send CMS_MSG_REBOOT_SYSTEM msg to smd, ret=%d", ret);
		} else {
			cmsLog_debug("sent CMS_MSG_REBOOT_SYSTEM msg to smd, ret=%d", ret);
		}

		timerPeriodicStop(timerid);
		checkVoipInUseTimerId = -1;
	}
	else{		
		VOICEAPP_PRINTF("%s, VoIP In Used\n",__FUNCTION__);
	}
    
	return 1;
}


/*************************************************************************
*
*  FUNCTION NAME  : resetConfigTimer
*
*  DESCRIPTION    : This function will send CMS_MSG_CONFIG_WRITTEN to ZyIMS when timeout
*
*  RETURNS		  :
*************************************************************************/
int
resetConfigTimer(
	tm_id_t unused, /* timerid */
	void	*vp
)
{
	CmsMsgHeader msg=EMPTY_MSG_HEADER;
	CmsRet ret = CMSRET_SUCCESS;

	msg.type = CMS_MSG_CONFIG_WRITTEN;
	msg.src = EID_ZYIMS;
	msg.dst = EID_ZYIMS;
	msg.flags_event = 1;

	if ((ret = cmsMsg_send(msgHandle, &msg)) != CMSRET_SUCCESS) {
		cmsLog_error("could not send CMS_MSG_VOICE_CONFIG_CHANGED msg to zyims, ret=%d", ret);
	} else {
		cmsLog_debug("sent CMS_MSG_VOICE_CONFIG_CHANGED msg to zyims, ret=%d", ret);
	}
    
	return 1;
}

//Ssu-Ying, support Multiple Profile
/*************************************************************************
*
*  FUNCTION NAME  : processCmsMsg
*
*  DESCRIPTION    : This function handles Broadcom CMS message
*
*  RETURNS		  :
*************************************************************************/
static void processCmsMsg(CmsMsgHeader *msgHdr)
{
	VoiceDiagMsgBody *diagMsg = NULL;
	CmsMsgHeader replyMsg = EMPTY_MSG_HEADER;
	voiceParams_t parms;
	char buf[sizeof(CmsMsgHeader) + sizeof(_VoiceLineStatsObject)]={0};
	CmsMsgHeader *msg = (CmsMsgHeader *) buf;
	_VoiceLineStatsObject *info = (_VoiceLineStatsObject *) &(buf[sizeof(CmsMsgHeader)]);
	CmsRet ret;
	int i=0, j=0;
	uint32 lineInst = 0, profInst = 0;
	uint32 SipAccountId = 0;
	uint16 LineItr = 0, VpItr=0;

	char bufCallHistory[sizeof(CmsMsgHeader) + sizeof(VoiceCallHistoryBody)]={0};
	char bufCallHistorySummary[sizeof(CmsMsgHeader) + sizeof(VoiceCallHistorySummaryBody)]={0};
	CmsMsgHeader *msgCallHistory = (CmsMsgHeader *) bufCallHistory;
	VoiceCallHistoryBody *infoCallHistory = (VoiceCallHistoryBody *) &(bufCallHistory[sizeof(CmsMsgHeader)]);
	VoiceCallHistorySummaryBody *infoCallHistorySummary = (VoiceCallHistorySummaryBody *) &(bufCallHistorySummary[sizeof(CmsMsgHeader)]);
	VoiceProfLineObj_t *pLine = NULL;
	char *tmpPtr = NULL;
	
	switch (msgHdr->type) {   
		/* MSTC porting by Andylin, bind interface , Iproute2 */
		case CMS_MSG_WAN_CONNECTION_UP:	{				
				VOICEAPP_DBG("\n processCmsMsg: CMS_MSG_WAN_CONNECTION_UP \n");
				#ifndef MSTC_SERVICE_ROUTE
				if(msgHdr->dataLength > 1){
					tmpPtr = (char *)(msgHdr + 1); /*pointer to payload*/	
					processIpRoute2(CMS_MSG_WAN_CONNECTION_UP,tmpPtr);	
				}
				#endif
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_WAN_CONNECTION_UP Exit!!\n");					
			}
			break;
		case CMS_MSG_WAN_CONNECTION_DOWN:{
				VOICEAPP_DBG("\n processCmsMsg: CMS_MSG_WAN_CONNECTION_DOWN \n");
				#ifndef MSTC_SERVICE_ROUTE
				if(msgHdr->dataLength > 1){
					tmpPtr = (char *)(msgHdr + 1); /*pointer to payload*/
					processIpRoute2(CMS_MSG_WAN_CONNECTION_DOWN,tmpPtr);
				}
				#endif
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_WAN_CONNECTION_DOWN Exit!!\n");					
			}
			break;
		/* bound interface IP changed, activate VoIP service & re-register */
		case CMS_MSG_ZYIMS_SELF_IP_CHANGE: {
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_ZYIMS_SELF_IP_CHANGE\n");
				if(VoIPIsActive) {
					if(isVoIPinUsed()) {
						timerStartS(10000, resetIPTimer, NULL);
					} else {
						voipDeactivate();
						tr104ReloadAll();							
						#ifdef DIAL_PLAN
						dialplanReload();
						#endif
						voipActivate();
					}
				} else {
					tr104ReloadAll();						
					#ifdef DIAL_PLAN
					dialplanReload();
					#endif
					voipActivate();
				}
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_ZYIMS_SELF_IP_CHANGE Exit!!\n");				
			}
			break;
		/* bound interface disconnected, deactivate VoIP service */
		case CMS_MSG_ZYIMS_WITHOUT_IP: {
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_ZYIMS_WITHOUT_IP\n");		
				if(VoIPIsActive) {
					voipDeactivate();
				}
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_ZYIMS_WITHOUT_IP Exit\n");				
			}
			break;
		case CMS_MSG_VOICE_CONFIG_CHANGED: {
				VOICEAPP_DBG("processCmsMsg: CMS_MSG_VOICE_CONFIG_CHANGED");
			
				if(msgHdr->dataLength == 2*sizeof(uint32)){ /*profInst, lineInst*/
					tmpPtr = (char *)(msgHdr + 1); /*pointer to payload*/
					
					SipAccountId = *((uint32 *)tmpPtr+1);	
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					pLine = tr104VoiceLineObjPtrGet(VpItr , LineItr);
			
					if((NULL != pLine) && (TR104I_STATUS_UNREGISTERED == pLine->status)){						
						//pLine->status = TR104I_STATUS_QUIECCENT;
						pLine->status = TR104I_STATUS_IDLE;
					}
				}
				/* Set voice changed flag */				
				voiceChangedFlag = 1 ;
			}
			break;
		//Ssu-Ying, support Multiple Profile
		case CMS_MSG_VOICE_CRITICAL_CHANGED: 
			{				
				VOICEAPP_DBG("processCmsMsg: CMS_MSG_VOICE_CRITICAL_CHANGED");
				voiceCriticalChangedFlag = 1;
			}
			break;
		/* voice config was changed & written to flash, reload tr104 & reset IPTK config */
		case CMS_MSG_CONFIG_WRITTEN: {
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_CONFIG_WRITTEN\n");			
				if(voiceCriticalChangedFlag){
					if(VoIPIsActive) {
					VOICEAPP_DBG("processCmsMsg: CMS_MSG_CONFIG_WRITTEN & voiceCriticalChangedFlag");
						if(isVoIPinUsed()) {
							timerStartS(10000, resetConfigTimer, NULL);
						} else {
							voipDeactivate();
							deleteAllSipCCMConvert();
							delete_AllPhoneCB();
							#ifdef DIAL_PLAN
							doDialplanClear();
							#endif
							tr104ModuleFree();											
							tr104ModuleInit();
							#ifdef DIAL_PLAN
							initDialPlan();
							#endif
							phoneInit();
							initSipCCMConvert();
							voiceCriticalChangedFlag = 0;
							voiceChangedFlag = 0;							
							voipActivate();
						}
					} else {
						deleteAllSipCCMConvert();
						delete_AllPhoneCB();
						#ifdef DIAL_PLAN
						doDialplanClear();
						#endif
						tr104ModuleFree();											
						tr104ModuleInit();
						#ifdef DIAL_PLAN
						initDialPlan();
						#endif
						phoneInit();
						initSipCCMConvert();
						voiceCriticalChangedFlag = 0;
						voiceChangedFlag = 0;						
						voipActivate();
					}
					
				}else if ( voiceChangedFlag ){
					if(VoIPIsActive) {
						VOICEAPP_DBG("processCmsMsg: CMS_MSG_CONFIG_WRITTEN & voiceChangedFlag");
						if(isVoIPinUsed()) {
							timerStartS(10000, resetConfigTimer, NULL);
						} else {
							voipDeactivate();
							tr104ReloadAll();							
							tr104UpdateFXSLineSelect();
							tr104UpdateLinePhyReferenceFXSList();
							tr104UpdateSipCallStatus();
							voiceChangedFlag = 0;
							#ifdef DIAL_PLAN
							dialplanReload();
							#endif
							if(dspReinitNeeded){								
								voiceDspDeinit();
								if(voiceDspInit()<0){
									ZyPrintf("voiceDsp Reinit failed, terminate voiceApp\n");
									exit(1);
								}
								else{
									VOICEAPP_DBG("voiceDsp Reinit successful\n");
									
								}
							}
							voipActivate();
						}
					} else {
						tr104ReloadAll();
						tr104UpdateFXSLineSelect();
						tr104UpdateLinePhyReferenceFXSList();
						tr104UpdateSipCallStatus();
						voiceChangedFlag = 0;
						#ifdef DIAL_PLAN
						dialplanReload();
						#endif
						if(dspReinitNeeded){								
							voiceDspDeinit();
							if(voiceDspInit()<0){
								ZyPrintf("voiceDsp Reinit failed, terminate voiceApp\n");
								exit(1);
							}
							else{
								VOICEAPP_DBG("voiceDsp Reinit successful\n");
								
							}
						}
						voipActivate();
					}
				}
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_CONFIG_WRITTEN Exit\n");					
			}
			break;
		case CMS_MSG_VOICE_DIAG: {
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_VOICE_DIAG\n");			
				diagMsg = (VoiceDiagMsgBody *) (msgHdr + 1);
				if(VOICE_DIAG_EPTCMD == diagMsg->type){         
					ZyPrintf("VOICE_DIAG_EPTCMD: %s\n",diagMsg->cmdLine);
					sip_endptcmd(diagMsg->cmdLine);
				}
				else if(VOICE_DIAG_ZYIMSCMD == diagMsg->type){
					if ( diagMsg->cmdLine != NULL ){
						doZyims(diagMsg->cmdLine);
					}
				}
				else{
					ZyPrintf("Unknown diag message type\n");
				}

				/* Prepare response message */ 
				replyMsg.type = msgHdr->type;
				replyMsg.src = EID_ZYIMS;
				replyMsg.dst = msgHdr->src;
				replyMsg.flags_request = 0;
				replyMsg.flags_response = 1;
				replyMsg.wordData = CMSRET_SUCCESS;

				if ((ret = cmsMsg_send(msgHandle, &replyMsg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_VOICE_DIAG Exit\n");				
			}
			break;
		#ifdef MSTC_VOICE_GR909 /* Support GR909 function,__MSTC_VOICE__,Klose,20121004*/
      case CMS_MSG_VOICE_LINE_TEST: {
            VoiceLineTestMsgBody *ltMsgBody = (VoiceLineTestMsgBody *) (msgHdr + 1);
            ENDPOINTDRV_LT_STATISTICS_PARM lineTestParm; 
            
            lineTestParm.lineId = ltMsgBody->eptIdx;
            lineTestParm.mode = ltMsgBody->testMode;
            
            vrgEndptVoiceLineTest(&lineTestParm);
      
            /* Prepare response message */ 
            replyMsg.type = msgHdr->type;
            replyMsg.src = EID_ZYIMS;
            replyMsg.dst = msgHdr->src;
            replyMsg.flags_request = 0;
            replyMsg.flags_response = 1;
            replyMsg.wordData = CMSRET_SUCCESS;
      
            if ((ret = cmsMsg_send(msgHandle, &replyMsg)) != CMSRET_SUCCESS){
               cmsLog_error("msg send failed, ret=%d",ret);
            }
         }
         break;

      case CMS_MSG_VOICE_PHONE_FLAG: {
            phoneCh_t* PhoneCB = voicePhoneCh.phoneChEntry;
            int phStatus1 = 0, phStatus2 = 0;
            int hookStatus1 = 0, hookStatus2 = 0;

            
            phStatus1 = PhoneCB->phStatus;
            hookStatus1 = phHookStatus(PhoneCB);
            
            PhoneCB = PhoneCB->next;

            if(PhoneCB != NULL){
               phStatus2 = PhoneCB->phStatus;
               hookStatus2 = phHookStatus(PhoneCB);
            }

            cmsLog_debug("phStatus1 = %d , hookStatus1 = %d", phStatus1, hookStatus1);
            cmsLog_debug("phStatus2 = %d , hookStatus2 = %d", phStatus2, hookStatus2);

            VOICEAPP_PRINTF("Process: CMS_MSG_VOICE_PHONE_FLAG\n");					

				/* Prepare response message */ 
				replyMsg.type = msgHdr->type;
				replyMsg.src = EID_ZYIMS;
				replyMsg.dst = msgHdr->src;
				replyMsg.flags_request = 0;
				replyMsg.flags_response = 1;
				
				if(phStatus1 != 0){
				   replyMsg.wordData = 1;/* NOT IDEL, return phone line plugeed status */
				}else if(hookStatus1){
				   replyMsg.wordData = 1;/* IDEL && OFF_HOOK, return phone line plugeed status */
				}else{
				   /* IDEL && ON-HOOK , do LineTest */
				   ENDPOINTDRV_LT_STATISTICS_PARM lineTestParm;
	            lineTestParm.lineId = 0;
               lineTestParm.mode = 3;
				   vrgEndptVoiceLineTest(&lineTestParm);
				   if(lineTestParm.ren > 20){
				      replyMsg.wordData = 1;
				   }else{
				      replyMsg.wordData = 0;
				   }
				}
            if(PhoneCB != NULL){
   				if(phStatus2 != 0){
   				   replyMsg.wordData = (replyMsg.wordData << 1) | 1;/* NOT IDEL, return phone line plugeed status */
   				}else if(hookStatus2){
   				   replyMsg.wordData = (replyMsg.wordData << 1) | 1;/* IDEL && OFF_HOOK, return phone line plugeed status */
   				}else{
   				   /* IDEL && ON-HOOK , do LineTest */
   				   ENDPOINTDRV_LT_STATISTICS_PARM lineTestParm;
   	            lineTestParm.lineId = 1;
                  lineTestParm.mode = 3;
   				   vrgEndptVoiceLineTest(&lineTestParm);
   				   if(lineTestParm.ren > 20){
   				      replyMsg.wordData = (replyMsg.wordData << 1) | 1;
   				   }else{
   				      replyMsg.wordData = (replyMsg.wordData << 1) | 0;
   				   }
   				}
				}

				if ((ret = cmsMsg_send(msgHandle, &replyMsg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
				VOICEAPP_PRINTF("Process: CMS_MSG_VOICE_PHONE_FLAG Exit\n");	
         }
         break;         
      #endif   
	  #ifdef DMP_X_5067F0_PWRMNGT_1  /*__MSTC__ Leo, support phone LED pwrmngt*/
	    case CMS_MSG_PHONE_LED_MGMT: {
            voipLedOnOff(0);
			voipLedOnOff(1);
        }
        break;
	  #endif
		case CMS_MSG_VOICE_STATISTICS_REQUEST: {			
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_VOICE_STATISTICS_REQUEST\n");				
				parms.op[0] = 1; /*the 1st profile */
			
				parms.op[1] = msgHdr->wordData; /*SipAccount , set in stl function*/
				tr104SetMdmVoiceLineStats(&parms , info);

				/* Prepare response message */ 
				msg->type = CMS_MSG_VOICE_STATISTICS_RESPONSE;
				msg->src = EID_ZYIMS;
				msg->dst = msgHdr->src;
				msg->flags_request = 0;
				msg->flags_response = 1;
				msg->wordData = CMSRET_SUCCESS;
				msg->dataLength = sizeof(_VoiceLineStatsObject);				

				if ((ret = cmsMsg_send(msgHandle, msg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
			}
			break;	
		case CMS_MSG_VOICE_STATISTICS_RESET: {	
				parms.op[0] = 1; /*the 1st profile */
				parms.op[1] = msgHdr->wordData; /*SipAccount , set in rcl function*/
				
				tr104ResetVoiceLineStats(&parms);
				
			}
			break;
		case CMS_MSG_CLEAR_VOICE_CALLHISTORY: {
			VOICEAPP_PRINTF("receive CMS_MSG_CLEAR_VOICE_CALLHISTORY\n");				
				if(msgHdr->wordData == CLEAR_CALL_HISTORY_DAY_SUMMARY){ 		
					webCallHistorySummaryClearAll();
				}
				else if(msgHdr->wordData == CLEAR_CALL_HISTORY_ENTRY_OUTCALLS){			
						webCallHistoryClearOutCalls();	
				}
				else if(msgHdr->wordData == CLEAR_CALL_HISTORY_ENTRY_INCALLS){			
						webCallHistoryClearInCalls();			
				}
				else{
					ZyPrintf("Unknown diag message type\n");
				}
				/* Prepare response message */ 


				msgCallHistory->type = CMS_MSG_CLEAR_VOICE_CALLHISTORY;
				msgCallHistory->src = EID_ZYIMS;

			  #ifdef SUPPORT_HTTPD_SSL
			   if ( msgHdr->src ==  EID_HTTPD_SSL) {
			      /* if a child had been forked, wait for its exit */
			      msgCallHistory->dst = EID_HTTPD_SSL;
			   }
			   else {
			     msgCallHistory->dst  = EID_HTTPD;
			   }
			  #else
			   msgCallHistory->dst  = EID_HTTPD;
			  #endif

				msgCallHistory->flags_request = 0;
				msgCallHistory->flags_response = 1;
				msgCallHistory->wordData = CMSRET_SUCCESS;
				msgCallHistory->dataLength = 0;

				if ((ret = cmsMsg_send(msgHandle, msgCallHistory)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
			}
			break;
		case CMS_MSG_GET_VOICE_OUTGOING_CALLHISTORY: {				
				/* Prepare response message */ 
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_GET_VOICE_OUTGOING_CALLHISTORY\n");
				msgCallHistory->type = CMS_MSG_GET_VOICE_OUTGOING_CALLHISTORY;
				msgCallHistory->src = EID_ZYIMS;

			  #ifdef SUPPORT_HTTPD_SSL
			   if ( msgHdr->src ==  EID_HTTPD_SSL ) {
			      /* if a child had been forked, wait for its exit */
			      msgCallHistory->dst = EID_HTTPD_SSL;
			   }
			   else {
			     msgCallHistory->dst  = EID_HTTPD;
			   }
			  #else
			   msgCallHistory->dst  = EID_HTTPD;
			  #endif

				msgCallHistory->flags_request = 0;
				msgCallHistory->flags_response = 1;
				msgCallHistory->wordData = CMSRET_SUCCESS;
				msgCallHistory->dataLength = sizeof(VoiceCallHistoryBody);	

				if(webCallHistory_pool->webCallHistoryEntryOutCalls[webCallHistory_pool->outCallIndex].callHistoryStartTime==0){
					infoCallHistory->totalcalls = webCallHistory_pool->outCallIndex;
				}
				else{
					infoCallHistory->totalcalls = MAX_CALL_HISTORY_ENTRY_OUTCALLS;
				}

				j = 0;
				while(j<MAX_CALL_HISTORY_ENTRY_OUTCALLS){
					for(i = webCallHistory_pool->outCallIndex; i < MAX_CALL_HISTORY_ENTRY_OUTCALLS; i++){
						if(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime != 0){										
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryStartTime, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime)));
							infoCallHistory->webCallHistoryEntryCalls[j].callHistoryTalkDuration = webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryTalkDuration;
							infoCallHistory->webCallHistoryEntryCalls[j].phonePort = webCallHistory_pool->webCallHistoryEntryOutCalls[i].portNum;
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryLocalNumber, webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryPeerNumber);
							j++;
						}
					}
					
					for(i=0; i< webCallHistory_pool->outCallIndex; i++){
						if(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime != 0){				
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryStartTime, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryStartTime)));
							infoCallHistory->webCallHistoryEntryCalls[j].callHistoryTalkDuration = webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryTalkDuration;
							infoCallHistory->webCallHistoryEntryCalls[j].phonePort = webCallHistory_pool->webCallHistoryEntryOutCalls[i].portNum;
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryLocalNumber, webCallHistory_pool->webCallHistoryEntryOutCalls[i].callHistoryPeerNumber);
							j++;
						}
					}
					break;
				}					

				if ((ret = cmsMsg_send(msgHandle, msgCallHistory)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
			}
			break;
		case CMS_MSG_GET_VOICE_INCOMING_CALLHISTORY: {
				diagMsg = (VoiceDiagMsgBody *) (msgHdr + 1);
				/* Prepare response message */ 
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_GET_VOICE_INCOMING_CALLHISTORY\n");			
							
				msgCallHistory->type = CMS_MSG_GET_VOICE_INCOMING_CALLHISTORY;
				msgCallHistory->src = EID_ZYIMS;


#ifdef SUPPORT_HTTPD_SSL
				if ( msgHdr->src ==  EID_HTTPD_SSL ) {
				/* if a child had been forked, wait for its exit */
					msgCallHistory->dst = EID_HTTPD_SSL;
				}
				else{
					msgCallHistory->dst  = EID_HTTPD;
				}
#else
					msgCallHistory->dst  = EID_HTTPD;
#endif

				msgCallHistory->flags_request = 0;
				msgCallHistory->flags_response = 1;
				msgCallHistory->wordData = CMSRET_SUCCESS;
				msgCallHistory->dataLength = sizeof(VoiceCallHistoryBody);
	
				if(webCallHistory_pool->webCallHistoryEntryInCalls[webCallHistory_pool->inCallIndex].callHistoryStartTime==0){
					infoCallHistory->totalcalls = webCallHistory_pool->inCallIndex;
				}
				else{
					infoCallHistory->totalcalls = MAX_CALL_HISTORY_ENTRY_INCALLS;
				}				
						
				j = 0;
				while(j<MAX_CALL_HISTORY_ENTRY_INCALLS){
					for(i = webCallHistory_pool->inCallIndex; i < MAX_CALL_HISTORY_ENTRY_INCALLS; i++){
						if(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime != 0){										
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryStartTime, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime)));
							infoCallHistory->webCallHistoryEntryCalls[j].callHistoryTalkDuration = webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryTalkDuration;
							infoCallHistory->webCallHistoryEntryCalls[j].phonePort = webCallHistory_pool->webCallHistoryEntryInCalls[i].portNum;
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryLocalNumber, webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryPeerNumber);
							j++;
						}
					}
								
					for(i=0; i< webCallHistory_pool->inCallIndex; i++){						
						if(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime != 0){
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryStartTime, webCallHistoryGetTime(&(webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryStartTime)));
							infoCallHistory->webCallHistoryEntryCalls[j].callHistoryTalkDuration = webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryTalkDuration;
							infoCallHistory->webCallHistoryEntryCalls[j].phonePort = webCallHistory_pool->webCallHistoryEntryInCalls[i].portNum;
							strcpy(infoCallHistory->webCallHistoryEntryCalls[j].callHistoryLocalNumber, webCallHistory_pool->webCallHistoryEntryInCalls[i].callHistoryPeerNumber);
							j++;
						}
					}
					break;
				}
				
				if ((ret = cmsMsg_send(msgHandle, msgCallHistory)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
			}
			break;
		case CMS_MSG_GET_VOICE_CALLHISTORY_SUMMARY: {
			msgCallHistory = (CmsMsgHeader *) bufCallHistorySummary;
			/* Prepare response message */ 
								
			msgCallHistory->type = CMS_MSG_GET_VOICE_CALLHISTORY_SUMMARY;
			msgCallHistory->src = EID_ZYIMS;

		  #ifdef SUPPORT_HTTPD_SSL
		   if ( msgHdr->src ==  EID_HTTPD_SSL ) {
		      /* if a child had been forked, wait for its exit */
		      msgCallHistory->dst = EID_HTTPD_SSL;
		   }
		   else {
		     msgCallHistory->dst  = EID_HTTPD;
		   }
		  #else
		   msgCallHistory->dst  = EID_HTTPD;
		  #endif

			msgCallHistory->flags_request = 0;
			msgCallHistory->flags_response = 1;
			msgCallHistory->wordData = CMSRET_SUCCESS;
			msgCallHistory->dataLength = sizeof(VoiceCallHistorySummaryBody);

			if(webCallHistory_pool->dayCallHistory[webCallHistory_pool->currentDaySummaryIndex+1].startTime==0){
				infoCallHistorySummary->totalcount = webCallHistory_pool->currentDaySummaryIndex+1;
			}
			else{
				infoCallHistorySummary->totalcount = MAX_CALL_HISTORY_SUMMARY_DAY;
			}


			j = 0;
			while(j<MAX_CALL_HISTORY_SUMMARY_DAY){
								
				for(i=webCallHistory_pool->currentDaySummaryIndex+1; i<MAX_CALL_HISTORY_SUMMARY_DAY; i++){
					if(0 != webCallHistory_pool->dayCallHistory[i].startTime){
						infoCallHistorySummary->dayCallHistory[j].startTime = webCallHistory_pool->dayCallHistory[i].startTime;
						infoCallHistorySummary->dayCallHistory[j].outgoingCalls = webCallHistory_pool->dayCallHistory[i].outgoingCalls;
						infoCallHistorySummary->dayCallHistory[j].incomingCalls = webCallHistory_pool->dayCallHistory[i].incomingCalls;
						infoCallHistorySummary->dayCallHistory[j].missingCalls = webCallHistory_pool->dayCallHistory[i].missingCalls;
						infoCallHistorySummary->dayCallHistory[j].duration = webCallHistory_pool->dayCallHistory[i].duration;
						j++;
					}
				}
													
				for(i=0; i<=webCallHistory_pool->currentDaySummaryIndex; i++){
					if(0 != webCallHistory_pool->dayCallHistory[i].startTime){		
						infoCallHistorySummary->dayCallHistory[j].startTime = webCallHistory_pool->dayCallHistory[i].startTime;
						infoCallHistorySummary->dayCallHistory[j].outgoingCalls = webCallHistory_pool->dayCallHistory[i].outgoingCalls;
						infoCallHistorySummary->dayCallHistory[j].incomingCalls = webCallHistory_pool->dayCallHistory[i].incomingCalls;
						infoCallHistorySummary->dayCallHistory[j].missingCalls = webCallHistory_pool->dayCallHistory[i].missingCalls;
						infoCallHistorySummary->dayCallHistory[j].duration = webCallHistory_pool->dayCallHistory[i].duration;
						j++;
					}
				}
				break;
			}
														
								
				if ((ret = cmsMsg_send(msgHandle, msgCallHistory)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
			}
			break;
		case CMS_MSG_GET_VOICE_LINE_STATUS:
			{
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_GET_VOICE_LINE_STATUS\n");
				uint16 line;
				uint16 SipAccountId = 0;
				uint16 LineItr = 0, VpItr=0;

				char buf[sizeof(CmsMsgHeader) + 20]={0}; /*VoiceProfile.{i}.Line.{i}.Status string*/				
				CmsMsgHeader *msg=(CmsMsgHeader *)buf;
				char *statusStr = (char *)buf + sizeof(CmsMsgHeader);
				VoiceProfLineObj_t *lineObj = NULL;
				char *ptr;

				ptr = ((char *)msgHdr + sizeof(CmsMsgHeader));					

				SipAccountId = *((uint16 *)ptr);
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					VOICEAPP_PRINTF("receive CMS_MSG_GET_VOICE_LINE_STATUS, SipAccountId = %d, VpItr = %d, LineItr = %d\n", SipAccountId,VpItr,LineItr);
					lineObj = tr104VoiceLineObjPtrGet(VpItr , LineItr);
					
					msg->type = CMS_MSG_GET_VOICE_LINE_STATUS;
					msg->src = EID_ZYIMS;
					msg->dst = msgHdr->src;
					msg->flags_request = 0;
					msg->flags_response = 1;
					
					

					if(NULL != lineObj){
						msg->wordData = CMSRET_SUCCESS;
						msg->dataLength = 20;
						VOICEAPP_PRINTF("voice line status: %d\n", lineObj->status);
						switch(lineObj->status){
							case TR104I_STATUS_UP:
								snprintf(statusStr, 20-1, "OnRegister");
								break;
							case TR104I_STATUS_REGISTERING:						
							case TR104I_STATUS_INITIALIZING:													
							case TR104I_STATUS_UNREGISTERING:							
							case TR104I_STATUS_TESTING:							
							case TR104I_STATUS_QUIECCENT:
							case TR104I_STATUS_IDLE:
							case TR104I_STATUS_ERROR:						
							case TR104I_STATUS_UNREGISTERED:
								snprintf(statusStr, 20-1, "Not Registered");
								break;
							case TR104I_STATUS_DISABLED:
							default:
								snprintf(statusStr, 20-1, "Inactive");
						}
					}
					else{
						msg->wordData = CMSRET_INTERNAL_ERROR;
						msg->dataLength = 0;
					}

				if ((ret = cmsMsg_send(msgHandle, msg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
				VOICEAPP_PRINTF("processCmsMsg: CMS_MSG_GET_VOICE_LINE_STATUS Exit\n");					
			}
			break;
#if 1 /*Jason , [SPRID # 090825486]*/
		case CMS_MSG_VOICE_CM_ENDPT_STATUS: /*for TR069 get value*/
			{
				uint16 line=0;
		        /* Create replybuf */
		        char buf[sizeof(CmsMsgHeader) + sizeof(VoiceLineObjStatus)]={0}; 
				CmsMsgHeader *msg=(CmsMsgHeader *)buf;
				VoiceLineObjStatus *statusStr = (VoiceLineObjStatus*) (msg + 1);  
				VoiceProfLineObj_t *lineObj = NULL;
				sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
				ccmConn_t *ccmConn_p = NULL;

				uint16 SipAccountId = 0;	
				uint16 LineItr = 0, VpItr=0;
				SipAccountId = msgHdr->wordData;
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					VOICEAPP_PRINTF("receive CMS_MSG_VOICE_CM_ENDPT_STATUS, SipAccountId = %d,VpItr = %d,LineItr = %d\n", SipAccountId,VpItr,LineItr);
					lineObj = tr104VoiceLineObjPtrGet(VpItr , LineItr);
					
					msg->type = CMS_MSG_VOICE_CM_ENDPT_STATUS;
					msg->src = EID_ZYIMS;
					msg->dst = msgHdr->src;
					msg->flags_request = 0;
					msg->flags_response = 1;

					if(NULL != lineObj){
						msg->wordData = CMSRET_SUCCESS;
						msg->dataLength = sizeof(VoiceLineObjStatus);
						VOICEAPP_PRINTF("voice line status: %d\n", lineObj->status);
						switch(lineObj->status){
							case TR104I_STATUS_UP:
								snprintf(statusStr->regStatus, 128-1, "Up");
								break;
							case TR104I_STATUS_INITIALIZING:/*Unuse*/
								snprintf(statusStr->regStatus, 128-1, "Initializing");
								break;							
							case TR104I_STATUS_REGISTERING:
								snprintf(statusStr->regStatus, 128-1, "Registering");
								break;
							case TR104I_STATUS_UNREGISTERING:
								snprintf(statusStr->regStatus, 128-1, "Unregistering");
								break;		
							case TR104I_STATUS_ERROR:
								snprintf(statusStr->regStatus, 128-1, "Error");							
								break;
							case TR104I_STATUS_TESTING:/*Unuse*/
								snprintf(statusStr->regStatus, 128-1, "Testing");						
								break;							
							case TR104I_STATUS_QUIECCENT:/*not support Quieccent state*/
							case TR104I_STATUS_IDLE:
							case TR104I_STATUS_DISABLED:
							case TR104I_STATUS_UNREGISTERED:	
							default:
								snprintf(statusStr->regStatus, 128-1, "Disabled");
								break;	
						}

						/*second parameter, status*/	
						tmpConvertEntry_p = findPermanentSipCCMConvertByLineId(SipAccountId);
						if(NULL != tmpConvertEntry_p){		
							ccmConn_p = findOrigCcmConn(SIP_PORT, tmpConvertEntry_p->sipCcmPortId);
							switch(ccmConn_p->connSt){						
								case CALL_CONN_ST_CONNIN:
									lineObj->callState = TR104I_STATE_RINGING;
									snprintf(statusStr->callStatus, 128-1, "Ringing");
									break;
								case CALL_CONN_ST_CONNOUT:
									lineObj->callState = TR104I_STATE_CALLING;
									snprintf(statusStr->callStatus, 128-1, "Calling");								
									break;
								case CALL_CONN_ST_CONNRDY:
									if(ccmConn_p->flags&CALL_CONN_FLAG_HOLD){
										lineObj->callState = TR104I_STATE_HOLD;
										snprintf(statusStr->callStatus, 128-1, "Hold");										
									}else{
										lineObj->callState = TR104I_STATE_INCALL;
										snprintf(statusStr->callStatus, 128-1, "InCall");									
									}
									break;
								case CALL_CONN_ST_IDLE:
								default:
									lineObj->callState = TR104I_STATE_IDLE;
									snprintf(statusStr->callStatus, 128-1, "Idle");									
									break;						
							}
						}
					}else{
						msg->wordData = CMSRET_INTERNAL_ERROR;
						msg->dataLength = 0;
					}

				if ((ret = cmsMsg_send(msgHandle, msg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
			}
			break;
#endif	/*Jason , [SPRID # 090825486]*/		
		case CMS_MSG_VOICE_SIP_REGISTER:
			{
				uint16 SipAccountId = msgHdr->wordData;
				int LineItr = 0, VpItr=0;				
				VOICEAPP_PRINTF("receive CMS_MSG_VOICE_SIP_REGISTER for SipAccount %d!\n",SipAccountId);	

				VpItr = mapSipAccountIdToVpItr(SipAccountId);
				LineItr = mapSipAccountIdToLineItr(SipAccountId);				
				VoiceProfLineObj_t* lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
				/*[RD Internal] HGW shouldn't register sip account if account is "changeme".,__CHT__,Mitrastar Jason,20120611*/
				VOICEAPP_PRINTF("%s: VoIPIsActive is %u \n",__FUNCTION__,VoIPIsActive );					
				if((lineObj!=NULL) && lineObj->enable && strcmp(lineObj->directoryNumber,"changeme")&& ICFLineInitDone[SipAccountId])
				{
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
					voiceAppSendRegister(SipAccountId);
				}else{
					if(!ICFLineInitDone[SipAccountId]){
						VOICEAPP_PRINTF("ICFLineInitDone[%d] is 0!!\n",SipAccountId);
					}else if(lineObj==NULL){
						ZyPrintf("lineObj is NULL!!\n");
					}else{
						VOICEAPP_PRINTF("enable : %d, directoryNumber is %s\n", lineObj->enable, lineObj->directoryNumber);
					} 		
				}
				VOICEAPP_PRINTF("receive CMS_MSG_VOICE_SIP_REGISTER Exit!\n");				
			}
			break;
		case CMS_MSG_VOICE_SIP_DEREGISTER:
			{
				uint16 SipAccountId = msgHdr->wordData;
				int LineItr = 0, VpItr=0;

				VpItr = mapSipAccountIdToVpItr(SipAccountId);
				LineItr = mapSipAccountIdToLineItr(SipAccountId);
				VOICEAPP_PRINTF("receive CMS_MSG_VOICE_SIP_DEREGISTER for SipAccountId = %d\n",SipAccountId);
				VoiceProfLineObj_t* lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);				
				VoiceProfLineCallingFeaturesObj_t* pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr, LineItr);
				/*[RD Internal] HGW shouldn't register sip account if account is "changeme".,__CHT__,Mitrastar Jason,20120611*/
				if(lineObj!=NULL && lineObj->enable && (lineObj->status == TR104I_STATUS_UP))
				{
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
					
					if(pLineFeature->MWIEnable) {
						voiceAppUnSubscribe(SipAccountId);
					}
					voiceAppSendDeRegister(SipAccountId);
				}else{
					if(lineObj==NULL){
						ZyPrintf("lineObj is NULL!!\n");
					}else{
						VOICEAPP_PRINTF("enable : %d, directoryNumber is %s, status is %u.\n", lineObj->enable, lineObj->directoryNumber, lineObj->status);
					} 
					VOICEAPP_PRINTF("Don't send de-register req in this case!!\n");				
				}
				VOICEAPP_PRINTF("receive CMS_MSG_VOICE_SIP_DEREGISTER! Exit\n");				
			}
			break;
		case CMS_MSG_GET_VOICE_LINE_SIP_STATUS:
			{
				uint16 line;
				/*Jason , [SPRID # 090909666] */
				char buf[sizeof(CmsMsgHeader) + 516]={0}; 				
				CmsMsgHeader *msg=(CmsMsgHeader *)buf;
				char *sipStatusStr = (char *)buf + sizeof(CmsMsgHeader);
				char regStatus[20]={0};
				char RegTime[30]={0};
				char uri[389]={0}; /*string length 389 defined in tr104, VoiceService.{i}.VoiceProfile.{i}.Line.{i}.SIP.URI*/
				char messageWaiting[10]={0};
				char lastIncomingNum[WEB_SIP_STATUS_PHONENUMB_LENGTH]={0};
				char lastOutgoingNum[WEB_SIP_STATUS_PHONENUMB_LENGTH]={0};				
				VoiceProfLineObj_t *lineObj = NULL;
				VoiceProfSipObj_t *profSipObj = NULL;
				VoiceProfLineCallingFeaturesObj_t *callfeatureObj = NULL;					
				char *ptr;				
				time_t currTime;
				int durationSec = 0, durationHour = 0, durationMin = 0, durationDay = 0;

				uint16 SipAccountId = 0;
				uint16 LineItr = 0, VpItr=0;
				ptr = ((char *)msgHdr + sizeof(CmsMsgHeader));					

				SipAccountId  = *((uint16 *)ptr);
				VpItr = mapSipAccountIdToVpItr(SipAccountId);
				LineItr = mapSipAccountIdToLineItr(SipAccountId);
				VOICEAPP_PRINTF("receive CMS_MSG_GET_VOICE_LINE_SIP_STATUS, SipAccountId = %d\n", SipAccountId );
				VOICEAPP_PRINTF("receive CMS_MSG_GET_VOICE_LINE_SIP_STATUS, VpItr = %d\n", VpItr);
				VOICEAPP_PRINTF("receive CMS_MSG_GET_VOICE_LINE_SIP_STATUS, LineItr = %d\n", LineItr);
				lineObj = tr104VoiceLineObjPtrGet(VpItr , LineItr);
				profSipObj = tr104VoiceProfSipObjPtrGet(VpItr);
				callfeatureObj = lineObj->callingFeatures;
				
				msg->type = CMS_MSG_GET_VOICE_LINE_SIP_STATUS;
				msg->src = EID_ZYIMS;
#if 1 //__ZYXEL__, Autumn
  #ifdef SUPPORT_HTTPD_SSL
   if ( msgHdr->src ==  EID_HTTPD_SSL ) {
      /* if a child had been forked, wait for its exit */
      msg->dst = EID_HTTPD_SSL;
   }
   else {
     msg->dst  = EID_HTTPD;
   }
  #else
   msg->dst  = EID_HTTPD;
  #endif
#else
   msg->dst = EID_HTTPD;
#endif
				msg->flags_request = 0;
				msg->flags_response = 1;
				
				msg->wordData = CMSRET_SUCCESS;
				/*Jason , [SPRID # 090909666] */
				msg->dataLength = 516;

				if(NULL != lineObj){
					/*first parameter, regStatus*/
					VOICEAPP_PRINTF("voice line status: %d\n", lineObj->status);
					switch(lineObj->status){
						case TR104I_STATUS_UP:
							snprintf(regStatus, 20-1, "On Register");
							break;	
						case TR104I_STATUS_DISABLED:
							snprintf(regStatus, 20-1, "Inactive");
							break;	
						default:
							snprintf(regStatus, 20-1, "Not Registered");
							break;
					}

					/*second parameter, RegTime*/					
					currTime = time(NULL);

					if((NULL!=lineObj) && (lineObj->status == TR104I_STATUS_UP) && (webSipStatus[SipAccountId].lastRegisteration)){
						durationSec = currTime - webSipStatus[SipAccountId].lastRegisteration;
						durationDay = durationSec / 86400;
						durationHour = (durationSec - (durationDay*86400))/ 3600;
						durationMin = (durationSec - (durationDay*86400 + durationHour*3600))/60;
						durationSec = (durationSec - (durationDay*86400 + durationHour*3600 + durationMin*60));
						snprintf(RegTime, sizeof(RegTime)-1, "%d day(s), %2d:%02d:%02d", durationDay, durationHour, durationMin, durationSec);
						RegTime[sizeof(RegTime)-1]=0;
					}
					else{
						snprintf(RegTime, sizeof(RegTime)-1, " ");
					}
					
					/*third parameter, uri*/
					if(NULL != profSipObj){
						snprintf(uri, 389-1, "%s@%s", lineObj->directoryNumber, profSipObj->userAgentDomain);
					}
					else{
						snprintf(uri, 389-1, " ");
					}

					/*forth parameter, messageWaiting*/
					if(NULL != callfeatureObj){
						if((TR104I_STATUS_UP == lineObj->status) && callfeatureObj->messageWaiting){
							snprintf(messageWaiting, 10-1, "Yes");
						}
						else{
							snprintf(messageWaiting, 10-1, "No");
						}
					}
					else{
						snprintf(messageWaiting, 10-1, " ");
					}

					/*fifth parameter, lastIncomingNum*/
					if(strlen(webSipStatus[SipAccountId].lastIncomingNumber)){
						zyStrncpy(lastIncomingNum, webSipStatus[SipAccountId].lastIncomingNumber, WEB_SIP_STATUS_PHONENUMB_LENGTH);
					}
					else{
						zyStrncpy(lastIncomingNum, " ", WEB_SIP_STATUS_PHONENUMB_LENGTH);
					}
					
					/*sixth parameter, lastOutgoingNum*/
					if(strlen(webSipStatus[SipAccountId].lastOutgoingNumber)){
						zyStrncpy(lastOutgoingNum, webSipStatus[SipAccountId].lastOutgoingNumber, WEB_SIP_STATUS_PHONENUMB_LENGTH);
						#if 1 /*Jason, [SPRID # 090909666] */
						lastOutgoingNum[(sizeof(lastOutgoingNum) / sizeof(lastOutgoingNum[0])) - 1] = 0; // make last char null
						#endif
					}
					else{
						zyStrncpy(lastOutgoingNum, " ", WEB_SIP_STATUS_PHONENUMB_LENGTH);
					}

					/*Jason ,[SPRID # 090909666] */
					snprintf(sipStatusStr, 516-1, "%s;%s;%s;%s;%s;%s", 
						 					              regStatus, 
					    						          RegTime, 
											              uri, 
					    						          messageWaiting, 
											              lastIncomingNum, 
											              lastOutgoingNum);	
					sipStatusStr[516-1]=0; /*prevent string overflow*/
					VOICEAPP_PRINTF("reply %s\n", sipStatusStr);
				
				}



				if ((ret = cmsMsg_send(msgHandle, msg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
				VOICEAPP_PRINTF("receive CMS_MSG_GET_VOICE_LINE_SIP_STATUS Exit\n");				
			}
			break;
		case CMS_MSG_GET_VOICE_LINE_CALL_STATUS:
			{
				char buf[sizeof(CmsMsgHeader) + MAX_SIP_CALL_STATUS*sizeof(sipCallStatus_t)]={0};
				CmsMsgHeader *msg=(CmsMsgHeader *)buf;
				sipCallStatus_t *callStatus_p = (sipCallStatus_t *)(buf + sizeof(CmsMsgHeader));
				
				char *ptr;
				char duration[20];
				char status[20];
				char codec[20];
				char peerNum[32+1];
				time_t currTime;
				int durationSec = 0, durationHour = 0, durationMin = 0;
				sipCCMConvertEntry_t *tmpConvertEntry_p = NULL;
				ccmConn_t *ccmConn_p = NULL;
				int i = 0;
				

				ptr = ((char *)msgHdr + sizeof(CmsMsgHeader));					
				
				
				VOICEAPP_PRINTF("receive CMS_MSG_GET_VOICE_LINE_CALL_STATUS\n");
				
				
				msg->type = CMS_MSG_GET_VOICE_LINE_CALL_STATUS;
				msg->src = EID_ZYIMS;
#if 1 //__ZYXEL__, Autumn
  #ifdef SUPPORT_HTTPD_SSL
   if ( msgHdr->src ==  EID_HTTPD_SSL ) {
      /* if a child had been forked, wait for its exit */
      msg->dst = EID_HTTPD_SSL;
   }
   else {
     msg->dst  = EID_HTTPD;
   }
  #else
   msg->dst  = EID_HTTPD;
  #endif
#else
   msg->dst = EID_HTTPD;
#endif
				msg->flags_request = 0;
				msg->flags_response = 1;
				
				msg->wordData = CMSRET_SUCCESS;
				msg->dataLength = sizeof(sipCallStatus_t)*MAX_SIP_CALL_STATUS;

				VOICEAPP_PRINTF("send CMS_MSG_GET_VOICE_LINE_CALL_STATUS, dataLength = %d\n", msg->dataLength);

				currTime = time(NULL);
				VOICEAPP_PRINTF("currTime %d\n", currTime);
				
				#if 1 /*Eason, sipCallStatus*/
				if ( sipCCMConvert.sipCCMConvertEntry != NULL ) {
					i = 0;
					tmpConvertEntry_p = sipCCMConvert.sipCCMConvertEntry;
					while ( tmpConvertEntry_p != NULL ) {
						if(i<MAX_SIP_CALL_STATUS){
							memcpy(callStatus_p, &(tmpConvertEntry_p->sipCallStatus), sizeof(sipCallStatus_t));
							VOICEAPP_PRINTF("CMS_MSG_GET_VOICE_LINE_CALL_STATUS, i = %d, addr = %d\n", i, callStatus_p);
							VOICEAPP_PRINTF("status = %d\n", callStatus_p->status);
							VOICEAPP_PRINTF("callStartTime = %d\n", callStatus_p->callStartTime);
							VOICEAPP_PRINTF("codec = %s\n", callStatus_p->codec);
							VOICEAPP_PRINTF("selfNumber = %s\n", callStatus_p->selfNumber);
							VOICEAPP_PRINTF("peerNumber = %s\n", callStatus_p->peerNumber);
							callStatus_p++;
							i++;
						}
						else{
							break;
						}
												
						tmpConvertEntry_p = tmpConvertEntry_p->next;
						
					}
				}				
				#endif

				if ((ret = cmsMsg_send(msgHandle, msg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
				
			}
			break;
		case CMS_MSG_VOICE_STOP_REQUEST:
			{
				CmsMsgHeader msg;
				msg.type = CMS_MSG_VOICE_STOP_REQUEST;
				msg.src = EID_ZYIMS;
				msg.dst = msgHdr->src;
				msg.flags_request = 0;
				msg.flags_response = 1;
				VOICEAPP_PRINTF("Process: CMS_MSG_VOICE_STOP_REQUEST\n");
				
				msg.dataLength = 0;
				if(!isVoIPinUsed()){
					ZyPrintf("VoIP Not In USE\n");						
					ZyPrintf("De-Init DSP....\n");
					voiceDspDeinit();
					voipDeactivate();
					msg.wordData = CMSRET_SUCCESS;
					if ((ret = cmsMsg_send(msgHandle, &msg)) != CMSRET_SUCCESS){
						cmsLog_error("msg send failed, ret=%d",ret);
					}
					ZyPrintf("%s, send VOICE_STOP response success\n",__FUNCTION__);
					
				}
				else{
					ZyPrintf("Stop VoiceApp Error, VoIP is In USE\n");					
					msg.wordData = CMSRET_REQUEST_DENIED;					
					if ((ret = cmsMsg_send(msgHandle, &msg)) != CMSRET_SUCCESS){
						cmsLog_error("msg send failed, ret=%d",ret);
					}
					if(msgHdr->wordData == 1){ /*1-REBOOT, 2-FIRMWARE_LOAD_START*/
						if(checkVoipInUseTimerId == -1){
							checkVoipInUseTimerId = timerPeriodicStartS(1000,checkVoipInUseTimer, NULL);
						}
					}
					
				}				
				VOICEAPP_PRINTF("Process: CMS_MSG_VOICE_STOP_REQUEST Exit\n");				
			}
			break;
		case CMS_MSG_GET_VOICE_IN_USE_FLAG:
			{
				VOICEAPP_PRINTF("Process: CMS_MSG_GET_VOICE_IN_USE_FLAG\n");					
				uint8 inUseFlag = isVoIPinUsed();
				VOICEAPP_PRINTF("\n inUseFlag = %d \n",inUseFlag);
				/* Prepare response message */ 
				replyMsg.type = msgHdr->type;
				replyMsg.src = EID_ZYIMS;
				replyMsg.dst = msgHdr->src;
				replyMsg.flags_request = 0;
				replyMsg.flags_response = 1;
				replyMsg.wordData = inUseFlag;

				if ((ret = cmsMsg_send(msgHandle, &replyMsg)) != CMSRET_SUCCESS){
					cmsLog_error("msg send failed, ret=%d",ret);
				}
				VOICEAPP_PRINTF("Process: CMS_MSG_GET_VOICE_IN_USE_FLAG Exit\n");				
			}
			break;
		default:
			VOICEAPP_PRINTF("processCmsMsg: Unhandled msg: 0x%x \n", msgHdr->type);
			break;
	} /* switch msgHdr->type */
}

/*************************************************************************
*
*  FUNCTION NAME  : cmsThread
*
*  DESCRIPTION    : This function receives Broadcom CMS message with 1 second timeout
*
*  RETURNS		  :
*************************************************************************/
static void cmsThread(void)
{
	CmsRet ret;
	CmsMsgHeader *msgHdr = NULL;

	while ( 1 ) {
		/* receive msg with 1 second timeout */
		ret = cmsMsg_receiveWithTimeout(msgHandle, &msgHdr, 1000);
		if (ret == CMSRET_SUCCESS) {
			processCmsMsg(msgHdr);
			cmsMem_free(msgHdr);
		}
	}
}

#endif

/*************************************************************************
*
*  FUNCTION NAME  : processMMMsg
*
*  DESCRIPTION    : This function handles MM message for static route & QoS & firewall control
*
*  RETURNS		  :
*************************************************************************/
//Ssu-Ying, support Multiple Profile
void processMMConfigMsg(rtpPortMsg_t *msg)
{
	VOICEAPP_PRINTF("%s > ENTER\n",__FUNCTION__);

	time_t before, after;
	#if 0 /* MSTC porting by Andylin, bind interface , Iproute2 */
	char addr[STRMAXSIZE];
	struct in_addr ipAddr;
	#else
	uint32 ipaddr;
	#endif
	int VpItr= mapSipAccountIdToVpItr(msg->SipAccoundId);
	VoiceProfObj_t* pProf = tr104VoiceProfObjPtrGet(VpItr);
	#ifdef MSTC_VOICE_VLAN_ID_MARK
	VoiceProfRtpObj_t* pProfRtp = tr104VoiceProfRtpObjPtrGet(VpItr);
	#endif

	VOICEAPP_PRINTF("%s > enable [%u]\n",__FUNCTION__,msg->enable);	
	VOICEAPP_PRINTF("%s > src_addr [%u]\n",__FUNCTION__,msg->src_addr); 
	VOICEAPP_PRINTF("%s > src_port [%u]\n",__FUNCTION__,msg->src_port);
	VOICEAPP_PRINTF("%s > dst_port [%u]\n",__FUNCTION__,msg->dst_port);

	if(pProf == NULL){
		ZyPrintf("%s > pProf is NULL\n",__FUNCTION__);
		return;
	}

	before = time(NULL);

	if(msg->enable) {
		if(0 != msg->src_addr) {
			/* MSTC porting by Andylin, bind interface , Iproute2 */
			#if 0
			ipAddr.s_addr = htonl(msg->addr);
			sprintf( addr, inet_ntoa(ipAddr) );
			addVoipStaticRoute(addr , msg->SipAccoundId);
			#else			
			addVoipRTPIproute2(pProf->ZyXEL_ActiveBoundIfName, msg->dst_port, msg->src_port, TR104I_TRANSPORT_UDP);
			#endif
		}
        /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
		#if 0
		if(msg->src_port != 0){
			addVoIPQoSFirewall(msg->src_port, TR104I_TRANSPORT_UDP, pProf->ZyXEL_ActiveBoundIfName);//CLK
		}
		#else
		if(msg->dst_port != 0){                        
			     #ifdef MSTC_VOICE_AUTO_QOS
      			     addVoIPQoSFirewall(msg->dst_port, TR104I_TRANSPORT_UDP, pProf->ZyXEL_ActiveBoundIfName);
			     #elif defined(MSTC_VOICE_VLAN_ID_MARK)				 
				 addVoIPQoSFirewall(msg->dst_port, TR104I_TRANSPORT_UDP, VpItr, (pProfRtp!=NULL)?pProfRtp->VLANIDMark:-1, (pProfRtp!=NULL)?pProfRtp->ethernetPriorityMark:-1);
			     #else
			     addVoIPQoSFirewall(msg->dst_port, TR104I_TRANSPORT_UDP, VpItr);
			     #endif	
		}
		#endif
		
	} else {
		if(0 != msg->src_addr) {
			/* MSTC porting by Andylin, bind interface , Iproute2 */
			#if 0		
			ipAddr.s_addr = htonl(msg->addr);
			sprintf( addr, inet_ntoa(ipAddr) );
			delVoipStaticRoute(addr , msg->SipAccoundId);
			#endif
			delVoipRTPIproute2(pProf->ZyXEL_ActiveBoundIfName, msg->dst_port, msg->src_port, TR104I_TRANSPORT_UDP);
		}
        /* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
		#if 0
		if(msg->src_port != 0){
			delVoIPQoSFirewall(msg->src_port, TR104I_TRANSPORT_UDP , pProf->ZyXEL_ActiveBoundIfName); //CLK
		}
		#else
		if(msg->dst_port != 0){
			#ifdef MSTC_VOICE_AUTO_QOS			
			delVoIPQoSFirewall(msg->dst_port, TR104I_TRANSPORT_UDP , pProf->ZyXEL_ActiveBoundIfName);
			#elif defined(MSTC_VOICE_VLAN_ID_MARK)
			delVoIPQoSFirewall(msg->dst_port, TR104I_TRANSPORT_UDP , VpItr, (pProfRtp!=NULL)?pProfRtp->VLANIDMark:-1, (pProfRtp!=NULL)?pProfRtp->ethernetPriorityMark:-1);
			#else			
			delVoIPQoSFirewall(msg->dst_port, TR104I_TRANSPORT_UDP , VpItr);
			#endif
	    	}
		#endif
	}
	after = time(NULL);
	ZyPrintf("Static route, Firewall, QoS rule setting duration %d\n", after-before);
	VOICEAPP_PRINTF("%s > EXIT\n",__FUNCTION__);	
}

/*************************************************************************
*
*  FUNCTION NAME  : configSysInit
*
*  DESCRIPTION    : This function register ZyIMS user application to the configuration system
*
*  RETURNS		  :
*************************************************************************/
void configSysInit(void)
{
#if 1	/* Broadcom private */
	cmsInit();
#endif
}

/*************************************************************************
*
*  FUNCTION NAME  : configSysTask
*
*  DESCRIPTION    : This function receives events from the configuration system
*
*  RETURNS		  :
*************************************************************************/
void configSysTask(void)
{
#if 1	/* Broadcom private */
	cmsThread();
#endif
}


void sendConfigWrittenToZyIMS(void){
	  CmsMsgHeader msg = EMPTY_MSG_HEADER;
	  msg.src = EID_ZYIMS;
      msg.dst = EID_ZYIMS;
      msg.type = CMS_MSG_CONFIG_WRITTEN;
      msg.flags_event = 1;
      msg.wordData = EID_ZYIMS;  /* this is who wrote out the config file */
      cmsMsg_send(msgHandle, &msg);

}

void sendConfigChangeToZyIMS(void)
{
	  CmsMsgHeader msg = EMPTY_MSG_HEADER;
	  msg.src = EID_ZYIMS;
      msg.dst = EID_ZYIMS;
      msg.type = CMS_MSG_VOICE_CONFIG_CHANGED;
      msg.flags_event = 1;
      msg.wordData = EID_ZYIMS;  /* this is who wrote out the config file */
      cmsMsg_send(msgHandle, &msg);

	  return;
}

#ifdef MSTC_EMAIL_NOTIFICATION
void sendMissedCallEmailNotification(uint8 SipAccountId, uint8 portType, char *localNumber, char *peerNumber, uint32 callStartTime){
	char content[256];
	char title[64]={"You've Got 1 Missed Call"};
	int LineItr = 0, VpItr=0;

	VOICEAPP_PRINTF("sendMissedCallEmailNotification > ENTER, SipAccountId = %d\n", SipAccountId);
	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	VOICEAPP_PRINTF("sendMissedCallEmailNotification > ENTER, VpItr = %d\n", VpItr);
	VOICEAPP_PRINTF("sendMissedCallEmailNotification > ENTER, LineItr = %d\n", LineItr);
	VoiceProfLineCallingFeaturesObj_t* lineFeatureObj = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);
	char timeStr[30]={0};
	struct tm *tmptr =NULL;	
	VOICEAPP_PRINTF("entering %s\n",__FUNCTION__);
	VOICEAPP_PRINTF("LineItr = %d\n",LineItr);
	tmptr = localtime((const long *)&callStartTime);
	if(NULL != tmptr){ 		
		snprintf(	timeStr,
						30,
						"%02d/%02d/%d %.2d:%.2d:%.2d",
						tmptr->tm_mon + 1,
						tmptr->tm_mday,
						tmptr->tm_year + 1900,
						tmptr->tm_hour,
						tmptr->tm_min,
						tmptr->tm_sec
					);
		
	}
	else{
		ZyPrintf("tmptr NULL\n");
		memset(timeStr, 0, 30);
	}
	
	VOICEAPP_PRINTF("callStartTime = %s\n", timeStr);
	if((NULL != lineFeatureObj) && lineFeatureObj->ZyXEL_missedCallEmailEnable){
		//ZyPrintf("ZyXEL_missedCallEmailEnable = %d\n",lineFeatureObj->ZyXEL_missedCallEmailEnable);
		snprintf(content, 256, "You have one missed call on SIP%d:%s from number %s on %s",
			SipAccountId,
			localNumber,
			peerNumber,
			timeStr	
			);
		if((NULL != lineFeatureObj->ZyXEL_missedCallEmailTitle) && (zyStrlen(lineFeatureObj->ZyXEL_missedCallEmailAddr)>0)){
			zyStrcpy(title, lineFeatureObj->ZyXEL_missedCallEmailTitle);		
		}

		VOICEAPP_PRINTF("%s, toMail = %s, subject = %s, content = %s\n", __FUNCTION__, lineFeatureObj->ZyXEL_missedCallEmailAddr, title, content);

		if( (NULL != lineFeatureObj->ZyXEL_missedCallEmailAddr)&&(zyStrlen(lineFeatureObj->ZyXEL_missedCallEmailAddr)>0) &&
			(NULL != lineFeatureObj->ZyXEL_missedCallEmailServerAddr)&&(zyStrlen(lineFeatureObj->ZyXEL_missedCallEmailServerAddr)>0)){
#ifdef SUPPORT_406_VOICE
			if(CMSRET_SUCCESS != dalEmail_sendEmailNotification(lineFeatureObj->ZyXEL_missedCallEmailServerAddr, lineFeatureObj->ZyXEL_missedCallEmailAddr, title, content,"")){
				ZyPrintf("send email notification failed\n");
			}
#else
			if(CMSRET_SUCCESS != dalEmail_sendEmailNotification(lineFeatureObj->ZyXEL_missedCallEmailServerAddr, lineFeatureObj->ZyXEL_missedCallEmailAddr, title, content)){
				ZyPrintf("send email notification failed\n");
			}
#endif
		}
		else{
			ZyPrintf("ZyXEL_missedCallEmailAddr or ZyXEL_missedCallEmailServerAddr NULL\n");
		}
	}
	else{
		ZyPrintf("%s, missed-call email not enabled, skip\n",__FUNCTION__);
	}
	
}
#endif

/* MSTC porting by Andylin, bind interface , Iproute2 */
void resetVoipSIPIproute2(void)
{	
	#if 0 /* fork has hung problem*/
	char *iptablesArgv[7]={0};
	int iptablesArgc=0;
	VOICEAPP_PRINTF("%s > ENTER\n",__FUNCTION__);		
	iptablesArgc=0;
	iptablesArgv[iptablesArgc++]="iptables";
	iptablesArgv[iptablesArgc++]="-t";
	iptablesArgv[iptablesArgc++]="mangle";
	iptablesArgv[iptablesArgc++]="-F";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	
	iptablesArgv[iptablesArgc++]=NULL;	

	forkAndExecvp(iptablesArgv);	
	#else
	char cmd[64];
	sprintf(cmd, "iptables -t mangle -F IP_ROUTE2_VOIP");
	system(cmd);	
	#endif
	VOICEAPP_PRINTF("%s > EXIT\n",__FUNCTION__);		
}
/*************************************************************************
*
*  FUNCTION NAME  : addVoipSIPIproute2
*
*  DESCRIPTION    : This function add VoIP SIP related port to apply ip route2 rule
*
*  RETURNS		  :
*************************************************************************/
void addVoipSIPIproute2(char *ifName, char *bindIfName,char *runtimeIfName,char *regAddr ,uint32 srcPort, uint32 protocol)
{	
	char *protocolStr;
	struct in_addr ipAddr;
	unsigned int bindIfMark = 0;
	unsigned int fd;
	char *iptablesArgv[20]={0};
	int iptablesArgc=0;
	char markValueStr[20]={0};
	char srcPortStr[10]={0};
	VOICEAPP_PRINTF("%s > ENTER\n",__FUNCTION__);		
	protocolStr = protocol ? "TCP" : "UDP";


	if( zyStrcmp(ifName, "Any_WAN") == 0 || strlen(bindIfName) == 0)
		return;
	
	if(get_iface_ipv4_address(runtimeIfName, &ipAddr) != 0) {
		cmsLog_error("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}	

	cmsLck_acquireLock();	
	bindIfMark = Set_mark_by_bindIfname(runtimeIfName);	
	cmsLck_releaseLock();
	
	if(bindIfMark <=0 )
		return;

	#if 0 /* fork has hung problem*/

	iptablesArgv[iptablesArgc++]="iptables";
	iptablesArgv[iptablesArgc++]="-t";
	iptablesArgv[iptablesArgc++]="mangle";
	iptablesArgv[iptablesArgc++]="-I";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	
	iptablesArgv[iptablesArgc++]="-o";
	iptablesArgv[iptablesArgc++]=bindIfName;
	iptablesArgv[iptablesArgc++]="-p";
	iptablesArgv[iptablesArgc++]=protocolStr;
	iptablesArgv[iptablesArgc++]="--sport";
	sprintf(srcPortStr, "%d", srcPort);
	iptablesArgv[iptablesArgc++]=srcPortStr;
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="MARK";
	iptablesArgv[iptablesArgc++]="--set-mark";
	sprintf(markValueStr, "0x%x", bindIfMark);
	iptablesArgv[iptablesArgc++]=markValueStr;
	iptablesArgv[iptablesArgc++]=NULL;
	
	forkAndExecvp(iptablesArgv);	
	#else
	char cmd[128];
	memset(cmd, 0, sizeof(cmd));

	sprintf(cmd, "iptables -t mangle -I IP_ROUTE2_VOIP -o %s -p %s --sport %d -j MARK --or-mark 0x%x"
			,runtimeIfName , protocolStr, srcPort, bindIfMark);
	system(cmd);
	#endif
}
/*************************************************************************
*
*  FUNCTION NAME  : addVoipRTPIproute2
*
*  DESCRIPTION    : This function add VoIP RTP related port to apply ip route2 rule
*
*  RETURNS		  :
*************************************************************************/
void addVoipRTPIproute2(char *bindIfName, uint32 srcPort, uint32 dstPort, uint32 protocol)
{
	unsigned int bindIfMark=0;
        char *protocolStr;
	char srcPortStr[10]={0};
	char dstPortStr[10]={0};
	char markValueStr[20]={0};
	char *iptablesArgv[30]={0};
	int iptablesArgc=0;
	Rtplist_t *newentry = NULL;
	Rtplist_t *tmpentry = NULL;

	protocolStr = protocol ? "TCP" : "UDP";

	cmsLck_acquireLock();	
	bindIfMark = Set_mark_by_bindIfname(bindIfName);
	cmsLck_releaseLock();
	
        if(bindIfMark == 0)
                return;		

	/* return if ifname is exist in rtplist. */
	if(voipRtp_list_H!=NULL){
		tmpentry = voipRtp_list_H;
		while(tmpentry!=NULL){
			if((strcmp(tmpentry->bindIfname,bindIfName)==0)&&
				(tmpentry->dport == dstPort)&&(tmpentry->sport==srcPort))	{
				return;
			}
			tmpentry = tmpentry->next;
		}
	}

	newentry =(Rtplist_t *) malloc(sizeof(Rtplist_t));	
	if(newentry == NULL){
		cmsLog_error("newlist is NULL");
		return;
	}

	if(strlen(bindIfName) >= MAX_IFNAME_LEN){
		cmsLog_error("bindIfName length is too long");	
		free(newentry);
		return;
	}
	strcpy(newentry->bindIfname, bindIfName);
	
	newentry->dport = dstPort;
	newentry->sport = srcPort;	
	
	cmsLog_debug("add voip Rtp rule, ifname = %s, dst port = %d, src port =%d",
		newentry->bindIfname, newentry->dport, newentry->sport);
	
	if(voipRtp_list_H == NULL){
		newentry->next = NULL;			
	}
	else{
		tmpentry = voipRtp_list_H;
		newentry->next = tmpentry;
	}
	voipRtp_list_H = newentry;	

	iptablesArgc=0;
	iptablesArgv[iptablesArgc++]="iptables";
	iptablesArgv[iptablesArgc++]="-t";
	iptablesArgv[iptablesArgc++]="mangle";
	iptablesArgv[iptablesArgc++]="-A";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	
	iptablesArgv[iptablesArgc++]="-o";
	iptablesArgv[iptablesArgc++]=bindIfName;
	iptablesArgv[iptablesArgc++]="-p";
	iptablesArgv[iptablesArgc++]=protocolStr;
	iptablesArgv[iptablesArgc++]="--sport";
	sprintf(srcPortStr, "%d", srcPort);
	iptablesArgv[iptablesArgc++]=srcPortStr;
	iptablesArgv[iptablesArgc++]="--dport";
	sprintf(dstPortStr, "%d", dstPort);
	iptablesArgv[iptablesArgc++]=dstPortStr;
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="MARK";
	iptablesArgv[iptablesArgc++]="--or-mark";
	sprintf(markValueStr, "0x%x", bindIfMark);
	iptablesArgv[iptablesArgc++]=markValueStr;
	iptablesArgv[iptablesArgc++]=NULL;
	#if 0
	while(zyStrcmp(voipRtprule[rtpcount].bindIfname,"") != 0){
		if((zyStrcmp(voipRtprule[rtpcount].bindIfname,bindIfName) == 0 )&&
			(voipRtprule[rtpcount].sport == srcPort)&&(voipRtprule[rtpcount].dport == dstPort) ){
			cmsLog_debug("\n  $$$$$$$ Rtprule : %s , %d , %d  => Return \n",voipRtprule[rtpcount].bindIfname, voipRtprule[rtpcount].sport, voipRtprule[rtpcount].dport);
			return;
		}			
		rtpcount++;			
	}
	zyStrncpy(voipRtprule[rtpcount].bindIfname, bindIfName, zyStrlen(bindIfName));
	voipRtprule[rtpcount].sport = srcPort;
	voipRtprule[rtpcount].dport = dstPort;
	cmsLog_debug("\n  $$$$$$$ Rtprule : %s , %d , %d  => Add \n",voipRtprule[rtpcount].bindIfname, voipRtprule[rtpcount].sport, voipRtprule[rtpcount].dport);

	cmsLog_error("\n iptables add Rtp => +++ %s", iptablesArgv);
	#endif
	
	forkAndExecvp(iptablesArgv);

}

/*************************************************************************
 *
 *  FUNCTION NAME  : delVoipIproute2
 *
 *  DESCRIPTION    : This function del VoIP related port to apply ip route2 rule
 *
 *  RETURNS		  :
 *************************************************************************/
void delVoipRTPIproute2(char *bindIfName, uint32 srcPort, uint32 dstPort, uint32 protocol)
{
	unsigned int bindIfMark=0;
	char *protocolStr;
	char srcPortStr[10]={0};
	char dstPortStr[10]={0};
	char markValueStr[20]={0};
	char *iptablesArgv[30]={0};
	int iptablesArgc=0;
	UBOOL8 found=FALSE;	
	Rtplist_t *preventry = NULL;
	Rtplist_t *tmpentry = NULL;

	protocolStr = protocol ? "TCP" : "UDP";

	cmsLck_acquireLock();
	bindIfMark = Set_mark_by_bindIfname(bindIfName);	
	cmsLck_releaseLock();
	
        if(bindIfMark == 0)
                return;	
	
	if(voipRtp_list_H!=NULL){
		tmpentry = voipRtp_list_H;			
		while(tmpentry!=NULL){							
			if((strcmp(tmpentry->bindIfname,bindIfName) == 0)&&
				(tmpentry->dport == dstPort)&&(tmpentry->sport==srcPort)){				
				cmsLog_debug("Remove voip Rtp rule , ifname = %s, dst port = %d, src port = %d",
					tmpentry->bindIfname, tmpentry->dport, tmpentry->sport);
				if(tmpentry == voipRtp_list_H){		
					voipRtp_list_H = tmpentry->next;
				}
				else if(preventry != NULL){
					preventry->next = tmpentry->next;
				}	
				else{
					cmsLog_error("preventry is NULL");
				}
				found = TRUE;
				break;
			}	
			else{
				preventry = tmpentry;	
				tmpentry = tmpentry->next;		
			}										
		}		
	}	
	        
	iptablesArgc=0;
	iptablesArgv[iptablesArgc++]="iptables";
	iptablesArgv[iptablesArgc++]="-t";
	iptablesArgv[iptablesArgc++]="mangle";
	iptablesArgv[iptablesArgc++]="-D";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	 
	iptablesArgv[iptablesArgc++]="-o";
	iptablesArgv[iptablesArgc++]=bindIfName;
	iptablesArgv[iptablesArgc++]="-p";
	iptablesArgv[iptablesArgc++]=protocolStr;	
	iptablesArgv[iptablesArgc++]="--sport";
	sprintf(srcPortStr, "%d", srcPort);
	iptablesArgv[iptablesArgc++]=srcPortStr;
	iptablesArgv[iptablesArgc++]="--dport";
	sprintf(dstPortStr, "%d", dstPort);
	iptablesArgv[iptablesArgc++]=dstPortStr;
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="MARK";
	iptablesArgv[iptablesArgc++]="--or-mark";
	sprintf(markValueStr, "0x%x", bindIfMark);
	iptablesArgv[iptablesArgc++]=markValueStr;
	iptablesArgv[iptablesArgc++]=NULL;

	forkAndExecvp(iptablesArgv);

	if(found){
		free(tmpentry);
	}
	#if 0
	while(zyStrcmp(voipRtprule[rtpcount].bindIfname,"") != 0){			
		if((zyStrcmp(voipRtprule[rtpcount].bindIfname,bindIfName) == 0 )&&
			(voipRtprule[rtpcount].sport == srcPort)&&(voipRtprule[rtpcount].dport == dstPort) )
		{
			Found = TRUE;
			cmsLog_debug("\n  $$$$$$$ Rtprule : %s , %d , %d => Del \n",voipRtprule[rtpcount].bindIfname, voipRtprule[rtpcount].sport, voipRtprule[rtpcount].dport);
			
			zyStrcpy(voipRtprule[rtpcount].bindIfname, "");
			voipRtprule[rtpcount].sport = 0;
			voipRtprule[rtpcount].dport = 0;

			tmpCount=rtpcount;
			while(zyStrcmp(voipRtprule[tmpCount+1].bindIfname,"") != 0){
				zyStrcpy(voipRtprule[tmpCount].bindIfname, voipRtprule[tmpCount+1].bindIfname);
				voipRtprule[tmpCount].sport = voipRtprule[tmpCount+1].sport;
				voipRtprule[tmpCount].dport = voipRtprule[tmpCount+1].dport;
				tmpCount++;
			}	
			break;
		}
		else{
			rtpcount++;
		}			
	}		
	if(Found){
		
	}	
	#endif
}

int Set_mark_by_bindIfname(const char *bindifName)
{
	#ifdef MSTC_SERVICE_ROUTE
	int fwmark = 0;	
	fwmark = rutWan_getServiceRouteFwmarkByWANIfName(bindifName, 1);
	VOICEAPP_PRINTF("rutWan_getServiceRouteFwmarkByWANIfName %s, fwmark 0x%x", bindifName, fwmark);
	return fwmark;
	#else
	char tempifname[zyStrlen(bindifName)+1];
	InstanceIdStack iidStack;
	void *mdmObj = NULL;
	CmsRet ret;
	int Ret = 0;
	UBOOL8 found=FALSE;

	if (IS_EMPTY_STRING(bindifName))
	{
	  	cmsLog_error("invalid argument. intfname=0x%x", (UINT32)bindifName);
		return Ret;
	}
	
	memset(tempifname, 0, zyStrlen(bindifName)+1 );
	zyStrncpy(tempifname, bindifName, zyStrlen(bindifName));
	tempifname[zyStrlen(bindifName)+1]='\0';	
		
	INIT_INSTANCE_ID_STACK(&iidStack);

	if (cmsUtl_strstr(tempifname, PPP_IFC_STR)){
		while(!found && (ret = cmsObj_getNext(MDMOID_WAN_PPP_CONN, &iidStack, &mdmObj)) == CMSRET_SUCCESS){
#ifdef MSTC_TELEFONICA_CUSTOMIZATION /* MSTC, for Telefonica common,Andylin */
                if(0 == cmsUtl_strcmp(((WanPppConnObject *)mdmObj)->X_BROADCOM_COM_IfName , tempifname)){
#else
		if(0 == cmsUtl_strcmp(((WanPppConnObject *)mdmObj)->X_5067F0_IfName , tempifname)){
#endif
				found = TRUE;
				break;
			}
			cmsObj_free(&mdmObj);	
		}			
		if(found){		
			#ifdef MSTC_SERVICE_ROUTE
			Ret = (((WanPppConnObject *)mdmObj)->X_5067F0_ServiceRoute_Marking);
			#else
			Ret = (((WanPppConnObject *)mdmObj)->X_5067F0_IPR2_MARKING) << IPR2_MARK_OFFSET;	
			#endif
			cmsObj_free(&mdmObj);	
		}
		
	}
	else{
		while(!found && (ret = cmsObj_getNext(MDMOID_WAN_IP_CONN, &iidStack, &mdmObj)) == CMSRET_SUCCESS){
#ifdef MSTC_TELEFONICA_CUSTOMIZATION /* MSTC, for Telefonica common,Andylin */
                        if(0 == cmsUtl_strcmp(((WanIpConnObject *)mdmObj)->X_BROADCOM_COM_IfName , tempifname)){
#else
			if(0 == cmsUtl_strcmp(((WanIpConnObject *)mdmObj)->X_5067F0_IfName , tempifname)){
#endif
				found = TRUE;
				break;
			}
			cmsObj_free(&mdmObj);	
		}			
		if(found){
			#ifdef MSTC_SERVICE_ROUTE
			Ret = (((WanIpConnObject *)mdmObj)->X_5067F0_ServiceRoute_Marking);	
			#else
			Ret = (((WanIpConnObject *)mdmObj)->X_5067F0_IPR2_MARKING) << IPR2_MARK_OFFSET;	
			#endif
			cmsObj_free(&mdmObj);	
		}  
		
	}
	
	return Ret;
        #endif
}
void initiproute2(void){
	#if 0 /* fork has hung problem*/
	char *iptablesArgv[10]={0};
	int iptablesArgc=0;
	ipmark_list_H = NULL;
	voipRtp_list_H = NULL;
	
	iptablesArgc=0;
	iptablesArgv[iptablesArgc++]="mkdir";
	iptablesArgv[iptablesArgc++]="-p";
	iptablesArgv[iptablesArgc++]="/var/iproute2";
	iptablesArgv[iptablesArgc++]=">";
	iptablesArgv[iptablesArgc++]="/dev/null";		
	iptablesArgv[iptablesArgc++]=NULL;	
	forkAndExecvp(iptablesArgv);
	
	iptablesArgc=0;
	iptablesArgv[iptablesArgc++]="iptables";
	iptablesArgv[iptablesArgc++]="-t";
	iptablesArgv[iptablesArgc++]="mangle";
	iptablesArgv[iptablesArgc++]="-N";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2";	
	iptablesArgv[iptablesArgc++]=NULL;	
	forkAndExecvp(iptablesArgv);
	
	#if 1 //Eason, delete before add
	iptablesArgc=3;
	iptablesArgv[iptablesArgc++]="-D";
	iptablesArgv[iptablesArgc++]="OUTPUT";	
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2";	
	iptablesArgv[iptablesArgc++]=NULL;	
	forkAndExecvp(iptablesArgv);
	#endif 
	
	iptablesArgc=3;
	iptablesArgv[iptablesArgc++]="-A";
	iptablesArgv[iptablesArgc++]="OUTPUT";	
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2";	
	iptablesArgv[iptablesArgc++]=NULL;	
	forkAndExecvp(iptablesArgv);

	iptablesArgc=3;
	iptablesArgv[iptablesArgc++]="-N";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	
	iptablesArgv[iptablesArgc++]=NULL;	
	forkAndExecvp(iptablesArgv);

	#if 1 //Eason, delete before add
	iptablesArgc=3;	
	iptablesArgv[iptablesArgc++]="-D";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2";	
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	
	iptablesArgv[iptablesArgc++]=NULL;
	forkAndExecvp(iptablesArgv);
	#endif
	
	iptablesArgc=3;	
	iptablesArgv[iptablesArgc++]="-A";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2";	
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="IP_ROUTE2_VOIP";	
	iptablesArgv[iptablesArgc++]=NULL;
	forkAndExecvp(iptablesArgv);
	
	#else
	
	system("mkdir -p /var/iproute2 > /dev/null 2 > /dev/null");
        system("iptables -t mangle -N IP_ROUTE2");
        system("iptables -t mangle -D OUTPUT -j IP_ROUTE2");
        system("iptables -t mangle -A OUTPUT -j IP_ROUTE2");
        system("iptables -t mangle -N IP_ROUTE2_VOIP");
	system("iptables -t mangle -D IP_ROUTE2 -j IP_ROUTE2_VOIP");
        system("iptables -t mangle -A IP_ROUTE2 -j IP_ROUTE2_VOIP");

	#endif	
}


/* MSTC porting by Andylin, bind interface , Iproute2 */
void processIpRoute2(CmsMsgType msgType, const char *ifName)
{       
        char gwaddr[32]={0};
        char tableName[128]="\0" ,cmd[128] = "\0";
        int mark = 0, x_5067f0_ipr2_marking = 0, tableID = 0;

        switch(msgType)
        {
                case CMS_MSG_WAN_CONNECTION_UP:
			cmsLck_acquireLock();
			x_5067f0_ipr2_marking = getGwandMarkbyifname(ifName, gwaddr);
			cmsLck_releaseLock();
			
                        cmsLog_debug("\n !!! === IProute2 WAN_CONNECTION_UP, ifName : %s , gateway : %s , ifc Mark : %d \n",ifName, gwaddr, x_5067f0_ipr2_marking);			 

			mark = MARK_X_ZyXEL_IPR2_MARKING(x_5067f0_ipr2_marking);
			tableID = TABLEID_X_ZyXEL_IPR2_MARKING(x_5067f0_ipr2_marking);

			cmsLog_debug("\n !!! === IProute2 mark : %d , tableID : %d \n",mark, tableID);

		
			snprintf(tableName, sizeof(tableName), "%d %s%s\n", tableID, RT2_NAME_PRECEDENCE, ifName);
			/* modify /var/iproute2/rt_tables  */
			processIpruleMarkRemoveTable(tableName);
			processIpruleMarkInsertTable(tableName);
			
			/*add default gw to this new routing table*/
			processRoutingTableAccessCmd(ifName, gwaddr, RT2_COMMAND_UNCONFIG);			
			processRoutingTableAccessCmd(ifName, gwaddr, RT2_COMMAND_CONFIG);
						
			/* before add ip rule , remove it avoid repetition*/
			processIpRuleMarkCmd(mark, IPRULE_MASK, ifName, RT2_COMMAND_UNCONFIG, tableID);
			
			/* add new ip rule */
			processIpRuleMarkCmd(mark, IPRULE_MASK, ifName, RT2_COMMAND_CONFIG, tableID);						
			break;
                case CMS_MSG_WAN_CONNECTION_DOWN:					
			x_5067f0_ipr2_marking = RemoveMarkvaluebyifname(ifName);
			if(x_5067f0_ipr2_marking == -1){
				cmsLog_error("\n Ip route Mark error");
				break;
			}
			mark = MARK_X_ZyXEL_IPR2_MARKING(x_5067f0_ipr2_marking);

			processIpRuleMarkCmd(mark, IPRULE_MASK, ifName, RT2_COMMAND_UNCONFIG, tableID);
			cmsLog_debug("\n !!! === IProute2 WAN_CONNECTION_DOWN, mark : %d , ipr2 mark : %d \n",mark, x_5067f0_ipr2_marking);
			
			processRoutingTableAccessCmd(ifName, "0.0.0.0", RT2_COMMAND_CONFIG);

			snprintf(tableName, sizeof(tableName), "%s%s\n", RT2_NAME_PRECEDENCE, ifName);
			processIpruleMarkRemoveTable(tableName);
			
                        break;

        }

}

int getGwandMarkbyifname(const char *ifName, char *gwaddr)
{
	InstanceIdStack iidStack;
	void *mdmObj = NULL;
	CmsRet ret;
	int Mark=0;
	UBOOL8 found=FALSE;

	if (IS_EMPTY_STRING(ifName)){
	  	cmsLog_error("invalid argument. intfname=0x%x", (UINT32)ifName);
	  	Mark = -1;
	}

	Mark = addMarkvaluebyifname(ifName);	
	
	INIT_INSTANCE_ID_STACK(&iidStack);

	if (cmsUtl_strstr(ifName, PPP_IFC_STR)){
		while(!found && (ret = cmsObj_getNext(MDMOID_WAN_PPP_CONN, &iidStack, &mdmObj)) == CMSRET_SUCCESS){			
#ifdef MSTC_TELEFONICA_CUSTOMIZATION /* MSTC, for Telefonica common,Andylin */
                        if(0 == cmsUtl_strcmp(((WanPppConnObject *)mdmObj)->X_BROADCOM_COM_IfName , ifName)){
#else
			if(0 == cmsUtl_strcmp(((WanPppConnObject *)mdmObj)->X_5067F0_IfName , ifName)){
#endif
				found = TRUE;
				break;
			}
			cmsObj_free(&mdmObj);	
		}
		if(found){	
#ifdef MSTC_TELEFONICA_CUSTOMIZATION /* MSTC, for Telefonica common,Andylin */
                        strcpy(gwaddr,((WanPppConnObject *)mdmObj)->X_BROADCOM_COM_DefaultGateway);
#else
			strcpy(gwaddr,((WanPppConnObject *)mdmObj)->X_5067F0_DefaultGateway);
#endif
			((WanPppConnObject *)mdmObj)->X_5067F0_IPR2_MARKING = Mark;
		}  
		else{
			cmsLog_error("cmsObj_get failed");
			Mark = -1;
		}
		
	}
	else{
		while(!found && (ret = cmsObj_getNext(MDMOID_WAN_IP_CONN, &iidStack, &mdmObj)) == CMSRET_SUCCESS){			
#ifdef MSTC_TELEFONICA_CUSTOMIZATION /* MSTC, for Telefonica common,Andylin */
                        if(0 == cmsUtl_strcmp(((WanIpConnObject *)mdmObj)->X_BROADCOM_COM_IfName , ifName)){
#else
			if(0 == cmsUtl_strcmp(((WanIpConnObject *)mdmObj)->X_5067F0_IfName , ifName)){
#endif
				found = TRUE;
				break;
			}
			cmsObj_free(&mdmObj);	
		}
		if(found){	
			strcpy(gwaddr,((WanIpConnObject *)mdmObj)->defaultGateway);
			((WanIpConnObject *)mdmObj)->X_5067F0_IPR2_MARKING = Mark;
		}  
		else{
			cmsLog_error("cmsObj_get failed");
			Mark = -1;
		}		
	}

	if((ret = cmsObj_set(mdmObj, &iidStack)) != CMSRET_SUCCESS)	{
		cmsLog_error("cmsObj_set failed, ret = %d", ret);
		Mark = -1;
	} 
	cmsObj_free(&mdmObj);	
	return Mark;
}

int processIpruleMarkRemoveTable(char *name){

	char *line=NULL, *buf=NULL;
	size_t len = 64;
	ssize_t read;

	FILE* fs;

	if ((fs = fopen(IPROUTE2_ROUTING_TABLE_FILE, "r")) == NULL){		
		cmsLog_error("fopen read error ,invalid intfname: %s", name);
		return -1;
	}

	line = (char *) calloc(1, 64);
	buf = (char *) calloc(1, 1024);
	memset(buf, '\0', sizeof(buf));

	while((read = getline(&line, &len, fs)) != -1){
		if(strstr(line, name) == NULL){
			strncat(buf, line, read);
		}
	}

	if(line){
		free(line);
	}
	fclose(fs);

	if ((fs = fopen(IPROUTE2_ROUTING_TABLE_FILE, "w")) == NULL){		
		cmsLog_error("fopen write ,invalid intfname: %s", name);
		free(buf);
		return -1;
	}
	fputs(buf, fs);
	fclose(fs);
	free(buf);
	
	return 0;
}

int processIpruleMarkInsertTable(char *name){

	FILE* fs;

	if ((fs = fopen(IPROUTE2_ROUTING_TABLE_FILE, "a")) == NULL){		
		cmsLog_error("fopen apend error ,invalid intfname: %s", name);
		return -1;
	}
	fputs(name, fs);
	fclose(fs);

	return 0;
}

void processRoutingTableAccessCmd(char *ifname, char *ipaddr, int cmdType){

	#if 0 /* fork has hung problem*/
	char *iprouteArgv[30]={0};
	int iprouteArgc=0;
	char rt_table[20]={0};
	//memset(rt_table, 0, 20);	

	if (cmdType == RT2_COMMAND_CONFIG){		
		iprouteArgv[iprouteArgc++]="ip";
		iprouteArgv[iprouteArgc++]="route";
		iprouteArgv[iprouteArgc++]="add";
		iprouteArgv[iprouteArgc++]="default";
		if(!IS_EMPTY_STRING(ipaddr) && (strcmp(ipaddr, "0.0.0.0") != 0)){
			iprouteArgv[iprouteArgc++]="via";
			iprouteArgv[iprouteArgc++]=ipaddr;
		}
		else{
			cmsLog_error("invalid ipaddr: %s", ipaddr);
		}
		iprouteArgv[iprouteArgc++]="dev";
		iprouteArgv[iprouteArgc++]=ifname;		
	}else if (cmdType == RT2_COMMAND_UNCONFIG){
		iprouteArgv[iprouteArgc++]="ip";
		iprouteArgv[iprouteArgc++]="route";
		iprouteArgv[iprouteArgc++]="flush";
	}
	iprouteArgv[iprouteArgc++]="table";	
	sprintf(rt_table,"%s%s",RT2_NAME_PRECEDENCE, ifname);	
	iprouteArgv[iprouteArgc++]=rt_table;	
	iprouteArgv[iprouteArgc++]=NULL;	
	
	forkAndExecvp(iprouteArgv);
	
	#else
	char cmd[128];
	memset(cmd, 0, sizeof(cmd));

	if (cmdType == RT2_COMMAND_CONFIG){		
		
		sprintf(cmd, "ip route add default");

		if(!IS_EMPTY_STRING(ipaddr) && (strcmp(ipaddr, "0.0.0.0") != 0)){
			strcat(cmd, " via ");
			strcat(cmd, ipaddr);
		}
		else{
			cmsLog_error("invalid ipaddr: %s", ipaddr);
		}

		strcat(cmd, " dev ");
		strcat(cmd, ifname);
	}
	else if (cmdType == RT2_COMMAND_UNCONFIG){
		sprintf(cmd, "ip route flush");
	}

	strcat(cmd, " table ");
	strcat(cmd, RT2_NAME_PRECEDENCE);
	strcat(cmd, ifname);
	strcat(cmd, " 2>/dev/null");

	system(cmd);
	#endif
	
}

void processIpRuleMarkCmd(unsigned int mark ,unsigned int mask, const char *tableName, int cmdType, int tableID){
	#if 0 /* fork has hung problem*/
	char *ipruleArgv[30]={0};
	int ipruleArgc=0;
	char tmpM[30]={0};
	char tmpId[10]={0};
	char tmpT[20]={0};		

	if (cmdType == RT2_COMMAND_CONFIG){
		ipruleArgv[ipruleArgc++]="ip";
		ipruleArgv[ipruleArgc++]="rule";
		ipruleArgv[ipruleArgc++]="add";
		ipruleArgv[ipruleArgc++]="fwmark";
		sprintf(tmpM,"0x%x/0x%x",mark, mask);
		ipruleArgv[ipruleArgc++]=tmpM;
		ipruleArgv[ipruleArgc++]="prio";
		sprintf(tmpId,"%d",tableID);
		ipruleArgv[ipruleArgc++]=tmpId;
		ipruleArgv[ipruleArgc++]="table";
		sprintf(tmpT,"%s%s",RT2_NAME_PRECEDENCE,tableName);
		ipruleArgv[ipruleArgc++]=tmpT;
		
	}else if (cmdType == RT2_COMMAND_UNCONFIG){
		ipruleArgv[ipruleArgc++]="ip";
		ipruleArgv[ipruleArgc++]="rule";
		ipruleArgv[ipruleArgc++]="del";
		ipruleArgv[ipruleArgc++]="fwmark";		
		sprintf(tmpM,"0x%x/0x%x",mark, mask);
		ipruleArgv[ipruleArgc++]=tmpM;		
	}	
	ipruleArgv[ipruleArgc++]=NULL;	
	
	forkAndExecvp(ipruleArgv);
	#else
	char cmd[128];
	memset(cmd, 0, sizeof(cmd));
	if(cmdType == RT2_COMMAND_CONFIG){
		sprintf(cmd, "ip rule add fwmark 0x%x/0x%x prio %d", mark, mask, tableID);
		strcat(cmd, " table ");
		strcat(cmd, RT2_NAME_PRECEDENCE);
		strcat(cmd, tableName);
	}
	else{
		sprintf(cmd, "ip rule del fwmark 0x%x/0x%x", mark, mask);		
	}
	strcat(cmd, " 2>/dev/null");

	system(cmd);
	#endif
}


int addMarkvaluebyifname(char *ifName){
	#if 0
	int i;	
	for(i=0; i<MAX_MARK_RANGE; i++){
		if(strcmp(iprule_mark_data[i].ifname,"")==0){
			strcpy(iprule_mark_data[i].ifname, ifName);
			iprule_mark_data[i].mark_index = i+1;
			return iprule_mark_data[i].mark_index;
		}
	}	
	return -1;
	#else
	int Mark = 1, count=1;
	ipmarklist_t *newentry = NULL;
	ipmarklist_t *tmpentry = NULL;	
	UBOOL8 found = FALSE;

	if(ipmark_list_H !=NULL){		
		tmpentry = ipmark_list_H;
		while(tmpentry !=NULL){			
			if(!strcmp(tmpentry->ifname,ifName)){
				Mark=tmpentry->mark_index;
				found=TRUE;
				break;
			}
			count++;
			tmpentry = tmpentry->next;
		}
		if(found==FALSE){
			Mark = count;
		}		
	}
	
	if(found==FALSE){
		newentry = (ipmarklist_t *) malloc(sizeof(ipmarklist_t));	
		if(newentry == NULL){
			cmsLog_error("tmplist is NULL");
			return -1;
		}

		if(strlen(ifName) >= MAX_IFNAME_LEN){
			cmsLog_error("interface name length is too long");
			free(newentry);
			return -1;
		}
		strcpy(newentry->ifname, ifName);			
		newentry->mark_index = Mark;
		cmsLog_debug("add mark value, ifname, %s, mark : %d",newentry->ifname, newentry->mark_index);
		
		if(ipmark_list_H == NULL){
			newentry->next = NULL;				
		}
		else{
			tmpentry  = ipmark_list_H;
			newentry->next = tmpentry;		
		}
		ipmark_list_H = newentry;
	}
	
	return Mark;
	#endif
}
int RemoveMarkvaluebyifname(char *ifName){
	#if 0
	int i, Mark=0;
	for(i=0; i<MAX_MARK_RANGE; i++){
		if(strcmp(iprule_mark_data[i].ifname,ifName)==0){			
			strcpy(iprule_mark_data[i].ifname, "");
			Mark = iprule_mark_data[i].mark_index;
			iprule_mark_data[i].mark_index = 0;
			return Mark;
		}
	}
	return -1;
	#else	
	ipmarklist_t *tmpentry = NULL;	
	ipmarklist_t *preventry = NULL;
	int Mark = -1;
	UBOOL8 found = FALSE;
	
	if(ipmark_list_H !=NULL){
		tmpentry = ipmark_list_H;	
		while(tmpentry !=NULL){				
			if(strcmp(tmpentry->ifname, ifName)==0){
				Mark = tmpentry->mark_index;
				VOICEAPP_PRINTF("Remove Mark value , ifname = %s, mark = %d",tmpentry->ifname, Mark);
				if(tmpentry == ipmark_list_H){	
					ipmark_list_H = tmpentry->next;
				}
				else if(preventry != NULL){
					preventry->next = tmpentry->next;
				}	
				else{
					VOICEAPP_PRINTF("preventry is NULL");
				}
				found = TRUE;
				break;
			}	
			else{
				preventry = tmpentry;	
				tmpentry = tmpentry->next;	
			}											
		}			
	}
	if(found){
		free(tmpentry);
	}
	return Mark;
	#endif
}
/* MSTC porting */
