/******************************************************************************/
/*                                                    
 *  Copyright (C) 2009 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and
 * shall not be reproduced, copied, disclosed, or used in whole or
 * in part for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 */                                                                      
/******************************************************************************/

#include "mm_port_wrapper.h"
#include "mm_port_prototypes.h"
#include "mm_cfg_types.h"
#include "mm_glb.h"
#include "icf_clib.h"
#include <errno.h>
#include "global.h"
#include "voice_dsp_common_api.h"
#include "voice_dsp_utils.h"
#include "sys_msg.h"
icf_int32_t gMmFd ;
fd_set		mmFdset;

struct sockaddr_un  g_voiceApp_addr;
struct sockaddr_un  g_config_addr;

/*************************************************************************
*
*  FUNCTION NAME  : mm_port_udp_init
*
*  DESCRIPTION    : This function is called to initialize the ipc. It  
*					opens socket and bind it to given port.
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
mm_return_t 
mmPortUdpInit(void)
{
	mm_return_t    ret_val = MM_SUCCESS;
	struct sockaddr_un addr;

	gMmFd = socket(PF_LOCAL, SOCK_DGRAM, 0);

	if(gMmFd < 0) {
		ret_val = ICF_FAILURE;
	} else {
		zyMemset(&addr, 0, sizeof(addr));
		addr.sun_family = PF_LOCAL;
		zyStrncpy(addr.sun_path, MM_CHANNEL_PATH, sizeof(MM_CHANNEL_PATH));
		unlink(MM_CHANNEL_PATH);
		if ( bind( gMmFd, (struct sockaddr*)(&addr), sizeof(addr)) < 0) {
			MM_PRINTF("\n\tIPTKAPP > Port is currently being used by some other application \n");
			mmPortClose();
			ret_val = ICF_FAILURE;
		}
		
	}

	g_voiceApp_addr.sun_family = PF_LOCAL;
	zyStrncpy(g_voiceApp_addr.sun_path, VOICEAPP_MM_CHANNEL_PATH, sizeof(VOICEAPP_MM_CHANNEL_PATH));	

	g_config_addr.sun_family = PF_LOCAL;
	zyStrncpy(g_config_addr.sun_path, CONFIG_CHANNEL_PATH, sizeof(CONFIG_CHANNEL_PATH));	

	MM_PRINTF("\n\tIMM > mmPortUdpInit created fd = %d for channel = %s\n", gMmFd, MM_CHANNEL_PATH);
	return ret_val;
}


/*************************************************************************
*
*  FUNCTION NAME  : mmPortClose
*
*  DESCRIPTION    : This function is called to close the ipc.  
*
*  RETURNS		  : void
*************************************************************************/

icf_void_t 
mmPortClose(void)
{
	close(gMmFd);
	unlink(MM_CHANNEL_PATH);
}


