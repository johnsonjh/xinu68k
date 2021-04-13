/* io.h -  fgetc, fputc, getchar, isbaddev, putchar */


#define	INTVECI	inint      	/* input interrupt dispatch routine	*/
#define	INTVECO	outint		/* output interrupt dispatch routine	*/
extern	int	INTVECI();
extern	int	INTVECO();
#define BASE    060		/* base address of the autovector table */
				/* L.L.					*/
#define OFFSET	04		/* offset of each autovector, L.L.	*/

struct	intmap	{		/* device-to-interrupt routine mapping	*/
	int	(*iin)();	/* address of input interrupt routine	*/
	int	icode;		/* argument passed to input routine	*/
	int	(*iout)();	/* address of output interrupt routine	*/
	int	ocode;		/* argument passed to output routine	*/
	};

#ifdef	NDEVS
extern	struct	intmap intmap[NDEVS];
#define	isbaddev(f)	( (f)<0 || (f)>=NDEVS )
#endif
#define	BADDEV		-1

/* In-line I/O procedures */

#define	getchar()	getc(CONSOLE)
#define	putchar(ch)	putc(CONSOLE,(ch))
#define	fgetc(unit)	getc((unit))
#define	fputc(unit,ch)	putc((unit),(ch))
#define level(vec)      ((vec-BASE)/OFFSET)     /* determine autovector level */
                                                /* L.L.                       */

struct	vector	{
	int     (*vproc)();     /* address of interrupt procedure       */
};

extern	struct vector disptab[];

/* fd_to_dd()  converts file descriptor to device descriptor
 * -- to be used in converting old XINU code to new I/O formats
 */
#define fd_to_dd(f)	(proctab[getpid()].fildes[f])

#define findfd()	(dd_to_fd(FDFREE))

/* Unix open flags */
#define	O_RDONLY	000		/* open for reading */
#define	O_WRONLY	001		/* open for writing */
#define	O_RDWR		002		/* open for read & write */
#define	O_NDELAY	00004		/* non-blocking open */
#define	O_APPEND	00010		/* append on each write */
#define	O_CREAT		01000		/* open with file create */
#define	O_TRUNC		02000		/* open with truncation */
#define	O_EXCL		04000		/* error on create if file exists */

/* conversion cheats -- to aid in upgrading old XINU code
 *			to the new unix like I/O calls      
 */
#define o_r		O_RDONLY
#define o_w		O_WRONLY
#define	o_rw		O_RDWR
#define o_o		0
#define o_n		O_CREAT | O_EXCL
#define o_dc		O_CREAT		/* don't care, old or new */

#define o_rn		o_r | o_n
#define o_ro		o_r | o_o
#define	o_rdc		o_r | o_dc
#define o_wn		o_w | o_n
#define o_wo		o_w | o_o
#define o_wdc		o_w | o_dc
#define o_rwn		o_rw | o_n
#define o_rwo		o_rw | o_o
#define o_rwdc		o_rw | o_dc
