/******************************************************************************
*                                    
* FILE NAME    : arMD5_hash.c                
*                                    
* DESCRIPTION  : Contains the arMD5 hash Calculation routines  
*                                    
* Revision History :                            
*
*  DATE            NAME                 REFERENCE       REASON        
* ------         ------------           ---------     ----------
* 19-Dec-2008    Alok Tiwari            None          Initial
* 22-Dec-2008    Alok Tiwari            LLd           arMD5 1.0
* 09-Apr-2008    Alok Tiwari            Rel 8.2       Patch 6473 merged.
*           
* Copyright 2008, Aricent.                       
*                                    
*******************************************************************************/
#include <stdio.h>
#include "arMD5_api.h"
#include "arMD5_macro.h"
#include "arMD5_defs.h"

static AR_CRYPTO_VOID arMD5_transform(INOUT UINT4 *state, IN UINT1 *block);

static const UINT1 pad[64] = {
        0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 
 };

static const UINT4 npad[] = {
       56, 55, 54, 53, 52, 51, 50, 49, 
       48, 47, 46, 45, 44, 43, 42, 41, 
       40, 39, 38, 37, 36, 35, 34, 33, 
       32, 31, 30, 29, 28, 27, 26, 25, 
       24, 23, 22, 21, 20, 19, 18, 17, 
       16, 15, 14, 13, 12, 11, 10, 9, 
       8,  7,  6,  5,  4,  3,  2,  1, 
       64, 63, 62, 61, 60, 59, 58, 57
 };

static const UINT4 T_md5[] = {
       0xD76AA478, 0xE8C7B756, 0x242070DB, 0xC1BDCEEE,
       0xF57C0FAF, 0x4787C62A, 0xA8304613, 0xFD469501,
       0x698098D8, 0x8B44F7AF, 0xFFFF5BB1, 0x895CD7BE,
       0x6B901122, 0xFD987193, 0xA679438E, 0x49B40821,

       0xF61E2562, 0xC040B340, 0x265E5A51, 0xE9B6C7AA,
       0xD62F105D, 0x02441453, 0xD8A1E681, 0xE7D3FBC8,
       0x21E1CDE6, 0xC33707D6, 0xF4D50D87, 0x455A14ED,
       0xA9E3E905, 0xFCEFA3F8, 0x676F02D9, 0x8D2A4C8A,

       0xFFFA3942, 0x8771F681, 0x6D9D6122, 0xFDE5380C,
       0xA4BEEA44, 0x4BDECFA9, 0xF6BB4B60, 0xBEBFBC70,
       0x289B7EC6, 0xEAA127FA, 0xD4EF3085, 0x04881D05,
       0xD9D4D039, 0xE6DB99E5, 0x1FA27CF8, 0xC4AC5665,

       0xF4292244, 0x432AFF97, 0xAB9423A7, 0xFC93A039,
       0x655B59C3, 0x8F0CCC92, 0xFFEFF47D, 0x85845DD1,
       0x6FA87E4F, 0xFE2CE6E0, 0xA3014314, 0x4E0811A1,
       0xF7537E82, 0xBD3AF235, 0x2AD7D2BB, 0xEB86D391
 };

 
/*******************************************************************************
 * Function:    arMD5_transform
 *    
 * Description: This function is used for MD5 basic transformation.
 *              It transforms state based on block.
 *
 * Return:      Nothing
 ******************************************************************************/

