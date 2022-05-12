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
# 03-Dec-2008    Anuradha Gupta         None          Initial
# 04-Dec-2008    Anuradha Gupta         None          Start of Makefile
# 15-Jan-2009    Anuradha Gupta         None          changes Makefile
#
# Copyright 2008, Aricent.                       
#                                    
#******************************************************************************
include $(CMN_DEV_DIR)/build/linux/makefile.mk

INCLUDE     +=      -I $(CMN_SRC_DIR)/h

SRCDIR	=	$(CMN_SRC_DIR)/src
OBJDIR	=	$(CMN_SRC_DIR)/obj

SRCFILES	=			$(SRCDIR)/common_util.c

OBJFILES	=			$(OBJDIR)/common_util.o

TARGET=$(OBJDIR)/common.o

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
		$(CMN_DEPEND_BUILD_PATH)  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
