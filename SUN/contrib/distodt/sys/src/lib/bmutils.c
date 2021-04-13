/* bmutils.c - Routines for sending and receiving bm structure (bed protocol)
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 20:53:41 1988
 */

#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "bed.h"

#define NULLCH '\00'    

/*---------------------------------------------------------------------------
 * bmsend - send a bm struct in a UDP packet
 *---------------------------------------------------------------------------
 */
int
bmsend( s, pbm, len, flags )
    int s;
    struct bm *pbm;
    int len, flags;
{
	return( send( s, (char *) pbm, len, flags ) );
}
     

/*---------------------------------------------------------------------------
 * bmsendto - send a bm struct in a UDP packet to a specific machine
 *---------------------------------------------------------------------------
 */
int
bmsendto( s, pbm, len, flags, to, tolen )
    int s;
    struct bm *pbm;
    int len, flags;
    struct sockaddr_in *to;
    int tolen;
{
	return( sendto( s, (char *) pbm, len, flags, to, tolen ) );
}
     

/*---------------------------------------------------------------------------
 * bmrecv - receive a bm structure in a UDP packet
 *---------------------------------------------------------------------------
 */
int
bmrecv( s, pbm, len, flags )
    int s;
    struct bm *pbm;
    int len, flags;
{
	static char bufrecv[ MAXNETPACKET ];
	int status;

	if ( ( status = recv( s, bufrecv, MAXNETPACKET, flags ) ) <= 0 ) {
		return( status );
	}
	bcopy( bufrecv, (char *) pbm, sizeof( struct bm ) );
	return( sizeof( struct bm ) );
}
     


/*---------------------------------------------------------------------------
 * bmrecvfrom - receive a bm structure from a specified machine
 *---------------------------------------------------------------------------
 */
int
bmrecvfrom( s, pbm, len, flags, to, tolen )
    int s;
    struct bm *pbm;
    int len, flags;
    struct sockaddr_in *to;
    int tolen;
{
	static char bufrecv[ MAXNETPACKET ];
	int status;
	
	status = recvfrom( s, bufrecv, MAXNETPACKET, flags, to, tolen );
	if ( status <= 0 ) {
		return( status );
	}
	bcopy( bufrecv, (char *) pbm, sizeof( struct bm ) );
	return( sizeof( struct bm ) );
}
