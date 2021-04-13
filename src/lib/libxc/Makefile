#Makefile for 68000/68010 C library  "make libxc.a"  "make install"
DIR	=	../../..
LIB	=	$(DIR)/lib
GNUBIN	=	/usr/local/gnu/bin
# where gnulib is built for 68000--
GNULIB68 = ../gnulib
# Standard C library--a few arithmetic routines are taken from here--
#  even tho that risks inclusion of 68020-extended instructions
CLIB    =   /lib/libc.a
# save code space by using -O--as code looks nicer too--
CC	=	gcc -m68000
AS	=	$(GNUBIN)/as
ASFLAGS =	-m68000
CFLAGS	=	-I$(DIR)/include

CFILES	=	abs.c atoi.c ctype_.c doprnt.c doscan.c ecvt.c	\
		fgets.c fprintf.c fputs.c gets.c	\
		index.c printf.c puts.c qsort.c		\
		rand.c rindex.c scanf.c sprintf.c	\
		strcat.c strcmp.c strcpy.c strlen.c	\
		strncat.c strncmp.c strncpy.c

SFILES	=	setjmp.s

OFILES	=	abs.o68 atoi.o68 doscan.o68 ecvt.o68 \
		fgets.o68 fprintf.o68 fputs.o68	\
		gets.o68 index.o68 printf.o68 puts.o68		\
		qsort.o68 rand.o68 rindex.o68 scanf.o68		\
		setjmp.o68 sprintf.o68 doprnt.o68 strcat.o68 strcmp.o68	\
		strcpy.o68 strlen.o68 strncat.o68 strncmp.o68	\
		strncpy.o68 getchar.o68 putchar.o68

#extracted from system C library--look OK for 68000 (arith. support for cc)
OFILES_FROM_LIBC = lmodt.o ldivt.o ptwo.o lmult.o

#clear old suffixes:
.SUFFIXES:
#set up our set:  first try .c.o68, then .s.o68--
.SUFFIXES: .o68 .c .s

.c.o68:
		$(CC) $(CFLAGS) -c -o $*.o68 $*.c

.s.o68:
		$(AS) $(ASFLAGS) -o $*.o68 $*.s

all:		libxc.a libcsupp.a

libxc.a:	${OFILES}
		rm -f libxc.a
		ar cr libxc.a $(OFILES)
		ranlib libxc.a

# combine the arithmetic support from gnulib and the C library
# in one library to avoid multiple library searches--
libcsupp.a:	$(CLIB)
		cp $(GNULIB68)/gnulib libcsupp.a
		ar x $(CLIB) $(OFILES_FROM_LIBC)
		ar r libcsupp.a $(OFILES_FROM_LIBC)
		ranlib libcsupp.a

install:	all
		install -c libxc.a $(LIB)
		ranlib $(LIB)/libxc.a
		install -c libcsupp.a $(LIB)
		ranlib $(LIB)/libcsupp.a
		
clean:
		rm -f ${OFILES} ${OFILES_FROM_LIBC} libxc.a libcsupp.a

