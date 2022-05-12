#include <stdio.h>
#include "endpt.h"
#include "Endpoint_itf.h"
#include "mm_port_wrapper.h"
#include "mm_core_fsm_types.h"
#include "tr104_object.h"
#include "switch.h"

/* ---- Private Function Prototypes  for MM --------------------------------------- */
int PhoneInit(int country);
int PhoneChanOpen(int index, int codecType, int codecNum, int cnxid, int callId, int stream_id,int mode, int silenceSupp, int echoCancel, int digitRelay, int ptime, int dtmfCodecNum);
int PhoneChanClose(int index, int cnxid);
int PhoneChanModify(int index, int codecType, int codecNum, int cnxid, int callId, int stream_id, int mode, int silenceSupp, int echoCancel, int digitRelay, int ptime, int dtmfCodecNum);
void TestPktRecvCb( ENDPT_STATE *endptState, int streamId, EPPACKET *epPacketp, int length );
int
RtpSendToDsp(
icf_call_id_t	call_id,
icf_stream_id_t  	stream_id,
icf_media_type_t	stream_type,
uint32 null_pload_type, 
uint8 *InMessage, 
uint16 MessageLen, 
uint32 TimeStamp, 
void* handle
);

void dspSendToDsp(
		icf_call_id_t call_id, 
		icf_stream_id_t stream_id, 
		icf_media_type_t	stream_type,
		uint16 logicalId,
		icf_uint32_t pload_type, 
		icf_uint8_t *packet, 
		icf_uint16_t len, 
		icf_uint32_t timestamp, 
		void* handle);


/* ---- Public Variables ------------------------------------------------- */
CMPHYSENDPT cmPhysEndpt[MAX_ENDPT];
EPZCNXPARAM parm; 
static int ringcnt=1;
extern uint8 mmDspDebug;

/*for MM*/
//extern pthread_mutex_t dsp_chan_mutex;
/* ---- Public Functions for MM-------------------------------------------------------- */

int PhoneInit(int country)
{
	/* Platform porting */
	/* END of Platform porting */
	uint16 cmNumEndpts = 0; 
	int rc;
	EPSTATUS result;
	int i;

	VRG_ENDPT_INIT_CFG   vrgEndptInitCfg;

	vrgEndptInitCfg.country = country;  /* NORTH_AMERICA 16 */

	vrgEndptInitCfg.currentPowerSource = 0;

	printf("BroadcomPhoneInit ::START~~~~~~\n");
	printf("Initializing endpoint with country code %d ,vrgEndptInitCfg.currentPowerSource=%d \n", vrgEndptInitCfg.country,vrgEndptInitCfg.currentPowerSource);
	/* Start the media endpoint service */
	rc = vrgEndptInit( &vrgEndptInitCfg, //endptInitCfg
					NULL,		    //notifyCallback					
					TestPktRecvCb,  				    //packetCallback
					NULL,				   //getProvisionCallback
					NULL,				   //setProvisionCallback
					NULL,				   //packetReleaseCallback
					NULL			          // taskShutdownCallback
					);

                   
	if(rc!=EPSTATUS_SUCCESS){
		VDSP_PRINTF("vrgEndptInit failed!!\n");
		return (-1);
	}


	/* Get the nubmer of endpoints available */
	cmNumEndpts = vrgEndptGetNumEndpoints();
	VDSP_PRINTF("BroadcomPhoneInit::cmNumEndpts=%d \n", cmNumEndpts);
	if ( cmNumEndpts > MAX_ENDPT ){
		VDSP_PRINTF("BroadcomPhoneInit: can only support %d of %d endpoints\n", MAX_ENDPT, cmNumEndpts);
		cmNumEndpts = MAX_ENDPT;
	}

	for ( i=0; i<MAX_PHYS_ENDPT; i++ ) {
		result = vrgEndptCreate(i, i, (VRG_ENDPT_STATE *)(&cmPhysEndpt[i].endptObjState) );

		if( result != EPSTATUS_SUCCESS ){
			VDSP_PRINTF("vrgEndptCreate failed!!\n");
			return (-1);
		}

		VDSP_PRINTF("BroadcomPhoneInit:: cmPhysEndpt[%d].endptObjState=%x, the address of cmPhysEndpt[%d].endptObjState=%x\n",i,cmPhysEndpt[i].endptObjState,i ,&cmPhysEndpt[i].endptObjState);  
		result = vrgEndptCapabilities( &cmPhysEndpt[i].endptObjState, &cmPhysEndpt[i].capabilities );

		if( result != EPSTATUS_SUCCESS ){
			VDSP_PRINTF("vrgEndptCapabilities failed!!\n");
			return (-1);
		}

		cmPhysEndpt[i].toneDetection = 1; //default to 1
#if !GLOBAL_CFG_SIPDQOS      
		/* endptCapabilities reserves a connection vhd, we need to delete it */      
		vrgEndptDeleteConnection( &cmPhysEndpt[i].endptObjState, UNKNOWN );
#endif
	}

	return (0);
}

