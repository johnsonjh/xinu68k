/* x_cp.c - x_cp */

#include <conf.h>
#include <kernel.h>

LOCAL	char	errfmt[] = "Cannot open %s\n";

/*------------------------------------------------------------------------
 *  x_cp  -  (copy command) copy one file to another
 *------------------------------------------------------------------------
 */
COMMAND	x_cp(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	*buf;
	int	from, to;
	int	ret;
	int	len;

	if (nargs != 3) {
		fprintf(stderr, "usage: cp file1 file2\n");
		return(SYSERR);
	}
	if ( (from=open(NAMESPACE, args[1], "ro")) == SYSERR) {
		fprintf(stderr, errfmt, args[1]);
		return(SYSERR);
	}
	if ( (to=open(NAMESPACE, args[2], "w")) == SYSERR) {
		close(from);
		fprintf(stderr, errfmt, args[2]);
		return(SYSERR);
	}
	if ( ((int) (buf = (char *)getmem(512)) ) == SYSERR) {
		fprintf(stderr, "no memory\n");
		ret = SYSERR;
	} else {
		while ( (len = read(from, buf, 512)) > 0 )
			write(to, buf, len);
		freemem(buf, 512);
		ret = OK;
	}
	close(from);
	close(to);
	return(ret);
}
