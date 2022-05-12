COMMON_BASE = $(PRJ_BASE)/common
COMMON_SRC = $(COMMON_BASE)/src
LIB = $(AR) -ruv
RM = rm -f
CP = cp

CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

COMMON_INCLUDES =	-I$(COMMON_BASE)/h\
			-I$(PRJ_BASE)/voiceApp/core/h\
			-I$(PRJ_BASE)/voice/h\
			-I$(PRJ_BASE)/config/h\
			-I$(ICF_DEV_DIR)/ext_if\
			-I$(PRJ_BASE)/mm/common/h\
			-I$(PRJ_BASE)/mm/port/h \
			-I$(BUILD_DIR)/userspace/public/include\
			-I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                        -I$(BUILD_DIR)/userspace/private/include\
			$(IPTK_COMMON_INC)
			
COMMON_OBJ_FILES = 	sys_ipc.o\
			sys_misc.o\
			uitimer.o\
			timer_list.o\
			syscall_wrapper.o\
			voip_syslog.o
			
COMMON_SRC_FILES = 	sys_ipc.c\
			sys_misc.c\
			uitimer.c\
			timer_list.c\
			syscall_wrapper.c\
			voip_syslog.c

COMMON_LIB_A = libcommon.a

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

CFLAGS_CMN          =  $(COPTIONS)  -D__ANSI_C__ -DICF_PORT_LINUX

ifneq ($(strip $(BUILD_MSTC_LOG)),)
CFLAGS_CMN += -DMSTC_LOG
endif

ifneq ($(strip $(BUILD_NORWAY_VOIP_CUSTOMIZATION)),)
CFLAGS_CMN += -DNORWAY_VOIP_CUSTOMIZATION
endif

######################################################
#              Compiling COMMON part                 #
######################################################
all: $(COMMON_LIB_A)
	$(CP) $(COMMON_LIB_A) $(PRJ_BASE)/voiceApp/build/lib/linux

$(COMMON_LIB_A): $(COMMON_OBJ_FILES)
		$(LIB) $(COMMON_LIB_A) $(COMMON_OBJ_FILES)

%.o: $(COMMON_SRC)/%.c
	$(CC) $(COMMON_INCLUDES) $(CFLAGS_CMN) -c $< -o $@

clean:
	$(RM) $(COMMON_LIB_A) $(COMMON_OBJ_FILES)