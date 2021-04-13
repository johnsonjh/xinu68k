/*
	C library - ascii to floating
*/

#include <ctype.h>

double	twoe56	= 72057594037927936.; /*2^56*/
double	exp5[]	= {5.,25.,625.,390625.,152587890625.,23232710361480712890625.};

double
atof(p)
register char *p;
{
	extern double ldexp();
	register c, exp = 0, eexp = 0;
	double fl = 0, flexp = 1.0;
	int bexp, neg = 1, negexp = 1;

	while((c = *p++) == ' ');
	if (c == '-') neg = -1;	else if (c == '+'); else --p;

	while ((c = *p++), isdigit(c))
		if (fl < twoe56) fl = 10*fl + (c-'0'); else exp++;
	if (c == '.')
	while ((c = *p++), isdigit(c))
		if (fl < twoe56)
		{
			fl = 10*fl + (c-'0');
			exp--;
		}
	if ((c == 'E') || (c == 'e'))
	{
		if ((c= *p++) == '+'); else if (c=='-') negexp = -1; else --p;
		while ((c = *p++), isdigit(c)) eexp = 10*eexp + (c-'0');
		if (negexp < 0) eexp = -eexp; exp += eexp;
	}
	bexp = exp;
	if (exp < 0) exp = -exp;

	for (c = 0; c < 6; c++)
	{
		if (exp & 01) flexp *= exp5[c];
		exp >>= 1; if (exp == 0) break;
	}

	if (bexp < 0) fl /= flexp; else fl *= flexp;
	fl = ldexp(fl, bexp);
	if (neg < 0) return(-fl); else return(fl);
}
