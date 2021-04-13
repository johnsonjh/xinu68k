|* setclkr.s - setclkr *|


|*------------------------------------------------------------------------
|* setclkr  --  set cklruns to 1 iff real-time clock exists, 0 otherwise
|*------------------------------------------------------------------------
	.data
	.text
	.globl	_setclkr
_setclkr:

	movl	#1,_clkruns	| On the MECB or ATT7300 there's always
	rts			| a clock
