#define	FLOAT	0
/*
 *	_doprnt: common code for printf, fprintf, sprintf
 *	Floating-point code is included or not, depending
 *	on whether the preprocessor variable FLOAT is 1 or 0.
 */

/* Maximum number of digits in any integer (long) representation */
#define	MAXDIGS	11
#define MAXINT (0x7fffffff)

/* A long with only the high-order bit turned on */
#define	HIBIT	0x80000000L

#define	isdigit(x)	((x) - '0'>=0 && (x) - '0'<=9)
#define	tonumber(x)	((x)-'0')
#define	todigit(x)	((x)+'0')

typedef	char	Bool;			/* Data type for flags		*/
#define	MAXECVT	17			/* Maximum total number of
					   digits in E format		*/
#define	MAXFCVT	60			/* Maximum number of digits
					   after decimal point in F format */
#define	MAXFSIG	17			/* Maximum significant figures
					   in a floating-point number	*/
#define	MAXESIZ	4			/* Maximum number of characters
					   in an exponent		*/
#define	MAXEXP	40			/* Maximum (positive) exponent
					   or greater			*/

#define max(a,b) ((a) > (b)? (a): (b))
#define min(a,b) ((a) < (b)? (a): (b))

#define FZERO 1				/* If this symbol is nonzero,
					   allow '0' as a flag 		*/

#if FLOAT
	/*	System-supplied routines for floating conversion	*/
	char *fcvt();
	char *ecvt();
#endif

static int count;			/* This variable counts output
					   characters. 			*/

int 	_doprnt(format, args, func, farg)
	char	*format;
	char	*args;
	int	(*func)();
	int	farg;

