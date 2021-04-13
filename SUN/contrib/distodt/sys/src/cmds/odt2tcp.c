/* odt2tcp.c - 
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Oct 17 13:18:51 1990
 */
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/time.h>
#include <sgtty.h>
#include <signal.h>


#include "bed.h"
#include "bmutils.h"
#include "odtd.h"

#define BUFSIZE 100

static int doMapping = 0;
static unsigned char globalMask;

/*---------------------------------------------------------------------------
 * odt2tcp - Take standard input and send it to BED process on frontend 
 *---------------------------------------------------------------------------
 */
odt2tcp( filein, fileout, devtcp, escch, initialMask )
     int devtcp;
     FILE *filein, *fileout;
     char escch;
     unsigned char initialMask;
{
	int devin, devout;
	int bmaskRead;
	int bmaskResult;
	int zero = 0;
	int i;

	globalMask = initialMask;
	devin = fileno(filein);
	devout = fileno(fileout);
	if ( isatty( devin ) ) {
		rawtty( devin );
	}
	else {
		doMapping = 1;
	}
	if ( isatty( devout ) ) {
		rawtty( devout );
	}
	bmaskRead = BITMASK( devin ) | BITMASK( devtcp );

	for ( ; 1 ; ) {
		do {
			bmaskResult = bmaskRead;
			i = select( 32, &bmaskResult, &zero, &zero, 0 );
		} while( i == -1 && errno == EINTR );
		if ( i <= 0 ) {
			fprintf( stderr, "error: bad return from select\n" );
			break;
		}
		if ( ( bmaskResult & BITMASK( devtcp ) ) != 0 ) {
			if ( flushtcp( devtcp, devout ) <= 0 ) {
				break;
			}
		}
		if ( ( ( bmaskResult & BITMASK(devin ) ) != 0 ) ) {
			if ( flushinput( escch, filein, devtcp,
					fileout ) <= 0 ) {
				break;
			}
		}
	}
	if ( ! isatty( devin ) ) {	/* flush output if not a normal tty */
		flushAllTcp( devtcp, devout );
	}
	fprintf( fileout, "\n\r" );
	fflush( fileout );
	restorettyAll();
	return;
}

static 
flushAllTcp( devtcp, devout )
     int devtcp, devout;
{
	int bmaskResult, bmaskRead;
	int i;
	int zero = 0;
	struct timeval timev;

	timev.tv_sec = 1;
	timev.tv_usec = 0;

	bmaskRead = BITMASK( devtcp );
	while ( 1 ) {
		bmaskResult = bmaskRead;
		do {
			i = select( 32, & bmaskResult, & zero, & zero,
				   & timev );
		} while( i == -1 && errno == EINTR );
		if ( i <= 0 ) {
			break;
		}
		if ( flushtcp( devtcp, devout ) <= 0 ) {
			break;
		}
	}
}

static int
flushtcp( devtcp, devout )
     int devtcp, devout;
{
	char buf[ BUFSIZE ];
	int len;
	int i;

	if ( ( len = read( devtcp, buf, BUFSIZE ) ) <= 0 ) {
		if ( len != 0 ) {
			fprintf( stderr,
				"error: bad return from read(tcp)\n" );
		}
		return( -1 );
	}
	for ( i = 0; i < len; i++ ) {
		buf[ i ] &= globalMask;
	}
	if ( ( len = write( devout, buf, len ) ) <= 0 )  {
		fprintf( stderr, "error: bad return from write(devout)\n" );
		return( -1 );
	}
	return( len );
}

static int
flushinput( escch, filein, devtcp, fileout )
     char escch;
     FILE *filein, *fileout;
     int devtcp;
{
	unsigned char buf[ 1 ];
	int len;
	int devin = fileno( filein );
	int i;

	if ( (len = read( devin, buf, 1 ) ) <= 0 ) {
		if ( len != 0 ) {
			fprintf( stderr,
				"error: bad return from read(devin)\n" );
		}
		return( -1 );
	}
	for ( i = 0; i < len; i++ ) {
		buf[ i ] &= globalMask;
	}
	if ( buf[ 0 ] == escch ) {
		return( handlebreak( escch, filein, devtcp, fileout ) );
	}
	return( escapewrite( devtcp, buf[ 0 ] ) );
}

/*---------------------------------------------------------------------------
 * breakch - handle magic character
 *---------------------------------------------------------------------------
 */
static int
handlebreak( escch, filein, devtcp, fileout )
     char escch;
     FILE *filein, *fileout;
     int devtcp;
{
	int ch;
	int done;
	int devin, devout;
	int rv = 1;

	devin = fileno( filein );
	devout = fileno( fileout );

	fprintf( fileout, "\n\r" );
	done = FALSE;
	while ( ! done ) {
		fprintf( fileout, "\n\r(odt-command-mode) " );
		ch = fgetc( filein );
		ch &= 0x7F;
		fprintf( fileout, "%c\n\r", ch );
		switch ( ch ) {
		      case EOF:
		      case 'q':
			rv = -1;
			done = 1;
			break;

		      case '7':
			globalMask = 0x7F;
			break;

		      case '8':
			globalMask = 0xFF;
			break;
			
		      case 'c':
			done = TRUE;
			break;

		      case '?':
		      case 'h':
			fprintf( fileout, "h, ? : help message\n\r" );
			fprintf( fileout, "s     : send default character 0x%x\n\r", (unsigned int) escch );
			fprintf( fileout, "b     : send break\n\r" );
			fprintf( fileout, "c     : continue session\n\r" );
			fprintf( fileout, "z     : suspend\n\r" );
			fprintf( fileout, "7     : 7-Bit ASCII\n\r" );
			fprintf( fileout, "8     : 8-Bit ASCII\n\r" );
			fprintf( fileout, "q     : quit\n\r" );
			break;

		      case 's':
			done = TRUE;
			rv = escapewrite( devtcp, escch );
			break;

		      case 'b':
			done = TRUE;
			rv = puttcp( devtcp, ODTD_ESCCH );
			rv &= puttcp( devtcp, ODTD_BREAKCH );
			break;

		      case 'z':
			done = FALSE;
			restorettyAll();
			kill( getpid(), SIGTSTP );
			if ( isatty( devin ) ) {
				rawtty( devin );
			}
			if ( isatty( devout ) ) {
				rawtty( devout );
			}
			break;

		      default:
			fprintf( fileout, "Illegal option '0x%x'.  Type h for help\n\r", (unsigned char) ch );
			break;
		}
	}
	fprintf( fileout, "\n\r" );
	fflush( fileout );
	return( rv );
}

static int
escapewrite( devtcp, ch )
{
	if ( ch == ODTD_ESCCH ) {
		if ( puttcp( devtcp, ODTD_ESCCH ) <= 0 ) {
			return( -1 );
		}
	}
	return( puttcp( devtcp, ch ) );
}
     

/*---------------------------------------------------------------------------
 * puttcp - put a character
 *---------------------------------------------------------------------------
 */
static int
puttcp( devtcp, ch )
     int devtcp;
     char ch;
{
	if ( doMapping && ch == '\012' ) {/* newline maps to carriage return */
		ch = '\015';
	}
	if ( write( devtcp, &ch, 1 ) <= 0 ) {
		fprintf( stderr, "error: bad return from write(tcp)\n" );
		return( -1 );
	}
	return( 1 );
}

