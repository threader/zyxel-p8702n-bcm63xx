#******************************************************************************
#        Definitions for makefile.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Dec,2004         Mayank Rastogi                 Initial Version
#        Apr,2005         Amit Sharma                    Added services flags
#        30-Apr-2009      Anuradha Gupta    SPR 19443    Algining CC, CFLAGS options
#        13-Jul-2009      Rajiv Kumar       IPTK Rel 8.2 Openssl Removal
#        02-Aug-2009	  Rajiv Kumar	    SPR 19886    Merged CSR-1-7423906
#        21-Aug-2009      Tarun Gupta       SPR 19932    Changed OPEN_SSL_INCLUDE_PATH
#        25-Aug-2009      Rajiv Kumar       SPR: 19982   OPENSSL include Path
#                                                        removed for Linux
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************
CPU = intel

# The make to be done in Bourne shell
#
SHELL           =       /bin/sh

GW_COMMON = $(ICF_DEV_DIR)/../gw
NAT_LIB_COMMON = $(ICF_DEV_DIR)/../utils/natlib
ICF_SRC_DIR = $(ICF_DEV_DIR)/source
STACK = $(ICF_SRC_DIR)/stacks/sip_micro_ua/microua_3_0
WARNINGS = -Wall -Wstrict-prototypes -Wimplicit -Wmissing-prototypes -Wmissing-declarations -Wreturn-type -Wcomment -Waggregate-return -Wparentheses -Wpointer-arith -fno-builtin -fsigned-char -pipe -Wstrict-prototypes -Wno-format -W -fcond-mismatch -Wnested-externs -Wswitch -fsigned-char


# Two comments removed -O(optimizing flag) and -Wredundant-decls 

#ZyXEL porting
#CC_CMN=gcc -g
#CC_CMN=gcc
#CC = $(CC_CMN)

#ICF_LD = ld
#ICF_AR = ar

ICF_LD = $(CROSS_COMPILE)ld
ICF_AR = $(CROSS_COMPILE)ar
#ZyXEL porting end

ICF_RM = rm -f
ICF_CP = cp
ICF_CD = cd
ICF_MV = mv
ICF_AR_FLAGS = -r
ICF_LD_FLAGS = -r

COPTIONS = -c


INCLUDE		= -I$(ICF_SRC_DIR)/common/h \
				-I$(ICF_DEV_DIR)/ext_if \
				-I$(STACK)/ua_3_0_src/source/UAlib/h \
				-I$(ICF_SRC_DIR)/port/h

#IPTK_SUPPORT_TLS
ifeq ($(IPTK_SUPPORT_TLS), yes)
INCLUDE		+= -I $(BUILD_DIR)/userspace/public/libs/openssl/include
CFLAGS		= -x c $(COPTIONS) $(WARNINGS) \
				-DICF_PORT_LINUX -UICF_PORT_DEBIAN -DICF_ENABLE_TLS
else
CFLAGS		= -x c $(COPTIONS) $(WARNINGS) \
				-DICF_PORT_LINUX -UICF_PORT_DEBIAN
endif

#IPTK_DISABLE_PRACK
ifeq ($(IPTK_DISABLE_PRACK), yes)
CFLAGS		+= -DMSTC_DISABLE_PRACK
endif

ICF_LIBS	= -lsocket -lnsl
