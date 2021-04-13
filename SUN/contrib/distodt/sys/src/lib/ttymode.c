/* ttymode.c - 
 * 
 * Author:	Patrick A. Muckelbauer
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Wed Oct 17 13:09:16 1990
 */

#include <stdio.h>
#include <sgtty.h>

#define initializedTTY() ( devsize > 0 )
#define baddevice(dev)	  ( !initializedTTY() || dev < 0 || dev >= devsize )

static struct sgttyb *kttymode = NULL;	/* keyboard mode upon entry 	*/
static int *saved = NULL;
static int *inrawmode = NULL;
static int devsize = 0;

static void
inittty()
{
	int i;
	
	devsize = getdtablesize();
	if ( devsize <= 0 ) {
		return;
	}
	saved = (int *) xmalloc( sizeof( int ) * devsize );
	inrawmode = (int *) xmalloc( sizeof( int ) * devsize );
	kttymode = (struct sgttyb *) xmalloc( sizeof(struct sgttyb)*devsize );
	for ( i = 0; i < devsize; i++ ) {
		saved[ i ] = 0;
		inrawmode[ i ] = 0;
	}
}

int
savetty(dev)
     int dev;
{
	if ( !initializedTTY() ) {
		inittty();
	}
	if ( baddevice( dev ) ) {
		return( -1 );
	}
	if ( saved[ dev ] ) {
		return( 1 );
	}
	if ( ioctl( dev, TIOCGETP, &kttymode[ dev ] ) < 0 )  {
		return( -1 );
	}
	saved[ dev ] = 1;
	return( 1 );
}

int
rawtty( dev )
     int dev;
{
	struct sgttyb cttymode;	/* changed keyboard mode	*/
	
	if ( !initializedTTY() ) {
		inittty();
	}
	if ( baddevice( dev ) ) {
		return( -1 );
	}
	if ( inrawmode[ dev ] ) {
		return( 1 );
	}
	if ( ! saved[ dev ] ) {
		savetty( dev );
	}
	if ( ! saved[ dev ] ) {
		return( -1 );
	}
	cttymode = kttymode[dev];
	cttymode.sg_flags |= RAW;
	cttymode.sg_flags &= ~ECHO;
	cttymode.sg_flags &= ~CRMOD;
	if ( ioctl( dev, TIOCSETP, &cttymode ) < 0 ) {
		return( -1 );
	}
	inrawmode[ dev ] = 1;
	return( 1 );
}

int
restoretty(dev)
     int dev;
{
	if ( !initializedTTY() ) {
		inittty();
	}
	if ( baddevice( dev ) ) {
		return( -1 );
	}
	if ( ! inrawmode[ dev ] ) {
		return( 1 );
	}
	if ( ! saved[ dev ] ) {
		return( -1 );
	}
	if ( ioctl( dev, TIOCSETP, &kttymode[ dev ] ) < 0 ) {
		return( -1 );
	}
	inrawmode[ dev ] = 0;
	return( 1 );
}

void
restorettyAll()
{
	int i;
	
	i = devsize;
	
	for ( i = (devsize - 1); i >= 0; i-- ) {
		restoretty( i );
	}
}
