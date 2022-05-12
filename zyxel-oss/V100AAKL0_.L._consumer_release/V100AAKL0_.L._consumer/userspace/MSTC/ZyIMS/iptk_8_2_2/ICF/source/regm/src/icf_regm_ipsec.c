/*------------------------------------------------------------------------------
 *
 * File name        : icf_regm_ipsec.c
 *
 * Purpose          : This file contains functions for handling IP security. The
 *                    entire contents of this file will be under IPSec flag.
 *
 * Revision History :
 *
 * Date         Name            Ref#                Comments
 * --------     ------------    ----                ---------------
 * 07-Dec-2005  Aman Arora      ICF Rel 4.0 CDD     Initial
 * 07-Dec-2005  Rohit Aggarwal  ICF Rel 4.0 CDD     Coding for IPSec support
 *
 * Copyright (c) 2006, Aricent.
 *----------------------------------------------------------------------------*/


#include "icf_internal_api_id.h"
#include "icf_dbm_prototypes.h"
#include "icf_dbm_regm_prototypes.h"
#include "icf_regm_macro.h"
#include "icf_regm_prototypes.h"

#ifdef ICF_IPSEC_ENABLED


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_sa_clean_up
 *
 * DESCRIPTION:     This is a utility function that sends deletion request to NM
 *                  for all existing SAs, starts response timer and changes 
 *                  state to the one passed as argument. The calling function 
 *                  needs to set the sa_identifier and next_state correctly 
 *                  before invoking this function.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_sa_clean_up(
        INOUT   icf_crm_pdb_st		*p_crm_pdb,
        IN	    icf_uint8_t		    next_state)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Deallocate the security client parameters */
    if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params)
    {
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)
    }
    
    /* Delete any existing SAs */
    if ((ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context) ||
        (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context))
    {
        /* send SA delete request to NM */
        if (ICF_FAILURE == icf_rgm_ipsec_nm_sa_delete_req(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* start timer for awaiting SA delete response */
        else if (ICF_FAILURE == icf_rgm_start_timer(
                                        p_crm_pdb->p_glb_pdb,
                                        ICF_TIMER_TYPE_RGM_SA_DELETE,
                                        p_crm_pdb->p_rgm_context,
                                        p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Change state to await SA deletion response */
            p_crm_pdb->p_rgm_context->rgm_state = next_state; 
        }
    } /* End if(ICF_NULL != p_sa_context) */


    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_sa_clean_up() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_alloc_sa_context
 *
 * DESCRIPTION:     This is a utility function that will allocate memory for a 
 *                  new SA context structure, initialize its members with 
 *                  default values and return the pointer to calling function.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_alloc_sa_context(
	    INOUT   icf_crm_pdb_st			*p_crm_pdb,
        OUT 	icf_ipsec_sa_context_st  **p_p_sa_context)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ipsec_sa_context_st		    *p_sa_context = ICF_NULL;
    icf_line_data_st			        *p_line_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Allocate memory for the new SA context structure */
    ICF_MEMGET(
        p_crm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_ipsec_sa_context_st),
        ICF_MEM_COMMON,
        (icf_void_t *)p_sa_context,
        ICF_RET_ON_FAILURE,
        p_crm_pdb->p_ecode,
        ret_val)
        
    /* set the integrity algorithm to null */
    p_sa_context->p_integrity_algo = ICF_NULL;
    
    /* set the encryption algorithm to null */
    p_sa_context->p_encryption_algo = ICF_NULL;
    
    /* set the q-value string to null */
    p_sa_context->p_q_value = ICF_NULL;
    
    /* set the local security parameters to null */
    p_sa_context->p_ue_security_params = ICF_NULL;
    
    /* set the peer security parameters to null */
    p_sa_context->p_proxy_security_params = ICF_NULL;
    
    /* set the SA duration to zero */
    p_sa_context->sa_lifetime = ICF_NULL;
    
    /* set the number of active transactions to zero */
    p_sa_context->num_active_trans = ICF_NULL;
    
    /* set the SA active flag to false */
    p_sa_context->is_active = ICF_FALSE;
    
    /* set the proxy address to proxy address configured on line 0 */
    if (ICF_SUCCESS == icf_dbm_fetch_line_cntxt_blk(
                                p_crm_pdb->p_glb_pdb,
                                p_crm_pdb->p_rgm_context->rgm_list_of_lines[0],
                                &p_line_data,
                                p_crm_pdb->p_ecode))
    {
        icf_port_memcpy(
            &(p_sa_context->proxy_ip_addr),
            &(p_line_data->proxy_addr.addr),
            ICF_PORT_SIZEOF(icf_transport_addr_st));
    }
    else
    {
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            p_sa_context,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)
        
        ret_val = ICF_FAILURE;
    }
    
    /* the newly allocated block will be returned to the calling function */
    *p_p_sa_context = p_sa_context;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_alloc_sa_context() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_dealloc_sa_context
 *
 * DESCRIPTION:     This is a utility function that will de-allocate SA context
 *                  identified by the SA identifier in REGM context.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_dealloc_sa_context(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ipsec_sa_context_st		    *p_sa_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: sa_identifier before deallocating= %d\n", 
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier));

    /* get the pointer to SA context that needs to be deleted */
    if ((ICF_RGM_IPSEC_SA_NEW == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier) ||
        (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier))
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;
    }
    else if (ICF_RGM_IPSEC_SA_OLD == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;
    }

    /* check if the received pointer is already null */
    if (ICF_NULL != p_sa_context)
    {
        /* free the pointers inside SA context */
        if (ICF_NULL != p_sa_context->p_integrity_algo)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_integrity_algo,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        if (ICF_NULL != p_sa_context->p_encryption_algo)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_encryption_algo,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        if (ICF_NULL != p_sa_context->p_q_value)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_q_value,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
		if (ICF_NULL != p_sa_context->p_ue_security_params)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_ue_security_params,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
		if (ICF_NULL != p_sa_context->p_proxy_security_params)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_proxy_security_params,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }

        /* finally free the SA context */
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            p_sa_context,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)


    } /* End if(p_sa_context) */
    
    /* For ICF_RGM_IPSEC_SA_ALL, delete the old SA context as well */
    if (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;

        /* check if the received pointer is already null */
        if (ICF_NULL != p_sa_context)
        {
            /* free the pointers inside SA context */
            if (ICF_NULL != p_sa_context->p_integrity_algo)
            {
                ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_sa_context->p_integrity_algo,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)
            }
            if (ICF_NULL != p_sa_context->p_encryption_algo)
            {
                ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_sa_context->p_encryption_algo,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)
            }
            if (ICF_NULL != p_sa_context->p_q_value)
            {
                ICF_MEMFREE(
                    p_crm_pdb->p_glb_pdb,
                    p_sa_context->p_q_value,
                    ICF_MEM_COMMON,
                    p_crm_pdb->p_ecode,
                    ret_val)
            }
			if (ICF_NULL != p_sa_context->p_ue_security_params)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_ue_security_params,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}
			if (ICF_NULL != p_sa_context->p_proxy_security_params)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_proxy_security_params,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}

            /* finally free the SA context */
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
            
        } /* End if(p_sa_context) */

    } /* End if(sa_identifier) */

	if (ICF_RGM_IPSEC_SA_NEW == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context = ICF_NULL;
    }
    else if (ICF_RGM_IPSEC_SA_OLD == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context = ICF_NULL;
    }
	else if (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
		((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context = ICF_NULL;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context = ICF_NULL;
    }

	/* reset the SA identifier */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;

    /* debug traces */
    ICF_PRINT(("\n[REGM]: sa_identifier after deallocating= %d\n", 
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier));

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_dealloc_sa_context() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_get_sec_params
 *
 * DESCRIPTION:     This is a utility function that will allocate memory for a 
 *                  new security parameter structure, initialize its members 
 *                  with default values and return pointer to calling function.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_get_sec_params(
	    INOUT   icf_crm_pdb_st				*p_crm_pdb,
        OUT 	icf_ipsec_security_params_st	**p_p_sec_params)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ipsec_security_params_st	    *p_sec_params = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Allocate memory for the new security parameter structure */
    ICF_MEMGET(
        p_crm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_ipsec_security_params_st),
        ICF_MEM_COMMON,
        (icf_void_t *)p_sec_params,
        ICF_RET_ON_FAILURE,
        p_crm_pdb->p_ecode,
        ret_val)
    
    /* initialize the members to zero */
    p_sec_params->port_s = ICF_NULL;
    p_sec_params->port_c = ICF_NULL;
    p_sec_params->spi_s = ICF_NULL;
    p_sec_params->spi_c = ICF_NULL;
    
    /* set the local protected server port */
    p_sec_params->port_s = 
        p_crm_pdb->p_config_data->p_ipsec_config_data->start_port_val;

    /* set the local protected client port */
    if (ICF_FAILURE == icf_rgm_ipsec_get_unused_port(
                                p_crm_pdb, 
                                &(p_sec_params->port_c)))
    {
        ret_val = ICF_FAILURE;
    }

    /* set the spi for inbound SA at server port */
    else if (ICF_FAILURE == icf_rgm_ipsec_get_unused_spi(
                                p_crm_pdb, 
                                p_sec_params->spi_c,
                                &(p_sec_params->spi_s)))
    {
        ret_val = ICF_FAILURE;
    }

    /* set the spi for inbound SA at client port */
    else if (ICF_FAILURE == icf_rgm_ipsec_get_unused_spi(
                                p_crm_pdb, 
                                p_sec_params->spi_s, 
                                &(p_sec_params->spi_c)))
    {
        ret_val = ICF_FAILURE;
    }

    if (ICF_FAILURE == ret_val)
    {
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            p_sec_params,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)
    }
    else
    {
        /* debug traces */
        ICF_PRINT(("\n[REGM]: New security parameters allocated >>\n"));
        ICF_PRINT(("[REGM]: port_s = %d\n", p_sec_params->port_s));
        ICF_PRINT(("[REGM]: port_c = %d\n", p_sec_params->port_c));
        ICF_PRINT(("[REGM]: spi_s = %ld\n", p_sec_params->spi_s));
        ICF_PRINT(("[REGM]: spi_c = %ld\n", p_sec_params->spi_c));
    }

    /* the newly allocated block will be returned to the calling function */
    *p_p_sec_params = p_sec_params;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_get_sec_params() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_get_unused_port
 *
 * DESCRIPTION:     This is a utility function that will return, by value, a 
 *                  port from the configured range that is not in use by any 
 *                  existing SA. This port will then be used as the local 
 *                  protected client port for new SA creation. Whenever this 
 *                  function is invoked, pointer to old SA context in regm_cntxt
 *                  will always be null but the pointer to new SA context may 
 *                  (initial reg) or may not (re-reg) be null.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_get_unused_port(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb,
        OUT 	icf_uint16_t		    *p_new_port)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
   	icf_uint16_t			            new_port = ICF_NULL;
    icf_uint16_t			            start_port = ICF_NULL;
    icf_uint16_t			            end_port = ICF_NULL;
    icf_ipsec_sa_context_st	        *p_new_sa = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* temporary placeholders */
    start_port = p_crm_pdb->p_config_data->p_ipsec_config_data->start_port_val;
    end_port = p_crm_pdb->p_config_data->p_ipsec_config_data->end_port_val;
    p_new_sa = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;
    
    /* For each port in the configured protected client port range, find the 
     * first available port that is not already in use by any existing SA */
    for (new_port = (start_port + 1); new_port <= end_port; new_port++)
    {
        if (ICF_NULL == p_new_sa)
        {
            *p_new_port = new_port;
            break;
        }
        else if (ICF_NULL == p_new_sa->p_ue_security_params)
        {
            *p_new_port = new_port;
            break;
        }
        else if (new_port != p_new_sa->p_ue_security_params->port_c)
        {
            *p_new_port = new_port;
            break;
        } /* End if-else(p_new_sa) */

    } /* End for(port) */

    if (ICF_NULL == *p_new_port)
    {
        ret_val = ICF_FAILURE;
        /* Raise alarm */
        ICF_ALARM(("\nNo free port available in configured \
            protected port range\n"));
    }

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_get_unused_port() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_get_unused_spi
 *
 * DESCRIPTION:     This is a utility function that will generate a random spi 
 *                  value for a new security association. It will return a value
 *                  that is not equal to used_spi and also not in use in new SA 
 *                  context. Whenever this function is invoked, pointer to old 
 *                  SA context in regm_cntxt will always be null but the pointer
 *                  to new SA ctx may (initial reg) or may not (re-reg) be null.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_get_unused_spi(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb,
        IN 	    icf_uint32_t		    used_spi,
        OUT 	icf_uint32_t		    *p_new_spi)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint32_t			            new_spi = ICF_NULL;
    icf_ipsec_sa_context_st	        *p_new_sa = ICF_NULL;
    icf_uint32_t			            spi_ue_s = ICF_NULL;
    icf_uint32_t			            spi_ue_c = ICF_NULL;
    icf_uint32_t			            spi_proxy_s = ICF_NULL;
    icf_uint32_t			            spi_proxy_c = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* temporary placeholders */
    p_new_sa = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;

    /* collect the list of spi values already in use */
    if (ICF_NULL != p_new_sa)
    {
        if (ICF_NULL != p_new_sa->p_ue_security_params)
        {
            spi_ue_s = p_new_sa->p_ue_security_params->spi_s;
            spi_ue_c = p_new_sa->p_ue_security_params->spi_c;
        }
        if (ICF_NULL != p_new_sa->p_proxy_security_params)
        {
            spi_proxy_s = p_new_sa->p_proxy_security_params->spi_s;
            spi_proxy_c = p_new_sa->p_proxy_security_params->spi_c;
        }
    } /* End if(p_new_sa) */
    
    do
    {
        /* random value between 0 and 4294967295 as per RFC 3329 */
        new_spi = (icf_uint32_t)icf_port_random(
                                    (icf_uint16_t)ICF_RGM_IPSEC_SPI_MIN,
                                    (icf_uint16_t)ICF_RGM_IPSEC_SPI_MAX);

    } while((new_spi == used_spi) ||
            (new_spi == spi_ue_s) ||
            (new_spi == spi_ue_c) ||
            (new_spi == spi_proxy_s) ||
            (new_spi == spi_proxy_c));
    
    *p_new_spi = new_spi;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_get_unused_spi() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_add_sec_agree_tag
 *
 * DESCRIPTION:     This is a utility function that will add tag lists for 
 *                  "sec-agree" in Require and Proxy-Require headers in the 
 *                  application context in REGM context. This function needs to 
 *                  be called only on receiving register request from 
 *                  application, and will be available to SSA for sending all 
 *                  subsequent Register requests.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_add_sec_agree_tag(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint32_t			            *p_count = ICF_NULL;
    icf_header_st			        *p_header = ICF_NULL;
    icf_header_st			        *p_new_header = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* temporary placeholders */
    p_count = &(p_crm_pdb->p_rgm_context->p_app_data_list->header_list.count);
    p_header = (p_crm_pdb->p_rgm_context->p_app_data_list->header_list.hdr);
    
    /* Allocate memory for the node having Require header */
    ICF_MEMGET(
        p_crm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_header_st),
        ICF_MEM_COMMON,
        (icf_void_t *)p_new_header,
        ICF_RET_ON_FAILURE,
        p_crm_pdb->p_ecode,
        ret_val)
        
    /* assign values to the new header */
    icf_port_strcpy(p_new_header->hdr_name.str, "Require");
    p_new_header->hdr_name.str_len = 
        icf_port_strlen(p_new_header->hdr_name.str);
    icf_port_strcpy(p_new_header->hdr_value.str, "sec-agree");
    p_new_header->hdr_value.str_len = 
        icf_port_strlen(p_new_header->hdr_value.str);
    p_new_header->next = ICF_NULL;
    
    /* Allocate memory for the next node having Proxy-Require header */
    ICF_MEMGET(
        p_crm_pdb->p_glb_pdb,
        ICF_PORT_SIZEOF(icf_header_st),
        ICF_MEM_COMMON,
        (icf_void_t *)(p_new_header->next),
        ICF_RET_ON_FAILURE,
        p_crm_pdb->p_ecode,
        ret_val)

    /* assign values to the next new header */
    icf_port_strcpy(p_new_header->next->hdr_name.str, "Proxy-Require");    
    p_new_header->next->hdr_name.str_len = 
        icf_port_strlen(p_new_header->next->hdr_name.str);
    icf_port_strcpy(p_new_header->next->hdr_value.str, "sec-agree");
    p_new_header->next->hdr_value.str_len = 
        icf_port_strlen(p_new_header->next->hdr_value.str);
    p_new_header->next->next = ICF_NULL;
    
    /* Add the new nodes to the end of the existing list */
    p_crm_pdb->p_rgm_context->p_app_data_list->bitmask |= 
        ICF_HDR_LIST_PRESENT;
    
    /* Check if existing list is empty */
    if (ICF_NULL == p_header)
    {
        p_crm_pdb->p_rgm_context->p_app_data_list->header_list.hdr = 
            p_new_header;
    }
    /* existing list is not empty */
    else
    {
        /* traverse to the end of the list */
        while (ICF_NULL != p_header->next)
        {
            p_header = p_header->next;
        }
        p_header->next = p_new_header;
    }
    *p_count += 2;

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_add_sec_agree_tag() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_fetch_algo
 *
 * DESCRIPTION:     This is a utility function for fetching the list of locally 
 *                  supported security algorithms from NM and saving them in 
 *                  REGM global data.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_fetch_algo(
        INOUT   icf_crm_pdb_st		    *p_crm_pdb,
        OUT     icf_ipsec_self_algo_st   **p_p_rgm_algo_list)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* construct the request for NM */
    p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_RGM_NM_SA_FETCH_ALGO;
    p_crm_pdb->p_internal_msg->msg_hdr.payload_length = ICF_NULL;
    p_crm_pdb->p_internal_msg->p_msg_data = ICF_NULL;

    /* send the request to NM */
    if (ICF_FAILURE == icf_nm_process_mesg(p_crm_pdb->p_internal_msg))
    {
        ret_val = ICF_FAILURE;
    }
    else
    {
        /* save the returned pointer in RGM global data */
        *p_p_rgm_algo_list = 
          (icf_ipsec_self_algo_st *)(p_crm_pdb->p_internal_msg->p_msg_data);
    }

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Security Algorithms from NM >>\n"));
    ICF_PRINT(("[REGM]: Encryption algo [1] = %s\n",
        (*p_p_rgm_algo_list)->encryption_algo[0].str));
    ICF_PRINT(("[REGM]: Encryption algo [2] = %s\n",
        (*p_p_rgm_algo_list)->encryption_algo[1].str));
    ICF_PRINT(("[REGM]: Integrity algo [1] = %s\n",
        (*p_p_rgm_algo_list)->integrity_algo[0].str));
    ICF_PRINT(("[REGM]: Integrity algo [2] = %s\n",
        (*p_p_rgm_algo_list)->integrity_algo[1].str));
  
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_nm_sa_fetch_algo() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_create_req
 *
 * DESCRIPTION:     This is a utility function that will make and send request 
 *                  to NM for the creation of all new unidirectional SAs for a 
 *                  secure connection between one IMPI and P-CSCF. It will use 
 *                  the parameters stored in new SA context structure within the
 *                  REGM context for filling the request structure.
 *
 *                  This function will take care of sending as many requests to 
 *                  NM as the number of unidirectional SAs that should be 
 *                  created. Hence, the state machine handler needs to invoke 
 *                  this function only once for SA creation.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_create_req(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t			            ctr = ICF_NULL;
	icf_rgm_nm_sa_create_req_st	    req_pld;
    icf_uint32_t			            spi_arr[ICF_RGM_IPSEC_NUM_SA];
    icf_uint8_t			            dir_arr[ICF_RGM_IPSEC_NUM_SA];
    icf_uint16_t			            port_ue[ICF_RGM_IPSEC_NUM_SA];
    icf_uint16_t			            port_proxy[ICF_RGM_IPSEC_NUM_SA];
    icf_ipsec_sa_context_st	        *p_sa_context = ICF_NULL;
    icf_rgm_glb_data_st              *p_rgm_glb_data = ICF_NULL;
    icf_line_data_st			        *p_line_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Call DBM to get the pointer to the RGM global data in p_buff */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_crm_pdb->p_glb_pdb,
                            (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                            (icf_void_t **)&p_rgm_glb_data,
                            p_crm_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
    /* set the proxy address to proxy address configured on line 0 */
    else if (ICF_FAILURE == icf_dbm_fetch_line_cntxt_blk(
                                p_crm_pdb->p_glb_pdb,
                                p_crm_pdb->p_rgm_context->rgm_list_of_lines[0],
                                &p_line_data,
                                p_crm_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* debug traces */
    ICF_PRINT(("\n[REGM]: sa_identifier = %d\n",
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier));

    /* temp pointer to SA context whose SAs need to be modified */
    /* this check may look redundant at this stage since only new SA will be 
     * created, but this is a generic function */
    if (ICF_RGM_IPSEC_SA_NEW == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_NEW_SA_SPI_ALL;
    }
    else
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* Fill the 4 spi values in spi array */
    spi_arr[0] = p_sa_context->p_ue_security_params->spi_s;
    spi_arr[1] = p_sa_context->p_ue_security_params->spi_c;
    spi_arr[2] = p_sa_context->p_proxy_security_params->spi_s;
    spi_arr[3] = p_sa_context->p_proxy_security_params->spi_c;
    
    /* Fill the 4 SA directions in dir array */
    dir_arr[0] = ICF_SA_DIR_IN;
    dir_arr[1] = ICF_SA_DIR_IN;
    dir_arr[2] = ICF_SA_DIR_OUT;
    dir_arr[3] = ICF_SA_DIR_OUT;

    /* Fill the port values in port arrays */
    port_ue[0] = p_sa_context->p_ue_security_params->port_s;
    port_proxy[0] = p_sa_context->p_proxy_security_params->port_c;

    port_ue[1] = p_sa_context->p_ue_security_params->port_c;
    port_proxy[1] = p_sa_context->p_proxy_security_params->port_s;

    port_ue[2] = p_sa_context->p_ue_security_params->port_c;
    port_proxy[2] = p_sa_context->p_proxy_security_params->port_s;

    port_ue[3] = p_sa_context->p_ue_security_params->port_s;
    port_proxy[3] = p_sa_context->p_proxy_security_params->port_c;

    /* construct and send the create request for each of the four SAs */
    for (ctr = ICF_NULL; ctr < ICF_RGM_IPSEC_NUM_SA; ctr++)
    {
        /* fill the request header */
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_RGM_NM_SA_CREATE_REQ;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
            ICF_PORT_SIZEOF(icf_rgm_nm_sa_create_req_st);

        /* fill the request payload */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&req_pld;
        
        icf_port_memcpy(
            (icf_void_t *)&(req_pld.ue_ip_port.addr),
            (icf_void_t *)&(p_crm_pdb->p_config_data->self_ip_address.addr),
            ICF_PORT_SIZEOF(icf_transport_addr_st));
        req_pld.ue_ip_port.port_num = port_ue[ctr];

        icf_port_memcpy(
            (icf_void_t *)&(req_pld.proxy_ip_port.addr),
            (icf_void_t *)&(p_line_data->proxy_addr.addr),
            ICF_PORT_SIZEOF(icf_transport_addr_st));
        req_pld.proxy_ip_port.port_num = port_proxy[ctr];

        req_pld.spi = spi_arr[ctr];

        /* Duration in Platform API - 
		 * If SA Lifetime Modification is supported, it is in milliseconds,
	     * else this value is (2**32 - 1) seconds */
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
        req_pld.lifetime = p_sa_context->sa_lifetime / 1000;
#else
		req_pld.lifetime = p_sa_context->sa_lifetime;
#endif
        
        req_pld.direction = dir_arr[ctr];

        if (ICF_NULL != p_sa_context->p_encryption_algo)
        {
            icf_port_strcpy(req_pld.enc_algo, p_sa_context->p_encryption_algo);
        }
        else
        {
            icf_port_strcpy(req_pld.enc_algo, 
                p_rgm_glb_data->p_ue_algo_list->encryption_algo[0].str);
        }
        
        icf_port_strcpy(req_pld.auth_algo, p_sa_context->p_integrity_algo);

        icf_port_memcpy(
            (icf_void_t *)&(req_pld.enc_key),
            (icf_void_t *)&(p_crm_pdb->p_rgm_context->enc_key),
            ICF_PORT_SIZEOF(icf_string_st));

        icf_port_memcpy(
            (icf_void_t *)&(req_pld.auth_passwd),
            (icf_void_t *)&(p_crm_pdb->p_rgm_context->auth_passwd),
            ICF_PORT_SIZEOF(icf_string_st));
        
        req_pld.spi = spi_arr[ctr];
        
        /* debug traces */
        ICF_PRINT(("\n[REGM]: Create SA Request to NM #%d >>\n", (ctr+1)));
        ICF_PRINT(("[REGM]: UE port = %d\n", req_pld.ue_ip_port.port_num));
        ICF_PRINT(("[REGM]: P-CSCF port = %d\n", 
            req_pld.proxy_ip_port.port_num));
        ICF_PRINT(("[REGM]: Spi = %ld\n", req_pld.spi));
        ICF_PRINT(("[REGM]: Lifetime = %lu seconds\n", req_pld.lifetime));
        ICF_PRINT(("[REGM]: Direction = %d\n", req_pld.direction));
        ICF_PRINT(("[REGM]: Enc algo = %s\n", req_pld.enc_algo));
        ICF_PRINT(("[REGM]: Auth algo = %s\n", req_pld.auth_algo));

        /* send the request to NM */
        if (ICF_FAILURE == icf_nm_process_mesg(p_crm_pdb->p_internal_msg))
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
            ret_val = ICF_FAILURE;
            break;
        }
        else
        {
            p_rgm_glb_data->p_sa_resp_awaiting = p_crm_pdb->p_rgm_context;
        }
    } /* End for(ctr) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_nm_sa_create_req() */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_modify_req
 *
 * DESCRIPTION:     This is a utility function that will make and send request 
 *                  to NM for the modification of all unidirectional SAs for a 
 *                  secure connection between one IMPI and P-CSCF. It will use 
 *                  the parameters stored in new SA context structure within the
 *                  REGM context and the new duration passed as argument for 
 *                  filling the request structure.
 *
 *                  This function will take care of sending as many requests to 
 *                  NM as the number of unidirectional SAs that should be 
 *                  modified. Hence, the state machine handler needs to invoke 
 *                  this function only once for SA modification.
 *
 *                  It will be invoked only if the underlying platform supports 
 *                  SA lifetime modification and hence shall be under 
 *                  ICF_SA_LIFETIME_MOD_SUPPORT compilation flag.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_modify_req(
	    INOUT   icf_crm_pdb_st		*p_crm_pdb,
        OUT	    icf_duration_t		new_lifetime)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t			            ctr = ICF_NULL;
	icf_rgm_nm_sa_modify_req_st      req_pld;
    icf_uint32_t			            spi_arr[ICF_RGM_IPSEC_NUM_SA];
    icf_ipsec_sa_context_st	        *p_sa_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: sa_identifier = %d\n", 
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier));

    /* temp pointer to SA context whose SAs need to be modified */
    /* this check may look redundant at this stage since only new SA will be 
     * modified, but this is a generic function */
    if (ICF_RGM_IPSEC_SA_NEW == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_NEW_SA_SPI_ALL;
    }
    else if (ICF_RGM_IPSEC_SA_OLD == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_OLD_SA_SPI_ALL;
    }
    else
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* Fill the 4 spi values in spi array */
    spi_arr[0] = p_sa_context->p_ue_security_params->spi_s;
    spi_arr[1] = p_sa_context->p_ue_security_params->spi_c;
    spi_arr[2] = p_sa_context->p_proxy_security_params->spi_s;
    spi_arr[3] = p_sa_context->p_proxy_security_params->spi_c;

    /* construct and send the modify request for each of the four SAs */
    for (ctr = ICF_NULL; ctr < ICF_RGM_IPSEC_NUM_SA; ctr++)
    {
        /* fill the request header */
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_RGM_NM_SA_MODIFY_REQ;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
            ICF_PORT_SIZEOF(icf_rgm_nm_sa_modify_req_st);

        /* fill the request payload */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&req_pld;
        req_pld.spi = spi_arr[ctr];
        /* Duration in Platform API is in seconds */
        req_pld.lifetime = new_lifetime / 1000;

        /* debug traces */
        ICF_PRINT(("\n[REGM]: Modify SA Request to NM #%d >>\n", (ctr+1)));
        ICF_PRINT(("[REGM]: Spi = %ld\n", req_pld.spi));
        ICF_PRINT(("[REGM]: Lifetime = %lu seconds\n", req_pld.lifetime));

        /* send the request to NM */
        if (ICF_FAILURE == icf_nm_process_mesg(p_crm_pdb->p_internal_msg))
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
            ret_val = ICF_FAILURE;
            break;
        }
        else
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_sa_resp_awaiting = p_crm_pdb->p_rgm_context;
        }

        
    } /* End for(ctr) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_nm_sa_modify_req() */
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_nm_sa_delete_req
 *
 * DESCRIPTION:     This is a utility function that will make and send request 
 *                  to NM for the deletion of all unidirectional SAs for a 
 *                  secure connection between one IMPI and P-CSCF. It will use 
 *                  the sa_identifier parameter stored in REGM context to find 
 *                  the list of SAs that need to be filled in request structure.
 *
 *                  This function will take care of sending as many requests to 
 *                  NM as the number of unidirectional SAs that should be 
 *                  deleted. Hence, the state machine handler needs to invoke 
 *                  this function only once for SA deletion.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_nm_sa_delete_req(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_uint8_t			            ctr = ICF_NULL;
	icf_rgm_nm_sa_delete_req_st      req_pld;
    icf_uint32_t			            spi_arr[ICF_RGM_IPSEC_NUM_SA];
    icf_ipsec_sa_context_st	        *p_sa_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: sa_identifier = %d\n", 
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier));

    /* temp pointer to SA context whose SAs need to be deleted */
    if (ICF_RGM_IPSEC_SA_NEW == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_NEW_SA_SPI_ALL;
    }
    else if (ICF_RGM_IPSEC_SA_OLD == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_OLD_SA_SPI_ALL;
    }
    else if (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_BOTH_SA_SPI_ALL;
    }
    else
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_sa_context)

    /* mark the SA as inactive */
    p_sa_context->is_active = ICF_FALSE;

    /* Trigger SSA to close the server in case of De-register request */
    if (ICF_NULL == p_crm_pdb->p_rgm_context->rgm_duration)
    {
        p_crm_pdb->p_internal_msg->p_msg_data = 
            (icf_uint8_t *)(p_crm_pdb->p_rgm_context);
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
            ICF_SSA_SA_CLOSE_SERVER;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
            ICF_PORT_SIZEOF(icf_rgm_context_st);

        if (ICF_FAILURE == icf_ssa_process_mesg(p_crm_pdb->p_internal_msg))
        {
            ret_val = ICF_FAILURE;
            ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
            return ret_val;
        }
    }

    /* Fill the 4 spi values in spi array */
    spi_arr[0] = p_sa_context->p_ue_security_params->spi_s;
    spi_arr[1] = p_sa_context->p_ue_security_params->spi_c;
    spi_arr[2] = p_sa_context->p_proxy_security_params->spi_s;
    spi_arr[3] = p_sa_context->p_proxy_security_params->spi_c;

    /* construct and send the delete request for each of the four SAs */
    for (ctr = ICF_NULL; ctr < ICF_RGM_IPSEC_NUM_SA; ctr++)
    {
        /* fill the request header */
        p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_RGM_NM_SA_DELETE_REQ;
        p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
            ICF_PORT_SIZEOF(icf_rgm_nm_sa_delete_req_st);

        /* fill the request payload */
        p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&req_pld;
        req_pld.spi = spi_arr[ctr];
        
        /* debug traces */
        ICF_PRINT(("\n[REGM]: Delete SA Request to NM #%d >>\n", (ctr+1)));
        ICF_PRINT(("[REGM]: Spi = %ld\n", req_pld.spi));
        
        /* send the request to NM */
        if (ICF_FAILURE == icf_nm_process_mesg(p_crm_pdb->p_internal_msg))
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
            ret_val = ICF_FAILURE;
            break;
        }
        else
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_sa_resp_awaiting = p_crm_pdb->p_rgm_context;
        }
        
    } /* End for(ctr) */

    /* For sa_identifier = SA_ALL, also send delete requests for old SA */
    if ((ICF_SUCCESS == ret_val) &&
        (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier) &&
		(ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context))
    {
        p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;
        ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_sa_context)

        /* mark the SA as inactive */
        p_sa_context->is_active = ICF_FALSE;

        /* Fill the 4 spi values in spi array */
        spi_arr[0] = p_sa_context->p_ue_security_params->spi_s;
        spi_arr[1] = p_sa_context->p_ue_security_params->spi_c;
        spi_arr[2] = p_sa_context->p_proxy_security_params->spi_s;
        spi_arr[3] = p_sa_context->p_proxy_security_params->spi_c;
        
        /* construct and send the modify request for each of the four SAs */
        for (ctr = ICF_NULL; ctr < ICF_RGM_IPSEC_NUM_SA; ctr++)
        {
            /* fill the request header */
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                ICF_RGM_NM_SA_DELETE_REQ;
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_rgm_nm_sa_delete_req_st);
            
            /* fill the request payload */
            p_crm_pdb->p_internal_msg->p_msg_data = (icf_uint8_t *)&req_pld;
            req_pld.spi = spi_arr[ctr];
            
            /* debug traces */
            ICF_PRINT(("\n[REGM]: Delete SA Request to NM #%d >>\n", (ctr+1)));
            ICF_PRINT(("[REGM]: Spi = %ld\n", req_pld.spi));

            /* send the request to NM */
            if (ICF_FAILURE == 
                    icf_nm_process_mesg(p_crm_pdb->p_internal_msg))
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
                ret_val = ICF_FAILURE;
                break;
            }
            else
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_sa_resp_awaiting = p_crm_pdb->p_rgm_context;
            }
            
        } /* End for(ctr) */
    } /* End if(ret_val && sa_identifier) */
	else if ((ICF_SUCCESS == ret_val) &&
        (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier) &&
		(ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context))
	{
		/* This else is needed to avoid a scenario when the calling function 
		 * has set sa_identifier to ALL but old SA does not exist. e.g. in case
		 * of De-register request handling */

		((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = 
            ICF_RGM_IPSEC_NEW_SA_SPI_ALL;

		((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NEW;
	} /* End else(ret_val && sa_identifier) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_nm_sa_delete_req() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_match_resp_spi
 *
 * DESCRIPTION:     This is a utility function that resets the bit in 
 *                  req_bitmask corresponding to the spi value for which 
 *                  response from NM has been received.
 *
 * RETURNS:         Success if the response SPI matches with any of the request
 *                  SPI values, else Failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_match_resp_spi(
        INOUT   icf_crm_pdb_st		*p_crm_pdb,
        IN      icf_uint32_t         resp_spi)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_rgm_context_st               *p_rgm_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the pointer to REGM context locally */
    p_rgm_context = p_crm_pdb->p_rgm_context;
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_rgm_context)
    
    /* debug traces */
    ICF_PRINT(("\n[REGM]: sa_identifier = %d\n",
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier));
    ICF_PRINT(("[REGM]: spi in response = %ld\n", resp_spi));
    ICF_PRINT(("[REGM]: req_bitmask before resetting = %x\n",
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask));
    
    /* check the SA context (new/old) for which the request was sent to NM */
    if (ICF_RGM_IPSEC_SA_ALL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier)
    {
		icf_boolean_t				check_both_sa_ctxt = ICF_FALSE;
		
		if (((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context && 
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
		{
			/* match SPI value in both SA contexts */
			check_both_sa_ctxt = ICF_TRUE;
		}

        /* check the response has been received for which unidirectional SA */
        if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context)
        {
            if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_ue_security_params->spi_s)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_NEW_SA_UE_SPI_S));
            }
            else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_ue_security_params->spi_c)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_NEW_SA_UE_SPI_C));
            }
            else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_proxy_security_params->spi_s)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_NEW_SA_PROXY_SPI_S));
            }
            else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_proxy_security_params->spi_c)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_NEW_SA_PROXY_SPI_C));
            }
            else if (ICF_FALSE == check_both_sa_ctxt)
            {
                ret_val = ICF_FAILURE;
            }
        }
        if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
        {
            if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_ue_security_params->spi_s)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_OLD_SA_UE_SPI_S));
            }
            else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_ue_security_params->spi_c)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_OLD_SA_UE_SPI_C));
            }
            else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_proxy_security_params->spi_s)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_OLD_SA_PROXY_SPI_S));
            }
            else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_proxy_security_params->spi_c)
            {
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                    (~(ICF_RGM_IPSEC_OLD_SA_PROXY_SPI_C));
            }
            else
            {
                ret_val = ICF_FAILURE;
            }
        }

    } /* End if(SA_ALL) */
    else if ((ICF_RGM_IPSEC_SA_NEW == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier) &&
             (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context))
    {
        /* check the response has been received for which unidirectional SA */
        if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_ue_security_params->spi_s)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= (~(ICF_RGM_IPSEC_NEW_SA_UE_SPI_S));
        }
        else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_ue_security_params->spi_c)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= (~(ICF_RGM_IPSEC_NEW_SA_UE_SPI_C));
        }
        else if (resp_spi == 
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_proxy_security_params->spi_s)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                (~(ICF_RGM_IPSEC_NEW_SA_PROXY_SPI_S));
        }
        else if (resp_spi == 
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->p_proxy_security_params->spi_c)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                (~(ICF_RGM_IPSEC_NEW_SA_PROXY_SPI_C));
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    } /* End else(SA_NEW) */
    else if ((ICF_RGM_IPSEC_SA_OLD == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier) &&
             (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context))
    {
        /* check the response has been received for which unidirectional SA */
        if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_ue_security_params->spi_s)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= (~(ICF_RGM_IPSEC_OLD_SA_UE_SPI_S));
        }
        else if (resp_spi == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_ue_security_params->spi_c)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= (~(ICF_RGM_IPSEC_OLD_SA_UE_SPI_C));
        }
        else if (resp_spi == 
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_proxy_security_params->spi_s)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                (~(ICF_RGM_IPSEC_OLD_SA_PROXY_SPI_S));
        }
        else if (resp_spi == 
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->p_proxy_security_params->spi_c)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask &= 
                (~(ICF_RGM_IPSEC_OLD_SA_PROXY_SPI_C));
        }
        else
        {
            ret_val = ICF_FAILURE;
        }
    } /* End else(SA_OLD) */

    /* debug traces */
    ICF_PRINT(("\n[REGM]: req_bitmask after resetting = %x\n",
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask));

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_match_resp_spi() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_process_nm_msg
 *
 * DESCRIPTION:     This is a function that is invoked by CFG on receiving a 
 *                  message from NM. It will trigger the state machine handler.
 *
 * RETURNS:         Nothing
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_process_nm_msg(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_rgm_glb_data_st              *p_rgm_glb_data = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
    
    /* Call DBM to get the pointer to the RGM global data in p_buff */
    if (ICF_FAILURE == icf_dbm_get_module_glb_data(
                            p_crm_pdb->p_glb_pdb,
                            (icf_int_module_id_t)(ICF_INT_MODULE_REGM),
                            (icf_void_t**)&p_rgm_glb_data,
                            p_crm_pdb->p_ecode))
    {
        ret_val = ICF_FAILURE;
        ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
        return ret_val;
    }

    /* assign the relavent rgm ctxt into p_crm_pdb->p_rgm_context */
    p_crm_pdb->p_rgm_context = p_rgm_glb_data->p_sa_resp_awaiting;
    
    if(ICF_NULL != p_crm_pdb->p_rgm_context)
    {
        switch(p_crm_pdb->p_internal_msg->msg_hdr.msg_id)
        {
        case ICF_NM_RGM_SA_CREATE_RESP:
            p_rgm_glb_data->p_sa_resp_awaiting = ICF_NULL;
            ret_val = p_rgm_glb_data->icf_rgm_state_machine
                [p_crm_pdb->p_rgm_context->rgm_state]
                [ICF_RGM_SA_CREATE_RESP](p_crm_pdb);
#ifdef ICF_SIGCOMP_SUPPORT
            if (ICF_TRUE == p_crm_pdb->p_config_data->sigcomp_required)
            {
                if (ICF_SUCCESS == ret_val)
                {
                    icf_rgm_send_create_comp_req_to_sigsa(p_crm_pdb);
                }
            }
#endif
            break;
            
#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
        case ICF_NM_RGM_SA_MODIFY_RESP:
            p_rgm_glb_data->p_sa_resp_awaiting = ICF_NULL;
            ret_val = p_rgm_glb_data->icf_rgm_state_machine
                [p_crm_pdb->p_rgm_context->rgm_state]
                [ICF_RGM_SA_MODIFY_RESP](p_crm_pdb);
            break;
#endif
            
        case ICF_NM_RGM_SA_DELETE_RESP:
            p_rgm_glb_data->p_sa_resp_awaiting = ICF_NULL;
            ret_val = p_rgm_glb_data->icf_rgm_state_machine
                [p_crm_pdb->p_rgm_context->rgm_state]
                [ICF_RGM_SA_DELETE_RESP](p_crm_pdb);
            
            break;
            
        case ICF_NM_RGM_SA_EXPIRY_IND:
            p_rgm_glb_data->p_sa_resp_awaiting = ICF_NULL;
            ret_val = p_rgm_glb_data->icf_rgm_state_machine
                [p_crm_pdb->p_rgm_context->rgm_state]
                [ICF_RGM_SA_EXPIRY_IND](p_crm_pdb);
            break;
            
        default:
            ret_val = ICF_FAILURE;
            break;
        } /* End switch(msg_id) */
    }
#ifdef IMS_CLIENT
ICF_CHECK_IMS_START(p_crm_pdb->p_glb_pdb,ICF_TRUE)
    ret_val = icf_process_buffer(p_crm_pdb);
ICF_CHECK_IMS_END
#endif

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_process_nm_msg() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_01_sa_delete_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion response in registering state.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_01_sa_delete_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     spi_match = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        *p_nm_resp_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the received payload pointer locally */
    p_nm_resp_pld = (icf_nm_rgm_sa_cmd_resp_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_nm_resp_pld)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Delete SA Response from NM >>\n"));
    ICF_PRINT(("[REGM]: spi = %ld\n", p_nm_resp_pld->spi));
    ICF_PRINT(("[REGM]: retval = %d\n", p_nm_resp_pld->response));

    spi_match = icf_rgm_ipsec_match_resp_spi(p_crm_pdb, p_nm_resp_pld->spi);
  
    /* check if it is success or failure response from NM */  
    if ((((ICF_SUCCESS == p_nm_resp_pld->response) &&
        (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask)) ||
        (ICF_FAILURE == p_nm_resp_pld->response)) &&
        (ICF_SUCCESS == spi_match))
    {
        /* stop the SA delete timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* deallocate the SA context, already set in sa_identifier */
        else if (ICF_FAILURE == icf_rgm_ipsec_dealloc_sa_context(
                                        p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* additional things to be done for failure response */
        else if(ICF_FAILURE == p_nm_resp_pld->response)
        {
            /* Raise alarm */
            ICF_ALARM(("\nSA deletion failed\n"));
        } /* End if(p_nm_resp_pld->response) */

    } /* End if(p_nm_resp_pld->response && req_bitmask) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_01_sa_delete_resp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_01_sa_delete_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion guard timer expiry in registering 
 *                  state. This will just raise an alarm and return.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_01_sa_delete_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_data_st 		        *p_timer_pld = ICF_NULL;
    icf_timer_data_st 		        **p_p_timer_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* Typecast the API payload to timer data structure */
    p_p_timer_pld = 
        (icf_timer_data_st **)(p_crm_pdb->p_glb_pdb->p_recv_msg->payload);
    
    p_timer_pld = (icf_timer_data_st *)(*p_p_timer_pld);
    
    /* Get the pointer to REGM context from received timer data */
    p_crm_pdb->p_rgm_context = 
        (icf_rgm_context_st *)(p_timer_pld->p_timer_buffer);

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Timer Expiry for timer-id = %d\n",
        p_crm_pdb->p_rgm_context->timer_id));
    
    /* reset the number of responses awaited from NM */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
   
    /* deallocate the SA context, already set in sa_identifier */
    ret_val = icf_rgm_ipsec_dealloc_sa_context(p_crm_pdb);
    
    /* Free the timer buffer */
    
    /* This is a call to DBM to free the registration timer buffer */
    ret_val = icf_dbm_free_timer_block(
                    p_crm_pdb->p_glb_pdb,
                    (icf_void_t *)(p_timer_pld),
                    p_crm_pdb->p_ecode);    
    
    /* Set Timer ID to INVALID */
    p_crm_pdb->p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    
    /* Set pointer to timer as NULL */
    p_crm_pdb->p_rgm_context->p_timer_buff = ICF_NULL;
        
    /* Raise alarm */
    ICF_ALARM(("\nSA delete timer expired\n"));

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_01_sa_delete_timer_exp() */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_modify_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA modification response in registered state.
 *
 *                  SA modification will be done only if the underlying platform
 *                  supports SA lifetime change, and hence this function will be
 *                  inside the compilation flag ICF_SA_LIFETIME_MOD_SUPPORT.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_modify_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     spi_match = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        *p_nm_resp_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the received payload pointer locally */
    p_nm_resp_pld = (icf_nm_rgm_sa_cmd_resp_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_nm_resp_pld)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Modify SA Response from NM >>\n"));
    ICF_PRINT(("[REGM]: spi = %ld\n", p_nm_resp_pld->spi));
    ICF_PRINT(("[REGM]: response = %d\n", p_nm_resp_pld->response));

    spi_match = icf_rgm_ipsec_match_resp_spi(p_crm_pdb, p_nm_resp_pld->spi);
    
    /* check if it is success or failure response from NM */
    if ((ICF_SUCCESS == p_nm_resp_pld->response) &&
        (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask) &&
        (ICF_SUCCESS == spi_match))
    {
        /* Update the lifetime in SA context */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->sa_lifetime = 
            p_crm_pdb->p_rgm_context->rgm_duration + 
            ICF_RGM_IPSEC_DELTA_DURATION;

        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;

        /* stop SA modify timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* Delete any old SA if there are no pending transactions */
        else if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
        {
            if (ICF_NULL == 
                  ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->num_active_trans)
            {
                /* send SA delete request to NM */
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = 
                    ICF_RGM_IPSEC_SA_OLD;
                
                ret_val = icf_rgm_ipsec_sa_clean_up(
                            p_crm_pdb,
                            ICF_RGM_03_REGISTERED);
            } /* End if(num_active_trans) */
        } /* End if(p_old_sa_context) */
        else
        {
            /* start reg timer for the remaining duration */
            ret_val = icf_rgm_start_timer(
                            p_crm_pdb->p_glb_pdb,
                            ICF_TIMER_TYPE_RGM_REGISTRATION, 
                            p_crm_pdb->p_rgm_context, 
                            p_crm_pdb->p_ecode);
        } /* End else(p_old_sa_context) */

    } /* End if(p_nm_resp_pld->response) */
    else if ((ICF_FAILURE == p_nm_resp_pld->response) &&
             (ICF_SUCCESS == spi_match))
    {
        /* reset the number of responses awaited from NM */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;

        /* reset the SA identifier */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;

        /* stop the SA modify timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* start reg timer for the remaining duration */
        else if (ICF_FAILURE == icf_rgm_start_timer(
                                    p_crm_pdb->p_glb_pdb,
                                    ICF_TIMER_TYPE_RGM_REGISTRATION,
                                    p_crm_pdb->p_rgm_context,
                                    p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* Raise alarm */
            ICF_ALARM(("\nSA modification failed\n"));
        }
    } /* End else(p_nm_resp_pld->response) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_03_sa_modify_resp() */
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


#ifdef ICF_SA_LIFETIME_MOD_SUPPORT
/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_modify_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA modification guard timer expiry in 
 *                  registered state.
 *
 *                  SA modification will be done only if the underlying platform
 *                  supports SA lifetime change, and hence this function will be
 *                  inside the compilation flag ICF_SA_LIFETIME_MOD_SUPPORT.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_modify_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
         
    /* reset the number of responses awaited from NM */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
    
    /* reset the SA identifier */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;
        
    /* Raise alarm */
    ICF_ALARM(("\nSA modify timer expired\n"));
        
    /* Delete any old SA if there are no pending transactions */
    if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
    {
        if (ICF_NULL == 
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context->num_active_trans)
        {
            /* send SA delete request to NM for old SA */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_OLD;
            
            ret_val = icf_rgm_ipsec_sa_clean_up(
                        p_crm_pdb,
                        ICF_RGM_03_REGISTERED);
        } /* End if(num_active_trans) */
    } /* End if(p_old_sa_context) */
    else
    {
        /* start reg timer for the remaining duration */
        ret_val = icf_rgm_start_timer(
                        p_crm_pdb->p_glb_pdb,
                        ICF_TIMER_TYPE_RGM_REGISTRATION,
                        p_crm_pdb->p_rgm_context,
                        p_crm_pdb->p_ecode);
    } /* End else(p_old_sa_context) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_03_sa_modify_timer_exp() */
#endif /* ICF_SA_LIFETIME_MOD_SUPPORT */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_delete_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion response in registered state.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_delete_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     spi_match = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        *p_nm_resp_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the received payload pointer locally */
    p_nm_resp_pld = (icf_nm_rgm_sa_cmd_resp_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_nm_resp_pld)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Delete SA Response from NM >>\n"));
    ICF_PRINT(("[REGM]: spi = %ld\n", p_nm_resp_pld->spi));
    ICF_PRINT(("[REGM]: response = %d\n", p_nm_resp_pld->response));

    spi_match = icf_rgm_ipsec_match_resp_spi(p_crm_pdb, p_nm_resp_pld->spi);
    
    /* check if it is success or failure response from NM */
    if ((((ICF_SUCCESS == p_nm_resp_pld->response) &&
        (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask)) ||
        (ICF_FAILURE == p_nm_resp_pld->response)) &&
        (ICF_SUCCESS == spi_match))
    {
        /* stop the SA delete timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* deallocate the SA context, already set in sa_identifier */
        else if (ICF_FAILURE == icf_rgm_ipsec_dealloc_sa_context(
                                        p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* start timer for the remaining REG duration */
        else if (ICF_FAILURE == icf_rgm_start_timer(
                                        p_crm_pdb->p_glb_pdb,
                                        ICF_TIMER_TYPE_RGM_REGISTRATION,
                                        p_crm_pdb->p_rgm_context,
                                        p_crm_pdb->p_ecode))
        {
            ret_val = ICF_FAILURE;
        }
        /* additional things to be done for failure response */
        else if(ICF_FAILURE == p_nm_resp_pld->response)
        {
            /* Raise alarm */
            ICF_ALARM(("\nSA deletion failed\n"));
        } /* End if(p_nm_resp_pld->response) */
        
    } /* End if(p_nm_resp_pld->response && req_bitmask) */
    
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_03_sa_delete_resp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_delete_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion guard timer expiry in registered 
 *                  state.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_delete_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* reset the number of responses awaited from NM */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
    
    /* deallocate SA context, already set in sa_identifier */
    ret_val = icf_rgm_ipsec_dealloc_sa_context(p_crm_pdb);
    
    /* Raise alarm */
    ICF_ALARM(("\nSA delete timer expired\n"));
        
    /* start timer for the remaining REG duration */
    ret_val = icf_rgm_start_timer(
                    p_crm_pdb->p_glb_pdb,
                    ICF_TIMER_TYPE_RGM_REGISTRATION,
                    p_crm_pdb->p_rgm_context,
                    p_crm_pdb->p_ecode);

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_03_sa_delete_timer_exp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_03_sa_expiry_ind
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives an SA expiry indication from NM. In case new 
 *                  SA has expired, it will forcefully clear all the ongoing 
 *                  dialogs, send indication to the application, stop any timer 
 *                  that is running and clear the REGM context.
 *
 * RETURNS:         ICF_SUCCESS always
 *
 ******************************************************************************/
icf_return_t icf_rgm_03_sa_expiry_ind(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_ipsec_sa_context_st	        *p_sa_context = ICF_NULL;
    icf_nm_rgm_sa_expiry_ind_st      *p_nm_resp_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the received payload pointer locally */
    p_nm_resp_pld = (icf_nm_rgm_sa_expiry_ind_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_nm_resp_pld)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd SA Expiry Indication from NM >>\n"));
    ICF_PRINT(("[REGM]: spi = %ld\n", p_nm_resp_pld->spi));

    /* If old SA context pointer is not null, simply raise alarm and all the 
     * pending transactions on old SA will clean up on timeout. If it is null, 
     * it means currently used new SA has expired, so raise alarm and do all the
     * forceful clean up */

     /* Raise alarm */
     ICF_ALARM(("\nSA expiry indication received\n"));

	/* do not check the return value of any function */

    /* check if old SA has expired */
    p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;
    if (ICF_NULL != p_sa_context)
    {      
        /* debug traces */
        ICF_PRINT(("\n[REGM]: Old SA expired, only freeing the context\n"));

        /* free the pointers inside SA context */
        if (ICF_NULL != p_sa_context->p_integrity_algo)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_integrity_algo,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        if (ICF_NULL != p_sa_context->p_encryption_algo)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_encryption_algo,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        if (ICF_NULL != p_sa_context->p_q_value)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_q_value,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
		if (ICF_NULL != p_sa_context->p_ue_security_params)
		{
			ICF_MEMFREE(
				p_crm_pdb->p_glb_pdb,
				p_sa_context->p_ue_security_params,
				ICF_MEM_COMMON,
				p_crm_pdb->p_ecode,
				ret_val)
		}
		if (ICF_NULL != p_sa_context->p_proxy_security_params)
		{
			ICF_MEMFREE(
				p_crm_pdb->p_glb_pdb,
				p_sa_context->p_proxy_security_params,
				ICF_MEM_COMMON,
				p_crm_pdb->p_ecode,
				ret_val)
		}

        /* finally free the SA context */
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            p_sa_context,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)

    } /* End if(p_sa_context) */
    else
    {
        /* debug traces */
        ICF_PRINT(("\n[REGM]: New SA expired, freeing all contexts\n"));

        /* stop any currently running timer */
	    ret_val = icf_rgm_stop_timer(p_crm_pdb);
	    
        /* clear all active dialogs */
	    ret_val = icf_cfg_cm_call_clear_ind(p_crm_pdb);
	    
         /* clear the reg context */
	    ret_val = icf_rgm_clear_reg_context(
                        p_crm_pdb,
                        p_crm_pdb->p_rgm_context);

    } /* End else(p_sa_context) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_03_sa_expiry_ind() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_01_sa_create_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA creation response in state awaiting SA 
 *                  creation response.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_01_sa_create_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     spi_match = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        *p_nm_resp_pld = ICF_NULL;
    icf_rgm_register_resp_st	        register_response;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the received payload pointer locally */
    p_nm_resp_pld = (icf_nm_rgm_sa_cmd_resp_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_nm_resp_pld)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Create SA Response from NM >>\n"));
    ICF_PRINT(("[REGM]: spi = %ld\n", p_nm_resp_pld->spi));
    ICF_PRINT(("[REGM]: response = %d\n", p_nm_resp_pld->response));

    spi_match = icf_rgm_ipsec_match_resp_spi(p_crm_pdb, p_nm_resp_pld->spi);
  
    /* check if it is success or failure response from NM */
    if ((ICF_SUCCESS == p_nm_resp_pld->response) &&
        (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask) &&
        (ICF_SUCCESS == spi_match))
    {
        /* reset the SA identifier */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;

        /* set the new SA to active */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context->is_active = ICF_TRUE;

        /* stop the SA create timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            p_crm_pdb->p_rgm_context->indicator |= ICF_RGM_AUTH_SET;

            /* Trigger SSA to open the server and then send register request 
             * with Auth. This needs to be done only once, i.e., if old SA is
             * null */
            if (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
            {
                p_crm_pdb->p_internal_msg->p_msg_data = 
                    (icf_uint8_t *)(p_crm_pdb->p_rgm_context);
                p_crm_pdb->p_internal_msg->msg_hdr.msg_id = 
                    ICF_SSA_SA_OPEN_SERVER;
                p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
                    ICF_PORT_SIZEOF(icf_rgm_context_st);
            
                ret_val = icf_ssa_process_mesg(
                                p_crm_pdb->p_internal_msg);
            }
            
            if ((ICF_SUCCESS == ret_val) &&
                (ICF_SUCCESS == icf_rgm_register_user(
                                        p_crm_pdb,
                                        p_crm_pdb->p_rgm_context)))
            {
                p_crm_pdb->p_rgm_context->rgm_state = ICF_RGM_04_AUTHORIZING;
            }
            else
            {
                ret_val = ICF_FAILURE;
            }
        } /* End else */

    } /* End if(p_nm_resp_pld->response) */
    else if ((ICF_FAILURE == p_nm_resp_pld->response) &&
             (ICF_SUCCESS == spi_match))
    {
        /* reset the number of responses awaited from NM */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;

        /* reset the SA identifier */
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;

        /* stop the SA create timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* Clear all active dialogs */
        else if (ICF_FAILURE == icf_cfg_cm_call_clear_ind(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* Delete old SA if it is not null */
        else if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
        {
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_OLD;
            
            ret_val = icf_rgm_ipsec_sa_clean_up(
                            p_crm_pdb,
                            p_crm_pdb->p_rgm_context->rgm_state);
        }
        
        /* If Auto-registration is supported, trigger fresh registration else 
         * send failure to application */
        if ((ICF_SUCCESS == ret_val) &&
            (ICF_NULL != p_crm_pdb->p_config_data->auto_register))
        {
            /* Register request needs to be sent on new call object, so trigger 
             * SSA to delete the existing call object */
            
            /* fill CRM PDB Structure */
            p_crm_pdb->p_internal_msg->p_msg_data = 
                (icf_uint8_t *)(p_crm_pdb->p_rgm_context);
            
            p_crm_pdb->p_internal_msg->msg_hdr.msg_id = ICF_CLEAR_REG_OBJECT;
            
            p_crm_pdb->p_internal_msg->msg_hdr.payload_length = 
                ICF_PORT_SIZEOF(icf_rgm_context_st);
            
            /* call SSA to clear Registration Call Object */
            if (ICF_FAILURE == icf_ssa_process_mesg(
                                        p_crm_pdb->p_internal_msg))
            {
                ret_val = ICF_FAILURE;
            }
            /* send fresh register request */
            else
            {
                ret_val = icf_rgm_register_user(
                                        p_crm_pdb,
                                        p_crm_pdb->p_rgm_context);
            }

        } /* End if(ret_val and auto_register) */
        else if (ICF_SUCCESS == ret_val)
        {
            ICF_ALARM(("\nSA creation failed\n"));
           
            /* Send failure response/indication to application */
			register_response.options_flag = ICF_NULL;
            register_response.options_flag |= ICF_RGM_RESPONSE_IND;
            register_response.options_flag |= ICF_RGM_CTXT_COPY;            
            register_response.status = ICF_FAILURE;
            register_response.error_cause = ICF_CAUSE_SA_CREATION_FAILURE;
            register_response.request_type = ICF_NULL;
            register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
            register_response.p_request_payload = ICF_NULL;
			register_response.p_crm_pdb = p_crm_pdb;
            
            p_crm_pdb->resp_to_be_sent = ICF_FALSE;
            if (ICF_FAILURE == icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                &register_response, p_crm_pdb->p_ecode))
			{
				ret_val = ICF_FAILURE;
			}
			/* Clear REGM context */
			else
			{
				ret_val = icf_rgm_clear_reg_context(p_crm_pdb, 
					p_crm_pdb->p_rgm_context);
			}
        }
    } /* End else(p_nm_resp_pld->response) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_01_sa_create_resp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_01_sa_create_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA creation guard timer expiry in state 
 *                  awaiting SA create response.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_01_sa_create_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_data_st 		        *p_timer_pld = ICF_NULL;
    icf_timer_data_st 		        **p_p_timer_pld = ICF_NULL;
    icf_rgm_register_resp_st	        register_response;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Typecast the API payload to timer data structure */
    p_p_timer_pld = 
        (icf_timer_data_st **)(p_crm_pdb->p_glb_pdb->p_recv_msg->payload);
    
    p_timer_pld = (icf_timer_data_st *)(*p_p_timer_pld);
    
    /* Get the pointer to REGM context from received timer data */
    p_crm_pdb->p_rgm_context = 
        (icf_rgm_context_st *)(p_timer_pld->p_timer_buffer);
    
    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Timer Expiry for timer-id = %d\n",
        p_crm_pdb->p_rgm_context->timer_id));

    /* reset the number of responses awaited from NM */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;

    /* reset the SA identifier */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;

    /* Free the timer buffer */
    
    /* This is a call to DBM to free the registration timer buffer */
    ret_val = icf_dbm_free_timer_block(
                    p_crm_pdb->p_glb_pdb,
                    (icf_void_t *)(p_timer_pld),
                    p_crm_pdb->p_ecode);    
    
    /* Set Timer ID to INVALID */
    p_crm_pdb->p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    
    /* Set pointer to timer as NULL */
    p_crm_pdb->p_rgm_context->p_timer_buff = ICF_NULL;
    
    /* Deallocate the security client parameters */
    if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params)
    {
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_security_client_params,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)
    }
    
    /* Deallocate the new SA context */
    if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context)
    {
		icf_ipsec_sa_context_st		*p_sa_context = ICF_NULL;

		p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context;

        /* free the pointers inside SA context */
        if (ICF_NULL != p_sa_context->p_integrity_algo)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_integrity_algo,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        if (ICF_NULL != p_sa_context->p_encryption_algo)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_encryption_algo,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        if (ICF_NULL != p_sa_context->p_q_value)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_q_value,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
		if (ICF_NULL != p_sa_context->p_ue_security_params)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_ue_security_params,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
		if (ICF_NULL != p_sa_context->p_proxy_security_params)
        {
            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                p_sa_context->p_proxy_security_params,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        }
        ICF_MEMFREE(
            p_crm_pdb->p_glb_pdb,
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_new_sa_context,
            ICF_MEM_COMMON,
            p_crm_pdb->p_ecode,
            ret_val)
    }
    
    ICF_ALARM(("\nSA create timer expired\n"));
   
    /* Send failure response/indication to application */
	register_response.options_flag = ICF_NULL;
    register_response.options_flag |= ICF_RGM_RESPONSE_IND;
    register_response.options_flag |= ICF_RGM_CTXT_COPY;    
    register_response.status = ICF_FAILURE;
    register_response.error_cause = ICF_CAUSE_SA_CREATION_FAILURE;
    register_response.request_type = ICF_NULL;
    register_response.p_rgm_context = p_crm_pdb->p_rgm_context;
    register_response.p_request_payload = ICF_NULL;
	register_response.p_crm_pdb = p_crm_pdb;
	    
    p_crm_pdb->resp_to_be_sent = ICF_FALSE;
    if (ICF_FAILURE == icf_rgm_send_response(p_crm_pdb->p_glb_pdb,
                            &register_response, p_crm_pdb->p_ecode))
	{
        ret_val = ICF_FAILURE;
    }   
    /* Clear all active dialogs */
    else if (ICF_FAILURE == icf_cfg_cm_call_clear_ind(p_crm_pdb))
    {
        ret_val = ICF_FAILURE;
    }
    /* Delete old SA if it is not null */
    else if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
    {
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_OLD;
        
        ret_val = icf_rgm_ipsec_sa_clean_up(
                        p_crm_pdb,
                        ICF_RGM_07_IPSEC_AWT_SA_DEL_RESP);
    }
    /* Clear REGM context */
    else
    {
        ret_val = icf_rgm_clear_reg_context(p_crm_pdb, 
            p_crm_pdb->p_rgm_context);
    }
        
    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_01_sa_create_timer_exp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_07_sa_delete_resp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion response in state awaiting SA 
 *                  delete response.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_07_sa_delete_resp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_return_t                     spi_match = ICF_SUCCESS;
    icf_nm_rgm_sa_cmd_resp_st        *p_nm_resp_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)

    /* copy the received payload pointer locally */
    p_nm_resp_pld = (icf_nm_rgm_sa_cmd_resp_st *)
                    (p_crm_pdb->p_internal_msg->p_msg_data);
    ICF_ASSERT(p_crm_pdb->p_glb_pdb, p_nm_resp_pld)

    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Delete SA Response from NM >>\n"));
    ICF_PRINT(("[REGM]: spi = %ld\n", p_nm_resp_pld->spi));
    ICF_PRINT(("[REGM]: response = %d\n", p_nm_resp_pld->response));

    spi_match = icf_rgm_ipsec_match_resp_spi(p_crm_pdb, p_nm_resp_pld->spi);

    /* check if it is success or failure response from NM */  
    if ((((ICF_SUCCESS == p_nm_resp_pld->response) &&
        (ICF_NULL == ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask)) ||
        (ICF_FAILURE == p_nm_resp_pld->response)) &&
        (ICF_SUCCESS == spi_match))
    {
        /* stop the SA delete timer and free the timer block */
        if (ICF_FAILURE == icf_rgm_stop_timer(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* deallocate SA context, already set in sa_identifier */
        else if (ICF_FAILURE == icf_rgm_ipsec_dealloc_sa_context(
                                        p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        /* additional things to be done for failure response */
        else if(ICF_FAILURE == p_nm_resp_pld->response)
        {
            /* Raise alarm */
            ICF_ALARM(("\nSA deletion failed\n"));
        } /* End if(p_nm_resp_pld->response) */
        
        /* Clean up */
        ret_val = icf_rgm_clear_reg_context(
                        p_crm_pdb,
                        p_crm_pdb->p_rgm_context);
        
    } /* End if(p_nm_resp_pld->response && req_bitmask) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_07_sa_delete_resp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_07_sa_delete_timer_exp
 *
 * DESCRIPTION:     This is a state machine handler that will be invoked when 
 *                  REGM receives SA deletion guard timer expiry in state 
 *                  awaiting SA delete response. This will just raise an alarm 
 *                  and return.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_07_sa_delete_timer_exp(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
    icf_timer_data_st 		*p_timer_pld = ICF_NULL;
    icf_timer_data_st 		**p_p_timer_pld = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    /* Typecast the API payload to timer data structure */
    p_p_timer_pld = 
        (icf_timer_data_st **)(p_crm_pdb->p_glb_pdb->p_recv_msg->payload);
    
    p_timer_pld = (icf_timer_data_st *)(*p_p_timer_pld);
    
    /* Get the pointer to REGM context from received timer data */
    p_crm_pdb->p_rgm_context = 
        (icf_rgm_context_st *)(p_timer_pld->p_timer_buffer);
    
    /* debug traces */
    ICF_PRINT(("\n[REGM]: Recd Timer Expiry for timer-id = %d\n",
        p_crm_pdb->p_rgm_context->timer_id));

    /* reset the number of responses awaited from NM */
    ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;

    /* deallocate SA context, already set in sa_identifier */
    ret_val = icf_rgm_ipsec_dealloc_sa_context(p_crm_pdb);
    
    /* Free the timer buffer */
    
    /* This is a call to DBM to free the registration timer buffer */
    ret_val = icf_dbm_free_timer_block(
                    p_crm_pdb->p_glb_pdb,
                    (icf_void_t *)(p_timer_pld),
                    p_crm_pdb->p_ecode);    
    
    /* Set Timer ID to INVALID */
    p_crm_pdb->p_rgm_context->timer_id = ICF_TIMER_ID_INVALID;
    
    /* Set pointer to timer as NULL */
    p_crm_pdb->p_rgm_context->p_timer_buff = ICF_NULL;
    
    /* Raise alarm */
    ICF_ALARM(("\nSA delete timer expired\n"));
        
    /* Clean up */
    ret_val = icf_rgm_clear_reg_context(p_crm_pdb, p_crm_pdb->p_rgm_context);

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_07_sa_delete_timer_exp() */


/*******************************************************************************
 *
 * FUNCTION:        icf_rgm_ipsec_delete_old_sa
 *
 * DESCRIPTION:     This new REGM function shall be invoked by CFG on getting 
 *                  ICF_DELETE_OLD_SA internal message from SSA. It will be 
 *                  handled outside the state machine and shall send request to 
 *                  NM to delete the old SA. REGM will not wait for the response
 *                  of this request and will not start any guard timer.
 *
 * RETURNS:         ICF_SUCCESS on success, ICF_FAILURE on failure
 *
 ******************************************************************************/
icf_return_t icf_rgm_ipsec_delete_old_sa(
        INOUT   icf_crm_pdb_st		*p_crm_pdb)
{
    icf_return_t                     ret_val = ICF_SUCCESS;
	icf_ipsec_sa_context_st			*p_sa_context = ICF_NULL;

    ICF_FUNCTION_ENTER(p_crm_pdb->p_glb_pdb)
        
    p_crm_pdb->p_rgm_context = 
        (icf_rgm_context_st *)(p_crm_pdb->p_internal_msg->p_msg_data);
    
    if (ICF_NULL != ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context)
    {
		p_sa_context = ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context;

        /* Send request to NM to delete old SA*/
        ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_OLD;
        
        if (ICF_FAILURE == icf_rgm_ipsec_nm_sa_delete_req(p_crm_pdb))
        {
            ret_val = ICF_FAILURE;
        }
        else
        {
            /* reset the SA identifier */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->sa_identifier = ICF_RGM_IPSEC_SA_NONE;
            
            /* reset the number of responses awaited from NM */
            ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->req_bitmask = ICF_NULL;
            
			/* free the pointers inside SA context */
			if (ICF_NULL != p_sa_context->p_integrity_algo)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_integrity_algo,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}
			if (ICF_NULL != p_sa_context->p_encryption_algo)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_encryption_algo,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}
			if (ICF_NULL != p_sa_context->p_q_value)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_q_value,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}
			if (ICF_NULL != p_sa_context->p_ue_security_params)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_ue_security_params,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}
			if (ICF_NULL != p_sa_context->p_proxy_security_params)
			{
				ICF_MEMFREE(
					p_crm_pdb->p_glb_pdb,
					p_sa_context->p_proxy_security_params,
					ICF_MEM_COMMON,
					p_crm_pdb->p_ecode,
					ret_val)
			}

            ICF_MEMFREE(
                p_crm_pdb->p_glb_pdb,
                ((icf_rgm_glb_data_st *)(p_crm_pdb->p_rgm_glb_data))->p_old_sa_context,
                ICF_MEM_COMMON,
                p_crm_pdb->p_ecode,
                ret_val)
        } /* End else */

    } /* End if(p_old_sa_context) */

    ICF_FUNCTION_EXIT(p_crm_pdb->p_glb_pdb)
    return ret_val;
} /* End function icf_rgm_ipsec_delete_old_sa() */


#endif /* ICF_IPSEC_ENABLED */
