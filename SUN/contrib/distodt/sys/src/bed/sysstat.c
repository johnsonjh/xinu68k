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

/* 
 *--------------------------------------------------------------------------- 
 * getloadaver(ldaver)
 *	This routine gets the 1, 5, and 15 minute load average of the
 *	system.
 *--------------------------------------------------------------------------- 
 */
void
getloadaver( ldaver )
     double ldaver[];
{
#ifndef DEC	/* handle later */
	
	int	kmem;
	int	i;
#if defined(VAX) || defined(SEQ) || defined(DEC) || defined(HPPA)
	double	avenrun[3];
#endif
#if defined(SUN)
	long	avenrun[3];
#endif
	
	ldaver[ 0 ] = 0.0;
	ldaver[ 1 ] = 0.0;
	ldaver[ 2 ] = 0.0;
	
	if ( ( kmem = open( "/dev/kmem", 0 ) ) < 0 ) {
		Log( "getloadaver: No /dev/kmem" );
		return;
	}
	nlist( "/vmunix", nl );
	if ( nl[ X_AVENRUN ].n_type == 0 ) {
		Log( "getloadaver: No namelist" );
		close( kmem );
		return;
	}
	
	/*
	 * Find 1, 5, and 15 minute load averages.
	 * (Found by looking in kernel for avenrun).
	 */
	lseek( kmem, (long) nl[ X_AVENRUN ].n_value, 0 );
	read( kmem, avenrun, sizeof( avenrun ) );
	for ( i = 0; i < ( sizeof( avenrun )/sizeof( avenrun[ 0 ] ) ); i++ ) {
		
#if defined(SEQ) || defined(VAX) || defined(DEC) || defined(HPPA)
		ldaver[ i ] = avenrun[ i ];
#endif
#if defined(SUN)
		ldaver[ i ] = (double) avenrun[ i ]/FSCALE;
#endif
		
	}
	close( kmem );

#else
	
	ldaver[ 0 ] = 0.0;
	ldaver[ 1 ] = 0.0;
	ldaver[ 2 ] = 0.0;

#endif
	
}


/*---------------------------------------------------------------------------
 * getnumusers - This gets the number of users from the front end
 *---------------------------------------------------------------------------
 */
int
getnumusers()
{
	int nusers;
	FILE * ut;
	struct utmp utmp;
	
	ut = fopen( "/etc/utmp", "r" );
	if ( ut == (FILE *) NULL ) {
		Log( "getnumusers: error opening /etc/utmp" );
		return( 0 );
	}
	
	nusers = 0;
	while ( fread( &utmp, sizeof( utmp ), 1, ut ) ) {
#if defined(SEQ) || defined(VAX) || defined(DEC) || defined(HPPA)
		if ( utmp.ut_name[ 0] != '\0' )  {
			nusers++;
		}
#endif
#if defined(SUN)
		if ( utmp.ut_name[ 0 ] != '\0' && !nonuser( utmp ) )  {
			nusers++;
		}
#endif
	}
	fclose( ut );
	return( nusers );
}

