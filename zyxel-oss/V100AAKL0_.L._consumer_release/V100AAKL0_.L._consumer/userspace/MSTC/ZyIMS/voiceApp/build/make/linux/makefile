include		makefile.mk

IPTKAPP_EXE	=	$(IPTKAPP_BASE)$(IPTKAPP_SLASH)build$(IPTKAPP_SLASH)bin$(IPTKAPP_SLASH)voiceApp

all:
	$(MAKE) -f makefile -C $(IPTKAPP_BASE)$(IPTKAPP_SLASH)core$(IPTKAPP_SLASH)make$(IPTKAPP_SLASH)linux
ifeq ($(strip $(VOICE_HW)),BRCM6368)
	$(CC) -o $(IPTKAPP_EXE) $(IPTKAPP_LIB_PATH) $(IPTKAPP_AR) $(BRCM_PRIVATE_LIB_PATH) $(BRCM_CMS_PRIVATE_AR) $(BRCM_PUBLIC_LIB_PATH) $(BRCM_CMS_PUBLIC_AR)
else
	$(CC) -o $(IPTKAPP_EXE) $(IPTKAPP_LIB_PATH) $(IPTKAPP_AR) 
endif

clean:
	$(MAKE) -f makefile clean -C $(IPTKAPP_BASE)$(IPTKAPP_SLASH)core$(IPTKAPP_SLASH)make$(IPTKAPP_SLASH)linux
	$(RM) $(IPTKAPP_EXE)
	$(RM) $(PRJ_BASE)/voiceApp/build/lib/linux/*.a


