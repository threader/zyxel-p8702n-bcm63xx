INC_MK			=       ../../../build/make/linux

include                 $(INC_MK)/makefile.mk

IPTKAPP_CORE_BASE			=	..$(IPTKAPP_SLASH)..
#IPTKAPP_IPTK_BASE			=	..$(IPTKAPP_SLASH)..$(IPTKAPP_SLASH)..$(IPTKAPP_SLASH)iptk
#IPTKAPP_CCM_BASE			=	..$(IPTKAPP_SLASH)..$(IPTKAPP_SLASH)..$(IPTKAPP_SLASH)ccm
IPTKAPP_CORE_SRC		=	$(IPTKAPP_CORE_BASE)$(IPTKAPP_SLASH)src

IPTKAPP_CORE_OBJ_FILES          =	voiceApp_iptk_MsgHandler.o\
					voiceApp_iptk_api.o\
					voiceApp_iptk_trace.o\
					voiceApp_main.o

IPTKAPP_CORE_SRC_FILES  =       $(IPTKAPP_CORE_SRC)$(IPTKAPP_SLASH)voiceApp_iptk_MsgHandler.c\
				$(IPTKAPP_CORE_SRC)$(IPTKAPP_SLASH)voiceApp_iptk_api.c\
				$(IPTKAPP_CORE_SRC)$(IPTKAPP_SLASH)voiceApp_iptk_trace.c\
				$(IPTKAPP_CORE_SRC)$(IPTKAPP_SLASH)voiceApp_main.c

IPTKAPP_CORE_INCLUDES		=	-I$(IPTKAPP_CORE_BASE)$(IPTKAPP_SLASH)h \
				-I$(ICF_DEV_DIR)$(IPTKAPP_SLASH)ext_if \
				-I$(PRJ_BASE)$(IPTKAPP_SLASH)common$(IPTKAPP_SLASH)h \
				-I$(PRJ_BASE)$(IPTKAPP_SLASH)config$(IPTKAPP_SLASH)h \
				-I$(PRJ_BASE)/sys_itf/h \
				-I$(PRJ_BASE)/mm/common/h \
				-I$(PRJ_BASE)/mm/port/h \
				-I$(PRJ_BASE)$(IPTKAPP_SLASH)voice$(IPTKAPP_SLASH)h \
				-I$(XCHANGE_DIR)/dslx/apps/cfginc/xchg_common \
				-I$(XCHANGE_DIR)/dslx_common/xchg_common/bos/publicInc \
				-I$(XCHANGE_DIR)/dslx_common/xchg_common/assert/cfginc \
				-I$(XCHANGE_DIR)/dslx_common/xchg_common/assert/inc \
				-I$(XCHANGE_DIR)/dslx_common/xchg_common/str \
				-I$(BUILD_DIR)/userspace/public/include \
				-I$(BUILD_DIR)/userspace/public/include/linux \
				-I$(BUILD_DIR)/userspace/private/include \
				-I$(BUILD_DIR)/userspace/private/include/linux \
				$(IPTK_COMMON_INC)

ifeq ($(strip $(VOICE_HW)),BRCM6368)
CFLAGS_CMN = -DLINUX -DBOS_OS_LINUXUSER $(IPTK_COMPILE_FLAG) $(CMS_COMPILE_FLAGS)
else
CFLAGS_CMN = -DLINUX -DBOS_OS_LINUXUSER $(IPTK_COMPILE_FLAG)
endif

#***************************************
#**Compiling CORE elements
#***************************************

all		:	depend	$(IPTKAPP_CORE_LIB_A)


$(IPTKAPP_CORE_LIB_A):	$(IPTKAPP_CORE_OBJ_FILES)
			$(LIB) $(IPTKAPP_CORE_LIB_A)  $(IPTKAPP_CORE_OBJ_FILES)
			$(CP) $(IPTKAPP_CORE_LIB_A) $(LIB_OS)

%.o: $(IPTKAPP_CORE_SRC)$(IPTKAPP_SLASH)%.c
	$(CC) $(INCLUDE) $(IPTKAPP_CORE_INCLUDES) $(IPTKAPP_COMMON_INCLUDES) $(CFLAGS_CMN) -c $< -o $@

depend.$(CPU).core:
	$(CC) -M $(INCLUDE) $(IPTKAPP_CORE_INCLUDES) $(IPTKAPP_COMMON_INCLUDES) $(CFLAGS_CMN) $(IPTKAPP_CORE_SRC_FILES) > depend.$(CPU).core

include depend.$(CPU).core
clean:
			$(RM) $(IPTKAPP_CORE_LIB_A) $(IPTKAPP_CORE_OBJ_FILES) $(LIB_OS_IPTKAPP_CORE_LIB_A)
			$(RM) depend.$(CPU).core
depend:
	$(CC) -M $(INCLUDE) $(IPTKAPP_CORE_INCLUDES) $(IPTKAPP_COMMON_INCLUDES) $(CFLAGS_CMN) $(IPTKAPP_CORE_SRC_FILES) > depend.$(CPU).core
