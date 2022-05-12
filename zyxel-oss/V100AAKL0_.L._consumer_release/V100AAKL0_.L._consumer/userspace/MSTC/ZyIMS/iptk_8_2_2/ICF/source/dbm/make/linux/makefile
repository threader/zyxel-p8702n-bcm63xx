#******************************************************************************
#        Makefile for DBM module of ICF.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial Version
#        30-Apr-2009        Anuradha Gupta   SPR 19443       Removing local options
#
#        Copyright (c) 2009, Aricent.
#******************************************************************************
ICF_LIB= $(ICF_DEV_DIR)/lib/linux
include $(ICF_DEV_DIR)/build/linux/makefile.mk

INCLUDE					+=	 -I $(ICF_SRC_DIR)/dbm/h\
						-I $(ICF_SRC_DIR)/regm/h\
						-I $(STACK)/ua_2_1_src/uatk_headers\
						-I $(STACK)/sip_stack_2_0_src/stack_headers


SRCDIR				=	$(ICF_SRC_DIR)/dbm/src
OBJDIR				=	$(ICF_SRC_DIR)/dbm/bin/$(CPU)

SRCFILES			=	$(SRCDIR)/icf_dbm_la_util.c\
					$(SRCDIR)/icf_dbm_alloc.c\
					$(SRCDIR)/icf_dbm_get.c\
					$(SRCDIR)/icf_dbm_attach.c\
					$(SRCDIR)/icf_dbm_dealloc.c\
					$(SRCDIR)/icf_dbm_init.c\
					$(SRCDIR)/icf_dbm_deinit.c
                                                

OBJFILES			=	$(OBJDIR)/icf_dbm_la_util.o\
					$(OBJDIR)/icf_dbm_alloc.o\
					$(OBJDIR)/icf_dbm_get.o\
					$(OBJDIR)/icf_dbm_attach.o\
					$(OBJDIR)/icf_dbm_dealloc.o\
					$(OBJDIR)/icf_dbm_init.o\
					$(OBJDIR)/icf_dbm_deinit.o

CFLAGS				+=	-D__ANSI_C__

TARGET=$(ICF_LIB)/libdbm.a

all	:$(TARGET)

$(TARGET):$(OBJFILES)
	$(ICF_AR) $(ICF_AR_FLAGS) $(TARGET) $(OBJFILES)
$(OBJDIR)/%.o:$(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDE) $< -o $@

clean	:
		$(ICF_RM) $(OBJFILES)
		$(ICF_RM) $(ICF_LIB)/libdbm.a

depend.$(CPU):
		touch depend.$(CPU)

depend  :
		$(CC) -M $(CFLAGS) $(INCLUDE) $(SRCFILES) > depend.$(CPU)
		$(ICF_DEV_DIR)/build/linux/build.sh  depend.$(CPU) $(OBJDIR)


include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.
