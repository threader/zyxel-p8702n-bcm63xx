#include <stdio.h>
#include "endpt.h"
#include "Endpoint_itf.h"
#include "mm_macros.h"
#include "mm_core_fsm_types.h"
#include "voice_dsp_core_fsm_types.h"
#include "global.h"
#include "icf_types.h"
#include "dsp_driver_itf.h"
#include "tr104_object.h"
#include "switch.h"
extern CMPHYSENDPT cmPhysEndpt[MAX_ENDPT];
static bool gIsDspInitialized = FALSE;
extern uint8 mmDspDebug;
#if KSOCKET_RTP_SUPPORT
extern int mmKSocketEnable;
#endif
#if 1  /*Jason , rtp static*/
extern SipLineStats_t sipLineStats[2];
#endif


/******************************************************************************
 * dspDriverInit
 *
 * Description: 
 *     dsp driver interface for upper application, calls dsp driver init function
 *	 called when VOICE_DSP_INIT request is received from phfsm, 
 * Parameters:
 *  
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure.
 *    	MM_SUCCESS  = If success. 
 *****************************************************************************/

mm_return_t dspDriverInit(int country){
	int ret=-1;

	/*driver endpoint init*/
	if(!isEndptInitialized()){
		ret=PhoneInit(country);
	}
	else{
		ZyPrintf("dsp driver already initialized\n");
		ret = 0;
	}
	
	if(ret==0){
		gIsDspInitialized = TRUE;
		return MM_SUCCESS;
	}
	else{
		return MM_FAILURE;	
	}
}

/******************************************************************************
 * dspDriverGetInitData
 *
 * Description: 
 *     dsp driver interface for upper application, calls dsp driver init function
 *	 called when VOICE_DSP_INIT request is received from phfsm, 
 * Parameters:
 *	Output
 *    	uint8 **initData - init data for phfsm to control dsp driver
 *    	uint16 *data_length - data length    
 *  
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure.
 *    	MM_SUCCESS  = If success. 
 *****************************************************************************/

