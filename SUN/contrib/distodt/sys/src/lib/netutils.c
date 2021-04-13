/* netutils.c - Handy routines for dealing with the network
 * 
 * Author:	Tim Korb/Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Thu Jun  9 21:32:25 1988
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <assert.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "netutils.h"

extern int errno;

/*---------------------------------------------------------------------------
 * ConnectUdp (sbHost, port) -- create a udp socket to destination
 * port on sbHost. Connect the socket to the port.
 *---------------------------------------------------------------------------
 */
int
ConnectUdp( sbHost, port )
     char * sbHost;
     int port;
{
	int fd;
	struct sockaddr_in sa;	/* sa = socket address */
	struct hostent *phe;	/* he = host entry */
	struct in_addr na;		/* na = network address */
	struct servent *pse;	/* se = service entry */
	
	if ( ( fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
		return( -1 );
	}
	
	if ( ( phe = gethostbyname( sbHost ) ) == NULL ) {
		return( -1 );
	}
	
	na = *( (struct in_addr *) ( phe->h_addr ) );
	
	sa.sin_port = htons( port );
	
	sa.sin_family = AF_INET;
	sa.sin_addr = na;
	if ( connect( fd, & sa, lsa ) == -1 ) {
		return( -1 );
	}
	return( fd );
}

/*---------------------------------------------------------------------------
 * ConnectTcp (sbHost, port) -- open a tcp connection to host sbHost at
 * the port named port. Return a two-way file descriptor to that socket.
 *---------------------------------------------------------------------------
 */
int
ConnectTcp (sbHost, port)
     char *sbHost;
     int port;
{
	int fd;
	struct sockaddr_in sa;	/* sa = socket address */
	struct hostent *phe;	/* he = host entry */
	struct servent *pse;	/* se = service entry */
	
	if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 ) {
		return(-1);
	}
	
	sa.sin_family = AF_INET;
	
	if ( ( phe = gethostbyname( sbHost ) ) == NULL ) {
		return( -1 );
	}
	assert( phe->h_length == 4 );
	sa.sin_addr = *( (struct in_addr *) ( phe->h_addr ));
	
	sa.sin_port = htons( port );
	
	if ( connect( fd, & sa, sizeof( sa ) ) == -1 ) {
		return( -1 );
	}
	return( fd );
}

/*---------------------------------------------------------------------------
 * ListenTcp () -- create and return an fd that will accept TCP connections.
 *---------------------------------------------------------------------------
 */
int
ListenTcp ( pport )
     u_short * pport;
{
	int fd;
	struct sockaddr_in sa;
	int len;
	
	if ( (fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0) {
		return( -1 );
	}
	
	sa.sin_family = AF_INET;
	sa.sin_port = htons( *pport );
	sa.sin_addr.s_addr = INADDR_ANY;/* accept connection from all */
	
	if ( bind( fd, & sa, lsa ) == -1 ) {
		return( -1 );
	}
	
	len = sizeof( sa );
	if ( getsockname( fd, & sa, & len ) < 0 ) {
		return( -1 );
	}
	if ( listen( fd, SOMAXCONN ) == -1 ) {	/* set up for listening */
		return( -1 );
	}
	*pport = ntohs( sa.sin_port );
	return( fd );
}

/*---------------------------------------------------------------------------
 * ListenUdp (port) -- create and return an fd that will receive
 * a datagram on port named by port
 *---------------------------------------------------------------------------
 */
int
ListenUdp( port )
     int port;
{
	int fd;
	struct sockaddr_in sa;
	struct servent *pse;
	
	if ( ( fd = socket( AF_INET, SOCK_DGRAM, 0 ) ) < 0 ) {
		return( -1 );
	}
	sa.sin_family = AF_INET;
	sa.sin_port = htons( port );
	sa.sin_addr.s_addr = INADDR_ANY;
	
	if ( bind( fd, & sa, lsa ) == -1 ) {
		return( -1 );
	}
	return( fd );
}

#include <setjmp.h>

/*---------------------------------------------------------------------------
 * sockgetstr(sock, str, timeout) - get str from tcp socket-ret NULL if error
 *---------------------------------------------------------------------------
 */
char *
sockgetstr( sock, str, timeout, env )
     int sock;				/* socket descriptor handle	*/
     char *str;
     int timeout;
     jmp_buf env;
{
	char *sb;
	
	settimer( timeout );
	if ( setjmp( env ) == EINTR ) {
		canceltimer();
		return( NULL );
	}
	sb = str;
	do {
		if ( recv( sock, sb, 1, 0 ) < 1 ) {
			canceltimer();
			return( NULL );
		}
	} while ( *sb++ != '\0' );
	canceltimer();
	return( str );
}

#include <net/if.h>
#include <sys/ioctl.h>

#if defined(SUN) || defined(DEC) || defined(VAX) || defined(SEQ)

#include <arpa/inet.h>

#endif

/*
 * The following is kludged-up support for simple rpc broadcasts.
 * Someday a large, complicated system will replace these trivial
 * routines which only support udp/ip .
 */

static int
getBroadcastNets( addrs, sock )
        struct in_addr *addrs;
        int sock;       /* any valid socket will do */
{
	struct ifconf ifc;
	struct ifreq ifreq, *ifr;
	struct sockaddr_in *sin;
	int n, i;
	char buf[ 2000 ];

	ifc.ifc_len = sizeof( buf );
	ifc.ifc_buf = buf;
	if (ioctl(sock, SIOCGIFCONF, (char *)&ifc) < 0) {
		return (0);
	}
	ifr = ifc.ifc_req;
	for ( i = 0, n = ifc.ifc_len/sizeof (struct ifreq); n > 0; n--, ifr++){
		ifreq = *ifr;
		if ( ioctl( sock, SIOCGIFFLAGS, (char *) &ifreq ) < 0 ) {
			continue;
		}
		if ( ( ifreq.ifr_flags & IFF_BROADCAST ) &&
		    ( ifreq.ifr_flags & IFF_UP ) &&
		    ifr->ifr_addr.sa_family == AF_INET ) {
			sin = (struct sockaddr_in *) &ifr->ifr_addr;
			if ( ioctl(sock, SIOCGIFBRDADDR, (char *)&ifreq) < 0 ){
				addrs[i++] = inet_makeaddr(inet_netof
					(sin->sin_addr.s_addr), INADDR_ANY);
			}
			else {
				addrs[i++] = ((struct sockaddr_in*)
						&ifreq.ifr_addr)->sin_addr;
			}
		}
	}
	return( i );
}

int
BroadcastUDP( sock, buf, len , port )
     int sock;
     char * buf;
     int len;
     int port;
{
	struct in_addr addrs[ 20 ];
	struct sockaddr_in baddr; /* broadcast and response addresses */
	int nets;
	int i;
	int on = 1;

	/* set socket options so we can broadcast the IP addr back */
	if ( setsockopt( sock , SOL_SOCKET , SO_BROADCAST ,
			& on , sizeof( on ) ) < 0 ) {
		return( -1 );
	}
	nets = getBroadcastNets( addrs, sock );
	bzero( (char *)&baddr, sizeof( baddr ) );
	baddr.sin_family = AF_INET;
	baddr.sin_port = htons( port );
	baddr.sin_addr.S_un.S_addr = htonl( INADDR_ANY );
	for (i = 0; i < nets; i++) {
		baddr.sin_addr = addrs[ i ];
		if ( sendto( sock, buf, len, 0, (struct sockaddr *) & baddr,
			    sizeof( struct sockaddr_in ) ) != len ) {
			return( -1 );
		}
	}
}
