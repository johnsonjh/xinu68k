|* ctxsw.s - ctxsw

|*-----------------------------------------------------------------------------
|* ctxsw  --	actually perform context switch, saving and loading registers
|*		This routine is executed in kernel mode.
|*
|*		Upon entry, d0-d7, a0-a6, the ssp, the sr, and the pc are
|*		saved.  The usp is ignored.
|*
|*		Upon exit, all registers saved are restored, and the value
|*		of the supervisor stack pointer is copied into the user 
|*		stack pointer.
|*-----------------------------------------------------------------------------

	.data
	.text
	.globl	_ctxsw
_ctxsw:
	movl	a0, 	sp@(-4)		; Save A0 onto old stack	
	movl	sp@(4),	a0		; Move address of old area into A0 
	moveml	#0xffff,a0@		; Save all registers		
	movl	sp@(-4),a0@(32)		; Put original A0 in old savearea	
	addl	#64,	a0		; Skip past registers d0-7,a0-7
	movw	#0,	a0@+		; Pad SR savearea, since SR is a word
	movw	sr,	a0@+		; Save SR in old savearea	
	movl	sp@,	a0@+		; Save PC in old savearea	

|Save image:d0,d1,d2,d3,d4,d5,d6,d7,a0,a1,a2,a3,a4,a5,a6,ssp,0x00, sr, pc
|            0  4  8 12 16 20 24 28 32 36 40 44 48 52 56  60  64   66  68 

	movl	sp@(8),	a0		; Move address of new area into A0
	moveml	a0@,#0x7fff		; Restore all regs (even A0) except SP
	movl	sp@(8),	a0		; Move address of new area into A0 again
	movl	a0@(60),sp		; Put  SSP into both kernel stack
	movl	sp,	usp		;      and            user  stack
	movw	a0@(66),sr		; Restore SR from new area	
	movl	a0@(68),sp@		; Move PC onto current stack	
	movl	a0@(32),a0		; Restore A0 from new area	
	rts
	.data