void dspDriverGetInitData(uint8 **initData, uint16 *data_length){
	/*copy necessary data for voiceApp to control endpoint driver*/
	*data_length=sizeof(cmPhysEndpt);
	*initData=zyMalloc(*data_length);
	zyMemcpy(*initData,&cmPhysEndpt,*data_length);
}
#if 0
/******************************************************************************
 * dsp_driver_get_capabilities
 *
 * Description: 
 *     dsp driver interface for upper application, get dsp capabilities (support codec, ptime, silence suppression), called by MM
 *
 * Parameters:
 *    	Output
 *    	icf_cfg_codec_info_req_st *p_cfg_codec_info_req - fill supported codec in codec_supp  
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dsp_driver_get_capabilities(icf_cfg_codec_info_req_st *p_cfg_codec_info_req){
	icf_config_codec_attrib_st *p_config_codec_attrib = NULL;
	int i = 0, count = 0;
	EPZCAP cap;	
	icf_list_st *p_encoded_params = ICF_NULL;
	icf_string_st *str_encoded_param;
	
	cap=cmPhysEndpt[0].capabilities;
	
	
	for(i=0;i<CODEC_MAX_TYPES;i++){
		switch(i){
			case CODEC_PCMU:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("PCMU supported\n");
					p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[count];
					p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
					zyStrcpy(p_config_codec_attrib->codec_name.str,"PCMU");
					p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

					p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
					p_config_codec_attrib->codec_num = ZYNOS_CODEC_PCMU;

					p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime=2;
					p_config_codec_attrib->codec_param.audio_param.ptime[0]=cap.pCap[0];
					p_config_codec_attrib->codec_param.audio_param.ptime[1]=cap.pCap[1];
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
					p_config_codec_attrib->codec_param.audio_param.silence_suppression=cap.sCap;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
					p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
					p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;
	
					p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
					p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));

					p_encoded_params = p_config_codec_attrib->p_encoded_params;					
					p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
					
					str_encoded_param = (icf_string_st *)p_encoded_params->p_data;
					zyStrcpy(str_encoded_param->str, "PCMU");
					str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
					p_encoded_params->p_next = NULL;

					count++;

				}
				break;
			case CODEC_PCMA:			
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("PCMA supported\n");
					p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[count];				
					p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
					zyStrcpy(p_config_codec_attrib->codec_name.str,"PCMA");
					p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

					p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
					p_config_codec_attrib->codec_num = ZYNOS_CODEC_PCMA;

					p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime=2;
					p_config_codec_attrib->codec_param.audio_param.ptime[0]=cap.pCap[0];
					p_config_codec_attrib->codec_param.audio_param.ptime[1]=cap.pCap[1];
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
					p_config_codec_attrib->codec_param.audio_param.silence_suppression=cap.sCap;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
					p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
					p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;
					
					p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
					p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));
					
					p_encoded_params = p_config_codec_attrib->p_encoded_params;					
					p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));					
					str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
					zyStrcpy(str_encoded_param->str, "PCMA");
					str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
					p_encoded_params->p_next = NULL;
					count++;
				}			
				break;
			case CODEC_G7231A_53:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G723_53 supported\n");
				}
				break;
			case CODEC_G7231A_63:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G723_63 supported\n");
				}
				break;
			case CODEC_G726_16:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G726_16 supported\n");
				}
				break;
			case CODEC_G726_24:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G726_24 supported\n");
				}
				break;
			case CODEC_G726_32:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G726_32 supported\n");
				}
				break;
			case CODEC_G726_40:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G726_40 supported\n");
				}
				break;
			case CODEC_G729:				
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G729 supported\n");
					p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[count];					
					p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
					zyStrcpy(p_config_codec_attrib->codec_name.str,"G729");
					p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

					p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
					p_config_codec_attrib->codec_num = ZYNOS_CODEC_G729;

					p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_PTIME;
					p_config_codec_attrib->codec_param.audio_param.num_of_supported_ptime=2;
					p_config_codec_attrib->codec_param.audio_param.ptime[0]=cap.pCap[0];
					p_config_codec_attrib->codec_param.audio_param.ptime[1]=cap.pCap[1];
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SILENCE_SUPPRESSION;
					p_config_codec_attrib->codec_param.audio_param.silence_suppression=cap.sCap;
					p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
					p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
					p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;
					
					p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
					p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));

					p_encoded_params = p_config_codec_attrib->p_encoded_params;					
					p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));					
					str_encoded_param = (icf_string_st*)p_encoded_params->p_data;
					zyStrcpy(str_encoded_param->str, "G729");
					str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
					p_encoded_params->p_next = NULL;

					count++;
				}				
				break;
			case CODEC_G729E:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("G729E supported\n");
				}
				break;
			case CODEC_ILBC_20:
				if((cap.codecCap[i]) && (count < ICF_MAX_CODEC_SUPPORTD)){
					ZyPrintf("ILBC 20ms supported\n");
				}
				break;	
			default:
				//ZyPrintf("MM:%s:Capabilities of [%d]= %d\n",__FUNCTION__,i,cap.codecCap[i]);
				break;
				
		}
	}
	
	
	if(count < ICF_MAX_CODEC_SUPPORTD)
	{
		ZyPrintf("telephone-event supported\n");
		p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[count];
		p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_AUDIO;
		zyStrcpy(p_config_codec_attrib->codec_name.str,"telephone-event");
		p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

		p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
		p_config_codec_attrib->codec_num = ZYNOS_CODEC_RFC2833;

		p_config_codec_attrib->bitmask |= ICF_CODEC_PARAM_PRESENT;
		p_config_codec_attrib->codec_param.audio_param.bit_mask |= ICF_AUDIO_CODEC_SAMPLING_RATE;
		p_config_codec_attrib->codec_param.audio_param.num_of_supp_sampling_rate = 1;
		p_config_codec_attrib->codec_param.audio_param.sampling_rate[0] = 8000;
					
		p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
		p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));

		p_encoded_params = p_config_codec_attrib->p_encoded_params;					
		p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
		
		str_encoded_param = (icf_string_st *)p_encoded_params->p_data;
		zyStrcpy(str_encoded_param->str, "telephone-event");
		str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
		p_encoded_params->p_next = NULL;

		count++;

	}

	

	if(count < ICF_MAX_CODEC_SUPPORTD)
	{
		ZyPrintf("T.38 supported\n");
		p_config_codec_attrib=&p_cfg_codec_info_req->codec_supp[count];
		p_config_codec_attrib->codec_type = ICF_CODEC_TYPE_IMAGE;
		zyStrcpy(p_config_codec_attrib->codec_name.str,"t38");
		p_config_codec_attrib->codec_name.str_len=zyStrlen(p_config_codec_attrib->codec_name.str);

		p_config_codec_attrib->bitmask |= ICF_CODEC_NUM_PRESENT;
		p_config_codec_attrib->codec_num = ZYNOS_CODEC_T38;
				
		p_config_codec_attrib->bitmask |= ICF_ENCODED_PARAM_PRESENT;
		p_config_codec_attrib->p_encoded_params = zyMalloc(sizeof(icf_list_st));

		p_encoded_params = p_config_codec_attrib->p_encoded_params;					
		p_encoded_params->p_data = zyMalloc(sizeof(icf_string_st));
		
		str_encoded_param = (icf_string_st *)p_encoded_params->p_data;
		zyStrcpy(str_encoded_param->str, "t38");
		str_encoded_param->str_len = zyStrlen(str_encoded_param->str);
		p_encoded_params->p_next = NULL;

		count++;
	}


	p_cfg_codec_info_req->count=count;
	return MM_SUCCESS;
	
}
#endif
/******************************************************************************
 * dspDriverGetDtmfMethod
 *
 * Description: 
 *     Method by which DTMF type MUST be passed.
 * Enumeration of:
 * ¡§InBand¡¨
 * ¡§RFC2833¡¨
 * ¡§SIPInfo¡¨
 * If the parameter DTMFMethodG711 is non-empty, then this parameter applies only when the current codec is not G.711. 
 * The value ¡§SIPInfo¡¨ is applicable only if the SignalingProtocol is SIP. This parameter is applicable only for a VoIP endpoint
 *
 * Parameters:
 *  
 * Return Value:
 *****************************************************************************/

