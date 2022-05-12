#ifdef SDF_AUTHENTICATE

/******************************************************************************
** FUNCTION:
**			This file has the implementation of the Digest authentication
**			scheme.
**
*******************************************************************************
**
** FILENAME			: digcalc.c
**
** DESCRIPTION		: This file has the implementation of the Digest
**						authentication scheme
**  	
** DATE			NAME		REFERENCE		REASON
** ----			----		---------		------
** 13/01/01 	Subhash						Creation
**
*******************************************************************************
**				COPYRIGHT , Aricent, 2006
*******************************************************************************/

#include "ALheader.h"
#include "md5algo.h"
#include "digcalc.h"
#include "sdf_common.h"
#include "sdf_portlayer.h"

void CvtHex(
    IN HASH Bin,
    OUT HASHHEX Hex
    )
{
    unsigned short i;
    unsigned char j;

    for (i = 0; i < HASHLEN; i++) {
        j = (Bin[i] >> 4) & 0xf;
        if (j <= 9)
            Hex[i*2] = (j + '0');
         else
            Hex[i*2] = (j + 'a' - 10);
        j = Bin[i] & 0xf;
        if (j <= 9)
            Hex[i*2+1] = (j + '0');
         else
            Hex[i*2+1] = (j + 'a' - 10);
    };
    Hex[HASHHEXLEN] = '\0';
}

/* calculate H(A1) as per spec */
void DigestCalcHA1(
    IN char * pszAlg,
    IN char * pszUserName,
    IN char * pszRealm,
    IN char * pszPassword,
    IN char * pszNonce,
    IN char * pszCNonce,
    OUT HASHHEX SessionKey
    )
{
      SDF_MD5_CTX Md5Ctx;
      HASH HA1;
      HASHHEX HHA1;  /* MD5-Sess Fix Added*/

      sdf_MD5Init(&Md5Ctx);
      sdf_MD5Update(&Md5Ctx, (unsigned char *)pszUserName, Sdf_mc_strlen(pszUserName));
      sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
      sdf_MD5Update(&Md5Ctx,(unsigned char *) pszRealm, Sdf_mc_strlen(pszRealm));
      sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
      sdf_MD5Update(&Md5Ctx,(unsigned char *) pszPassword, Sdf_mc_strlen(pszPassword));
      sdf_MD5Final((unsigned char *)HA1, &Md5Ctx);

      CvtHex(HA1, HHA1);  /* MD5-Sess Fix Added */

      if (Sdf_mc_strcasecmp(pszAlg, "md5-sess") == 0) {
            sdf_MD5Init(&Md5Ctx);
            sdf_MD5Update(&Md5Ctx,(unsigned char *) HHA1, HASHHEXLEN); /* MD5-Sess Fix Added*/
           /*sdf_MD5Update(&Md5Ctx,(unsigned char *) HA1, HASHLEN);  MD5-Sess Fix Deleted */
	    
            sdf_MD5Update(&Md5Ctx,(unsigned char *) HA1, HASHLEN);
            sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
			if(pszNonce !=Sdf_co_null)
			{
            sdf_MD5Update(&Md5Ctx,(unsigned char *) pszNonce, Sdf_mc_strlen(pszNonce));
            sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
			}
			if(pszCNonce!=Sdf_co_null)
			{
            sdf_MD5Update(&Md5Ctx,(unsigned char *) pszCNonce, Sdf_mc_strlen(pszCNonce));
            sdf_MD5Final((unsigned char *)HA1, &Md5Ctx);
			}
      };
      CvtHex(HA1, SessionKey);
}

/* calculate H(entity-body) as per spec */
void DigestCalcHEntity(
	IN char * pszEntityBody,
	int dLength,
	OUT HASHHEX HEntity
	)
{
	SDF_MD5_CTX Md5Ctx;
	HASH temp;

	sdf_MD5Init(&Md5Ctx);
	sdf_MD5Update(&Md5Ctx,(unsigned char *) pszEntityBody, dLength);
	sdf_MD5Final((unsigned char *)temp, &Md5Ctx);

	CvtHex(temp, HEntity);
}

/* calculate request-digest/response-digest as per HTTP Digest spec */
void DigestCalcResponse(
    IN HASHHEX HA1,           /* H(A1) */
    IN char * pszNonce,       /* nonce from server */
    IN char * pszNonceCount,  /* 8 hex digits */
    IN char * pszCNonce,      /* client nonce */
    IN char * pszQop,         /* qop-value: "", "auth", "auth-int" */
    IN char * pszMethod,      /* method from the request */
    IN char * pszDigestUri,   /* requested URL */
    IN HASHHEX HEntity,       /* H(entity body) if qop="auth-int" */
    OUT HASHHEX Response      /* request-digest or response-digest */
    )
{
      SDF_MD5_CTX Md5Ctx;
      HASH HA2;
      HASH RespHash;
       HASHHEX HA2Hex;

      /* calculate H(A2) */
      sdf_MD5Init(&Md5Ctx);
      sdf_MD5Update(&Md5Ctx,(unsigned char *) pszMethod, Sdf_mc_strlen(pszMethod));
      sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
      sdf_MD5Update(&Md5Ctx,(unsigned char *) pszDigestUri, Sdf_mc_strlen(pszDigestUri));
	  if (pszQop != Sdf_co_null)
	  {
      	if (Sdf_mc_strcasecmp(pszQop, "auth-int") == 0) {
            sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
            sdf_MD5Update(&Md5Ctx,(unsigned char *) HEntity, HASHHEXLEN);
      	};
	  }
      sdf_MD5Final((unsigned char *)HA2, &Md5Ctx);
       CvtHex(HA2, HA2Hex);

      /* calculate response */
      sdf_MD5Init(&Md5Ctx);
      sdf_MD5Update(&Md5Ctx,(unsigned char *) HA1, HASHHEXLEN);
      sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
	  if(pszNonce !=Sdf_co_null)
	  {
      sdf_MD5Update(&Md5Ctx,(unsigned char *) pszNonce, Sdf_mc_strlen(pszNonce));
      sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
	  }
	  if ((pszQop != Sdf_co_null) && (Sdf_mc_strcmp(pszQop,""))!= 0)
	  {
          sdf_MD5Update(&Md5Ctx,(unsigned char *) pszNonceCount, Sdf_mc_strlen(pszNonceCount));
          sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
          sdf_MD5Update(&Md5Ctx,(unsigned char *) pszCNonce, Sdf_mc_strlen(pszCNonce));
          sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
          sdf_MD5Update(&Md5Ctx,(unsigned char *) pszQop, Sdf_mc_strlen(pszQop));
          sdf_MD5Update(&Md5Ctx,(unsigned char *) ":", 1);
      };
      sdf_MD5Update(&Md5Ctx,(unsigned char *) HA2Hex, HASHHEXLEN);
      sdf_MD5Final((unsigned char *)RespHash, &Md5Ctx);
      CvtHex(RespHash, Response);
}
#endif /* SDF_AUTHENTICATE */
