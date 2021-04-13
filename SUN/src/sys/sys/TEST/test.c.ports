/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <mark.h>
#include <ports.h>

/*----------------------------------------------------------------------
 *  main  --  simple ports test
 *----------------------------------------------------------------------
 */
main()
{
	int	port1, port2, p();

	kprintf("Testing ports\n");
	kprintf("Pcreate(2) returns %d\n", port1=pcreate(2) );
	kprintf("Pcreate(3) returns %d\n", port2=pcreate(3) );
	kprintf("Psend(%d,100) = %d\n", port1, psend(port1,100) );
	kprintf("Preceive(%d) = %d\n", port1, preceive(port1) );
	kprintf("Psend(%d, 200) = %d\n", port1, psend(port1,200) );
	kprintf("Psend(%d, 300) = %d\n", port1, psend(port1,300) );
	kprintf("Psend(%d, 400) = %d\n", port2, psend(port2,400) );
	kprintf("Preceive(%d) = %d\n", port2, preceive(port2) );
	kprintf("Preceive(%d) = %d\n", port1, preceive(port1) );
	kprintf("Preceive(%d) = %d\n", port1, preceive(port1) );
	kprintf("Psend(%d, 500) = %d\n", port2, psend(port2,500) );
	kprintf("Psend(%d, 600) = %d\n", port2, psend(port2,600) );
	kprintf("Psend(%d, 700) = %d\n", port2, psend(port2,700) );
	kprintf("Creating process to read port %d\n", port2);
	resume(create(p,300,25,"ptest",1, port2));
	kprintf("Attempt to overflow port and cause wait\n");
	kprintf("Psend(%d, 800) = %d\n", port2, psend(port2,800) );
	kprintf("Psend(%d, 810) = %d\n", port2, psend(port2,810) );
	kprintf("Psend(%d, 820) = %d\n", port2, psend(port2,820) );
	kprintf("Psend(%d, 830) = %d\n", port2, psend(port2,830) );

}
p(port)
{
	int	msg;

	kprintf(" P:reading port %d every five seconds\n",port);
	for (msg=0 ; msg != 830 ; sleep10(50) )
	    kprintf(" P: preceive(%d) = %d\n",port,msg=preceive(port));
	kprintf("End of port test\n");
}
