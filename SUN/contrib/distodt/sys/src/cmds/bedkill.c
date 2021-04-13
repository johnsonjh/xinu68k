/* 
 * bedkill.c - kill a backend daemon
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 15 09:40:43 1990
 *
 *		code stolen mostly from bedreboot
 *
 * Copyright (c) 1990 Patrick A. Muckelbauer
 */

#include <stdio.h>

#include "bed.h"

/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main ( csb, rgsb )
     int csb;
     char *rgsb[];
{
	int i;
	int sock;
	struct bm bm;

	if ( csb == 1 ) {
		fprintf( stderr, "usage: bedkill [frontend-machine]*\n" );
	}
	for ( i = 1; i < csb; i++ ) {
		bm.ver = CURVER;
		sprintf( bm.cmd, "%d", REQ_QUIT );
		strncpy( bm.detail, "", MAXDETAIL );

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
		printf( "killing front end %s\n", bm.detail );
	}
	exit( 0 );
}

