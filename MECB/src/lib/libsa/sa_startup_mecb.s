TUTOR = 228
.globl _main,_console
.data
_console: .long 0	
.text

	movel #0x1000,sp	|use mem from 0x900-0x1000 for stack
	jsr _main		|do main program (can't use argv)
	moveb #TUTOR,d7
	trap #14
