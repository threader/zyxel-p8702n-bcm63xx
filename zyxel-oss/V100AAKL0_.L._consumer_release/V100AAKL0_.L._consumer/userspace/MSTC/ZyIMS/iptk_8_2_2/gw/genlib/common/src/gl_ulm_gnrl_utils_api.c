/*****************************************************************************
* FUNCTION    : Source file for Generic library - Common TimeStamp and Memory
*               Utilities implementation.
******************************************************************************
*
* FILENAME    : gl_ulm_gnrl_utils_api.c
*
* DESCRIPTION : This file implements string manipulation functionality of 
*               generic libraries. 
*
* DATE            NAME         REFERENCE        REASON
*----------   -------------    ---------    -------------------
* April'01      Rajesh Jain                 New file
*                                           Generic Libraries
* July' 01      Shobhit Bansal              Functions for case 
*                                           insensitive string compare
*                                           added
* 18/10/2001    Ravi Jindal     SPR-2788    Packaging of Genlib-Code.
* 19/10/2001    Ravi Jindal     SPR-2802    Adding Pre-ANSI-C features.
* 17/11/2001    Ravi Jindal     SPR-3107    Incoprate the defects and change
*                                           cosmmetics of file.
*
* 03/04/2002    Ravi Jindal     SPR-3959    Enhancements for Rel-1.0.
*
* 12/07/2004    Navneet Gupta   SPR 6946    QNX port
* 28/03/2008    Shruti Thakral  SPR 18248   Klocwork warning removal
* 22/04/2008    Alok Tiwari     SPR 18456   Klocwork warning removal
* 24/04/2008    Alok Tiwari     SPR 18456   Klocwork warning removal
* 
* COPYRIGHT , Aricent, 2006
*
*****************************************************************************/


#include "gl_ulm_interfaces.h"


/*****************************************************************************
* Function name    : gl_mm_memcpy 
* Arguments        : gw_U8bit           *p_dest_buffer, 
                     gw_U8bit           *p_source_buffer, 
                     gw_U32bit          memory_size
* Return Value     : void
* Functionality    : Copy memory form source to destination.
* Functions called : memcpy
*****************************************************************************/
void    
gl_mm_memcpy
     
#ifdef _ANSI_C_
    ( gw_U8bit                  *p_dest_buffer, 
      gw_U8bit                  *p_source_buffer, 
      gw_U32bit                 memory_size
    )
#else
    ( p_dest_buffer, p_source_buffer, memory_size )
      gw_U8bit                  *p_dest_buffer; 
      gw_U8bit                  *p_source_buffer; 
      gw_U32bit                 memory_size;
#endif
    
/****************************************************************************/
{
    /*
     * For Win 95 and NT include LIBC.LIB (Single thread _STATIC_  library) 
     * or LIBCMT.LIB (Multithread _STATIC_  library).
     */
    memcpy( p_dest_buffer, p_source_buffer, memory_size);
}


/*****************************************************************************
* Function name    : gl_mm_memset 
* Arguments        : gw_U8bit           *p_dest_buffer, 
                     gw_U8bit           rep_char, 
                     gw_U32bit          memory_size
* Return Value     : void
* Functionality    : Set memory with a char.
* Functions called : memset
*****************************************************************************/
void    
gl_mm_memset

#ifdef _ANSI_C_
    ( gw_U8bit                  *p_dest_buffer, 
      gw_U8bit                  rep_char, 
      gw_U32bit                 memory_size
    )
#else
    ( p_dest_buffer, rep_char, memory_size )
      gw_U8bit                  *p_dest_buffer; 
      gw_U8bit                  rep_char; 
      gw_U32bit                 memory_size;
#endif
    
/****************************************************************************/
{
    /*
     * For Win 95 and NT include LIBC.LIB (Single thread _STATIC_  library) 
     * or LIBCMT.LIB (Multithread _STATIC_  library).
     */
    memset( p_dest_buffer, rep_char, memory_size);
}





/*****************************************************************************
* Function name    : gl_tm_get_timeval
* Arguments        : gl_time_stamp_t *p_timeval
* Return Value     : gl_return_t
* Functionality    : Gets the current time stamp from the OS. It gives 
*                    the number of seconds elapced since Jan 1, 1970 and 
*                    gives the remainder in milliseconds. The current time
*                    stamp is strored in the memory location pointed by the
*                    argument to this function.
*                    This function returns GL_SUCCESS or GL_FAILURE.
* Functions called :
*****************************************************************************/
gl_return_t
gl_tm_get_timeval

