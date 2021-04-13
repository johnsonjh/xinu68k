#
#  Make Xinu Manual Pages - only FILES change between man[01234]
#

DIR	=	/usr/xinu7
MACROS	=	$(DIR)/lib/tmac.an

.SUFFIXES:	.ms .doc .1 .2 .3 .4

.ms.doc:
		nroff -ms $< > ,$* ; mv ,$* $*.doc

.1.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

.2.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

.3.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

.4.doc:
		nroff $(MACROS) $< > ,$* ; mv ,$* $*.doc

FILES	=	$(MACROS)	\
		Intro4.doc	\
		ether.doc	\
		internet.doc	\
		namespace.doc	\
		rfilsys.doc	\
		tty.doc

all:		$(FILES)

install:
		@echo no installation possible - use "'make print'"

clean:
		rm -rf *.doc core ,*

Intro1.doc:	Intro1.1
		nroff -man Intro1.1 > ,Intro1 ; mv ,Intro1 Intro1.doc

Intro2.doc:	Intro2.2
		nroff -man Intro2.2 > ,Intro2 ; mv ,Intro2 Intro2.doc

Intro3.doc:	Intro3.3
		nroff -man Intro3.3 > ,Intro3 ; mv ,Intro3 Intro3.doc

Intro4.doc:	Intro4.4
		nroff -man Intro4.4 > ,Intro4 ; mv ,Intro4 Intro4.doc

print:		listings

listings:	$(FILES)
		cat *.doc | sed '1,2d;s/^/	/' | lpr
