/* fs.c - main */

#include "fs.h"

struct	filent	fcache[MAXNFILES + 1];	/* Table of all open files	*/
int	freelst;			/* pointer to fcache free list	*/
int	port;				/* UDP port. (if <0 use pname)	*/
char	*pname;				/* UDP port name if port < 0	*/

/* Areas to hold incoming request packet and address of sender */

struct	fpacket	req;			/* actual request/reply packet	*/
struct	sockaddr_in	faddr;		/* sender's address saved here	*/
int	flen;				/* length of sender's address	*/

struct	reqmap	rq[] = {		/* table  of requests & handlers*/
	FS_OPEN,	fsopen,
	FS_CLOSE,	fsclose,
	FS_READ,	fsread,
	FS_WRITE,	fswrite,
	FS_UNLINK,	fsunlink,
	FS_RENAME,	fsrename,
	FS_MKDIR,	fsmkdir,
	FS_RMDIR,	fsrmdir,
	FS_ACCESS,	fsaccess,
	FS_TRANS,	fstrans};
#define	NREQS		(sizeof(rq)/sizeof(struct reqmap))

/*------------------------------------------------------------------------
 *  main  -  4.3BSD UNIX-based file server for Xinu V7  Diskless Node
 *------------------------------------------------------------------------
 */
main(argc, argv)
int	argc;
char	*argv[];
{
	int	len;			/* length of request		*/
	int	sndlen;			/* length of reply or zero	*/
	int	reqop;			/* operation in latest request	*/
	int	i;			/* index of request in rq	*/
	int	sock;			/* Internet datagram socket	*/

	sock = finit(argc, argv);
	while (TRUE) {
		flen = sizeof(faddr);
		if ( (len=fgetreq(sock, &req, &faddr, &flen)) == SYSERR)
			continue;
		reqop = req.fp_h.f_op;
		for (i=0 ; i<NREQS ; i++) {
			if (rq[i].req == reqop)
				break;
		}
		if (i >= NREQS || req.fp_h.f_name[0] != '/') {
			req.fp_h.f_op = FS_ERROR;
			sndlen = FHDRLEN;
		} else {
			sndlen = (*rq[i].reqproc)(&req, len);
		}
		if (sndlen > 0)
			fsndrply(sock, &req, sndlen, &faddr, flen);
	}
}
