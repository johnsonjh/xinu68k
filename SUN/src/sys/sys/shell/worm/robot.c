/* robot.c -- robot */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>
#include "game.h"

int robotx;
int roboty;

robot()
{
    int oldx, oldy;
    int direc;
    int diffx;
    int diffy;
    
    robotx = rand()%(XMAXFIELD-3)+2;
    roboty = rand()%(YMAXFIELD-3)+2;
    
    direc = EAST;

    oldx=robotx;
    oldy=roboty;

    while (TRUE) {
	sleep10(1);
	diffx = targetx-robotx;
	diffy = targety-roboty;
	if (diffy<0) {			/* NORTH */
	    if (diffx>0)
		direc = NE;
	    else if (diffx<0)
		direc = NW;
	    else
		direc = NORTH;
	}
	
	else if (diffy>0) {		/* SOUTH */
	    if (diffx>0)
		direc = SE;
	    else if (diffx<0)
		direc = SW;
	    else
		direc = SOUTH;
	}
	else {				/* HORIZONTAL */
	    if (diffx>0)
		direc = EAST;
	    else if (diffx<0)
		direc = WEST;
	    else
		direc = SIT;
	}

	switch (direc) {
	  case SIT:	continue;
	  case NORTH:	if (roboty > 1)     --roboty; break;
	  case SOUTH:	if (roboty < YMAXFIELD)  ++roboty; break;
	  case WEST:	if (robotx > 1)     --robotx; break;
	  case EAST:	if (robotx < XMAXFIELD)  ++robotx; break;
	  case NE:	if (roboty>1 && robotx<XMAXFIELD) {
	                    --roboty; ++robotx;  }    break;
	  case NW:	if (roboty>1 && robotx>1) {
	                    --roboty; --robotx;  }    break;
	  case SE:	if (roboty<YMAXFIELD && robotx<XMAXFIELD) {
	                    ++roboty; ++robotx;  }    break;
	  case SW:	if (roboty<YMAXFIELD && robotx>1) {
	                    ++roboty; --robotx;  }    break;
	}
	newsend(pidplotter,3,oldx,oldy,' ');
	newsend(pidplotter,3,robotx,roboty,'R');
	oldx=robotx;
	oldy=roboty;
    }
}



