/* printf.c - printf */

#define	OK	1
#define	CONSOLE	0

/*------------------------------------------------------------------------
 *  printf  --  write formatted output on CONSOLE 
 *------------------------------------------------------------------------
 */
printf(fmt, args)
	char	*fmt;
{
	int	putc();

	_doprnt(fmt, &args, putc, CONSOLE);
	return(OK);
}
