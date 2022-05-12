#******************************************************************************
#        Makefile for SCM module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        23-Mar-2005         Amit Sharma                    Initial Version
#        
#
#
#        Copyright (c) 2006, Aricent.
#******************************************************************************
ICF_LIB= $(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE				+=	-I $(GW_COMMON)/common/h \
					-I $(GW_COMMON)/genlib/common/h \
					-I $(GW_COMMON)/genlib/db/h \
					-I $(GW_COMMON)/genlib/os/h \
					-I $(ICF_SRC_DIR)/cfg/h \
					-I $(ICF_SRC_DIR)/regm/h \
					-I $(ICF_SRC_DIR)/dbm/h \
					-I $(ICF_SRC_DIR)/sc/common/h \
					-I $(ICF_SRC_DIR)/sc/scm/h \
					-I $(ICF_SRC_DIR)/sc/chr/h \
					-I .

SRCDIR	=	$(ICF_SRC_DIR)/sc/scm/src
OBJDIR	=	$(ICF_SRC_DIR)/sc/scm/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_scm_main.c \
					$(SRCDIR)/icf_scm_util.c \
					$(SRCDIR)/icf_scm_trace.c 


OBJFILES	=			$(OBJDIR)/icf_scm_main.o\
					$(OBJDIR)/icf_scm_util.o\
					$(OBJDIR)/icf_scm_trace.o 


CFLAGS		+=			-D__ANSI_C__

TARGET=$(ICF_LIB)/libscm.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libscm.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
