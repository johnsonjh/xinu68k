/* ethcntl.c - ethcntl */

#include <conf.h>
#include <kernel.h>
#include <lereg.h>
#include <ether.h>

/*------------------------------------------------------------------------
 *  ethcntl  -  control the ethernet device by setting modes
 *------------------------------------------------------------------------
 */
ethcntl(devptr, func)
struct	devsw	*devptr;
int func;
{
        struct	etblk	      *etptr;
        struct	le_init_block *ib;
	
        etptr = &eth[devptr->dvminor];
        ib    = etptr->etib;
	
	switch ( func )	{
	case ETHPROMON:			/* turn on promiscuous mode	*/
	        ib->ib_prom = 1;
	        return ethstrt(etptr);
		break;
	case ETHPROMOFF:		/* turn off promiscuous mode	*/
	        ib->ib_prom = 0;
	        return ethstrt(etptr);
		break;
	default:
		return(SYSERR);
	}
	return(OK);
}
