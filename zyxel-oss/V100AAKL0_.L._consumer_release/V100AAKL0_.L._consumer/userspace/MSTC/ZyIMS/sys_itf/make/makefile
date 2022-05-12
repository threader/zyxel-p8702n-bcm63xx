INC_MK			=       ../../voiceApp/build/make/linux

include                 $(INC_MK)/makefile.mk

SYS_ITF_BASE = $(PRJ_BASE)/sys_itf
SYS_ITF_SRC = $(SYS_ITF_BASE)/src
LIB = $(AR) -ruv
RM = rm -f
CP = cp

SYS_ITF_INCLUDES =	-I$(SYS_ITF_BASE)/h\
			-I$(PRJ_BASE)/voiceApp/core/h\
			-I$(PRJ_BASE)/mm/port/h\
			-I$(PRJ_BASE)/mm/core/h\
			-I$(PRJ_BASE)/common/h\
			-I$(PRJ_BASE)/config/h\
			-I$(ICF_DEV_DIR)/ext_if\
			-I$(XCHANGE_DIR)/dslx/src/mta/inc/cfg \
			-I$(XCHANGE_DIR)/dslx/apps/cfginc/xchg_common \
			-I$(XCHANGE_DIR)/dslx_common/xchg_common/bos/publicInc \
			-I$(XCHANGE_DIR)/dslx_common/xchg_common/assert/cfginc \
			-I$(XCHANGE_DIR)/dslx_common/xchg_common/assert/inc \
			-I$(BUILD_DIR)/userspace/public/include \
			-I$(BUILD_DIR)/userspace/public/include/linux \
			-I$(BUILD_DIR)/userspace/private/include \
			-I$(BUILD_DIR)/userspace/private/include/linux

SYS_ITF_OBJ_FILES = sys_itf.o

SYS_ITF_SRC_FILES = sys_itf.c

SYS_ITF_LIB_A = libsys_itf.a

#***********************************************************
#command line variables definition in the makefile---END
#***********************************************************

ifeq ($(strip $(VOICE_HW)),BRCM6368)
CFLAGS_CMN = -DLINUX -DBOS_OS_LINUXUSER $(CMS_COMPILE_FLAGS)
else
CFLAGS_CMN = -DLINUX -DBOS_OS_LINUXUSER
endif


######################################################
#              Compiling SYS_ITF part                #
######################################################
all: $(SYS_ITF_LIB_A)
	$(CP) $(SYS_ITF_LIB_A) $(PRJ_BASE)/voiceApp/build/lib/linux

$(SYS_ITF_LIB_A): $(SYS_ITF_OBJ_FILES)
		$(LIB) $(SYS_ITF_LIB_A) $(SYS_ITF_OBJ_FILES)

%.o: $(SYS_ITF_SRC)/%.c
	$(CC) $(SYS_ITF_INCLUDES) $(CFLAGS_CMN) -c $< -o $@

clean:
	$(RM) $(SYS_ITF_LIB_A) $(SYS_ITF_OBJ_FILES)
