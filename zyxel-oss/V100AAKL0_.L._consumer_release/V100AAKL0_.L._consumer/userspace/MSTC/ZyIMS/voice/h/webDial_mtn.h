/*
  $Id: $
*/
/************************************************************************
 *
 *  Copyright (C) 2007 ZyXEL Communications, Inc.
 *  All Rights Reserved.
 *
 * ZyXEL Confidential; Need to Know only.
 * Protected as an unpublished work.
 *
 * The computer program listings, specifications and documentation
 * herein are the property of ZyXEL Communications, Inc. and shall 
 * not be reproduced, copied, disclosed, or used in whole or in part 
 * for any reason without the prior express written permission of
 * ZyXEL Communications, Inc.
 *
 ************************************************************************/
/*
  $Log: $

*/

#ifndef _WEB_DIAL_MTN_H
#define _WEB_DIAL_MTN_H

/*____________________________________________________________________________*/
/*	function definition                                                                                                        */
/*____________________________________________________________________________*/
int	doWebPhone(int argc, char *argv[], void *p);
int	doWebPhoneDial(int argc, char *argv[], void *p);
int	doWebPhoneStopRedial(int argc, char *argv[], void *p);
int	doWebPhoneReDial(int argc, char *argv[], void *p);
int	doWebPhoneDialStatus(int argc, char *argv[], void *p);
int	doWebPhoneDialVision(int argc, char *argv[], void *p);




#endif
