#
#  Make All Xinu Manual Pages
#

all:
		@echo making all manual pages in man[01234]
		cd man0;make;cd ..
		cd man1;make;cd ..
		cd man2;make;cd ..
		cd man3;make;cd ..
		cd man4;make;cd ..

install:
		@echo type "\`make print'" to make and print the entire manual

clean:
		rm -rf *.doc core ,*
		cd man0;make clean;cd ..
		cd man1;make clean;cd ..
		cd man2;make clean;cd ..
		cd man3;make clean;cd ..
		cd man4;make clean;cd ..

print:		manual

listings:	manual

manual:		all
		cat man0/*.doc man1/*.doc man2/*.doc man3/*.doc man4/*.doc | \
			sed '1,2d;s/^/	/' | lpr
