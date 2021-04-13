#include <conf.h>
#include <kernel.h>
#include <cpu.addrs.h>    
#include <interreg.h>
#include <clock.h>

int proc2();
int proc3();

main ()
{
    int a,b,c;
    a=1;
    b=2;
    c=3;
    
    kprintf("\n\nHello world, Xinu lives\n\n");
    resume(create(proc2,1000,20,"proc2",2,1,2));
    resume(create(proc3,1000,20,"proc3",1,314159));

    p1(a,b,c);
}

p1(x,y,z)
{
    x=1;
    y=2;
    
    p2(x,y);
}
     

p2(a,b)
{
    while(TRUE) {
	sleep(1);
	kprintf("CLICK\n");
    }
}




proc2(x,y)
{
    recur(15);
}

recur(x)
{
    if (x > 0)
	recur(x-1);
    else
	while (TRUE) {
	    kprintf("KLOK\n");
	    sleep(1);
	}
}


proc3(x)
{
    sleep(1000);
}
