
# File: drivers/char/bcm96352/endpoint/vocm/bos/makefile
#
LIB  = libbos.so
OBJS = $(BOSTARGET)

BOSTARGET       = bos.o
BOSLIBS_SUBDIR  = lib
BOSLIBS			 = assert bos str
XCHANGE_BUILD_EXTRA_CFLAGS = $(addprefix -D ,$(BRCM_VOICE_GLOBAL_CFLAGS))
ifeq ($(strip $(BRCM_MIPS_ONLY_BUILD)),y)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , ENDPOINT_DRIVER_MIPS_ONLY)
endif
ifeq ($(strip $(BRCM_LDX_APP)),1_97)
XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , LDX_APP_1_97)
endif

XCHANGE_BUILD_EXTRA_CFLAGS += $(addprefix -D , _GNU_SOURCE)

ifneq ($(strip $(BRCM_LDX_APP)),1_97)
BOSLIBS += blog
endif

bos.o: $(BOSLIBS_SUBDIR)/*.o $(LIB) generic_private_lib_install
	$(LD) -G 0 -r lib/*.o -o $(BOSTARGET);
include $(BUILD_DIR)/make.common

$(BOSLIBS_SUBDIR)/*.o:
	cd $(XCHANGE_DIR)/dslx;					\
	$(XCHANGE_DIR)/dslx/build -a $(XCHANGE_BUILD_APP) -d $(XCHANGE_DSP_APP)$(XCHANGE_DSP_APP_EXTENSION) -t bosu $(XCHANGE_BUILD_EXTRA_CFLAGS);
	for i	in	$(BOSLIBS);	do					\
		cp $(XCHANGE_DIR)/dslx/lib/LinuxUser/$(XCHANGE_BUILD_APP)/xchg/$$i.a $(CURDIR)/$(BOSLIBS_SUBDIR)/$$i.lib;										\
	done;											\

	cd	$(CURDIR)/$(BOSLIBS_SUBDIR);		\
	for i	in	$(BOSLIBS);	do					\
		$(AR)	x $$i.lib;						\
	done;									

	
$(LIB):
	$(LD) -shared -G 0 -soname $(LIB) --whole-archive $(BOSLIBS_SUBDIR)/*.o -o $(LIB);

clean:
	rm -f $(OBJS) bos.o $(BOSLIBS_SUBDIR)/*.a $(LIB) lib/*.o
