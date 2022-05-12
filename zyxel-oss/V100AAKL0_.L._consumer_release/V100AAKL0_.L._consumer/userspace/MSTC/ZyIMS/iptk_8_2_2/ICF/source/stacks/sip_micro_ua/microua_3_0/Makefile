include MakeVars
all: uatk stack
all_wt_tls: uatk_wt_tls stack
stack:
	@cd $(STACK_DIR)/source;make all
uatk:
	@cd $(UA_DIR)/source;make UA
uatk_wt_tls:
	@cd $(UA_DIR)/source;make UA_wt_tls
ua:
	@cd $(UA_DIR)/source;make all
clean:
	@cd $(UA_DIR)/source; make clean
	@cd $(STACK_DIR)/source; make clean
allclean:
	@cd $(UA_DIR)/source; make allclean
	@cd $(STACK_DIR)/source; make allclean
