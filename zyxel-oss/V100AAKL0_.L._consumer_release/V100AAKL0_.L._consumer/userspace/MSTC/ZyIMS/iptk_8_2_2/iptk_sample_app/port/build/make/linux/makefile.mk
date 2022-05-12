#***************************************************************************************
#        	Makefile Macros for Sample Program (IPTK) 
#  
#                
#  
#        DATE            NAME            REFERENCE        REASON
#        ----            ----            ---------        ------
#        07/12/2007      Amit Sikka                       Initial Creation 
#        19/04/2009      Rajiv Kumar     IPTK Rel 8.2     Fix for CSR-1-7130952 
#  
#          Copyright (c) 2009 Aricent Inc . All Rights Reserved
#
#***************************************************************************************


#***********************************************************
#command line variables definition in the makefile---BEGIN
#***********************************************************
export CPU=PENTIUM
#export SAMPLE_PROGRAM_BASE=
export IPTKAPP_BASE=$(SAMPLE_PROGRAM_BASE)$(IPTKAPP_SLASH)iptk_sample_app
export IPTK_BASE=$(SAMPLE_PROGRAM_BASE)$(IPTKAPP_SLASH)ICF
export STACK=$(SAMPLE_PROGRAM_BASE)$(IPTKAPP_SLASH)ICF/source/stacks/sip_micro_ua/microua_3_0
export CP=cp
export IPTKAPP_SLASH=/
export OS=linux
export CC=gcc

#CC=/opt/Rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -windows=yes -log-file=ssastub_purify.plog -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g

#for purify
#CC=/opt/Rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g
LIB		        =	ar -ruv

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
IPTKAPP_COMMON_INCLUDES		= -I$(IPTKAPP_BASE)$(IPTKAPP_SLASH)common$(IPTKAPP_SLASH)h \
                              -I$(STACK)/ua_3_0_src/source/UAlib/h
                              
######################
#macro for Sample Program archive paths
######################

LIB_OS				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)port$(IPTKAPP_SLASH)build$(IPTKAPP_SLASH)lib$(IPTKAPP_SLASH)$(OS)

IPTKAPP_LIB_PATH	=	-L$(LIB_OS)

# DEFINITIONS FOR Sample Program - CFG+CORE+PORT

IPTKAPP_CFG_BASE				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)cfg
IPTKAPP_CFG_OBJ				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)cfg
IPTKAPP_CFG_LIB_O			=	$(IPTKAPP_CFG_OBJ)$(IPTKAPP_SLASH)libcfg.o
IPTKAPP_CFG_LIB_A			=	$(IPTKAPP_CFG_OBJ)$(IPTKAPP_SLASH)libcfg.a
LIB_OS_IPTKAPP_CFG_LIB_O			=	$(LIB_OS)$(IPTKAPP_SLASH)libcfg.o
LIB_OS_IPTKAPP_CFG_LIB_A			=	$(LIB_OS)$(IPTKAPP_SLASH)libcfg.a

IPTKAPP_CORE_BASE				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)core
IPTKAPP_CORE_OBJ				=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)core
IPTKAPP_CORE_LIB_O			=	$(IPTKAPP_CORE_OBJ)$(IPTKAPP_SLASH)libcore.o
IPTKAPP_CORE_LIB_A			=	$(IPTKAPP_CORE_OBJ)$(IPTKAPP_SLASH)libcore.a
LIB_OS_IPTKAPP_CORE_LIB_O			=	$(LIB_OS)$(IPTKAPP_SLASH)libcore.o
LIB_OS_IPTKAPP_CORE_LIB_A			=	$(LIB_OS)$(IPTKAPP_SLASH)libcore.a

IPTKAPP_PORT_BASE			=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)port
IPTKAPP_PORT_OBJ			=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)port
IPTKAPP_PORT_LIB_O			=	$(IPTKAPP_PORT_OBJ)$(IPTKAPP_SLASH)libport.o
IPTKAPP_PORT_LIB_A			=	$(IPTKAPP_PORT_OBJ)$(IPTKAPP_SLASH)libport.a
LIB_OS_IPTKAPP_PORT_LIB_O		=	$(LIB_OS)$(IPTKAPP_SLASH)libport.o
LIB_OS_IPTKAPP_PORT_LIB_A		=	$(LIB_OS)$(IPTKAPP_SLASH)libport.a


IPTKAPP_AR	=	 -lport -lvldt -lclib -lvldt -lcore -lport -lclib -lcfg


