| set_evec.s - set_evec()

|------------------------------------------------------------------------
| set_evec -- set exception vector to point to an exception handler
|	set_evec(offset, func)
|		int offset;		/* vector number to assign */
|		int (*func)();		/* pointer to handler      */
|------------------------------------------------------------------------

	.text
	.globl	_set_evec
_set_evec:
	movc	vbr,a0		| vbr == Vector Base Register
	addl	sp@(4),a0	| compute vbr[offset]
	movl	a0@,d0		| return current value.
	movl	sp@(8),a0@	| vbr[offset] = func
	rts
