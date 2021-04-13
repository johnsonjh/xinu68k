/* statusrequest.c - 
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

#include <signal.h>
#include <setjmp.h>
#include <errno.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <ctype.h>

#include "bed.h"
#include "bmutils.h"
#include "netutils.h"
#include "status.h"

extern int errno;		/* error number when system err		*/
jmp_buf env;			/* used to return from read time-outs	*/

extern char *xmalloc();

#define MAXBEDS		200
#define MAXLINELENGTH 	256

extern char * getenv();
extern char * index();

/*----------------------------------------------------------------------
 * statusrequest - obtain socket and send request
 *----------------------------------------------------------------------
 */
int
statusrequest()
{
	int fd_sock;
	int status;
	struct sockaddr_in sa;
	struct sockaddr_in sa_to;
	int salen;
	int i;
	struct bm bm;
	struct bm *bmtmp;
	int len;
	int maxbuff, lmb;	/* used to increase socket buf	*/
	int on = 1;		/* used to set socket options on*/
	struct stat statbuf;	/* status of file struct	*/
	char * beds[ MAXBEDS ];	/* names of bedservers		*/
	int numbeds;		/* number of bedservers		*/
	int bcast;
	char * sb;
	
	
	initsigs();		/* initialize interrupts	*/
	
	if ( ( fd_sock = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
		fprintf( stderr, "error: socket failed\n" );
		return( -1 );
	}
	
#if defined(SUN) || defined(DEC) || defined(VAX) || defined(SEQ)

	maxbuff = BIGBUFSIZE;
	lmb = sizeof( maxbuff );
	if ( setsockopt( fd_sock, SOL_SOCKET, SO_RCVBUF,
			(char *) & maxbuff, lmb )!=0 ) {
		fprintf( stderr, "error: setsockopt falied\n" );
		return( -1 );
	}

#endif
		
	bcast = 0;
	numbeds = 0;
	beds[ 0 ] = NULL;
	if ( ( ( sb = getenv( ENVBEDSERVERS ) ) != NULL ) &&
		 ( *sb != '\0' ) ) {
		if ( *sb == ':' ) {
			sb++;
			numbeds = getbedsFromString( beds, sb );
		}
		else {
			numbeds = getbedsFromFile( beds, sb );
		}
	}
	else if ( stat( BEDSERVERS_FILE, & statbuf ) == 0 ) {
		numbeds = getbedsFromFile( beds, BEDSERVERS_FILE );
	}
	else {
		bcast = 1;
	}
	if ( numbeds < 0 ) {
		return( -1 );
	}
	if ( bcast == 0 ) {
		bcast = fakemulticast( beds, numbeds, fd_sock );
	}
	if ( bcast > 0 ) {
		bm.ver = CURVER;
		sprintf( bm.cmd, "%d", REQ_STATUS );
		strncpy( bm.detail, "detail", MAXDETAIL );

		status = BroadcastUDP( fd_sock, (char *) &bm, lbm,
				      KNOWN_UDP_PORT );
		
		if ( status < 0 ) {
			fprintf( stderr,
				"error: broadcasting status request failed\n");
			return( -1 );
		}
	}
	return( fd_sock );
}

extern char *fgets();

/*
 *---------------------------------------------------------------------------
 * getbedsFromFile - read the BEDSERVERS file and return # of bed servers
 *---------------------------------------------------------------------------
 */
static int
getbedsFromFile( beds, filename )
     char * beds[];
     char * filename;
{
	FILE * fs;	
	char * sb;
	char * p;
	int	i;	
	
	if ( ( fs = fopen( filename, "r" ) ) == (FILE *) NULL ) {
		fprintf( stderr, "error: unable to open file %s\n",filename);
		return( -1 );
	}
	i = 0;
	beds[ 0 ] = (char *) xmalloc( MAXLINELENGTH + 1 );
	while ( fgets( beds[ i ], MAXLINELENGTH, fs ) != NULL ) {
		if ( ( (*beds[i]) == '#' ) ) {
			continue;
		}
		for ( p = beds[ i ]; *p != '\0' && isspace( *p ); p++ ) {
			/* null statement */;
		}
		if ( *p == '\0' ) {
			continue;
		}
		for ( sb = beds[ i ]; *p != '\0' && !isspace( *p ); ) {
			*sb++ = *p++;
		}
		*sb = '\0';
		i++;
		beds[ i ] = (char *) xmalloc( MAXLINELENGTH + 1 );
	}
	free( beds[ i ] );
	beds[ i ] = NULL;
	return( i );
}

/*
 *---------------------------------------------------------------------------
 * getbedsFromString - read the BEDSERVERS from the environment variable
 *---------------------------------------------------------------------------
 */
static int
getbedsFromString( beds, sb)
     char * beds[];
     char * sb;
{
	char * envBed;
	char * p;
	int	i;
	
	envBed = xmalloc( strlen( sb ) + 1 );
	strcpy( envBed, sb );
	i = 0;
	while ( envBed != NULL && *envBed != '\0' ){
		if ( ( sb = index( envBed, ':' ) ) != NULL ) {
			*sb = '\0';
			beds[ i ] = envBed;
			envBed = sb + 1;
		}
		else {
			beds[ i ] = envBed;
			envBed = NULL;
		}
		i++;
	}
	beds[ i ] = NULL;
	return( i );
}

/*
 *---------------------------------------------------------------------------
 * fakemulticast - Fake a multicast by sending to the given list of bed servers
 *---------------------------------------------------------------------------
 */
static int fakemulticast( beds, numbeds, fd_sock )
     char *beds[];
     int numbeds;
     int fd_sock;
{
	int status;
	struct sockaddr_in sa;
	struct sockaddr_in sa_to;
	int i;
	struct bm bm;
	struct stat statbuf;		/* status of file struct	*/
	struct hostent *phent;		/* host network entry		*/
	int bcast;
	unsigned long inet_addr(), addr;
	
	sa.sin_family = AF_INET;
	sa.sin_port   = htons( 0 );
	sa.sin_addr.s_addr = INADDR_ANY;
	status = bind( fd_sock, & sa, sizeof( struct sockaddr_in ) );
	
	bcast = 0;
	for ( i = 0; i < numbeds; i++ ) {
		if ( ! strcmp( "*", beds[ i ] ) ) {
			bcast = 1;
			continue;
		}
		if ( strlen( beds[ i ] ) == 0 ) {
			continue;
		}
		sa_to.sin_family = AF_INET;
		sa_to.sin_port   = htons( KNOWN_UDP_PORT );
		if ( ( addr = inet_addr( beds[ i ] ) ) != -1 ) {
			bcopy( (char *) &addr, (char *) &sa_to.sin_addr,
			      sizeof( unsigned long ) );
		}
		else if ( ( phent = gethostbyname( beds[ i ] ) ) != NULL ) {
			bcopy( phent->h_addr, (char *)&sa_to.sin_addr,
			      phent->h_length );
		}
		else {
			fprintf( stderr, "warning: unknown host '%s'\n",
				beds[ i ] );
			continue;
		}
		
		bm.ver = CURVER;
		sprintf( bm.cmd, "%d", REQ_STATUS );
		strncpy ( bm.detail, "detail", MAXDETAIL );
		status = sendto( fd_sock, (char *) &bm, lbm, 0,
				&sa_to, sizeof( struct sockaddr_in ) );
		if ( status < 0 ) {
			fprintf( stderr,
				"warning: sendto(status request) failed\n",
				beds[ i ]);
		}
	}
	return( bcast );
}

/*
 *---------------------------------------------------------------------------
 * alarmhandler - return from alarm calls
 *---------------------------------------------------------------------------
 */
static void
alarmhandler()
{
	signal( SIGALRM, alarmhandler );
	longjmp( env, EINTR );
	/* not reached */
}

/*
 *---------------------------------------------------------------------------
 * inthandler -- interrupt processing
 *---------------------------------------------------------------------------
 */
static void
inthandler()
{
	exit( 2 );
}
/*
 *---------------------------------------------------------------------------
 * initsigs() - initialize interrupts
 *---------------------------------------------------------------------------
 */
static 
initsigs()
{
	if ( signal( SIGINT,SIG_IGN ) != SIG_IGN ) {
		signal( SIGINT,inthandler );
	}
	if ( signal( SIGTERM, SIG_IGN) != SIG_IGN ) {
		signal( SIGTERM, inthandler );
	}
	if ( signal( SIGQUIT, SIG_IGN ) != SIG_IGN ) {
		signal( SIGQUIT, inthandler );
	}
	signal( SIGALRM, alarmhandler );
}
