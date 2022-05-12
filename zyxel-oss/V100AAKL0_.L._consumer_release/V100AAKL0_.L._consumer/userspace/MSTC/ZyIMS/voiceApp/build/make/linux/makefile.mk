#***************************************************************************************
#        	Makefile Macros for Sample Program (IPTK) 
#  
#                
#  
#        DATE            NAME            REFERENCE        REASON
#        ----            ----            ---------        ------
#        07/12/2007      Amit Sikka                       Initial Creation 
#  
#          Copyright (c) 2007 Aricent Inc . All Rights Reserved
#
#***************************************************************************************


#***********************************************************
#command line variables definition in the makefile---BEGIN
#***********************************************************
#export CPU=PENTIUM
export CPU=intel
export IPTKAPP_BASE=$(PRJ_BASE)$(IPTKAPP_SLASH)voiceApp
#export IPTK_BASE=$(PRJ_BASE)$(IPTKAPP_SLASH)..$(IPTKAPP_SLASH)..$(IPTKAPP_SLASH)third-party$(IPTKAPP_SLASH)Aricent$(IPTKAPP_SLASH)iptk_7_1$(IPTKAPP_SLASH)ICF
export CP=cp
export IPTKAPP_SLASH=/
export OS=linux
#export CC=gcc
#export CC=mips-linux-gcc

#CC=/opt/Rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -windows=yes -log-file=ssastub_purify.plog -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g

#for purify
#CC=/opt/Rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g
#LIB		        =	ar -ruv
#LIB		        =	mips-linux-ar -ruv
LIB		        =	$(AR) -ruv

#***********************************************************
#command line variables definition in the makefile---END
#***********************************************************
COPTIONS                =       -g -c -Wall -Wimplicit -Wmissing-prototypes\
                                -Wshadow -Wcomment -Waggregate-return\
                                -Wmissing-declarations -Wreturn-type\
                                -Wparentheses -Wpointer-arith\
                                -fno-builtin -fno-inline-functions\
                                -fsigned-char -pipe -Wstrict-prototypes\
                                -Wno-format -W -fcond-mismatch\
                                -Wswitch -pedantic

CFLAGS_CMN          =  $(COPTIONS) -D__ANSI_C__ -DICF_PORT_LINUX

#*******************************************************
# Common header file Path
# ******************************************************
#IPTKAPP_COMMON_INCLUDES		=	-I$(IPTKAPP_BASE)$(IPTKAPP_SLASH)common$(IPTKAPP_SLASH)h

######################
#macro for Sample Program archive paths
######################

LIB_OS				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)build$(IPTKAPP_SLASH)lib$(IPTKAPP_SLASH)$(OS)

IPTKAPP_LIB_PATH	=	-L$(LIB_OS)
ifeq ($(strip $(VOICE_HW)),BRCM6368)
BRCM_PRIVATE_LIB_PATH	= -L$(INSTALL_DIR)/lib/private
BRCM_PUBLIC_LIB_PATH	= -L$(INSTALL_DIR)/lib/public
endif

# DEFINITIONS FOR Sample Program - CFG+CORE+PORT

#IPTKAPP_CFG_BASE				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)cfg
#IPTKAPP_CFG_OBJ				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)cfg
#IPTKAPP_CFG_LIB_O			=	$(IPTKAPP_CFG_OBJ)$(IPTKAPP_SLASH)libcfg.o
#IPTKAPP_CFG_LIB_A			=	$(IPTKAPP_CFG_OBJ)$(IPTKAPP_SLASH)libcfg.a
#LIB_OS_IPTKAPP_CFG_LIB_O			=	$(LIB_OS)$(IPTKAPP_SLASH)libcfg.o
#LIB_OS_IPTKAPP_CFG_LIB_A			=	$(LIB_OS)$(IPTKAPP_SLASH)libcfg.a

IPTKAPP_CORE_BASE				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)core
IPTKAPP_CORE_OBJ				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)core
IPTKAPP_CORE_LIB_O			=	$(IPTKAPP_CORE_OBJ)$(IPTKAPP_SLASH)libcore.o
IPTKAPP_CORE_LIB_A			=	$(IPTKAPP_CORE_OBJ)$(IPTKAPP_SLASH)libcore.a
LIB_OS_IPTKAPP_CORE_LIB_O			=	$(LIB_OS)$(IPTKAPP_SLASH)libcore.o
LIB_OS_IPTKAPP_CORE_LIB_A			=	$(LIB_OS)$(IPTKAPP_SLASH)libcore.a



ifeq ($(strip $(VOICE_HW)),BRCM6368)
IPTKAPP_AR	=	 -lcore -lclib -lvldt -lsys_itf -lconfig -ldriver_itf -lvoice -lcommon -lmmcommon -ldriver_itf -lbroadcom -pthread
else
IPTKAPP_AR	=	 -lcore -lclib -lvldt -lsys_itf -lconfig -ldriver_itf -lvoice -lcommon -lmmcommon -ldriver_itf -pthread
endif

ifeq ($(strip $(VOICE_HW)),BRCM6368)
BRCM_CMS_PRIVATE_AR	= $(CMS_CORE_LIBS)
endif

ifeq ($(strip $(VOICE_HW)),BRCM6368)
BRCM_CMS_PUBLIC_AR = $(CMS_COMMON_LIBS) -lcms_dal -lcms_msg
endif