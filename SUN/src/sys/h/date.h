/* date.h - net2xt, xt2net, isleap, ut2ltim */

/* Xinu stores time as seconds past Jan 1, 1970 (UNIX format), with	*/
/* 1 being 1 second into Jan. 1, 1970, GMT (universal time).  The	*/
/* Internet uses seconds past Jan 1, 1900 (also GMT or universal time)	*/

#define	net2xt(x)	((x)-2208988800L)  /* convert net-to-xinu time	*/
#define	xt2net(x)	((x)+2208988800L)  /* convert xinu-to-net time	*/

/* Days in months and month names used to format a date */

struct	datinfo	{
	int	dt_msize[12];
	char	*dt_mnam[12];
};

extern	struct	datinfo	Dat;

/* Constants for converting time to month/day/year/hour/minute/second	*/

#define	isleap(x)	((x)%4==0)	/* leap year? (1970-1999)	*/
#define	SECPERDY	(60L*60L*24L)	/* one day in seconds		*/
#define	SECPERHR	(60L*60L)	/* one hour in seconds		*/
#define	SECPERMN	(60L)		/* one minute in seconds	*/

/* date doesn't understand daylight savings time (it was built in	*/
/*	Indiana where we're smart enough to realize that renumbering	*/
/*	doesn't save anything).	However, the local time zone can be	*/
/*	set to EST, CST, MST,or PST.					*/

#define	ZONE_EST	5		/* Eastern Standard time is 5	*/
#define	ZONE_CST	6		/*  hours west of England	*/
#define	ZONE_MST	7
#define	ZONE_PST	8
#define	TIMEZONE	ZONE_EST	/* timezone for this system	*/

/* In-line procedures to convert universal-to-local time and vice versa	*/

#define	ut2ltim(x)	((x)-TIMEZONE*SECPERHR)
#define	ltim2ut(x)	((x)+TIMEZONE*SECPERHR)

#ifndef	TSERVER
#define	TSERVER		"128.10.2.3:37"
#endif
