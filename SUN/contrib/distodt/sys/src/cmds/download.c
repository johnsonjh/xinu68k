/* download.c -  version 1 - copies a.out file to backend mach
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Mar  2 11:08:01 1988
 */

#include <stdio.h>

#include "bed.h"
#include "downloadd.h"

extern void setupopt();
extern char * nextopt();
extern char * nextarg();
extern char * nextword();

extern char * getenv();

/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main ( csb, rgsb )
     int csb;
     char *rgsb[];
{
	int devtcp;
	char verbose;
	char class[ MAXCLASS ];
	char femachine[ MAXMACHINENAME ];
	char bemachine[ MAXMACHINENAME ];
	char filename[ MAXFILENAME ];
	struct genericreq * dnreq;
	char * option;
	int rv;

	verbose = 0;
	bemachine[ 0 ] = '\0';
	bemachine[ MAXMACHINENAME - 1 ] = '\0';
	filename[ 0 ] = '\0';
	filename[ MAXFILENAME - 1 ] = '\0';
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
				strncpy(bemachine, option, MAXMACHINENAME-1);
			}
		}
		else if ( strequ( option, "-c" ) ) {
			if ( ( option = nextarg() ) == NULL ) {
				printusage( "error: missing class name\n" );
			}
			strncpy( class, option, MAXCLASS - 1 );
		}
		else {
			printusage("error: unexpected argument '%s'\n",option);
		}
	}
	if ( ( option = nextarg() ) != NULL ) {
		strncpy( filename, option, MAXFILENAME - 1 );
	}
	else {
		char * sb = getenv( ENVFILENAME );

		if ( sb != NULL ) {
			strncpy( filename, sb, MAXFILENAME - 1 );
		}
		else {
			strcpy( filename, "a.out" );
		}
	}
	if ( ( option = nextword() ) != NULL ) {
		printusage( "error: unexpected argument '%s'\n", option );
	}
	
	strcpy( femachine, "" );
	
	if ( verbose ) {
		printf( "connect to bemachine '%s', class '%s', filename '%s'\n", bemachine, class, filename );
	}
	
	if ( ( devtcp = connectbed( femachine, bemachine, class,
				   REQ_DNLD_CONNECT ) ) < 0 ){
		exit( 1 );
	}
	if ( verbose ) {
		printf( "permission to use the backend machine\n" );
	}
	
	printf( "downloading file: %s\n", filename );

	rv = dload2tcp( filename, devtcp );
	if ( rv == -1 ) {
		exit( 1 );
	}
	exit( 0 );
}


/*---------------------------------------------------------------------------
 * printusage -
 *---------------------------------------------------------------------------
 */
printusage( sb, w1, w2 )
     char *sb;
     int w1, w2;
{
	fprintf( stderr, sb, w1, w2 );
	fprintf( stderr,
	    "usage: download [-c  CLASS] [-m BACKENDMACHINE] [filename]\n" );
	exit( 1 );
}
	
