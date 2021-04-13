global main
start: mov.l &0x80000,%sp # top of user space
	jsr c_startup
loop:  jmp loop

