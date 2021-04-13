#Makefile for  SA library for SA program development on ATT7300 running UNIX
# Here set up to use gcc, but should work with cc
# Assembler in AT&T assembler syntax
#  "make libsa.a"  "make install"
SALIB	=
SAINC	=    .
# gcc not -O'd since sa programs never big and at least kputc/kgetc fail
# to work with -O (see Makefile in kernel dir)
CC	=	gcc 
AS	=	as
ASFLAGS =
CFLAGS	=	-I$(SAINC) -DATT7300

OFILES	=	kprintf.o68 pscfuncts.o68 pssfuncts.o68 \
		sa_startup.o68 startup.o68 \
		printf.o68 sprintf.o68 scanf.o68 doprnt.o68 doscan.o68 \
		getchar.o68 putchar.o68 \
		sluinit_7300.o68 slu6850.o68 slu7201.o68 \
		gets.o68 puts.o68 \
		putc_7300.o68 kputc6850.o68 kputc7201.o68 \
	        getc_7300.o68 kgetc6850.o68 kgetc7201.o68

#clear old suffixes:
.SUFFIXES:
#set up our set:  first try .c.o68, then .s.o68--
.SUFFIXES: .o68 .c .s

.c.o68:
		$(CC) $(CFLAGS) -c -o $*.o68 $*.c

.s.o68:
		$(AS) $(ASFLAGS) -o $*.o68 $*.s

all:		libsa.a sa_startup.o68 

libsa.a:	${OFILES}
		rm -f libsa.a
		ar cr libsa.a $(OFILES) 

install:	all
		install -c libsa.a $(SALIB)
		cp sa_startup.o68 $(SALIB)
		
clean:
		rm -f ${OFILES} libsa.a

