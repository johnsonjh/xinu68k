/* sleep.h */

#define	C5VECTOR	0x0074	/* Clock vector for autovector 5	*/
#define	C7VECTOR	0x007c	/* Clock vector for autovector 7	*/


extern	int	clkruns;	/* 1 iff clock exists; 0 otherwise	*/
				/* Set at system startup.		*/
extern	int	clockq;		/* q index of sleeping process list	*/
extern	int	count6;		/* used to ignore 5 of 6 interrupts	*/
extern	int	count10;	/* used to ignore 9 of 10 ticks		*/
extern	WORD	clktime;	/* current time in secs since 1/1/70	*/
extern	int	clmutex;	/* mutual exclusion sem. for clock	*/
extern	int	*sltop;		/* address of first key on clockq	*/
extern	int	slnempty;	/* 1 iff clockq is nonempty		*/

extern	int	defclk;		/* >0 iff clock interrupts are deferred	*/
extern	int	clkdiff;	/* number of clock clicks deferred	*/
extern	int	clkint();	/* clock interrupt handler		*/
