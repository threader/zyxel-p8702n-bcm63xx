#include <stdio.h>
#include <sys/syslog.h>
#include <string.h>
#include "voip_syslog.h"
#include "tr104_object.h"

#ifdef MSTC_LOG	
#include "cms_core.h"
#include "cms_log.h" 
#include "cms_util.h"
#endif
//#include "config_tr104.h"
#include <stdint.h>  
/*______________________________________________________________________________
**  VoIP_Syslog
**
**  descriptions:
**	this function is using syslog function to log voip messages.
**  parameters:
**  local:
**  global:
**  return: error < 0 fail, 0:success.
**  called by:
**  call:
**  revision:
**____________________________________________________________________________*/

icf_uint8_t category_mask_set;
char deviceModelName[MODEL_NAME_MAX_LEN];
char deviceSerialNumber[SERIAL_NUMBER_MAX_LEN];
char deviceMACAddress[MAC_ADDRESS_MAX_LEN];
char devicePhoneNumber[PHONE_NUMBER_MAX_LEN];
icf_uint8_t deviceFXS;

int voipSyslog(
	const int category, 			
	const int level, 				
	const char *Phone_Num, 			
	int  Phone_line_Indicator, 	/*FXS0 or FXS1*/
	char *Content				
){
		char syslog_fmt[SYSLOG_FMT_SIZE];
		char text_voip[TEXT_VOIP_SIZE];
		#if 1 /*Below is the symptom of the bug fix of the unregister issue, __P876__,Ailsa, 20120326 */
		char limitContent[TEXT_VOIP_SIZE-50];
		#endif
		int cpLen = 0, error = 0;
		uint32 ifindex;
		uint8 macAddr[6];
		char  devMac[48];
		//eric.chung temp mark set 
		category_mask_set=0xff;
		//ZyXEL_VoiceCommon_t* pCommon = tr104VoiceCommonObjPtrGet();
//		printf("\r\n$$########run voipSyslog!!!########$$\r\n");
		zyMemset(syslog_fmt, 0, SYSLOG_FMT_SIZE);
		zyMemset(text_voip, 0, TEXT_VOIP_SIZE);
		#if 1 /*Below is the symptom of the bug fix of the unregister issue, __P876__,Ailsa, 20120326 */
		zyMemset(limitContent, 0, (TEXT_VOIP_SIZE-50));
		if(zyStrlen(Content) >= (TEXT_VOIP_SIZE-50)) {
			strncpy(limitContent, Content, (TEXT_VOIP_SIZE-51));
		}else{
			strcpy(limitContent, Content);
		}
		#endif

#ifdef NORWAY_VOIP_CUSTOMIZATION
		if (Phone_line_Indicator == 1 ){
			Phone_line_Indicator = 2;
		}else if (Phone_line_Indicator == 2){
			Phone_line_Indicator = 1;
		}
#endif
		
//		printf("\r\ncategory_mask_set=%d, category=%d", category_mask_set, category);
//		printf("\r\nContent= %s",Content);
//		printf("\r\ndeviceSerialNumber=%s, deviceModelName=%s", deviceSerialNumber, deviceModelName);
//		printf("\r\nPhone_line_Indicator is %d\n",Phone_line_Indicator);

	  	switch(category & category_mask_set){
			case LOG_CALL_STATISTICS: 
				strcpy(syslog_fmt, CALL_STATISTICS);
				if(Phone_line_Indicator!=0){
					sprintf(text_voip, " [%s][FXS%d] %s", Phone_Num, Phone_line_Indicator, limitContent);
				}else{
					sprintf(text_voip, " %s",  limitContent);
				}			
				break;
			case LOG_SIP_CALL_SIGNALING:
				strcpy(syslog_fmt, SIP_CALL_SIGNALING);
				if(Phone_line_Indicator!=0){
					sprintf(text_voip, " [%s][FXS%d] %s", Phone_Num, Phone_line_Indicator, limitContent);
				}else{
					sprintf(text_voip, " %s",  limitContent);
				}
				break;
			case LOG_SIP_REGISTRATION:
				strcpy(syslog_fmt, SIP_REGISTRATION);
				if(Phone_Num!=NULL){
					sprintf(text_voip, " SIP:%s %s", Phone_Num, limitContent);
				}else{
					sprintf(text_voip, " %s", limitContent);
				}

				break;
			case LOG_PHONE_EVENT:
				strcpy(syslog_fmt, PHONE_EVENT);
				if(Phone_line_Indicator!=0){
					sprintf(text_voip, " [%s][FXS%d] %s", Phone_Num, Phone_line_Indicator, limitContent);
				}else{
					sprintf(text_voip, " %s",  limitContent);
				}
				break;
			case LOG_OTHERS:
				strcpy(syslog_fmt, OTHERS);
				sprintf(text_voip, " %s",  limitContent);
				break;
			case 0:
				error= -1;
				break;
			default:
				printf("[%s_%d]: irrr.....unknow\r\n", __FILE__, __LINE__);
				error = -2;
				break;
			}	

		if (error < 0 ) {
			printf("\r\n return error!!!");
			return error;
		}
			
		//printf("\r\nlog= %s",text_voip);
		#if 0 /*Below is the symptom of the bug fix of the unregister issue, __P876__,Ailsa, 20120326 */
		cpLen = (zyStrlen(text_voip) > MAX_LEN_VOIP)?(MAX_LEN_VOIP-1): zyStrlen(text_voip);
		#else
		cpLen = (zyStrlen(text_voip) >= MAX_LEN_VOIP) ? (MAX_LEN_VOIP-1) : zyStrlen(text_voip);
		#endif
		strncat(syslog_fmt, text_voip, cpLen);
	
		#ifdef MSTC_LOG		
			zyLog_app(LOG_LEVEL_INFO,LOG_FAC_VoIP,text_voip);
		#else
                        syslog(level, "%s",syslog_fmt);
                #endif
		
		return 0;
}


