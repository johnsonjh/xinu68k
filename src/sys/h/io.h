/* io.h -  fgetc, fputc, getchar, isbaddev, putchar */


#define	INTVECI	inint		/* input interrupt dispatch routine	*/
#define	INTVECO	outint		/* output interrupt dispatch routine	*/
extern	int	INTVECI();
extern	int	INTVECO();

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

/* In-line I/O procedures */

#define	getchar()	getc(CONSOLE)
#define	putchar(ch)	putc(CONSOLE,(ch))
#define	fgetc(unit)	getc((unit))
#define	fputc(unit,ch)	putc((unit),(ch))

struct	vector	{
	char	*vproc;		/* address of interrupt procedure	*/
	WORD	vps;		/* saved process status word		*/
};

extern	struct vector disptab[64];
