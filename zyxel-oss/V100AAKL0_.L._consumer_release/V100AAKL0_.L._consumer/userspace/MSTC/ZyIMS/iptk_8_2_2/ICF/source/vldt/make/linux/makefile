#------------------------------------------------------------------------------
#
#  File name        : makefile
# 
#  Purpose          : Makefile for VLDT module of ICF.
# 
#  Revision History :
# 
#  Date         Name              Ref#            Comments
#  --------     ------------      ----            ---------------
#  13-JUN-2007  Jagmohan Chauhan   No              Initial
#                Rishi Saxena
#  30-Apr-2009   Anuradha Gupta   SPR 19443      Removing local options    
#  Copyright (c) 2009, Aricent.
# -----------------------------------------------------------------------------

ICF_LIB=$(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE                         +=      -I $(ICF_SRC_DIR)/vldt/h \
                                        -I $(ICF_DEV_DIR)/ext_if \
                                       

SRCDIR  =       $(ICF_SRC_DIR)/vldt/src
OBJDIR  =       $(ICF_SRC_DIR)/vldt/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_vldt_api.c\
                                        $(SRCDIR)/icf_vldt_utils.c\
                                        $(SRCDIR)/icf_vldt_main.c\
					$(SRCDIR)/icf_vldt_mm_api.c\
					$(SRCDIR)/icf_vldt_mm_utils.c\
                                        $(SRCDIR)/icf_vldt_mm_main.c\
					$(SRCDIR)/icf_vldt_port_utils.c
                                        

OBJFILES        =                       $(OBJDIR)/icf_vldt_api.o\
                                        $(OBJDIR)/icf_vldt_utils.o\
                                        $(OBJDIR)/icf_vldt_main.o\
					$(OBJDIR)/icf_vldt_mm_api.o\
					$(OBJDIR)/icf_vldt_mm_utils.o\
                                        $(OBJDIR)/icf_vldt_mm_main.o\
					$(OBJDIR)/icf_vldt_port_utils.o

CFLAGS          +=                      -D__ANSI_C__


TARGET=$(ICF_LIB)/libvldt.a

all     :$(TARGET)

$(TARGET):$(OBJFILES)
	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  $(INCLUDE) $< -o $@



clean   :
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libvldt.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)

include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
