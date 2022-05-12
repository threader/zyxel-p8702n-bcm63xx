#******************************************************************************
#        Makefile for CC module of ICF.
#
#
#
#        DATE               NAME            REFERENCE      REASON
#        ----               ----            ---------      ------
#        24-Nov-2006    Anuradha Gupta                     Initial Version
#
#
#        Copyright (c) 2006, Aricent.
#******************************************************************************
include $(ICF_DEV_DIR)/build/linux/makefile.mk


oDir	=	./bin/$(CPU)
Bin	=	../../lib/linux
INCLUDE		=	-I ../../source/nat_common/h \
			-I ../../ext_if/h \
			-I ../../source/nat_glb_mgmt/h \
			-I ../../source/nat_port/h \
			-I ../../source/stun_enc_dec/h \
			-I ../../source/stun_lifetime_sm/h \
			-I ../../source/stun_shsec_sm/h \
			-I ../../source/stun_sm/h \
			-I ../../source/stun_type_disc_sm/h \
			-I $(GW_COMMON)/common/h \
			-I $(GW_COMMON)/genlib/common/h \
			-I $(GW_COMMON)/genlib/os/h 

C_FLAGS	=	-D__ANSI_C__ -DNAT_PORT_LINUX 
SRCS	=\
	../../source/nat_glb_mgmt/src/nat_glb_mgmt_entry_point.c\
	../../source/nat_glb_mgmt/src/nat_glb_mgmt_utility.c\
	../../source/nat_port/src/nat_port_pltfm_util.c\
	../../source/nat_port/src/nat_port_pltfm_ipc.c\
	../../source/nat_port/src/nat_port_util.c\
	../../source/stun_enc_dec/src/stun_decoder.c\
	../../source/stun_enc_dec/src/stun_encoder.c\
	../../source/stun_lifetime_sm/src/stun_lifetime_entry_point.c\
	../../source/stun_shsec_sm/src/stun_shsec_api_handlers.c\
	../../source/stun_shsec_sm/src/stun_shsec_entry_point.c\
	../../source/stun_sm/src/stun_api_handlers.c\
	../../source/stun_sm/src/stun_entry_point.c\
	../../source/stun_sm/src/stun_utility.c\
	../../source/stun_type_disc_sm/src/stun_discovery_entry_point.c\
	../../source/stun_type_disc_sm/src/stun_discovery_utility.c\
	../../source/nat_common/src/nat_cmn_util.c\
	../../source/stun_enc_dec/src/stun_sha.c

EXOBJS	=\
	$(oDir)/nat_glb_mgmt_entry_point.o\
	$(oDir)/nat_glb_mgmt_utility.o\
	$(oDir)/nat_port_pltfm_util.o\
	$(oDir)/nat_port_pltfm_ipc.o\
	$(oDir)/nat_port_util.o\
	$(oDir)/stun_decoder.o\
	$(oDir)/stun_encoder.o\
	$(oDir)/stun_lifetime_entry_point.o\
	$(oDir)/stun_shsec_api_handlers.o\
	$(oDir)/stun_shsec_entry_point.o\
	$(oDir)/stun_api_handlers.o\
	$(oDir)/stun_entry_point.o\
	$(oDir)/stun_utility.o\
	$(oDir)/stun_discovery_entry_point.o\
	$(oDir)/stun_discovery_utility.o\
	$(oDir)/stun_cmn_util.o\
	$(oDir)/stun_sha.o

ALLOBJS	=	$(EXOBJS)
ALLBIN	=	$(Bin)/libnat.a
ALLTGT	=	$(Bin)/libnat.a

# User defines:

#@# Targets follow ---------------------------------

all:	$(ALLTGT)

objs:	$(ALLOBJS)

cleanobjs:
	$(ICF_RM) $(ALLOBJS)

cleanbin:
	$(ICF_RM) $(ALLBIN)

clean:	cleanobjs cleanbin

cleanall:	cleanobjs cleanbin

#@# User Targets follow ---------------------------------


#@# Dependency rules follow -----------------------------

$(Bin)/libnat.a: $(EXOBJS)
	rm -f $(Bin)/libnat.a
	ar cr $(Bin)/libnat.a $(EXOBJS)
	ranlib $(Bin)/libnat.a

$(oDir)/nat_glb_mgmt_entry_point.o:  \
 ../../source/nat_glb_mgmt/src/nat_glb_mgmt_entry_point.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/nat_glb_mgmt_utility.o:  \
 ../../source/nat_glb_mgmt/src/nat_glb_mgmt_utility.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/nat_port_pltfm_util.o: ../../source/nat_port/src/nat_port_pltfm_util.c 
	$(CC) $(C_FLAGS) $(PORT_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/nat_port_pltfm_ipc.o: ../../source/nat_port/src/nat_port_pltfm_ipc.c 
	$(CC) $(C_FLAGS) $(PORT_FLAGS) $(INCLUDE) -c -o $@ $<
		
$(oDir)/nat_port_util.o: ../../source/nat_port/src/nat_port_util.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_decoder.o: ../../source/stun_enc_dec/src/stun_decoder.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_encoder.o: ../../source/stun_enc_dec/src/stun_encoder.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_lifetime_entry_point.o:  \
 ../../source/stun_lifetime_sm/src/stun_lifetime_entry_point.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_shsec_api_handlers.o:  \
 ../../source/stun_shsec_sm/src/stun_shsec_api_handlers.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_shsec_entry_point.o:  \
 ../../source/stun_shsec_sm/src/stun_shsec_entry_point.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_api_handlers.o: ../../source/stun_sm/src/stun_api_handlers.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_entry_point.o: ../../source/stun_sm/src/stun_entry_point.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_utility.o: ../../source/stun_sm/src/stun_utility.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_discovery_entry_point.o:  \
 ../../source/stun_type_disc_sm/src/stun_discovery_entry_point.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_discovery_utility.o:  \
 ../../source/stun_type_disc_sm/src/stun_discovery_utility.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_cmn_util.o:  \
 ../../source/nat_common/src/nat_cmn_util.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

$(oDir)/stun_sha.o: ../../source/stun_enc_dec/src/stun_sha.c
	$(CC) $(C_FLAGS) $(INCLUDE) -c -o $@ $<

depend.$(CPU):
	touch depend.$(CPU)

depend  :
	$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCS) > depend.$(CPU)
	$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(EXOBJS)

include depend.$(CPU)

#DO NOT DELETE THIS LINE -- make depend depends on it.

