#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <netinet/in.h>
#include "icf_types.h"
#include "icf_feature_flags.h"
#include "icf_defs.h"
#include "icf_trace_id.h"
#include "icf_trace_error_types.h"
#include "icf_error_id.h"
#include "icf_api.h"
#include "global.h"
#include "mwiParse.h"
#include "call_mgnt.h"
#include "Voipccm_sip_itf.h"
#include "voiceApp_main.h"
#include "voiceApp_iptk_MsgHandler.h"
#include "voiceApp_iptk_api.h"
#include "voiceApp_macro.h"
#include "tr104.h"
#include "switch.h" 	
#if HEARTBEAT_MECHANISM_ENABLE	
#include "tr104_global.h"
#endif
/*Jason , syslog*/
#ifdef VOIP_SYSLOG
#include "syslog.h"
#include "voip_syslog.h"
#include <sys/time.h>
#endif

int sigRegisterTimer(tm_id_t unused, void *vp);
int voiceAppSubscribeTimer(tm_id_t unused, void *vp);
int GetInfoDtmf(icf_info_ind_st *p_info_ind, uint8 *digit, uint8 *volume, uint16 *duration);
/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/
void voiceAppRecDNSMap(uint8 record_type,uint8 cause,char* fqdn_str, char* ip_str);
void voiceAppRecFailReReg(uint16 line_id,uint8 request_type,uint16 error_cause,uint16  response_code);
void voiceAppRecICFErr(uint32 error_id, char *error_str);

/* variable init in tr104ModuleInit*/
uint8* reg_error_cnt = NULL;
tm_id_t reg_status_timer = 0;
icf_call_id_t being_replaced_callId = 0;
extern webSipStatus_t *webSipStatus;
/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/
voiceAppICFErrRec_t voiceAppICFErrRec;
#if HEARTBEAT_MECHANISM_ENABLE
void voiceAppHeartbeatOptionsCheckResp(icf_msg_st *p_msg);
int voiceAppHeartbeatOptionsTimer( tm_id_t unused, void *vp);
/* variable init in tr104ModuleInit*/
heartbeatRec_t* heartbeatRecord = NULL;
#endif

extern int APICFMInitflag;
/*******************************************************************************
 *
 * FUNCTION NAME : voiceAppIPTKMsgProcess
 *
 * DESCRIPTION   : The function is handle IPTK message
 *
 * RETURNS       : bool
 *
 ******************************************************************************/
