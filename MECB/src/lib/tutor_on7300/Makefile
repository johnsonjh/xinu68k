# makefile for building C or assembly language programs for the
# ATT7300 running standalone--
# Format a scratch floppy with fdfmt.sl, mount it on /mnt, and
#     cp test.68 /mnt/unix
#  then reboot the system.

AS = atest

# system directories needed for compilers, libraries, header files--
# Note that the Tutor sources do require ANSI C
GNUBIN = /usr/local/gnu/bin
MLIB = ../libsa
MINC = ../libsa
GNULIB = /usr/local/gnu/lib

# File suffixes:
# .c	C source (often useful both for host and mecb)
# .s 	assembly language source
# .o68 	relocatable machine code, initialized data, etc., 68000-compatible
# .68 	executable image (bits as in memory), 68000/mecb compatible

# Symbol file "syms"--for most recently built executable in directory

test.68: test.o68 remcom.o68 mem.o68 remcom_supp.o68 traps.o68 tutor.o68 fast.o68 slex.o68 brkpoint.o68 $(MLIB)/libsa.a
	ld -N -o test.68 $(MLIB)/sa_startup.o68 test.o68 \
	traps.o68 remcom.o68 remcom_supp.o68 tutor.o68 slex.o68 \
	mem.o68 brkpoint.o68 fast.o68 \
        $(MLIB)/libsa.a -lc $(GNULIB)/gcc-gnulib -lc $(MLIB)/libsa.a

# tell gcc to stay within the MC68000 instruction set, use $(MINC) for headers
remcom.o68: remcom.c
	gcc -m68000 -I$(MINC) -c -Dm68010 -o remcom.o68 remcom.c

remcom_supp.o68: remcom_supp.c
	gcc -m68000 -I$(MINC) -c -Dm68010 -o remcom_supp.o68 remcom_supp.c

fast.o68: fast.s
	/lib/cpp fast.s>temp.s
	as -o fast.o68 temp.s

tutor.o68: tutor.c
	gcc -m68000 -I$(MINC) -c -Dm68010 -o tutor.o68 tutor.c

brkpoint.o68: brkpoint.c
	gcc -I$(MINC) -c -o brkpoint.o68 brkpoint.c

traps.o68:   traps.c
	gcc -I$(MINC) -c -o traps.o68 traps.c

mem.o68:  mem.c
	gcc -I$(MINC) -c -o mem.o68 mem.c

slex.o68: slex.c
	gcc -m68000 -I$(MINC) -c -o slex.o68 slex.c

test.o68: test.c
	gcc -m68000 -I$(MINC) -c -o test.o68 test.c

test1.o68: test1.c
	gcc -m68000 -I$(MINC) -c -o test1.o68 test1.c

clean:
	rm -f *.o *.o68 syms *.pad *.68 core
