/* rwho.c - rwho */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rwho.h>

/*------------------------------------------------------------------------
 *  rwho  -  Initialize rwho subsystem and start daemon processes
 *------------------------------------------------------------------------
 */
rwho()
{
	resume( create(RWIN,  RWISTK, RWIPRIO, RWINAM, RWIARGS) );
	resume( create(RWOUT, RWOSTK, RWOPRIO, RWONAM, RWOARGS) );
}

struct	rwinfo	Rwho;		/* all globals used by rwho subsystem	*/
