#******************************************************************************
#               Makefile MACROS for all Gateway products
#
#
#
# DATE          NAME            REFERENCE        REASON
# ----------- --------------- ---------------- -------------------------------
# 
# 09/04/2002    Ravi Jindal     SPR-3656         Intial draft for Linux Port.
#                              
# 24/10/2002    Ravi Jindal     SPR-4986         CFGLAG_CMN needs to be += .
#
# COPYRIGHT , Aricent, 2006
#******************************************************************************

CODE_BASE       =               /cm_data

# The CC macro and COPTIONS are linked together logically based on the compiler
# specifed by CC macro. Also if required, the compiler and the relevant options
# can be changed easily by uncommenting the required set and commenting out the
# current set which is in effect

#CC                     =       g++
#CC                      =       gcc

CP			            =	cp
RM                      =       \rm -rf
MV                      =       mv
GW_SLASH                =       /
OBJ_SUFFIX	        =	o
#LIB		        =	ar -ruv
LIB		        =	$(AR) -ruv


CFLAGS_CMN		+=      -DGW_LINUX\
                                -DGW_INTEGER_IS_32




#########################################################
# Some Optional Compile time flags for differnt Features.
#########################################################

#To compile with ANSI-C features.
#
CFLAGS_CMN      	+=       -D_ANSI_C_
 

#To Enable Statis-Validations in GENLIB 
# e.g validation of pool-id, buf-size etc
#
CFLAGS_CMN             +=      -DGL_STATIC_VALIDATION_ENABLED


#To Add the Mememory Manager Debugger.
#
#CFLAGS_CMN     	+=      -DGL_MM_ADD_DEBUGGER 

 
#To Add Audit Mgt for Memory Manager.
#
#CFLAGS_CMN     	+=      -DGL_MM_ADD_AUDIT_MGT

 
#To Add the blocking Feature in Message Queue.
# (Use only if GW_MT_ENABLE is defined) 
#
#CFLAGS_CMN		+=	-DGL_MQM_BLOCKING_ON

 
#To ADD Key Reference Count for each key in Hash-Table.
#
#CFLAGS_CMN             +=      -DGL_HTM_ENABLE_KEY_REF_COUNT


#To Enable Stats for GENLIB
#
#CFLAGS_CMN             +=      -DGL_STATS_ENABLED

 
#To Use Memory OverWrite operations in GENLIB Memory Manager
#
CFLAGS_CMN             +=      -DGL_MM_ADD_MEM_OVERW_OPR


#To Use GENLIB in only INIT-Time MT-Safe Environment.
#
#CFLAGS_CMN             +=      -DGW_MT_ENABLED


#To Use GENLIB in Complete MT-Safe Environment.
#
#CFLAGS_CMN             +=      -DGL_TOTAL_MT_SAFE

######################################################



# opitons are for gcc cnd g++ compiler
COPTIONS                =       -g -c -Wall -Wimplicit -Wmissing-prototypes\
                                -Wshadow -Wcomment -Waggregate-return\
                                -Wmissing-declarations -Wreturn-type\
                                -Wparentheses -Wpointer-arith\
                                -fno-builtin -fno-inline-functions\
                                -fsigned-char -pipe -Wstrict-prototypes\
                                -Wno-format -W -fcond-mismatch\
                                -Wswitch -pedantic

#                -Wredundant-decls
#               The above flag has not been set. With this flag on,
#               warnings start coming up in the system header files.