#ifdef _ANSI_C_
    ( gl_time_stamp_t           *p_timeval )
#else
    ( p_timeval )
      gl_time_stamp_t           *p_timeval;
#endif
    
/****************************************************************************/
{
    gl_return_t                 ret_val = GL_SUCCESS;

#ifdef GL_VXWORKS
    struct timespec             ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    
    if(GL_TM_MAX_SYSTEM_TIME_IN_SECS < ts.tv_sec)
    {
        p_timeval->sec = (ts.tv_sec % GL_TM_MAX_SYSTEM_TIME_IN_SECS);
    }
    else
    {
        p_timeval->sec = ts.tv_sec;
    }
    p_timeval->nsec = ts.tv_nsec;
    
#elif GL_WINNT
    struct _timeb               ts;
    _ftime(&ts);
    
    if(GL_TM_MAX_SYSTEM_TIME_IN_SECS < ts.time)
    {
        p_timeval->sec = (ts.time % GL_TM_MAX_SYSTEM_TIME_IN_SECS);
    }
    else
    {
        p_timeval->sec = ts.time;
    }
    p_timeval->nsec = ts.millitm * 1000000;
    
#elif defined(GL_SOLARIS)  || defined(GL_LINUX) || defined(GL_QNX)
    struct timeval              tp;
    
    if(gettimeofday(&tp, GL_NULL) != -1)
    {
        if(GL_TM_MAX_SYSTEM_TIME_IN_SECS < (gw_U32bit)tp.tv_sec)
        {
           p_timeval->sec = ((gw_U32bit)tp.tv_sec % 
                                         GL_TM_MAX_SYSTEM_TIME_IN_SECS);
        }
        else
        {
           p_timeval->sec = tp.tv_sec;
        }
        p_timeval->nsec = tp.tv_usec * 1000;
    }
    else 
    {
        ret_val = GL_FAILURE;
    }
    
#elif GL_OSE
    struct TimePair            tp;
    rtc_get_time(&tp);
     
    if(GL_TM_MAX_SYSTEM_TIME_IN_SECS < tp.seconds)
    {
       p_timeval->sec = (tp.seconds % GL_TM_MAX_SYSTEM_TIME_IN_SECS);
    }
    else
    {
       p_timeval->sec = tp.seconds;
    }
    p_timeval->nsec = tp.micros * 1000;
     
#endif
     
    return (ret_val);  
}


/*****************************************************************************
* Function name    : gl_tm_get_abs_time
* Arguments        : gw_U8bit           time_flag,
*                    gl_time_stamp_ut   *p_time_stamp
* Return Value     : gw_void_t
* Functionality    : Returns the wall clock time to an accuracy of 1 ms.
* Functions called :
*****************************************************************************/
void
gl_tm_get_abs_time

#ifdef _ANSI_C_
    ( gw_U8bit                  time_flag,
      gl_time_stamp_ut          *p_time_stamp
    )
#else
    ( time_flag, p_time_stamp )
      gw_U8bit                  time_flag;
      gl_time_stamp_ut          *p_time_stamp;
#endif
    
/****************************************************************************/
{
#ifndef GL_OSE    
    if(time_flag == GL_TIME_STAMP_UTC_STR )
    {
        if(p_time_stamp != GL_NULL)
        {
            time_t              ltime;
            struct tm           *p_today;
            
#if defined(GL_LINUX) || defined(GL_QNX)
            ctime(&ltime);
#else
            time(&ltime);
#endif
            p_today = (struct tm *)localtime(&ltime);
            if (GL_NULL != p_today)
            {
                strftime((gw_S8bit*)p_time_stamp->p_utc_time_str->date,9,
                        "%Y%m%d",p_today);
                strftime((gw_S8bit*)p_time_stamp->p_utc_time_str->time,7,
                        "%H%M%S",p_today);
            }
            p_time_stamp->p_utc_time_str->time[6] ='0';
            p_time_stamp->p_utc_time_str->time[7] = '0';
            p_time_stamp->p_utc_time_str->time[8] = '\0';
            p_time_stamp->p_utc_time_str->date[8] = '\0';
        } 

    }
    else
    {
        if(time_flag == GL_TIME_STAMP_IN_NS)
        {
            gl_return_t         retval;
            
            retval = gl_tm_get_timeval(&(p_time_stamp->time_in_ns));
                      
        }
             
    }
#endif
}



