/* x_mount.c - x_mount, mprint */

#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <name.h>

#define	PADTO	24

/*------------------------------------------------------------------------
 *  x_mount  -  (command mount) change or display namespace table
 *------------------------------------------------------------------------
 */
COMMAND	x_mount(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	dev;

	if (nargs == 1)
		return(mprint(stdin, stdout, stderr));
	if (nargs != 4) {
		fprintf(stderr,"use: mount [prefix device new_prefix]\n");
		return(SYSERR);
	}
	for (dev=0 ; dev<NDEVS ; dev++)
		if (strcmp(args[2], devtab[dev].dvname) == 0)
			break;
	if (dev >=  NDEVS)
		if (strcmp(args[2],"SYSERR") == 0)
			dev = SYSERR;
		else {
			fprintf(stderr, "Device %s not found\n", args[2]);
			return(SYSERR);
		}
	if (mount(args[1], dev, args[3]) == SYSERR) {
		fprintf(stderr, "Mount failed\n");
		return(SYSERR);
	}
	return(OK);
}


/*------------------------------------------------------------------------
 *  mprint  -  print the current contents of the namespace prefix table
 *------------------------------------------------------------------------
 */
LOCAL	mprint(stdin, stdout, stderr)
int	stdin, stdout, stderr;
{
	struct	nament	*nptr;
	int	i, len, dev;
	char	str[80];
	char	*p;

	for (i=0 ; i<Nam.nnames ; i++) {
		nptr = & Nam.nametab[i];
		sprintf(str, "\"%-s\"", nptr->npre);
		for (len=strlen(str) ; len < PADTO ; len++)
			str[len] = ' ';
		write(stdout, str, PADTO);
		dev = nptr->ndev;
		p = isbaddev(dev) ? "SYSERR" : devtab[dev].dvname;
		sprintf(str, " -> (%-9s) \"%s\"\n", p, nptr->nrepl);
		write(stdout, str, strlen(str));
	}
	return(OK);
}