/*************************************************************************
*
*  FUNCTION NAME  :  mmPortUdpRecv
*
*  DESCRIPTION    : This functions implements the udp recieve. It will 
*					first receive the message from UDP and then decode it
*					by calling IPTK clib decode function. Finally it will
*					assign the decoded message pointer to p_buf.
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t 
mmPortUdpRecv(icf_uint8_t **p_buf)
{
	struct sockaddr_in  source_addr;
	socklen_t           source_addr_len;
	icf_uint16_t        len_of_msg_read ;
	icf_uint8_t         buffer[MAX_MTU_SIZE];
	icf_uint8_t         *p_icf_decoded_buf = MM_NULL;
	mm_return_t	    ret_val ;
	mm_error_t         p_ecode = MM_NULL;
	icf_api_id_t api_id;

	source_addr_len = sizeof (struct sockaddr_in);
	ret_val = MM_SUCCESS;
	
	len_of_msg_read = recvfrom (gMmFd, buffer, MAX_MTU_SIZE, 0, (struct sockaddr*)(&source_addr), &source_addr_len);
	
	if(len_of_msg_read > 0){    	
		api_id=((icf_msg_st*)&buffer)->hdr.api_id;
		MM_PRINTF("MM: %s :IPC recv %d bytes,  api_id %d\n",__FUNCTION__,len_of_msg_read,api_id);

    		if(api_id>=VOICE_DSP_API_START && api_id<=VOICE_DSP_API_END){
			MM_PRINTF("MM: %s :voice event\n",__FUNCTION__);
			ret_val=voiceMsgDecode((voiceMsg_t**)&p_icf_decoded_buf,
	                                buffer,
	                                len_of_msg_read,
	                                &p_ecode);
			*p_buf = p_icf_decoded_buf;

		}else{
			MM_PRINTF("MM: %s :icf event\n",__FUNCTION__);
			ret_val=icf_decode_msg((icf_msg_st**)&p_icf_decoded_buf,
	                                buffer,
	                                len_of_msg_read,
	                                &p_ecode);
			
			if(ret_val == ICF_FAILURE){
				ret_val = ICF_FAILURE;
			}else{
				*p_buf = p_icf_decoded_buf;
			}
		}
	}
	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmIptkIpcSend
*
*  DESCRIPTION    : This is the function that sends the UDP packet to n/w
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t 
mmIptkIpcSend(icf_void_t *p_msg,icf_uint32_t buf_len, mm_error_t *p_error)
{
	struct sockaddr_un addr;	
	icf_uint32_t		payload_length,packet_length;
	icf_uint16_t		bytes_written;
	icf_uint8_t		*p_buf;

	
	if(p_msg ==NULL) {
		return MM_FAILURE;
	}

	/* get the port corresponding to the dest-id from cfg */
	addr.sun_family = PF_LOCAL;
	zyStrncpy(addr.sun_path, ICF_ES_CHANNEL_PATH, sizeof(ICF_ES_CHANNEL_PATH));
	packet_length = payload_length = buf_len;
		
	p_buf = ( icf_uint8_t *)p_msg;

	while( packet_length ) {
		if( packet_length > MAX_MTU_SIZE ) {
			packet_length = MAX_MTU_SIZE;
		}
		bytes_written = sendto(gMmFd,(icf_void_t*)p_buf,packet_length,0,
									(struct sockaddr*)&addr,
									sizeof(struct sockaddr_un));

		if ( packet_length != bytes_written) {
			return (MM_FAILURE);
		}
		
		p_buf +=packet_length;

		payload_length -= packet_length;

		packet_length = payload_length;	
	}
	p_error = p_error;

	return (MM_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  : mmConfigIpcSend
*
*  DESCRIPTION    : This is the function that sends the IPC packet to voiceApp for config
*
*  RETURNS:       :  Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t 
mmConfigIpcSend(icf_void_t *p_msg, icf_uint32_t buf_len)
{
	struct sockaddr_un addr;	
	icf_uint32_t		payload_length,packet_length;
	icf_uint16_t		bytes_written;
	icf_uint8_t		*p_buf;

	
	if(p_msg ==NULL) {
		return MM_FAILURE;
	}

	/* get the port corresponding to the dest-id from cfg */
	addr.sun_family = PF_LOCAL;
	zyStrncpy(addr.sun_path, CONFIG_CHANNEL_PATH, sizeof(CONFIG_CHANNEL_PATH));
	packet_length = payload_length = buf_len;
		
	p_buf = ( icf_uint8_t *)p_msg;

	while( packet_length ) {
		if( packet_length > MAX_MTU_SIZE ) {
			packet_length = MAX_MTU_SIZE;
		}
		bytes_written = sendto(gMmFd,(icf_void_t*)p_buf,packet_length,0,
									(struct sockaddr*)&addr,
									sizeof(struct sockaddr_un));

		if ( packet_length != bytes_written) {
			return (MM_FAILURE);
		}
		
		p_buf +=packet_length;

		payload_length -= packet_length;

		packet_length = payload_length;	
	}

	return (MM_SUCCESS);
}

/*************************************************************************
*
*  FUNCTION NAME  : mmRouteQosFirewallControl
*
*  DESCRIPTION    : This function sends the VoIP related ports and remote address to voiceApp
*                             for applying static route & QoS & firewall rule   
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/
//Ssu-Ying, support Multiple Profile
/* MSTC porting by Andylin, bind interface , Iproute2 */
mm_return_t 
mmRouteQosFirewallControl(uint32 srcAddr, uint32 srcPort, uint32 dstPort, uint8 enable, uint16 SipAccountId)
{
	rtpPortMsg_t *msgSend = NULL;
	MM_PRINTF("%s, arcaddr = 0x%x, srcport = %d, dstport = %d, enable = %d, SipAccountId = %d\n", __FUNCTION__, srcAddr, srcPort, dstPort, enable, SipAccountId);
	msgSend = (rtpPortMsg_t*)zyMalloc(sizeof(rtpPortMsg_t));

	if (msgSend == NULL) {
		return MM_FAILURE;
	}
	/* MSTC porting by Andylin, bind interface , Iproute2 */
	msgSend->src_addr = srcAddr;
	msgSend->src_port = srcPort;
	msgSend->dst_port = dstPort;
	msgSend->enable = enable;
	msgSend->SipAccoundId = SipAccountId;
	
	mmConfigIpcSend(msgSend, sizeof(rtpPortMsg_t));

	if(NULL!=msgSend){
		zyFree(msgSend);
	}
}

/*************************************************************************
*
*  FUNCTION NAME  : mmPortSelect
*
*  DESCRIPTION    : This is the wrapper function to the select. it returns 
*				    ICF_SUCCESS whether the data is there on the fd or
*					if the select is timed out.
* 
*  PARAMETER	  :	 p_timeout : will be TRUE if the select has timed out and no
*					 data was there on the FD
*				  :  usec : is the time in micro sec				
*
*  RETURNS		  : Either ICF_SUCCESS or ICF_FAILURE
*************************************************************************/

mm_return_t
mmPortSelect(icf_uint32_t usec, icf_boolean_t *p_timeout_flag)
{
	struct timeval     timeout;
	struct timeval	   *p_timeout ;
	icf_int8_t   retval;

    	p_timeout = &timeout;
	
	FD_ZERO(&mmFdset);
	FD_SET(gMmFd,&mmFdset);

	if(usec == MM_SELECT_INFINITE)
	{
		/* blocking wait for select */
		p_timeout = MM_NULL;
	}
	else
	{
		/* wait for the time specified */
		timeout.tv_sec = usec/1000000;  /* get the time in seconds */
		timeout.tv_usec = usec % 1000000 ;/* rest of the time in micro sec */
	}
	
	retval = select(FD_SETSIZE,&mmFdset,(fd_set *)0,NULL,p_timeout);
	if(-1 == retval)
	{
		return MM_FAILURE;

	}
	else if(0 == retval)
	{
		/* fd is not set select timed out*/
		*p_timeout_flag = ICF_TRUE;
		return MM_SUCCESS;
	}
	/* fd is set return the success */
	*p_timeout_flag = ICF_FALSE;
	return ICF_SUCCESS;
}


/*************************************************************************
*
*  FUNCTION NAME  :  mmGetNextNodeInList 
*
*  DESCRIPTION    :  This function will be used to fetch the next node in the
*  					 list. The calling function will pass the payload of the
*  					 current node. This function will return the pointer to the
*  					 payload of the next node.
*
*  RETURNS        :
*
*************************************************************************/
icf_void_t*	mmGetNextNodeInList 
(
	icf_list_st	*p_list_start,
	icf_void_t		*p_payload /* current payload */
)
{
	/* local variables */
	icf_list_st			*p_hdr = MM_NULL;
	icf_void_t			*p_node = MM_NULL;

	p_hdr = p_list_start;

	while (MM_NULL != p_hdr)
	{
		if (p_payload == p_hdr->p_data)
		{
			p_hdr = p_hdr->p_next;
			/* if this is the last node of the list, it shall return NULL */
			if (p_hdr)
			{
				p_node = p_hdr->p_data;
			}
			break;
		}
		else
		{
			p_hdr = p_hdr->p_next;
		}
	}

	/* the function will return the pointer to the payload of the next node
	 * in the list */
	return p_node;
}


/*************************************************************************
*
*  FUNCTION NAME  :  mmAddNodeToList 
*
*  DESCRIPTION    :  This function will be used to insert a node into the 
*					 list of the type icf_list_st. The function will always
*					 insert the node at the end of the list
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t	mmAddNodeToList 
(
	icf_list_st		**p_p_list_start, 
									  /* incase we r adding the first node 
									   * in the list then we will be allocating
									   * memory for the node, that will be
									   * returned to the calling function
									   */
	icf_void_t		*p_payload
)
{
	/* the function will allocate memory for the node and assign the node
	 * to the list
	 */
	/* local variable */
	icf_list_st			*p_tmp_list = MM_NULL;
   	icf_list_st			*p_pre_tmp_list = MM_NULL;
	icf_list_st			*p_tmp_node = MM_NULL;
	icf_return_t		ret_val	= MM_SUCCESS;

	p_tmp_list = *p_p_list_start;
	p_tmp_node = zyMalloc(sizeof(icf_list_st));
	p_tmp_node->p_data = (icf_void_t*)p_payload;	
	p_tmp_node->p_next = MM_NULL;
	
	/* if this is the first node of the list */
	if ( MM_NULL == p_tmp_list )
	{
		(*p_p_list_start) = p_tmp_node;
	}
	else /* traverse to the end of the list to insert the node */
	{
		while (MM_NULL != p_tmp_list)
		{
			p_pre_tmp_list = p_tmp_list;
              	p_tmp_list = p_tmp_list->p_next;
		}
		p_pre_tmp_list->p_next = p_tmp_node;
	}
	
	return ret_val;
}	

