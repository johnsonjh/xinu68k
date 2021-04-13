/* gets.c - gets */
#include <stdio.h>

extern int console;

/*------------------------------------------------------------------------
 *  gets  -- gets string from the console device reading to user buffer
 *------------------------------------------------------------------------
 */
char *gets(s)
        char *s;
{
	register c;
	register char *cs;

	cs = s;
        while ((c = getc(console)) != '\n' && c != '\r' && c != EOF)
		*cs++ = c;
	if (c==EOF && cs==s)
		return(NULL);
	*cs++ = '\0';
	return(s);
}
