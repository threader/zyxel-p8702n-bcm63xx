/*****************************************************************************

 **

 ** FILENAME:        icf_es_main.c

 **

 ** DESCRIPTION:    This is the main file of the ICF Application. It 

 **                    initializes the toolkit and relevant data structures and 

 **                    then spawns the various threads to execute the UA.

 **

 ** DATE        NAME            REFERENCE    REASON

 ** ----        ----            ---------   ------

 ** 08-Dec-2005 Rohit Aggarwal  ICF 4.0 CDD Coding for IPSec support

 ** 08-Feb-2006  Amit Sharma               Network PDU Logging API from ICF

 ** 29-Nov-2006 Deepti Goyal    ICF 6.2  ES Changes for NAT Client support
 ** 13-Jan-2007 Umang Singh     SPR 14035
 ** 13-Jan-2007 Umang Singh     SPR 14038
 ** 01-Dec-2008 Vinay Jindal    SPR 19220 Memset for DNS purge timer removed
 ** 09-Jul-2009  Rohit Aggarwal  SPR 19772   Changes for Big-endian Linux
 ** 
 *****************************************************************************

 **            Copyrights 2008, Aricent.

 *****************************************************************************/



#ifdef __cplusplus

extern "C" {

#endif 





#include "icf_es_prototypes.h"

#include "icf_es_macro.h"

#include "icf_common_defs.h"

#include "icf_macro.h"



#ifdef ICF_PORT_WINDOWS

#include <windows.h>
#pragma warning ( disable : 4702 )

#endif



#include "icf_port_struct.h"    

#include "icf_port_intf_prototypes.h"

#include "icf_port_macros.h"    



ICF_INIT_START

#include "icf_internal_api_id.h"

#include "icf_common_types.h"



ICF_INIT_END



#if !defined(ICF_PORT_WINDOWS) && !defined(SDF_OSE)

#include <unistd.h>

#endif





#if !defined(ICF_PORT_WINDOWS) && !defined(SDF_VXWORKS) && !defined(ICF_PORT_VXWORKS)

#include <sys/time.h>

#endif



#if defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)

#ifndef ICF_PORT_VXWORKS
#include <strings.h>
#endif

#include <fcntl.h>

#include <string.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <netdb.h>

#include <signal.h>

#include <stdio.h>

#endif




#ifdef ICF_PORT_VXWORKS
#include<sys/times.h>
#include<icf_port_defs.h>
#else
#include <time.h>
#endif


#ifndef ICF_PORT_WINCE

#include <sys/stat.h>

#endif

#include <ctype.h>

#ifdef ICF_NAT_MEDIA_TRAVERSAL
#include "nat_glb_mgmt_prototypes.h"
#endif

#ifdef ICF_DEBUG_MEMPOOL

FILE            *p_memget_file_ptr;

/* Commented : Not required Now

FILE            *p_mem_free_file_ptr;

FILE            *p_mem_cpy_file_ptr;

*/

#endif

#ifdef ICF_PORT_VXWORKS
icf_ftp_info_st g_icf_ftp_info;
icf_ramdisk_param_st g_icf_ramdisk_param;
icf_boolean_t    g_icf_is_ramdisk_present = ICF_FALSE;
#endif

#if defined(ICF_TRACE_ENABLE) && defined(ICF_PORT_WINDOWS)
FILE			*p_log_file_ptr;
extern icf_uint32_t				dMaxLogFileSize = 150 * 1024;
#endif


ICF_INIT_START

/* Global variable - to store parameters from configuration file */    

icf_init_param_st    icf_init_param;



/* global variable to store IPC info related to MM */

icf_es_mm_info_st    icf_es_mm_param;

icf_es_mm_info_st    *p_icf_es_mm_param=&icf_es_mm_param;

#ifdef ICF_NAT_MEDIA_TRAVERSAL
/* global variable to store IPC info related to NAT Client Library */

icf_es_nat_info_st    icf_es_nat_param;

icf_es_nat_info_st    *p_icf_es_nat_param=&icf_es_nat_param;

nat_glb_pdb_st        *p_nat_glb_pdb = ICF_NULL;
#endif

icf_uint16_t		 recv_from_app_port;

icf_string_st		 icf_ipc_self_ip;

ICF_INIT_END





extern icf_void_t icf_port_print_mem_pool_stats(icf_void_t);
#if 1 /* ZyXEL porting */
icf_void_t icf_register_signal(icf_void_t);
#endif


/*****************************************************************************

 * FUNCTION:        IFIN_SIP_Init/main

 *

 * DESCRIPTION:    The main function of the IP Phone Toolkit

 *

 * The PA will call this function to spawn the SIP CC.

 * The Spawned process will then send back a response to PA through

 * FIFO pipe indicating successful INIT

 *

 *****************************************************************************/

#ifndef ICF_SSA_ES_UT

#if defined(ICF_PORT_SOLARIS) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)



#ifdef ICF_PA_TRIGGERED

/*

 * The PA will call this function to spawn the SIP CC.

 * The Spawned process will then send back a response to PA through

 * FIFO pipe indicating successful INIT

 */

icf_uint32_t    IFIN_SIP_Init()

#else



#ifdef ICF_PORT_WINDOWS

#ifndef _CONSOLE

int

WINAPI

WinMain(

    HINSTANCE hInstance,

    HINSTANCE hPrevInstance,

    LPSTR lpCmdLine,

    int nShowCmd

    )

#else

icf_int32_t main(icf_int32_t argc, icf_int8_t *argv[])

#endif

#endif /*PORT_WINDOWS*/



#ifdef ICF_PORT_LINUX

icf_int32_t main(icf_int32_t argc, icf_int8_t *argv[])

#endif /*PORT_LINUX*/

#ifdef ICF_PORT_VXWORKS
icf_int32_t icf_main()
#endif


#endif

#endif

{

    

    

    icf_return_t    ret_val = ICF_SUCCESS;



    /*

     *  This variable is passed to MRM during init time 

     * and later returned to MRM during process_msg

     */

    

    icf_void_t				*p_icf_glb_data  = ICF_NULL;

	icf_init_param_st		*p_icf_init_param = ICF_NULL;



ICF_INIT_START
	
#ifdef ICF_VERIZON
	char						 filename[ICF_CONF_MAX_LINE_SIZE] ; 
	HKEY key;
	unsigned char				filename_unicode[256],filepath[256];
	unsigned long			App	len=sizeof(filename_unicode);
#else

#ifdef ICF_PORT_WINCE
	char						 filename[ICF_CONF_MAX_LINE_SIZE] = "\\icf-6\\cfg\\icf.cfg";
#else 
	 char						 filename[ICF_CONF_MAX_LINE_SIZE] = "/bin/icf.cfg"; 
#endif
#endif

    char						 temp_str[ICF_CONF_MAX_LINE_SIZE] = "\0",

                                    val_str[ICF_CONF_MAX_LINE_SIZE] = "\0"; 

    FILE *                       fp = ICF_NULL; 

    icf_conf_file_st			 conf_file; 

    icf_int32_t					 count = 0, value, num_elements = 0, invalid_range = ICF_FALSE, null_val_present = ICF_FALSE; /* temp variables */

    icf_uint16_t            bit_mask=0; 




    icf_error_t             ecode; 

    icf_route_info_st       generic_rt_info;

    icf_route_info_st       *p_generic_rt_info = &generic_rt_info;





ICF_INIT_END

       

#ifdef ICF_TRACE_REDIRECT

    FILE* p_log_file;

#endif            




#ifndef ICF_PORT_VXWORKS
#ifndef ICF_PA_TRIGGERED

#ifdef _CONSOLE

	(void)argv;

	(void)argc;

#endif
#endif

#endif            

    ICF_INIT_ERROR_TRACE(("\n[ENTERED]:ICF_ES\n"));

#if 1 /*ZyXEL, Eason*/
	nice(-10);
#endif

#ifdef ICF_VERIZON
	if( ERROR_SUCCESS != RegOpenKeyEx( HKEY_LOCAL_MACHINE, L"SOFTWARE\\FSS_Microphone",0,KEY_READ, &key ))
	{
		return FALSE;
	}

	RegQueryValueEx( key, L"Path", NULL, NULL, filename_unicode, &len );

	len = 0;
	len = wcslen(filename_unicode);
	count = wcstombs((char*)filepath,(WCHAR*)filename_unicode,len + 1);
	if(count != len)
	{
		return ICF_FAILURE;
	}
	strcpy(filename,filepath);
	strcat(filename,"/cfg/icf.cfg");
#endif


#ifdef ICF_TRACE_REDIRECT

    p_log_file = freopen("ICF_INIT_ERROR_TRACE.txt","w+",stdout);

    if ( NULL == p_log_file)

    {

        ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:Failure in opening trace file, Exiting!!"));

        return (0);

    }

    ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:Trace Redirection Enabled"));

#endif

#ifdef ICF_DEBUG_MEMPOOL    

    p_memget_file_ptr = icf_port_fopen("icf_memget.csv", "w" );

    if(ICF_NULL != p_memget_file_ptr)

    {

        fclose(p_memget_file_ptr);

        p_memget_file_ptr=ICF_NULL;

    }/* if(ICF_NULL != p_memget_fil... */

    

    /* Commented Intentionally : Not Required*/

    /*p_mem_free_file_ptr =  icf_port_fopen("icf_memfree.csv", "w" );

    * p_mem_cpy_file_ptr = icf_port_fopen("icf_memcpy.csv", "w" );

    */



#endif







ICF_INIT_START



    /* initialise the init parameters with all zeroes using memset */

	memset(&icf_init_param,0,sizeof(icf_init_param));



    /* allocate memory for IPSec init parameters */

    ICF_STATIC_MEMGET(

        p_icf_glb_data,

        ICF_PORT_SIZEOF(icf_ipsec_init_params_st),

        icf_init_param.p_ipsec_init_data,

        ICF_RET_ON_FAILURE,

        &ecode,

        ret_val)



	if(ICF_FAILURE == ret_val)

   {

		return ICF_FAILURE;

   }



   

	/* allocate memory for Scalable parameters */

    ICF_STATIC_MEMGET(

        p_icf_glb_data,

        ICF_PORT_SIZEOF(icf_scalable_info_st),

        icf_init_param.p_scalable_info,

        ICF_RET_ON_FAILURE,

        &ecode,

        ret_val)





	if(ICF_FAILURE == ret_val)
   {
       	/* free the memory allocated for IPSec parms */

		ICF_STATIC_MEMFREE(
			p_icf_glb_data,
			icf_init_param.p_scalable_info,
			&ecode,
			ret_val)

	   return ICF_FAILURE;
   }


#ifndef ICF_PORT_VXWORKS
#ifdef _CONSOLE

    if ( argc > 1 )

    {

        if ( icf_port_strlen((icf_uint8_t *)argv[1]) < ICF_CONF_MAX_LINE_SIZE)

        {

            icf_port_strcpy((icf_uint8_t *)filename, (icf_uint8_t *)argv[1]);

        }

        else

        {

            ICF_INIT_ERROR_TRACE(("ERROR: Invalid filename %s; opting "

                "for default file %s \n", argv[1],  filename));

        }

    }

#endif
#endif
#if 0

    /* Get the command line options */

    while ((c = getopt(C,V,"hc:")) != EOF)

    {

            switch(c) {

            case 'c' :   /* configuration file name */

                icf_port_strcpy(filename, optarg);

                break;



            case 'h' :       /* fall thru */

            default :

                /* using printf not SS7P_PORT_TRACE */

                ICF_INIT_ERROR_TRACE(("\n\nUsage: ipphonetk [-c conf_file] [-h]\n"));

                ICF_INIT_ERROR_TRACE(("-c\tconfiguration file\n"));

                ICF_INIT_ERROR_TRACE(("\t\tby default icf.cfg\n"));

                ICF_INIT_ERROR_TRACE(("-h\thelp\n"));

                exit(0);

        }

    }  /* while */

#endif



    /* conf_file needs to be allocated */



    ICF_STATIC_MEMGET(

            p_icf_glb_data,

            (ICF_PORT_SIZEOF(icf_conf_line_st) * ICF_CONF_MAX_LINES),

            conf_file.line,

            ICF_RET_ON_FAILURE,

            &(ecode),

            ret_val)



	if(ICF_FAILURE == ret_val)

    {



        /* free the memory allocated for IPSec parms */

		ICF_STATIC_MEMFREE(

			p_icf_glb_data,

			icf_init_param.p_ipsec_init_data,

			&ecode,

			ret_val)



		ICF_STATIC_MEMFREE(

			p_icf_glb_data,

			icf_init_param.p_scalable_info,

			&(ecode),

			ret_val)



		return ICF_FAILURE;

	}



    conf_file.count = 0; /* initialising conf_file */
	
#ifdef ICF_PORT_VXWORKS
/*	ret_val = icf_set_ftp_info(
					ICF_FTP_SERVER_IP,
                    ICF_FTP_LOGIN,
                    ICF_FTP_PASSWORD,
                    ICF_FTP_CONFIG_PATH);*/

	if (ICF_FAILURE == icf_set_ramdrive_info(
					ICF_BYTES_PER_BLOCK,
                    ICF_BLOCK_PER_TRACK,
                    300))/* NO OF BLOCKS ON DEVICE?*/
	{
		printf("\n setRamDriveInfo Failed");
	}
	else if (ICF_FAILURE == 
			 icf_setup_ram_disk(
					&g_icf_ramdisk_param))
	{
		printf("\n setup Ram DISK failed");
	}
#endif

    if ((fp = icf_port_fopen ((const char *)filename, "r")) == NULL)

    {

        /* add a trace here */

        ICF_INIT_ERROR_TRACE(("ERROR: Could not open configuration file %s \n", 

            filename));
printf("\n file open for config file failed ");

		/* exit */

		exit(0);



        

    }

    else

    {

        ret_val = icf_read_conf_file(fp, &conf_file);

        fclose(fp);

    }



	for (count=0; count< conf_file.count; count++)

    {

		icf_port_strcpy((icf_uint8_t *)val_str,(icf_uint8_t *)"");



#ifdef ICF_NW_ACTIVATE



		if(icf_port_strmatch( "AUTO_ACTIVATE", conf_file.line[count].data )==0
			&& !(bit_mask & AUTO_ACTIVATE_PRESENT))
		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif
			if(icf_port_strmatch( val_str,"" )!=0 && icf_isnumeric(val_str)==ICF_TRUE )

                        {num_elements++; bit_mask |= AUTO_ACTIVATE_PRESENT; continue;}

			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: AUTO_ACTIVATE is either null or incorrect \n"));

				break;

			}

		}

		else if(icf_port_strmatch( "AUTO_REGISTER", conf_file.line[count].data )==0
			&& !(bit_mask & AUTO_REGISTER_PRESENT))

		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_strmatch(  val_str,"" )!=0 && icf_isnumeric(val_str)==ICF_TRUE )	

                        {num_elements++; bit_mask |= AUTO_REGISTER_PRESENT; continue;}

			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: AUTO_REGISTER is either null or incorrect \n")); 

				break;

			}
		}

		else

