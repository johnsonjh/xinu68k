/* 
 * log.c - log routines
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 11:36:27 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>
#include <ctype.h>
#include <sys/time.h>
#include <errno.h>
#include <sys/file.h>

#include "bed.h"
#include "utils.h"

#define	DEFAULTLOGTYPE		TERMINAL

extern struct option opt;

static char logfilename[ MAXFILENAME ] = { '\0' };

/*
 *---------------------------------------------------------------------------
 * TimeToSb (ti) -- return pointer to current time string.
 *---------------------------------------------------------------------------
 */
static char *
TimeToSb ( ti )
     long ti;
{
	struct tm *ptm;
	char *sb;
	
	ptm = localtime( &ti );
	sb = asctime( ptm );
	
	/* sb in form: "Sat Nov 10 11:23:13 1984" */
	
	sb[ 19 ] = '\0';	/* clobber year */
	sb += 4;		/* scoot over day of week */
	
	return( sb );
}

/*
 *---------------------------------------------------------------------------
 * Log (sb, w1, w2, w3, w4, w5) -- add log message to stderr.
 *---------------------------------------------------------------------------
 */
void
Log( sb, w1, w2, w3, w4, w5 )
     char *sb;
     int w1, w2, w3, w4, w5;
{
	FILE *fp;
	int	fd;
	int	status;
	int	pid, ppid;
	char procid[ 32 ];
	char parentid[ 32 ];
	
	if ( opt.logtype == NOLOG ) {
		return;
	}
	if ( ( opt.logtype == DONTCARE ) ||
	    ( ( opt.logtype != TERMINAL ) && ( opt.logtype != ALOGFILE ) ) ) {
		opt.logtype = DEFAULTLOGTYPE;
	}
	if ( opt.logtype == TERMINAL ) {
		fprintf( stderr, "%s\t", TimeToSb ( time (0L ) ) );
		fprintf( stderr, sb, w1, w2, w3, w4, w5 );
		fprintf( stderr, "\n" );
		fflush( stderr );
	}
	else if ( opt.logtype == ALOGFILE ) {
		
		pid = getpid();
		ppid = getppid();
		
		getLogFileName();
		
		if ( ( fd = open( logfilename, O_CREAT | O_APPEND | O_WRONLY,
				 0666 ) ) < 0) {
			exit( 1 );
		}
		/*
		  if ( ( status = flock( fd, LOCK_EX ) ) != 0 ) {
		  exit( 1 );
		  }
		  */
		if ( ( fp = fdopen( fd, "a" ) ) == (FILE *)NULL ) {
			exit( 1 );
		}
		fprintf ( fp, "%s:ppid=%d,pid=%d:\t", TimeToSb( time (0L) ),
			 ppid, pid );
		fprintf( fp, sb, w1, w2, w3, w4, w5 );
		fprintf( fp, "\n" );
		fflush( fp );
		fclose( fp );
	}
}

/*---------------------------------------------------------------------------
 * Error(sb, w1, w2) -- print error and exit.
 *---------------------------------------------------------------------------
 */
void
Error( sb, w1, w2, w3, w4, w5 )
     char * sb;
     int w1, w2, w3, w4, w5;
{
	Log( sb, w1, w2, w3, w4, w5 );
	quit();
}


getLogFileName()
{
	if ( *logfilename != '\0' ) {
		return;
	}
	strcpy( logfilename, LOGGINGDIR );
	if ( *logfilename == '\0' ) {
		exit( 1 );
	}
	if ( logfilename[ strlen( logfilename ) - 1 ] != '/' ) {
		logfilename[ strlen( logfilename ) - 1 ] = '/';
		logfilename[ strlen( logfilename ) ] = '\0';
	}
	gethostname( logfilename + strlen( logfilename ),
		    MAXFILENAME - strlen( logfilename ) );
	logfilename[ MAXFILENAME - 1 ] = '\0';
}
