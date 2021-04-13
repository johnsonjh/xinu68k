DISABLE	=	0x2700
	.data
	.text
	.globl	__disable
__disable:
	movl	a0,	sp@-
	movl	sp@(8),	a0
	movw	sr,	a0@
	movw	#DISABLE,sr
	movl	sp@+,	a0
	rts

	.globl __restore
__restore:
	movl	a0,	sp@-
	movl	sp@(8),	a0
	movw	a0@,	sr
	movl	sp@+,	a0
	rts

|for initial lowering of priority, after device initialization--

LOWPRI	=	0x2000

	.globl	_setlowpri

_setlowpri:
	movw	#LOWPRI,sr	| set CPU SR register to 0x2000
	rts

