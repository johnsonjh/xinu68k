/* dtc.h */

/* Data Technology Corp. DTC-11-1 disk controller host interface */

struct	dtc	{			/* controller interface regs.	*/
	int	dt_ccsr;		/* command completion status	*/
	int	dt_csr;			/* control and status register	*/
	char	*dt_dar;		/* data address register	*/
	struct	xbdcb	*dt_car;	/* command address register	*/
	int	dt_xdar;		/* extension of dar (not used)	*/
	int	dt_xcar;		/* extension of car (not used)	*/
};

/* bits in the dtc csr register */

#define	DTGO	0000001			/* "go" bit - start interface	*/
#define	DTRESET	0000002			/* "force reset" bit		*/
#define	DTINTR	0000100			/* enable interface interrupt	*/
#define	DTDONE	0000200			/* command done			*/
#define	DTERROR	0100000			/* some error occurred		*/
