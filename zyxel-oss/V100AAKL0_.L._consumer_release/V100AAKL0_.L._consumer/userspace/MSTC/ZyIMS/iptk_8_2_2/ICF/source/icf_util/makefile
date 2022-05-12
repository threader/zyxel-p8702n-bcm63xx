#******************************************************************************
#                                    
# FILE NAME    : makefile                
#                                    
# DESCRIPTION  : Contains the linux and vxworks rules for
#                makefile for Common Utility to Build the
#                identified set of Libraries as per the rules specified in 
#                make.rule. 
#                                    
# Revision History :                            
#
#  DATE            NAME                 REFERENCE       REASON        
# ------         ------------           ---------     ----------
# 20-Jan-2009    Anuradha Gupta         None          Initial
# 09-Mar-2009    Alok Tiwari            IPTK 8.1      rule added to compile
#                                                     arMD5
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
export CMN_UTIL_BASE_DIR=$(ICF_DEV_DIR)/source/icf_util
include $(CMN_UTIL_BASE_DIR)/make.rule
include $(CMN_UTIL_BASE_DIR)/make.env

# the default rule
all:MAKE_LINUX

#the rule to create linux libraries
lx:MAKE_LINUX

#the rule to create vxworks libraries
vx:MAKE_VXWORKS

MAKE_LINUX:
ifeq (${ABASE64},1)
	$(MAKE) -w -C $(ABASE64_DEV_DIR)/build/linux
endif
ifeq (${ADIGEST},1)
	$(MAKE) -w -C $(ADIGEST_DEV_DIR)/build/linux
endif
ifeq (${ARSTUNENCDEC},1)
	$(MAKE) -w -C $(ARSTUNENCDEC_DEV_DIR)/build/linux
endif
ifeq (${ARMD5},1)
	$(MAKE) -w -C $(CRYPTO_BASE_DIR)/build/linux
endif

MAKE_VXWORKS:
ifeq (${ABASE64},1)
	$(MAKE) -w -C $(ABASE64_DEV_DIR)/build/vxworks
endif
ifeq (${ADIGEST},1)
	$(MAKE) -w -C $(ADIGEST_DEV_DIR)/build/vxworks
endif
ifeq (${ARSTUNENCDEC},1)
	$(MAKE) -w -C $(ARSTUNENCDEC_DEV_DIR)/build/vxworks
endif
ifeq (${ARMD5},1)
	$(MAKE) -w -C $(CRYPTO_BASE_DIR)/build/vxworks
endif

clean:CLEAN_LINUX
clean_lx:CLEAN_LINUX
clean_vx:CLEAN_VXWORKS

CLEAN_LINUX:
ifeq (${ABASE64},1)
	$(MAKE) -w -C $(ABASE64_DEV_DIR)/build/linux clean
endif
ifeq (${ADIGEST},1)
	$(MAKE) -w -C $(ADIGEST_DEV_DIR)/build/linux clean
endif
ifeq (${ARSTUNENCDEC},1)
	$(MAKE) -w -C $(ARSTUNENCDEC_DEV_DIR)/build/linux clean
endif
ifeq (${ARMD5},1)
	$(MAKE) -w -C $(CRYPTO_BASE_DIR)/build/linux clean
endif

CLEAN_VXWORKS:
ifeq (${ABASE64},1)
	$(MAKE) -w -C $(ABASE64_DEV_DIR)/build/vxworks clean
endif
ifeq (${ADIGEST},1)
	$(MAKE) -w -C $(ADIGEST_DEV_DIR)/build/vxworks clean
endif
ifeq (${ARSTUNENCDEC},1)
	$(MAKE) -w -C $(ARSTUNENCDEC_DEV_DIR)/build/vxworks clean
endif
ifeq (${ARMD5},1)
	$(MAKE) -w -C $(CRYPTO_BASE_DIR)/build/vxworks clean
endif


