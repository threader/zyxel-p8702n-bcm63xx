ICF_STACK_DIR=$(ICF_DEV_DIR)/source/stacks/sip_micro_ua/microua_3_0
include $(ICF_STACK_DIR)/ua_3_0_src/source/MakeVars

UA:
			@cd UAlib;make

all:
			@cd UAlib;make
	@cd ServiceFW; make
		@cd CallTransfer; make

clean:
		rm -f obj/*.o
	rm -f simua/obj/*.o

allclean: clean
	rm -f lib/*.a lib/*.so
	rm -f simua/bin/simUA
	rm -f super/bin/SUPEr
	rm -f super/bin/timerproc
	rm -f super/bin/sipcallgen

