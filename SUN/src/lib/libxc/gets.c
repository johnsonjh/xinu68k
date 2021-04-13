/* gets.c - gets */

#define NULL    0
#define CONSOLE 0
#define	EOF	'\004'		/* control-d is end-of-file		*/

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
        while ((c = getc(CONSOLE)) != '\n' && c != '\r' && c != EOF)
		*cs++ = c;
	if (c==EOF && cs==s)
		return(NULL);
	*cs++ = '\0';
	return(s);
}
