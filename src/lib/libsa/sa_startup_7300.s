.data
_console: .long 0
.text
.globl _main,_console

	movel #0xffffe,sp	|use top of mem for stack
	jsr _main		|do main program (can't use argv)
	trap #1			| breakpoint for Tutor