#endif

		if(icf_port_strmatch( (icf_uint8_t *)"OPERATION_MODE",(icf_uint8_t *) conf_file.line[count].data )==0
			&& !(bit_mask & OPERATION_MODE_PRESENT)) 

		{ icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_strmatch( (icf_uint8_t *) val_str,(icf_uint8_t *)"" )!=0 && icf_isnumeric((icf_uint8_t *)val_str)==ICF_TRUE )

                	{num_elements++; bit_mask |=  OPERATION_MODE_PRESENT; continue;}

			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: OPERATION_MODE is either null or incorrect \n"));

				break; 

			}
		}

		else if(icf_port_strmatch( (icf_uint8_t *)"AUTO_SCALE", (icf_uint8_t *)conf_file.line[count].data )==0
			&& !(bit_mask & AUTO_SCALE_PRESENT))  

		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )!=0 && icf_isnumeric((icf_uint8_t *)val_str)==ICF_TRUE )	

                        {num_elements++; bit_mask |= AUTO_SCALE_PRESENT; continue;}

			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: AUTO_SCALE is either null or incorrect \n")); 

				break;

			}
		}

		else if(icf_port_strmatch( (icf_uint8_t *)"ICF_GENERIC_APP_IP",(icf_uint8_t *) conf_file.line[count].data )==0
			&& !(bit_mask & ICF_GENERIC_APP_IP_PRESENT))  

		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_check_if_ip((icf_uint8_t *)val_str)!=ICF_NULL && icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )!=0)
			{num_elements++; bit_mask |= ICF_GENERIC_APP_IP_PRESENT; continue;}
			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: GENERIC_APP_IP is either null or incorrect \n"));

				break;

			}
		}

		else if(icf_port_strmatch( (icf_uint8_t *)"ICF_GENERIC_APP_PORT", (icf_uint8_t *)conf_file.line[count].data )==0
			&& !(bit_mask & ICF_GENERIC_APP_PORT_PRESENT))  


		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf((icf_uint8_t *)"\nLine Number: %d",__LINE__);
#endif
			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )!=0 &&icf_isnumeric((icf_uint8_t *)val_str)==ICF_TRUE)

			{

				num_elements++;

				bit_mask |= ICF_GENERIC_APP_PORT_PRESENT; 

				continue;

			}

			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: GENERIC_APP_PORT is either null or incorrect \n"));

				break;

			}
		}

		else if(icf_port_strmatch( (icf_uint8_t *)"ICF_MM_IP", (icf_uint8_t *)conf_file.line[count].data )==0
			&& !(bit_mask & ICF_MM_IP_PRESENT))  

		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_check_if_ip((icf_uint8_t *)val_str)!=ICF_NULL && icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )!=0)
			{num_elements++; bit_mask |= ICF_MM_IP_PRESENT; continue;}
			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: ICF_MM_IP is either null or incorrect \n"));

				break;

			}
		}

		else if(icf_port_strmatch( (icf_uint8_t *)"ICF_MM_PORT", (icf_uint8_t *)conf_file.line[count].data )==0
			&& !(bit_mask & ICF_MM_PORT_PRESENT))  

		{ 

			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)""  )!=0 && icf_isnumeric((icf_uint8_t *)val_str)==ICF_TRUE ) 

			{

				num_elements++; 

				bit_mask |= ICF_MM_PORT_PRESENT; 

				continue;

			}

			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: ICF_MM_PORT is either null or incorrect \n"));

				break;

			}
		}

		else if(icf_port_strmatch( (icf_uint8_t *)"ICF_IPC_SELF_IP", (icf_uint8_t *)conf_file.line[count].data )==0
			&& !(bit_mask & ICF_IPC_SELF_IP_PRESENT)) 
		{ 
			icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

			if(icf_port_check_if_ip((icf_uint8_t *)val_str)!=ICF_NULL && icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )!=0)
			{num_elements++; bit_mask |= ICF_IPC_SELF_IP_PRESENT; continue;}
			else

			{

				ICF_INIT_ERROR_TRACE(("NOTE: ICF_IP_SELF_IP is either null or incorrect \n"));

				break;

			}	
		}
#ifdef ICF_NAT_MEDIA_TRAVERSAL
        else if(icf_port_strmatch( "ICF_NAT_IP", conf_file.line[count].\
                        data)==0 && !(bit_mask & ICF_NAT_IP_PRESENT)) 
		{ 

            icf_port_sscanf(conf_file.line[count].data, "%s %s\n", \
                    temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif

            if(icf_port_check_if_ip(val_str)!=ICF_NULL){num_elements++; \
                continue;}

		}

		else if(icf_port_strmatch( "ICF_NAT_PORT", conf_file.line[count].\
                    data)==0 && !(bit_mask & ICF_NAT_PORT)) 

		{ 

            icf_port_sscanf(conf_file.line[count].data, "%s %s\n", \
                    temp_str, val_str);
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif
			if(icf_port_strmatch(  val_str,""  )!=0){num_elements++; \
                continue;}
		}
#endif
	}





#ifdef ICF_NW_ACTIVATE
#ifdef ICF_NAT_MEDIA_TRAVERSAL
	if (num_elements != ICF_NUM_MANDATORY_PARAMS)
#else
	if (num_elements != ICF_NUM_MANDATORY_PARAMS-2)

#endif
#else
#ifdef	ICF_NAT_MEDIA_TRAVERSAL
        if (num_elements != ICF_NUM_MANDATORY_PARAMS - 2)
#else
        if (num_elements != ICF_NUM_MANDATORY_PARAMS - 4)
#endif

