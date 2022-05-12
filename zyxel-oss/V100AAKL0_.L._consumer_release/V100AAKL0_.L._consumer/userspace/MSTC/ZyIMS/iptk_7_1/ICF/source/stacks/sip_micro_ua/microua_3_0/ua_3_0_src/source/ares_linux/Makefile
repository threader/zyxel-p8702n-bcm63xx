# Generated automatically from Makefile.in by configure.
# $Id: Makefile,v 1.3 2007/01/20 11:17:03 gur10841 Exp $

include ../../../MakeVars
include ../MakeVars

#SHELL=/bin/sh
#INSTALL=/opt/sfw/bin/ginstall -c
RANLIB=ranlib

SRC =	ares__close_sockets.c ares__get_hostent.c ares__read_line.c \
	ares_destroy.c ares_expand_name.c ares_fds.c ares_free_errmem.c \
	ares_free_hostent.c ares_free_string.c ares_gethostbyaddr.c \
	ares_gethostbyname.c ares_init.c ares_mkquery.c ares_parse_a_reply.c \
	ares_parse_ptr_reply.c ares_process.c  ares_query.c ares_search.c \
	ares_send.c ares_strerror.c ares_timeout.c 

DEFS= -DETC_INET=1 
LDFLAGS=
#ARESLIBS= ${LIBS} -lsocket  -lnsl 
ALL_CFLAGS= ${CFLAGS} ${DEFS}
OBJS= ${SRC:%.c=../obj/%.o}
ARESINCLUDES=$(INCLUDES:-I%=-I../source/%) \
		-Ih/
all: libares.a adig ahost

libares.a: ${OBJS}
	$(STACK_AR) ../lib/$@ ${OBJS}
#	${RANLIB} ../lib/$@

adig: ../obj/adig.o ../lib/libares.a
	${CC} ${LDFLAGS} -o bin/$@ ../obj/adig.o ../lib/libares.a ${ARESLIBS}

ahost: ../obj/ahost.o ../lib/libares.a
	${CC} ${LDFLAGS} -o bin/$@ ../obj/ahost.o ../lib/libares.a ${ARESLIBS}

../obj/%.o: src/%.c
	${CC} ${ALL_CFLAGS} ${ARESINCLUDES} -c ${LIB} $< -o $@ 

clean:
	rm -f ${OBJS} ../lib/libares.a ../obj/adig.o bin/adig \ 
	bin/ahost.o bin/ahost

distclean: clean
	rm -f config.cache config.log config.status Makefile
