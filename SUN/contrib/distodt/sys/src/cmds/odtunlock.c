/* odtunlock.c - unlock a backend machine locked by odt or download
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 20:07:53 1988
 */

#include <stdio.h>

#include "bed.h"

extern void setupopt();
extern char * nextopt();
extern char * nextarg();
extern char * nextword();

/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main (csb, rgsb)
int csb;
char *rgsb[];
{
	int i;				/* counter variable		*/
	char verbose;
	char removelock;
	char class[ MAXCLASS ];
	char bemachine[ MAXMACHINENAME ];
	char * option;
	
	verbose = 0;
	bemachine[ 0 ] = '\0';
	bemachine[ MAXMACHINENAME - 1 ] = '\0';
	class[ 0 ] = '\0';
	class[ MAXCLASS - 1 ] = '\0';
	removelock = 0;
	setupopt( csb, rgsb );
	for ( option = nextopt(); option != NULL; option = nextopt() ) {
		if ( strequ( option, "-v" ) ) {
			verbose = 1;
		}
		else if ( strequ( option, "-m" ) ) {
			if ( ( option = nextarg() ) == NULL ) {
				strcpy( bemachine, "-new" );
			}
			else {
				strncpy(bemachine, option, MAXMACHINENAME-1);
			}
		}
		else if ( strequ( option, "-c" ) ) {
			if ( ( option = nextarg() ) == NULL ) {
				printusage( "error: missing class name\n" );
			}
			strncpy( class, option, MAXCLASS - 1 );
		}
		else if ( strequ( option, "-r" ) ) {
			removelock = 1;
		}
		else {
			printusage("error: unexpected argument '%s'\n",option);
		}
	}
	if ( ( option = nextword() ) != NULL ) {
		printusage( "error: unexpected argument '%s'\n", option );
	}
	
	if ( releaselock( class, bemachine, removelock ) > 0 ) {
		printf( "released machine '%s'\n", bemachine );
	}
	else {
		exit( 1 );
	}
}

/*---------------------------------------------------------------------------
 * printusage
 *---------------------------------------------------------------------------
 */
printusage( sb, w1, w2 )
     char *sb;
     int w1, w2;
{
	fprintf( stderr, sb, w1, w2 );
	fprintf( stderr,
	   "usage: odtunlock [-r] [-c CLASS] [-m BACKENDMACHINE]\n");
	exit( 1 );
}


