
#export CPU=PENTIUM
export CPU=intel
#export MM_BASE=/home/vincent/mag/userspace/ZyXEL/new_voip
export MM_BASE=$(PRJ_BASE)/mm
export ICF_BASE=$(IPTK_BASE)/ICF
export CP=cp
export OS=linux
#export CC=gcc

#CC=/opt/Rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -windows=yes -log-file=ssastub_purify.plog -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g

#for purify
#CC=/opt/Rational/releases/purify.i386_linux2.2003a.06.15/purify -best-effort -inuse_at_exit=yes -threads=yes -use-internal-locks=yes  -cache-dir=$(ICF_DEV_DIR)/temp/ -max_threads=40 -chain-length="30" -mt_safe_malloc=yes gcc -g
#LIB		        =	ar -ruv
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

CFLAGS_CMN          =  $(COPTIONS) -D__ANSI_C__ -DICF_PORT_LINUX -DICF_TRACE_ENABLE -DZYXEL_PORTING -DCHIP_ID=\"$(BRCM_CHIP)\"
ifneq ($(strip $(BUILD_MSTC_VOICE_SRTP)),)
CFLAGS_CMN += -DMSTC_VOICE_SUPPORT_SRTP
endif
ifneq ($(strip $(BUILD_MSTC_VOICE_KSOCKET_RTP)),)
CFLAGS_CMN += -DMSTC_VOICE_KSOCKET_RTP
endif
ifneq ($(strip $(BUILD_MSTC_VOICE_412)),)
CFLAGS_CMN += -DSUPPORT_412_VOICE
endif
ifneq ($(strip $(BUILD_NORWAY_VOIP_CUSTOMIZATION)),)
CFLAGS_CMN += -DNORWAY_VOIP_CUSTOMIZATION
endif
#*******************************************************
# Common header file Path
# ******************************************************
MM_COMMON_INCLUDES		=	-I$(MM_BASE)/common/h

######################
#macro for Sample Program archive paths
######################

LIB_OS				=	$(MM_BASE)/port/build/lib/$(OS)

MM_LIB_PATH	=	-L$(LIB_OS) -L$(PRJ_BASE)/ICF/lib/linux

# DEFINITIONS FOR MM Program - CFG+CORE+PORT

MM_CFG_BASE				=	$(MM_BASE)/cfg
MM_CFG_OBJ				=	$(MM_BASE)/cfg
MM_CFG_LIB_O			=	$(MM_CFG_OBJ)/libcfg.o
MM_CFG_LIB_A			=	$(MM_CFG_OBJ)/libcfg.a
LIB_OS_MM_CFG_LIB_O			=	$(LIB_OS)/libcfg.o
LIB_OS_MM_CFG_LIB_A			=	$(LIB_OS)/libcfg.a

MM_CORE_BASE				=	$(MM_BASE)/core
MM_CORE_OBJ				=	$(MM_BASE)/core
MM_CORE_LIB_O			=	$(MM_CORE_OBJ)/libcore.o
MM_CORE_LIB_A			=	$(MM_CORE_OBJ)/libcore.a
LIB_OS_MM_CORE_LIB_O			=	$(LIB_OS)/libcore.o
LIB_OS_MM_CORE_LIB_A			=	$(LIB_OS)/libcore.a

MM_PORT_BASE			=	$(MM_BASE)/port
MM_PORT_OBJ			=	$(MM_BASE)/port
MM_PORT_LIB_O			=	$(MM_PORT_OBJ)/libport.o
MM_PORT_LIB_A			=	$(MM_PORT_OBJ)/libport.a
LIB_OS_MM_PORT_LIB_O		=	$(LIB_OS)/libport.o
LIB_OS_MM_PORT_LIB_A		=	$(LIB_OS)/libport.a


ifeq ($(strip $(VOICE_HW)),BRCM6368)
MM_AR	=	 -lport -lvldt -lclib -lvldt -lcore -lport -lclib -lcfg -lmm_driver_itf -lbroadcom -lpthread -lmmcommon -lcommon -lsys_itf
else
MM_AR	=	 -lport -lvldt -lclib -lvldt -lcore -lport -lclib -lcfg -lmm_driver_itf -lpthread -lmmcommon -lcommon -lsys_itf 
endif

ifneq ($(strip $(BUILD_MSTC_VOICE_SRTP)),)
MM_AR += -lsrtp
endif
