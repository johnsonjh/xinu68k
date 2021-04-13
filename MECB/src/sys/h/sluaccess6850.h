/* sluaccess6850.h */
/* access macros for the MECB Serial Line Unit */
/* The specific chip is the MC6850, by Motorola */

/* get/set_control_reg6850 and slu_init6850 are in the SA and Xinu
   libraries to provide an effectively-readable UART control register
   even though the hardware SLU (Motorola 6850 "ACIA")
   has a write-only control reg.  They simply maintain a copy of the bits
   put into the control reg in a private memory location and supply them
   via get_control_reg.  Naturally, this system only works if all accesses
   to the hardware control reg. go through this package, incl. init. */
/* The transmitter control macros assume that the system is running
   SLUTRANSDE0 in SLUTRANSBITS when SLU trans ints are disabled,
   SLUTRANSIE when trans ints are enabled.  Since no Xinu system
   call allows the user to switch the port to SLUTRANSDE1 (RTS*=1,
   not 0), this works fine */

#define sluinitcsr(ioaddr)      slu_init6850(ioaddr)
#define slutransready(ioaddr)   ((ioaddr->cstat)&SLUTRANSREADY)
#define slurecvready(ioaddr)    ((ioaddr->cstat)&SLURECVREADY)
#define slugetch(ioaddr)	((ioaddr)->cbuf)
#define sluputch(ioaddr, ch)	((ioaddr)->cbuf = (ch))
#define slurenable(ioaddr)   set_control_reg6850(ioaddr,get_control_reg6850(ioaddr)|SLURECVIE)
#define slurdisable(ioaddr)  set_control_reg6850(ioaddr,get_control_reg6850(ioaddr)&~SLURECVIE)
/* avoid turning on interrupts if they are already on--may lose output */
#define slutenable(ioaddr)  if ((get_control_reg6850(ioaddr)&SLUTRANSIE)==0) \
   set_control_reg6850(ioaddr,get_control_reg6850(ioaddr)|SLUTRANSIE)
#define slutdisable(ioaddr)  set_control_reg6850(ioaddr,(get_control_reg6850(ioaddr)&~SLUTRANSIE))
/* {slusetbrk, slurstbrk} have side effect of disabling trans ints--
   caller should disable trans ints first, reinstate later */
#define slusetbrk(ioaddr)       set_control_reg6850(ioaddr,((get_control_reg6850(ioaddr)&~SLUTRANSBITS)|SLUBREAK))
#define slurstbrk(ioaddr)       set_control_reg6850(ioaddr,((get_control_reg6850(ioaddr)&~SLUTRANSBITS)|SLUTRANSDE0))
#define slugetintstate(ioaddr) get_control_reg6850(ioaddr)&SLUINTBITS
#define slusetintstate(ioaddr,oldstate) set_control_reg6850(ioaddr,oldstate| \
   (get_control_reg6850(ioaddr)&~SLUINTBITS))



