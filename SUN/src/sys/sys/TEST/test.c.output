/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <network.h>

char buf1[1000], buf2[1000], buf3[1000], buf4[1000];
char *foob1, *foob2, *foob3, *foob4;

char strfoo[1000];
char *pchfoo;

/*------------------------------------------------------------------------
 *  main  --  start UDP echo server and then run Xinu pseudo-shell
 *------------------------------------------------------------------------
 */
main()
{
	int i,j;

	foob1 = buf1;
	foob2 = buf2;
	foob3 = buf3;
	foob4 = buf4;

	pchfoo = strfoo;

	printf("\n\n");
	for (i=0; i<5; ++i) {
/*disable();	  */
		sprintf(pchfoo,"%03d ",i);
		for (j=0; j<26; ++j)
		  sprintf(pchfoo + strlen(pchfoo),"%c",(char) ('a'+(char)j));
		sprintf(pchfoo + strlen(pchfoo),"  ");
		for (j=0; j<26; ++j)
		  sprintf(pchfoo + strlen(pchfoo),"%c",(char) ('A'+(char)j));
		sprintf(pchfoo + strlen(pchfoo),"\n");
/*restore();*/
	      	write(CONSOLE,pchfoo,strlen(pchfoo));
		pchfoo += strlen(pchfoo);
	}
	sleep(2);
	kprintf("\n\n==========================================\n");
	kprintf("strfoo contains:\n");
	kprintf("%s",strfoo);

	kprintf("\n\n==========================================\n");
	kprintf("Buf 1 contains:\n");
	*foob1 = '\00';
	kprintf("%s",buf1);

	kprintf("\n==========================================\n");
	kprintf("Buf 2 contains:\n");
	*foob2 = '\00';
	kprintf("%s",buf2);

	kprintf("\n==========================================\n");
	kprintf("Buf 3 contains:\n");
	*foob3 = '\00';
	kprintf("%s",buf3);

	kprintf("\n\n");

	ret_mon();
}

