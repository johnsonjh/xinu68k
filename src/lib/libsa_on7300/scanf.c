/* scanf.c - scanf, fscanf, sscanf, getch, ungetch, sgetch, sungetch */

/* Character look-ahead is implemented with a local one-char buffer
   rather than with Xinu read-ahead (ch=control(unit,TCNEXTC))
   so that the same library can be used standalone as with Xinu.
   This only affects code that switches from scanf to getc,
   or scanf to scanf, leaving a scanf-terminating char in the
   buffer that isn't ever used again.  It's usually newline or other
   white space that gets skipped over anyway. */

#include <stdio.h>

#define	EMPTY	(-2)
#define EMPTYFLAG (1<<12)	/* here was the bug */
extern int console;

/*-----------------------------------------------------------------------
 *  scanf  --  read from the console according to a format
 *------------------------------------------------------------------------
 */
scanf(fmt, args)
	char	*fmt;
	int	args;
{
	int	getch();
	int	ungetch();
	int	buf;		/* under Xinu: per-process buffer */

	buf = EMPTYFLAG;
	return(_doscan(fmt, &args, getch, ungetch, console, &buf));
}

/*------------------------------------------------------------------------
 *  fscanf  --  read from a device (file) according to a format
 *------------------------------------------------------------------------
 */
fscanf(dev, fmt, args)
	int	dev;
	char	*fmt;
	int	args;
{
	int	getch();
	int	ungetch();
	int	buf;

	buf = EMPTYFLAG;
	return(_doscan(fmt, &args, getch, ungetch, dev, &buf));
}

/*------------------------------------------------------------------------
 *  getch  --  get a character from a device with pushback
 *------------------------------------------------------------------------
 */
static	getch(dev, buf)
	int	dev;
	int	*buf;
{
        int ch;

	if( *buf&EMPTYFLAG)
		*buf = getc(dev); /* make sure one there */
	ch = *buf;		/* pick up buffered char */
	*buf |= EMPTYFLAG;		/* none there now */
	return(ch);
}

/*------------------------------------------------------------------------
 *  ungetch  --  pushback a character for getch
 *------------------------------------------------------------------------
 */
static	ungetch(dev, buf)
	int	dev;
	int	*buf;
{
	*buf &= (~EMPTYFLAG);	/* turn off emptyflag */
}

/*------------------------------------------------------------------------
 *  sscanf  --  read from a string according to a format
 *------------------------------------------------------------------------
 */
sscanf(str, fmt, args)
	char	*str;
	char	*fmt;
	int	args;
{
	int	sgetch();
	int	sungetch();
	char	*s;

	s = str;
	return(_doscan(fmt, &args, sgetch, sungetch, 0, &s));
}

/*------------------------------------------------------------------------
 *  sgetch  -- get the next character from a string
 *------------------------------------------------------------------------
 */
static	sgetch(dummy,cpp)
	int	dummy;
	char	**cpp;
{
	return( *(*cpp) == '\0' ? EOF : *(*cpp)++ );
}

/*------------------------------------------------------------------------
 *  sungetc  --  pushback a character in a string
 *------------------------------------------------------------------------
 */
static	sungetch(dummy,cpp)
	int	dummy;
	char	**cpp;
{
	return(*(*cpp)--);
}
