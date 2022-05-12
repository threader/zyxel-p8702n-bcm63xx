#ifndef __SYS_IPC_INCLUDE__
#define __SYS_IPC_INCLUDE__


/*____________________________________________________________________________*/
/*	functions                                                                                                                     */
/*____________________________________________________________________________*/
int eventVoiceSimpleSend(uint16 event, uint16 dest_fid, uint16 chann_id);
int eventVoiceDataSend(uint16	event, uint16	dest_fid, uint16 chann_id, void *data_p, uint16 data_size);
int eventVoiceCCMSimpleSend(uint16 event, uint16 dest_fid, uint16 chann_id, uint16 type);
int eventVoiceCCMDataSend(uint16 event, uint16 dest_fid, uint16 chann_id, uint16 type, void *data_p, uint16 data_size);
int eventVoiceDspDataSend(uint8 *p_input_buf, uint8 **p_p_output_buf);

#endif

