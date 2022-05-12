/********************************************************************
 * Copyright (C) 2008 Aricent Inc . All Rights Reserved
 *
 * $Id: cryartdf.h,v 1.4 2009/04/13 13:38:57 gur16603 Exp $
 *
 * Description: This file contains the common typedefines for Aricent 
 * Crypto.
 *********************************************************************/
#ifndef _CRY_TDF_H
#define _CRY_TDF_H
/********************************************************************/
typedef unsigned char        UINT1;
typedef unsigned short int   UINT2;
typedef unsigned int         UINT4;
typedef unsigned long long   AR_CRYPTO_UINT8;
typedef char                 INT1;
typedef int                  INT4;
typedef void                 AR_CRYPTO_VOID;

/* Structure for Keys used for AES/DES/TDES */
typedef union {
	struct tArDes{
		AR_CRYPTO_UINT8 au8ArSubkey[16]; /* Key Schedule for DES*/
	}tArDes;

	struct tArTdes{
		struct tArDes ArKey0, ArKey1, ArKey2; /* Keys required for TDES */
	}tArTdes;

	struct tArAes{
		UINT4 u4ArRounds; /* Rounds required for AES Encryption */
		UINT1 au1ArSubkey[240]; /* Key Expansion for AES */
	}tArAes;
}unArCryptoKey;

/* Union for Different Hash Functions */
typedef union {
	struct tArMd5{
		AR_CRYPTO_UINT8 u8ArNbytes;
		UINT4 au4ArState[4];
		UINT1 au1ArBlock[64];
	}tArMd5;
	struct tArSha1{
		AR_CRYPTO_UINT8 u8ArNbytes; /* Size of data to be encrypted */
		UINT4 au4ArState[5];/* Words of the hash values holds the initial values,
                           * replaced by succesessive intervmediate value and 
                           * ends with final hash value.*/
		UINT1 au1ArBlock[64];/* Character set to process the data 
                            * in 64 byte block */
	}tArSha1;
}unArCryptoHash;

/* Common structure for Encryption and Decryption routines */
typedef struct {
	UINT4 u4ArBlocksize;/*Size of data block required*/
    UINT4 u4ArKeysize;/* Key Size */
    /* Key Scheduling Function pointer*/
	AR_CRYPTO_VOID (*ArCryptoSetup)( unArCryptoKey *, const UINT1 * );
    /* Fointer pointer for Encryption Function */
	AR_CRYPTO_VOID (*ArCryptoEncrypt)( const unArCryptoKey *, const UINT1 *, UINT1 * );
    /* Function pointer for Decryption Function */
	AR_CRYPTO_VOID (*ArCryptoDecrypt)( const unArCryptoKey *, const UINT1 *, UINT1 * );
}tArBlockcipher;

typedef struct {
	UINT4 u4ArHashsize;/* Block size required for Hashing*/
    /* Function Pointer to initialise the states */
	AR_CRYPTO_VOID (*ArCryptoHashStart)( unArCryptoHash * );
    /* Function Pointer to perform hashing functionality */
	AR_CRYPTO_VOID (*ArCryptoHashUpdate)( unArCryptoHash *, const UINT1 *, UINT4 );
    /* Function Pointer to perform padding operation */
	AR_CRYPTO_VOID (*ArCryptoHashFinish)( unArCryptoHash *, UINT1 * );
}tArHashfunction;

typedef struct {
	UINT4 u4ArDataLength;/* Input Data Length */
	UINT1 au1ArKeyBlock[64];/* Input key */
    const tArHashfunction *pArHashFunction; /* Pointer to the hash function */
	unArCryptoHash  ArContext; /* Context required for selected hash function */
}tArHmac;
#endif
