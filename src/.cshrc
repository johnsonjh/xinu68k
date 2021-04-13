set PROJECT="/usr/xinu68"
set SRC="$PROJECT/src"
set INSTALL="$PROJECT/install"
set NOTE="$INSTALL/Notes"
set WD="$SRC/cmd/download"
set OWD=`pwd`
# GOTO the working directory
cd $WD
# Keep list of new aliases
set new="reso viso quit note log"
# First three aliases are for maintenance of the aliases themselves
alias	reso	"source $SRC/.cshrc"
alias	viso	"vi $SRC/.cshrc"
alias	quit	"cd $OWD; unset PROJECT SRC INSTALL NOTE WD OWD; unalias $new"
# "Working aliases"
alias	note	'date>>$NOTE;vi +\$ $NOTE'
alias	log	'mo $WD/.logfile'
#
echo New aliases in effect:
foreach i ($new)
    echo -n "$i	"
    alias $i
end
