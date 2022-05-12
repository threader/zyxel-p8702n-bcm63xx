/******************************************************************************
 * 
 * 
 * FILE NAME:   stun_enc_dec_sha.h 
 * 
 * DESCRIPTION: This file contains the types and prototypes of SHA algo
 *              
 * REVISION HISTORY:
 *       Name              Date            Reference
 *    Anuradha Gupta    27-Nov-2006        Initial Draft
 *          
 * Copyright 2006, Aricent                                *
 ******************************************************************************/

#ifndef __STUN_ENC_DEC_SHA_H__
#define __STUN_ENC_DEC_SHA_H__

#ifdef __cplusplus

extern "C"
{
#endif /* __cplusplus */
    
/* 
 *  This structure will hold context information for the hashing
 *  operation
 */
typedef struct SHA1Context
{
    unsigned Message_Digest[5]; /* Message Digest (output)          */

    unsigned Length_Low;        /* Message length in bits           */
    unsigned Length_High;       /* Message length in bits           */

    unsigned char Message_Block[64]; /* 512-bit message blocks      */
    int Message_Block_Index;    /* Index into message block array   */

    int Computed;               /* Is the digest computed?          */
    int Corrupted;              /* Is the message digest corruped?  */
} SHA1Context;

/*
 *  Function Prototypes
 */
void SHA1Reset(SHA1Context *);
int SHA1Result(SHA1Context *);
void SHA1Input( SHA1Context *,
                const unsigned char *,
                unsigned);


#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __STUN_ENC_DEC_SHA_H__ */



