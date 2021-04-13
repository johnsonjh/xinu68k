/* naminit.c - naminit */

#include <conf.h>
#include <kernel.h>
#include <name.h>

#ifndef	RFILSYS
#define	RFILSYS	SYSERR
#endif
#ifdef	Nnsys
struct	nam	Nam;
#endif

/*------------------------------------------------------------------------
 *  naminit  -  initialize the syntactic namespace pseudo-device
 *------------------------------------------------------------------------
 */
naminit()
{
	Nam.nnames = 0;

	/* Xinu namespace definition */

	mount("",		NAMESPACE, "Xinu/storage/");
	mount("Xinu/",		RFILSYS,   "/usr/xinu7/");
	mount("h/",		NAMESPACE, "Xinu/src/sys/h/");
	mount("kernel/",	NAMESPACE, "Xinu/src/sys/sys/");
	mount("a.out",		NAMESPACE, "kernel/a.out");
	mount("pm.out",		NAMESPACE, "kernel/pm.out");
	mount("/dev/console",	CONSOLE,   NULLSTR);
	mount("/dev/null",	RFILSYS,   "/dev/null");
	mount("/dev/",		SYSERR,    NULLSTR);
	mount("/",		RFILSYS,   "/");
	mount("~/",		NAMESPACE, "Xinu/");
}
