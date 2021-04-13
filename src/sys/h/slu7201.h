/* slu7201.h--only used for ATT7300, since MECB has no such SLU */
/* The NEC7201 is a dual UART, used for console on ATT7300 because of
   its programmable baudrates as defined below */

/* console is channel A: #define SLUCSR1 (struct csr *)0xe50000  */
/* channel B: #define SLUCSR2 (struct csr *)0xe50002 */

/* magic addrs to write to to select baud rate for console SLU1-- */
#define SLUSELECT38400 ((char *)0x4b0001)
#define SLUSELECT19200 ((char *)0x4b0002)
#define SLUSELECT9600 ((char *)0x4b0004)
#define SLUSELECT4800 ((char *)0x4b0008)
#define SLUSELECT2400 ((char *)0x4b0010)
#define SLUSELECT1200 ((char *)0x4b0020)

/* control/status regs--write reg # to cstat, then read or write stat/cmd */
/* valid when current reg = 0, the base state we stay in most of the time-- */
#define SLUREGMASK     0x7             /* bits for reg # for next access */
#define SLURESET      (3<<3)            /* reg 0 cmd to reset chip */
/* control reg defaults-- program reg2, then reg 4, then others */
/* reread data sheet before trying to use channel B */
#define SLUCREG1DEF   0              /* no ints */
#define SLUTRANSIE    (1<<1)         /* trans int ena */
#define SLURECVIE     (2<<3)         /* recv int on ea char recvd */
#define SLUINTBITS    (0x3<<3|SLUTRANSIE) /* 2bits receiver, 1 bit xmit int */
#define SLUCREG2DEF   0              /* non-DMA, nonvect ints, etc. */
#define SLUCREG3DEF   (0xc1)         /* 8 bits on recv, recv ena, inactive cts*/
#define SLUCREG4DEF   (0x44)          /* 16x clock,async,1stopbit,no par.*/
#define SLUCREG5DEF    0x68          /* 8 bits,no brk,trans ena,rts lev,no crc*/
#define SLUCREG5DTR    (1<<7)         /* 1 here makes DTR* low (asserted) */
/* status reg 0 */
#define	SLUTRANSREADY	(1<<2)		/* device ready bit		*/
#define SLURECVREADY	(1<<0)		/* receiver ready bit */
#define SLUASCII7BITS   0177    	/* mask for 7 bits of ASCII char,
				          to mask off parity bit */
#define SLUCHMASK      0x7f             /* 7 ASCII bits */

struct	csr	{
	unsigned short cbuf;	/* write for transmit, read for input */
	short dummy;            /* skip other reg in between */
	unsigned short cstat;	/* read for status, write for control reg */
};

