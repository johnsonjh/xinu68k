/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <frame.h>

/*------------------------------------------------------------------------
 *  main  --  test frame I/O
 *------------------------------------------------------------------------
 */
main()
{
	struct	frame	*fptr;
	int	i, len, pr();
	char	ch;

	kprintf("\nTesting frames - PLEASE USE 1 FOR MACHINE ID\n");
	fptr = getbuf(frbpool);
	kprintf("Creating a process to print incoming messages\n");
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

	kprintf("\n P: Starting with netid=%d\n",netid);
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
