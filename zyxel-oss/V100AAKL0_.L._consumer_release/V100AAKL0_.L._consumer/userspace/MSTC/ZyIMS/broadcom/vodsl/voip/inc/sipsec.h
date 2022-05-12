/***************************************************************************
 *     Copyright 2006  Broadcom Corporation
 *     All Rights Reserved
 *     No portions of this material may be reproduced in any form without the
 *     written permission of:
 *              Broadcom Corporation
 *              16215 Alton Parkway
 *              P.O. Box 57013
 *              Irvine, California 92619-7013
 *     All information contained in this document is Broadcom Corporation
 *     company private, proprietary, and trade secret.
 *
 ****************************************************************************
 *
 *     Filename:  sipsec.h
 *
 ****************************************************************************
 *     Description:
 *
 *       Implementation of sec interface to ip stack
 *
 *     Notes:
 *
 ****************************************************************************/
#ifndef _SIPSEC_H
#define _SIPSEC_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_CERT_NUM    5

/*
******************************************************************************
** FUNCTION: sipSecCertsRetrieve
**
** PURPOSE:  read sip certs/keys from nvram
**
** PARAMETERS:
**
** RETURNS:
**
** NOTE:
**      MAX SIZE FOR CERTS IS 4K, FOR KEYS is 1K
**
******************************************************************************
*/
int sipSecCertsRetrieve(char **trustedCerts, char **certChain, char *priv_key);

#ifdef __cplusplus
}
#endif

#endif
