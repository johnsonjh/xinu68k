/* x_rls.c - x_rls */

#include <conf.h>
#include <kernel.h>
#include <shell.h>
#include <network.h>


/*------------------------------------------------------------------------
 *  x_rls  -  (command rls) list contents of remote file system directory
 *------------------------------------------------------------------------
 */
COMMAND	x_rls(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	char	*p, *buf;
	int	dev, len;
	char	str[256];
	struct	dirent	{	/* UNIX directory entry		*/
		long	d_inum;	/* file's inode number		*/
		short	d_rlen;	/* length of this record	*/
		short	d_nlen;	/* length of this file's name	*/
		char	d_nam[1]; /* start of file name		*/
	};
	struct	dirent	*d;
	Bool	aflag;

	aflag = FALSE;
	if (nargs > 1 && strcmp(p=args[1],"-a") == 0) {
		nargs--;
		aflag = TRUE;
		p = args[2];
        }
	if (nargs == 1)
	  p = ".";
	else if (nargs != 2) {
		printf("Usage: rls [-a] directory\n");
		return(SYSERR);
	}
	if ( ((long)(buf=(char *)getmem(512))) == SYSERR) {
		fprintf(stderr, "rls: no memory\n");
		return(SYSERR);
	}
	if (nammap(p, buf) != RFILSYS || (dev=open(NAMESPACE, p, "ro")) == SYSERR) {
		fprintf(stderr, "cannot open %s\n", p);
		freemem(buf, 512);
		return(SYSERR);
	}
	while ( (len = read(dev, buf, 512)) > 0) {
		for (p=buf ; p< &buf[512] ; p += d->d_rlen) {
			d = (struct dirent *)p;
			/* this could be a Vax or a Sun, so be */
			/* prepared to swap integer fields     */
			if (d->d_nlen != strlen(d->d_nam)) {
			    d->d_nlen = vax2h(d->d_nlen);
			    d->d_rlen = vax2h(d->d_rlen);
			}
			
			if (d->d_inum == 0)
			  continue;
			if (len < 512 ||
			    d->d_nlen != strlen(d->d_nam) ||
			    d->d_nlen > 255 ||
			    d->d_rlen < sizeof(struct dirent) ||
			    d->d_rlen > &buf[512] - p) {
			    fprintf(stderr, "Not a directory\n");
			    close(dev);
			    freemem(buf, 512L);
			    return(SYSERR);
		        }
			if (aflag || d->d_nam[0] != '.') {
				strcpy(str, d->d_nam);
				strcat(str, "\n");
				write(stdout, str, strlen(str));
			}
			if (d->d_rlen == 0)
			  break;
		}
	}
	freemem(buf, 512);
	close(dev);
	return(OK);
}
