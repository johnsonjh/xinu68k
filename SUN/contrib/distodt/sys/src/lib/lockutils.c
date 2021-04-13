/* lockutils.c - 
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue May 24 22:10:01 1988
 */

#include <stdio.h>
#include <ctype.h>

#include "bed.h"

/*----------------------------------------------------------------------
 * requnlock - request unlock on remote machine 
 *----------------------------------------------------------------------
 */
int
requnlock( fename, bename, user )
     char * fename;			/* name of frontend with bename	*/
     char * bename;			/* name of backend desired	*/
     char * user;
{
	int i;
	struct bm bm;
	struct lockreq *lreq;
	char * sb;
	char buf[ MAXMACHINENAME ];
	int sock;
	
	/* make lock request packet	*/
	bm.ver = CURVER;
	sprintf( bm.cmd, "%d", REQ_UNLOCK );
	lreq = (struct lockreq *) bm.detail;
	
	strcpy( lreq->uid, user );
	if ( gethostname( buf, MAXMACHINENAME ) != 0 ) {
		strcpy( buf, "dunno" );
	}
	strcpy( lreq->hostid, buf );
	strcpy( lreq->bename, bename );
	
	if ( ( sock = ConnectUdp( fename, KNOWN_UDP_PORT ) ) < 0 ) {
		return( -1 );
	}
	if ( bmsend( sock, & bm, lbm, 0 ) <= 0 ) {
		return( -1 );
	}
	if ( bmrecv( sock, & bm, lbm, 0 ) <= 0 ) {
		return( -1 );
	}
	if ( atoi( bm.cmd ) != RESP_OK ) {
		return(-1);
	}
	else {
		return( 1 );
	}
}


/*----------------------------------------------------------------------
 * reqlock - request lock on remote machine 
 *----------------------------------------------------------------------
 */
int
reqlock( fename, bename, user )
     char * fename;			/* name of frontend with bename	*/
     char * bename;			/* name of backend desired	*/
     char * user;
{
	int i;
	struct bm bm;
	struct lockreq *lreq;
	char *sb;
	char buf[ MAXMACHINENAME ];
	int sock;
	
	/* make lock request packet	*/
	bm.ver = CURVER;
	sprintf( bm.cmd, "%d", REQ_LOCK );
	lreq = (struct lockreq *) bm.detail;
	
	strcpy( lreq->uid, user );
	if ( gethostname( buf, MAXMACHINENAME ) != 0 ) {
		strcpy( buf, "dunno" );
	}
	strcpy( lreq->hostid, buf );
	strcpy( lreq->bename, bename );
	
	if ( ( sock = ConnectUdp( fename, KNOWN_UDP_PORT ) ) <= 0 ) {
		return( -1 );
	}
	if ( bmsend( sock, & bm, lbm, 0 ) <= 0 ) {
		return( -1 );
	}
	if ( bmrecv( sock, & bm, lbm, 0 ) <= 0 ) {
		return( -1 );
	}
	
	if ( atoi( bm.cmd ) != RESP_OK ) {
		return( -1 );
	}
	else {
		return( 1 );
	}
}