bool voiceAppIPTKMsgProcess(uint8 *recvBuf, int recvLen) {
	icf_error_t p_ecode = ICF_FAILURE;
	icf_msg_st *p_msg = NULL;
	icf_error_t ecode = 0;
	icf_api_id_t	api_id;
	icf_call_id_t	call_id;
	icf_uint8_t *p_payload = NULL;
	icf_return_t ret_val = ICF_SUCCESS;

	if(recvBuf == NULL) {
		return FALSE;
	}

	if(icf_decode_msg((icf_msg_st**)&p_msg, recvBuf, recvLen, &p_ecode) != ICF_SUCCESS){
		ZyPrintf("IPC packet from IPTK decode error!!\n");
		return FALSE;
	}

	api_id  = VOICEAPP_IPTK_MSG_GET_API(p_msg);
	call_id = VOICEAPP_IPTK_MSG_GET_CALL_ID(p_msg);

	VOICEAPP_DBG("\n######################################################");
	VOICEAPP_PRINTF("IPTK -> voiceApp : API id %d\n", api_id);
#if defined (IPTK_REL_8_2_2)	
	VOICEAPP_PRINTF("IPTK -> voiceApp : API name %s\n\r",iptk_api_id2str(api_id));
#endif
	VOICEAPP_DBG1("IPTK -> voiceApp : call_id ", call_id);
	VOICEAPP_DBG("######################################################");
	
	switch (api_id) {
		/* application startup APIs*/
		case ICF_APP_ADD_CFM: {
				/* Verifying the result fot APP_ADD_REQ */
				p_payload = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				if( ICF_FALSE == ((icf_app_add_cfm_st*)p_payload)->result) {
					ZyPrintf("VOICEAPP> APP_ADD_REQ Failure\n");
					if((((icf_app_add_cfm_st*)p_payload)->bit_mask) & ICF_APP_ADD_CFM_ERROR_CAUSE_PRESENT) {
						ZyPrintf("error code %d\n", ((icf_app_add_cfm_st*)p_payload)->error_cause);
					}
                  	ret_val = ICF_FAILURE;
				} else {
					globalVoiceApp.appId = VOICEAPP_IPTK_MSG_GET_APP_ID(p_msg);
					VOICEAPP_DBG1("VOICEAPP> APP_ADD_REQ Successful app_id = ", globalVoiceApp.appId);
					voipActivate();
				}
			}
			break;
		case ICF_APP_REMOVE_CFM: {
				/* Verifying the result fot APP_REMOVE_REQ */
				p_payload = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				if( ICF_FALSE == ((icf_app_remove_cfm_st*)p_payload)->result) {
					ZyPrintf("VOICEAPP> APP_REMOVE_REQ Failure, error code %d\n", ((icf_app_remove_cfm_st*)p_payload)->error_cause);
                  	  		ret_val = ICF_FAILURE;
				} else {
					VOICEAPP_DBG("VOICEAPP> APP_REMOVE_REQ Successful");
				}
			}
			break;
#ifdef ICF_NW_ACTIVATE
		case ICF_NETWORK_ACTIVATE_CFM:
			break;
		case ICF_NETWORK_DEACTIVATE_CFM:
			break;
#endif
		case ICF_CREATE_SECURE_CONN_RESP:
			break;
		case ICF_CLOSE_SECURE_CONN_RESP:
			break;
		case ICF_CONFIGURE_CFM: {
				/* ICF_CONFIGURE_CFM can have the response for a number of
				* configuration APIs. Therefor e verifying the result on basis 
				* of req-id.
				*/
				VOICEAPP_PRINTF(" ICF_CONFIGURE_CFM : %d \n",ICF_CONFIGURE_CFM);
				icf_configure_cfm_st  *p_resp_icf = NULL;
				p_resp_icf=(icf_configure_cfm_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				icf_api_id_t cfm_api_id = p_resp_icf->api_id;
				switch(cfm_api_id) {
					case ICF_SET_TRANSPORT_REQ: {
							if( p_resp_icf->result==ICF_SUCCESS ) {
								VOICEAPP_PRINTF("%s, ICF_SET_TRANSPORT_REQ successfull, line %d,APICFMInitflag = %d \n",__FUNCTION__,p_resp_icf->line_id,APICFMInitflag);
								ret_val = ICF_SUCCESS;
							} else {
								ZyPrintf("%s, ICF_SET_TRANSPORT_REQ Failure, line %d, error code %d,APICFMInitflag = %d\n",__FUNCTION__,p_resp_icf->line_id, p_resp_icf->error_cause,APICFMInitflag);
								ret_val = ICF_FAILURE;
							}
							if(APICFMInitflag > 0) {
								voiceAppIPTKAcquireMutex();
								APICFMInitflag = APICFMInitflag - 1;
								VOICEAPP_PRINTF("%s,%d : voiceapp-acif(dec)APICFMInitflag = %d,line %d!!\n" , __FUNCTION__,ICF_SET_TRANSPORT_REQ,APICFMInitflag,p_resp_icf->line_id);								
								voiceAppIPTKReleaseMutex();
							}
						}
						break;

					case ICF_SET_CALL_PARAM_REQ: {
							if( p_resp_icf->result==ICF_SUCCESS ) {
								VOICEAPP_PRINTF("VOICEAPP> ICF_SET_CALL_PARAM_REQ successfull, line %d \n", p_resp_icf->line_id);
								ret_val = ICF_SUCCESS;
							} else {
								ZyPrintf("VOICEAPP> SET_CALL_PARAM_REQ Failure, error code %d\n", p_resp_icf->error_cause);
								ret_val = ICF_FAILURE;
							}
						}
						break;

					case ICF_SET_SELF_ID_REQ: {
							if( p_resp_icf->result==ICF_SUCCESS ) {
								VOICEAPP_PRINTF("%s, ICF_SET_SELF_ID_REQ successfull, line %d,APICFMInitflag = %d \n",__FUNCTION__ ,p_resp_icf->line_id,APICFMInitflag);	
								ret_val = ICF_SUCCESS;
							} else {
								ZyPrintf("%s, ICF_SET_SELF_ID_REQ Failure, line %d, error code %d,APICFMInitflag = %d\n",__FUNCTION__ ,p_resp_icf->line_id, p_resp_icf->error_cause,APICFMInitflag);
								
								ret_val = ICF_FAILURE;
							}
							if(APICFMInitflag > 0) {
								voiceAppIPTKAcquireMutex();
								APICFMInitflag = APICFMInitflag - 1;
								VOICEAPP_PRINTF("%s,%d : voiceapp-acif(dec)APICFMInitflag = %d!!,line %d\n" , __FUNCTION__,ICF_SET_SELF_ID_REQ,APICFMInitflag,p_resp_icf->line_id);									
								voiceAppIPTKReleaseMutex();
							}
						}
						break;

					case ICF_SET_REGISTRATION_REQ: {
							if( p_resp_icf->result==ICF_SUCCESS ) {
								VOICEAPP_PRINTF("%s, ICF_SET_REGISTRATION_REQ successfull,line %d,APICFMInitflag = %d \n",__FUNCTION__ ,p_resp_icf->line_id,APICFMInitflag);
								ret_val = ICF_SUCCESS;
							} else {
								ZyPrintf("%s, ICF_SET_REGISTRATION_REQ Failure, line %d, error code %d,APICFMInitflag = %d\n",__FUNCTION__ ,p_resp_icf->line_id, p_resp_icf->error_cause,APICFMInitflag);
								ret_val = ICF_FAILURE;
							}
							if(APICFMInitflag > 0) {
								voiceAppIPTKAcquireMutex();
								APICFMInitflag = APICFMInitflag - 1;
								VOICEAPP_PRINTF("%s,%d : voiceapp-acif(dec)APICFMInitflag = %d,line %d!!\n" , __FUNCTION__,ICF_SET_REGISTRATION_REQ,APICFMInitflag,p_resp_icf->line_id);								
								voiceAppIPTKReleaseMutex();
							}
						}
						break;

					case ICF_SET_SERVICE_PARAMS_REQ: {
							if( p_resp_icf->result==ICF_SUCCESS ) {
								VOICEAPP_PRINTF("%s, ICF_SET_SERVICE_PARAMS_REQ successfull, line %d,APICFMInitflag = %d \n",__FUNCTION__,p_resp_icf->line_id,APICFMInitflag);
								ret_val = ICF_SUCCESS;
							} else {
								ZyPrintf("%s, ICF_SET_SERVICE_PARAMS_REQ Failure, line %d, error code %d,APICFMInitflag = %d\n",__FUNCTION__,p_resp_icf->line_id, p_resp_icf->error_cause,APICFMInitflag);
								ret_val = ICF_FAILURE;
							}
							if(APICFMInitflag > 0) {
								voiceAppIPTKAcquireMutex();
								APICFMInitflag = APICFMInitflag - 1;
								VOICEAPP_PRINTF("%s,%d : voiceapp-acif(dec)APICFMInitflag = %d,line %d!!\n" , __FUNCTION__,ICF_SET_SERVICE_PARAMS_REQ,APICFMInitflag,p_resp_icf->line_id);								
								voiceAppIPTKReleaseMutex();
							}
						}
						break;
						
					case ICF_SET_LINE_PARAM_REQ: {
							if( p_resp_icf->result==ICF_SUCCESS ) {
								VOICEAPP_PRINTF("%s, ICF_SET_LINE_PARAM_REQ successfull, line %d,APICFMInitflag = %d \n",__FUNCTION__, p_resp_icf->line_id,APICFMInitflag);								
								ret_val = ICF_SUCCESS;
							} else {
								ZyPrintf("%s. ICF_SET_LINE_PARAM_REQ Failure, line %d, error code %d,APICFMInitflag = %d\n",__FUNCTION__ ,p_resp_icf->line_id, p_resp_icf->error_cause,APICFMInitflag);
								ret_val = ICF_FAILURE;
							}
							if(APICFMInitflag > 0) {
								voiceAppIPTKAcquireMutex();
								APICFMInitflag = APICFMInitflag - 1;
								VOICEAPP_PRINTF("%s,%d : voiceapp-acif(dec)APICFMInitflag = %d,line %d!!\n" , __FUNCTION__,ICF_SET_LINE_PARAM_REQ,APICFMInitflag,p_resp_icf->line_id);															
								voiceAppIPTKReleaseMutex();
							}
						}
						break;
				}
			}
			break;
		
		case ICF_GET_SYSTEM_SETTINGS_RESP: {
				icf_get_system_settings_resp_st *p_resp_sys = NULL;
				p_resp_sys=(icf_get_system_settings_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				ZyPrintf("\n#####IPTK SYSTEM_SETTINGS DUMP#####\n");
				ZyPrintf("prack_enabled = %d\n", p_resp_sys->prack_enabled); 
#ifdef IPTK_REL_7_1
				ZyPrintf("auth_type = %d\n", p_resp_sys->auth_type);
#endif
				ZyPrintf("privacy_supported = %d\n", p_resp_sys->privacy_supported);
				ZyPrintf("max_lines = %d\n", p_resp_sys->max_lines);
				ZyPrintf("setup_timer = %d\n", p_resp_sys->setup_timer);
				ZyPrintf("alerting_timer = %d\n", p_resp_sys->alerting_timer);
				ZyPrintf("release_timer = %d\n", p_resp_sys->release_timer);
				ZyPrintf("modify_media_timer = %d\n", p_resp_sys->modify_media_timer);
				ZyPrintf("registration_resp_timer = %d\n", p_resp_sys->registration_resp_timer);
				ZyPrintf("registration_retry_timer = %d\n", p_resp_sys->registration_retry_timer);
				ZyPrintf("subs_retry_timer = %d\n", p_resp_sys->subs_retry_timer);
				ZyPrintf("subs_resp_timer = %d\n", p_resp_sys->subs_resp_timer);
				ZyPrintf("sig_comp_enabled = %d\n", p_resp_sys->sig_comp_enabled);
				ZyPrintf("security_enabled = %d\n", p_resp_sys->security_enabled);
				ZyPrintf("self_ip_address = %d.", p_resp_sys->self_ip_address.addr.addr.ipv4_addr.octet_1);
				ZyPrintf("%d.%d.", p_resp_sys->self_ip_address.addr.addr.ipv4_addr.octet_2, p_resp_sys->self_ip_address.addr.addr.ipv4_addr.octet_3);
				ZyPrintf("%d\n", p_resp_sys->self_ip_address.addr.addr.ipv4_addr.octet_4);
				ZyPrintf("self_ip_address.port_num = %d\n", p_resp_sys->self_ip_address.port_num);
				ZyPrintf("self_transport_mode = %d\n", p_resp_sys->self_transport_mode);
				ZyPrintf("self_port = %d\n", p_resp_sys->self_port);
				ZyPrintf("call_thru_proxy = %d\n", p_resp_sys->call_thru_proxy);
				ZyPrintf("sip_compact_header = %d\n", p_resp_sys->sip_compact_header);
				ZyPrintf("qos_mark = %d\n", p_resp_sys->qos_mark);
				ZyPrintf("session_interval_timer = %d\n", p_resp_sys->session_interval_timer);
#ifdef IPTK_REL_8_0
				ZyPrintf("min_session_interval_timer = %d\n", p_resp_sys->min_session_interval_timer);
#endif
#ifdef ICF_NAT_RPORT_SUPPORT
				ZyPrintf("binding_refresh_timer = %d\n", p_resp_sys->binding_refresh_timer);
				ZyPrintf("binding_query_enable = %d\n", p_resp_sys->binding_query_enable);
#endif
				ZyPrintf("\n###IPTK SYSTEM_SETTINGS DUMP END###\n");
			
				if( p_resp_sys->bit_mask && ICF_API_GET_SYS_STG_RESULT_SUCCESS != 0) {
					VOICEAPP_DBG("VOICEAPP> GET_SYSTEM_SETTINGS_REQ successfull");
					ret_val = ICF_SUCCESS;
				} else {
					ZyPrintf("VOICEAPP> GET_SYSTEM_SETTINGS_REQ Failure\n");
					ret_val = ICF_FAILURE;
				}
			}
			break;
		
		case ICF_GET_LINE_SETTINGS_RESP: {
				icf_get_line_settings_resp_st *p_resp_line = NULL;
				p_resp_line=(icf_get_line_settings_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				ZyPrintf("\n#####IPTK LINE_SETTINGS DUMP#####\n");
				ZyPrintf("proxy_address = %d.", p_resp_line->proxy_address.addr.addr.ipv4_addr.octet_1);
				ZyPrintf("%d.%d.", p_resp_line->proxy_address.addr.addr.ipv4_addr.octet_2, p_resp_line->proxy_address.addr.addr.ipv4_addr.octet_3);
				ZyPrintf("%d\n", p_resp_line->proxy_address.addr.addr.ipv4_addr.octet_4);
				ZyPrintf("proxy_address.port_num = %d\n", p_resp_line->proxy_address.port_num);
				ZyPrintf("registrar_address = %d.", p_resp_line->registrar_address.addr.addr.ipv4_addr.octet_1);
				ZyPrintf("%d.%d.", p_resp_line->registrar_address.addr.addr.ipv4_addr.octet_2, p_resp_line->registrar_address.addr.addr.ipv4_addr.octet_3);
				ZyPrintf("%d\n", p_resp_line->registrar_address.addr.addr.ipv4_addr.octet_4);
				ZyPrintf("registrar_address.port_num = %d\n", p_resp_line->registrar_address.port_num);
				ZyPrintf("proxy_transport_mode = %d\n", p_resp_line->proxy_transport_mode);
				ZyPrintf("self_address_list = %s\n", p_resp_line->self_address_list.addr_val.str);
				ZyPrintf("self_address_list addr_type= %d\n", p_resp_line->self_address_list.addr_type);
				ZyPrintf("self_name = %s\n", p_resp_line->self_name.str);
				ZyPrintf("presentation_allowed = %d\n", p_resp_line->presentation_allowed);
				ZyPrintf("auth_key = %s\n", p_resp_line->auth_key.str);
				ZyPrintf("auth_password = %s\n", p_resp_line->auth_password.str);
				ZyPrintf("reg_mode = %d\n", p_resp_line->reg_mode);
				ZyPrintf("reg_duration = %d\n", p_resp_line->reg_duration);
				ZyPrintf("reg_route_addr = %d.", p_resp_line->reg_route_addr.addr.addr.ipv4_addr.octet_1);
				ZyPrintf("%d.%d.", p_resp_line->reg_route_addr.addr.addr.ipv4_addr.octet_2, p_resp_line->reg_route_addr.addr.addr.ipv4_addr.octet_3);
				ZyPrintf("%d\n", p_resp_line->reg_route_addr.addr.addr.ipv4_addr.octet_4);
				ZyPrintf("reg_route_addr.port_num = %d\n", p_resp_line->reg_route_addr.port_num);
				ZyPrintf("sip_T1_timer = %d\n", p_resp_line->sip_T1_timer);
				ZyPrintf("sip_B_timer = %d\n", p_resp_line->sip_B_timer);
				ZyPrintf("sip_F_timer = %d\n", p_resp_line->sip_F_timer);
				ZyPrintf("\n###IPTK LINE_SETTINGS DUMP END###\n");
				
				if( p_resp_line->bit_mask && ICF_API_GET_LINE_STG_RESULT_SUCCESS != 0) {
					VOICEAPP_DBG("VOICEAPP> GET_LINE_SETTINGS_REQ successfull");
					ret_val = ICF_SUCCESS;
				} else {
					ZyPrintf("VOICEAPP> GET_LINE_SETTINGS_REQ Failure\n");
					ret_val = ICF_FAILURE;
			        }
			}
			break;
		case ICF_GET_REGISTER_STATUS_RESP: {
				icf_get_register_status_resp_st     *p_get_reg_status_resp = ICF_NULL;
				icf_list_st							*p_user_reg_list = ICF_NULL;
				icf_user_reg_status_st				*p_user_reg_data = ICF_NULL;	
				icf_uint32_t  						i = 1, addr_type = 0;
				int SipAccountId = 0;
				VoiceProfLineObj_t *lineObj = NULL;
				int LineItr = 0, VpItr=0;
		    
				p_get_reg_status_resp = (icf_get_register_status_resp_st *)p_msg->payload;

				VOICEAPP_DBG1("Result = ", p_get_reg_status_resp->result);        
			  	if (p_get_reg_status_resp->result == ICF_FAILURE) {
					ZyPrintf("error_cause = %d\n", p_get_reg_status_resp->error_cause);        
					ret_val = ICF_FAILURE;
				} else {
					p_user_reg_list = p_get_reg_status_resp->p_user_reg_status;

					while (p_user_reg_list != ICF_NULL) {
					p_user_reg_data = (icf_user_reg_status_st *)(p_user_reg_list->p_data);

					ZyPrintf("Info of user %u:\n", i);
					ZyPrintf("     addr_type = %u,", p_user_reg_data->user.addr_type);
					ZyPrintf(" addr_val = %s,", p_user_reg_data->user.addr_val.str);
					SipAccountId = tr104FindLineIdFromRegisterUrl((char *)p_user_reg_data->user.addr_val.str,p_user_reg_data->user.addr_type);
					VOICEAPP_PRINTF("receive ICF_GET_REGISTER_STATUS_RESP, SipAccountId %d\n", SipAccountId);
					VpItr = mapSipAccountIdToVpItr(SipAccountId);
					LineItr = mapSipAccountIdToLineItr(SipAccountId);
					lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
						switch (p_user_reg_data->reg_status)
						{
							case ICF_RGM_00_IDLE:
								ZyPrintf(" reg_status = IDLE\n");
								if(NULL != lineObj){
									if(lineObj->status == TR104I_STATUS_UNREGISTERING) {
										sipEvtToCCM(SipAccountId, call_id, EVT_CCM_UNREG_SUCC, NULL, 0);
										/*Jason , syslog*/
										#ifdef VOIP_SYSLOG									
										voipSyslog(LOG_SIP_REGISTRATION,LOG_INFO,lineObj->directoryNumber,0,"DeRegister Success");
										#endif
									}
									if((lineObj->status == TR104I_STATUS_UNREGISTERING) || (lineObj->status == TR104I_STATUS_UNREGISTERED)){
										/*user activate unregister action, will not automatically register up when server available*/
										lineObj->status = TR104I_STATUS_UNREGISTERED;
									}
									else if(lineObj->status == TR104I_STATUS_REGISTERING){
										/*keep registering status, for retry register timer to resend register*/
									}
									else{
										/*not register up currently, will automatically register if server is available*/
										lineObj->status = TR104I_STATUS_IDLE;
									}
								}								
								break;
							case ICF_RGM_01_REGISTERING:
								ZyPrintf(" reg_status = REGISTERING\n");
								#if 1 /*Jason , [SPRID # 090825486] */
								if(SipAccountId != -1){
									if(NULL != lineObj){
										lineObj->status = TR104I_STATUS_REGISTERING;
									}
								}	
								#endif
								break;
							case ICF_RGM_02_UNREGISTERED:
								ZyPrintf(" reg_status = UNREGISTERED\n");
								if(SipAccountId != -1){
									if(NULL != lineObj){
										lineObj->status = TR104I_STATUS_UNREGISTERED;
									}
									sipEvtToCCM(SipAccountId, call_id, EVT_CCM_UNREG_SUCC, NULL, 0);
									/*Jason , syslog*/
									#ifdef VOIP_SYSLOG									
									if(NULL != lineObj){
									voipSyslog(LOG_SIP_REGISTRATION,LOG_INFO,lineObj->directoryNumber,0,"DeRegister Success");
									}
									#endif	
								}
								break;
							case ICF_RGM_03_REGISTERED:
								ZyPrintf(" reg_status = REGISTERED\n");
								if(SipAccountId != -1){
									if(NULL != lineObj){
										lineObj->status = TR104I_STATUS_UP;
									}
									sipEvtToCCM(SipAccountId, call_id, EVT_CCM_REG_SUCC, NULL, 0);
									/*Jason , syslog*/
									#ifdef VOIP_SYSLOG	
									voipSyslog(LOG_SIP_REGISTRATION,LOG_INFO,lineObj->directoryNumber,0,"Register Success");
									#endif	
									reg_error_cnt[SipAccountId] = 0;
									zyMemset((void *)&webSipStatus[SipAccountId], 0, sizeof(webSipStatus));
									webSipStatus[SipAccountId].lastRegisteration = time(NULL);
									ZyPrintf("webSipStatus[%d] set lastRegisteration to %d\n", SipAccountId, webSipStatus[SipAccountId].lastRegisteration);
								}
								break;
							case ICF_RGM_04_AUTHORIZING:
								ZyPrintf(" reg_status = AUTHORIZING\n");
								break;
							case ICF_RGM_05_DEREGISTERING:
								ZyPrintf(" reg_status = DEREGISTERING\n");
								#if 1 /*Jason , [SPRID # 090825486] */
								if(SipAccountId != -1){
									if(NULL != lineObj){
										lineObj->status = TR104I_STATUS_UNREGISTERING;
									}
								}	
								#endif							
								break;
							default:
								ZyPrintf(" reg_status = UNKNOWN\n");
								break;
						}	
						if(NULL != lineObj){
							VOICEAPP_PRINTF("SipAccountId %d status = %d\n", SipAccountId, lineObj->status);
						}
						voipLedOnOff( tr104GetNumOfPhyIntf() );
						p_user_reg_list = p_user_reg_list->p_next;
						i++;
					} /* while */

					addr_type = p_get_reg_status_resp->registrar_address.addr.addr_type;

					switch (addr_type)
					{
						case ICF_TRANSPORT_ADDRESS_DOMAIN_NAME:
			            			ZyPrintf("Registrar address = %s,",\
							p_get_reg_status_resp->registrar_address.addr.addr.domain.str);
							break;

						case ICF_TRANSPORT_ADDRESS_IPV4_ADDR:
							ZyPrintf("\nRegistrar_address = %u.%u.%u.%u,",\
								p_get_reg_status_resp->registrar_address.addr.addr.\
								ipv4_addr.octet_1,\
								p_get_reg_status_resp->registrar_address.addr.addr.\
								ipv4_addr.octet_2,\
								p_get_reg_status_resp->registrar_address.addr.addr.\
								ipv4_addr.octet_3,\
								p_get_reg_status_resp->registrar_address.addr.addr.\
			                                                      ipv4_addr.octet_4);
							break;
					} /* switch */

					ZyPrintf(" port = %u,", \
						p_get_reg_status_resp->registrar_address.port_num);

					ZyPrintf(" expiry = %u\n", \
						p_get_reg_status_resp->expires_duration / 1000);

			      	} /* else */

				ret_val = ICF_SUCCESS;
			}
			break;
		/* application startup APIs end*/
		/* control plane APIs*/
		case ICF_REGISTER_CFM: {				
				icf_register_cfm_st *p_resp_reg;
				p_resp_reg = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				int LineItr = 0, VpItr=0;
				VOICEAPP_PRINTF("receive ICF_REGISTER_CFM, SipAccountId %d\n", p_resp_reg->line_id);				
			#if HEARTBEAT_MECHANISM_ENABLE 
				int i=0,profileInUse=FALSE;
				int NumOfLine = 0;
				VpItr = mapSipAccountIdToVpItr(p_resp_reg->line_id);	//p_resp_reg->line_id  is  SipAccountId
				LineItr = mapSipAccountIdToLineItr(p_resp_reg->line_id);				

				VoiceProfLineObj_t* p_lineObj = NULL;
				VoiceProfObj_t*	p_profileObj = tr104VoiceProfObjPtrGet(VpItr);

				uint32 rateOfHeartbeat = p_profileObj->SIPObj->ZyXEL_RateOfHeartbeat;
			#endif 
				VpItr = mapSipAccountIdToVpItr(p_resp_reg->line_id);	//p_resp_reg->line_id  is  SipAccountId
				LineItr = mapSipAccountIdToLineItr(p_resp_reg->line_id);
				
				VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
				VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
				VoiceProfLineObj_t* pLine = NULL;

				VOICEAPP_PRINTF("REGISTER CFM (SipAccountId):%d ,request_type is %d,result is %d\r\n ", p_resp_reg->line_id,p_resp_reg->request_type,p_resp_reg->result);
		      /* Waiting for successfull response of registration with registrar */
				if( p_resp_reg->result == ICF_FALSE) {
					VOICEAPP_PRINTF("Result is FAILURE, error cause is %d \n", p_resp_reg->error_cause);				
					if(p_resp_reg->error_cause == ICF_UNABLE_TO_SEND_REG_REQ){
						ZyPrintf("REGISTER CFM: ICF_UNABLE_TO_SEND_REG_REQ for SipAccountId %d.\n", p_resp_reg->line_id); 
					}
					if(p_resp_reg->error_cause == ICF_UNABLE_TO_SEND_DEREG_REQ){
						ZyPrintf("REGISTER CFM: ICF_UNABLE_TO_SEND_DEREG_REQ for SipAccountId %d.\n", p_resp_reg->line_id); 
					}
               /* If re-registrations is fail after a successful initial registration,voiceapp almost retry to register. Mitrastar, Ailsa, 20130313 */
					if(p_resp_reg->request_type != ICF_REQUEST_TYPE_DEREGISTER ||
						p_resp_reg->request_type != ICF_REQUEST_TYPE_DEREGISTERING)
					{
					if(p_resp_reg->error_cause == ICF_CAUSE_ONGOING_REGISTRATION) {
						ZyPrintf("REGISTER CFM: ONGOING REGISTRATION for SipAccountId %d.\n", p_resp_reg->line_id); 										
					}else if(p_resp_reg->error_cause == ICF_CAUSE_INVALID_REG_REQUEST){
						ZyPrintf("REGISTER CFM: INVALID_REG_REQUEST for SipAccountId %d.\n", p_resp_reg->line_id); 
					}
					else{					
						if(reg_timer[p_resp_reg->line_id].regRetryTimer!= 0){
							VOICEAPP_PRINTF("Stop regRetryTimer %d for SipAccountId %d.\n",reg_timer[p_resp_reg->line_id].regRetryTimer, p_resp_reg->line_id);						
							timerStop(reg_timer[p_resp_reg->line_id].regRetryTimer);
							reg_timer[p_resp_reg->line_id].regRetryTimer = 0;
						}
						VOICEAPP_PRINTF("%s> VoIPIsActive =[%u],ICFLineInitDone(%d) =[%u]\n",__FUNCTION__,VoIPIsActive,p_resp_reg->line_id,ICFLineInitDone[p_resp_reg->line_id]);
						if(!ICFLineInitDone[p_resp_reg->line_id]){
							VOICEAPP_PRINTF("%s> ICF line(%d) is inactive!\n",__FUNCTION__,p_resp_reg->line_id);
						}else	if(pProfSip==NULL){
							VOICEAPP_PRINTF("ICF_REGISTER_CFM> pProfSip is NULL!\r\n ");
						}else{
							reg_timer[p_resp_reg->line_id].regRetryTimer= timerStartS(1000*(pProfSip->registerRetryInterval), sigRegisterTimer, (void*)p_resp_reg->line_id);
							VOICEAPP_PRINTF("Try register after %d seconds for SipAccountId %d ,timer = %d. \n",pProfSip->registerRetryInterval,p_resp_reg->line_id, reg_timer[p_resp_reg->line_id].regRetryTimer);							
							/* Update the ccm status when the line status has changed, Mitrastar, Ailsa, 20130313*/
							pLine  = tr104VoiceLineObjPtrGet(VpItr, LineItr);
							if(NULL != pLine ){
								VOICEAPP_PRINTF("ICF_REGISTER_CFM: status:%d LineID:%d\n",pLine->status,p_resp_reg->line_id);	
								sipEvtToCCM(p_resp_reg->line_id, call_id, EVT_CCM_UNREG_SUCC, NULL, 0);
							}
							else{
								VOICEAPP_PRINTF("pLine is NULL!");
							}			
						}				
					}	
               }
               else
					{
						VOICEAPP_PRINTF("REGISTER CFM: request_type is DEREGISTER or DEREGISTERING \n");
					}		

					ret_val = ICF_FAILURE;										
				} else {
					if( p_resp_reg->request_type == ICF_REQUEST_TYPE_DEREGISTER){
						VOICEAPP_DBG("VOICEAPP> DEREGISTER_REQ successfull");	
						#if 0
						sipEvtToCCM(p_resp_reg->line_id, call_id, EVT_CCM_UNREG_SUCC, NULL, 0);				
						#endif	
							
						
				#if HEARTBEAT_MECHANISM_ENABLE 
						VOICEAPP_PRINTF("ICF_REGISTER_CFM : hearbeatOngoing = %d \r\n" ,heartbeatRecord[VpItr].hearbeatOngoing);
						VOICEAPP_PRINTF("ICF_REGISTER_CFM : timerID= %d  \r\n",heartbeatRecord[VpItr].timerID);		
						NumOfLine = tr104GetVPNumOfLine(VpItr);
						profileInUse = FALSE;
						for(i=0 ; i < NumOfLine ; i++){
							if( NULL!=(p_profileObj->lineProfPtrLst[i])){
								if(TR104_TRUE == (p_profileObj->lineProfPtrLst[i]->enable)){
									profileInUse=TRUE;
									break;
								}
							}
						}
					
						if(FALSE == profileInUse){
							if(heartbeatRecord[VpItr].hearbeatOngoing){
								timerStop(heartbeatRecord[VpItr].timerID);
								heartbeatRecord[VpItr].hearbeatOngoing = FALSE;
								heartbeatRecord[VpItr].timerID = 0;
							}
						}
						VOICEAPP_PRINTF("Change heartbeatRecord status");
						VOICEAPP_PRINTF("ICF_REGISTER_CFM : hearbeatOngoing = %d \r\n" ,heartbeatRecord[VpItr].hearbeatOngoing);
						VOICEAPP_PRINTF("ICF_REGISTER_CFM : timerID= %d \r\n",heartbeatRecord[VpItr].timerID);
					
				#endif
				
					} else {
						VOICEAPP_PRINTF("VOICEAPP> REGISTER REQ successfull \n");
						#if 0
						sipEvtToCCM(p_resp_reg->line_id, call_id, EVT_CCM_REG_SUCC, NULL, 0);
						#endif	
						if(reg_timer[p_resp_reg->line_id].subscribeTimer!= 0){
							VOICEAPP_PRINTF("Stop subscribeTimer %d for SipAccountId %d.\n",reg_timer[p_resp_reg->line_id].subscribeTimer, p_resp_reg->line_id);						
							timerStop(reg_timer[p_resp_reg->line_id].subscribeTimer);
							reg_timer[p_resp_reg->line_id].subscribeTimer = 0;
						}	

						pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);
						
						VOICEAPP_PRINTF("%s> VoIPIsActive =[%u],ICFLineInitDone(%d) =[%u]\n",__FUNCTION__,VoIPIsActive,p_resp_reg->line_id,ICFLineInitDone[p_resp_reg->line_id]);						
						if(!ICFLineInitDone[p_resp_reg->line_id]){
							VOICEAPP_PRINTF("%s> ICF line(%d) is inactive!\n",__FUNCTION__,p_resp_reg->line_id);
						}else	if(pLineFeature==NULL){
							ZyPrintf("pLineFeature is NULL!\n");						
						}else if(pLineFeature->MWIEnable){
							VOICEAPP_PRINTF("MWIEnable is Enable!\n");							
							reg_timer[p_resp_reg->line_id].subscribeTimer = timerStartS(1000*5, voiceAppSubscribeTimer, (void*)p_resp_reg->line_id);
						}
				#if HEARTBEAT_MECHANISM_ENABLE 
						if(p_profileObj->SIPObj->ZyXEL_HeartbeatEnable){ 					
							/* check the heartbeat mechanism for this Profile*/
							if(!heartbeatRecord[VpItr].hearbeatOngoing){
								VOICEAPP_PRINTF(" The first Line be added into this profile \r\n");
								VOICEAPP_PRINTF(" start the Heartbeat mechanism for this profile \r\n ");
								heartbeatRecord[VpItr].hearbeatOngoing = TRUE ;
								heartbeatRecord[VpItr].timerID = timerStartS(1000*rateOfHeartbeat, voiceAppHeartbeatOptionsTimer, (void*)VpItr);
								VOICEAPP_PRINTF("ICF_REGISTER_CFM : hearbeatOngoing = %d \r\n" ,heartbeatRecord[VpItr].hearbeatOngoing);
								VOICEAPP_PRINTF("ICF_REGISTER_CFM : timerID= %d  \r\n",heartbeatRecord[VpItr].timerID);							
							}else{
								VOICEAPP_PRINTF(" The Heartbeat mechanism for this profile is ongoing ");
								VOICEAPP_PRINTF("ICF_REGISTER_CFM : hearbeatOngoing = %d \r\n" ,heartbeatRecord[VpItr].hearbeatOngoing);
								VOICEAPP_PRINTF("ICF_REGISTER_CFM : timerID= %d  \r\n",heartbeatRecord[VpItr].timerID);							
							}
						}
				#endif		
					}

					ret_val = ICF_SUCCESS;
				}
				if (APICFMInitflag > 0) {
					voiceAppIPTKAcquireMutex();
					APICFMInitflag = APICFMInitflag - 1;
					VOICEAPP_PRINTF("%s,ICF_REGISTER_CFM : voiceapp-acif(dec)APICFMInitflag = %d!!\n" , __FUNCTION__,APICFMInitflag); 
					voiceAppIPTKReleaseMutex();
					VOICEAPP_PRINTF("VOICEAPP> REGISTER_REQ APICFMInitflag = %d\n" , APICFMInitflag);
				}

			}
			break;		
		case ICF_REGISTER_STATUS_IND: {
				/* IP Phone Toolkit sends unsolicited registration failure 
				* indications to the application. These are sent in case 
				* of network initiated de-registration or when 
				* re-registration attempts are failed. 
				* ?œIP Phone Toolkit??performs re-registrations on 
				* behalf of the application. Re-registrations are 
				* started automatically after a successful initial registration. 
				* Initial registration is triggered by the application using 
				* Register-Request API.
				*/
				icf_register_status_ind_st *p_reg_stat;
				int LineItr = 0 , VpItr=0;
#ifdef VOIP_SYSLOG				
				uint8 syslogmsg[TEXT_VOIP_SIZE-50]={0};
#endif
				p_reg_stat = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				VOICEAPP_PRINTF("receive ICF_REGISTER_STATUS_IND, SipAccountId %d\n", p_reg_stat->line_id);
				VpItr = mapSipAccountIdToVpItr(p_reg_stat->line_id);	//p_reg_stat->line_id  is SipAccountId
				LineItr = mapSipAccountIdToLineItr(p_reg_stat->line_id);
				VoiceProfSipObj_t* pProfSip = tr104VoiceProfSipObjPtrGet(VpItr);
				VoiceProfLineObj_t *p_lineObj = NULL;
				VOICEAPP_PRINTF("ICF_REGISTER_STATUS_IND (line ID):%d \r\n ", p_reg_stat->line_id);
				VOICEAPP_PRINTF("request_type = %d\n", p_reg_stat->request_type);
				VOICEAPP_PRINTF("result = %u \n", p_reg_stat->result);
				
				if( p_reg_stat->result == ICF_FALSE) {					
					/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/				
					voiceAppRecFailReReg(p_reg_stat->line_id,p_reg_stat->request_type,p_reg_stat->error_cause,p_reg_stat->response_code);			
					if(p_reg_stat->bit_mask & ICF_REG_IND_ERROR_CAUSE_PRESENT) {
						VOICEAPP_PRINTF("VOICEAPP> Error_cause = %u \r\n", p_reg_stat->error_cause,p_reg_stat->response_code);
						snprintf(syslogmsg,sizeof(syslogmsg)-1,"[STATUS IND]: %u/%u/%u/%u",p_reg_stat->line_id,p_reg_stat->request_type,p_reg_stat->error_cause,p_reg_stat->response_code);						
                  /* If re-registrations is fail after a successful initial registration,voiceapp almost retry to register. Mitrastar, Ailsa, 20130313 */
						if( p_reg_stat->request_type != ICF_REQUEST_TYPE_DEREGISTER ||
							 p_reg_stat->request_type != ICF_REQUEST_TYPE_DEREGISTERING )
							{
							if(p_reg_stat->error_cause != ICF_CAUSE_ONGOING_REGISTRATION ||
								p_reg_stat->error_cause != ICF_CAUSE_INVALID_REG_REQUEST )
								{
									if(reg_timer[p_reg_stat->line_id].regRetryTimer!= 0){
										VOICEAPP_PRINTF("Stop regRetryTimer %d for SipAccountId %d.\n",reg_timer[p_reg_stat->line_id].regRetryTimer, p_reg_stat->line_id);						
										timerStop(reg_timer[p_reg_stat->line_id].regRetryTimer);
										reg_timer[p_reg_stat->line_id].regRetryTimer = 0;
									}
									VOICEAPP_PRINTF("%s> VoIPIsActive =[%u],ICFLineInitDone(%d) =[%u]\n",__FUNCTION__,VoIPIsActive,p_reg_stat->line_id,ICFLineInitDone[p_reg_stat->line_id]);						
									if(!ICFLineInitDone[p_reg_stat->line_id]){
										VOICEAPP_PRINTF("%s> ICF line(%d) is inactive!\n",__FUNCTION__,p_reg_stat->line_id);
									}else	if(pProfSip==NULL){
										ZyPrintf("REGISTER_STATUS_IND> pProfSip is NULL!\n");						
									}else{
										reg_timer[p_reg_stat->line_id].regRetryTimer = timerStartS(1000*(pProfSip->registerRetryInterval), sigRegisterTimer, (void*)p_reg_stat->line_id);
										VOICEAPP_PRINTF("Try register after %d seconds for SipAccountId %d ,timer = %d. \n",pProfSip->registerRetryInterval,p_reg_stat->line_id,reg_timer[p_reg_stat->line_id].regRetryTimer);							
									}
								}
							}
                     else
							{
								VOICEAPP_PRINTF("REGISTER_STATUS_IND: request_type is DEREGISTER or DEREGISTERING \n");
						}
					} else {
						VOICEAPP_DBG("VOICEAPP> REGISTER_STATUS_IND FAILURE from Registrar!!!");
						snprintf(syslogmsg,sizeof(syslogmsg)-1,"[STATUS IND]: %u/%u",p_reg_stat->line_id,p_reg_stat->request_type);												
					}
					ret_val = ICF_FAILURE;

				#ifdef VOIP_SYSLOG						
					voipSyslog(LOG_SIP_REGISTRATION,LOG_ERR,NULL,0,syslogmsg);	
				#endif

					p_lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
				
					if(NULL != p_lineObj){
						VOICEAPP_PRINTF("ICF_REGISTER_STATUS_IND: p_lineObj->status = %u \n",p_lineObj->status);						
						if(p_lineObj->status == TR104I_STATUS_UP){
							/*set line status to registering to resend register in sigRegisterTimer*/
							p_lineObj->status = TR104I_STATUS_REGISTERING;
							/* Update the ccm status when the line status has changed, Mitrastar, Ailsa, 20130313*/
							VOICEAPP_PRINTF("ICF_REGISTER_STATUS_IND: status:%d LineID:%d\n",p_lineObj->status,p_reg_stat->line_id);
							sipEvtToCCM(p_reg_stat->line_id, call_id, EVT_CCM_UNREG_SUCC, NULL, 0);
						}
						else{
							p_lineObj->status = TR104I_STATUS_ERROR;
						}						
					}else{
						VOICEAPP_PRINTF("p_lineObj is NULL!");
					}
					
				}									
				#if 1 /*Jason ,*/
				else{
					VOICEAPP_PRINTF("REGISTER_STATUS_IND SUCCESS :p_reg_stat->result =%d " , p_reg_stat->result);
				}	
				#endif

				//Ssu-Ying, support Multiple Profile
				voiceAppStartCheckRegisterStatus();

#ifdef IPTK_REL_8_0
				icf_duration_t    retry_after_duration = 0;
				if((p_reg_stat->bit_mask)&ICF_REG_IND_RETRY_AFTER_DURATION_PRESENT) {
					retry_after_duration = p_reg_stat->retry_after_duration;
					ZyPrintf("REGISTER retry not implemented\n");
				}
#endif
			}
			break;
		case ICF_INCOMING_CALL_IND: {
				icf_incoming_call_ind_st	*p_incom_call = NULL;
				icf_line_id_t line_id;
				dialInfor_t dialInform;
				char sipUrl[ICF_MAX_STR_LEN];
#if 1 /*Jennifer, Anonymous Call Block*/
				int LineItr = 0, VpItr=0;

				/*AnonymousCallBlockEnable*/
				VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
#endif

				p_incom_call=(icf_incoming_call_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				line_id = p_incom_call->line_id;
#if 1 /*Jennifer, Anonymous Call Block*/
				VpItr = mapSipAccountIdToVpItr(p_incom_call->line_id);	//p_resp_reg->line_id  is  SipAccountId
				LineItr = mapSipAccountIdToLineItr(p_incom_call->line_id);
				pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);;
#endif
				VOICEAPP_DBG2("VOICEAPP> INCOMING_CALL_IND(call_id = ", call_id, ", line_id = ", line_id);
				
				zyMemset(&dialInform, 0, sizeof(dialInfor_t) );
				zyMemset(sipUrl, 0, sizeof(sipUrl) );
				if((p_incom_call->bit_mask)&ICF_CALLING_PARTY_ADD_PRESENT) {
					zyStrncpy(sipUrl, p_incom_call->calling_party_addr.addr_val.str, p_incom_call->calling_party_addr.addr_val.str_len);
					zyStrcpy(&(dialInform.dialDigitNumber.dial_digits), zyStrtok(sipUrl,(char *)"@\0"));
					dialInform.dialDigitNumber.current_length = zyStrlen(&(dialInform.dialDigitNumber.dial_digits));
					
					zyStrncpy(webSipStatus[line_id].lastIncomingNumber, dialInform.dialDigitNumber.dial_digits, WEB_SIP_STATUS_PHONENUMB_LENGTH);
					VOICEAPP_PRINTF("webSipStatus line %d, lastIncomingNumber = %s\n", line_id, webSipStatus[line_id].lastIncomingNumber);
					
				}
				if((p_incom_call->bit_mask)&ICF_CALLING_PARTY_NAME_PRESENT) {
					zyMemcpy(&(dialInform.dialDigitNumber.cname), p_incom_call->calling_party_name.str, min(zyStrlen(p_incom_call->calling_party_name.str), MAX_CNAME_LENGTH) );
				}
				if((p_incom_call->bit_mask)&ICF_CALLED_PARTY_ADD_PRESENT) {
					VOICEAPP_PRINTF("called_party_addr = %s\n", p_incom_call->called_party_addr.addr_val.str);
				}
#if 1 /*Jennifer, Anonymous Call Block*/
				if (pLineFeature != NULL){
					VOICEAPP_PRINTF("%s(): pLineFeature->AnonymousCallBlockEnable=%d \n", __FUNCTION__, pLineFeature->anonymousCallBlockEnable);
				}
				VOICEAPP_PRINTF("%s(): sipUrl = %s\n", __FUNCTION__, sipUrl);
				VOICEAPP_PRINTF("%s(): p_incom_call->calling_party_name.str = %s\n", __FUNCTION__, p_incom_call->calling_party_name.str);
#endif
				if((p_incom_call->bit_mask)&ICF_TRANSF_PARTY_ADD_PRESENT) {
					VOICEAPP_PRINTF("transferring_party_addr = %s\n", p_incom_call->transferring_party_addr.addr_val.str);
#if 1 /* ZyXEL Jacky, added for being transfer-target */
					if(being_replaced_callId != 0) {
						sipCCMConvertEntry_t *tmpEntry = NULL;
#if 1 /*Jennifer, Anonymous Call Block*/
						ccmConn_t *tmpCcmConn = NULL;
#endif
						tmpEntry = findSipCCMConvertByCallId(being_replaced_callId);
						if(tmpEntry != NULL) {
#if 1 /*Jennifer, Anonymous Call Block*/
							tmpCcmConn = findOrigCcmConn(SIP_PORT, tmpEntry->sipCcmPortId);
							if (tmpCcmConn != NULL){
								if (tmpCcmConn->connSt == CALL_CONN_ST_CONNIN){
									/*Call transfer:semi-attended, reject anonymous call when AnonymousCallBlockEnable is set to "1" and B meets the following criteria. */
									if(1 == determineAnonymousCallCriteria(p_incom_call)){
										VOICEAPP_DBG("Incoming call is anonymous, reject with 433 response!");
										voiceAppTerminateCall(call_id, line_id, ICF_CALL_CLEAR_REASON_UNDEFINED, 433);
										sipEvtToCCM(-1, being_replaced_callId, EVT_CCM_DISC_IND, NULL, 0);
									} else {
#endif
							dialInform.prevLogicId = tmpEntry->sipCcmPortId;
										sipEvtToCCM(tmpEntry->SipAccountId, call_id, EVT_CCM_ATTEND_CALL_TRANS_REPLACE, &dialInform, sizeof(dialInfor_t));
										sipEvtToCCM(-1, being_replaced_callId, EVT_CCM_DISC_IND, NULL, 0);
#if 1 /*Jennifer, Anonymous Call Block*/
									}
								} else {
									/*Call transfer:attended transfer, allow anonymous call.*/
									dialInform.prevLogicId = tmpEntry->sipCcmPortId;
									sipEvtToCCM(tmpEntry->SipAccountId, call_id, EVT_CCM_ATTEND_CALL_TRANS_REPLACE, &dialInform, sizeof(dialInfor_t));
							sipEvtToCCM(-1, being_replaced_callId, EVT_CCM_DISC_IND, NULL, 0);
								}
							} else {
								ZyPrintf("can't found SIP CCM\n");
							}
#endif
						} else {
							ZyPrintf("can't found being replaced SIP CCM convert\n");
						}
						being_replaced_callId = 0;
					} else {
#if 1 /*Jennifer, Anonymous Call Block*/
						/*Call transfer:unattended transfer, reject anonymous call when AnonymousCallBlockEnable is set to "1" and B meets the following criteria.*/
						if(1 == determineAnonymousCallCriteria(p_incom_call)){
							VOICEAPP_DBG("Incoming call is anonymous, reject with 433 response!");
							voiceAppTerminateCall(call_id, line_id, ICF_CALL_CLEAR_REASON_UNDEFINED, 433);
						} else {
#endif
						sipEvtToCCM(line_id, call_id, EVT_CCM_ASSIGN_REQ, &dialInform, sizeof(dialInfor_t));
					}
#if 1 /*Jennifer, Anonymous Call Block*/
					}
#endif
#endif					
#if 1 /*Jennifer, Anonymous Call Block*/
					} else if(1 == determineAnonymousCallCriteria(p_incom_call)){
					VOICEAPP_DBG("Incoming call is anonymous, reject with 433 response!");
					voiceAppTerminateCall(call_id, line_id, ICF_CALL_CLEAR_REASON_UNDEFINED, 433);
#endif
				} else {
					sipEvtToCCM(line_id, call_id, EVT_CCM_ASSIGN_REQ, &dialInform, sizeof(dialInfor_t));
				}
			}
			break;	
		case ICF_REMOTE_USER_ALERTED_IND: {
				icf_remote_user_alerted_ind_st *p_alert_ind = NULL;
				
				p_alert_ind=(icf_remote_user_alerted_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG1("VOICEAPP> REMOTE_USER_ALERTED_IND( inband_info_status = ", p_alert_ind->inband_info_status);

			#if 1	/* aircheng, for IPTK CSR No: 1-8780514*/
				if(ICF_TRUE == p_alert_ind->is_sdp_received){
			#else
				if(ICF_TRUE == p_alert_ind->inband_info_status){
			#endif
					sipEvtToCCM(-1, call_id, EVT_CCM_SDP_ALERT_IND, NULL, 0);
				} else{
					sipEvtToCCM(-1, call_id, EVT_CCM_ALERT_IND, NULL, 0);
				}
			}
			break;
		case ICF_TERMINATE_CALL_IND:{
				icf_terminate_call_ind_st *p_term_ind = NULL;
				icf_uint16_t   response_code = 0;
				icf_duration_t    retry_after_duration = 0;
				VOICEAPP_PRINTF("\n ICF_TERMINATE_CALL_IND EVENT\n");
#if 1	/*Jason ,[SPRID # 090622474]*/						
				sipCCMConvertEntry_t* entryTmp=NULL;
				entryTmp = findSipCCMConvertByCallId(call_id);				
				VOICEAPP_PRINTF("\n call_id = %d \n",call_id);
			
				if(NULL != entryTmp){
					VOICEAPP_PRINTF("\n entryTmp->SipAccountId =%d \n",entryTmp->SipAccountId);		
					if(oneShotEvent[entryTmp->SipAccountId].needRecover){
						recoverServiceParamForOneShotEvent(entryTmp->SipAccountId);
					}
					VOICEAPP_PRINTF("\n oneShotEvent[entryTmp->lineId].needRecover=%d \n",oneShotEvent[entryTmp->SipAccountId].needRecover);
					VOICEAPP_PRINTF("\n oneShotEvent[entryTmp->lineId].OneShotEventFlag=%d \n",oneShotEvent[entryTmp->SipAccountId].OneShotEventFlag);
				}
				else{
					VOICEAPP_PRINTF("findSipCCMConvertByCallId fail to find sipCCMConvert by callId %d\n", call_id);
				}
#endif
				
				p_term_ind=(icf_terminate_call_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				if((p_term_ind->bit_mask)&ICF_REMOTE_CALL_CLEAR_RESPONSE_CODE_PRESENT) {
					response_code = p_term_ind->response_code;
				}
				if((p_term_ind->bit_mask)&ICF_RETRY_AFTER_DURATION_PRESENT) {
					retry_after_duration = p_term_ind->retry_after_duration;
					ZyPrintf("CREATE_CALL retry not implemented\n");
				}
				VOICEAPP_DBG2("VOICEAPP> TERMINATE_CALL_IND( call_clear_reason = ", p_term_ind->call_clear_reason, ", response_code = ", p_term_ind->response_code);
				voiceAppTermCallResp(call_id);
				
				switch(p_term_ind->call_clear_reason) {
					case ICF_CALL_CLEAR_REASON_UNDEFINED:
					case ICF_CALL_CLEAR_REASON_REMOTE_RELEASED:
						sipEvtToCCM(-1, call_id, EVT_CCM_DISC_IND, NULL, 0);
						break;
					case ICF_CALL_CLEAR_REASON_REMOTE_REJECTED:
					case ICF_CALL_CLEAR_REASON_RESOURCES_NOT_AVAILABLE:
					case ICF_CALL_CLEAR_REASON_USER_NOT_FOUND:
					case ICF_CALL_CLEAR_REASON_CORRECT_PROXY_REQD:
					case ICF_CALL_CLEAR_REASON_PAYMENT_REQD:
					case ICF_CALL_CLEAR_REMOTE_UNREACHABLE:
					case ICF_CALL_CLEAR_REASON_NW_ERROR:
					case ICF_CALL_CLEAR_REMOTE_BUSY:
					case ICF_CALL_CLEAR_MEDIA_MISMATCH:
					case ICF_CALL_CLEAR_REASON_NW_REJECTED:
					case ICF_CALL_CLEAR_REASON_NOANSWER:
					case ICF_CALL_CLEAR_REASON_MAX_CALLS_EXCEEDED:		
						#ifdef NORWAY_VOIP_CUSTOMIZATION
							sipEvtToCCM(-1, call_id, EVT_CCM_REJ_IND, &response_code, sizeof(icf_uint16_t));
						#else					
						#ifdef OVERLAP_DIGIT						
						if((p_term_ind->response_code == 404) || (p_term_ind->response_code == 484)){
							sipEvtToCCM(-1, call_id, EVT_CCM_REJ_IND_INCOMPLETE, NULL, 0);
						}
						else{
							sipEvtToCCM(-1, call_id, EVT_CCM_REJ_IND, NULL, 0);
						}
						#else
						sipEvtToCCM(-1, call_id, EVT_CCM_REJ_IND, NULL, 0);						
						#endif
						#endif
						break;
					default:
						#ifdef NORWAY_VOIP_CUSTOMIZATION
							sipEvtToCCM(-1, call_id, EVT_CCM_REJ_IND, &response_code, sizeof(icf_uint16_t));
						#else
						sipEvtToCCM(-1, call_id, EVT_CCM_REJ_IND, NULL, 0);
						#endif
						break;
				}		
			}
			break;
		case ICF_REMOTE_USER_TERMINATE_CALL_RESP:{
				VOICEAPP_DBG("VOICEAPP> REMOTE_USER_TERMINATE_CALL_RESP");
				
#if 1	/*Jason ,[SPRID # 090622474]*/				
				sipCCMConvertEntry_t* entryTmp=NULL;
				entryTmp = findSipCCMConvertByCallId(call_id);		
				VOICEAPP_PRINTF("\n call_id = %d \n",call_id);		
			
				if(NULL != entryTmp){
					VOICEAPP_PRINTF("\n entryTmp->SipAccountId =%d \n",entryTmp->SipAccountId);		
					if(oneShotEvent[entryTmp->SipAccountId].needRecover){
						recoverServiceParamForOneShotEvent(entryTmp->SipAccountId);
					}
					VOICEAPP_PRINTF("\n oneShotEvent[entryTmp->lineId].needRecover=%d \n",oneShotEvent[entryTmp->SipAccountId].needRecover);
					VOICEAPP_PRINTF("\n oneShotEvent[entryTmp->lineId].OneShotEventFlag=%d \n",oneShotEvent[entryTmp->SipAccountId].OneShotEventFlag);
				}
				else{
					VOICEAPP_PRINTF("findSipCCMConvertByCallId fail to find sipCCMConvert by callId %d\n", call_id);
				}
#endif

#ifdef IPTK_REL_8_0
				icf_remote_user_term_call_resp_st *p_remote_term_resp = NULL;
				p_remote_term_resp = (icf_remote_user_term_call_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				if((p_remote_term_resp->bitmask) & ICF_ERROR_CAUSE_PRESENT) {
					VOICEAPP_DBG1("VOICEAPP> REMOTE_USER_TERMINATE_CALL_RESP FAILURE error_cause = ", p_remote_term_resp->error_cause);
				}
#endif
				sipEvtToCCM(-1, call_id, EVT_CCM_DISC_ACK, NULL, 0);
			}
			break;
		case ICF_MEDIA_CONNECTED_IND:{
				icf_media_connected_ind_st *p_media_ind = NULL;
				
				VOICEAPP_DBG("VOICEAPP> MEDIA_CONNECTED_IND");

				p_media_ind=(icf_media_connected_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				if(p_media_ind->p_accepted_stream_list != NULL){
					sipEvtToCCM(-1, call_id, EVT_CCM_SDP_ALERT_IND, NULL, 0);
				}
				else{
					sipEvtToCCM(-1, call_id, EVT_CCM_ALERT_IND, NULL, 0);
				}
			}
			break;
		case ICF_CONNECT_IND:{
				icf_list_st *acceptedList = NULL, *tmpList = NULL;
				icf_call_modify_media_data_st *mediaData = NULL;
				icf_codec_attrib_st *codecAttr = NULL;
				sipCCMConvertEntry_t *tmpEntry = NULL;
				p_payload = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG2("VOICEAPP> CONNECT_IND(callid = ", call_id, ", inband_dtmf_supported = ", ((icf_connect_ind_st*)p_payload)->inband_dtmf_supported);
				acceptedList=((icf_connect_ind_st*)p_payload)->p_accepted_stream_list;
				if(NULL != acceptedList){
					mediaData = (icf_call_modify_media_data_st *) acceptedList->p_data;

					if(NULL != mediaData){
						if(NULL != mediaData->p_codec_info_list){
							codecAttr = (icf_codec_attrib_st *)mediaData->p_codec_info_list->p_data;
							if(NULL != codecAttr){
								VOICEAPP_PRINTF("codec name = %s\n", codecAttr->codec_name.str);
								tmpEntry = findSipCCMConvertByCallId(call_id);
								if(NULL != tmpEntry){									
									#if 1 /*Eason, sipCallStatus*/
									snprintf(tmpEntry->sipCallStatus.codec, WEB_CALLSTATUS_CODEC_LENGTH, "%s", codecAttr->codec_name.str);
									VOICEAPP_PRINTF("ICF_CONNECT_IND, codec = %s\n", tmpEntry->sipCallStatus.codec);
									#endif									
								}
							}
						}						
					}
				}
				sipEvtToCCM(-1, call_id, EVT_CCM_CONN_IND, NULL, 0);
			}
			break;
		case ICF_INCOMING_CALL_MODIFY_IND:{
				icf_incoming_call_modify_ind_st *p_modify_ind = NULL;
				sipCCMConvertEntry_t* entryTmp=NULL;
				ccmConn_t* ccmConn_p = NULL;
				VOICEAPP_PRINTF("VOICEAPP> INCOMING_CALL_MODIFY_IND(callid = %d )" , call_id);			
				p_modify_ind=(icf_incoming_call_modify_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				entryTmp = findSipCCMConvertByCallId(call_id);	
				if(NULL != entryTmp){
					ccmConn_p = findOrigCcmConn(SIP_PORT, entryTmp->sipCcmPortId);
				}			
				if((p_modify_ind->bit_mask)&ICF_INC_CALL_ACCEPT_STREAM_LIST_PRESENT) {
					VOICEAPP_DBG("ACCEPT_STREAM_LIST_PRESENT");
					#if 1  /*Jason , [SPRID # 090624637]*/
					{
						icf_list_st	* list_tmp = p_modify_ind->p_active_stream_list ;
						while(NULL != list_tmp)
						{
							icf_call_modify_media_data_st *data_tmp = NULL;
							if(NULL != list_tmp->p_data){
								data_tmp = (icf_call_modify_media_data_st *)list_tmp->p_data;
								
								if(data_tmp->bit_mask&ICF_CALL_MOD_MEDIA_MODE_PRESENT){
									if(ccmConn_p!=NULL){
										if(ICF_MEDIA_MODE_RECV_ONLY == data_tmp->media_mode
											|| ICF_MEDIA_MODE_INACTIVE == data_tmp->media_mode)
										{								
											ccmConn_p->flags |= CALL_CONN_FLAG_HOLD ;
											if(NULL != entryTmp){
												entryTmp->sipCallStatus.status = TR104I_STATE_HOLD;
											}
										}
										else if(ICF_MEDIA_MODE_SEND_RECV == data_tmp->media_mode)
										{								
											ccmConn_p->flags &= ~CALL_CONN_FLAG_HOLD ;
											if(NULL != entryTmp){
												entryTmp->sipCallStatus.status = TR104I_STATE_INCALL;
											}
										}
									}
								}
							}
							list_tmp = list_tmp->p_next;
						}
					}
					#endif
				}
				if((p_modify_ind->bit_mask)&ICF_INC_CALL_MOD_DELETED_STREAM_PRESENT) {
					VOICEAPP_DBG("MOD_DELETED_STREAM_PRESENT");
				}
				if(p_modify_ind->confirm_required) {
					VOICEAPP_DBG("confirm_required");
					voiceAppCallModifyCfm(call_id);
					sipEvtToCCM(-1, call_id, EVT_CCM_MEDIA_CHANGE, NULL, 0);
				}
			}
			break;
		case ICF_INCOMING_CALL_MODIFY_CANCEL_IND:
			break;
		case ICF_CALL_MODIFY_RESP:
			break;
		case ICF_PUBLISH_RESP:
			break;
		case ICF_MESSAGE_RESP:
			break;
		case ICF_OPTIONS_RESP:{
#if HEARTBEAT_MECHANISM_ENABLE		
				voiceAppHeartbeatOptionsCheckResp(p_msg);
#endif	
			}			
			break;
		case ICF_REFER_RESP:
			break;
		case ICF_CALL_HOLD_RESP: {
				icf_api_resp_st *p_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> CALL_HOLD_RESP");
				VOICEAPP_PRINTF("VOICEAPP> CALL_HOLD_RESP(callid = %d )" , call_id);	
				ccmConn_t* ccmConn_p = NULL;
				sipCCMConvertEntry_t* entryTmp=NULL;
				p_resp = (icf_api_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				entryTmp = findSipCCMConvertByCallId(call_id);	
				if(NULL != entryTmp){
					ccmConn_p = findOrigCcmConn(SIP_PORT, entryTmp->sipCcmPortId);
				}	
				
				/* The application needs to run the timer and send the request again after its expiry */
				if((p_resp->bit_mask)&ICF_RETRY_AFTER_DURATION_PRESENT) {
					VOICEAPP_DBG1("retry_after_duration = ", p_resp->retry_after_duration);
					ZyPrintf("CALL_HOLD retry not implemented\n");
				}
				
				if(p_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> CALL_HOLD_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_resp->error_cause);
				} else {
					if(!(ccmConn_p->flags &CALL_CONN_FLAG_HOLD)){								
						ccmConn_p->flags |= CALL_CONN_FLAG_HOLD ;
						if(NULL != entryTmp){
							entryTmp->sipCallStatus.status = TR104I_STATE_HOLD;
						}
					}
					sipEvtToCCM(-1, call_id, EVT_CCM_ONHOLD_CONF, NULL, 0);
				}
			}
			break;
		case ICF_CALL_RESUME_RESP:{
				icf_api_resp_st *p_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> CALL_RESUME_RESP");
				VOICEAPP_PRINTF("VOICEAPP> CALL_HOLD_RESP(callid = %d )" , call_id);	
				ccmConn_t* ccmConn_p = NULL;
				sipCCMConvertEntry_t* entryTmp=NULL;				
				p_resp = (icf_api_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
	
				entryTmp = findSipCCMConvertByCallId(call_id);	
				if(NULL != entryTmp){
					ccmConn_p = findOrigCcmConn(SIP_PORT, entryTmp->sipCcmPortId);
				}	
				/* The application needs to run the timer and send the request again after its expiry */
				if((p_resp->bit_mask)&ICF_RETRY_AFTER_DURATION_PRESENT) {
					VOICEAPP_DBG1("retry_after_duration = ", p_resp->retry_after_duration);
					ZyPrintf("CALL_RESUME retry not implemented\n");
				}
				
				if(p_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> CALL_RESUME_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_resp->error_cause);					
					if(p_resp->error_cause == ICF_CAUSE_ANOTHER_CALL_ACTIVE_ON_SAME_LINE){
						VOICEAPP_DBG1("error_cause ANOTHER_CALL_ACTIVE_ON_SAME_LINE, send Resume Request again, call_id = ", call_id);
						voiceAppCallResume(call_id);
					}
				} else {
					if(ccmConn_p->flags &CALL_CONN_FLAG_HOLD){								
						ccmConn_p->flags &= ~CALL_CONN_FLAG_HOLD ;
					}
					if(NULL != entryTmp){
						entryTmp->sipCallStatus.status = TR104I_STATE_INCALL;
					}
					sipEvtToCCM(-1, call_id, EVT_CCM_UNHELD_CONF, NULL, 0);
				}
			}
			break;
		case ICF_CALL_TRANSFER_ATTENDED_RESP:{				
				icf_cta_resp_st *p_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> ICF_CALL_TRANSFER_ATTENDED_RESP");
				
				p_resp = (icf_msg_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
		
				if(p_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> ICF_CALL_TRANSFER_ATTENDED_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_resp->error_cause);					
				} 
			}
			break;
		case ICF_CALL_TRANSFER_UNATTENDED_RESP:{				
				icf_ctu_resp_st *p_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> ICF_CALL_TRANSFER_UNATTENDED_RESP");
				
				p_resp = (icf_msg_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
		
				if(p_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> ICF_CALL_TRANSFER_UNATTENDED_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_resp->error_cause);					
				} 
			}
			break;
		case ICF_REMOTE_CALL_TRANSFER_INITIATED_IND:{
				icf_remote_call_transfer_initiated_ind_st *p_transfer_ind = NULL;
				char *token;
				dialInfor_t dialInform;
				sipCCMConvertEntry_t *tmpEntry = NULL;
				
				VOICEAPP_DBG("VOICEAPP> ICF_REMOTE_CALL_TRANSFER_INITIATED_IND");
				p_transfer_ind = (icf_remote_call_transfer_initiated_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG2("ori callId = ", call_id, "new callId = ", p_transfer_ind->new_call_id);

				zyMemset(&dialInform, 0, sizeof(dialInfor_t) );
				token = zyStrtok(p_transfer_ind->referred_party_addr.addr_val.str, "@");
				zyMemcpy(&(dialInform.dialDigitNumber.dial_digits[0]), token, zyStrlen(token));
				dialInform.dialDigitNumber.current_length = zyStrlen(token);
				voiceAppRemoteCallTransferCfm(call_id);

				tmpEntry = findSipCCMConvertByCallId(call_id);
				if(tmpEntry != NULL) {
					dialInform.prevLogicId = tmpEntry->sipCcmPortId;
				
					sipEvtToCCM(tmpEntry->SipAccountId, p_transfer_ind->new_call_id, EVT_CCM_CALL_TRANS_IND, &dialInform, sizeof(dialInfor_t));
				} else {
					ZyPrintf("ICF_REMOTE_CALL_TRANSFER_INITIATED_IND : can't found SIP CCM convert!\n");
				}
			}
			break;
		case ICF_CALL_MERGED_IND:
			break;
		case ICF_CONFERENCE_RESP:{
				icf_msg_resp_st *p_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> ICF_CONFERENCE_RESP");
				
				p_resp = (icf_msg_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
		
				if(p_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> ICF_CONFERENCE_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_resp->error_cause);
					if(p_resp->error_cause != 0){
						sipEvtToCCM(-1, call_id, EVT_CCM_CONFCALL_MEETROOM_FAIL, NULL, 0);
					}
				} 
			}
			break;
		case ICF_SUBSCRIPTION_STATUS_IND: {
				icf_subscription_status_ind_st *p_subscription_stat;

				p_subscription_stat = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG2("VOICEAPP> SUBSCRIPTION_STATUS_IND status = ", p_subscription_stat->status, ", duration = ", p_subscription_stat->duration);
				switch(p_subscription_stat->status) {
					case ICF_SUBS_ACCEPTED:
					case ICF_SUBS_SUBSCRIBED:
						VOICEAPP_DBG("VOICEAPP> SUBSCRIBE_REQ accepted");
						ret_val = ICF_SUCCESS;
						break;
					case ICF_SUBS_FAIL_RETRY:
					case ICF_SUBS_FAIL_NO_RETRY:
						VOICEAPP_DBG("VOICEAPP> SUBSCRIBE_REQ failed");
						ret_val = ICF_FAILURE;
						break;
					case ICF_SUBS_UNSUBSCRIBED:
						VOICEAPP_DBG("VOICEAPP> UNSUBSCRIBED");
						ret_val = ICF_SUCCESS;
						break;
					default:
						break;
				}
			}
			break;
		case ICF_INFO_RESP:
			break;
		case ICF_CONNECT_RESP:
			{
				icf_list_st *acceptedList = NULL, *tmpList = NULL;
				icf_call_modify_media_data_st *mediaData = NULL;
				icf_codec_attrib_st *codecAttr = NULL;
				sipCCMConvertEntry_t *tmpEntry = NULL;
				p_payload = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG1("VOICEAPP> CONNECT_RESP(callid = ", call_id);
				acceptedList=((icf_connect_resp_st*)p_payload)->p_accepted_stream_list;
				if(NULL != acceptedList){
					mediaData = (icf_call_modify_media_data_st *) acceptedList->p_data;

					if(NULL != mediaData){
						if(NULL != mediaData->p_codec_info_list){
							codecAttr = (icf_codec_attrib_st *)mediaData->p_codec_info_list->p_data;
							if(NULL != codecAttr){
								VOICEAPP_PRINTF("codec name = %s\n", codecAttr->codec_name.str);
								tmpEntry = findSipCCMConvertByCallId(call_id);
								if(NULL != tmpEntry){
									#if 1 /*Eason, sipCallStatus*/
									snprintf(tmpEntry->sipCallStatus.codec, WEB_CALLSTATUS_CODEC_LENGTH, "%s", codecAttr->codec_name.str);										
									#endif
									
								}
							}
						}						
					}
				}					
				sipEvtToCCM(-1, call_id, EVT_CCM_CONN_CONF, NULL, 0);
			}
			break;
		case ICF_CALL_REDIRECTION_IND:
			break;
		case ICF_CALL_LOG_RESP:
			break;
		case ICF_PARTY_REPLACED_IND:{
				icf_party_replaced_ind_st	*p_replaced_call = NULL;
				dialInfor_t dialInform;
				sipCCMConvertEntry_t *tmpEntry = NULL;

				p_replaced_call=(icf_party_replaced_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				VOICEAPP_DBG2("VOICEAPP> ICF_PARTY_REPLACED_IND call_id = ", call_id, ", replacing callId = ", p_replaced_call->replacing_call_id);

				being_replaced_callId = call_id;
#if 0 /* ZyXEL Jacky, moved to ICF_INCOMING_CALL_IND case */
				zyMemset(&dialInform, 0, sizeof(dialInfor_t) );
				
				if((p_replaced_call->bit_mask)&ICF_PARTY_REPLACED_NAME) {
					zyMemcpy(&(dialInform.dialDigitNumber.dial_digits), p_replaced_call->replacing_party_name.str, min(zyStrlen(p_replaced_call->replacing_party_name.str), ICF_MAX_STR_LEN) );
					dialInform.dialDigitNumber.current_length = min(zyStrlen(p_replaced_call->replacing_party_name.str), ICF_MAX_STR_LEN);
				}

				tmpEntry = findSipCCMConvertByCallId(call_id);
				if(tmpEntry != NULL) {
					dialInform.prevLogicId = tmpEntry->sipCcmPortId;
					sipEvtToCCM(tmpEntry->lineId, p_replaced_call->replacing_call_id, EVT_CCM_ATTEND_CALL_TRANS_REPLACE, &dialInform, sizeof(dialInfor_t));
					sipEvtToCCM(-1, call_id, EVT_CCM_DISC_IND, NULL, 0);
				} else {
					ZyPrintf("ICF_PARTY_REPLACED_IND can't found SIP CCM convert\n");
				}
#endif
			}
			break;
		case ICF_START_FAX_RESP: {
				icf_start_fax_resp_st *p_start_fax_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> START_FAX_RESP");
				
				p_start_fax_resp = (icf_start_fax_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				if(p_start_fax_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> START_FAX_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_start_fax_resp->error_cause);
				} else {
					VOICEAPP_DBG1("fax_type = ", p_start_fax_resp->fax_type);
					sipEvtToCCM(-1, call_id, EVT_CCM_MEDIA_CHANGE, NULL, 0);
				}
				if((p_start_fax_resp->bit_mask)&ICF_RETRY_AFTER_DURATION_PRESENT) {
					VOICEAPP_DBG1("retry_after_duration = ", p_start_fax_resp->retry_after_duration);
					ZyPrintf("START_FAX retry not implemented\n");
				}
			}
			break;
		case ICF_START_FAX_IND: {
				icf_start_fax_ind_st *p_start_fax_ind = NULL;

				VOICEAPP_DBG("VOICEAPP> START_FAX_IND");
				
				p_start_fax_ind = (icf_start_fax_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG1("fax_type = ", p_start_fax_ind->fax_type);
				sipEvtToCCM(-1, call_id, EVT_CCM_MEDIA_CHANGE, NULL, 0);
			}
			break;
		case ICF_NOTIFY_CFM:{
				VOICEAPP_DBG("VOICEAPP> NOTIFY_CFM");
			}
			break;
		case ICF_OPTIONS_CFM:
			break;
		case ICF_INFO_IND:{
				dialDigitBuf_t *dialDigitBuf ;
				uint8 digit, volume = 0;
				uint16 duration;

				VOICEAPP_DBG("VOICEAPP>ICF_INFO_IND");

				icf_info_ind_st *p_info_ind = NULL;
			
				p_info_ind=(icf_info_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				voiceAppInfoCfm(call_id);
				if((p_info_ind->bit_mask)&ICF_BODY_LIST_PRESENT){
					dialDigitBuf = (dialDigitBuf_t *)zyMalloc(sizeof(dialDigitBuf_t));
					if(NULL == dialDigitBuf) {
						return FALSE;
					}
					zyMemset(dialDigitBuf, 0, sizeof(dialDigitBuf_t) );
					if((p_info_ind->bit_mask)&ICF_BODY_LIST_PRESENT) {
						GetInfoDtmf(p_info_ind, &digit, &volume, &duration);
						VOICEAPP_DBG1("digit= ",digit );
						VOICEAPP_DBG1("volume= ", volume );
						VOICEAPP_DBG1("duration= ",duration );
						dialDigitBuf->digitStr = digit;
						dialDigitBuf->duration = duration;
						dialDigitBuf->volume = volume;
					}

					sipEvtToCCM(-1, call_id, EVT_CCM_DIAL_DIGIT, dialDigitBuf, sizeof(dialDigitBuf_t));
					zyFree(dialDigitBuf);
				}else{
					VOICEAPP_DBG("INFO_IND BODY_LIST not present");
				}
			}
			break;
		case ICF_CALL_FORWARDED_IND:
			VOICEAPP_DBG("ICF_CALL_FORWARDED_IND");
			break;
		case ICF_INFO_CFM:
			break;
		case ICF_NOTIFY_IND:{
				icf_notify_ind_st *p_notify_ind = NULL;
				icf_line_id_t SipAccountId;
				SdpSessMWI sdpMWI=NULL;
				uint32 voiceCount;
				VoiceProfLineObj_t *lineObj = NULL;
				VoiceProfLineCallingFeaturesObj_t *callfeaturesObj = NULL;
				int LineItr = 0, VpItr=0;


				VOICEAPP_DBG("VOICEAPP> ICF_NOTIFY_IND");
				voiceAppNotifyCfm(call_id);
				
				p_notify_ind=(icf_notify_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				SipAccountId = p_notify_ind->line_id;

				VOICEAPP_PRINTF("receive ICF_NOTIFY_IND, SipAccountId %d\n", SipAccountId);
				VpItr = mapSipAccountIdToVpItr(SipAccountId);
				LineItr = mapSipAccountIdToLineItr(SipAccountId);
				lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
				
				if(lineObj==NULL){
					ZyPrintf("lineObj is NULL!");
				}else{
					callfeaturesObj = lineObj->callingFeatures;

					if(p_notify_ind->body_list.body == NULL){
						ZyPrintf("No Body presented!! \n");						
					}else if(p_notify_ind->body_list.body->value == NULL){
						ZyPrintf("No body->value presented!! \n");
					}else if ( (sdpMWI = sdpMWINewFromText(p_notify_ind->body_list.body->value) ) != NULL ){
						voiceCount = sdpMWIGetNewVoiceMailCount(sdpMWI);
						if (voiceCount > 0){
							VOICEAPP_DBG("VOICEAPP> ICF_NOTIFY_IND sipEvtToCCM--EVT_CCM_NEW_VOICE_MESSAGE");
							if(NULL != callfeaturesObj) {
								callfeaturesObj->messageWaiting = TRUE;
							}
							setNewVoiceMessage(SipAccountId, TRUE);
#ifdef VISUAL_MWI					
							sipEvtToCCM(SipAccountId, call_id, EVT_CCM_NEW_VOICE_MESSAGE, NULL, 0);
#endif
						} else {
							VOICEAPP_DBG("VOICEAPP> ICF_NOTIFY_IND sipEvtToCCM--EVT_CCM_NO_NEW_VOICE_MESSAGE");
							if(NULL != callfeaturesObj) {
								callfeaturesObj->messageWaiting = FALSE;
							}
							setNewVoiceMessage(SipAccountId, FALSE);
#ifdef VISUAL_MWI					
							sipEvtToCCM(SipAccountId, call_id, EVT_CCM_NO_NEW_VOICE_MESSAGE, NULL, 0);
#endif
						}
						voipLedOnOff( tr104GetNumOfPhyIntf() );
						
						sdpMWIFree(sdpMWI);
					}
				}
			}
			break;			
		case ICF_EARLY_INC_CALL_IND:
			break;
		case ICF_EARLY_INC_CALL_CFM:
			break;
		case ICF_STOP_FAX_RESP: {
				icf_stop_fax_resp_st *p_stop_fax_resp = NULL;

				VOICEAPP_DBG("VOICEAPP> STOP_FAX_RESP");
				
				p_stop_fax_resp = (icf_stop_fax_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				if(p_stop_fax_resp->result == ICF_FAILURE) {
					VOICEAPP_DBG("VOICEAPP> STOP_FAX_RESP FAILURE");
					VOICEAPP_DBG1("error_cause = ", p_stop_fax_resp->error_cause);
				} else {
					VOICEAPP_DBG("VOICEAPP> STOP_FAX_RESP SUCCEED");
				}
			}
			break;
		case ICF_STOP_FAX_IND: {
				icf_stop_fax_ind_st *p_stop_fax_ind = NULL;

				VOICEAPP_DBG("VOICEAPP> STOP_FAX_IND");
				
				p_stop_fax_ind = (icf_stop_fax_ind_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG1("fax_type = ", p_stop_fax_ind->fax_type);
				sipEvtToCCM(-1, call_id, EVT_CCM_MEDIA_CHANGE, NULL, 0);
			}
			break;
		case ICF_UNEXPECTED_MSG_IND:
			{
   				icf_error_t error_cause;
				p_payload = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				error_cause = ((icf_unexpected_msg_ind_st*)p_payload)->error_cause;
				ZyPrintf("VOICEAPP> UNEXPECTED_MSG_IND error_cause = %d\n", error_cause);
				if(error_cause == ICF_CAUSE_INVALID_CALL_ID) {
					ZyPrintf("error_cause = ICF_CAUSE_INVALID_CALL_ID\n");
				}
			}
			break;
		case ICF_REMOTE_CALL_TRANSFER_INITIATED_CFM:
			break;
		case ICF_CALL_MODIFY_CANCEL_RESP:
			break;
		case ICF_RELEASE_XCONF_IND:
			break;
		case ICF_CREATE_XCONF_RESP:
			break;
		case ICF_ADD_XCONF_PARTY_RESP:
			break;
		case ICF_DELETE_XCONF_PARTY_RESP:
			break;
		case ICF_RELEASE_XCONF_RESP:
			break;
		case ICF_PROGRESS_IND:
			{
				p_payload = VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_DBG1("VOICEAPP> PROGRESS_IND(response_code = ", ((icf_progress_ind_st*)p_payload)->response_code);
				sipEvtToCCM(-1, call_id, EVT_CCM_ALERT_IND, NULL, 0);
			}
			break;
#ifdef ICF_TRACE_ENABLE
		case ICF_REPORT_TRACE_IND:
			/* This API is sent by IPTK for reporting traces to the
			* VoiceApp application.
			*/
			{
				icf_report_trace_ind_st	*p_resp_trace;
				p_resp_trace=(icf_report_trace_ind_st * )VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

				ZyPrintf("trace ID %d\n", p_resp_trace->trace_id);
				ZyPrintf("trace type %d\n", p_resp_trace->trace_type);
				ZyPrintf("\n\nTRACE STRING : %s",p_resp_trace->trace_data.trc_string.str);

				switch(p_resp_trace->trace_id) {
					case ICF_TRACE_APP_MESG:
					case ICF_TRACE_MEDIA_MESG:
					case ICF_TRACE_SIP_PDU:
						{
							icf_generic_string_st *p_trace_data;
							p_trace_data = (icf_generic_string_st *)(p_resp_trace->trace_data.p_var_data);
							ZyPrintf("%s\n\n",p_trace_data->p_buff);
						}
						break;
					default:
						break;
				}
			}
			break;
#endif
#ifdef ICF_ERROR_ENABLE
		case ICF_REPORT_ERROR_IND:
			{
				icf_report_error_ind_st	*p_error_report;
			    	p_error_report=(icf_report_error_ind_st * )VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_PRINTF("ICF_REPORT_ERROR_IND: id %d ,%s\n", p_error_report->error_id,p_error_report->error_data.err_string.str);	
				/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/	
				voiceAppRecICFErr(p_error_report->error_id,p_error_report->error_data.err_string.str);

			}
			break;
#endif
		case ICF_GET_ICF_STATS_RESP:
			break;
		case ICF_CHECK_MODULE_STATUS_RESP:
			break;
		case ICF_RESTART_RESP:
			break;
		case ICF_REINIT_RESP:
			break;
		case ICF_REINIT_IND:
			break;
		case ICF_STATS_RESP:
			break;
		/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/	
		case ICF_DNS_MAPPING_CHANGE_IND:{
				#ifdef VOIP_SYSLOG				
				uint8 syslogmsg[TEXT_VOIP_SIZE-50]={0};
				#endif
				icf_dns_mapping_change_ind_st *p_dns_ind=NULL;
				p_dns_ind=(icf_dns_mapping_change_ind_st * )VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				VOICEAPP_PRINTF("ICF_DNS_MAPPING_CHANGE_IND: %u/%u/%s/%s\n",p_dns_ind->record_type,p_dns_ind->cause,p_dns_ind->fqdn.str,p_dns_ind->ip.str);

				voiceAppRecDNSMap(p_dns_ind->record_type,p_dns_ind->cause,p_dns_ind->fqdn.str,p_dns_ind->ip.str);

				#ifdef VOIP_SYSLOG
				snprintf(syslogmsg,sizeof(syslogmsg)-1,"[DNS_MAPPING]: %u/%u",p_dns_ind->record_type,p_dns_ind->cause);							
				voipSyslog(LOG_OTHERS,LOG_INFO,NULL,0,syslogmsg);	
				memset(syslogmsg,0,sizeof(syslogmsg));
				snprintf(syslogmsg,sizeof(syslogmsg)-1," %s/%s ",p_dns_ind->fqdn.str,p_dns_ind->ip.str);	
				voipSyslog(LOG_OTHERS,LOG_INFO,NULL,0,syslogmsg);
				#endif

			}
			break;
		/* control plane APIs end */
		/* media plane APIs */
		case ICF_CFG_SET_PORT_RANGE_RESP:
			VOICEAPP_DBG("CFG_SET_PORT_RANGE_RESP");
			break;
		case ICF_CFG_SET_SELF_IP_RESP:
			VOICEAPP_DBG("CFG_SET_SELF_IP_RESP");
			break;
		/* media plane APIs end */
		/* IPsec APIs */
		case ICF_PORT_PLTFM_SA_CREATE_RESP:
			break;
		case ICF_PORT_PLTFM_SA_MODIFY_RESP:
			break;
		case ICF_PORT_PLTFM_SA_DELETE_RESP:
			break;
		case ICF_PORT_PLTFM_SA_FETCH_ENC_ALGO_RESP:
			break;
		case ICF_PORT_PLTFM_SA_FETCH_AUTH_ALGO_RESP:
			break;
		case ICF_TRACE_NW_BUFF_IND:
#ifdef VOIP_SYSLOG
	
			{
				
				icf_trace_nw_buff_ind_st	*p_trace_nw_buff;
			    	p_trace_nw_buff=(icf_trace_nw_buff_ind_st * )VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);
				#if 1/*Below is the symptom of the bug fix of the unregister issue, __P876__,Ailsa, 20120326 */
				if(p_trace_nw_buff->nw_buffer.len >= 69) {
					sipEvtToVoIPLOG(call_id, p_trace_nw_buff->nw_buffer.p_buff, 69);
				} else {
					sipEvtToVoIPLOG(call_id, p_trace_nw_buff->nw_buffer.p_buff, p_trace_nw_buff->nw_buffer.len);
				}
				#endif
				VOICEAPP_PRINTF("\r\n### p_trace_nw_buf payload len is %d\n",p_trace_nw_buff->nw_buffer.len);
				VOICEAPP_PRINTF("\r\n### p_trace_nw_buf payload is %s\n",p_trace_nw_buff->nw_buffer.p_buff);
				
			}
#endif			
			VOICEAPP_DBG("TRACE_NW_BUFF_IND");
			break;
		/* IPsec APIs end */
		default:
			VOICEAPP_DBG1("voiceAppIPTKMsgProcess receive other command api_id = ", api_id);
			break;
	}

	icf_free_decoded_msg((icf_msg_st *)p_msg,&ecode);

	if(ret_val==ICF_FAILURE) {
		return(FALSE);
	}

	return(TRUE);
}
/*Enhance Debug log for ZyIMS,Mitrastar,Jason,2013.01.29*/	
void voiceAppRecICFErr(uint32 error_id, char *error_str)
{
	uint8 lastIdx=0,nextIdx=0; 
	VOICEAPP_PRINTF("%s >ENTER\n",__FUNCTION__);

	nextIdx = voiceAppICFErrRec.errIndNext;

	if(nextIdx != 0){
		lastIdx = nextIdx -1;
	}else{
		lastIdx = VOICEAPP_ICF_MAX_ERR_REC -1;
	}
	if(error_str!=NULL){
		VOICEAPP_PRINTF("error id/str: %d/%s \n",error_id,error_str);
	}else{
		VOICEAPP_PRINTF("error_id: %d \n",error_id);
	}
	
	if(voiceAppICFErrRec.lastErrInd[lastIdx].error_id != error_id)
	{
		memset(&(voiceAppICFErrRec.lastErrInd[nextIdx]),0,sizeof(voiceAppICF_Err_ind_t));
		voiceAppICFErrRec.lastErrInd[nextIdx].error_id= error_id;
		if(error_str!=NULL){
			strncpy(voiceAppICFErrRec.lastErrInd[nextIdx].error_str,error_str,ICF_MAX_STR_LEN-1);
		}

		/*Update errIndNext*/
		voiceAppICFErrRec.errIndNext++;
		if(voiceAppICFErrRec.errIndNext >= VOICEAPP_ICF_MAX_ERR_REC){
			voiceAppICFErrRec.errIndNext = 0;
		}	
	}
}

void voiceAppRecFailReReg(uint16 line_id,uint8 request_type,uint16 error_cause,uint16  response_code)
{
	uint8 nextIdx=0; 
	VOICEAPP_PRINTF("%s >ENTER\n",__FUNCTION__);
	VOICEAPP_PRINTF("== %u/%u/%u/%u\n",line_id,request_type,error_cause,response_code);	

	nextIdx = voiceAppICFErrRec.failreRegNext;

	if(nextIdx < VOICEAPP_ICF_MAX_REREG_REC){
		memset(&(voiceAppICFErrRec.lastReRegErr[nextIdx]),0,sizeof(voiceAppICF_Fail_rereg_t));
		/*Recode the Error indication*/
		voiceAppICFErrRec.lastReRegErr[nextIdx].line_id = line_id;
		voiceAppICFErrRec.lastReRegErr[nextIdx].request_type = request_type;						
		voiceAppICFErrRec.lastReRegErr[nextIdx].error_cause = error_cause;
		voiceAppICFErrRec.lastReRegErr[nextIdx].response_code = response_code;
		voiceAppICFErrRec.lastReRegErr[nextIdx].timerec = time(NULL);
	}
	voiceAppICFErrRec.failreRegNext++;
	if(voiceAppICFErrRec.failreRegNext >= VOICEAPP_ICF_MAX_REREG_REC){
		voiceAppICFErrRec.failreRegNext = 0;
	}

}


void voiceAppRecDNSMap(uint8 record_type,uint8 cause,char* fqdn_str, char* ip_str)
{
	uint8 nextIdx=0; 
	VOICEAPP_PRINTF("%s >ENTER\n",__FUNCTION__);
	VOICEAPP_PRINTF("== %u/%u \n",record_type,cause);	

	nextIdx = voiceAppICFErrRec.DNSMapChgNext;

	if(nextIdx<VOICEAPP_ICF_MAX_DNS_REC){
		memset(&(voiceAppICFErrRec.lastDNSMapChgInd[nextIdx]),0,sizeof(voiceAppICF_Dnsmapchg_ind_t));
		voiceAppICFErrRec.lastDNSMapChgInd[nextIdx].record_type = record_type;
		voiceAppICFErrRec.lastDNSMapChgInd[nextIdx].cause = cause; 
		if(fqdn_str!= NULL){
			strncpy(voiceAppICFErrRec.lastDNSMapChgInd[nextIdx].fqdn, fqdn_str,ICF_MAX_STR_LEN-1);	
		}
		if(ip_str!=NULL){
			strncpy(voiceAppICFErrRec.lastDNSMapChgInd[nextIdx].ip, ip_str,ICF_MAX_STR_LEN-1); 
		}
	}
	/*Update DNSMapChgNext*/
	voiceAppICFErrRec.DNSMapChgNext++;
	if(voiceAppICFErrRec.DNSMapChgNext >= VOICEAPP_ICF_MAX_DNS_REC){
		voiceAppICFErrRec.DNSMapChgNext = 0;
	}

}


/*************************************************************************
*
*  FUNCTION NAME  : sigRegisterTimer
*
*  DESCRIPTION    : This function will execute voiceAppSendRegister(0, line_id) when timeout
*
*  RETURNS		  : voiceAppSendRegister(0, line_id) return value
*************************************************************************/
int
sigRegisterTimer(
	tm_id_t unused, /* timerid */
	void	*vp
)
{
	int	SipAccountId = (int )vp;
	int result = FALSE;	
	int LineItr = 0, VpItr=0;

	VpItr = mapSipAccountIdToVpItr(SipAccountId);
	LineItr = mapSipAccountIdToLineItr(SipAccountId);
	VoiceProfLineObj_t *lineObj = tr104VoiceLineObjPtrGet(VpItr, LineItr);
	VOICEAPP_PRINTF("%s,timer id = %d , %d\n",__FUNCTION__,unused,reg_timer[SipAccountId].regRetryTimer);
	reg_timer[SipAccountId].regRetryTimer =0;			
	
	if(NULL != lineObj){	
		VOICEAPP_PRINTF("%s, line status = %d\n",__FUNCTION__ , lineObj->status);
		if(TR104I_STATUS_REGISTERING== lineObj->status){
			result = voiceAppSendRegister(SipAccountId);
		}
		else if(TR104I_STATUS_UNREGISTERING== lineObj->status){
			result = voiceAppSendDeRegister(SipAccountId);
		}
	}	
    
	return result;
}

/*************************************************************************
*
*  FUNCTION NAME  : voiceAppSubscribeTimer
*
*  DESCRIPTION    : This function will execute voiceAppSubscribe(0, line_id) when timeout
*
*  RETURNS		  : voiceAppSubscribe(0, line_id) return value
*************************************************************************/
int
voiceAppSubscribeTimer(
	tm_id_t unused, /* timerid */
	void	*vp
)
{
	int SipAccountId =(int) vp;
	int result;
	VOICEAPP_PRINTF("%s,timer id = %d , %d\n",__FUNCTION__,unused,reg_timer[SipAccountId].subscribeTimer);
	reg_timer[SipAccountId].subscribeTimer = 0;	
	result = voiceAppSubscribe(SipAccountId);
    
	return result;
}

/*************************************************************************
*
*  FUNCTION NAME  : GetInfoDtmf
*
*  DESCRIPTION    : This function will parse DTMF from SIP info 
*
*  RETURNS		  : int
*************************************************************************/
int GetInfoDtmf(icf_info_ind_st *p_info_ind, uint8 *digit, uint8 *volume, uint16 *duration)
{
	char *p;
	char signalStr[] = "Signal=";
	char volumeStr[] = "Volume=";
	char durStr[] = "Duration=";
	int  i = 0, j = 0, k = 0, slen, vlen, dlen;
	char sigValue[8]; 
	char durValue[8]; 
	char volValue[8]; 

	if(p_info_ind != NULL){
		if(p_info_ind->body_list.body !=NULL){
			p = p_info_ind->body_list.body->value;
			slen = zyStrlen(signalStr);
			vlen = zyStrlen(volumeStr);
			dlen = zyStrlen(durStr);
			
			if( strncmp(p,signalStr,slen) == 0){
				p = p + slen;
				while( *p == ' ' ) p++;
				
				while( *p != ' ' && *p != 0x0d ){
					sigValue[i++] = *p; 
					p++;
				}
				sigValue[i] = '\0';

				if( strncmp("*", sigValue, 1) == 0) {
					/* digit = 10 */
					*digit='*';
				} else if( strncmp("#", sigValue, 1) == 0) {
					/* digit = 11 */
					*digit='#';
				} else if( strncmp("A", sigValue, 1) == 0) {
					/* digit = 12 */
					*digit='A';
				}else if( strncmp("B", sigValue, 1) == 0) {
					/* digit = 13 */
					*digit='B';
				}else if( strncmp("C", sigValue, 1) == 0) {
					/* digit = 14 */
					*digit='C';
				}else if( strncmp("D", sigValue, 1) == 0) {
					/* digit = 15 */
					*digit='D';
				} else {
					*digit='0'+(uint8)atoi(sigValue);
				}
				
				while( *p != 0x0a ) p++;	
				p++;
				if( strncmp(p,volumeStr,vlen) == 0 ){
					p = p + vlen;
					while( *p == ' ' ) p++;
					
					while( *p != ' ' && *p != 0x0d ){
						volValue[j++] = *p; 
						p++;
					}
					volValue[j] = '\0';
					*volume = (uint8)atoi(volValue);
					while( *p != 0x0a ) p++;	
					p++;
				}
				if( strncmp(p,durStr,dlen) == 0 ){
					p = p + dlen;
					while( *p == ' ' ) p++;
					
					while( *p != ' ' && *p != 0x0d ){
						durValue[k++] = *p;	
						p++;
					}
					durValue[k] = '\0';
					*duration = (uint16)atoi(durValue);
					return 0;
				}
				return -1;		
			}
				
		}	
	}
	return -1;
}

#if 1 /*Jennifer, Anonymous Call Block*/
/*************************************************************************
*
*  FUNCTION NAME  : determineAnonymousCallTerms
*
*  DESCRIPTION      : This function will determine the anonymous call terms
*
*  RETURNS		  : boolean
*  
*************************************************************************/
int determineAnonymousCallCriteria(icf_incoming_call_ind_st *p_incoming_call)
{
	icf_line_id_t line_id;
	char sipUrl[ICF_MAX_STR_LEN]="";
	char* tmpStr_p = NULL;
	char* anonymous = "anonymous";
	char* id = "id";
	char* user= "user";
	int LineItr = 0, VpItr=0;
	/*AnonymousCallBlockEnable*/
	VoiceProfLineCallingFeaturesObj_t* pLineFeature = NULL;
	line_id = p_incoming_call->line_id;
	VpItr = mapSipAccountIdToVpItr(p_incoming_call->line_id);	//p_resp_reg->line_id  is  SipAccountId
	LineItr = mapSipAccountIdToLineItr(p_incoming_call->line_id);
	pLineFeature = tr104VoiceLineCallingFeatureObjPtrGet(VpItr , LineItr);;

	if(p_incoming_call->calling_party_addr.addr_val.str_len > (ICF_MAX_STR_LEN - 1)){
		VOICEAPP_PRINTF("%s(): This URI is out of range.\n", __FUNCTION__);
	} else {
		zyStrncpy(sipUrl, p_incoming_call->calling_party_addr.addr_val.str, p_incoming_call->calling_party_addr.addr_val.str_len);
		if(strlen(sipUrl) > 0){
			tmpStr_p = zyStrtok(sipUrl,(char *)"@\0");
		}
		VOICEAPP_PRINTF("%s(): sipUrl = %s\n", __FUNCTION__, sipUrl);
	}	
	/*The criteria that are used to determine anonymous call */
	if (((0 == strcasecmp(sipUrl, anonymous))
		||(0 == strcasecmp(p_incoming_call->calling_party_name.str, anonymous))
		||(0 == strcasecmp(p_incoming_call->calling_party_name.str, id))
		||(0 == strcasecmp(p_incoming_call->calling_party_name.str, user)))
		&& pLineFeature->anonymousCallBlockEnable){
		return 1;
	} else {
		return 0;
	}
}
#endif

#if HEARTBEAT_MECHANISM_ENABLE 
/*************************************************************************
*
*  FUNCTION NAME  : voiceAppHeartbeatOptionsCheckResp
*
*  DESCRIPTION    : 
*					 
*  RETURNS		  : 
*************************************************************************/
void
voiceAppHeartbeatOptionsCheckResp(icf_msg_st *p_msg)
{
	int i=0 , VpItr=-1 , LineItr = -1, SipAccountId = 0; 
	int NumOfProfile = tr104GetNumProfile();
	int NumOfLine = 0;
	VoiceProfObj_t* p_profObj=NULL; 
	icf_options_resp_st * p_options_resp = NULL;
	icf_uint16_t   response_code = 0;
	uint32   rateOfHeartbeat = 0 ; 
	VOICEAPP_DBG("VOICEAPP> ICF_OPTIONS_RESP");
	icf_uint16_t call_id = VOICEAPP_IPTK_MSG_GET_CALL_ID(p_msg);
	ZyPrintf("call_id = %d \r\n" , call_id);
	
	/*check profile index*/
	for( i=0 ; i < NumOfProfile ; i++){
		if(call_id == (HEARTBEATCALLIDBASE|i)){
			VpItr = i ; 
			break ; 
		}
	}
	VOICEAPP_PRINTF("ICF_OPTIONS_RESP : VpItr = %d \r\n", VpItr);
	p_profObj = tr104VoiceProfObjPtrGet(VpItr); 
	rateOfHeartbeat = p_profObj->SIPObj->ZyXEL_RateOfHeartbeat ;
	p_options_resp =(icf_options_resp_st *)VOICEAPP_IPTK_MSG_GET_PAYLOAD(p_msg);

	VOICEAPP_PRINTF("ICF_OPTIONS_RESP original: \r\n");
	VOICEAPP_PRINTF("ICF_OPTIONS_RESP : hearbeatOngoing = %d \r\n" ,heartbeatRecord[VpItr].hearbeatOngoing);
	VOICEAPP_PRINTF("ICF_OPTIONS_RESP : timerID= %d  \r\n",heartbeatRecord[VpItr].timerID);	
	
	if(ICF_SUCCESS == p_options_resp->result){
		if(p_profObj->SIPObj->ZyXEL_HeartbeatEnable){
			heartbeatRecord[VpItr].hearbeatOngoing = TRUE;						
			heartbeatRecord[VpItr].timerID = timerStartS(1000*rateOfHeartbeat, voiceAppHeartbeatOptionsTimer, (void*)VpItr);
		}else{
			heartbeatRecord[VpItr].hearbeatOngoing = FALSE;
			heartbeatRecord[VpItr].timerID = 0;
		}
	}else{

		if((p_options_resp->bit_mask)&ICF_OPTIONS_RESP_CODE_PRESENT){
			VOICEAPP_PRINTF("p_options_resp->response_code_for_options = %d \r\n",p_options_resp->response_code_for_options);

			switch(p_options_resp->response_code_for_options){
				case 408:
				case 500:
				case 502:
				case 606:{						
						heartbeatRecord[VpItr].hearbeatOngoing = FALSE;						
						heartbeatRecord[VpItr].timerID =0;
						NumOfLine = tr104GetVPNumOfLine(VpItr);
						for(LineItr=0 ; LineItr < NumOfLine; LineItr++ ){
							if(NULL!=p_profObj->lineProfPtrLst[LineItr]){
								if(TR104_TRUE == (p_profObj->lineProfPtrLst[LineItr]->enable)){
									SipAccountId = mapVpItrLineItrToSipAccountId(VpItr, LineItr);
									voiceAppSendRegister(SipAccountId);
								}
							}	
						}	
					}
					break;
				case 503:{ /* need to check the retry-after is present*/
						if(0==(p_options_resp->retry_after_duration)){
							VOICEAPP_PRINTF("retry-after is not present \r\n");							
							heartbeatRecord[VpItr].hearbeatOngoing = FALSE;						
							heartbeatRecord[VpItr].timerID =0;	
							NumOfLine = tr104GetVPNumOfLine(VpItr);
							for(LineItr=0 ; LineItr < NumOfLine; LineItr++ ){
								if(NULL!=p_profObj->lineProfPtrLst[LineItr]){
									if(TR104_TRUE == (p_profObj->lineProfPtrLst[LineItr]->enable)){
										SipAccountId = mapVpItrLineItrToSipAccountId(VpItr, LineItr);
										voiceAppSendRegister(SipAccountId);
									}
								}	
							}
						}else{
							VOICEAPP_PRINTF("retry-after is present, duration = %d \r\n",p_options_resp->retry_after_duration);
							if(p_profObj->SIPObj->ZyXEL_HeartbeatEnable){
								timerStartS(1000*(p_options_resp->retry_after_duration),voiceAppHeartbeatOptionsTimer, (void*)VpItr);
							}else{
								heartbeatRecord[VpItr].hearbeatOngoing = FALSE;						
								heartbeatRecord[VpItr].timerID =0;							
							}
						}
					}
					break;	
				default:{
						VOICEAPP_PRINTF("Other Response : p_options_resp->response_code_for_options = %d \r\n" ,p_options_resp->response_code_for_options);
						heartbeatRecord[VpItr].hearbeatOngoing = FALSE;						
						heartbeatRecord[VpItr].timerID =0;	
					}
					break;						
			}
		}else{
			if((p_options_resp->bit_mask)&ICF_OPTIONS_ERROR_CODE_PRESENT){
				VOICEAPP_PRINTF("ICF_OPTIONS_ERROR_CODE_PRESENT \r\n");				
				heartbeatRecord[VpItr].hearbeatOngoing = FALSE;						
				heartbeatRecord[VpItr].timerID =0;
				
				switch(p_options_resp->error_code_for_options){
					case ICF_CAUSE_REMOTE_UNREACHABLE:{ 
							VOICEAPP_PRINTF("ICF_CAUSE_REMOTE_UNREACHABLE : %d \r\n" , ICF_CAUSE_REMOTE_UNREACHABLE);
							NumOfLine = tr104GetVPNumOfLine(VpItr);
							for(LineItr=0 ; LineItr < NumOfLine; LineItr++ ){
								if(NULL!=p_profObj->lineProfPtrLst[LineItr]){
									if(TR104_TRUE == (p_profObj->lineProfPtrLst[LineItr]->enable)){
										SipAccountId = mapVpItrLineItrToSipAccountId(VpItr, LineItr);
										voiceAppSendRegister(SipAccountId);
									}
								}	
							}
						}
						break;
					default:{
							ZyPrintf("Other error : p_options_resp->error_code_for_options = %d \r\n" ,p_options_resp->error_code_for_options);
						}
						break;
				}
			}
		}
	}
}

/***********************************************************************
*
*  FUNCTION NAME  : voiceAppHeartbeatOptionsTimer
*
*  DESCRIPTION    : This function will execute voiceAppSendOptions(VpItr, line_id) when timeout
*					for the Heartbeat mechanism ,we choose one line which status is enable to send OPTIONS. 
*  RETURNS		  : voiceAppSendOptions(VpItr, line_id) return value
*************************************************************************/
int
voiceAppHeartbeatOptionsTimer(
	tm_id_t unused, /* timerid */
	void	*vp
)
{
	int *VpItr = vp;
	int	LineItr = -1, SipAccountId= 0;
	int result = FALSE,i=0;

	VoiceProfObj_t * p_profileObj = tr104VoiceProfObjPtrGet(VpItr);
	int NumOfLine = tr104GetVPNumOfLine(VpItr);	
	for(i=0 ; i < NumOfLine ; i++){
		if( NULL!=p_profileObj->lineProfPtrLst[i]){
			if(TR104_TRUE==p_profileObj->lineProfPtrLst[i]->enable){
				LineItr = i;
			}
		}
	}
	SipAccountId = mapVpItrLineItrToSipAccountId(VpItr, LineItr);
	result = voiceAppSendOptions(VpItr, SipAccountId);
	if(!result){
		ZyPrintf("voiceAppHeartbeatOptionsTimer: voiceAppSendOptions ERROR");
	}	
	return result;
}

#endif

