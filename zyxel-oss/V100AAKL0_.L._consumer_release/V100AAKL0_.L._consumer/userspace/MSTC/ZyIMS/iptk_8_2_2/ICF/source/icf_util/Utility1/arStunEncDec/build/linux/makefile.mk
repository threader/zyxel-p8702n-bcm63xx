#******************************************************************************
#                                    
# FILE NAME    : makefile                
#                                    
# DESCRIPTION  : Contains the linux makefile. 
#                                    
# Revision History :                            
#
#  DATE            NAME                 REFERENCE       REASON        
# ------         ------------           ---------     ----------
# 05-Dec-2009    Anuradha Gupta         None          Initial
# 07-Dec-2009    Anuradha Gupta         None          Code Start 
# 30-Apr-2009    Anuradha Gupta         SPR 19443     Integration of rules with IPTK
#
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************/
include $(ICF_DEV_DIR)/build/linux/makefile.mk
#CPU = intel
# The make to be done in Bourne shell
#
#SHELL           =       /bin/sh

export ARSTUNENCDEC_DEV_DIR=$(ICF_DEV_DIR)/source/icf_util/Utility1/arStunEncDec
export CMN_DEV_DIR=$(ARSTUNENCDEC_DEV_DIR)

ARSTUNENCDEC_SRC_DIR = $(ARSTUNENCDEC_DEV_DIR)/source
CMN_SRC_DIR = $(ARSTUNENCDEC_DEV_DIR)/../Common
ARSTUNENCDEC_DEPEND_BUILD_PATH =  $(ICF_DEV_DIR)/build/linux/build.sh
CMN_DEPEND_BUILD_PATH =  $(ARSTUNENCDEC_DEPEND_BUILD_PATH)

#CC=gcc

FTR_LD = $(ICF_LD) $(ICF_LD_FLAGS) 
FTR_RM = $(ICF_RM)
FTR_CP = $(ICF_CP)
FTR_CD = $(ICF_CD)
FTR_AR = $(ICF_AR) $(ICF_AR_FLAGS)
FTR_MV = $(ICF_MV)
FTR_RANLIB = ranlib

INCLUDE		= -I$(CMN_SRC_DIR)/h \
				-I$(ARSTUNENCDEC_DEV_DIR)/ext_if 

#Flag: ARSTUNENCDEC_TRACE_ENABLE:This will enable the tracing functionality

CFLAGS		+= -DUTIL_PORT_LINUX -UUTIL_TRACE_ENABLE

FTR_LIBS	= -lsocket -lnsl