void PhoneSetCodecByCodecNum(CODECDESC *codecDesc, int codecType, int codecNum){
	switch(codecType){
		case ZYNOS_CODEC_PCMU:
			codecDesc->type=CODEC_PCMU;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_PCMA:
			codecDesc->type=CODEC_PCMA;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G729:
			codecDesc->type=CODEC_G729;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G723:
			codecDesc->type=CODEC_G7231_53_VAR;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G726_16:
			codecDesc->type=CODEC_G726_16;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G726_24:
			codecDesc->type=CODEC_G726_24;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G726_32:
			codecDesc->type=CODEC_G726_32;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G726_40:
			codecDesc->type=CODEC_G726_40;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_G722MODE1:
			codecDesc->type=CODEC_G722_MODE_1;
			codecDesc->rtpPayloadType=codecNum;			
			break;
		case ZYNOS_CODEC_GSM:
			codecDesc->type=CODEC_GSMAMR_12K;
			codecDesc->rtpPayloadType=codecNum;		
			break;
		/** this will cause T38 failed due to no case for ZYNOS_CODEC_T38
		* refer detailed info from http://172.23.56.144/trac/VoIPTeam/ticket/612
		* cindy modify 20100927 */
		case ZYNOS_CODEC_T38:
			printf("\n %s(): case ZYNOS_CODEC_T38: \n",__FUNCTION__);
			codecDesc->type=CODEC_T38;
			codecDesc->rtpPayloadType=codecNum;		
			break;
			/**< cindy change for T38 */
		default:
			VDSP_PRINTF("%s, unknown codec num %d, set to PCMU\n", __FUNCTION__, codecNum);
			codecDesc->type=CODEC_PCMU;
			codecDesc->rtpPayloadType=ZYNOS_CODEC_PCMU;			
			break;
			
	}
}

#if KSOCKET_RTP_SUPPORT
int PhoneKSoketMakeSession(int index, int chanId, int callId, int stream_id, uint32 saddr, uint16 sport, uint32 daddr, uint16 dport, uint8 mediaMode, uint8 qosMark, char *ifName){
	#if 0
	VDSP_PRINTF("entering %s\n", __FUNCTION__);
	VDSP_PRINTF("index = %d, cnx= %d, saddr = 0x%x, sport = %d, daddr = 0x%x, dport = %d\n", index, chanId, saddr, sport, daddr, dport);
	VDSP_PRINTF("mediaMode = %d, qosMark = %d\n", mediaMode, qosMark);
	#endif
	vrgEndptMakeSession( &cmPhysEndpt[index].endptObjState, chanId, callId, stream_id, saddr, sport, daddr, dport, mediaMode, qosMark, ifName);
	return 0;
}
int PhoneKSoketDeleteSession(int index, int chanId, int callId, int stream_id, int *tx_rtp_pkt_cnt, int * tx_rtp_octet_cnt, int * rx_rtp_pkt_cnt, int * rx_rtp_octet_cnt, int * lost){
	#if 0
	VDSP_PRINTF("entering %s\n", __FUNCTION__);
	VDSP_PRINTF("index = %d, cnx= %d\n", index, chanId);
	#endif
	vrgEndptDeleteSession( &cmPhysEndpt[index].endptObjState, chanId, callId, stream_id, tx_rtp_pkt_cnt, tx_rtp_octet_cnt, rx_rtp_pkt_cnt, rx_rtp_octet_cnt, lost);
	return 0;
}

