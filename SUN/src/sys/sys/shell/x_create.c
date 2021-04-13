/* x_create.c - x_creat */

#include <conf.h>
#include <kernel.h>
#include <a.out.h>

LOCAL	char	symfile[] = "pm.out";	/* name of object file to search*/


/* what we expect the a.out header of pm.out to look like */
#define EXPECTED_MACHTYPE 2
#define EXPECTED_MAGIC 263


/*------------------------------------------------------------------------
 *  x_creat  -  (command create) create a process given a starting address
 *------------------------------------------------------------------------
 */
COMMAND	x_creat(stdin, stdout, stderr, nargs, args)
int	stdin, stdout, stderr, nargs;
char	*args[];
{
	int	ssize, prio;		/* arguments passed in		*/
	char	tmp[30];		/* for adding _ to the symbol	*/
	
	struct	exec	*aoutptr;	/* the a.out header		*/
        char    *symb_tab;		/* the symbol table		*/
	char	*str_tab;		/* the string table		*/
	int	nsyms;			/* number of symbols in pm.out	*/
	int	strtabsize;		/* size of string table		*/
	
	int	dev;			/* the pm.out device		*/
	Bool	found;			/* was the symbol found		*/
	int	pid;			/* pid of the new process	*/
	char	*loc;			/* entry point of new process	*/
	
	struct	nlist	*symptr;	/* pointer to the symbols	*/
        int	byteswanted;		/* total bytes to be read	*/
	int	bytesread;		/* number of bytes read		*/
	
	char    *pbuf;
	int	len;
	int	i;

	if (nargs < 4 || nargs > 5) {
	    fprintf(stderr,
		"usage: create procedure stack-size prio [name]\n");
	    return(SYSERR);
	}

	found = FALSE;
	ssize = atoi(args[2]);
	prio  = atoi(args[3]);
	
	if ( (dev=open(NAMESPACE, symfile, "ro")) == SYSERR) {
		fprintf(stderr, "Cannot open %s\n", symfile);
		return(SYSERR);
	}

	pbuf=(char *)getmem(512);
	if ( (int) pbuf == SYSERR) {
		fprintf(stderr, "no memory for exec header\n");
		return(SYSERR);
	}
	
	strcpy(tmp, "_");
	strcat(tmp, args[1]);
	printf("Looking up %s\n", tmp);
    
	len = read(dev, pbuf, sizeof(struct exec));
	aoutptr = (struct exec *)pbuf;
    
#ifdef DEBUG
        fprintf(stderr,"  machtype: %d\n", aoutptr->a_machtype);
        fprintf(stderr,"  magic   : %d\n", aoutptr->a_magic);
        fprintf(stderr,"  text siz: 0x%08lx\n", aoutptr->a_text);
        fprintf(stderr,"  data siz: 0x%08lx\n", aoutptr->a_data);
        fprintf(stderr,"   bss siz: 0x%08lx\n", aoutptr->a_bss);
        fprintf(stderr,"  syms siz: 0x%08lx\n", aoutptr->a_syms);
#endif

	/* see if this is REALLY a file in a.out.h format */
	if ( (aoutptr->a_machtype != EXPECTED_MACHTYPE) ||
	     (aoutptr->a_magic    != EXPECTED_MAGIC)) {
	    fprintf(stderr,"File does not appear to be in a.out format\n");
	    fprintf(stderr,"  machtype: %d (expected %d)\n",
		    aoutptr->a_machtype, EXPECTED_MACHTYPE);
	    fprintf(stderr,"  magic: %d (expected %d)\n",
		    aoutptr->a_magic,EXPECTED_MAGIC);
	    freemem(aoutptr, 512);
	    return(SYSERR);
	}

	/* allocate space for the symbol table */
	symb_tab=(char *)getmem(aoutptr->a_syms);
	if ( (int) symb_tab == SYSERR) {
		fprintf(stderr, "no memory for symbol table\n");
		return(SYSERR);
	}

	/* read the symbol table */
	seek(dev, N_SYMOFF(*aoutptr));
	pbuf = symb_tab;
	byteswanted = aoutptr->a_syms;
	bytesread = 0;
	for (;byteswanted > 0;) {
	    len = read(dev, pbuf, byteswanted);
	    if (len > 0) {
		pbuf += len;
		byteswanted -= len;
		bytesread += len;
	    }
	}
	
	nsyms = (aoutptr->a_syms) / sizeof(struct nlist);
	
#ifdef DEBUG
        fprintf(stderr,"read %d (%x) symbols\n",nsyms,nsyms);
#endif

	strtabsize = filesize(dev) - N_STROFF(*aoutptr);
	
	/* allocate space for the string table */
	str_tab=(char *)getmem(strtabsize);
	if ( (int) str_tab == SYSERR) {
		fprintf(stderr, "no memory for string table\n");
		return(SYSERR);
	}

	/* read the string table */
	seek(dev, N_STROFF(*aoutptr));
	pbuf = str_tab;
	byteswanted = strtabsize;
	bytesread = 0;
	len = 1;
	for (;len > 0;) {
	    len = read(dev, pbuf, byteswanted);
	    if (len > 0) {
		pbuf += len;
		byteswanted -= len;
		bytesread += len;
	    }
	}

	/* search for the desired symbol */
	symptr=(struct nlist *) (symb_tab);
	for (i = 0; i < nsyms; ++i) {
	    if (symptr->n_type == (N_TEXT|N_EXT) &&
		strcmp(&str_tab[symptr->n_un.n_strx],tmp)==0) {
		loc = (char *)symptr->n_value;
		found = TRUE;
		break;
	    }
	    symptr++;
        }
	close(dev);
	freemem(aoutptr, 512);
	freemem(symb_tab, aoutptr->a_syms);
	freemem(str_tab, strtabsize);
	if (found==FALSE) {
	    fprintf(stderr,"symbol %s not found\n",tmp);
	    return(SYSERR);
	}
#ifdef DEBUG
	printf("Calling create(loc: 0x%08lx, ssize: %d, prio: %d, name: %s)\n",
	       loc,ssize,prio,nargs==5?args[4]:tmp);
#endif	
	pid = create(loc, ssize, prio, nargs==5?args[4]:tmp, 0);
	fprintf(stderr, "[%d]\n", pid );
	return(resume(pid));
}



#define INIT_TOO_BIG 100000	/* someplace to start, not really a limit */
#define CLOSE_ENOUGH 100	/* largest allowed over-estimation	  */

/*------------------------------------------------------------------------
 *  FILESIZE -- return the size of the file, overestimating by at most
 *              CLOSE_ENOUGH.  Uses binary search and seeks.
 *------------------------------------------------------------------------
 */
filesize(dev)
     int dev;
{
    int toobig, toosmall, avg;
    int status;
    char buf[2];

    toosmall = 0;
    toobig   = INIT_TOO_BIG;

    /* just in case INIT_TOO_BIG is too small */
    seek(dev,toobig);
    while (read(dev,buf,1) > 0) {
	toosmall = toobig;
	toobig *= 2;
	seek(dev,toobig);
    }

    while ((toobig - toosmall) > CLOSE_ENOUGH) {
	avg = (toosmall + toobig) / 2;
	seek(dev,avg);
	status = read(dev,buf,1);
	if (status < 1) {	    /* avg is too big */
	    toobig = avg;
	} else {        	    /* avg is too small */
	    toosmall = avg;
	}
    }
#ifdef DEBUG
    printf("filesize returns %d\n",toobig);
#endif
    return(toobig);
}
