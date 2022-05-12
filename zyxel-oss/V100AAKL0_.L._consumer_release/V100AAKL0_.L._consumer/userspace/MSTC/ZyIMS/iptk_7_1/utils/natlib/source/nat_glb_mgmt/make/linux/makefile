#******************************************************************************
#        Makefile for MMI module of IPPSTK.
#
#
#
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        24-Feb-2005        Jalaj Negi                       Initial Version
#
#
#        Copyright (c) 2006, Aricent 
#******************************************************************************
IPPSTK_LIB=$(IPPSTK_DEV_DIR)/lib/linux
include $(IPPSTK_DEV_DIR)/build/linux/makefile.mk

#CC	=	gcc

INCLUDE					+=	-I $(GW_COMMON)/common/h \
						-I $(GW_COMMON)/genlib/common/h \
						-I $(GW_COMMON)/genlib/db/h \
						-I $(GW_COMMON)/genlib/os/h \
						-I $(IPPSTK_DEV_DIR)/extif \
						-I $(IPPSTK_DEV_DIR)/common/h \
						-I $(IPPSTK_DEV_DIR)/mmi/h \
						-I $(IPPSTK_DEV_DIR)/dbm/h \
						-I $(IPPSTK_DEV_DIR)/ifx_al/mm_al/h \
						-I .

SRCDIR				=	$(IPPSTK_DEV_DIR)/mmi/src
OBJDIR				=	$(IPPSTK_DEV_DIR)/mmi/bin/$(CPU)

SRCFILES			=	$(SRCDIR)/ippstk_mmi_entry_point.c\
					$(SRCDIR)/ippstk_mmi_api_handlers.c\
					$(SRCDIR)/ippstk_mmi_utility.c

OBJFILES			=	$(OBJDIR)/ippstk_mmi_entry_point.o\
					$(OBJDIR)/ippstk_mmi_api_handlers.o\
					$(OBJDIR)/ippstk_mmi_utility.o

CFLAGS				+=	-D__ANSI_C__ -DIIP -DIPV4
                

all	:
		$(CC) $(CFLAGS) $(SRCFILES)
		mv *.o $(OBJDIR)
		ar -r libmmi.a $(OBJFILES)
		mv libmmi.a $(IPPSTK_LIB)
clean	:
		\rm -f $(OBJFILES)
		\rm -f $(IPPSTK_LIB)/libmmi.a

depend	:
	\rm -f $(SRCDIR)/makefile.tmp*
	cp makefile $(SRCDIR)/makefile.tmp
	cd $(SRCDIR); makedepend $(INCLUDE) $(DEFINES) -f makefile.tmp -w 80 -s"# IP-MAKE --> Do not edit below this line" $(SRCFILES)
	$(IPPSTK_DEV_DIR)/bin/put_obj.pl < $(SRCDIR)/makefile.tmp > depend.$(CPU)

include depend.$(CPU)
# DO NOT DELETE THIS LINE -- make depend depends on it.

