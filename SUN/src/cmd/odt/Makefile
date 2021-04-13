CFLAGS = -O -I./h
odt: odt.o
	cc -o odt odt.o libd.a

install: odt
	chmod 755 odt
	cp odt ../../../bin
