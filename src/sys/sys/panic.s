DISABLE	=	0x2700		| PS to disable interrupts
ENABLE	=	0x2000		| PS to enable interrupts

	.text
	.globl panic, _panic

|------------------------------------------------------------------------
|  panic  --  entry point when called by exception processor
|------------------------------------------------------------------------

panic:
	movl	#panmsg2, sp@-
	jmp	continue

|------------------------------------------------------------------------
|  _panic  --  entry point when called by user programs
|------------------------------------------------------------------------

_panic:
	movl	#panmsg1,sp@-
continue:
	jsr	_kprintf
	jsr	_regdump
loop:	jmp	loop 
	.data
panmsg1:
	.asciz 	"Panic trap: Return Address = 0x%x\12\15Message: %s\12\15"
	.even
panmsg2:
	.asciz 	"Panic trap: Return Address = 0x%x\12\15Vector: %x\12\15"
	.even
	.skip	300
	.globl kernstk
	.even
kernstk:
	.long	0
