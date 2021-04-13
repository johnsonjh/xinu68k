/* fsrename.c - fsrename */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fsrename  -  handle rename request and prepare reply
 *------------------------------------------------------------------------
 */
fsrename(fpacptr, flen)
struct	fpacket	*fpacptr;
int	flen;
{
	struct	fphdr	*fptr;
	int	rfd;
	int	len;
	int	from, to;		/* for copying, from & to files	*/
	char	buf[4096];		/* file copy buffer area	*/

	fptr = &fpacptr->fp_h;
	if (rename(fptr->f_name, fpacptr->fpdata) < 0) {
		if (errno != EXDEV) {
			fptr->f_op = FS_ERROR;
			Uncache(fptr->f_name);
			Uncache(fpacptr->fpdata);
			return(FHDRLEN);
		}
		/* Copy the data to target file system */
		if ((from=openfile(fptr->f_name, FLOLD|FLREAD)) < 0) {
			fptr->f_op = FS_ERROR;
			return(FHDRLEN);
		}
		if ((rmfile(fpacptr->fpdata) < 0 && errno != ENOENT) ||
			(to=openfile(fpacptr->fpdata, FLWRITE)) < 0) {
			fptr->f_op = FS_ERROR;
			close(from);
			return(FHDRLEN);
		}
		while ((len=read(from, buf, sizeof(buf))) > 0)
			if ((len=write(to, buf, len)) < 0)
				break;
		close(from);
		close(to);
		if (len != 0) {		/* error transferring data	*/
			fptr->f_op = FS_ERROR;
			rmfile(fpacptr->fpdata);
			return(FHDRLEN);
		}
		if (rmfile(fptr->f_name) < 0) {
			fptr->f_op = FS_ERROR;
			rmfile(fpacptr->fpdata);
			return(FHDRLEN);
		}
		Uncache(fptr->f_name);
		Uncache(fpacptr->fpdata);
		return(FHDRLEN);
	}
	return(flen);
}
