#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>
#include "game.h"

extern int pidtimer;
int removetarget();

int targetvalue;

#define REMOVE -1

targeter()
{

    while (TRUE) {
	recvclr();
	targetx = (rand() % (XMAXFIELD-6)) + 2;
	targety = (rand() % (YMAXFIELD-6)) + 2;
	targetvalue=(rand() % 9)+1;
	newsend(pidplotter,3,targetx,targety,targetvalue+48 );
	resume(pidtimer = create(removetarget,200,13,"remover",0));
	if (receive() != REMOVE)
	    kill(pidtimer);
	else	
	    newsend(pidplotter,3,targetx,targety ,' ');
    }
}


removetarget()
{
    sleep(30);
    send(pidtargeter,REMOVE);
}
     



