/*---------------------------------------------------------------------------
 * lock.c
 * 	This file contains routines for locking (really reserving)
 * 	a machine, for finding out who owns the lock, and for releasing
 *	a lock.
 * 	Note: we are using 'cooperative' locking, so that we really
 *  	aren't locking at all; anyone can unlock any backend machine,
 *  	which makes this code trivial; it just deletes the file if it
 *  	exists.
 *---------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <sys/timeb.h>

#include "../h/bed.h"

/*---------------------------------------------------------------------------
 * getlock(user, host, backend)
 * 	char *user, *host, *backend;
 *
 * get the lock on the backend machine for user@host, if possible.
 * if a) no one holds the lock, or
 *    b) user@host holds the lock,
 * SYSOK is returned, else SYSERR (i.e. someone else holds the lock,
 * or the user holds it at a different host).
 *---------------------------------------------------------------------------
 */
int getlock(user, host, backend)
char *user, *host, *backend;
{
	FILE *fs;
	char filename[256];
	struct stat statbuf;
	char buf[256];
	char *atsign;
	int fd;
	int flags;
	
	strcpy(filename, BASEDIR);
	strcat(filename, backend);

	flags = O_CREAT | O_TRUNC | O_WRONLY | O_EXCL;
	if (stat(filename, &statbuf) < 0) {   /* no file, so make it */
		if ((fd = open(filename, flags, 0666)) < 0)
		    return(SYSERR);
		sprintf(buf, "%s@%s", user, host);
		write(fd, buf, strlen(buf));
		close(fd);
		return(SYSOK);
	}

	flags = O_RDONLY;
	fd = open(filename, flags, 0666);
	fs = fdopen(fd, "r");
	flock(fd, LOCK_SH);
	fscanf(fs, "%s", buf);
	close(fd);
	atsign = (char *) strchr(buf, '@');
	
	*atsign = '\0';
	
	if (strcmp(buf, user)) {
		return(SYSERR);    /* someone else holds it */
	}
	
	return(SYSOK);   /* the joker already has it */
	
}


/*---------------------------------------------------------------------------
 * unlock(backend)
 * 	char *backend;
 *
 * release the lock on the backend machine.
 *---------------------------------------------------------------------------
 */
int unlock(user, host, backend)
char *user, *host, *backend;
{
	char filename[256];
	struct stat statbuf;
	int fd;
	FILE *fs;
	char buf[256];
	char *atsign;
	
	strcpy(filename, BASEDIR);
	strcat(filename, backend);
	
	if (stat(filename, &statbuf) < 0) {   /* machine already unlocked */
		return(SYSERR);
	}

	fd = open(filename, O_RDONLY, 0666);
	flock(fd, LOCK_EX);		/* wait for exclusive use	*/
	
	/* small trojan horse */
	/* jng, sdo, raj can release anything and won't allow */
	/* anyone to steal their machines */
	if ((strcmp(user, "jng") != 0) &&
	    (strcmp(user, "sdo") != 0) &&
	    (strcmp(user, "raj") != 0)) { /* anybody but jng,sdo,raj */
		fs = fdopen(fd, "r");
		fscanf(fs, "%s", buf);
		close(fd);
		atsign = (char *) strchr(buf, '@');
		*atsign = '\0';
		if (opt.verbose) {
			printf("%s attempting to release machine ", user);
			printf("belonging to %s\n", buf);
		}
		if (strcmp(buf, "jng") == 0) {
			if (opt.verbose) {
				printf("someone tried to steal ");
				printf("jng's machine - they failed\n");
			}
			return(SYSERR);    /* don't release lock */
		}
		else if (strcmp(buf, "sdo") == 0) {
			if (opt.verbose) {
				printf("someone tried to steal ");
				printf("sdo's machine - they failed\n");
			}
			return(SYSERR);    /* don't release lock */
		}
		else if (strcmp(buf, "raj") == 0) {
			if (opt.verbose) {
				printf("someone tried to steal ");
				printf("raj's machine - they failed\n");
			}
			return(SYSERR);    /* don't release lock */
		}
	}
	/* end trojan horse */

	/* go ahead and remove the lock file */
	unlink(filename);
	
	return(SYSOK);   /* always succeeds */
	
}


/*---------------------------------------------------------------------------
 * checklock(user, host, backend, idletime)
 * 	char *user, *host, *backend, *idletime;
 *
 * check who is using the backend, return user@host, else if noone holds
 * the lock, then return nobody@nowhere
 *---------------------------------------------------------------------------
 */
int checklock(user, host, backend, idletime)
char *user, *host, *backend, *idletime;
{
	FILE *fs;
	int fd;
	char filename[256];
	struct stat statbuf;
	char buf[256];
	char *atsign;
	long curtime;
	long modtime;
	long notusedtime;
	int hours;
	int minutes;
	int seconds;
	

	strcpy(filename, BASEDIR);
	strcat(filename, backend);
	
	if (stat(filename, &statbuf) < 0) {   /* no file, so make it */
		strcpy(user, NOBODY);
		strcpy(host, NOWHERE);
		strcpy(idletime, "00:00:00");
		return(SYSOK);
	}

	modtime = statbuf.st_mtime;
	curtime = time(0);
	notusedtime = curtime - modtime;
	hours = (int) (notusedtime / 3600);
	notusedtime = notusedtime % 3600;
	minutes = (int) (notusedtime / 60);
	seconds = (int) (notusedtime % 60);

	sprintf(idletime, "%02d:%02d:%02d", hours, minutes, seconds);


	if ((fd = open(filename, O_RDONLY, 0666)) < 0)
	    return(SYSERR);
	fs = fdopen(fd, "r");
	flock(fd, LOCK_SH);
	fscanf(fs, "%s", buf);
	fclose(fs);

	atsign = (char *) strchr(buf, '@');
	
	*atsign = '\0';

	strcpy(user, buf);
	strcpy(host, atsign+1);

	return(SYSOK);  
	
}



/*---------------------------------------------------------------------------
 * touchlock(backend)
 * 	char *backend;
 *
 * touch the lock file so the access time is modified
 *---------------------------------------------------------------------------
 */
touchlock(backend)
char *backend;
{
	FILE *fs;
	int fd;
	char filename[256];
	struct stat statbuf;
	char buf[256];

	strcpy(filename, BASEDIR);
	strcat(filename, backend);
	
	if (stat(filename, &statbuf) < 0) {   /* no file to touch */
		return(SYSOK);
	}

	/* open and read/write file - this modifies the modify time */
	fd = open(filename, O_RDONLY, 0666);
	fs = fdopen(fd, "r");
	flock(fd, LOCK_SH);
	fscanf(fs, "%s", buf);
	fclose(fs);

	fd = open(filename, O_CREAT|O_TRUNC|O_WRONLY, 0666);
	fs = fdopen(fd, "w+");
	flock(fd, LOCK_EX);		/* wait for exclusive use	*/
	fprintf(fs, "%s", buf);
	fclose(fs);
	
	return(SYSOK);  
	
}
