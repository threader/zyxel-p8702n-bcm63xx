#include "global.h"
#if 1	/* Broadcom private */
#include <bosTask.h>
#endif
#include "sys_itf.h"
#ifdef BUILD_IGNORE_DIRECT_IP
#include "tr104_object.h"
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#endif


/*************************************************************************
*
*  FUNCTION NAME  : vendorOsInit
*
*  DESCRIPTION    : This function Initialize what vendor OS needs to init
*
*  RETURNS		  :
*************************************************************************/
int 
vendorOsInit(void)
{
	/* Platform porting: for init VoIP related functions */
	/* END of Platform porting */
#if 1	/* Broadcom private */
	bosInit();
#endif
	return 0;
}

static uint16 
taskClassConvert(TASK_CLASS taskClass)
{
	/* Platform porting: for convert task priority */
	/* END of Platform porting */
#if 1	/* Broadcom private */
	switch(taskClass) {
	case TASK_CLASS_HIGH:
		return BOS_CFG_TASK_HIGH_VAL;
		break;
	case TASK_CLASS_MED_HIGH:
		return BOS_CFG_TASK_MED_HIGH_VAL;
		break;
	case TASK_CLASS_MED:
		return BOS_CFG_TASK_MED_VAL;
		break;
	case TASK_CLASS_MED_LOW:
		return BOS_CFG_TASK_MED_LOW_VAL;
		break;
	case TASK_CLASS_LOW:
		return BOS_CFG_TASK_LOW_VAL;
		break;
	default:
		return BOS_CFG_TASK_LOW_VAL;
	}
#endif
}

/*************************************************************************
*
*  FUNCTION NAME  : taskCreate
*
*  DESCRIPTION    : This function create task
*
*  RETURNS		  : int
*************************************************************************/
int 
taskCreate
(
   const char	*name,
   int			stackSize,
   TASK_CLASS	taskClass,
   void			*taskMain
)
{
	/* Platform porting: for creating a task */
	/* END of Platform porting */
#if 1	/* Broadcom private */
	BOS_STATUS  status;
	BOS_TASK_ID taskId;
	BOS_TASK_CLASS bosTaskClass;

	bosTaskClass = taskClassConvert(taskClass);
   
	status = bosTaskCreateMain( name,
				stackSize,
				bosTaskClass,
				NULL,
				(void *)taskMain,
				NULL,
				(BOS_TASK_ARG)0,
				&taskId,
				0,
				BOS_TASKSTART_RUNNING);

	if ( status != BOS_STATUS_OK ) {
		printf("!!%s fail !!\n", __FUNCTION__);
		return RET_FAILURE;
	}
#endif
	
	return RET_SUCCESS;  
}

#ifdef BUILD_IGNORE_DIRECT_IP
int get_voip_iface_address(char *ifname,char *runtimeifname, struct in_addr *ipaddr, char *bindifname)
{
	int socketfd;
	struct ifreq ifrequest;
	char tempifname[256]="";
	int result=0;
	int i=0;
	char *outer_ptr=NULL;
	char *tmpstr = NULL;


	if( zyStrcmp(ifname, "ANY_WAN") == 0 ){
		/* if set ANY_WAN on webgui , use wan interface list*/
		if(zyStrlen(runtimeifname) == 0){
			printf("No available interface for voice service \n");
			return -1;
		}

		zyStrncpy(tempifname, runtimeifname, zyStrlen(runtimeifname));
		i=zyStrlen(runtimeifname);
		tempifname[i+1]='\0';

	}else{
		/* copy current setting for usage */
		zyStrncpy(tempifname, ifname, zyStrlen(ifname));
		i=zyStrlen(ifname);
		tempifname[i+1]='\0';
	}

	/* if the iFname data in rom file is  eth0.14,ppp0,nas1 , search all ifname */
	tmpstr = strtok_r(tempifname, ",",&outer_ptr);

	if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			printf("Error openning socket when getting IP address info\n");
			return -1;
	}


	zyStrncpy(ifrequest.ifr_name,tmpstr, min(IFNAMSIZ, zyStrlen(tmpstr)+1));
	if (ioctl(socketfd,SIOCGIFADDR,&ifrequest) < 0) {
		close(socketfd);
		result=0;
	}else{
		result=1;
	}


	while(result==0){
		tmpstr = strtok_r(NULL, ",",&outer_ptr);
		if(tmpstr==NULL){
			break;
		}

		if ((socketfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			printf("Error openning socket when getting IP address info\n");
			return -1;
		}

		zyStrncpy(ifrequest.ifr_name,tmpstr, min(IFNAMSIZ, zyStrlen(tmpstr)+1));
		if (ioctl(socketfd,SIOCGIFADDR,&ifrequest) < 0) {
			close(socketfd);
			result=0;
		}else{
			result=1;
			break;
		}
	}


	if(result==1){
		//success to get interface.
		*ipaddr = ((struct sockaddr_in *)&(ifrequest.ifr_addr))->sin_addr;
		if (bindifname) {
			zyStrcpy(bindifname, ifrequest.ifr_name);
		}
		close(socketfd);
		return 0;
	}else{
		//fail, can't get any interface ip address.
		return -1;
	}

}

