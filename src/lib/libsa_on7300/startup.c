#include <stdio.h>
#include "slu7201.h"

int errno;
c_startup()
{
	slu_init(CONSOLE); /* normal console */
        slu_init(TTYC); /* 6850 too--alternate console */
	*(short *)0x4e0000 = 0xff00;   /* turn off floppy drive */
	set_debug_traps();	/* not in libsa.a--debugging init */
	breakpoint();           /* not in libsa.a--run Tutor immediately */
	main();                 /* not really used if downloading program */
	breakpoint();		/* not in libsa.a--trap back to debugging */
}