#endif
int PhoneChanOpen(int index,int codecType, int codecNum,int cnxid, int callId, int stream_id, int mode, int silenceSupp, int echoCancel, int digitRelay, int ptime, int dtmfCodecNum)
{
	/* Platform porting */
	/* END of Platform porting */
	EPZCNXPARAM parm;     
	int rc;
	EPZCAP cap;
	int preferredFaxPTCodec = EPVBDCODEC_PCMU; /**< cindy */

	cap=cmPhysEndpt[0].capabilities;

	if(index>MAX_ENDPT || cnxid >MAX_CNX){ //ZyPhone not need to open dsp chan
		return -1;
	}

	if(mode == -1){
		mode = EPCNXMODE_SNDRX;
	}

	if(silenceSupp < 0){
		silenceSupp = 1;
	}
	
	if(echoCancel < 0){
		echoCancel = 1;
	}
	

	if(dtmfCodecNum == 0 && digitRelay == EPDTMFRFC2833_ENABLED){
		digitRelay = EPDTMFRFC2833_DISABLED;
		VDSP_PRINTF("RFC2833 not applicable, use InBand\n");
	}

	if(codecType == ZYNOS_CODEC_G729){
		digitRelay = EPDTMFRFC2833_ENABLED;
		if(dtmfCodecNum < 96){
			dtmfCodecNum = ZYNOS_CODEC_RFC2833;
		}
		VDSP_PRINTF("G729, driver restriction, set RFC2833, dtmfCodecNum = %d\n", dtmfCodecNum);
	}

	if(cap.pCap[0]&0x8000){
		if((ptime < (cap.pCap[0] & 0x7fff)) || (ptime > cap.pCap[1])){		
			ptime = 20;
		}
	}
	else{
		ptime = cap.pCap[0];
	}
	
	
	
	zyMemset( &parm, 0, sizeof(parm) );
   
	PhoneSetCodecByCodecNum(&(parm.cnxParmList.recv.codecs[0]), codecType, codecNum);
	PhoneSetCodecByCodecNum(&(parm.cnxParmList.send.codecs[0]), codecType, codecNum);


	
	
	parm.autoEncoder=1;
	parm.cnxParmList.recv.numCodecs=2;
	parm.cnxParmList.recv.codecs[0].options.opt1=0;
	parm.cnxParmList.recv.codecs[0].options.opt2=0;
	parm.cnxParmList.recv.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.recv.period[0]=ptime;
	parm.cnxParmList.recv.codecs[1].type=CODEC_NTE;
	if(dtmfCodecNum<96 || dtmfCodecNum>150){
		parm.cnxParmList.recv.codecs[1].rtpPayloadType=ZYNOS_CODEC_RFC2833;
	}
	else{
		parm.cnxParmList.recv.codecs[1].rtpPayloadType=dtmfCodecNum;
	}

	parm.cnxParmList.send.codecs[0].options.opt1=0;
	parm.cnxParmList.send.codecs[0].options.opt2=0;
	parm.cnxParmList.send.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.send.period[0]=ptime;	 
	if(digitRelay == EPDTMFRFC2833_ENABLED){
		if(dtmfCodecNum <96 && dtmfCodecNum > 150){
			dtmfCodecNum = 101; /*default value*/
		}
		parm.cnxParmList.send.numCodecs=2;
		parm.cnxParmList.send.codecs[1].type=CODEC_NTE;
		parm.cnxParmList.send.codecs[1].rtpPayloadType=dtmfCodecNum;		
	}
	else{
		parm.cnxParmList.send.numCodecs=1;
	}

	parm.t38param.ecMode=0;
	parm.t38param.maxBitRate=0;
	parm.t38param.maxRemDgrm=0;
	parm.t38param.numFaxPagesRx=0;
	parm.t38param.numFaxPagesTx=0;
	parm.vbdparam.vbdMode=0;
	parm.vbdparam.vbdRedMode=0;
	parm.digitRelayType=digitRelay; /*test, orig 0*/
	parm.echocancel= echoCancel;
	parm.silence=silenceSupp;
	parm.mode=mode; /*EPCNXMODE_SNDRX*/
	parm.namedPhoneEvts = 0;
	if(digitRelay == EPDTMFRFC2833_ENABLED){		
		parm.namedPhoneEvts |= CODEC_NTE_DTMF; 		
		parm.namedPhoneEvts |= CODEC_NTE_LCS;/*CODEC_NTE_LCS = CODEC_NTE_RING, CODEC_NTE_ONHOOK, CODEC_NTE_OSI*/
	}	
	parm.preserveFaxMode=0;
#ifdef BUILD_RFC3611
	parm.rtcpXRMode=1;
#else
	parm.rtcpXRMode=0;
#endif
	parm.secHdrSize=0;
	parm.secHdrSize=0;
	parm.comfortNoise = 2;/*HAPI_GCNG_HOTHNOISE; */
	parm.callId = callId;
	parm.stream_id = stream_id;
	/** this will cause T38 failed due to codecType is mismatched
	* refer detailed info from http://172.23.56.144/trac/VoIPTeam/ticket/612
	* cindy modify 20100927 */
	if(ZYNOS_CODEC_T38 == codecType){
		parm.dataMode = EPDATAMODE_T38;
		VDSP_PRINTF("\n PhoneChanModify(): parm.dataMode = EPDATAMODE_T38; \n");
		parm.digitRelayType = 1; /*needed for fax*/
		parm.silence = 0;
		parm.echocancel = 1;
	}
	else{
		parm.dataMode = EPDATAMODE_VBD;		
		VDSP_PRINTF("\n PhoneChanModify(): parm.dataMode = EPDATAMODE_VBD;  \n");
	
		/**< set fax pt codec, cindy */
		if(codecNum == ZYNOS_CODEC_PCMA){
			/**< support VBD fax (pass through) by using G711a PCMA */
			preferredFaxPTCodec = EPVBDCODEC_PCMA;
			VDSP_PRINTF("%s(): vrgEndptProvSet(EPPROV_PreferredVBDCodec G711a PCMA) \n",__FUNCTION__);	
			if(vrgEndptProvSet(index, EPPROV_PreferredVBDCodec, &preferredFaxPTCodec, sizeof(preferredFaxPTCodec))) {
				VDSP_PRINTF("%s(): Set EPPROV_PreferredVBDCodec G711a PCMA failed \n",__FUNCTION__);	
			}
		}else if(codecNum == ZYNOS_CODEC_PCMU){
			/**< support VBD fax (pass through) by using G711u PCMU */
			preferredFaxPTCodec = EPVBDCODEC_PCMU;
			VDSP_PRINTF("%s(): vrgEndptProvSet(EPPROV_PreferredVBDCodec G711u PCMU) \n",__FUNCTION__);	
			if(vrgEndptProvSet(index, EPPROV_PreferredVBDCodec, &preferredFaxPTCodec, sizeof(preferredFaxPTCodec))) {
				VDSP_PRINTF("%s(): Set EPPROV_PreferredVBDCodec G711u PCMU failed \n",__FUNCTION__);	
			}
		}else{
			/**< not specify prefer codec, it is up to driver's xChgCodec */
			preferredFaxPTCodec = EPVBDCODEC_NONE;
			VDSP_PRINTF("%s(): codecNum is not G711 series \n",__FUNCTION__);	
			VDSP_PRINTF("%s(): vrgEndptProvSet(EPPROV_PreferredVBDCodec EPVBDCODEC_NONE ) \n",__FUNCTION__);	
			if(vrgEndptProvSet(index, EPPROV_PreferredVBDCodec, &preferredFaxPTCodec, sizeof(preferredFaxPTCodec))) {
				VDSP_PRINTF("%s(): Set EPPROV_PreferredVBDCodec EPVBDCODEC_NONE failed \n",__FUNCTION__);	
			}
		}	
	}
	/**< cindy change for T38 */
	
	VDSP_PRINTF("~~~~~~~~~BroadcomPhoneChanOpen~~~~~~~~~~~~~index = %d, cnxid = %d, codec = %d, codecNum = %d\n", index, cnxid, codecType, codecNum);
	VDSP_PRINTF("\t dtmf codec num = %d\n",dtmfCodecNum);
	VDSP_PRINTF("\t silence = %d\n",parm.silence);
	VDSP_PRINTF("\t echoCancel = %d\n",parm.echocancel);
	VDSP_PRINTF("\t digitRelay = %d\n",parm.digitRelayType);
	VDSP_PRINTF("\t mode = %d\n",parm.mode);
	VDSP_PRINTF("\t ptime = %d\n",ptime);
	rc = vrgEndptCreateConnection( &cmPhysEndpt[index].endptObjState,cnxid,&parm);
	if (rc)	{
	  VDSP_PRINTF("*** ERROR: Failed to create stream %d, err %d ***\n",cnxid, rc);
	  return (-1);
	}
	return(0);
}



