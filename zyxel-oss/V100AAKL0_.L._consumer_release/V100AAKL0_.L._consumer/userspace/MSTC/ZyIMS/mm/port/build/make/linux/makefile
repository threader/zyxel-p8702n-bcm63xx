#***************************************************************************************
#               Top level makefile for Sample Program
#               
#
#                       
#        DATE               NAME            REFERENCE        REASON
#        ----               ----            ---------        ------
#        07/12/2007         Amit Sikka                       Initial Creation
#               
#        Copyright (c) 2007 Aricent Inc . All Rights Reserved
#               
#***************************************************************************************  
include		makefile.mk

MM_EXE	=	$(MM_BASE)/port/build/bin/mm.exe

all:
	$(MAKE) -f make_cfg -C ../ -C ../ -C ../ -C ../ -C cfg/ -C make/ -C linux
	$(MAKE) -f make_core -C ../ -C ../ -C ../ -C ../ -C core/ -C make/ -C linux
	$(MAKE) -f make_port -C ../ -C ../ -C ../ -C ../ -C port/ -C make/ -C linux
	$(MAKE) -f makefile all -C ../ -C ../ -C ../ -C ../ -C common/ -C make/ -C linux
	$(CC) -o $(MM_EXE) $(MM_LIB_PATH) $(MM_AR)

clean_all:
	$(MAKE) -f make_cfg clean_all -C ../ -C ../ -C ../ -C ../ -C cfg/ -C make/ -C linux
	$(MAKE) -f make_core clean_all -C ../ -C ../ -C ../ -C ../ -C core/ -C make/ -C linux
	$(MAKE) -f make_port clean_all -C ../ -C ../ -C ../ -C ../ -C port/ -C make/ -C linux
	$(MAKE) -f makefile clean -C ../ -C ../ -C ../ -C ../ -C common/ -C make/ -C linux
	$(RM) $(MM_EXE)
	$(RM) $(PRJ_BASE)/mm/port/build/lib/linux/*.a