#endif

	{

		ICF_INIT_ERROR_TRACE(("NOTE: Mandatory parameters are MISSING or have INVALID entries in ICF.CFG \n"));
#ifdef ICF_PORT_VXWORKS
			printf("\nLine Number: %d",__LINE__);
#endif
		icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

		return ICF_SUCCESS;

	}



	/* Check if icf.cfg has no null values ie. value is not present, but string is present*/



	for (count=0; count < conf_file.count; count++)

	{

		icf_port_strcpy((icf_uint8_t *)val_str,(icf_uint8_t *)"");



		icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, val_str);
		icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);




		if (icf_port_strmatch( (icf_uint8_t *)"MAX_LINES", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE )
			{ null_val_present = ICF_TRUE; break;}
			
			else if(value<1 || value>ICF_MAX_NUM_OF_LINES) { invalid_range = ICF_TRUE; break; } 
			
			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"MAX_REGISTRATIONS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE )
			{ null_val_present = ICF_TRUE; break;}
			
			else if(value<0 || value>255) { invalid_range = ICF_TRUE; break;}

			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"MAX_APPLICATIONS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE )
			{ null_val_present = ICF_TRUE; break;}
			else if(value<1 || value>64) { invalid_range = ICF_TRUE; break;}

			else { continue;}  

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"MAX_AUDIO_CALLS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{
			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE ){ null_val_present = ICF_TRUE; break;}

			else if(value<0 || value>10) { invalid_range = ICF_TRUE; break;}

			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"MAX_VIDEO_CALLS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>10) { invalid_range = ICF_TRUE; break;}
			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"MAX_SERVICE_CALLS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>10) { invalid_range = ICF_TRUE; break; }	
			else { continue;}

		}

#ifdef ICF_NW_ACTIVATE

		else if (icf_port_strmatch( "MAX_NETWORKS", conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  val_str,"" )==0 || icf_isnumeric(val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>255) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

#endif

			else if (icf_port_strmatch( (icf_uint8_t *)"MAX_URIS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>10) { invalid_range = ICF_TRUE; break; }
			else { continue;}
		}

		else if (icf_port_strmatch( (icf_uint8_t *)"MAX_CALL_HISTORY", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch( (icf_uint8_t *) val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>255) { invalid_range = ICF_TRUE; break; }
			else { continue;}
		}

		else if (icf_port_strmatch( (icf_uint8_t *)"SIP_COMPACT_HDR", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>1) { invalid_range = ICF_TRUE; break; }

			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"NONIMS-AUTH-REQUIRED", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0){ null_val_present = ICF_TRUE; break;}

			else { continue;}

		}

		else if (icf_port_strmatch((icf_uint8_t *)"SIGCOMP-REQUIRED", (icf_uint8_t *)conf_file.line[count].data) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>1) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

       else if (icf_port_strmatch((icf_uint8_t *)"IPSEC_ENABLED", (icf_uint8_t *)conf_file.line[count].data) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>1) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch((icf_uint8_t *)"TXN_THRESHOLD", (icf_uint8_t *)conf_file.line[count].data) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch((icf_uint8_t *)"START_PORT_VAL", (icf_uint8_t *)conf_file.line[count].data) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>65535) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}


#ifdef ICF_NW_ACTIVATE

		

		else if (icf_port_strmatch( "APN", conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  val_str,"" )==0){ null_val_present = ICF_TRUE; break;}

			else { continue;}

		}

