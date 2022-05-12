/*****************************************************************************
 ** FUNCTION:
 **                     This file contains utility functions invoked by the SSA
 **                     for SDP handling in incoming and outgoing messages.
 **
 *****************************************************************************
 **
 ** FILENAME:           icf_ssa_sdp_util.c
 **
 ** DESCRIPTION:        This file contains utility functions invoked by the SSA
 **                     for SDP handling in incoming and outgoing messages.
 **
 ** DATE                NAME            REFERENCE               REASON
 ** ----                ----            ---------               ------
 ** 25-July-2006     Deepti Goyal       mulitple m-lines LLD    INITIAL CREATION
 **
 **
 ** 28-July-2006     Aman Aggarwal      ICF_6_0                 Multiple m-lines 
 **                  Deepti Goyal                               changes
 **
 ** 3-Aug-2006       Deepti Goyal       ICF_6_0                 Review comments 
 **                                                             Incorporated.
 ** 25-Aug-2006     Amit Sharma      	SPR 12550
 **                 Umang Singh
 **
 ** 25-Aug-2006		Deepti Goyal		SPR 12570				Check for added
 **																SDP List for NULL        **
 ** 28-Aug-2006		Deepti Goyal		SPR 12583				RTCP Attribute 
 **																extraction in in
 **																coming SDP.		
 ** 29-Aug-2006		Deepti Goyal		SPR 12625				T.38 handling in
 **																responses to RE- **																INVITE	
 ** 29-Aug-2006		Deepti Goyal        SPR 12636				SSA Call Ctxt
 **																Call type is 
 **																not to be 
 **																updated	every
 **																time SDP is to 
 **																be send.
 ** 30-Aug-2006		Deepti Goyal		SPR 12676				RTP	MAP line
 **																for static code
 **																cs also.
 ** 05-Sep-2006		Umang Singh         SPR 12715				
 **	
 **	6-Sep-2006      Deepti Goyal        SPR 12848               Channel bitmask 
 **																for audio codecs
 **	                                                            check updated.
 **
 **	7-Sep-2006		Deepti Goyal		SPR 12713				Memory Leak 
 **									    SPR 12884	   		    in enc_attr 
 **																fixed
 ** 16-Jan-2007     Amit Sharma      	SPR 14014
 ** 19-Jan-2007     Aman Aggarwal       SPR 14017               Send error response                 **                                                             for wrong b-line in				   **                                                             SDP   
 ** 05-Feb-2007     Umang Singh         SPR 14470
 ** 22-Dec-2007     Rajiv Kumar         SPR 17773               Handling of
 **                                                             Invalid
 **                                                             scenario of
 **                                                             sipbomber for
 **                                                             codec name and
 **                                                             encoded attributes
 ** 15-May-2008     Alok Tiwari         SPR 18552     Function 
 **                                 icf_ssa_form_audio_from_sdp_info
 **                                 is modified for sending application
 **                                 mode in 200 OK of Reinvite without
 **                                 SDP received from network when 
 **                                 application is on remote hold. 
 ** 25-Jun-2008   Rajiv Kumar        SPR - 18722        IPTK not compilable with gcc
 **                                                     version 4.2.3
 **23-Jul-2008     Vinay Jindal      SPR 18829          Typecasting for height
 ** 													and video parameters removed
 ** 01-Aug-2008    Puneet Sharma    SPR 18842 CSR 1-6207880
 ** 29-Jun-2009	   Aman Aggarwal	SPR 19792		Added handling of mode in 200OK in case of 
 **													P-Early-media in initial INVITE	
 **
 *****************************************************************************
 **                 Copyrights 2008, Aricent.
 *****************************************************************************/

/*Message body header changes Start*/
#define SIP_MIME_PARSING
/*Message body header changes Ends*/
#include "stack_headers.h"
#include "uatk_headers.h"

#include "icf_internal_api_id.h"
#include "icf_common_types.h"
#include "icf_common_defs.h"

#include "icf_common_prototypes.h"
#include "icf_port_prototypes.h"
#include "icf_macro.h"
#include "icf_ssa_prototypes.h"
#include "icf_ssa_macro.h"
#include "icf_common_prototypes.h"
#include "icf_dbm_prototypes.h"
#include "icf_regm_types.h"
#include "icf_dbm_regm_prototypes.h"

#include "basic.h"
#include "icf_ssa_common_types.h"

#include "icf_ecode.h"
#ifdef ICF_PORT_WINDOWS
#pragma warning(disable:4701)
#pragma warning ( disable : 4702 )
#endif



/*****************************************************************************
 * FUNCTION:         icf_ssa_check_no_of_m_lines
 *
 * DESCRIPTION:     Function to check the number of m-lines in SDP coming
 *                  in SIP messages from n/w.
 ******************************************************************************/
icf_return_t   icf_ssa_check_no_of_m_lines(
        INOUT SdpMessage *p_sdp_message,
        OUT Sdf_ty_u32bit *p_count)
{
    SipError   dSipErr;
    if (SipSuccess == sip_listSizeOf((SipList*) &(p_sdp_message->slMedia),
                (Sdf_ty_u32bit *)p_count, &dSipErr))
    {
        return ICF_SUCCESS;
    }
    return ICF_FAILURE;
}

/***************************************************************************
**
** FUNCTION:     icf_ssa_form_sdp_from_stream_sdp_info
** DESCRIPTION:  This function forms the SDP from the media parameters 
**               present in the SDP structure of the call context.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**               This function forms Sip Sdp from p_local_sdp in the list 
**               of type icf_stream_sdp_info_st
**
***************************************************************************/
icf_return_t icf_ssa_form_sdp_from_stream_sdp_info(
        INOUT  SdpMessage                **p_p_sdpmsg,
        INOUT    icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT    icf_uint8_t               hold_option,
        OUT   Sdf_st_error              *p_error)
{
    SdpMedia                    *p_media = Sdf_co_null;
    SdpOrigin                   *p_origin = Sdf_co_null;
    SdpConnection               *p_connection = Sdf_co_null;
    Sdf_ty_s8bit                pSaddr[ICF_MAX_SIZE_MEDIUM_STR]="\0";
	icf_stream_id_t				media_count = ICF_NULL;

    SdpTime                             *p_sdpTime = Sdf_co_null;
    SdpMessage                          *p_sdpmsg = Sdf_co_null;
    icf_return_t                        ret_val = ICF_SUCCESS;
	icf_stream_sdp_info_st              *p_stream_sdp = ICF_NULL;
    icf_stream_sdp_info_st              *p_stream_sdp_fax = ICF_NULL;
    icf_internal_list_st                 *p_stream_list = ICF_NULL;
    icf_media_type_t                  stream_type;

	/* list of type icf_stream_sdp_info_st */
	icf_internal_list_st		*p_stream_sdp_info = p_ssa_pdb->p_glb_pdb->\
											p_call_ctx->p_stream_sdp_info;
    icf_transport_address_st    stream_address ;

	
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    ICF_SSA_INIT_TRANS_ADDR(&stream_address)
	/* this function will allocate memory for the SDP message and init
	 * the buffer */
	if (SipFail == sip_initSdpMessage(
		p_p_sdpmsg,(SipError *)&(p_error->stkErrCode)))
	{
		ICF_PRINT(((icf_uint8_t *)"\nFailure in sip_initSdpMessage"));
		ret_val = ICF_FAILURE;
	}
	else if(ICF_NULL == *p_p_sdpmsg)
	{
		ret_val = ICF_FAILURE;
	}
	else
	{
		p_sdpmsg = *p_p_sdpmsg;
	}
		
	if(ICF_NULL == p_stream_sdp_info)
	{
		ret_val = ICF_FAILURE;
	}
	else
	{	
    	/*Get the first node of the list*/
		p_stream_sdp = (icf_stream_sdp_info_st *)p_stream_sdp_info->p_payload;
	}

	/*	p_ssa_pdb->p_ssa_ctxt->call_type = p_ssa_pdb->p_glb_pdb->p_call_ctx->\
		call_type;*/


	if ( ICF_FAILURE != ret_val)
	{
		/* Get the session level stream address.
         	* If first stream address is zero then set session level
         	* c line address to the first non zero address.
         	* Check that stream addr is to be fetched first NW-ALLOW stream
		*/
       		icf_port_memset((icf_void_t *)&stream_address,\
           	0, sizeof(icf_transport_address_st));
		
		stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
       
       		for (p_stream_list = p_stream_sdp_info;p_stream_list != ICF_NULL;\
           		p_stream_list = p_stream_list->p_next)
        	{
           		p_stream_sdp = (icf_stream_sdp_info_st *)p_stream_list->p_payload;
            		if(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp->stream_status  || 
            					(ICF_NULL == p_stream_sdp->p_local_sdp))
			{
                		continue;
            		}            
           		else
            		{
                		icf_port_memcpy(&stream_address,&(p_stream_sdp->p_local_sdp->\
                    		sdp_stream.stream_address),sizeof(icf_transport_address_st));
				/* Convert the transport address to a string*/
		       		if(ICF_FAILURE == icf_ssa_convert_to_IP_addr(\
			       			(icf_transport_address_st *)&\
							stream_address,(icf_uint8_t *)pSaddr))
				{
					ret_val = ICF_FAILURE;	
				}		
                		break;
            		}
       		}
	}

	/* Set values into SdpMessage */
	if (ICF_FAILURE != ret_val)
	{
		/* Setting the v= line */
		if (SipFail == sdp_setVersion(
			p_sdpmsg, Sdf_mc_strdupSdp("0"),
			(SipError *)&(p_error->stkErrCode)))
		{
			sip_freeSdpMessage(p_sdpmsg);
			ret_val =  ICF_FAILURE;
		}
        
        /*Set the Origin Line */
		if(ICF_FAILURE == icf_ssa_init_n_form_o_line(p_ssa_pdb,
									hold_option,
									stream_address,	
									&p_origin,
									p_error))
		{
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}			
		else if (SipFail == sdp_setOrigin (
					p_sdpmsg, p_origin,
				(SipError *)(&(p_error->stkErrCode))))
		{
			p_error->errCode = p_error->stkErrCode;
			sip_freeSdpOrigin (p_origin);
			ret_val = ICF_FAILURE;
		}
		else
		{
			sip_freeSdpOrigin (p_origin);
		}	
	}

	if ( ICF_FAILURE != ret_val)
	{
       if (ICF_NULL != p_sdpmsg)
       {
		/* Set the Session-Line */
		p_sdpmsg->pSession = Sdf_mc_strdupSdp("Session");
		
		/* Set the t-line */
		if (SipFail == sip_initSdpTime(
			&p_sdpTime, (SipError *)&(p_error->errCode)))
		{
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else if (SipFail == sdp_setStartInTime(
			p_sdpTime, Sdf_mc_strdupCallHandler("0"),
			(SipError *)&(p_error->errCode)))
		{
			sip_freeSdpTime(p_sdpTime);
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else if (SipFail == sdp_setStopInTime(
			p_sdpTime, Sdf_mc_strdupCallHandler("0"),
			(SipError *)&(p_error->errCode)))
		{
			sip_freeSdpTime(p_sdpTime);
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else if ((ICF_NULL == p_sdpmsg) ||
                (SipFail == sdp_insertTimeAtIndex(
			p_sdpmsg, p_sdpTime, (Sdf_ty_u32bit)0,
			(SipError *)&(p_error->errCode))))
		{
			sip_freeSdpTime(p_sdpTime);
            if (ICF_NULL != p_sdpmsg)
                sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else
		{
			sip_freeSdpTime(p_sdpTime);
        }
       }
       else
       {
         ret_val = ICF_FAILURE;
       }  
		
	}/*O Line Handling Block End*/

	if ( ICF_FAILURE != ret_val)
	{
        /* Form the session level c-line for SDP.
         */
        if(ICF_FAILURE == icf_ssa_init_n_form_c_line(
					p_ssa_pdb, 
					&stream_address,
					&p_connection,
					p_error))
        {
            sip_freeSdpConnection (p_connection);
            sip_freeSdpMessage(p_sdpmsg);
            ret_val = ICF_FAILURE;
        }
        else if (SipFail == sdp_setConnection(p_sdpmsg,p_connection,\
            (SipError *)&(p_error->stkErrCode)))
        {
            sip_freeSdpConnection (p_connection);
            sip_freeSdpMessage(p_sdpmsg);
            ret_val = ICF_FAILURE;
        }

		if(ICF_FAILURE != ret_val)
		{
			sip_freeSdpConnection (p_connection);		
		}
    }/*C Line handling Block End*/

    /* Traverse the stream-level list and form media and attribute lines for
     * each stream */
    if(ICF_FAILURE != ret_val)
	{
        for (p_stream_list = p_stream_sdp_info;
        p_stream_list != ICF_NULL; p_stream_list = p_stream_list->p_next)
        {
            p_stream_sdp = (icf_stream_sdp_info_st *)p_stream_list->p_payload;
            
            stream_type = p_stream_sdp->stream_type;
            
            switch(stream_type)
            {
            case ICF_STREAM_TYPE_TBCP: 
                if(ICF_FAILURE == icf_ssa_form_tbcp_from_sdp_info(
                    p_ssa_pdb,\
                    p_stream_sdp, 
                    pSaddr, 
                    &p_media, 
                    p_error))
                {
                    ret_val = ICF_FAILURE;
                    sip_freeSdpMessage(p_sdpmsg);
                    
                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;		
                }
                
                break;
                
            case ICF_STREAM_TYPE_AUDIO:
                if(ICF_FAILURE ==icf_ssa_form_audio_from_sdp_info(p_ssa_pdb,\
                    p_stream_sdp, pSaddr, hold_option,&p_media, p_error))
                {
                    ret_val = ICF_FAILURE;
                    sip_freeSdpMessage(p_sdpmsg);
                    
                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;		
                }
                
                break;
                
            case ICF_STREAM_TYPE_VIDEO:
                if(ICF_FAILURE == icf_ssa_form_video_from_sdp_info(p_ssa_pdb,\
                    p_stream_sdp, pSaddr,hold_option, &p_media, p_error))
                {
                    ret_val = ICF_FAILURE;
                    sip_freeSdpMessage(p_sdpmsg);
                    
                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;		
                }
                
                break;
                
#ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
            case ICF_STREAM_TYPE_FAX:
                if(ICF_FAILURE == icf_ssa_form_t38_from_sdp_info(p_ssa_pdb,\
                    p_stream_sdp, pSaddr, &p_media, p_error))
                {
                    ret_val = ICF_FAILURE;
                    sip_freeSdpMessage(p_sdpmsg);
                    
                    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;		
                }
                break;
ICF_CHECK_IMS_END
#endif
#endif
            }
            /* Check if rejected stream bitmask for T38 is set in the ssa 
             * context, then form m line for with port 0.
             */

			if(Sdf_co_null != p_media)
			{ 
           		if ((SipFail == sdp_insertMediaAtIndex(
               		p_sdpmsg, p_media, media_count,
               		(SipError *)&(p_error->stkErrCode))))
				{
               		sip_freeSdpMedia(p_media);
               		sip_freeSdpMessage(p_sdpmsg);
               
               		ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                   		return ICF_FAILURE;
				}
           		else
				{
               		sip_freeSdpMedia(p_media);
               		p_media = Sdf_co_null;		
               		media_count++;
				}
			}
        } /* for (p_stream_list */
    #ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)

    if(ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL == (p_ssa_pdb->p_ssa_ctxt->bitmask
            & ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL))
    {
        /* Send the t38 m line with port 0
         */
        ICF_MEMGET(p_ssa_pdb->p_glb_pdb,\
                ICF_PORT_SIZEOF(icf_stream_sdp_info_st),ICF_MEM_COMMON,\
                p_stream_sdp_fax,\
                ICF_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)
           
        icf_port_memset((icf_void_t *)p_stream_sdp_fax,
                            0,
                            sizeof(icf_stream_sdp_info_st));
    
           
        p_stream_sdp_fax->stream_type = ICF_STREAM_TYPE_FAX;
        p_stream_sdp_fax->stream_status = ICF_MEDIA_STREAM_NW_ALLOW;
        p_stream_sdp_fax->stream_id = 2;

        if(ICF_FAILURE == icf_ssa_form_t38_from_sdp_info(p_ssa_pdb,\
                    p_stream_sdp_fax, pSaddr, &p_media, p_error))
        {
            ret_val = ICF_FAILURE;
            sip_freeSdpMessage(p_sdpmsg);
           
            if(Sdf_co_null != p_media)
            {
               sip_freeSdpMedia(p_media);
            } 
            ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
                    p_stream_sdp_fax,
                    ICF_MEM_COMMON,
                   	p_ssa_pdb->p_ecode,
                    ret_val)
            ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
            return ret_val;		
        }
			if(Sdf_co_null != p_media)
			{ 
           		if ((SipFail == sdp_insertMediaAtIndex(
               		p_sdpmsg, p_media, media_count,
               		(SipError *)&(p_error->stkErrCode))))
				{
               		sip_freeSdpMedia(p_media);
               		sip_freeSdpMessage(p_sdpmsg);
                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
                    p_stream_sdp_fax,
                    ICF_MEM_COMMON,
                   	p_ssa_pdb->p_ecode,
                    ret_val)
               
               		ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                   		return ICF_FAILURE;
				}
           		else
				{
               		sip_freeSdpMedia(p_media);
               		p_media = Sdf_co_null;		
               		media_count++;
                ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
                    p_stream_sdp_fax,
                    ICF_MEM_COMMON,
                   	p_ssa_pdb->p_ecode,
                    ret_val)
            }
				}
			}
ICF_CHECK_IMS_END
#endif
#endif
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_tbcp_from_sdp_info
 *
 * DESCRIPTION:     This function forms SDP for tbcp stream
 *   
 ******************************************************************/
 icf_return_t icf_ssa_form_tbcp_from_sdp_info(
     INOUT  	        icf_ssa_pdb_st	        *p_ssa_pdb, 
     INOUT 	        icf_stream_sdp_info_st  *p_stream_sdp,
     INOUT 		Sdf_ty_s8bit	        *pSAddr,
     OUT	SdpMedia                **p_p_media,
     OUT	Sdf_st_error		    *p_error)
{        

	icf_uint16_t            tbcp_port_num = ICF_NULL;
	Sdf_ty_s8bit            pString[ICF_MAX_STR_LEN + 2] = "\0";
    icf_uint8_t				index = ICF_NULL;    
	Sdf_ty_s8bit            pAddr[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    SdpMedia                *p_media = Sdf_co_null;
    SdpConnection           *p_connection = Sdf_co_null;
    icf_return_t            ret_val = ICF_SUCCESS;
    icf_uint8_t				transport_mode[ICF_MAX_SIZE_MEDIUM_STR] = "\0";
    Sdf_st_error            error;
	
	icf_sdp_stream_st		tbcp_sdp ;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    tbcp_sdp.p_encoded_attributes=ICF_NULL;
    tbcp_sdp.stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_INVALID;

    if(ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status)
    {                     

	if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT &\
						p_stream_sdp->stream_status)
	{
		tbcp_port_num = 0;
		tbcp_sdp = p_stream_sdp->p_local_sdp->sdp_stream; 
	}
	/*Check if the stream is ACTIVE*/
    else if(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)
    {
       	tbcp_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
        tbcp_port_num = tbcp_sdp.stream_address.port_num;
    }
	else if(!(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
	{
			for(index = 0; index < ICF_MAX_STREAM_PER_CALL; index++)
			{
				if(p_stream_sdp->stream_id == p_ssa_pdb->p_ssa_ctxt->\
												rej_stream[index].stream_id)
				{
                	if(!(ICF_STREAM_TYPE_TBCP & p_stream_sdp->stream_type))		
				    {
					   icf_port_strcpy(transport_mode, p_ssa_pdb->p_ssa_ctxt->\
                             rej_stream[index].protocol_name.str);				
                    }
					break;
				}
			}
			tbcp_port_num = 0;
		}
        if('\0' == transport_mode[0])
        {
            icf_port_strcpy(transport_mode, (icf_uint8_t *)"udp");
        }
		
		if('\0' == pString[0])
		{
			icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"TBCP");
		}

		/* Forming SDP media */
		if (Sdf_co_fail == sdf_ivk_uaFormSdpMedia(
			(icf_int8_t *)"application", tbcp_port_num, 1,(icf_int8_t *)transport_mode,(icf_int8_t *)pString,
			Sdf_co_null, Sdf_co_null, Sdf_co_null, Sdf_co_null,  
			Sdf_co_null, Sdf_co_null, &p_media, p_error))
		{
			/*ICF_SSA_TRACE(("\nFailure in froming SDP media"))*/
			sip_freeSdpMedia (p_media);
			ret_val = ICF_FAILURE;
		}
    }                      

	/*Now form the a-lines only for active streams*/
    if(((ICF_SUCCESS == ret_val) && 
		(ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status) &&
		(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)) &&\
		(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT &\
						p_stream_sdp->stream_status)))
    {
        /*Form c-line only if it is different from session level*/
		/*Fetch stream addr from the local sdp and compare it with Session 
		 * address. */
		
        if(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp->stream_status)
        {
            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR & tbcp_sdp.stream_address.\
                addr.addr_type)
            {
                icf_port_memset((icf_void_t *)&(tbcp_sdp.stream_address.addr.addr.\
                    ipv4_addr), 0, sizeof(icf_ipv4_int_address_st));
            }
        }
        if (ICF_FAILURE == icf_ssa_convert_to_IP_addr(
			(icf_transport_address_st *)&(tbcp_sdp.stream_address),(icf_uint8_t *) pAddr))
		{
			ret_val = ICF_FAILURE;
		}
				
	  if((ICF_FAILURE != ret_val) && (ICF_NULL != icf_port_strcasecmp(pAddr,\
			pSAddr)))
      {
			if(ICF_FAILURE == icf_ssa_init_n_form_c_line(p_ssa_pdb,
                &(tbcp_sdp.stream_address),
                &p_connection,p_error))
			{
				sip_freeSdpConnection (p_connection);
				sip_freeSdpMedia(p_media);
				ret_val = ICF_FAILURE;
			}			
			else if(Sdf_co_fail == sdf_listAppend((Sdf_ty_slist *)&(p_media->\
                slConnection),p_connection,&error))
            {
				sip_freeSdpConnection (p_connection);
				ret_val = ICF_FAILURE;
			}
		
		}
		
		/* form a lines for the TBCP m line */
		if(ICF_SDP_STREAM_ENCPARAMS_PRESENT & 
			p_stream_sdp->p_local_sdp->sdp_stream.bit_mask)
		{
				if(ICF_FAILURE == icf_ssa_form_encoded_attrs_in_sdp_stream(
					p_ssa_pdb,\
					p_media, 
					tbcp_sdp.p_encoded_attributes,
					p_error))
				{
					sip_freeSdpMedia (p_media);
					ret_val = ICF_FAILURE;	
				}
		}
	}
	
   if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT &\
						p_stream_sdp->stream_status)
	{
		p_stream_sdp->stream_status = ~(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
	}	
	*p_p_media = p_media;
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_ssa_form_tbcp_from_sdp_info*/


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_t38_from_sdp_info
 *
 * DESCRIPTION:     This function forms SDP for t38 stream.In case of t.38,
 *					there would be no stream which is mid-call rejected.
 *   
 ******************************************************************/
/*#ifdef ICF_FAX_SUPPORT*/
#ifdef NON_IMS_CLIENT

icf_return_t icf_ssa_form_t38_from_sdp_info(
		INOUT	icf_ssa_pdb_st			*p_ssa_pdb, 
		INOUT 	icf_stream_sdp_info_st  *p_stream_sdp,
		INOUT 		Sdf_ty_s8bit			*pSAddr,
		OUT		SdpMedia				**p_p_media,
		OUT		Sdf_st_error			    *p_error)
{

    icf_return_t         ret_val = ICF_SUCCESS;
	icf_sdp_stream_st	 t38_sdp ;
	icf_codec_attrib_st	 t38_codec_attrb ;
    SdpMedia			 *p_media = Sdf_co_null;
	Sdf_ty_s8bit         pString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pTempString[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    icf_uint8_t          transport_mode[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    icf_uint16_t         rtp_port_num = 0;
    icf_uint8_t          index = 0;
    Sdf_ty_s8bit         pAddr[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    SdpConnection        *p_connection = Sdf_co_null;
    Sdf_st_error        error;
    ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)	
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    icf_port_memset((void *)&t38_sdp, 0 , sizeof(icf_sdp_stream_st));
    t38_sdp.p_encoded_attributes=ICF_NULL;
    t38_sdp.stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_INVALID;
    t38_sdp.p_codec_info_list = ICF_NULL;
    t38_sdp.bit_mask = 0;

    t38_codec_attrb.bit_mask = ICF_NULL;
    t38_codec_attrb.p_encoded_attributes = ICF_NULL;
	
    if(ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status)
    {                    
		if(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)	
		{
			t38_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
			rtp_port_num = t38_sdp.stream_address.port_num;
		}
		else if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT &\
						p_stream_sdp->stream_status)
		{
			rtp_port_num = 0;
			t38_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
		}
		else if(!(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
		{
			for(index = 0; index < ICF_MAX_STREAM_PER_CALL; index++)
			{
				if(p_stream_sdp->stream_id == p_ssa_pdb->p_ssa_ctxt->\
						rej_stream[index].stream_id)

				{
					if(!(ICF_STREAM_TYPE_FAX & p_stream_sdp->stream_type)) 
					{
						icf_port_strcpy(transport_mode, p_ssa_pdb->p_ssa_ctxt->\
				                        rej_stream[index].protocol_name.str);				
					}
					break;
				}
			}
            rtp_port_num = 0;
		}

		if('\0' == transport_mode[0])
        {
           icf_port_strcpy(transport_mode, (icf_uint8_t *)"udptl");
        }

        if('\0' == pString[0])
		{
			icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"t38");
		}

		/* Forming SDP media */
		if (Sdf_co_fail == sdf_ivk_uaFormSdpMedia(
			(icf_int8_t *)"image", rtp_port_num, 1,(icf_int8_t *)transport_mode, (icf_int8_t *)pString, 
			Sdf_co_null, Sdf_co_null,Sdf_co_null,
			Sdf_co_null,Sdf_co_null, Sdf_co_null, &p_media, p_error))
		{
			sip_freeSdpMedia (p_media);
			return ICF_FAILURE;
		}
	}

	/*Form a-line attributes, if the stream is active*/
	if(((ICF_FAILURE != ret_val) &&\
        (ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status) &&
        (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)) &&\
		(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT &\
						p_stream_sdp->stream_status)))
	{
	   	/*Fetch the codec attributes for T.38*/

		if(ICF_NULL != t38_sdp.p_codec_info_list)
		{
			t38_codec_attrb = 
				*((icf_codec_attrib_st *)(t38_sdp.p_codec_info_list->p_data));
		}

	    /* Check if the stream status is set to c line zero then 
         * set the stream address as 0. This is done for IPV4 in this.
         */       
        if(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp->stream_status)
        {
            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR & t38_sdp.stream_address.\
                addr.addr_type)
            {
                icf_port_memset((icf_void_t *)&(t38_sdp.stream_address.addr.addr.\
                    ipv4_addr), 0, sizeof(icf_ipv4_int_address_st));
            }
        }

		if (ICF_FAILURE == icf_ssa_convert_to_IP_addr(
			(icf_transport_address_st *)&(t38_sdp.stream_address), (icf_uint8_t *)pAddr))
		{
				ret_val = ICF_FAILURE;
		}
		
        /* Form media level c-line only when the address is different from the 
         * Session level address.
         */ 
	    if((ICF_FAILURE != ret_val) && (ICF_NULL != icf_port_strcasecmp(pAddr,\
			pSAddr)))
		{
			if(ICF_FAILURE == icf_ssa_init_n_form_c_line(p_ssa_pdb, &(t38_sdp.\
                stream_address), &p_connection,p_error))
			{
				sip_freeSdpConnection (p_connection);
				sip_freeSdpMedia(p_media);
				ret_val = ICF_FAILURE;
			}
			
            else if(Sdf_co_fail == sdf_listAppend((Sdf_ty_slist *)&(p_media->\
                slConnection),p_connection,&error))
            {
                sip_freeSdpConnection (p_connection);
                ret_val = ICF_FAILURE;
            }           
		}

		if((ICF_FAILURE != ret_val) && 
		 (ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT == 
			(ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & t38_codec_attrb.bit_mask)))
		{
		if((ICF_FAILURE != ret_val) 
			&& (ICF_NULL != (ICF_FAX_T38_VERSION & 
			t38_codec_attrb.codec_params.t38_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxVersion:");
			
			icf_port_sprintf(pTempString,"%d",
				t38_codec_attrb.codec_params.t38_param.fax_version);
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			
			if (Sdf_co_fail ==    
				sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				sip_freeSdpMedia (p_media);
				ret_val = ICF_FAILURE;
			}
		}
		
		if(ICF_FAILURE != ret_val 
			&& (ICF_NULL != (ICF_FAX_T38_BITRATE & 
			t38_codec_attrb.codec_params.t38_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38MaxBitRate:");
			
			icf_port_sprintf(pTempString,"%d",
				t38_codec_attrb.codec_params.t38_param.bitrate);
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			
			if (Sdf_co_fail ==    
				sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				sip_freeSdpMedia (p_media);
				ret_val = ICF_FAILURE;
			}
		}
		
		if(ICF_FAILURE != ret_val 
			&& (ICF_NULL != (ICF_FAX_T38_RATE_MANAGEMENT & 
			t38_codec_attrb.codec_params.t38_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxRateManagement:");
			
			if(ICF_T38_RATE_MANAGEMENT_TRANSFERRED == 
				t38_codec_attrb.codec_params.t38_param.rate_management)	
			{			
				icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)"transferredTCF");
				
				if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
					pString,p_media,p_error))
				{
					sip_freeSdpMedia (p_media);
					ret_val = ICF_FAILURE;
				}
			}
		}
		if(ICF_FAILURE != ret_val 
			&& (ICF_NULL != (ICF_FAX_T38_UDP_BUFFSIZE & 
			t38_codec_attrb.codec_params.t38_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxMaxBuffer:");
			
			icf_port_sprintf(pTempString,"%d",
				t38_codec_attrb.codec_params.t38_param.udp_buffersize);
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			
			if(Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				sip_freeSdpMedia (p_media);
				ret_val = ICF_FAILURE;
			}
		}
		
		if(ICF_FAILURE != ret_val 
			&& (ICF_NULL != (ICF_FAX_T38_UDP_PACKETSIZE & 
			t38_codec_attrb.codec_params.t38_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxMaxDatagram:");
			
			icf_port_sprintf(pTempString,"%d",
				t38_codec_attrb.codec_params.t38_param.udp_packetsize);
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			
			if(Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				sip_freeSdpMedia (p_media);
				ret_val = ICF_FAILURE;
			}
		}
		
		if(ICF_FAILURE != ret_val 
			&& (ICF_NULL != (ICF_FAX_T38_ERR_CORRECTION & 
			t38_codec_attrb.codec_params.t38_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxUdpEC:");
			
			if(ICF_NULL !=  
				(ICF_T38_ERROR_CORR_REDUNDANCY &
				t38_codec_attrb.codec_params.t38_param.err_correction_method))
			{
				icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
				icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxUdpEC:");
				
				icf_port_strcpy((icf_uint8_t *)pTempString,(icf_uint8_t *)"t38UDPRedundancy ");
				icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
				
				if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
					pString,p_media,p_error))
				{
					sip_freeSdpMedia (p_media);
					ret_val = ICF_FAILURE;
				}
			}
			
			if(ICF_NULL !=  
				(ICF_T38_ERROR_CORR_FEC &
					t38_codec_attrb.codec_params.t38_param.err_correction_method))
			{
				icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
				icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  T38FaxUdpEC:");
				
				icf_port_strcpy((icf_uint8_t *)pTempString,(icf_uint8_t *)"t38UDPFEC ");
				icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
				
				if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
					pString,p_media,p_error))
				{
					sip_freeSdpMedia (p_media);
					ret_val = ICF_FAILURE;
				}
			}
		}
		}	
		/*Form attributes based on the codec level encoding parameters*/

		if(ICF_CODEC_ENC_ATTRIB_PRESENT & t38_codec_attrb.bit_mask)
		{
			if(ICF_FAILURE == icf_ssa_form_encoded_attrs_in_sdp_stream(
								p_ssa_pdb,
								p_media, 
								t38_codec_attrb.p_encoded_attributes, 
								p_error))
			{
				sip_freeSdpMedia (p_media);
				ret_val = ICF_FAILURE;	
			}
		}

		/*Form attributes based on the media level encoding parameters*/

		if(ICF_SDP_STREAM_ENCPARAMS_PRESENT & t38_sdp.bit_mask)
		{
			if(ICF_FAILURE == icf_ssa_form_encoded_attrs_in_sdp_stream(
								p_ssa_pdb,
								p_media, 
								t38_sdp.p_encoded_attributes, 
								p_error))
			{
				sip_freeSdpMedia (p_media);
				ret_val = ICF_FAILURE;
			}
		}
	}
	if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT &\
						p_stream_sdp->stream_status)
	{
		p_stream_sdp->stream_status = ~(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
	}

	*p_p_media = p_media;

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
ICF_CHECK_IMS_END
    return ret_val;

}/*End function icf_ssa_form_t38_from_sdp_info*/


#endif
/*#endif*/

/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_video_from_sdp_info
 *
 * DESCRIPTION:     This function forms SDP for vidoe stream.
 *   
 ******************************************************************/

icf_return_t icf_ssa_form_video_from_sdp_info(
		INOUT	icf_ssa_pdb_st		*p_ssa_pdb, 
		INOUT 	icf_stream_sdp_info_st  *p_stream_sdp,
		INOUT 	Sdf_ty_s8bit		*pSAddr,
	        INOUT    icf_uint8_t              hold_option,
		OUT	SdpMedia		**p_p_media,
		OUT	Sdf_st_error		*p_error)
{
	icf_codec_attrib_st  *p_codec = ICF_NULL;
    icf_sdp_stream_st	 video_sdp ;
	icf_list_st			 *p_codec_info_lst = ICF_NULL;
    SdpMedia             *p_media = Sdf_co_null;
	Sdf_ty_s8bit         pString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pTempString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pTempString_2[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]="\0";
	icf_uint8_t          transport_mode[ICF_MAX_SIZE_MEDIUM_STR]="\0"; 
    Sdf_ty_s8bit         pMediaProtocol[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    Sdf_ty_s8bit         *pBandwidth= ICF_NULL;
	Sdf_ty_s8bit         pAddr[ICF_MAX_SIZE_MEDIUM_STR]="\0";

    icf_uint8_t          index = 0;
    icf_uint16_t         rtp_port_num = 0;
    icf_uint8_t          media_mode[ICF_MAX_STR_LEN + 2] = {'\0'};
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_boolean_t        encode_codec = ICF_TRUE;
    icf_uint8_t          *p_media_mode = ICF_NULL;
    SdpConnection        *p_connection = Sdf_co_null;
    Sdf_st_error         error;

#ifdef ICF_QOS_SUPPORT
	 /* This is an 2-D array which is used to hold Qos precondition strings.
	 * Row 0 and Row 1 are used to store remote and local confirmation status.
	 * Row 2 to Row 5 are used to store remote and local desired status.
	 * Row 6 and Row 7 are used to store remote and local current status.
	 */
	icf_uint8_t			qos_iterator = 0;
    Sdf_ty_s8bit		pQosStatusStrings[ICF_MAX_NUM_QOS_LINES]
						[ICF_MAX_LENGTH_OF_QOS_LINE] = {"","","","","","",""};
    
    /* This is a 2-D array which is used to hold Bandwidth strings */
    /* SIP_S8bit           pBandwidth[ICF_MAX_NUM_BW_LINES]\
	                    [ICF_MAX_LENGTH_OF_BW_LINE] = {"RR:","RS:","AS:"}; */
#endif
    
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    icf_port_memset((void *)&video_sdp, 0 , sizeof(icf_sdp_stream_st));
    video_sdp.p_encoded_attributes=ICF_NULL;
    video_sdp.stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_INVALID;
    video_sdp.p_codec_info_list = ICF_NULL;
    video_sdp.bit_mask = 0;
    video_sdp.rtcp_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_INVALID;
    video_sdp.rtcp_address.port_num = 0;
	
    if(ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status)
    {   		
        /*If the rejected bitmask is set, then its a answer stream for the rejected stream*/
		if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & \
									p_stream_sdp->stream_status)
		{
			rtp_port_num = 0;
    			video_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
			icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"0");
            
		}

		if((ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) && 
			(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & \
									p_stream_sdp->stream_status)))
		{
    			video_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
			rtp_port_num = video_sdp.stream_address.port_num;
			
			/* Forming media formats*/
			for (p_codec_info_lst = video_sdp.p_codec_info_list;
				ICF_NULL != p_codec_info_lst; 
				p_codec_info_lst = p_codec_info_lst->p_next)
			{
				p_codec = (icf_codec_attrib_st *)p_codec_info_lst->p_data;
				
				if(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec->bit_mask)
				{
                    if(p_codec->codec_num >= 96 && p_codec->codec_num <= 127)
                    {
                        /* Dynamic Codec present
                         */
		                if((ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & \
                            p_codec->bit_mask) &&\
                            (!(ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT & p_codec->\
                            codec_params.video_param.bit_mask)))
                        {
                            encode_codec = ICF_FALSE;
                        }
                    }
                    if(ICF_TRUE == encode_codec)
                    {
                        icf_port_sprintf(pTempString,"%d ",p_codec->codec_num);
                        icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)pTempString);
                        icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)" ");
                    }  
                        
				}
			}/* End of for loop for format string.*/
		}
		if(!(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
		{
			for(index = 0; index < ICF_MAX_STREAM_PER_CALL; index++)
			{
				if(p_stream_sdp->stream_id == 
					p_ssa_pdb->p_ssa_ctxt->rej_stream[index].stream_id)
				{
					icf_port_strcpy((icf_uint8_t *)pMediaProtocol, (icf_uint8_t *)p_ssa_pdb->p_ssa_ctxt->\
                        rej_stream[index].protocol_name.str);					
				
					break;
				}
			}	
			/*This is the media format in case of rejected*/
			icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"0");
			rtp_port_num = 0;
		}	
		
		if('\0' == transport_mode[0])
        {
            icf_port_strcpy(transport_mode,(icf_uint8_t *)"RTP/AVP");
        }
        /*if the media protocol is not set, then set it as default one.*/
	/*	if('\0' == pMediaProtocol[0])
		{
		icf_port_strcpy(pMediaProtocol,transport_mode);
		}*/

		if((ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) &&
			(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & 
									p_stream_sdp->stream_status)))
		{
				/* Check for the bandwidth parameter and if present then form the 
				 * bandwidth value.
				 */
				if((ICF_SDP_STREAM_MEDIA_STREAM_PRESENT &\
										p_stream_sdp->p_local_sdp->sdp_stream.bit_mask) && 
								(ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT & 
								 video_sdp.media_stream_param.video_profile.bit_mask))
				{    	
						pTempString_2[0] = '\0';
						icf_port_strcpy((icf_uint8_t *)pTempString_2,(icf_uint8_t *)"AS:");

						icf_port_sprintf(pTempString,"%d ",\
										video_sdp.media_stream_param.\
										video_profile.bandwidth);

						icf_port_strcat((icf_uint8_t *)pTempString_2, (icf_uint8_t *)pTempString);

						ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
										icf_port_strlen((icf_uint8_t *)pTempString_2) + 1, 
										ICF_MEM_COMMON, 
										pBandwidth,
										ICF_RET_ON_FAILURE,
										p_ssa_pdb->p_ecode,
										ret_val)

								icf_port_strcpy((icf_uint8_t *)pBandwidth, (icf_uint8_t *)pTempString_2);
				}

                /* CSR 1-6207880 SPR 18842: merged from _7_1_1_Enh*/
                /* Fix Done for SPR : 18695 -----> CSR-1-6073587*/
                /* Removing the ICF_MEDIA_STATE_REMOTE_HELD bitmask check for
                 * supporting the scenario in which Incoming call with the initial
                 * INVITE containing c-line 0.0.0.0 and mode INACTIVE(Remote Hold)
                 * received from remote .
                 * In this case this bitmask is not set by IPTK and while sending
                 * the response for Re-Invite without SDP invalid mode is send during
                 * making the SDP in 200 OK as an offer  */ 
               if((ICF_CALL_MOD_REQ_WO_SDP ==
                  ((p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask)&
                                  ICF_CALL_MOD_REQ_WO_SDP)))
               {
                  ret_val = icf_ssa_get_mode_from_stream_info(p_ssa_pdb,
                               p_stream_sdp->application_mode,
                               hold_option,
                               media_mode);
               }
               else
               {
				  /* Get the media mode*/
			      ret_val = icf_ssa_get_mode_from_stream_info(p_ssa_pdb,
				    			p_stream_sdp->p_local_sdp->media_mode,\
					   			hold_option,media_mode);      
               }

				if('\0' != media_mode[0])
				{
						ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
										icf_port_strlen(media_mode) + 1, 
										ICF_MEM_COMMON, 
										p_media_mode,
										ICF_RET_ON_FAILURE,
										p_ssa_pdb->p_ecode,
										ret_val)    

								icf_port_strcpy(p_media_mode, media_mode);
				}
		}    
		/* Forming SDP media */
		
		if (Sdf_co_fail == sdf_ivk_uaFormSdpMedia(
								(icf_int8_t *)"video", rtp_port_num, 1, (icf_int8_t *)transport_mode, (icf_int8_t *)pString, (icf_int8_t *)p_media_mode, 
								Sdf_co_null, Sdf_co_null,pBandwidth,Sdf_co_null,Sdf_co_null, 
								&p_media, p_error))
		{
				ret_val = ICF_FAILURE;
		}
		
	}        

	/*Form attributes for only ACTIVE stream*/
	if ((ICF_FAILURE != ret_val) && 
					(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) &&
					(ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status) &&
		(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & \
							p_stream_sdp->stream_status)))
	{
			/* Form the connection (c=) line*/
			pString[0]='\0';

			/* Check if the stream status is set to c line zero then 
			 * set the stream address as 0. This is done for IPV4 in this.
			 */
			if(ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp->stream_status)
			{
					if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR & video_sdp.stream_address.\
									addr.addr_type)
					{
							icf_port_memset((icf_void_t *)&(video_sdp.stream_address.addr.addr.\
													ipv4_addr), 0, sizeof(icf_ipv4_int_address_st));
					}
			}
			/* Convert the transport address to a string*/
			if(ICF_FAILURE == icf_ssa_convert_to_IP_addr(\
									(icf_transport_address_st *)&(video_sdp.\
																  stream_address),(icf_uint8_t *)pAddr))
			{
					ret_val = ICF_FAILURE;	
			}

			if((ICF_FAILURE != ret_val) && (ICF_NULL != icf_port_strcasecmp(pAddr,\
											pSAddr)))
			{
					if(ICF_FAILURE == icf_ssa_init_n_form_c_line(p_ssa_pdb,\
											&(video_sdp.stream_address),&p_connection,p_error))
					{
							sip_freeSdpConnection(p_connection);
							ret_val = ICF_FAILURE;
					}           
					else if(Sdf_co_fail == sdf_listAppend((Sdf_ty_slist *)&(p_media->\
													slConnection),p_connection,&error))
					{
				sip_freeSdpConnection (p_connection);
				ret_val = ICF_FAILURE;
			}            
		}
		pString[0]='\0';
		pTempString[0] = '\0';
		
		for (p_codec_info_lst = video_sdp.p_codec_info_list;
			 ICF_NULL != p_codec_info_lst; 
			 p_codec_info_lst = p_codec_info_lst->p_next)
		{
			p_codec = (icf_codec_attrib_st *)p_codec_info_lst->p_data;
			
			pAttributeString[0] = '\0'; 
			
			if (ICF_NULL !=  p_codec->codec_name.str_len)
			{	
				/* If this is a dynamic codec then form the rtpmap parameter */
				if(ICF_FAILURE != ret_val) 
				{                             
					if (ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & 
									p_codec->bit_mask)
					{
						if(ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT &
							p_codec->codec_params.video_param.bit_mask)
						{
						 icf_port_sprintf(pAttributeString, "  rtpmap:%d %s/%d",
                                                                            p_codec->codec_num,
                                                                            p_codec->codec_name.str,
								p_codec->codec_params.video_param.clock_rate);
							if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
									pAttributeString,p_media,p_error))
							{
								ret_val = ICF_FAILURE;
							}
						}
                       				else if(!(ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & 
									p_codec->bit_mask))
                        			{
                            				/* Clock rate not present*/
                            				ICF_PRINT(((icf_uint8_t *)"\n Clock Rate for codec "
                                				"attribute missing"));
                            				continue;
                        			}
					}
				}
			}
					
			if(ICF_FAILURE != ret_val && 
				(ICF_NULL != (ICF_VIDEO_CODEC_FRAMESIZE_PRESENT &
				p_codec->codec_params.video_param.bit_mask)))
			{
				icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
				icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  framesize:");
				
                icf_port_sprintf(pTempString,"%d %d-%d",\
					p_codec->codec_num,\
                    p_codec->codec_params.video_param.frame_size.width,
					p_codec->codec_params.video_param.frame_size.height);	
				
				icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *) pTempString);
				if (Sdf_co_fail ==    
					sdf_fn_uaFormAttributeFromString(
					pString,p_media,p_error))
				{
					ret_val = ICF_FAILURE;
				}
			}
