#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include "game.h"    
    
plotchar(x,y,ch)
int x;
int y;
int ch;
{
    printf(CM,y,x);
    printf("%c",(char) ch);
}

terminit()
{
    control(CONSOLE,TCMODEK);
    control(CONSOLE,TCNOECHO);
    tty[devtab[CONSOLE].dvminor].ocrlf = 0;
}

termrepair()
{
    control(CONSOLE,TCMODEC);
    control(CONSOLE,TCECHO);
    tty[devtab[CONSOLE].dvminor].ocrlf = 1;
}


clearscreen()
{
    printf(CLEARSCREEN);
}


printstring(string, x, y)
     char *string;
     int x,y;
{
    while (*string != '\00') {
        newsend(pidplotter,3,x++,y,*string);
	++string;
    }
}