static void forkAndExecvp(char **argv)
{
	pid_t pid;
	int status=0;

	if ((pid=fork()) < 0 ) {
		printf("voiceApp:-   fork FAIL!\n");
		return;
	} else if (pid == 0) {
		if (execvp(argv[0], argv) < 0) {
			printf("voiceApp:-   %s,%s(%d) execvp fail, error:%s(%d)\n",__FILE__,__FUNCTION__,__LINE__,strerror(errno),errno);
		}
		_exit(127);
	} else {
		while(waitpid(pid, &status, 0) < 0) {
			if (errno != EINTR) {
				printf("voiceApp:-   waitpid FAIL!\n");
				return;
			}
		}
	}

	return;
}


/* Insert default DROP rule to VOIP_INPUT chain for peer to peer call issue */
static void voiceSipFilterDrop(int enable, char* ifName, char *srcAddr, char* dstAddr, int dstPort, int protocol)
{
	char *action;
	char *action2;
	char *protocolStr;
	char dstPortStr[10]={0};
	char *iptablesArgv[30]={0};
	int iptablesArgc=0;

	action = enable ? "-I" : "-D";
	protocolStr = protocol ? "TCP" : "UDP";

	iptablesArgc=0;
	iptablesArgv[iptablesArgc++]="iptables";
	iptablesArgv[iptablesArgc++]=action;
	iptablesArgv[iptablesArgc++]="VOIP_INPUT";
	iptablesArgv[iptablesArgc++]="-i";
	iptablesArgv[iptablesArgc++]=ifName;
	iptablesArgv[iptablesArgc++]="-p";
	iptablesArgv[iptablesArgc++]=protocolStr;

	iptablesArgv[iptablesArgc++]="-s";
	iptablesArgv[iptablesArgc++]="!";
	iptablesArgv[iptablesArgc++]=srcAddr;

	iptablesArgv[iptablesArgc++]="-d";
	iptablesArgv[iptablesArgc++]=dstAddr;

	if (dstPort != 0) {
		iptablesArgv[iptablesArgc++]="--dport";
		sprintf(dstPortStr, "%d", dstPort);
		iptablesArgv[iptablesArgc++]=dstPortStr;
	}
	
	iptablesArgv[iptablesArgc++]="-j";
	iptablesArgv[iptablesArgc++]="DROP";
	iptablesArgv[iptablesArgc++]=NULL;
	
	forkAndExecvp(iptablesArgv);
}

/* Insert default DROP rule to VOIP_INPUT chain for peer to peer call issue */
void addVoipFirewallDrop(char *ifName,char *runtimeIfName, char *srcAddr, uint32 dstPort, uint32 protocol)
{
	char dstAddr[32];
	struct in_addr ipAddr;
	char bindIfName[32];

	if (get_voip_iface_address(ifName, runtimeIfName, &ipAddr, bindIfName) != 0) {
		printf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}

	sprintf( dstAddr, inet_ntoa(ipAddr) );
	voiceSipFilterDrop(1, bindIfName, srcAddr, dstAddr, dstPort, (protocol == TR104I_TRANSPORT_UDP)?0:1);
}

void delVoipFirewallDrop(char *ifName,char *runtimeIfName, char *srcAddr, uint32 dstPort, uint32 protocol)
{
	char dstAddr[32];
	struct in_addr ipAddr;
	char bindIfName[32];

	if (get_voip_iface_address(ifName, runtimeIfName, &ipAddr, bindIfName) != 0) {
		printf("voiceApp:-   GET IP ADDRESS FAIL!\n");
		return;
	}

	sprintf( dstAddr, inet_ntoa(ipAddr) );
	
	voiceSipFilterDrop(0, bindIfName, srcAddr, dstAddr, dstPort, (protocol == TR104I_TRANSPORT_UDP)?0:1);
}


#endif