#ifdef ICF_RTCP_ATTR_SUPPORT
            /* Form the rtcp attribute line , if present
             */
            if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_RTCP_INFO_PRESENT &\
                        video_sdp.bit_mask))
            {
                pAttributeString[0] = '\0';
                pTempString[0] = '\0';
                icf_port_strcpy(pAttributeString,"  rtcp:");
                icf_port_itoa(video_sdp.rtcp_address.\
                        port_num,pTempString);
                /*            pTempString.str_len = icf_port_strlen(pTempString.str);
                 */

                icf_port_strcat(pAttributeString,pTempString);

                pTempString[0] = '\0';

                icf_port_strcat(pAttributeString," IN");

                if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == video_sdp.rtcp_address.\
                        addr.addr_type)
                {

#ifdef ICF_IPV6_ENABLED
                    icf_port_strcat(pAttributeString," IP6");
#endif
                }
                else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == video_sdp.\
                        rtcp_address.addr.addr_type)
                {
                    icf_port_strcat(pAttributeString," IP4");

                    /* Convert the transport address to a string*/
                    ret_val = icf_ssa_convert_to_IP_addr(\
                            (icf_transport_address_st *)&(video_sdp.rtcp_address),
                            pTempString);
                }
                else
                {
                    /* Domain Name or any other address type is not valid*/
                    ret_val = ICF_FAILURE;
                }
                if(ICF_FAILURE != ret_val)
                {               
                    icf_port_strcat(pAttributeString," ");
                    icf_port_strcat(pAttributeString,pTempString);  

                    if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                                pAttributeString,p_media,p_error))
                    {
                        ret_val = ICF_FAILURE;
                    }
                }
            }/* End of rtcp attribute information*/
#endif
			/*Form the a-line corresponding to the encoded attributes of the codec*/ 
			if(ICF_FAILURE != ret_val && (ICF_NULL != (p_codec->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)))
			{
				if(ICF_FAILURE == icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
				p_media, p_codec->p_encoded_attributes, p_error))
				{
					ret_val = ICF_FAILURE;	
				}
			}
		}

        	if(ICF_FAILURE != ret_val && 
			(ICF_NULL != (ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT
			& video_sdp.media_stream_param.video_profile.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  quality:");
			
			icf_port_sprintf(pTempString,"%d",
				video_sdp.media_stream_param.video_profile.encoding_quality);
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			
			if (Sdf_co_fail ==    
				sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				ret_val = ICF_FAILURE;
			}
		}

		if(ICF_FAILURE != ret_val && 
			(ICF_NULL != (ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT &
			video_sdp.media_stream_param.video_profile.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  framerate:");
			
			icf_port_sprintf(pTempString,"%d",
				video_sdp.media_stream_param.video_profile.frame_rate);	
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			if (Sdf_co_fail ==    
				sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				ret_val = ICF_FAILURE;
			}
		}

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
			/* Create and append QoS related attribute strings */
			/* Create and append the QoS status strings to the 
			* SDP media attribute lines */
			if(ICF_FAILURE == icf_ssa_prepare_qos_status_lines_from_sdp(p_stream_sdp->p_local_sdp,pQosStatusStrings))
			{
				ret_val = ICF_FAILURE;
			}
			else
			{
				for(;qos_iterator<ICF_MAX_NUM_QOS_LINES;qos_iterator++)
				{
					icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
					icf_port_strcpy((icf_uint8_t *)pTempString,(icf_uint8_t *)"");
					if (0 != icf_port_strcmp((icf_uint8_t *)pQosStatusStrings[qos_iterator],(icf_uint8_t *)"\0") )
					{
						icf_port_sprintf((icf_int8_t *)pTempString,"%s",pQosStatusStrings[qos_iterator]);
						icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)pTempString);
						if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
							pString,p_media,p_error))
						{
							ret_val = ICF_FAILURE;
						}
					}	
				}
			}
ICF_CHECK_QOS_SUPPORT_END
#endif	

		/*Form the attributes based on the media level encoding parameters*/         
		if(ICF_FAILURE != ret_val &&\
		 	(ICF_NULL != (ICF_SDP_STREAM_ENCPARAMS_PRESENT
				& video_sdp.bit_mask)))
		{ 
			if(ICF_FAILURE == 
				icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
									p_media,
									video_sdp.p_encoded_attributes,
									 p_error))
			{
				ret_val = ICF_FAILURE;	
			}
		}
	}

	/*Free this memory as this has alredy been pointed by SIP-STACK
	*So,there is no need to keep it locally
	*/

	if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & p_stream_sdp->stream_status)
	{
       /*Reset the bitmask*/
        p_stream_sdp->stream_status &= \
                ~(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
	}

	if(p_media_mode)	
	{
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
                    p_media_mode,
                    ICF_MEM_COMMON,
                   	p_ssa_pdb->p_ecode,
                    ret_val)
	}

	if(pBandwidth)	
	{
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
					pBandwidth,
					ICF_MEM_COMMON,
					p_ssa_pdb->p_ecode,
					ret_val)
	}

	if(ICF_FAILURE == ret_val)
	{
		sip_freeSdpMedia (p_media);
	}
	else
	{
		/*Assign the media-line*/
		*p_p_media = p_media;
	}

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    	return ret_val;
}

/***************************************************************************
**
** FUNCTION:     icf_ssa_form_audio_from_sdp_info
** DESCRIPTION:  This function forms the SDP for the audio media parameters 
**               present in the SDP structure of the call context.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**               This function forms Sip Sdp from p_local_sdp in the list 
**               of type icf_stream_sdp_info_st
**
***************************************************************************/
icf_return_t icf_ssa_form_audio_from_sdp_info(
        INOUT    icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT     icf_stream_sdp_info_st    *p_stream_sdp,
        INOUT 	  Sdf_ty_s8bit        	    *pSAddr,
        INOUT     icf_uint8_t               hold_option,
        OUT SdpMedia                  **p_p_media,
        /* list of type icf_stream_sdp_info_st */
        OUT   Sdf_st_error              *p_error)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_sdp_stream_st    audio_sdp ;
    icf_codec_attrib_st  *p_codec_info = ICF_NULL;
    icf_uint8_t          media_mode[ICF_MAX_STR_LEN + 2] = {'\0'};        
    icf_uint16_t         rtp_port_num = 0;
    Sdf_ty_s8bit         pString[ICF_MAX_STR_LEN + 2] = "\0";
    Sdf_ty_s8bit         pTempString[ICF_MAX_STR_LEN + 2] = "\0";
    Sdf_ty_s8bit         pTempString_2[ICF_MAX_STR_LEN + 2] = "\0";
    icf_uint8_t          index = 0;
    SdpMedia             *p_media = Sdf_co_null;
    Sdf_ty_s8bit         *pBandwidth= ICF_NULL;
    icf_uint8_t          transport_mode[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    icf_list_st          *p_codec_lst = ICF_NULL;
    icf_boolean_t        encode_codec = ICF_TRUE;
    icf_uint8_t          *p_media_mode = ICF_NULL;
   
    SdpConnection        *p_connection=Sdf_co_null;
    Sdf_st_error         error;



#ifdef ICF_QOS_SUPPORT
	 /* This is an 2-D array which is used to hold Qos precondition strings.
	 * Row 0 and Row 1 are used to store remote and local confirmation status.
	 * Row 2 to Row 5 are used to store remote and local desired status.
	 * Row 6 and Row 7 are used to store remote and local current status.
	 */
	icf_uint8_t			qos_iterator = 0;
    Sdf_ty_s8bit		pQosStatusStrings[ICF_MAX_NUM_QOS_LINES]
						[ICF_MAX_LENGTH_OF_QOS_LINE] = {"","","","","","",""};
    
    /* This is a 2-D array which is used to hold Bandwidth strings */
    /* SIP_S8bit           pBandwidth[ICF_MAX_NUM_BW_LINES]\
	                    [ICF_MAX_LENGTH_OF_BW_LINE] = {"RR:","RS:","AS:"}; */
#endif
#ifdef ICF_RTCP_ATTR_SUPPORT
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]="\0";

#endif
    
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    
/* UMR Fix */
    error.errCode=0;
    error.stkErrCode=0;
    error.appErrCode=0;
    error.ErrMsg[0]='\0';

    icf_port_memset((void *)&audio_sdp, 0 , sizeof(icf_sdp_stream_st));    

    audio_sdp.p_encoded_attributes=ICF_NULL;
    audio_sdp.stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_INVALID;
    audio_sdp.p_codec_info_list = ICF_NULL;
    audio_sdp.bit_mask = 0;
    audio_sdp.rtcp_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_INVALID;
    audio_sdp.rtcp_address.port_num = 0;


	p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
    
        /* IF ICF_MEDIA_STREAM_NW_ALLOW bit is set in the stream_status of stream sdp 
         * IF this is an answer to an offer then ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT bit
         * will be set. Search for this stream id in the rejected stream array for the protocol. 
         * If found then set the protocol for this rejected media stream.
         * Else if ICF_MEDIA_STREAM_ACTIVE bit is set in the stream status of stream sdp
         * then extract the rtp_port number.
         * If the ICF_MEDIA_STREAM_ACTIVE bit is not set then media stream is rejected.
         * Mark the rtp port number as 0.
         */
    if(ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status)
    {
        if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & p_stream_sdp->stream_status)
        {
			rtp_port_num = 0;
    	   audio_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
			icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"0");
        }

       if ((ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) &&
			(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & \
									p_stream_sdp->stream_status)))
       {
    	   audio_sdp = p_stream_sdp->p_local_sdp->sdp_stream;
           rtp_port_num = audio_sdp.stream_address.port_num;
           
           for (p_codec_lst = audio_sdp.p_codec_info_list; 
           ICF_NULL != p_codec_lst;
           p_codec_lst = p_codec_lst->p_next)
           {
               p_codec_info = (icf_codec_attrib_st *)p_codec_lst->p_data;
               encode_codec=ICF_TRUE;
               
               if(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_info->bit_mask)
               {
                   if(p_codec_info->codec_num >= 96 && p_codec_info->codec_num <= 127)
                   {
                   /* Dynamic Codec present
                       */
                       if(!(ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & \
                           p_codec_info->bit_mask) &&\
                           (!(ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE & p_codec_info->\
                           codec_params.audio_param.bit_mask)))
                       {
                           encode_codec = ICF_FALSE;
		       }
		   }
		   if(ICF_TRUE == encode_codec)
		   {
			   icf_port_sprintf((icf_int8_t *)pTempString,"%d ",p_codec_info->codec_num);
			   icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)pTempString);
			   icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)" ");
		   }  
	       }

	   }
       }
       if(!(ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status))
       {
	       icf_stream_id_t    stream_id;
	       for(index = 0; index < ICF_MAX_STREAM_PER_CALL; index++)
	       {
		       stream_id = p_ssa_ctxt->rej_stream[index].stream_id;

		       if(stream_id == p_stream_sdp->stream_id)
		       {
			       icf_port_strcpy(transport_mode,p_ssa_ctxt->\
					       rej_stream[index].protocol_name.str);                    
			       break;
		       }

	       }
	       rtp_port_num = 0;
	       icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"0");
       }

       if(0 == icf_port_strcmp((icf_uint8_t *)transport_mode,(icf_uint8_t *)"\0"))
       {
	       icf_port_strcpy((icf_uint8_t *)transport_mode,(icf_uint8_t *)"RTP/AVP");
       }   

       if((ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status) &&
			(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & \
									p_stream_sdp->stream_status)))
       {
         /*Changes done for CSR:1-5943186 / SPR:18552*/
         /* 
          * If Re-Invite without SDP is received from remote and application
          * is on remote hold then send application mode in SDP of 200 OK for
          * received Re-Invite without sdp.
          */ 

         /* Fix: CSR 1-6207880 : merged from 7_1_1_Enh branch*/
         /* Fix Done for SPR : 18695 -----> CSR-1-6073587*/
         /* Removing the ICF_MEDIA_STATE_REMOTE_HELD bitmask check for
          * supporting the scenario in which Incoming call with the initial
          * INVITE containing c-line 0.0.0.0 and mode INACTIVE(Remote Hold)
          * received from remote .
          * In this case this bitmask is not set by IPTK and while sending
          * the response for Re-Invite without SDP invalid mode is send during
          * making the SDP in 200 OK as an offer  */ 

           if((ICF_CALL_MOD_REQ_WO_SDP ==
                  ((p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->common_bitmask)&
                                  ICF_CALL_MOD_REQ_WO_SDP)))

           {
	         ret_val = icf_ssa_get_mode_from_stream_info(p_ssa_pdb,
		                   p_stream_sdp->application_mode,
		   	               hold_option,
			               media_mode);
           }
           else
           { 
             ret_val = icf_ssa_get_mode_from_stream_info(p_ssa_pdb,
		                   p_stream_sdp->p_local_sdp->media_mode,
		                   hold_option,
			               media_mode);

            /*Reset bitmask when it is set and call is resumed*/
  
             if((ICF_MEDIA_STATE_REMOTE_HELD == 
                ((p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->media_state) & 
                ICF_MEDIA_STATE_REMOTE_HELD)) &&
                (ICF_MEDIA_MODE_SEND_RECV == 
                 p_stream_sdp->p_local_sdp->media_mode))
              {
                 p_ssa_pdb->p_ssa_ctxt->p_call_ctxt->media_state &= 
                          ~ICF_MEDIA_STATE_REMOTE_HELD;
              }
           }

	       if('\0' != media_mode[0])
	       {
		       ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
				       icf_port_strlen(media_mode)+1, 
				       ICF_MEM_COMMON, 
				       p_media_mode,
				       ICF_RET_ON_FAILURE,
				       p_ssa_pdb->p_ecode,
				       ret_val)    

			       icf_port_strcpy(p_media_mode, media_mode);
	       }    
	       /* Check for the bandwidth parameter and if present then form the 
		* bandwidth value.
		*/

	       if((ICF_SDP_STREAM_MEDIA_STREAM_PRESENT &\
				       p_stream_sdp->p_local_sdp->sdp_stream.bit_mask) &&
			       (ICF_AUDIO_PROF_BNDWDTH_PRESENT & audio_sdp.media_stream_param.\
				audio_profile.bit_mask))
	       {	
		       icf_port_strcpy((icf_uint8_t *)pTempString_2,(icf_uint8_t *)"AS:");

		       icf_port_sprintf(pTempString,"%d ",\
				       audio_sdp.media_stream_param.\
				       audio_profile.bandwidth);

		       icf_port_strcat((icf_uint8_t *)pTempString_2, (icf_uint8_t *)pTempString);

		       ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
				       icf_port_strlen((icf_uint8_t *)pTempString_2)+1, 
				       ICF_MEM_COMMON, 
				       pBandwidth,
				       ICF_RET_ON_FAILURE,
				       p_ssa_pdb->p_ecode,
				       ret_val)

			       icf_port_strcpy((icf_uint8_t *)pBandwidth, (icf_uint8_t *)pTempString_2);
	       }    

       }
       /* Form the Media Stream in the SDP*/

       if(Sdf_co_fail == sdf_ivk_uaFormSdpMedia((icf_int8_t *)"audio", rtp_port_num,\
			       1,(icf_int8_t *)transport_mode, (icf_int8_t *)pString, (icf_int8_t *)p_media_mode, Sdf_co_null, Sdf_co_null,\
			       (icf_int8_t *)pBandwidth,Sdf_co_null, Sdf_co_null, &p_media, p_error))
       {
	       ret_val = ICF_FAILURE;
       }

    } /*End of NW_ALLOW if.*/

    /* Form the attribute parameters for the media stream : rtp_port_num != 0*/
    if (((ICF_FAILURE != ret_val) &&\
		    (ICF_MEDIA_STREAM_NW_ALLOW & p_stream_sdp->stream_status) &&
		    (ICF_MEDIA_STREAM_ACTIVE & p_stream_sdp->stream_status)) &&
			(!(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & \
									p_stream_sdp->stream_status)))
    {
		
        pString[0]='\0';
        pTempString[0] = '\0';    
        /* Form the connection (c=) line*/
		
        /* Check if the stream status is set to c line zero then 
         * set the stream address as 0. This is done for IPV4 in this.
         */
        if((ICF_MEDIA_STREAM_CLINE_ZERO & p_stream_sdp->stream_status) ||
           (ICF_SSA_HOLD_CLINE_ZERO == (ICF_SSA_HOLD_CLINE_ZERO & hold_option)))
        {
            if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR & audio_sdp.stream_address.\
                addr.addr_type)
            {
                icf_port_memset((icf_void_t *)&(audio_sdp.stream_address.addr.addr.\
                    ipv4_addr), 0, sizeof(icf_ipv4_int_address_st));
            }
        }

		/* Convert the transport address to a string*/
		if(ICF_FAILURE == icf_ssa_convert_to_IP_addr(\
			(icf_transport_address_st *)&(audio_sdp.\
			stream_address),(icf_uint8_t *)pString))
		{
			sip_freeSdpMedia (p_media);
			ret_val = ICF_FAILURE;	
		}
		
		if((ICF_FAILURE != ret_val) && (ICF_NULL != icf_port_strcasecmp(pString,\
			pSAddr)))
		{
			if(ICF_FAILURE == icf_ssa_init_n_form_c_line(p_ssa_pdb,\
				&(audio_sdp.stream_address),&p_connection,p_error))
			{
				sip_freeSdpMedia (p_media);
				sip_freeSdpConnection(p_connection);
				ret_val = ICF_FAILURE;
			}
			
			else if(Sdf_co_fail == sdf_listAppend((Sdf_ty_slist *)&(p_media->\
                slConnection),p_connection,&error))
            {
				sip_freeSdpConnection (p_connection);
				ret_val = ICF_FAILURE;
			}            
		}
        
        /* Form the codec attributes for the audio media stream.*/
        ret_val = icf_ssa_form_audio_codec_attrs_in_media_stream(p_ssa_pdb,\
												p_media,
												audio_sdp.p_codec_info_list,                                                
												p_error);
              
       /* IF the media stream bitmask is present then form the media stream attributes.
        */
        if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_MEDIA_STREAM_PRESENT &\
            audio_sdp.bit_mask))
        {
            ret_val = icf_ssa_form_media_attrs_in_audio_stream(p_ssa_pdb,						      p_stream_sdp,											      p_media,												      p_error);
        }
        