int PhoneChanModify(int index,int codecType, int codecNum,int cnxid, int callId, int stream_id, int mode, int silenceSupp, int echoCancel, int digitRelay, int ptime, int dtmfCodecNum)

{
	/* Platform porting */
	/* END of Platform porting */
	EPZCNXPARAM parm;   
	int rc;
	EPZCAP cap;
	int preferredFaxPTCodec = EPVBDCODEC_PCMU; /**< cindy */
	cap=cmPhysEndpt[0].capabilities;

	if(mode == -1){
		mode = EPCNXMODE_SNDRX;
	}

	if(silenceSupp < 0){
		silenceSupp = 1;
	}

	if(echoCancel < 0){
		echoCancel = 1;
	}
	
	
	if(dtmfCodecNum == 0 && digitRelay == EPDTMFRFC2833_ENABLED){
		digitRelay = EPDTMFRFC2833_DISABLED;
		VDSP_PRINTF("RFC2833 not applicable, use InBand\n");
	}

	if(codecType == ZYNOS_CODEC_G729){
		digitRelay = EPDTMFRFC2833_ENABLED;
		if(dtmfCodecNum < 96){
			dtmfCodecNum = ZYNOS_CODEC_RFC2833;
		}
		VDSP_PRINTF("G729, driver restriction, set RFC2833, dtmfCodecNum = %d\n", dtmfCodecNum);
	}

	if(cap.pCap[0]&0x8000){
		if((ptime < (cap.pCap[0] & 0x7fff)) || (ptime > cap.pCap[1])){		
			ptime = 20;
		}
	}
	else{
		ptime = cap.pCap[0];
	}


	zyMemset( &parm, 0, sizeof(parm) );
	PhoneSetCodecByCodecNum(&(parm.cnxParmList.recv.codecs[0]), codecType, codecNum);
	PhoneSetCodecByCodecNum(&(parm.cnxParmList.send.codecs[0]), codecType, codecNum);

	parm.autoEncoder=1;
	parm.cnxParmList.recv.numCodecs=2;
	parm.cnxParmList.recv.codecs[0].options.opt1=0;
	parm.cnxParmList.recv.codecs[0].options.opt2=0;
	parm.cnxParmList.recv.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.recv.period[0]=ptime;
	parm.cnxParmList.recv.codecs[1].type=CODEC_NTE;
	if(dtmfCodecNum<96 || dtmfCodecNum>150){
		parm.cnxParmList.recv.codecs[1].rtpPayloadType=ZYNOS_CODEC_RFC2833;
	}
	else{
		parm.cnxParmList.recv.codecs[1].rtpPayloadType=dtmfCodecNum;
	}
	
	parm.cnxParmList.send.codecs[0].options.opt1=0;
	parm.cnxParmList.send.codecs[0].options.opt2=0;
	parm.cnxParmList.send.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.send.period[0]=ptime;
	
	if(digitRelay == EPDTMFRFC2833_ENABLED){
		if(dtmfCodecNum <96 && dtmfCodecNum > 150){
			dtmfCodecNum = 101; /*default value*/
		}
		parm.cnxParmList.send.numCodecs=2;
		parm.cnxParmList.send.codecs[1].type=CODEC_NTE;
		parm.cnxParmList.send.codecs[1].rtpPayloadType=dtmfCodecNum;		
	}
	else{
		parm.cnxParmList.send.numCodecs=1;
	}


	parm.t38param.ecMode=0;
	parm.t38param.maxBitRate=0;
	parm.t38param.maxRemDgrm=0;
	parm.t38param.numFaxPagesRx=0;
	parm.t38param.numFaxPagesTx=0;
	parm.vbdparam.vbdMode=0;
	parm.vbdparam.vbdRedMode=0;
	parm.digitRelayType=digitRelay;
	parm.echocancel= echoCancel;
	parm.silence=silenceSupp;
	parm.mode=mode;
	parm.namedPhoneEvts = 0;
	if(digitRelay == EPDTMFRFC2833_ENABLED){
		parm.namedPhoneEvts |= CODEC_NTE_DTMF; 			
		parm.namedPhoneEvts |= CODEC_NTE_LCS;/*CODEC_NTE_LCS = CODEC_NTE_RING, CODEC_NTE_ONHOOK, CODEC_NTE_OSI*/
	}
	parm.preserveFaxMode=0;
#ifdef BUILD_RFC3611
	parm.rtcpXRMode=1;
#else
	parm.rtcpXRMode=0;
#endif
	parm.secHdrSize=0;
	parm.secHdrSize=0;	
	parm.comfortNoise = 2;/*HAPI_GCNG_HOTHNOISE; */
	parm.callId = callId;
	parm.stream_id = stream_id;
	/** this will cause T38 failed due to codecType is mismatched
	* refer detailed info from http://172.23.56.144/trac/VoIPTeam/ticket/612
	* cindy modify 20100927 */
	if(ZYNOS_CODEC_T38 == codecType){
		parm.dataMode = EPDATAMODE_T38;
		VDSP_PRINTF("\n PhoneChanModify(): parm.dataMode = EPDATAMODE_T38; \n");
		parm.digitRelayType = 1; /*needed for fax*/
		parm.silence = 0;
		parm.echocancel = 1;
	}
	else{
		parm.dataMode = EPDATAMODE_VBD;		
		VDSP_PRINTF("\n PhoneChanModify(): parm.dataMode = EPDATAMODE_VBD;  \n");
	
		/**< set fax pt codec, cindy */
		if(codecNum == ZYNOS_CODEC_PCMA){
			/**< support VBD fax (pass through) by using G711a PCMA */
			preferredFaxPTCodec = EPVBDCODEC_PCMA;
			VDSP_PRINTF("%s(): vrgEndptProvSet(EPPROV_PreferredVBDCodec G711a PCMA) \n",__FUNCTION__);	
			if(vrgEndptProvSet(index, EPPROV_PreferredVBDCodec, &preferredFaxPTCodec, sizeof(preferredFaxPTCodec))) {
				VDSP_PRINTF("%s(): Set EPPROV_PreferredVBDCodec G711a PCMA failed \n",__FUNCTION__);	
			}
		}else if(codecNum == ZYNOS_CODEC_PCMU){
			/**< support VBD fax (pass through) by using G711u PCMU */
			preferredFaxPTCodec = EPVBDCODEC_PCMU;
			VDSP_PRINTF("%s(): vrgEndptProvSet(EPPROV_PreferredVBDCodec G711u PCMU) \n",__FUNCTION__);	
			if(vrgEndptProvSet(index, EPPROV_PreferredVBDCodec, &preferredFaxPTCodec, sizeof(preferredFaxPTCodec))) {
				VDSP_PRINTF("%s(): Set EPPROV_PreferredVBDCodec G711u PCMU failed \n",__FUNCTION__);	
			}
		}else{
			/**< not specify prefer codec, it is up to driver's xChgCodec */
			preferredFaxPTCodec = EPVBDCODEC_NONE;
			VDSP_PRINTF("%s(): codecNum is not G711 series \n",__FUNCTION__);	
			VDSP_PRINTF("%s(): vrgEndptProvSet(EPPROV_PreferredVBDCodec EPVBDCODEC_NONE ) \n",__FUNCTION__);	
			if(vrgEndptProvSet(index, EPPROV_PreferredVBDCodec, &preferredFaxPTCodec, sizeof(preferredFaxPTCodec))) {
				VDSP_PRINTF("%s(): Set EPPROV_PreferredVBDCodec EPVBDCODEC_NONE failed \n",__FUNCTION__);	
			}
		}	
	}
	/**< cindy change for T38 */
	VDSP_PRINTF("~~~~~~~~~BroadcomPhoneChanModify~~~~~~~~~~~~~index = %d, cnxid = %d, codec = %d, codecNum = %d\n", index, cnxid, codecType, codecNum);
	VDSP_PRINTF("\t dtmf codec num = %d\n",dtmfCodecNum);
	VDSP_PRINTF("\t silence = %d\n",parm.silence);
	VDSP_PRINTF("\t echoCancel = %d\n",parm.echocancel);
	VDSP_PRINTF("\t digitRelay = %d\n",parm.digitRelayType);
	VDSP_PRINTF("\t mode = %d\n",parm.mode);
	VDSP_PRINTF("\t ptime = %d\n",ptime);
	rc=vrgEndptModifyConnection( &cmPhysEndpt[index].endptObjState,cnxid,&parm);
	if (rc){
	  VDSP_PRINTF("*** ERROR: Failed to Modify stream %d, err %d ***\n",cnxid, rc);
	  return (-1);
	}
	return (0);
}