{
	char *cp;			/* Current position in format	*/
	char *bp, *p; 			/* Starting and ending points for
					   value to be printed 		*/
	int width, prec;		/* Field width and precision	*/
	char fcode;			/* Format code			*/

	int lzero;			/* Number of padding zeroes 
					   required on the left		*/
	/* Flags - nonzero if corresponding character appears in format */
	Bool length;		/* l */
	Bool fplus;		/* + */
	Bool fminus;		/* - */
	Bool fblank;		/* blank */
	Bool fsharp;		/* # */

#if FZERO
	Bool fzero;		/* 0 */
#endif

	char *prefix;			/* Pointer to sign, "0x", "0X",
					   or empty			*/
#if FLOAT
	char *suffix;			/* Exponent or empty		*/
	char expbuf[MAXESIZ + 1];	/* Buffer to create exponent	*/
	int rzero;			/* Number of padding zeroes 
					   required on the right 	*/
	double dval;			/* The value being converted, if real */
	int decpt, sign;		/* Output values from fcvt and ecvt */
	int k;				/* Scratch */

	/* Values are developed in this buffer */
	char buf[max (MAXDIGS, max (MAXFCVT + MAXEXP, MAXECVT) + 1)];
#else
	char buf[MAXDIGS];
#endif
	long val;			/* The value being converted,
					   if integer			*/
	char *tab;			/* Set to point to a translate
					   table for digits of whatever radix */
	int n, hradix, lowbit;		/* Work variables		*/

	cp = format;
	count = 0;

	/*
	 *	The main loop -- this loop goes through one iteration
	 *	for each ordinary character or format specification.
	 */
	while (*cp)
		if (*cp != '%') {
			/* Ordinary (non-%) character */
			(*func)(farg, *cp++);
			++count;
		} else {
			/*
			 *	% has been found.
			 *	First, parse the format specification.
			 */

			/* Scan the <flags> */
			fplus = fminus = fblank = fsharp = 0;
#if FZERO
			fzero = 0;
#endif
		scan:	switch (*++cp) {
			case '+':
				fplus = 1;
				goto scan;
			case '-':
				fminus = 1;
				goto scan;
			case ' ':
				fblank = 1;
				goto scan;
			case '#':
				fsharp = 1;
				goto scan;
#if FZERO
			case '0':
				fzero = 1;
				goto scan;
#endif
			}

			/* Scan the field width */
			if (*cp == '*') {
				width = *(int *)args; args += sizeof(int);
				if (width < 0) {
					width = -width;
					fminus = 1;
				}
				cp++;
			} else {
				width = 0;
				while (isdigit (*cp)) {
					n = tonumber (*cp++);
					width = width * 10 + n;
				}
			}

			/* Scan the precision */
			if (*cp == '.') {

				/* '*' instead of digits? */
				if (*++cp == '*') {
					prec = *(int *)args; args += sizeof(int);
					cp++;
				} else {
					prec = 0;
					while (isdigit (*cp)) {
						n = tonumber (*cp++);
						prec = prec * 10 + n;
					}
				}
			} else
				prec = -1;

			/* Scan the length modifier */
			length = 0;
			switch (*cp) {
			case 'l':
				length = 1;
				/* No break */
			case 'h':
				cp++;
			}

			/*
			 *	The character addressed by cp must be the
			 *	format letter -- there is nothing left for
			 *	it to be.
			 *
			 *	The status of the +, -, #, blank, and 0
			 *	flags are reflected in the variables
			 *	"fplus", "fminus", "fsharp", "fblank",
			 *	and "fzero", respectively.
			 *	"width" and "prec" contain numbers
			 *	corresponding to the digit strings
			 *	before and after the decimal point,
			 *	respectively. If there was no decimal
			 *	point, "prec" is -1.
			 *
			 *	The following switch sets things up
			 *	for printing.  What ultimately gets
			 *	printed will be padding blanks, a prefix,
			 *	left padding zeroes, a value, right padding
			 *	zeroes, a suffix, and more padding
			 *	blanks.  Padding blanks will not appear
			 *	simultaneously on both the left and the
			 *	right.  Each case in this switch will
			 *	compute the value, and leave in several
			 *	variables the information necessary to
			 *	construct what is to be printed.
			 *
			 *	The prefix is a sign, a blank, "0x", "0X",
			 *	or null, and is addressed by "prefix".
			 *
			 *	The suffix is either null or an exponent,
			 *	and is addressed by "suffix".
			 *
			 *	The value to be printed starts at "bp"
			 *	and continues up to and not including "p".
			 *
			 *	"lzero" and "rzero" will contain the number
			 *	of padding zeroes required on the left
			 *	and right, respectively.  If either of
			 *	these variables is negative, it will be
			 *	treated as if it were zero.
			 *
			 *	The number of padding blanks, and whether
			 *	they go on the left or the right, will be
			 *	computed on exit from the switch.
			 */
			
			lzero = 0;
			prefix = "";
#if FLOAT
			rzero = lzero;
			suffix = prefix;
#endif
			switch (fcode = *cp++) {

			/*
			 *	fixed point representations
			 *
			 *	"hradix" is half the radix for the conversion.
			 *	Conversion is unsigned unless fcode is 'd'.
			 *	HIBIT is 1000...000 binary, and is equal to
			 *		the maximum negative number.
			 *	We assume a 2's complement machine
			 */

			case 'd':
			case 'u':
				hradix = 5;
				goto fixed;
			case 'D':
				fcode = 'd';
				length = 1;
				hradix = 5;
				goto fixed;

			case 'o':
				hradix = 4;
				goto fixed;

			case 'X':
			case 'x':
				hradix = 8;

			fixed:
				/* Establish default precision */
				if (prec < 0)
					prec = 1;

				/* Fetch the argument to be printed */
				if (length)
					{val = *(long *)args; args += sizeof(long);}
				else if (fcode == 'd')
					{val = *(int *)args; args += sizeof(int);}
				else
					{val = *(unsigned *)args; args += sizeof(unsigned);}

				/* If signed conversion, establish sign */
				if (fcode == 'd') {
					if (val < 0) {
						prefix = "-";
						/*
						 *	Negate, checking in
						 *	advance for possible
						 *	overflow.
						 */
						if (val != HIBIT)
							val = -val;
					} else if (fplus)
						prefix = "+";
					else if (fblank)
						prefix = " ";
				}
#if FZERO
				if (fzero) {
					int n = width - strlen (prefix);
					if (n > prec)
						prec = n;
				}
#endif
				/* Set translate table for digits */
				if (fcode == 'X')
					tab = "0123456789ABCDEF";
				else
					tab = "0123456789abcdef";

				/* Develop the digits of the value */
				p = bp = buf + MAXDIGS;
				while (val) {
					lowbit = val & 1;
					val = (val >> 1) & ~HIBIT;
					*--bp = tab[val % hradix * 2 + lowbit];
					val = val / hradix;
				}

				/* Calculate padding zero requirement */
				lzero = bp - p + prec;

				/* Handle the # flag */
				if (fsharp && bp != p)
					switch (fcode) {
					case 'o':
						if (lzero < 1)
							lzero = 1;
						break;
					case 'x':
						prefix = "0x";
						break;
					case 'X':
						prefix = "0X";
						break;
					}

				break;
#if FLOAT
			case 'E':
			case 'e':
				/*
				 *	E-format.  The general strategy
				 *	here is fairly easy: we take
				 *	what ecvt gives us and re-format it.
				 */

				/* Establish default precision */
				if (prec < 0)
					prec = 6;

				/* Fetch the value */
				dval = *(double *)args; args += sizeof(double);

				/* Develop the mantissa */
				bp = ecvt (dval,
					min (prec + 1, MAXECVT),
					&decpt,
					&sign);

				/* Determine the prefix */
			e_merge:
				if (sign)
					prefix = "-";
				else if (fplus)
					prefix = "+";
				else if (fblank)
					prefix = " ";

				/* Place the first digit in the buffer */
				p = &buf[0];
				*p++ = *bp != '\0'? *bp++: '0';

				/* Put in a decimal point if needed */
				if (prec != 0 || fsharp)
					*p++ = '.';

				/* Create the rest of the mantissa */
				rzero = prec;
				while (rzero > 0 && *bp!= '\0') {
					--rzero;
					*p++ = *bp++;
				}

				bp = &buf[0];

				/* Create the exponent */
				suffix = &expbuf[MAXESIZ];
				*suffix = '\0';
				if (dval != 0) {
					n = decpt - 1;
					if (n < 0)
						n = -n;
					while (n != 0) {
						*--suffix = todigit (n % 10);
						n /= 10;
					}
				}

				/* Prepend leading zeroes to the exponent */
				while (suffix > &expbuf[MAXESIZ - 2])
					*--suffix = '0';

				/* Put in the exponent sign */
				*--suffix = (decpt > 0 || dval == 0)? '+': '-';

				/* Put in the e */
				*--suffix = isupper(fcode)? 'E' : 'e';

				break;

			case 'f':
				/*
				 *	F-format floating point.  This is
				 *	a good deal less simple than E-format.
				 *	The overall strategy will be to call
				 *	fcvt, reformat its result into buf,
				 *	and calculate how many trailing
				 *	zeroes will be required.  There will
				 *	never be any leading zeroes needed.
				 */

				/* Establish default precision */
				if (prec < 0)
					prec = 6;

				/* Fetch the value */
				dval = *(double *)args; args += sizeof(double);

				/* Do the conversion */
				bp = fcvt (dval,
					min (prec, MAXFCVT),
					&decpt,
					&sign);

				/* Determine the prefix */
			f_merge:
				if (sign && decpt > -prec &&
				    *bp != '\0' && *bp != '0')
					prefix = "-";
				else if (fplus)
					prefix = "+";
				else if (fblank)
					prefix = " ";

				/* Initialize buffer pointer */
				p = &buf[0];

				/* Emit the digits before the decimal point */
				n = decpt;
				k = 0;
				if (n <= 0)
					*p++ = '0';
				else
					do	if (*bp == '\0' || k >= MAXFSIG)
							*p++ = '0';
						else {
							*p++ = *bp++;
							++k;
						}
					while (--n != 0);

				/* Decide whether we need a decimal point */
				if (fsharp || prec > 0)
					*p++ = '.';

				/* Digits (if any) after the decimal point */
				n = min (prec, MAXFCVT);
				rzero = prec - n;
				while (--n >= 0)
					if (++decpt <= 0
					    || *bp == '\0'
					    || k >= MAXFSIG)
						*p++ = '0';
					else {
						*p++ = *bp++;
						++k;
					}

				bp = &buf[0];

				break;

			case 'G':
			case 'g':
				/*
				 *	g-format.  We play around a bit
				 *	and then jump into e or f, as needed.
				 */
			
				/* Establish default precision */
				if (prec < 0)
					prec = 6;

				/* Fetch the value */
				dval = *(double *)args; args += sizeof(double);

				/* Do the conversion */
				bp = ecvt (dval,
					min (prec, MAXECVT),
					&decpt,
					&sign);
				if (dval == 0)
					decpt = 1;

				k = prec;
				if (!fsharp) {
					n = strlen (bp);
					if (n < k)
						k = n;
					while (k >= 1 && bp[k-1] == '0')
						--k;
				}
					
				if (decpt < -3 || decpt > prec) {
					prec = k - 1;
					goto e_merge;
				} else {
					prec = k - decpt;
					goto f_merge;
				}

#endif
			case 'c':
				buf[0] = *(int *)args; args += sizeof(int);
				bp = &buf[0];
				p = bp + 1;
				break;

			case 's':
				bp = *(char **)args; args += sizeof(char **);
				if (prec < 0)
					prec = MAXINT;
				for (n=0; *bp++ != '\0' && n < prec; n++);
				p = --bp;
				bp -= n;
				break;

			case '\0':
				cp--;
				break;

		/*	case '%':	*/
			default:
				p = bp = &fcode;
				p++;
				break;

			}
			if (fcode != '\0') {
				/* Calculate number of padding blanks */
				int nblank;
				nblank = width
#if FLOAT
					- (rzero < 0? 0: rzero)
					- strlen (suffix)
#endif
					- (p - bp)
					- (lzero < 0? 0: lzero)
					- strlen (prefix);

				/* Blanks on left if required */
				if (!fminus)
					while (--nblank >= 0)
						(*func) (farg,' ');

				/* Prefix, if any */
				while (*prefix != '\0')
					(*func) (farg, *prefix++);

				/* Zeroes on the left */
				while (--lzero >= 0)
					(*func) (farg, '0');
				
				/* The value itself */
				while (bp < p)
					(*func) (farg, *bp++);
#if FLOAT
				/* Zeroes on the right */
				while (--rzero >= 0)
					(*func) (farg, '0');

				/* The suffix */
				while (*suffix != '\0')
					(*func) (farg, *suffix++);
#endif
				/* Blanks on the right if required */
				if (fminus)
					while (--nblank >= 0)
						(*func) (farg, ' ');
			}
		}

	return ;
}



