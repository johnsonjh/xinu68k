

	.data
regs:
data:
	.ascii "D0 = %08x\11D1 = %08x\11D2 = %08x\11D3 = %08x\12\15"
	.ascii "D4 = %08x\11D5 = %08x\11D6 = %08x\11D7 = %08x\12\15"
address:
	.ascii "A0 = %08x\11A1 = %08x\11A2 = %08x\11A3 = %08x\12\15"
	.ascii "A4 = %08x\11A5 = %08x\12\15\12\15"
	.ascii "FP = %08x\11A7 = %08x\11"
	.ascii "SR = %8x\11PC = %08x\12\15\12\15\0"

	.text
	.globl	_regdump
_regdump:

	movl	sp@,	sp@-
	movw	sr, 	sp@-
	movw	#0,	sp@-
	movl	a7, 	sp@-
	subl	#8,	sp@
	subl	#60,	sp
	moveml  #0x7fff,sp@
	movl	#regs,	sp@-	
	jsr	_kprintf
	addl	#76,	sp
	rts
	.data
