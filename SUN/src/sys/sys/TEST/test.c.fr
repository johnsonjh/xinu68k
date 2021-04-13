/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

/*----------------------------------------------------------------------
 *  main  --  test network initialization
 *----------------------------------------------------------------------
 */
main()
{
	struct	frame	*fptr;
	int	i, len, pr();
	char	ch;

	kprintf("Testing frame I/O - PLEASE USE 1 FOR MACHINE ID\n");
	kprintf("Net 0: frbpool=%d, machid=%d\n",
		frbpool, fdata[0].fmachid);
	fptr = getbuf(frbpool);
	kprintf("Type a message and I'll send it around the net: ");
	gets(fptr->frdata);
	fptr->frlen = strlen(fptr->frdata)+3;
	kprintf("Sending message of %d bytes: %s\n",
		getflen(fptr), fptr->frdata);
	kprintf("Fsend returns %d\n", fsend(0, 1, fptr) );
	kprintf("Buffer obtained at %o\n", fptr=getbuf(frbpool) );
	kprintf("Attempting to read message...\n");
	fptr = freceive(0);
	len = getflen(fptr);
	kprintf("Got %d bytes: ", len);
	for (i=0 ; i < len-3 ; i++)
		if ((ch=fptr->frdata[i]) < ' ')
			kprintf("^%c", ch|0100);
		else if (ch > 0177)
			kprintf("#%c", ch&0177 );
		else
			kprintf("%c",ch);
	kprintf("\n");
	kprintf("creating process to print incoming messages\n");
	resume( create(pr,200,19,"print",1,0) );
	kprintf("\nType a message at each prompt.\n");
	while(TRUE) {
		kprintf("Message to send: ");
		gets(fptr->frdata);
		if ( (i = strlen(fptr->frdata)+3) > 120 )
			i = 120;
		fptr->frlen = i;
		fsend(0,1,fptr);
		fptr = getbuf(frbpool);
	}
}
pr(netid)
int netid;
{
	struct	frame	*fptr;
	int	i;

	kprintf(" P: Starting with netid=%d\n",netid);
	while (TRUE) {
		fptr = freceive(netid);
		i = getflen(fptr);
		fptr->frdata[i-3] = '\0';
		kprintf(" P: received %s\n", fptr->frdata);
		freebuf(fptr);
	}
}

P()
{
	prdumph();
	qdumph();
}