int PhoneChanClose(int index,int cnxid)
{
	/* Platform porting */
	/* END of Platform porting */
   int rc;

   if(index>MAX_ENDPT){ //ZyPhone not need to close dsp chan
   	return 0;
   }
   VDSP_PRINTF("~~~~~~~~~BroadcomPhoneChanClose~~~~~~~~~~~~~index=%d, cnxid=%d\n", index, cnxid);
   
   rc = vrgEndptDeleteConnection( &cmPhysEndpt[index].endptObjState,cnxid);
      
   if (rc) {
      VDSP_PRINTF("*** ERROR: Failed to close stream %d, err %d ***\n",cnxid, rc);
      return (-1);
   }
   VDSP_PRINTF("*** BroadcomPhoneChanClose END**\n");

	return (0);
}


int PhoneChanModifyForFaxRelay(int index,int cnxid)
{
	/* Platform porting */
	/* END of Platform porting */
	EPZCNXPARAM parm;     
	int rc;
	 if(index>MAX_ENDPT || cnxid >MAX_CNX){ //ZyPhone not need to open dsp chan
	 	return -1;
	 }
	zyMemset( &parm, 0, sizeof(parm) );
   
	parm.cnxParmList.recv.codecs[0].type=CODEC_PCMU;
	parm.cnxParmList.recv.codecs[0].rtpPayloadType=RTP_PAYLOAD_PCMU;
	parm.cnxParmList.send.codecs[0].type=CODEC_PCMU;
	parm.cnxParmList.send.codecs[0].rtpPayloadType=RTP_PAYLOAD_PCMU;
	
	
	parm.autoEncoder=0;
	parm.cnxParmList.recv.numCodecs=1;
	parm.cnxParmList.recv.codecs[0].options.opt1=0;
	parm.cnxParmList.recv.codecs[0].options.opt2=0;
	parm.cnxParmList.recv.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.recv.period[0]=20;
	parm.cnxParmList.send.numCodecs=1;
	parm.cnxParmList.send.codecs[0].options.opt1=0;
	parm.cnxParmList.send.codecs[0].options.opt2=0;
	parm.cnxParmList.send.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.send.period[0]=20;
	parm.digitRelayType=DTMF_RELAY_RFC2833_DISABLED; //DTMF_RELAY_RFC2833_DISABLED	
	parm.echocancel= 1;
	parm.mode=EPCNXMODE_SNDRX;
	parm.namedPhoneEvts=0;
	parm.preserveFaxMode=0;
#ifdef BUILD_RFC3611
	parm.rtcpXRMode=1;
#else
	parm.rtcpXRMode=0;
#endif
	parm.secHdrSize=0;
	parm.secHdrSize=0;
	parm.silence=0;
	parm.comfortNoise = 2;/*HAPI_GCNG_HOTHNOISE; */
	parm.t38param.ecMode=0;
	parm.t38param.maxBitRate=0;
	parm.t38param.maxRemDgrm=0;
	parm.t38param.numFaxPagesRx=0;
	parm.t38param.numFaxPagesTx=0;
	parm.dataMode = EPDATAMODE_VBD; /*test*/
	parm.vbdparam.vbdMode=0;
	parm.vbdparam.vbdRedMode=0;
	VDSP_PRINTF("~~~~~~~~~BroadcomPhoneChanModifyForFaxRelay~~~~~~~~~~~~~cnxid=%d \n",cnxid);
	rc = vrgEndptModifyConnection( &cmPhysEndpt[index].endptObjState,cnxid,&parm);
	if (rc){
	  VDSP_PRINTF("*** ERROR: Failed to create stream %d, err %d ***\n",cnxid, rc);
	  return (-1);
	}

	return (0);
}

