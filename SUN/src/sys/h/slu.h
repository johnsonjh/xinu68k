/* slu.h */

#define SVECTOR 0x0078		/* location of zilog interrupt vector (6) */
#define ZSCC_PCLK 4915200
#define SERIAL0_BASE    ((struct zscc_device *)         0x0FE02000)
#define EVEC_LEVEL6     0x078

/*
 * Delay units are in microseconds.
 */
#define	DELAY(n)	\
{ \
        extern int cpudelay; \
	register int N = (((n)<<4) >> cpudelay); \
 \
	while (--N > 0) ; \
}



