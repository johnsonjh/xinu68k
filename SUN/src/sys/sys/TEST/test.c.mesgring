/* test.c - main */

#include <conf.h>
#include <kernel.h>

#define	CIRCUIT 1000

/*------------------------------------------------------------------------
 *  main  --  current test routines
 *------------------------------------------------------------------------
 */
main()
{
	int ctr, succpid;
	int ringnode(), startnode();

	succpid = getpid();
	for (ctr=0; ctr<NPROC-2; ctr++) {
		succpid = create (ringnode, 500, 19, "ringnode", 1, succpid);
		if (succpid != SYSERR)
			resume (succpid);
		else{
			kprintf("error creating ring node\n");
			break;
			}
		}
	chprio (getpid(), 19);
	startnode(succpid);
}

ringnode(succpid)
int succpid;
{
	int ctr, inval, predpid;

	/* send messages around ring CIRCUIT times */
	for (ctr = 0; ctr < CIRCUIT; ctr++) {
		inval = receive();
		while (send(succpid, inval) == SYSERR) {
			kprintf ("error sending\n");
			sleep(1);
			}
		}
	/* now each node tells its successor its pid */
	while (send(succpid, getpid()) == SYSERR)  {
		kprintf("error sending\n");
		sleep(1);
		}
	predpid = receive();
	/* switch directions and send messages around ring CIRCUIT times */
	for (ctr = 0; ctr < CIRCUIT; ctr++) {
		inval = receive();
		while (send(predpid, inval) == SYSERR)  {
			kprintf("error sending\n");
			sleep(1);
			}
		}
}

startnode(succpid)
int succpid;
{
	int ctr, inval, predpid;

	/* send messages through ring CIRCUIT times */
	kprintf("sending %d messages around ring\n", CIRCUIT);
	for (ctr = 0; ctr < CIRCUIT; ctr++) {
		while (send(succpid, ctr) == SYSERR) {
			kprintf ("error sending\n");
			sleep(1);
			}
		if ((inval = receive()) != ctr)
			kprintf ("bad message\n");
		}
	kprintf("reversing ring direction\n");
	/* now each node tells its successor its pid */
	while (send(succpid, getpid()) == SYSERR)  {
		kprintf("error sending\n");
		sleep(1);
		}
	predpid = receive(predpid);
	/* switch directions and send messages around ring CIRCUIT times */
	kprintf("sending %d messages around ring in reversed direction\n", CIRCUIT);
	for (ctr = 0; ctr < CIRCUIT; ctr++) {
		while (send(predpid, ctr) == SYSERR)  {
			kprintf("error sending\n");
			sleep(1);
			}
		if ((inval = receive()) != ctr)
			kprintf("bad message\n");
	}
	kprintf("done with message passing test\n");
}