static AR_CRYPTO_VOID arMD5_transform(INOUT UINT4 *state, IN UINT1 *block)
{
    UINT4 a, b, c, d, tab[16];
    UINT4 i;

    /* The MD5 algorithm operates on a 128-bit state, 
     * divided into four 32-bit words, denoted a,b,c and d.
     */ 
    a = state[0]; 
    b = state[1]; 
    c = state[2]; 
    d = state[3]; 

    for( i=0; i<16; i++ ) {
    	const UINT1 *b = block + 4 * i;
    
    	tab[i] = b[0] | (b[1] << 8) | (b[2] << 16) | (b[3] << 24);
    }

    /* MD5 algorithm operates on each 512-bit message block in turn,
     * each block modifying the state.The processing of a message block
     * consists of four stages,termed rounds.Each round is composed of 
     * 16 similar operations based on an auxiliary function.
     */  

    /* Round-1 */
    a = ARMD5_TRANSFORM (ARMD5_F, a, b, c, d, 0,  7,  0);
    d = ARMD5_TRANSFORM (ARMD5_F, d, a, b, c, 1,  12, 1);
    c = ARMD5_TRANSFORM (ARMD5_F, c, d, a, b, 2,  17, 2);
    b = ARMD5_TRANSFORM (ARMD5_F, b, c, d, a, 3,  22, 3);
    a = ARMD5_TRANSFORM (ARMD5_F, a, b, c, d, 4,  7,  4);
    d = ARMD5_TRANSFORM (ARMD5_F, d, a, b, c, 5,  12, 5);
    c = ARMD5_TRANSFORM (ARMD5_F, c, d, a, b, 6,  17, 6);
    b = ARMD5_TRANSFORM (ARMD5_F, b, c, d, a, 7,  22, 7);
    a = ARMD5_TRANSFORM (ARMD5_F, a, b, c, d, 8,  7,  8);
    d = ARMD5_TRANSFORM (ARMD5_F, d, a, b, c, 9,  12, 9);
    c = ARMD5_TRANSFORM (ARMD5_F, c, d, a, b, 10, 17, 10);
    b = ARMD5_TRANSFORM (ARMD5_F, b, c, d, a, 11, 22, 11);
    a = ARMD5_TRANSFORM (ARMD5_F, a, b, c, d, 12, 7,  12);
    d = ARMD5_TRANSFORM (ARMD5_F, d, a, b, c, 13, 12, 13);
    c = ARMD5_TRANSFORM (ARMD5_F, c, d, a, b, 14, 17, 14);
    b = ARMD5_TRANSFORM (ARMD5_F, b, c, d, a, 15, 22, 15);
    
    /* Round-2 */
    a = ARMD5_TRANSFORM (ARMD5_G, a, b, c, d, 1,  5,  16);
    d = ARMD5_TRANSFORM (ARMD5_G, d, a, b, c, 6,  9,  17);
    c = ARMD5_TRANSFORM (ARMD5_G, c, d, a, b, 11, 14, 18);
    b = ARMD5_TRANSFORM (ARMD5_G, b, c, d, a, 0,  20, 19);
    a = ARMD5_TRANSFORM (ARMD5_G, a, b, c, d, 5,  5,  20);
    d = ARMD5_TRANSFORM (ARMD5_G, d, a, b, c, 10, 9,  21);
    c = ARMD5_TRANSFORM (ARMD5_G, c, d, a, b, 15, 14, 22);
    b = ARMD5_TRANSFORM (ARMD5_G, b, c, d, a, 4,  20, 23);
    a = ARMD5_TRANSFORM (ARMD5_G, a, b, c, d, 9,  5,  24);
    d = ARMD5_TRANSFORM (ARMD5_G, d, a, b, c, 14, 9,  25);
    c = ARMD5_TRANSFORM (ARMD5_G, c, d, a, b, 3,  14, 26);
    b = ARMD5_TRANSFORM (ARMD5_G, b, c, d, a, 8,  20, 27);
    a = ARMD5_TRANSFORM (ARMD5_G, a, b, c, d, 13, 5,  28);
    d = ARMD5_TRANSFORM (ARMD5_G, d, a, b, c, 2,  9,  29);
    c = ARMD5_TRANSFORM (ARMD5_G, c, d, a, b, 7,  14, 30);
    b = ARMD5_TRANSFORM (ARMD5_G, b, c, d, a, 12, 20, 31);

    /* Round-3 */
    a = ARMD5_TRANSFORM (ARMD5_H, a, b, c, d, 5,  4,  32);
    d = ARMD5_TRANSFORM (ARMD5_H, d, a, b, c, 8,  11, 33);
    c = ARMD5_TRANSFORM (ARMD5_H, c, d, a, b, 11, 16, 34);
    b = ARMD5_TRANSFORM (ARMD5_H, b, c, d, a, 14, 23, 35);
    a = ARMD5_TRANSFORM (ARMD5_H, a, b, c, d, 1,  4,  36);
    d = ARMD5_TRANSFORM (ARMD5_H, d, a, b, c, 4,  11, 37);
    c = ARMD5_TRANSFORM (ARMD5_H, c, d, a, b, 7,  16, 38);
    b = ARMD5_TRANSFORM (ARMD5_H, b, c, d, a, 10, 23, 39);
    a = ARMD5_TRANSFORM (ARMD5_H, a, b, c, d, 13, 4,  40);
    d = ARMD5_TRANSFORM (ARMD5_H, d, a, b, c, 0,  11, 41);
    c = ARMD5_TRANSFORM (ARMD5_H, c, d, a, b, 3,  16, 42);
    b = ARMD5_TRANSFORM (ARMD5_H, b, c, d, a, 6,  23, 43);
    a = ARMD5_TRANSFORM (ARMD5_H, a, b, c, d, 9,  4,  44);
    d = ARMD5_TRANSFORM (ARMD5_H, d, a, b, c, 12, 11, 45);
    c = ARMD5_TRANSFORM (ARMD5_H, c, d, a, b, 15, 16, 46);
    b = ARMD5_TRANSFORM (ARMD5_H, b, c, d, a, 2,  23, 47);
    
    /* Round-4 */
    a = ARMD5_TRANSFORM (ARMD5_I, a, b, c, d, 0,  6,  48);
    d = ARMD5_TRANSFORM (ARMD5_I, d, a, b, c, 7,  10, 49);
    c = ARMD5_TRANSFORM (ARMD5_I, c, d, a, b, 14, 15, 50);
    b = ARMD5_TRANSFORM (ARMD5_I, b, c, d, a, 5,  21, 51);
    a = ARMD5_TRANSFORM (ARMD5_I, a, b, c, d, 12, 6,  52);
    d = ARMD5_TRANSFORM (ARMD5_I, d, a, b, c, 3,  10, 53);
    c = ARMD5_TRANSFORM (ARMD5_I, c, d, a, b, 10, 15, 54);
    b = ARMD5_TRANSFORM (ARMD5_I, b, c, d, a, 1,  21, 55);
    a = ARMD5_TRANSFORM (ARMD5_I, a, b, c, d, 8,  6,  56);
    d = ARMD5_TRANSFORM (ARMD5_I, d, a, b, c, 15, 10, 57);
    c = ARMD5_TRANSFORM (ARMD5_I, c, d, a, b, 6,  15, 58);
    b = ARMD5_TRANSFORM (ARMD5_I, b, c, d, a, 13, 21, 59);
    a = ARMD5_TRANSFORM (ARMD5_I, a, b, c, d, 4,  6,  60);
    d = ARMD5_TRANSFORM (ARMD5_I, d, a, b, c, 11, 10, 61);
    c = ARMD5_TRANSFORM (ARMD5_I, c, d, a, b, 2,  15, 62);
    b = ARMD5_TRANSFORM (ARMD5_I, b, c, d, a, 9,  21, 63);
   
   /*Update the state*/  
    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;
    
}    
    

