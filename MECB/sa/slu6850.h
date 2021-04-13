/* slu6850.h--register defs for Motorola 6850 SLU chip */
/* MC6850 Serial Line Unit (also known as UART, or ACIA) */
/* MECB has two of these for console and hostline, each with jumpered
   baudrate up to 9600 baud */
/* ATT7300 has one for serial keyboard interface, baudrate hardwired
   for 1200 baud, so not used as console */

/* control register bits-- */
#define SLURESET	3		/* write to cntrl reg to reset dev */
#define SLUDIV16	1		/* division ratio of 16--normal op */
#define SLUFMTBITS      (7<<2)	        /* 3 bits--8 formats, 2 def'd here--*/
#define SLU8NP1         (5<<2)	        /* dataformat of 8 bits, no parity,
					   one stop bit (most common) */
#define SLU7EP1		(2<<2)          /* dataformat of 7 bits, even parity,
					   one stop bit (second most common)*/
#define SLUTRANSBITS    (3<<5)	        /* 2 bits--following 4 cases-- */
#define SLUTRANSDE0	(0<<5)		/* no transmitter interrupt, RTS*=0 */
#define SLUTRANSDE1	(2<<5)		/* no transmitter int, RTS*=1 */
#define SLUTRANSIE	(1<<5)		/* transmit int ena:bit5 on,bit6 off*/
#define SLUBREAK	(3<<5)		/* no trans int, send break signal 
					   (continuous space logic level) */
#define SLURECVDE	(0<<7)		/* no receiver int */
#define SLURECVIE	(1<<7)		/* receiver int enabled */
#define SLUINTBITS      (7<<5)          /* all off for no ints, RTS*=0 */
#define SLUDEFAULT	(SLURECVDE|SLUTRANSDE0|SLU8NP1|SLUDIV16)
					/* counter divide 16, dataformat of
					   8 bits, no parity, 1 stop bit,
					   RTS*=0, no interrupts */
/* status register bits-- */
#define SLURECVREADY	(1<<0)		/* receiver ready bit */
#define	SLUTRANSREADY	(1<<1)	        /* transmitter ready bit */
#define SLUDCD		(1<<2)		/* data carrier detect bit (always 0
					   on mecb--DCD pin is wired to gnd)*/
#define SLUCTS		(1<<3)		/* clear to send bit--reports logic
					   level on CTS pin, wired to DTR on
					   console, CTS on hostline port */
#define SLUFE		(1<<4)		/* framing error (bad stop bit) */
#define SLUOVRN		(1<<5)		/* overrun err--input char was lost */
#define SLUPE		(1<<6)		/* parity err--can't happen in 8NP1 */
#define SLUIRQ		(1<<7)		/* 6850 signaling trans or recv int */
#define	SLUERMASK	(SLUPE|SLUOVRN|SLUFE) /* mask for error flags */

/* for receive register-- */
#define	SLUCHMASK	0xff		/* mask for input character	*/
#define SLUASCII7BITS   0x7f    	/* mask for 7 bits of ASCII char,
				          to mask off parity bit */

/* MC6850 device register layout 	*/
/* The 6850 uses 2 memory-mapped byte register locations, at addr n,n+2 */
/* for example, on MECB-- */
/* 10040: port 1 csr (control and status register) */
/* 10041: port 2 csr */
/* 10042: port 1 data buf */
/* 10043: port 2 data buf */
/* so look at it as port 1-- csr, dummy, data buf starting at 10040 */
/*        and       port 2-- csr, dummy, data buf starting at 10041 */
/* Further, each address accesses 2 different hardware registers */
/*  in the chip, one on read and another on write. */
/* 10040, 10041-- status register on read, control register on write */
/* 10042, 10043-- read buffer on read, write buffer on write */

struct	csr	{
	unsigned char cstat;	/* read for status, write for control reg */
	char dummy;	  /* skip a byte--only uses even, or odd, addrs */
	unsigned char cbuf;	/* write for transmit, read for input */
};

