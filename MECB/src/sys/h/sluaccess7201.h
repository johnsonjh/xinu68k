/* sluaccess7201.h */
/* assumes inclusion of slu7201.h */
/* The specific chip is the NEC7201A dual UART */

/* get/set_control_reg7201 and slu_init7201 are in the SA and Xinu libraries.
   get/set_control_reg7201 provide an effectively-readable UART control
   register set even though the hardware SLU, a NEC 7201A,
   has 5 write-only control regs.  They simply maintain a copy of the bits
   put into the control reg in a private memory location and supply them
   via get_control_reg.  Naturally, this system only works if all accesses
   to the hardware control reg. go through this package. */

#define sluinitcsr(ioaddr)    slu_init7201(ioaddr)
#define slutransready(ioaddr)      ((ioaddr->cstat)&SLUTRANSREADY)
#define slurecvready(ioaddr)       ((ioaddr->cstat)&SLURECVREADY)
#define slugetch(ioaddr)	(((ioaddr)->cbuf)&SLUCHMASK)
#define sluputch(ioaddr, ch)	((ioaddr)->cbuf = (ch))
#define slurenable(ioaddr)   set_control_reg7201(ioaddr,get_control_reg7201(ioaddr,1)|SLURECVIE,1)
#define slurdisable(ioaddr)  set_control_reg7201(ioaddr,get_control_reg7201(ioaddr,1)&~SLURECVIE,1)
/* reenable trans ints if nec, start output with null-- */
#define slutenable(ioaddr) slu_tstart7201(ioaddr)
#define slutdisable(ioaddr)  set_control_reg7201(ioaddr,get_control_reg7201(ioaddr,1)&(~SLUTRANSIE),1)
/* unimplemented-- */
#define slusetbrk(ioaddr)
#define slurstbrk(ioaddr)
#define slugetintstate(ioaddr) get_control_reg7201(ioaddr,1)&SLUINTBITS
#define slusetintstate(ioaddr,oldstate) set_control_reg7201(ioaddr,oldstate| \
   (get_control_reg7201(ioaddr,1)&~SLUINTBITS),1)

