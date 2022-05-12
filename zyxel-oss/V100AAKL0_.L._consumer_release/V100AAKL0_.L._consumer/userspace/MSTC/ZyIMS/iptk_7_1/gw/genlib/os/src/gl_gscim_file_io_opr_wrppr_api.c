/*****************************************************************************
* 
* FUNCTION    : Source file for Generic Library - File manager implementation
* 
******************************************************************************
*
* FILENAME    : gl_gscim_file_io_opr_wrppr_api.c
*
* DESCRIPTION : This file implements file management functionality of generic 
*               libraries. 
*               
*
* DATE              NAME        REFERENCE       REASON
* ----------    ------------    ----------  ---------------------
*
* April'01       Rajesh Jain    New file    Generic Libraries
* 04/10/2001     Ravi Jindal    SPR-2751    Remove warnings with gcc,cc,CC
*                                           on Solaris. 
* 18/10/2001     Ravi Jindal    SPR-2788    Packaging of Genlib-Code. 
* 19/10/2001     Ravi Jindal    SPR-2802    Adding Pre-ANSI-C features.
* 17/11/2001     Ravi Jindal    SPR-3107    Incoprate the defects and change
*                                           cosmmetics of file.       
*
* 02/04/2002     Ravi Jindal    SPR-3959    Enhancements for Rel-1.0.
*        
*COPYRIGHT , Aricent, 2006
*****************************************************************************/

#include "gl_gscim_interfaces.h"

#ifndef GL_OSE

/*****************************************************************************
* Function Name    : gl_fm_open_file
* Arguments        : gw_S8bit           *filename,
*                    gl_fm_mode_et      mode,
*                    gl_error_t         *p_ecode    
* Return value     : gl_pvoid_t
* Functionality    : Open a file in a specified mode.
* Functions called : 
*****************************************************************************/
gl_pvoid_t
gl_fm_open_file

#ifdef _ANSI_C_
    ( gw_S8bit                  *filename, 
      gl_fm_mode_et             mode,
      gl_error_t                *p_ecode    
    )
#else
    ( filename, mode, p_ecode )
      gw_S8bit                  *filename;
      gl_fm_mode_et             mode;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    FILE                        *p_file;
    gw_S8bit                    *file_mode;

    switch(mode)
    {
        case GL_FM_RDONLY:
            file_mode=(gw_S8bit *)"r";
            break;
        case GL_FM_WRONLY:
            file_mode=(gw_S8bit *)"w";
            break;
        case GL_FM_APONLY:
            file_mode=(gw_S8bit *)"a";
            break;
        case GL_FM_RDWR_EXIST:
            file_mode=(gw_S8bit *)"r+";
            break;
        case GL_FM_RDWR_NEW:
            file_mode=(gw_S8bit *)"w+";
            break;
        case GL_FM_RDAP:
            file_mode=(gw_S8bit *)"a+";
            break;
        case GL_FM_RDONLY_BINARY:
            file_mode=(gw_S8bit *)"rb";
            break;
        case GL_FM_WRONLY_BINARY:
            file_mode=(gw_S8bit *)"wb";
            break;
        case GL_FM_APONLY_BINARY:
            file_mode=(gw_S8bit *)"ab";
            break;
        case GL_FM_RDWR_EXIST_BINARY:
            file_mode=(gw_S8bit *)"r+b";
            break;
        case GL_FM_RDWR_NEW_BINARY:
            file_mode=(gw_S8bit *)"w+b";
            break;
        case GL_FM_RDAP_BINARY:
            file_mode=(gw_S8bit *)"a+b";
            break;    
        default:
            *p_ecode = GL_FM_ERR_INV_FILE_MODE;
            return GL_NULL;
    }
    
    p_file = fopen(filename, file_mode);
    return p_file;
}


/*****************************************************************************
* Function Name    : gl_fm_close_file
* Arguments        : gl_pvoid_t         fd,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Closes a file. 
* Functions called :
*****************************************************************************/
gl_return_t gl_fm_close_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                fd,
      gl_error_t                *p_ecode
    )
#else
    ( fd, p_ecode )
      gl_pvoid_t                fd;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }
    if(EOF != fclose((FILE*)fd))
    {
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}



