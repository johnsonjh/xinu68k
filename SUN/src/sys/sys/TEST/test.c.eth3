/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <deqna.h>
#include <ether.h>
#include <ip.h>
#include <net.h>

/*------------------------------------------------------------------------
 *  main  --  fiddle around while Ethernet runs...
 *------------------------------------------------------------------------
 */
main()
{
	int	snd();

	resume(create(snd, 200, 20, "sender", 0));

	while(TRUE) {
		printf("\nAlive\n");
		sleep10(200);
	}
}

/*------------------------------------------------------------------------
 *  snd  -  send datagram just for kicks
 *------------------------------------------------------------------------
 */
snd()
{
	char	*ch;
	int	i, r;
	struct	epacket	*packet;

	sleep10(50);
	kprintf("Sending a datagram\n");
	packet = (struct epacket *) getbuf(netpool);
	ch = (char *) ( (struct udpip *)packet->ep_data)->u_data;
	for (i=0 ; i<10; i++)
		*ch++ = 'D';
	r = udpsend(netgate, 7/*ECHO*/, 50, packet, 10);
	kprintf("udpsend returns %d\n",r);
}
