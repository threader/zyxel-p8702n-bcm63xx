#******************************************************************************
#        Makefile for ADM module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        -------           -----------      ---------        ------
#        24-Feb-2005       Jalaj Negi                        Initial Version
#
#
#
#        Copyright (c) 2006, Aricent.
#******************************************************************************
ICF_LIB=$(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE                         +=      -I $(GW_COMMON)/common/h \
                                        -I $(GW_COMMON)/genlib/common/h \
                                        -I $(GW_COMMON)/genlib/db/h \
                                        -I $(GW_COMMON)/genlib/os/h \
                                        -I $(NAT_LIB_COMMON)/ext_if/h\
                                        -I $(ICF_SRC_DIR)/ifx_al/es/h \
                                        -I $(ICF_SRC_DIR)/cfg/h \
                                        -I $(ICF_SRC_DIR)/regm/h \
                                        -I $(ICF_SRC_DIR)/dbm/h \
                                        -I $(ICF_SRC_DIR)/port/h \
                                        -I $(ICF_SRC_DIR)/sc/scm/h \
                                        -I $(ICF_SRC_DIR)/adm/h \
                                        -I .

SRCDIR  =       $(ICF_SRC_DIR)/adm/src
OBJDIR  =       $(ICF_SRC_DIR)/adm/bin/$(CPU)

SRCFILES	=			$(SRCDIR)/icf_adm_main.c


OBJFILES        =                       $(OBJDIR)/icf_adm_main.o \


CFLAGS          +=                      -D__ANSI_C__

TARGET=$(ICF_LIB)/libadm.a

all     :$(TARGET)

$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@



clean   :
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libadm.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)

include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
