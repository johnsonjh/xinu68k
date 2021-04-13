/* frsend.c - _frsend */

#include <conf.h>
#include <kernel.h>
#include <frame.h>
#include <dlc.h>

/*------------------------------------------------------------------------
 *  _frsend  --  transmit one frame until receipt acknowledged
 *------------------------------------------------------------------------
 */
_frsend(fptr, fgptr)
	struct	frame	*fptr;
	struct	fglob	*fgptr;
{
	int	len;
	int	seq, nextseq;
	int	msg;		/* response from receiver or timeout	*/
	int	msgcode;	/* ack code without sequence number	*/

	len = getflen(fptr);
	fgptr->fofails = 0;
	fptr->frseq = seq = fgptr->foseq;
	nextseq = seq >= FRMAXSEQ ? 0 : seq+1;
	while ( TRUE ) {
		recvclr();
		write(fgptr->foutdev, fptr, len);

		/* schedule timeout message */

		fgptr->ftfuse = FRTIME;
		fgptr->ftpid = getpid();
		resume(fgptr->ftimproc);
		if ( (msg=receive()) != FRTMSG) {
			fgptr->ftfuse = -1;
			switch (msg & FRCODE) {

			case FRACK:
				if ( (msg&FRSEQ) == nextseq) {
					fgptr->foseq = nextseq;
					return;
				}
				break;

			case FRSACK:
				fptr->frseq = fgptr->foseq = 0;
				nextseq = 1;
				sleep10(1);
			}
		}
		sleep10(1);
	}
}