#endif

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_DEF_SIP_SERVER_PORT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>65535) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_DEF_SIP_PROXY_PORT",(icf_uint8_t *) conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>65535) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_RECV_FROM_APP_PORT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>65535) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		/* add code regarding tcp/both mode and max-tcp-conn */

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_DEF_SIP_TRANSPORT_MODE", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>2) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_ENABLE_NW_TRACE", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>1) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}
		else if (icf_port_strmatch((unsigned char *)"ICF_ENABLE_NON_SUBSCRIBE_NOTIFY", (unsigned char *)conf_file.line[count].data ) == 0)
		{
			if(icf_port_strmatch((unsigned char *)val_str,(unsigned char *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>1) { invalid_range = ICF_TRUE; break; }
			else { continue;}
		}

#ifdef ICF_NW_ACTIVATE

		else if (icf_port_strmatch( "ACCESS_NET_TYPE", conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  val_str,"" )==0 || icf_isnumeric(val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0 || value>1) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch( (unsigned char *)"NET_ACT_RETRY_TIMER_DURATION", (unsigned char *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch((unsigned char *)val_str,(unsigned char *)"" )==0 || icf_isnumeric(val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch( (unsigned char *)"NET_ACT_RETRY_TIMER_ATTEMPTS", (unsigned char *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch((unsigned char *)val_str,(unsigned char *)"" )==0 || icf_isnumeric(val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

#endif

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_CNONCE_STRING", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0){ null_val_present = ICF_TRUE; break;}

			else { continue;}

		}

        else if (icf_port_strmatch( (icf_uint8_t *)"ICF_LOG_FILE_MAX_SIZE",(icf_uint8_t *) conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			 else if(value<0) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}

		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_SYS_RESOURCE_LIMIT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value<0) { invalid_range = ICF_TRUE; break; }
			else { continue;}

		}
		else if(icf_port_strmatch( (icf_uint8_t *)"ICF_IPC_SELF_IP", (icf_uint8_t *)conf_file.line[count].data )==0) 

		{ 

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0){ null_val_present = ICF_TRUE; break;}

			else { continue;}

		}
		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_TLS_INFO_PRESENT", (icf_uint8_t *)conf_file.line[count].data ) == 0)
		{
			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0){ null_val_present = ICF_TRUE; break;}
			else { continue;}
		}
		else if (icf_port_strmatch( (icf_uint8_t *)"BLOCKING_CONNECT", (icf_uint8_t *)conf_file.line[count].data ) == 0)
		{
			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0){ null_val_present = ICF_TRUE; break;}
			else { continue;}
		}


#ifdef ICF_QOS_SUPPORT
		else if (icf_port_strmatch( (icf_uint8_t *)"ICF_RES_RESERVATION_SUPPORTED", (icf_uint8_t *)conf_file.line[count].data ) == 0)
		{

			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0){ null_val_present = ICF_TRUE; break;}

			else { continue;}

		}
#endif
		else if (icf_port_strmatch ((icf_uint8_t *)"ICF_LOGS_IND", (icf_uint8_t *)conf_file.line[count].data) == 0)
		{
			if(icf_port_strmatch(  (icf_uint8_t *)val_str,(icf_uint8_t *)"" )==0 || icf_isnumeric((icf_uint8_t *)val_str)==ICF_FALSE){ null_val_present = ICF_TRUE; break;}
			else if(value <1 || value>4) { invalid_range = ICF_TRUE; break; }
			else { continue;}
		}
	}

	

	if( ICF_TRUE == null_val_present || ICF_TRUE == invalid_range)
	{

		ICF_INIT_ERROR_TRACE(("NOTE: NULL values for parameters found in ICF.CFG \n"));

		icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

		return ICF_SUCCESS;

	}



	for (count=0; count < conf_file.count; count++)

    {

#ifdef ICF_DNS_LOOKUP_ENABLED	
		/* code added to read the retry and dns purge timer */

				if (icf_port_strmatch( (icf_uint8_t *)"SIP_REQ_RETRY", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
					if (value < 1 || value > 7) 
					{
						/* put the default value of SIP request retry */
						icf_init_param.sip_req_retry = 7;
					}
					else 
					{
						/* put the configured value */
						icf_init_param.sip_req_retry = (icf_uint8_t)value;

					}
					continue;
				}
				
				if (icf_port_strmatch( (icf_uint8_t *)"DNS_PURGE_TIMER", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value < 1)  /* the value is in minutes */
					{
						/* put the default value as 20 minutes */
						icf_init_param.dns_purge_timer = 20;
					}
					else 
					{
						/* put the assigned valued */
						icf_init_param.dns_purge_timer = value;
					}
					continue;
				}

				/* end of the code for DNS */
#endif

				/* check if auto_scale is enabled */

				if (icf_port_strmatch( (icf_uint8_t *)"AUTO_SCALE", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value == 0) 

					{

						/* disable auto_scale in the init_param_st */

						icf_init_param.auto_scale = ICF_FALSE;

					}

					else if (value == 1)

					{

						/* enable auto_scale in the init_param_st */

						icf_init_param.auto_scale = 1;

						memset(icf_init_param.p_scalable_info,0,sizeof(icf_scalable_info_st)); 

					}

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: AUTO_SCALE - Invalid Value \n"));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue;

				}

				if(ICF_FALSE == icf_init_param.auto_scale)

				{

					/* fill the scalable_param_st */

					if (icf_port_strmatch( (icf_uint8_t *)"MAX_LINES", (icf_uint8_t *)conf_file.line[count].data ) == 0)

					{

						/* MAX_LINES parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if( ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_LINES - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS; 

						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_lines = value;     

						}

						continue; 

					}

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_REGISTRATIONS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

					{

						/* MAX_REGISTRATIONS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if( ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_REGISTRATIONS - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS; 

						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_reg = value;  

						}

						continue;  

					}

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_APPLICATIONS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

					{

						/* MAX_APPLICATIONS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if(ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_APPLICATIONS - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS; 

						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_apps = value;     

						}

						continue;   

					}

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_AUDIO_CALLS",(icf_uint8_t *) conf_file.line[count].data ) == 0)

					{

						/* MAX_AUDIO_CALLS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if( ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_AUDIO_CALLS - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS; 

						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_audio_calls = value;

						}

						continue;  

					}

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_VIDEO_CALLS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

					{

						/* MAX_VIDEO_CALLS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if(ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_VIDEO_CALLS - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS; 

						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_video_calls = value;     

						}

						continue;  

					}

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_SERVICE_CALLS", (icf_uint8_t *)conf_file.line[count].data ) == 0)

					{

						/* MAX_SERVICE_CALLS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if(ICF_NULL == value )

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_SERVICE_CALLS - Invalid \n"));

							icf_init_param.p_scalable_info->num_of_max_service_calls = ICF_DEFAULT_MAX_SERVICE_CALLS; 

						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_service_calls = value;     

						}

						continue;   

					}

#ifdef ICF_NW_ACTIVATE

					else if (icf_port_strmatch( "MAX_NETWORKS", conf_file.line[count].data ) == 0)

					{

						/* MAX_NETWORKS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if( ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_NETWORKS - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS;						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_nws = value;     

						}

						continue;   

					}

#endif

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_URIS",(icf_uint8_t *) conf_file.line[count].data ) == 0)

					{

						/* MAX_URIS parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if( ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_URIS - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS;						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_uri = value;     

						}

						continue;    

					}

					else if (icf_port_strmatch( (icf_uint8_t *)"MAX_CALL_HISTORY", (icf_uint8_t *)conf_file.line[count].data ) == 0)

					{

						value = ICF_NULL;

						/* MAX_CALLS_TO_LOG parameter */

						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

						if( ICF_NULL == value)

						{

							ICF_INIT_ERROR_TRACE(("NOTE: MAX_CALL_HISTORY - Invalid \n"));

							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

							return ICF_SUCCESS;						}

						else

						{

							icf_init_param.p_scalable_info->num_of_max_call_history = value;     

						}

						continue;   

					}

				}

#if defined IMS_CLIENT && defined NON_IMS_CLIENT 

				

				/* check for operating mode */

				if (icf_port_strmatch( "OPERATION_MODE", conf_file.line[count].data ) == 0)

				{

				    icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value==1)

					{

						/* set operating mode as IMS even for out of range values */

						icf_init_param.operation_mode = ICF_OPERATION_MODE_IMS;

					}

					else if (value==0)

					{

						/* set operating mode as Non-IMS */

						icf_init_param.operation_mode = ICF_OPERATION_MODE_NON_IMS;

					}

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: MODE_OF_OPERATION - Invalid \n"));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue;

				}

#endif



				/* now fill in the init_param_st */

				if (icf_port_strmatch( (icf_uint8_t *)"SIP_COMPACT_HDR", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* icf_sip_compact_hdr parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value == 1)

					{

						/* set the bitmask to indicate SIP header compaction is required */

						ICF_SET_BIT(icf_init_param.bitmask,ICF_SIP_COMPACT_HDR);

					}

					else

					{

						/* reset the opt_parms_bitmask */

						ICF_RESET_BIT(icf_init_param.bitmask,ICF_SIP_COMPACT_HDR);

					}

					continue;

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"NONIMS-AUTH-REQUIRED", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* NONIMS-AUTH-REQUIRED parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, icf_init_param.auth_password.str);

					icf_init_param.auth_password.str_len = (icf_uint16_t)icf_port_strlen(icf_init_param.auth_password.str);

					if ( icf_init_param.auth_password.str_len )

					{

						ICF_SET_BIT(icf_init_param.bitmask,

						ICF_PLAIN_DIGEST_AUTH_REQUIRED);

					}

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: NONIMS-AUTH-REQUIRED - Invalid password \n"));

						ICF_INIT_ERROR_TRACE(("NOTE: IMS Authentication used \n")); 

					}

					continue;

				}

#ifdef ICF_NW_ACTIVATE

				else if (icf_port_strmatch( "AUTO_REGISTER", conf_file.line[count].data ) == 0)

				{

					/* AUTO_REGISTER parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value == 1) 

						icf_init_param.auto_register = ICF_TRUE;

					else if (value == 0)

						icf_init_param.auto_register = ICF_FALSE;

					else

					{

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue; 

				}

#endif

				else if (icf_port_strmatch((icf_uint8_t *)"SIGCOMP-REQUIRED", (icf_uint8_t *)conf_file.line[count].data) == 0)

				{

					/* SIGCOMP_ENABLED */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (1 == value)

					{	ICF_SET_BIT(icf_init_param.bitmask,

								           ICF_SIGCOMP_REQUIRED);

					}

					else if (0 == value)

					{	ICF_RESET_BIT(icf_init_param.bitmask,

								           ICF_SIGCOMP_REQUIRED);

					}

					continue;

				}

                /* IPSec related parameters */

				/* read the IPSec init-time configuration parameters */

				else if (icf_port_strmatch((icf_uint8_t *)"IPSEC_ENABLED", (icf_uint8_t *)conf_file.line[count].data) == 0)

				{

					/* IPSEC_ENABLED parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (1 == value)

					{	ICF_SET_BIT(icf_init_param.bitmask,

								           ICF_IPSEC_REQUIRED);

					}

					else if (0 == value)

					{	ICF_RESET_BIT(icf_init_param.bitmask,

								           ICF_IPSEC_REQUIRED);

					}

                    continue;

				}

				else if (icf_port_strmatch((icf_uint8_t *)"TXN_THRESHOLD", (icf_uint8_t *)conf_file.line[count].data) == 0)

				{

					/* TXN_THRESHOLD parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					icf_init_param.p_ipsec_init_data->transaction_threshold = (icf_uint8_t)value;

					continue;

				}

				else if (icf_port_strmatch((icf_uint8_t *)"START_PORT_VAL", (icf_uint8_t *)conf_file.line[count].data) == 0)

				{

					/* START_PORT_VAL parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					/* check that 5060 and 5061 do not fall in the configured range */

					if (((5060 >= value) && (5060 < (value + ICF_DEFAULT_IPSEC_PORT_COUNT))) || 

						((5061 >= value) && (5061 < (value + ICF_DEFAULT_IPSEC_PORT_COUNT))))

					{

						ICF_INIT_ERROR_TRACE(("NOTE: START_PORT_VAL - Supported Input Value \
												%d includes Default port 5060 or 5061 in the range \n"));

						ICF_INIT_ERROR_TRACE(("NOTE: START_PORT_VAL - Default Value [%d] \
												 used \n", ICF_DEFAULT_IPSEC_START_PORT_VAL));

						/* default values already set up in structure initialization */

					}

					else if(value>=1024 && value <= 0xFFFF)

					{

						icf_init_param.p_ipsec_init_data->start_port_val =(icf_uint16_t) value;

					}

					else

					{

						icf_init_param.p_ipsec_init_data->start_port_val = 

												ICF_DEFAULT_IPSEC_START_PORT_VAL;

					}

					continue;

				}

#ifdef ICF_NW_ACTIVATE

				else if (icf_port_strmatch( "AUTO_ACTIVATE", conf_file.line[count].data ) == 0)

				{

					/* AUTO_ACTIVATE parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value == 1)

						icf_init_param.auto_activate = ICF_TRUE;

					else if (value == 0)

						icf_init_param.auto_activate = ICF_FALSE;

					else

					{

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue; 

				}

				else if (icf_port_strmatch( "APN", conf_file.line[count].data ) == 0)

				{

					/* APN parameter - set the bitmask first */

					ICF_SET_BIT(icf_init_param.bitmask,

								           ICF_APN_STRING_PRESENT);

					icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,

						               icf_init_param.apn.str);

					icf_init_param.apn.str_len = icf_port_strlen(icf_init_param.apn.str);

					if ((icf_init_param.auto_activate == 0) && (icf_init_param.apn.str_len != 0))

					{

						ICF_INIT_ERROR_TRACE(("NOTE: Since AUTO-ACTIVATE is 0, APN is ignored\n"));

						icf_port_strcpy(icf_init_param.apn.str,"");

						icf_init_param.apn.str_len = ICF_DEFAULT_APN_STR_LEN;

					}

					continue;

				}

				else if (icf_port_strmatch((unsigned char *)"NET_ACT_RETRY_TIMER_DURATION", (unsigned char *)conf_file.line[count].data) == 0)



				{



					/* NET_ACT_RETRY_TIMER_DURATION parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);



                    if (ICF_NULL == value)

                    {

                        ICF_INIT_ERROR_TRACE(("\nNOTE: NET_ACT_RETRY_TIMER_DURATION - Invalid value \n"));

						icf_free_mem_and_exit((icf_glb_pdb_st *)p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

                    }

					else

					{

						icf_init_param.net_act_retry_timer_duration = value;

					}



					continue; 

				}



				else if (icf_port_strmatch((unsigned char *)"NET_ACT_RETRY_TIMER_ATTEMPTS", (unsigned char *)conf_file.line[count].data) == 0)



				{



					/* NET_ACT_RETRY_TIMER_ATTEMPTS parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);



                    if (ICF_NULL == value)

                    {

                        ICF_INIT_ERROR_TRACE(("\nNOTE: NET_ACT_RETRY_TIMER_ATTEMPTS - Invalid value \n"));

						icf_free_mem_and_exit((icf_glb_pdb_st *)p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

                    }

					else

					{

						icf_init_param.net_act_retry_timer_attempts = value;

					}



					continue; 

				}

#endif

				else if (icf_port_strmatch((icf_uint8_t *) "ICF_DEF_SIP_SERVER_PORT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* ICF_DEF_SIP_SERVER_PORT parameter - set the bitmask first */

					ICF_SET_BIT(icf_init_param.bitmask,ICF_DEF_SIP_SERVER_PORT);

					

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if ( value>=1024 && value <= 0xFFFF )

					{

						icf_init_param.sip_serv_port_num = (icf_uint16_t)value; 

						

					}

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: ICF_DEF_SIP_SERVER_PORT - Invalid port \n"));

                        ICF_INIT_ERROR_TRACE(("NOTE: Default ICF_DEF_SIP_SERVER_PORT port %d used \n", 

						                ICF_DEFAULT_SIP_SERVER_PORT)); 

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue; 

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_DEF_SIP_PROXY_PORT",(icf_uint8_t *) conf_file.line[count].data ) == 0)

				{

					/* ICF_DEF_SIP_PROXY_PORT parameter - set the bitmask first */

					ICF_SET_BIT(icf_init_param.bitmask,ICF_DEF_SIP_PROXY_PORT);

					

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if ( value>=1024 && value <= 0xFFFF )

					{

						icf_init_param.sip_proxy_port_num = (icf_uint16_t)value; 

						

					}

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: ICF_DEF_SIP_PROXY_PORT - Invalid port \n"));

                        ICF_INIT_ERROR_TRACE(("NOTE: Default ICF_DEF_SIP_PROXY_PORT port %d used \n", 

					                    ICF_DEFAULT_SIP_PROXY_PORT));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}    

					continue; 

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_RECV_FROM_APP_PORT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if ( value>=1024 && value <= 0xFFFF )

						recv_from_app_port = (icf_uint16_t)value; 

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: ICF_RECV_FROM_APP_PORT - Invalid port \n"));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue; 

				}

				/* add code regarding tcp/both mode and max-tcp-conn */

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_DEF_SIP_TRANSPORT_MODE",(icf_uint8_t *) conf_file.line[count].data ) == 0)

				{

					/* TRANSPORT_MODE parameter - set the bitmask */

					ICF_SET_BIT(icf_init_param.bitmask,

						           ICF_DEF_SIP_TRANSPORT_MODE);

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (( value >= 0 ) && ( value <= 2 ))

						icf_init_param.sip_mode = (icf_uint8_t)value;

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: ICF_DEF_SIP_TRANSPORT_MODE - Invalid Value \n"));

                        ICF_INIT_ERROR_TRACE(("NOTE: Default TRANSPORT_MODE port %d used \n", 

						                  icf_init_param.sip_mode));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;



					}    

					continue;

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_GENERIC_APP_IP", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					icf_uint8_t  temp_ip_str[32]="";

					p_generic_rt_info->type = ICF_IPC_UDP;

					icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, temp_ip_str);

					p_generic_rt_info->ipc.udp.ip.addr_type = ICF_TRANSPORT_ADDRESS_IPV4_ADDR;

					/* Fix for CSR 1-7423906: SPR 19772 */
#ifdef ICF_PORT_DEBIAN
					if (ICF_FAILURE == (icf_port_str_to_trans_addr(
								(icf_uint8_t *)temp_ip_str,
								&(p_generic_rt_info->ipc.udp.ip))))
					{
						ICF_INIT_ERROR_TRACE(("NOTE: ICF_GENERIC_APP_IP - Invalid Value \n"));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;
					}
