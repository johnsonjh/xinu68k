#
#  Makefile for the Sun-3 version of Xinu.
#

DIR	=	/u/u5/xinu/xinu7.sun3
CFLAGS	=	-c -O -I../h
SFLAGS	=	-I../h
AR	=	/bin/ar
LIB	=	${DIR}/lib

#
#   for cross compiling
#
#BINDIR	=	/usr/sun/bin
#CLIB	=	/usr/sun/lib
#CC	=	${BINDIR}/cc68
#CPP	=	${BINDIR}/cpp
#LD	=	${BINDIR}/ld68

CFILES=	arp_in.c	arpfind.c	arpinit.c	autostart.c	\
	blkcopy.c	blkequ.c	chprio.c	ckmode.c	\
	cksum.c		clkinit.c			close.c		\
	concntl.c	conf.c		coniin.c	coninit.c	\
	coninput.c			conoutput.c	control.c	\
	create.c			dgalloc.c	dgclose.c	\
	dgcntl.c	dginit.c	dgmcntl.c	dgmopen.c	\
	dgparse.c	dgread.c	dgwrite.c			\
	dot2ip.c	ethinit.c	ethcntl.c	ethinter.c	\
	ethread.c	ethcntl.c	ethstrt.c	ethwrite.c	\
	ethwstrt.c	evec.c		freebuf.c	freemem.c	\
	getaddr.c	getbuf.c	getc.c		getdmem.c	\
			getitem.c	getmem.c	getname.c	\
	getnet.c	getpath.c	getpid.c	getprio.c	\
	getstk.c	gettime.c	getutim.c	icmp_in.c	\
	init.c		insert.c	insertd.c			\
	ioerr.c		ionull.c	ip2name.c	ip_in.c		\
	ipsend.c	kill.c		kprintf.c	mark.c		\
	mkarp.c		mkdpool.c	mkpool.c	mount.c		\
	naminit.c	nammap.c	namopen.c	namrepl.c	\
	netdump.c	netin.c		netinit.c	netnum.c	\
	netout.c	newqueue.c	nqalloc.c	open.c		\
	panic.c		pcount.c	pcreate.c	pdelete.c	\
	pinit.c		poolinit.c	preceive.c	preset.c	\
	printq.c	psend.c		ptclear.c	putc.c		\
	queue.c		rarp_in.c	read.c		ready.c		\
	receive.c	recvclr.c	recvtim.c	remove.c	\
	rename.c	resched.c	resume.c	ret_mon.c	\
	rfalloc.c	rfclose.c	rfcntl.c	rfdump.c	\
	rfgetc.c	rfinit.c	rfio.c		rfmkpac.c	\
	rfopen.c	rfputc.c	rfread.c	rfseek.c	\
	rfsend.c	rfwrite.c	route.c		rwho.c		\
	rwhod.c		rwhoind.c	scount.c	screate.c	\
	sdelete.c	seek.c		send.c		sendf.c		\
			setdev.c	setnok.c			\
	signal.c	signaln.c	sleep.c		sleep10.c	\
	sndrarp.c	sreset.c	ssclock.c	suspend.c	\
			ttycntl.c	ttygetc.c	ttyiin.c	\
	ttyinit.c			ttyoin.c	ttyputc.c	\
	ttyread.c	ttyutils.c	ttywrite.c	udpecho.c	\
	udpnxtp.c	udpsend.c	unmount.c	unsleep.c	\
	userret.c	wait.c		wakeup.c	write.c		\
	xdone.c		access.c


SFILES=	clkint.s	conint.s	ctxsw.s		ethint.s	\
	getidprom.s	intr.s		set_evec.s	startup.s	\
	trap.s		ttyint.s	mmu.s


