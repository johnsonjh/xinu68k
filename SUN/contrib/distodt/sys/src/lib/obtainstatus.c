/* obtainstatus.c - This routine obtains status info and returns it in a struct
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri May 20 11:57:03 1988
 */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include <errno.h>

#include "bed.h"
#include "bmutils.h"
#include "netutils.h"
#include "status.h"

extern int errno;		/* error number when system err		*/

extern char *xmalloc();

/*#define DEBUG*/

/*----------------------------------------------------------------------
 * obtainstatus - obtain status info from all front ends
 *----------------------------------------------------------------------
 */
int
obtainstatus( stats, cnt )
     struct bm * stats[];
     int cnt;
{
	int i;
	int fd_sock;
	struct bm * pbm;
	
	if ( ( fd_sock = statusrequest() ) < 0 ) {
		exit( 1 );
	}
	for ( i = 0; i < cnt; i++ ) {
		pbm = (struct bm *) xmalloc( sizeof( struct bm ) );
		if ( statusrecv( fd_sock, pbm ) <= 0 ) {
			break;
		}
		stats[ i ] = pbm;
#ifdef DEBUG
		printfusers( (struct statresp *)pbm->detail );
#endif
	}
#ifdef DEBUG
	printf( "obtainstatus: return %i\n", i );
#endif
	return( i );
}