#ifdef ICF_RTCP_ATTR_SUPPORT
        /* Form the rtcp attribute line , if present
         */
        if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_RTCP_INFO_PRESENT &\
            audio_sdp.bit_mask))
        {
            pAttributeString[0] = '\0';
            pTempString[0] = '\0';
            icf_port_strcpy(pAttributeString,"  rtcp:");
            icf_port_itoa(audio_sdp.rtcp_address.\
                port_num,pTempString);
/*            pTempString.str_len = icf_port_strlen(pTempString.str);
*/

            icf_port_strcat(pAttributeString,pTempString);

            pTempString[0] = '\0';

            icf_port_strcat(pAttributeString," IN");
          
            if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == audio_sdp.rtcp_address.\
                addr.addr_type)
            {
                
#ifdef ICF_IPV6_ENABLED
                icf_port_strcat(pAttributeString," IP6");
#endif
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == audio_sdp.\
                rtcp_address.addr.addr_type)
            {
                icf_port_strcat(pAttributeString," IP4");
			
            /* Convert the transport address to a string*/
            ret_val = icf_ssa_convert_to_IP_addr(\
        		(icf_transport_address_st *)&(audio_sdp.rtcp_address),
				pTempString);
			}
            else
            {
                /* Domain Name or any other address type is not valid*/
                ret_val = ICF_FAILURE;
            }
            if(ICF_FAILURE != ret_val)
            {               
                icf_port_strcat(pAttributeString," ");
                icf_port_strcat(pAttributeString,pTempString);  
                
                if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                    pAttributeString,p_media,p_error))
                {
                    ret_val = ICF_FAILURE;
                }
            }
        }/* End of rtcp attribute information*/
#endif

#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
			
			/* Create and append QoS related attribute strings */
			/* Create and append the QoS status strings to the 
			* SDP media attribute lines */
			if(ICF_FAILURE == icf_ssa_prepare_qos_status_lines_from_sdp(p_stream_sdp->p_local_sdp,pQosStatusStrings))
			{
				ret_val = ICF_FAILURE;
			}
			else
			{
				for(;qos_iterator<ICF_MAX_NUM_QOS_LINES;qos_iterator++)
				{
					icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
					icf_port_strcpy((icf_uint8_t *)pTempString,(icf_uint8_t *)"");
					if ( 0 != icf_port_strcmp((icf_uint8_t *)pQosStatusStrings[qos_iterator],(icf_uint8_t *)"\0") )
					{
						icf_port_sprintf(pTempString,"%s",pQosStatusStrings[qos_iterator]);
						icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)pTempString);
						if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
							pString,p_media,p_error))
						{
							ret_val = ICF_FAILURE;
						}
					}	
				}
			}
ICF_CHECK_QOS_SUPPORT_END
#endif

		 /* Form the encoded attributes if present for the media
        */
        if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_ENCPARAMS_PRESENT &\
            audio_sdp.bit_mask))
        {
            ret_val = icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
                p_media,(icf_list_st *)audio_sdp.\
                p_encoded_attributes,p_error);
            
        }/*End of if block for Encoded attributes.*/
        
    }/* End of if check for making the attributes line.*/

	
	if(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT & p_stream_sdp->stream_status)
	{
       /*Reset the bitmask*/
        p_stream_sdp->stream_status &= \
                ~(ICF_MEDIA_STREAM_REMOTE_OFFER_MID_CALL_REJECT);
	}

	if(p_media_mode)	
	{	
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
					p_media_mode,
					ICF_MEM_COMMON,
					p_ssa_pdb->p_ecode,
					ret_val)
	}

	if(pBandwidth)	
	{
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
					pBandwidth,
					ICF_MEM_COMMON,
					p_ssa_pdb->p_ecode,
					ret_val)
	}

	/* Free the SDP Message and p_media in case of failure.    */

	if(ICF_FAILURE == ret_val)
	{
			sip_freeSdpMedia (p_media);
	}
	else
	{
			*p_p_media = p_media;
	}

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
			return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_mode_from_stream_info
 * DESCRIPTION:     This function gets the mode for the specified stream
 *                  information.If media mode is send_recv then mode 
 *                  information will be set to NULL.
 *   
 ***********************************************************************/

icf_return_t icf_ssa_get_mode_from_stream_info(
				INOUT	icf_ssa_pdb_st	*p_ssa_pdb,
				INOUT  icf_uint8_t     media_mode,
				INOUT  icf_uint8_t     hold_option,
				OUT icf_uint8_t     *p_mode)
{
    icf_return_t    ret_val = ICF_SUCCESS;

		
    if(ICF_NULL == p_ssa_pdb)
    {
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

				if ( ICF_TRUE == 
    				    ICF_IS_BIT_SET(hold_option,ICF_SSA_OPTIONS_MODE_INACT))
    			{
						icf_port_strcpy(p_mode,(icf_uint8_t *)"inactive");
				}
				else if (ICF_TRUE == 
				        ICF_IS_BIT_SET(hold_option,ICF_SSA_HOLD_MODE_SENDONLY))
				{
					icf_port_strcpy(p_mode,(icf_uint8_t *)"sendonly");
				}
				else if ( ICF_FALSE == 
				        ICF_IS_BIT_SET(hold_option,ICF_SSA_HOLD_MODE_SENDONLY))
				{
						if(ICF_MEDIA_MODE_INACTIVE == media_mode)
						{
								icf_port_strcpy(p_mode,(icf_uint8_t *)"inactive");
						}
						else if (ICF_MEDIA_MODE_SEND_ONLY == media_mode)
						{
								icf_port_strcpy(p_mode,(icf_uint8_t *)"sendonly");
						}
						else if(ICF_MEDIA_MODE_RECV_ONLY == media_mode)
						{
								icf_port_strcpy(p_mode,(icf_uint8_t *)"recvonly");
						}
						else if(ICF_MEDIA_MODE_SEND_RECV == media_mode)
						{                
								p_mode[0] = '\0';	
						}
						else
						{
								ret_val = ICF_FAILURE;
						}
				}

		ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
				return ret_val;;
}



/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_audio_codec_attrs_in_media_stream
 * DESCRIPTION:     This function forms the codec attributes for the audio 
 *                  stream. This function will form the rtpmap and AMR codec 
 *                  attribute in the media stream. Also form the encoded attributes
 *                  lines.
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_audio_codec_attrs_in_media_stream(
        INOUT	  icf_ssa_pdb_st	        *p_ssa_pdb,
        INOUT          SdpMedia                  *p_media,
        INOUT     icf_list_st               *p_codec_info_list,    
        OUT   Sdf_st_error              *p_error)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    icf_uint8_t          pTempString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    icf_list_st          *p_codec_lst = ICF_NULL;   
    icf_codec_attrib_st  *p_codec_info = ICF_NULL;
    icf_string_st        sampling_rate;
   
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

	for (p_codec_lst = p_codec_info_list;ICF_NULL != p_codec_lst;\
        p_codec_lst = p_codec_lst->p_next)
    {
        p_codec_info = (icf_codec_attrib_st *)p_codec_lst->p_data;

       /* Form the rtpmap parameter if codec name is present
        */
        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"\0");        
        /* If this is a dynamic codec than form the rtpmap parameter 
        * with encoding attributes
        */
        
        if((ICF_FAILURE != ret_val) && \
            (ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_info->bit_mask))
            
        {
            if(p_codec_info->codec_name.str_len)
            {
                icf_port_sprintf(pAttributeString,"  rtpmap:%d %s",
                    p_codec_info->codec_num,p_codec_info->codec_name.str);

                if(ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & p_codec_info->\
                    bit_mask)
                {
                    if(ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE & p_codec_info->\
                        codec_params.audio_param.bit_mask)
                    {
                        
                        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"/");

                        icf_port_sprintf((icf_int8_t *)sampling_rate.str,"%d",p_codec_info->\
                            codec_params.audio_param.sampling_rate);
                        sampling_rate.str_len = (icf_int16_t)icf_port_strlen(\
                            sampling_rate.str);
                        
                        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)sampling_rate.str);
                    }
                    else if(!(ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE & p_codec_info->\
                        codec_params.audio_param.bit_mask))
                    {
                        ICF_PRINT(((icf_uint8_t *)"\nAudio Codec Sampling Rate not present"));
                        /* This codec attribute will not be encoded, continue*/
                       continue;

                    }
                    if(ICF_AUDIO_CODEC_PARAM_CHANNEL & p_codec_info->\
                        codec_params.audio_param.bit_mask)
                    {
                        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"/");
                        
                        icf_port_sprintf((icf_int8_t *)pTempString,"%d",p_codec_info->codec_params.\
							audio_param.channels);

                        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);
                        
                    }

                    if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                       pAttributeString,p_media,p_error))
                    {   
                        ret_val = ICF_FAILURE;
                    }
                    
                    /* Form the AMR codec params attribute. 
                     */
                    if(ICF_AUDIO_CODEC_PARAM_AMR_PARAM & p_codec_info->\
                        codec_params.audio_param.bit_mask)
                    {
                        /* Invoke the AMR encoding function.*/
                        ret_val = icf_ssa_form_amr_codec_attr_in_media_stream(\
                            p_ssa_pdb,p_media,p_codec_info,p_error);                       
                        
                    } /*End of AMR Params.*/
                    
                }
            }
            else
            {
                /* Encoding Name for dynamic codec is not present. */
                ret_val = ICF_FAILURE;
            }
            
            if((ICF_FAILURE != ret_val) && (ICF_AUDIO_CODEC_PARAM_VBD_VAL &\
                  p_codec_info->codec_params.audio_param.bit_mask))
            {
                  icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"\0");        
                  icf_port_sprintf(pAttributeString,"  gpmd:%d vbd=",
                      p_codec_info->codec_num);
                 
                  if(ICF_TRUE == p_codec_info->codec_params.audio_param.vbd_val)
                  {
                      icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"yes");
                  }
                  else
                  {
                      icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"no");
                  }
                 if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                       pAttributeString,p_media,p_error))
                  {
                        ret_val = ICF_FAILURE;
                  }
            }
        }/* End of dynamic codec if block.*/
        
        /* Form the codec encoded attributes if present.*/
        if((ICF_FAILURE != ret_val) && (ICF_CODEC_ENC_ATTRIB_PRESENT &\
            p_codec_info->bit_mask))
        {
            ret_val = icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
                p_media,(icf_list_st *)p_codec_info->p_encoded_attributes,\
                p_error);
        }
    }
           
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_amr_codec_attr_in_media_stream
 * DESCRIPTION:     This function forms the amr codec attributes in the media
 *                  stream. 
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_amr_codec_attr_in_media_stream(
        INOUT	  icf_ssa_pdb_st	        *p_ssa_pdb,
        INOUT         SdpMedia                  *p_media,
        INOUT     icf_codec_attrib_st       *p_codec_info,
        OUT   Sdf_st_error              *p_error)

{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_amr_params_st   *p_amr_params = ICF_NULL;
    Sdf_ty_s8bit        pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit        pTempString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    icf_uint32_t	iterator = 0;

    if(ICF_NULL == p_ssa_pdb)
    {
         return ICF_FAILURE;
    }
   
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    p_amr_params = &(p_codec_info->codec_params.audio_param.amr_params);

    icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  fmtp:");
    icf_port_sprintf(pTempString,"%d",p_codec_info->codec_num);
    icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);
    icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" mode-set=");
    icf_port_sprintf(pTempString,"%d",p_amr_params->amr_initial_mode);
    icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);

    if ( ICF_AMR_MODE_LIST_PRESENT == 
		(p_amr_params->bit_mask & ICF_AMR_MODE_LIST_PRESENT))
    {
    	for ( iterator = 0;iterator < p_amr_params->amr_mode_count;
		iterator++)
    	{
		if ( p_amr_params->amr_mode_list[iterator] ==
			p_amr_params->amr_initial_mode)
			continue;
		icf_port_sprintf(pTempString,",%d",
			p_amr_params->amr_mode_list[iterator]);
		icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);
    	}
    } 

    if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
        pAttributeString,p_media,p_error))
    {
        ret_val = ICF_FAILURE;
    }

    if(ICF_FAILURE != ret_val)
    {
        /* IF octect aligned mode is set / crc enabled then only the 
         * attribute line will be formed. But if bandwidth efficient and 
         * crc_enabled both are set no attribute line for this will be formed
         * No attr line will be specified for amr_codec_mode value 0 or value 
         * Bandwidth efficient as this is the default value.
         */
        if(ICF_BANDWIDTH_EFF_MODE == p_amr_params->codec_mode)
        {
            /* No attribute line for this mode will be send.
             */
            if(ICF_TRUE == p_amr_params->crc_enabled)
            {
                ICF_PRINT(((icf_uint8_t *)"\nCRC has no significance with bandwidth\
                     efficient mode."));
            }
            else
            {
                ICF_PRINT(((icf_uint8_t *)"\nAMR codec mode is bandwidth\
                     efficient."));
            }
        }
        else if(ICF_OCTET_ALIGNED_MODE == p_amr_params->codec_mode ||
            ICF_TRUE == p_amr_params->crc_enabled)
        {
            /* Set the octet align attribute in the media stream.
             */
            icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  fmtp:");
            icf_port_sprintf(pTempString,"%d",p_codec_info->codec_num);
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" octet-align=1");
           
            if(ICF_TRUE == p_amr_params->crc_enabled)
            {
                icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"; crc=1");
            }                   
             
            if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                pAttributeString,p_media,p_error))
            {
                ret_val = ICF_FAILURE;
            }            
              
        }/* End of octet-aligned AMR parameters*/
    }         

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    
    return ret_val;
}

/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_encoded_attrs_in_sdp_stream
 * DESCRIPTION:     This function forms the encoded attributes in the media 
 *                  stream.
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_encoded_attrs_in_sdp_stream(
        INOUT	  icf_ssa_pdb_st	        *p_ssa_pdb,
        INOUT         SdpMedia                  *p_media,
        INOUT     icf_list_st               *p_encoded_attr_list,
        OUT   Sdf_st_error              *p_error)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    icf_string_st        *pString = ICF_NULL;
    icf_uint8_t			 *p_str = ICF_NULL; 
    icf_list_st          *p_list= ICF_NULL;


    if(ICF_NULL == p_ssa_pdb)
     {
        return ICF_FAILURE;
     }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    if(ICF_NULL != p_encoded_attr_list)
    {
        for (p_list = p_encoded_attr_list; ICF_NULL != p_list;\
            p_list= p_list->p_next)         
        {
            pString = (icf_string_st *)p_list->p_data;
			/* Ignore white spaces in the recieved string. 
			*/
						
            if(ICF_NULL != pString->str_len)
            {
   				p_str = pString->str;
				while(icf_port_isspace(*p_str))
				{
					p_str++;
				} 
				pAttributeString[0] = '\0';
		        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  ");
                icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)p_str);
                
                if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                    pAttributeString,p_media,p_error))
                {
                    ret_val = ICF_FAILURE;
                }
                
            }
        }/* End of for loop for encoded attributes.*/
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}



/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_media_attrs_in_audio_stream
 * DESCRIPTION:     This function forms the media attributes for the audio 
 *                  stream. This function will form the ptime,
 *                  silence_suppresion attributes in the media stream.
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_media_attrs_in_audio_stream(
        INOUT		icf_ssa_pdb_st	        *p_ssa_pdb,
        INOUT               icf_stream_sdp_info_st  *p_stream_sdp,
        /* list of type icf_stream_sdp_info_st */
        INOUT  SdpMedia                  *p_media,
        OUT   Sdf_st_error              *p_error)

{
    icf_return_t         ret_val = ICF_SUCCESS;
    Sdf_ty_s8bit         pTempString[ICF_MAX_STR_LEN + 2] = "\0";
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]="\0";
	icf_sdp_stream_st	 audio_sdp = p_stream_sdp->p_local_sdp->sdp_stream;

   
    if(ICF_NULL == p_ssa_pdb)
    {
       return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    /* Form the ptime attribute if present in audio profile*/
    if((ICF_AUDIO_PROF_PTIME_PRESENT & p_stream_sdp->p_local_sdp->sdp_stream.\
        media_stream_param.audio_profile.bit_mask) && \
        (ICF_NULL != audio_sdp.media_stream_param.audio_profile.pref_ptime))
    {
        /* Create Packetization period attribute */
        /* Append the packetization period in the Sdp Media
        */
		if(audio_sdp.media_stream_param.audio_profile.pref_ptime)
		{
			pAttributeString[0] = '\0';
			pTempString[0] = '\0';
			icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  ptime:");
			icf_port_sprintf(pTempString,"%d",\
				audio_sdp.media_stream_param.audio_profile.pref_ptime);
			
			icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);
			if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
				pAttributeString,p_media,p_error))
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			ICF_PRINT(((icf_uint8_t *)"\nFailed to get Ptime from AUDIO stream"));
		}		
    }
    
     /* Form the silence suppression attribute if present in audio profile*/
    if((ICF_FAILURE != ret_val) && \
        (ICF_AUDIO_PROF_SILN_SUPP_PRESENT &\
		audio_sdp.media_stream_param.audio_profile.bit_mask))
    {
        /* Create Silence Suppresion attribute */
        /* Append the Silence Suppresion in the Sdp Media
        */
        pAttributeString[0] = '\0';
        pTempString[0] = '\0';
        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"");
        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  silenceSupp:");
        if(ICF_TRUE == audio_sdp.media_stream_param.\
				audio_profile.silence_suppression)
        {
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"on");
        }
        else
        {
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"off");
        }
        
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        
        if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
            pAttributeString,p_media,p_error))
        {
            ret_val = ICF_FAILURE;
        }
     }/* End of if check for silence suppresion attribute.*/
        if((ICF_FAILURE != ret_val) && \
        (ICF_AUDIO_PROF_ECHO_CANC_PRESENT &\
		audio_sdp.media_stream_param.audio_profile.bit_mask))
       {
        /* Create Echo Canc attribute */
        /* Append the Echo Canc in the Sdp Media
        */
        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  ecan:fb ");
        if(ICF_TRUE == audio_sdp.media_stream_param.\
				audio_profile.echo_canc)
        {
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"on-");
        }
        else
        {
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"off-");
        }
        
        
        if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
            pAttributeString,p_media,p_error))
        {
            ret_val = ICF_FAILURE;
        }
        
    }/* End of if check for silence suppresion attribute.*/


    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_init_n_form_c_line
 * DESCRIPTION:     This function forms the connection line for the
 *                  transport address passed to it.Either IPV4 or IPV6
 *                   address connection line is formed.
 *
 ***********************************************************************/

icf_return_t icf_ssa_init_n_form_c_line
(
 INOUT		icf_ssa_pdb_st	            *p_ssa_pdb,
 INOUT		icf_transport_address_st	*p_conn_addr, 	
 OUT	SdpConnection               **p_p_connection,
 OUT    Sdf_st_error                *p_error)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_uint8_t                 pString[ICF_MAX_STR_LEN]= "";
 	icf_uint8_t	                ip_type[4] = "\0"; 
								/* This will store IP4 or IP6 string*/
	
  
    if(ICF_NULL == p_ssa_pdb)
    {
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
              
   /* Convert the transport address to a string*/
	if (ICF_TRANSPORT_ADDRESS_IPV6_ADDR == p_conn_addr->addr.addr_type)
	{
#ifdef ICF_IPV6_ENABLED
		strcpy(ip_type,"IP6");
#endif
	}
	else
	{
		strcpy((icf_int8_t *)ip_type,"IP4");
	}

    if(ICF_FAILURE == icf_ssa_convert_to_IP_addr(\
        (icf_transport_address_st *)&(p_conn_addr->addr),pString))
    
	{
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sip_initSdpConnection(\
		p_p_connection,(SipError *)&(p_error->stkErrCode)))
	{
		ret_val = ICF_FAILURE;
	}
	
	else if (SipFail == sdp_setNetTypeInConnection(\
		*p_p_connection, Sdf_mc_strdupSdp ("IN"),\
		(SipError *) & (p_error->stkErrCode)))
	{
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setAddrTypeInConnection(\
		*p_p_connection, Sdf_mc_strdupSdp((const icf_int8_t *)ip_type),\
		(SipError *) & (p_error->stkErrCode)))
	{
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setAddrInConnection(\
		*p_p_connection,Sdf_mc_strdupSdp ((const icf_int8_t *)pString),\
		(SipError *)&(p_error->stkErrCode)))
	{
		ret_val = ICF_FAILURE;
	}
	
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_populate_media_sdp_info 
 *
 * DESCRIPTION:     This function populates the media related   
 *                  parameters in the structures from the SDP message.
 *
 ******************************************************************/

icf_return_t icf_ssa_populate_media_sdp_info(
    INOUT   icf_ssa_pdb_st              *p_ssa_pdb,
    INOUT   SipMessage                  *p_sip_msg,
    INOUT       SdpMessage                  *p_sdpMsg,
    INOUT       SdpMedia                    *p_media_line,
    INOUT       Sdf_ty_s8bit                *p_mvalue,
    INOUT       Sdf_ty_s8bit                *p_mformat,
    INOUT      icf_inc_media_sdp_info_st	*p_media_sdp_info)
{
    icf_return_t                        ret_val = ICF_SUCCESS;
    Sdf_ty_s8bit                        dTempFormat[ICF_MAX_SIZE_MEDIUM_STR] = "\0";
    icf_uint8_t                         codecnum = 0, old_codec_count = 0, 
                                        codec_count = 0,
                                        old_codec_list[ICF_MAX_CODEC_IN_LIST] = "\0";
    Sdf_ty_s8bit                        *p_remainingcodecs = Sdf_co_null;
	
	icf_uint8_t							*p_codec = ICF_NULL;
    icf_internal_sdp_stream_st          *p_int_sdp = ICF_NULL;
	icf_list_st							*p_codec_info_list = ICF_NULL;
	icf_codec_attrib_st					*p_codec_info = ICF_NULL;
	
       
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    /* Allocate memory for the new internal sdp level structure */
    if (ICF_FAILURE == icf_dbm_get_mem_from_pool(
                            p_ssa_pdb->p_glb_pdb,
                            ICF_MEM_SDP,
                            (icf_void_t **)&p_int_sdp,
                            p_ssa_pdb->p_ecode))
    {
        ICF_SSA_DEBUG(("\[SSA]:Failure in allocating SDP block\n"));
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
        return ret_val;
    }
 
    /* Assign the newly allocated internal_sdp to the media_sdp_info      
       node */

	if(ICF_NULL != p_media_sdp_info->p_int_sdp_stream)
	{
		icf_cmn_delete_internal_sdp_stream(p_ssa_pdb->p_glb_pdb,
					&(p_media_sdp_info->p_int_sdp_stream),
					p_ssa_pdb->p_ecode);
	}

    p_media_sdp_info->p_int_sdp_stream = p_int_sdp;
    /*Set the mode as SEND RECV by default.*/
   	/* Initialize the media attrib as sendrecv */
   
    if(ICF_FALSE == icf_ssa_parse_p_early_media_hdr(
                                                 p_ssa_pdb,
                                                 (SipMessage *)p_sip_msg,
                                                 p_int_sdp))
    {
       p_int_sdp->media_mode = ICF_MEDIA_MODE_SEND_RECV;
    }

	/* NOTE: From this point onwards, there is no need to free the memory 
  	 * allocated to p_media_sdp_info and p_int_sdp_stream in failure legs.   
	 * They are being freed by the calling function in its failure leg */

	icf_port_strcpy( (icf_uint8_t *)dTempFormat, (icf_uint8_t *)p_mformat);
    
    p_codec = (icf_uint8_t *)icf_port_strtokr(dTempFormat," ", &p_remainingcodecs); 
    
    while (ICF_NULL != p_codec)
    {
        /*The current block is added so that if the m-line contains
          same codes more than once then it gets ignored and is not
          added into the codec list*/
        if ((ICF_NULL != icf_port_strcasecmp( (icf_int8_t *)"t38", (icf_int8_t *)p_codec))
            && (ICF_NULL != icf_port_strcasecmp( (icf_int8_t *)"TBCP", (icf_int8_t *)p_codec)))
        {
            codecnum = (icf_uint8_t )icf_port_atoi((const icf_int8_t *)p_codec);

            /* here we search throughout the already recieved codecs list
               i.e. the old_codec_list for presence of the current codec*/
            for(codec_count=0; codec_count < old_codec_count; codec_count++)
            {
                if(old_codec_list[codec_count] == codecnum)
                {
                    break;
                }
            }

            /* if the iterator (codec_count) value is equal to old_codec_count
               then it means that the codec was not present in the already
               recvd codec list so we add it in the recvd list and process
               it further*/
            if(old_codec_count == codec_count)
            {
                old_codec_list[old_codec_count] = codecnum;
                old_codec_count++;
            }
            /*if iterator (codec_count) is not equal to old_codec_count thn it
              means that the present codec was present in the old codec list
              so we ignore this and move ahead*/
            else
            {
                if(p_remainingcodecs)
                {
                    icf_port_strcpy((icf_uint8_t *)dTempFormat, (icf_uint8_t *)p_remainingcodecs);
                    p_codec = (icf_uint8_t *)icf_port_strtokr(dTempFormat," ", &p_remainingcodecs);
                    continue;
                }
                else
                {
                    break;
                }
            }
        }  
      ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
                ICF_PORT_SIZEOF(icf_codec_attrib_st),
                ICF_MEM_COMMON, 
                p_codec_info,
                ICF_RET_ON_FAILURE, 
                p_ssa_pdb->p_ecode,    
                ret_val)
                
        icf_port_memset((icf_void_t *)p_codec_info, 0, sizeof(icf_codec_attrib_st));
            
        /* Convert the codecs into integers */
        /*Check if the codec is t38, then fill the codec name
        * else fill the codec number
        */
        if ((ICF_NULL != icf_port_strcasecmp((const icf_int8_t *)"t38",(const icf_int8_t *)p_codec)) 
            && (ICF_NULL != icf_port_strcasecmp((const icf_int8_t *)"TBCP",(const icf_int8_t *)p_codec)))
        {
            codecnum = (icf_uint8_t)icf_port_atoi((const icf_int8_t *)p_codec);
            p_codec_info->bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
        }

        
            
            if(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_info->bit_mask)
            {
                p_codec_info->codec_num = codecnum;
            }
            else
            {
                icf_port_strcpy(p_codec_info->codec_name.str, p_codec);
                p_codec_info->codec_name.str_len = (icf_uint16_t )icf_port_strlen(p_codec);
            }

        if (ICF_NULL == icf_port_strcasecmp((const icf_int8_t *)"t38",(const icf_int8_t *)p_codec)) 
        {
            p_codec_info->codec_num = 128;
            p_codec_info->bit_mask |= ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT;
        }

        if (ICF_FAILURE == icf_cmn_add_node_to_list(
            p_ssa_pdb->p_glb_pdb,
            &p_codec_info_list,
            (icf_void_t *)p_codec_info))
            
            /* Free all the acquired memory */
        {           
            ret_val = ICF_FAILURE;
            break;
        }
        else
        {
	         p_codec_info = ICF_NULL;	
            /*If any codec is left, then copy it
            * otherwise break the loop
            */   
            if(p_remainingcodecs) 
            {
                icf_port_strcpy((icf_uint8_t *)dTempFormat, (icf_uint8_t *)p_remainingcodecs);
            	p_codec = (icf_uint8_t *)icf_port_strtokr(dTempFormat," ", &p_remainingcodecs);
	    }
	    else
	    {
            break;	
	    }
        }
    }
    
    if(ICF_FAILURE != ret_val)
    {
        /*FILL THE LIST TO THE SDP INFO STREAM*/
        p_int_sdp->sdp_stream.p_codec_info_list = p_codec_info_list;
    }

    /* Retrieve the connection details (i.e. RTP address and port) */
    
    /*
     * Check if this m= line contains a c= line with IP. If not, 
     * the session level c= line is used to retrieve RTP address
     */
    
	if((ICF_FAILURE != ret_val) && \
		(ICF_FAILURE == icf_ssa_get_rtp_addr_n_port_frm_media_line(p_ssa_pdb,\
										p_sdpMsg, p_media_line, p_int_sdp)))
	{
		ret_val = ICF_FAILURE;
	}
	
	else if((ICF_FAILURE != ret_val) && \
		(ICF_FAILURE == icf_ssa_get_bandwidth_frm_media_line(p_ssa_pdb,
													p_media_line,p_mvalue,p_int_sdp)))
																
	{
		ret_val = ICF_FAILURE;
	}
	
	else if((ICF_FAILURE != ret_val) && \
		(ICF_FAILURE == icf_ssa_get_media_parameters_frm_attrb_line(p_ssa_pdb,\
													p_media_line,p_mvalue, p_int_sdp)))
	{
		ret_val = ICF_FAILURE;
	}



	 /* Get the codec encoding name, if not received.
     */ 
     if((ICF_FAILURE != ret_val) &&\
        ((0 != icf_port_strcasecmp(p_mvalue,"application")) && \
        (0 != icf_port_strcasecmp(p_mvalue,"image"))))
    {
        ret_val = icf_ssa_fill_encoding_name_for_standard_codecs(p_ssa_pdb,\
            p_codec_info_list);
    }
     if(ICF_FAILURE == ret_val)
     {
         icf_cmn_delete_codec_attrib_list(p_ssa_pdb->p_glb_pdb,
             &p_codec_info_list,
             p_ssa_pdb->p_ecode);
            
         ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
             p_codec_info,
             ICF_MEM_COMMON,
             p_ssa_pdb->p_ecode,
             ret_val)

      	 if(ICF_NULL != p_int_sdp->sdp_stream.p_encoded_attributes)             
         {
			 icf_cmn_delete_list(p_ssa_pdb->p_glb_pdb,
						&(p_int_sdp->sdp_stream.p_encoded_attributes),
						ICF_MEM_COMMON);  
         }
	icf_dbm_ret_mem_to_pool(p_ssa_pdb->p_glb_pdb, 
                        ICF_MEM_SDP,
                        p_int_sdp, p_ssa_pdb->p_ecode);       
         p_media_sdp_info->p_int_sdp_stream = ICF_NULL;
		 ret_val = ICF_FAILURE;
     }
    
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_rtp_addr_n_port_frm_media_line 
 *
 * DESCRIPTION:     This function populates the rtp addr and port from the connection line.
 *					It first checks at the session level otherwise it checks at the
 *					media level.
 *
 ******************************************************************/

