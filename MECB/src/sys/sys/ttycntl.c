/* ttycntl.c - ttycntl */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/* set up macros for appropriate SLU as spec. by SLUCHIP:
   if SLUCHIP==6850, #include slu6850.h, #define TTYCNTL ttycntl6850, etc.
   if SLUCHIP==7201, #include slu7201.h, #define TTYCNTL ttycntl7201, etc.
   if SLUCHIP undef.,#include slu.h, #define TTYCNTL ttycntl  (orig. case)
   similarly sluaccess.h, other TTY fns */
#include <sluinclude.h>

/*------------------------------------------------------------------------
 *  ttycntl  -  control a tty device by setting modes
 *------------------------------------------------------------------------
 */
TTYCNTL(devptr, func, addr)
	struct	devsw	*devptr;
	int func;
	char *addr;
{
	register struct	tty *ttyp;
	register struct csr *cptr;
	char	ch;

#ifdef	DEBUG
	dotrace("ttycntl", &devptr, 3);
#endif

	ttyp = &tty[devptr->dvminor];

	cptr = (struct csr *)devptr->dvcsr;
	switch ( func )	{
	case TCSETBRK:
		slusetbrk(cptr);
		break;
	case TCRSTBRK:
		slurstbrk(cptr);
		break;
	case TCNEXTC:
		disable();
		wait(ttyp->isem);
		ch = ttyp->ibuff[ttyp->itail];
		restore();
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
		return(scount(ttyp->isem));
	default:
		return(SYSERR);
	}
	return(OK);
}
