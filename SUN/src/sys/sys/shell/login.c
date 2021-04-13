/* login.c - login */

#include <conf.h>
#include <kernel.h>
#include <shell.h>

/*------------------------------------------------------------------------
 *  login  -  log user onto system
 *------------------------------------------------------------------------
 */
login(dev)
int	dev;
{
	int	len;

	Shl.shused = FALSE;
	Shl.shuser[0] = NULLCH;
	for (getname(Shl.shmach) ; TRUE ; ) {
		fprintf(dev,"\n\n%s - The magic of Xinu\n\nlogin: ",
			Shl.shmach);
		while ( (len=read(dev,Shl.shbuf,SHBUFLEN))==0 || len==1)
			fprintf(dev, "login: ");
		if (len == EOF) {
			read(dev, Shl.shbuf, 0);
			Shl.shused = FALSE;
			continue;
		}
		Shl.shbuf[len-1] = NULLCH;
		strncpy(Shl.shuser, Shl.shbuf, SHNAMLEN-1);
		Shl.shused = TRUE;
		getutim(&Shl.shlogon);
		mark(Shl.shmark);
		fprintf(dev,"\n%s\n\n",
		"      Welcome to Xinu (type ? for help)" );
		getutim(&Shl.shlast);
		return(OK);
	}
}