OFILES=	arp_in.o	arpfind.o	arpinit.o	autostart.o	\
	blkcopy.o	blkequ.o	chprio.o	ckmode.o	\
	cksum.o		clkinit.o	clkint.o	close.o		\
	concntl.o	conf.o		coniin.o	coninit.o	\
	coninput.o	conint.o	conoutput.o	control.o	\
	create.o	ctxsw.o		dgalloc.o	dgclose.o	\
	dgcntl.o	dginit.o	dgmcntl.o	dgmopen.o	\
	dgparse.o	dgread.o	dgwrite.o			\
	dot2ip.o	ethinit.o	ethint.o	ethinter.o	\
	ethread.o	ethcntl.o	ethstrt.o	ethwrite.o	\
	ethwstrt.o	evec.o		freebuf.o	freemem.o	\
	getaddr.o	getbuf.o	getc.o		getdmem.o	\
	getidprom.o	getitem.o	getmem.o	getname.o	\
	getnet.o	getpath.o	getpid.o	getprio.o	\
	getstk.o	gettime.o	getutim.o	icmp_in.o	\
	init.o		insert.o	insertd.o	intr.o		\
	ioerr.o		ionull.o	ip2name.o	ip_in.o		\
	ipsend.o	kill.o		kprintf.o	mark.o		\
	mkarp.o		mkdpool.o	mkpool.o	mount.o		\
	naminit.o	nammap.o	namopen.o	namrepl.o	\
	netdump.o	netin.o		netinit.o	netnum.o	\
	netout.o	newqueue.o	nqalloc.o	open.o		\
	panic.o		pcount.o	pcreate.o	pdelete.o	\
	pinit.o		poolinit.o	preceive.o	preset.o	\
	printq.o	psend.o		ptclear.o	putc.o		\
	queue.o		rarp_in.o	read.o		ready.o		\
	receive.o	recvclr.o	recvtim.o	remove.o	\
	rename.o	resched.o	resume.o	ret_mon.o	\
	rfalloc.o	rfclose.o	rfcntl.o	rfdump.o	\
	rfgetc.o	rfinit.o	rfio.o		rfmkpac.o	\
	rfopen.o	rfputc.o	rfread.o	rfseek.o	\
	rfsend.o	rfwrite.o	route.o		rwho.o		\
	rwhod.o		rwhoind.o	scount.o	screate.o	\
	sdelete.o	seek.o		send.o		sendf.o		\
	set_evec.o	setdev.o	setnok.o	sh.o		\
	signal.o	signaln.o	sleep.o		sleep10.o	\
	sndrarp.o	sreset.o	ssclock.o	suspend.o	\
	trap.o		ttycntl.o	ttygetc.o	ttyiin.o	\
	ttyinit.o	ttyint.o	ttyoin.o	ttyputc.o	\
	ttyread.o	ttyutils.o	ttywrite.o	udpecho.o	\
	udpnxtp.o	udpsend.o	unmount.o	unsleep.o	\
	userret.o	wait.o		wakeup.o	write.o		\
	xdone.o		mmu.o		access.o



XOFILES=	startup.o initialize.o

HFILES=		../h/conf.h	\
		../h/disk.h	\
		../h/io.h	\
		../h/kernel.h	\
		../h/mem.h	\
		../h/nec7201.h	\
		../h/proc.h	\
		../h/q.h	\
		../h/sem.h	\
		../h/sunmem2.h	\
		../h/tty.h


all:		libx.a xinu.o a.out

download:
		/bin/rm -f /tftpboot/a.out
		cp a.out /tftpboot/a.out
		chmod a+r /tftpboot/a.out


install:
		/bin/cp libx.a $(LIB)/libx.a
		ranlib $(LIB)/libx.a
		/bin/cp xinu.o $(LIB)

clean:
		-rm -rf *.o ,* .,* ../h/,*
		-rm -rf libx.a a.out conf.c ../h/conf.h core make.out

everything:	a.out lint

lint:		,lint

,lint:		$(CFILES)
		lint -n -I../h $(CFILES) | Flint > ,lint

nm:		,nm
		mv ,nm nm

,nm:		a.out
		nm pm.out > ,nm

conf.c:		Configuration
		./config

sh.o:
		(cd shell; make install)

libx.a:		$(OFILES) 
		/bin/rm -f  libx.a
		$(AR) cr libx.a	`lorder $(OFILES) | tsort`
		ranlib libx.a

xinu.o:		$(XOFILES)
		$(LD) -r $(XOFILES) -o xinu.o

fast:		x_create.o test.o
		$(LD) -e _start -N -X -T 00004000 xinu.o x_create.o libx.a test.o libx.a ${LIB}/libxc.a libx.a ${LIB}/libxc.a -o b.out
		cp b.out pm.out
	        strip b.out;dd if=b.out of=a.out ibs=32 skip=1

