#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>
#include "game.h"

    
int src;
int nargs;
int x;
int y;
int chint;

plotter()
{
    int i;
    
    while (TRUE) {
	nargs = 3;
	newrecv(&src, &nargs, &x, &y, &chint);
	plotchar(x,y,(char) chint);
    }
}

