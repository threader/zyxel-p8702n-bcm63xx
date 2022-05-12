#ifndef __CALLCONF_INCLUDE__
#define __CALLCONF_INCLUDE__

/* The definition for the conferencecall_t->confcallStatus */
#define CONF_CALL_ST_IDLE						0
#define CONF_CALL_ST_MEET_ROOM_CREATED			1
#define CONF_CALL_ST_1stPARTY_JOINED			2
#define CONF_CALL_ST_ALL_MEMBER_JOINED			3
#define CONF_CALL_ST_ONE_CALL_ONHOLD			4


#define CCM_CONFERENCE_MIXER_LOCAL				0x01
#define CCM_CONFERENCE_MIXER_REMOTE			0x02

#define CCM_CONFERENCE_TYPE_CREATOR_CALL		0x01
#define CCM_CONFERENCE_TYPE_ACTIVE_CALL			0x02
#define CCM_CONFERENCE_TYPE_ONHOLD_CALL		0x03

#define CCM_CALLCONF_CREATOR_CALL		((CCM_CONFERENCE_CALL<<4)|(CCM_CONFERENCE_TYPE_CREATOR_CALL))
#define CCM_CALLCONF_ACTIVE_CALL		((CCM_CONFERENCE_CALL<<4)|(CCM_CONFERENCE_TYPE_ACTIVE_CALL))
#define CCM_CALLCONF_ONHOLD_CALL		((CCM_CONFERENCE_CALL<<4)|(CCM_CONFERENCE_TYPE_ONHOLD_CALL))

/* The definition for the conference call event */
#define EVT_LOCAL_MEETROOM_REQ			(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_LOCAL << 2)) << 8)  | (EVT_CCM_CONFCALL_MEETROOM_REQ & 0x00FF))
#define EVT_REMOTE_MEETROOM_REQ		(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_REMOTE << 2)) << 8)  | (EVT_CCM_CONFCALL_MEETROOM_REQ & 0x00FF))
#define EVT_REMOTE_MEETROOM_CONF		(((CCM_CALLCONF_CREATOR_CALL | (CCM_CONFERENCE_MIXER_REMOTE << 2)) << 8)  | (EVT_CCM_CONN_IND & 0x00FF))
#define EVT_REMOTE_ACTIVE_JOIN			(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_REMOTE << 2)) << 8)  | (EVT_CCM_DISC_IND & 0x00FF))
#define EVT_REMOTE_ONHOLD_JOIN			(((CCM_CALLCONF_ONHOLD_CALL | (CCM_CONFERENCE_MIXER_REMOTE << 2)) << 8)  | (EVT_CCM_DISC_IND & 0x00FF))
#define EVT_LOCAL_MEMBER1_LEAVE			(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_LOCAL << 2)) << 8)  | (EVT_CCM_DISC_IND & 0x00FF))
#define EVT_LOCAL_MEMBER2_LEAVE			(((CCM_CALLCONF_ONHOLD_CALL | (CCM_CONFERENCE_MIXER_LOCAL << 2)) << 8)  | (EVT_CCM_DISC_IND & 0x00FF))
#define EVT_REMOTE_ACTIVE_LEAVE			(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_REMOTE << 2)) << 8)  | (EVT_CCM_DISC_REQ & 0x00FF))
#define EVT_LOCAL_ACTIVE_LEAVE			(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_LOCAL << 2)) << 8)  | (EVT_CCM_DISC_REQ & 0x00FF))
#define EVT_LOCAL_CONFCALL_SPLIT		(((CCM_CALLCONF_ACTIVE_CALL | (CCM_CONFERENCE_MIXER_LOCAL << 2)) << 8)  | (EVT_CCM_ONHOLD_REQ & 0x00FF))


#define CONF_CALL_CHECK_MIXER_MODE							(1<<0)
#define CONF_CALL_CHECK_PEER_ISLOCAL						(1<<1)

/*____________________________________________________________________________*/
/*	type definition                                                                                                             */
/*____________________________________________________________________________*/
typedef struct conferencecall_s
{
	uint8		confcallStatus;
	uint8		confcallFlags;
	uint16		threeWayCallBaseLogicId;
	char*		ConfAddr;
	char*		ContactHeader;
	ccmConn_t*	confcallActiveCCM;
	ccmConn_t*	confcallOnHoldCCM;
	ccmConn_t*	confcallCreatorCCM;
} conferencecall_t;


/*____________________________________________________________________________*/
/*	function                                                                                                                      */
/*____________________________________________________________________________*/
int chkPeerisLocal(dialDigitNum_t *dialDigitNum_p);
int createConferenceCallCB(ccmConn_t*,uint16);
void removeConferenceCallCB(conferencecall_t*);
void ConferenceCallFsmProc(conferencecall_t*, uint16, void*, uint16);
void ConferenceCallFsmIdle(conferencecall_t*, uint16, void*, uint16);
void ConferenceCallFsm1stParty(conferencecall_t*, uint16, void*, uint16);
void ConferenceCallFsm2ndParty(conferencecall_t*, uint16, void*, uint16);
void ConferenceCallFsmMemberJoined(conferencecall_t*, uint16, void*, uint16);

#endif

