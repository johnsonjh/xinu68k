/* dma.c - various Sun virtual memory utils */

#include <conf.h>
#include <kernel.h>
#include <network.h>

/*
 * getfc3 - Fetch a value from Function Code 3 space
 */
u_long
getfc3(size,addr)
register u_long			size, *addr;
{
	MOVL(#3, d0);
	MOVC(d0, sfc);
	if (size == sizeof(u_char))
		MOVSB(a5@, d7);
	else if (size == sizeof(u_short))
		MOVSW(a5@, d7);
	else if (size == sizeof(u_long))
		MOVSL(a5@, d7);
	return(size);
}

/*
 * setfc3 - write to Function Code 3 space
 */
u_long
setfc3(size,addr,entry)
register u_long			size, *addr,entry;
{
	MOVL(#3, d0);
	MOVC(d0, dfc);
	if (size == sizeof(u_char))
		MOVSB(d6, a5@);
	else if (size == sizeof(u_short))
		MOVSW(d6, a5@);
	else if (size == sizeof(u_long))
		MOVSL(d6, a5@);
}

pg_size
getpgreg(addr)
register u_long			addr;
{
	addr = ((addr & ~PAGEMASK) + PG_OFF) & ADDRMASK;
	return((pg_size) getfc3(sizeof(pg_size), addr));
}

pg_size
setpgreg(addr,entry)
register u_long			addr;
register pg_size		entry;
{
	register pg_size	ret = getpgreg(addr);

	addr = ((addr & ~PAGEMASK) + PG_OFF) & ADDRMASK;
	setfc3(sizeof(pg_size), addr, entry);
	return(ret);
}

/*
 * getsmreg - get the contents of the segment map corresponding
 *            to virtual address "addr"
 */
sm_size
getsmreg(addr)
register u_long			addr;
{
	addr = ((addr & ~SEGMASK) + SM_OFF) & ADDRMASK;
	return((sm_size) getfc3(sizeof(sm_size), addr));
}

/*
 * setsmreg - set the segment map entry for virtual address "addr"
 *            to point to "entry"
 */
sm_size
setsmreg(addr,entry)
register u_long			addr;
register sm_size		entry;
{
	register sm_size	ret = getsmreg(addr);

	addr = ((addr & ~SEGMASK) + SM_OFF) & ADDRMASK;
	setfc3(sizeof(sm_size), addr, entry);
	return(ret);
}

/*
 * map(virt, size, phys, space)
 *    Make redundant virtual mapping for frames.
 *    Used to allow ethernet DMA for V7.
 */
map(virt, size, phys, space)
register u_long				virt, size, phys;
register enum pm_type			space;
{
	pg_t				page;
	register struct pg_field	*pgp = &page.pg_field;
	register			i;

	pgp->pg_valid = 1;
	pgp->pg_permission = PMP_ALL;
	pgp->pg_space = space;

	phys = BTOP(phys);
	size = BTOP(size);

	for (i = 0; i < size; i++){		/* for each page, */
		pgp->pg_pagenum = phys++;
		setpgreg(virt + PTOB(i), page.pg_whole);
	}
}



