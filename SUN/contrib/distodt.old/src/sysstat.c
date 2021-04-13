/* 
 * sysstat.c - These routines gets the system load average and num users
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Oct  7 1987
 * Copyright (c) 1987 Jim Griffioen/Raj Yavatkar
 *
 * Revised: June 1988 - revised to work with backend daemon program (bed)
 */


#include <sys/param.h>
#include <nlist.h>
#include <stdio.h>
#include <ctype.h>
#include <utmp.h>
#ifndef NULL				/* don't double define stuff	*/
#include <stdio.h>
#endif


struct	nlist nl[] = {
	{ "_proc" },
#define	X_PROC		0
	{ "_swapdev" },
#define	X_SWAPDEV	1
	{ "_Usrptmap" },
#define	X_USRPTMA	2
	{ "_usrpt" },
#define	X_USRPT		3
	{ "_nswap" },
#define	X_NSWAP		4
	{ "_avenrun" },
#define	X_AVENRUN	5
	{ "_boottime" },
#define	X_BOOTTIME	6
	{ "_nproc" },
#define	X_NPROC		7
	{ "_dmmin" },
#define	X_DMMIN		8
	{ "_dmmax" },
#define	X_DMMAX		9
	{ "" },
};

FILE	*ut;
struct utmp utmp;


/* 
 *--------------------------------------------------------------------------- 
 * getloadaver(ldaver)
 *	This routine gets the 1, 5, and 15 minute load average of the
 *	system.
 *--------------------------------------------------------------------------- 
*/ 
getloadaver(ldaver)
double ldaver[];
{
	int	kmem;
	int	i;
#ifdef VAX				/* vax specific */
	double	avenrun[3];
#else					/* sun specific */
	long	avenrun[3];
#endif

	ldaver[0] = 0.0;
	ldaver[1] = 0.0;
	ldaver[2] = 0.0;
	if ((kmem = open("/dev/kmem", 0)) < 0) {
		fprintf(stderr, "No kmem\n");
		return;
	}
	nlist("/vmunix", nl);
	if (nl[0].n_type==0) {
		fprintf(stderr, "No namelist\n");
		return;
	}


	/*
	 * Find 1, 5, and 15 minute load averages.
	 * (Found by looking in kernel for avenrun).
	 */
	lseek(kmem, (long)nl[X_AVENRUN].n_value, 0);
	read(kmem, avenrun, sizeof(avenrun));
	for (i = 0; i < (sizeof(avenrun)/sizeof(avenrun[0])); i++) {
#ifdef VAX				/* vax specific */
		ldaver[i] = avenrun[i];
#else					/* sun specific */
		ldaver[i] = (double)avenrun[i]/FSCALE;
#endif
	}
	close(kmem);
}


/*---------------------------------------------------------------------------
 * getnumusers - This gets the number of users from the front end
 *---------------------------------------------------------------------------
 */
getnumusers()
{
	int nusers;
	
	ut = fopen("/etc/utmp","r");

	nusers = 0;
	/* Print number of users logged in to system */
	while (fread(&utmp, sizeof(utmp), 1, ut)) {
#ifdef VAX				/* vax specific */
		if (utmp.ut_name[0] != '\0')
#else					/* sun specific */
		if (utmp.ut_name[0] != '\0'&& !nonuser(utmp))
#endif
			nusers++;
	}
	fclose(ut);
	return(nusers);
}
