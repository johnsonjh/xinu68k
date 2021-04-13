#
#  Makefile for the Distributed Odt/Download
#

# yes do purdue localisms 
SITE = -DPURDUE

# type of machine
MACHINETYPE = -DDEC

include ../Makefile.common

RANLIB = /usr/bin/ranlib

depend:
	/usr/local/bin/maketd $(DEFINES) $(INCLUDES) $(SRCDIR)/*/*.c

# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
# Dependencies generated at: Mon Nov 25 03:55:37 EST 1991

batchodt.o: ../../src/cmds/batchodt.c
bed.o: ../../includes/bed.h
bed.o: ../../includes/bedata.h
bed.o: ../../includes/bmutils.h
bed.o: ../../includes/netutils.h
bed.o: ../../includes/sitedep.h
bed.o: ../../includes/utils.h
bed.o: ../../src/bed/bed.c
bedkill.o: ../../includes/bed.h
bedkill.o: ../../includes/sitedep.h
bedkill.o: ../../src/cmds/bedkill.c
bedkillall.o: ../../includes/bed.h
bedkillall.o: ../../includes/netutils.h
bedkillall.o: ../../includes/sitedep.h
bedkillall.o: ../../includes/status.h
bedkillall.o: ../../src/cmds/bedkillall.c
bedlog.o: ../../includes/bed.h
bedlog.o: ../../includes/sitedep.h
bedlog.o: ../../includes/utils.h
bedlog.o: ../../src/bed/bedlog.c
bedreboot.o: ../../includes/bed.h
bedreboot.o: ../../includes/sitedep.h
bedreboot.o: ../../src/cmds/bedreboot.c
bmutils.o: ../../includes/bed.h
bmutils.o: ../../includes/sitedep.h
bmutils.o: ../../src/lib/bmutils.c
busers.o: ../../includes/bed.h
busers.o: ../../includes/sitedep.h
busers.o: ../../src/cmds/busers.c
connectbed.o: ../../includes/bed.h
connectbed.o: ../../includes/odtd.h
connectbed.o: ../../includes/sitedep.h
connectbed.o: ../../src/lib/connectbed.c
dload2tcp.o: ../../includes/bed.h
dload2tcp.o: ../../includes/bmutils.h
dload2tcp.o: ../../includes/downloadd.h
dload2tcp.o: ../../includes/netutils.h
dload2tcp.o: ../../includes/sitedep.h
dload2tcp.o: ../../src/cmds/dload2tcp.c
download.o: ../../includes/bed.h
download.o: ../../includes/downloadd.h
download.o: ../../includes/sitedep.h
download.o: ../../src/cmds/download.c
downloadd.o: ../../includes/bed.h
downloadd.o: ../../includes/bedata.h
downloadd.o: ../../includes/bmutils.h
downloadd.o: ../../includes/downloadd.h
downloadd.o: ../../includes/netutils.h
downloadd.o: ../../includes/sitedep.h
downloadd.o: ../../includes/utils.h
downloadd.o: ../../src/bed/downloadd.c
fusers.o: ../../includes/bed.h
fusers.o: ../../includes/sitedep.h
fusers.o: ../../src/cmds/fusers.c
getoptions.o: ../../includes/bed.h
getoptions.o: ../../includes/sitedep.h
getoptions.o: ../../src/lib/getoptions.c
getuser.o: ../../src/lib/getuser.c
idletime.o: ../../includes/bed.h
idletime.o: ../../includes/bedata.h
idletime.o: ../../includes/sitedep.h
idletime.o: ../../src/bed/idletime.c
lock.o: ../../includes/bed.h
lock.o: ../../includes/bedata.h
lock.o: ../../includes/sitedep.h
lock.o: ../../src/bed/lock.c
lockutils.o: ../../includes/bed.h
lockutils.o: ../../includes/sitedep.h
lockutils.o: ../../src/lib/lockutils.c
netutils.o: ../../includes/netutils.h
netutils.o: ../../src/lib/netutils.c
obtainlock.o: ../../includes/bed.h
obtainlock.o: ../../includes/sitedep.h
obtainlock.o: ../../includes/status.h
obtainlock.o: ../../src/lib/obtainlock.c
obtainstatus.o: ../../includes/bed.h
obtainstatus.o: ../../includes/bmutils.h
obtainstatus.o: ../../includes/netutils.h
obtainstatus.o: ../../includes/sitedep.h
obtainstatus.o: ../../includes/status.h
obtainstatus.o: ../../src/lib/obtainstatus.c
odt.o: ../../includes/bed.h
odt.o: ../../includes/sitedep.h
odt.o: ../../src/cmds/odt.c
odt2tcp.o: ../../includes/bed.h
odt2tcp.o: ../../includes/bmutils.h
odt2tcp.o: ../../includes/odtd.h
odt2tcp.o: ../../includes/sitedep.h
odt2tcp.o: ../../src/cmds/odt2tcp.c
odtd.o: ../../includes/baud.h
odtd.o: ../../includes/bed.h
odtd.o: ../../includes/bedata.h
odtd.o: ../../includes/bmutils.h
odtd.o: ../../includes/netutils.h
odtd.o: ../../includes/odtd.h
odtd.o: ../../includes/sitedep.h
odtd.o: ../../includes/utils.h
odtd.o: ../../src/bed/odtd.c
odtunlock.o: ../../includes/bed.h
odtunlock.o: ../../includes/sitedep.h
odtunlock.o: ../../src/cmds/odtunlock.c
printstatus.o: ../../includes/bed.h
printstatus.o: ../../includes/bedata.h
printstatus.o: ../../includes/sitedep.h
printstatus.o: ../../src/lib/printstatus.c
releaselock.o: ../../includes/bed.h
releaselock.o: ../../includes/bmutils.h
releaselock.o: ../../includes/sitedep.h
releaselock.o: ../../src/lib/releaselock.c
restart.o: ../../includes/bed.h
restart.o: ../../includes/sitedep.h
restart.o: ../../src/cmds/restart.c
status.o: ../../includes/bed.h
status.o: ../../includes/sitedep.h
status.o: ../../src/cmds/status.c
statusrecv.o: ../../includes/bed.h
statusrecv.o: ../../includes/netutils.h
statusrecv.o: ../../includes/sitedep.h
statusrecv.o: ../../src/lib/statusrecv.c
statusrequest.o: ../../includes/bed.h
statusrequest.o: ../../includes/bmutils.h
statusrequest.o: ../../includes/netutils.h
statusrequest.o: ../../includes/sitedep.h
statusrequest.o: ../../includes/status.h
statusrequest.o: ../../src/lib/statusrequest.c
support.o: ../../includes/bed.h
support.o: ../../includes/bedata.h
support.o: ../../includes/sitedep.h
support.o: ../../includes/tokens.h
support.o: ../../src/bed/support.c
sysstat.o: ../../src/bed/sysstat.c
ttymode.o: ../../src/lib/ttymode.c
utils.o: ../../includes/bed.h
utils.o: ../../includes/sitedep.h
utils.o: ../../includes/status.h
utils.o: ../../src/lib/utils.c
xmalloc.o: ../../src/lib/xmalloc.c

# DO NOT ADD ANYTHING HERE - WILL GO AWAY
