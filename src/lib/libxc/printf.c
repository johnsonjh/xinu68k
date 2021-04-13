/* printf.c - printf */

#define	OK	1
extern int console;
/*------------------------------------------------------------------------
 *  printf  --  write formatted output on console 
 *------------------------------------------------------------------------
 */
printf(fmt, args)
	char	*fmt;
{
	int	putc();

	_doprnt(fmt, &args, putc, console);
	return(OK);
}
