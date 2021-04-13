/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*------------------------------------------------------------------------
 *  main  --  run UDP echo server and Xinu pseudo-shell
 *------------------------------------------------------------------------
 */
main()
{
	int	eserver();
	Bool	first;

	/* start UDP echo server */

	resume( create(eserver, 400, 30, "UDPecho", 0) );

	for (first=TRUE ; TRUE ; first=FALSE) {
		pshell(first);
	}
}

static	struct	xgram	buff;	/* Here because the stack is small */

/*------------------------------------------------------------------------
 *  eserver  -  implement UDP echo server that runs in background
 *------------------------------------------------------------------------
 */
eserver()
{
	int	echodev;
	int	len;

	echodev = open(NETWORK, ANYFPORT, UECHO);

	while (TRUE) {
		len = read(echodev, &buff, sizeof(struct xgram));
		if (len == SYSERR)
			continue;
		write(echodev, &buff, len);
	}
}

/*------------------------------------------------------------------------
 *  dumpit  -  really just to add dump routines to memory image for debug
 *------------------------------------------------------------------------
 */
dumpit()
{
/*T*/
	prdump();
	rfdump();
	dgdump();
	adump();
/*T*/
}
