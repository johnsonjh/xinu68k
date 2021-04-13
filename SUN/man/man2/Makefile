#
#  Make Xinu Manual Pages - only FILES change between man[01234]
#

DIR	=	/usr/xinu7
MACROS	=	$(DIR)/lib/tmac.an

.SUFFIXES:	.ms .doc .1 .2 .3 .4

.ms.doc:
		nroff -ms $< > ,$* ; mv ,$* $*.doc

.1.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

.2.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

.3.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

.4.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

FILES	=	$(MACROS)	\
		Intro2.doc	\
		access.doc	\
		chprio.doc	\
		close.doc	\
		control.doc	\
		create.doc	\
		freebuf.doc	\
		freemem.doc	\
		freestk.doc	\
		getaddr.doc	\
		getbuf.doc	\
		getc.doc	\
		getmem.doc	\
		getname.doc	\
		getnet.doc	\
		getpid.doc	\
		getprio.doc	\
		gettime.doc	\
		getutim.doc	\
		ip2name.doc	\
		kill.doc	\
		mark.doc	\
		mkpool.doc	\
		mount.doc	\
		nammap.doc	\
		namrepl.doc	\
		open.doc	\
		panic.doc	\
		pcount.doc	\
		pcreate.doc	\
		pdelete.doc	\
		pinit.doc	\
		preceive.doc	\
		preset.doc	\
		psend.doc	\
		putc.doc	\
		read.doc	\
		receive.doc	\
		recvclr.doc	\
		recvtim.doc	\
		remove.doc	\
		resume.doc	\
		scount.doc	\
		screate.doc	\
		sdelete.doc	\
		seek.doc	\
		send.doc	\
		sendf.doc	\
		setdev.doc	\
		setnok.doc	\
		signal.doc	\
		sleep.doc	\
		sreset.doc	\
		suspend.doc	\
		unmount.doc	\
		unsleep.doc	\
		wait.doc	\
		write.doc

all:		$(FILES)

install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf *.doc core ,*

Intro1.doc:	Intro1.1
		nroff -man Intro1.1 > ,Intro1 ; mv ,Intro1 Intro1.doc

Intro2.doc:	Intro2.2
		nroff -man Intro2.2 > ,Intro2 ; mv ,Intro2 Intro2.doc

Intro3.doc:	Intro3.3
		nroff -man Intro3.3 > ,Intro3 ; mv ,Intro3 Intro3.doc

Intro4.doc:	Intro4.4
		nroff -man Intro4.4 > ,Intro4 ; mv ,Intro4 Intro4.doc

print:		listings

listings:	$(FILES)
		cat *.doc | sed '1,2d;s/^/	/' | lpr