/*************************************************************************
*
*  FUNCTION NAME  :  mmDeleteNodeFrmList 
*
*  DESCRIPTION    :  This function will be used to delete a node from the 
*					 list of the type icf_list_st. The function will return
*					 the pointer to the payload, so that the calling module
*					 can free that memory as the payload was allocated mem.
*					 by the calling module.
*
*  RETURNS        :
*
*************************************************************************/
icf_return_t mmDeleteNodeFrmList 
(
	icf_list_st		**p_list_start,
	icf_void_t		**p_payload
)
{
	/* local variables */
	icf_list_st		*p_tmp_list = MM_NULL, *p_prev_node = MM_NULL;
	icf_return_t	ret_val = MM_FAILURE;
	

	p_tmp_list = *p_list_start;
	/* if the node is present in the function argument, it means that the
	 * calling function is aware of the payload node, so the list will be 
	 * traversed based on the node pointer value */
	if ( MM_NULL != p_payload )
	{
		while ( MM_NULL != p_tmp_list )
		{
			if (*p_payload == p_tmp_list->p_data)
			{
				break;
			}
			else
			{
				p_prev_node = p_tmp_list ;
				p_tmp_list = p_tmp_list->p_next;
			}
		}
		

	/* if a match is found, then delete the node */
	if ( MM_NULL != p_tmp_list )
	{
		/* if this is not the first/last node then we need to move the pointers
		 * here and remove this node from the list */ 
		if (MM_NULL != p_prev_node)
		{
			if(MM_NULL != p_tmp_list->p_next)
			{
				p_prev_node->p_next = p_tmp_list->p_next;
			}
			else
			{
				/* if this is the last node and not the fist node of the list, then
				 * we need to set the pointer of the prev. node to NULL */
				p_prev_node->p_next = MM_NULL;
			}
		}
		else if (MM_NULL == p_prev_node)
		{
			if(MM_NULL != p_tmp_list->p_next)
			{
				/* if this is the first node of the list then the list header
				 * needs to be updated */
				*p_list_start = p_tmp_list->p_next;
			}
			else
			{
				/* if this is the only node in the list, then we need to make the
				 * list header NULL */
				*p_list_start = MM_NULL;
			}

		}
		
		/* Now free the memory for the complete node */
		zyFree(p_tmp_list->p_data);
		zyFree(p_tmp_list);

		*p_payload = MM_NULL;

		}
	}
	
	return ret_val;
}	


