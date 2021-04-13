/* 
 * downloadd.c - Downloader Daemon - copies a.out file over network
 *               to /tftpboot/a.out
 * 
 * Author:	Jim Griffioen/Steve Chapin/Shawn Oosterman
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Sun Feb 21 23:15:19 1988
 *
 * Copyright (c) 1988 Jim Griffioen/Steve Chapin/Shawn Oosterman
 */


#include <stdio.h>
#include <errno.h>

#include <signal.h>
#include <sgtty.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <setjmp.h>

#include "utils.h"
#include "bed.h"
#include "bmutils.h"
#include "netutils.h"
#include "downloadd.h"
#include "bedata.h"

#define SECS_TO_CONNECT 10

jmp_buf env;			/* used to return from read time-outs	*/
struct option opt;
int errno;	        	/* error number when system err		*/

/*---------------------------------------------------------------------------
 * downloadd - Download daemon forked off by the bed process
 *---------------------------------------------------------------------------
 */
main( argc, argv )
     int argc;
     char **argv;
{
	int fdTcp;
	int ns;
	struct sockaddr_in sa;
	char mesg[ MAXDETAIL ];
	int addrlen = sizeof(struct sockaddr_in);
	int socktimeout();
	struct bedata *beconfig, bedata;
	int i;
	char **ptr;
	
#ifdef DEBUG
	opt.verbose = TRUE;
#else
	opt.verbose = FALSE;
#endif
	opt.logtype = ALOGFILE;
	
	beconfig = &bedata;
	fdTcp = atoi( argv[1] );
	
	ptr = (char **) &( beconfig->data );
	for( i = 0; i < NUM_BED_DATA; i++, ptr++ ) {
		*ptr = argv[ i + 2 ];
	}
	
	signal( SIGALRM, socktimeout );
	
	settimer( SECS_TO_CONNECT );
	ns = accept( fdTcp, &sa, &addrlen );
	canceltimer();
	
	sprintf( mesg,"Using %s%c%c", beconfig->data.ttyname, '\015',
		'\012' );
	write( ns, mesg, strlen( mesg ) + 1 );
#ifdef DOWNLOADDIR
	{
		char aout[ 512 ];
		
		if ( *beconfig->data.dloadfile == '/' ) {
			getaout( ns, beconfig->data.dloadfile );
		}
		else {
			sprintf( aout,"%s%s", DOWNLOADDIR,
				beconfig->data.dloadfile );
			getaout( ns, aout );
		}
	}
#else
	getaout( ns, beconfig->data.dloadfile );
#endif /* DOWNLOADDIR */
	quit();
}

static
Write( fd, str )
     int fd;
     char * str;
{
	write( fd, str, strlen( str ) );
}

static
getaout( ns, name )
     int ns;
     char *name;
{
	int status;			/* ret status of sys calls       */
	int fd;			/* file descriptor of a.out file */
	char buff[ BIGBUFSIZE ];	/* buffer to read data into      */
	long aoutsize;		/* size fo the a.out file        */
	int len;			/* number of bytes read in       */
	
	
	/* Open name for writing */
	if ( status = unlink( name ) != 0 ) {
		Log( "Error removing %s", name );
	}
	
	if ( ( fd = open( name, O_WRONLY | O_CREAT | O_TRUNC, 0666 ) ) < 0 ){
		Write( ns, "\ndownloadd: error opening file!\n" );
		Error( "error: cannot open %s\n", name );
	}
	if ( fchmod( fd, 0666 ) ) {
		Write( ns, "\ndownloadd: error setting permissons!\n" );
		Error( "error: cannot change mode of %s\n", name );
	}
	
	/* Get length of file being sent */
	if ( sockgetstr( ns, buff, STIMEOUT, env ) == NULL ) {
		Write( ns, "\ndownloadd: error getting length!\n" );
		Error( "error: cannot get length of file to download\n" );
	}
	
	/* Read file from the TCP connection and write into /tftpboot/a.out */
	aoutsize = atol( buff );
	while ( aoutsize > 0 ) {
		if ( ( len = read( ns, buff, sizeof( buff ) ) ) == -1 ) {
			Write( ns, "\ndownloadd: read failed!\n" );
			Error( "error: bad return from read(tcp)\n" );
		}
		if ( write( fd, buff, len ) == -1 ) {
			Write( ns, "\ndownloadd: write failed!\n" );
			Error( "error: bad write to %s\n", name );
		}
		aoutsize -= len;
	}

	Write( ns, "downloadd completed\n" );
	/* Close the file descriptors before exiting */
	close( fd );
	close( ns );
	
}

/*---------------------------------------------------------------------------
 * socktimeout
 *---------------------------------------------------------------------------
 */
static
socktimeout()
{
	Log( "error: Connect (or Read) timeout - client not responding" ); 
	quit( 1 );
}

/*---------------------------------------------------------------------------
 * quit - 
 *---------------------------------------------------------------------------
 */
quit()
{
	CleanUp();
	exit( 0 );
}

/*---------------------------------------------------------------------------
 * CleanUp
 *---------------------------------------------------------------------------
 */
static
CleanUp()
{
}