icf_return_t icf_ssa_get_rtp_addr_n_port_frm_media_line(
					INOUT		icf_ssa_pdb_st				 *p_ssa_pdb,
					INOUT      SdpMessage					 *p_sdpMsg,
					INOUT		SdpMedia					 *p_media_line,
		       		OUT		icf_internal_sdp_stream_st   *p_int_sdp)																  
{
	SipError        dError;
	Sdf_ty_u32bit	dConnCount = 0;
	Sdf_ty_s8bit	*p_addr = Sdf_co_null;
	SdpConnection	*p_sdpConnection = Sdf_co_null;	
	icf_uint16_t	port_num = 0;
	icf_return_t	ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

	if (SipFail == sdp_getConnectionCountFromMedia(
                        p_media_line,
                        &dConnCount,
                        &dError))
    {
        ICF_PRINT(((icf_uint8_t *)"\nFailed to get SdpConnection count from SdpMessage"));
        return ICF_FAILURE;
    }

    /* If the m= line doesn't contain RTP address retreive it from c= line */
    if (dConnCount == 0)
    {
        if (SipFail == sdp_getConnection(
                        p_sdpMsg,
                        &p_sdpConnection,
                        &dError)) 
        {
            ICF_PRINT(((icf_uint8_t *)"\nFailed to get SdpConnection from SdpMessage"));
            ret_val = ICF_FAILURE;
        }
    }
    else
    {
        if (SipFail == sdp_getConnectionAtIndexFromMedia(
                        p_media_line,
                        &p_sdpConnection,
                        0,
                        &dError)) 
        {
            ICF_PRINT(((icf_uint8_t *)"\nFailed to get SdpConnection from Media"));
            ret_val = ICF_FAILURE;
        }
    }

    if (ICF_SUCCESS == ret_val)
    {
        /* Retreive the RTP address */
        if (SipFail == sdp_getAddrFromConnection(
                        p_sdpConnection,
                        &p_addr,
                        &dError)) 
        {
            ICF_PRINT(((icf_uint8_t *)"\nFailed to get Address from connection details"));
            /* Free the local reference */
            sip_freeSdpConnection(p_sdpConnection);
            return  ICF_FAILURE;
        }
        /* Store the remote RTP IP in  SDP stucture of call context */
        if (ICF_FAILURE == icf_ssa_convert_to_tran_addr((icf_uint8_t *)p_addr,
            (icf_transport_address_st *)&(p_int_sdp->sdp_stream.stream_address)))
        {
			p_ssa_pdb->p_ssa_ctxt->failure_bitmask |= 
				ICF_SSA_FAILURE_RESP_CODE_488;
        
            /* This warning code and header will be used for forming warning header.*/
            p_ssa_pdb->warning_header_code = 301;
            icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"Incompatible Network Address Format");
            p_ssa_pdb->warning_text.str_len = (icf_uint16_t)icf_port_strlen(p_ssa_pdb->warning_text.str);

            sip_freeSdpConnection(p_sdpConnection);
            return  ICF_FAILURE;
        }
        if ((0 == icf_port_strcasecmp(p_sdpConnection->pAddrType,"IP4")) &&
		(p_int_sdp->sdp_stream.stream_address.addr.addr_type !=ICF_TRANSPORT_ADDRESS_IPV4_ADDR))
        {
			p_ssa_pdb->p_ssa_ctxt->failure_bitmask |= 
				ICF_SSA_FAILURE_RESP_CODE_488;
            sip_freeSdpConnection(p_sdpConnection);
            return  ICF_FAILURE;
        }

        /* Free the local reference */
        sip_freeSdpConnection(p_sdpConnection);
        
        /* Extract the port */
        if (SipFail == sdp_getPortFromMedia(p_media_line, &port_num, &dError))
        {
			ICF_PRINT(((icf_uint8_t *)"\nFailed to get port from connection details"));
            return ICF_FAILURE;
        }
        /* Store the remote port in the SDP structure */
        p_int_sdp->sdp_stream.stream_address.port_num = port_num;   
	}

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}

/*********************************************************************************
 *
 * FUNCTION:        icf_ssa_get_bandwidth_frm_media_line 
 *
 * DESCRIPTION:     This function fetches the bandwidth parameter from the 
 *					media line and populates the bandwidth value in the corresponding
 *					stream.If the ongoing stream is AUDIO, then value is populated in 
 *					parameters and if stream is VIDEO, then value is populated in its 
 *					media parameters.
 **********************************************************************************/

icf_return_t icf_ssa_get_bandwidth_frm_media_line(
				INOUT		icf_ssa_pdb_st				*p_ssa_pdb,
				INOUT		SdpMedia					*pSdpMedia,
				INOUT		Sdf_ty_s8bit				*p_mvalue,
				OUT		icf_internal_sdp_stream_st  *p_int_sdp)
{
	SIP_U32bit		dBandwidthCount = 0;
    SipError	        sip_err;
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_int8_t		*pBandwthType = ICF_NULL, *p_rem = ICF_NULL;
	SIP_S8bit		*pBandwidth = ICF_NULL;
					

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

	if(sdp_getBandwidthCountFromMedia(pSdpMedia,&dBandwidthCount,\
				&sip_err)==SipFail)
	{
		ret_val = ICF_FAILURE;
	}

	if(ICF_NULL != dBandwidthCount)
	{
		if(sdp_getBandwidthAtIndexFromMedia(pSdpMedia,&pBandwidth,0,\
					 &sip_err)==SipFail)
		{
			ret_val = ICF_FAILURE;
		}
		
		pBandwthType = icf_port_strtokr(pBandwidth, ":", &p_rem);
		
		if((0 == icf_port_strcasecmp(pBandwthType, "AS")))
		{
			if(ICF_NULL != p_rem)
			{
				if((0 == icf_port_strcasecmp(p_mvalue, "audio")))
				{
					p_int_sdp->sdp_stream.media_stream_param.audio_profile.bit_mask |= 
														ICF_AUDIO_PROF_BNDWDTH_PRESENT;

					p_int_sdp->sdp_stream.media_stream_param.audio_profile.bandwidth =\
																			(icf_uint16_t)icf_port_atoi(p_rem);
					
				}
				if((0 == icf_port_strcasecmp(p_mvalue, "video")))
				{
					p_int_sdp->sdp_stream.media_stream_param.video_profile.bit_mask |= 
														ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT;
		
					p_int_sdp->sdp_stream.media_stream_param.video_profile.bandwidth =\
																			(icf_uint16_t)icf_port_atoi(p_rem);
				}
			}
		}
        else
        {
            p_ssa_pdb->p_ssa_ctxt->failure_bitmask=\
                                        ICF_SSA_FAILURE_RESP_CODE_488;
            
            /* This warning code and header will be used for forming warning header.*/
            p_ssa_pdb->warning_header_code = 399;
            icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"Bandwidth Not Available in Media Line");
            p_ssa_pdb->warning_text.str_len = (icf_uint16_t)icf_port_strlen(p_ssa_pdb->warning_text.str);
            ret_val = ICF_FAILURE;
        }
	}
	
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}



/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_media_parameters_frm_attrb_line
 *
 * DESCRIPTION:     This function checks for any attributes parameters   
 *                  corresponding to the media-line received.
 *                  It checks for all the valid attr that are supposed   
 *                  to be received.
 *
 ******************************************************************/

icf_return_t icf_ssa_get_media_parameters_frm_attrb_line(
		 INOUT		icf_ssa_pdb_st		        *p_ssa_pdb,
         INOUT 		SdpMedia			        *p_media_line,
		 INOUT 		Sdf_ty_s8bit			    *p_mvalue,
		 INOUT   icf_internal_sdp_stream_st  *p_int_sdp_info)
{
    icf_uint32_t		dAttrCount = ICF_NULL;
	icf_uint16_t		iterator2 = 0;
	icf_list_st         *p_codec_info_list = ICF_NULL;
	icf_codec_attrib_st *p_codec_attr = ICF_NULL;
	SipError		dError;
	icf_return_t	ret_val = ICF_SUCCESS;
	icf_uint8_t		*p_temp_codec = ICF_NULL;
	icf_uint16_t	port_num = ICF_NULL;

#ifdef ICF_QOS_SUPPORT
    /* This array is used to store preconditions strings extracted from
    * SDP message body */
    Sdf_ty_s8bit    pQosStatusStrings[ICF_MAX_NUM_QOS_LINES]
                    [ICF_MAX_LENGTH_OF_QOS_LINE] = {"","","","","","",""};
    
    /* This is used as an index into the above array while filling it */
    icf_uint16_t    QosStringsIterator = 0;
#endif

	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
  
	/*get the number of attributes from the media-line*/
	if (SipFail == sdp_getAttrCountFromMedia(p_media_line,\
				&dAttrCount, &dError))
	{
		ICF_PRINT(((icf_uint8_t *)"\nAttribute not present"));
        
	}
	for (iterator2 = 0; iterator2 < (icf_uint16_t)dAttrCount; 
			iterator2++)
	{
		SdpAttr		*p_tempAttr = Sdf_co_null;
				
		if (SipFail == sdp_getAttrAtIndexFromMedia(p_media_line,\
				 &p_tempAttr, iterator2,&dError))
		{
			ICF_PRINT(((icf_uint8_t *)"\nNo attribute in media"));
			break;
		}
				
		/* Value Attribute handling */
        
        if ((Sdf_co_null != p_tempAttr) &&
                (Sdf_co_null  != p_tempAttr->pName))
		{
			icf_uint8_t                 codec_str = ICF_NULL;
			icf_int8_t                  temp_codec_str[ICF_MAX_SIZE_MEDIUM_STR] = "\0";
			icf_int8_t                  *p_rem = ICF_NULL, *p_temp_cdc_str = ICF_NULL;
			icf_boolean_t		    	attr_match = ICF_FALSE;

			if (0 == icf_port_strcasecmp(p_mvalue,"application"))
			{
				attr_match = ICF_TRUE;
				if(0 == 
					icf_port_strcasecmp(p_tempAttr->pName,"fmtp"))
				{
					icf_string_st   *p_str = ICF_NULL;
					
					ICF_MEMGET(
						p_ssa_pdb->p_glb_pdb,
						ICF_PORT_SIZEOF(icf_string_st),
						ICF_MEM_COMMON,
						(p_str),
						ICF_RET_ON_FAILURE,
						p_ssa_pdb->p_ecode,
						ret_val)
					
					icf_port_memset((icf_void_t *)p_str, 0, sizeof(icf_string_st));
					
					icf_port_strcpy(p_str->str, (icf_uint8_t *)p_tempAttr->pName);
					icf_port_strcat(p_str->str,(icf_uint8_t *) ": ");
                    if (ICF_NULL != p_tempAttr->pValue)
                    {    
					    icf_port_strcat(p_str->str, (icf_uint8_t *)p_tempAttr->pValue);
                    }    
					p_str->str_len = (icf_uint16_t)icf_port_strlen(p_str->str);
					
					icf_cmn_add_node_to_list(
						p_ssa_pdb->p_glb_pdb,
						&(p_int_sdp_info->sdp_stream.p_encoded_attributes),
						(icf_void_t *)p_str);
		
					p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;
					
				}
				else
				{
					ret_val = ICF_FAILURE;	
				} 
					sip_freeSdpAttr(p_tempAttr);
                    p_tempAttr = ICF_NULL;
					break;
			} /* if (p_mvalue == application) */
            /* fmtp code deleted for CSR-1-5118243 */	
            /*if the attrb is rtpmap, then check for dynamic codec,
             * fetch the parameters corresponding to it nd then 
			 * populate it in the respective streams.
			 */
			
			else if(0 == icf_port_strcasecmp(p_tempAttr->pName,"rtpmap"))
			{
				attr_match = ICF_TRUE;
                /*Fix for CERT window .
                 * Value of rtmap is compare against ICF_MAX_STR_LEN ie 128
                 * if the string length is greater than 128 just copy the 
                 * first 127 bytes and NULL in the last position else copy the 
                 * whole value.
                 */ 
                if (ICF_NULL != p_tempAttr->pValue)
                {    
                    if (ICF_MAX_SIZE_MEDIUM_STR < \
                        icf_port_strlen((icf_uint8_t *)p_tempAttr->pValue))
                    {
                        icf_port_strncpy(temp_codec_str,p_tempAttr->pValue, \
                                         ICF_MAX_SIZE_MEDIUM_STR - 1);
                        temp_codec_str[ICF_MAX_SIZE_MEDIUM_STR - 1]='\0'; 
                    }
                    else
                    {
                        icf_port_strcpy((icf_uint8_t *)temp_codec_str,(icf_uint8_t *)p_tempAttr->pValue);    
				    }
                }    
                p_temp_cdc_str = 
					icf_port_strtokr(temp_codec_str, " ", &p_rem);
				
				if(ICF_NULL == p_temp_cdc_str)
                     		       break;

                 /*SPR - 15079.if p_rem would be null,if the value in the rtpmap 
                  * is corrupted.So,IPTK should reject the SDP and clear the
                  * call.
                  */
                 if(ICF_NULL == p_rem)
                 {	
                    sip_freeSdpAttr(p_tempAttr);
                    p_tempAttr = ICF_NULL;
                    ret_val = ICF_FAILURE;
                    break;
                 }
	
				codec_str = (icf_uint8_t)icf_port_atoi(p_temp_cdc_str);
 				
				p_codec_info_list = p_int_sdp_info->sdp_stream.p_codec_info_list;
					
				/*Check for any dynamic codec received, if any*/
				/*Traverse the whole list*/
				while(ICF_NULL != p_codec_info_list)
				{
					p_codec_attr = (icf_codec_attrib_st *)p_codec_info_list->p_data;

					/*Check for which codec attributes has come*/
					if(codec_str == p_codec_attr->codec_num)
					{
						
						icf_port_strcpy((icf_uint8_t *)temp_codec_str,(icf_uint8_t *) p_rem);
						
						/*Fetch the codec name from codec string*/
						p_temp_cdc_str = icf_port_strtokr(temp_codec_str, "/", &p_rem);
						if(ICF_NULL != p_rem)
						{
							if(ICF_NULL != p_temp_cdc_str)
							{
								    icf_port_strcpy(p_codec_attr->codec_name.str, (icf_uint8_t *)p_temp_cdc_str);
                                
								p_codec_attr->codec_name.str_len = (icf_uint16_t)icf_port_strlen((icf_uint8_t *)p_temp_cdc_str);
						        if((p_codec_attr->codec_num >=96)
						            	&& (p_codec_attr->codec_num<= 127))
						         {
							
						            	if ((!icf_port_strcmp((icf_uint8_t *)p_temp_cdc_str, (icf_uint8_t *)"telephone-events"))     
								&& (ICF_NULL != p_ssa_pdb->p_glb_pdb->p_call_ctx))
							{
								p_ssa_pdb->p_glb_pdb->p_call_ctx->inband_dtmf_supported = ICF_TRUE;
							}      
						}	
							}
						
						
							icf_port_strcpy((icf_uint8_t *)temp_codec_str,(icf_uint8_t *) p_rem);
							/*Fetch the rate*/
							p_temp_cdc_str = icf_port_strtokr(temp_codec_str, "/", &p_rem);
								
							if(ICF_NULL != p_temp_cdc_str)
							{
								if((0 == icf_port_strcasecmp("audio", p_mvalue)))	
								{
									p_codec_attr->codec_params.audio_param.sampling_rate =\
										(icf_uint16_t)icf_port_atoi(p_temp_cdc_str);
									
									p_codec_attr->codec_params.audio_param.bit_mask    
											|= ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE;
									p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
								}
								
								if((0 == icf_port_strcasecmp("video", p_mvalue)))
								{
									p_codec_attr->codec_params.video_param.clock_rate = \
										icf_port_atoi(p_temp_cdc_str);
									p_codec_attr->codec_params.video_param.bit_mask \
										|= ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT;
									p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
								}
							}
                           }
							else
                        	{
                            	if((p_codec_attr->codec_num >=96) && \
                                (p_codec_attr->codec_num<= 127))
                            	{
                                	/* Generate 415 as codec sampling rate is missing.
                               		 */
                                	p_ssa_pdb->p_ssa_ctxt->failure_bitmask=\
                                    	ICF_SSA_FAILURE_RESP_CODE_488;
                               		ICF_PRINT(((icf_uint8_t *)"Clock-Rate/Sampling Rate for"
                                    	" dynamic codecs missing in rtpmap."));
                            
                                        /* This warning code and header will be used for forming warning header.*/            
                                        p_ssa_pdb->warning_header_code = 399;
                                        icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"Codec Sampling Rate is Missing");
                                        p_ssa_pdb->warning_text.str_len = (icf_uint16_t)icf_port_strlen(p_ssa_pdb->warning_text.str); 

                                	ret_val = ICF_FAILURE;
                                	break;
                           		 }
                        	}
								
						/*Fetch the channel*/
						if(ICF_NULL != p_rem)
						{
							icf_port_strcpy((icf_uint8_t *)temp_codec_str, (icf_uint8_t *)p_rem);
									
							if((0 == icf_port_strcasecmp("audio", p_mvalue)))
							{
								p_codec_attr->codec_params.audio_param.channels = \
												(icf_uint8_t)icf_port_atoi((const icf_int8_t *)p_rem);
											
								p_codec_attr->codec_params.audio_param.bit_mask    
											|= ICF_AUDIO_CODEC_PARAM_CHANNEL;
								p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
							}
						}		
						break;	
					}
				
					p_codec_info_list = p_codec_info_list->p_next;
				}
			}	
	        else if(0 == (icf_port_strcasecmp(p_tempAttr->pName,"gpmd")))
            {
                attr_match = ICF_TRUE;
                if (0 == icf_port_strcasecmp(p_mvalue,"audio"))
                {
                   icf_port_strcpy((icf_uint8_t *)temp_codec_str,(icf_uint8_t *) p_tempAttr->pValue);
				
					p_codec_info_list = p_int_sdp_info->sdp_stream.p_codec_info_list;
				
					p_temp_codec = (icf_uint8_t *)icf_port_strtokr(temp_codec_str, " ", &p_rem);
					if(ICF_NULL == p_temp_codec)
						break;

                  	while(ICF_NULL != p_codec_info_list)
					{
                       	p_codec_attr = (icf_codec_attrib_st *)p_codec_info_list->p_data;
                        codec_str = (icf_uint8_t)icf_port_atoi((icf_int8_t *)p_temp_codec);

                        if(codec_str == p_codec_attr->codec_num)
                        {
                           	icf_port_strcpy((icf_uint8_t *)temp_codec_str,(icf_uint8_t *) p_rem);
                           	p_temp_cdc_str = icf_port_strtokr(temp_codec_str, "=", &p_rem);
 
                            if(ICF_NULL != p_rem)
                            {
                                 if(0 == icf_port_strcasecmp(p_rem,"yes"))
                                 {
                                      p_codec_attr->codec_params.audio_param.vbd_val = ICF_TRUE;
                                      p_codec_attr->codec_params.audio_param.bit_mask |=
                                                        ICF_AUDIO_CODEC_PARAM_VBD_VAL;
                                      p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                                      break;
                                 }
                                 else
                                 {
                                      p_codec_attr->codec_params.audio_param.vbd_val = ICF_FALSE;
                                      p_codec_attr->codec_params.audio_param.bit_mask |=
                                                        ICF_AUDIO_CODEC_PARAM_VBD_VAL;
                                      p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
                                      break;
                                 }
                            }
                        }
                      
                        p_codec_info_list = p_codec_info_list->p_next;
                     }
                }
            }	
			else if(0 == (icf_port_strcasecmp(p_tempAttr->pName,"framesize")))
			{		
				attr_match = ICF_TRUE;
				if (0 == icf_port_strcasecmp(p_mvalue,"video"))
				{	
					icf_port_strcpy((icf_uint8_t *)temp_codec_str, (icf_uint8_t *)p_tempAttr->pValue);
				
					p_codec_info_list = p_int_sdp_info->sdp_stream.p_codec_info_list;
				
					p_temp_codec = (icf_uint8_t *)icf_port_strtokr(temp_codec_str, " ", &p_rem);
					if(ICF_NULL == p_temp_codec)
						break;
					
					while(ICF_NULL != p_codec_info_list)
					{
						p_codec_attr = (icf_codec_attrib_st *)p_codec_info_list->p_data;
					
						codec_str = (icf_uint8_t)icf_port_atoi((const icf_int8_t *)p_temp_codec);	
					
						if(codec_str == p_codec_attr->codec_num)
						{
							icf_port_strcpy((icf_uint8_t *)temp_codec_str, (icf_uint8_t *)p_rem);
						
							p_temp_cdc_str = icf_port_strtokr(temp_codec_str, "-", &p_rem);
						
							if(ICF_NULL != p_temp_cdc_str)
							{
								p_codec_attr->codec_params.video_param.frame_size.height = 
																(icf_uint16_t )icf_port_atoi(p_temp_cdc_str);

								if(ICF_NULL != p_rem)
								{
									p_codec_attr-> codec_params.video_param.frame_size.width = 
																(icf_uint16_t )icf_port_atoi(p_rem);
									p_codec_attr->codec_params.video_param.bit_mask |= 
									ICF_VIDEO_CODEC_FRAMESIZE_PRESENT;
								
									p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
									break;
								}
							}
						}	
					
						p_codec_info_list = p_codec_info_list->p_next;
					}/*end while*/
				}
			}

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "ptime"))
			{
				if (0 == icf_port_strcasecmp(p_mvalue,"audio"))
				{
					attr_match = ICF_TRUE;
					p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.pref_ptime = \
												(icf_uint8_t)icf_port_atoi(p_tempAttr->pValue);
					
					p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.bit_mask |= 
												ICF_AUDIO_PROF_PTIME_PRESENT;
					p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
				}
			} /* if (p_tempAttr->pName == "ptime") */

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "silenceSupp"))
            {
				attr_match = ICF_TRUE;
				if(0 == icf_port_strncmp((icf_uint8_t *)p_tempAttr->pValue,(icf_uint8_t *)"on",2))	
				{
					p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.silence_suppression = 
															ICF_TRUE;
				}
				else
				{
					p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.silence_suppression = 
															ICF_FALSE;
				}

				p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.bit_mask |= 
														ICF_AUDIO_PROF_SILN_SUPP_PRESENT;
				p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
			}
            
            else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "ecan"))
            {
				attr_match = ICF_TRUE;
               	icf_port_strcpy((icf_uint8_t *)temp_codec_str,(icf_uint8_t *) p_tempAttr->pValue);
               	p_temp_codec = (icf_uint8_t *)icf_port_strtokr(temp_codec_str, " ", &p_rem);
              
                if(ICF_NULL == p_rem)
                   break;
                 
                icf_port_strcpy((icf_uint8_t *)temp_codec_str, (icf_uint8_t *)p_rem);
				p_temp_cdc_str = icf_port_strtokr(temp_codec_str, "-", &p_rem);

				if(0 == icf_port_strcasecmp(p_temp_cdc_str,"on"))	
				{
					p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.echo_canc = 
															ICF_TRUE;
				}
				else
				{
					p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.echo_canc = 
															ICF_FALSE;
				}

				p_int_sdp_info->sdp_stream.media_stream_param.audio_profile.bit_mask |= 
														ICF_AUDIO_PROF_ECHO_CANC_PRESENT;
				p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
			}

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "framerate"))
			{
				attr_match = ICF_TRUE;
				if (0 == icf_port_strcasecmp(p_mvalue,"video"))
				{
					p_int_sdp_info->sdp_stream.media_stream_param.video_profile.frame_rate = 
														(icf_uint8_t)icf_port_atoi(p_tempAttr->pValue);
					
					p_int_sdp_info->sdp_stream.media_stream_param.video_profile.bit_mask |=\
										ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT;
					p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
				}
			} 
			
			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "quality"))
			{
				attr_match = ICF_TRUE;
				/*Check if quality value is greater than 10, then set it as 5 else fill the value received*/
				if (0 == icf_port_strcasecmp(p_mvalue,"video"))
				{
					p_int_sdp_info->sdp_stream.media_stream_param.video_profile.encoding_quality =\
										(icf_uint8_t )icf_port_atoi(p_tempAttr->pValue);
					
					p_int_sdp_info->sdp_stream.media_stream_param.video_profile.bit_mask |=\
										ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT;
					p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_MEDIA_STREAM_PRESENT;
				}
            } /* if (p_tempAttr->pName == "quality")*/
			
			/* Fix for SPR 12583
			*/

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "rtcp"))
			{
				/* Extract the rtcp attribute Line.
				*/
				icf_port_strcpy((icf_uint8_t *)temp_codec_str, (icf_uint8_t *)p_tempAttr->pValue);
				p_temp_codec = (icf_uint8_t *)icf_port_strtokr(temp_codec_str, " ", &p_rem);

				if(ICF_NULL == p_temp_codec)
					break;
					
				port_num = (icf_uint16_t )icf_port_atoi((icf_int8_t *)p_temp_codec);
				p_int_sdp_info->sdp_stream.rtcp_address.port_num = port_num;
				/* IP address is optional in the rtcp attribute line.
				*/
			    if(ICF_NULL != p_rem)
				{
					p_temp_codec = (icf_uint8_t *)icf_port_strtokr(p_rem, " ", &p_rem);
					if(ICF_NULL == p_temp_codec)
						break;

					if(ICF_NULL == icf_port_strcmp(p_temp_codec,(icf_uint8_t *)"IN"))
					{
					/* Get the Transport Address type
					*/
					p_temp_codec = (icf_uint8_t *)icf_port_strtokr(p_rem," ",&p_rem);
					if(ICF_NULL == p_temp_codec)
						break;

					if(ICF_NULL == icf_port_strcmp(p_temp_codec,(icf_uint8_t *)"IP4"))
					{
						p_int_sdp_info->sdp_stream.rtcp_address.addr.addr_type = \
							ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
						p_temp_codec = (icf_uint8_t *)icf_port_strtokr(p_rem, " ", &p_rem);
						if(ICF_NULL == p_temp_codec)
						{
							/* Incomplete RTCP Address received.*/
							ICF_PRINT(((icf_uint8_t *)"\n Invalid RTCP address"));
							ret_val = ICF_FAILURE;
						}	
						else
						{
							/* Store the IPV4 address in the rtcp address of the stream.
							*/
        					icf_ssa_convert_to_tran_addr(p_temp_codec, (icf_transport_address_st *)\
								&(p_int_sdp_info->sdp_stream.rtcp_address));
						}
					}
					else if(ICF_NULL == icf_port_strcmp(p_temp_codec,(icf_uint8_t *)"IP6"))
					{
#ifdef ICF_IPV6_ENABLED
					p_int_sdp_info->sdp_stream.rtcp_address.addr.addr_type = \
							ICF_TRANSPORT_ADDRESS_IPV6_ADDR;

					p_temp_codec = icf_port_strtokr(p_rem, " ", &p_rem);
					if(ICF_NULLL == p_temp_codec)
					{
						/* Incomplete RTCP Address received.*/
						ICF_PRINT(("\n Invalid RTCP address"));
						ret_val = ICF_FAILURE;
					}	
					else
					{
						/* Store the IPV6 address in the rtcp address of the stream.
						*/
       					icf_port_strcpy(p_int_sdp_info->sdp_stream.rtcp_address.addr.addr.ipv6_addr,\
							 p_temp_codec);
					}

#endif
					}
					else
					{
						/* Set the default domain name as Invalid
						*/
						p_int_sdp_info->sdp_stream.rtcp_address.addr.addr_type = \
							ICF_TRANSPORT_ADDRESS_INVALID;
						ret_val = ICF_FAILURE; 
					}

					}
				}
				else
				{
					/* Fill the session level address in the rtcp address.	
					*/
				    icf_port_memcpy(&(p_int_sdp_info->sdp_stream.rtcp_address.addr),\
						&(p_int_sdp_info->sdp_stream.stream_address.addr),\
						sizeof(icf_transport_addr_st));	
				}
				p_int_sdp_info->sdp_stream.bit_mask |= \
					ICF_SDP_STREAM_RTCP_INFO_PRESENT;
			}/* End of rtcp attribute line extraction*/
			/* SPR 12583 fix ends.
             */
/* Precondition: Merged from ICF_QOS_PRE_M_LINE_MERGE */
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
            if(ICF_SUCCESS == ret_val)
            {
			if ((0 == icf_port_strcasecmp(p_mvalue,"audio")) ||
				(0 == icf_port_strcasecmp(p_mvalue,"video")))
			{
					if ((0 == icf_port_strcasecmp(p_tempAttr->pName,"curr")) ||
							(0 == icf_port_strcasecmp(p_tempAttr->pName,"des")) ||
							(0 == icf_port_strcasecmp(p_tempAttr->pName,"conf")))
					{
						attr_match = ICF_TRUE;
						/* Here we are omitting "a=" token while
						* storing preconditions line. It means
						* preconditions line will be copied as it is
						* except that it will not contain "a=" token
						*/
						icf_port_strcpy(
							(icf_uint8_t *)pQosStatusStrings[QosStringsIterator],
							(icf_uint8_t *)p_tempAttr->pName);
						icf_port_strcat(
							(icf_uint8_t *)pQosStatusStrings[QosStringsIterator],
							(icf_uint8_t *)":");
						icf_port_strcat(
							(icf_uint8_t *)pQosStatusStrings[QosStringsIterator],
							(icf_uint8_t *)p_tempAttr->pValue);
						QosStringsIterator++;
					} /* if (p_tempAttr->pName == "curr/des/conf")) */
			}
            }