/*******************************************************************************
 * Function:    arMD5_start
 *    
 * Description: This function is used to MD5 initialization. It begins an MD5
 *              operation, writing a new context.
 *
 * Return:      Nothing
 ******************************************************************************/
       
AR_CRYPTO_VOID arMD5_start(OUT unArCryptoHash *pHash_ctx )
{

#ifdef CRYPTO_DEBUG_LOG
    /* If pHash_ctx is NULL then return. Since without
     * this information no further processing can be done.
     */
    if(ARMD5_NULL == pHash_ctx)
    {
       CRYPTO_DEBUG_PRINT("arMD5_start:pHash_ctx is NULL\n");
       return; 
    }  
#endif

    pHash_ctx->tArMd5.u8ArNbytes = 0;
   
    /* The MD5 algorithm operates on a 128-bit state, 
     * divided into four 32-bit words.These sre initialized to certain
     * fixed constants. 
     */   
    pHash_ctx->tArMd5.au4ArState[0] = 0x67452301; 
    pHash_ctx->tArMd5.au4ArState[1] = 0xEFCDAB89;
    pHash_ctx->tArMd5.au4ArState[2] = 0x98BADCFE; 
    pHash_ctx->tArMd5.au4ArState[3] = 0x10325476;
}

/*******************************************************************************
 * Function:    arMD5_update
 *    
 * Description: This function is used to update MD5 block.It Continues an
 *              MD5 message-digest operation, processing another message block,
 *              and updating the context.
 *
 * Return:      Nothing
 ******************************************************************************/


