#ifeq ($(shell dir $(PROGRAM_PATH)/make.config 2>&1), $(PROGRAM_PATH)/make.config)
#include                 $(PROGRAM_PATH)/make.config
#else
#$(error "Cannot find $(PROGRAM_PATH)/make.config"!)
#endif
LIB = $(AR) -ruv
RM = rm -f
CP = cp

TARGET = driver_itf
DRIVER_ITF_LIB = libdriver_itf.a 
MM_DRIVER_ITF_LIB = libmm_driver_itf.a

SOURCE_DIR = $(PRJ_BASE)/$(TARGET)/src

OBJ_DIR = $(PRJ_BASE)/$(TARGET)/make/linux

INCLUDES =	-I$(PRJ_BASE)/$(TARGET)/h\
		-I$(PRJ_BASE)/config/h\
		-I$(PRJ_BASE)/voice/h\
		-I$(PRJ_BASE)/common/h\
		-I$(ICF_DEV_DIR)/ext_if\
		-I$(SRTP_LIB_DIR)/include \
		-I$(SRTP_LIB_DIR)/crypto/include \
		-I$(PRJ_BASE)/mm/common/h\
		-I$(PRJ_BASE)/mm/core/h\
		-I$(PRJ_BASE)/mm/cfg/h\
		-I$(PRJ_BASE)/mm/port/h\
		-I$(PRJ_BASE)/config/h\
		-I$(PRJ_BASE)/broadcom/vodsl/voip/inc\
		$(IPTK_COMMON_INC)

EXTRA_LDFLAGS =

SRCS =  slic_endpoint_itf.c\
	dsp_endpoint_itf.c\
	Endpoint_itf.c	
	
MM_SRCS = dsp_driver_itf.c\
		flash_itf.c\
		Endpoint_itf_mm.c

OBJS =  slic_endpoint_itf.o\
	dsp_endpoint_itf.o\
	Endpoint_itf.o
	

MM_OBJS = dsp_driver_itf.o\
			Endpoint_itf_mm.o\
			flash_itf.o

#***********************************************************
#command line variables definition in the makefile---END
#***********************************************************
COPTIONS =	-g -c -Wall -Wimplicit -Wmissing-prototypes\
		-Wshadow -Wcomment -Waggregate-return\
		-Wmissing-declarations -Wreturn-type\
		-Wparentheses -Wpointer-arith\
		-fno-builtin -fno-inline-functions\
		-fsigned-char -pipe -Wstrict-prototypes\
		-Wno-format -W -fcond-mismatch\
		-Wswitch -pedantic

CFLAGS_CMN =  $(COPTIONS)  -D__ANSI_C__ -DICF_PORT_LINUX $(CMS_COMPILE_FLAGS)


all: $(DRIVER_ITF_LIB) $(MM_DRIVER_ITF_LIB)
	$(CP) $(DRIVER_ITF_LIB) $(PRJ_BASE)/voiceApp/build/lib/linux
	$(CP) $(MM_DRIVER_ITF_LIB) $(PRJ_BASE)/mm/port/build/lib/linux
	
$(DRIVER_ITF_LIB): $(OBJS)
	$(LIB) $(DRIVER_ITF_LIB) $(OBJS)	
	
$(MM_DRIVER_ITF_LIB): $(MM_OBJS)
	$(LIB) $(MM_DRIVER_ITF_LIB) $(MM_OBJS)		

%.o: $(SOURCE_DIR)/%.c
	$(CC) $(INCLUDES) $(CFLAGS_CMN) -c $< -o $@

clean:
	$(RM) -f $(OBJS) $(DRIVER_ITF_LIB) $(MM_OBJS) $(MM_DRIVER_ITF_LIB)
