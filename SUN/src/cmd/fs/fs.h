/* fs.h */

#include "../../sys/h/kernel.h"		/*** Xinu include files ***/
#include "../../sys/h/file.h"
#include "../../sys/h/fserver.h"
#undef   EOF				/*** remove conflicts ***/
#undef   NULL
#include <sys/file.h>			/*** UNIX include files ***/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <strings.h>
#include <errno.h>
#include <netdb.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>

/* Defines for miscellaneous constants */

#define PORTNAME	"wizard"	/* UDP port name server uses	*/
#define FILEMODE	0664		/* protection mode for new files*/
#define DIRMODE		0775		/* protection mode for new dirs	*/
#define	MAXNFILES	15		/* maximum number of open files	*/
#define	NOFILE		MAXNFILES	/* last file entry is not a file*/
#define	FHDRLEN		sizeof(struct fphdr)/* request/reply header only*/

/* Declaration of file table entry format */

struct	filent	{			/* entry in table of files	*/
	char	name[RNAMLEN];		/* file's name			*/
	int	fd;			/* UNIX file descriptor to use	*/
	int	mode;			/* mode of file			*/
	long	currpos;		/* current offset for this file	*/
	int	prev;			/* link to previously opened fl	*/
	int	next;			/* link to next file opened	*/
};
extern	struct	filent	fcache[];	/* table of open files		*/
extern	int	freelst;		/* first free entry in fcache	*/
#define	NEWEST  fcache[MAXNFILES].prev	/* youngest file in file table	*/
#define OLDEST  fcache[MAXNFILES].next	/* oldest file in file table	*/

/* Declaration of the request-to-handler mapping */

struct	reqmap	{			/* request-to-procedure mapping	*/
	int	req;			/* request (e.g., FS_OPEN)	*/
	int	(*reqproc)();		/* procedure to handle request	*/
};
extern	struct	reqmap	rq[];		/* array of request mappings	*/
extern	int	fsopen(), fsclose(), fsread(), fswrite(), fsunlink(),
		fsrename(),fsrmdir(),fsmkdir(),fsaccess(),fstrans();
extern	int	errno;			/* UNIX's error code return var.*/
extern	int	port;			/* UDP port number to use or -1	*/
extern	char	*pname;			/* UDP port name to use		*/
