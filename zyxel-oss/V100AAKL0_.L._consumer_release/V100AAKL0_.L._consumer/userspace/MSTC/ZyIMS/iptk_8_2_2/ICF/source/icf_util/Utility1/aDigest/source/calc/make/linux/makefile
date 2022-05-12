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
# 20-Jan-2009    Anuradha Gupta         None          Open Source Guideline changes
#           
# Copyright 2009, Aricent.                       
#                                    
#******************************************************************************
include $(ADIGEST_DEV_DIR)/build/linux/makefile.mk
CALC_DEV_DIR=$(ADIGEST_DEV_DIR)/source/calc

INCLUDE     +=      -I $(CALC_DEV_DIR)/h

SRCDIR	=	$(CALC_DEV_DIR)/src
OBJDIR	=	$(CALC_DEV_DIR)/obj

SRCFILES	=			$(SRCDIR)/aDigest_util.c \
                        $(SRCDIR)/aDigest_calc.c

OBJFILES	=			$(OBJDIR)/aDigest_util.o \
                        $(OBJDIR)/aDigest_calc.o

TARGET=$(OBJDIR)/aDigestCalc.o

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
		$(ADIGEST_DEPEND_BUILD_PATH) depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