a.out:		libx.a xinu.o test.o Makefile
		$(LD) -e _start -N -X -T 00004000 xinu.o libx.a test.o libx.a ${LIB}/libxc.a libx.a ${LIB}/libxc.a -o b.out
		cp b.out pm.out
	        strip b.out;dd if=b.out of=a.out ibs=32 skip=1

../h/conf.h:	Configuration
		./config
#		rm $(DIR)/include/conf.h
#		ln ../h/conf.h $(DIR)/include/conf.h

depend:
		grep '^#include' *.c *.s 			\
		| sed	-e 's/</"..\/h\//;s/>/"/'		\
			-e 's/:[^"]*"\([^"]*\)".*/: \1/'	\
		| sed	's/\.c/.o/'				\
		| sed	's/\.s/.o/' >makedep
		echo '/^# DO NOT DELETE	THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile .,oldmake
		cp Makefile ,Makefile
		ed - ,Makefile < eddep
		/bin/rm eddep makedep
		echo '#	DEPENDENCIES MUST END AT END OF	FILE' >>,Makefile
		echo '#	IF YOU PUT STUFF HERE IT WILL GO AWAY'>>,Makefile
		echo '#	see make depend	above'>>,Makefile
		/bin/mv ,Makefile Makefile

# DO NOT DELETE	THIS LINE
# DEPENDENCIES MUST END	AT END OF FILE
arp_in.o: ../h/conf.h
arp_in.o: ../h/kernel.h
arp_in.o: ../h/proc.h
arp_in.o: ../h/network.h
arpfind.o: ../h/conf.h
arpfind.o: ../h/kernel.h
arpfind.o: ../h/proc.h
arpfind.o: ../h/network.h
arpinit.o: ../h/conf.h
arpinit.o: ../h/kernel.h
arpinit.o: ../h/proc.h
arpinit.o: ../h/network.h
autostart.o: ../h/conf.h
autostart.o: ../h/sunromvec.h
autostart.o: ../h/kernel.h
blkcopy.o: ../h/kernel.h
blkequ.o: ../h/kernel.h
chprio.o: ../h/conf.h
chprio.o: ../h/kernel.h
chprio.o: ../h/proc.h
ckmode.o: ../h/conf.h
ckmode.o: ../h/kernel.h
ckmode.o: ../h/file.h
clkinit.o: ../h/conf.h
clkinit.o: ../h/kernel.h
clkinit.o: ../h/sleep.h
clkinit.o: ../h/cpu.addrs.h
clkinit.o: ../h/clock.h
clkinit.o: ../h/interreg.h
clkinit.o: ../h/sunromvec.h
close.o: ../h/conf.h
close.o: ../h/kernel.h
close.o: ../h/io.h
concntl.o: ../h/conf.h
concntl.o: ../h/kernel.h
conf.o: ../h/conf.h
coniin.o: ../h/conf.h
coniin.o: ../h/kernel.h
coniin.o: ../h/io.h
coniin.o: ../h/slu.h
coniin.o: ../h/zsreg.h
coniin.o: ../h/sunromvec.h
coninit.o: ../h/conf.h
coninit.o: ../h/kernel.h
coninit.o: ../h/tty.h
coninit.o: ../h/io.h
coninit.o: ../h/slu.h
coninit.o: ../h/zsreg.h
coninput.o: ../h/conf.h
coninput.o: ../h/kernel.h
coninput.o: ../h/io.h
conoutput.o: ../h/conf.h
conoutput.o: ../h/kernel.h
conoutput.o: ../h/io.h
conoutput.o: ../h/tty.h
conoutput.o: ../h/sunromvec.h
console.o: ../h/conf.h
console.o: ../h/kernel.h
console.o: ../h/io.h
control.o: ../h/conf.h
control.o: ../h/kernel.h
control.o: ../h/io.h
create.o: ../h/conf.h
create.o: ../h/kernel.h
create.o: ../h/proc.h
create.o: ../h/sem.h
create.o: ../h/mem.h
create.o: ../h/io.h
dgalloc.o: ../h/conf.h
dgalloc.o: ../h/kernel.h
dgalloc.o: ../h/network.h
dgclose.o: ../h/conf.h
dgclose.o: ../h/kernel.h
dgclose.o: ../h/proc.h
dgclose.o: ../h/network.h
dgcntl.o: ../h/conf.h
dgcntl.o: ../h/kernel.h
dgcntl.o: ../h/network.h
dgdump.o: ../h/conf.h
dgdump.o: ../h/kernel.h
dgdump.o: ../h/network.h
dginit.o: ../h/conf.h
dginit.o: ../h/kernel.h
dginit.o: ../h/network.h
dgmcntl.o: ../h/conf.h
dgmcntl.o: ../h/kernel.h
dgmcntl.o: ../h/network.h
dgmopen.o: ../h/conf.h
dgmopen.o: ../h/kernel.h
dgmopen.o: ../h/network.h
dgparse.o: ../h/conf.h
dgparse.o: ../h/kernel.h
dgparse.o: ../h/network.h
dgparse.o: ../h/ctype.h
dgread.o: ../h/conf.h
dgread.o: ../h/kernel.h
dgread.o: ../h/proc.h
dgread.o: ../h/network.h
dgwrite.o: ../h/conf.h
dgwrite.o: ../h/kernel.h
dgwrite.o: ../h/network.h
dot2ip.o: ../h/conf.h
dot2ip.o: ../h/kernel.h
dot2ip.o: ../h/network.h
ethcntl.o: ../h/conf.h
ethcntl.o: ../h/kernel.h
ethcntl.o: ../h/lereg.h
ethcntl.o: ../h/ether.h
ethinit.o: ../h/conf.h
ethinit.o: ../h/kernel.h
ethinit.o: ../h/network.h
ethinit.o: ../h/idprom.h
ethinit.o: ../h/mmu.h
ethinter.o: ../h/conf.h
ethinter.o: ../h/kernel.h
ethinter.o: ../h/proc.h
ethinter.o: ../h/network.h
ethread.o: ../h/conf.h
ethread.o: ../h/kernel.h
ethread.o: ../h/proc.h
ethread.o: ../h/network.h
ethstrt.o: ../h/conf.h
ethstrt.o: ../h/kernel.h
ethstrt.o: ../h/network.h
ethstrt.o: ../h/idprom.h
ethstrt.o: ../h/mmu.h
ethwrite.o: ../h/conf.h
ethwrite.o: ../h/kernel.h
ethwrite.o: ../h/proc.h
ethwrite.o: ../h/network.h
ethwstrt.o: ../h/conf.h
ethwstrt.o: ../h/kernel.h
ethwstrt.o: ../h/proc.h
ethwstrt.o: ../h/network.h
evec.o: ../h/conf.h
evec.o: ../h/kernel.h
evec.o: ../h/proc.h
evec.o: ../h/sem.h
evec.o: ../h/mem.h
evec.o: ../h/tty.h
evec.o: ../h/q.h
evec.o: ../h/io.h
evec.o: ../h/sunromvec.h
evec.o: ../h/cpu.addrs.h
evec.o: ../h/interreg.h
freebuf.o: ../h/conf.h
freebuf.o: ../h/kernel.h
freebuf.o: ../h/mark.h
freebuf.o: ../h/bufpool.h
freemem.o: ../h/conf.h
freemem.o: ../h/kernel.h
freemem.o: ../h/mem.h
getaddr.o: ../h/conf.h
getaddr.o: ../h/kernel.h
getaddr.o: ../h/network.h
getbuf.o: ../h/conf.h
getbuf.o: ../h/kernel.h
getbuf.o: ../h/mark.h
getbuf.o: ../h/bufpool.h
getc.o: ../h/conf.h
getc.o: ../h/kernel.h
getc.o: ../h/io.h
getdmem.o: ../h/conf.h
getdmem.o: ../h/kernel.h
getdmem.o: ../h/mark.h
getdmem.o: ../h/mmu.h
getdmem.o: ../h/mem.h
getdmem.o: ../h/network.h
getitem.o: ../h/conf.h
getitem.o: ../h/kernel.h
getitem.o: ../h/q.h
getmem.o: ../h/conf.h
getmem.o: ../h/kernel.h
getmem.o: ../h/mem.h
getname.o: ../h/conf.h
getname.o: ../h/kernel.h
getname.o: ../h/network.h
getnet.o: ../h/conf.h
getnet.o: ../h/kernel.h
getnet.o: ../h/network.h
getpath.o: ../h/conf.h
getpath.o: ../h/kernel.h
getpath.o: ../h/proc.h
getpath.o: ../h/network.h
getpid.o: ../h/conf.h
getpid.o: ../h/kernel.h
getpid.o: ../h/proc.h
getprio.o: ../h/conf.h
getprio.o: ../h/kernel.h
getprio.o: ../h/proc.h
getstk.o: ../h/conf.h
getstk.o: ../h/kernel.h
getstk.o: ../h/mem.h
gettime.o: ../h/conf.h
gettime.o: ../h/kernel.h
gettime.o: ../h/date.h
getutim.o: ../h/conf.h
getutim.o: ../h/kernel.h
getutim.o: ../h/sleep.h
getutim.o: ../h/date.h
getutim.o: ../h/network.h
icmp_in.o: ../h/conf.h
icmp_in.o: ../h/kernel.h
icmp_in.o: ../h/network.h
init.o: ../h/conf.h
init.o: ../h/kernel.h
init.o: ../h/io.h
initialize.o: ../h/conf.h
initialize.o: ../h/kernel.h
initialize.o: ../h/proc.h
initialize.o: ../h/sem.h
initialize.o: ../h/sleep.h
initialize.o: ../h/mem.h
initialize.o: ../h/tty.h
initialize.o: ../h/q.h
initialize.o: ../h/io.h
initialize.o: ../h/network.h
insert.o: ../h/conf.h
insert.o: ../h/kernel.h
insert.o: ../h/q.h
insertd.o: ../h/conf.h
insertd.o: ../h/kernel.h
insertd.o: ../h/q.h
ioerr.o: ../h/conf.h
ioerr.o: ../h/kernel.h
ionull.o: ../h/conf.h
ionull.o: ../h/kernel.h
ip2name.o: ../h/conf.h
ip2name.o: ../h/kernel.h
ip2name.o: ../h/network.h
ip_in.o: ../h/conf.h
ip_in.o: ../h/kernel.h
ip_in.o: ../h/proc.h
ip_in.o: ../h/network.h
ipsend.o: ../h/conf.h
ipsend.o: ../h/kernel.h
ipsend.o: ../h/network.h
kill.o: ../h/conf.h
kill.o: ../h/kernel.h
kill.o: ../h/proc.h
kill.o: ../h/sem.h
kill.o: ../h/mem.h
kill.o: ../h/io.h
kprintf.o: ../h/sunromvec.h
kprintf.o: ../h/conf.h
kprintf.o: ../h/kernel.h
kprintf.o: ../h/io.h
kprintf.o: ../h/tty.h
mark.o: ../h/conf.h
mark.o: ../h/kernel.h
mark.o: ../h/mark.h
mkarp.o: ../h/conf.h
mkarp.o: ../h/kernel.h
mkarp.o: ../h/network.h
mkdpool.o: ../h/conf.h
mkdpool.o: ../h/kernel.h
mkdpool.o: ../h/mark.h
mkdpool.o: ../h/bufpool.h
mkdpool.o: ../h/network.h
mkpool.o: ../h/conf.h
mkpool.o: ../h/kernel.h
mkpool.o: ../h/mark.h
mkpool.o: ../h/bufpool.h
mount.o: ../h/conf.h
mount.o: ../h/kernel.h
mount.o: ../h/name.h
naminit.o: ../h/conf.h
naminit.o: ../h/kernel.h
naminit.o: ../h/name.h
nammap.o: ../h/conf.h
nammap.o: ../h/kernel.h
nammap.o: ../h/name.h
namopen.o: ../h/conf.h
namopen.o: ../h/kernel.h
namopen.o: ../h/name.h
namrepl.o: ../h/conf.h
namrepl.o: ../h/kernel.h
namrepl.o: ../h/name.h
netdump.o: ../h/conf.h
netdump.o: ../h/kernel.h
netdump.o: ../h/network.h
netin.o: ../h/conf.h
netin.o: ../h/kernel.h
netin.o: ../h/proc.h
netin.o: ../h/network.h
netin.o: ../h/mark.h
netin.o: ../h/bufpool.h
netinit.o: ../h/conf.h
netinit.o: ../h/kernel.h
netinit.o: ../h/sleep.h
netinit.o: ../h/network.h
netnum.o: ../h/conf.h
netnum.o: ../h/kernel.h
netnum.o: ../h/network.h
netout.o: ../h/conf.h
netout.o: ../h/kernel.h
netout.o: ../h/network.h
newqueue.o: ../h/conf.h
newqueue.o: ../h/kernel.h
newqueue.o: ../h/q.h
nqalloc.o: ../h/conf.h
nqalloc.o: ../h/kernel.h
nqalloc.o: ../h/proc.h
nqalloc.o: ../h/network.h
open.o: ../h/conf.h
open.o: ../h/kernel.h
open.o: ../h/io.h
panic.o: ../h/kernel.h
pcount.o: ../h/conf.h
pcount.o: ../h/kernel.h
pcount.o: ../h/mark.h
pcount.o: ../h/ports.h
pcreate.o: ../h/conf.h
pcreate.o: ../h/kernel.h
pcreate.o: ../h/mark.h
pcreate.o: ../h/ports.h
pdelete.o: ../h/conf.h
pdelete.o: ../h/kernel.h
pdelete.o: ../h/mark.h
pdelete.o: ../h/ports.h
pinit.o: ../h/conf.h
pinit.o: ../h/kernel.h
pinit.o: ../h/mark.h
pinit.o: ../h/ports.h
poolinit.o: ../h/conf.h
poolinit.o: ../h/kernel.h
poolinit.o: ../h/mark.h
poolinit.o: ../h/bufpool.h
preceive.o: ../h/conf.h
preceive.o: ../h/kernel.h
preceive.o: ../h/mark.h
preceive.o: ../h/ports.h
preset.o: ../h/conf.h
preset.o: ../h/kernel.h
preset.o: ../h/mark.h
preset.o: ../h/ports.h
printq.o: ../h/q.h
psend.o: ../h/conf.h
psend.o: ../h/kernel.h
psend.o: ../h/mark.h
psend.o: ../h/ports.h
ptclear.o: ../h/conf.h
ptclear.o: ../h/kernel.h
ptclear.o: ../h/mark.h
ptclear.o: ../h/ports.h
putc.o: ../h/conf.h
putc.o: ../h/kernel.h
putc.o: ../h/io.h
queue.o: ../h/conf.h
queue.o: ../h/kernel.h
queue.o: ../h/q.h
rarp_in.o: ../h/conf.h
rarp_in.o: ../h/kernel.h
rarp_in.o: ../h/proc.h
rarp_in.o: ../h/network.h
read.o: ../h/conf.h
read.o: ../h/kernel.h
read.o: ../h/io.h
ready.o: ../h/conf.h
ready.o: ../h/kernel.h
ready.o: ../h/proc.h
ready.o: ../h/q.h
receive.o: ../h/conf.h
receive.o: ../h/kernel.h
receive.o: ../h/proc.h
recvclr.o: ../h/conf.h
recvclr.o: ../h/kernel.h
recvclr.o: ../h/proc.h
recvtim.o: ../h/conf.h
recvtim.o: ../h/kernel.h
recvtim.o: ../h/proc.h
recvtim.o: ../h/q.h
recvtim.o: ../h/sleep.h
remove.o: ../h/conf.h
remove.o: ../h/kernel.h
remove.o: ../h/file.h
remove.o: ../h/name.h
rename.o: ../h/conf.h
rename.o: ../h/kernel.h
rename.o: ../h/file.h
rename.o: ../h/name.h
resched.o: ../h/conf.h
resched.o: ../h/kernel.h
resched.o: ../h/proc.h
resched.o: ../h/q.h
resume.o: ../h/conf.h
resume.o: ../h/kernel.h
resume.o: ../h/proc.h
ret_mon.o: ../h/conf.h
ret_mon.o: ../h/kernel.h
ret_mon.o: ../h/cpu.addrs.h
ret_mon.o: ../h/interreg.h
ret_mon.o: ../h/sunromvec.h
ret_mon.o: ../h/clock.h
ret_mon.o: ../h/proc.h
rfalloc.o: ../h/conf.h
rfalloc.o: ../h/kernel.h
rfalloc.o: ../h/fserver.h
rfalloc.o: ../h/rfile.h
rfclose.o: ../h/conf.h
rfclose.o: ../h/kernel.h
rfclose.o: ../h/network.h
rfcntl.o: ../h/conf.h
rfcntl.o: ../h/kernel.h
rfcntl.o: ../h/network.h
rfdump.o: ../h/conf.h
rfdump.o: ../h/kernel.h
rfdump.o: ../h/fserver.h
rfdump.o: ../h/rfile.h
rfgetc.o: ../h/conf.h
rfgetc.o: ../h/kernel.h
rfgetc.o: ../h/network.h
rfinit.o: ../h/conf.h
rfinit.o: ../h/kernel.h
rfinit.o: ../h/fserver.h
rfinit.o: ../h/rfile.h
rfio.o: ../h/conf.h
rfio.o: ../h/kernel.h
rfio.o: ../h/network.h
rfmkpac.o: ../h/conf.h
rfmkpac.o: ../h/kernel.h
rfmkpac.o: ../h/network.h
rfopen.o: ../h/conf.h
rfopen.o: ../h/kernel.h
rfopen.o: ../h/network.h
rfputc.o: ../h/conf.h
rfputc.o: ../h/kernel.h
rfputc.o: ../h/network.h
rfread.o: ../h/conf.h
rfread.o: ../h/kernel.h
rfread.o: ../h/network.h
rfseek.o: ../h/conf.h
rfseek.o: ../h/kernel.h
rfseek.o: ../h/fserver.h
rfseek.o: ../h/rfile.h
rfsend.o: ../h/conf.h
rfsend.o: ../h/kernel.h
rfsend.o: ../h/network.h
rfwrite.o: ../h/conf.h
rfwrite.o: ../h/kernel.h
rfwrite.o: ../h/network.h
route.o: ../h/conf.h
route.o: ../h/kernel.h
route.o: ../h/network.h
rwho.o: ../h/conf.h
rwho.o: ../h/kernel.h
rwho.o: ../h/network.h
rwho.o: ../h/rwho.h
rwhod.o: ../h/conf.h
rwhod.o: ../h/kernel.h
rwhod.o: ../h/network.h
rwhod.o: ../h/rwho.h
rwhod.o: ../h/shell.h
rwhoind.o: ../h/conf.h
rwhoind.o: ../h/kernel.h
rwhoind.o: ../h/network.h
rwhoind.o: ../h/rwho.h
scount.o: ../h/conf.h
scount.o: ../h/kernel.h
scount.o: ../h/sem.h
screate.o: ../h/conf.h
screate.o: ../h/kernel.h
screate.o: ../h/proc.h
screate.o: ../h/q.h
screate.o: ../h/sem.h
sdelete.o: ../h/conf.h
sdelete.o: ../h/kernel.h
sdelete.o: ../h/proc.h
sdelete.o: ../h/q.h
sdelete.o: ../h/sem.h
seek.o: ../h/conf.h
seek.o: ../h/kernel.h
seek.o: ../h/io.h
send.o: ../h/conf.h
send.o: ../h/kernel.h
send.o: ../h/proc.h
sendf.o: ../h/conf.h
sendf.o: ../h/kernel.h
sendf.o: ../h/proc.h
setdev.o: ../h/conf.h
setdev.o: ../h/kernel.h
setdev.o: ../h/proc.h
setnok.o: ../h/conf.h
setnok.o: ../h/kernel.h
setnok.o: ../h/proc.h
signal.o: ../h/conf.h
signal.o: ../h/kernel.h
signal.o: ../h/proc.h
signal.o: ../h/q.h
signal.o: ../h/sem.h
signaln.o: ../h/conf.h
signaln.o: ../h/kernel.h
signaln.o: ../h/proc.h
signaln.o: ../h/q.h
signaln.o: ../h/sem.h
sleep.o: ../h/conf.h
sleep.o: ../h/kernel.h
sleep.o: ../h/proc.h
sleep.o: ../h/q.h
sleep.o: ../h/sleep.h
sleep10.o: ../h/conf.h
sleep10.o: ../h/kernel.h
sleep10.o: ../h/proc.h
sleep10.o: ../h/q.h
sleep10.o: ../h/sleep.h
sndrarp.o: ../h/conf.h
sndrarp.o: ../h/kernel.h
sndrarp.o: ../h/network.h
sreset.o: ../h/conf.h
sreset.o: ../h/kernel.h
sreset.o: ../h/proc.h
sreset.o: ../h/q.h
sreset.o: ../h/sem.h
ssclock.o: ../h/conf.h
ssclock.o: ../h/kernel.h
ssclock.o: ../h/proc.h
ssclock.o: ../h/q.h
ssclock.o: ../h/sleep.h
suspend.o: ../h/conf.h
suspend.o: ../h/kernel.h
suspend.o: ../h/proc.h
test.o: ../h/conf.h
test.o: ../h/kernel.h
test.o: ../h/network.h
ttycntl.o: ../h/conf.h
ttycntl.o: ../h/kernel.h
ttycntl.o: ../h/tty.h
ttycntl.o: ../h/io.h
ttycntl.o: ../h/slu.h
ttygetc.o: ../h/conf.h
ttygetc.o: ../h/kernel.h
ttygetc.o: ../h/tty.h
ttygetc.o: ../h/io.h
ttyiin.o: ../h/conf.h
ttyiin.o: ../h/kernel.h
ttyiin.o: ../h/tty.h
ttyiin.o: ../h/io.h
ttyiin.o: ../h/slu.h
ttyiin.o: ../h/zsreg.h
ttyinit.o: ../h/conf.h
ttyinit.o: ../h/kernel.h
ttyinit.o: ../h/proc.h
ttyinit.o: ../h/tty.h
ttyinit.o: ../h/io.h
ttyinit.o: ../h/slu.h
ttyinit.o: ../h/zsreg.h
ttyoin.o: ../h/conf.h
ttyoin.o: ../h/kernel.h
ttyoin.o: ../h/tty.h
ttyoin.o: ../h/io.h
ttyoin.o: ../h/slu.h
ttyoin.o: ../h/zsreg.h
ttyputc.o: ../h/conf.h
ttyputc.o: ../h/kernel.h
ttyputc.o: ../h/tty.h
ttyputc.o: ../h/io.h
ttyputc.o: ../h/slu.h
ttyputc.o: ../h/zsreg.h
ttyread.o: ../h/conf.h
ttyread.o: ../h/kernel.h
ttyread.o: ../h/tty.h
ttyread.o: ../h/io.h
ttyutils.o: ../h/conf.h
ttyutils.o: ../h/kernel.h
ttyutils.o: ../h/tty.h
ttyutils.o: ../h/io.h
ttyutils.o: ../h/slu.h
ttyutils.o: ../h/zsreg.h
ttywrite.o: ../h/conf.h
ttywrite.o: ../h/kernel.h
ttywrite.o: ../h/tty.h
ttywrite.o: ../h/io.h
ttywrite.o: ../h/slu.h
udpecho.o: ../h/conf.h
udpecho.o: ../h/kernel.h
udpecho.o: ../h/network.h
udpnxtp.o: ../h/conf.h
udpnxtp.o: ../h/kernel.h
udpnxtp.o: ../h/network.h
udpsend.o: ../h/conf.h
udpsend.o: ../h/kernel.h
udpsend.o: ../h/network.h
unmount.o: ../h/conf.h
unmount.o: ../h/kernel.h
unmount.o: ../h/name.h
unsleep.o: ../h/conf.h
unsleep.o: ../h/kernel.h
unsleep.o: ../h/proc.h
unsleep.o: ../h/q.h
unsleep.o: ../h/sleep.h
userret.o: ../h/conf.h
userret.o: ../h/kernel.h
wait.o: ../h/conf.h
wait.o: ../h/kernel.h
wait.o: ../h/proc.h
wait.o: ../h/q.h
wait.o: ../h/sem.h
wakeup.o: ../h/conf.h
wakeup.o: ../h/kernel.h
wakeup.o: ../h/proc.h
wakeup.o: ../h/q.h
wakeup.o: ../h/sleep.h
write.o: ../h/conf.h
write.o: ../h/kernel.h
write.o: ../h/io.h
xdone.o: ../h/kernel.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
