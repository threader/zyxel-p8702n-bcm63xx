/*************************************************************************
 *
 *     FILENAME           :  icf_port_trace_error.c
 *
 *     DESCRIPTION        :
 *
 *     Revision History   :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         07/01/05    Aashish Suchdev                        Initial
 *         10/01/05    Aashish Suchdev                   Updated for
 *                                                       module specific
 *                                                       error/trace printing 
 *
 *     Copyright 2006, Aricent.
 *
 *************************************************************************/
#ifdef ICF_PORT_SOLARIS
#include <stdio.h>
#endif

#ifdef ICF_PORT_LINUX
#include <stdarg.h>
#endif

#include "icf_common_types.h"
#include "icf_port_prototypes.h"
#include "icf_port_defs.h"
#include "icf_es_prototypes.h"
#include "icf_port_types.h"
#include "icf_port_prototypes.h"
#include "icf_feature_flags.h"

#ifdef ICF_TRACE_ENABLE
	extern FILE			*p_log_file_ptr;
	icf_uint32_t	endIndex = 0;
    extern icf_glb_pdb_st     *p_persistent_glb_pdb;

#endif

extern icf_return_t icf_port_make_n_send_log_to_app(char *log_buffer,\
						    icf_void_st* p_routing_info);
extern icf_void_t icf_port_rewind(FILE *fptr);

#ifdef ICF_ERROR_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_error
*
*  DESCRIPTION    :    
*
*  RETURNS        :     
*
 *************************************************************************/

