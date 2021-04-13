: 'cprint - format and print a bunch of files'
:
: 'cprint [-c coverpagefile] file...
:
PATH=/usr/local/bin:/bin:/usr/bin:/usr/ucb
case $# in
	0) echo "usage: $0 \[-c coverfile] file.c ..."
	   ;;
	1) n=`echo $1 | sed 's/.*\///'`
	   cat -n $1 | pr -h "                         $n" | lpr &
	   ;;
	*) (TMP=/tmp/cpr$$
	   trap "rm -rf $TMP" 0 1 2 3
	   rm -rf $TMP
	   case $1 in
		-c) shift;
		    cat $1 >> $TMP
		    shift
		    ;;
	   esac
	   for i in $*
	   do n=`echo $i | sed 's/.*\///'`
	      cat -n $i | pr -h "                         $n" >> $TMP
	   done
	   lpr $TMP
	   rm -rf $TMP) &
	   ;;
esac
