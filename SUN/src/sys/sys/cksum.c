/* cksum.c -- cksum */

short	cksum(buf, nwords)
register unsigned short	*buf;
int			nwords;
{
	register unsigned long	sum;
	register int		i;

	sum = 0;
	for (i = nwords; i > 0; --i)
		sum += *buf++;

	/* from /etc/ping */
	/* add in the carry from high 2 bytes */
	sum = (sum >> 16) + (sum & 0xffff);
	/* just in case there was another carry */
	sum += (sum >> 16);
	return(~sum);
}
