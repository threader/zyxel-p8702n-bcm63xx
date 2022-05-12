include MakeVars
all: uatk stack
stack:
	@cd $(STACK_DIR)/source;make all
uatk:
	@cd $(UA_DIR)/source;make UA
ua:
	@cd $(UA_DIR)/source;make all
clean:
	@cd $(UA_DIR)/source; make clean
	@cd $(STACK_DIR)/source; make clean
allclean:
	@cd $(UA_DIR)/source; make allclean
	@cd $(STACK_DIR)/source; make allclean
