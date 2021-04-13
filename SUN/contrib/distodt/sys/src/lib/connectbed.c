/* connectbed.c - Connect the user's terminal to a backend machine
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 22:02:03 1988
 */

#include <stdio.h>
#include <ctype.h>

#include "bed.h"
#include "odtd.h"

/*---------------------------------------------------------------------------
 * connectbed -
 *---------------------------------------------------------------------------
 */
int
connectbed( femachine, bemachine, class, type )
     char *femachine, *bemachine, *class;
     int type;
{
	int sock, devtcp;
	struct bm bm;
	int remoteport;
	struct genericreq * odtreq;	/* odt request structure	*/
	
	if ( obtainlock( femachine, bemachine, class ) < 0 ) {
		return(-1);
	}
	fprintf( stdout, ODT_DFLT_MSG );
	fprintf( stdout, "using front end '%s':    Using backend '%s'\n",
		femachine, bemachine );
	fflush( stdout );
	bm.ver = CURVER;
	sprintf( bm.cmd, "%d", type );
	odtreq = (struct genericreq *) bm.detail;
	if ( getuser( odtreq->uid ) < 0 ) {
		fprintf( stderr, "error: intruder alert\n" );
		return( -1 );
	}
	if ( gethostname( odtreq->hostid, MAXMACHINENAME ) != 0 ) {
		strcpy( odtreq->hostid, "dunno" );
		fprintf( stderr, "error: cannot get local host name\n" );
	}
	strcpy( odtreq->bename, bemachine );
	if ( ( sock = ConnectUdp( femachine, KNOWN_UDP_PORT ) ) < 0 ) {
		fprintf( stderr, "error: ConnectUdp failed to %s:%d\n",
			femachine, KNOWN_UDP_PORT );
		return( -1 );
	}
	if ( bmsend( sock, & bm, lbm, 0 ) < 0 ) {
		fprintf( stderr, "error: bad return from bmsend\n" );
		return( -1 );
	}
	if ( bmrecv( sock, & bm, lbm, 0 ) < 0 )  {
		fprintf( stderr, "error: bad return from bmrecv\n" );
		return( -1 );
	}
	if ( atoi( bm.cmd ) != RESP_OK ) {
		fprintf( stderr,"error: bad response (%s) from daemon: '%s'\n",
			bm.cmd, bm.detail );
		return( -1 );
	}
	remoteport = atoi( bm.detail );
	if ( ( devtcp = ConnectTcp( femachine, remoteport ) ) < 0 ) {
		fprintf( stderr, "error: ConnectTcp to %s:%d failed\n", 
			femachine, remoteport );
	}
	return( devtcp );
}
