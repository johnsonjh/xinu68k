|
|  trap.s -- handle Xinu traps elegantly so we can return to the monitor
	.globl _trapX


	.text
_trapX:
	jbsr	_doevec
	rte

