/* ascdate.c - ascdate */

#include <conf.h>
#include <kernel.h>
#include <date.h>

/*------------------------------------------------------------------------
 *  ascdate  -  print a given date in ascii including hours:mins:secs
 *------------------------------------------------------------------------
 */
ascdate(time, str)
long	time;
char	*str;
{
	long	tmp;
	int	year, month, day, hour, minute, second;
	long	days;

	/* set year (1970-1999) */
	for (year=1970 ; TRUE ; year++) {
		days = isleap(year) ? 366 : 365;
		tmp = days * SECPERDY;
		if (tmp > time)
			break;
		time -= tmp;
	}
	/* set month (0-11) */
	for (month=0 ; month<12 ; month++) {
		tmp = Dat.dt_msize[month] * SECPERDY;
		if (isleap(year) && (month == 1))
		    tmp += SECPERDY;
		if (tmp > time)
			break;
		time -= tmp;
	}
	/* set day of month (1-31) */
	day = (int)( time/SECPERDY ) + 1;
	time %= SECPERDY;
	/* set hour (0-23) */
	hour = (int) ( time/SECPERHR );
	time %= SECPERHR;
	/* set minute (0-59) */
	minute = time / SECPERMN;
	time %= SECPERMN;
	/* set second (0-59) */
	second = (int) time;
	sprintf(str, "%3s %2d %4d %2d:%02d:%02d", Dat.dt_mnam[month],
		day, year, hour, minute, second);
	return(OK);
}

struct	datinfo	Dat = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31,
			"Jan", "Feb", "Mar", "Apr", "May", "Jun",
			"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
