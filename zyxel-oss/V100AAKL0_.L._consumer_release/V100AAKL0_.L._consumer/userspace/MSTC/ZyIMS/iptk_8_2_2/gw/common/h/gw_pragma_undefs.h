/*******************************************************************************
*
* FUNCTION : Pragma defines for the HSS Gateway.
*
********************************************************************************
*
* FILENAME : gw_pragma_undefs.h
*
* DESCRIPTION : Contains global pragma defines for byte alignment in structures.
*               The idea is to include gw_pragma_defs.h file in the beginning of
*               a header file that contains structure declarations and include
*               gw_pragma_undefs.h at the end of it. This is to ensure that
*               these pragma defines are effective in only this header file.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* 19/1/2001  Sachin Mittal                    Initial
*
* 15/04/2002 Ravi Jindal	SPR-3928      To add Linux OS Port.
* 12/07/2004 Navneet Gupta  SPR 6946      QNX port
*
*
* COPYRIGHT , Aricent, 2006
*
*******************************************************************************/

/* Because of the idea explained above the conditional inclusion starts in
gw_pragma_defs.h and ends in gw_pragma_undefs.h */

#ifdef _GW_PRAGMA_H_
#undef _GW_PRAGMA_H_

#ifdef GW_WINNT
/* MSVC++ version 6.0 is the development environment */
#pragma pack(pop, gw_defs)
#endif

#ifdef GW_VXWORKS
#if 0
#pragma align 0
#endif
#endif

#if defined(GW_SOLARIS) || defined(GW_LINUX) || defined(GW_QNX)
/* to be added */
#pragma pack()
#endif

#endif  /* end of _GW_PRAGMA_H_ */
