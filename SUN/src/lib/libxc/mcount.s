|Count subroutine calls during profiling

|The basic idea here is a call of the form:
|
|	movl	#LABEL,a0
|	jsr	mcount
|	.data
|LABEL:	.long	0
|	.text
|
|which is generated automatically by the C compiler for each subroutine call.
|The first time mcount is called, the contents of LABEL will be 0 so mcount
|will allocate two words from its buffer like:
|
|struct
|{
|	char *fncptr;		/* ptr to subroutine */
|	long fnccnt;		/* number of calls to this function */
|};
|
|and fill int fncptr from the return pc, fill in LABEL with a ptr to
|the newly allocated structure and bump the count.  On subsequent calls,
|the contents of LABEL will be used to just bump the count.

.globl	mcount
.comm	cntbase,2

mcount:	movl	a0@,d0		| fetch contents of LABEL
	jne	1$		| something there
	movl	cntbase,a1	| ptr to next available cnt structure
	addl	#8,cntbase	| bump cntbase to followng structure
	movl	sp@,a1@+	| save ptr to function
	movl	a1,a0@		| save ptr to cnt in structure in LABEL
	jra	2$

1$:	movl	d0,a1		| so we can use it as a ptr
2$:	addql	#1,a1@		| bump the function count
	rts