uint8 dspDriverGetDtmfMethod(dspChan_t *dspChan_p){
	if(NULL == dspChan_p){
		VDSP_PRINTF("%s, dspChan_p NULL, return RFC2833\n",__FUNCTION__);
		return TR104I_DTMF_RFC2833;
	}
	if((dspChan_p->dtmfMethodG711 != TR104I_DTMF_NULL) 
		&& ((dspChan_p->codec_num == ZYNOS_CODEC_PCMU) 
			|| (dspChan_p->codec_num == ZYNOS_CODEC_PCMA))){
		return dspChan_p->dtmfMethodG711;	
	}
	else{
		return dspChan_p->dtmfMethod;
	}
}

/******************************************************************************
 * dspDriverChanOpen
 *
 * Description: 
 *     dsp driver interface for upper application, dsp channel open function, called by MM
 *
 * Parameters:
 * 	Input
 *    dspChan_t *dsp_chan - specify dsp channel to open
 *	int mode -specify which connection mode to use
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverChanOpen(dspChan_t *dspChan_p, int mode){
	int ret = -1;
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}

	if(dspChan_p == NULL) {
		return MM_FAILURE;
	}

	

	VDSP_PRINTF(" DSP DRIVER > %s : endpt %d, cnx %d, data type %d \n",__FUNCTION__, dspChan_p->phPhysicalId, dspChan_p->chanId, dspChan_p->codec_num);
	ret=PhoneChanOpen(dspChan_p->phPhysicalId, dspChan_p->codec_type, dspChan_p->codec_num, dspChan_p->chanId, dspChan_p->call_id, dspChan_p->stream_id, mode, dspChan_p->silenceSupp, dspChan_p->echoCancel, dspDriverGetDigitRelay(dspChan_p), dspChan_p->ptime, dspChan_p->dtmfCodecNum);
	
	if(ret == 0)
		return MM_SUCCESS;
	else
		return MM_FAILURE;
}

int dspDriverGetDigitRelay(dspChan_t *dspChan_p){
	int dtmfMethod = dspDriverGetDtmfMethod(dspChan_p);
	int digitRelay = 0;
	VDSP_PRINTF("DTMF Method = %d\n", dtmfMethod);
	switch(dtmfMethod){		
		case TR104I_DTMF_RFC2833:
			VDSP_PRINTF("RFC2833\n");
			digitRelay = 0;
			break;
		case TR104I_DTMF_SIPINFO:
			VDSP_PRINTF("SIPINFO\n");
			digitRelay = 2;
			break;
		case TR104I_DTMF_INBAND:
		default:
			VDSP_PRINTF("INBAND\n");
			digitRelay = 1;
			break;
	}

	return digitRelay;
}
/******************************************************************************
 * dspDriverChanModify
 *
 * Description: 
 *     dsp driver interface for upper application, dsp channel modify function, called by MM
 *
 * Parameters:
 * 	Input
 *    dspChan_t *dsp_chan - specify dsp channel to modify
 *	int mode -specify which connection mode to use
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverChanModify(dspChan_t *dspChan_p, int mode){
	int ret = -1;
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}

	if(dspChan_p == NULL){
		return MM_FAILURE;	
	}

	
	VDSP_PRINTF(" DSP DRIVER > %s : endpt %d, cnx %d, data type %d\n",__FUNCTION__, dspChan_p->phPhysicalId, dspChan_p->chanId, dspChan_p->codec_num);
	ret=PhoneChanModify(dspChan_p->phPhysicalId, dspChan_p->codec_type, dspChan_p->codec_num, dspChan_p->chanId, dspChan_p->call_id, dspChan_p->stream_id, mode, dspChan_p->silenceSupp, dspChan_p->echoCancel, dspDriverGetDigitRelay(dspChan_p), dspChan_p->ptime, dspChan_p->dtmfCodecNum);

	if(ret == 0)
		return MM_SUCCESS;
	else
		return MM_FAILURE;
}

/******************************************************************************
 * dspDriverChanClose
 *
 * Description: 
 *     dsp driver interface for upper application, dsp channel close function, called by MM
 *
 * Parameters:
 * 	Input
 *    dspChan_t *dsp_chan - specify dsp channel to close
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverChanClose(dspChan_t *dspChan_p){
	int ret = 0;
#if KSOCKET_RTP_SUPPORT
	rtpTable_t *table = NULL;
	uint32 lost, jitter;
#endif
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	
#if KSOCKET_RTP_SUPPORT /*KSOCKET_RTP_SUPPORT, SYSLOG - RTP Statistics*/
	if(mmKSocketEnable){
		if(dspChan_p != NULL){
			if( (table = rtpTableGetById(dspChan_p->call_id, dspChan_p->stream_id, dspChan_p->stream_type)) == MM_NULL ){
				VDSP_PRINTF("%s, cannot find rtp table", __FUNCTION__);
			}	

			if(table != NULL){
				//dspDriverKSocketDeleteSession(dspChan_p, table, &lost, &jitter);
				dspDriverGetRtpStatistics(dspChan_p, table, &lost, &jitter);
				sipLineStats[table->line_id].pkt_lost = lost;
				sipLineStats[table->line_id].avgjitter= jitter;
			}
		}
	}
