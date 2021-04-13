|  as68 routines for interrupt programming
| read/xmit int for SLU2, i.e, the hostline --

.text
.globl _slu2int			| make _slu2int extern
 
 _slu2int: movel a0,sp@-		| save C scratch regs
	movel a1,sp@-
	movel d0,sp@-
	movel d1,sp@-		| (or use movem for better performance)
	jsr _slu2intc		| C will save other regs on entry to routine
	movel sp@+,d1
	movel sp@+,d0
	movel sp@+,a1
	movel sp@+,a0
	rte			| return from interrupt