AR_CRYPTO_VOID arMD5_update( INOUT unArCryptoHash *pHash_ctx, IN UINT1 *pInput,IN UINT4 input_len )
{
    
    UINT4  inputlen = input_len;
    UINT4  current_index = 0;
    UINT1  *block = ARMD5_NULL;

#ifdef CRYPTO_DEBUG_LOG
    /* If pHash_ctx is NULL then return. Since without
     * this information no further processing can be done.
     */
    if(ARMD5_NULL == pHash_ctx)
    {
       CRYPTO_DEBUG_PRINT("arMD5_update:pHash_ctx is NULL\n");
       return; 
    }  
#endif

    block = pHash_ctx->tArMd5.au1ArBlock;

    /* current_index points to the index till where the data is filled in the block.
     * but not yet processed.
     */
    current_index = pHash_ctx->tArMd5.u8ArNbytes & 0x3F;

    /* Add the inputlen in context nBytes to update the total number of processed
     * bytes in the context.
     */   
    pHash_ctx->tArMd5.u8ArNbytes += inputlen;
   
   /* Copy the input string character by character in the block and process the 
    * MD5_transform if the block is filled comletely.
    */
 
    while(inputlen)
    {
        block[current_index++] = *pInput++;
        inputlen--;
    
    /* The block is processed only when it is completely filled.
     * i.e.when the 512 bits (64 bytes) have been written in block.
     * (processing of 64-byte block)
     */  
   
    	if (ARMD5_BLOCK_SIZE == current_index)
        {
    	    arMD5_transform( pHash_ctx->tArMd5.au4ArState, block );

            /* Reset the current_index to zero. Now the input will be 
             * written in block from 0th index.
             */
            current_index = 0;
    	}
    }
}

/*******************************************************************************
 * Function:    arMD5_finish
 *    
 * Description: This function is used for MD5 finalization. It is used to ends
 *              an MD5 message-digest operation and writing the message digest
 *              at output.
 *
 * Return:      Nothing
 ******************************************************************************/

AR_CRYPTO_VOID arMD5_finish( INOUT unArCryptoHash *pHash_ctx, OUT UINT1 *pOutput )
{
    UINT4   current_index = 0;
    UINT4   index = 0;
    UINT1   msg_length[ARMD5_MSG_LENGTH] = "\0";
    AR_CRYPTO_UINT8   nbits  = 0;
    UINT4   *state = 0;
    UINT4   temp = 0;

#ifdef CRYPTO_DEBUG_LOG
    /* If pHash_ctx is NULL then return. Since without
     * this information no further processing can be done.
     */
    if(ARMD5_NULL == pHash_ctx)
    {
       CRYPTO_DEBUG_PRINT("arMD5_finish :pHash_ctx is NULL\n");
       return; 
    } 
#endif 

    state = pHash_ctx->tArMd5.au4ArState;

    /* Total number of bits in context which have been processed.  
     * Number of bits = number of bytes multiplied by 8.
     */

    nbits = pHash_ctx->tArMd5.u8ArNbytes << 3;

    /* current_index points to the index till where the data is filled in the block.
     * but not yet processed.
     */
    /* Number of unprocessed bytes in the context */
    current_index = pHash_ctx->tArMd5.u8ArNbytes & 0x3F;

    for( index = 0; index < ARMD5_MSG_LENGTH; index++ )
    {
        msg_length[index] = (UINT1)(nbits & 0xFF);
	    nbits >>= 8;
    }

    /*The required padding is done and updated*/  
    arMD5_update(pHash_ctx, pad, npad[current_index]);

    /*actual length of message is updated in last 64 bits block*/ 
    arMD5_update(pHash_ctx, msg_length, ARMD5_MSG_LENGTH);

    /*Now output is the 128 bit encoded mesasge.*/ 
    for (index = 0; index < 4; index++)
    {
        temp = state[index];
    
        /* Output will be state[0],state[1],state[2],state[3].
         * i.e. the pOutput will begin with the lower-order byte of state[0] 
         * and end with the high-order byte of state[3]. 
         */

        *pOutput++ = (UINT1) (temp & 0xFF); temp >>= 8;
        *pOutput++ = (UINT1) (temp & 0xFF); temp >>= 8;
        *pOutput++ = (UINT1) (temp & 0xFF); temp >>= 8;
        *pOutput++ = (UINT1) (temp & 0xFF); 
    }
}