#endif
	
	ret=PhoneChanClose(dspChan_p->phPhysicalId, dspChan_p->chanId);

	if(ret==0)
		return MM_SUCCESS;
	else
		return MM_FAILURE;
}

#if KSOCKET_RTP_SUPPORT

mm_return_t dspDriverKSocketMakeSession(int call_id, int stream_id,uint32 saddr, uint16 sport, uint32 daddr, uint16 dport, uint8 mediaMode, uint8 qosMark, char *ifName){
	int ret = 0;
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	VDSP_PRINTF("%s, callId = %d, stream_id = %d\n", __FUNCTION__, call_id, stream_id);
	VDSP_PRINTF("saddr = 0x%x, sport = %d\n", saddr, sport);
	VDSP_PRINTF("daddr = 0x%x, dport = %d\n", daddr, dport);
	VDSP_PRINTF("mediaMode = %d, qosMark = %d\n", mediaMode, qosMark);
	PhoneKSoketMakeSession(0/*don't care*/, 0/*don't care*/, call_id, stream_id ,saddr, sport, daddr, dport, mediaMode, qosMark, ifName);
	
	return MM_SUCCESS;
	
}

mm_return_t dspDriverKSocketDeleteSession(int call_id, rtpTable_t *table,uint32 *lost, uint32 *jitter){
	int ret = 0;
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	VDSP_PRINTF("%s, callId = %d, stream_id = %d\n", __FUNCTION__, call_id, table->stream_id);

	PhoneKSoketDeleteSession(0, 0, call_id, table->stream_id, &(table->rtcp_stats.tx_rtp_pkt_cnt), &(table->rtcp_stats.tx_rtp_octet_cnt), &(table->rtcp_stats.rx_rtp_pkt_cnt), &(table->rtcp_stats.rx_rtp_octet_cnt), lost, jitter);
	
	return MM_SUCCESS;
	
}

