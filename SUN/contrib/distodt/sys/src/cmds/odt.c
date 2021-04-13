/* odt.c - Connect the user's terminal to a backend machine
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 22:02:03 1988
 */

#include <stdio.h>
#include <signal.h>

#include "bed.h"

int errno;	        		/* error number when system err	*/

#define ODT_DFLT_CH 0

extern void setupopt();
extern char * nextopt();
extern char * nextarg();
extern char * nextword();

/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main ( csb, rgsb )
     int csb;
     char *rgsb[];
{
	int  devtcp;
	char verbose = 0;
	unsigned char globalMask = 0xFF;
	char unlock = 0;
	char remove = 0;
	char class[ MAXCLASS ];
	char femachine[ MAXMACHINENAME ];
	char bemachine[ MAXMACHINENAME ];
	char * option;

	bemachine[ 0 ] = '\0';
	bemachine[ MAXMACHINENAME - 1 ] = '\0';
	class[ 0 ] = '\0';
	class[ MAXCLASS - 1 ] = '\0';
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
				strncpy( bemachine, option, MAXMACHINENAME-1 );
			}
		}
		else if ( strequ( option, "-c" ) ) {
			if ( ( option = nextarg() ) == NULL ) {
				printusage( "error: missing class name\n" );
			}
			strncpy( class, option, MAXCLASS - 1 );
		}
		else if ( strequ( option, "-7" ) ) {
			globalMask = 0x7F;
		}
		else if ( strequ( option, "-8" ) ) {
			globalMask = 0xFF;
		}
		else if ( strequ( option, "-u" ) ) {
			unlock = 1;
		}
		else if ( strequ( option, "-r" ) ) {
			remove = 1;
		}
		else {
			printusage("error: unexpected argument '%s'\n",option);
		}
	}
	if ( ( option = nextword() ) != NULL ) {
		printusage( "error: unexpected argument '%s'\n", option );
	}
	
	strcpy( femachine, "" );
	
	if ( verbose ) {
		fprintf( stdout, "connect to bemachine '%s', class '%s'\n",
			bemachine, class );
	}
	if ( ( devtcp = connectbed( femachine, bemachine, class,
				   REQ_ODT_CONNECT ) ) < 0 ) {
		exit( 1 );
	}
	if ( verbose ) {
		fprintf( stdout, "using TCP port %d\n", devtcp );
	}
	fflush( stdout );
	odt2tcp( stdin, stdout, devtcp, ODT_DFLT_CH, globalMask );
	if ( remove || unlock ) {
		bemachine[ 0 ] = '\0';
		class[ 0 ] = '\0';
		if ( releaselock( class, bemachine, remove ) > 0 ) {
			printf( "released machine '%s'\n", bemachine );
		}
		else {
			printf( "release failed\n" );
			exit( 1 );
		}
	}
	return( 0 );
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
		"usage: odt [-u] [-r] [-7] [-8] [-c  CLASS] [-m BACKENDMACHINE]\n" );
	exit( 1 );
}


