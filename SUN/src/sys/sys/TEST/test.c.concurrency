#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <io.h>

main ()
{
	int A(), B();
        int pidA, pidB;

	kprintf("\n\nHello world, Xinu lives\n\n");

        resume(pidA = create(A, 500, 19, "cons", 0));
	resume(pidB = create(B, 500, 19, "prod", 0));

	sleep10(100);
	kprintf("\n\nHi Honey, I'm home!!!\n");
	kill(pidA);
	kill(pidB);
}

A()
{
    int n;
	
    while (TRUE) {
	for(n=0;n<20000; ++n);
	kprintf("A");
    }
}

B()
{
    int n;
    
    while (TRUE) {
	for(n=0;n<20000; ++n);
	kprintf("B");
    }
}
