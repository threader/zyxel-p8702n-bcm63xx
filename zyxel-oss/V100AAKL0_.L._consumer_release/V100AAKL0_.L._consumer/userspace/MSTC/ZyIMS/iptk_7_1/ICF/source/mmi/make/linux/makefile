#******************************************************************************

#        Makefile for MMI module of ICF.

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

ICF_LIB=$(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk



#CC	=	gcc



INCLUDE					+=	-I $(GW_COMMON)/common/h \
						-I $(GW_COMMON)/genlib/common/h \
						-I $(GW_COMMON)/genlib/db/h \
						-I $(GW_COMMON)/genlib/os/h \
						-I $(NAT_LIB_COMMON)/ext_if/h \
						-I $(ICF_SRC_DIR)/ext_if \
						-I $(ICF_SRC_DIR)/common/h \
						-I $(ICF_SRC_DIR)/mmi/h \
						-I $(ICF_SRC_DIR)/mnl/nm/h \
						-I $(ICF_SRC_DIR)/dbm/h \
						-I $(ICF_SRC_DIR)/ifx_al/mm_al/h \
						-I $(ICF_SRC_DIR)/cfg/h \
						-I .



SRCDIR				=	$(ICF_SRC_DIR)/mmi/src

OBJDIR				=	$(ICF_SRC_DIR)/mmi/bin/$(CPU)



SRCFILES			=	$(SRCDIR)/icf_mmi_entry_point.c\
					$(SRCDIR)/icf_mmi_api_handlers.c\
					$(SRCDIR)/icf_mmi_00_idle.c\
					$(SRCDIR)/icf_mmi_01_awt_gcl_resp.c\
					$(SRCDIR)/icf_mmi_02_awt_cms_resp.c\
					$(SRCDIR)/icf_mmi_03_awt_commit_resp.c\
					$(SRCDIR)/icf_mmi_04_awt_mms_resp.c\
					$(SRCDIR)/icf_mmi_05_awt_dms_resp.c\
					$(SRCDIR)/icf_mmi_06_awt_open_chnl_resp.c\
					$(SRCDIR)/icf_mmi_07_awt_merge_resp.c\
					$(SRCDIR)/icf_mmi_10_awt_public_addr_resp.c\
					$(SRCDIR)/icf_mmi_utility.c



OBJFILES			=	$(OBJDIR)/icf_mmi_entry_point.o\
					$(OBJDIR)/icf_mmi_api_handlers.o\
					$(OBJDIR)/icf_mmi_00_idle.o\
					$(OBJDIR)/icf_mmi_01_awt_gcl_resp.o\
					$(OBJDIR)/icf_mmi_02_awt_cms_resp.o\
					$(OBJDIR)/icf_mmi_03_awt_commit_resp.o\
					$(OBJDIR)/icf_mmi_04_awt_mms_resp.o\
					$(OBJDIR)/icf_mmi_05_awt_dms_resp.o\
					$(OBJDIR)/icf_mmi_06_awt_open_chnl_resp.o\
					$(OBJDIR)/icf_mmi_07_awt_merge_resp.o\
					$(OBJDIR)/icf_mmi_10_awt_public_addr_resp.o\
					$(OBJDIR)/icf_mmi_utility.o



CFLAGS				+=	-D__ANSI_C__ -DIIP -DIPV4

                

TARGET=$(ICF_LIB)/libmmi.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  -c  $< -o $@



clean	:

		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libmmi.a



depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)




include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