#endif

/******************************************************************************
 * dspDriverCreateMeetingRoom
 *
 * Description: 
 *     dsp driver interface for upper application, create dsp conference function, called by MM
 *
 * Parameters:
 * 	Input
 *    dspConf_t *dspConf_p - create dsp conference according to dsp pool dspConf_t structure which
 *						    contains peer-b dsp channel and peer-c dsp channel
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverCreateMeetingRoom(dspConf_t *dspConf_p){
	dspChan_t *bDspChan_p = NULL, *cDspChan_p = NULL;

	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	if(dspConf_p==NULL){
		return MM_FAILURE;
	}
	#if 0
	bDspChan_p=dspConf_p->bDspChan_p;
	cDspChan_p=dspConf_p->cDspChan_p;
	#else
	bDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->bLogicalId);
	cDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->cLogicalId);
	#endif

	if(NULL != bDspChan_p && NULL != cDspChan_p){	
#if 1	
		PhoneChanModify(bDspChan_p->phPhysicalId, bDspChan_p->codec_type, bDspChan_p->codec_num, bDspChan_p->chanId, bDspChan_p->call_id, bDspChan_p->stream_id, EPCNXMODE_CONF, bDspChan_p->silenceSupp, bDspChan_p->echoCancel, dspDriverGetDigitRelay(bDspChan_p), bDspChan_p->ptime ,bDspChan_p->dtmfCodecNum);
	  	PhoneChanModify(cDspChan_p->phPhysicalId, cDspChan_p->codec_type, cDspChan_p->codec_num, cDspChan_p->chanId, cDspChan_p->call_id, cDspChan_p->stream_id, EPCNXMODE_CONF, cDspChan_p->silenceSupp, cDspChan_p->echoCancel, dspDriverGetDigitRelay(cDspChan_p), cDspChan_p->ptime ,cDspChan_p->dtmfCodecNum);
		/*PhoneChanModify(cDspChan_p->phPhysicalId, c_profile,cDspChan_p->chanId,EPCNXMODE_CONF);*/
		/*PhoneChanModify(cDspChan_p->phPhysicalId, c_profile,cDspChan_p->chanId,EPCNXMODE_CONF);*/
#endif
	}
	else{
		VDSP_PRINTF("MM: %s : bDspChan or cDspChan is NULL, return fail\n", __FUNCTION__);
		return MM_FAILURE;
	}


	return MM_SUCCESS;
}