ICF_CHECK_QOS_SUPPORT_END
#else
		if(ICF_SUCCESS == ret_val)
        {
        	if ((0 == icf_port_strcasecmp(p_mvalue,"audio")) ||
				(0 == icf_port_strcasecmp(p_mvalue,"video")))
			{
					if ((0 == icf_port_strcasecmp(p_tempAttr->pName,"curr")) ||
							(0 == icf_port_strcasecmp(p_tempAttr->pName,"des")) ||
							(0 == icf_port_strcasecmp(p_tempAttr->pName,"conf")))
					{
						/* If the session handling is required,send back 488 response to peer */
						if (ICF_SSA_SESSION_HANDLING_REQUIRED == 
								(ICF_SSA_SESSION_HANDLING_REQUIRED & p_ssa_pdb->common_bitmask))
						{
							p_ssa_pdb->p_ssa_ctxt->failure_bitmask |= 
								ICF_SSA_FAILURE_RESP_CODE_488;
							ret_val = ICF_FAILURE;
							break;
						}
					} /* if (p_tempAttr->pName == "curr/des/conf")) */
			}
          }
#endif     

			/*Check for the t38 related attributes
			*If any attrb is found, then populate the media info 
			* corresponding to that list.
			*/
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
#ifdef ICF_FAX_SUPPORT
            if(ICF_SUCCESS != ret_val)
            {
			p_codec_info_list = p_int_sdp_info->sdp_stream.p_codec_info_list;
			if(ICF_NULL != p_codec_info_list->p_data)
			{
			p_codec_attr = (icf_codec_attrib_st *)p_codec_info_list->\
			p_data;
			}
			if(0 == icf_port_strcasecmp(p_tempAttr->pName,"T38FaxVersion"))
			{
				icf_uint8_t		fax_version = ICF_NULL;
				
				attr_match = ICF_TRUE;
				fax_version = (icf_uint8_t)icf_port_atoi(p_tempAttr->pValue);
				
				if(fax_version > ICF_T38_2002)
				{
					/* Raise error */
					ret_val = ICF_FAILURE;
				}
				else
				{
					/* Assign value */		
					p_codec_attr->codec_params.t38_param.fax_version = 
						fax_version;
					p_codec_attr->codec_params.t38_param. bit_mask 
						|= ICF_FAX_T38_VERSION;
					p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
				}
			}
			
			else if(0 == icf_port_strcasecmp(p_tempAttr->pName,"T38MaxBitRate"))
			{			
				icf_uint32_t bit_rate = ICF_NULL;
				attr_match = ICF_TRUE;	
				bit_rate = icf_port_atoi(p_tempAttr->pValue);
                
				switch(bit_rate)
				{
					case ICF_T38_2400_BIT_PER_SEC :
							p_codec_attr->codec_params.t38_param.bitrate = 
								ICF_T38_2400_BIT_PER_SEC;
						break;
					
					case ICF_T38_4800_BIT_PER_SEC : 
							p_codec_attr->codec_params.t38_param.bitrate = 
								ICF_T38_4800_BIT_PER_SEC;
						break;

					case ICF_T38_7200_BIT_PER_SEC :
							p_codec_attr->codec_params.t38_param.bitrate = 
								ICF_T38_7200_BIT_PER_SEC;
						break;		
					
					case ICF_T38_9600_BIT_PER_SEC:
							p_codec_attr->codec_params.t38_param.bitrate = 
								ICF_T38_9600_BIT_PER_SEC;
						break;		
					
					case ICF_T38_14400_BIT_PER_SEC:
							p_codec_attr->codec_params.t38_param.bitrate = 
								ICF_T38_14400_BIT_PER_SEC;
						break;		
					
					case ICF_T38_33600_BIT_PER_SEC:
							p_codec_attr->codec_params.t38_param.bitrate = 
								ICF_T38_33600_BIT_PER_SEC;
						break;		
					
					default : ret_val = ICF_FAILURE;		
				}
				if (ICF_SUCCESS == ret_val)
				{
					p_codec_attr->codec_params.t38_param.bit_mask |= ICF_FAX_T38_BITRATE;
					p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
				}
			}

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "T38FaxRateManagement"))
			{
				attr_match = ICF_TRUE;
				if(0 == icf_port_strcasecmp(p_tempAttr->pValue, "transferredTCF"))
				{
					p_codec_attr->codec_params.t38_param.rate_management =\
										ICF_T38_RATE_MANAGEMENT_TRANSFERRED;
					p_codec_attr->codec_params.t38_param.bit_mask |= ICF_FAX_T38_RATE_MANAGEMENT;
					p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
				}
			}

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "T38FaxMaxBuffer"))
			{
				attr_match = ICF_TRUE;
				p_codec_attr->codec_params.t38_param.udp_buffersize = icf_port_atoi(p_tempAttr->pValue);

				p_codec_attr->codec_params.t38_param.bit_mask |= ICF_FAX_T38_UDP_BUFFSIZE;
				p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
			}

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "T38FaxMaxDatagram"))
			{
				attr_match = ICF_TRUE;
				p_codec_attr->codec_params.t38_param.udp_packetsize = icf_port_atoi(p_tempAttr->pValue);
				
				p_codec_attr->codec_params.t38_param.bit_mask |= ICF_FAX_T38_UDP_PACKETSIZE;
				p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
			}

			else if(0 == icf_port_strcasecmp(p_tempAttr->pName, "T38FaxUdpEC"))
			{
				attr_match = ICF_TRUE;
				if(0 == icf_port_strcasecmp(p_tempAttr->pValue, "t38UDPFEC"))
				{
					p_codec_attr->codec_params.t38_param.err_correction_method |=\
													ICF_T38_ERROR_CORR_FEC;
					p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
				}
				else
				{
					p_codec_attr->codec_params.t38_param.err_correction_method |=\
														ICF_T38_ERROR_CORR_REDUNDANCY;
					p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
				}

				p_codec_attr->codec_params.t38_param.bit_mask |= ICF_FAX_T38_ERR_CORRECTION;
				p_codec_attr->bit_mask |= ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT;
			}
            }
#endif
ICF_CHECK_IMS_END
#endif

/*merged from ASB branch*/
		if((ICF_FALSE == attr_match) && (ICF_SUCCESS == ret_val))

		{
            if (ICF_NULL != p_tempAttr)
            {     
			    if(ICF_FAILURE == \
                    icf_ssa_get_media_n_encoded_attrb_frm_media_line(\
                              p_ssa_pdb,p_tempAttr, p_int_sdp_info))
                 {
                        ret_val = ICF_FAILURE;
                 }
            }
            else
            {
                ret_val = ICF_FAILURE;
            }            
		}
      }
            if(ICF_NULL != p_tempAttr)
            {    
			    sip_freeSdpAttr(p_tempAttr);
                p_tempAttr = ICF_NULL;
            }    
        	if(ICF_FAILURE == ret_val)
        	{
           		 break;
        	}
	}/* for (iterator2) */

/* Precondition: Merged from ICF_QOS_PRE_M_LINE_MERGE */
#ifdef ICF_QOS_SUPPORT
ICF_CHECK_QOS_SUPPORT_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
    if(ICF_FAILURE == ret_val)
    {
	if ((0 == icf_port_strcasecmp(p_mvalue,"audio")) ||
		(0 == icf_port_strcasecmp(p_mvalue,"video")))
	{
        /* Initialize the remote received SDP structure with 
        * default Qos values
        */
		if(ICF_SUCCESS == icf_ssa_validate_qos_status_lines(p_ssa_pdb,
			&(pQosStatusStrings[0])))
		{
			/* Initialize the remote received SDP structure with 
			* default Qos values
			*/
			ret_val = icf_ssa_init_remote_sdp_with_default_qos_values
	            (p_int_sdp_info);
			/* Update remote SDP structure with the preconditions
	         * received in SDP message.
			 */
			ret_val = icf_ssa_prepare_sdp_from_qos_status_lines(
	            &pQosStatusStrings[0],
				p_int_sdp_info,
				p_ssa_pdb);    
		}
		else
		{
			ret_val = ICF_FAILURE;
		}
	}
    }
ICF_CHECK_QOS_SUPPORT_END
#endif
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_get_media_n_encoded_attrb_frm_media_line
 *
 * DESCRIPTION:     The function checks the attribute line.
 *		    If a-line contains mode value, then populate the 
 *		    corresponding mode in the internal stream sdp
 *		    else it is known in our present scenerio.So, populate
 *		    encoded parameters at media level.
 ******************************************************************/

icf_return_t icf_ssa_get_media_n_encoded_attrb_frm_media_line(
					INOUT	icf_ssa_pdb_st		    *p_ssa_pdb,
					INOUT 	SdpAttr			    *p_tempAttr,
					INOUT    icf_internal_sdp_stream_st  *p_int_sdp_info)
{
	icf_string_st	*p_str = ICF_NULL;
    icf_uint32_t    index = 0;
    icf_uint32_t    length = 0,attrib_val_len=0;
    icf_uint32_t    attrib_length = 0;
	icf_return_t	ret_val = ICF_SUCCESS;
    icf_boolean_t     mode_present = ICF_FALSE; 
    icf_media_mode_t  media_mode;

   ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

   if(0 == icf_port_strcasecmp("sendonly",p_tempAttr->pName))
	{
        media_mode = ICF_MEDIA_MODE_SEND_ONLY;
        mode_present = ICF_TRUE;
	}
	else if(0 == icf_port_strcasecmp("recvonly", p_tempAttr->pName))
	{
        media_mode = ICF_MEDIA_MODE_RECV_ONLY; 
        mode_present = ICF_TRUE;
	}
	else if(0 == icf_port_strcasecmp("inactive",p_tempAttr->pName))
	{
        media_mode = ICF_MEDIA_MODE_INACTIVE; 
        mode_present = ICF_TRUE;
	}
	else if(0 == icf_port_strcasecmp("sendrecv",p_tempAttr->pName))
	{
        media_mode = ICF_MEDIA_MODE_SEND_RECV; 
        mode_present = ICF_TRUE;
	}
	else if(0 == icf_port_strcasecmp("invalid",p_tempAttr->pName))
	{
        media_mode = ICF_MEDIA_MODE_INVALID; 
        mode_present = ICF_TRUE;
	}

    if(ICF_TRUE == mode_present)
    { 
	   /* Copy the rcvd media mode into media backup */
       p_ssa_pdb->p_glb_pdb->p_call_ctx->recvd_media_attrib_backup = media_mode; 

       if(!(ICF_SSA_MEDIA_MODE_RECEIVED & p_ssa_pdb->common_bitmask))
       {
            p_int_sdp_info->media_mode = media_mode;
            p_ssa_pdb->common_bitmask &= ~(ICF_SSA_MEDIA_MODE_RECEIVED); 
       }
    }
	else
	{
		ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
			ICF_PORT_SIZEOF(icf_string_st),
			ICF_MEM_COMMON,
			p_str,
			ICF_RET_ON_FAILURE,
			p_ssa_pdb->p_ecode,
			ret_val)
		
		icf_port_memset((icf_void_t *)p_str, 0, sizeof(icf_string_st));
        /* Fix for handling sip bomber cases in which the combine 
         * length of name and value field of encoded attribute is 
         * greater than ICF_MAX_STR_LEN ie 128 . In This case simply
         * copy the first 64 bytes of name field (if name field alone is
         * greater than 128 bytes ,otherwise copy whole name field ),
         *  attach two bytes of ": ".
         * And in the remaining part copy the value of encoded
         * attribute
         * This particular scenario is covered in the if block of follo-
         * -wing statement only.
         * Normal sucess case will remain in the else part
         */ 

        if(ICF_NULL != p_tempAttr->pValue)
        {
            attrib_val_len = icf_port_strlen((icf_uint8_t *)p_tempAttr->pValue);
        }
        attrib_length = icf_port_strlen((icf_uint8_t *)p_tempAttr->pName) + \
                            attrib_val_len +2;
        if ( attrib_length > ICF_MAX_STR_LEN)                  
        {
            if (ICF_MAX_STR_LEN <= (icf_port_strlen((icf_uint8_t *)p_tempAttr->pName) + 2))
            {
                icf_port_strncpy((icf_int8_t *)p_str->str,p_tempAttr->pName,
                      ICF_MAX_SHORT_STR_LEN);
                if(ICF_NULL != attrib_val_len)
                {
                    icf_port_strcat(p_str->str, (icf_uint8_t *)": ");
                    if (icf_port_strlen((icf_uint8_t *)p_tempAttr->pValue) >= ICF_MAX_STR_LEN -
                        icf_port_strlen(p_str->str) - 1)
                    {    
                        length = icf_port_strlen(p_str->str);
                        for(index = 0;index < (ICF_MAX_STR_LEN - length - 1)\
                                ;index++)
                        {
                           p_str->str[length + index] =\
                                       p_tempAttr->pValue[index];
                        }        
                        p_str->str[ICF_MAX_STR_LEN - 1]='\0';
                    }
                    else
                    {
                        icf_port_strcat(p_str->str,(icf_uint8_t *)p_tempAttr->pValue);
                    }
                }
            }
            else
            {
                icf_port_strcpy(p_str->str,(icf_uint8_t *)p_tempAttr->pName);   
                if(ICF_NULL != attrib_val_len)
                {
                    icf_port_strcat(p_str->str, (icf_uint8_t *)": ");
                    if (icf_port_strlen((icf_uint8_t *)p_tempAttr->pValue) >= ICF_MAX_STR_LEN -
                        icf_port_strlen(p_str->str) - 1)
                    {
                        length = icf_port_strlen(p_str->str);
                        for(index = 0;index < (ICF_MAX_STR_LEN - length -1)\
                               ;index++)
                        {
                            p_str->str[length + index] =\
                                        p_tempAttr->pValue[index];
                        }
                        p_str->str[ICF_MAX_STR_LEN - 1]='\0';
                    }
                    else
                    {
                        icf_port_strcat(p_str->str,(icf_uint8_t *)p_tempAttr->pValue);
                    }
                }
            }
        }
        else
        {             
		    icf_port_strcpy(p_str->str, (icf_uint8_t *)p_tempAttr->pName);
		    if (ICF_NULL != p_tempAttr->pValue)
		    {
			    icf_port_strcat(p_str->str, (icf_uint8_t *)": ");
			    icf_port_strcat(p_str->str, (icf_uint8_t *)p_tempAttr->pValue);
		    }
		}
        p_str->str_len = (icf_uint16_t )icf_port_strlen(p_str->str);
		icf_cmn_add_node_to_list(
			p_ssa_pdb->p_glb_pdb,
			&(p_int_sdp_info->sdp_stream.p_encoded_attributes),
			(icf_void_t *)p_str);
		
		p_int_sdp_info->sdp_stream.bit_mask |= ICF_SDP_STREAM_ENCPARAMS_PRESENT;		
	}

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	return ret_val;
}


/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_validate_n_alloc_inc_media_line
 * DESCRIPTION:     This function validates the incoming SDP for duplicate
 *                  and rejected streams.This function also validates the 
 *                  protocol for the received stream. IF any validation 
 *                  fails then 415 "Unsupported Media Type" bitmask will be
 *                  set in ssa context.
 *
 ***********************************************************************/

icf_return_t icf_ssa_validate_n_alloc_inc_media_line
(
 INOUT     icf_ssa_pdb_st                      *p_ssa_pdb,
 INOUT     SdpMedia                            *p_media,
 INOUT     Sdf_ty_s8bit                        *p_mvalue,
 INOUT     icf_uint8_t                         msg_identifier,
 INOUT     icf_uint8_t                         stream_id,
 OUT    icf_ssa_inc_media_stream_info_st    *p_inc_media_info,
 OUT    icf_error_t                         *p_error)
{
    icf_return_t                    ret_val = ICF_SUCCESS;
    icf_call_ctx_st                 *p_call_ctxt = ICF_NULL;
    icf_ssa_ctxt_st                 *p_ssa_ctxt = ICF_NULL;
    icf_uint16_t                    dPort = 0;
    SipError                        dError;
    icf_uint8_t                     index = 0;
    Sdf_ty_s8bit                      *p_protocol = Sdf_co_null;
    icf_boolean_t                   rejected_stream = ICF_FALSE;
    icf_inc_media_sdp_info_st       *p_media_sdp_info = ICF_NULL;
    icf_media_type_t               stream_type = ICF_STREAM_TYPE_INVALID;
    icf_call_type_t                 call_type = ICF_NULL;
    icf_boolean_t                   stream_active = ICF_TRUE;    
    icf_boolean_t                   form_sdp_line = ICF_TRUE;   
    icf_boolean_t                   valid_protocol = ICF_TRUE;
    icf_boolean_t                   add_t38 = ICF_FALSE;
    icf_boolean_t                   nw_reject_stream = ICF_FALSE;
    
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    
   /* To remove warning */
   p_error=p_error;

    p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
    p_call_ctxt = p_ssa_pdb->p_ssa_ctxt->p_call_ctxt;   
   
    /* Get the protocol from the media line and Validate.
     */
    sdp_getProtoFromMedia(p_media,&p_protocol,&dError);
    
    /* Get the stream_type from the media_value.
     */
    if (ICF_NULL == p_protocol)
        valid_protocol = ICF_FALSE;
    else if(0 == icf_port_strcasecmp(p_mvalue,"audio"))
    {
        call_type |= ICF_CALL_TYPE_AUDIO;
        stream_type = ICF_STREAM_TYPE_AUDIO;
        if(ICF_NULL != (icf_port_strcasecmp(p_protocol,"RTP/AVP")))
        {
            valid_protocol = ICF_FALSE;
        }

    }
    else if(0 == icf_port_strcasecmp(p_mvalue,"video"))
    {
        call_type |= ICF_CALL_TYPE_VIDEO;
        stream_type = ICF_STREAM_TYPE_VIDEO;
        if(ICF_NULL != (icf_port_strcasecmp(p_protocol,"RTP/AVP")))
        {
            valid_protocol = ICF_FALSE;
        }
    }
    
    else if(0 == icf_port_strcasecmp(p_mvalue,"application"))
    {        
        call_type |= ICF_CALL_TYPE_TBCP;
        stream_type = ICF_STREAM_TYPE_TBCP;
        if(ICF_NULL != (icf_port_strcasecmp(p_protocol,"udp")))
        {
            valid_protocol = ICF_FALSE;
        }
    }
    else if(0 == icf_port_strcasecmp(p_mvalue,"image"))
    {
        call_type |= ICF_CALL_TYPE_T38;
        stream_type = ICF_STREAM_TYPE_FAX;
        if(ICF_NULL != (icf_port_strcasecmp(p_protocol,"udptl")))
        {
            valid_protocol = ICF_FALSE;
        }
    }
     
    if(ICF_FALSE == valid_protocol)
    {
    /* Invalid protocol received. Mark the 4xx bitmask in ssa context.
     * Free any memory allocated
     */
        p_ssa_ctxt->failure_bitmask |= ICF_SSA_FAILURE_RESP_CODE_488;
        ICF_PRINT(((icf_uint8_t *)"\nInvalid Stream Protocol Received"));
       
        /* This warning code and header will be used for forming warning header.*/            
        p_ssa_pdb->warning_header_code = 302;
        icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"Invalid Stream Protocol Received");
        p_ssa_pdb->warning_text.str_len = (icf_uint16_t)icf_port_strlen(p_ssa_pdb->warning_text.str); 

        ret_val = ICF_FAILURE;
    }
    if(ICF_SUCCESS == ret_val)
    {
        if(SipFail == sdp_getPortFromMedia(p_media, &dPort, &dError))
        {
             ICF_PRINT(((icf_uint8_t *)"\n Unable to get port from media stream."));
            ret_val = ICF_FAILURE;
        }

        if(0 == dPort)
        {
            stream_active = ICF_FALSE;

            /* if port is coming as 0 and that call type exists,
             * then we will not mark this stream in rejected streams.
             */ 
            if(!(call_type & p_ssa_ctxt->call_type))
                  nw_reject_stream = ICF_TRUE;

            if (ICF_STREAM_TYPE_FAX == stream_type)
            {
                call_type &= ~ICF_CALL_TYPE_T38;
                p_ssa_pdb->common_bitmask |= ICF_SSA_INC_STOP_FAX_REINV;
            }
        }

        /* This  check has been added to fix CSR_1-5278440/SPR 17321.
         * If the peer adds a new stream(except a fax stream) during 
         * a call and the port for that m line is not 0,we have to simply 
         * reject that stream with port 0.We will check this only in 
         * the case of REINVITE/UPDATE(offer change).
         * The following bitmasks are set by SSA in the respective
         * callbacks.
         */
        if((ICF_SSA_CTXT_REINVITE_RECVD == 
           (p_ssa_ctxt->bitmask & ICF_SSA_CTXT_REINVITE_RECVD)) ||
           (ICF_SSA_CTXT_UPDATE_RECVD == 
           (p_ssa_ctxt->bitmask & ICF_SSA_CTXT_UPDATE_RECVD)))
        {
            if((0 != dPort) && !(p_call_ctxt->call_type & call_type) &&
                    (ICF_STREAM_TYPE_FAX != stream_type))
            {
                stream_active = ICF_FALSE;
                nw_reject_stream = ICF_TRUE;
            }
        }
        
        
        if(ICF_SSA_INCOMING_INVITE == msg_identifier)
        {
            if((call_type & p_ssa_pdb->line_recvd)&&(0 != dPort))

            {
                ICF_PRINT(((icf_uint8_t *)"\nDuplicate stream received in Invite"));
                rejected_stream = ICF_TRUE;
                stream_active = ICF_FALSE;                    
            }
            
            /* Validate for call type T38
             */
#ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
           if(ICF_STREAM_TYPE_FAX == stream_type)
           {
               /* T38 stream in incoming invite is to be rejected.
                */            
                
                ICF_PRINT(((icf_uint8_t *)"\nT.38 Stream not allowed in incoming Invite"));
                rejected_stream = ICF_TRUE; 
                stream_active = ICF_FALSE;
		form_sdp_line = ICF_FALSE;
/* If Audio line is also there than donot send 415
                 * instead mark this stream as rejected.
                 * else if only fax line is received then send 415
                 */
                if(ICF_CALL_TYPE_AUDIO == (ICF_CALL_TYPE_AUDIO & p_ssa_pdb->line_recvd))
                {
                    /* Audio m line received.
                     */
                    p_ssa_ctxt->bitmask |= ICF_SSA_CTXT_T38_RCVD_IN_NEW_CALL; 

                    ret_val = ICF_SUCCESS;
                }
				else
                {
              		ret_val = ICF_FAILURE;
			    }
            }/* End for call type T38*/
ICF_CHECK_IMS_END
#endif
#endif
            if(ICF_TRUE == stream_active && ICF_FALSE == rejected_stream)
            {
                p_ssa_pdb->line_recvd |= call_type;
                
                p_ssa_ctxt->call_type |= call_type;
            }

            if(ICF_NULL != p_call_ctxt)
			{
            p_call_ctxt->call_type = p_ssa_ctxt->call_type;
			}
            
		}/* End of if for Incoming Invite Validation.*/
        
        else
        {
			if(ICF_NULL == p_ssa_ctxt->call_type)
			{
				/*messages destined for SIC*/
			}
            
			else
			{
             /* This is call_type in ssa ctxt is being taken from 
              *  p_call_ctx->call_type.this was causing problem in the 
              *  scenario,if stop_fax_req received from remote and then
              *  again remote send start_fax_req.*/
               
              if(ICF_STREAM_TYPE_FAX == stream_type)
              p_ssa_ctxt->call_type = p_call_ctxt->call_type;


             /* If call type not set and not T38 then mark the stream as 
              * rejected. If T38 stream then add this stream to the add_sdp list.
              */
            if(call_type & p_ssa_ctxt->call_type)
            {
                if(call_type & p_ssa_pdb->line_recvd)
                {                    
                    if(0 != dPort)
                    {
                        /*Duplicate Stream, Reject this stream.*/
                        ICF_PRINT(((icf_uint8_t *)"\nDuplicate stream received."));
                        rejected_stream = ICF_TRUE;                        
                        p_inc_media_info->rejected_streams[p_inc_media_info->count].stream_id\
                            = (icf_uint8_t)(stream_id + 1);
                        p_inc_media_info->rejected_streams[p_inc_media_info->count++].stream_type\
                            = stream_type;                        
                        form_sdp_line = ICF_FALSE;
                    }
                    else
                    {
                        form_sdp_line = ICF_TRUE;
                    }
                }
                else
                {
                   /* Set stream type in ssa pdb. Not a duplicate stream.
                    */
                    ICF_PRINT(((icf_uint8_t *)"\nValid Stream Received"));
                    p_ssa_pdb->line_recvd |= call_type;
                }
            }
            else if(!(call_type & p_ssa_ctxt->call_type) && \
                (ICF_STREAM_TYPE_FAX != stream_type))
            {
                ICF_PRINT(((icf_uint8_t *)"\nStream received other than connected streams: Rejected")); 
               /* Mark the stream as rejected.
                */
                if((0 != dPort) && (ICF_FALSE == nw_reject_stream))  
                {
                    p_inc_media_info->rejected_streams[p_inc_media_info->count].stream_id 
                        = (icf_uint8_t)(stream_id + 1);
                    p_inc_media_info->rejected_streams[p_inc_media_info->count++].\
                        stream_type = stream_type;
                    form_sdp_line = ICF_FALSE;
                }
                else
                {
                    form_sdp_line = ICF_TRUE;
                }
            }
            
            else if(ICF_STREAM_TYPE_FAX == stream_type)
            {
#ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
                    /* Add this new stream to the sdp list*/
                
                if(ICF_SSA_RECVD_ANSWER_TO_OFFER == msg_identifier)
                {
                    ICF_PRINT(((icf_uint8_t *)"\nNew T.38 stream received in Answer Non-IMS:\
                        Rejected"));
                    rejected_stream = ICF_TRUE;
                    p_inc_media_info->rejected_streams\
                        [p_inc_media_info->count].stream_id = (icf_uint8_t)(stream_id + 1);
               
                    p_inc_media_info->rejected_streams\
                        [p_inc_media_info->count++].stream_type = stream_type;
                    form_sdp_line = ICF_FALSE;

                }
                else if(ICF_NULL != p_inc_media_info->p_added_sdp_list)
                {
                    ICF_PRINT(((icf_uint8_t *)"\nMultiple T.38 stream received in offer Non-IMS:\
                        Rejected"));
                    rejected_stream = ICF_TRUE;
                    p_inc_media_info->rejected_streams\
                        [p_inc_media_info->count].stream_id = (icf_uint8_t)(stream_id + 1);
               
                    p_inc_media_info->rejected_streams\
                        [p_inc_media_info->count++].stream_type = stream_type;
                    form_sdp_line = ICF_FALSE;
                }
                else if (ICF_TRUE == stream_active)
                {
                   if(0 != dPort)
                   {
                    ICF_PRINT(((icf_uint8_t *)"\nNew T.38 stream received in Offer Non-IMS"));
                    add_t38 = ICF_TRUE;
                    p_ssa_pdb->line_recvd |= call_type;
                  }
                }
                
ICF_CHECK_IMS_END
#endif
                    
#ifdef IMS_CLIENT	
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_TRUE)
                
               ICF_PRINT(("\nT.38 stream received in IMS mode: Rejected"));    
               rejected_stream = ICF_TRUE;
               p_inc_media_info->rejected_streams[p_inc_media_info->count].\
                   stream_id = stream_id + 1;
               
               p_inc_media_info->rejected_streams[p_inc_media_info->count++].\
                    stream_type = stream_type;
               form_sdp_line = ICF_FALSE;
                
ICF_CHECK_IMS_END
#endif
                    
#endif
            }
			}
        }/* End of validation for messages other than INVITE (Initial Offer)*/
        
        
        if(ICF_TRUE == rejected_stream)
        {
            for(index = 0;index < ICF_MAX_STREAM_PER_CALL &&\
                p_ssa_ctxt->rej_stream[index].stream_id == 0;index++)
            {
                break;
            }

            if(ICF_MAX_STREAM_PER_CALL > index)
            {
                p_ssa_ctxt->rej_stream[index].stream_id = (icf_uint8_t)(stream_id + 1);
                       
                icf_port_strcpy(p_ssa_ctxt->rej_stream[index].\
                    protocol_name.str,(icf_uint8_t *)p_protocol);
                p_ssa_ctxt->rej_stream[index].protocol_name.str_len = \
                    (icf_uint16_t )icf_port_strlen((icf_uint8_t *)p_protocol);
            }            
        }
        
        /* Media stream is valid/rejected. Form the media node and mark it as 
         * active/inactive.depending upon the value of stream_active flag.
         */
        if(ICF_NULL != p_inc_media_info && ICF_TRUE == form_sdp_line)
        {
	    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,\
                ICF_PORT_SIZEOF(icf_inc_media_sdp_info_st),ICF_MEM_COMMON,\
                p_media_sdp_info,\
                ICF_RET_ON_FAILURE,p_ssa_pdb->p_ecode, ret_val)
           
            icf_port_memset((icf_void_t *)p_media_sdp_info,
                            0,
                            sizeof(icf_inc_media_sdp_info_st));
    
            p_media_sdp_info->stream_active = stream_active;
            p_media_sdp_info->stream_id = (icf_uint8_t)(stream_id + 1);
            p_media_sdp_info->stream_type = stream_type;
            p_media_sdp_info->p_int_sdp_stream = ICF_NULL;
            
            /* Insert the node in the sdp list. If T38 and msg_identifier is not
            * INCOMING INVITE then insert in the add_sdp_list. Else insert in the 
            * sdp info list
            */
            
            if(ICF_TRUE == add_t38)
            {

#ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
               if(ICF_CALL_TYPE_T38  == 
			      (ICF_CALL_TYPE_T38 & p_ssa_pdb->p_glb_pdb->p_call_ctx->call_type))
               {
                  ret_val = icf_cmn_add_node_to_list(p_ssa_pdb->p_glb_pdb,\
                            &((p_inc_media_info)->p_sdp_info_list),
                            p_media_sdp_info);  
               }
               else
               {
	               ret_val = icf_cmn_add_node_to_list(p_ssa_pdb->p_glb_pdb,\
			                 &((p_inc_media_info)->p_added_sdp_list),\
			                 p_media_sdp_info);
               }
ICF_CHECK_IMS_END
#endif
#endif
            }
            else if(ICF_TRUE == nw_reject_stream)
            {
                ret_val = icf_cmn_add_node_to_list(p_ssa_pdb->p_glb_pdb,\
                              &((p_inc_media_info)->p_nw_reject_stream_list),\
                                 p_media_sdp_info);
            }
            else
            {		
                ret_val = icf_cmn_add_node_to_list(p_ssa_pdb->p_glb_pdb,\
                              &((p_inc_media_info)->p_sdp_info_list),
                                 p_media_sdp_info);
            }

            if(ICF_FAILURE == ret_val)
            {
               /* memory allocated to p_media_sdp_info will be freed in the 
                * calling function.
                */
                ICF_PRINT(((icf_uint8_t *)"\nFailure in Validating the media stream"));    
            }

            
        } 
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}



