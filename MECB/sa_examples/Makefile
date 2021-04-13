# makefile for building C or assembly language programs for the
#  ATT7300 or MECB, by simple method not using PLATFORM preproc. name
#  or transition rules
# Single-source-file makefile--needs editing for multiple-module programs

#  A C source, say myprog_mecb.c, written for the MECB, yields the
#  executable myprog.68 via  "make C=myprog_mecb myprog_mecb.68"
#  Then myprog.68 is ready for downloading via "mtip -f myprog_mecb.68"
#  (followed by ~r and ~d inside the mtip program)

#  A C source, say myprog_7300.c, written for the ATT7300, yields the
#  executable myprog_7300.68a via  "make C=myprog_7300 myprog_7300.68a" or
#  more simply "make C=myprog_7300".
#  Then myprog_7300.68a is ready for downloading via "Mtip -f myprog_7300.68a"
#  (followed by ~r and ~d inside the Mtip program)
C = ctest

# For an assembler source, say myprog_mecb.s, written for the MECB,
#  set AS in the make command line: "make AS=myprog_mecb myprog_mecb.68"
#  Then myprog_mecb.68 is ready for downloading via "mtip -f myprog_mecb.68"
#  Similarly for myprog_7300.68a for the ATT7300

# Creates symbol file "syms" for most recently built executable in directory

AS = atest

# also "make clean" to clean up non-source files in a directory

# system directories needed for compilers, libraries, header files--
XINU = /nfs/src/XINU68
SALIB = $(XINU)/sa
SAINC = $(XINU)/sa
# for m68000-coded C-library code--
LIB = $(XINU)/lib
# File suffixes: Here the file names hold the platform-dependence
# .c	C source (foo_mecb.c or foo_7300.c)
# .s 	assembly language source
# .o68 	object file, 68000-compatible
# .68 	executable file, 68000/mecb compatible
# .68a 	executable file, 68000/ATT7300 compatible

# ATT7300 executable--tell ld to start code at 0x90000, load special startup
# module, then the program, then search the 68000 C library, then
# the SA i/o library, finally 68000-gcc arithmetic support library
# (gnulib plus a few arithmetic routines from /lib/libc.a)
# But since kprintf, in SA lib, calls _doprnt, we need 2nd scan of libxc.a
$(C).68a: $(C).o68
	ld -N -T 90000 -o $(C).68a $(SALIB)/sa_startup_7300.o68 \
		 $(C).o68 $(LIB)/libxc.a $(SALIB)/libsa_7300.a \
		 $(LIB)/libxc.a $(LIB)/libcsupp.a
	nm -n $(C).68a>syms

# MECB executable--tell ld to start code at 0x1000, load special startup
# module, etc.--
$(C).68: $(C).o68
	ld -N -T 1000 -o $(C).68 $(SALIB)/sa_startup_mecb.o68 \
		 $(C).o68 $(LIB)/libxc.a $(SALIB)/libsa_mecb.a \
		 $(LIB)/libxc.a $(LIB)/libcsupp.a
	nm -n $(C).68>syms

# tell gcc to stay within the MC68000 instruction set, use $(SAINC) for headers
$(C).o68: $(C).c
	gcc -m68000 -I$(SAINC) -g -c -o $(C).o68 $(C).c
	
$(AS).68a: $(AS).o68
	ld -N -T 1000 -g -o $(AS).68 $(AS).o68 \
		 $(LIB)/libxc.a $(SALIB)/libsa_7300.a \
		 $(LIB)/libxc.a $(LIB)/libcsupp.a
	nm -n $(AS).68>syms

$(AS).68: $(AS).o68
	ld -N -T 1000 -g -o $(AS).68 $(AS).o68 \
		 $(LIB)/libxc.a $(SALIB)/libsa_mecb.a \
		 $(LIB)/libxc.a $(LIB)/libcsupp.a

	nm -n $(AS).68>syms

$(AS).o68: $(AS).s
	$(GNUBIN)/as -m68000 -o $(AS).o68 $(AS).s

clean:
	rm -f *.o *.o68 syms *.68 *.68a core

