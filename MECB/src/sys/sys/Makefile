# makefile for xinu kernel on ATT7300 (3B1) or MECB
# wipe out default suffixes list--
.SUFFIXES:
.SUFFIXES: .o68 .c .s
GNUBIN = /usr/local/gnu/bin
LIB	=	../../../lib
# cc should also work--
CC	=	gcc
CFLAGS	=	-O -m68000 -c -I../h -DDEBUG
CFLAGS_NOOPT =	-m68000 -c -I../h -DDEBUG
# Sun as should also work--
AS	=	$(GNUBIN)/as -m68000
LD	=	ld
AR	=	ar
#default loadaddr for xinu exe--
A	=	1000
AA	=       90000

.c.o68:
		$(CC) $(CFLAGS)	-o $*.o68 $*.c

.s.o68:
		$(AS) -o $*.o68 $*.s
	
OFILES	=	create.o68 dotrace.o68 freemem.o68	\
		getitem.o68 getmem.o68 getpid.o68 getstk.o68	\
		init.o68 initialize.o68 insert.o68 insertd.o68	\
		ioerr.o68 ionull.o68 kill.o68	\
		mark.o68 mdump.o68 newqueue.o68		\
	 	mkpool.o68 poolinit.o68 freebuf.o68 getbuf.o68	\
		queue.o68 ready.o68 receive.o68		\
		recvclr.o68 resched.o68 resume.o68 scount.o68	\
		screate.o68 sdelete.o68 send.o68 sendf.o68	\
		pcreate.o68 pdelete.o68 preceive.o68 psend.o68  \
		pinit.o68 preset.o68 pcount.o68                 \
		signal.o68 signaln.o68 sleep.o68 sleep10.o68	\
		sreset.o68 ssclock.o68 suspend.o68		\
		piint.o68 ttyread.o68 ttygetc.o68 		\
		ttycntl6850.o68 ttyiin6850.o68 ttyinit6850.o68	\
		ttyoin6850.o68 ttyputc6850.o68 ttywrite6850.o68	\
		sluint6850.o68 sluint7201.o68 slu7201.o68	\
		ttycntl7201.o68 ttyiin7201.o68 ttyinit7201.o68	\
		ttyoin7201.o68 ttyputc7201.o68 ttywrite7201.o68	\
		slu6850.o68 slu7201.o68		  		\
		userret.o68 wait.o68 wakeup.o68 xdone.o68	\
		read.o68 write.o68 init.o68 getc.o68 putc.o68	\
		control.o68 close.o68 open.o68 seek.o68 	\
		ctxsw.o68 getprio.o68 chprio.o68		\
		panic.o68 regdump.o68			 	\
		slu1int.o68 slu2int.o68 slu3int.o68 spurint.o68	\
		pscfuncts.o68 pssfuncts.o68		 	\
		setclkr.o68 startup.o68				\
		dlccntl.o68 dlciin.o68 dlcinit.o68 dlcoin.o68 	\
		dlcputc.o68 dlcread.o68 dlcwrite.o68 dldump.o68 \
		finput.o68 foutput.o68 freceive.o68 freebuf.o68 \
		freemem.o68 frinit.o68 frsend.o68 frtimer.o68 	\
		fsend.o68 netinit.o68 getmid.o68

# XOFILES--object files unconditionally loaded for Xinu.  The
# rest of the object files reside in libx.a, the Xinu library.
# We default to no net code by loading nullnet.o68 ahead of library--
# similarly, default to no real lp driver by loading lpstubs.o68,
# and no real dlc driver by loading dlcstubs.o68.
# Only conf/conf7300.c, kprintf.c, clkinit.c, lpstubs.c,
# sizmem/sizmem7300.s and clkint.s depend explicitly on 
# the platform (ATT7300 vs. MECB), and they are all
# in here, so the library libx.a can be used for both.
# Platform-independent unconditionally-loaded files--
XOFILES_COMMON =	startup.o68 initialize.o68 nullnet.o68 dlcstubs.o68

# MECB-only versions--
XOFILES_MECB =	conf.o68 clkinit.o68 clkint.o68 \
		kprintf.o68 lpstubs.o68 sizmem.o68

# ATT7300-only versions--
XOFILES_7300 =	conf7300.o68 clkinit7300.o68 clkint7300.o68 \
		kprintf7300.o68 lpstubs7300.o68 sizmem7300.o68

all:		libx.a $(XOFILES_COMMON) $(XOFILES_MECB) $(XOFILES_7300)

install:
		cp libx.a $(LIB)/libx.a
		ranlib $(LIB)/libx.a
		cp $(XOFILES_COMMON) $(XOFILES_MECB) $(XOFILES_7300) $(LIB)

