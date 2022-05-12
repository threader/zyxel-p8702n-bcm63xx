ICF_STACK_DIR=$(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0
include $(ICF_STACK_DIR)/MakeVars
include $(ICF_STACK_DIR)/ua_3_0_src/source/MakeVars
INCLUDES	+=	-I $(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0/ua_3_0_src/source/ares_linux/h
UASRC =	sdf_reqresp.c\
		sdf_init.c\
                sdf_txn.c\
		sdf.c\
		sdf_free.c\
		md5c.c\
		sdf_hash.c\
		sdf_list.c\
		sdf_debug.c\
		sdf_statistics.c\
		sdf_accessor.c\
		sdf_trace.c\
		sdf_configs.c\
		sdf_sdpinternal.c\
	 	sdf_authorization.c\
		sdf_sdp.c\
		sdf_internal.c\
		sdf_network.c\
		sdf_callapis.c\
		sdf_mempool.c\
		sdf_sdpinit.c\
		sdf_sdpfree.c\
		sdf_sdpaccessor.c\
		sdf_sessTimer.c\
		basic.c\
		digcalc.c

UALOOKUPSRC =	sdf_lookup.c

UAPORTSRC = sdf_networkport.c\
			sdf_lookupport.c \
			sdf_portlayer.c
	

UA_SRC =        src/sdf_reqresp.c\
		src/sdf_init.c\
                src/sdf_txn.c\
		src/sdf.c\
		src/sdf_free.c\
		src/md5c.c\
		src/sdf_hash.c\
		src/sdf_list.c\
		src/sdf_debug.c\
		src/sdf_statistics.c\
		src/sdf_accessor.c\
		src/sdf_trace.c\
		src/sdf_configs.c\
		src/sdf_sdpinternal.c\
		src/sdf_authorization.c\
		src/sdf_sdp.c\
		src/sdf_internal.c\
		src/sdf_network.c\
		src/sdf_callapis.c\
		src/sdf_mempool.c\
		src/sdf_sdpinit.c\
		src/sdf_sdpfree.c\
		src/sdf_sdpaccessor.c\
		src/sdf_sessTimer.c\
		src/basic.c\
		src/digcalc.c
UALOOKUP_SRC =   src/sdf_lookup.c

UAPORT_SRC = src/sdf_networkport.c\
                        src/sdf_lookupport.c \
                        src/sdf_portlayer.c




OBJECTS=$(UASRC:%.c=../obj/%.o) 
PORT_OBJECTS=$(UAPORTSRC:%.c=../obj/%.o) 
LOOKUPOBJECTS=$(UALOOKUPSRC:%c=../obj/%o)

OBJDIR  =       ../obj

all: libua libuaport

libua: ../lib/libua.a

libuaport: ../lib/libuaport.a

../lib/libua.a: $(OBJECTS) $(LOOKUPOBJECTS)
	$(STACK_AR) ../lib/libua.a $(OBJECTS)
	$(STACK_AR) ../lib/libua.a $(LOOKUPOBJECTS)

../lib/libuaport.a: $(PORT_OBJECTS)
	$(STACK_AR) ../lib/libuaport.a $(PORT_OBJECTS)



../obj/%.o: src/%.c
	${CC} ${CFLAGS} ${INCLUDES} ${LIB} -c $< -o $@

depend.$(CPU):
		touch depend.$(CPU)

depend  :	
		touch depend1.intel depend2.intel	
		$(CC) -M $(CFLAGS) ${INCLUDES} $(wildcard $(UA_SRC)) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)	
		
		$(CC) -M $(CFLAGS) ${INCLUDES} $(wildcard $(UALOOKUP_SRC)) > depend2.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build2.sh  depend2.$(CPU) $(OBJDIR)

		$(CC) -M $(CFLAGS) ${INCLUDES} $(wildcard $(UAPORT_SRC)) > depend1.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend1.$(CPU) $(OBJDIR)

		cat depend1.intel >> depend.intel
		cat depend2.intel >> depend.intel
		rm depend1.intel depend2.intel
clean:
	$(ICF_RM) ../obj/*.o 
	$(ICF_RM) ../lib/libua.a
	$(ICF_RM) ../lib/libuaport.a

include depend.$(CPU)
#do not delete this line -Make depend depends on it.
