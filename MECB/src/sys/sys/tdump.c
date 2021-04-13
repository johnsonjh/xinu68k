/* tdump.c - tdump, tdumph, tdump1 */

#include <conf.h>
#include <kernel.h>
#include <tty.h>

/*
 * routines to dump contents of tty control block
 */
static	char	*unctrl[]	= {	/* unctrl codes for ttys	*/
	"^@", "^A", "^B", "^C", "^D", "^E", "^F", "^G", "^H", "^I", "^J", "^K",
	"^L", "^M", "^N", "^O", "^P", "^Q", "^R", "^S", "^T", "^U", "^V", "^W",
	"^X", "^Y", "^Z", "^[", "^\\", "^]", "^~", "^_",
	" ", "!", "\"", "#", "$",  "%", "&", "'", "(", ")", "*", "+", ",", "-",
	".", "/", "0",  "1", "2",  "3", "4", "5", "6", "7", "8", "9", ":", ";",
	"<", "=", ">",  "?", "@",  "A", "B", "C", "D", "E", "F", "G", "H", "I",
	"J", "K", "L",  "M", "N",  "O", "P", "Q", "R", "S", "T", "U", "V", "W",
	"X", "Y", "Z",  "[", "\\", "]", "^", "_", "`", "a", "b", "c", "d", "e",
	"f", "g", "h",  "i", "j",  "k", "l", "m", "n", "o", "p", "q", "r", "s",
	"t", "u", "v",  "w", "x",  "y", "z", "{", "|", "}", "~", "^?"
};

/*------------------------------------------------------------------------
 *  tdump  --  dump the tty control blocks
 *------------------------------------------------------------------------
 */
tdump()
{
	int i;

	for (i=0 ; i<Ntty ; i++)
		tdump1(i);
}

/*------------------------------------------------------------------------
 *  tdumph  --  dump the tty control blocks and halt
 *------------------------------------------------------------------------
 */
tdumph()
{
	tdump();
	kprintf("\nDump complete -- type P to continue\n");
/*	halt();		*/
}

/*------------------------------------------------------------------------
 *  tdump1  --  dump one tty control block
 *------------------------------------------------------------------------
 */
tdump1(tnum)
int tnum;
{
        register struct tty *iptr;
	int len;
	int i;

        disable();
        iptr = &tty[tnum];
	kprintf("Tty %d CSR at 0x%x\n",tnum,iptr->ioaddr);
        kprintf("INPUT  -- tail at %d, head at %d, sem #%d = value %d\n",
		iptr->itail,iptr->ihead,iptr->isem,i=scount(iptr->isem));
	if (i<0)
		i=0;
	else if (i > IBUFLEN)
		i = IBUFLEN;
	tqdump(iptr->itail,i,IBUFLEN,iptr->ibuff);
        kprintf("OUTPUT -- tail at %d, head at %d, sem #%d = value %d\n",
		iptr->otail,iptr->ohead,iptr->osem,i=scount(iptr->osem));
	i = OBUFLEN - i;
	if (i < 0)
		i = 0;
	else if (i>OBUFLEN)
		i = OBUFLEN;
	tqdump(iptr->otail,i,OBUFLEN,iptr->obuff);
        restore();
}
static tqdump(start, len, maxlen, buff)
int start,len,maxlen;
char *buff;
{
	register int i;
        for ( i=start ; len>0 ; len-- ) {
                if ( buff[i]&0200 ) kprintf("M-");
                kprintf("%s",unctrl[buff[i]&0177]);
		if (++i >= maxlen)
			i = 0;
        }
	kprintf("\n");
}
