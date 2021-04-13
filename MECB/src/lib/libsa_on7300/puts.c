/* puts.c - puts */

extern int console;
/*------------------------------------------------------------------------
 *  puts  --  write a null-terminated string to the console
 *------------------------------------------------------------------------
 */
puts(s)
register char *s;
{
	register c;
	int	putc();

	while (c = *s++)
                putc(console,c);
        return(putc(console,'\n'));
}
