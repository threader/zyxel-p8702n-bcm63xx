#include <bcmos.h>
#include <hausenv.h>

#ifndef _TESTVOIP_H
#define _TESTVOIP_H

#ifdef __cplusplus
extern "C" {
#endif

int testSocketOpen( void );
int testSocketBind( void );
int testSocketSend( void );

#ifdef __cplusplus
    }
#endif

#endif /* _TESTVOIP_H */
