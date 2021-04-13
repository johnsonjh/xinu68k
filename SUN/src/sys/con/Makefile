#
# Make the Xinu configuration program
#
all:		config

config:		lex.yy.c y.tab.c
		cc -o config y.tab.c -ll

lex.yy.c:	config.l
		lex config.l

y.tab.c:	config.y
		yacc config.y

install:	config
		cp config ../sys/config

clean:
		rm -f lex.yy.c y.tab.c core
