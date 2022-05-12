#ifndef __CONFIG_SYS_H__
#define __CONFIG_SYS_H__

#define DELETE_ALL 0x80000000

typedef enum {
	LED_UNKNOWN = 0,
	LED_REG = 1,
	LED_MWI = 2
} eVoIPLedType;

typedef enum {
	VOIP_LED_STATE_UNKNOWN = 0,
	VOIP_LED_STATE_OFF = 1,
	VOIP_LED_STATE_ON = 2,
	VOIP_LED_STATE_ALL_ON = 3,
	VOIP_LED_STATE_ALL_OFF = 4,
	VOIP_LED_STATE_FAIL = 5,
	VOIP_LED_STATE_FAIL_OFF = 6,
	VOIP_LED_STATE_SLOW_BLINK = 7,
	VOIP_LED_STATE_FAST_BLINK = 8,
	VOIP_LED_STATE_BLINK_ONECE = 9
} eVoIPLedState;

//Ssu-Ying, support Multiple Profile
void addVoipStaticRoute(char* dest , uint16 SipAccountId);
void delVoipStaticRoute(char* dest , uint16 SipAccountId);

void addVoipFirewallAcl(uint32 port, uint32 protocol , uint16 SipAccountId);
void delVoipFirewallAcl(uint32 port, uint32 protocol , uint16 SipAccountId);
#if 0
void delVoipFirewallAclAll(void);
#endif
void addVoipQosClass(uint32 port, uint32 protocol);
void delVoipQosClass(uint32 port, uint32 protocol);
#if 0
void delVoipQosClassAll(void);
#endif
void voipLedOnOff(uint16 phid);
void voipLedBlinking(uint16 phid);
void voipSetLed(uint16 phid, eVoIPLedType type, eVoIPLedState state);
void configSysInit(void);
void configSysTask(void);

void sendConfigChangeToZyIMS(void);

/* MSTC porting by Andylin, bind interface , Iproute2 */
void resetVoipSIPIproute2(void);
void addVoipSIPIproute2(char *ifName, char *bindIfName,char *runtimeIfName,char *regAddr ,uint32 srcPort, uint32 protocol);
void addVoipRTPIproute2(char *bindIfName, uint32 srcPort, uint32 dstPort, uint32 protocol);
void delVoipRTPIproute2(char *bindIfName, uint32 srcPort, uint32 dstPort, uint32 protocol);
int Set_mark_by_bindIfname(const char *bindifName);
void initiproute2(void);
int getGwandMarkbyifname(const char *ifName, char *gwaddr);
int processIpruleMarkRemoveTable(char *name);
int processIpruleMarkInsertTable(char *name);
void processRoutingTableAccessCmd(char *ifname, char *ipaddr, int cmdType);
void processIpRuleMarkCmd(unsigned int mark ,unsigned int mask, const char *tableName, int cmdType, int tableID);
int addMarkvaluebyifname(char *ifName);
int RemoveMarkvaluebyifname(char *ifName);

#define MAX_IFNAME_LEN	16
#define TABLE_ID_BASE	100
//#define IPR2_MARK_OFFSET	16
#define IPR2_MARK_OFFSET	24 //20
#define MARK_X_ZyXEL_IPR2_MARKING(x)	(x << IPR2_MARK_OFFSET);
#define TABLEID_X_ZyXEL_IPR2_MARKING(x)	(x + TABLE_ID_BASE)
//#define IPRULE_MASK 		0xf0000
#define IPRULE_MASK 		0x3000000 //0xf00000
#define IPROUTE2_ROUTING_TABLE_FILE 	"/var/iproute2/rt_tables"
#define RT2_NAME_PRECEDENCE		"RT2_"
#define IS_EMPTY_STRING(s) ((s == NULL) || (*s == '\0'))
#define RT2_COMMAND_UNCONFIG	0
#define RT2_COMMAND_CONFIG	1

#define WANIPCONNECTTION 0
#define WANPPPCONNECTTION 1

typedef struct ipmarklist_s{
	char ifname[MAX_IFNAME_LEN];	
	int mark_index;
	struct ipmarklist_s *next;
}ipmarklist_t;
ipmarklist_t *ipmark_list_H;


typedef struct Rtplist_s{
	char bindIfname[MAX_IFNAME_LEN];
	uint32 sport;
	uint32 dport;
	struct Rtplist_s *next;
}Rtplist_t;
Rtplist_t *voipRtp_list_H;
/* MSTC porting by Andylin, bind interface , Iproute2 */

/* MSTC, fix voice issue about qos on ftp transport situation, by Andylin */
typedef struct qosFirewallList_s{
	char ifname[MAX_IFNAME_LEN];
	uint32 port;
	uint32 protocol;
	struct qosFirewallList_s *next;
}qosFirewallList_t;
qosFirewallList_t *voipqosFirewall_list_H;

#endif

