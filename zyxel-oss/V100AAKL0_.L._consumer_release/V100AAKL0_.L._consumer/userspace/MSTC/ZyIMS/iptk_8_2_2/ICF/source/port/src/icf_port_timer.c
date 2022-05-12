/*************************************************************************
 *
 *     FILENAME     :    icf_port_timer.c
 *
 *     DESCRIPTION  :  This file defines the porting functions for timers.
 *                     
 *     Revision History    :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         25/11/2004    Pankaj Negi        LLD         Initial version.
 *         06/01/2005    Aashish Suchdev                Updated for port
 *                                                      timer changes.
 *         06/01/2005    Aashish Suchdev                Updated for changes
 *                                                      in check_timer
 *                                                      prototype
 *         19/10/2005    Saurabh Gupta                  SPR 8021
 *         23/02/2007    Umang Singh        Solving the race condition 
 *                                          for timer expiries
 *         07/04/08  Sumant Gupta    ICF 8.0         Rel7.1 Windows porting
 *         07/11/08  Tarun Gupta     SPR 19189       Merged Rel 7.1.2 SPR 18880
 *         07/11/08  Rajiv Kumar     SPR 19188       Rel 7.1.2 SPR merging (SPR
 *                                                   18909) 
 *     Copyright 2008, Aricent.
 *
 *************************************************************************/
#ifdef GENLIB_TIMER
#include "gl_types.h"
#include "gl_tm_interfaces.h"
#endif

#include "icf_common_types.h"
#include "icf_ecode.h"
#include "icf_port.h"
#include "icf_port_struct.h"
#include "icf_port_prototypes.h"
#include "icf_port_intf_prototypes.h"

extern icf_glb_pdb_st *p_persistent_glb_pdb;
extern icf_return_t icf_cmn_memfree(
            INOUT   icf_glb_pdb_st        *p_glb_pdb,
            INOUT      icf_void_t*           p_buf,
            IN      icf_pool_id_t     pool_id,
            OUT     icf_error_t    *p_ecode );


/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_start_timer
 *
 *  DESCRIPTION     :    This function starts the ICF timer for an SLP
 *                       It consequently has an SLP specific interface. 
 *
 *  RETURNS         :    ICF_SUCCESS if successful else ICF_FAILURE 
 *************************************************************************/

icf_return_t 
icf_port_start_timer(
 INOUT    icf_void_t           *p_port_info,
 INOUT    icf_timer_data_st    *p_buff,
 IN	   icf_uint32_t         duration,
 OUT   icf_timer_id_t       *p_timer_id,
 OUT   icf_error_t  	       *p_ecode)

{

    icf_uint16_t                 current_index = 0;
    icf_global_port_info_st      *p_glb_port_info = ICF_NULL;
    icf_port_timer_data_st       *p_port_timer_data = ICF_NULL;

#ifdef GENLIB_TIMER    
    gl_timer_id_t                   timer_id = GL_NULL;
    gl_error_t                      gl_ecode = GW_ERROR;
#endif

    /* SPR 18880 : Added NULL checks */
    if((ICF_NULL == p_port_info) ||
       (ICF_NULL == p_buff))
    {
        /* Return failure as not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_info or p_buff is NULL.\n"))
        return ICF_FAILURE;
    }
#ifdef GENLIB_TIMER  
    p_glb_port_info = (icf_global_port_info_st*)p_port_info;
    p_port_timer_data = &(p_glb_port_info->glb_timer_data);

    if((ICF_NULL == p_port_timer_data) ||
       (ICF_NULL == p_port_timer_data->icf_genlib_timer_cb_fn))
    {
        /* Return failure as not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_timer_data or "
                   "p_port_timer_data->icf_genlib_timer_cb_fn is NULL.\n"))
        return ICF_FAILURE;
    }
    current_index = p_port_timer_data->current_list_index;

    if(GL_FAILURE==gl_tm_start_timer(p_port_timer_data->timer_id_list[current_index],
                (gw_U32bit)duration, 
                (gl_pvoid_t)(p_port_timer_data->icf_genlib_timer_cb_fn),
                (icf_void_t *) (p_buff),
                ICF_FALSE, /* We donot need periodic timer currently*/
                &(timer_id),/*U32 bit*/
                &gl_ecode))  
    {
        /* Increase the value of current_list_index */

        p_port_timer_data->current_list_index++;

        p_port_timer_data->current_list_index =
            (icf_uint16_t)(p_port_timer_data->current_list_index % 
            ICF_MAX_TIMER_LIST_COUNT);
        *p_ecode = ICF_ERROR_TMR_START_FAILURE;

        return ICF_FAILURE;
    }

    /* Copy current index list in MS 16 bits and timer id in first 16 bits and
     * return to module; */
    *p_timer_id = (timer_id & 0xFFFF)|(p_port_timer_data->current_list_index<<16);

    /* Increase the value of current_list_index */

    p_port_timer_data->current_list_index++;

    p_port_timer_data->current_list_index =
        (icf_uint16_t)(p_port_timer_data->current_list_index % 
        ICF_MAX_TIMER_LIST_COUNT);

    /*Save the timer id in timer data*/
    p_buff->timer_id = *p_timer_id;
    return ICF_SUCCESS;

