#******************************************************************************
#        Definitions for makefile.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Dec,2004         Mayank Rastogi                 Initial Version
#        Apr,2005         Amit Sharma                    Added services flags
#        09-Jul-2009	  Rohit Aggarwal	SPR 19772	Changes for Big-endian
#														Linux machine
#
#        Copyright (c) 2006, Aricent.
#******************************************************************************
CPU = intel
#export ICF_SRC_DIR = $(PRJ_BASE)/iptk_7_1/ICF/source
#export ICF_DEV_DIR = $(PRJ_BASE)/iptk_7_1/ICF
GW_COMMON = $(ICF_DEV_DIR)/../gw
NAT_LIB_COMMON = $(ICF_DEV_DIR)/../utils/natlib
ICF_SRC_DIR = $(ICF_DEV_DIR)/source
STACK = $(ICF_SRC_DIR)/stacks/sip_micro_ua/microua_3_0
OPEN_SSL_INCLUDE_PATH = $(ICF_SRC_DIR)/utils/ssl/openssl/openssl-0.9.7b/include
WARNINGS = -Wall -Wstrict-prototypes -Wimplicit -Wmissing-prototypes -Wmissing-declarations -Wreturn-type -Wcomment -Waggregate-return -Wparentheses -Wpointer-arith -fno-builtin -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fsigned-char


# Two comments removed -O(optimizing flag) and -Wredundant-decls 
#For Maching 172.16.105.235
#CC=/opt/Rational/releases/purify.i386_linux2.7.0.0.0-008/purify -best-effort -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g

#For Maching 10.203.154.91
#CC=/opt/rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g

#CC=gcc
#CC=mips-linux-gcc

#ICF_LD = ld -r
ICF_LD = $(LD) -r
#ICF_LD = mips-linux-ld -r
ICF_RM = rm -f
ICF_CP = cp
ICF_CD = cd
#AR = ar
#AR = mips-linux-ar
#ICF_AR = ar
ICF_AR = $(AR) 
#ICF_AR = mips-linux-ar 
ICF_MV = mv

COPTIONS = -c

INCLUDE		= -I$(ICF_SRC_DIR)/common/h \
				-I$(ICF_DEV_DIR)/ext_if \
				-I$(ICF_SRC_DIR)/port/h

#Flag: ICF_TRACE_ENABLE:This will enable the tracing functionality
#Flag: ICF_ERROR_ENABLE:This will enable the error functionality
#Flag: ICF_SM_LOG_ENABLE: This will enable the port functionality for
# sending the traces/errors to SM.  This should be enabled only when either of
#the above mentioned flags is on

CFLAGS		= -x c $(COPTIONS) $(WARNINGS) $(INCLUDE) -c \
				-DICF_PORT_LINUX -UICF_PORT_DEBIAN

ICF_LIBS	= -lsocket -lnsl