/*****************************************************************************
* Function name    : gl_tm_convert_sec_to_date
* Arguments        : gl_time_stamp_t    *p_time_stamp,
*                    gl_time_notation_t *p_date_time
* Return Value     : mg_void_t
* Functionality    : Returns the wall clock time to an accuracy of 1 ms.
* Functions called :
*****************************************************************************/
void
gl_tm_convert_sec_to_date

#ifdef _ANSI_C_
    ( gl_time_stamp_ut          *p_time_stamp, 
      gl_time_notation_t        *p_date_time
    )
#else
    ( p_time_stamp, p_date_time )
      gl_time_stamp_ut          *p_time_stamp; 
      gl_time_notation_t        *p_date_time;
#endif
    
/****************************************************************************/
{
#ifndef GL_OSE
    if(p_time_stamp != GL_NULL)
    {
        time_t                  ltime;
        struct tm               *p_today;
        
        ltime = p_time_stamp->time_in_ns.sec;
        p_today = (struct tm *)localtime(&ltime);
        /*Klocwork warning removal*/
        if(GL_NULL != p_today)
        {
           strftime((gw_S8bit*)p_date_time->date,9,"%Y%m%d",p_today);
           strftime((gw_S8bit*)p_date_time->time,7,"%H%M%S",p_today);
        }
        p_date_time->time[6] = '0';
        p_date_time->time[7] = '0';
        p_date_time->time[8] = '\0';
        p_date_time->date[8] = '\0';
    } 
#endif
}




/*****************************************************************************
* Function name    : gl_tm_comp_time
* Arguments        : gl_time_stamp_t   *p_time1,
*                    gl_time_stamp_t   *p_time2
* Return Value     : gw_S8bit
* Functionality    : Returns 0 if time1 is equal to time2 ,
*                    if time1 is greater than time2 returns 1 and 
*                    returns -1 if time1 is less than time2
* Functions called :
*****************************************************************************/
gw_S8bit gl_tm_comp_time

#ifdef _ANSI_C_
    ( gl_time_stamp_t           *p_time1,
      gl_time_stamp_t           *p_time2
    )
#else
    ( p_time1, p_time2 )
      gl_time_stamp_t           *p_time1;
      gl_time_stamp_t           *p_time2;
#endif
    
/****************************************************************************/
{
    
    if(p_time1->sec > p_time2->sec)
    {
          return(1);
    }
    else
    {
        if(p_time1->sec == p_time2->sec)
        {
            if(p_time1->nsec > p_time2->nsec)
            {
                return(1);
            }
            else
            {
                if(p_time1->nsec < p_time2->nsec)
                {
                    return(-1);
                }
                else
                {
                    return(0);
                }
            }
        }
        else
        {
            return(-1);
            
        }
    }
}




/*****************************************************************************
* Function name    : gl_tm_get_relative_time_diff
* Arguments        : gl_time_stamp_t  *p_new_time,  
*                    gl_time_stamp_t  *p_old_time, It is timestamp taken before 
*                                                  new-timestamp.
*                    gl_time_stamp_t  *p_diff_time
* Return Value     : void
* Functionality    : Finds the difference between old_time and new_time and 
*                    stores in diff_time. This function gets the difference 
*                    between two timestamps.
* Functions called :
*****************************************************************************/
void 
gl_tm_get_relative_time_diff

#ifdef _ANSI_C_
    ( gl_time_stamp_t           *p_new_time,
      gl_time_stamp_t           *p_old_time,
      gl_time_stamp_t           *p_diff_time
    )
#else
    ( p_new_time, p_old_time, p_diff_time )
      gl_time_stamp_t           *p_new_time;
      gl_time_stamp_t           *p_old_time;
      gl_time_stamp_t           *p_diff_time;
#endif
    
