/* obtainlock.c - Routines for obtaining a lock on a remote machine
 * 
 * Author:	Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Tue May 24 22:10:01 1988
 *
 * Copyright (c) 1988 Jim Griffioen
 */

#include <stdio.h>
#include <sys/file.h>

#include "bed.h"
#include "status.h"

/*---------------------------------------------------------------------------
 * getfilename
 *---------------------------------------------------------------------------
 */
int
getfilename( filename, user )
     char * filename, * user;
{
	char * ttyline;			/* tty line used		*/
	char * devname;			/* tty device line name		*/
	char * sb;
	
	strcpy( filename, BASEDIR );
	strcat( filename, user );
	strcat( filename, "." );
	
	if ( ( devname = (char *) ttyname( 0 ) ) != NULL ) {
		ttyline = (char *) strrchr( devname, '/' );
		ttyline++;
		strcat( filename, ttyline );
	}
	else {
		strcat( filename, "notty" );
	}
	return( 1 );
}

/*---------------------------------------------------------------------------
 * getidleminutes - extract minutes from idletime string
 *---------------------------------------------------------------------------
 */
int
getidleminutes( idletime )
     char * idletime;
{
	int hours, minutes, seconds;
	
	sscanf( idletime, "%d:%d:%d", & hours, & minutes, & seconds );
	if ( hours > 0 ) {
		minutes = minutes + ( hours * 60 );
	}
	return( minutes );
}


#ifdef HPPA

bzero( buf, len )
     char * buf;
     int len;
{
	for (; len > 0; len-- ) {
		*buf++ = 0;
	}
}

bcopy( from, to, len )
     char * from, * to;
     int len;
{
	for (; len > 0; len-- ) {
		*to++ = *from++;
	}
}

char *
index( buf, ch )
     char * buf;
     char ch;
{
	for (; *buf; buf++ ) {
		if ( *buf == ch ) {
			return( buf );
		}
	}
	return( NULL );
}

int
getdtablesize()
{
	return( _NFILE );
}

#endif
