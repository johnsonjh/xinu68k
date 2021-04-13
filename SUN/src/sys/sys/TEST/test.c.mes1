/* test.c - main */

#include <conf.h>
#include <kernel.h>

/*------------------------------------------------------------------------
 *  main  --  current test routines
 *------------------------------------------------------------------------
 */
main()
{
	int prhello(), sender(), receiver();
	int i, isend, irecv;

	prhello();
	for (i=0; i<(NPROC-2)/2; i++) {
		isend = create (sender, 12000, 19, "sender", 1, i);
		if (isend != SYSERR)
			resume (isend);
		else {
			kprintf ("main: create failed\n");
			break;
			}
		resume (
	    	create (receiver,12000, 19, "receiv", 2, isend, i)
				);
		}
}

prhello()
{
	kprintf("hello world\n");
}

sender(ikey)
int ikey;
{
	int	irecvpid;

	irecvpid = receive();
	kprintf ("sender #%d: about to send message to %d\n", ikey, irecvpid);
	send (irecvpid, ikey);
	while (1)
		; /* hog cpu time */
}

receiver(isender, ikey)
int isender, ikey;
{
	int	mes = recvclr();

	if (send (isender, getpid()) == SYSERR)
		kprintf ("receiver: can't send\n");
	kprintf ("receiver #%d: waiting for my message, using recvtim\n", ikey);
	while (mes != ikey)  /* wait until my sender sends me the key */
		mes = recvtim(20); /* wait at most 2 seconds */
	kprintf ("receiver #%d: got my message, killing sender and dying\n",
		 ikey);
	kill (isender);	/* kill off my sender */
}
