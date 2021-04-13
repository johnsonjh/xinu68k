#
#  Make the post mortum diagnostic program
#

DIR	=	/u5/xinu/xinu7.sun3
HDIR	=	$(DIR)/src/sys/h
DESTDIR =	.
CPP	=	/lib/cpp
CFLAGS	=	-O -DSUN3
LIB	=	../lib.a
CFILES	=	pm.c setup.c sun3.c trace.c connectsun.c	
OFILES	=	pm.o setup.o sun3.o trace.o connectsun.o 	\
		../obtainlock.o ../releaselock.o ../obtainstatus.o 
XHFILES	=	$(DIR)/include/a.out.h 	\
		$(HDIR)/conf.h		\
		$(HDIR)/kernel.h	\
		$(HDIR)/q.h		\
		$(HDIR)/proc.h		\
		$(HDIR)/sem.h		\
		$(HDIR)/tty.h


all:		pm

install:	all
		chmod 755 pm
		cp pm $(DESTDIR)

clean:
		rm -f core pm *.o ,* x.h

x.h:		$(XHFILES)
		sed '/#define	NULL	/d;s/^#/+%+/' $(XHFILES) >,x
		$(CPP) -UNULL ,x | \
		sed 's/^+%+/#/;/^# /d;/^[ 	]*$$/d' > x.h
		rm ,x

pm:		x.h $(OFILES)
		$(CC) $(CFLAGS) -o pm $(OFILES) $(LIB)

depend:
		grep '^#include' $(CFILES) pm.h \
		| sed -e '/<.*>/d' -e 's/:[^"]*"\([^"]*\)".*/: \1/' \
		| sed 's/\.c/.o/' >makedep
		echo '/^# DO NOT DELETE THIS LINE/+2,$$d' >eddep
		echo '$$r makedep' >>eddep
		echo 'w' >>eddep
		cp Makefile Makefile.bak
		ed - Makefile < eddep
		rm eddep makedep
		echo '# DEPENDENCIES MUST END AT END OF FILE' >> Makefile
		echo '# IF YOU PUT STUFF HERE IT WILL GO AWAY' >> Makefile
		echo '# see make depend above' >> Makefile

# DO NOT DELETE THIS LINE
# DEPENDENCIES MUST END AT END OF FILE
pm.o: pm.h
setup.o: pm.h
sun3.o: pm.h
trace.o: pm.h
connectsun.o: ../../h/utils.h
connectsun.o: ../../h/bed.h
connectsun.o: ../../h/bmutils.h
connectsun.o: ../../h/netutils.h
pm.h: x.h
# DEPENDENCIES MUST END AT END OF FILE
# IF YOU PUT STUFF HERE IT WILL GO AWAY
# see make depend above