/*****************************************************************************
* Function Name    : gl_fm_seek_file
* Arguments        : gl_pvoid_t         fd,
*                    gw_S32bit          offset,
*                    gl_fm_origin_et    origin,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Moves the file pointer to a specified location.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_fm_seek_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                fd, 
      gw_S32bit                 offset, 
      gl_fm_origin_et           origin,
      gl_error_t                *p_ecode
    )
#else
    ( fd, offset, origin, p_ecode )
      gl_pvoid_t                fd;
      gw_S32bit                 offset;
      gl_fm_origin_et           origin;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_S32bit                   ptrname;
    
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }

    switch(origin)
    {
        case GL_FM_BEGIN:
            ptrname = SEEK_SET;
            break;
        case GL_FM_CURR:
            ptrname = SEEK_CUR;
            break;
        case GL_FM_END:
            ptrname = SEEK_END;
            break; 
        default:
            *p_ecode = GL_FM_ERR_INV_SEEK_ORIGIN;
            return GL_FAILURE;
    }
    if(GL_NULL==fseek((FILE*)fd, offset, ptrname))
    {
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}



/*****************************************************************************
* Function Name    : gl_fm_read_file
* Arguments        : gl_pvoid_t         buffer,
*                    gw_U32bit          size,
*                    gw_U32bit          count,
*                    gl_pvoid_t         fd,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Reads data from a file.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_fm_read_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buffer, 
      gw_U32bit                 size,
      gw_U32bit                 count,
      gl_pvoid_t                fd,
      gl_error_t                *p_ecode
    )
#else
    ( p_buffer, size, count, fd, p_ecode )
      gl_pvoid_t                p_buffer;
      gw_U32bit                 size;
      gw_U32bit                 count;
      gl_pvoid_t                fd;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   num_item;
    
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_buffer)
    {
        *p_ecode = GL_FM_ERR_NULL_BUFFER_PTR;
        return GL_FAILURE;
    }
    
    num_item = fread(p_buffer, size, count, (FILE*)fd);
    
    if((GL_NULL == num_item) || (num_item < count))
    {
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_fm_write_file
* Arguments        : gl_pvoid_t         buffer,
*                    gw_U32bit          size,
*                    gw_U32bit          count,
*                    gl_pvoid_t         fd,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Writes data to a file.
* Functions called :
*****************************************************************************/
gl_return_t 
gl_fm_write_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                p_buffer,
      gw_U32bit                 size,
      gw_U32bit                 count,
      gl_pvoid_t                fd,
      gl_error_t                *p_ecode
    )
#else
    ( p_buffer, size, count, fd, p_ecode )
      gl_pvoid_t                p_buffer; 
      gw_U32bit                 size;
      gw_U32bit                 count;
      gl_pvoid_t                fd;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    gw_U32bit                   num_item;
    
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL == p_buffer)
    {
        *p_ecode = GL_FM_ERR_NULL_BUFFER_PTR;
        return GL_FAILURE;
    }
    num_item = fwrite(p_buffer, size, count, (FILE*)fd);
    
    if((GL_NULL == num_item) || (num_item < count))
    {
        return GL_FAILURE;
    }
    return GL_SUCCESS;
}



/*****************************************************************************
* Function Name    : gl_fm_end_of_file
* Arguments        : gl_pvoid_t         fd,
*                    gl_error_t         *p_ecode
* Return value     : gl_boolean_t
* Functionality    : Tests for end-of-file on a file.
* Functions called :
*****************************************************************************/
gl_boolean_t 
gl_fm_end_of_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                fd,
      gl_error_t                *p_ecode
    )
#else
    ( fd, p_ecode )
      gl_pvoid_t                fd;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL != feof((FILE*)fd))
    {
        return GL_TRUE;
    }
    return GL_FALSE;
}
    


/*****************************************************************************
* Function Name    : gl_fm_flush_file
* Arguments        : gl_pvoid_t         fd,
*                    gl_error_t         *p_ecode
* Return value     : gl_return_t
* Functionality    : Flushes a file.
* Functions called :
*****************************************************************************/
gl_return_t    
gl_fm_flush_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                fd,
      gl_error_t                *p_ecode
    )
#else
    ( fd, p_ecode )
      gl_pvoid_t                fd;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL==fflush((FILE*)fd))
    {
        return GL_SUCCESS;
    }
    return GL_FAILURE;
}



/*****************************************************************************
* Function Name    : gl_fm_error_in_file
* Arguments        : gl_pvoid_t         fd,
*                    gl_error_t         *p_ecode
* Return value     : gl_boolean_t
* Functionality    : Tests for an error on a file.
* Functions called :
*****************************************************************************/
gl_boolean_t 
gl_fm_error_in_file

#ifdef _ANSI_C_
    ( gl_pvoid_t                fd,
      gl_error_t                *p_ecode
    )
#else
    ( fd, p_ecode )
      gl_pvoid_t                fd;
      gl_error_t                *p_ecode;
#endif
    
/****************************************************************************/
{
    if(GL_NULL == fd)
    {
        *p_ecode = GL_FM_ERR_NULL_FILE_PTR;
        return GL_FAILURE;
    }
    if(GL_NULL != ferror((FILE*)fd))
    {
        return GL_TRUE;
    }
    return GL_FALSE;
}

#endif /* Endif for GL_OSE not defined */
