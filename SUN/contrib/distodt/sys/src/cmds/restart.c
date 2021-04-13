
/* 
 * restartmuckel.c - restart the bed server for the back end machine and
 *		     remove bedlog files
 * 
 * Author:	Ken Roderman (stolen by Andy Muckelbauer)
 * 		Dept. of Computer Sciences
 * 		Purdue University
 * Date:	Mon Oct 30 12:59:54 1989
 *
 * Copyright (c) 1989 
 */

#include <stdio.h>
#include <stdio.h>

#include "bed.h"

#define LOGFILE "/u3/503/bedlog"

main()

{
  char buf[5000];
sprintf(buf, "#!/bin/sh\n\
PATH=/bin:/usr/bin:/usr/ucb\n\
umask 002\n\
LIST=\"\"\n\
echo `hostname`:  restarted by $USER at `date` >> %s\n\
for i in `ps ax | grep \"^......?.*/bed\" | sed -e \"s/\\ *\\([0-9]*\\)\\ *\\?.*/\\1/\"`\n\
do\n\
	LIST=\"$LIST $i\"\n\
done\n\
echo killing processes: $LIST\n\
kill -9 $LIST\n\
echo cleaning up %s...\n\
FILE=`grep ttya %s | awk ' BEGIN 	{FC=\" \"} { print $2 }'`\n\
rm -f %s\n\
echo cleaning up %s...\n\
rm -f %s/*\n\
echo firing off a new bed server...%s\n\
/usr/bin/nice --20 %s\n",LOGFILE, BASEDIR, MACHINES_FILE, BASEDIR,LOGGINGDIR,
	LOGGINGDIR, BED_EXEC_PATH, BED_EXEC_PATH);

  system(buf);
}

