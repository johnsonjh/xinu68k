/* fputs.c - fputs */

/*------------------------------------------------------------------------
 *  fputs  --  write a null-terminated string to a device (file)
 *------------------------------------------------------------------------
 */
fputs(dev, s)
        register char *s;
        register int dev;
{
	register r;
	register c;
	int	putc();

	while (c = *s++)
                r = putc(dev, c);
	return(r);
}
