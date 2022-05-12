#******************************************************************************
#        Makefile for CFW module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Mar,2005         Amit Sharma                    Initial Version
#        
#
#
#        Copyright (c) 2006, Aricent .
#******************************************************************************
ICF_LIB= $(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE                         +=      -I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(GW_COMMON)/genlib/os/h \
					-I $(ICF_SRC_DIR)/cfg/h \
					-I $(ICF_SRC_DIR)/regm/h \
					-I $(ICF_SRC_DIR)/dbm/h \
					-I $(ICF_SRC_DIR)/sc/cfw/h \
					-I .

SRCDIR	=	$(ICF_SRC_DIR)/sc/cfw/src
OBJDIR	=	$(ICF_SRC_DIR)/sc/cfw/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_cfw_entry_pt.c \
					$(SRCDIR)/icf_cfw_util.c

OBJFILES	=			$(OBJDIR)/icf_cfw_entry_pt.o \
					$(OBJDIR)/icf_cfw_util.o


CFLAGS		+=			-D__ANSI_C__  -DICF_CFW_SERVICE

TARGET=$(ICF_LIB)/libcfw.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libcfw.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
