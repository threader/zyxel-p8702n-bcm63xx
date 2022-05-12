#******************************************************************************
#        Makefile for Wrapper SSL module of ICF.
#
#        DATE              NAME            REFERENCE        REASON
#        ----              ----            ---------        ------
#        06-July-2009      Rajiv Kumar     OpenSSL removal  Initial Version
#        25-Aug-2009       Rajiv Kumar     SPR: 19982       OPENSSL include Path
#                                                           removed for Linux
#        
#        Copyright (c) 2009, Aricent.
#******************************************************************************
include $(ICF_DEV_DIR)/build/linux/makefile.mk

SRC_DIR  = ../../src
OBJ_DIR  = ../../obj/$(CPU)

SRC_FILE  = $(SRC_DIR)/openssl_wrapper.c	
OBJ_FILE  = $(OBJ_DIR)/openssl_wrapper.o

CFLAGS		+=			-D__ANSI_C__ -D_REENTRANT
    
WRAPPER_INCLUDE_PATH  += -I../../h \
                         -I .
                         
WRAPPER_SSL_LIB= ../../lib/linux

TARGET=$(WRAPPER_SSL_LIB)/libwrapperssl.a


all     :$(TARGET)
$(TARGET):$(OBJ_FILE)
	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJ_FILE)
$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) ${WRAPPER_INCLUDE_PATH} $< -o $@

clean	:
		$(ICF_RM) $(OBJ_FILE)
		$(ICF_RM) $(WRAPPER_SSL_LIB)/libwrapperssl.a

depend.$(CPU):
		touch depend.$(CPU)

depend	:
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRC_FILE) > depend.$(CPU) 
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJ_DIR)

include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.               