#else
					icf_port_sscanf((icf_int8_t *)temp_ip_str, "%d.%d.%d.%d", 

					&p_generic_rt_info->ipc.udp.ip.addr.ipv4_addr.octet_1,

					&p_generic_rt_info->ipc.udp.ip.addr.ipv4_addr.octet_2,

					&p_generic_rt_info->ipc.udp.ip.addr.ipv4_addr.octet_3,

					&p_generic_rt_info->ipc.udp.ip.addr.ipv4_addr.octet_4);
#endif

					icf_init_param.p_generic_rt_info = (icf_void_t*)&(generic_rt_info);

					continue;

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_GENERIC_APP_PORT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* ICF_SEND_TO_NETWORK_PORT parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if ( value <= 0xFFFF )

					{

						p_generic_rt_info->ipc.udp.port = (icf_uint16_t)value;

					}

					else

					{	icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val); 

						return ICF_SUCCESS;

					

					}

					continue; 

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_ENABLE_NW_TRACE", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* ICF_ENABLE_NW_TRACE parameter */					

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value == 1)

					{

						/* set the bitmask */

						ICF_SET_BIT(icf_init_param.bitmask,

						           ICF_NW_TRACE_ENABLED);

					}

					else

					{

						/* reset the bitmask */

						ICF_RESET_BIT(icf_init_param.bitmask,

						           ICF_NW_TRACE_ENABLED);

					}

					continue; 

				}
				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_ENABLE_NON_SUBSCRIBE_NOTIFY", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					/* ICF_ENABLE_NON_SUBSCRIBE_NOTIFY parameter */					
					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
					if (value == 1)
					{
						/* set the bitmask */
						ICF_SET_BIT(icf_init_param.bitmask,
							ICF_NON_SUBSCRIBE_NOTIFY_ENABLE);
					}
					else
					{
						/* reset the bitmask */
						ICF_RESET_BIT(icf_init_param.bitmask,
							ICF_NON_SUBSCRIBE_NOTIFY_ENABLE);
					}
					continue;
				}

#ifdef ICF_NW_ACTIVATE

				else if (icf_port_strmatch( "ACCESS_NET_TYPE", conf_file.line[count].data ) == 0)

				{

					/* Access_net_type parameter - set the bitmask first */

					ICF_SET_BIT(icf_init_param.bitmask,ICF_ACCESS_NET_TYPE_PRESENT);



					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if (value == 1)

					{

						/* set the access type to be of type GPRS */

						icf_init_param.access_net_type = ICF_ACCESS_NET_TYPE_GPRS;

					}

					else

					{

						ICF_INIT_ERROR_TRACE(("NOTE: Invalid access_net_type used \n"));

					}

					continue;

				}

#endif

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_CNONCE_STRING", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* CNONCE parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,

						               icf_init_param.c_nonce_string.str);

					icf_init_param.c_nonce_string.str_len = (icf_uint16_t)icf_port_strlen(icf_init_param.c_nonce_string.str);

					if (icf_init_param.c_nonce_string.str_len != 0)

					{

						/* set the bitmask */

						ICF_SET_BIT(icf_init_param.bitmask,

								       ICF_SIP_AUTH_CNONCE);

					}

					else

					{

						/* reset the bitmask */

						ICF_RESET_BIT(icf_init_param.bitmask,

								       ICF_SIP_AUTH_CNONCE);

					}

					continue;

				}



				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_MM_IP", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					p_icf_es_mm_param->mm_addr[0] = '\0';

					icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, p_icf_es_mm_param->mm_addr);

				}

				else if (icf_port_strmatch( (icf_uint8_t *)"ICF_MM_PORT", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					/* ICF_SEND_TO_NETWORK_PORT parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

					if ( value <= 0xFFFF )

					{

						p_icf_es_mm_param->mm_port = value;

					}

					else

					{

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

					}

					continue; 

				}

#ifdef ICF_NAT_MEDIA_TRAVERSAL
                else if (icf_port_strmatch( "ICF_NAT_IP", conf_file.\
                            line[count].data ) == 0)
                {

                    p_icf_es_nat_param->nat_addr[0] = '\0';

                    icf_port_sscanf(conf_file.line[count].data, "%s %s\n", \
                            temp_str, p_icf_es_nat_param->nat_addr);
                }
                else if (icf_port_strmatch( "ICF_NAT_PORT", conf_file.\
                            line[count].data ) == 0)
                {

                    /* ICF_SEND_TO_NETWORK_PORT parameter */

                    icf_port_sscanf(conf_file.line[count].data, "%s %d\n",\
                            temp_str, &value);
                    if ( value <= 0xFFFF )
                    {
                        p_icf_es_nat_param->nat_port = value;
                    }
                    else
                    {
                        icf_free_mem_and_exit(p_icf_glb_data,conf_file,\
                                ecode,ret_val);
                        return ICF_SUCCESS;
                    }
                    continue; 
                }
#endif


				

				else if (icf_port_strmatch((icf_uint8_t *)"ICF_SYS_RESOURCE_LIMIT", (icf_uint8_t *)conf_file.line[count].data) == 0)



				{



					/* ICF_SYS_RESOURCE_LIMIT parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);



                    if (ICF_NULL == value)

                    {

                        ICF_INIT_ERROR_TRACE(("\nNOTE: ICF_SYS_RESOURCE_LIMIT - Invalid value \n"));

						icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

						return ICF_SUCCESS;

                    }

					else

					{

						icf_init_param.p_scalable_info->sys_resource_limit = (icf_uint8_t)value;

					}



					continue; 

				}
else if (icf_port_strmatch( (icf_uint8_t *)"ICF_IPC_SELF_IP", (icf_uint8_t *)conf_file.line[count].data ) == 0)

				{

					icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str, icf_ipc_self_ip.str);

				}
				else if (icf_port_strmatch((icf_uint8_t *)"ICF_TLS_INFO_PRESENT", (icf_uint8_t *)conf_file.line[count].data) == 0)
				{
					/* TLS_INFO_PRESENT */
					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
					if (1 == value)
					{
						/* Allocate memory for the TLS parameters */
						ICF_STATIC_MEMGET(
								p_icf_glb_data,
								ICF_PORT_SIZEOF(icf_tls_init_params_st),
								icf_init_param.p_tls_init_data,
								ICF_RET_ON_FAILURE,
								&ecode,
								ret_val)
					}
					else if (0 == value)
					{
						icf_init_param.p_tls_init_data = ICF_NULL;
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"CERTFILE", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->cert_file.str);
						icf_init_param.p_tls_init_data->cert_file.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->cert_file.str);
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"PRIVATEKEYFILE", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->private_key_file.str);
						icf_init_param.p_tls_init_data->private_key_file.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->private_key_file.str);
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"PRIVATEKEYPASSWORD", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->private_key_passwd.str);
						icf_init_param.p_tls_init_data->private_key_passwd.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->private_key_passwd.str);
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"DHPARAMFILE", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->dh_param_file.str);
						icf_init_param.p_tls_init_data->dh_param_file.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->dh_param_file.str);
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"CACERTFILE", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->ca_cert_file.str);
						icf_init_param.p_tls_init_data->ca_cert_file.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->ca_cert_file.str);
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"CACERTDIRPATH", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->ca_cert_dir_path.str);
						icf_init_param.p_tls_init_data->ca_cert_dir_path.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->ca_cert_dir_path.str);
					}
					continue;
				}
				else if (icf_port_strmatch( (icf_uint8_t *)"CIPHERSUITES", (icf_uint8_t *)conf_file.line[count].data ) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %s\n", temp_str,
								icf_init_param.p_tls_init_data->cipher_suites.str);
						icf_init_param.p_tls_init_data->cipher_suites.str_len = 
							(icf_uint16_t)icf_port_strlen(icf_init_param.p_tls_init_data->cipher_suites.str);
					}
					continue;
				}
				else if (icf_port_strmatch((icf_uint8_t *)"CERTFORMAT", (icf_uint8_t *)conf_file.line[count].data) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
						if (ICF_NULL == value)
						{
							ICF_INIT_ERROR_TRACE(("\nNOTE: CERTFORMAT - Invalid value \n"));
							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);
							return ICF_SUCCESS;
						}
						else
						{
							icf_init_param.p_tls_init_data->cert_format = value;
						}
					}
					continue; 
				}
				else if (icf_port_strmatch((icf_uint8_t *)"FORMATDHPARAM", (icf_uint8_t *)conf_file.line[count].data) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
						if (ICF_NULL == value)
						{
							ICF_INIT_ERROR_TRACE(("\nNOTE: DHPARAMFILEFORMAT - Invalid value \n"));
							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);
							return ICF_SUCCESS;
						}
						else
						{
							icf_init_param.p_tls_init_data->dh_param_file_format = value;
						}
					}
					continue; 
				}
				else if (icf_port_strmatch((icf_uint8_t *)"CERTVERIFYMODE", (icf_uint8_t *)conf_file.line[count].data) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
						if (value > 7)
						{
							ICF_INIT_ERROR_TRACE(("\nNOTE: CERTVERIFYMODE - Invalid value \n"));
							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);
							return ICF_SUCCESS;
						}
						else
						{
							icf_init_param.p_tls_init_data->cert_verify_mode = (icf_uint8_t)value;
						}
					}
					continue; 
				}
				else if (icf_port_strmatch((icf_uint8_t *)"TLS_SIP_SERV_PORT_NUM",(icf_uint8_t *) conf_file.line[count].data) == 0)
				{
					if (ICF_NULL != icf_init_param.p_tls_init_data)
					{
						icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
						if (ICF_NULL == value)
						{
							ICF_INIT_ERROR_TRACE(("\nNOTE: TLS_SIP_SERV_PORT_NUM - Invalid value \n"));
							icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);
							return ICF_SUCCESS;
						}
						else
						{
							icf_init_param.p_tls_init_data->tls_sip_serv_port_num = value;
						}
					}
					continue; 
				}
				else if (icf_port_strmatch((icf_uint8_t *)"BLOCKING_CONNECT", (icf_uint8_t *)conf_file.line[count].data) == 0)
				{
					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
					if (1 == value)
					{
						icf_init_param.bitmask |= ICF_BLOCKING_CONNECT;
					}
					else if (0 == value)
					{
						icf_init_param.bitmask &= ~ICF_BLOCKING_CONNECT;
					}
					continue;
				}
