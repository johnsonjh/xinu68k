#
#  Makefile for worm, a simple Xinu example
#  "make a.out" will make a standalone executable
#  "make worm.o" will make the object module which can be loaded into the shell
#
    
.SUFFIXES:.o .c .s
DIR	=	{Xinu-directory}
CFLAGS	=	-c -I../../../h
AFLAGS	=	-u $(DIR)/include/sys.s
SIZE	=	$(DIR)/bin/size11
PRINT	=	$(DIR)/bin/cprint
LORDER	=	$(DIR)/bin/lorder11
RANLIB	=	$(DIR)/bin/ranlib11
LIB	=	$(DIR)/lib
XINUDIR = 	$(DIR)/src/sys/sys
HEADERS	=	game.h

.c.o: $(HEADERS)
		$(CC) $(CFLAGS)	$<

GAMEFILES = main.o	\
            mesg.o	\
	    mover.o	\
	    plotter.o	\
	    referee.o 	\
	    robot.o	\
	    targeter.o	\
	    timedgetchar.o \
	    utils.o

worm.o:		$(GAMEFILES)
		$(LD) -r -o worm.o $(GAMEFILES)

install:	worm.o
		/bin/cp worm.o ..

a.out:		worm.o test.o
		$(LD) -e _start -N -X -T 00004000 $(XINUDIR)/xinu.o test.o worm.o $(XINUDIR)/libx.a $(LIB)/libxc.a $(XINUDIR)/libx.a -o b.out
		cp b.out pm.out
	        strip b.out;dd if=b.out of=a.out ibs=32 skip=1

