
/* console.c - conputc */

#include <conf.h>
#include <kernel.h>
#include <io.h>

/*------------------------------------------------------------------------
 *  conputc - write one character to a con device
 *------------------------------------------------------------------------
 */
conputc(devptr, ch )
struct	devsw	*devptr;
char	ch;
{
}
conoin(iptr )
register char *iptr;
{
}
coniin(iptr )
register char *iptr;
{
}
concntl(devptr, func, addr )
struct	devsw	*devptr;
int func;
char *addr;
{
}
coninit(devptr )
struct	devsw	*devptr;
{
}
congetc(devptr )
struct	devsw	*devptr;
{
}
conopen(descrp, nam, mode )
int descrp;
char *nam;
char *mode;
{
}
conread(devptr, buff, count )
struct	devsw	*devptr;
char *buff;
int count;
{
}
conwrite(devptr, buff, count)
struct	devsw	*devptr;
char *buff;
int count;
{
}
