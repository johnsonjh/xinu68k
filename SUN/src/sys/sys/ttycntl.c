/* ttycntl.c - ttycntl */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>
#include <slu.h>

/*------------------------------------------------------------------------
 *  ttycntl  -  control a tty device by setting modes
 *------------------------------------------------------------------------
 */
ttycntl(devptr, func)
struct	devsw	*devptr;
int func;
{
	STATWORD ps;    
	register struct	tty *ttyp;
	char	ch;
	
	ttyp = &tty[devptr->dvminor];
	switch ( func )	{
	case TCSETBRK:
	        return(SYSERR);
		/* ORIG: ttyp->ioaddr->ctstat |= SLUTBREAK; */
		break;
	case TCRSTBRK:
		return(SYSERR);
		/* ORIG: ttyp->ioaddr->ctstat &= ~SLUTBREAK; */
		break;
	case TCNEXTC:
		disable(ps);
		wait(ttyp->isem);
		ch = ttyp->ibuff[ttyp->itail];
		restore(ps);
		signal(ttyp->isem);
		return(ch);
	case TCMODER:
		ttyp->imode = IMRAW;
		break;
	case TCMODEC:
		ttyp->imode = IMCOOKED;
		break;
	case TCMODEK:
		ttyp->imode = IMCBREAK;
		break;
	case TCECHO:
		ttyp->iecho = TRUE;
		break;
	case TCNOECHO:
		ttyp->iecho = FALSE;
		break;
	case TCICHARS:
		return(ttyp->icnt);
	case TCINT:
		ttyp->iintr = TRUE;
		ttyp->iintpid = getpid();
		break;
	case TCNOINT:
		ttyp->iintr = FALSE;
		break;
	default:
		return(SYSERR);
	}
	return(OK);
}
