/* kprintf.c - kprintf, kputc */

#include <tftpload.h>
#include <sunromvec.h>

#define prputchar(x) (romp->v_putchar)((unsigned char) x)

/*
 * By using the version of kprintf in the Sun3 rom, we save about 1500 bytes
 * in the downloaded image.  The ROM version doesn't support field widths,
 * but will print simple strings, numbers, hex, and character stuff
 */


#define USE_ROM_VERSION


#ifdef USE_ROM_VERSION
/*------------------------------------------------------------------------
 *  kprintf  --  ROM printf: formatted, unbuffered output to selected device
 *               currently only supports up to 9 args..
 *------------------------------------------------------------------------
 */
kprintf(fmt, a1,a2,a3,a4,a5,a6,a7,a8,a9)
char *fmt;
int a1,a2,a3,a4,a5,a6,a7,a8,a9;
{
        (romp->v_printf)(fmt,a1,a2,a3,a4,a5,a6,a7,a8,a9);
}
#endif




#ifdef USE_XINU_VERSION
/*------------------------------------------------------------------------
 *  kprintf  --  kernel printf: formatted, unbuffered output to CONSOLE
 *------------------------------------------------------------------------
 */
kprintf(fmt, args)		/* Derived by Bob Brown, Purdue U.	*/
char *fmt;
int args;
{
        int     kputc();

        _doprnt(fmt, &args, kputc, 0);
        return(OK);
}

/*------------------------------------------------------------------------
 *  kputc  --  write a character on the console using polled I/O
 *------------------------------------------------------------------------
 */
kputc(device ,c)
	int	device;
	register unsigned char c;	/* character to print from _doprnt	*/
{
	if ( c == 0 )
		return;
	if ( c > 127 ) {
	    prputchar('M');		/* do what "cat -v" does */
	    prputchar('-');		/* prom routine dies otherwise */
	    prputchar((char) (c & 0x007f) );
	    return;
	}

	if (c==NEWLINE)
	    prputchar(RETURN);

	prputchar(c);
}

#endif
