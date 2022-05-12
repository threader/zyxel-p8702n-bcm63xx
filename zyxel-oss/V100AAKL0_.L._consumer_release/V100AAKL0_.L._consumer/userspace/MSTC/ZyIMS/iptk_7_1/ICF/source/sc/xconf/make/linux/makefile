#******************************************************************************
#        Makefile for XCONF module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        Jul,2007         Abhishek Dhammawat                 Initial Version
#        
#
#
#        Copyright (c) 2007, Aricent.
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
					-I $(ICF_SRC_DIR)/sc/xconf/h \
					-I .

SRCDIR	=	$(ICF_SRC_DIR)/sc/xconf/src
OBJDIR	=	$(ICF_SRC_DIR)/sc/xconf/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_xconf_fsm.c \
					$(SRCDIR)/icf_xconf_util.c \
					$(SRCDIR)/icf_xconf_main.c


OBJFILES	=			$(OBJDIR)/icf_xconf_fsm.o \
					$(OBJDIR)/icf_xconf_util.o \
					$(OBJDIR)/icf_xconf_main.o


CFLAGS		+=			-D__ANSI_C__  -DICF_XCONF_SERVICE

TARGET=$(ICF_LIB)/libxconf.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libxconf.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
