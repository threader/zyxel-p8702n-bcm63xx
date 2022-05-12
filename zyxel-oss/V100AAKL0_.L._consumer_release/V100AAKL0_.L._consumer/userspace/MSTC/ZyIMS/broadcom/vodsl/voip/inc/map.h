/****************************************************************************
*
*     Copyright (c) 2005 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California  92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  maps.h
*
*  PURPOSE:
*
*     Generic data mapping table utility
*
*  NOTES:
*
****************************************************************************/

#ifndef __MAPS_H__INCLUDED__
#define __MAPS_H__INCLUDED__

/* ---- Constants and Types ---------------------------------------------- */

#define CMMAP_MAX_TEXT_WIDTH  40
#define UNKNOWN (-1)         /* end-of-map marker value */


typedef struct CMMAP
{
   int id;
   int evt;

} CMMAP;


typedef struct CMTXTMAP 
{
   int id;
   char *txt;

} CMTXTMAP;


/* ---- Function Prototypes ---------------------------------------------- */

extern int cmMapById( const CMMAP *map, int id);
extern int cmMapByEvt( const CMMAP *map, int evt);
extern int cmTxtMapByTxt( const CMTXTMAP *map, char* txt);
extern char* cmTxtMapById( const CMTXTMAP *map, int id);
extern int cmTxtMapByTxtNoCase( const CMTXTMAP *map, char *txt);
extern int cmTxtMapByTxtStr( const CMTXTMAP *map, char *txt);

#endif /* __MAPS_H__INCLUDED__ */
