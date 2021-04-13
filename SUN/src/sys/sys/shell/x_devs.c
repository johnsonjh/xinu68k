/* x_devs.c - x_devs */

#include <conf.h>
#include <kernel.h>

LOCAL	char hd1[] =
	"Num  Device   minor   CSR    i-vect.  o-vect.   cntrl blk\n";
LOCAL	char hd2[] =
	"--- --------  ----- -------- -------- --------  ---------\n";

/*------------------------------------------------------------------------
 *  x_devs  -  (command devs) print main fields of device switch table
 *------------------------------------------------------------------------
 */
COMMAND	x_devs(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	struct	devsw	*devptr;
	char	str[60];
	int	i;

	write (stdout, hd1, strlen(hd1) );
	write (stdout, hd2, strlen(hd2) );
	for (i=0 ; i<NDEVS ; i++) {
		devptr = &devtab[i];
		sprintf(str, "%2d. %-9s %3d   %08x %08x %08x  %08x\n",
			i, devptr->dvname, devptr->dvminor,
			devptr->dvcsr, devptr->dvivec, devptr->dvovec,
			devptr->dvioblk);
		write(stdout, str, strlen(str));
	}
}
