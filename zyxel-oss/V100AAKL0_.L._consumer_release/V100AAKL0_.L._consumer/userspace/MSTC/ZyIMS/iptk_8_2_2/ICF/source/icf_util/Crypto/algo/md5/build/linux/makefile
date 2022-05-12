#******************************************************************************
#                                    
# FILE NAME    : makefile                
#                                    
# DESCRIPTION  : Contains the linux makefile for MD5 utility. 
#                                    
# Revision History :                            
#
#  DATE            NAME                 REFERENCE       REASON        
# ------         ------------           ---------     ----------
# 22-Dec-2008    Anuradha Gupta         None          Initial
#           
# Copyright 2008, Aricent.                       
#                                    
#******************************************************************************/
include $(CRYPTO_BASE_DIR)/build/linux/make.rule
include $(CRYPTO_BASE_DIR)/build/linux/make.h

ARMD5_DEV_DIR= $(CRYPTO_ALGO_DIR)/md5
ARMD5_SRC_DIR= $(ARMD5_DEV_DIR)/src
ARMD5_OBJ_DIR= $(ARMD5_DEV_DIR)/obj

ARMD5_INCLUDE     +=      -I $(CRYPTO_BASE_DIR)/common \
                    -I $(ARMD5_DEV_DIR)/h

ARMD5_SRCFILES	=			$(ARMD5_SRC_DIR)/arMD5_hash.c 
ARMD5_OBJFILES	=			$(ARMD5_OBJ_DIR)/arMD5_hash.o

TARGET=$(ARMD5_OBJ_DIR)/md5.o

all:$(TARGET)

$(TARGET):$(ARMD5_OBJFILES)
		 $(LD) $(LD_FLAGS) -o $(TARGET) $(ARMD5_OBJFILES)

#ZyXEL porting
ifeq ($(strip $(BUILD_ZYXEL_406_VOICE)),y)
$(ARMD5_OBJ_DIR)/%.o:$(ARMD5_SRC_DIR)/%.c
	$(CC) -fPIC $(CFLAGS) $(ARMD5_INCLUDE) -c  $< -o $@
else
$(ARMD5_OBJ_DIR)/%.o:$(ARMD5_SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(ARMD5_INCLUDE) -c  $< -o $@
endif
#ZyXEL porting end

clean:
		 $(RM) -f $(ARMD5_OBJ_DIR)/*.o

depend.$(CPU):
		 touch depend.$(CPU)

depend:
	$(CC) -M $(CFLAGS) $(ARMD5_INCLUDE) $(ARMD5_SRCFILES) > depend.$(CPU)
	$(CRYPTO_DEPEND_BUILD_PATH)  depend.$(CPU) $(ARMD5_OBJ_DIR)

include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
#
#
#
