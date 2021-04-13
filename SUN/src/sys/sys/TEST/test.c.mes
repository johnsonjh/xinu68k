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
	    	irecv = create (receiver,12000, 19, "receiv", 1, i);
		if (irecv == SYSERR) {
			kprintf ("main: create failed\n");
			break;
			}
		else
			resume (irecv);
		isend = create (sender, 12000, 19, "sender", 2, i, irecv);
		if (isend != SYSERR)
			resume (isend);
		else {
			kprintf ("main: create failed\n");
			break;
			}
		}
}

prhello()
{
	kprintf("hello world\n");
}

sender(isend, irecv)
int isend, irecv;
{
	kprintf ("sender #%d: about to send message %d\n", isend, irecv);
	send (irecv, isend);	/* send my partner its message */
}

receiver(irecv)
int irecv;
{
	int	mes = recvclr();

	kprintf ("receiver #%d: waiting for my message, using %s\n", irecv,
		  (irecv % 2)?"recvclr":"receive");
	while (mes != irecv) {
		if (irecv % 2) {
			mes = recvclr();
			sleep (0);
			}
		else
			mes = receive();
		}
	kprintf ("receiver #%d: got my message\n",irecv);
}
