|  as68 routine for interrupt programming
.text
.globl _clockint

_clockint: movel a0,sp@-		| save C scratch regs
	movel a1,sp@-
	movel d0,sp@-
	movel d1,sp@-		| (or use movem for better performance)
	jsr _clockintc		| C will save other regs on entry to routine
	movel sp@+,d1
	movel sp@+,d0
	movel sp@+,a1
	movel sp@+,a0
	rte			| return from interrupt