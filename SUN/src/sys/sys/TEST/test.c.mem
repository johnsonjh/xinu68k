/* test.c - main */
#include <conf.h>

main ()
{
	int	*a,*b,*c,*d,*e,*f;

	mdump();
	kprintf("a=getmem(10) is 0%o\n", a=getmem(10));
	mdump();
	kprintf("b=getmem(20) is 0%o\n", b=getmem(20));
	mdump();
	kprintf("freemem(a,10) is %d\n", freemem(a,10));
	mdump();
	kprintf("c=getmem(20) is 0%o\n", c=getmem(20));
	mdump();
	kprintf("d=getmem(10) is 0%o\n", d=getmem(10));
	mdump();
	kprintf("freemem(b,20) is %d\n", freemem(b,20));
	mdump();
	kprintf("freemem(c,20) is %d\n", freemem(c,20));
	mdump();
	kprintf("freemem(d,10) is %d\n", freemem(d,10));
	mdump();
}
