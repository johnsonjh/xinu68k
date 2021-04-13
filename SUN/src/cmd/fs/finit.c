/* finit.c - finit */

#include "fs.h"

/*------------------------------------------------------------------------
 *  finit  -  initialize data structures  for file server; return socket
 *------------------------------------------------------------------------
 */
finit(argc, argv)
int	argc;
char	*argv[];
{
	int	pid;
	int	i;
	int	sock;
	struct	servent	*servptr;
	struct	servent	*getservbyname();
	struct	sockaddr_in	sockadr;

	freelst = 0;
	fgetargs(argc, argv);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (port < 0) {
		if ( (servptr=getservbyname(pname, "udp")) == NULL)
			exit(2);
		port = servptr->s_port;
	} else
		port = htonl(port);
	sockadr.sin_family = AF_INET;
	sockadr.sin_port = port;
	if (bind(sock, &sockadr, sizeof(sockadr)) < 0)
		exit(3);
	NEWEST = OLDEST = NOFILE;
	umask(0);

	/* Initialize open file information cache */

	for (i=0 ; i<MAXNFILES ; i++) {
		fcache[i].prev = NOFILE;
		fcache[i].next = i==MAXNFILES ? NOFILE : i+1;
	}
	if ( (pid=fork()) < 0)		/* if fork fails, abort		*/
		exit(4);
	else if (pid > 0)		/* let parent exit so child can	*/ 
		exit(0);		/*  run in background		*/
	return(sock);
}
