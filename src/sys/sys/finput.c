/* finput.c - finput, sendack, sendnack, sendsack */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

/*------------------------------------------------------------------------
 *  finput  -- read frames and enqueue for local machine or forwarding
 *------------------------------------------------------------------------
 */
PROCESS	finput(netid)
	int	netid;
{
	int	len;			/* length of actual data	*/
	int	to, from;		/* "to" and "from" addresses	*/
	struct	fglob	*fgptr;
	struct	frame	*fptr, *fptr2;
	char	*t, *f;			/* used to copy broadcast frames*/

	fgptr = &fdata[netid];
	fptr = getbuf(frbpool);
	for (fgptr->fiseq = fgptr->fifails = 0 ; TRUE ; ) {
		len = read(fgptr->findev, fptr, FRMAXLEN);
		if (len < FRMINLEN || len != getflen(fptr))
			sendnack(fgptr);
		else if ( (int)fptr->frseq != fgptr->fiseq)
			sendsack(fgptr);
		else {
			to = getfto(fptr->fraddr);
			from = getfrom(fptr->fraddr);
			if (to == FRBCAST) {	/* broadcast */
				if (from == fgptr->fmachid) {
					psend(fgptr->fiport, fptr);
					fptr = getbuf(frbpool);
				} else {
					fptr2 = getbuf(frbpool);
					for (f=fptr,t=fptr2; len-->0; )
						*t++ = *f++;
					psend(fgptr->fiport, fptr);
					psend(fgptr->ffport, fptr2);
					signal(fgptr->fosem);
					fptr = getbuf(frbpool);
				}
			} else if (to == fgptr->fmachid) {
				psend(fgptr->fiport, fptr);
				fptr = getbuf(frbpool);
			} else if (from == fgptr->fmachid) {
				printf("Can't deliver to %d\n", to);
			} else {
				psend(fgptr->ffport, fptr);
				signal(fgptr->fosem); /* bug fix to text */
				fptr = getbuf(frbpool);
			}
			sendack(fgptr);
		}
	}
}

/*------------------------------------------------------------------------
 *  sendack  --  send a positive acknowledgement with sequence number
 *------------------------------------------------------------------------
 */
LOCAL	sendack(fgptr)
	struct	fglob	*fgptr;
{
	fgptr->fifails = 0;
	if (++fgptr->fiseq > FRMAXSEQ)
		fgptr->fiseq = 0;
	putc(fgptr->findev, FRACK | fgptr->fiseq);
}

/*------------------------------------------------------------------------
 *  sendnack --  send a negative acknowledgement with sequence number
 *------------------------------------------------------------------------
 */
LOCAL	sendnack(fgptr)
	struct	fglob	*fgptr;
{
	putc(fgptr->findev, FRNACK | fgptr->fiseq);
}

/*------------------------------------------------------------------------
 *  sendsack --  send a negative acknowledgement and restart sequence
 *------------------------------------------------------------------------
 */
LOCAL	sendsack(fgptr)
	struct	fglob	*fgptr;
{
	if (++fgptr->fifails >= FRFAIL) {
		fgptr->fiseq = 0;
		fgptr->fifails = 0;
		putc(fgptr->findev, FRSACK);
	} else
		putc(fgptr->findev, FRNACK);
}
