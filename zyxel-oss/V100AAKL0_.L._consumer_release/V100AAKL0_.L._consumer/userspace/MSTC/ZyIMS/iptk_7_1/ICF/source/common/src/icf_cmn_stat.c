/*************************************************************************
 *
 *     FILENAME     :    icf_cmn_stat.c
 *
 *     DESCRIPTION  :  This file defines the porting functions for timers.
 *                     
 *     Revision History    :
 *         DATE        NAME                REFERENCE            REASON
 *         ----------------------------------------------------------
 *         28/12/2004  Aashish Suchdev                 Initial version.
 *         07/01/2005  Aashish Suchdev                 replaced retval.
 *         11/01/2005  Aashish Suchdev                 corrected name for
 *                                                  init deinit stats
 *
 *     Copyright 2006, Aricent.
 *
 *************************************************************************/

#include "icf_common_types.h"
#include "icf_common_prototypes.h"
#include "icf_macro.h"
#include "icf_port_defs.h"

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_cmn_init_stats
 *
 *  DESCRIPTION     :    This function allocated the global stats structure
 *                       initializes it and stores the address in global
 *                       data.
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
 *************************************************************************/

icf_return_t
icf_cmn_init_stats(INOUT  icf_glb_pdb_st        *p_glb_pdb,
        OUT icf_error_t    *p_ecode )
{

#ifndef ICF_STAT_ENABLED
/* To remove warnings */
p_glb_pdb=p_glb_pdb;
p_ecode=p_ecode;
#endif 

#ifdef ICF_STAT_ENABLED
    icf_return_t     ret_val = ICF_SUCCESS;
    icf_uint32_t     size = 0, count = 0;
    icf_stat_glb_st  *p_glb_stat = ICF_NULL;

    p_ecode = p_ecode;
    p_glb_pdb = p_glb_pdb;

    size = ICF_PORT_SIZEOF(icf_stat_glb_st);

    ICF_MEMGET(p_glb_pdb, size, ICF_MEM_COMMON, (icf_void_t*)p_glb_stat, ICF_RET_ON_FAILURE, p_ecode, ret_val)

        for ( count = 1; count < ICF_STAT_MAX_ID_VAL; count++)
        {
            p_glb_stat->stat_counter_list[count] = 0;

        }
    p_glb_pdb->p_glb_data->p_stats_data = (icf_void_t*)p_glb_stat;
#endif
    return ICF_SUCCESS; 
}

/*************************************************************************
 *
 *  FUNCTION NAME   :    icf_cmn_deinit_stats
 *
 *  DESCRIPTION     :    This function deallocates the global stats structure
 *
 *  RETURNS         :    ICF_SUCCESS if successful, else ICF_FAILURE
 *************************************************************************/

icf_return_t
icf_cmn_deinit_stats(INOUT  icf_glb_pdb_st        *p_glb_pdb,
        OUT icf_error_t    *p_ecode )
{
#ifndef ICF_STAT_ENABLED
  /* To remove warnings */
   p_glb_pdb=p_glb_pdb;
   p_ecode=p_ecode;
#endif
#ifdef ICF_STAT_ENABLED
    icf_return_t     ret_val = ICF_SUCCESS;

    ICF_MEMFREE(p_glb_pdb, p_glb_pdb->p_glb_data->p_stats_data, ICF_MEM_COMMON, p_ecode, ret_val)

#endif
        return ICF_SUCCESS; 
}