/***************************************************************************
**
** FUNCTION: icf_ssa_fetch_curr_node_from_list
** DESCRIPTION: This function fetch the current node from the list of 
**              sdp streams based on the stream id.
**
***************************************************************************/
icf_return_t icf_ssa_fetch_curr_node_from_list
(
 INOUT     icf_ssa_pdb_st                      *p_ssa_pdb,
 INOUT     icf_ssa_inc_media_stream_info_st    *p_inc_media_info,
 INOUT     icf_stream_id_t                     stream_id,
 OUT    icf_inc_media_sdp_info_st           **p_p_media,
 OUT    Sdf_st_error                        *p_error)
{
    icf_return_t                ret_val = ICF_SUCCESS;
    icf_list_st                 *p_temp_sdp_list = ICF_NULL;
    icf_inc_media_sdp_info_st   *p_temp_media_node = ICF_NULL;
	icf_boolean_t				 list_found = ICF_FALSE;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
   /* To remove warning */
   p_error=p_error;

#ifdef ICF_FAX_SUPPORT
#ifdef NON_IMS_CLIENT
ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
    if(ICF_CALL_TYPE_T38 & p_ssa_pdb->line_recvd && \
		(!(ICF_CALL_TYPE_T38 & p_ssa_pdb->p_ssa_ctxt->call_type)))
    {
        /* Traverse the add sdp list for stream id
         * and get the current node
         */
		list_found = ICF_TRUE;
        p_temp_sdp_list = p_inc_media_info->p_added_sdp_list;
    }
ICF_CHECK_IMS_END
#endif
#endif
        /* Traverse the sdp info list for stream id
         * and get the current node
         */
	if(ICF_FALSE == list_found)
    {
		p_temp_sdp_list = p_inc_media_info->p_sdp_info_list;
	}
	stream_id++;

    while(ICF_NULL != p_temp_sdp_list)
    {
        p_temp_media_node = (icf_inc_media_sdp_info_st *) p_temp_sdp_list->\
            p_data;
        if(stream_id == p_temp_media_node->stream_id)
        {
            /* Current node is found.
             */
            *p_p_media = p_temp_media_node;
            break;
        }
        else
            p_temp_sdp_list = p_temp_sdp_list->p_next;

    }

    if(ICF_NULL == p_temp_sdp_list)
    {
        p_temp_sdp_list = p_inc_media_info->p_nw_reject_stream_list;

        while(ICF_NULL != p_temp_sdp_list)
        {
            p_temp_media_node = (icf_inc_media_sdp_info_st *) p_temp_sdp_list->\
                p_data;
            if(stream_id == p_temp_media_node->stream_id)
            {
                /* Current node is found */
                *p_p_media = p_temp_media_node;
                break;
            }
            else
            {
                p_temp_sdp_list = p_temp_sdp_list->p_next;
            }
        }
    }
    if(ICF_NULL == p_temp_sdp_list)
    {
        ret_val = ICF_FAILURE;
    }

        
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/***************************************************************************
**
** FUNCTION: icf_ssa_get_media_parameters_from_sdp
** DESCRIPTION: This function gets the media parameters from the SipMessage.
**              It converts the SDP to the SDP structure of the call
**              context.The calling function needs to pass the entire SIP
**              msg to the function.The function extracts the SDP from the
**              SIP message and then then fills the appropiate fields of 
**              the stream-level SDP structure(icf_inc_media_sdp_info_st) 
**
***************************************************************************/

icf_return_t icf_ssa_get_media_parameters_from_sdp(
        INOUT    icf_ssa_pdb_st						*p_ssa_pdb,
        INOUT    SipMessage							*p_sipMessage,
        INOUT    icf_uint8_t							msg_identifier,
        OUT   icf_ssa_inc_media_stream_info_st		**p_p_inc_media_sdp_info, 
        /* list of type icf_inc_media_sdp_info_st */
        OUT   icf_error_t							*p_error)
{
    SipError                            dError;
    en_SipMsgBodyType                   dType;
    Sdf_ty_s8bit                        *p_format = Sdf_co_null;                          
    SipMsgBody                          *p_msgbody = Sdf_co_null;
    SdpMessage                          *p_sdpMessage = Sdf_co_null;
    SdpMedia                            *p_media = Sdf_co_null;
    Sdf_ty_u32bit                       dCount = 0, m_line_count =0;
    Sdf_ty_s8bit                        *p_mvalue = Sdf_co_null;
    icf_uint16_t                        iterator1 = 0, msg_body_count = 0;
    Sdf_ty_s8bit                        *p_buffer = Sdf_co_null;
    Sdf_ty_u32bit                       bufLength = 0;
    Sdf_st_error                        Err;    
    icf_call_ctx_st                     *p_call_ctxt = ICF_NULL;
    icf_msg_body_st                     *p_msg_body = ICF_NULL;    
    icf_inc_media_sdp_info_st           *p_media_sdp_info = ICF_NULL;
    en_SipMessageType                   dReqRespType;
    
    icf_msg_body_list_st				*p_body_list = ICF_NULL;
    icf_boolean_t						is_sdp_body_found = ICF_FALSE;
    
/*icf_ssa_inc_media_stream_info_st    *p_inc_media_info = ICF_NULL;*/
    icf_return_t                        ret_val = ICF_SUCCESS;
    icf_boolean_t                      to_check_active_stream = ICF_TRUE;
    
    /* To remove compiler warning */
    (void)p_error;

	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    /* UMR fix */
	icf_port_memset(
		  (icf_void_t *)&Err,
		  0,
		  ICF_PORT_SIZEOF(Sdf_st_error));

    p_call_ctxt = p_ssa_pdb->p_glb_pdb->p_call_ctx;
    /* Reset the count for each header list/msg-body list/ stream list */
    if (ICF_NULL == p_call_ctxt || p_call_ctxt->common_bitmask & ICF_IN_DLG_OPTIONS_SEND)
	{
		if(ICF_NULL != p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt)
		{

			p_body_list = &(p_ssa_pdb->p_ssa_ctxt->p_sic_ctxt->body_list);
			p_body_list->count = 0;
            to_check_active_stream = ICF_FALSE;
		}
	}
	else
	{
		p_body_list = &(p_call_ctxt->body_list);

		/* Reset the count for each header list/msg-body list/ stream list */

   	    /*Fix for CSR 1-5132873 We should not reset the header list count
        This should be done while filling app_specific_hdrs after freeing the hdrlist*/
        /*p_call_ctxt->header_list.count = 0;
        p_call_ctxt->tag_list.count = 0;*/

        /*free the list of additional message bodies */
        ret_val = icf_cmn_free_msg_body_list(p_ssa_pdb->p_glb_pdb, 
				&(p_call_ctxt->body_list),
				p_error);
        p_call_ctxt->body_list.count = 0;
    }
       
    /* 
     * Extract the count of message bodies in the SIP message 
     */ 
    if (SipFail == sip_getMsgBodyCount(p_sipMessage, &dCount, &dError))
    {
        /*ICF_SSA_TRACE(("\nFailed to get MsgBodyCount from SipMessage"))*/
        return ICF_FAILURE;
    }
    /* Iterate through all the message bodies */ 
    for (iterator1 = 0; iterator1 < (icf_uint16_t)dCount; iterator1++)
    {
        if (SipFail == sip_getMsgBodyTypeAtIndex(p_sipMessage,\
            &dType, iterator1, &dError))
        {
            /*ICF_SSA_TRACE(("\nFailed to get MsgBody type from SipMessage"))*/
                ret_val = ICF_FAILURE;
        }
        else if (dType != SipSdpBody)
        {
          if (ICF_NULL != p_call_ctxt)
          {
            /* handle remaining unknown message bodies only*/
            if(sdf_ivk_uaGetUnknownBodyFromSipMessage(p_sipMessage, iterator1, \
                &p_buffer, &bufLength, &Err) == Sdf_co_success)
            {                              
                if (p_call_ctxt->body_list.count == 0)
                {
                    /* allocate memory for a msg body list node and add to List*/
                    ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
                        ICF_PORT_SIZEOF(icf_msg_body_st),
                        ICF_MEM_COMMON, 
                        p_msg_body,
                        ICF_RET_ON_FAILURE, p_ssa_pdb->p_ecode, ret_val)
                        
                        p_call_ctxt->body_list.body = p_msg_body;   
                	p_msg_body->next = ICF_NULL;
                }
                else if(ICF_NULL != p_msg_body)
                {
                    /* allocate memory for a msg body list node and add to List*/
                    ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
                        ICF_PORT_SIZEOF(icf_msg_body_st),
                        ICF_MEM_COMMON, 
                        p_msg_body->next,
                        ICF_RET_ON_FAILURE, p_ssa_pdb->p_ecode, ret_val)
                        
                        p_msg_body = p_msg_body->next;                     
                	p_msg_body->next = ICF_NULL;
                }
                /* Increment the msg body count by one */
                p_call_ctxt->body_list.count++;
                
                /* Message Body Header Changes Start
				 */
				sip_listGetAt(&(p_sipMessage->slMessageBody),\
			      iterator1,((SIP_Pvoid *) &p_msgbody),&dError);
               	sip_getMessageType(p_sipMessage, 
	            	&dReqRespType, &dError);
	           
				if((ICF_NULL != p_msgbody) &&
                        (ICF_NULL != p_msg_body))
				{ 
                    /* Klocwork warning removal.*/
                    if (ICF_NULL == p_msg_body)
                    {
                        ret_val = ICF_FAILURE;
                        break;
                    }
					icf_ssa_get_mime_hdrs_from_sip_msg(dReqRespType,\
						p_ssa_pdb,p_msgbody,p_msg_body);
				
					p_msg_body->content_type.str_len = 
						(icf_uint16_t )icf_port_strlen(p_msg_body->content_type.str);
					/* Message Body Header Changes Ends
				 	*/			
			                
                	p_msg_body->length= bufLength;
                	/* Allocate memory for message body value and copy the recvd msg body*/
                	if (ICF_NULL != p_msg_body->length)
                	{
                    ICF_MEMGET(p_ssa_pdb->p_glb_pdb,
                        bufLength,
                        ICF_MEM_COMMON,
                        p_msg_body->value,
                        ICF_RET_ON_FAILURE,
                        p_ssa_pdb->p_ecode, ret_val)
                        
                        icf_port_memcpy(p_msg_body->value,(char*)p_buffer,bufLength);
                    
                	}
				}                 
            }
          }
          else
          {
            ret_val = ICF_FAILURE; 
          }             
    
        } /*  if (dType != SipSdpBody) */
	else
	{
	   /*So,if SDP body is already found,then we are not supporting it.*/
	   if(ICF_TRUE == is_sdp_body_found)
	   {
	       p_ssa_pdb->p_ssa_ctxt->failure_bitmask |=
			      ICF_SSA_FAILURE_RESP_CODE_415;                               
	       sip_freeSipMsgBody(p_msgbody);				
               ret_val = ICF_FAILURE;
            }
	    else
	    {	
	       is_sdp_body_found = ICF_TRUE;
               msg_body_count = iterator1;	
	    }
     }
     if(ICF_FAILURE == ret_val)
     {
         break;
     }
    }	
    if(ICF_SUCCESS == ret_val) 
    {          
            if (SipFail == sip_getMsgBodyAtIndex(p_sipMessage,\
                &p_msgbody, msg_body_count, &dError))
            {
                /*ICF_SSA_TRACE(("\nFailed to get the Message body at an index"*/
                /*"from the SipMessage"))*/
                ret_val = ICF_FAILURE;
            }
            else if (SipFail == sip_getSdpFromMsgBody(p_msgbody,
                &p_sdpMessage, &dError))
            {
                /*ICF_SSA_TRACE(("\nFailed to get the SdpMessage index from"      */
                /*"the SipMessageBody"))*/
                /* Free all the acquired memory */
                sip_freeSipMsgBody(p_msgbody);
                ret_val = ICF_FAILURE;
            }
            /* Get the number of m-lines */
            else if (ICF_FAILURE == icf_ssa_check_no_of_m_lines(p_sdpMessage,
                &m_line_count))
            {
                /* Free all the acquired memory */
                sip_freeSipMsgBody(p_msgbody);
                sip_freeSdpMessage(p_sdpMessage);
                ret_val = ICF_FAILURE;
            }
            else
            {

                icf_uint8_t             ctr = ICF_NULL;
                /* If p_p_inc_media_info is NULL and message identifier is Not Invite
                 * then allocate memory to this structure.
                 */
                ICF_MEMGET(p_ssa_pdb->p_glb_pdb,\
                    ICF_PORT_SIZEOF(icf_ssa_inc_media_stream_info_st),ICF_MEM_COMMON,\
                    *p_p_inc_media_sdp_info,ICF_RET_ON_FAILURE,p_ssa_pdb->\
                    p_ecode, ret_val)
                    
                    icf_port_memset((icf_void_t *)*p_p_inc_media_sdp_info,\
                    0, ICF_PORT_SIZEOF(icf_ssa_inc_media_stream_info_st));          
                
                /* Now we need to iterate over m_line_count and form a node for
                 * each m-line in the stream-level media info list 
                 * p_inc_media_sdp_info_list. 
                 */           

		if(ICF_NULL == m_line_count)
		{
                    ret_val = ICF_FAILURE;
                    sip_freeSipMsgBody(p_msgbody);
                    sip_freeSdpMessage(p_sdpMessage);
		}

                for (; ctr < m_line_count; ctr++)
                {
                    if (SipFail == sdp_getMediaAtIndex(
                                        p_sdpMessage,
                                        &p_media,
                                        ctr,
                                        &dError))
                    {
                        /*ICF_SSA_TRACE(("Media not present"))*/
                        sip_freeSipMsgBody(p_msgbody);
                        sip_freeSdpMessage(p_sdpMessage);
						
						/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
						icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
													p_p_inc_media_sdp_info);
						
						ret_val = ICF_FAILURE;
                        break;
                    }
                    else if (SipFail == sdp_getMvalueFromMedia(
                                        p_media,
                                        &p_mvalue,
                                        &dError))
                    {
                        /*ICF_SSA_TRACE(("\nUnable to get the MValue from media"))*/
                        /* Free all the acquired memory */
                        sip_freeSdpMedia(p_media);
                        sip_freeSipMsgBody(p_msgbody);
                        sip_freeSdpMessage(p_sdpMessage);
						
						/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
						icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
													p_p_inc_media_sdp_info);
						
                        ret_val = ICF_FAILURE;
                        break;
                    }
				    else if (SipFail == sdp_getFormatFromMedia(
                        p_media,&p_format,&dError))
                    {
                       /* ICF_SSA_TRACE(("\nNo Media Format"))*/
                            /* Free all the acquired memory */
                        sip_freeSdpMedia(p_media);
                        sip_freeSipMsgBody(p_msgbody);
                        sip_freeSdpMessage(p_sdpMessage);
                   
						/*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                        icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                                    p_p_inc_media_sdp_info);
				        ret_val =  ICF_FAILURE;
                    }
                    else
                    {
                     
                       /* If p_p_inc_media_info is NULL and message identifier is Not Invite
                        * then allocate memory to this structure.
                        */
                        if(ICF_FAILURE == \
                            icf_ssa_validate_n_alloc_inc_media_line(\
												p_ssa_pdb,
												p_media,
												p_mvalue,
												msg_identifier,
												ctr, *p_p_inc_media_sdp_info,
												p_error))
                        {
                        	/* SDP Stream Validation Failed.
                             */
                            ICF_PRINT(((icf_uint8_t *)"\nFailure in SDP Stream Validation"));

							/* Free all the acquired memory */
							sip_freeSdpMedia(p_media);
							sip_freeSipMsgBody(p_msgbody);
							sip_freeSdpMessage(p_sdpMessage);
						
							/*	Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
							icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
													p_p_inc_media_sdp_info);
						
                            ret_val = ICF_FAILURE;
                            break;
                        }
                        else
                        {
                           if((ICF_NULL != p_call_ctxt) && 
										(ICF_NULL == p_call_ctxt->call_type))
                           {
                               /* Call type not set. Send 4xx
                                */
                               p_ssa_pdb->p_ssa_ctxt->failure_bitmask |=
                                  ICF_SSA_FAILURE_RESP_CODE_415;
			       /* Fix for CSR 1-6792719 referred from IPTK Rel 7.1.2 */ 	
                               /* Make the function call to free sdp media,sip
                                * msg body and sdp message along with memory
                                * allocated for p_p_inc_media_sdp_info.Since
                                * when call type is not set we set the value
                                * of ret_val to ICF_FAILURE .In the following
                                * code we freed the memory for the same in
                                * ret_val=ICF_SUCCESS ,however in case of
                                * failure it is to be freed in the same block
                                * which is setting the ret_val to ICF_FAILURE.*/
                               /* Free all the acquired memory */
                               sip_freeSdpMedia(p_media);
                               sip_freeSipMsgBody(p_msgbody);
                               sip_freeSdpMessage(p_sdpMessage);

                               /*Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                               icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                                    p_p_inc_media_sdp_info);     
                               ret_val = ICF_FAILURE;
                               break;
                           }
                            /* Fetch the current node from the list
                            */
                           else if(ICF_FALSE == icf_ssa_fetch_curr_node_from_list(\
												p_ssa_pdb,
												*p_p_inc_media_sdp_info,
												ctr,&p_media_sdp_info,
												&Err))
                            {
				sip_freeSdpMedia(p_media);
				continue;    
                            }
                           else
	                       {
	   				/* Check wheteher the stream is marked as 
								* active or not. IF active then only fetch
								* the attribute line.
								*/
                               if(ICF_TRUE == to_check_active_stream)
                               {
					if(ICF_TRUE == p_media_sdp_info->stream_active)
					{
						if(ICF_FAILURE == icf_ssa_populate_media_sdp_info(\
												p_ssa_pdb,
                                                p_sipMessage,
												p_sdpMessage,
												p_media, p_mvalue,
												p_format,
												p_media_sdp_info))
									{
										/* Free all the acquired memory */
										sip_freeSdpMedia(p_media);
										sip_freeSipMsgBody(p_msgbody);
										sip_freeSdpMessage(p_sdpMessage);

										/*	Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
										icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
												p_p_inc_media_sdp_info);

										ret_val = ICF_FAILURE;
										break;
									}
								}
			       }
                               else
                               {
                                   if(ICF_FAILURE == icf_ssa_populate_media_sdp_info(\
                                       p_ssa_pdb,
                                       p_sipMessage,
                                       p_sdpMessage,
                                       p_media, p_mvalue,
                                       p_format,
                                       p_media_sdp_info))
                                   {
                                       /* Free all the acquired memory */
                                       sip_freeSdpMedia(p_media);
                                       sip_freeSipMsgBody(p_msgbody);
                                       sip_freeSdpMessage(p_sdpMessage);
                                       
                                       /*	Free the memory allocated to icf_ssa_inc_media_stream_info_st*/
                                       icf_ssa_free_inc_media_stream_info(p_ssa_pdb,
                                           p_p_inc_media_sdp_info);
                                       
                                       ret_val = ICF_FAILURE;
                                       break;
                                   }
                               }
                           }
			}
		    }
			if(Sdf_co_null != p_media)
			{
		    	sip_freeSdpMedia(p_media);
				p_media = Sdf_co_null;	
			}
		}/* End of for loop for muliptle m-lines.*/
	    }/* End of else for m line processing*/

	    if(ICF_FAILURE == ret_val)
	    {

		    /*break from the outer for loop for all message bodies*/
		    /* Free all local references */
		    if((!(ICF_SSA_FAILURE_RESP_CODE_415 & p_ssa_pdb->p_ssa_ctxt->\
					    failure_bitmask)) && (ICF_SSA_FAILURE_RESP_CODE_488 !=\
                        (ICF_SSA_FAILURE_RESP_CODE_488 & p_ssa_pdb->p_ssa_ctxt->failure_bitmask )))
		    {
			    p_ssa_pdb->p_ssa_ctxt->failure_bitmask |= \
			      ICF_SSA_FAILURE_RESP_CODE_415;
		    }
	    }
	    if (ICF_SUCCESS == ret_val)
	    {
		    /* Free all local references */
		    sip_freeSipMsgBody(p_msgbody);
		    sip_freeSdpMessage(p_sdpMessage);
		    /* sip_freeSdpMedia(p_media);*/
		    /* p_msgbody = Sdf_co_null;*/
		    /* p_sdpMessage = Sdf_co_null;*/
		    /* p_media = Sdf_co_null;*/

	    } /* if (ICF_SUCCESS == ret_val... */
	} /* SDP body handling */

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
	    return ret_val;
} /* End function icf_ssa_get_media_parameters_from_sdp() */


/*****************************************************************************
 ** FUNCTION:           icf_ssa_create_stream_sdp_info_in_cctx
 **
 ** DESCRIPTION:        When an incoming SIP message with SDP is received by 
 **                     SSA, it gets the media parameters from SDP and creates
 **                     a list of type icf_inc_media_sdp_info_st. Once this is
 **                     done, this function updates the remote_received_sdp in
 **                     call_ctx->p_stream_sdp_info for each received stream.
 **
 **                     This function also allocates memory to the list
 **                     p_stream_sdp_info before setting remote SDP in it.
 **
 **                     This function should be used in scenarios when the list 
 **                     p_stream_sdp_info in call_ctx is null.
 **                     (eg. on receiving initial offer from peer)
 **
 *****************************************************************************/