/******************************************************************************
 * dspDriverDestroyMeetingRoom
 *
 * Description: 
 *     dsp driver interface for upper application, destroy dsp conference function, called by MM
 *
 * Parameters:
 * 	Input
 *    dspConf_t *dspConf_p - destroy dsp conference according to dsp pool dspConf_t structure which
 *						    contains peer-b dsp channel and peer-c dsp channel
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverDestroyMeetingRoom(dspConf_t *dspConf_p){

	dspChan_t *bDspChan_p = NULL, *cDspChan_p = NULL;
	
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}

	if(NULL == dspConf_p){
		VDSP_PRINTF("%s failed, dspConf NULL!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	VDSP_PRINTF("MM: %s : bLogicalId %d, cLogicalId %d\n",__FUNCTION__, dspConf_p->bLogicalId, dspConf_p->cLogicalId);
	#if 0
	bDspChan_p=dspConf_p->bDspChan_p;
	cDspChan_p=dspConf_p->cDspChan_p;
	#else
	bDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->bLogicalId);
	cDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->cLogicalId);
	#endif

	if(NULL != bDspChan_p && NULL != cDspChan_p){	
		VDSP_PRINTF("MM: %s : b_codec_num %d, c_codec_num %d\n",__FUNCTION__, bDspChan_p->codec_num, cDspChan_p->codec_num);
		VDSP_PRINTF("MM: %s : b_physical_id %d, c_physical_id %d\n",__FUNCTION__,bDspChan_p->phPhysicalId, cDspChan_p->phPhysicalId);
		VDSP_PRINTF("MM: %s : b_chan_id %d, c_chan_id %d\n",__FUNCTION__,bDspChan_p->chanId, cDspChan_p->chanId);
#if 1
		PhoneChanModify(bDspChan_p->phPhysicalId, bDspChan_p->codec_type, bDspChan_p->codec_num, bDspChan_p->chanId, bDspChan_p->call_id, bDspChan_p->stream_id, EPCNXMODE_SNDRX,bDspChan_p->silenceSupp, bDspChan_p->echoCancel, dspDriverGetDigitRelay(bDspChan_p), bDspChan_p->ptime , bDspChan_p->dtmfCodecNum);
	  	PhoneChanModify(cDspChan_p->phPhysicalId, cDspChan_p->codec_type, cDspChan_p->codec_num, cDspChan_p->chanId, cDspChan_p->call_id, cDspChan_p->stream_id, EPCNXMODE_SNDRX,cDspChan_p->silenceSupp, cDspChan_p->echoCancel, dspDriverGetDigitRelay(cDspChan_p), cDspChan_p->ptime , cDspChan_p->dtmfCodecNum);
#endif
	}
	else{
		VDSP_PRINTF("MM: %s : bDspChan or cDspChan is NULL, skip and return success\n", __FUNCTION__);
	}
	return MM_SUCCESS;
}

/******************************************************************************
 * dspDriverCreateLocalpeerMeetingRoom
 *
 * Description: 
 *     dsp driver interface for upper application, create dsp localpeer conference function(2 local peer:fxs, 1 remote peer:sip), called by MM
 *
 * Parameters:
 * 	Input
 *    dspConf_t *dspConf_p - create dsp conference according to dsp pool dspConf_t structure which
 *						    contains peer-b dsp channel and peer-c dsp channel
 * 	dspChan_t *p_b_logical_dsp_chan - another logical dsp channel of peer b
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverCreateLocalpeerMeetingRoom(dspConf_t *dspConf_p, dspChan_t *p_b_logical_dsp_chan){
	dspChan_t *bDspChan_p = NULL, *cDspChan_p = NULL;

	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	
	if(dspConf_p == NULL){
		return MM_FAILURE;
	}
	
	#if 0
	bDspChan_p=dspConf_p->bDspChan_p;
	cDspChan_p=dspConf_p->cDspChan_p;
	#else
	bDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->bLogicalId);
	cDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->cLogicalId);
	#endif
	
	
	if(NULL != bDspChan_p && NULL != cDspChan_p){
		PhoneChanModify(bDspChan_p->phPhysicalId, bDspChan_p->codec_type, bDspChan_p->codec_num, bDspChan_p->chanId, bDspChan_p->call_id, bDspChan_p->stream_id, EPCNXMODE_CONF, bDspChan_p->silenceSupp, bDspChan_p->echoCancel, dspDriverGetDigitRelay(bDspChan_p), bDspChan_p->ptime , bDspChan_p->dtmfCodecNum);
		PhoneChanModify(cDspChan_p->phPhysicalId, cDspChan_p->codec_type, cDspChan_p->codec_num, cDspChan_p->chanId, cDspChan_p->call_id, cDspChan_p->stream_id, EPCNXMODE_CONF, cDspChan_p->silenceSupp, cDspChan_p->echoCancel, dspDriverGetDigitRelay(cDspChan_p), cDspChan_p->ptime , cDspChan_p->dtmfCodecNum);
		PhoneChanModify(p_b_logical_dsp_chan->phPhysicalId, bDspChan_p->codec_type, bDspChan_p->codec_num, p_b_logical_dsp_chan->chanId, p_b_logical_dsp_chan->call_id, p_b_logical_dsp_chan->stream_id, EPCNXMODE_SNDRX, bDspChan_p->silenceSupp, bDspChan_p->echoCancel, dspDriverGetDigitRelay(bDspChan_p), bDspChan_p->ptime , bDspChan_p->dtmfCodecNum);
	}
	else{
		VDSP_PRINTF("MM: %s : bDspChan or cDspChan is NULL, return fail\n", __FUNCTION__);
		return MM_FAILURE;
	}
	
	return MM_SUCCESS;
}

/******************************************************************************
 * dspDriverDestroyLocalpeerMeetingRoom
 *
 * Description: 
 *     dsp driver interface for upper application, destroy dsp localpeer conference function(2 local peer:fxs, 1 remote peer:sip), called by MM
 *
 * Parameters:
 * 	Input
 *    dspConf_t *dspConf_p - destroy dsp conference according to dsp pool dspConf_t structure which
 *						    contains peer-b dsp channel and peer-c dsp channel
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverDestroyLocalpeerMeetingRoom(dspConf_t *dspConf_p){
	
	dspChan_t *bDspChan_p = NULL, *cDspChan_p = NULL;
	
	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}

	#if 0
	bDspChan_p=dspConf_p->bDspChan_p;
	cDspChan_p=dspConf_p->cDspChan_p;
	#else
	bDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->bLogicalId);
	cDspChan_p=dspPoolFindDspChanByLogicalId(dspConf_p->cLogicalId);
	#endif
	
	if(NULL != bDspChan_p && NULL != cDspChan_p){
		VDSP_PRINTF("MM: %s : b_codec_num %d, c_codec_num %d\n",__FUNCTION__, bDspChan_p->codec_num, cDspChan_p->codec_num);
		VDSP_PRINTF("MM: %s : b_physical_id %d, c_physical_id %d\n",__FUNCTION__,bDspChan_p->phPhysicalId, cDspChan_p->phPhysicalId);
		VDSP_PRINTF("MM: %s : b_chan_id %d, c_chan_id %d\n",__FUNCTION__,bDspChan_p->chanId, cDspChan_p->chanId);
#if 1
		PhoneChanModify(bDspChan_p->phPhysicalId, bDspChan_p->codec_type, bDspChan_p->codec_num,bDspChan_p->chanId, bDspChan_p->call_id, bDspChan_p->stream_id, EPCNXMODE_SNDRX, bDspChan_p->silenceSupp, bDspChan_p->echoCancel, dspDriverGetDigitRelay(bDspChan_p), bDspChan_p->ptime , bDspChan_p->dtmfCodecNum);
		PhoneChanModify(cDspChan_p->phPhysicalId, cDspChan_p->codec_type, cDspChan_p->codec_num,cDspChan_p->chanId, cDspChan_p->call_id, cDspChan_p->stream_id, EPCNXMODE_SNDRX, cDspChan_p->silenceSupp, cDspChan_p->echoCancel, dspDriverGetDigitRelay(cDspChan_p), cDspChan_p->ptime ,cDspChan_p->dtmfCodecNum);
#endif
	}
	else{
		VDSP_PRINTF("MM: %s : bDspChan or cDspChan is NULL, skip and return success\n", __FUNCTION__);
	}

	return MM_SUCCESS;
}
/******************************************************************************
 * dspDriverSetFaxParameter
 *
 * Description: 
 *     dsp driver interface for upper application, set fax parameter on dsp chan(jitter buffer...)
 *
 * Parameters:
 * 	Input
 *    dspChan_t *dspChan_p - set fax parameter on dsp chan
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/
mm_return_t dspDriverSetFaxParameter(dspChan_t *dspChan_p){
	int ret = -1;

	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	
	if(NULL != dspChan_p){
		if(CHAN_OPENED == dspChan_p->chanState && ZYNOS_CODEC_PCMU == dspChan_p->codec_num){			
			ret=PhoneChanModifyForFaxRelay(dspChan_p->phPhysicalId, dspChan_p->chanId);
			
			if(ret == 0){
				return MM_SUCCESS;
			}
			
		}		
		
	}
	return MM_FAILURE;
}




/******************************************************************************
 * dspDriverDumpRtpStatus
 *
 * Description: 
 *     dsp driver interface for upper application, dsp channel open function, called by MM
 *
 * Parameters:
 * 	Input
 *    dspChan_t *dsp_chan - specify dsp channel to open
 *	uint8 data_type -specify which codec to use
 *	uint16 mode - 0 : sendonly, 1: recvonly, 2: sendrecv
 *  
 *
 * Return Value:
 *    Indicate if the ioctl success or failure.
 *    	MM_FAILURE = failure
 *    MM_SUCCESS = success 
 *****************************************************************************/

