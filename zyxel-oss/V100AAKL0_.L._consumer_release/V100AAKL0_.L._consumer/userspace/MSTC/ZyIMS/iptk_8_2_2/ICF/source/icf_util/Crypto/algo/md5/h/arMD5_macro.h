/******************************************************************************
*                                    
* FILE NAME    : arMD5_macro.h                
*                                    
* DESCRIPTION  : Contains the definitions for macros. 
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 19-Dec-2008    Alok Tiwari             None          Initial
* 22-Dec-2008    Alok Tiwari             LLd           arMD5 1.0            
*
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/

#ifndef __ARMD5_MACRO_H__
#define __ARMD5_MACRO_H__

/* Prevent Name Mangling By C++ Compilers */

#define ARMD5_LEFT_ROTATE(x,n)     (((x) << (n)) | ((x) >> (32-(n)))) 

#define	ARMD5_F(x,y,z)              (((x) & (y)) | ((~x) & (z))) 

#define	ARMD5_G(x,y,z)              (((x) & (z)) | ((y) & (~z))) 

#define	ARMD5_H(x,y,z)              ((x) ^ (y) ^ (z))            

#define	ARMD5_I(x,y,z)              ((y) ^ ((x) | (~z)))         

#define ARMD5_TRANSFORM(aux_func,a,b,c,d,x,s,ac) \
 (ARMD5_LEFT_ROTATE(((a) + aux_func((b), (c), (d)) + tab[(x)] + T_md5[(ac)]), (s)) + (b))


#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ARMD5_MACRO_H__ */



