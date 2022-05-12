ICF_STACK_DIR=$(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0
include $(ICF_STACK_DIR)/ua_3_0_src/source/MakeVars

UA:
			@cd UAlib;make

UA_wt_tls:
			@cd UAlib;make all_wt_tls

all:
		@cd UAlib;make
		@cd hssUA;make
		@cd ServiceFW; make
		@cd CallTransfer; make

libuaport:
		@cd UAlib;make libuaport

        
libuaport_wt_tls:
		@cd UAlib;make libuaport_wt_tls

clean:
		$(ICF_RM) obj/*.o
	$(ICF_RM) simua/obj/*.o
	@cd UAlib;make clean

allclean: clean
	$(ICF_RM) lib/*.a lib/*.so
	$(ICF_RM) simua/bin/simUA
	$(ICF_RM) super/bin/SUPEr
	$(ICF_RM) super/bin/timerproc
	$(ICF_RM) super/bin/sipcallgen
	$(ICF_RM) hssua/bin/hssUA

