CONFIG_BASE = $(PRJ_BASE)/config
CONFIG_SRC = $(CONFIG_BASE)/src
#CC = gcc
#LIB = ar -ruv
LIB = $(AR) -ruv
RM = rm -f
CP = cp

CONFIG_INCLUDES =	-I$(CONFIG_BASE)/h\
			-I$(ICF_DEV_DIR)/ext_if\
			-I$(PRJ_BASE)/voiceApp/core/h\
			-I$(PRJ_BASE)/common/h\
			-I$(PRJ_BASE)/driver_itf/h\
			-I$(PRJ_BASE)/voice/h\
			-I$(PRJ_BASE)/mm/common/h \
			-I$(BUILD_DIR)/userspace/public/include\
			-I$(BUILD_DIR)/userspace/public/include/linux\
			-I$(BUILD_DIR)/userspace/private/include\
			-I$(BUILD_DIR)/userspace/private/include/linux

CONFIG_OBJ_FILES = config_sys.o commands.o tr104.o tr104_init.o tr104_voicemdm.o tr104_convert.o config_voip_stats.o

CONFIG_SRC_FILES = config_sys.c commands.c tr104.c tr104_init.c tr104_voicemdm.c tr104_convert.c config_voip_stats.c

CONFIG_LIB_A = libconfig.a

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

CFLAGS_CMN          =  $(COPTIONS)  -D__ANSI_C__ -DICF_PORT_LINUX\
												-DLINUX -DBOS_OS_LINUXUSER\
					$(BRCM_DSP_CHAN_DEFINES) $(CMS_COMPILE_FLAGS)


######################################################
#              Compiling CONFIG part                 #
######################################################
all: $(CONFIG_LIB_A)
	$(CP) $(CONFIG_LIB_A) $(PRJ_BASE)/voiceApp/build/lib/linux

$(CONFIG_LIB_A): $(CONFIG_OBJ_FILES)
		$(LIB) $(CONFIG_LIB_A) $(CONFIG_OBJ_FILES)

%.o: $(CONFIG_SRC)/%.c
	$(CC) $(CONFIG_INCLUDES) $(CFLAGS_CMN) -c $< -o $@

clean:
	$(RM) $(CONFIG_LIB_A) $(CONFIG_OBJ_FILES)