int PhoneChanModifyForFaxT38(int index,int cnxid)
{
	/* Platform porting */
	/* END of Platform porting */

	EPZCNXPARAM parm;     
	int rc;
	 if(index>MAX_ENDPT || cnxid >MAX_CNX){ //ZyPhone not need to open dsp chan
	 	return -1;
	 }
	zyMemset( &parm, 0, sizeof(parm) );
   
	parm.cnxParmList.recv.codecs[0].type=CODEC_T38;
	parm.cnxParmList.recv.codecs[0].rtpPayloadType=RTP_PAYLOAD_PCMU;
	parm.cnxParmList.send.codecs[0].type=CODEC_T38;
	parm.cnxParmList.send.codecs[0].rtpPayloadType=RTP_PAYLOAD_PCMU;
	
	
	parm.autoEncoder=0;
	parm.cnxParmList.recv.numCodecs=1;
	parm.cnxParmList.recv.codecs[0].options.opt1=0;
	parm.cnxParmList.recv.codecs[0].options.opt2=0;
	parm.cnxParmList.recv.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.recv.period[0]=20;
	parm.cnxParmList.send.numCodecs=1;
	parm.cnxParmList.send.codecs[0].options.opt1=0;
	parm.cnxParmList.send.codecs[0].options.opt2=0;
	parm.cnxParmList.send.codecs[0].options.rfc2198Type=0;
	parm.cnxParmList.send.period[0]=20;
	parm.digitRelayType=DTMF_RELAY_RFC2833_DISABLED; //DTMF_RELAY_RFC2833_DISABLED	
	parm.echocancel= 1;
	parm.mode=EPCNXMODE_SNDRX;
	parm.namedPhoneEvts=0;
	parm.preserveFaxMode=0;
#ifdef BUILD_RFC3611
	parm.rtcpXRMode=1;
#else
	parm.rtcpXRMode=0;
#endif
	parm.secHdrSize=0;
	parm.secHdrSize=0;
	parm.silence=0;
	parm.comfortNoise = 2; /*test*/
	parm.t38param.ecMode=0;
	parm.t38param.maxBitRate=0;
	parm.t38param.maxRemDgrm=0;
	parm.t38param.numFaxPagesRx=0;
	parm.t38param.numFaxPagesTx=0;
	parm.dataMode = EPDATAMODE_T38; /*test*/
	parm.vbdparam.vbdMode=0;
	parm.vbdparam.vbdRedMode=0;
	VDSP_PRINTF("~~~~~~~~~BroadcomPhoneChanModifyForFaxRelay~~~~~~~~~~~~~cnxid=%d \n",cnxid);
	rc = vrgEndptModifyConnection( &cmPhysEndpt[index].endptObjState,cnxid,&parm);
	if (rc){
	  VDSP_PRINTF("*** ERROR: Failed to create stream %d, err %d ***\n",cnxid, rc);
	  return (-1);
	}

	return (0);
}

int PhoneGenHowlerTone(
     int index, int cnx
){

 	/* Platform porting */
	/* END of Platform porting */
	return (0);
}


