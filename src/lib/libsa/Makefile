
#Makefile for  SA Xinu library, for MECB and ATT7300
SALIB	=   ../../../sa
SAINC	=   .
# gcc not -O'd since sa programs never big and at least kputc/kgetc fail
# to work with -O (see Makefile in kernel dir)
#  In fact, s91, added -g in hopes that remote gdb debugging will be avail
CC	=	gcc 
AS	=	as
ASFLAGS =	-m68000
CFLAGS	=	-I$(SAINC) -g -m68000

OFILES_MECB  =  kprintf6850.o68 pscfuncts.o68 slu6850.o68 \
		putc_mecb.o68 kputc6850.o68 \
	        getc_mecb.o68 kgetc6850.o68 pssfuncts.o68 \
		slu1int.o68 slu2int.o68 slu3int.o68 clockint.o68 \
		spurint.o68

OFILES_ATT7300  = kprintf7201.o68 pscfuncts.o68 \
		slu6850.o68 slu7201.o68 \
		putc_7300.o68 kputc6850.o68 kputc7201.o68 \
	        getc_7300.o68 kgetc6850.o68 kgetc7201.o68 pssfuncts.o68 \
		slu1int.o68 slu2int.o68 slu3int.o68 clockint.o68 \
		spurint.o68

#clear old suffixes:
.SUFFIXES:
#set up our set:  first try .c.o68, then .s.o68--
.SUFFIXES: .o68 .c .s

.c.o68:
		$(CC) $(CFLAGS) -c -o $*.o68 $*.c

.s.o68:
		$(AS) $(ASFLAGS) -o $*.o68 $*.s

all:		libsa_mecb.a libsa_7300.a \
		sa_startup_mecb.o68 sa_startup_7300.o68 

libsa_mecb.a:	${OFILES_MECB}
		rm -f libsa_mecb.a
		ar cr libsa_mecb.a $(OFILES_MECB) 
		ranlib libsa_mecb.a

libsa_7300.a: ${OFILES_ATT7300}
		rm -f libsa_7300.a
		ar cr libsa_7300.a $(OFILES_ATT7300) 
		ranlib libsa_7300.a

install:	all
		install -c libsa_mecb.a $(SALIB)
		install -c libsa_7300.a $(SALIB)
		ranlib $(SALIB)/libsa_mecb.a
		ranlib $(SALIB)/libsa_7300.a
		cp sa_startup_mecb.o68 sa_startup_7300.o68 $(SALIB)
		cp stdio.h slu7201.h slu6850.h $(SALIB)
clean:
		rm -f ${OFILES} libsa.a

# 2 version of kprintf--
# for MECB--6850 console
kprintf6850.o68: kprintf.c
		$(CC) $(CFLAGS) -c -o kprintf6850.o68 kprintf.c

# build for ATT7300--7201 console
kprintf7201.o68: kprintf.c
		$(CC) $(CFLAGS) -c -DATT7300 -o kprintf7201.o68 kprintf.c