#ifdef ICF_PORT_WINCE
				else if (icf_port_strmatch((unsigned char *)"ICF_LOGS_IND", conf_file.line[count].data) == 0)

				{

					/* ICF_LOGS_IND parameter */
					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);

                    if (ICF_NULL == value)
                    {
                        ICF_INIT_ERROR_TRACE(("\nNOTE: ICF_LOGS_IND - Invalid value \n"));
						icf_free_mem_and_exit((icf_glb_pdb_st *)p_icf_glb_data,conf_file,ecode,ret_val);
						return ICF_SUCCESS;
                    }
					else
					{
						icf_init_param.logs_indication = value;
					}
					if ((icf_init_param.logs_indication == 1) ||
						(icf_init_param.logs_indication == 3))
					{
						
#ifdef ICF_TRACE_ENABLE
#ifdef ICF_VERIZON
						
						strcpy(filename, filepath);
						strcat(filename, "/log/icf_logs.txt");
						p_log_file_ptr = icf_port_fopen(filename, "w+" ); 
#else
#ifdef ICF_PORT_WINCE
						p_log_file_ptr = icf_port_fopen("\\icf-6\\log\\icf_logs.txt", "w+" );
#else
						p_log_file_ptr = icf_port_fopen("..\\log\\icf_logs.txt", "w+" );
#endif	
#endif 
#endif

					}

					continue; 
				}
#endif

#ifdef ICF_QOS_SUPPORT

				else if (icf_port_strmatch((icf_uint8_t *)"ICF_RES_RESERVATION_SUPPORTED",(icf_uint8_t *) conf_file.line[count].data) == 0)

				{

					/* ICF_RES_RESERVATION_SUPPORTED parameter */

					icf_port_sscanf(conf_file.line[count].data, "%s %d\n", temp_str, &value);
					
					if (value == 1)

					{

						/* set the bitmask */

						ICF_SET_BIT(icf_init_param.bitmask,

						           ICF_QOS_RES_RESERV_SUPPORTED);

					}

					else

					{

						/* reset the bitmask */

						ICF_RESET_BIT(icf_init_param.bitmask,

						           ICF_QOS_RES_RESERV_SUPPORTED);

					}

					continue; 

				}
#else
				/* reset the bitmask */

				ICF_RESET_BIT(icf_init_param.bitmask,
			          ICF_QOS_RES_RESERV_SUPPORTED);
#endif
		}

#ifdef ICF_DNS_LOOKUP_ENABLED
    /* this check is to make sure that if parameter is missing then default
     * value is assogned */

    if(icf_init_param.sip_req_retry <=0 || icf_init_param.sip_req_retry >7)
    {
        icf_init_param.sip_req_retry = 7;
    }
    if(icf_init_param.dns_purge_timer < 1)
    {
        icf_init_param.dns_purge_timer = 20;
    }
#endif

#ifndef NON_IMS_CLIENT

	icf_init_param.operation_mode = ICF_OPERATION_MODE_IMS;	

#endif

#ifndef IMS_CLIENT

	icf_init_param.operation_mode = ICF_OPERATION_MODE_NON_IMS;	

#endif



#ifndef ICF_NW_ACTIVATE

/* Setting AUTO_REGISTER off */

	icf_init_param.auto_register = 0;

#endif


#ifdef ICF_NW_ACTIVATE

    ICF_INIT_ERROR_TRACE(("\nINFO: AUTO_ACTIVATE: %d \n", 

        icf_init_param.auto_activate )); 

ICF_4_0_CRM_START

    ICF_INIT_ERROR_TRACE(("\nINFO: AUTO_REGISTER: %d \n", 

        icf_init_param.auto_register )); 

ICF_4_0_CRM_END

#endif

    ICF_INIT_ERROR_TRACE(("\nINFO: AUTO_SCALE: %d \n", 

        icf_init_param.auto_scale ));

    ICF_INIT_ERROR_TRACE(("\nINFO: MODE_OF_OPERATION: %d \n", 

        icf_init_param.operation_mode ));

#ifdef ICF_NW_ACTIVATE

    ICF_INIT_ERROR_TRACE(("\nINFO: ACCESS_NET_TYPE: %s \n",

		(icf_init_param.bitmask & ICF_ACCESS_NET_TYPE_PRESENT) &&

		(icf_init_param.access_net_type == ICF_ACCESS_NET_TYPE_GPRS)?"GPRS":"Invalid access_net_type" ));

	ICF_INIT_ERROR_TRACE(("\nINFO: NET_ACT_RETRY_TIMER_DURATION: %d \n", 

        icf_init_param.net_act_retry_timer_duration )); 

	ICF_INIT_ERROR_TRACE(("\nINFO: NET_ACT_RETRY_TIMER_ATTEMPTS: %d \n", 

        icf_init_param.net_act_retry_timer_attempts )); 

#endif

	ICF_INIT_ERROR_TRACE(("\nINFO: C_NONCE required: %s \n",

	icf_init_param.c_nonce_string.str_len?"YES":"NO" ));

	ICF_INIT_ERROR_TRACE(("\nINFO: SIP_COMPACT_HDR: %s \n",

		icf_init_param.bitmask & ICF_SIP_COMPACT_HDR?"YES":"NO" ));

	ICF_INIT_ERROR_TRACE(("\nINFO: NW_TRACE_ENABLED: %s \n",

		icf_init_param.bitmask & ICF_NW_TRACE_ENABLED?"YES":"NO" ));

#ifdef ICF_NW_ACTIVATE

    ICF_INIT_ERROR_TRACE(("INFO: APN: %s \n", 

        icf_init_param.apn.str_len?(icf_uint8_t*)icf_init_param.apn.str:(icf_uint8_t*)"NONE" )); 

	    ICF_INIT_ERROR_TRACE(("INFO: NET_ACT_RETRY_TIMER_DURATION: %d \n", 

		icf_init_param.net_act_retry_timer_duration )); 

    ICF_INIT_ERROR_TRACE(("INFO: NET_ACT_RETRY_TIMER_ATTEMPTS: %d \n", 

		icf_init_param.net_act_retry_timer_attempts )); 

#endif

    ICF_INIT_ERROR_TRACE(("INFO: SIGCOMP-REQUIRED: %s \n", 

        icf_init_param.bitmask & ICF_SIGCOMP_REQUIRED?"YES":"NO" )); 

    ICF_INIT_ERROR_TRACE(("INFO: %s \n", 

        icf_init_param.bitmask & ICF_PLAIN_DIGEST_AUTH_REQUIRED?

        "NON-IMS Authentication used":"IMS Authentication used" )); 