clean:
		-rm -rf *.o68 ,* .,* *.68 *.68a
		-rm -rf libx.a a.out core make.out

conf.o68:	conf.c ../h/conf.h
		$(CC) $(CFLAGS) -o conf.o68 conf.c

conf7300.o68:   conf7300.c ../h/conf.h
		$(CC) $(CFLAGS) -DATT7300 -o conf7300.o68 conf7300.c

# clocks are different on MECB and ATT7300--
clkinit.o68:	clkinit.c  ../h/clock.h
		$(CC) $(CFLAGS) -o clkinit.o68 clkinit.c

clkinit7300.o68: clkinit.c
		$(CC) $(CFLAGS) -DATT7300 -o clkinit7300.o68 clkinit.c

clkint.o68:	clkint.s 
		/lib/cpp clkint.s > temp.s
		$(AS) $(ASFLAGS) temp.s -o clkint.o68; rm temp.s

clkint7300.o68: clkint.s
		/lib/cpp -DATT7300 clkint.s > temp.s
		$(AS) $(ASFLAGS) temp.s -o clkint7300.o68; rm temp.s

lpstubs.o68:	lpstubs.c
		$(CC) $(CFLAGS) -o lpstubs.o68 lpstubs.c

# Not entirely trivial, to prevent lp interrupt from crashing system--
lpstubs7300.o68:   lpstubs.c ../h/pi7300.h
		$(CC) $(CFLAGS) -DATT7300 -o lpstubs7300.o68 lpstubs.c

# consoles are different on MECB and ATT7300, and also
# avoid optimizer removing reloading status reg in polling loop--
kprintf.o68:	kprintf.c
		$(CC) $(CFLAGS_NOOPT) -DSLUCHIP=6850 -o kprintf.o68 kprintf.c
		
kprintf7300.o68: kprintf.c
		$(CC) $(CFLAGS_NOOPT) -DATT7300 -DSLUCHIP=7201 -o kprintf7300.o68 kprintf.c
		
# create 2 versions of tty code, one for 6850 UART, one for 7201 UART
		
# 6850 version--
ttycntl6850.o68:	ttycntl.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o ttycntl6850.o68 ttycntl.c

ttywrite6850.o68:	ttywrite.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o ttywrite6850.o68 ttywrite.c

ttyputc6850.o68:	ttyputc.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o ttyputc6850.o68 ttyputc.c

ttyiin6850.o68:	ttyiin.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o ttyiin6850.o68 ttyiin.c

ttyoin6850.o68:	ttyoin.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o ttyoin6850.o68 ttyoin.c

ttyinit6850.o68:	ttyinit.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o ttyinit6850.o68 ttyinit.c
		
sluint6850.o68:	sluint.c
		$(CC) $(CFLAGS) -DSLUCHIP=6850 -o sluint6850.o68 sluint.c
		
# 7201 version--
ttycntl7201.o68:	ttycntl.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o ttycntl7201.o68 ttycntl.c

ttywrite7201.o68:	ttywrite.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o ttywrite7201.o68 ttywrite.c

ttyputc7201.o68:	ttyputc.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o ttyputc7201.o68 ttyputc.c

ttyiin7201.o68:	ttyiin.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o ttyiin7201.o68 ttyiin.c

ttyoin7201.o68:	ttyoin.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o ttyoin7201.o68 ttyoin.c

ttyinit7201.o68: ttyinit.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o ttyinit7201.o68 ttyinit.c

sluint7201.o68:	sluint.c
		$(CC) $(CFLAGS) -DSLUCHIP=7201 -o sluint7201.o68 sluint.c

libx.a:		$(OFILES) 
		/bin/rm -f  libx.a
		$(AR) cr libx.a	 $(OFILES)
		ranlib libx.a

$(U).o68:	$(U).c
		$(CC) $(CFLAGS) -I../../../include $(U).c -o $(U).o68

$(U).68:	libx.a $(U).o68 
		$(LD) -N -T $(A) -o $(U).68 $(XOFILES_COMMON) \
		$(XOFILES_MECB) $(U).o68 $(LIB)/libxc.a \
		./libx.a $(LIB)/libxc.a $(LIB)/libcsupp.a

$(U).68a:	libx.a $(U).o68 
		$(LD) -N -T $(AA) -o $(U).68a $(XOFILES_COMMON) \
		$(XOFILES_7300) $(U).o68 $(LIB)/libxc.a \
		./libx.a $(LIB)/libxc.a $(LIB)/libcsupp.a