icf_return_t icf_ssa_create_stream_sdp_info_in_cctx(
		INOUT    icf_ssa_pdb_st	            *p_ssa_pdb,
		INOUT      icf_list_st		            *p_media_sdp_info, /* list of type icf_inc_media_sdp_info_st */
		INOUT    icf_call_ctx_st             *p_call_ctx)
{
	icf_return_t                        ret_val = ICF_SUCCESS;
	icf_list_st                         *p_media_list_node = ICF_NULL;
	icf_inc_media_sdp_info_st           *p_media_info = ICF_NULL;
	icf_stream_sdp_info_st              *p_cctx_stream_info = ICF_NULL;
	icf_uint32_t			stream_id;
	icf_error_t				err = ICF_NULL;	
	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

		/* Iterate over the nodes of p_media_sdp_info list, data of each node is 
		 * of type icf_inc_media_sdp_info_st and corresponds to one stream */
    p_media_list_node = p_media_sdp_info;

    while(p_media_list_node)
    {
        /* Get the stream-level data from the p_media_sdp_info list */
        p_media_info = (icf_inc_media_sdp_info_st *)(p_media_list_node->p_data);

        /* Allocate memory to the data in the node */
        ICF_MEMGET(
            p_ssa_pdb->p_glb_pdb,
            ICF_PORT_SIZEOF(icf_stream_sdp_info_st),
            ICF_MEM_COMMON,
            p_cctx_stream_info,
            ICF_RET_ON_FAILURE,
            p_ssa_pdb->p_ecode,
            ret_val)
        p_cctx_stream_info->stream_id = ICF_STREAM_ID_INVALID;
        p_cctx_stream_info->stream_type = ICF_STREAM_TYPE_INVALID;
        p_cctx_stream_info->p_local_sdp = ICF_NULL;
        p_cctx_stream_info->p_applied_sdp = ICF_NULL;
        p_cctx_stream_info->p_remote_received_sdp = ICF_NULL;
        p_cctx_stream_info->p_last_commited_sdp = ICF_NULL;
        p_cctx_stream_info->mm_applied_mode = ICF_MEDIA_MODE_INVALID;
        p_cctx_stream_info->media_modify_type = ICF_NULL;

        /* Set the remote recd SDP in the data */
        p_cctx_stream_info->stream_id = p_media_info->stream_id;
        
	stream_id = p_media_info->stream_id;
        
	p_cctx_stream_info->stream_type = p_media_info->stream_type;
	p_cctx_stream_info->p_remote_received_sdp = p_media_info->p_int_sdp_stream;
	
	/*This is to free the allocated memory*/
	p_media_info->p_int_sdp_stream = ICF_NULL;

	/*Since, its an offer, the stream always flows over the nw*/
	p_cctx_stream_info->stream_status |= ICF_MEDIA_STREAM_NW_ALLOW;

	/*The stream would be active only if its a valid stream.*/
	if(ICF_TRUE == p_media_info->stream_active)
	{
		p_cctx_stream_info->stream_status |= ICF_MEDIA_STREAM_ACTIVE;
	}

        /* Set the newly allocated data in stream-list node */
        /* Add the newly allocated node to the list in call_ctx */
        if (ICF_FAILURE == icf_cmn_add_node_to_internal_list(
                            p_ssa_pdb->p_glb_pdb,
                            &(p_call_ctx->p_stream_sdp_info),
                            (icf_void_t *)p_cctx_stream_info,
			    (icf_void_t *)(&stream_id),
                            (icf_void_t *)ICF_NULL))
        {
            ret_val = ICF_FAILURE;
            break;
        }
        p_cctx_stream_info = ICF_NULL;
        p_media_list_node = p_media_list_node->p_next;
    } /* End for(p_media_list_node) */

    if(p_media_sdp_info)
    {
	icf_cmn_free_media_sdp_info_list(p_ssa_pdb->p_glb_pdb,
             				&p_media_sdp_info,
        				&err);
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_ssa_create_stream_sdp_info_in_cctx() */


/*****************************************************************************
 ** FUNCTION:           icf_ssa_get_amr_attrib_from_media_stream
 **
 ** DESCRIPTION:        This function will retrieve the AMR attributes 
 **                     received in the audio stream.This will onl extract the
 **                     octet-align,crc and mode-set attribute parameters.
 **
 *****************************************************************************/

icf_return_t icf_ssa_get_amr_attrib_from_media_stream
(
 INOUT     icf_ssa_pdb_st         *p_ssa_pdb,
 INOUT  icf_codec_attrib_st    *p_codec_attr,
 INOUT     icf_uint8_t            *p_codec_str
 
)
{
    icf_return_t        ret_val = ICF_SUCCESS;
    icf_amr_params_st   *p_amr_params = &(p_codec_attr->codec_params.audio_param.\
                        amr_params);
    icf_uint8_t         temp_codec_str[ICF_MAX_SIZE_MEDIUM_STR] = "\0";
    icf_int8_t         *p_rem = ICF_NULL, *p_temp_cdc_str = ICF_NULL;
    icf_uint8_t         mode_set;
    icf_uint8_t         index=0;

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    
    
    icf_port_strcpy(temp_codec_str, p_codec_str);
    
    p_rem = (icf_int8_t *)temp_codec_str;
	/* Set default codec mode as bandwidth 
     * efficient.
     */ 
    p_amr_params->codec_mode = ICF_BANDWIDTH_EFF_MODE;
    
    p_temp_cdc_str = 
        icf_port_strtokr(p_rem, "=",&p_rem);
    
    if(0 == icf_port_strcasecmp(p_temp_cdc_str, "octet-align"))
    {
        p_temp_cdc_str = ICF_NULL;
	/*	icf_port_strcpy(temp_codec_str, p_rem);*/
        
        p_temp_cdc_str = 
            icf_port_strtokr(p_rem, ";", &p_rem);
        
        if(0 == icf_port_strcasecmp(p_temp_cdc_str, "1"))
        {
            p_amr_params->codec_mode = ICF_OCTET_ALIGNED_MODE;
        }
        
        p_temp_cdc_str = ICF_NULL;
		if(ICF_NULL != p_rem)
        {
		/*	icf_port_strcpy(temp_codec_str, p_rem);*/
        
        	p_temp_cdc_str = 
            	icf_port_strtokr(p_rem, " =", &p_rem);
        /*    icf_port_strcpy(temp_codec_str, p_rem);*/
		}
        
        /*p_temp_cdc_str = */
          /*  icf_port_strtokr(temp_codec_str, "=", &p_rem);*/
        
        if((ICF_NULL != p_temp_cdc_str) && (0 == icf_port_strcasecmp(\
			p_temp_cdc_str,"crc")))
        {
            p_temp_cdc_str = icf_port_strtokr(\
                p_rem, " ", &p_rem);
            if(0 == icf_port_strcasecmp(\
                p_temp_cdc_str, "1"))
            {
                if(ICF_BANDWIDTH_EFF_MODE & p_amr_params->codec_mode)
                {
                    /* CRC along with Bandwidth efficient
                    * mode not allowed.
                    */
                    ICF_PRINT(((icf_uint8_t *)"\n CRC Set with Bandwidth efficient mode:"
                        " Send 488 Media Unsupported"));

                    p_ssa_pdb->p_ssa_ctxt->failure_bitmask |=\
                        ICF_SSA_FAILURE_RESP_CODE_488;

                    /* This warning code and header will be used for forming warning header.*/            
                    p_ssa_pdb->warning_header_code = 306;
                    icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"CRC Set with Bandwidth efficient mode Not Supported");
                    p_ssa_pdb->warning_text.str_len = (icf_uint16_t)icf_port_strlen(p_ssa_pdb->warning_text.str);

                    ret_val = ICF_FAILURE;
                }
                else
                {
                   p_amr_params->crc_enabled = 1;
                  /* icf_port_strcpy(temp_codec_str, p_rem);*/
                }
            }
        }
    }
     else if(0 == icf_port_strcasecmp(p_temp_cdc_str,"crc"))
     {
            p_temp_cdc_str = icf_port_strtokr(\
                p_rem, " ", &p_rem);
            if(0 == icf_port_strcasecmp(p_temp_cdc_str, "1"))
            {
			    p_amr_params->crc_enabled = 1;
			   	p_amr_params->codec_mode = ICF_OCTET_ALIGNED_MODE;
			}

	}
    else if(0 == icf_port_strcasecmp(p_temp_cdc_str, "mode-set"))
    {
    /* Retrieve the mode set value from the attribute line.
     * This will retrieve the complete list of mode-set values.
     * Only the first value of "mode-set=" will be set in the
     * initial mode.
     */
        ICF_PRINT(((icf_uint8_t *)"\n Mode-Set Attribute for AMR Codec Received"));
        index = 0;
      
        while(ICF_NULL != p_temp_cdc_str && ICF_NULL != p_rem)
        {
            p_temp_cdc_str = 
                icf_port_strtokr(p_rem, ",;", &p_rem);

	    if(ICF_NULL != p_temp_cdc_str)
	    {
            	mode_set = (icf_uint8_t)icf_port_atoi(p_temp_cdc_str);
            	p_amr_params->amr_mode_list[index] = mode_set;
	    }
            index ++;
        }
        if(0 < index)
        {
            p_amr_params->bit_mask |= ICF_AMR_MODE_LIST_PRESENT;
            p_amr_params->amr_initial_mode = p_amr_params->\
                amr_mode_list[0];            
        }   
        p_amr_params->amr_mode_count = index;
    }

    if(ICF_SUCCESS == ret_val)
    {
        p_codec_attr->codec_params.audio_param.bit_mask |= 
            ICF_AUDIO_CODEC_PARAM_AMR_PARAM;
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
 }


/*****************************************************************************
 ** FUNCTION:           icf_ssa_form_transport_mode_for_stream
 **
 ** DESCRIPTION:        This function will form the transport mode for the 
 **                     requested stream.
 *****************************************************************************/

icf_return_t icf_ssa_form_transport_mode_for_stream
(
 INOUT icf_ssa_pdb_st                  *p_ssa_pdb,
 INOUT icf_media_transport_mode_et     mode,
 OUT icf_uint8_t                    *p_transport_mode)
{
    icf_return_t    ret_val=ICF_SUCCESS;
    if(ICF_NULL == p_ssa_pdb)
    {
       return ICF_FAILURE;
    }
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

    switch(mode)
    {
        case ICF_TRANSPORT_MODE_UDP:
            icf_port_strcpy(p_transport_mode,(icf_uint8_t *)"udp");
            break;
        case ICF_TRANSPORT_MODE_TCP:
            icf_port_strcpy(p_transport_mode,(icf_uint8_t *)"tcp");
            break;
        case ICF_TRANSPORT_MODE_RTP_AVP:
            icf_port_strcpy(p_transport_mode,(icf_uint8_t *)"RTP/AVP");
            break;
#ifdef ICF_FAX_SUPPORT
#ifdef ICF_NON_IMS_CLIENT
        case ICF_TRANSPORT_MODE_UDPTL:
            icf_port_strcpy(p_transport_mode,"udptl");
            break;
#endif
#endif
        default:
            icf_port_strcpy(p_transport_mode,(icf_uint8_t *)"\0");            
    }

    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:           icf_ssa_free_inc_stream_stream_info
 **
 ** DESCRIPTION:        This function free the allocated memory to the structure
 **						icf_ssa_inc_media_stream_info_st.	
 *****************************************************************************/

icf_return_t icf_ssa_free_inc_media_stream_info(
			INOUT		icf_ssa_pdb_st *p_ssa_pdb,
			INOUT 	icf_ssa_inc_media_stream_info_st **p_p_inc_media_sdp_info)
{
	icf_error_t		ecode = ICF_ERROR_NONE;
	icf_return_t	ret_val = ICF_SUCCESS;


    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    if (ICF_NULL != *p_p_inc_media_sdp_info)
    {
	    if(ICF_NULL != (*p_p_inc_media_sdp_info)->p_sdp_info_list)	
	    {		
		    icf_cmn_free_media_sdp_info_list(p_ssa_pdb->p_glb_pdb,
				    &((*p_p_inc_media_sdp_info)->p_sdp_info_list),
				    &ecode);
	    }
#ifdef NON_IMS_CLIENT
	    ICF_CHECK_IMS_START(p_ssa_pdb->p_glb_pdb,ICF_FALSE)
#ifdef ICF_FAX_SUPPORT
		    /* SPR 12570 Fix.
		     */	
		    if(ICF_NULL != (*p_p_inc_media_sdp_info)->p_added_sdp_list)	
		    {		
			    icf_cmn_free_media_sdp_info_list(p_ssa_pdb->p_glb_pdb,
					    &((*p_p_inc_media_sdp_info)->p_added_sdp_list),
					    &ecode);
		    }

        if (ICF_NULL != (*p_p_inc_media_sdp_info)->p_nw_reject_stream_list)
        {
			    icf_cmn_free_media_sdp_info_list(p_ssa_pdb->p_glb_pdb,
					    &((*p_p_inc_media_sdp_info)->p_nw_reject_stream_list),
					    &ecode);            
        }

#endif
	    ICF_CHECK_IMS_END
#endif
		    ICF_MEMFREE(p_ssa_pdb->p_glb_pdb,
				    *p_p_inc_media_sdp_info,
				    ICF_MEM_COMMON,
				    p_ssa_pdb->p_ecode,
				    ret_val)
    }
    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:           icf_ssa_init_n_form_o_line
 **
 ** DESCRIPTION:        This function forms the origin line.
 **						In case of failure, the memory that is allocated p_origin is 
 **						freed here and failure is returned above and the callee function
 **						free the other memories.
 *****************************************************************************/
icf_return_t icf_ssa_init_n_form_o_line(
		INOUT		icf_ssa_pdb_st			*p_ssa_pdb,
		INOUT      icf_uint8_t				hold_option,
		INOUT	icf_transport_address_st		stream_addr,	
		INOUT 	SdpOrigin				**p_p_origin,
		INOUT	Sdf_st_error			*p_error)
{
	icf_string_st	    local_name;
	icf_uint8_t		    ip_type[4] = {"\0"};
	icf_return_t	    ret_val = ICF_SUCCESS;
    icf_uint8_t         version[11]="\0";
	Sdf_ty_s8bit        pSaddr[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    hold_option=hold_option;
	ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    (void)hold_option;
    
    /*if(ICF_TRUE == p_ssa_pdb->p_glb_pdb->p_call_ctx->sdp_changed)
      {*/
        (p_ssa_pdb->p_ssa_ctxt->sdp_o_version)++;
    /*}*/
    icf_port_sprintf((icf_int8_t *)version,"%d",p_ssa_pdb->p_ssa_ctxt->sdp_o_version);

    /* Set the local name for o line of SDP */
    /* 6_3 enhancement merge */
    /* fix for csr 5249946 */
#if 0
  	if ((ICF_TRUE == ICF_IS_BIT_SET(hold_option, ICF_SSA_OPTIONS_MODE_INACT)) ||
            ((ICF_NULL != (p_ssa_pdb->p_glb_pdb->p_call_ctx) &&
            (ICF_NULL != ((p_ssa_pdb->p_glb_pdb->p_call_ctx->common_bitmask)
             & ICF_LOCAL_NAME_PRESENT)))))
    {
        icf_port_strcpy(local_name.str,
                     p_ssa_pdb->p_glb_pdb->p_call_ctx->local_name.str);

            local_name.str_len = icf_port_strlen(local_name.str);

            icf_cmn_change_string_to_alphanumeric(p_ssa_pdb->p_glb_pdb, &local_name);

    }
	else
#endif
	{
        icf_port_strcpy((icf_uint8_t *)local_name.str, (icf_uint8_t *)ICF_NAME_IN_O_LINE);
	}

	if (ICF_TRANSPORT_ADDRESS_IPV4_ADDR == stream_addr.addr.addr_type)
	{
		strcpy((icf_int8_t *)ip_type,(const icf_int8_t *)"IP4");
	}
	else
	{
#ifdef ICF_IPV6_ENABLED
		strcpy(ip_type,"IP6");
#endif
	}

	if (SipFail == sip_initSdpOrigin(
		p_p_origin,(SipError *)&(p_error->stkErrCode)))
	{
		ret_val = ICF_FAILURE;
	}
	/*Fix done for SPR-18722 : CSR-1-6068965*/
	else if (SipFail == sdp_setUserInOrigin(
			*p_p_origin,Sdf_mc_strdupSdp((icf_int8_t *)local_name.str),
			(SipError *)&(p_error->stkErrCode)))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setSessionIdInOrigin(
			*p_p_origin, Sdf_mc_strdupSdp ((icf_int8_t *)"12345"),
			(SipError *) & (p_error->stkErrCode)))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setVersionInOrigin(
			*p_p_origin,Sdf_mc_strdupSdp((icf_int8_t *)version),
			(SipError *) & (p_error->stkErrCode)))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setNetTypeInOrigin (
			*p_p_origin, Sdf_mc_strdupSdp("IN"),
			(SipError *) & (p_error->stkErrCode)))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setAddrTypeInOrigin(
			*p_p_origin, Sdf_mc_strdupSdp((icf_int8_t *)ip_type),
			(SipError *) & (p_error->stkErrCode)))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}
	/* convert the transport address into a string */
	else if (ICF_FAILURE == icf_ssa_convert_to_IP_addr(
			(icf_transport_address_st *)&(stream_addr),(icf_uint8_t *)pSaddr))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}
	else if (SipFail == sdp_setAddrInOrigin(
			*p_p_origin, Sdf_mc_strdupSdp (pSaddr),
			(SipError *)&(p_error->stkErrCode)))
	{
		sip_freeSdpOrigin (*p_p_origin);
		ret_val = ICF_FAILURE;
	}


	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:           icf_ssa_fill_encoding_name_for_standard_codecs
 **
 ** DESCRIPTION:        This function fills codec encoding name for the codecs
 **                     for which the encoding name is not received in the SDP.
 **                     This function will also validate the dynamic codecs 
 **                     encoding name. IF not present then send 4xx.
 **
 *****************************************************************************/

 icf_return_t   icf_ssa_fill_encoding_name_for_standard_codecs
 (
 INOUT     icf_ssa_pdb_st  *p_ssa_pdb,
 INOUT   icf_list_st     *p_codec_list
 )
 {
     icf_return_t           ret_val = ICF_SUCCESS;
     icf_list_st            *p_temp_codec_list=p_codec_list;
     icf_codec_attrib_st    *p_codec_attrib = ICF_NULL;

     ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)

     while(ICF_NULL != p_temp_codec_list)
     {
         p_codec_attrib = (icf_codec_attrib_st*)(p_temp_codec_list->p_data);

         if (p_codec_attrib->codec_num > 127)
         {
            /* Generate 488 as codec no more than 127 are not Acceptable here.*/
            p_ssa_pdb->p_ssa_ctxt->failure_bitmask=\
                    ICF_SSA_FAILURE_RESP_CODE_488;
            ICF_PRINT(((icf_uint8_t *)"Codecs having no more than 127 are not Acceptable here."));
                  
            /* This warning code and header will be used for forming warning header.*/            
            p_ssa_pdb->warning_header_code = 307;
            icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"Codec Not Acceptable Here");
            p_ssa_pdb->warning_text.str_len = (icf_uint16_t )icf_port_strlen(p_ssa_pdb->warning_text.str);
 
            ret_val = ICF_FAILURE;
            break;
         }

         if(ICF_NULL == icf_port_strcmp(p_codec_attrib->codec_name.str,(icf_uint8_t *)"\0"))
         {
             ICF_PRINT(((icf_uint8_t *)"\n Encoding name for %d codec not received.",\
                 p_codec_attrib->codec_num));
             /* Check if the codec is dynamic then return failure
              */
               if((p_codec_attrib->codec_num >=96) && \
                                (p_codec_attrib->codec_num<= 127))
               {
                   /* Generate 415 as dynamic codec encoding name is missing.
                   */
                   p_ssa_pdb->p_ssa_ctxt->failure_bitmask=\
                       ICF_SSA_FAILURE_RESP_CODE_488;
                   ICF_PRINT(((icf_uint8_t *)"Encoding Name for dynamic codecs missing."));
                  
                   /* This warning code and header will be used for forming warning header.*/            
                   p_ssa_pdb->warning_header_code = 307;
                   icf_port_strcpy(p_ssa_pdb->warning_text.str,(icf_uint8_t *)"Encoding Name for dynamic codecs missing");
                   p_ssa_pdb->warning_text.str_len = (icf_uint16_t )icf_port_strlen(p_ssa_pdb->warning_text.str);
 
                   ret_val = ICF_FAILURE;
                   break;
               }
             /* Encoding Name for this codec is not present*/
             switch(p_codec_attrib->codec_num)
             {
             case 0:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"PCMU");                
                 break;
             case 3:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"GSM");                
                 break;
             case 4:
                icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"G723");  
                 break;
             case 8:
                icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"PCMA");  
                break;
             case 9:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"G722");  
                 break;
             case 15:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"G728");  
                 break;
             case 26:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"JPEG");  
                 break;                         
             case 31:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"H261");  
                 break; 
             case 32:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"MPV");  
                 break; 
             case 33:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"MP2T");  
                 break; 
             case 34:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"H264");  
                 break;  
             default:
                 icf_port_strcpy(p_codec_attrib->codec_name.str,(icf_uint8_t *)"\0");  
             }
             p_codec_attrib->codec_name.str_len=(icf_uint16_t )icf_port_strlen(\
                 p_codec_attrib->codec_name.str);
         }
         p_temp_codec_list = p_temp_codec_list->p_next;

     }

     ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
     return ret_val;
}





/***************************************************************************
**
** FUNCTION:     icf_ssa_form_sdp_for_options_resp
** DESCRIPTION:  This function forms the SDP from the media parameters 
**               present in the SDP structure of the call context.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**               This function forms Sip Sdp from p_local_sdp in the list 
**               of type icf_stream_sdp_info_st
**
***************************************************************************/
icf_return_t icf_ssa_form_sdp_for_options_resp(
        INOUT  SdpMessage                **p_p_sdpmsg,
	INOUT	  icf_ssa_pdb_st	*p_ssa_pdb,
        INOUT  icf_list_st            	*p_stream_sdp_info,
        OUT   Sdf_st_error              *p_error)
{
	/* local variables */
    SdpMedia                    *p_media = Sdf_co_null;
    SdpOrigin                   *p_origin = Sdf_co_null;
    SdpConnection               *p_connection = Sdf_co_null;
    Sdf_ty_s8bit                pSaddr[ICF_MAX_SIZE_MEDIUM_STR]="\0";
	icf_stream_id_t				media_count = ICF_NULL;

    SdpTime                             *p_sdpTime = Sdf_co_null;
    SdpMessage                          *p_sdpmsg = Sdf_co_null;
    icf_return_t                        ret_val = ICF_SUCCESS;
	icf_inc_media_sdp_info_st           *p_stream_sdp = ICF_NULL;
    icf_internal_sdp_stream_st          *p_stream_node = ICF_NULL;
	icf_list_st							*p_stream_list = ICF_NULL;
    icf_media_type_t                  	stream_type = ICF_STREAM_TYPE_INVALID;
    icf_transport_address_st    		stream_address = {{0,{{0,"\0"}}},0};
    icf_uint8_t				hold_option = ICF_SSA_HOLD_PORT_ZERO;

	
	/* this function will allocate memory for the SDP message and init
	 * the buffer */
    ICF_SSA_INIT_TRANS_ADDR(&stream_address)
	if (SipFail == sip_initSdpMessage(
		p_p_sdpmsg,(SipError *)&(p_error->stkErrCode)))
	{
		ICF_PRINT(((icf_uint8_t *)"\nFailure in sip_initSdpMessage"));
		return ICF_FAILURE;
	}
	p_sdpmsg = *p_p_sdpmsg;
	if(ICF_NULL == p_sdpmsg)
	{
		return ICF_FAILURE;
	}

	if(ICF_NULL != p_stream_sdp_info)
	{
	/*Get the first node of the list*/
	p_stream_sdp = (icf_inc_media_sdp_info_st*) p_stream_sdp_info->p_data;
	}

	if (p_stream_sdp)
	{
		/* get the icf_internal_sdp_stream_st from the payload */
		/* populate the session level c line in the SDP */
		if ( ICF_FAILURE != ret_val)
		{
		   /* Get the session level stream address.
         	* If first stream address is zero then set session level
         	* c line address to the first non zero address.
         	* Check that stream addr is to be fetched first NW-ALLOW stream
		    */
       		icf_port_memset((icf_void_t *)&stream_address,
           					0,
							sizeof(icf_transport_address_st));
		
			stream_address.addr.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;
       
       		for (p_stream_list = p_stream_sdp_info;p_stream_list != ICF_NULL;\
           		p_stream_list = p_stream_list->p_next)
        	{
           		p_stream_sdp = (icf_inc_media_sdp_info_st *)p_stream_list->p_data;
				p_stream_node = p_stream_sdp->p_int_sdp_stream;
				if (p_stream_node) 
           		{
                	icf_port_memcpy(&stream_address,
							&(p_stream_node->sdp_stream.stream_address),
                    		sizeof(icf_transport_address_st));

					/* Convert the transport address to a string*/
		       		if(ICF_FAILURE == icf_ssa_convert_to_IP_addr(\
			       			(icf_transport_address_st *)&\
							stream_address,(icf_uint8_t *)pSaddr))
					{
						ret_val = ICF_FAILURE;	
					}		
					break;
				}	
       		}
		}
	}	/* formed c line */

	if ( ICF_FAILURE != ret_val)
	{
		/* Setting the v= line */
		if (SipFail == sdp_setVersion(
			p_sdpmsg, Sdf_mc_strdupSdp("0"),
			(SipError *)&(p_error->stkErrCode)))
		{
			sip_freeSdpMessage(p_sdpmsg);
			ret_val =  ICF_FAILURE;
		}
        	/*Set the Origin Line */
		if(ICF_FAILURE == icf_ssa_init_n_form_o_line(p_ssa_pdb,
					hold_option,
					stream_address,	
					&p_origin,
					p_error))
		{
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}			
		else if (SipFail == sdp_setOrigin (
					p_sdpmsg, p_origin,
				(SipError *)(&(p_error->stkErrCode))))
		{
			p_error->errCode = p_error->stkErrCode;
			sip_freeSdpOrigin (p_origin);
			ret_val = ICF_FAILURE;
		}
		else
		{
			sip_freeSdpOrigin (p_origin);
		}	
	}

	if ( ICF_FAILURE != ret_val)
	{
		/* Set the Session-Line */
		p_sdpmsg->pSession = Sdf_mc_strdupSdp("Session");
		
		/* Set the t-line */
		if (SipFail == sip_initSdpTime(
			&p_sdpTime, (SipError *)&(p_error->errCode)))
		{
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else if (SipFail == sdp_setStartInTime(
			p_sdpTime, Sdf_mc_strdupCallHandler("0"),
			(SipError *)&(p_error->errCode)))
		{
			sip_freeSdpTime(p_sdpTime);
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else if (SipFail == sdp_setStopInTime(
			p_sdpTime, Sdf_mc_strdupCallHandler("0"),
			(SipError *)&(p_error->errCode)))
		{
			sip_freeSdpTime(p_sdpTime);
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else if (SipFail == sdp_insertTimeAtIndex(
			p_sdpmsg, p_sdpTime, (Sdf_ty_u32bit)0,
			(SipError *)&(p_error->errCode)))
		{
			sip_freeSdpTime(p_sdpTime);
			sip_freeSdpMessage(p_sdpmsg);
			ret_val = ICF_FAILURE;
		}
		else
		{
			sip_freeSdpTime(p_sdpTime);
		}
	}/*O Line Handling Block End*/

	if ( ICF_FAILURE != ret_val)
	{
        /* Form the session level c-line for SDP */
        if(ICF_FAILURE == icf_ssa_init_n_form_c_line(
					p_ssa_pdb, 
					&stream_address,
					&p_connection,
					p_error))
        {
            sip_freeSdpConnection (p_connection);
            sip_freeSdpMessage(p_sdpmsg);
            ret_val = ICF_FAILURE;
        }
        else if (SipFail == sdp_setConnection(p_sdpmsg,p_connection,\
            (SipError *)&(p_error->stkErrCode)))
        {
            sip_freeSdpConnection (p_connection);
            sip_freeSdpMessage(p_sdpmsg);
            ret_val = ICF_FAILURE;
        }

		if(ICF_FAILURE != ret_val)
		{
			sip_freeSdpConnection (p_connection);		
		}
    }/*C Line handling Block End*/

    /* Traverse the stream-level list and form media and attribute lines for
     * each stream */
    if(ICF_FAILURE != ret_val)
	{
        for (p_stream_list = p_stream_sdp_info;
        p_stream_list != ICF_NULL; p_stream_list = p_stream_list->p_next)
        {
            p_stream_sdp = (icf_inc_media_sdp_info_st *)p_stream_list->p_data;
			p_stream_node = p_stream_sdp->p_int_sdp_stream;

            stream_type = p_stream_sdp->stream_type;
            
            switch(stream_type)
            {
            	case ICF_STREAM_TYPE_AUDIO:
                	if(ICF_FAILURE == icf_ssa_form_audio_for_options_from_sdp_info(p_ssa_pdb,
                    					p_stream_sdp, 
										&p_media, 
										p_error))
                	{
                    	ret_val = ICF_FAILURE;
                    	sip_freeSdpMessage(p_sdpmsg);
                    
                    	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;		
                	}
                	break;
                
            	case ICF_STREAM_TYPE_VIDEO:
                	if(ICF_FAILURE == icf_ssa_form_video_for_options_from_sdp_info(p_ssa_pdb,
			                    p_stream_sdp, 
								&p_media, 
								p_error))
                	{
                    	ret_val = ICF_FAILURE;
                    	sip_freeSdpMessage(p_sdpmsg);
                    
                    	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                        return ret_val;		
                	}
                	break;
            }

           if(Sdf_co_null != p_media)
	   	   { 
           	 	if ((SipFail == sdp_insertMediaAtIndex(
                	p_sdpmsg, p_media, media_count,
                	(SipError *)&(p_error->stkErrCode))))
           	 	{
                	sip_freeSdpMedia(p_media);
                	sip_freeSdpMessage(p_sdpmsg);
                
                	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
                    	return ICF_FAILURE;
            	}
           		else
            	{
                	sip_freeSdpMedia(p_media);
                	p_media = Sdf_co_null;		
                	media_count++;
            	}
	    	}
        } /* for (p_stream_list */
    }

	return ret_val;
}


icf_return_t icf_ssa_form_video_for_options_from_sdp_info
(
	INOUT	icf_ssa_pdb_st				*p_ssa_pdb, 
    INOUT 	icf_inc_media_sdp_info_st	*p_stream_sdp, 
	OUT		SdpMedia					**p_p_media,
	OUT		Sdf_st_error				*p_error
)
{
	/* local variables */
	icf_codec_attrib_st  *p_codec = ICF_NULL;
	icf_sdp_stream_st	 video_sdp;
	icf_list_st			 *p_codec_info_lst = ICF_NULL;
    SdpMedia             *p_media = Sdf_co_null;
	Sdf_ty_s8bit         pString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pTempString[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pTempString_2[ICF_MAX_SIZE_MEDIUM_STR]={"\0"};
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]="\0";
	icf_uint8_t          transport_mode[ICF_MAX_SIZE_MEDIUM_STR]="\0"; 
    Sdf_ty_s8bit         *pBandwidth= ICF_NULL;
	
    icf_uint16_t         rtp_port_num = 0;
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_boolean_t        encode_codec = ICF_TRUE;
	
	video_sdp = p_stream_sdp->p_int_sdp_stream->sdp_stream;
	
	rtp_port_num = 0;
	
	/* Forming media formats*/
	for (p_codec_info_lst = video_sdp.p_codec_info_list; 
		ICF_NULL != p_codec_info_lst; p_codec_info_lst = p_codec_info_lst->p_next)
	{
		p_codec = (icf_codec_attrib_st *)p_codec_info_lst->p_data;
		
		if(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec->bit_mask)
		{
			encode_codec = ICF_TRUE;
			if(p_codec->codec_num >= 96 && p_codec->codec_num <= 127)
			{
				encode_codec = ICF_FALSE;
				/* Dynamic Codec present */
				if((ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & \
					p_codec->bit_mask) &&\
					(ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT & p_codec->\
					codec_params.video_param.bit_mask))
				{
					encode_codec = ICF_TRUE;
				}
			}
			if(ICF_TRUE == encode_codec)
			{
				icf_port_sprintf(pTempString,(const icf_int8_t *)"%d ",p_codec->codec_num);
				icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)pTempString);
				icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)" ");
			}  
		}
	}/* End of for loop for format string.*/
			
	icf_port_strcpy((icf_uint8_t *)pString, (icf_uint8_t *)"0");
	icf_ssa_form_transport_mode_for_stream(p_ssa_pdb,\
		video_sdp.trans_mode, transport_mode);
	if('\0' == transport_mode[0])
	{
		icf_port_strcpy(transport_mode,(icf_uint8_t *)"RTP/AVP");
	}
	
	/* Check for the bandwidth parameter and if present then form the 
	* bandwidth value.
	*/
	if((ICF_SDP_STREAM_MEDIA_STREAM_PRESENT &\
		video_sdp.bit_mask) && 
		(ICF_VIDEO_PROFILE_BANDWIDTH_PRESENT & 
		video_sdp.media_stream_param.video_profile.bit_mask))
	{    	
		pTempString_2[0] = '\0';
		icf_port_strcpy((icf_uint8_t *)pTempString_2,(icf_uint8_t *)"AS:");
		
		icf_port_sprintf(pTempString,"%d ",\
			video_sdp.media_stream_param.\
			video_profile.bandwidth);
		
		icf_port_strcat((icf_uint8_t *)pTempString_2, (icf_uint8_t *)pTempString);
		
		ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
			icf_port_strlen((icf_uint8_t *)pTempString_2) + 1, 
			ICF_MEM_COMMON, 
			pBandwidth,
			ICF_RET_ON_FAILURE,
			p_ssa_pdb->p_ecode,
			ret_val)
			
			icf_port_strcpy((icf_uint8_t *)pBandwidth, (icf_uint8_t *)pTempString_2);
	}
				
	/* Forming SDP media */
	if (Sdf_co_fail == sdf_ivk_uaFormSdpMedia(
		(icf_int8_t *)"video", 
		rtp_port_num, 
		1, 
		(icf_int8_t *)transport_mode, 
		pString,
		Sdf_co_null , 
		Sdf_co_null,
		Sdf_co_null,
		pBandwidth,
		Sdf_co_null,
		Sdf_co_null, 
		&p_media, 
		p_error))
	{
		ret_val = ICF_FAILURE;
	}
	
	/* Form the connection (c=) line*/
	pString[0]='\0';
	
	pTempString[0] = '\0';
	
	for (p_codec_info_lst = video_sdp.p_codec_info_list;
	ICF_NULL != p_codec_info_lst; 
	p_codec_info_lst = p_codec_info_lst->p_next)
	{
		p_codec = (icf_codec_attrib_st *)p_codec_info_lst->p_data;
		
		pAttributeString[0] = '\0'; 
		
		if (ICF_NULL !=  p_codec->codec_name.str_len)
		{	
			/* If this is a dynamic codec then form the rtpmap parameter */
			if(ICF_FAILURE != ret_val) 
			{                             
				if (ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & 
					p_codec->bit_mask)
				{
					if(ICF_VIDEO_CODEC_CLOCK_RATE_PRESENT &
						p_codec->codec_params.video_param.bit_mask)
					{
						icf_port_sprintf(pAttributeString, "  rtpmap:%d %s/%d",
							p_codec->codec_num,
							p_codec->codec_name.str,
							p_codec->codec_params.video_param.clock_rate);
						if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(
							pAttributeString,p_media,p_error))
						{
							ret_val = ICF_FAILURE;
						}
					}
					else if(!(ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & 
						p_codec->bit_mask))
					{
						/* Clock rate not present*/
						ICF_PRINT(((icf_uint8_t *)"\n Clock Rate for codec "
							"attribute missing"));
						continue;
					}
				}
			}
		}
		
		if(ICF_FAILURE != ret_val && 
			(ICF_NULL != (ICF_VIDEO_CODEC_FRAMESIZE_PRESENT &
			p_codec->codec_params.video_param.bit_mask)))
		{
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  framesize:");
			
			icf_port_sprintf(pTempString,"%d %d-%d",\
				p_codec->codec_num,\
				p_codec->codec_params.video_param.frame_size.width,
				p_codec->codec_params.video_param.frame_size.height);	
			
			icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
			if (Sdf_co_fail ==    
				sdf_fn_uaFormAttributeFromString(
				pString,p_media,p_error))
			{
				ret_val = ICF_FAILURE;
			}
		}
		
		/*Form the a-line corresponding to the encoded attributes of the codec*/
		if(ICF_FAILURE != ret_val && 
			(ICF_NULL != (p_codec->bit_mask & ICF_CODEC_ENC_ATTRIB_PRESENT)))
		{
			if(ICF_FAILURE == icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
				p_media, p_codec->p_encoded_attributes, p_error))
			{
				ret_val = ICF_FAILURE;	
			}
		}
	} /* fr loop ends here */
				
	if(ICF_FAILURE != ret_val && 
		(ICF_NULL != (ICF_VIDEO_PROFILE_ENC_QUALITY_PRESENT
		& video_sdp.media_stream_param.video_profile.bit_mask)))
	{
		icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
		icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  quality:");
		
		icf_port_sprintf(pTempString,"%d",
			video_sdp.media_stream_param.video_profile.encoding_quality);
		
		icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
		
		if (Sdf_co_fail ==    
			sdf_fn_uaFormAttributeFromString(
			pString,p_media,p_error))
		{
			ret_val = ICF_FAILURE;
		}
	}
	
	if(ICF_FAILURE != ret_val && 
		(ICF_NULL != (ICF_VIDEO_PROFILE_FRAME_RATE_PRESENT &
		video_sdp.media_stream_param.video_profile.bit_mask)))
	{
		icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"");
		icf_port_strcpy((icf_uint8_t *)pString,(icf_uint8_t *)"  framerate:");
		
		icf_port_sprintf(pTempString,"%d",
			video_sdp.media_stream_param.video_profile.frame_rate);	
		
		icf_port_strcat((icf_uint8_t *)pString, (icf_uint8_t *)pTempString);
		if (Sdf_co_fail ==    
			sdf_fn_uaFormAttributeFromString(
			pString,p_media,p_error))
		{
			ret_val = ICF_FAILURE;
		}
	}
	
	
	/*Form the attributes based on the media level encoding parameters*/         
	if(ICF_FAILURE != ret_val &&\
		(ICF_NULL != (ICF_SDP_STREAM_ENCPARAMS_PRESENT
		& video_sdp.bit_mask)))
	{ 
		if(ICF_FAILURE == 
			icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
			p_media,
			video_sdp.p_encoded_attributes,
			p_error))
		{
			ret_val = ICF_FAILURE;	
		}
	}
	
	/*Free this memory as this has alredy been pointed by SIP-STACK
	*So,there is no need to keep it locally
	*/
	
	if(pBandwidth)	
	{
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
			pBandwidth,
			ICF_MEM_COMMON,
			p_ssa_pdb->p_ecode,
			ret_val)
	}
	
	if(ICF_FAILURE == ret_val)
	{
		sip_freeSdpMedia (p_media);
	}
	else
	{
		/*Assign the media-line*/
		*p_p_media = p_media;
	}
	
	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
		return ret_val;
}