/*************************************************************************
*
*  FUNCTION NAME  :  mmGetDataListCount 
*
*  DESCRIPTION    :  This function is used to get the count/size of the 
*					 list. 
*
*  RETURNS        :
*
*************************************************************************/
icf_uint8_t	mmGetDataListCount 
(
	icf_list_st		*p_list
)
{
	/* local variable */
	icf_list_st	*p_tmp_list = ICF_NULL;
	icf_uint8_t count;
	
	count = 0;
	p_tmp_list = p_list;
	while( ICF_NULL != p_tmp_list )
	{
		if ( ICF_NULL != p_tmp_list->p_data){
			count += 1;	
			p_tmp_list = p_tmp_list->p_next;
		}
		else{
			break;
		}
	}
	return count;
}


/*************************************************************************
*
*  FUNCTION NAME  :  mmDeleteList 
*
*  DESCRIPTION    :  This function will be used to delete all nodes in the 
*					 list of the type icf_list_st. This function will also
*					 delete the payload in the list, based on the pool_id
*					 passed by the calling function/module
*
*  RETURNS        :
*
*************************************************************************/

icf_return_t	mmDeleteList 
(
	icf_list_st		**p_list_start
)
{
	/* local variables */
	icf_list_st			*p_hdr = ICF_NULL, 
						*p_tmp_list = ICF_NULL;
	icf_return_t		ret_val	= MM_SUCCESS;

	p_hdr = *p_list_start;
	/* free the elements in the list */
	while ( ICF_NULL != p_hdr )
	{
		p_tmp_list = p_hdr->p_next;
		zyFree(p_hdr->p_data);
		p_hdr->p_data = ICF_NULL;
		
		/* now free the node */
		zyFree(p_hdr);
		p_hdr = p_tmp_list;	
	}
	
	*p_list_start = ICF_NULL;

	return ret_val;
}