/****************************************************************************/
{
    gw_S8bit                    retval;
    
    if(p_diff_time != GL_NULL)
    {
        retval = gl_tm_comp_time(p_new_time, p_old_time);
        
        /*
         * Difference in Seconds.
         */ 
        if((retval == 1) || (retval == 0))
        {
            p_diff_time->sec = p_new_time->sec - p_old_time->sec;
        }
        else
        {
            /*
             * This is a case of Roll Over in timestamp Value.
             */
            p_diff_time->sec = (GL_TM_MAX_SYSTEM_TIME_IN_SECS) - p_old_time
                ->sec + p_new_time->sec;
        }
        
        /*
         * Difference in Nano-Seconds is independent of Rollover Case.
         */ 
        if(p_new_time->nsec >= p_old_time->nsec)
        {
            p_diff_time->nsec = p_new_time->nsec - p_old_time->nsec;           
        }
        else
        {
            p_diff_time->nsec = p_old_time->nsec - p_new_time->nsec;
            p_diff_time->nsec = 1000000000 - p_diff_time->nsec;
            p_diff_time->sec--;
        }
    }
}




/*****************************************************************************
* Function name    : gl_tm_diff_time
* Arguments        : gl_time_stamp_t    *p_time1,
*                    gl_time_stamp_t    *p_time2,
*                    gl_time_stamp_t    *p_diff_time
* Return Value     : void
* Functionality    : Finds the difference between time1 and time2 and stores in 
*                    p_time
* Functions called :
*****************************************************************************/
void gl_tm_diff_time

#ifdef _ANSI_C_

    (gl_time_stamp_t           *p_time1,
     gl_time_stamp_t           *p_time2,
     gl_time_stamp_t           *p_diff_time)
#else
    (p_time1, p_time2, p_diff_time)

      gl_time_stamp_t           *p_time1;
      gl_time_stamp_t           *p_time2;
      gl_time_stamp_t           *p_diff_time;
#endif
    
/****************************************************************************/
{
    gw_S8bit                    retval;
    
    if(p_diff_time != GL_NULL)
    {
        retval = gl_tm_comp_time(p_time1,p_time2);
        if((retval == 1) || (retval == 0))
        {
            p_diff_time->sec = p_time1->sec - p_time2->sec;
            if(p_time1->nsec >= p_time2->nsec)
            {
                p_diff_time->nsec = p_time1->nsec - p_time2->nsec;           
            }
            else
            {
                p_diff_time->nsec = p_time2->nsec - p_time1->nsec;
                p_diff_time->nsec = 1000000000 - p_diff_time->nsec;
                p_diff_time->sec--;
            }
        }
        else
        {
            p_diff_time->sec = p_time2->sec - p_time1->sec;
            if(p_time2->nsec >= p_time1->nsec)
            {
                p_diff_time->nsec = p_time2->nsec - p_time1->nsec;
            }
            else
            {
                p_diff_time->nsec = p_time1->nsec - p_time2->nsec;
                p_diff_time->nsec = 1000000000 - p_diff_time->nsec;
                p_diff_time->sec--;
            }
        }
    }
}




/*****************************************************************************
* Function name    : gl_tm_add_time
* Arguments        : gl_time_stamp_t    *p_time1,
*                    gl_time_stamp_t    *p_time2,
*                    gl_time_stamp_t    *p_total_time
* Return Value     : void
* Functionality    : Adds time1 and time2 and stores in p_time
* Functions called :
*****************************************************************************/
void gl_tm_add_time

#ifdef _ANSI_C_
    ( gl_time_stamp_t           *p_time1,
      gl_time_stamp_t           *p_time2,
      gl_time_stamp_t           *p_total_time
    )
#else
    (p_time1, p_time2, p_total_time)
      gl_time_stamp_t           *p_time1;
      gl_time_stamp_t           *p_time2;
      gl_time_stamp_t           *p_total_time;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   nsec_val;
    gw_U32bit                   total_sec_val;

    if(p_total_time != GL_NULL)
    {
        p_total_time->sec = p_time1->sec + p_time2->sec;
        nsec_val = p_time1->nsec + p_time2->nsec;

        total_sec_val = p_total_time->sec + (nsec_val/1000000000);

        if(GL_TM_MAX_SYSTEM_TIME_IN_SECS >= total_sec_val)
        {
            p_total_time->sec = total_sec_val;
        }
        else
        {
            p_total_time->sec = (total_sec_val % GL_TM_MAX_SYSTEM_TIME_IN_SECS);
        }
        if(nsec_val >= 1000000000)
        {
            p_total_time->nsec = nsec_val - 1000000000;
        }
        else
        {
            p_total_time->nsec = nsec_val;
        }
    }
}