void TestPktRecvCb( ENDPT_STATE *endptState, int streamId, EPPACKET *epPacketp, int length )
{
	/* Platform porting */
	/* END of Platform porting */	
	dspChan_t *dspChan_p = NULL;			
	dspPoolAcquireMutex();
	dspChan_p=dspPoolFindDspChanByChanId(0, streamId); /*cpu0 */	
	if (EPMEDIATYPE_RTCP == epPacketp->mediaType)
	{		
		if(NULL != dspChan_p) {
			if(NULL != dspChan_p->voipDspRtcpCallBack){	
				dspChan_p->voipDspRtcpCallBack(dspChan_p->call_id,
												dspChan_p->stream_id,
												dspChan_p->stream_type,
												dspChan_p->phLogicalId,
												epPacketp->mediaType, 
												epPacketp->packetp, 
												length, 
												0, 
												NULL);/*rtcp_send*/
			}
			else{			
				//VDSP_PRINTF("voipDspRtcpCallBack = NULL \n");
			}
		}
		else{
			//VDSP_PRINTF("(%s)dspChan_p NULL\n", __FUNCTION__);
		}
	}
	else 
	{

		if(NULL != dspChan_p) {
			if(NULL != dspChan_p->voipDspCallBack){			
				dspChan_p->voipDspCallBack(dspChan_p->call_id,
											dspChan_p->stream_id,
											dspChan_p->stream_type,
											dspChan_p->phLogicalId,
											epPacketp->mediaType, 
											epPacketp->packetp, 
											length, 
											0, 
											NULL);/*rtp_send*/
			}
			else{			
				//VDSP_PRINTF("voipDspCallBack = NULL \n");
			}		
		}
		else{
			//VDSP_PRINTF("(%s)dspChan_p NULL\n", __FUNCTION__);
		}
	}
	dspPoolReleaseMutex();	
}


int
RtpSendToDsp(
	icf_call_id_t	call_id,
	icf_stream_id_t  	stream_id,
	icf_media_type_t	stream_type,
	uint32 null_pload_type, 
	uint8 *InMessage, 
	uint16 MessageLen, 
	uint32 TimeStamp, 
	void* handle)
{
	/* Platform porting */
	/* END of Platform porting */
	EPPACKET epPacketp;	
	dspChan_t *dspChan_p = NULL;	
#if SRTP_SUPPORT
	rtpTable_t *table =NULL;
	int err = 0;
#endif
	dspPoolAcquireMutex();	
	dspChan_p=dspPoolFindDspChanById(call_id, stream_id, stream_type);
	if ( RTPPACKET_IS_VALID_VERSION( (RTPPACKET *) InMessage ) )
	{
	  /* The first 2 bits contains the RTP version.  We will assume
	   * this is a RTP packet for now.  More error checking will be
	   * done later
	  */
	  epPacketp.mediaType = EPMEDIATYPE_RTP;
#if SRTP_SUPPORT
		if( (table = rtpTableGetById(call_id, stream_id, stream_type)) != NULL ) {
			if(NULL != table->srtpRecvSession){
				int length = MessageLen;			
				err = srtp_unprotect(table->srtpRecvSession, InMessage, &length);
				if(err != 0){
					printf("srtp_unprotect ret = %d\n", err);
				}
				MessageLen = length;			
			}
		}
#endif
	}
	else
	{
	  epPacketp.mediaType = EPMEDIATYPE_T38;
	}
	
	
	epPacketp.packetp = InMessage;
	
	if(NULL != dspChan_p && CHAN_OPENED == dspChan_p->chanState && dspChan_p->phPhysicalId != ID_NOT_USED /*&& (dspChan_p->stream_id==stream_id) && (dspChan_p->stream_type == stream_type)*/){						
		if(vrgEndptPacket( &cmPhysEndpt[dspChan_p->phPhysicalId].endptObjState, dspChan_p->chanId, &epPacketp, MessageLen, 0 )!=0){
			VDSP_PRINTF("vrgEndptPacket fail, phy_id %d, cnx %d\n",dspChan_p->phPhysicalId, dspChan_p->chanId);
		}
		
	}
	else{
		VDSP_PRINTF("MM: %s :dsp chan not found, call_id = %d, stream_id = %d, stream_type = %d\n",__FUNCTION__, call_id, stream_id, stream_type);
	}
	dspPoolReleaseMutex();	
	return 0;
}

int
RtcpSendToDsp(
icf_call_id_t	call_id,
icf_stream_id_t  	stream_id,
icf_media_type_t	stream_type,
uint32 null_pload_type, 
uint8 *InMessage, 
uint16 MessageLen, 
uint32 TimeStamp, 
void* handle
)
{
#if SRTP_SUPPORT
		rtpTable_t *table =NULL;
		int err = 0;
#endif

	/* Platform porting */
	/* END of Platform porting */
	EPPACKET epPacketp;
	dspChan_t *dspChan_p=NULL;	
	dspPoolAcquireMutex();
	dspChan_p=dspPoolFindDspChanById(call_id, stream_id, stream_type);
	epPacketp.mediaType = EPMEDIATYPE_RTCP;
	epPacketp.packetp = InMessage;
	
	if(NULL != dspChan_p && CHAN_OPENED == dspChan_p->chanState && dspChan_p->phPhysicalId != ID_NOT_USED ){				
		#if SRTP_SUPPORT
		if( (table = rtpTableGetById(call_id, stream_id, stream_type)) != NULL ) {
			if(NULL != table->srtpRecvSession){
				int length = MessageLen;			
				err = srtp_unprotect_rtcp(table->srtpRecvSession, InMessage, &length);
				if(err != 0){
					printf("srtp_unprotect ret = %d\n", err);
				}
				MessageLen = length;			
			}
		}
		#endif
		
		if(vrgEndptPacket( &cmPhysEndpt[dspChan_p->phPhysicalId].endptObjState, dspChan_p->chanId, &epPacketp, MessageLen, 0 )!=0){
			VDSP_PRINTF("vrgEndptPacket fail, phy_id %d, cnx %d\n",dspChan_p->phPhysicalId, dspChan_p->chanId);
		}
		else{
			//VDSP_PRINTF("RTCP SendToDsp: dsp chan find  NULL, call_id %d\n", dspChan_p->call_id);
		}
	}
	else{
		VDSP_PRINTF("MM: %s :dsp chan not found, call_id = %d\n",__FUNCTION__, call_id);
	}
	dspPoolReleaseMutex();
	return 0;
}



