/* 
 * xmalloc.c - 
 * 
 * Author:	Shawn Oostermann/Jim Griffioen
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Fri Jun 10 11:36:27 1988
 *
 * Copyright (c) 1988 Shawn Oostermann/Jim Griffioen
 */

#include <stdio.h>

extern char * malloc();

char *
xmalloc( size )
     int size;
{
	char * p;
	
	if ( ( p = malloc( size ) ) == NULL ) {
		fprintf( stderr, "malloc failed\n" );
		exit( 1 );
	}
	return( p );
}

