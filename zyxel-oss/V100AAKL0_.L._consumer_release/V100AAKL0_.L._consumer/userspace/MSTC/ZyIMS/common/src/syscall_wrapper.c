#include "global.h"
#include "icf_types.h"
#include "syscall_wrapper.h"

/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
#include <fcntl.h>
extern uint8 log_dest;

/*************************************************************************
*
*  FUNCTION NAME  : zyMemcpy
*
*  DESCRIPTION    : This function is the wrapper for memcpy 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
void *zyMemcpy(void *p_destination, const void *p_source, unsigned int num_bytes_to_copy)
{
	return memcpy(p_destination,p_source,num_bytes_to_copy);
}

/*************************************************************************
*
*  FUNCTION NAME  : zyMemset
*
*  DESCRIPTION    : This function is the wrapper for memset 
*
*  RETURNS:       : return the pointer(ptr), given in the argument
*************************************************************************/
void *zyMemset( void *p_mem_block, unsigned int value_to_set, unsigned int num)
{
	return memset(p_mem_block,value_to_set, num);
} 

/*************************************************************************
*
*  FUNCTION NAME  : zyMalloc
*
*  DESCRIPTION    : This function is the wrapper for malloc
*
*  RETURNS:       : void pointer to the allocated memory
*************************************************************************/
void *zyMalloc(unsigned int size)
{
	void *p_mem_to_return;

	p_mem_to_return = (void *) malloc(size);
	if(NULL == p_mem_to_return) {
		printf("zyMalloc FAIL\n");
		return NULL;
	}
	zyMemset(p_mem_to_return, 0, size);
	return p_mem_to_return;

}

/*************************************************************************
*
*  FUNCTION NAME  : zyFree
*
*  DESCRIPTION    : This function is the wrapper for free
*
*  RETURNS:       : no return value 
*************************************************************************/
void zyFree(void *ptr)
{
	if(ptr!=NULL){
		free(ptr);
	}
}

/*************************************************************************
*
*  FUNCTION NAME  : zyStrcpy
*
*  DESCRIPTION    : This function is the wrapper for strcpy 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
char *zyStrcpy(char *p_destination, const char *p_source)
{
	return strcpy(p_destination,p_source);
}


/*************************************************************************
*
*  FUNCTION NAME  : zyStrncpy
*
*  DESCRIPTION    : This function is the wrapper for strncpy 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
char *zyStrncpy(char *p_destination, const char *p_source, unsigned int max_num_bytes)
{
	return strncpy(p_destination,p_source,max_num_bytes);
}

/*************************************************************************
*
*  FUNCTION NAME  : zyStrcat
*
*  DESCRIPTION    : This function is the wrapper for strcat 
*
*  RETURNS:       : pointer to destination is returned
*************************************************************************/
char *zyStrcat(char *p_destination, const char *p_source)
{
	return strcat(p_destination,p_source);
}


/*************************************************************************
*
*  FUNCTION NAME  : zyStrtok
*
*  DESCRIPTION    : This function is the wrapper for strtok 
*
*  RETURNS:       : pointer to the last token found in string
*************************************************************************/
char *zyStrtok(char * p_source, const char * p_delimiters)
{
	return strtok(p_source,p_delimiters);
}


/*************************************************************************
*
*  FUNCTION NAME  : zyStrcmp
*
*  DESCRIPTION    : This function is the wrapper for strncmp 
*
*  RETURNS:       : It returns an integer less than, equal to, or greater 
*		            than zero if first n charaters of  string1  is  found, 
*	      	        respectively, to be less than, to match, or be greater 
*		            than string2.
*************************************************************************/

unsigned int zyStrcmp(const char *p_string_1, const char *p_string_2)
{
	return strcmp(p_string_1,p_string_2);
}

/*************************************************************************
*
*  FUNCTION NAME  : mm_port_strcasecmp
*
*  DESCRIPTION    : This function is the wrapper for strncmp 
*
*  RETURNS:       : It returns an integer less than, equal to, or greater 
*		            than zero if first n charaters of  string1  is  found, 
*	      	        respectively, to be less than, to match, or be greater 
*		            than string2.
*************************************************************************/

unsigned int zyStrcasecmp(const char *p_string_1, const char *p_string_2)
{
	return strcasecmp(p_string_1,p_string_2);
}

/*************************************************************************
*
*  FUNCTION NAME  : zyStrlen
*
*  DESCRIPTION    : This function is the wrapper for strlen 
*
*  RETURNS:       : It returns the number of characters in string
*************************************************************************/
unsigned int zyStrlen(const char *p_string)
{
	return strlen(p_string);
}

/*************************************************************************
*
*  FUNCTION NAME  : icf_memget
*
*  DESCRIPTION    : This function will allocate memory for icf client library
*
*  RETURNS		  : Allocated buffer
*************************************************************************/

icf_void_t* icf_memget(icf_uint32_t size)
{

	return zyMalloc(size);
}

/*************************************************************************
*
*  FUNCTION NAME  : icf_memfree
*
*  DESCRIPTION    : This function will free allocated memory for icf client library
*
*  RETURNS		  : NONE
*************************************************************************/

icf_void_t icf_memfree(void* buffer)
{

	zyFree(buffer);
	return;
}

/*************************************************************************
*
*  FUNCTION NAME  : icf_memcpy
*
*  DESCRIPTION    : This function will copy data from one memory location to other 
*					 for icf client library
*
*  RETURNS		  : Either IMSC_SUCCESS or IMSC_FAILURE
*************************************************************************/

icf_void_t* icf_memcpy(icf_void_t* s1, icf_void_t* s2,icf_uint32_t size)
{

	zyMemcpy(s1,s2,size);
	return s1;
}

/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
/*************************************************************************
*
*  FUNCTION NAME  : Zyrint
*
*  DESCRIPTION    : This function will print to console or ssh depend on which opens the debug log
*					 
*
*  RETURNS		  : no return value 
*************************************************************************/

void
ZyPrintf
(
	const char *fmt,
	...
) {
	va_list args;
	char debugBuffer[1024]={0};
	char devnode[16] = {0};
	int fdDestStdOut = -1;
	int telnet_num = 0;

	va_start(args, fmt);
	vsnprintf(debugBuffer, sizeof(debugBuffer), fmt, args);
   /**/
	if (log_dest == 1){
		for(telnet_num = 0 ; telnet_num<2 ; telnet_num++)
		{
			sprintf(devnode,"/dev/ttyp%x", telnet_num);
			fdDestStdOut = open(devnode, O_RDWR);
			if(fdDestStdOut != -1)
			{
				write((FILE *)fdDestStdOut, debugBuffer, strlen(debugBuffer));
				close(fdDestStdOut);
			}
		}
	/* output to console & telnet */
	}else if (log_dest == 2){
		fprintf(stderr, "%s", debugBuffer);	

		for(telnet_num = 0 ; telnet_num<2 ; telnet_num++)
		{
			sprintf(devnode,"/dev/ttyp%x", telnet_num);
			fdDestStdOut = open(devnode, O_RDWR);
			if(fdDestStdOut != -1)
			{
				write((FILE *)fdDestStdOut, debugBuffer, strlen(debugBuffer));
				close(fdDestStdOut);
			}
		}			
	}else{
		fprintf(stderr, "%s", debugBuffer);	
	}
	va_end(args);	
	return;
}
