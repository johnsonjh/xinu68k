/* bedreboot.c - This program reboots the specified backends
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 20:50:05 1988
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "bed.h"

/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main (csb, rgsb)
     int csb;
     char *rgsb[];
{
	int i;
	int sock;
	struct bm bm;

	if ( csb == 1 )  {
		fprintf( stderr, "usage: bedreboot [frontend-machine]*\n" );
		exit( 1 );
	}
	
	for ( i = 1; i < csb; i++ ) {
		bm.ver = CURVER;
		sprintf( bm.cmd, "%d", REQ_REBOOT );
		strncpy ( bm.detail, "", MAXDETAIL );
		
		if ( ( sock = ConnectUdp( rgsb[ i ], KNOWN_UDP_PORT ) ) < 0 ){
			fprintf( stderr,"error: ConnectUdp to %s:%d failed\n",
				rgsb[ i ], KNOWN_UDP_PORT );
			exit( 1 );
		}
		if ( bmsend( sock, &bm, lbm, 0 ) <= 0 ) {
			fprintf( stderr, "error: bmsend failed\n" );
			exit( 1 );
		}
		bmrecv( sock, &bm, lbm, 0 );
		printf( "rebooted front end %s: '%s'\n", rgsb[ i ],bm.detail);
	}
	exit( 0 );
}