icf_void_t icf_port_log_error(
        INOUT   icf_void_t       *p_glb_port_info,
        IN icf_uint8_t           *p_error_string, 
        IN icf_error_t           error_id,
        IN icf_int_module_id_t   module_id,
        IN icf_error_data_st     *p_error_data)
{
    icf_uint8_t           *p_lcl_error_string;
    icf_error_t           lcl_error_id;
    icf_int_module_id_t   lcl_module_id;
    icf_error_data_st     *p_lcl_error_data;

#ifndef ICF_SM_LOG_ENABLE
    ICF_PRINT(((icf_uint8_t *)"\nERROR for Module: %d",module_id));
    ICF_PRINT(((icf_uint8_t *)"\nERROR ID:%d",error_id));
    if(ICF_NULL != p_error_string)
    {
        ICF_PRINT(((icf_uint8_t *)"\nERROR STRING: %s",p_error_string));
    }
    
 if(ICF_NULL != p_error_data)
 {
                  
    /*
     * Module Authors to add their module id hash defines and corresponding
     * calls to the functions to print the contents of error data
     * icf_port_log_CC_error(error_id, p_error_data)
     */
    switch (module_id){
        case ICF_INT_MODULE_MRM:
/*            {
                icf_port_log_mrm_error(error_id, p_error_data);
            }
*/
            break;
        case ICF_INT_MODULE_CM:
            {
                icf_port_log_cm_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_CC:
            {
                icf_port_log_cc_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_DBM:
            {
                icf_port_log_dbm_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_SM:
            {
                icf_port_log_sm_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_REGM:
            {
                icf_port_log_regm_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_CFG:
            {
                icf_port_log_cfg_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_SSA:
            {
                icf_port_log_ssa_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_MMI:
            {
                icf_port_log_mmi_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_SCM:
            {
                icf_port_log_scm_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_ADM:
            {
                icf_port_log_adm_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_CHR:
            {
                icf_port_log_chr_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_RNW:
            {
                icf_port_log_rfn_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_TWC:
            {
                icf_port_log_twc_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_CTA:
            {
                icf_port_log_cta_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_CTU:
            {
                icf_port_log_ctua_error(error_id, p_error_data);
            }
            break;
        case ICF_INT_MODULE_CFW:
/*            {
                icf_port_log_cfw_error(error_id, p_error_data);
            }
*/
            break;
        default:
            break;
    }
  }
#endif
    /*
     * Functionality to be added to send the error information
     * to SM
     */
    p_glb_port_info = p_glb_port_info;
    p_lcl_error_string  = (icf_uint8_t*)p_error_string;
    lcl_error_id        = error_id;
    lcl_module_id       = module_id;
    p_lcl_error_data    = (icf_error_data_st*)p_error_data;

}
#endif

#ifdef ICF_TRACE_ENABLE
/*************************************************************************
*
*  FUNCTION NAME  :   icf_port_log_trace
*
*  DESCRIPTION    :    
*
*  RETURNS        :     
*
 *************************************************************************/

icf_void_t icf_port_log_trace(
        INOUT   icf_void_t       *p_glb_port_info,
        IN icf_uint8_t       *p_trace_string, 
        IN icf_trace_id_t    trace_id,
        IN icf_int_module_id_t   module_id,
        IN icf_trace_data_st *p_trace_data)
{

    icf_uint8_t           *p_lcl_trace_string;
    icf_trace_id_t           lcl_trace_id;
    icf_int_module_id_t   lcl_module_id;
    icf_trace_data_st     *p_lcl_trace_data;

#ifndef ICF_SM_LOG_ENABLE

   if(ICF_TRACE_FUNCTION_ENTRY == trace_id)
   {
#ifdef ICF_DETAILED_DEBUG_TRACE
       ICF_PRINT(((icf_uint8_t *)"\nICF:%s",p_trace_string));

#ifdef ICF_PORT_WINDOWS
		ICF_PRINT(((icf_uint8_t *)"\n>> %s",p_trace_data->string_val[0].str));
		ICF_PRINT(((icf_uint8_t *)"\n>> File = %s",p_trace_data->string_val[1].str));
		ICF_PRINT(((icf_uint8_t *)"\n>> Line = %u\n\n",p_trace_data->int_val[0]));
#else
		ICF_PRINT((" %s",p_trace_data->string_val[0].str));
#endif
#endif
		return ;
   }
   else if(ICF_TRACE_FUNCTION_EXIT == trace_id) 
   {
#ifdef ICF_DETAILED_DEBUG_TRACE

        ICF_PRINT(((icf_uint8_t *)"\nICF:%s",p_trace_string));

#ifdef ICF_PORT_WINDOWS
		ICF_PRINT(((icf_uint8_t *)"\n<< %s, with retval %d",p_trace_data->string_val[0].str,
                         p_trace_data->int_val[0]));
		ICF_PRINT(((icf_uint8_t *)"\n<< File = %s",p_trace_data->string_val[1].str));
		ICF_PRINT(((icf_uint8_t *)"\n<< Line = %u\n\n",p_trace_data->int_val[1]));
#else
		ICF_PRINT(((icf_uint8_t *)" %s, with retval %d",p_trace_data->string_val[0].str,
                         p_trace_data->int_val[0]));
#endif
#endif
		return ;
   }
   
    /*
     * Module Authors to add their module id hash defines and corresponding
     * calls to the functions to print the contents of trace data
     * icf_port_log_CC_trace(trace_id, p_trace_data)
     * The implementation of the above mentioned routine is to be done
     * by Module authors in respective files indicating the module for
     * which traces/errors are being covered.  The print need not be 
     * necessarily in format, it can be a default printing for all.
     */
    switch (module_id){
        case ICF_INT_MODULE_MRM:
/*            {
                icf_port_log_mrm_trace(trace_id, p_trace_data);
            }
*/
            break;
        case ICF_INT_MODULE_CM:
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CM]: "));
                icf_port_log_cm_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_CC:
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CC]: "));
                icf_port_log_cc_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_DBM:
            {
                icf_port_log_dbm_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_SM:
            {
                icf_port_log_sm_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_REGM:
            {
                ICF_PRINT(((icf_uint8_t *)"\n[REGM]: "));
                icf_port_log_regm_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_CFG:
            {
                ICF_PRINT(((icf_uint8_t *)"\n[CFG]: "));
                icf_port_log_cfg_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_SSA:
            {
                icf_port_log_ssa_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_MMI:
            {
                ICF_PRINT(((icf_uint8_t *)"\n[MMI]: "));
                icf_port_log_mmi_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_SCM:
            {
                icf_port_log_scm_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_ADM:
            {
                icf_port_log_adm_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_CHR:
            {
                icf_port_log_chr_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_RNW:
            {
                icf_port_log_rfn_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_TWC:
            {
                icf_port_log_twc_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_CTA:
            {
                icf_port_log_cta_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_CTU:
            {
                icf_port_log_ctua_trace(trace_id, p_trace_data);
            }
            break;
        case ICF_INT_MODULE_CFW:
/*            {
                icf_port_log_cfw_trace(trace_id, p_trace_data);
            }
*/
            break;
        default:
            break;
    }
#endif
    /*
     * Functionality to be added to send the error information
     * to SM
     */
    p_glb_port_info = p_glb_port_info;
    p_lcl_trace_string  = (icf_uint8_t*)p_trace_string;
    lcl_trace_id        = trace_id;
    lcl_module_id       = module_id;
    p_lcl_trace_data    = (icf_trace_data_st*)p_trace_data;

}


icf_void_t icf_port_rewind(FILE *fptr)
{
	long currentPos;
	long available;
	long index;

	fprintf(fptr,"\n[GOTO TOP OF THE FILE]");
	currentPos = ftell(fptr);
	fseek(fptr,0L,SEEK_END);
	available = ftell(fptr);
	fseek(fptr,currentPos,SEEK_SET);
	for( index = currentPos;index < available; index++)
	{
		fprintf(fptr," ");
	}
	fseek(fptr,0L,SEEK_SET);
	fprintf(fptr,"[CONTINUED FROM THE LAST]\n");
}	


#ifdef ICF_PORT_WINDOWS
icf_void_t icf_port_printf(char* fmt, ...)
{

	FILE *fptr = ICF_NULL;

	va_list ap;

	
	char *p, *sval;
	int ival;
    char cval;
	float fval;
	double dval;
	long	available;
	char tmpBuffer[15];
	int tmpBufferLength = 0;
	long dMaxLogFileSize;

	fptr = p_log_file_ptr;
    if (!fptr) {
        fprintf(stderr, "\nFATAL: NULL File pointer to IMS_PRINT()\n");
        return;
    }
	fseek(fptr,endIndex,SEEK_SET);
#ifdef ICF_PORT_WINDOWS
	dMaxLogFileSize = icf_es_get_max_log_file_size();
#else
	dMaxLogFileSize = 150 * 1024;
#endif
    available = dMaxLogFileSize - ftell(fptr);
    va_start(ap, fmt);

	for (p = fmt; *p; p++) {
		if( available <= 0)
		{
			icf_port_rewind(fptr);
			available = dMaxLogFileSize;
		}
		if (*p != '%') {
			fputc(*p, fptr);
			available--;
			continue;
		}
			
		switch (*++p) {
		case 'c':
			cval = (char)va_arg(ap, int);
			fprintf(fptr, "%c", cval);
            available--;
			break;
			
		case 'd':
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%d",ival);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%d", ival);
			available = available - tmpBufferLength;
			break;

		case 'l': /*lu */
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%lu",ival);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%lu", ival);
			available = available - tmpBufferLength;
			p++;
            
			break;
			
		case 'x':
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%x",ival);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%x", ival);
			available = available - tmpBufferLength;
			break;
		case '0':  /*02x*/
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%02x",ival);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%02x", ival);
			available = available - tmpBufferLength;
			p+=2;
            
			break;
			
		case 'X':
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%X",ival);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%X", ival);
			available = available - tmpBufferLength;
            
			break;
			
		case 'f':
			fval = (float)va_arg(ap, double);
			icf_port_sprintf(tmpBuffer,"%f",fval);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%f", fval);
			available = available - tmpBufferLength;
            
			break;
			
		case 'g':
			dval = va_arg(ap, double);
			icf_port_sprintf(tmpBuffer,"%g",dval);
			tmpBufferLength = icf_port_strlen((unsigned char*)tmpBuffer);
			if ( tmpBufferLength > available)
			{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
			}
			fprintf(fptr, "%g", dval);
			available = available - tmpBufferLength;
            
			break;
			
		case 's':
			for (sval = va_arg(ap, char*); *sval; sval++) {
				fputc(*sval, fptr);
				available--;
				if( available <= 0)
				{
				icf_port_rewind(fptr);
				available = dMaxLogFileSize;
				}
            }
			break;

		default:
			fputc(*p, fptr);
			available--;
			break;
			
		}
	}
	endIndex = ftell(fptr);
	fprintf(fptr,"\n********************ICF LOG FILE END**********************\n");
	va_end(ap);
	fflush(fptr);
}
#endif

icf_return_t icf_port_make_n_send_log_to_app(char *log_buffer,icf_void_st* p_routing_info)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_msg_st                       *p_pa_msg = ICF_NULL;
    icf_uint32_t                     msg_len = 0;    
    icf_logs_ind_st                  *p_log_ind = ICF_NULL;
	icf_config_data_st               *p_cfg_data = ICF_NULL;
    icf_error_t                      ecode = ICF_ERROR_NONE;

    p_cfg_data = (icf_config_data_st *)(p_persistent_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]);
    if (ICF_NULL == p_cfg_data)
    {
        return ICF_FAILURE;
    }

    /*Calculate the message length*/
    msg_len = sizeof(icf_api_header_st) + sizeof(icf_logs_ind_st);
    
    p_pa_msg = (icf_msg_st *)icf_port_memget(
                                p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
                                msg_len,
                                ICF_MEM_COMMON,
                                &ecode);

    if (ICF_NULL != p_pa_msg)
    {
        /* Fill the message header for ICF_LOGS_IND */
        
        p_pa_msg->hdr.api_id = ICF_LOGS_IND;
        p_pa_msg->hdr.api_length = (icf_uint16_t )msg_len;
        p_pa_msg->hdr.call_id = ICF_INVALID_CALL_ID;
        p_pa_msg->hdr.app_id = ICF_GENERIC_APP_ID;
        p_pa_msg->hdr.source_id = ICF_MODULE_ICF;
        p_pa_msg->hdr.version = ICF_VERSION_1_0;
        p_pa_msg->hdr.destination_id = ICF_GENERIC_APP_MODULE_ID;
    }
    
    /*Fill message payload*/
    p_log_ind = (icf_logs_ind_st*)&(p_pa_msg->payload[0]);
   
    if(ICF_NULL != p_log_ind)
    { 
    p_log_ind->trace_string.str_len = (icf_uint16_t)icf_port_strlen((icf_uint8_t *)log_buffer);
    icf_port_strcpy((icf_uint8_t *)p_log_ind->trace_string.str, (icf_uint8_t *)log_buffer);
    }
    
    ret_val = icf_send_to_app_for_logs(p_persistent_glb_pdb, p_pa_msg,
				                    p_routing_info, &ecode);


    icf_port_memfree(
        p_persistent_glb_pdb->p_glb_data->p_glb_port_info,
        p_pa_msg,
        ICF_MEM_COMMON,
        &ecode);


    return ret_val;
}

#if defined ICF_PORT_WINCE

icf_void_t icf_port_logs_ind(char* fmt, ...)
{

	FILE *fptr = ICF_NULL;

	va_list ap;

	
	char    *p, *sval;
	int     ival;
    char    cval;
	float   fval;
	double  dval;
	char    tmpBuffer[15];
	int     tmpBufferLength = 0;
    char    log_buffer[ICF_MAX_LONG_STR_LEN] = "\0";
    int     index_log_buf = 0;

    va_start(ap, fmt);

	for (p = fmt; *p; p++) 
    {
        if (*p != '%') 
        {
			log_buffer[index_log_buf] = *p;
            index_log_buf++;
            log_buffer[index_log_buf] = ICF_NULL;
            continue;
		}
			
		switch (*++p) 
        {
		case 'c':
			cval = (char)va_arg(ap, int);
            log_buffer[index_log_buf] = *p;
            index_log_buf++;
            p++;
            log_buffer[index_log_buf] = ICF_NULL;
			break;
			
		case 'd':
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%d",ival);
            icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			break;

		case 'l': /*lu */
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%lu",ival);
            icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			p++;
			break;
			
		case 'x':
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%lu",ival);
            icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			break;

        case '0':  /*02x*/
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%02x",ival);
			icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			p+=2;
            break;
			
		case 'X':
			ival = va_arg(ap, int);
			icf_port_sprintf(tmpBuffer,"%X",ival);
			icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			break;
			
		case 'f':
			fval = (float)va_arg(ap, double);
			icf_port_sprintf(tmpBuffer,"%f",fval);
			icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			break;
			
		case 'g':
			dval = va_arg(ap, double);
			icf_port_sprintf(tmpBuffer,"%g",dval);
			icf_port_strcat(log_buffer, tmpBuffer);
            index_log_buf  = index_log_buf + (icf_port_strlen((unsigned char*)tmpBuffer));
			break;
			
		case 's':
			for (sval = va_arg(ap, char*); *sval; sval++) 
            {
				log_buffer[index_log_buf] = *sval;
                index_log_buf++;
            }
			break;

		default:
			log_buffer[index_log_buf] = *p;
            index_log_buf++;
            break;
			
		}
	}
	
    if (ICF_NULL == p_persistent_glb_pdb)
    {
        return;
    }
    else
    {
        icf_config_data_st              *p_config_data = ICF_NULL;
        p_config_data = (icf_config_data_st *)(
                            p_persistent_glb_pdb->p_glb_data->p_module_data[ICF_INT_MODULE_CFG]);
        
      if ((ICF_NULL == p_config_data) || (ICF_NULL == p_config_data->p_generic_routing_info))
      {
            return;
      }
      else
      {        
        if ((p_persistent_glb_pdb->logs_indication == 1) ||
            (p_persistent_glb_pdb->logs_indication == 3))
        {
           /* 2 -> logging enabled in file AND 
              3 -> logging enabled in file and via application API */
        
           fptr = p_log_file_ptr;

            if (!fptr) 
            {
                fprintf(stderr, "\nFATAL: NULL File pointer to ICF_PRINT()\n");
                return;
            }
            endIndex = ftell(fptr);
            fseek(fptr,endIndex,SEEK_SET);
            fprintf(fptr, "%s", log_buffer);
            endIndex = ftell(fptr);
        }
        else if ((p_persistent_glb_pdb->logs_indication == 2) ||
            (p_persistent_glb_pdb->logs_indication == 3))
        {
         /* 3 -> logging enabled through Application API AND 
            4 -> logging enabled through Application API and to file also */
        
            icf_port_make_n_send_log_to_app(log_buffer,
                p_config_data->p_generic_routing_info);
        }
        else
        {
            /* No logging enabled*/
        }
      }/* if ((ICF_NULL == p_config_data) ||... */

    } /* if (p_persistent_glb_pdb) */    
}

#endif



#endif

icf_void_t icf_port_dummy()
{
    /*
     * This routine is here so as to avoid compialtion warnings
     * when both the error and trace flags are switched off. In that 
     * case this function will not let the file remain empty
     */
}
