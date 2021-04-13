#include <conf.h>
#include <kernel.h>
#include <mem.h>
#include <io.h>

int noint;

int n;

main ()
{

	int prod2(), cons2();
	int produced, consumed;

	kprintf ("\n\n\n        Happy Birthday Doug!!!\n\n\n\n");


	consumed = screate(1);
	produced = screate(0);

	resume(create(cons2, 2000, 20, "cons", 2, consumed, produced));
	resume(create(prod2, 2000, 20, "prod", 2, consumed, produced));
	chprio(getpid(),19);
	printcake();
}

prod2(consumed, produced)
{
    int i;
    for(i=1; i<=38; ++i) {
	wait(consumed);
	++n;
	kprintf("Shawn and Steve light candle %d\n",n);
	signal(produced);
    }
}

cons2(consumed, produced)
{
     int i;
     for(i=1; i<=38; ++i) {
	 wait(produced);
	 kprintf("     Doug blows out candle %d \n",n);
	 signal(consumed);
     }
 }


printcake()
{

kprintf("\n");
kprintf("                                *\n");
kprintf("                        *       #       *\n");
kprintf("                *       #    ___#___    #       *    \n");
kprintf("                #   *___#---/   #   \\---#___*   #    \n");
kprintf("                #---#   #   *       *   #   #---#    \n");
kprintf("               /#   #       #       #       #   #\\    \n");
kprintf("              |\\    #       #       #       #    /|    \n");
kprintf("              | ---___      #       #      ___--- |    \n");
kprintf("              |       -----___________-----       |    \n");
kprintf("              |                                   |    \n");
kprintf("              |\\          Happy Birthday!        /|    \n");
kprintf("              | ---___                     ___--- |    \n");
kprintf("              |       -----___________-----       |    \n");
kprintf("              |                                   |    \n");
kprintf("               \\                                 /     \n");
kprintf("                ---___                     ___---      \n");
kprintf("                      -----___________-----            \n");
kprintf("                                                       \n");
kprintf("    \n");
}



