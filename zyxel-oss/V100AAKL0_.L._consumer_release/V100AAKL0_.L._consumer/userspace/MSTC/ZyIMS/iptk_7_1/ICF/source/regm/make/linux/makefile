#******************************************************************************

#        Makefile for REGM module of ICF.

#

#

#

#        DATE               NAME            REFERENCE      REASON

#        ----               ----            ---------      ------

#        23-Feb-2005        Jalaj Negi                     Initial Version

#

#

#        Copyright (c) 2006, Aricent.

#******************************************************************************

ICF_LIB= $(ICF_DEV_DIR)/lib/linux

include $(ICF_DEV_DIR)/build/linux/makefile.mk



INCLUDE                 +=  -I $(ICF_SRC_DIR)/regm/h \
                            -I $(ICF_SRC_DIR)/dbm/h \
                            -I $(ICF_SRC_DIR)/mnl/nm/h \
                            -I $(ICF_SRC_DIR)/cfg/h  \
                            -I $(ICF_SRC_DIR)/sm/h



SRCDIR				=	$(ICF_SRC_DIR)/regm/src

OBJDIR				=	$(ICF_SRC_DIR)/regm/bin/$(CPU)



SRCFILES			=	$(SRCDIR)/icf_regm_basic.c\
					$(SRCDIR)/icf_regm_init.c \
					$(SRCDIR)/icf_regm_timer.c \
					$(SRCDIR)/icf_regm_fsm_handler.c \
					$(SRCDIR)/icf_regm_nw_response.c \
					$(SRCDIR)/icf_regm_ipsec.c \
					$(SRCDIR)/icf_regm_util.c



OBJFILES			=	$(OBJDIR)/icf_regm_basic.o\
					$(OBJDIR)/icf_regm_init.o \
					$(OBJDIR)/icf_regm_timer.o \
					$(OBJDIR)/icf_regm_fsm_handler.o \
					$(OBJDIR)/icf_regm_nw_response.o \
					$(OBJDIR)/icf_regm_ipsec.o \
					$(OBJDIR)/icf_regm_util.o



CFLAGS				+=	-D__ANSI_C__



TARGET=$(ICF_LIB)/libregm.a



all     :$(TARGET)

$(TARGET):$(OBJFILES)

	$(ICF_AR) -r $(TARGET) $(OBJFILES)

$(OBJDIR)/%.o:$(SRCDIR)/%.c

	$(CC) $(CFLAGS)  -c  $< -o $@



clean	:

		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libregm.a



depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)




include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it
