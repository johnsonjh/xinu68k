#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <io.h>

#define NUM 5
#define LOOP 10000    

int pid[100];

Child(chint)
     int chint;
{
    while (TRUE) {
        int n;
	for(n=0;n<LOOP; ++n);	kprintf("%c",(char) chint);
    }
}



main ()
{
    int i;

    kprintf("\n\nHello world, Xinu lives\n\n");

    for (i=0; i<NUM; ++i) {
        resume(pid[i] = create(Child, 500, 19, "child", 1, (int) ('A' + i)));
    }

    sleep10(100);
    kprintf("\n\nHi Honey, I'm home!!!\n");

    for (i=0; i<NUM; ++i) {
        kill(pid[i]);
    }

}
