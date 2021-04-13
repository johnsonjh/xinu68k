DESTDIR	=
DOT	= src
SUBDIRS	= lib/libxc lib/libsa lib/gnulib sys/sys

all install clean:
	for i in ${SUBDIRS}; do\
	    (echo "Making $(DOT)/$$i:"; cd $$i;\
             make ${MFLAGS} DESTDIR=${DESTDIR} $@);done

