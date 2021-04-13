#
#  Makefile for the Distributed Odt/Download
#

VAXBIN	=	../vaxbin
SUNBIN	=	../sunbin
SEQBIN	=	../sequentbin
VAXOPT	=	-DVAX
SUNOPT	=	-DSUN
SEQOPT	=	-DVAX

# ***** Change these next lines to SUN or VAX depending on which you want ****
BINDIR	=	${SUNBIN}
CFLAGS	=	-g -I../h ${SUNOPT}
# ***** End Changes *************

AR	=	/bin/ar
LIB	=	lib.a
BED	=	bed.o odtd.o downloadd.o prstatus.o scanner.o 	\
		y.tab.o support.o lock.o parsestatus.o sysstat.o
STATUS	=	status.o prstatus.o obtainstatus.o
ODT	=	odt.o obtainlock.o releaselock.o obtainstatus.o
DOWNLOAD=	download.o obtainlock.o releaselock.o obtainstatus.o
BEDKILL	=	bedkill.o
BEDREBOOT = 	bedreboot.o
ODTUNLOCK = 	odtunlock.o releaselock.o obtainstatus.o
BUSERS	=	busers.o prstatus.o obtainstatus.o
FUSERS	=	fusers.o prstatus.o obtainstatus.o
EXE	=	${BINDIR}/bed 					\
		${BINDIR}/status				\
		${BINDIR}/odt					\
		${BINDIR}/download				\
		${BINDIR}/bedkill				\
		${BINDIR}/bedreboot				\
		${BINDIR}/odtunlock				\
		${BINDIR}/busers				\
		${BINDIR}/fusers
LIBOBJ	=	netutils.o utils.o bmutils.o

all: ${LIB} ${EXE}

${BINDIR}/bed: ${BED} ${LIB}
	cc ${CFLAGS} ${BED} ${LIB} -ll -o ${BINDIR}/bed

${BINDIR}/status: ${STATUS} ${LIB}
	cc ${CFLAGS} ${STATUS} ${LIB} -o ${BINDIR}/status

${BINDIR}/odt: ${ODT} ${LIB} 
	cc ${CFLAGS} ${ODT} ${LIB} -o ${BINDIR}/odt

${BINDIR}/download: ${DOWNLOAD} ${LIB}
	cc ${CFLAGS} ${DOWNLOAD} ${LIB} -o ${BINDIR}/download

${BINDIR}/bedkill: ${BEDKILL} ${LIB} 
	cc ${CFLAGS} ${BEDKILL} ${LIB} -o ${BINDIR}/bedkill

${BINDIR}/bedreboot: ${BEDREBOOT} ${LIB} 
	cc ${CFLAGS} ${BEDREBOOT} ${LIB} -o ${BINDIR}/bedreboot

${BINDIR}/odtunlock: ${ODTUNLOCK} ${LIB}
	cc ${CFLAGS} ${ODTUNLOCK} ${LIB} -o ${BINDIR}/odtunlock

${BINDIR}/busers: ${BUSERS} ${LIB}
	cc ${CFLAGS} ${BUSERS} ${LIB} -o ${BINDIR}/busers

${BINDIR}/fusers: ${FUSERS} ${LIB}
	cc ${CFLAGS} ${FUSERS} ${LIB} -o ${BINDIR}/fusers

y.tab.c: parser.y
	yacc -d parser.y
	(cmp -s y.tab.h tokens.h || cp y.tab.h tokens.h)

${LIB}: ${LIBOBJ}
	/bin/rm -f  ${LIB}
	$(AR) cr ${LIB}	`lorder ${LIBOBJ} | tsort`
	ranlib ${LIB}

clean:
	rm *.o ${EXE}

semiclean:
	rm *.o

depend:
		grep '^#include "' *.c ../h/*.h			\
		| sed	-e 's/:[^"]*"\([^"]*\)".*/: \1/'	\
		| sed	's/\.c:/.o:/' >makedep
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
bed.o: ../h/utils.h
bed.o: ../h/bed.h
bed.o: ../h/bmutils.h
bed.o: ../h/netutils.h
bed.o: ../h/bedata.h
bedkill.o: ../h/utils.h
bedkill.o: ../h/bed.h
bedkill.o: ../h/bmutils.h
bedkill.o: ../h/netutils.h
bedkill.o: ../h/status.h
bedreboot.o: ../h/utils.h
bedreboot.o: ../h/bed.h
bedreboot.o: ../h/bmutils.h
bedreboot.o: ../h/netutils.h
bmutils.o: ../h/bed.h
bmutils.o: ../h/bmutils.h
busers.o: ../h/utils.h
busers.o: ../h/bed.h
busers.o: ../h/bmutils.h
busers.o: ../h/netutils.h
download.o: ../h/utils.h
download.o: ../h/bed.h
download.o: ../h/bmutils.h
download.o: ../h/netutils.h
download.o: ../h/downloadd.h
downloadd.o: ../h/utils.h
downloadd.o: ../h/bed.h
downloadd.o: ../h/bmutils.h
downloadd.o: ../h/netutils.h
downloadd.o: ../h/downloadd.h
downloadd.o: ../h/bedata.h
fusers.o: ../h/utils.h
fusers.o: ../h/bed.h
fusers.o: ../h/bmutils.h
fusers.o: ../h/netutils.h
lock.o: ../h/bed.h
netutils.o: ../h/bed.h
obtainlock.o: ../h/utils.h
obtainlock.o: ../h/bed.h
obtainlock.o: ../h/bmutils.h
obtainlock.o: ../h/netutils.h
obtainlock.o: ../h/status.h
obtainstatus.o: ../h/utils.h
obtainstatus.o: ../h/bed.h
obtainstatus.o: ../h/bmutils.h
obtainstatus.o: ../h/netutils.h
obtainstatus.o: ../h/status.h
odt.o: ../h/utils.h
odt.o: ../h/bed.h
odt.o: ../h/bmutils.h
odt.o: ../h/netutils.h
odtd.o: ../h/utils.h
odtd.o: ../h/bed.h
odtd.o: ../h/bmutils.h
odtd.o: ../h/netutils.h
odtd.o: ../h/bedata.h
odtunlock.o: ../h/utils.h
odtunlock.o: ../h/bed.h
odtunlock.o: ../h/bmutils.h
odtunlock.o: ../h/netutils.h
parsestatus.o: ../h/bed.h
prstatus.o: ../h/bed.h
releaselock.o: ../h/utils.h
releaselock.o: ../h/bed.h
releaselock.o: ../h/bmutils.h
releaselock.o: ../h/netutils.h
status.o: ../h/utils.h
status.o: ../h/bed.h
status.o: ../h/bmutils.h
status.o: ../h/netutils.h
support.o: ../h/bed.h
support.o: ../h/bedata.h
utils.o: ../h/utils.h
y.tab.o: ../h/bedata.h
#	DEPENDENCIES MUST END AT END OF	FILE
#	IF YOU PUT STUFF HERE IT WILL GO AWAY
#	see make depend	above
