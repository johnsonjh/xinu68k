#include <kernel.h>
#include <proc.h>
disable()
{
#ifdef	DEBUG
	dotrace("disable()",NULL,NULL);
#endif
	if (!(proctab[currpid].phasps)++)
		_disable(&(proctab[currpid].pstatreg));
}

restore()
{
#ifdef	DEBUG
	dotrace("restore()",NULL,NULL);
#endif
	if (--(proctab[currpid].phasps) == 0)
		_restore(&(proctab[currpid].pstatreg));
}
