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

DECODER_DEV_DIR=$(ARSTUNENCDEC_DEV_DIR)/source/decoder

INCLUDE     +=      -I $(DECODER_DEV_DIR)/h

SRCDIR	=	$(DECODER_DEV_DIR)/src
OBJDIR	=	$(DECODER_DEV_DIR)/obj

SRCFILES	=			$(SRCDIR)/arStun_dec_util.c \
                        $(SRCDIR)/arStun_decoder.c

OBJFILES	=			$(OBJDIR)/arStun_dec_util.o \
                        $(OBJDIR)/arStun_decoder.o


CFLAGS		+=	-D_ANSI_C_ 

TARGET=$(OBJDIR)/arStunEncDec_dec.o

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
#
