#******************************************************************************
#        Makefile for CC module of ICF.
#
#
#
#        DATE               NAME            REFERENCE      REASON
#        ----               ----            ---------      ------
#        24-Nov-2006    Anuradha Gupta                     Initial Version
#        02-Mar-2009    Alok Tiwari        IPTK Rel8.1    Changes done to
#                                            integrate arStunEncDec with IPTK
#        30-Apr-2009      Anuradha Gupta    SPR 19443    Algining CC, CFLAGS options             
#
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************
include $(ICF_DEV_DIR)/build/linux/makefile.mk
ARSTUNENCDEC_DEV_DIR=$(ICF_DEV_DIR)/source/icf_util/Utility1/arStunEncDec

oDir	=	./bin/$(CPU)
Bin	=	../../lib/linux
INCLUDE		=	-I ../../source/nat_common/h \
			-I ../../ext_if/h \
			-I ../../source/nat_glb_mgmt/h \
			-I ../../source/nat_port/h \
			-I $(ARSTUNENCDEC_DEV_DIR)/../Common/h \
			-I $(ARSTUNENCDEC_DEV_DIR)/ext_if \
			-I ../../source/stun_lifetime_sm/h \
			-I ../../source/stun_shsec_sm/h \
			-I ../../source/stun_sm/h \
			-I ../../source/stun_type_disc_sm/h \
			-I $(GW_COMMON)/common/h \
			-I $(GW_COMMON)/genlib/common/h \
			-I $(GW_COMMON)/genlib/os/h 

CFLAGS	+=	-D__ANSI_C__ -DNAT_PORT_LINUX 
SRCS	=\
	../../source/nat_glb_mgmt/src/nat_glb_mgmt_entry_point.c\
	../../source/nat_glb_mgmt/src/nat_glb_mgmt_utility.c\
	../../source/nat_port/src/nat_port_pltfm_util.c\
	../../source/nat_port/src/nat_port_pltfm_ipc.c\
	../../source/nat_port/src/nat_port_util.c\
	../../source/stun_lifetime_sm/src/stun_lifetime_entry_point.c\
	../../source/stun_shsec_sm/src/stun_shsec_api_handlers.c\
	../../source/stun_shsec_sm/src/stun_shsec_entry_point.c\
	../../source/stun_sm/src/stun_api_handlers.c\
	../../source/stun_sm/src/stun_entry_point.c\
	../../source/stun_sm/src/stun_utility.c\
	../../source/stun_type_disc_sm/src/stun_discovery_entry_point.c\
	../../source/stun_type_disc_sm/src/stun_discovery_utility.c\
	../../source/nat_common/src/nat_cmn_util.c

EXOBJS	=\
	$(oDir)/nat_glb_mgmt_entry_point.o\
	$(oDir)/nat_glb_mgmt_utility.o\
	$(oDir)/nat_port_pltfm_util.o\
	$(oDir)/nat_port_pltfm_ipc.o\
	$(oDir)/nat_port_util.o\
	$(ARSTUNENCDEC_DEV_DIR)/../Common/obj/common.o\
	$(ARSTUNENCDEC_DEV_DIR)/source/decoder/obj/arStunEncDec_dec.o\
	$(ARSTUNENCDEC_DEV_DIR)/source/encoder/obj/arStunEncDec_enc.o\
	$(oDir)/stun_lifetime_entry_point.o\
	$(oDir)/stun_shsec_api_handlers.o\
	$(oDir)/stun_shsec_entry_point.o\
	$(oDir)/stun_api_handlers.o\
	$(oDir)/stun_entry_point.o\
	$(oDir)/stun_utility.o\
	$(oDir)/stun_discovery_entry_point.o\
	$(oDir)/stun_discovery_utility.o\
	$(oDir)/stun_cmn_util.o

ALLOBJS	=	$(EXOBJS)
ALLBIN	=	$(Bin)/libnat.a
ALLTGT	=	$(Bin)/libnat.a

# User defines:

#@# Targets follow ---------------------------------

all:	libarStunEncDec $(ALLTGT)

objs:	$(ALLOBJS)

libarStunEncDec:
	@cd $(ARSTUNENCDEC_DEV_DIR)/build/linux;make

cleanobjs:
	$(ICF_RM) $(ALLOBJS)

cleanbin:
	$(ICF_RM) $(ALLBIN)

cleanarstunencdec:
	@cd $(ARSTUNENCDEC_DEV_DIR)/build/linux;make clean

clean:	cleanobjs cleanbin cleanarstunencdec 

cleanall:	cleanobjs cleanbin cleanarstunencdec

#@# User Targets follow ---------------------------------


#@# Dependency rules follow -----------------------------

$(Bin)/libnat.a: $(EXOBJS)
	$(ICF_RM) $(Bin)/libnat.a
	$(ICF_AR) $(ICF_AR_FLAGS) $(Bin)/libnat.a $(EXOBJS)

$(oDir)/nat_glb_mgmt_entry_point.o:  \
 ../../source/nat_glb_mgmt/src/nat_glb_mgmt_entry_point.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/nat_glb_mgmt_utility.o:  \
 ../../source/nat_glb_mgmt/src/nat_glb_mgmt_utility.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/nat_port_pltfm_util.o: ../../source/nat_port/src/nat_port_pltfm_util.c 
	$(CC) $(CFLAGS) $(PORT_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/nat_port_pltfm_ipc.o: ../../source/nat_port/src/nat_port_pltfm_ipc.c 
	$(CC) $(CFLAGS) $(PORT_FLAGS) $(INCLUDE) -c -o $@ $<
		
$(oDir)/nat_port_util.o: ../../source/nat_port/src/nat_port_util.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_lifetime_entry_point.o:  \
 ../../source/stun_lifetime_sm/src/stun_lifetime_entry_point.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_shsec_api_handlers.o:  \
 ../../source/stun_shsec_sm/src/stun_shsec_api_handlers.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_shsec_entry_point.o:  \
 ../../source/stun_shsec_sm/src/stun_shsec_entry_point.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_api_handlers.o: ../../source/stun_sm/src/stun_api_handlers.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_entry_point.o: ../../source/stun_sm/src/stun_entry_point.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_utility.o: ../../source/stun_sm/src/stun_utility.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_discovery_entry_point.o:  \
 ../../source/stun_type_disc_sm/src/stun_discovery_entry_point.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_discovery_utility.o:  \
 ../../source/stun_type_disc_sm/src/stun_discovery_utility.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_cmn_util.o:  \
 ../../source/nat_common/src/nat_cmn_util.c
	$(CC) $(CFLAGS) $(INCLUDE) -c -o $@ $<

depend.$(CPU):
	touch depend.$(CPU)

depend  :
	$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCS) > depend.$(CPU)
	$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(EXOBJS)

	@cd $(ARSTUNENCDEC_DEV_DIR)/build/linux;make depend

include depend.$(CPU)

#DO NOT DELETE THIS LINE -- make depend depends on it.

