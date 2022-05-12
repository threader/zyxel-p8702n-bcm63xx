/******************************************************************************
*                                    
* FILE NAME    : arDnsClient_clib_utils.c                
*                                    
* DESCRIPTION  : Contains the arDnsClient CLIB utility routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 05-Feb-2009    Anuradha Gupta         None          Initial
* 11-Feb-2009    Abbas Haque            None          Code added for arDns 
*
* Copyright 2009, Aricent.                       
*                                    
*******************************************************************************/
#include "arDnsClient_cmn_prototypes.h"

/****************************************************************************
 * API Name: arDnsClient_free_request
 * 
 * Description: This API is invoked by the application for freeing the request
 *              structure. 
 * INPUT       :pDnscl_glb_data - The pointer to the global data of arDnsClient.
 *             :pRequest - arDnsClient_free_requestThe pointer to request structure.  
 * OUTPUT      :pError - Error pointer to be populated in case of error. 
 *
 * return : It will return ADNSCL_SUCCESS if successful else return ARDNSCL_FAILURE.
 ******************************************************************************/
dnscl_return_t  arDnsClient_free_request (
		   dnscl_void_t            *pDnsCl_glb_data,
		   arDnsClient_request_st  *pReq,
		   dnscl_error_t           *pError)
{
	/*Pointer to global variable*/
	arDnsClient_global_data_st  *pGlob_data =  ARDNSCL_NULL;

	/*Type cast global data*/       
	pGlob_data =(arDnsClient_global_data_st*)pDnsCl_glb_data;

	ARDNSCL_CLIB_PRINT (pGlob_data, ("Entered Function arDnsClient_free_request\n"))

    /* this field is for future use , statement to remove compilation warning*/
    if(ARDNSCL_NULL != pError)
    {
        *pError = *pError;
    }
     
	/*Check for configuration data in request structure*/
    if( ARDNSCL_NULL != pReq->pConfData)
    {
          /*Freeing memory for configuration data
          structure*/
          ARDNSCL_MEMFREE(pReq->pConfData);
    }

	/*Check for query data in request structure*/
	if(ARDNSCL_NULL != pReq->pDnsQuery)
	{
		/*Freeing memory for Query data structure*/
		ARDNSCL_MEMFREE(pReq->pDnsQuery);
	}

	/*Check for clear cache data in request structure*/				   
	if(ARDNSCL_NULL != pReq->pClearCache)
	{
		/*Freeing memory for clear cache data structure*/
		ARDNSCL_MEMFREE(pReq->pClearCache);
	}
	
	ARDNSCL_CLIB_PRINT (pGlob_data, ("Exiting Function arDnsClient_free_request\n"))
	return ARDNSCL_SUCCESS;
}