mm_return_t dspDriverDumpRtpStatus(dspChan_t *dspChan_p ){
	int ret = -1;

	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	
	if(NULL != dspChan_p){
		ZyPrintf(" DSP DRIVER > %s : endpt %d, cnx %d\n",__FUNCTION__, dspChan_p->phPhysicalId, dspChan_p->chanId);
		ret=GetRtpStatus(dspChan_p->phPhysicalId, dspChan_p->chanId);
	}
	else{
		ret = -1;
	}
	
	if(ret == 0)
		return MM_SUCCESS;
	else
		return MM_FAILURE;
}

#if 1	/* aircheng add for RTP statistics */

mm_return_t dspDriverGetRtpStatistics(dspChan_t *dspChan_p, rtpTable_t *table, uint32 *lost, uint32 *jitter){
	int ret = -1;

	if(!gIsDspInitialized){
		VDSP_PRINTF("%s failed, dsp driver not initialized!!\n", __FUNCTION__);
		return MM_FAILURE;
	}
	
	if(NULL != dspChan_p){
		VDSP_PRINTF(" DSP DRIVER > %s : endpt %d, cnx %d\n", __FUNCTION__, dspChan_p->phPhysicalId, dspChan_p->chanId);
		PhoneGetRtpStatistics(dspChan_p->chanId, dspChan_p->phPhysicalId, &(table->rtcp_stats.tx_rtp_pkt_cnt), &(table->rtcp_stats.tx_rtp_octet_cnt), &(table->rtcp_stats.rx_rtp_pkt_cnt), &(table->rtcp_stats.rx_rtp_octet_cnt), lost, jitter);
		ret = 0;
	}
	else{
		ret = -1;
	}
	
	if(ret == 0)
		return MM_SUCCESS;
	else
		return MM_FAILURE;
}

#endif

