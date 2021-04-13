|  as68 routines for interrupt programming
| read/xmit int for SLU3, i.e, the hostline --

.text
.globl _slu3int			| make _slu3int extern
 
 _slu3int: movel a0,sp@-		| save C scratch regs
	movel a1,sp@-
	movel d0,sp@-
	movel d1,sp@-		| (or use movem for better performance)
	jsr _slu3intc		| C will save other regs on entry to routine
	movel sp@+,d1
	movel sp@+,d0
	movel sp@+,a1
	movel sp@+,a0
	rte			| return from interrupt

