/* ttyread.c - ttyread, readcopy */

#include <conf.h>
#include <kernel.h>
#include <tty.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  ttyread - read one or more characters from a tty device
 *------------------------------------------------------------------------
 */
ttyread(devptr, buff, count)
struct	devsw	*devptr;
int count;
char *buff;
{
	STATWORD ps;    
	register struct	tty *iptr;
	int avail, nread;
	char ch, eofch;
	int	donow, dolater;

	if (count < 0)
		return(SYSERR);
	disable(ps);
	avail=(iptr= &tty[devptr->dvminor])->icnt;

	if (count == 0) {	/* read whatever is available */
		if (avail == 0) {
			restore(ps);
			return(0);
		}
		count = avail;
	}
	if (count < avail) {
		donow = count;
		dolater = 0;
	} else {
		donow = avail;
		dolater = count - avail;
	}
	nread = 0;
	if (donow > 0) {
		ch = iptr->ibuff[iptr->itail++];
		--iptr->icnt;
		if (iptr->itail >= IBUFLEN)
			iptr->itail = 0;
		if ( ((eofch=iptr->ieofc) == ch) && iptr->ieof) {
				sreset(iptr->isem, avail-1);
				restore(ps);
				return(EOF);
		}
		*buff++ = ch;
		for (nread=1 ; nread < donow ; ) {
			ch = iptr->ibuff[iptr->itail];
			if ( (ch==eofch) && iptr->ieof) {
				sreset(iptr->isem, avail - nread);
				restore(ps);
				return(nread);
			}
			*buff++ = ch;
			--iptr->icnt;
			if (++iptr->itail >= IBUFLEN)
				iptr->itail = 0;
			nread++;
			if (ch == NEWLINE || ch == RETURN) {
			        iptr->icnt = avail - nread;
				sreset(iptr->isem, avail - nread);
				restore(ps);
				return(nread);
			}
		}
		sreset(iptr->isem, avail - nread);
	}
	donow = nread;
	for (nread=0 ; nread < dolater ; ) {
		wait(iptr->isem);
		ch = iptr->ibuff[iptr->itail];
		if (ch == iptr->ieofc && iptr->ieof) {
			if (nread == 0 && donow == 0) {
			        --iptr->icnt;
				if (++iptr->itail >= IBUFLEN)
				iptr->itail = 0;
				restore(ps);
				return(EOF);
			}
			signal(iptr->isem);
			break;
		}
		*buff++ = ch;
		nread++;
		--iptr->icnt;
		if (++iptr->itail >= IBUFLEN)
			iptr->itail = 0;
		if (ch == NEWLINE || ch == RETURN)
			break;
	}
	restore(ps);
	return(donow + nread);
}
