/* fusers.c - This program list the status of the front end machine
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu May 26 23:07:00 1988
 */

#include <stdio.h>

#include "bed.h"

int errno;	       		/* error number when system err	*/

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
	char verbose;
	struct bm bm;
	int fd_sock;
	char * option;
	
	verbose = 0;
	setupopt( csb, rgsb );
	for ( option = nextopt(); option != NULL; option = nextopt() ) {
		if ( strequ( option, "-v" ) ) {
			verbose = 1;
		}
		else {
			printusage("error: unexpected argument '%s'\n",option);
		}
	}
	if ( ( option = nextword() ) != NULL ) {
		printusage( "error: unexpected argument '%s'\n", option );
	}

	if ( verbose ) {
		printf( "Issuing broadcast message\n" );
	}
	if ( ( fd_sock = statusrequest() ) < 0 ) {
		exit( 1 );
	}
	while ( TRUE ) {
		if ( statusrecv( fd_sock, &bm ) <= 0 ) {
			break;
		}
		printfusers( (struct statresp *) bm.detail );
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
	fprintf( stderr, "usage: fusers [-v]\n" );
	exit( 1 );
}
	