void dspSendToDsp(
		icf_call_id_t call_id, 
		icf_stream_id_t stream_id, 
		icf_media_type_t	stream_type,
		uint16 logicalId,
		icf_uint32_t pload_type, 
		icf_uint8_t *packet, 
		icf_uint16_t len, 
		icf_uint32_t timestamp, 
		void* handle)

{
	/* Platform porting */
	/* END of Platform porting */
	EPPACKET epPacketp;
	dspChan_t *dspChan_p = NULL, *p_peer_dsp_chan = NULL;	

	dspChan_p = dspPoolFindDspChanByLogicalId(logicalId);
	
	if(NULL != dspChan_p){
		p_peer_dsp_chan = dspPoolFindAnotherDspChanByCallId(dspChan_p->call_id, logicalId);

		if(NULL != p_peer_dsp_chan){
			epPacketp.mediaType = EPMEDIATYPE_RTP;
			epPacketp.packetp = packet;
			vrgEndptPacket( &cmPhysEndpt[p_peer_dsp_chan->phPhysicalId].endptObjState, p_peer_dsp_chan->chanId, &epPacketp, len, 0 );
		}
		else{
			VDSP_PRINTF("dsp chan find  NULL, call_id %d\n", dspChan_p->call_id);
		}
	}
	else{
		VDSP_PRINTF("dsp chan find  NULL, call_id %d\n", logicalId);
	}	
	
	return 0;
}


void GetRtpStatus(uint16 physicalId, int cnxid){
	/* Platform porting */
	/* END of Platform porting */
	int rc = -1;
	EPCMD_PARMS parm;
	parm.arg[0]=cnxid;
	VDSP_PRINTF("\n\n");
	
	VDSP_PRINTF("EPCMD_RTP_STATS>>\n");
	rc=vrgEndptConsoleCmd( &cmPhysEndpt[physicalId].endptObjState,EPCMD_RTP_STATS, &parm);		
}

void GetVhdStatus(uint16 physicalId, uint16 vhdHdl){
	/* Platform porting */
	/* END of Platform porting */
	int rc = -1;
	EPCMD_PARMS parm;	
	
	VDSP_PRINTF("EPCMD_PRINT_VHDS>>\n");
	rc=vrgEndptConsoleCmd( &cmPhysEndpt[physicalId].endptObjState,EPCMD_PRINT_VHDS, &parm);

	parm.arg[0]=vhdHdl;
	VDSP_PRINTF("EPCMD_VHD_STATS_FROM_CNX>>\n");
	rc=vrgEndptConsoleCmd( &cmPhysEndpt[physicalId].endptObjState,EPCMD_VHD_STATS_FROM_CNX, &parm);	
	
	VDSP_PRINTF("EPCMD_VHD_STATS>>\n");
	rc=vrgEndptConsoleCmd( &cmPhysEndpt[physicalId].endptObjState,EPCMD_VHD_STATS, &parm);	
	
	VDSP_PRINTF("EPCMD_VHD_JBREGS>>\n");
	rc=vrgEndptConsoleCmd( &cmPhysEndpt[physicalId].endptObjState,EPCMD_VHD_JBREGS, &parm);	
	
	VDSP_PRINTF("EPCMD_FAXR_STATS>>\n");
	rc=vrgEndptConsoleCmd( &cmPhysEndpt[physicalId].endptObjState,EPCMD_FAXR_STATS, &parm);	
	
	
}



int PhoneDeinit(void)
{	
	if(isEndptInitialized()){
		printf("%s, deinit DSP..., vrgEndptDeinit()\n",__FUNCTION__);
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
				pthread_mutex_lock(&mutexDspDeinit);
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("mm.exe %d: Signal %d received, TERMINATING mm.exe \n",getpid(), sig);
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			//PhoneDeinit();			
			pthread_mutex_unlock(&mutexDspDeinit);
			//sleep(5);
			exit(0);
			break;
		default:
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("mm.exe: Signal %d received\n", sig);
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			break;
			
	}
}



void AppRegisterSignal(void){
	struct sigaction sa;

   sa.sa_sigaction = AppSignalHandler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;
   printf("%s, mm pid = %d\n",__FUNCTION__, getpid());
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
   sigaction( SIGHUP ,  &sa,  NULL );     /* Hangup (POSIX).  */
   sigaction( SIGSEGV,  &sa,  NULL );     /* Segmentation violation (ANSI).  */   
   sigaction( SIGTERM,  &sa,  NULL );     /* Termination (ANSI).  */

}

/* aircheng add for statisfics */

int PhoneGetRtpStatistics(int chanId, int phyId, int *tx_rtp_pkt_cnt, int * tx_rtp_octet_cnt, int * rx_rtp_pkt_cnt, int * rx_rtp_octet_cnt, int * lost, int *jitter){
	VDSP_PRINTF("entering %s\n", __FUNCTION__);
	VDSP_PRINTF("phyId = %d, chanId= %d\n", phyId, chanId);
	
	vrgEndptGetRtpStatistics(chanId, phyId, tx_rtp_pkt_cnt, tx_rtp_octet_cnt, rx_rtp_pkt_cnt, rx_rtp_octet_cnt, lost, jitter);
	return 0;
}


