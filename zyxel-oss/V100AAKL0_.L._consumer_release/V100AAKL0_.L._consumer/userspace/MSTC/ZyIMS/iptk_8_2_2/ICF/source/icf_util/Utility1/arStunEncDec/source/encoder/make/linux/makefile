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
# 05-Jan-2009    Anuradha Gupta         None          Initial
# 07-Jan-2009    Anuradha Gupta         None          Code Start 
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************
include $(ARSTUNENCDEC_DEV_DIR)/build/linux/makefile.mk

ENCODER_DEV_DIR=$(ARSTUNENCDEC_DEV_DIR)/source/encoder

INCLUDE     +=      -I $(ENCODER_DEV_DIR)/h

SRCDIR	=	$(ENCODER_DEV_DIR)/src
OBJDIR	=	$(ENCODER_DEV_DIR)/obj

SRCFILES	=			$(SRCDIR)/arStun_enc_util.c \
                        $(SRCDIR)/arStun_encoder.c

OBJFILES	=			$(OBJDIR)/arStun_enc_util.o \
                        $(OBJDIR)/arStun_encoder.o


CFLAGS		+=	-D_ANSI_C_ 

TARGET=$(OBJDIR)/arStunEncDec_enc.o

all     :$(TARGET)

$(TARGET):$(OBJFILES)
		 $(FTR_LD) -o $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@


clean	:
		$(FTR_RM) $(OBJDIR)/*.o

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ARSTUNENCDEC_DEPEND_BUILD_PATH)  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
#
