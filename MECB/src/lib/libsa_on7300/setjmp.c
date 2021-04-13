/* |setjmp, longjmp
|
|	longjmp(a, v)
|causes a "return(v)" from the
|last call to
|
|	setjmp(v)
|by restoring all the registers and
|adjusting the stack
|
|jmp_buf is set up as:
|
|	_________________
|	|	pc	|
|	-----------------
|	|	d2	|
|	-----------------
|	|	...	|
|	-----------------
|	|	d7	|
|	-----------------
|	|	a2	|
|	-----------------
|	|	...	|
|	-----------------
|	|	a7	|
|	-----------------
*/
#ifdef m68010
asm("
global setjmp, longjmp
text
setjmp:
	mov.l 4(%sp),%a0	#pointer to jmp_buf
	mov.l	(%sp),(%a0)	#pc
	movm.l	&0xFCFC,4(%a0)	#d2-d7, a2-a7
	clr.l	%d0		#return 0
	rts

longjmp:
	mov.l	4(%sp),%a0	#pointer to jmp_buf
	mov.l	8(%sp),%d0	#value returned
	movm.l	4(%a0),&0xFCFC	#restore d2-d7, a2-a7
	mov.l	(%a0),(%sp)	#restore pc of call to setjmp to stack
	rts
");

#else  /* 68000 */
asm("
	.globl _setjmp, _longjmp
	.text

_setjmp:
	movl	sp@(4),a0	|pointer to jmp_buf
	movl	sp@,a0@		|pc
	moveml	#0xFCFC,a0@(4)	|d2-d7, a2-a7
	clrl	d0		|return 0
	rts

_longjmp:
	movl	sp@(4),a0	|pointer to jmp_buf
	movl	sp@(8),d0	|value returned
	moveml	a0@(4),#0xFCFC	|restore d2-d7, a2-a7
	movl	a0@,sp@		|restore pc of call to setjmp to stack
	rts
");
#endif
