#ifndef __SYS_CALL_WRAPPER_H__
#define __SYS_CALL_WRAPPER_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "icf_types.h"

/*____________________________________________________________________________*/
/*	functions                                                                                                                     */
/*____________________________________________________________________________*/
void *zyMemcpy(void *p_destination, const void *p_source, unsigned int num_bytes_to_copy);
void *zyMemset(void *p_mem_block, unsigned int value_to_set, unsigned int num);
void *zyMalloc(unsigned int size);
void zyFree(void *ptr);
char *zyStrcpy(char *p_destination, const char *p_source);
char *zyStrncpy(char *p_destination, const char *p_source, unsigned int max_num_bytes);
char *zyStrcat(char *p_destination, const char *p_source);
char *zyStrtok(char *p_source, const char *p_delimiters);
unsigned int zyStrcmp(const char *p_string_1, const char *p_string_2);
unsigned int zyStrcasecmp(const char *p_string_1, const char *p_string_2);
unsigned int zyStrlen(const char *p_string);

icf_void_t* icf_memget(icf_uint32_t size);
icf_void_t icf_memfree(icf_void_t* buffer);
icf_void_t* icf_memcpy(icf_void_t* s1, icf_void_t* s2,icf_uint32_t size);

/* Redirect Debug message for ZyIMS to setting direction., __P876__, Klose, 20130128 */
void ZyPrintf( const char *fmt, ...);

#endif
