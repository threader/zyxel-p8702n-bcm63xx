#******************************************************************************
#        Makefile for sic module of ICF.
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

INCLUDE					+=	 -I $(ICF_SRC_DIR)/sic/h\
						-I $(ICF_SRC_DIR)/port/h\
						-I $(ICF_SRC_DIR)/dbm/h\
						-I $(ICF_SRC_DIR)/regm/h\
						-I $(ICF_SRC_DIR)/sm/h\
						-I $(STACK)/ua_2_1_src/uatk_headers\
						-I $(STACK)/sip_stack_2_0_src/stack_headers

SRCDIR				=	$(ICF_SRC_DIR)/sic/src
OBJDIR				=	$(ICF_SRC_DIR)/sic/bin/$(CPU)
SRCFILES			=	$(SRCDIR)/icf_sic_init.c\
					$(SRCDIR)/icf_sic_entry_point.c\
					$(SRCDIR)/icf_sic_fsm.c\
					$(SRCDIR)/icf_sic_util.c

OBJFILES			=	$(OBJDIR)/icf_sic_init.o\
					$(OBJDIR)/icf_sic_entry_point.o\
					$(OBJDIR)/icf_sic_fsm.o\
					$(OBJDIR)/icf_sic_util.o

CFLAGS				+=	-D__ANSI_C__



TARGET=$(ICF_LIB)/libsic.a

all     :$(TARGET)
$(TARGET):$(OBJFILES)
	$(ICF_AR) -r $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS)  -c  $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libsic.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)

# DO NOT DELETE THIS LINE -- make depend depends on it.