/*************************************************************************
*
*  FUNCTION NAME  : mmDeleteCodecAttribList
*
*  DESCRIPTION    : for delete p_codec_info_list
*                  
*
*  RETURNS:       : 
*************************************************************************/
icf_return_t	
mmDeleteCodecAttribList(icf_list_st **p_list)
{
//	printf("\r\n mmDeleteCodecAttribList \r\n");
	icf_list_st	*p_hdr = MM_NULL; 
	icf_list_st *p_next_node = MM_NULL;
    icf_codec_attrib_st *p_icf_codec_attrib = MM_NULL;
	icf_return_t		ret_val	= MM_SUCCESS;
	
	p_hdr = *p_list;
	for(;p_hdr != MM_NULL; p_hdr = p_next_node)
	{
//		printf("\r\n mmDeleteCodecAttribList :p_hdr != MM_NULL \r\n");
		p_next_node = p_hdr->p_next;
        p_icf_codec_attrib = (icf_codec_attrib_st*)p_hdr->p_data;
	     if(p_icf_codec_attrib->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)
 	     {
			mmDeleteList(&(p_icf_codec_attrib->p_encoded_attributes));
	     }
		zyFree(p_hdr->p_data);
		zyFree(p_hdr);
	}
	*p_list = MM_NULL;
	return ret_val;
}

