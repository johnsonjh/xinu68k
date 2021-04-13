/* dload2tcp.c -  
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Mar  2 11:08:01 1988
 */

#include <stdio.h>
#include <errno.h>

#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <setjmp.h>

#include "bed.h"
#include "bmutils.h"
#include "netutils.h"
#include "downloadd.h"

jmp_buf env;			/* used to return from read time-outs	*/

int errno;	        	/* error number when system err	*/

#define READ_TCP

/*---------------------------------------------------------------------------
 * dload2tcp
 *    this routine sends the a.out file to the front end machine so that
 *    it can store it in /tftpboot/a.out for the backend machine to boot
 *    with
 *---------------------------------------------------------------------------
 */
int
dload2tcp( aoutfile, devtcp )
     char *aoutfile;
     int devtcp;
{
	int fdaout;		/* file descriptor of a.out file   	*/
	char buff[ BIGBUFSIZE ];/* big buffer to read into and out of  */
	char sblen[ SMALLSTR ];	/* string form of length               */
	struct stat statbuf;	/* file status structure               */
	long aoutsize;		/* size of a.out file                  */
	int  packetnum;		/* number of packets sent              */
	long  bytessent;	/* number of bytes sent			*/
	
	if ( ( fdaout = open( aoutfile, O_RDONLY, 0400 ) ) < 0 ) {
		fprintf( stderr, "error: cannot open file %s\n", aoutfile );
		return( -1 );
	}
	
	/* Get name of the machine you are using */
	if ( sockgetstr( devtcp, buff, STIMEOUT, env ) == NULL )  {
		fprintf( stderr, "error: cannot read machine name\n" );
		return( -1 );
	}
	if ( fstat( fdaout, &statbuf ) != 0 ) {
		fprintf( stderr, "error: fstat failed on %s\n", aoutfile );
		return( -1 );
	}
	aoutsize = (long) statbuf.st_size;
	sprintf( sblen,"%ld", (long) aoutsize );
	if ( write( devtcp, sblen, strlen( sblen ) + 1 ) !=
	    ( strlen( sblen ) + 1 ) ) {
		fprintf( stderr,
			"error: cannot write file length to frontend\n" );
		return( -1 );
	}
	
	bytessent = 0;
	packetnum = 1;
	while ( aoutsize > sizeof( buff ) ) {
		if ( read( fdaout, buff, sizeof( buff ) ) != sizeof(buff) ) {
			fprintf( stderr, "error: bad read(%s)\n", aoutfile );
			return( -1 );
		}
		
		if ( write( devtcp, buff, sizeof( buff ) ) != sizeof(buff) ){
			fprintf(stderr, "error: bad write to (fdtcp)\n");
			return( -1 );
		}
		packetnum++;
		bytessent += sizeof(buff);
		aoutsize -= sizeof(buff);
		printf("Sent %ld k bytes\r", bytessent/1000);
	}
	if ( read( fdaout, buff, aoutsize ) != aoutsize ) {
		fprintf( stderr, "\nerror: bad read(%s)\n", aoutfile );
		return( -1 );
	}
	if ( write( devtcp, buff, aoutsize ) != aoutsize ) {
		fprintf( stderr, "\nerror: bad write to (fdtcp)\n" );
		return( -1 );
	}
	bytessent += aoutsize;
	printf( "Sent %ld k bytes\n", bytessent/1000 );

#ifdef READ_TCP
	/* read output from the daemon, too */
	shutdown( devtcp, 1 );
	for (;;) {
		int len;

		if ( ( len = read( devtcp, buff, sizeof( buff ) ) ) <= 0 ) {
			if ( len < 0 ) {
				fprintf( stderr, "error: bad read(fdtcp)\n" );
				return( -1 );
			}
			break;
		}
		write( fileno(stdout), buff, len );
	}
#endif
	close( devtcp );
	close( fdaout );
	return( bytessent );
}


