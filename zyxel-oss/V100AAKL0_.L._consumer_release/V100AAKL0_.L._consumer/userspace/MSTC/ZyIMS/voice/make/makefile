VOICE_BASE = $(PRJ_BASE)/voice
VOICE_SRC = $(VOICE_BASE)/src
LIB = $(AR) -ruv
RM = rm -f
CP = cp

VOICE_INCLUDES =	-I$(VOICE_BASE)/h\
			-I$(PRJ_BASE)/voiceApp/core/h\
			-I$(PRJ_BASE)/common/h\
			-I$(PRJ_BASE)/config/h\
			-I$(PRJ_BASE)/mm/common/h\
			-I$(ICF_DEV_DIR)/ext_if\
			-I$(PRJ_BASE)/../../public/include\
			-I$(PRJ_BASE)/../../private/include\
			-I$(PRJ_BASE)/../../public/include/linux\
			$(IPTK_COMMON_INC)

VOICE_OBJ_FILES = call_mgnt.o\
			phfsm.o\
			callTrans.o\
			callConf.o\
			Voipccm_sip_itf.o\
			phcmd.o\
			zyphone.o\
			fxs.o\
			ivrcmd.o\
			mwiParse.o\
			numberingplan.o\
			dialplan.o\
			webCallHistory.o\
			fxo.o\
			webDial_mtn.o\
			webDial_fsm.o

VOICE_SRC_FILES = call_mgnt.c\
			phfsm.c\
			callTrans.c\
			callConf.c\
			Voipccm_sip_itf.c\
			phcmd.c\
			zyphone.c\
			fxs.c\
			ivrcmd.c\
			mwiParse.c\
			numberingplan.c\
			dialplan.c\
			webCallHistory.c\
			fxo.c\
			webDial_mtn.c\
			webDial_fsm.c


VOICE_LIB_O = voice.o
VOICE_LIB_A = libvoice.a

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
			$(BRCM_DSP_CHAN_DEFINES) $(CMS_COMPILE_FLAGS)

ifneq ($(strip $(BUILD_MSTC_UNREG_INTERNAL_CALL)),)
CFLAGS_CMN += -DMSTC_UNREG_INTERNAL_CALL
endif
ifneq ($(strip $(BUILD_MSTC_EMAIL_NOTIFICATION)),)
CFLAGS_CMN += -DMSTC_EMAIL_NOTIFICATION
endif

######################################################
#   Compiling VOICE part : ccm/phone state machine   #
######################################################
all: $(VOICE_LIB_A)
	$(CP) $(VOICE_LIB_A) $(PRJ_BASE)/voiceApp/build/lib/linux

$(VOICE_LIB_A): $(VOICE_OBJ_FILES)
		$(LIB) $(VOICE_LIB_A) $(VOICE_OBJ_FILES)

%.o: $(VOICE_SRC)/%.c
	$(CC) $(VOICE_INCLUDES) $(CFLAGS_CMN) -c $< -o $@

clean:
	$(RM) $(VOICE_LIB_A) $(VOICE_OBJ_FILES)