#endif
} 

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_stop_timer
 *
 *  DESCRIPTION     :    This function is the port function for stopping 
 *                       a started timer. It has checks for timers not
 *                       started.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
 *************************************************************************/
icf_return_t
icf_port_stop_timer(
        INOUT          icf_void_t       *p_port_info,
        INOUT       icf_timer_id_t   timer_id,
        OUT         icf_error_t      *p_ecode)
{
    icf_global_port_info_st      *p_glb_port_info = ICF_NULL; 
    icf_port_timer_data_st       *p_port_timer_data = ICF_NULL;
    icf_error_t          ecode = ICF_ERROR_NONE;
    icf_msg_list_st      *p_msg_lst = ICF_NULL,
                         *p_temp_msg_lst = ICF_NULL;
    icf_timer_data_st    *p_timer_data = ICF_NULL;

#ifdef GENLIB_TIMER
    gl_error_t       gl_ecode = GW_ERROR;
    gl_timer_id_t    gl_timer_id = GL_NULL;
    gl_pvoid_t       p_timer_buf = GL_NULL;
    icf_uint16_t list_index = 0;
#endif

    p_ecode = p_ecode;

#ifdef GENLIB_TIMER
    /* SPR 18880 : Added NULL checks */
    if(ICF_NULL == p_port_info)
    {
        /* Return failure.Not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_info is NULL.\n"))
        return ICF_FAILURE;
    }
    p_glb_port_info = (icf_global_port_info_st*)p_port_info;
 
    p_port_timer_data = &(p_glb_port_info->glb_timer_data);

    if(ICF_NULL == p_port_timer_data)
    {
        /* Return failure.Not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_timer_data is NULL.\n"))
        return ICF_FAILURE;
    }
    list_index = (icf_uint16_t ) ((timer_id & 0xffff0000) >> 16); /* Extract current list_index from timer_id */

    /* Extract 16 bit timer_id returned by genlib */
    gl_timer_id = (gl_timer_id_t)(timer_id&0xffff);

    /*We are not checking for stop timer failure*/
	if(GL_FAILURE== gl_tm_stop_timer(p_port_timer_data->timer_id_list[list_index],
							gl_timer_id,
							&p_timer_buf,
							&gl_ecode))
	{
        /* SPR 18880 : Added NULL checks */
        /*Fix for SPR : 19188 (Rel 7.1.2 SPR merging for SPR 18909)*/
        /* Fix for SPR 18909 : When genlib returns failure from the above
           function, it means that the timer ID passed to genlib has 
           already been catered to, i.e. the timer has already been stopped.
           Thus we free the timer data in the following failure leg. At this point,
           p_recv_msg is NULL and it should not be checked. Otherwise this 
           function would return failure and the calling function would loop
           infinitely. Thus removing the NULL check for p_recv_msg.
        */           
        if((ICF_NULL == p_persistent_glb_pdb) ||
           (ICF_NULL == p_persistent_glb_pdb->p_msg_lst))
        {
            /* Return failure.Not enough data to proceed. */
            ICF_PRINT(((icf_uint8_t *)"[PORT] : p_persistent_glb_pdb or" \
                        " p_persistent_glb_pdb->p_msg_lst is NULL.\n"))            
            return ICF_FAILURE;
        }
		p_msg_lst = p_persistent_glb_pdb->p_msg_lst;
		p_temp_msg_lst = p_msg_lst;
		while (ICF_NULL != p_temp_msg_lst && 
                             ICF_NULL != p_temp_msg_lst->p_msg)
		{
			if ((ICF_TIMER_MSG == p_temp_msg_lst->p_msg->hdr.api_id) &&
					(p_temp_msg_lst->p_msg != p_persistent_glb_pdb->p_recv_msg))
			{
				p_timer_data = *((icf_timer_data_st **)p_temp_msg_lst->p_msg->payload);
                if(ICF_NULL != p_timer_data)
                {
				    if(timer_id == p_timer_data->timer_id)
				    {
					    p_msg_lst->p_msg_next =
					        p_temp_msg_lst->p_msg_next;
					    p_temp_msg_lst->p_msg_next = ICF_NULL;

					    if (ICF_FAILURE ==
						    icf_cmn_memfree(p_persistent_glb_pdb,
   		  				    p_temp_msg_lst->p_msg,
						    (icf_pool_id_t const)ICF_MEM_COMMON,
						    &ecode))
					    {
						    return ICF_FAILURE;
					    }
					    p_temp_msg_lst->p_msg = ICF_NULL;
					    break;
				    }
                }
			}
			p_msg_lst = p_temp_msg_lst;
			p_temp_msg_lst = p_temp_msg_lst->p_msg_next;
		}
	}
#endif
	return (ICF_SUCCESS);
} 

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_timer_init
 *
 *  DESCRIPTION     :    This function is the port function for initializing 
 *                       the timer. It attaches a call back function to the
 *                       same.
 * DESCRIPTION	:  	This function initializes the genlib timer library.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
 ***************************************************************************/

icf_return_t icf_port_timer_init(
         IN icf_void_t            *p_port_info,
         IN icf_timer_cb_fn_t     p_timer_cb_fn,
         IN icf_uint8_t           no_of_timer_list,
         IN icf_uint16_t          no_of_timers,
         INOUT icf_error_t        *p_ecode)        
{
    icf_uint32_t                 counter;
    icf_port_timer_data_st       *p_port_timer_data = ICF_NULL;
    icf_global_port_info_st      *p_glb_port_info = ICF_NULL; 
#ifdef GENLIB_TIMER
    gl_error_t                      gl_ecode = GW_ERROR;
#endif

#ifdef GENLIB_TIMER 

    /* SPR 18880 : Added NULL checks */
    if((ICF_NULL == p_port_info) ||
       (ICF_NULL == p_timer_cb_fn))
    {
        /* Return failure.Not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_info or p_timer_cb_fn is NULL.\n"))
        return ICF_FAILURE;
    }
    p_glb_port_info = (icf_global_port_info_st*)p_port_info;    
    
    p_port_timer_data = &(p_glb_port_info->glb_timer_data);

    if(ICF_NULL == p_port_timer_data)
    {
        /* Return failure.Not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_timer_data is NULL.\n"))
        return ICF_FAILURE;
    }

    if (ICF_MAX_TIMER_LIST_COUNT < no_of_timer_list )
    {
        *p_ecode = ICF_ERROR_TMR_INIT;
        return ICF_FAILURE;
    }
   
    /*gl_tm_set_timer_step(ICF_SELECT_TIMER,ICF_DELTA_PROCESSING_TIMER); */
    for( counter = 0;counter < no_of_timer_list; counter++)

    {
        if(GL_FAILURE == gl_tm_timer_list_create(ICF_PORT_CMN_TASK_ID,
                    no_of_timers,
                    (gl_timer_list_t*)(&(p_port_timer_data->timer_id_list[counter])),
                    &gl_ecode))
        {
            *p_ecode = ICF_ERROR_TMR_INIT;
            return ICF_FAILURE;
        }
    }
    p_port_timer_data->icf_genlib_timer_cb_fn = p_timer_cb_fn;

    /*
     * Init the current index. this is used to rotate through the lists of timers
     * so that the timers are allocated with even distribution across the lists
     */
    p_port_timer_data->current_list_index = 0;

#endif

    return ICF_SUCCESS;
}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_timer_deinit
 *
 *  DESCRIPTION     :    This function is the port function for initializing 
 *                       the timer. It attaches a call back function to the
 *                       same.
 * DESCRIPTION	:  	This function initializes the genlib timer library.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
***************************************************************************/

icf_return_t icf_port_timer_deinit(
        IN icf_void_t            *p_port_info,
        OUT   icf_error_t *p_ecode)
{
#ifdef GENLIB_TIMER
	if(ICF_NULL != p_port_info)
	{
		gl_error_t                      gl_ecode = GW_ERROR;
		icf_uint8_t timer_list_count=ICF_NULL;
		icf_global_port_info_st      *p_glb_port_info = 
			(icf_global_port_info_st*)p_port_info;    
		
		icf_port_timer_data_st *p_port_timer_data =
			&(p_glb_port_info->glb_timer_data);

        /* SPR 18880 : Added NULL checks */
        if(ICF_NULL == p_port_timer_data)
        {
            /* Return failure.Not enough data to proceed. */
            ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_timer_data is NULL.\n"))
            return ICF_FAILURE;
        }
		
		for(timer_list_count=0;timer_list_count<ICF_MAX_TIMER_LIST_COUNT;
		timer_list_count++) 
		{
			if(ICF_NULL != 
				p_port_timer_data->timer_id_list[timer_list_count])
			{
				if(GL_FAILURE ==  gl_tm_delete_timer_list(
					p_port_timer_data->timer_id_list[timer_list_count] ,&gl_ecode))
				{
					*p_ecode = ICF_ERROR_TMR_INIT;
					return ICF_FAILURE;
				}
			}
		}
	}

#endif
    return ICF_SUCCESS;
}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_check_timer_expiry
 *
 *  DESCRIPTION     :    This function loops over all the timer lists
 *                      and checks the timers for expiry
 *  RETURNS         :    Nothing 
 *************************************************************************/

icf_void_t 
icf_port_check_timer_expiry(
        INOUT      icf_void_t       *p_port_info,
        OUT     icf_error_t      *p_ecode)
{
    icf_uint32_t                 counter = 0;
    icf_global_port_info_st      *p_glb_port_info = ICF_NULL;
    icf_port_timer_data_st       *p_port_timer_data = ICF_NULL;

    p_ecode = p_ecode;

#ifdef GENLIB_TIMER 
    /* SPR 18880 : Added Null checks */
    if(ICF_NULL == p_port_info)
    {
        /* Return failure. Not enough data to proceed. */
         ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_inf is NULL.\n"))
         return;
    }
    p_glb_port_info = (icf_global_port_info_st*)p_port_info;

    p_port_timer_data = &(p_glb_port_info->glb_timer_data);
    if(ICF_NULL == p_port_timer_data)
    {
        /* Return failure. Not enough data to proceed. */
        ICF_PRINT(((icf_uint8_t *)"[PORT] : p_port_timer_data is NULL.\n"))
        return;
    }

    for( counter = 0;counter < ICF_MAX_TIMER_LIST_COUNT; counter++)
    {
        gl_tm_check_timer_expiry (p_port_timer_data->timer_id_list[counter]);
    }
#endif
}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_get_current_time
 *
 *  DESCRIPTION     :    This function gets the current time in seconds
 *  RETURNS         :    Nothing 
 *************************************************************************/

icf_return_t 
icf_port_get_current_time(
		OUT		icf_time_t		 *p_curr_time)
{
	gl_time_stamp_t           timeval;

#ifdef GENLIB_TIMER 
    gl_tm_get_timeval(&timeval);
	*p_curr_time = timeval.sec;
#endif

	return ICF_SUCCESS;
}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_port_get_current_timestamp
 *
 *  DESCRIPTION     :    This function gets the current time in seconds
 *  RETURNS         :    Nothing
 *************************************************************************/

icf_return_t
icf_port_get_current_timestamp(
                OUT             icf_time_stamp_st                *p_curr_time)
{
        gl_time_stamp_t           timeval;

#ifdef GENLIB_TIMER
    gl_tm_get_timeval(&timeval);
        p_curr_time->time_in_secs = timeval.sec;
        p_curr_time->time_in_millisecs = (icf_uint32_t)(timeval.nsec / 1000000);

#endif

        return ICF_SUCCESS;
}