/***************************************************************************
**
** FUNCTION:     icf_ssa_form_audio_for_options_from_sdp_info
** DESCRIPTION:  This function forms the SDP for the audio media parameters 
**               present in the SDP structure of the Options Response.
**               (The SDP structure is expected to be initialised before
**               calling this function)
**
**
***************************************************************************/
icf_return_t icf_ssa_form_audio_for_options_from_sdp_info(
        INOUT    icf_ssa_pdb_st            *p_ssa_pdb,
        INOUT    icf_inc_media_sdp_info_st *p_stream_sdp,
        INOUT  SdpMedia                  **p_p_media,
        OUT   Sdf_st_error              *p_error)
{
    icf_return_t         ret_val = ICF_SUCCESS;
    icf_ssa_ctxt_st      *p_ssa_ctxt = ICF_NULL;
    icf_sdp_stream_st    audio_sdp;
    icf_codec_attrib_st  *p_codec_info = ICF_NULL;
    icf_uint16_t         rtp_port_num = 0;
    Sdf_ty_s8bit         pString[ICF_MAX_STR_LEN + 2] = "\0";
    Sdf_ty_s8bit         pTempString[ICF_MAX_STR_LEN + 2] = "\0";
    Sdf_ty_s8bit         pTempString_2[ICF_MAX_STR_LEN + 2] = "\0";
    SdpMedia             *p_media = Sdf_co_null;
    Sdf_ty_s8bit         *pBandwidth= ICF_NULL;
    icf_uint8_t          transport_mode[ICF_MAX_SIZE_MEDIUM_STR]="\0";
    icf_list_st          *p_codec_lst = ICF_NULL;
    icf_boolean_t        encode_codec = ICF_TRUE;
#ifdef ICF_RTCP_ATTR_SUPPORT
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]="\0";
#endif

    p_ssa_pdb=p_ssa_pdb; 
 
    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    
	p_ssa_ctxt = p_ssa_pdb->p_ssa_ctxt;
    
        audio_sdp = p_stream_sdp->p_int_sdp_stream->sdp_stream;
        rtp_port_num = 0;
        
        for (p_codec_lst = audio_sdp.p_codec_info_list; 
        ICF_NULL != p_codec_lst;
        p_codec_lst = p_codec_lst->p_next)
        {
            p_codec_info = (icf_codec_attrib_st *)p_codec_lst->p_data;
            encode_codec=ICF_TRUE;
            
            if(ICF_CODEC_ATTRIB_CODEC_NUM_PRESENT & p_codec_info->bit_mask)
            {
                if(p_codec_info->codec_num >= 96 && p_codec_info->codec_num <= 127)
                {
                /* Dynamic Codec present
                    */
		    encode_codec = ICF_FALSE;
                    if((ICF_CODEC_ATTRIB_CODEC_PARAMS_PRESENT & \
                        p_codec_info->bit_mask) &&\
                        (ICF_AUDIO_CODEC_PARAM_SAMPLING_RATE & p_codec_info->\
                        codec_params.audio_param.bit_mask))
                    {
                        encode_codec = ICF_TRUE;
                    }
                }
                if(ICF_TRUE == encode_codec)
                {
                    icf_port_sprintf(pTempString,"%d ",p_codec_info->codec_num);
                    icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)pTempString);
                    icf_port_strcat((icf_uint8_t *)pString,(icf_uint8_t *)" ");
                }  
            }

	   }      
	icf_ssa_form_transport_mode_for_stream(p_ssa_pdb,
		audio_sdp.trans_mode, transport_mode);
       if(0 == icf_port_strcmp(transport_mode,(icf_uint8_t *)"\0"))
       {
	       icf_port_strcpy(transport_mode,(icf_uint8_t *)"RTP/AVP");
       }   

	       /* Check for the bandwidth parameter and if present then form the 
		* bandwidth value.
		*/

	       if((ICF_SDP_STREAM_MEDIA_STREAM_PRESENT &\
				       p_stream_sdp->p_int_sdp_stream->sdp_stream.bit_mask) &&
			       (ICF_AUDIO_PROF_BNDWDTH_PRESENT & audio_sdp.media_stream_param.\
				audio_profile.bit_mask))
	       {	
		       icf_port_strcpy((icf_uint8_t *)pTempString_2,(icf_uint8_t *)"AS:");

		       icf_port_sprintf(pTempString,"%d ",\
				       audio_sdp.media_stream_param.\
				       audio_profile.bandwidth);

		       icf_port_strcat((icf_uint8_t *)pTempString_2, (icf_uint8_t *)pTempString);

		       ICF_MEMGET(p_ssa_pdb->p_glb_pdb, 
				       icf_port_strlen((icf_uint8_t *)pTempString_2)+1, 
				       ICF_MEM_COMMON, 
				       pBandwidth,
				       ICF_RET_ON_FAILURE,
				       p_ssa_pdb->p_ecode,
				       ret_val)

			       icf_port_strcpy((icf_uint8_t *)pBandwidth, (icf_uint8_t *)pTempString_2);
	       }    

       /* Form the Media Stream in the SDP*/
       if(Sdf_co_fail == sdf_ivk_uaFormSdpMedia((icf_int8_t *)"audio", rtp_port_num,\
			       1,(icf_int8_t *)transport_mode, (icf_int8_t *)pString, Sdf_co_null, Sdf_co_null, Sdf_co_null,\
			       pBandwidth,Sdf_co_null, Sdf_co_null, &p_media, p_error))
       {
	       ret_val = ICF_FAILURE;
       }

    
    /* Form the attribute parameters for the media stream : rtp_port_num != 0*/
    if(ICF_FAILURE != ret_val)
    {
        pString[0]='\0';
        pTempString[0] = '\0';           
        
        /* Form the codec attributes for the audio media stream.*/
        ret_val = icf_ssa_form_audio_codec_attrs_in_media_stream(p_ssa_pdb,\
												p_media,
												audio_sdp.p_codec_info_list,    
												p_error);
              
       /* IF the media stream bitmask is present then form the media stream attributes.
        */
        if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_MEDIA_STREAM_PRESENT &\
            audio_sdp.bit_mask))
        {
            ret_val = icf_ssa_form_media_attrs_for_options_in_audio_stream(p_ssa_pdb,
															   p_stream_sdp,
															   p_media,
															   p_error);
        }
        
#ifdef ICF_RTCP_ATTR_SUPPORT
        /* Form the rtcp attribute line , if present
         */
        if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_RTCP_INFO_PRESENT &\
            audio_sdp.bit_mask))
        {
            pAttributeString[0] = '\0';
            pTempString[0] = '\0';
            icf_port_strcpy(pAttributeString,"  rtcp:");
            icf_port_itoa(audio_sdp.rtcp_address.\
                port_num,pTempString);
/*            pTempString.str_len = icf_port_strlen(pTempString.str);
 *            */

            icf_port_strcat(pAttributeString,pTempString);

            pTempString[0] = '\0';

            icf_port_strcat(pAttributeString," IN");
          
            if(ICF_TRANSPORT_ADDRESS_IPV6_ADDR == audio_sdp.rtcp_address.\
                addr.addr_type)
            {
                
#ifdef ICF_IPV6_ENABLED
                icf_port_strcat(pAttributeString," IP6");
#endif
            }
            else if(ICF_TRANSPORT_ADDRESS_IPV4_ADDR == audio_sdp.\
                rtcp_address.addr.addr_type)
            {
                icf_port_strcat(pAttributeString," IP4");
			
            /* Convert the transport address to a string*/
            ret_val = icf_ssa_convert_to_IP_addr(\
        		(icf_transport_address_st *)&(audio_sdp.rtcp_address),
				pTempString);
			}
            else
            {
                /* Domain Name or any other address type is not valid*/
                ret_val = ICF_FAILURE;
            }
            if(ICF_FAILURE != ret_val)
            {               
                icf_port_strcat(pAttributeString," ");
                icf_port_strcat(pAttributeString,pTempString);  
                
                if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
                    pAttributeString,p_media,p_error))
                {
                    ret_val = ICF_FAILURE;
                }
            }
        }/* End of rtcp attribute information*/
#endif

        /* Form the encoded attributes if present for the media
        */
        if((ICF_FAILURE != ret_val) && (ICF_SDP_STREAM_ENCPARAMS_PRESENT &\
            audio_sdp.bit_mask))
        {
            ret_val = icf_ssa_form_encoded_attrs_in_sdp_stream(p_ssa_pdb,\
                p_media,(icf_list_st *)audio_sdp.\
                p_encoded_attributes,p_error);
            
        }/*End of if block for Encoded attributes.*/
        
    }/* End of if check for making the attributes line.*/

	if(pBandwidth)	
	{
		ICF_MEMFREE(p_ssa_pdb->p_glb_pdb, 
					pBandwidth,
					ICF_MEM_COMMON,
					p_ssa_pdb->p_ecode,
					ret_val)
	}

	/* Free the SDP Message and p_media in case of failure.    */

	if(ICF_FAILURE == ret_val)
	{
			sip_freeSdpMedia (p_media);
	}
	else
	{
			*p_p_media = p_media;
	}

	ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
			return ret_val;
}




/**********************************************************************    
 *
 * FUNCTION:        icf_ssa_form_media_attrs_for_options_in_audio_stream
 * DESCRIPTION:     This function forms the media attributes for the audio 
 *                  stream. This function will form the ptime,
 *                  silence_suppresion attributes in the media stream.
 *   
 ***********************************************************************/
icf_return_t icf_ssa_form_media_attrs_for_options_in_audio_stream(
        INOUT		icf_ssa_pdb_st	            *p_ssa_pdb,
        INOUT    icf_inc_media_sdp_info_st   *p_stream_sdp,
        INOUT  SdpMedia                       *p_media,
        OUT   Sdf_st_error                   *p_error)

{
    icf_return_t         ret_val = ICF_SUCCESS;
    Sdf_ty_s8bit         pTempString[ICF_MAX_STR_LEN + 2] = "\0";
    Sdf_ty_s8bit         pAttributeString[ICF_MAX_SIZE_MEDIUM_STR]="\0";
	icf_sdp_stream_st	 audio_sdp = p_stream_sdp->p_int_sdp_stream->sdp_stream;

    if(ICF_NULL == p_ssa_pdb)
    {
        return ICF_FAILURE;
    }

    ICF_FUNCTION_ENTER(p_ssa_pdb->p_glb_pdb)
    /* Form the ptime attribute if present in audio profile*/
    if((ICF_AUDIO_PROF_PTIME_PRESENT & audio_sdp.\
        media_stream_param.audio_profile.bit_mask) && \
        (ICF_NULL != audio_sdp.media_stream_param.audio_profile.pref_ptime))
    {
        /* Create Packetization period attribute */
        /* Append the packetization period in the Sdp Media
        */
		if(audio_sdp.media_stream_param.audio_profile.pref_ptime)
		{
			pAttributeString[0] = '\0';
			pTempString[0] = '\0';
			icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"");
			icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  ptime:");
			icf_port_sprintf(pTempString,"%d",\
				audio_sdp.media_stream_param.audio_profile.pref_ptime);
			
			icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)pTempString);
			if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
				pAttributeString,p_media,p_error))
			{
				ret_val = ICF_FAILURE;
			}
		}
		else
		{
			ICF_PRINT(((icf_uint8_t *)"\nFailed to get Ptime from AUDIO stream"));
		}		
    }
    
     /* Form the silence suppression attribute if present in audio profile*/
    if((ICF_FAILURE != ret_val) && \
        (ICF_AUDIO_PROF_SILN_SUPP_PRESENT &\
		audio_sdp.media_stream_param.audio_profile.bit_mask))
    {
        /* Create Silence Suppresion attribute */
        /* Append the Silence Suppresion in the Sdp Media
        */
        pAttributeString[0] = '\0';
        pTempString[0] = '\0';
        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"");
        icf_port_strcpy((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"  silenceSupp:");
        if(ICF_TRUE == audio_sdp.media_stream_param.\
				audio_profile.silence_suppression)
        {
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"on");
        }
        else
        {
            icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)"off");
        }
        
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        icf_port_strcat((icf_uint8_t *)pAttributeString,(icf_uint8_t *)" -");
        
        if (Sdf_co_fail == sdf_fn_uaFormAttributeFromString(\
            pAttributeString,p_media,p_error))
        {
            ret_val = ICF_FAILURE;
        }
    }/* End of if check for silence suppresion attribute.*/


    ICF_FUNCTION_EXIT(p_ssa_pdb->p_glb_pdb)
    return ret_val;
}


#ifdef ICF_QOS_SUPPORT
/*****************************************************************************
 ** FUNCTION:        icf_ssa_prepare_qos_status_lines_from_sdp
 ** 
 ** DESCRIPTION:    This function forms Current, Desired and Confirm status
 **                 lines for Qos Support from sdp structure.
 **
 *****************************************************************************/
icf_return_t icf_ssa_prepare_qos_status_lines_from_sdp(
        INOUT    icf_internal_sdp_stream_st *p_sdp,
        INOUT  Sdf_ty_s8bit  pQosStatusStrings[][ICF_MAX_LENGTH_OF_QOS_LINE])
{
    icf_return_t ret_val = ICF_SUCCESS;

    /* Preparing Confirm status Lines for remote end */
    if ((ICF_RESOURCE_RESERVATION_CONF_REQ == p_sdp->remote_send.
                confirmation_status) && (ICF_RESOURCE_RESERVATION_CONF_REQ
                    == p_sdp->remote_recv.confirmation_status))
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[0],(icf_uint8_t *)"  conf:qos remote sendrecv");
    }
    else if (ICF_RESOURCE_RESERVATION_CONF_REQ == p_sdp->remote_send.
            confirmation_status)
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[0],(icf_uint8_t *)"  conf:qos remote send");
    }
    else if (ICF_RESOURCE_RESERVATION_CONF_REQ
                    == p_sdp->remote_recv.confirmation_status)
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[0],(icf_uint8_t *)"  conf:qos remote recv");
    }
    else
    {
        /* If no confirmation is required in either direction then
         * mark as null string
         */
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[0],(icf_uint8_t *)"");
    }
    /* there is no concept of confirmation on
     * local end. But earlier we have reserved index 1 element of array
     * pQosStatusStrings for local end Qos confirmation so we are setting
     * it to empty string
     */
    
    /* Preparing Desired status lines for remote end */
    if (p_sdp->remote_send.desired_status == p_sdp->remote_recv.desired_status)
    {
        switch (p_sdp->remote_send.desired_status)
        {
            case ICF_RESOURCE_RESERVATION_MANDATORY:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos mandatory remote sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_OPTIONAL:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos optional remote sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_NONE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos none remote sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_FAILURE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos failure remote sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_UNKNOWN:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos unknown remote sendrecv");
                break;
        }
    }
    
    else
    {
        /* Preparing remote desired line in recv direction */
        switch (p_sdp->remote_recv.desired_status)
        {
            case ICF_RESOURCE_RESERVATION_MANDATORY:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos mandatory remote recv");
                break;
            case ICF_RESOURCE_RESERVATION_OPTIONAL:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos optional remote recv");
                break;
            case ICF_RESOURCE_RESERVATION_NONE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos none remote recv");
                break;
            case ICF_RESOURCE_RESERVATION_FAILURE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos failure remote recv");
                break;
            case ICF_RESOURCE_RESERVATION_UNKNOWN:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[1],
                        (icf_uint8_t *)"  des:qos unknown remote recv");
                break;
        }
        
        /* Preparing remote desired line in send direction */
        switch (p_sdp->remote_send.desired_status)
        {
            case ICF_RESOURCE_RESERVATION_MANDATORY:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[2],
                        (icf_uint8_t *)"  des:qos mandatory remote send");
                break;
            case ICF_RESOURCE_RESERVATION_OPTIONAL:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[2],
                        (icf_uint8_t *)"  des:qos optional remote send");
                break;
            case ICF_RESOURCE_RESERVATION_NONE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[2],
                        (icf_uint8_t *)"  des:qos none remote send");
                break;
            case ICF_RESOURCE_RESERVATION_FAILURE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[2],
                        (icf_uint8_t *)"  des:qos failure remote send");
                break;
            case ICF_RESOURCE_RESERVATION_UNKNOWN:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[2],
                        (icf_uint8_t *)"  des:qos unknown remote send");
                break;
        }
    }

    /* Preparing Desired status lines for local end */
    if (p_sdp->local_send.desired_status == p_sdp->local_recv.desired_status)
    {
        switch (p_sdp->local_send.desired_status)
        {
            case ICF_RESOURCE_RESERVATION_MANDATORY:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos mandatory local sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_OPTIONAL:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos optional local sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_NONE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos none local sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_FAILURE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos failure local sendrecv");
                break;
            case ICF_RESOURCE_RESERVATION_UNKNOWN:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos unknown local sendrecv");
                break;
        }
    }
    
    else
    {
        /* Preparing local desired line in recv direction */
        switch (p_sdp->local_recv.desired_status)
        {
            case ICF_RESOURCE_RESERVATION_MANDATORY:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos mandatory local recv");
                break;
            case ICF_RESOURCE_RESERVATION_OPTIONAL:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos optional local recv");
                break;
            case ICF_RESOURCE_RESERVATION_NONE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos none local recv");
                break;
            case ICF_RESOURCE_RESERVATION_FAILURE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos failure local recv");
                break;
            case ICF_RESOURCE_RESERVATION_UNKNOWN:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[3],
                        (icf_uint8_t *)"  des:qos unknown local recv");
                break;
        }
        
        /* Preparing local desired line in send direction */
        switch (p_sdp->local_send.desired_status)
        {
            case ICF_RESOURCE_RESERVATION_MANDATORY:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[4],
                        (icf_uint8_t *)"  des:qos mandatory local send");
                break;
            case ICF_RESOURCE_RESERVATION_OPTIONAL:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[4],
                        (icf_uint8_t *)"  des:qos optional local send");
                break;
            case ICF_RESOURCE_RESERVATION_NONE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[4],
                        (icf_uint8_t *)"  des:qos none local send");
                break;
            case ICF_RESOURCE_RESERVATION_FAILURE:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[4],
                        (icf_uint8_t *)"  des:qos failure local send");
                break;
            case ICF_RESOURCE_RESERVATION_UNKNOWN:
                icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[4],
                        (icf_uint8_t *)"  des:qos unknown local send");
                break;
        }
    }

    /* Preparing current line for remote end */
    if ((ICF_RESOURCE_RESERVED == p_sdp->remote_send.current_status) &&
            (ICF_RESOURCE_RESERVED == p_sdp->remote_recv.current_status))
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[5],(icf_uint8_t *)"  curr:qos remote sendrecv");
    }
    else if(ICF_RESOURCE_RESERVED == p_sdp->remote_recv.current_status)
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[5],(icf_uint8_t *)"  curr:qos remote recv");
    }
    else if(ICF_RESOURCE_RESERVED == p_sdp->remote_send.current_status)
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[5],(icf_uint8_t *)"  curr:qos remote send");
    }
    else
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[5],(icf_uint8_t *)"  curr:qos remote none");
    }
        
    /* Preparing current line for local end */
    if ((ICF_RESOURCE_RESERVED == p_sdp->local_send.current_status) &&
            (ICF_RESOURCE_RESERVED == p_sdp->local_recv.current_status))
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[6],(icf_uint8_t *)"  curr:qos local sendrecv");
    }
    else if(ICF_RESOURCE_RESERVED == p_sdp->local_recv.current_status)
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[6],(icf_uint8_t *)"  curr:qos local recv");
    }
    else if(ICF_RESOURCE_RESERVED == p_sdp->local_send.current_status)
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[6],(icf_uint8_t *)"  curr:qos local send");
    }
    else
    {
        icf_port_strcpy((icf_uint8_t *)pQosStatusStrings[6],(icf_uint8_t *)"  curr:qos local none");
    }
    ret_val = ICF_SUCCESS;
    return ret_val;
}


/*******************************************************************************
 *
 * FUNCTION:        icf_ssa_init_remote_sdp_with_default_qos_values
 *
 * DESCRIPTION:     This function would initialize the Qos parameters present
 *                  in remote SDP structure with default Qos preconditions.
 *
 ******************************************************************************/
icf_return_t icf_ssa_init_remote_sdp_with_default_qos_values(
        INOUT  icf_internal_sdp_stream_st *p_remote_received_sdp)
{
    icf_return_t ret_val = ICF_SUCCESS;
    
    /* Initializing Current Status */
    p_remote_received_sdp->local_send.current_status = (icf_uint8_t)
        ICF_RESOURCE_NOT_RESERVED;
    p_remote_received_sdp->local_recv.current_status = (icf_uint8_t)
        ICF_RESOURCE_NOT_RESERVED;
    p_remote_received_sdp->remote_send.current_status = (icf_uint8_t)
        ICF_RESOURCE_NOT_RESERVED;
    p_remote_received_sdp->remote_recv.current_status = (icf_uint8_t)
        ICF_RESOURCE_NOT_RESERVED;

    /* Initializing Desired Status */
    p_remote_received_sdp->local_send.desired_status = (icf_uint8_t)
        ICF_RESOURCE_RESERVATION_MANDATORY;
    p_remote_received_sdp->local_recv.desired_status = (icf_uint8_t)
        ICF_RESOURCE_RESERVATION_MANDATORY;
    p_remote_received_sdp->remote_send.desired_status = (icf_uint8_t)
        ICF_RESOURCE_RESERVATION_NONE;
    p_remote_received_sdp->remote_recv.desired_status = (icf_uint8_t)
        ICF_RESOURCE_RESERVATION_NONE;

    /* Initializing Confirmation Status */
    p_remote_received_sdp->remote_send.confirmation_status = (icf_uint8_t)
        ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
    p_remote_received_sdp->remote_recv.confirmation_status = (icf_uint8_t)
        ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;

    return ret_val;
}


/*****************************************************************************
 ** FUNCTION:        icf_ssa_prepare_sdp_from_qos_status_lines
 ** 
 ** DESCRIPTION:    This function prepares SDP structure from Current, Desired
 **                     and Confirm status lines for Qos Support.
 **
 *****************************************************************************/
icf_return_t icf_ssa_prepare_sdp_from_qos_status_lines(
        INOUT Sdf_ty_s8bit  pQosStatusStrings[][ICF_MAX_LENGTH_OF_QOS_LINE],
        INOUT  icf_internal_sdp_stream_st *p_sdp,
		INOUT icf_ssa_pdb_st   *p_ssa_pdb)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint16_t     QosStringsIterator = 0;

    for (QosStringsIterator = 0; QosStringsIterator < ICF_MAX_NUM_QOS_LINES;
            QosStringsIterator++)
    {
        if (0 != icf_port_strcasecmp(
                    pQosStatusStrings[QosStringsIterator],""))
        {
            /* Filling Current Status in SDP */
            if (ICF_NULL != Sdf_mc_strstr(
                        pQosStatusStrings[QosStringsIterator],
                        "curr:qos"))
            {
                if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "local"))
                {
                    if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "none"))
                    {
                        p_sdp->local_send.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                        p_sdp->local_recv.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "sendrecv"))
                    {
                        p_sdp->local_send.current_status = 
                            ICF_RESOURCE_RESERVED;
                        p_sdp->local_recv.current_status = 
                            ICF_RESOURCE_RESERVED;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "send"))
                    {
                        p_sdp->local_send.current_status = 
                            ICF_RESOURCE_RESERVED;
                        p_sdp->local_recv.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "recv"))
                    {
                        p_sdp->local_send.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                        p_sdp->local_recv.current_status = 
                            ICF_RESOURCE_RESERVED;
                    }
                    else
                    {
                        ret_val = ICF_FAILURE;
                        break;
                    }
                    
                }
                
                else if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "remote"))
                {
                    if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "none"))
                    {
                        p_sdp->remote_send.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                        p_sdp->remote_recv.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "sendrecv"))
                    {
                        p_sdp->remote_send.current_status = 
                            ICF_RESOURCE_RESERVED;
                        p_sdp->remote_recv.current_status = 
                            ICF_RESOURCE_RESERVED;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "send"))
                    {
                        p_sdp->remote_send.current_status = 
                            ICF_RESOURCE_RESERVED;
                        p_sdp->remote_recv.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "recv"))
                    {
                        p_sdp->remote_send.current_status = 
                            ICF_RESOURCE_NOT_RESERVED;
                        p_sdp->remote_recv.current_status = 
                            ICF_RESOURCE_RESERVED;
                    }
                    else
                    {
                        ret_val = ICF_FAILURE;
                        break;
                    }                    
                }
                else
                {
                    ret_val = ICF_FAILURE;
                    break;
                }
             
            }
            /* Filling Desired Status in SDP */
            else if (ICF_NULL != Sdf_mc_strstr(
                        pQosStatusStrings[QosStringsIterator],
                        "des:qos"))
            {
                if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "local"))
                {
                    if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "mandatory"))
                    {
                        if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "sendrecv"))
                        {
                            p_sdp->local_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                            p_sdp->local_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "send"))
                        {
                            p_sdp->local_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "recv"))
                        {
                            p_sdp->local_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "optional"))
                    {
                        if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "sendrecv"))
                        {
                            p_sdp->local_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                            p_sdp->local_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "send"))
                        {
                            p_sdp->local_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "recv"))
                        {
                            p_sdp->local_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }                        
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "none"))
                    {
                        if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "sendrecv"))
                        {
                            p_sdp->local_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                            p_sdp->local_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "send"))
                        {
                            p_sdp->local_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "recv"))
                        {
                            p_sdp->local_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }                        
                    }
                    else
                    {
                        /* We will not support unknown strength type
                         * and return failure
                         */
                        ret_val = ICF_FAILURE;
                        break;
                    }
                }
                else if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "remote"))
                {
                    if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "mandatory"))
                    {
                        if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "sendrecv"))
                        {
                            p_sdp->remote_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                            p_sdp->remote_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "send"))
                        {
                            p_sdp->remote_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "recv"))
                        {
                            p_sdp->remote_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_MANDATORY;
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "optional"))
                    {
                        if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "sendrecv"))
                        {
                            p_sdp->remote_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                            p_sdp->remote_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "send"))
                        {
                            p_sdp->remote_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "recv"))
                        {
                            p_sdp->remote_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_OPTIONAL;
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }                        
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "none"))
                    {
                        if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "sendrecv"))
                        {
                            p_sdp->remote_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                            p_sdp->remote_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "send"))
                        {
                            p_sdp->remote_send.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                        }
                        else if (ICF_NULL != Sdf_mc_strstr(
                                    pQosStatusStrings[QosStringsIterator],
                                    "recv"))
                        {
                            p_sdp->remote_recv.desired_status = 
                                ICF_RESOURCE_RESERVATION_NONE;
                        }
                        else
                        {
                            ret_val = ICF_FAILURE;
                            break;
                        }                        
                    }
                    else
                    {
                        /* We will not support unknown strength type
                         * and failure
                         */
                        ret_val = ICF_FAILURE;
                        break;
                    }
                }
                else
                {
					/* unsupported QoS status_type received - e2e
					 * set bitmask in call_ctxt indicating 580 to be sent */
					p_ssa_pdb->p_ssa_ctxt->failure_bitmask |= 
						ICF_SSA_FAILURE_RESP_CODE_580;
					ret_val = ICF_FAILURE;
                        break;
                }
            }   
            /* Filling Confirm Status in SDP */
            else if (ICF_NULL != Sdf_mc_strstr(
                        pQosStatusStrings[QosStringsIterator],
                        "conf:qos"))
            {
                if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "local"))
                {
                    /* conf:qos is never true for local as no confirmation(update) 
							is required from self(local) */
                }
                
                else if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "remote"))
                {
                    if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "none"))
                    {
                        p_sdp->remote_send.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
                        p_sdp->remote_recv.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "sendrecv"))
                    {
                        p_sdp->remote_send.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_REQ;
                        p_sdp->remote_recv.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_REQ;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "send"))
                    {
                        p_sdp->remote_send.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_REQ;
                        p_sdp->remote_recv.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
                    }
                    else if (ICF_NULL != Sdf_mc_strstr(
                                pQosStatusStrings[QosStringsIterator],
                                "recv"))
                    {
                        p_sdp->remote_send.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_NOT_REQ;
                        p_sdp->remote_recv.confirmation_status = 
                            ICF_RESOURCE_RESERVATION_CONF_REQ;
                    }
                    else
                    {
                        ret_val = ICF_FAILURE;
                        break;
                    }                    
                }
                else
                {
                    ret_val = ICF_FAILURE;
                    break;
                }
            }
            else
            {
                /* This means that some garbage value is sent in SDP message */
                ret_val = ICF_FAILURE;
                break;
            }   
        }
    }
    return ret_val;
}

/*****************************************************************************
 ** FUNCTION:        icf_ssa_validate_qos_status_lines
 ** 
 ** DESCRIPTION:    .This function validates that all the mandatory qos
 **					lines have been recieved in SDP
 **
 *****************************************************************************/
icf_return_t icf_ssa_validate_qos_status_lines
(
        INOUT icf_ssa_pdb_st    *p_ssa_pdb,
        INOUT Sdf_ty_s8bit      pQosStatusStrings[][ICF_MAX_LENGTH_OF_QOS_LINE]
)
{
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint16_t     QosStringsIterator = ICF_NULL;
	icf_boolean_t    is_conf = ICF_FALSE,
                     curr_loc = ICF_FALSE,
                     curr_rem = ICF_FALSE,
                     des_loc = ICF_FALSE,
                     conf_rem = ICF_FALSE;
     
	p_ssa_pdb = p_ssa_pdb;

    for (QosStringsIterator = 0; QosStringsIterator < ICF_MAX_NUM_QOS_LINES;
            QosStringsIterator++)
    {
        if (0 != icf_port_strcasecmp(
                    pQosStatusStrings[QosStringsIterator],""))
        {
            /* Validating Current Status in SDP */
            if (ICF_NULL != Sdf_mc_strstr(
                        pQosStatusStrings[QosStringsIterator],
                        "curr:qos"))
            {
                if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "local"))
                {
                    curr_loc = ICF_TRUE;
                }
                
                else if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "remote"))
                {
                    curr_rem = ICF_TRUE;
				}
            }
            /* validating Desired Status in SDP */
            else if (ICF_NULL != Sdf_mc_strstr(
                        pQosStatusStrings[QosStringsIterator],
                        "des:qos"))
            {
                if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "local"))
                {
                    des_loc = ICF_TRUE;
                }
                else if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "remote"))
                {
                    /*It is not mandatory that peer sends "desired remote" status*/
                }
            }   
            /* Validating Confirm Status in SDP */
            else if (ICF_NULL != Sdf_mc_strstr(
                        pQosStatusStrings[QosStringsIterator],
                        "conf:qos"))
            {
                if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "local"))
                {
                    /* conf:qos is never true for local as no confirmation(update) 
							is required from self(local) */
                }
                
                else if (ICF_NULL != Sdf_mc_strstr(
                            pQosStatusStrings[QosStringsIterator],
                            "remote"))
                {
					is_conf = ICF_TRUE;
                    conf_rem = ICF_TRUE;
				}

            }
 
        }
    }

    if(ICF_TRUE == is_conf)
	{
        if(conf_rem && curr_loc && curr_rem && des_loc)
		{
		    ret_val = ICF_SUCCESS;
		}
	    else
		{
		    ret_val = ICF_FAILURE;
		}
	}
	else
	{
        if(curr_loc && curr_rem && des_loc)
		{
		    ret_val = ICF_SUCCESS;
		}
	    else
		{
		    ret_val = ICF_FAILURE;
		}	
	}

    return ret_val;
}

#endif


