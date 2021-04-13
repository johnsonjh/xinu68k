/* puts.c - puts */

#define CONSOLE 0

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
                putc(CONSOLE,c);
        return(putc(CONSOLE,'\n'));
}
