/* 
 * idletime.c - routines to obtain the idle time
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Oct 27 01:49:32 1989
 *
 * Copyright (c) 1989 Patrick A. Muckelbauer
 */

#include <stdio.h>
#include <signal.h>

#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/timeb.h>

#include "bed.h"
#include "bedata.h"

/*
  -----------------------------------------------------------------------------
  settime(time, idle) -- set the idle with time seconds
  -----------------------------------------------------------------------------
  */
void
settime( time, idle )
     time_t time;
     char *idle;
{
	int hours, min;
	
	hours = time / ( 60 * 60 ); time %= ( 60 * 60 );
	min   = time / 60;
	time %= 60;
	sprintf( idle,"%02d:%02d:%02d", hours, min, (int) time );
}

/*
  -----------------------------------------------------------------------------
  getidletime(beconfig)  -- return the idle time (secs.) for the back end
  -----------------------------------------------------------------------------
  */
time_t
getidletime( beconfig )
     struct bedata * beconfig;
{
	struct stat stats, *buf;
	time_t t;
	
	if ( ! beconfig->lock.locked ) {
		return( 0 );
	}
	buf = &stats;
	if ( fstat( beconfig->lock.fd, buf ) ) {
		Log( "error reading stats for ttyline: %s",
		    beconfig->data.ttyname );
		return( 0 );
	}
	t = time( 0 );
	t -= buf->st_mtime;
	return( t );
}
