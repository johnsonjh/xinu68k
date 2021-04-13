/* conf.c (GENERATED FILE; DO NOT EDIT) */

#include <conf.h>

/* device independent I/O switch */

struct	devsw	devtab[NDEVS] = {

/*  Format of entries is:
device-number, device-name,
init, open, close,
read, write, seek,
getc, putc, cntl,
device-csr-address, input-vector, output-vector,
iint-handler, oint-handler, control-block, minor-device,
*/

/*  CONSOLE  is tty  */

0, "CONSOLE",
ttyinit, ionull, ionull,
ttyread, ttywrite, ioerr,
ttygetc, ttyputc, ttycntl,
01770020004, 0170, 0170,
ioerr, ioerr, NULLPTR, 0,

/*  ETHER  is eth  */

1, "ETHER",
ethinit, ioerr, ioerr,
ethread, ethwrite, ioerr,
ioerr, ioerr, ethcntl,
01770200000, 0154, 0154,
ethinter, ethinter, NULLPTR, 0,

/*  INTERNET  is dgm  */

2, "INTERNET",
ionull, dgmopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, dgmcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM1  is dg  */

3, "DGRAM1",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  DGRAM2  is dg  */

4, "DGRAM2",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  DGRAM3  is dg  */

5, "DGRAM3",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  DGRAM4  is dg  */

6, "DGRAM4",
dginit, ioerr, dgclose,
dgread, dgwrite, ioerr,
ioerr, ioerr, dgcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  RFILSYS  is rfm  */

7, "RFILSYS",
ioerr, rfopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE1  is rf  */

8, "RFILE1",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0,

/*  RFILE2  is rf  */

9, "RFILE2",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 1,

/*  RFILE3  is rf  */

10, "RFILE3",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 2,

/*  RFILE4  is rf  */

11, "RFILE4",
rfinit, ioerr, rfclose,
rfread, rfwrite, rfseek,
rfgetc, rfputc, rfcntl,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 3,

/*  NAMESPACE  is nam  */

12, "NAMESPACE",
naminit, namopen, ioerr,
ioerr, ioerr, ioerr,
ioerr, ioerr, ioerr,
0000000, 0000, 0000,
ioerr, ioerr, NULLPTR, 0
	};
