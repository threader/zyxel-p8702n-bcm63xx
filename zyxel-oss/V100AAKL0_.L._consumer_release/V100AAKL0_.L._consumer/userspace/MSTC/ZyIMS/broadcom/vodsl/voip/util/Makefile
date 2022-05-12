#SUBDIRS  = net regexp signal qos firewall map log
SUBDIRS  = net regexp signal qos map log
OBJS = $(foreach obj,$(SUBDIRS),$(obj)/$(obj).o)

SUBDIRS += bmMgr bqm
ifeq ($(strip $(BRCM_VODSL_VOICE_ANNOUNCEMENT)),1)
SUBDIRS += snd
endif

SUBDIRS += boardIoctl

ifeq ($(strip $(BRCM_VODSL_CALL_FEATURE_PERSIST_STORE_SUPPORT)),1)
SUBDIRS += feature
endif

util.o: subdirs $(OBJS)
	$(LD) $(EXTRA_LDFLAGS) -r -o $@ $(OBJS) 

subdirs: $(patsubst %, _dir_%, $(SUBDIRS))

$(patsubst %, _dir_%, $(SUBDIRS)) :
	$(MAKE) -C $(patsubst _dir_%, %, $@) $(TGT)

clean:
	$(MAKE) -i TGT=clean subdirs
	-rm -f util.o



