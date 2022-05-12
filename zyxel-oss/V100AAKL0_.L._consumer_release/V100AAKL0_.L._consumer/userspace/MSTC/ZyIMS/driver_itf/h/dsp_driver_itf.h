#ifndef __DSP_DRIVER_ITF_H__
#define __DSP_DRIVER_ITF_H__
/******************************************************************************
*  function prototypes
*******************************************************************************/
typedef enum {
	SENDONLY,
	RECVONLY, 
	SENDRECV,
	CONFERENCE,
	INACTIVE,
	LOOPBACK,
	TEST,
	REPLICATE,
	#if 1 /*Eason, Added for 404L01 voice driver*/
	TRANSCODE
	#endif
}chan_mode_t;


mm_return_t dspDriverInit(int country);
void dspDriverGetInitData(uint8 **initData, uint16 *data_length);
#if 0
mm_return_t dsp_driver_get_capabilities(icf_cfg_codec_info_req_st *p_cfg_codec_info_req);
#endif
uint8 dspDriverGetDtmfMethod(dspChan_t *dspChan_p);
mm_return_t dspDriverChanOpen(dspChan_t *dsp_chan, int mode);
mm_return_t dspDriverChanModify(dspChan_t *dspChan_p, int mode);

mm_return_t dspDriverCreateMeetingRoom(dspConf_t *dspConf_p);
mm_return_t dspDriverDestroyMeetingRoom(dspConf_t *dspConf_p);
mm_return_t dspDriverCreateLocalpeerMeetingRoom(dspConf_t *dspConf_p, dspChan_t *p_b_logical_dsp_chan);
mm_return_t dspDriverDestroyLocalpeerMeetingRoom(dspConf_t *dspConf_p);
int dspDriverGetDigitRelay(dspChan_t *dspChan_p);

#if KSOCKET_RTP_SUPPORT
//mm_return_t dspDriverKSocketMakeSession(dspChan_t *dspChan_p, uint32 saddr, uint16 sport, uint32 daddr, uint16 dport, uint8 mediaMode, uint8 qosMark);
//mm_return_t dspDriverKSocketDeleteSession(dspChan_t *dspChan_p, rtpTable_t *table,uint32 *lost, uint32 *jitter);
mm_return_t dspDriverKSocketMakeSession(int call_id, int stream_id, uint32 saddr, uint16 sport, uint32 daddr, uint16 dport, uint8 mediaMode, uint8 qosMark, char *ifName);
mm_return_t dspDriverKSocketDeleteSession(int call_id, rtpTable_t *table,uint32 *lost, uint32 *jitter);
#endif

#if 1	/* aircheng add for RTP statistics */
mm_return_t dspDriverGetRtpStatistics(dspChan_t *dspChan_p, rtpTable_t *table, uint32 *lost, uint32 *jitter);
#endif

#endif
