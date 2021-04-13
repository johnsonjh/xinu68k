/* x_uptime.c - x_uptime */

#include <conf.h>
#include <kernel.h>
#include <network.h>
#include <rwho.h>


#define SORTALL


/*------------------------------------------------------------------------
 *  x_uptime  -  (command uptime or ruptime) print remote machine status
 *------------------------------------------------------------------------
 */
COMMAND	x_uptime(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	i, j;
	struct	rwent	*rwptr;
	long	tottim;
	int	hours, days, mins;
	Bool	up;
	Bool	found, all;
	long	now;
	char	mach[32];
	char	str[200];
	int	rwcomp();
	int	nent;
        int	rwind[RWCSIZ];
	
	
	switch (nargs) {

	case 1:
		if (strcmp(args[0],"ruptime") == 0) {
			all = TRUE;
			break;
		}
		getname(mach); /* called as "uptime" */
		args[1] = mach;
		/* fall through */
	case 2:
		all = FALSE;
		break;
	default:
		fprintf(stderr, "use: %s [host]\n", args[0]);
		return(SYSERR);
	}
	gettime(&now);
	found = FALSE;

	nent = Rwho.rwnent;

	for (i=0 ; i<nent ; i++)
	  rwind[i] = i;

#ifdef SORTALL
	if (all) {
	    if (nent > 1)
   	        qsort((char *)rwind,nent,sizeof(int), rwcomp);
	}
#endif	

	for (i=0 ; i<nent ; i++) {
		rwptr = &Rwho.rwcache[rwind[i]];
		if (!all && strcmp(rwptr->rwmach,args[1])!=0)
			continue;
		found = TRUE;
		sprintf(str,"%-*s",RMACLEN, rwptr->rwmach);
		tottim = now - rwptr->rwlast;
		if (up = (tottim < RWCDOWN))
			tottim = rwptr->rwslast - rwptr->rwboot + 59;
		days = tottim / (24L * 60L * 60L);
		 tottim %= (24L * 60L * 60L);
		hours = tottim / (60L * 60L);
		 tottim %= (60L * 60L);
		mins = tottim / 60L;
		sprintf(&str[strlen(str)], "%s", up?"up  " : "down");
		if (days > 0)
			sprintf(&str[strlen(str)], " %2d +", days);
		else
			strcat(str, "     ");
		if ((hours>=0) && (mins>=0))
		        sprintf(&str[strlen(str)], " %2d:%02d", hours, mins);
		else
		        sprintf(&str[strlen(str)], " ??:??");
		if (!up) {
			strcat(str, "\n");
			write(stdout, str, strlen(str));
			continue;
		}
		sprintf(&str[strlen(str)],
			"  %2d users,    load ", (int)rwptr->rwusers);
		for (j=0 ; j<RWNLOAD ; j++) {
			if (j >0)
				strcat(str, ", ");
			sprintf(&str[strlen(str)], "%2d.%02d",
				rwptr->rwload[j] / 100,
				rwptr->rwload[j] % 100);
		}
		strcat(str, "\n");
		write(stdout, str, strlen(str));
	}
	if (!found && !all)
		fprintf(stderr, "%s ???\n", args[1]);
	return(OK);
}


/*
 * rwcomp -- used by qsort
 */
rwcomp(px, py)
     int *px, *py;
{
    return(strcmp(Rwho.rwcache[*px].rwmach,Rwho.rwcache[*py].rwmach));
}
