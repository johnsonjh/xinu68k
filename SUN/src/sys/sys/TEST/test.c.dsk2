/* test.c - main */

#include <conf.h>
#include <kernel.h>
#include <proc.h>
#include <disk.h>
#include <file.h>
#include <dir.h>

#define	DISKID		33		/* an arbitrary disk id number	*/

struct	dir	*dirptr;		/* pointer to the in-core dir	*/
int	dsize;				/* disk size in blocks		*/
int	niblks;				/* number of i-blocks this disk	*/
char	buf[200];			/* buffer for input line	*/
char	dbuff[DBUFSIZ];			/* one disk buffer		*/
char	msg[] =
"Type C-clear S-size M-makeFS Q-quickM D-dir O-open Z-zap E-exit: ";

/*------------------------------------------------------------------------
 *  main  --  general disk utility and test program.
 *------------------------------------------------------------------------
 */
main()
{
	char	get1ch();

	kprintf("\n\n   ***Disk Utility Program***\n\n");
	dsize = 0;
	dirptr = dsdirec(DISK0);
	while (1) {
		kprintf(msg);
		switch( get1ch() ) {
		case 'c':
		case 'C':	dupclr();
				break;
		case 's':
		case 'S':	dupsize();
				break;
		case 'm':
		case 'M':	dupmkfs();
				break;
		case 'd':
		case 'D':	dupdump();
				break;
		case 'o':
		case 'O':	dupopen();
				break;

		case 'q':
		case 'Q':	dupquick();
				break;

		case 'z':
		case 'Z':	dupzap();
				break;
		case 'e':
		case 'E':	dupexit();

		default:
				kprintf("invalid response...\n");
		}
	read(CONSOLE, buf, 0);
	}
}

/*------------------------------------------------------------------------
 *  dupclr  --  clear the disk completely
 *------------------------------------------------------------------------
 */
dupclr()
{
	DBADDR	i;
	char	*j, *k;

	sizeup();
	kprintf("Clearing disk...\n");
	k = dbuff;
	j = (char *) dirptr;
	for (i=0 ; i<DBUFSIZ ; i++)
		*k++ = *j++ = '\000';
	for (i=0 ; i<dsize ; i++) {
		write(DISK0, dskbcpy(dbuff), i);
		if (i%1000 == 0)
			kprintf("Finished clearing block %u\n", i);
	}
	kprintf("Finished clearing all %u blocks\n", dsize);
}


/*------------------------------------------------------------------------
 *  dupsize  --  size the disk and print out the number of blocks
 *------------------------------------------------------------------------
 */
dupsize()
{
	sizeup();
	kprintf("Disk has %u blocks on it\n", dsize);
}

/*------------------------------------------------------------------------
 *  dupmkfs  -- make a file system from scratch
 *------------------------------------------------------------------------
 */
dupmkfs()
{
	kprintf("Making a file system...\n");
	sizeup();
	dumkfs(DISK0, niblks, dsize, DISKID);
	kprintf("Finished rebuilding...\n");
}

/*------------------------------------------------------------------------
 *  dupdump  --  dump the disk directory
 *------------------------------------------------------------------------
 */
dupdump()
{
	sizeup();
	kprintf("Dumping disk directory...\n");
	dudir(DISK0);
}

/*------------------------------------------------------------------------
 *  dupopen  --  open a file on the disk
 *------------------------------------------------------------------------
 */
dupopen()
{
	int	strt, stp, val;
	char	*p;

	sizeup();
	kprintf("Enter name of file to open: ");
	if ( (buf[0] = getc(CONSOLE) ) == SYSERR ||
	     read(CONSOLE, &buf[1], 0) == SYSERR) {
		kprintf("Invalid length...\n");
		return;
	}
	for (strt=0 ; buf[strt]==' ' ; strt++)
		;
	for (stp=strt ; buf[stp]!='\n' ; stp++)
		;
	buf[stp] = '\000';
	val = open(DISK0, &buf[strt], "rw");
	kprintf("Open returns %d...\n",val);
	if (val > 0) {
		kprintf("Closing file %d...\n", val);
		close(val);
	}
}


/*------------------------------------------------------------------------
 *  dupquick  --  quick rebuild of directory block only
 *------------------------------------------------------------------------
 */
dupquick()
{
	int	i;

	sizeup();
	kprintf("Rebuilding directory block only...\n");
	for (i=0 ; i<DBUFSIZ ; i++)
		dbuff[i] = '\000';
	dirptr->d_iblks = niblks;
	dirptr->d_fblst = (DBADDR)ibtodb(niblks)+1;
	dirptr->d_filst = niblks-1;
	dirptr->d_id = 62; /*random id*/
	dirptr->d_nfiles = 0;
	write(DISK0, dskbcpy(dirptr), DIRBLK);
}


/*------------------------------------------------------------------------
 *  dupzap  --  zap a file from the directory
 *------------------------------------------------------------------------
 */
dupzap()
{
	struct	fdes	*fdptr, *fdptr2;
	int	i, j, strt, stp;

	sizeup();
	kprintf("Enter name of file to remove: ");
	if ( (buf[0] = getc(CONSOLE) ) == SYSERR ||
	     read(CONSOLE, &buf[1], 0) == SYSERR) {
		kprintf("Invalid length...\n");
		return;
	}
	for (strt=0 ; buf[strt]==' ' ; strt++)
		;
	for (stp=strt ; buf[stp]!='\n' ; stp++)
		;
	buf[stp] = '\000';
	for (i=0 ; i<dirptr->d_nfiles ; i++) {
		fdptr = &dirptr->d_files[i];
		if (strcmp(&buf[strt],fdptr->fdname)==0) {
			iblfree(DISK0, fdptr->fdiba);
			dirptr->d_nfiles--;
			fdptr2 = &dirptr->d_files[i+1];
			for (j=i ; j<dirptr->d_nfiles ; j++) {
				fdptr->fdlen = fdptr2->fdlen;
				fdptr->fdiba = fdptr2->fdiba;
				strcpy(fdptr->fdname, fdptr2->fdname);
				fdptr++;
				fdptr2++;
			}
			write(DISK0, dskbcpy(dirptr), DIRBLK);
			return;
		}
	}
	kprintf("Name not found in directory\n");
}


/*------------------------------------------------------------------------
 *  dupexit  --  exit from the program
 *------------------------------------------------------------------------
 */
dupexit()
{
	int	i;

	kprintf("Exiting utility program...\n");
	for (i=1 ; i<NPROC ; i++)
		if (i != currpid)
			kill(i);
	kill(currpid);
}


/*------------------------------------------------------------------------
 *  sizeup  --  size up the disk and place number of blocks in dsize
 *------------------------------------------------------------------------
 */
sizeup()
{
	DBADDR	low, high, mid;

	if (dsize != 0)
		return(dsize);
	low = 0;
	high = 31000;
	while (low < high-1) {
		mid = (DBADDR)(low + high)/(DBADDR)2;
		if (read(DISK0,dbuff,mid) == SYSERR)
			high = mid;
		else
			low = mid;
	}
	dsize = low+1;
	niblks = dsize/20;
}

/*------------------------------------------------------------------------
 *  get1ch  --  get one nonblank character from input and flush line
 *------------------------------------------------------------------------
 */
char	get1ch()
{
	char	ch;

	while ( (ch=getc(CONSOLE)) == ' ')
		;
	read(CONSOLE, buf, 0);
	sleep10(1);
	return(ch);
}
