/*
 *  Clear a block of characters to 0s
 */
bzero(pch, len)
	register char *pch;
	int len;
{
	register int n;

	if ((n = len) <= 0)
		return;
	do
		*pch++ = 0;
	while (--n);
}