/*	ICF_INIT_ERROR_TRACE(("INFO: PACKETIZATION_PERIOD: %d \n", 

        icf_init_param.packet_period)); 

*/    ICF_INIT_ERROR_TRACE(("INFO: ICF_DEF_SIP_SERVER_PORT: %d \n", 

        icf_init_param.sip_serv_port_num )); 

    ICF_INIT_ERROR_TRACE(("INFO: ICF_DEF_SIP_PROXY_PORT: %d \n", 

        icf_init_param.sip_proxy_port_num )); 

    ICF_INIT_ERROR_TRACE(("INFO: TRANSPORT_MODE: %d \n", 

        icf_init_param.sip_mode ));
    
	if(0 == icf_init_param.auto_scale)
    {
	    ICF_INIT_ERROR_TRACE(("INFO: MAX_LINES: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_lines )); 
        ICF_INIT_ERROR_TRACE(("INFO: MAX_APPLICATIONS: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_apps )); 
        ICF_INIT_ERROR_TRACE(("INFO: MAX_REGISTRATIONS: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_reg )); 
        ICF_INIT_ERROR_TRACE(("INFO: MAX_AUDIO_CALLS: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_audio_calls )); 
        ICF_INIT_ERROR_TRACE(("INFO: MAX_VIDEO_CALLS: %d \n", 
           icf_init_param.p_scalable_info->num_of_max_video_calls )); 
        ICF_INIT_ERROR_TRACE(("INFO: MAX_SERVICE_CALLS: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_service_calls )); 
#ifdef ICF_NW_ACTIVATE
	    ICF_INIT_ERROR_TRACE(("INFO: MAX_NETWORKS: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_nws )); 
#endif
        ICF_INIT_ERROR_TRACE(("INFO: MAX_URIS: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_uri )); 
        ICF_INIT_ERROR_TRACE(("INFO: MAX_CALL_HISTORY: %d \n", 
            icf_init_param.p_scalable_info->num_of_max_call_history )); 
    }
    
    ICF_INIT_ERROR_TRACE(("INFO: IPSEC_ENABLED: %s \n", 

        (icf_init_param.bitmask & ICF_IPSEC_REQUIRED)?"YES":"NO"));

    ICF_INIT_ERROR_TRACE(("INFO: TXN_THRESHOLD: %d \n", 

        icf_init_param.p_ipsec_init_data->transaction_threshold));

    ICF_INIT_ERROR_TRACE(("INFO: START_PORT_VAL: %d \n", 

        icf_init_param.p_ipsec_init_data->start_port_val));



    ICF_INIT_ERROR_TRACE(("INFO: ICF_SYS_RESOURCE_LIMIT: %d \n", 

        icf_init_param.p_scalable_info->sys_resource_limit));

    
#ifdef ICF_QOS_SUPPORT

	ICF_INIT_ERROR_TRACE(("INFO: RES_RESERVATION_SUPPORTED: %s \n",
		(icf_init_param.bitmask & ICF_QOS_RES_RESERV_SUPPORTED)?"YES":"NO"));

#endif

	/* assign the pointer to icf_init_param structure */

	p_icf_init_param = (icf_init_param_st *)(&icf_init_param);



    /* invoke icf_init() with the pointer to icf_init_param */



    ret_val = icf_init(p_icf_init_param, &p_icf_glb_data);



    if (ICF_FAILURE == ret_val)

    {

		ICF_INIT_ERROR_TRACE(("\n[ES] : EXITING ICF "));

		icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);

		return ICF_SUCCESS;

    }

#if 1 /* ZyXEL porting */
    icf_register_signal();
#endif

ICF_INIT_END

#ifdef ICF_TRACE_ENABLE
	icf_es_init_trace_db();	
#endif

	ret_val = icf_process_message(p_icf_glb_data);

    

	icf_free_mem_and_exit(p_icf_glb_data,conf_file,ecode,ret_val);



#if defined(ICF_PORT_SOLARIS) || (ICF_PORT_WINDOWS) || defined(ICF_PORT_LINUX) || defined(ICF_PORT_VXWORKS)

    return 0;

#endif    





} /* End of main */

#endif    /*ICF_SSA_ES_UT*/









/*

 *This routine is used to initialize FIFO structure

 *and open FIFOs

 */

 icf_boolean_t icf_es_init(

     icf_void_t       **p_p_port_data,

     icf_error_t      *p_ecode)

 {

     icf_return_t ret_val = ICF_SUCCESS;

     icf_global_port_info_st  *p_glb_port_info=ICF_NULL;

     

     icf_uint32_t    fd_id = ICF_NULL;

     icf_uint16_t      count = 0;

     

#if defined(ICF_PORT_LINUX) || defined(ICF_PORT_WINDOWS) || defined(ICF_PORT_VXWORKS)

     /*    icf_uint16_t port_num = 1010;*/

     icf_port_info_st        port_info;
#endif

	icf_string_st channel_path;

     

     ICF_INIT_ERROR_TRACE(("\n[ENTERING]:icf_es_init"));

     

     if(ICF_FAILURE==icf_port_init(p_p_port_data,p_ecode))

     {

         ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:PORT init failed. returning ..\n"));

         return ICF_FAILURE;

     }

     else

     {

         p_glb_port_info = (icf_global_port_info_st *)*p_p_port_data;

         ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:PORT initalized succesfully\n"));

     }

     

     /* initialize the internal message list */

     p_glb_port_info->internal_msg_list.p_msg=ICF_NULL;

     p_glb_port_info->internal_msg_list.p_msg_next=ICF_NULL;


#ifdef ICF_NAT_MEDIA_TRAVERSAL
     if(ICF_FAILURE == nat_init((nat_void_t **)&p_nat_glb_pdb))

     {
         ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:NAT Lib init failed. returning ..\n"));
         return ICF_FAILURE;
     }
     else
     {
         ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:NAT Lib initalized succesfully\n"));
         p_nat_glb_pdb->es_nat_open_udp_socket = es_open_udp_socket;
         p_nat_glb_pdb->es_nat_close_udp_socket = es_close_udp_socket;
         p_nat_glb_pdb->es_nat_send_udp_data = es_send_udp_data;
         p_nat_glb_pdb->es_nat_send_app_data = es_send_app_data;
         p_nat_glb_pdb->es_nat_send_mm_data = es_send_mm_data;
         p_nat_glb_pdb->es_nat_start_timer = es_start_timer;
         p_nat_glb_pdb->es_nat_stop_timer = es_stop_timer;
         
     }
 #endif     

    

     



#ifdef ICF_PORT_WINDOWS

    /* call the WSA init function */



        if(icf_wsa_init()==ICF_FAILURE)

        {

            return ICF_FAILURE;

        }

#endif

    /* make the read fdset to 0 */

    ICF_PORT_FD_ZERO(&(p_glb_port_info->win_ipc_info.win_read_fd));

    /* added as part of TCP UT */

    ICF_PORT_FD_ZERO(&(p_glb_port_info->win_ipc_info.win_write_fd));

    for (count = 0; count < FD_SETSIZE; count ++)

    {

        p_glb_port_info->win_ipc_info.act_read_fd[count]= 0;

    }

    p_glb_port_info->win_ipc_info.act_fd_count = 0;

	for (count = 0; count < ICF_MAX_TLS_CONN; count ++)
    {
        p_glb_port_info->ext_info.secure_map[count].fd_id= ICF_INVALID_FD;
        p_glb_port_info->ext_info.secure_map[count].p_secure_data= ICF_NULL;
    }

/* START: For MPH on Windows */
    for (count = 0; count < ICF_MAX_NO_OF_PERSISTENT_CONNECTIONS; count++)
    {
        p_glb_port_info->win_ipc_info.mph_tcp_conn_fd[count]= ICF_INVALID_FD;
    }
/* END: For MPH on Windows */


    /* call the get_port_info to get the port number of ICF 

        also fill the port numbers of PA, MM and CFG */

    /* open the port for listning to the IPC */

    if(ICF_FAILURE == get_port_info_from_file(&port_info,p_ecode))

    {

      ret_val =ICF_FAILURE;

    }



    p_glb_port_info->win_ipc_info.port_pa = port_info.port_pa;

    p_glb_port_info->win_ipc_info.port_cfg = port_info.port_cfg;

    p_glb_port_info->win_ipc_info.port_mm = port_info.port_mm;



    icf_port_strcpy(p_glb_port_info->win_ipc_info.mm_addr, port_info.mm_addr);

#ifdef ICF_NAT_MEDIA_TRAVERSAL
    p_glb_port_info->win_ipc_info.port_nat = port_info.port_nat;
    icf_port_strcpy(p_glb_port_info->win_ipc_info.nat_addr, port_info.nat_addr);
#endif

#if 0 //ZyXEL porting
    if(icf_port_open_channel(port_info.self_port_ipc, ICF_TRANSPORT_TYPE_UDP,

        ICF_TRUE, icf_ipc_self_ip.str, &fd_id, p_ecode)==ICF_FAILURE)
#else
    memset(&channel_path, 0, sizeof(channel_path));
   strncpy((char *)(&channel_path.str[0]), ICF_ES_CHANNEL_PATH, sizeof(ICF_ES_CHANNEL_PATH));
   channel_path.str_len = sizeof(ICF_ES_CHANNEL_PATH);
   if(icf_port_open_ipc_channel(&fd_id, p_ecode, channel_path) == ICF_FAILURE)
#endif
    {

        printf("the open channel faied \n");

        ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:Unable to open the socket for IPC\n"));

        return ICF_FAILURE;

    }

    

    printf("the open channel succeded \n");

    FD_SET(fd_id, &(p_glb_port_info->win_ipc_info.win_read_fd));

    p_glb_port_info->win_ipc_info.self_fd_ipc = (icf_uint16_t)fd_id;



    /* each time u write to win_read_fd, add the fd to the

     * act_read_fd also. This will be used for clearing the

     * the FD set. Since FD_CLR is not working */

    p_glb_port_info->win_ipc_info.act_read_fd[

        p_glb_port_info->win_ipc_info.act_fd_count] = 

        (icf_uint16_t)fd_id;

    p_glb_port_info->win_ipc_info.act_fd_count += 1;



#ifdef ICF_NAT_MEDIA_TRAVERSAL

    if(icf_port_open_channel(port_info.port_nat, ICF_TRANSPORT_TYPE_UDP,

        ICF_TRUE, port_info.nat_addr, &fd_id, p_ecode)==ICF_FAILURE)
    {
        printf("NAT : the open channel faied \n");
        ICF_INIT_ERROR_TRACE(("\n[ICF_ES]:Unable to open the socket for IPC\n"));
        return ICF_FAILURE;
    }

    printf("the open channel for NAT succeded \n");

    FD_SET(fd_id, &(p_glb_port_info->win_ipc_info.win_read_fd));

    p_glb_port_info->win_ipc_info.nat_fd_ipc = fd_id;

    /* each time u write to win_read_fd, add the fd to the
     * act_read_fd also. This will be used for clearing the
     * the FD set. Since FD_CLR is not working */

    p_glb_port_info->win_ipc_info.act_read_fd[
        p_glb_port_info->win_ipc_info.act_fd_count] = 
        fd_id;

    p_glb_port_info->win_ipc_info.act_fd_count += 1;

#endif

    printf("exit from the es init \n");

    ret_val = ICF_SUCCESS;

    

    return ret_val;

}



#ifdef ICF_PORT_WINDOWS

/*****************************************************************************

 * FUNCTION:        icf_wsa_init

 *

 * DESCRIPTION:    This is a windows specific function needed for socket based

 *                communication

 *

 *****************************************************************************/

icf_return_t icf_wsa_init()

{

    WSADATA wsa_data;

    WORD w_version_requested;

    w_version_requested = MAKEWORD(2,2); 

    if(WSAStartup( w_version_requested, &wsa_data )==0)

    {

        return ICF_SUCCESS;

    }

    else

    {

        return ICF_FAILURE;

    }

}

#endif

/*******************************************************************************************

 *  Function: icf_free_mem_and_exit

 *

 * Description: This function frees the memory allocated to the data structures, and exits.

 *******************************************************************************************/

icf_void_t icf_free_mem_and_exit(icf_glb_pdb_st	*p_glb_data,

								   icf_conf_file_st conf,

								   icf_error_t		ecode,

								   icf_return_t		ret_val)

{

	/* Free the memory allocated for configuration file */

		ICF_STATIC_MEMFREE(

        	p_glb_data,

        	conf.line,

       	 	&(ecode),

        	ret_val)



        	/* free the memory allocated for IPSec parms */

		ICF_STATIC_MEMFREE(

			p_glb_data,

			icf_init_param.p_ipsec_init_data,

			&ecode,

			ret_val)



		ICF_STATIC_MEMFREE(

			p_glb_data,

			icf_init_param.p_scalable_info,

			&(ecode),

			ret_val)

		if (ICF_NULL != icf_init_param.p_tls_init_data)
		{
			ICF_STATIC_MEMFREE(
					p_glb_data,
					icf_init_param.p_tls_init_data,
					&(ecode),
					ret_val)
		}
}	



/*****************************************************************************

 * FUNCTION:        get_port_info

 *

 * DESCRIPTION:    This is a windows specific function needed for socket based

 *                communication

 *

 *****************************************************************************/



icf_return_t get_port_info_from_file(

            icf_port_info_st  *p_port_info,

            icf_error_t    *err_code)

{

        

    /* read the file to get the port numbers */

    /* for the testing purpose we are hard-coding it */

        

    /*p_port_info->port_pa  =        17500;

    p_port_info->port_cfg =        17600;*/

    /* To remove warnings */
    err_code=err_code;

    p_port_info->port_mm  =        icf_es_mm_param.mm_port;
#ifdef ICF_NAT_MEDIA_TRAVERSAL
    p_port_info->port_nat = icf_es_nat_param.nat_port;
#endif
    icf_port_strcpy(p_port_info->mm_addr, icf_es_mm_param.mm_addr);

#ifdef ICF_NAT_MEDIA_TRAVERSAL
    icf_port_strcpy(p_port_info->nat_addr, icf_es_nat_param.nat_addr);
#endif
    p_port_info->self_port_ipc = recv_from_app_port;

    

    return ICF_SUCCESS;

}





ICF_INIT_START



/****************************************************************************

** Function: icf_read_conf_file

**

****************************************************************************

** DESCRIPTION: This function reads the configuration file and stores

**              in a structure p_file.

**              Comments in the configuration files are not stored.

** 

** 

**

***************************************************************************/

icf_return_t icf_read_conf_file(FILE* fp, icf_conf_file_st* p_file)

{

    p_file->count = 0;

    

    /* 

    ** reading conf file line by line up to end of file 

    */

    

    for (     p_file->count = 0 ; 

            p_file->count < ICF_CONF_MAX_LINES; 

            p_file->count++)

    {

        if (

              icf_port_fgets((char*)p_file->line[p_file->count].data,

                ICF_CONF_MAX_LINE_SIZE, fp) == ICF_NULL )

        {

            if ( feof(fp))

            {

               return ICF_SUCCESS;

            }

            

        }

        /* 

        ** string start and termination 

        */

        icf_str_trim((char*)p_file->line[p_file->count].data);

        /* 

        ** Check whether Line starts from # or \n or NULL 

        */

        if (p_file->line[p_file->count].data[0] == ICF_CONF_COMMENT_CHAR || 

            p_file->line[p_file->count].data[0] == '\n' || 

			p_file->line[p_file->count].data[0] == '\r' || 

            p_file->line[p_file->count].data[0] == 0)

        {

            p_file->count--; 

            /* if line starts #,\n or NULL , decrement the count  */

        }

        else

        {

            /* do nothing */

        }

    }

    return ICF_SUCCESS; 

}


/****************************************************************************

** Function: icf_isnumeric 

**

****************************************************************************

** DESCRIPTION: To find whether given string is numeric or not

** 

** 

**

***************************************************************************/

icf_return_t icf_isnumeric( icf_uint8_t *myStr )
{
	icf_uint32_t i=0;
	for( i=0; i < icf_port_strlen(myStr); i++ )
	{
		if( !isdigit(myStr[ i ]) ) 
			return ICF_FALSE;
	}
return ICF_TRUE;
}




/****************************************************************************

** Function: icf_str_trim 

**

****************************************************************************

** DESCRIPTION: This function removes extra TAB/spaces from start and 

**              end of the string. Replaces all tabs by space in the string.

** 

** 

**

***************************************************************************/

icf_void_t icf_str_trim(INOUT char * str)

{

    int starts_from, ends_at, count=0;



    ends_at = icf_port_strlen((icf_uint8_t *)str);

    /* 

    ** removing all tab bar from the line 

    */

    for (count = 0; str[count] !=  '\n' && count < ends_at; count++)

    {

        if (str[count] == '\t')

            str[count]=' ';

    }

    

    count = 0;

    while(str[count]==' ') count++;

    starts_from = count;

    

    

    while(str[ends_at]=='\n' || str[ends_at]==0) 

    {

        ends_at--;

        if ( ends_at < 0 )

        {

            break;

        }

    }

    str[ends_at+1] = ICF_NULL;

    icf_port_strcpy((icf_uint8_t *)str, (icf_uint8_t *)str+starts_from);

}

#if defined(ICF_TRACE_ENABLE) && defined(ICF_PORT_WINDOWS)
icf_uint32_t icf_es_get_max_log_file_size()
{
	return dMaxLogFileSize;
}
#endif

#ifdef ICF_PORT_VXWORKS
/*******************************************************************************
 **
 **  FUNCTION NAME : icf_setup_ram_disk
 **                      
 **  DESCRIPTION: This function is used to create a ram disk
 **               and formats as MSDOS FAT filesystem on vxworks platform
 **
 **  RETURNS: Returns Success or Failure
 **
 **  FUNCTIONS_CALLED:
 **
 ******************************************************************************/
icf_return_t icf_setup_ram_disk(
                 IN icf_ramdisk_param_st *p_icf_ramdisk_param)
{
    BLK_DEV        *p_BlkDev;
    DOS_VOL_DESC   *p_VolDesc;

    ICF_PRINT(("\nEntering Function icf_setup_ram_disk"));
   /*BLK_DEV *ramDevCreate
            (
             char * ramAddr,  where it is in memory (0 = malloc) 
             int bytesPerBlk,  number of bytes per block 
             int blksPerTrack,  number of blocks per track 
             int nBlocks,  number of blocks on this device 
             int blkOffset no. of blks to skip at start of device 
            )*/ 
    
    p_BlkDev = ramDevCreate (0, p_icf_ramdisk_param->bytes_per_block, 
                            p_icf_ramdisk_param->blocks_per_track,
                            p_icf_ramdisk_param->number_of_blocks_on_device, 
                            0);


    if(ICF_NULL == p_BlkDev)
    {
        ICF_PRINT(("\nFAILURE :: Creating ramdisk!"));
        ICF_PRINT(("\nExiting Function icf_setup_ram_disk"));
        return ICF_FAILURE;
    }

    /*DOS_VOL_DESC *dosFsMkfs
             (
              char * p_volName,                 volume name to use 
              BLK_DEV * p_BlkDev              pointer to block device struct
             )*/

    p_VolDesc = dosFsMkfs ((icf_int8_t*)ICF_VX_VOLUME_LABEL,
                       p_BlkDev);


    if(ICF_NULL == p_VolDesc)
    {
        ICF_PRINT(("\nFAILURE :: Formating ramdisk!"));
        ICF_PRINT(("\nExiting Function icf_setup_ram_disk"));
        return ICF_FAILURE;
    }
    g_icf_is_ramdisk_present = ICF_TRUE;

    ICF_PRINT(("\nSUCCESS :: RAMdisk created"));
    ICF_PRINT(("\nExiting Function icf_setup_ram_disk"));
    return ICF_SUCCESS;
}
/*******************************************************************************
 * Function name         :       icf_set_ftp_info
 * Return Value          :       icf_return_t
 * Functionality         :       Configures the various FTP parameters
 *                               from COMMANDLINE
 * Functions called      :       None.
 ******************************************************************************/
icf_return_t icf_set_ftp_info(IN icf_uint8_t *p_ftp_server_ip,
                                     IN icf_uint8_t *p_login,
                                     IN icf_uint8_t *p_password,
                                     IN icf_uint8_t *p_config_path)
{
    ICF_PRINT(("\nEntering Function icf_set_ftp_info"));
    icf_port_strcpy(g_icf_ftp_info.ftp_server_ip,p_ftp_server_ip);
    icf_port_strcpy(g_icf_ftp_info.login,p_login);
    icf_port_strcpy(g_icf_ftp_info.password,p_password);
    icf_port_strcpy(g_icf_ftp_info.config_path,p_config_path);
    ICF_PRINT(("\nExiting Function icf_set_ftp_info"));
    return(ICF_SUCCESS);
}

/*******************************************************************************
 * Function name         :       icf_set_ramdrive_info
 * Return Value          :       icf_return_t
 * Functionality         :       Configures the various ramdrive parameters
 *                               from COMMANDLINE
 * Functions called      :       None.
 ******************************************************************************/
icf_return_t icf_set_ramdrive_info(IN icf_uint16_t   bytes_per_block,
                                   IN icf_uint16_t   blocks_per_track,
                                   IN icf_uint16_t   number_of_blocks_on_device)
{
    ICF_PRINT(("\nEntering Function icf_set_ramdrive_info"));
    g_icf_ramdisk_param.bytes_per_block = bytes_per_block ;
    g_icf_ramdisk_param.blocks_per_track = blocks_per_track ;
    g_icf_ramdisk_param.number_of_blocks_on_device =
                                    number_of_blocks_on_device;
    
    ICF_PRINT(("\nExiting Function icf_set_ramdrive_info"));
    return(ICF_SUCCESS);
}
#endif

#ifdef ICF_TRACE_ENABLE
icf_void_t icf_es_init_trace_db(icf_void_t)
{
	icf_trace_database_st trace_db;

	memset(&trace_db,0,sizeof(icf_trace_database_st));

	trace_db.line_id[0] = 0;
	trace_db.line_id[1] = 1;

        strcpy((icf_int8_t *)trace_db.calling_party_address[0].addr_val.str,
             "publicid@flextronics.com");
	icf_port_populate_trace_database(&trace_db);
}

#endif

#if 1 /* ZyXEL porting */
icf_void_t icf_signal_handler(int sig, siginfo_t* siginfo, icf_void_t* notused ){
	switch(sig){
		case SIGTERM:
		case SIGSEGV:
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			printf("icf: Signal %d received, TERMINATING icf.exe \n", sig);
			printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
			exit(0);
			break;
		default:
			printf("Signal %d received\n", sig);
			break;
			
	}
}

icf_void_t icf_register_signal(icf_void_t){
	struct sigaction sa;

   sa.sa_sigaction = icf_signal_handler;
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;
#if 0
   sigaction( SIGHUP ,  &sa,  NULL );     /* Hangup (POSIX).  */
   sigaction( SIGINT ,  &sa,  NULL );     /* Interrupt (ANSI).  */
   sigaction( SIGQUIT,  &sa,  NULL );     /* Quit (POSIX).  */
   sigaction( SIGILL ,  &sa,  NULL );     /* Illegal instruction (ANSI).  */
   sigaction( SIGTRAP,  &sa,  NULL );     /* Trace trap (POSIX).  */
   sigaction( SIGABRT,  &sa,  NULL );     /* Abort (ANSI).  */
   sigaction( SIGEMT,   &sa,  NULL );
   sigaction( SIGFPE ,  &sa,  NULL );     /* Floating-point exception (ANSI).  */
   sigaction( SIGBUS,   &sa,  NULL );     /* BUS error (4.2 BSD).  */
   sigaction( SIGSYS,   &sa,  NULL );
   sigaction( SIGPIPE,  &sa,  NULL );     /* Broken pipe (POSIX).  */
   sigaction( SIGALRM,  &sa,  NULL );     /* Alarm clock (POSIX).  */
   sigaction( SIGUSR1,  &sa,  NULL );     /* User-defined signal 1 (POSIX).  */
   sigaction( SIGUSR2,  &sa,  NULL );     /* User-defined signal 2 (POSIX).  */
   sigaction( SIGCHLD,  &sa,  NULL );     /* Child status has changed (POSIX).  */
   sigaction( SIGPWR,   &sa,  NULL );     /* Power failure restart (System V).  */
   sigaction( SIGWINCH, &sa,  NULL );     /* Window size change (4.3 BSD, Sun).  */
   sigaction( SIGURG,   &sa,  NULL );     /* Urgent condition on socket (4.2 BSD).  */
   sigaction( SIGIO,    &sa,  NULL );     /* I/O now possible (4.2 BSD).  */
   sigaction( SIGTSTP,  &sa,  NULL );     /* Keyboard stop (POSIX).  */
   sigaction( SIGCONT,  &sa,  NULL );     /* Continue (POSIX).  */
   sigaction( SIGTTIN,  &sa,  NULL );     /* Background read from tty (POSIX).  */
   sigaction( SIGTTOU,  &sa,  NULL );     /* Background write to tty (POSIX).  */
   sigaction( SIGVTALRM,&sa,  NULL );     /* Virtual alarm clock (4.2 BSD).  */
   sigaction( SIGPROF,  &sa,  NULL );     /* Profiling alarm clock (4.2 BSD).  */
   sigaction( SIGXCPU,  &sa,  NULL );     /* CPU limit exceeded (4.2 BSD).  */
   sigaction( SIGXFSZ,  &sa,  NULL );     /* File size limit exceeded (4.2 BSD).  */   
#endif   
   sigaction( SIGSEGV,  &sa,  NULL );     /* Segmentation violation (ANSI).  */   
   sigaction( SIGTERM,  &sa,  NULL );     /* Termination (ANSI).  */

}
#endif

#ifdef __cplusplus

}

#endif


