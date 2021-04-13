/* 
 * bedkillall.c - This program sends out a kill request to all backend daemons
 * 
 * Author:	Jim Griffioen/Shawn Oostermann
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Jun  6 20:36:46 1988
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <setjmp.h>
#include <errno.h>

#include "bed.h"
#include "netutils.h"
#include "status.h"

jmp_buf env;			/* used to return from read time-outs	*/
int errno;		        /* error number when system err		*/
int quit();

/*---------------------------------------------------------------------------
 * main (csb, rgsb)
 *---------------------------------------------------------------------------
 */
main ( csb, rgsb )
     int csb;
     char *rgsb[];
{
	if ( csb > 1 ) {
		fprintf( stderr, "usage: bedkillall\n" );
		exit( 1 );
	}
	killall();
	exit( 0 );
}


/*----------------------------------------------------------------------
 * killall - send a quit message to all back end daemons
 *----------------------------------------------------------------------
 */
killall()
{
	int fd_sock;
	int status;
	struct bm bm;
	int i;
	int len;
	struct sockaddr_in sa;
	int salen;


	initsigs();			/* initialize interrupts	*/
	

	/***** set up broadcast connection *****/
	fd_sock = socket( AF_INET, SOCK_DGRAM, 0 );

	bm.ver = CURVER;
	sprintf( bm.cmd, "%d", REQ_QUIT );
	strncpy ( bm.detail, "detail", MAXDETAIL );

	status = BroadcastUDP( fd_sock, (char *) &bm, lbm, KNOWN_UDP_PORT );

	if ( status < 0 ) {
		fprintf(stderr, "error: sendto(broadcast) failed\n");
		exit(1);
	}
	
	i = 0;
	while ( TRUE ) {
		settimer( XSTIMEOUT );
		/* eventually we will timeout and jump here */
		if ( setjmp( env ) == EINTR ) {
			canceltimer();
			printf( "Kill backend daemon on %d machines\n", i );
			return( i );	/* return number of machines	*/
		}
		salen = sizeof( struct sockaddr_in );
		len = recvfrom( fd_sock, &bm, lbm, 0, &sa, &salen );
		if ( len <= 0 ) {
			canceltimer();
			fprintf( stderr, "error: recvfrom failed\n" );
			exit( 1 );
		}
		canceltimer();
		printf( "Killed bed on machine '%s'\n", bm.detail );
		i++;
	}
}

/*---------------------------------------------------------------------------
 * alarmhandler - return from alarm calls
 *---------------------------------------------------------------------------
 */
static
alarmhandler()
{
	signal( SIGALRM, alarmhandler );
	longjmp( env, EINTR );
	/* not reached */
}

/*---------------------------------------------------------------------------
 * quit() -
 *---------------------------------------------------------------------------
 */
quit()
{
	exit( 0 );
}

/*---------------------------------------------------------------------------
 * initsigs() - initialize interrupts
 *---------------------------------------------------------------------------
 */
static
initsigs()
{
	int alarmhandler();
	
	if ( signal( SIGINT,SIG_IGN ) != SIG_IGN ) {
		signal( SIGINT, quit );
	}
	if ( signal( SIGTERM, SIG_IGN ) != SIG_IGN ) {
		signal( SIGTERM, quit );
	}
	if ( signal( SIGQUIT, SIG_IGN ) != SIG_IGN ) {
		signal( SIGQUIT, quit );
	}
	signal( SIGALRM, alarmhandler );
}




