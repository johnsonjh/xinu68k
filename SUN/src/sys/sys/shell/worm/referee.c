#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>
#include "game.h"

int scoreworm;
int numworm;

int scorerobot;
int numrobot;

#define WORM 1
#define ROBOT 0

#define SCOREFORMAT "%4d"

referee()
{
    scoreworm=numworm=scorerobot=numrobot=0;

    printstring("worm   targets:    0",10,YMAX-1);
    printstring("         score:    0",10,YMAX);
    printstring("robot  targets:    0",(int) (XMAX/2),YMAX-1);
    printstring("         score:    0",(int) (XMAX/2),YMAX);

    while (TRUE) {
	if ((wormx == targetx) && (wormy == targety)) {
	    numworm += 1;
	    scoreworm += targetvalue;
	    send(pidtargeter,0);
	    updatescore(WORM);
	}
	if ((robotx == targetx) && (roboty == targety)) {
	    numrobot += 1;
	    scorerobot += targetvalue;
	    send(pidtargeter,0);
	    updatescore(ROBOT);
	}
	else if ((wormx<=1) || (wormx >= XMAXFIELD)
		 || (wormy<=1) || (wormy>=YMAXFIELD)) {
	    send(pidmain,0);
	    return(0);
	}
    }
}

updatescore(whom)
     int whom;
{
    char buf[20];

    if (whom == WORM) {
	printstring(sprintf(buf,SCOREFORMAT,numworm),27,YMAX-1);
	printstring(sprintf(buf,SCOREFORMAT,scoreworm),27,YMAX);
    }
    else {
	printstring(sprintf(buf,SCOREFORMAT,numrobot),(int) (XMAX/2)+16,YMAX-1);
	printstring(sprintf(buf,SCOREFORMAT,scorerobot),(int) (XMAX/2)+16,YMAX);
    }
}
