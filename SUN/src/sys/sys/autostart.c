/* autostart.c -- autostart */
#include <conf.h>    
#include <sunromvec.h>
#include <kernel.h>    

int fautostart;
int boottime   = TRUE;

/* experimental value, only approxiamte */
#define LOOPSPERSECOND 526300

/*
 * autostart - set the flag 'fautostart' to TRUE if the program should
 *    proceed, or false if it should return to monitor.  Decision
 *   made according to arg[1] on the boot command line:
 *          "-a" or "-a0" -- start now
 *          "-aN          -- start after about N seconds
 *          anything else -- return to monitor
 */
autostart()
{
    int delay;
    
    register struct bootparam *bp = *romp->v_bootparam;
    char *arg;

    if (! boottime) {
	fautostart = TRUE;
	return(0);
    }
    boottime = FALSE;
    fautostart = FALSE;
    
    arg = bp->bp_argv[1];

    if ( (arg != NULL) && (*arg != NULL) && (strncmp(arg,"-a",2) == 0)) {
	/* delay start */
	if (sscanf(arg,"-a%d",&delay) == 1)
	    for (delay *= LOOPSPERSECOND ;delay > 0; --delay) /* null */ ;
	
	fautostart = TRUE;
    }
}
