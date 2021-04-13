#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>
#include "game.h"

int wormx, wormy;

mover()
{
    int oldx, oldy;
    int direc;

    direc = EAST;
    wormx=2;
    wormy=10;
    oldx=wormx;
    oldy=wormy;

    newsend(pidplotter,3,wormx,wormy,'#');

    while (TRUE) {
	switch (timedgetchar(10)) {
	  case NORTH:	direc = NORTH; break;
	  case SOUTH:	direc = SOUTH; break;
	  case EAST:	direc = EAST; break;
	  case WEST:	direc = WEST; break;
	  case NE:	direc = NE; break;
	  case NW:	direc = NW; break;
	  case SE:	direc = SE; break;
	  case SW:	direc = SW; break;
	  case SIT:	direc = SIT; break;
	  case QUIT:	send(pidmain,0); return;
	}
	switch (direc) {
	  case SIT:	continue;
	  case NORTH:	if (wormy > 1)     --wormy; break;
	  case SOUTH:	if (wormy < YMAXFIELD)  ++wormy; break;
	  case WEST:	if (wormx > 1)     --wormx; break;
	  case EAST:	if (wormx < XMAXFIELD)  ++wormx; break;
	  case NE:	if (wormy>1 && wormx<XMAXFIELD) {
	                    --wormy; ++wormx;  }    break;
	  case NW:	if (wormy>1 && wormx>1) {
	                    --wormy; --wormx;  }    break;
	  case SE:	if (wormy<YMAXFIELD && wormx<XMAXFIELD) {
	                    ++wormy; ++wormx;  }    break;
	  case SW:	if (wormy<YMAXFIELD && wormx>1) {
	                    ++wormy; --wormx;  }    break;
	}
	newsend(pidplotter,3,oldx,oldy,' ');
	newsend(pidplotter,3,wormx,wormy,'#');
	oldx=wormx;
	oldy=wormy;
    }
}



