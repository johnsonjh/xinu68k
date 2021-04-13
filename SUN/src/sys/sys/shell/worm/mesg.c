#include <conf.h>
#include <kernel.h>
#include <io.h>
#include <tty.h>
#include "game.h"


struct msg {
    struct msg *next;
    int src;
    int nargs;
    int args[1];
};


struct msgheader {
    struct msg *msghead;
    struct msg *msgtail;
    int msem;
} msgtbl[NPROC];


msginit()
{
    int p;

    for (p=0; p< NPROC; ++p) {
	msgtbl[p].msem = SYSERR;
	}
}


initmsg(pid)
     int pid;
{
    STATWORD ps;
    
    disable(ps);
    msgtbl[pid].msem = screate(0);
    msgtbl[pid].msghead = NULL;
    msgtbl[pid].msgtail = NULL;
    restore(ps);
}


newsend(dest, nargs, args)
     int dest;
     int nargs;
     int args;
{
    STATWORD ps;
    
    int *pargs;
    int m;
    struct msg *pmsg;
    
    if (msgtbl[dest].msem == SYSERR)
	initmsg(dest);

    pmsg = (struct msg *) getmem(sizeof(struct msg) + (nargs*sizeof(int)));
    pmsg->next = NULL;
    pmsg->nargs = nargs;
    pmsg->src = getpid();
    pargs = &args;
    for (m=0; m<nargs; ++m) {
	pmsg->args[m] = *pargs++;
	}

    disable(ps);
    if (msgtbl[dest].msghead == NULL) {
	msgtbl[dest].msghead = pmsg;
	msgtbl[dest].msgtail = pmsg;
    }
    else {
	msgtbl[dest].msgtail->next = pmsg;
	msgtbl[dest].msgtail = pmsg;
    }
    restore(ps);

    signal(msgtbl[dest].msem);
}

newrecv(psrc, pnargs, args)
     int *psrc;
     int *pnargs;
     int args;
{
    STATWORD ps;
    struct msg *pmsg;
    int m;
    int mypid;
    int *pargs;
    int **ppargs;

    mypid = getpid();

    if (msgtbl[mypid].msem == SYSERR)
	initmsg(mypid);

    wait(msgtbl[mypid].msem);
    disable(ps);

    pmsg = msgtbl[mypid].msghead;
    msgtbl[mypid].msghead = pmsg->next;
    if (pmsg->next == NULL)
	msgtbl[mypid].msgtail = NULL;
    restore(ps);
    
    if (pmsg->nargs > *pnargs) {
	freemem(pmsg,sizeof(struct msg) + (pmsg->nargs*sizeof(int)));
	return(SYSERR);
    }
    *pnargs = pmsg->nargs;
    *psrc = pmsg->src;
    pargs = &args;
    for (m=0; m < (pmsg->nargs); ++m) {
	*( (int *) *pargs) = pmsg->args[m];
	++pargs;
    }

    freemem(pmsg,sizeof(struct msg) + (pmsg->nargs*sizeof(int)));
    
    return(OK);
}

#ifdef DEBUG
printmesg(pmsg)
    struct msg *pmsg;
{
    STATWORD ps;
    int n;

    disable(ps);
	
    kprintf("msg: src: %d, nargs: %d ",
	    pmsg->src, pmsg->nargs);
    for (n=0; n<pmsg->nargs; ++n) {
	kprintf(", arg[%d]: %d",n,pmsg->args[n]);
    }
    kprintf("\n");
    restore(ps);
	
}
#endif
