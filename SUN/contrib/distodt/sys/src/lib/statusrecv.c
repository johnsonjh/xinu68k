/* statusrecv.c - 
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

#include <setjmp.h>
#include <errno.h>

#include "bed.h"
#include "netutils.h"

extern jmp_buf env;		/* used to return from read time-outs	*/
extern int errno;		/* error number when system err		*/

extern char *xmalloc();

#define MAXBEDS		200
#define MAXLINE 	256


/*----------------------------------------------------------------------
 * statusrecv - obtain status info from all front ends
 *----------------------------------------------------------------------
 */
int
statusrecv( fd_sock, pbm )
     int fd_sock;
     struct bm * pbm;
{
	int len;
        struct sockaddr_in sa;
        int salen;
	
	settimer(STATTIMEOUT);
	if ( setjmp( env ) == EINTR ) {
		canceltimer();
		return( 0 );		/* return number of machines	*/
	}
        salen = sizeof( struct sockaddr_in );
        len = recvfrom( fd_sock , pbm , lbm , 0 , & sa , & salen );
	if ( len <= 0 ) {
		canceltimer();
		fprintf( stderr, "error: recvfrom failed\n" );
		return( -1 );
	}
	canceltimer();
	return( 1 );
}

