#******************************************************************************

#        Makefile for CFG module of ICF.

#

#

#

#        DATE               NAME            REFERENCE        REASON

#        ----               ----            ---------        ------

#        24-Feb-2005        Jalaj Negi                       Initial Version

#

#

#        Copyright (c) 2006, Aricent.

#******************************************************************************

ICF_LIB= $(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE			+=	-I $(GW_COMMON)/common/h \
				-I $(GW_COMMON)/genlib/common/h \
				-I $(GW_COMMON)/genlib/db/h \
				-I $(GW_COMMON)/genlib/os/h \
				-I $(ICF_SRC_DIR)/dbm/h \
				-I $(ICF_SRC_DIR)/regm/h \
				-I $(ICF_SRC_DIR)/cfg/h \
				-I $(ICF_SRC_DIR)/mnl/nm/h \
				-I $(ICF_SRC_DIR)/sm/h \
				-I $(ICF_SRC_DIR)/las/h \
				-I .





SRCDIR			=	$(ICF_SRC_DIR)/sm/src



OBJDIR			=	$(ICF_SRC_DIR)/sm/bin/$(CPU)



SRCFILES		=	$(SRCDIR)/icf_sm_init.c \
				$(SRCDIR)/icf_sm_entry_pt.c \
				$(SRCDIR)/icf_sm_api_handlers.c \
				$(SRCDIR)/icf_sm_util.c



OBJFILES		=	$(OBJDIR)/icf_sm_init.o \
				$(OBJDIR)/icf_sm_entry_pt.o \
				$(OBJDIR)/icf_sm_api_handlers.o \
				$(OBJDIR)/icf_sm_util.o



CFLAGS			+=	-D__ANSI_C__ \
				-DICF_TRACE_INIT_TIME_ENABLE \
				-UICF_REPORT_ENABLE



TARGET=$(ICF_LIB)/libsm.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  -c  $< -o $@





clean:

	$(ICF_RM) $(ICF_LIB)/libsm.a \
    $(ICF_RM) $(OBJFILES)



depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)




include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
