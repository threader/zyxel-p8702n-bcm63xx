MM_COMMON_BASE = $(PRJ_BASE)/mm/common
MM_COMMON_SRC = $(MM_COMMON_BASE)/src
#CC = gcc
#LIB = ar -ruv
LIB = $(AR) -ruv
RM = rm -f
CP = cp

MM_COMMON_INCLUDES =	-I$(MM_COMMON_BASE)/h\
			-I$(PRJ_BASE)/common/h\
			-I$(PRJ_BASE)/mm/port/h\
			-I$(IPTK_BASE)/ICF/ext_if\
			$(IPTK_COMMON_INC)


MM_COMMON_OBJ_FILES = voice_dsp_utils.o

MM_COMMON_SRC_FILES = voice_dsp_utils.c

MM_COMMON_LIB_A = libmmcommon.a

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


######################################################
#   #
######################################################
all: $(MM_COMMON_LIB_A)
	$(CP) $(MM_COMMON_LIB_A) $(PRJ_BASE)/voiceApp/build/lib/linux
	$(CP) $(MM_COMMON_LIB_A) $(PRJ_BASE)/mm/port/build/lib/linux
$(MM_COMMON_LIB_A): $(MM_COMMON_OBJ_FILES)
		$(LIB) $(MM_COMMON_LIB_A) $(MM_COMMON_OBJ_FILES)

%.o:$(MM_COMMON_SRC)/%.c
	$(CC) $(MM_COMMON_INCLUDES) $(CFLAGS_CMN) -c $< -o $@

clean:
	$(RM) $(MM_COMMON_LIB_A) $(MM_COMMON_OBJ_FILES)
