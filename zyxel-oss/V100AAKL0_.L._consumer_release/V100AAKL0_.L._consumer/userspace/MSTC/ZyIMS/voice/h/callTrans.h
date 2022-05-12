#ifndef __CALLTRANS_INCLUDE__
#define __CALLTRANS_INCLUDE__

/* The definition for the ccmConn_t->callFeatures ( (Feature<<8)|(types) ) */
#define CCM_TRANSFER_FIRST_CALL			((CCM_CALL_TRANSFER<<8)|(CCM_TRANSFER_TYPE_FIRST_CALL))
#define CCM_TRANSFER_SECOND_CALL		((CCM_CALL_TRANSFER<<8)|(CCM_TRANSFER_TYPE_SECOND_CALL))
#define CCM_TRANSFER_THIRD_CALL			((CCM_CALL_TRANSFER<<8)|(CCM_TRANSFER_TYPE_THIRD_CALL))

#define CCM_TRANSFER_TYPE_FIRST_CALL	0x01
#define CCM_TRANSFER_TYPE_SECOND_CALL	0x02
#define CCM_TRANSFER_TYPE_THIRD_CALL	0x03

/* The definition for the callTransfer_t->callTransferRole */
#define CALL_TRANSFEROR					1
#define CALL_TRANSFEREE					2
#define CALL_TRANSFER_TARGET			3

/* The definition for the callTransfer_t->callTransferStatus */
#define CALL_TRANSFER_ST_IDLE			0
#define CALL_TRANSFER_ST_SECOND_CALL	1
#define CALL_TRANSFER_ST_TRANSFER		2

/* The definition for the callTransfer_t->callTransferType */
#define CALL_TRANSFER_TYPE_UNATTENDED		0
#define CALL_TRANSFER_TYPE_ATTENDED			1
#define CALL_TRANSFER_TYPE_SEMIATTENDED		2

/* The definition for the callTransfer_t->callTransferFlags */
#define CALL_TRANSFER_CHECK_TYPE		(1<<0)
#define CALL_TRANSFER_SIGNAL_SENDED		(1<<1)

/* The definition for the callTransfer event */
#define	EVT_CT_FIRST_REL				((CCM_TRANSFER_TYPE_FIRST_CALL<<8)|(EVT_CCM_REL_REQ & 0x00FF))			/* Active to drop the call */
#define	EVT_CT_FIRST_DISC				((CCM_TRANSFER_TYPE_FIRST_CALL<<8)|(EVT_CCM_DISC_IND & 0x00FF))			/* Passive to drop the call */
#define	EVT_CT_SEC_START				((CCM_TRANSFER_TYPE_SECOND_CALL<<8)|(EVT_CCM_DIAL_REQ & 0x00FF))
#define	EVT_CT_SEC_OK					((CCM_TRANSFER_TYPE_SECOND_CALL<<8)|(EVT_CCM_CONN_IND & 0x00FF))
#define	EVT_CT_SEC_REL					((CCM_TRANSFER_TYPE_SECOND_CALL<<8)|(EVT_CCM_REL_REQ & 0x00FF))		/* Active to drop the call */
#define	EVT_CT_SEC_DISC					((CCM_TRANSFER_TYPE_SECOND_CALL<<8)|(EVT_CCM_DISC_IND & 0x00FF))		/* Passive to drop the call */
#define	EVT_CT_THIRD_START				((CCM_TRANSFER_TYPE_THIRD_CALL<<8)|(EVT_CCM_DIAL_REQ & 0x00FF))
#define	EVT_CT_THIRD_OK				((CCM_TRANSFER_TYPE_THIRD_CALL<<8)|(EVT_CCM_CONN_IND & 0x00FF))
#define	EVT_CT_THIRD_REL				((CCM_TRANSFER_TYPE_THIRD_CALL<<8)|(EVT_CCM_REL_REQ & 0x00FF))
#define	EVT_CT_THIRD_DISC				((CCM_TRANSFER_TYPE_THIRD_CALL<<8)|(EVT_CCM_DISC_IND & 0x00FF))
#define	EVT_CT_REL						0x000B

typedef struct callTransfer_s
{
	uint8		callTransferRole;		/* 0: UNDEFINE, 1: Transferor, 2: Transferee, 3: TransferTarget */
	uint8		callTransferStatus;
	uint8		callTransferType;		/* 0: Unattended-Transfer, 1: Attendent-Transfer, 2: Semi-Attendent-Transfer */
	uint8		callTransferFlags;
	dialDigitNum_t dialDigitNumber;
	ccmConn_t*	callTransferFirstCCM;	/* The first call between Transferor and Transferee */
	ccmConn_t*	callTransferSecondCCM;	/* The secod call between Transferor and Transfer-Target */
	ccmConn_t*	callTransferThirdCCM;	/* The third call between Transferee and Transfer-Target */
} callTransfer_t;


int createCallTransferCB(ccmConn_t*, ccmConn_t*, uint8, uint8);
void removeCallTransferCB(callTransfer_t*);
int callTransferFsmProc(callTransfer_t*, uint16, void*, uint16);
int callTransferorFsmIdle(callTransfer_t*, uint16, void	* , uint16);
int callTransferorFsmSecond(callTransfer_t*, uint16, void*, uint16);
int callTransferorFsmTransfer(callTransfer_t *, uint16, void*, uint16);
int callTransfereeFsmIdle(callTransfer_t*, uint16, void	* , uint16);
int callTransfereeFsmTransfer(callTransfer_t *, uint16, void*, uint16);

#endif

