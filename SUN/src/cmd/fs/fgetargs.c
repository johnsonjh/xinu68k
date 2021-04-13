/* fgetargs.c - fgetargs, fexit */

#include "fs.h"

/*------------------------------------------------------------------------
 *  fgetargs  -  parse arguments to file server and record information
 *------------------------------------------------------------------------
 */
fgetargs(argc, argv)
int	argc;
char	*argv[];
{
	char	ch;
	pname = PORTNAME;
	port = -1;
	if (argc == 1)
		return(OK);
	if (argc > 2 || strncmp(argv[1],"-p",2)!=0)
		fexit();
	argv[1]+=2;
	if ((ch= *argv[1]) >= '0' && ch <= '9') {
		if ((port = atoi(argv[1])) <= 0)
			fexit();
	} else {
		pname = argv[1];
	}
	return(OK);
}

/*------------------------------------------------------------------------
 *  fexit  -  print use message and exit
 *------------------------------------------------------------------------
 */
fexit()
{
	printf("fs: proper use is fs [-pPORT]\n");
	exit(1);
}
