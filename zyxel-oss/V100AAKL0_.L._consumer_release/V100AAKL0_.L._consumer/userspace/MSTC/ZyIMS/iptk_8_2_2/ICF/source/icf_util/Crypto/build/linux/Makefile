#####################################################################
#### (C) 2008 Aricent Technologies(H)                            ####
##|                                                               |##
##|    FILE NAME               ::  Makefile                       |##
##|                                                               |##
##|    DESCRIPTION             ::  Makefile for Aricent Crypto    |##
##|                                                               |##
##|###############################################################|##

# Following path need to be exported for compilation:
# CRYPTO_BASE_DIR: Path for Crypto Folder
export CRYPTO_BASE_DIR=$(ICF_DEV_DIR)/source/icf_util/Crypto
include $(CRYPTO_BASE_DIR)/build/linux/make.rule

FINAL_OBJ = $(OBJ_PATH)/crypto.o

obj: MAKEALL $(FINAL_OBJ)

lib: MAKEALL $(LIBTARGET)

all: MAKEALL obj lib
$(FINAL_OBJ): $(LIB_DEPENDENCIES) 
	$(LD) $(LD_FLAGS) -o $(FINAL_OBJ) $(LIB_DEPENDENCIES) 

$(LIBTARGET): $(LIB_DEPENDENCIES) 
	$(AR) $(AR_FLAGS)  $(LIBTARGET) $(LIB_DEPENDENCIES) 

MAKEALL:
ifeq (${MD5}, YES)
	@cd $(CRYPTO_ALGO_DIR)/md5/build/linux;touch depend.$(CPU); make all  
endif
ifeq (${DES}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/des
endif
ifeq (${SHA1}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/sha1
endif
ifeq (${AES}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/aes
endif
ifeq (${HMAC}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/hmac
endif


clean:
	$(RM) $(LIBTARGET) $(FINAL_OBJ) 
ifeq (${MD5}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/md5/build/linux clean
endif
ifeq (${DES}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/des clean
endif
ifeq (${SHA1}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/sha1 clean
endif
ifeq (${AES}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/aes clean
endif
ifeq (${HMAC}, YES)
	make -w -C $(CRYPTO_ALGO_DIR)/hmac clean
endif

depend:
ifeq (${MD5}, YES)
	@cd $(CRYPTO_ALGO_DIR)/md5/build/linux;$(RM) depend.$(CPU); touch depend.$(CPU); make depend  
endif
			
